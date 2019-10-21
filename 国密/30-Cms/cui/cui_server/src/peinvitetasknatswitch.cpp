/*****************************************************************************
模块名      : cui_server 
文件名      : peinvitetasknatswitch.h
相关文件    : 
文件实现功能: peinvitetasknatswitch.h 定义所有CU常规请求事务
作者        : sunqiang
版本        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2015/09/17   1.0         sunqiang        创建
******************************************************************************/
#include "peinvitetasknatswitch.h"
#include "normalcutask.h"
#include "cuiapp.h"
#include "redisdb.h"
#include "cms/utility/networkhelper.h"
#include "StreamDecentrationAdapter.h"

extern CRedisDBInterface g_cRedisDb;

CPeInviteTaskNatSwitch::CPeInviteTaskNatSwitch(CInstExt *pcInst, const string& strCuSession, CSipTask *pcInviteInterActTask, const string& strPeUri) : CCallBackTask(pcInst, strCuSession, strPeUri), m_cStrmNumRestorer(pcInst)
{
    CMS_ASSERT(!strCuSession.empty());
    CMS_ASSERT(!strPeUri.empty());
    CMS_ASSERT(NULL != pcInviteInterActTask);

    m_pcInviteInterActTask = pcInviteInterActTask;
    m_tInviteID = INVALID_DIALOG_ID;
    m_bByeWhileWaitInviteRsp = false;

    m_cStrmNumRestorer.SetUserLoginSession( strCuSession );
    m_dwErrorCode = CMS_SUCCESS;
}

CPeInviteTaskNatSwitch::~CPeInviteTaskNatSwitch()
{
    TerminateTask();
}

void CPeInviteTaskNatSwitch::TerminateTask()
{
	// 拆掉交换
	RemoveCuiSwitch();

    Iterator pPos;
    int nSeqNo = 0;
    int nTaskNo = INVALID_TASKNO;
    while(!pPos.End())
    {
        if (m_cChildTaskList.Iterate(pPos, nSeqNo, nTaskNo) )
        {
            CTask* pcTask = NULL;
            pcTask = GetInstance()->FindTask(nTaskNo);
            if(pcTask != NULL)
            {
                delete pcTask;
                pcTask = NULL;
            }        
        }
    }
    if ( m_tInviteID != INVALID_DIALOG_ID )
    { //释放会话
		PostInDlgReq(KDSIP_EVENT_INVITE_BYE_REQ, CByeReq(), m_tInviteID);
        //OspSipReleaseDlg(m_tInviteID);
        m_tInviteID = INVALID_DIALOG_ID;
    }

    //注销CuAgent中的浏览事务
    if ( INVALID_PLAY_ID != m_tReq.GetDstChn().GetChnNO() )
    {
        //m_pcCuAgent->m_cInviteTaskSet.Erase(m_tCuChn.GetChnNO());
        CCuAgent *pcCuAgent = (CCuAgent *)g_cCuiDataMgr.GetTaskPtrBySession(m_strCuSession, GetInstance());
        //当调用CuAgent析构后，Invite等待bye响应时，CuAgent已经被干掉了，所以去掉CMS_ASSERT
        //CMS_ASSERT(pcCuAgent != NULL);
        if(NULL != pcCuAgent)
        {
			pcCuAgent->m_cPeInviteTaskSet.Erase(m_tReq.GetDstChn().GetChnNO());
			PopMediaTask(pcCuAgent->m_strSession,m_tReq.GetDstChn().GetChnNO());
        }

        CPeAgent *pcPeAgent = (CPeAgent *)g_cCuiDataMgr.GetPeAgentPtrByPeName(m_tPeUri.GetURIString(), GetInstance());
        if(NULL != pcPeAgent)
        {
            pcPeAgent->m_cInviteTaskSet.Erase(m_tReq.GetDstChn().GetChnNO());
        }

		m_tReq.GetDstChn().SetChnNO(INVALID_PLAY_ID);
    }

    m_pcInviteInterActTask = NULL;
}


bool CPeInviteTaskNatSwitch::IsNotNeedPollState()
{
    if (GetState() == INVITE_HOLD_STATE)
    {
        return true;
    }

    return false;
}

void CPeInviteTaskNatSwitch::InitStateMachine()
{
    CStateProc cNotReqState;
    cNotReqState.ProcMsg = (CSipTask::PFProcMsg)&CPeInviteTaskNatSwitch::NotReqProcMsg;
    cNotReqState.TimerPoll = (CSipTask::PFTimerPoll)&CPeInviteTaskNatSwitch::WaitSetupReqTimerPoll;
    cNotReqState.ProcErrMsg = (CSipTask::PFProcMsg)&CPeInviteTaskNatSwitch::NotReqProcMsg;
    AddRuleProc(NOT_REQUEST_STATE, cNotReqState);

    CStateProc cWaitSetupRspState;
    cWaitSetupRspState.ProcMsg = (CSipTask::PFProcMsg)&CPeInviteTaskNatSwitch::WaitSetupRspProcMsg;
    cWaitSetupRspState.TimerPoll = (CSipTask::PFTimerPoll)&CPeInviteTaskNatSwitch::WaitSetupRspTimerPoll;
    cWaitSetupRspState.ProcErrMsg = (CSipTask::PFProcMsg)&CPeInviteTaskNatSwitch::WaitSetupRspProcMsg;
    AddRuleProc(WAIT_SETUP_RSP_STATE, cWaitSetupRspState);

    CStateProc cWaitSetupAckState;
    cWaitSetupAckState.ProcMsg = (CSipTask::PFProcMsg)&CPeInviteTaskNatSwitch::WaitSetupAckProcMsg;
    cWaitSetupAckState.TimerPoll = (CSipTask::PFTimerPoll)&CPeInviteTaskNatSwitch::WaitAckTimerPoll;
    cWaitSetupAckState.ProcErrMsg = (CSipTask::PFProcMsg)&CPeInviteTaskNatSwitch::WaitSetupAckProcMsg;
    AddRuleProc(WAIT_SETUP_ACK_STATE, cWaitSetupAckState);

    CStateProc cInviteHoldState;
    cInviteHoldState.ProcMsg = (CSipTask::PFProcMsg)&CPeInviteTaskNatSwitch::InviteHoldProcMsg;
    cInviteHoldState.TimerPoll = &CSipTask::TimerPollSkip;
    cInviteHoldState.ProcErrMsg = (CSipTask::PFProcMsg)&CPeInviteTaskNatSwitch::InviteHoldProcMsg;
    AddRuleProc(INVITE_HOLD_STATE, cInviteHoldState);

    CStateProc cWaitByeRspState;
    cWaitByeRspState.ProcMsg = (CSipTask::PFProcMsg)&CPeInviteTaskNatSwitch::WaitByeRspProcMsg;
    cWaitByeRspState.TimerPoll = (CSipTask::PFTimerPoll)&CPeInviteTaskNatSwitch::WaitByeRspTimerPoll;
    cWaitByeRspState.ProcErrMsg = (CSipTask::PFProcMsg)&CPeInviteTaskNatSwitch::WaitByeRspProcMsg;
    AddRuleProc(WAIT_BYE_RSP_STATE, cWaitByeRspState);

    NextState(NOT_REQUEST_STATE);
}

u32 CPeInviteTaskNatSwitch::NotReqProcMsg( CMessage *const pcMsg )
{
    TASKLOG(NORMAL_TASK, CRITICAL_LEV, "在[CInviteTask::NotReqProcMsg]收到未知消息[%s--%hu]，不处理\n",
        OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
    CMS_ASSERT(0&&"CInviteTask::NotReqProcMsg收到未知消息");
    return PROCMSG_OK;
}

u32 CPeInviteTaskNatSwitch::StartSendInviteReq(CInviteReq &cReq)
{
    CMS_ASSERT( NOT_REQUEST_STATE == GetState() );
	m_tReq = cReq;//2018/8/22 zsy
    if( !m_cStrmNumRestorer.Start() )
    {
        TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户会话[%s] 已达到最大并发数，本次请求被拒绝\n", m_strCuSession.c_str() );

        m_dwErrorCode = ERR_CUI_USER_EXCEEDED_MAX_STREAM;
        return PROCMSG_FAIL;
    }

    TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户会话[%s]发送Invite请求，建立源通道[%s--%d]到目的通道[%s--%d]的交换\n",
        m_strCuSession.c_str(),
        cReq.GetSrcChn().GetDevUri().c_str(), cReq.GetSrcChn().GetChnNO(),
        cReq.GetDstChn().GetDevUri().c_str(), cReq.GetDstChn().GetChnNO());
	
    u32 dwRet = PROCMSG_OK;
    CCuiInst *pcInst = (CCuiInst *)GetInstance();
    m_strPeSession = pcInst->IsProvideServer(m_tPeUri, GetTaskNO());

	//m_tReq = cReq;//2018/8/22 zsy 需要提前到第一个可能的return 之前，否则前面return了，后面销毁本对象的时候就无法释放invite了。
	m_cuChn = cReq.GetDstChn();

	// 网段选择,无punch
	m_tReq.GetSdp().SetIsNeedPunch(false);
	m_tReq.GetSdp().SetIsNatPacketMode(false);

	//cu在NAT內，更好碼流地址為其NAT外地址
	SwitchToNatAddressByNatPacket(m_tReq.GetSdp());
	
    if(!m_strPeSession.empty())
    {
        CPeAgentBase *pcPeBaseAgent = (CPeAgentBase *)g_cCuiDataMgr.GetPeAgentPtrByPeName(m_tPeUri.GetURIString(), GetInstance());
        CMS_ASSERT(NULL != pcPeBaseAgent);
        if(pcPeBaseAgent != NULL)
        {
            pcPeBaseAgent->m_cInviteTaskSet.Insert(m_tReq.GetDstChn().GetChnNO(), GetTaskNO());

			CInviteReq cuiReq = m_tReq;

			TNetAddr tCuiNetAddr;
			this->GetCuiNatAddrForPlat(tCuiNetAddr);

			// 发送时，将源地址改成CUI交换地址
			SwitchToCuiNatSwitchAddress(cuiReq.GetSdp(), tCuiNetAddr);
			

            cReq.SetSession(m_strPeSession);
            dwRet = PostReq(KDSIP_EVENT_INVITE_REQ, cuiReq, m_tPeUri);
            if(dwRet != PROCMSG_OK)
            {
				TASKLOG(NORMAL_TASK, ERROR_LEV, "PostReq failed, Ret:%u\n", dwRet);
                return dwRet;
            }
            NextState(WAIT_SETUP_RSP_STATE);
        }
		else
		{
			TASKLOG(NORMAL_TASK, ERROR_LEV, "pcPeBaseAgent == NULL\n");
			dwRet = PROCMSG_FAIL; 
		}        
    }

    return dwRet;
}

u32 CPeInviteTaskNatSwitch::StartSendInviteAck()
{ 
    CMS_ASSERT(WAIT_SETUP_ACK_STATE == GetState());
    CMS_ASSERT(INVALID_DIALOG_ID != m_tInviteID);
    TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户会话[%s]通过TaskNO[%u]发送InviteACK请求, Sip DialogID为 %u\n",
        m_strCuSession.c_str(), GetTaskNO(), m_tInviteID);

    CInviteACK cAck;
    cAck.SetSession(m_strPeSession);

#ifdef _UNIT_TEST_
    g_cTaskNOQue.push(GetTaskNO());
    cout << "Post InviteAck To " << m_pcCuAgent->m_tCmuURI.GetURIString().c_str() << endl;
    cout << cAck.ToXml().c_str() << endl;
#else
    u32 dwRet = PROCMSG_OK;
    dwRet = PostInDlgReq<CInviteACK>(KDSIP_EVENT_INVITE_ACK, cAck, m_tInviteID);
#endif

    NextState(INVITE_HOLD_STATE);
    return dwRet;
}

u32 CPeInviteTaskNatSwitch::StartSendInviteBye()
{
    u32 dwRet = PROCMSG_OK;
    if ( m_tInviteID != INVALID_DIALOG_ID )
    {
        TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户会话[%s]通过TaskNO[%u]发送InviteBye请求, Sip DialogID为 %u\n",
            m_strCuSession.c_str(), GetTaskNO(), m_tInviteID);
        string strNULL;
        dwRet = PostInDlgReq(KDSIP_EVENT_INVITE_BYE_REQ, strNULL, m_tInviteID);
    }

    NextState(WAIT_BYE_RSP_STATE);
    //BYE Rsp由对端SIP协议栈直接回复，等待回复后delete task
    return dwRet;
}


bool CPeInviteTaskNatSwitch::TryBye( CSipTask *pcActTask )
{
    if (GetState() != WAIT_SETUP_RSP_STATE)
	{
		// 先拆交换
		this->RemoveCuiSwitch();

        m_pcInviteInterActTask = pcActTask;
        StartSendInviteBye();
        return true;
    }
    else
    {
        m_bByeWhileWaitInviteRsp = true;
        return false;
    }
}


s32 CPeInviteTaskNatSwitch::StartVcrCtrl(const CCuVcrCtrlReq& cCuReq)
{
    if( m_tInviteID != INVALID_DIALOG_ID)
    {
        TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户会话[%s]通过TaskNO[%u]发送VcrCtrl请求, Sip DialogID为 %u\n",
            m_strCuSession.c_str(), GetTaskNO(), m_tInviteID);

        CVcrCtrlReq cReq;
        cReq.SetSeqNum(cCuReq.GetSeqNum());
        cReq.SetSession(m_strPeSession);
        cReq.SetPlaybackSessUri(m_tRsp.GetSdp().GetPlaybackSessUri());
        cReq.SetPlayRate(cCuReq.GetPlayRate());
        cReq.SetSeekTime(cCuReq.GetSeekTime());
        cReq.SetVcrCmd(cCuReq.GetVcrCmd());
        PostInDlgReq(KDSIP_EVENT_INFO_REQ, cReq, m_tInviteID);
    }

    return PROCMSG_OK;
}
u32 CPeInviteTaskNatSwitch::StartTask()
{
    u32 dwRet = PROCMSG_FAIL;
    if( m_tInviteID != INVALID_DIALOG_ID)
    {
        TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户会话[%s]通过TaskNO[%u]发送StartTask请求, Sip DialogID为 %u\n",
            m_strCuSession.c_str(), GetTaskNO(), m_tInviteID);
        CRcsTaskStartReq cReq;
        cReq.SetSession(m_strPeSession);
        
        dwRet = PostInDlgReq(KDSIP_EVENT_INFO_REQ, cReq, m_tInviteID);
        NextState(INVITE_HOLD_STATE);
    }
    return dwRet;
}
CPlaybackProgressNtfReq &CPeInviteTaskNatSwitch::GetPlayBackProgressNtf()
{
    return m_tNtfReq;
}
const CPlaybackProgressNtfReq &CPeInviteTaskNatSwitch::GetPlayBackProgressNtf() const
{
    return m_tNtfReq;
}

bool CPeInviteTaskNatSwitch::GetCuiNatAddrForPlat(TNetAddr &tCuiNatAddr)
{
	CCuAgent *pcCuAgent = (CCuAgent *)g_cCuiDataMgr.GetTaskPtrBySession(m_strCuSession, GetInstance());
	if ( pcCuAgent == NULL )
	{
		return false;
	}

	tCuiNatAddr.Clear();

	string strPlatCuiIp = pcCuAgent->m_tNatInfo.GetIpAddr();
	if ( strPlatCuiIp.empty() )
	{
		TASKLOG(NORMAL_TASK, ERROR_LEV, "m_tNatInfo.GetIpAddr() is empty!\n");
		return false;
	}

	int nPlatCuiPort = g_cCuiConfig.GetNatPort();
	if ( nPlatCuiPort <= 0 )
	{
		TASKLOG(NORMAL_TASK, ERROR_LEV, "g_cCuiConfig.GetNatPort() is 0!\n");
		return false;
	}

	tCuiNatAddr.SetNetIp(strPlatCuiIp);
	tCuiNatAddr.SetNetPort(nPlatCuiPort);

	TASKLOG(NORMAL_TASK, EVENT_LEV, "CuiNatAddrForPlat[%s:%d]!\n", strPlatCuiIp.c_str(), nPlatCuiPort);
	return true;
}

bool CPeInviteTaskNatSwitch::GetCuiNatAddrForCu(TNetAddr &tCuiNatAddr)
{
	CCuAgent *pcCuAgent = (CCuAgent *)g_cCuiDataMgr.GetTaskPtrBySession(m_strCuSession, GetInstance());
	if ( pcCuAgent == NULL )
	{
		return false;
	}

	tCuiNatAddr.Clear();

	string strPlatCuiIp;
	int nPlatCuiPort = 0;

	if ( NATModeInner == pcCuAgent->m_ePlatNat && !g_cCuiConfig.GetMapedNatIP().empty())
	{
		//cCuPunchAddr.SetNetIp(g_cCuiConfig.GetMapedNatIP());
		//当含有二层NAT时，平台不能同时把一个端口（8001）映射到第一层和第二层
		//这里直接使用cu的登陆ip，即为平台在的最外层映射IP
		strPlatCuiIp = pcCuAgent->m_strHostIp;
		if (pcCuAgent->m_tNatInfo.GetIpAddr() != pcCuAgent->m_strHostIp)
		{
			nPlatCuiPort = g_cCuiConfig.GetMapedNatPort();//需要判断hostip是否就是本地ip，若是本地ip，则不能使用natport。
		}
		else
		{
			nPlatCuiPort = g_cCuiConfig.GetNatPort();
		}
	}
	else
	{
		strPlatCuiIp = pcCuAgent->m_tNatInfo.GetIpAddr();
		nPlatCuiPort = g_cCuiConfig.GetNatPort();
	}

	if ( strPlatCuiIp.empty() )
	{
		TASKLOG(NORMAL_TASK, ERROR_LEV, "m_tNatInfo.GetIpAddr() is empty!\n");
		return false;
	}

	if ( nPlatCuiPort <= 0 )
	{
		TASKLOG(NORMAL_TASK, ERROR_LEV, "g_cCuiConfig.GetNatPort() is 0!\n");
		return false;
	}

	tCuiNatAddr.SetNetIp(strPlatCuiIp);
	tCuiNatAddr.SetNetPort(nPlatCuiPort);

	TASKLOG(NORMAL_TASK, EVENT_LEV, "CuiNatAddrForCu[%s:%d]!\n", IpToStr(tCuiNatAddr.GetNetIpv4()).c_str(), tCuiNatAddr.GetNetPort());
	return true;
}

void CPeInviteTaskNatSwitch::SwitchToNatAddressByNatPacket(TSdp &tSdp)
{
	TMediaDescList& tMediaDescList = tSdp.GetMediaDescList();

	CCuAgent *pcCuAgent = (CCuAgent *)g_cCuiDataMgr.GetTaskPtrBySession(m_strCuSession, GetInstance());

	int playId = m_tReq.GetDstChn().GetChnNO();

	for(TMediaDescList::iterator itrMediaDesc = tMediaDescList.begin(); 
		itrMediaDesc!=tMediaDescList.end(); itrMediaDesc++)
	{
		TMediaTransChannelList& tMediaTransChnList = itrMediaDesc->GetMediaTransChnList();
		for(TMediaTransChannelList::iterator itrTransChn = tMediaTransChnList.begin(); 
			itrTransChn!=tMediaTransChnList.end(); itrTransChn++)
		{
			if ((itrTransChn->GetTransChnType() == MEDIA_TRANS_CHN_RTP)
				|| (itrTransChn->GetTransChnType() == MEDIA_TRANS_CHN_RTCP)
				|| (itrTransChn->GetTransChnType() == MEDIA_TRANS_CHN_DATA))
			{
				TNetAddrList& tNatAddrList = itrTransChn->GetAddrList();
				for (TNetAddrList::iterator itrNatAddr = tNatAddrList.begin();
					itrNatAddr != tNatAddrList.end();itrNatAddr++)
				{
					if(NULL != pcCuAgent)
					{
						ChangeToNatAddr(pcCuAgent->m_strSession, *itrNatAddr, playId);
					}
				}
			}
			else
			{
				TASKLOG(NORMAL_TASK, ERROR_LEV, "不支持的媒体类型[%s]\n",
					itrTransChn->GetTransChnType().c_str());
			}
		}
	}
}

bool CPeInviteTaskNatSwitch::SwitchToCuiNatSwitchAddress(TSdp &tSdp, const TNetAddr &tCuiNatAddr)
{
	TMediaDescList& tMediaDescList = tSdp.GetMediaDescList();

	for(TMediaDescList::iterator itrMediaDesc = tMediaDescList.begin(); 
		itrMediaDesc!=tMediaDescList.end(); itrMediaDesc++)
	{
		TMediaTransChannelList& tMediaTransChnList = itrMediaDesc->GetMediaTransChnList();
		for(TMediaTransChannelList::iterator itrTransChn = tMediaTransChnList.begin(); 
			itrTransChn!=tMediaTransChnList.end(); itrTransChn++)
		{
			if (itrTransChn->GetTransChnType() == MEDIA_TRANS_CHN_RTP
				|| itrTransChn->GetTransChnType() == MEDIA_TRANS_CHN_RTCP
				|| itrTransChn->GetTransChnType() == MEDIA_TRANS_CHN_DATA)
			{
				list<string> tIpList;
				GetOnlineIpList(tIpList);
				
				TNetAddrList& tNatAddrList = itrTransChn->GetAddrList();
				if (tIpList.empty() || NOT_REQUEST_STATE != GetState()/*itrTransChn->GetTransChnType() == MEDIA_TRANS_CHN_DATA*/)
				{
					for (TNetAddrList::iterator itrNatAddr = tNatAddrList.begin();
						itrNatAddr != tNatAddrList.end();itrNatAddr++)
					{
						*itrNatAddr = tCuiNatAddr;
					}
				} 
				else
				{
					tNatAddrList.clear();
					for (list<string>::const_iterator itIp=tIpList.begin();
						itIp!=tIpList.end(); ++itIp)
					{
						TNetAddr tNetAddr;
						tNetAddr.SetNetIp(*itIp);
						tNetAddr.SetNetPort(tCuiNatAddr.GetNetPort());
						tNatAddrList.push_back(tNetAddr);
					}
				}

				itrTransChn->SetMediaChannelKey("");
			}
			else
			{
				TASKLOG(NORMAL_TASK, ERROR_LEV, "不支持的媒体类型[%s]\n",
					itrTransChn->GetTransChnType().c_str());
			}
		}
	}
	return true;
}

void CPeInviteTaskNatSwitch::ProcNatCBMsg(CMessage *const pcMsg)
{
	if ( pcMsg == NULL || pcMsg->content == NULL )
	{
		return;
	}

	TNatPacketInfo tNatPacket = *(reinterpret_cast<TNatPacketInfo*>(pcMsg->content));

	TMediaDescList& tMediaDescList = m_tReq.GetSdp().GetMediaDescList();

	bool bChnAddrChanged = false;

	//nat探测包的reserve无效，走老的替换流程：源地址和交换地址匹配才替换nat地址；
	//reserve有效，走（sdk）新的替换流程：nat地址和当前交换地址不匹配就替换nat地址。
	TASKLOG(NORMAL_TASK, EVENT_LEV, "Reserver[%d]\n", tNatPacket.m_byReserve);
	if (tNatPacket.IsReserveInvalid())//老流程
	{
		for(TMediaDescList::iterator itrMediaDesc = tMediaDescList.begin(); 
			itrMediaDesc!=tMediaDescList.end(); itrMediaDesc++)
		{
			TMediaTransChannelList& tMediaTransChnList = itrMediaDesc->GetMediaTransChnList();
			for(TMediaTransChannelList::iterator itrTransChn = tMediaTransChnList.begin(); 
				itrTransChn!=tMediaTransChnList.end(); itrTransChn++)
			{
				if ( itrTransChn->GetTransChnType() == MEDIA_TRANS_CHN_RTP
					|| itrTransChn->GetTransChnType() == MEDIA_TRANS_CHN_RTCP )
				{
					TNetAddrList& tNatAddrList = itrTransChn->GetAddrList();
					for (TNetAddrList::iterator itrNatAddr = tNatAddrList.begin();
						itrNatAddr != tNatAddrList.end();itrNatAddr++)
					{
						if (itrNatAddr->GetNetIpv4() == tNatPacket.m_dwSrcIp && itrNatAddr->GetNetPort() == tNatPacket.m_wSrcPort)
						{
							itrNatAddr->SetNetIpv4(tNatPacket.m_dwNatIp);
							itrNatAddr->SetNetPort(tNatPacket.m_wNatPort);

							bChnAddrChanged = true;
						}
					}
				}
				else if (itrTransChn->GetTransChnType() == MEDIA_TRANS_CHN_DATA)
				{
					TNetAddrList& tNatAddrList = itrTransChn->GetAddrList();
					for (TNetAddrList::iterator itrNatAddr = tNatAddrList.begin();
						itrNatAddr != tNatAddrList.end();itrNatAddr++)
					{
						if (itrNatAddr->GetNetIpv4() == tNatPacket.m_dwSrcIp && itrNatAddr->GetNetPort() == tNatPacket.m_wSrcPort)
						{
							itrNatAddr->SetNetIpv4(tNatPacket.m_dwNatIp);
							itrNatAddr->SetNetPort(tNatPacket.m_wNatPort);

							bChnAddrChanged = true;
						}
					}
				}
				else
				{
					TASKLOG(NORMAL_TASK, ERROR_LEV, "不支持的媒体类型[%s]\n",
						itrTransChn->GetTransChnType().c_str());
				}
			}
		}
	} 
	else//新流程
	{
		for(TMediaDescList::iterator itrMediaDesc = tMediaDescList.begin(); 
			itrMediaDesc!=tMediaDescList.end(); itrMediaDesc++)
		{
			if (MEDIA_TYPE_VIDEO == itrMediaDesc->GetMediaType()
				&& (NAT_PACK_TYPE_VIDEO_RTP == tNatPacket.m_byReserve || NAT_PACK_TYPE_VIDEO_RTCP == tNatPacket.m_byReserve))
			{
				TMediaTransChannelList& tMediaTransChnList = itrMediaDesc->GetMediaTransChnList();
				for(TMediaTransChannelList::iterator itrTransChn = tMediaTransChnList.begin(); 
					itrTransChn!=tMediaTransChnList.end(); itrTransChn++)
				{
					if (MEDIA_TRANS_CHN_RTP == itrTransChn->GetTransChnType() && NAT_PACK_TYPE_VIDEO_RTP == tNatPacket.m_byReserve)
					{
						TNetAddrList& tNatAddrList = itrTransChn->GetAddrList();
						for (TNetAddrList::iterator itrNatAddr = tNatAddrList.begin();
							itrNatAddr != tNatAddrList.end();itrNatAddr++)
						{
							if (itrNatAddr->GetNetIpv4() != tNatPacket.m_dwNatIp || itrNatAddr->GetNetPort() != tNatPacket.m_wNatPort)
							{
								TASKLOG(NORMAL_TASK, EVENT_LEV, "Bef LocalAddr[%u:%d], NatAddr[%u:%d], NatPacketNatAddr[%u:%d]\n",
									tNatPacket.m_dwNatIp, tNatPacket.m_wSrcPort,
									itrNatAddr->GetNetIpv4(), itrNatAddr->GetNetPort(),
									tNatPacket.m_dwNatIp, tNatPacket.m_wNatPort);
								itrNatAddr->SetNetIpv4(tNatPacket.m_dwNatIp);
								itrNatAddr->SetNetPort(tNatPacket.m_wNatPort);
								TASKLOG(NORMAL_TASK, EVENT_LEV, "Aft NatAddr[%u:%d]\n",
									itrNatAddr->GetNetIpv4(), itrNatAddr->GetNetPort());
								bChnAddrChanged = true;
							}
						}
					}
					else if (MEDIA_TRANS_CHN_RTCP == itrTransChn->GetTransChnType() && NAT_PACK_TYPE_VIDEO_RTCP == tNatPacket.m_byReserve)
					{
						TNetAddrList& tNatAddrList = itrTransChn->GetAddrList();
						for (TNetAddrList::iterator itrNatAddr = tNatAddrList.begin();
							itrNatAddr != tNatAddrList.end();itrNatAddr++)
						{
							if (itrNatAddr->GetNetIpv4() != tNatPacket.m_dwNatIp || itrNatAddr->GetNetPort() != tNatPacket.m_wNatPort)
							{
								TASKLOG(NORMAL_TASK, EVENT_LEV, "Bef LocalAddr[%u:%d], NatAddr[%u:%d], NatPacketNatAddr[%u:%d]\n",
									tNatPacket.m_dwNatIp, tNatPacket.m_wSrcPort,
									itrNatAddr->GetNetIpv4(), itrNatAddr->GetNetPort(),
									tNatPacket.m_dwNatIp, tNatPacket.m_wNatPort);
								itrNatAddr->SetNetIpv4(tNatPacket.m_dwNatIp);
								itrNatAddr->SetNetPort(tNatPacket.m_wNatPort);
								TASKLOG(NORMAL_TASK, EVENT_LEV, "Aft NatAddr[%u:%d]\n",
									itrNatAddr->GetNetIpv4(), itrNatAddr->GetNetPort());
								bChnAddrChanged = true;
							}
						}
					}
				}
			}
			else if (MEDIA_TYPE_AUDIO == itrMediaDesc->GetMediaType() 
				&& (NAT_PACK_TYPE_AUDIO_RTP == tNatPacket.m_byReserve || NAT_PACK_TYPE_AUDIO_RTCP == tNatPacket.m_byReserve))
			{
				TMediaTransChannelList& tMediaTransChnList = itrMediaDesc->GetMediaTransChnList();
				for(TMediaTransChannelList::iterator itrTransChn = tMediaTransChnList.begin(); 
					itrTransChn!=tMediaTransChnList.end(); itrTransChn++)
				{
					if (MEDIA_TRANS_CHN_RTP == itrTransChn->GetTransChnType() && NAT_PACK_TYPE_AUDIO_RTP == tNatPacket.m_byReserve)
					{
						TNetAddrList& tNatAddrList = itrTransChn->GetAddrList();
						for (TNetAddrList::iterator itrNatAddr = tNatAddrList.begin();
							itrNatAddr != tNatAddrList.end();itrNatAddr++)
						{
							if (itrNatAddr->GetNetIpv4() != tNatPacket.m_dwNatIp || itrNatAddr->GetNetPort() != tNatPacket.m_wNatPort)
							{
								TASKLOG(NORMAL_TASK, EVENT_LEV, "Bef LocalAddr[%u:%d], NatAddr[%u:%d], NatPacketNatAddr[%u:%d]\n",
									tNatPacket.m_dwNatIp, tNatPacket.m_wSrcPort,
									itrNatAddr->GetNetIpv4(), itrNatAddr->GetNetPort(),
									tNatPacket.m_dwNatIp, tNatPacket.m_wNatPort);
								itrNatAddr->SetNetIpv4(tNatPacket.m_dwNatIp);
								itrNatAddr->SetNetPort(tNatPacket.m_wNatPort);
								TASKLOG(NORMAL_TASK, EVENT_LEV, "Aft NatAddr[%u:%d]\n",
									itrNatAddr->GetNetIpv4(), itrNatAddr->GetNetPort());
								bChnAddrChanged = true;
							}
						}
					}
					else if (MEDIA_TRANS_CHN_RTCP == itrTransChn->GetTransChnType() && NAT_PACK_TYPE_AUDIO_RTCP == tNatPacket.m_byReserve)
					{
						TNetAddrList& tNatAddrList = itrTransChn->GetAddrList();
						for (TNetAddrList::iterator itrNatAddr = tNatAddrList.begin();
							itrNatAddr != tNatAddrList.end();itrNatAddr++)
						{
							if (itrNatAddr->GetNetIpv4() != tNatPacket.m_dwNatIp || itrNatAddr->GetNetPort() != tNatPacket.m_wNatPort)
							{
								TASKLOG(NORMAL_TASK, EVENT_LEV, "Bef LocalAddr[%u:%d], NatAddr[%u:%d], NatPacketNatAddr[%u:%d]\n",
									tNatPacket.m_dwNatIp, tNatPacket.m_wSrcPort,
									itrNatAddr->GetNetIpv4(), itrNatAddr->GetNetPort(),
									tNatPacket.m_dwNatIp, tNatPacket.m_wNatPort);
								itrNatAddr->SetNetIpv4(tNatPacket.m_dwNatIp);
								itrNatAddr->SetNetPort(tNatPacket.m_wNatPort);
								TASKLOG(NORMAL_TASK, EVENT_LEV, "Aft NatAddr[%u:%d]\n",
									itrNatAddr->GetNetIpv4(), itrNatAddr->GetNetPort());
								bChnAddrChanged = true;
							}
						}
					}
				}
			}
			else if (MEDIA_TYPE_DATA == itrMediaDesc->GetMediaType()
				&& (NAT_PACK_TYPE_DATA_RTP == tNatPacket.m_byReserve
				|| NAT_PACK_TYPE_DATA_RTCP == tNatPacket.m_byReserve
				|| NAT_PACK_TYPE_DATA_DATA == tNatPacket.m_byReserve))
			{
				TMediaTransChannelList& tMediaTransChnList = itrMediaDesc->GetMediaTransChnList();
				for(TMediaTransChannelList::iterator itrTransChn = tMediaTransChnList.begin(); 
					itrTransChn!=tMediaTransChnList.end(); itrTransChn++)
				{
					if (MEDIA_TRANS_CHN_RTP == itrTransChn->GetTransChnType() && NAT_PACK_TYPE_DATA_RTP == tNatPacket.m_byReserve)
					{
						TNetAddrList& tNatAddrList = itrTransChn->GetAddrList();
						for (TNetAddrList::iterator itrNatAddr = tNatAddrList.begin();
							itrNatAddr != tNatAddrList.end();itrNatAddr++)
						{
							if (itrNatAddr->GetNetIpv4() != tNatPacket.m_dwNatIp || itrNatAddr->GetNetPort() != tNatPacket.m_wNatPort)
							{
								TASKLOG(NORMAL_TASK, EVENT_LEV, "Bef LocalAddr[%u:%d], NatAddr[%u:%d], NatPacketNatAddr[%u:%d]\n",
									tNatPacket.m_dwNatIp, tNatPacket.m_wSrcPort,
									itrNatAddr->GetNetIpv4(), itrNatAddr->GetNetPort(),
									tNatPacket.m_dwNatIp, tNatPacket.m_wNatPort);
								itrNatAddr->SetNetIpv4(tNatPacket.m_dwNatIp);
								itrNatAddr->SetNetPort(tNatPacket.m_wNatPort);
								TASKLOG(NORMAL_TASK, EVENT_LEV, "Aft NatAddr[%u:%d]\n",
									itrNatAddr->GetNetIpv4(), itrNatAddr->GetNetPort());
								bChnAddrChanged = true;
							}
						}
					}
					else if (MEDIA_TRANS_CHN_RTCP == itrTransChn->GetTransChnType() && NAT_PACK_TYPE_DATA_RTCP == tNatPacket.m_byReserve)
					{
						TNetAddrList& tNatAddrList = itrTransChn->GetAddrList();
						for (TNetAddrList::iterator itrNatAddr = tNatAddrList.begin();
							itrNatAddr != tNatAddrList.end();itrNatAddr++)
						{
							if (itrNatAddr->GetNetIpv4() != tNatPacket.m_dwNatIp || itrNatAddr->GetNetPort() != tNatPacket.m_wNatPort)
							{
								TASKLOG(NORMAL_TASK, EVENT_LEV, "Bef LocalAddr[%u:%d], NatAddr[%u:%d], NatPacketNatAddr[%u:%d]\n",
									tNatPacket.m_dwNatIp, tNatPacket.m_wSrcPort,
									itrNatAddr->GetNetIpv4(), itrNatAddr->GetNetPort(),
									tNatPacket.m_dwNatIp, tNatPacket.m_wNatPort);
								itrNatAddr->SetNetIpv4(tNatPacket.m_dwNatIp);
								itrNatAddr->SetNetPort(tNatPacket.m_wNatPort);
								TASKLOG(NORMAL_TASK, EVENT_LEV, "Aft NatAddr[%u:%d]\n",
									itrNatAddr->GetNetIpv4(), itrNatAddr->GetNetPort());
								bChnAddrChanged = true;
							}
						}
					}
					else if (MEDIA_TRANS_CHN_DATA == itrTransChn->GetTransChnType() && NAT_PACK_TYPE_DATA_DATA == tNatPacket.m_byReserve)
					{
						TNetAddrList& tNatAddrList = itrTransChn->GetAddrList();
						for (TNetAddrList::iterator itrNatAddr = tNatAddrList.begin();
							itrNatAddr != tNatAddrList.end();itrNatAddr++)
						{
							if (itrNatAddr->GetNetIpv4() != tNatPacket.m_dwNatIp || itrNatAddr->GetNetPort() != tNatPacket.m_wNatPort)
							{
								TASKLOG(NORMAL_TASK, EVENT_LEV, "Bef LocalAddr[%u:%d], NatAddr[%u:%d], NatPacketNatAddr[%u:%d]\n",
									tNatPacket.m_dwNatIp, tNatPacket.m_wSrcPort,
									itrNatAddr->GetNetIpv4(), itrNatAddr->GetNetPort(),
									tNatPacket.m_dwNatIp, tNatPacket.m_wNatPort);
								itrNatAddr->SetNetIpv4(tNatPacket.m_dwNatIp);
								itrNatAddr->SetNetPort(tNatPacket.m_wNatPort);
								TASKLOG(NORMAL_TASK, EVENT_LEV, "Aft NatAddr[%u:%d]\n",
									itrNatAddr->GetNetIpv4(), itrNatAddr->GetNetPort());
								bChnAddrChanged = true;
							}
						}
					}
				}
			}
		}
	}	

	if ( bChnAddrChanged )
	{
		bChnAddrChanged = true;
		this->RemoveCuiSwitch();
		this->GenSwitchChn();
		this->AddCuiSwitch();
	}
}

bool CPeInviteTaskNatSwitch::CuiAddNatSwitch()
{
	if (!this->GenSwitchChn())
	{
		return false;
	}

	return AddCuiSwitch();
}

bool CPeInviteTaskNatSwitch::GenSwitchChn()
{
	//一次浏览事务只有一个交换Sess号，该Sess号被各个数据通道共享
	m_tSwitchSessID = GetTaskNO();

	TNetAddr tCuiNatAddr;
	if ( !this->GetCuiNatAddrForPlat(tCuiNatAddr) )
	{
		return false;
	}

	TMediaDescList& tDstCuMediaTransDescList = m_tReq.GetSdp().GetMediaDescList();

	for(size_t i=0; i<tDstCuMediaTransDescList.size(); i++)
	{
		TMediaDesc& tCuMediaDesc = tDstCuMediaTransDescList[i];

		TMediaTransChannelList& tCuMediaChnList = tCuMediaDesc.GetMediaTransChnList();
		for (size_t nChnIdx=0; nChnIdx<tCuMediaChnList.size(); nChnIdx++)
		{
			TMediaTransChannel& tCuMediaChn = tCuMediaChnList[nChnIdx];
			TSwitchChannel tSwitchChn;
			tSwitchChn.SetSwitchSsnID(m_tSwitchSessID);
			tSwitchChn.SetMediaType(tCuMediaDesc.GetMediaType());
			tSwitchChn.SetMediaFormat(tCuMediaDesc.GetMediaFormat());
			tSwitchChn.SetMediaTransProto(tCuMediaDesc.GetMediaTransProto());
			tSwitchChn.SetSwitchType(SWITCH_TYPE_M2P);
			tSwitchChn.SetTransChnType(tCuMediaChn.GetTransChnType());
			tSwitchChn.SetGapSwitch(false);

			tSwitchChn.SetRcvNetAddr(tCuiNatAddr);
			if (tCuMediaDesc.GetMediaType() == MEDIA_TYPE_VIDEO)
			{
				if (tCuMediaChn.GetTransChnType() == MEDIA_TRANS_CHN_RTP)
				{
					tSwitchChn.SetDstNetAddrList(tCuMediaChn.GetAddrList());                

					TNetAddrList tSrcVtduNetAddrList;
					TNetAddr tVtduNetAddr;
					tVtduNetAddr.SetNetIp(this->GetPeIp());
					tVtduNetAddr.SetNetPort(this->GetPeVideoRtpChn());
					tSrcVtduNetAddrList.push_back(tVtduNetAddr);
					tSwitchChn.SetSrcNetAddrList(tSrcVtduNetAddrList);

					tSwitchChn.SetSrcDevChn(m_peChn);
					tSwitchChn.SetDstDevChn(m_cuChn);

					m_tVideoRtpChn = tSwitchChn;
				}
				else if (tCuMediaChn.GetTransChnType() == MEDIA_TRANS_CHN_RTCP)
				{
					tSwitchChn.SetSrcNetAddrList(tCuMediaChn.GetAddrList());   

					TNetAddrList tSrcVtduNetAddrList;
					TNetAddr tVtduNetAddr;
					tVtduNetAddr.SetNetIp(this->GetPeIp());
					tVtduNetAddr.SetNetPort(this->GetPeVideoRtcpChn());

					tSrcVtduNetAddrList.push_back(tVtduNetAddr);
					tSwitchChn.SetDstNetAddrList(tSrcVtduNetAddrList);

					tSwitchChn.SetSrcDevChn(m_cuChn);
					tSwitchChn.SetDstDevChn(m_peChn);

					m_tVideoRtcpChn = tSwitchChn;
				}
				else
				{
					TASKLOG(NORMAL_TASK, ERROR_LEV, "不支持的媒体通道类型[%s]\n",
						tCuMediaChn.GetTransChnType().c_str());
				}              
			}
			else if (tCuMediaDesc.GetMediaType() == MEDIA_TYPE_AUDIO)
			{
				if (tCuMediaChn.GetTransChnType() == MEDIA_TRANS_CHN_RTP)
				{
					//audio rtp 通道

					tSwitchChn.SetDstNetAddrList(tCuMediaChn.GetAddrList());                

					TNetAddrList tSrcVtduNetAddrList;
					TNetAddr tVtduNetAddr;
					tVtduNetAddr.SetNetIp(this->GetPeIp());
					tVtduNetAddr.SetNetPort(this->GetPeAudioRtpChn());
					tSrcVtduNetAddrList.push_back(tVtduNetAddr);
					tSwitchChn.SetSrcNetAddrList(tSrcVtduNetAddrList);

					tSwitchChn.SetSrcDevChn(m_peChn);
					tSwitchChn.SetDstDevChn(m_cuChn);

					m_tAudioRtpChn = tSwitchChn;
				}
				else if (tCuMediaChn.GetTransChnType() == MEDIA_TRANS_CHN_RTCP)
				{
					//audio rtcp 通道

					tSwitchChn.SetSrcNetAddrList(tCuMediaChn.GetAddrList());   

					TNetAddrList tSrcVtduNetAddrList;
					TNetAddr tVtduNetAddr;
					tVtduNetAddr.SetNetIp(this->GetPeIp());
					tVtduNetAddr.SetNetPort(this->GetPeAudioRtcpChn());

					tSrcVtduNetAddrList.push_back(tVtduNetAddr);
					tSwitchChn.SetDstNetAddrList(tSrcVtduNetAddrList);

					tSwitchChn.SetSrcDevChn(m_cuChn);
					tSwitchChn.SetDstDevChn(m_peChn);

					m_tAudioRtcpChn = tSwitchChn;
				}
				else
				{
					TASKLOG(NORMAL_TASK, ERROR_LEV, "不支持的媒体通道类型[%s]\n",
						tCuMediaChn.GetTransChnType().c_str());
				}  
			}
			else if (tCuMediaDesc.GetMediaType() == MEDIA_TYPE_DATA)
			{
				if (tCuMediaChn.GetTransChnType() == MEDIA_TRANS_CHN_RTP)
				{
					//data rtp 通道

					tSwitchChn.SetDstNetAddrList(tCuMediaChn.GetAddrList());                

					TNetAddrList tSrcVtduNetAddrList;
					TNetAddr tVtduNetAddr;
					tVtduNetAddr.SetNetIp(this->GetPeIp());
					tVtduNetAddr.SetNetPort(this->GetPeDataRtpChn());
					tSrcVtduNetAddrList.push_back(tVtduNetAddr);
					tSwitchChn.SetSrcNetAddrList(tSrcVtduNetAddrList);

					tSwitchChn.SetSrcDevChn(m_peChn);
					tSwitchChn.SetDstDevChn(m_cuChn);

					m_tDataRtpChn = tSwitchChn;
				}
				else if (tCuMediaChn.GetTransChnType() == MEDIA_TRANS_CHN_RTCP)
				{
					//data rtcp 通道

					tSwitchChn.SetSrcNetAddrList(tCuMediaChn.GetAddrList());   

					TNetAddrList tSrcVtduNetAddrList;
					TNetAddr tVtduNetAddr;
					tVtduNetAddr.SetNetIp(this->GetPeIp());
					tVtduNetAddr.SetNetPort(this->GetPeDataRtcpChn());

					tSrcVtduNetAddrList.push_back(tVtduNetAddr);
					tSwitchChn.SetDstNetAddrList(tSrcVtduNetAddrList);

					tSwitchChn.SetSrcDevChn(m_cuChn);
					tSwitchChn.SetDstDevChn(m_peChn);

					m_tDataRtcpChn = tSwitchChn;
				}
				else if (tCuMediaChn.GetTransChnType() == MEDIA_TRANS_CHN_DATA)
				{
					//data 需要建立双向双向的交换

					// rtp data
					tSwitchChn.SetTransChnType(MEDIA_TRANS_CHN_RTP);

					tSwitchChn.SetDstNetAddrList(tCuMediaChn.GetAddrList());     
					
					TNetAddrList tRtpSrcVtduNetAddrList;
					TNetAddr tRtpVtduNetAddr;
					tRtpVtduNetAddr.SetNetIp(this->GetPeIp());
					tRtpVtduNetAddr.SetNetPort(this->GetPeDataChn());
					tRtpSrcVtduNetAddrList.push_back(tRtpVtduNetAddr);
					tSwitchChn.SetSrcNetAddrList(tRtpSrcVtduNetAddrList);

					tSwitchChn.SetSrcDevChn(m_peChn);
					tSwitchChn.SetDstDevChn(m_cuChn);


					m_tDataRtpChn = tSwitchChn;


					// rtcp data
					tSwitchChn.SetTransChnType(MEDIA_TRANS_CHN_RTCP);
					tSwitchChn.SetSrcNetAddrList(tCuMediaChn.GetAddrList());   

					TNetAddrList tSrcVtduNetAddrList;
					TNetAddr tVtduNetAddr;
					tVtduNetAddr.SetNetIp(this->GetPeIp());
					tVtduNetAddr.SetNetPort(this->GetPeDataChn());

					tSrcVtduNetAddrList.push_back(tVtduNetAddr);
					tSwitchChn.SetDstNetAddrList(tSrcVtduNetAddrList);

					tSwitchChn.SetSrcDevChn(m_cuChn);
					tSwitchChn.SetDstDevChn(m_peChn);

					m_tDataRtcpChn = tSwitchChn;
				}
				else
				{
					TASKLOG(NORMAL_TASK, ERROR_LEV, "不支持的媒体通道类型[%s]\n",
						tCuMediaChn.GetTransChnType().c_str());
				}  
			}
			else
			{
				TASKLOG(NORMAL_TASK, ERROR_LEV, "未知媒体类型[%s]\n", tCuMediaDesc.GetMediaType().c_str());
			}
		}
	}

	return true;
}

bool CPeInviteTaskNatSwitch::IsSendVideo()
{
	const TMediaDescList& tDstMediaTransDescList = m_tReq.GetSdp().GetMediaDescList();

	for(size_t i=0; i<tDstMediaTransDescList.size(); i++)
	{
		const TMediaDesc& tMediaDesc = tDstMediaTransDescList[i];
		if (tMediaDesc.GetMediaType() == MEDIA_TYPE_VIDEO)
		{
			return true;
		}
	}
	return false;
}

bool CPeInviteTaskNatSwitch::IsSendAudio()
{
	const TMediaDescList& tDstMediaTransDescList = m_tReq.GetSdp().GetMediaDescList();

	for(size_t i=0; i<tDstMediaTransDescList.size(); i++)
	{
		const TMediaDesc& tMediaDesc = tDstMediaTransDescList[i];
		if (tMediaDesc.GetMediaType() == MEDIA_TYPE_AUDIO)
		{
			return true;
		}
	}
	return false;
}

bool CPeInviteTaskNatSwitch::IsSendData()
{
	const TMediaDescList& tDstMediaTransDescList = m_tReq.GetSdp().GetMediaDescList();

	for(size_t i=0; i<tDstMediaTransDescList.size(); i++)
	{
		const TMediaDesc& tMediaDesc = tDstMediaTransDescList[i];
		if (tMediaDesc.GetMediaType() == MEDIA_TYPE_DATA)
		{
			return true;
		}
	}
	return false;
}
bool CPeInviteTaskNatSwitch::AddCuiSwitch()
{
	if ( IsSendData() )
	{
		if (AddSwitch(m_tDataRtpChn) != CMS_SUCCESS)
		{
			TASKLOG(NORMAL_TASK, ERROR_LEV, "add video rtp switch fail\n");
			return false;
		}

		if (AddSwitch(m_tDataRtcpChn) != CMS_SUCCESS)
		{
			TASKLOG(NORMAL_TASK, ERROR_LEV, "add video rtcp switch fail\n");
			return false;
		}

		// 不可能同时发送数据和音视频码流
		return  true;
	}

	if (IsSendVideo())
	{
		if (AddSwitch(m_tVideoRtpChn) != CMS_SUCCESS)
		{
			TASKLOG(NORMAL_TASK, ERROR_LEV, "add video rtp switch fail\n");
			return false;
		}

		if (AddSwitch(m_tVideoRtcpChn) != CMS_SUCCESS)
		{
			TASKLOG(NORMAL_TASK, ERROR_LEV, "add video rtcp switch fail\n");
			return false;
		}     
	}

	if (IsSendAudio())
	{
		if (AddSwitch(m_tAudioRtpChn) != CMS_SUCCESS)
		{
			TASKLOG(NORMAL_TASK, ERROR_LEV, "add audio rtp switch fail\n");
			return false;
		}

		if (AddSwitch(m_tAudioRtcpChn) != CMS_SUCCESS)
		{
			TASKLOG(NORMAL_TASK, ERROR_LEV, "add audio rtcp switch fail\n");
			return false;
		}  
	}

	return true;
}

void CPeInviteTaskNatSwitch::RemoveCuiSwitch()
{
	if (m_tSwitchSessID != 0)
	{
		if (RemoveSwitch(m_tSwitchSessID) == CMS_SUCCESS)
		{
			TASKLOG(NORMAL_TASK, WARNING_LEV,"删除交换[m_tSwitchSessID = %u]成功\n", u32(m_tSwitchSessID));
			m_tSwitchSessID = 0;
		}
		else
		{
			TASKLOG(NORMAL_TASK, ERROR_LEV, "删除交换[m_tSwitchSessID = %u]失败\n", u32(m_tSwitchSessID));
		}
	}
	else
	{
		TASKLOG(NORMAL_TASK, EVENT_LEV, "m_tSwitchSessID == 0\n");
	}
}

TIpAddr CPeInviteTaskNatSwitch::GetPeIp()
{
	TSdp &tsdp = m_tRsp.GetSdp();
	TMediaDescList &descList = tsdp.GetMediaDescList();
	for ( int iDesc = 0; iDesc < descList.size(); iDesc++)
	{
		if ( descList[iDesc].GetMediaType() == MEDIA_TYPE_VIDEO
			|| descList[iDesc].GetMediaType() == MEDIA_TYPE_AUDIO
			|| descList[iDesc].GetMediaType() == MEDIA_TYPE_DATA )
		{
			TMediaTransChannelList &chnList = descList[iDesc].GetMediaTransChnList();
			for ( int iTransChn = 0; iTransChn < chnList.size(); iTransChn++ )
			{
				if ( chnList[iTransChn].GetAddrList().size() > 0 )
				{
					return chnList[iTransChn].GetAddrList()[0].GetNetIp();
				}
			}
		}
	}
	//2018/10/31 竟然敢给string返回一个0代替，必然崩溃啊  zsy
	//return 0;
	return "";
}

s32 CPeInviteTaskNatSwitch::GetPeVideoRtpChn()
{
	TSdp &tsdp = m_tRsp.GetSdp();
	TMediaDescList &descList = tsdp.GetMediaDescList();
	for ( int iDesc = 0; iDesc < descList.size(); iDesc++)
	{
		if ( descList[iDesc].GetMediaType() == MEDIA_TYPE_VIDEO )
		{
			TMediaTransChannelList &chnList = descList[iDesc].GetMediaTransChnList();
			for ( int iTransChn = 0; iTransChn < chnList.size(); iTransChn++ )
			{
				if (chnList[iTransChn].GetTransChnType() == MEDIA_TRANS_CHN_RTP)
				{
					if ( chnList[iTransChn].GetAddrList().size() > 0 )
					{
						return chnList[iTransChn].GetAddrList()[0].GetNetPort();
					}
				}
			}
		}
	}
	return 0;
}

s32 CPeInviteTaskNatSwitch::GetPeVideoRtcpChn()
{
	TSdp &tsdp = m_tRsp.GetSdp();
	TMediaDescList &descList = tsdp.GetMediaDescList();
	for ( int iDesc = 0; iDesc < descList.size(); iDesc++)
	{
		if ( descList[iDesc].GetMediaType() == MEDIA_TYPE_VIDEO )
		{
			TMediaTransChannelList &chnList = descList[iDesc].GetMediaTransChnList();
			for ( int iTransChn = 0; iTransChn < chnList.size(); iTransChn++ )
			{
				if (chnList[iTransChn].GetTransChnType() == MEDIA_TRANS_CHN_RTCP)
				{
					if ( chnList[iTransChn].GetAddrList().size() > 0 )
					{
						return chnList[iTransChn].GetAddrList()[0].GetNetPort();
					}
				}
			}
		}
	}
	return 0;
}

s32 CPeInviteTaskNatSwitch::GetPeAudioRtpChn()
{
	TSdp &tsdp = m_tRsp.GetSdp();
	TMediaDescList &descList = tsdp.GetMediaDescList();
	for ( int iDesc = 0; iDesc < descList.size(); iDesc++)
	{
		if ( descList[iDesc].GetMediaType() == MEDIA_TYPE_AUDIO )
		{
			TMediaTransChannelList &chnList = descList[iDesc].GetMediaTransChnList();
			for ( int iTransChn = 0; iTransChn < chnList.size(); iTransChn++ )
			{
				if (chnList[iTransChn].GetTransChnType() == MEDIA_TRANS_CHN_RTP)
				{
					if ( chnList[iTransChn].GetAddrList().size() > 0 )
					{
						return chnList[iTransChn].GetAddrList()[0].GetNetPort();
					}
				}
			}
		}
	}
	return 0;
}

s32 CPeInviteTaskNatSwitch::GetPeAudioRtcpChn()
{
	TSdp &tsdp = m_tRsp.GetSdp();
	TMediaDescList &descList = tsdp.GetMediaDescList();
	for ( int iDesc = 0; iDesc < descList.size(); iDesc++)
	{
		if ( descList[iDesc].GetMediaType() == MEDIA_TYPE_AUDIO )
		{
			TMediaTransChannelList &chnList = descList[iDesc].GetMediaTransChnList();
			for ( int iTransChn = 0; iTransChn < chnList.size(); iTransChn++ )
			{
				if (chnList[iTransChn].GetTransChnType() == MEDIA_TRANS_CHN_RTCP)
				{
					if ( chnList[iTransChn].GetAddrList().size() > 0 )
					{
						return chnList[iTransChn].GetAddrList()[0].GetNetPort();
					}
				}
			}
		}
	}
	return 0;
}

s32 CPeInviteTaskNatSwitch::GetPeDataChn()
{
	TSdp &tsdp = m_tRsp.GetSdp();
	TMediaDescList &descList = tsdp.GetMediaDescList();
	for ( int iDesc = 0; iDesc < descList.size(); iDesc++)
	{
		if ( descList[iDesc].GetMediaType() == MEDIA_TYPE_DATA )
		{
			TMediaTransChannelList &chnList = descList[iDesc].GetMediaTransChnList();
			for ( int iTransChn = 0; iTransChn < chnList.size(); iTransChn++ )
			{
				if (chnList[iTransChn].GetTransChnType() == MEDIA_TRANS_CHN_DATA)
				{
					if ( chnList[iTransChn].GetAddrList().size() > 0 )
					{
						return chnList[iTransChn].GetAddrList()[0].GetNetPort();
					}
				}
			}
		}
	}
	return 0;
}
s32 CPeInviteTaskNatSwitch::GetPeDataRtpChn()
{
	TSdp &tsdp = m_tRsp.GetSdp();
	TMediaDescList &descList = tsdp.GetMediaDescList();
	for ( int iDesc = 0; iDesc < descList.size(); iDesc++)
	{
		if ( descList[iDesc].GetMediaType() == MEDIA_TYPE_DATA )
		{
			TMediaTransChannelList &chnList = descList[iDesc].GetMediaTransChnList();
			for ( int iTransChn = 0; iTransChn < chnList.size(); iTransChn++ )
			{
				if (chnList[iTransChn].GetTransChnType() == MEDIA_TRANS_CHN_RTP)
				{
					if ( chnList[iTransChn].GetAddrList().size() > 0 )
					{
						return chnList[iTransChn].GetAddrList()[0].GetNetPort();
					}
				}
			}
		}
	}
	return 0;
}

s32 CPeInviteTaskNatSwitch::GetPeDataRtcpChn()
{
	TSdp &tsdp = m_tRsp.GetSdp();
	TMediaDescList &descList = tsdp.GetMediaDescList();
	for ( int iDesc = 0; iDesc < descList.size(); iDesc++)
	{
		if ( descList[iDesc].GetMediaType() == MEDIA_TYPE_DATA )
		{
			TMediaTransChannelList &chnList = descList[iDesc].GetMediaTransChnList();
			for ( int iTransChn = 0; iTransChn < chnList.size(); iTransChn++ )
			{
				if (chnList[iTransChn].GetTransChnType() == MEDIA_TRANS_CHN_RTCP)
				{
					if ( chnList[iTransChn].GetAddrList().size() > 0 )
					{
						return chnList[iTransChn].GetAddrList()[0].GetNetPort();
					}
				}
			}
		}
	}
	return 0;
}

void CPeInviteTaskNatSwitch::AddChildTask(int nSeqNo, int nTaskNo)
{
    if(nTaskNo == INVALID_TASKNO)
    {
        TASKLOG(ERROR_LEV, "TaskNo is Invalid");
        return;
    }
    m_cChildTaskList.Insert(nSeqNo, nTaskNo);
}
void CPeInviteTaskNatSwitch::DelChildTask(int nSeqNo)
{
    m_cChildTaskList.Erase(nSeqNo);
}
CTask* CPeInviteTaskNatSwitch::FindChildTask(int nSeqNo)
{
    int nTaskNo = INVALID_TASKNO;
    m_cChildTaskList.Find(nSeqNo, nTaskNo);
    return GetInstance()->FindTask(nTaskNo);
}

void CPeInviteTaskNatSwitch::NotifyStop2Cu(s32 dwErrorCode)
{
    CInviteStopNotify cInviteStopNtf;
    cInviteStopNtf.playId = m_tReq.GetDstChn().GetChnNO();
    cInviteStopNtf.errorCode = dwErrorCode;

    CCuAgent *pcCuAgent = (CCuAgent *)g_cCuiDataMgr.GetTaskPtrBySession(m_strCuSession,GetInstance());
    CMS_ASSERT(pcCuAgent != NULL);
    if(pcCuAgent != NULL)
    {
        pcCuAgent->m_cNotifyPool.InsertInviteStopNotify(cInviteStopNtf);
    }
}

//TChannel& CPeInviteTaskNatSwitch::GetDevChn()
//{
//    return m_tReq.GetSrcChn();
//}

u32 CPeInviteTaskNatSwitch::WaitSetupReqTimerPoll()
{
    if ( GetCurStateHoldTime() > g_cCuiConfig.GetDlgWaitRspTimeout() )
    {
        TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户会话[%s]等待外设模块建链超时，销毁CInviteTask[TaskNO: %u]\n",
            m_strCuSession.c_str(), GetTaskNO());

        /*
        这里不用通知CInviteInterAction操作超时
        只是取消m_pcCuAgent->m_cInviteTaskSet中的登记
        等CInviteInterAction消息自己超时好了

        因为此时还未建立会话，所以也无需发送Bye，以及ReleaseDialog
        */
        return TIMERPOLL_DEL;
    }
    return TIMERPOLL_DONE;
}

u32 CPeInviteTaskNatSwitch::WaitSetupRspTimerPoll()
{
    if ( GetCurStateHoldTime() > g_cCuiConfig.GetDlgWaitRspTimeout() )
    {
        TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户会话[%s]等待InviteRsp超时，销毁CInviteTask[TaskNO: %u]\n",
            m_strCuSession.c_str(), GetTaskNO());

        /*
        这里不用通知CInviteInterAction操作超时
        只是取消m_pcCuAgent->m_cInviteTaskSet中的登记
        等CInviteInterAction消息自己超时好了

        因为此时还未建立会话，所以也无需发送Bye，以及ReleaseDialog
        */
        return TIMERPOLL_DEL;
    }
    return TIMERPOLL_DONE;
}

u32 CPeInviteTaskNatSwitch::WaitByeRspTimerPoll()
{
    if ( GetCurStateHoldTime() > g_cCuiConfig.GetDlgWaitRspTimeout())
    {
        TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户会话[%s]等待Bye Rsp超时，结束Dialog\n",
            m_strCuSession.c_str());
        m_tInviteID = INVALID_DIALOG_ID;
        return TIMERPOLL_DEL;
    }
    return TIMERPOLL_DONE;
}

u32 CPeInviteTaskNatSwitch::StartSendReq()
{
    u32 dwRet = PROCMSG_OK;

    CPeAgent *pcPeAgent = (CPeAgent *)g_cCuiDataMgr.GetPeAgentPtrByPeName(m_tPeUri.GetURIString(), GetInstance());
    CMS_ASSERT(NULL != pcPeAgent);

    if(pcPeAgent->GetState() != CPeAgent::Service)
    {
        if(m_pcInviteInterActTask != NULL)
        {
            CPeInviteInterAction *pcInviteInterActTask= (CPeInviteInterAction *)m_pcInviteInterActTask;
            pcInviteInterActTask->PostErrMsg2Cu(ERR_CUI_PE_OFFLINE);
            delete pcInviteInterActTask;
            pcInviteInterActTask = NULL;

            return PROCMSG_DEL;
        }
    }


    if( pcPeAgent )
    {
        pcPeAgent->m_cInviteTaskSet.Insert(m_tReq.GetDstChn().GetChnNO(), GetTaskNO());
        m_strPeSession = pcPeAgent->m_strSessionID;
        //m_strReq = ReplaceXmlKeyText(m_strReq, m_strPeSession, "<session>", "</session>");
        m_tReq.SetSession(m_strPeSession);

        dwRet = PostReq(KDSIP_EVENT_INVITE_REQ, m_tReq, m_tPeUri);
        //dwRet = PostMsgReq(m_strReq, m_tPeUri);
        if(dwRet != PROCMSG_OK)
        {
            TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户会话[%s]发送[INVITE_REQ]消息失败\n",
                m_strCuSession.c_str());
            return dwRet;
        }
        NextState(WAIT_SETUP_RSP_STATE);
    }  

    return dwRet;
}

u32 CPeInviteTaskNatSwitch::WaitSetupRspProcMsg(CMessage *const pcMsg)
{
    COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
    TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户会话[%s]收到业务应答消息[%s--%d] (SIP应答消息[%s--%d])\n",
        m_strCuSession.c_str(),
        OspExtEventDesc(pcOspSipMsg->GetOspEventID()).c_str(), pcOspSipMsg->GetOspEventID(),
        pcOspSipMsg->GetSipEventStr().c_str(), pcOspSipMsg->GetSipEventID());

    u32 dwRet = PROCMSG_OK;

    switch ( pcMsg->event )
    {
    case OSP_SIP_BYE_REQ:   //其实这里不可能会收到Bye，因为呼叫还未建立，要么成功要么拒绝，收到403错误码
        {
            CMS_ASSERT(0&&"不可能在WaitSetupRsp状态下收到OSP_SIP_BYE_REQ消息");
        }
        break;
    case OSP_SIP_MSG_PROC_FAIL:
    case OSP_SIP_DIALOG_TERMINATE:
        {
            //将消息回调给InviteInterAckTask类
            if ( m_pcInviteInterActTask != NULL )
            {
                dwRet = m_pcInviteInterActTask->ProcMsg(pcMsg);
                if(PROCMSG_DEL ==  dwRet)
                {
                    delete m_pcInviteInterActTask;
                    m_pcInviteInterActTask = NULL;
                }
            }
            TASKLOG(NORMAL_TASK, ERROR_LEV, "收到InviteRsp失败消息，销毁CInviteTask[TaskNO: %u]\n",
                GetTaskNO());
            m_tInviteID = INVALID_DIALOG_ID;
            NextState(NOT_REQUEST_STATE);

            delete this;
            dwRet = PROCMSG_DEL;
        }
        break;
    case INVITE_RSP:
        {
            //判断是否业务层失败
            CInviteRsp cRsp;
            pcOspSipMsg->GetMsgBody(cRsp);

            //保存DlgID
            m_tInviteID = pcOspSipMsg->GetSipDlgID();
            m_tRsp = cRsp;

			m_peChn = cRsp.GetSdp().GetOwner();

			if (cRsp.GetErrorCode() == CMS_SUCCESS && m_bByeWhileWaitInviteRsp)
			{
				cRsp.SetErrorCode(ERR_CUI_DIALOG_TERMINATED_BY_RCS);
			}
			else if ( CMS_SUCCESS == cRsp.GetErrorCode() && !CuiAddNatSwitch())
			{
			// 源交换成功，但目的交换不成功
				cRsp.SetErrorCode(ERR_CUI_ADD_SWITCH_FAILED);
			}

			TNetAddr cuiNetAddr;
			if ( !this->GetCuiNatAddrForCu(cuiNetAddr) )
			{
				cRsp.SetErrorCode(ERR_CUI_ADD_SWITCH_FAILED);
				RemoveCuiSwitch();
			}

			if ( cRsp.GetErrorCode() == CMS_SUCCESS )
			{
				this->SwitchToCuiNatSwitchAddress(cRsp.GetSdp(), cuiNetAddr);

				cRsp.GetSdp().SetIsNeedPunch(false);
			}

			CMessage cInviteRsp;
			cInviteRsp.event = INVITE_RSP;
			string content = cRsp.ToXml();
			COspSipMsg sipmsg;
			sipmsg.SetMsgBody(content.c_str(), content.length()+1);
			sipmsg.SetSipDlgID(m_tInviteID);
			cInviteRsp.content = (u8*)sipmsg.GetMsgBuf();
			cInviteRsp.length = sipmsg.GetMsgBodyLen();

            //将消息回调给InviteInterAckTask类
            if ( m_pcInviteInterActTask != NULL )
            {
                dwRet = m_pcInviteInterActTask->ProcMsg(&cInviteRsp);
                if(PROCMSG_DEL == dwRet)
                {
                    delete m_pcInviteInterActTask;
                    m_pcInviteInterActTask = NULL;
                }

				dwRet = PROCMSG_OK;//InviteInterAckTask处理后恢复正常返回值
            }

            if ( INVALID_DIALOG_ID == m_tInviteID )
            {
                TASKLOG(NORMAL_TASK, ERROR_LEV, "收到的InviteRsp含带DialogID为 0 !!!!直接销毁CInviteTask[TaskNO %u]\n",
                    GetTaskNO());
                m_tInviteID = INVALID_DIALOG_ID;
                NextState(NOT_REQUEST_STATE);
                dwRet = PROCMSG_DEL;
                delete this;
                break;
            }

            if ( CMS_SUCCESS != cRsp.GetErrorCode() )
            {
                TASKLOG(NORMAL_TASK, ERROR_LEV, "Invite请求失败，错误码[%d]，销毁CInviteTask[TaskNO: %u]，结束Sip Dialog: [%u]\n",
                    cRsp.GetErrorCode(), GetTaskNO(), m_tInviteID);

                //dwRet = StartSendInviteBye();
                //OspSipReleaseDlg(m_tInviteID); 发送了Bye就不需要再主动释放
                //等待Bye应答之后再销毁本Task
                delete this;
                return PROCMSG_DEL;
            }
            else
            {
                CCMSTime cStartTime(m_tReq.GetSdp().GetTimeRange().GetStartTime());
                CCMSTime cEndTime(m_tReq.GetSdp().GetTimeRange().GetEndTime());

                m_tNtfReq.SetCurPlayTime((u32)cStartTime.GetTime());
                m_tNtfReq.SetStartPlayTime((u32)cStartTime.GetTime());
                if(cEndTime.GetTime() >= cStartTime.GetTime())
                {
                    m_tNtfReq.SetTotalPlayTime((u32)(cEndTime.GetTime() - cStartTime.GetTime()));
                }
                else
                {
                    m_tNtfReq.SetTotalPlayTime(0);
                }

                m_tNtfReq.SetIsPlayFinished(false);

                TASKLOG(NORMAL_TASK, EVENT_LEV, "Invite请求成功，将CInviteTask[TaskNO: %u, PlayId: %d]登记到CCuAgent的任务列表中\n",
                    GetTaskNO(), m_tReq.GetDstChn().GetChnNO());
                NextState(WAIT_SETUP_ACK_STATE);
                StartSendInviteAck();
            }
        }
        break;
    default:
        {
            TASKLOG(NORMAL_TASK, CRITICAL_LEV, "收到未知消息[%s--%hu]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            CMS_ASSERT(0&&"收到未知消息");
        }
        break;
    }
    return dwRet;
}

u32 CPeInviteTaskNatSwitch::WaitSetupAckProcMsg( CMessage *const pcMsg )
{
    COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
    TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户会话[%s]收到业务应答消息[%s--%d] (SIP应答消息[%s--%d])\n",
        m_strCuSession.c_str(),
        OspExtEventDesc(pcOspSipMsg->GetOspEventID()).c_str(), pcOspSipMsg->GetOspEventID(),
        pcOspSipMsg->GetSipEventStr().c_str(), pcOspSipMsg->GetSipEventID());

    switch ( pcMsg->event )
    {
    case OSP_SIP_BYE_REQ:
        {
            TASKLOG(NORMAL_TASK, EVENT_LEV, "CInviteTask[TaskNO %u]收到InviteBye Req，DialogID[%u]，自动回复应答\n",
                GetTaskNO(), pcOspSipMsg->GetSipDlgID() );
            //将消息回调给InviteInterAckTask类
            if ( m_pcInviteInterActTask != NULL )
            {
                u32 dwRet = PROCMSG_OK;
                dwRet = m_pcInviteInterActTask->ProcMsg(pcMsg);
                if(PROCMSG_DEL == dwRet)
                {
                    delete m_pcInviteInterActTask;
                    m_pcInviteInterActTask = NULL;
                }
            }

            //播放任务没有成功建立至少cu认为没有建立
            if(pcOspSipMsg->GetMsgBodyLen() >0)
            {
                CByeReq cByeReq;
                pcOspSipMsg->GetMsgBody(cByeReq);
                NotifyStop2Cu(cByeReq.GetReason());
            }
            else
            {
                NotifyStop2Cu(ERR_CUI_DIALOG_TERMINATED);
            }

            string strNullMsg;
            PostRsp(KDSIP_EVENT_INVITE_BYE_RSP, pcOspSipMsg->GetSipTransID(), strNullMsg);
            m_tInviteID = INVALID_DIALOG_ID;

            NextState(NOT_REQUEST_STATE);
            delete this;
            return PROCMSG_DEL;
        }
        break;
    case OSP_SIP_MSG_PROC_FAIL:
    case OSP_SIP_DIALOG_TERMINATE:
        {
            //将消息回调给InviteInterAckTask类
            if ( m_pcInviteInterActTask != NULL )
            {
                u32 dwRet = m_pcInviteInterActTask->ProcMsg(pcMsg);
                if(PROCMSG_DEL == dwRet)
                {
                    delete m_pcInviteInterActTask;
                    m_pcInviteInterActTask = NULL;
                }
            }
            TASKLOG(NORMAL_TASK, ERROR_LEV, "销毁CInviteTask[TaskNO: %u]，结束Sip Dialog: [%u]\n",
                GetTaskNO(), m_tInviteID);
            delete this;
            return PROCMSG_DEL;
        }
        break;
    default:
        {
            TASKLOG(NORMAL_TASK, CRITICAL_LEV, "收到未知消息[%s--%hu]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            CMS_ASSERT(0&&"收到未知消息");
        }
        break;
    }
    return PROCMSG_OK;
}

u32 CPeInviteTaskNatSwitch::WaitAckTimerPoll()
{
    if ( GetCurStateHoldTime() > g_cCuiConfig.GetSipWaitRspTimeout() * 2 )
    {
        TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户[%s]会话等待InviteAck超时，直接发送InviteBye结束Dlg\n",
            m_strCuSession.c_str());
        StartSendInviteBye();
        //return TIMERPOLL_DEL;
        return TIMERPOLL_DONE;  //发送ByeReq之后切换到WaitByeRsp状态了，不用删除
    }
    return TIMERPOLL_DONE;
}

u32 CPeInviteTaskNatSwitch::InviteHoldProcMsg( CMessage *const pcMsg )
{
    COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
    TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户会话[%s]收到业务应答消息[%s--%d] (SIP应答消息[%s--%d])\n",
        m_strCuSession.c_str(),
        OspExtEventDesc(pcOspSipMsg->GetOspEventID()).c_str(), pcOspSipMsg->GetOspEventID(),
        pcOspSipMsg->GetSipEventStr().c_str(), pcOspSipMsg->GetSipEventID());

    u32 dwRet = PROCMSG_OK;
    switch ( pcMsg->event )
    {
    case OSP_SIP_BYE_REQ:
        {
            TASKLOG(NORMAL_TASK, EVENT_LEV, "CInviteTask[TaskNO %u]收到InviteBye Req，DialogID[%u]，自动回复应答\n",
                GetTaskNO(), pcOspSipMsg->GetSipDlgID() );
            //将消息回调给InviteInterAckTask类
            if ( m_pcInviteInterActTask != NULL )
            {
                u32 dwRet = m_pcInviteInterActTask->ProcMsg(pcMsg);
                if(PROCMSG_DEL == dwRet)
                {
                    delete m_pcInviteInterActTask;
                    m_pcInviteInterActTask = NULL;
                }
            }

            if(pcOspSipMsg->GetMsgBodyLen() > 0)
            {
                CByeReq cByeReq;
                pcOspSipMsg->GetMsgBody(cByeReq);
                NotifyStop2Cu(cByeReq.GetReason());
            }
            else
            {
                NotifyStop2Cu(ERR_CUI_DIALOG_TERMINATED);
            }

            CCuAgent *pcCuAgent = (CCuAgent *)g_cCuiDataMgr.GetTaskPtrBySession(m_strCuSession,GetInstance());
            CMS_ASSERT(pcCuAgent != NULL);
            if(pcCuAgent != NULL)
            {
                string strNullMsg;
                PostRsp(KDSIP_EVENT_INVITE_BYE_RSP, pcOspSipMsg->GetSipTransID(), strNullMsg);
            }

            NextState(NOT_REQUEST_STATE);
            delete this;
            dwRet =  PROCMSG_DEL;
        }
        break;
    case OSP_SIP_MSG_PROC_FAIL:
    case OSP_SIP_DIALOG_TERMINATE:
        {
            //将消息回调给InviteInterAckTask类
            if ( m_pcInviteInterActTask != NULL )
            {
                u32 dwRet = m_pcInviteInterActTask->ProcMsg(pcMsg);
                if(dwRet == PROCMSG_DEL)
                {
                    delete m_pcInviteInterActTask;
                    m_pcInviteInterActTask = NULL;
                } 
            }
            if (OSP_SIP_MSG_PROC_FAIL == pcMsg->event)
            {
                TASKLOG(NORMAL_TASK, ERROR_LEV, "recv OSP_SIP_MSG_PROC_FAIL, sipErrorCode=%d, invite req:%s\n", 
                    pcOspSipMsg->GetSipErrorCode(), m_tReq.ToXml().c_str());
                StartSendInviteBye();
            }

            TASKLOG(NORMAL_TASK, ERROR_LEV, "销毁CInviteTask[TaskNO: %u]，结束Sip Dialog: [%u]\n",
                GetTaskNO(), m_tInviteID);
            NextState(NOT_REQUEST_STATE);
            delete this;
            dwRet =  PROCMSG_DEL;
        }
        break;
	case CUI_NATDETECT_CALLBACK:
		{
			ProcNatCBMsg(pcMsg);
		}
		break;
    case END_OF_FILE_NTF_REQ:
        {
            CEndOfFileNtfReq cReq;
            pcOspSipMsg->GetMsgBody(cReq);

            CEndOfFileNtfRsp cRsp;
            cRsp.SetHeadFromReq(cReq);

            PostRsp(KDSIP_EVENT_MESSAGE_DIALOG_RSP, pcOspSipMsg->GetSipTransID(), cRsp);

            NotifyStop2Cu(ERR_CUI_RECORD_END);

            StartSendInviteBye();
        }
        break;
    case PLAYBACK_PROGRESS_QUERY_RSP:
        {
            TASKLOG(NORMAL_TASK, EVENT_LEV, "CInviteTask[TaskNO %u]收到QueryProcess Rsp，DialogID[%u]，自动回复应答\n",
                GetTaskNO(), pcOspSipMsg->GetSipDlgID() );

            //将消息回调给InviteInterAckTask类
            if ( m_pcInviteInterActTask != NULL )
            {
                m_pcInviteInterActTask->ProcMsg(pcMsg);
            }
        }
        break;
    case PLAYBACK_PROGRESS_NTF_REQ:
        {
            TASKLOG(NORMAL_TASK, EVENT_LEV, "CInviteTask[TaskNO %u]收到PlayBack Process Ntf Req，DialogID[%u]，自动回复应答\n",
                GetTaskNO(), pcOspSipMsg->GetSipDlgID() );
            CPlaybackProgressNtfReq cReq;
            pcOspSipMsg->GetMsgBody(cReq);
            CPlaybackProgressNtfRsp cRsp;
            cRsp.SetHeadFromReq(cReq);
            PostRsp(KDSIP_EVENT_INFO_RSP, pcOspSipMsg->GetSipTransID(), cRsp);
            m_tNtfReq = cReq;
            /*CCMSTime cStartTime(m_tReq.GetSdp().GetTimeRange().GetStartTime());
            m_tNtfReq.SetStartPlayTime((u32)cStartTime.GetTime());*/
        }
        break;
    case VCR_CTRL_RSP:
        {
            TASKLOG(NORMAL_TASK, EVENT_LEV, "CInviteTask[TaskNO %u]收到VCR Ctrl Req，DialogID[%u]，自动回复应答\n",
                GetTaskNO(), pcOspSipMsg->GetSipDlgID() );
            CTask* pcTask =FindChildTask(pcOspSipMsg->GetOspSeqNO());
            if(pcTask != NULL)
            {
                pcTask->ProcMsg(pcMsg);
            }         
        }
        break;
    case RCS_TASK_START_RSP:
        {
            TASKLOG(NORMAL_TASK, EVENT_LEV, "CInviteTask[TaskNO %u]收到TaskStart Rsp，DialogID[%u]，自动回复应答\n",
                GetTaskNO(), pcOspSipMsg->GetSipDlgID() );
            if ( m_pcInviteInterActTask != NULL )
            {
                m_pcInviteInterActTask->ProcMsg(pcMsg);
            }
        }
        break;
    default:
        {
            TASKLOG(NORMAL_TASK, CRITICAL_LEV, "收到未知消息[%s--%hu]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            CMS_ASSERT(0&&"收到未知消息");
            dwRet = PROCMSG_FAIL;
        }
        break;
    }
    return dwRet;
}

u32 CPeInviteTaskNatSwitch::WaitByeRspProcMsg(CMessage *const pcMsg)
{
    COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
    TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户会话[%s]收到业务应答消息[%s--%d]\n",
        m_strCuSession.c_str(),
        pcOspSipMsg->GetSipEventStr().c_str(), pcOspSipMsg->GetSipEventID());

    u32 dwRet = PROCMSG_OK;
    switch ( pcMsg->event )
    {
    case OSP_SIP_BYE_REQ:   
        {
            TASKLOG(NORMAL_TASK, EVENT_LEV, "CInviteTask[TaskNO %u]收到InviteBye Req，DialogID[%u]，回复应答后销毁CInviteTask\n",
                GetTaskNO(), pcOspSipMsg->GetSipDlgID() );
            //将消息回调给InviteInterAckTask类
            if ( m_pcInviteInterActTask != NULL )
            {
                u32 dwRet = m_pcInviteInterActTask->ProcMsg(pcMsg);
                if(PROCMSG_DEL == dwRet)
                {
                    delete m_pcInviteInterActTask;
                    m_pcInviteInterActTask = NULL;
                }
            }

            string strNullMsg;
            PostRsp(KDSIP_EVENT_INVITE_BYE_RSP, pcOspSipMsg->GetSipTransID(), strNullMsg);
            //已经删除了OspSip的Dialog绑定，可以直接删除task
            m_tInviteID = INVALID_DIALOG_ID;
            NextState(NOT_REQUEST_STATE);
            delete this;
            dwRet = PROCMSG_DEL;
        }
        break;
    case OSP_SIP_BYE_RSP:
        {
            if ( m_pcInviteInterActTask != NULL )
            {
                u32 dwRet = m_pcInviteInterActTask->ProcMsg(pcMsg);
                if(PROCMSG_DEL == dwRet)
                {
                    delete m_pcInviteInterActTask;
                    m_pcInviteInterActTask = NULL;
                }
            }

            TASKLOG(NORMAL_TASK, EVENT_LEV, "销毁CInviteTask[TaskNO: %u]，结束Sip Dialog: [%u]\n",
                GetTaskNO(), m_tInviteID);
            m_tInviteID = INVALID_DIALOG_ID;
            NextState(NOT_REQUEST_STATE);
            delete this;
            dwRet = PROCMSG_DEL;
        }
        break;
    case OSP_SIP_MSG_PROC_FAIL:
    case OSP_SIP_DIALOG_TERMINATE:
        {
            //将消息回调给InviteInterAckTask类
            if ( m_pcInviteInterActTask != NULL )
            {
                u32 dwRet = m_pcInviteInterActTask->ProcMsg(pcMsg);
                if(PROCMSG_DEL == dwRet)
                {
                    delete m_pcInviteInterActTask;
                    m_pcInviteInterActTask = NULL;
                }
            }
            TASKLOG(NORMAL_TASK, ERROR_LEV, "销毁CInviteTask[TaskNO: %u]，结束Sip Dialog: [%u]\n",
                GetTaskNO(), m_tInviteID);
            //m_tInviteID = INVALID_DIALOG_ID;
            NextState(NOT_REQUEST_STATE);
            delete this;
            dwRet = PROCMSG_DEL;
        }
        break;
    case END_OF_FILE_NTF_REQ:
        {
            CEndOfFileNtfReq cReq;
            pcOspSipMsg->GetMsgBody(cReq);

            CEndOfFileNtfRsp cRsp;
            cRsp.SetHeadFromReq(cReq);

            PostRsp(KDSIP_EVENT_MESSAGE_DIALOG_RSP, pcOspSipMsg->GetSipTransID(), cRsp);

            TASKLOG(NORMAL_TASK, ERROR_LEV, "在CInviteTask[TaskNO: %u][WaitByeRspProcMsg]收到消息[%s--%hu]\n",
                GetTaskNO(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    case PLAYBACK_PROGRESS_QUERY_RSP:
        {
            TASKLOG(NORMAL_TASK, EVENT_LEV, "CInviteTask[TaskNO %u]收到QueryProcess Rsp，DialogID[%u]，自动回复应答\n",
                GetTaskNO(), pcOspSipMsg->GetSipDlgID() );

            //将消息回调给InviteInterAckTask类
            if ( m_pcInviteInterActTask != NULL )
            {
                m_pcInviteInterActTask->ProcMsg(pcMsg);
            }
        }
        break;
    case PLAYBACK_PROGRESS_NTF_REQ:
        {
            TASKLOG(NORMAL_TASK, EVENT_LEV, "CInviteTask[TaskNO %u]收到PlayBack Process Ntf Req，DialogID[%u]，自动回复应答\n",
                GetTaskNO(), pcOspSipMsg->GetSipDlgID() );
            CPlaybackProgressNtfReq cReq;
            pcOspSipMsg->GetMsgBody(cReq);
            CPlaybackProgressNtfRsp cRsp;
            cRsp.SetHeadFromReq(cReq);
            PostRsp(KDSIP_EVENT_INFO_RSP, pcOspSipMsg->GetSipTransID(), cRsp);
            m_tNtfReq = cReq;
            TASKLOG(NORMAL_TASK, EVENT_LEV, "PlayBack Process Ntf Req--CurTime[%d], TotalTime[%d]\n",
                cReq.GetCurPlayTime(), cReq.GetTotalPlayTime());
            CCMSTime cStartTime(m_tReq.GetSdp().GetTimeRange().GetStartTime());
            m_tNtfReq.SetStartPlayTime((u32)cStartTime.GetTime());
        }
        break;
    case VCR_CTRL_RSP:
        {
            TASKLOG(NORMAL_TASK, EVENT_LEV, "CInviteTask[TaskNO %u]收到VCR Ctrl Req，DialogID[%u]，自动回复应答\n",
                GetTaskNO(), pcOspSipMsg->GetSipDlgID() );
            CTask* pcTask =FindChildTask(pcOspSipMsg->GetOspSeqNO());
            if(pcTask != NULL)
            {
                pcTask->ProcMsg(pcMsg);
            }  
        }
        break;
    default:
        {
            TASKLOG(NORMAL_TASK, CRITICAL_LEV, "收到未知消息[%s--%hu]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            CMS_ASSERT(0&&"收到未知消息");
            dwRet = PROCMSG_FAIL;
        }
        break;
    }
    return dwRet;
}

/////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CPeInviteInterActionNatSwitch::CPeInviteInterActionNatSwitch(CInstExt *pcInst, const string &strCuSession, PTRecvNode ptRecvNode) : CSipTask(pcInst)
{
	CMS_ASSERT(!strCuSession.empty());
	CMS_ASSERT(ptRecvNode != NULL);

	m_ptRecvNode = ptRecvNode;
	m_strRecordPlaySession.clear();
	m_strCuSession = strCuSession;
	m_dwEventID = 0;
	m_nplayId = INVALID_PLAY_ID;
	m_dwSeqNum = INVALID_SEQ_NO;
}

CPeInviteInterActionNatSwitch::~CPeInviteInterActionNatSwitch()
{
	TerminateTask();
}

void CPeInviteInterActionNatSwitch::TerminateTask()
{
	//将对应的InviteTask的绑定解除
	if ( INVALID_PLAY_ID != m_nplayId )
	{
		CPeInviteTaskNatSwitch *pcInviteTask = (CPeInviteTaskNatSwitch *)GetInviteTask(m_nplayId);
		if ( NULL != pcInviteTask )
		{
			if ( pcInviteTask->m_pcInviteInterActTask == this )
			{
				pcInviteTask->m_pcInviteInterActTask = NULL;
			}
			if(m_dwSeqNum != INVALID_SEQ_NO)
			{
				pcInviteTask->DelChildTask(m_dwSeqNum);
			}
		}
	}
}

void CPeInviteInterActionNatSwitch::InitStateMachine()
{
	CStateProc cNotRequestProc;
	cNotRequestProc.ProcMsg = (CSipTask::PFProcMsg)&CPeInviteInterActionNatSwitch::NotReqProcMsg;
	cNotRequestProc.TimerPoll = (CSipTask::PFTimerPoll)&CPeInviteInterActionNatSwitch::WaitReqTimerPoll;
	cNotRequestProc.ProcErrMsg = (CSipTask::PFProcMsg)&CPeInviteInterActionNatSwitch::NotReqProcMsg;
	AddRuleProc(NOT_REQUEST_STATE, cNotRequestProc);

	CStateProc cWaitRspProc;
	cWaitRspProc.ProcMsg = (CSipTask::PFProcMsg)&CPeInviteInterActionNatSwitch::WaitRspProcMsg;
	cWaitRspProc.TimerPoll = (CSipTask::PFTimerPoll)&CPeInviteInterActionNatSwitch::WaitRspTimerPoll;
	cWaitRspProc.ProcErrMsg = (CSipTask::PFProcMsg)&CPeInviteInterActionNatSwitch::WaitRspProcMsg;
	AddRuleProc(WAIT_RESPONSE_STATE, cWaitRspProc);

	NextState(NOT_REQUEST_STATE);
}
// string CPeInviteInterActionNatSwitch::GetUserLogDesc()
// {
//     CCuAgent *pcCuAgent = (CCuAgent *)g_cCuiDataMgr.GetTaskPtrBySession(m_strCuSession, GetInstance()); 
//     if(pcCuAgent != NULL)
//     {
//         return string("(") + GetLanDesc(LAN_DESC_USERNAME)  + ":" + 
//             pcCuAgent->m_pcUserAccount->m_strUserName + ")";
//     } 
//     string strEmpty;
//     return strEmpty;
// }
// void CPeInviteInterActionNatSwitch::GetOptLogDesc(const string& oprType, const string& OptDevInfo, const u32 optType)
// {
//     m_optType = optType;
//     m_strLogDesc =  GetUserLogDesc() + GetLanDesc(oprType) + "(" + OptDevInfo + ")";
// }
void CPeInviteInterActionNatSwitch::WriteLog(u32 dwErrorCode, const u32 dwEvent)
{
	CGetPuData *pcTask = new CGetPuData(GetInstance(), m_strCuSession, m_tDevChn, dwErrorCode, m_strReq, dwEvent);
	pcTask->InitStateMachine();
	if (pcTask->StartWriteLog() != PROCMSG_OK)
	{
		delete pcTask;
		pcTask = NULL;
	}
}
// string CPeInviteInterActionNatSwitch::GetErrorCode(u32 dwErrorCode)
// {
//     if( CMS_SUCCESS == dwErrorCode)
//     {
//         return GetLanDesc(LAN_DESC_SUCCCESS);
//     }
//     else if( ERR_CUI_OPR_TIME_OUT == dwErrorCode)
//     {
//         return GetLanDesc(LAN_DESC_TIMEOUT);
//     }
//     else
//     {
//         return GetLanDesc(LAN_DESC_FAIL);
//     }
// }

u32 CPeInviteInterActionNatSwitch::NotReqProcMsg( CMessage *const pcMsg )
{
	COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
	TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户会话[%s]收到业务消息[%s--%d] (SIP消息[%s--%d])\n",
		m_strCuSession.c_str(),
		OspExtEventDesc(pcOspSipMsg->GetOspEventID()).c_str(), pcOspSipMsg->GetOspEventID(),
		pcOspSipMsg->GetSipEventStr().c_str(), pcOspSipMsg->GetSipEventID());

	u32 dwRet = PROCMSG_OK;

	m_dwEventID = pcMsg->event;
	switch (pcMsg->event)
	{
	case CU_CUI_INVITE_PLAY_PLATFORMRECORD_REQ:
		{
			CCuAgent *pcCuAgent = (CCuAgent *)g_cCuiDataMgr.GetTaskPtrBySession(m_strCuSession, GetInstance());
			CMS_ASSERT(NULL != pcCuAgent);
			if (NULL == pcCuAgent)
			{
				TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户[%s]Agent已不存在\n",m_strCuSession.c_str());

				CInviteRsp cRsp;
				cRsp.SetErrorCode(ERR_CUI_CU_NOT_ONLINE);
				SendRspToCu<CInviteRsp>(cRsp, m_ptRecvNode);
				dwRet =  PROCMSG_DEL;
				delete this;
				break;
			}

			CCuSetupPlayPlatformRecordReq *pcReq = (CCuSetupPlayPlatformRecordReq *)pcMsg->content;
			CInviteReq cInviteReq = pcReq->GetReq();
			cInviteReq.GetSdp().SetIsNeedPunch(pcCuAgent->m_bIsSupportNA);

			m_nplayId = cInviteReq.GetDstChn().GetChnNO();
			m_tDevChn = cInviteReq.GetSrcChn();
			m_strReq = cInviteReq.ToXml();

			//1 鉴权
			bool bAuthRet = true; //鉴权结果
			do 
			{                
				string strDomainName = GetDomainNameWithoutPreNameFromURI(cInviteReq.GetSrcChn().GetDevUri());
				if ( ! pcCuAgent->IsExistDomainInfo(strDomainName))
				{
					bAuthRet = false;
					break;
				}
				
				if (pcCuAgent->m_pcUserAccount->IsSuperAdmin())
				{
					bAuthRet = true;
					break;
				}

				//判断用户是否有平台放像的权限
				CPrivilege cPri = pcCuAgent->m_pcUserAccount->GetUserPri();
				if(!cPri.IsEmpty())
				{
					if(cPri.HasPri(CUserPrivilege::en_Vod_Play))
					{
						bAuthRet = true;
						break;
					}
					else
					{
						bAuthRet = false;
					}
				}
				else
				{
					bAuthRet = false;
					break;
				}

				string uuid = GetNameWithoutDomainFromURI(m_tDevChn.GetDevUri());
				string strErrMsg;
				if (!g_cRedisDb.IsUserHasVideoSource(pcCuAgent->m_pcUserAccount->m_tUserID, uuid, cInviteReq.GetSrcChn().GetChnNO(), strErrMsg))
				{
					TASKLOG(NORMAL_TASK, ERROR_LEV, "用户name[%s] id[%s]没有划归视频源[%s]权限，无法播放平台录像 errMsg[%s]\n",
						pcCuAgent->m_strUserName.c_str(), pcCuAgent->m_pcUserAccount->m_tUserID.c_str(), cInviteReq.GetSrcChn().ToXml().c_str(), strErrMsg.c_str());
					bAuthRet = false;
					break;
				}
			} while (0);

			if (!bAuthRet)
			{
				TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户[%s]会话[%s]发送Invite请求，但是对源通道[%s--%d]无权限\n",
					pcCuAgent->m_strUserName.c_str(), pcCuAgent->m_strSession.c_str(), cInviteReq.GetSrcChn().GetDevUri().c_str(), 
					cInviteReq.GetSrcChn().GetChnNO());

				CInviteRsp cRsp;
				cRsp.SetErrorCode(ERR_CUI_CU_NO_PERMISSION);
				SendRspToCu<CInviteRsp>(cRsp, m_ptRecvNode);
				dwRet =  PROCMSG_DEL;
				delete this;
				break;
			}

			//查找InviteTask
			CPeInviteTaskNatSwitch *pcInviteTask = (CPeInviteTaskNatSwitch *)GetInviteTask(m_nplayId);
			if ( NULL == pcInviteTask )
			{
				if(pcReq->GetDomainName().empty())
				{
					TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户[%s]会话[%s]发送Invite请求，但是DomainName为空\n",
						pcCuAgent->m_strUserName.c_str(), pcCuAgent->m_strSession.c_str());

					CInviteRsp cRsp;
					cRsp.SetErrorCode(ERR_CUI_INVALID_PARAM);
					SendRspToCu<CInviteRsp>(cRsp, m_ptRecvNode);
					delete this;
					return PROCMSG_DEL;
				}

                TSipURI tPeUri;
                CStreamDecentrationAdapter::FindRcsUri( tPeUri, pcReq->GetDomainName() );
				pcInviteTask = new CPeInviteTaskNatSwitch(GetInstance(), m_strCuSession, this, tPeUri.GetURIString() );//strPeUri
				pcInviteTask->InitStateMachine();
				//将当前任务登记到CuAgent中,由于不知道是否连上外设模块，暂时不能加入到外设模块的表中
				pcCuAgent->m_cPeInviteTaskSet.Insert(cInviteReq.GetDstChn().GetChnNO(), pcInviteTask->GetTaskNO());

				//将当前事务保存起来，NAT探测回调时需要更改探测包地址
				PushMediaTask(pcReq->GetSession(), m_nplayId, pcInviteTask);
			}

			if ( pcInviteTask->GetState() != CPeInviteTaskNatSwitch::NOT_REQUEST_STATE )
			{
				CInviteRsp cRsp;
				cRsp.SetSession(pcReq->GetSession());
				cRsp.SetSeqNum(pcReq->GetSeqNum());
				cRsp.SetErrorCode(ERR_CUI_DIALOG_NOT_ESTABLISHED);
				SendRspToCu<CInviteRsp>(cRsp, m_ptRecvNode);

				TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户[%s]会话[%s]已经发起过播放窗口[%d]的InviteReq，本次请求被拒绝\n",
					pcCuAgent->m_strUserName.c_str(), pcCuAgent->m_strSession.c_str(), cInviteReq.GetDstChn().GetChnNO());

				//任务完成, 销毁之
				NextState(NOT_REQUEST_STATE);

				dwRet =  PROCMSG_DEL;
				delete this;
			}
			else
			{
				//这块不会有删除task操作，所以不需要判断
                dwRet = pcInviteTask->StartSendInviteReq(cInviteReq);
                if( dwRet == PROCMSG_OK )
                {
                    NextState(WAIT_RESPONSE_STATE);
                }
                else
                {
                    CInviteRsp cRsp;
                    cRsp.SetSession(pcReq->GetSession());
                    cRsp.SetSeqNum(pcReq->GetSeqNum());
                    cRsp.SetErrorCode( pcInviteTask->GetErrorCode() );
                    SendRspToCu<CInviteRsp>(cRsp, m_ptRecvNode);

                    delete pcInviteTask;
                    pcInviteTask = NULL;

                    dwRet =  PROCMSG_DEL;
                    delete this;
                }
			}
		}
		break;
	case CU_CUI_INVITE_ACK_PLAY_PLATFORMRECORD_REQ:
		{
			CCuPlayPlatformRecordReq *pcReq = (CCuPlayPlatformRecordReq *)pcMsg->content;
			m_nplayId = pcReq->GetPlayId();

			TASKLOG(NORMAL_TASK, ERROR_LEV, "playID is %d\n",pcReq->GetPlayId());

			CPeInviteTaskNatSwitch *pcInviteTask = (CPeInviteTaskNatSwitch *)GetInviteTask(m_nplayId);
			CCuPlayPlatformRecordRsp cRsp;
			int nErrorCode = ERR_CUI_DIALOG_NOT_ESTABLISHED;
			if ( NULL == pcInviteTask )
			{ //事务尚未建立
				TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户会话[%s]的Invite没有对应Task\n",
					m_strCuSession.c_str());
				//cRsp.SetErrorCode(nErrorCode);
			}
			else
			{
				if ( pcInviteTask->GetState() == CInviteTask::WAIT_SETUP_ACK_STATE )
				{
					//这块不会有删除task操作，所以不需要判断
					pcInviteTask->StartSendInviteAck();
					pcInviteTask->m_pcInviteInterActTask = NULL;
					nErrorCode = CMS_SUCCESS;
				}
				//pcInviteTask->m_pcInviteInterActTask = NULL;
			}

			TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户会话[%s]发送的InviteAck没有应答消息，CUI直接向CU回复应答\n",
				m_strCuSession.c_str());
			//ACK消息没有应答，直接回复
			cRsp.SetErrorCode(nErrorCode);
			SendRspToCu<CCuPlayPlatformRecordRsp>(cRsp, m_ptRecvNode);
			m_nplayId = INVALID_PLAY_ID;
			dwRet = PROCMSG_DEL;
			delete this;
		}
		break;
	case RCS_TASK_START_REQ:
		{
			CCuTaskStartReq *pcReq = (CCuTaskStartReq *)pcMsg->content;
			m_nplayId = pcReq->GetPlayId();
			TASKLOG(NORMAL_TASK, ERROR_LEV, "playID is %d\n",pcReq->GetPlayId());
			CPeInviteTaskNatSwitch *pcInviteTask = (CPeInviteTaskNatSwitch *)GetInviteTask(m_nplayId);
			CCuPlayPlatformRecordRsp cRsp;
			int nErrorCode = ERR_CUI_DIALOG_NOT_ESTABLISHED;
			if ( NULL == pcInviteTask )
			{ 
				TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户会话[%s]的Invite没有对应Task\n",
					m_strCuSession.c_str());
				cRsp.SetErrorCode(nErrorCode);
				SendRspToCu<CCuPlayPlatformRecordRsp>(cRsp, m_ptRecvNode);
				m_nplayId = INVALID_PLAY_ID;
				dwRet = PROCMSG_DEL;
				delete this;
			}
			else
			{
				if ( pcInviteTask->GetState() == CInviteTask::INVITE_HOLD_STATE )
				{
					pcInviteTask->StartTask();
					pcInviteTask->m_pcInviteInterActTask = this;
					nErrorCode = CMS_SUCCESS;
					NextState(WAIT_RESPONSE_STATE);
				}
				else
				{
					TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户会话[%s]的Invite Task不在正确的状态，当前状态[]\n",
						m_strCuSession.c_str());
					cRsp.SetErrorCode(ERR_CUI_STATE_ERROR);
					SendRspToCu<CCuPlayPlatformRecordRsp>(cRsp, m_ptRecvNode);
					m_nplayId = INVALID_PLAY_ID;
					dwRet = PROCMSG_DEL;
					delete this;
				}
			}
		}
		break;
	case CU_CUI_INVITE_BYE_PLAY_PLATFORMRECORD_REQ:
		{
			CCuStopPlatformRecordReq *pcReq = (CCuStopPlatformRecordReq *)pcMsg->content;
			m_nplayId = pcReq->GetPlayId();
			m_strReq = pcReq->ToXml();

			CPeInviteTaskNatSwitch *pcInviteTask = (CPeInviteTaskNatSwitch *)GetInviteTask(m_nplayId);

			if ( NULL != pcInviteTask )
			{
				if (pcInviteTask->TryBye(this))
				{
					m_tDevChn = pcInviteTask->m_tReq.GetSrcChn();
					NextState(WAIT_RESPONSE_STATE);
				}
				else
				{
					CCuStopPlatformRecordRsp cRsp;
					TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户会话[%s]的Invite Task Trybye failed\n", m_strCuSession.c_str());
					cRsp.SetErrorCode(ERR_CUI_DIALOG_NOT_ESTABLISHED);
					SendRspToCu<CCuStopPlatformRecordRsp>(cRsp, m_ptRecvNode);
					WriteLog(ERR_CUI_DIALOG_NOT_ESTABLISHED, m_dwEventID);
					m_nplayId = INVALID_PLAY_ID;
					dwRet = PROCMSG_DEL;
					delete this;
				}
			}
			else
			{
				CCuStopPlatformRecordRsp cRsp;
				TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户会话[%s]的Invite没有对应Task\n",
					m_strCuSession.c_str());
				cRsp.SetErrorCode(ERR_CUI_DIALOG_NOT_ESTABLISHED);
				TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户会话[%s]发送的InviteBye没有应答消息，CUI直接向CU回复应答\n",
					m_strCuSession.c_str());
				//BYE消息没有应答，直接回复
				SendRspToCu<CCuStopPlatformRecordRsp>(cRsp, m_ptRecvNode);
				WriteLog(ERR_CUI_DIALOG_NOT_ESTABLISHED, m_dwEventID);
				m_nplayId = INVALID_PLAY_ID;
				dwRet = PROCMSG_DEL;
				delete this;
			}            
		}
		break;
	case CU_CUI_INVITE_PLAY_DOWNLOADPLATFORMRECORD_REQ:
		{ 
			CCuAgent *pcCuAgent = (CCuAgent *)g_cCuiDataMgr.GetTaskPtrBySession(m_strCuSession, GetInstance());
			CMS_ASSERT(NULL != pcCuAgent);
			if (NULL == pcCuAgent)
			{
				TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户[%s]Agent已不存在\n",m_strCuSession.c_str());

				CInviteRsp cRsp;
				cRsp.SetErrorCode(ERR_CUI_CU_NOT_ONLINE);
				SendRspToCu<CInviteRsp>(cRsp, m_ptRecvNode);
				dwRet =  PROCMSG_DEL;
				delete this;
				break;
			}

			CCuDownloadPlatformRecordReq *pcReq = (CCuDownloadPlatformRecordReq *)pcMsg->content;
			CInviteReq cInviteReq = pcReq->GetReq();
			cInviteReq.GetSdp().SetIsNeedPunch(pcCuAgent->m_bIsSupportNA);
			m_nplayId = cInviteReq.GetDstChn().GetChnNO();
			m_tDevChn = cInviteReq.GetSrcChn();
			m_strReq = cInviteReq.ToXml();

			bool bAuthRet = true; //鉴权结果
			do 
			{                
				string strDomainName = GetDomainNameWithoutPreNameFromURI(cInviteReq.GetSrcChn().GetDevUri());
				if ( ! pcCuAgent->IsExistDomainInfo(strDomainName))
				{
					bAuthRet = false;
					break;
				}

				if (pcCuAgent->m_pcUserAccount->IsSuperAdmin())
				{
					bAuthRet = true;
					break;
				}

				CPrivilege cPri = pcCuAgent->m_pcUserAccount->GetUserPri();
				if(!cPri.IsEmpty())
				{
					if(cPri.HasPri(CUserPrivilege::en_Vod_Download))
					{
						bAuthRet = true;
						break;
					}
					else
					{
						bAuthRet = false;
					}
				}
				else
				{
					bAuthRet = true;
					break;
				}
			}while(0);

			if (!bAuthRet)
			{
				TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户[%s]会话[%s]发送Invite请求，但是对源通道[%s--%d]无权限\n",
					pcCuAgent->m_strUserName.c_str(), pcCuAgent->m_strSession.c_str(), cInviteReq.GetSrcChn().GetDevUri().c_str(), 
					cInviteReq.GetSrcChn().GetChnNO());

				CInviteRsp cRsp;
				cRsp.SetErrorCode(ERR_CUI_CU_NO_PERMISSION);
				SendRspToCu<CInviteRsp>(cRsp, m_ptRecvNode);
				dwRet =  PROCMSG_DEL;
				delete this;
				break;
			}

			//查找InviteTask
			CPeInviteTaskNatSwitch *pcInviteTask = (CPeInviteTaskNatSwitch *)GetInviteTask(m_nplayId);
			if ( NULL == pcInviteTask )
			{
                TSipURI tPeUri;
                CStreamDecentrationAdapter::FindRcsUri( tPeUri, pcReq->GetDomainName() );

				pcInviteTask = new CPeInviteTaskNatSwitch(GetInstance(), m_strCuSession, this, tPeUri.GetURIString() );
				pcInviteTask->InitStateMachine();
				//将当前任务登记到CuAgent中
				pcCuAgent->m_cPeInviteTaskSet.Insert(cInviteReq.GetDstChn().GetChnNO(), pcInviteTask->GetTaskNO());

				//将当前事务保存起来，NAT探测回调时需要更改探测包地址
				PushMediaTask(pcCuAgent->m_strSession, m_nplayId, pcInviteTask);
			}

			if ( pcInviteTask->GetState() != CPeInviteTaskNatSwitch::NOT_REQUEST_STATE )
			{
				CInviteRsp cRsp;
				cRsp.SetSession(pcReq->GetSession());
				cRsp.SetSeqNum(pcReq->GetSeqNum());
				cRsp.SetErrorCode(ERR_CUI_DIALOG_NOT_ESTABLISHED);
				SendRspToCu<CInviteRsp>(cRsp, m_ptRecvNode);

				TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户[%s]会话[%s]已经发起过播放窗口[%d]的InviteReq，本次请求被拒绝\n",
					pcCuAgent->m_strUserName.c_str(), pcCuAgent->m_strSession.c_str(), cInviteReq.GetDstChn().GetChnNO());

				//任务完成, 销毁之
				NextState(NOT_REQUEST_STATE);

				dwRet =  PROCMSG_DEL;
				delete this;
			}
			else
			{
                //这块不会有删除task操作，所以不需要判断
                dwRet = pcInviteTask->StartSendInviteReq(cInviteReq);
                if( dwRet == PROCMSG_OK )
                {
                    NextState(WAIT_RESPONSE_STATE);
                }
                else
                {
                    CInviteRsp cRsp;
                    cRsp.SetSession(pcReq->GetSession());
                    cRsp.SetSeqNum(pcReq->GetSeqNum());
                    cRsp.SetErrorCode( pcInviteTask->GetErrorCode() );
                    SendRspToCu<CInviteRsp>(cRsp, m_ptRecvNode);

                    delete pcInviteTask;
                    pcInviteTask = NULL;

                    dwRet =  PROCMSG_DEL;
                    delete this;
                }
			}
		}
		break;
	case CU_CUI_INVITE_ACK_DOWNLOADPLATFORMRECORD_REQ:
		{            
			CCuStartDownloadPlatformRecordReq *pcReq = (CCuStartDownloadPlatformRecordReq *)pcMsg->content;
			m_nplayId = pcReq->GetPlayId();

			TASKLOG(NORMAL_TASK, ERROR_LEV, "playID is %d\n",pcReq->GetPlayId());

			CPeInviteTaskNatSwitch *pcInviteTask = (CPeInviteTaskNatSwitch *)GetInviteTask(m_nplayId);
			CCuStartDownloadPlatformRecordRsp cRsp;
			int nErrorCode = ERR_CUI_DIALOG_NOT_ESTABLISHED;
			if ( NULL == pcInviteTask )
			{ //事务尚未建立
				TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户会话[%s]的Invite没有对应Task\n",
					m_strCuSession.c_str());
				cRsp.SetErrorCode(nErrorCode);
			}
			else
			{
				if ( pcInviteTask->GetState() == CInviteTask::WAIT_SETUP_ACK_STATE )
				{
					//这块不会有删除task操作，所以不需要判断
					pcInviteTask->StartSendInviteAck();
					pcInviteTask->m_pcInviteInterActTask = NULL;
					nErrorCode = CMS_SUCCESS;
				}
				//pcInviteTask->m_pcInviteInterActTask = NULL;
			}

			TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户会话[%s]发送的InviteAck没有应答消息，CUI直接向CU回复应答\n",
				m_strCuSession.c_str());
			//ACK消息没有应答，直接回复
			cRsp.SetErrorCode(nErrorCode);
			SendRspToCu<CCuStartDownloadPlatformRecordRsp>(cRsp, m_ptRecvNode);
			m_nplayId = INVALID_PLAY_ID;
			dwRet = PROCMSG_DEL;
			delete this;
		}
		break;
	case CU_CUI_INVITE_BYE_DOWNLOADPLATFORMRECORD_REQ:
		{
			CCuStopDownloadPlatformRecordReq *pcReq = (CCuStopDownloadPlatformRecordReq *)pcMsg->content;
			m_nplayId = pcReq->GetPlayId();
			m_strReq = pcReq->ToXml();

			CPeInviteTaskNatSwitch *pcInviteTask = (CPeInviteTaskNatSwitch *)GetInviteTask(m_nplayId);
			if ( NULL != pcInviteTask )
			{
				if (pcInviteTask->TryBye(this))
				{
					m_tDevChn = pcInviteTask->m_tReq.GetSrcChn();
					NextState(WAIT_RESPONSE_STATE);
				}
				else
				{
					CCuStopDownloadPlatformRecordRsp cRsp;
					TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户会话[%s]的Invite Task TryBye failed\n", m_strCuSession.c_str());
					cRsp.SetErrorCode(ERR_CUI_DIALOG_NOT_ESTABLISHED);
					SendRspToCu<CCuStopDownloadPlatformRecordRsp>(cRsp, m_ptRecvNode);
					WriteLog(ERR_CUI_DIALOG_NOT_ESTABLISHED, m_dwEventID);
					m_nplayId = INVALID_PLAY_ID;
					dwRet = PROCMSG_DEL;
					delete this;
				}
			}
			else
			{
				CCuStopDownloadPlatformRecordRsp cRsp;
				TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户会话[%s]的Invite没有对应Task\n",
					m_strCuSession.c_str());
				cRsp.SetErrorCode(ERR_CUI_DIALOG_NOT_ESTABLISHED);
				TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户会话[%s]发送的InviteBye没有应答消息，CUI直接向CU回复应答\n",
					m_strCuSession.c_str());
				//BYE消息没有应答，直接回复
				SendRspToCu<CCuStopDownloadPlatformRecordRsp>(cRsp, m_ptRecvNode);
				WriteLog(ERR_CUI_DIALOG_NOT_ESTABLISHED, m_dwEventID);
				m_nplayId = INVALID_PLAY_ID;
				dwRet = PROCMSG_DEL;
				delete this;
			} 
		}
		break;
	case VCR_CTRL_REQ:
		{
			CCuVcrCtrlReq *pcReq = (CCuVcrCtrlReq *)pcMsg->content;
			m_nplayId = pcReq->GetPlayId();
			m_dwSeqNum = pcReq->GetSeqNum();

			CPeInviteTaskNatSwitch *pcInviteTask = (CPeInviteTaskNatSwitch *)GetInviteTask(m_nplayId);
			if ( NULL != pcInviteTask )
			{
				pcInviteTask->AddChildTask(pcReq->GetSeqNum(), GetTaskNO());
				//pcInviteTask->m_pcInviteInterActTask = this;
				pcInviteTask->StartVcrCtrl(*pcReq);
				NextState(WAIT_RESPONSE_STATE);
			}
			else
			{
				CCuVcrCtrlRsp cRsp;
				TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户会话[%s]的Invite没有对应Task\n",
					m_strCuSession.c_str());
				cRsp.SetErrorCode(ERR_CUI_DIALOG_NOT_ESTABLISHED);

				SendRspToCu<CCuVcrCtrlRsp>(cRsp, m_ptRecvNode);
				m_nplayId = INVALID_PLAY_ID;
				delete this;
			}  
		}
		break;
	default:
		{
			TASKLOG(NORMAL_TASK, CRITICAL_LEV, "收到未知消息[%s--%hu]\n",
				OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
			CMS_ASSERT(0&&"收到未知消息");
		}
		break;
	}
	return dwRet;
}

u32 CPeInviteInterActionNatSwitch::WaitRspProcMsg( CMessage *const pcMsg )
{
	COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
	TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户会话[%s]收到业务应答消息[%s--%d] (SIP应答消息[%s--%d])\n",
		m_strCuSession.c_str(),
		OspExtEventDesc(pcOspSipMsg->GetOspEventID()).c_str(), pcOspSipMsg->GetOspEventID(),
		pcOspSipMsg->GetSipEventStr().c_str(), pcOspSipMsg->GetSipEventID());

	u32 dwRet = PROCMSG_OK;

	switch (pcMsg->event)
	{
	case OSP_SIP_MSG_PROC_FAIL:
	case OSP_SIP_DIALOG_TERMINATE:
		{
			PostErrMsg2Cu(ERR_CUI_SIP_MSG_FAIL);
			WriteLog(ERR_CUI_SIP_MSG_FAIL, m_dwEventID);
			TASKLOG(NORMAL_TASK, ERROR_LEV, "向CU用户会话[%s]发送失败应答消息，错误码[ERR_CUI_SIP_MSG_FAIL]\n",
				m_strCuSession.c_str());
			//任务完成, 销毁之
			NextState(NOT_REQUEST_STATE);
			dwRet =  PROCMSG_DEL;
		}
		break;
	case INVITE_RSP:
		{
			CInviteRsp cRsp;
			pcOspSipMsg->GetMsgBody(cRsp);
			if ( INVALID_DIALOG_ID == pcOspSipMsg->GetSipDlgID() )
			{
				cRsp.SetErrorCode(ERR_CUI_DIALOG_NOT_ESTABLISHED);
			}

			SendRspToCu<CInviteRsp>(cRsp, m_ptRecvNode);
			WriteLog((u32)cRsp.GetErrorCode(), m_dwEventID);
			//任务完成, 销毁之
			NextState(NOT_REQUEST_STATE);
			dwRet = PROCMSG_DEL;
		}
		break;
	case OSP_SIP_BYE_REQ:
		{ //服务器端主动结束会话
			if(pcOspSipMsg->GetMsgBodyLen() > 0)
			{
				CByeReq cByeReq;
				pcOspSipMsg->GetMsgBody(cByeReq);

				PostErrMsg2Cu((u32)cByeReq.GetReason());
				WriteLog(cByeReq.GetReason(), m_dwEventID);
			}
			else
			{
				PostErrMsg2Cu(ERR_CUI_DIALOG_TERMINATED);
				WriteLog(ERR_CUI_DIALOG_TERMINATED, m_dwEventID);
			}        


			//任务完成, 销毁之
			NextState(NOT_REQUEST_STATE);
			dwRet = PROCMSG_DEL;
		}
		break;
	case OSP_SIP_BYE_RSP:
		{
			CCuStopInviteRsp cRsp;
			WriteLog((u32)cRsp.GetErrorCode(), m_dwEventID);
			SendRspToCu<CCuStopInviteRsp>(cRsp, m_ptRecvNode);

			dwRet = PROCMSG_DEL;
		}
		break;
	case PLAYBACK_PROGRESS_QUERY_RSP:
		{
			CCuPlaybackProgressQueryRsp cRsp;
			pcOspSipMsg->GetMsgBody(cRsp);

			SendRspToCu<CCuPlaybackProgressQueryRsp>(cRsp, m_ptRecvNode);
			//任务完成, 销毁之
			NextState(NOT_REQUEST_STATE);
			delete this;
		}
		break;
	case VCR_CTRL_RSP:
		{
			CCuVcrCtrlRsp cRsp;
			pcOspSipMsg->GetMsgBody(cRsp);
			SendRspToCu<CCuVcrCtrlRsp>(cRsp, m_ptRecvNode);
			NextState(NOT_REQUEST_STATE);
			delete this;
		}
		break;
	case RCS_TASK_START_RSP:
		{
			CRcsTaskStartRsp cRcsRsp;
			pcOspSipMsg->GetMsgBody(cRcsRsp);
			CCuTaskStartRsp cCuRsp;
			cCuRsp.SetErrorCode(cRcsRsp.GetErrorCode());
			SendRspToCu<CCuTaskStartRsp>(cCuRsp, m_ptRecvNode);
			NextState(NOT_REQUEST_STATE);
			delete this;
		}
		break;
	default:
		{
			TASKLOG(NORMAL_TASK, CRITICAL_LEV, "收到未知消息[%s--%hu]\n",
				OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
			CMS_ASSERT(0&&"收到未知消息");
		}
		break;
	}
	return dwRet;
}

u32 CPeInviteInterActionNatSwitch::WaitReqTimerPoll()
{
	if ( GetCurStateHoldTime() > g_cCuiConfig.GetSipWaitRspTimeout() )
	{
		TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户会话[%s]等待Invite应答超时\n",
			m_strCuSession.c_str());

		PostErrMsg2Cu(ERR_CUI_OPR_TIME_OUT);
		WriteLog(ERR_CUI_OPR_TIME_OUT, m_dwEventID);

		NextState(NOT_REQUEST_STATE);
		return TIMERPOLL_DEL;
	}
	return TIMERPOLL_DONE;
}

u32 CPeInviteInterActionNatSwitch::WaitRspTimerPoll()
{
	if ( GetCurStateHoldTime() > g_cCuiConfig.GetSipWaitRspTimeout() )
	{
		TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户会话[%s]等待Invite应答超时\n",
			m_strCuSession.c_str());

		PostErrMsg2Cu(ERR_CUI_OPR_TIME_OUT);
		WriteLog(ERR_CUI_OPR_TIME_OUT, m_dwEventID);

		NextState(NOT_REQUEST_STATE);
		return TIMERPOLL_DEL;
	}
	return TIMERPOLL_DONE;
}

CTask *CPeInviteInterActionNatSwitch::GetInviteTask(s32 nPlayId)
{
	CCuAgent *pcCuAgent = (CCuAgent *)g_cCuiDataMgr.GetTaskPtrBySession(m_strCuSession, GetInstance());
	//由于用户下线不删除 PeInviteInterAciton，只删除CPeInvite，当用户先退出后，CPeInviteInterAction超时，导致找不到用户task，产生该断言
	//CMS_ASSERT(NULL != pcCuAgent);
	u32 dwTaskId = INVALID_TASKNO;
	if(pcCuAgent != NULL)
	{
		if ( ! pcCuAgent->m_cPeInviteTaskSet.Find(m_nplayId, dwTaskId) )
		{
			return NULL;
		}

		if ( INVALID_TASKNO == dwTaskId )
		{
			return NULL;
		}

		return GetInstance()->FindTask(dwTaskId);
	}

	return NULL;
}

void CPeInviteInterActionNatSwitch::PostErrMsg2Cu(u32 dwErrorCode)
{
	switch(m_dwEventID)
	{
	case CU_CUI_INVITE_PLAY_PLATFORMRECORD_REQ:
		{
			SendErrorMsg2Cu<CCuSetupPlayPlatformRecordRsp>(dwErrorCode);
		}
		break;
	case CU_CUI_INVITE_ACK_PLAY_PLATFORMRECORD_REQ:
		{
			SendErrorMsg2Cu<CCuPlayPlatformRecordRsp>(dwErrorCode);
		}
		break;
	case CU_CUI_INVITE_BYE_PLAY_PLATFORMRECORD_REQ:
		{
			SendErrorMsg2Cu<CCuStopPlatformRecordRsp>(dwErrorCode);
		}
		break;
	case CU_CUI_INVITE_PLAY_DOWNLOADPLATFORMRECORD_REQ:
		{            
			SendErrorMsg2Cu<CCuDownloadPlatformRecordRsp>(dwErrorCode);
		}
		break;
	case CU_CUI_INVITE_ACK_DOWNLOADPLATFORMRECORD_REQ:
		{            
			SendErrorMsg2Cu<CCuStartDownloadPlatformRecordRsp>(dwErrorCode);
		}
		break;
	case CU_CUI_INVITE_BYE_DOWNLOADPLATFORMRECORD_REQ:
		{
			SendErrorMsg2Cu<CCuStopDownloadPlatformRecordRsp>(dwErrorCode);
		}
		break;
	case PLAYBACK_PROGRESS_QUERY_REQ:
		{
			SendErrorMsg2Cu<CCuPlaybackProgressQueryRsp>(dwErrorCode);
		}
		break;
	case  VCR_CTRL_REQ:
		{
			SendErrorMsg2Cu<CCuVcrCtrlRsp>(dwErrorCode);
		}
		break;
	case RCS_TASK_START_REQ:
		{
			SendErrorMsg2Cu<CCuTaskStartRsp>(dwErrorCode);
		}
		break;
	default:
		{
			SendErrorMsg2Cu<CEventRsp>(dwErrorCode);
		}
		break;
	}
}
