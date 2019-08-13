
#include "cms/cms_errorcode.h"

#include "cbb/dataswitch/datadispatch.h"
#include "cmustubtestdata.h"
#include "cmustubtestconst.h"
#include "culogintasktest.h"
#include "cmustubtestcoreapp.h"
#include "invitetasktest.h"
#include "cmustubtestconf.h"
#include "cmustubtestdbg.h"

extern u64 g_TraceAddrID;
extern u32 g_dwDumpLogLevel;
extern CXSet<u16> g_cDumpPortSet;

//ֻ�Ǵ�ӡͳ�ƣ��������̰߳�ȫ
CDumpStatMap g_cDumpStatMap;

u32 GetBitField(u32 dwValue, s32 nStartBit, s32 nBits)
{
    s32  nMask = (1 << nBits) - 1;
    return ((u32)(dwValue >> nStartBit) & nMask); 
}

BOOL32 GetRtpInfo(const void* pBuf, s32 nSize, TRtpInfo* pInfo)
{
    //���ж�ͷ�ӽڵ������λ�Ƿ�ΪRTP�İ汾��
    if ( ((*(u8*)pBuf)&0xC0) != 0x80 )
    {
        return FALSE;
    }

    if ( nSize < 12 )
    { //Ϊ�ﵽRTP��ͷ����
        return FALSE;
    }

    //copy head to local buffer
    u32 Tmp = ntohl(((u32*)pBuf)[0]);

    //��֤�Ƿ�RTP����������Ƶ����Ƶ
    u8 byPayload   = (u8)GetBitField(Tmp, 16, 8);
    if ( byPayload >= 200 && byPayload <= 204 )
    {
        return FALSE;
    }

    //get seq
    pInfo->m_wSeqNO = (u16)GetBitField(Tmp, 0, 16);

    Tmp = ntohl(((u32*)pBuf)[2]);

    //get ssrc
    pInfo->m_dwSSRC = Tmp;
    return TRUE;
}

#define CMU_REPEAT_RTP_PACK_MAX_DIFF 1200
bool IsRepeatPack(u16 wExpectedSeqNO, u16 wRecvSeqNO)
{
    if ( wExpectedSeqNO == wRecvSeqNO )
    {
        return false;
    }

    u16 wDiff = wExpectedSeqNO - wRecvSeqNO;
    if ( wDiff <= CMU_REPEAT_RTP_PACK_MAX_DIFF )
    {
        return true;
    }
    return false;
}

bool IsLostPack(u16 wExpectedSeqNO, u16 wRecvSeqNO)
{
    if ( wExpectedSeqNO == wRecvSeqNO )
    {
        return false;
    }

    return ( ! IsRepeatPack(wExpectedSeqNO, wRecvSeqNO) );
}

char* IPStr(u32 dwIP, char szIpStr[], u32 dwLen)
{
    memset(szIpStr, 0, dwLen);

    struct in_addr tIn;
    tIn.s_addr = dwIP;
    strcpy(szIpStr, inet_ntoa(tIn));
    return szIpStr;
}

bool IsNeedPrint(u16 wPort)
{
    return (g_cDumpPortSet.IsEmpty() || g_cDumpPortSet.Exist(wPort));
}

u64 GetTimeMsByTicks()
{
    u64 ddwTicks = OspTickGet64();
    u64 ddwTickRate = 1000 / OspClkRateGet();    //ÿms����ticks��һ��win��1����linux��10��

    u64 ddwTicksTimeStamp = ddwTicks * ddwTickRate;
    return ddwTicksTimeStamp;
}

u32 CBDsFilterDumpStat(u32 dwRecvIP, u16 wRecvPort, u32 dwSrcIP, u16 wSrcPort, u8* pData, u32 uLen)
{
    char szSrcIP[64] = {0};
    IPStr(dwSrcIP, szSrcIP, sizeof(szSrcIP));

    char szDstIP[64] = {0};
    IPStr(dwRecvIP, szDstIP, sizeof(szDstIP));

    u64 ddwAddrID = MakeAddrID(dwRecvIP, wRecvPort);
    TDumpStat* ptDumpStat = g_cDumpStatMap.Find(ddwAddrID);
    if (ptDumpStat == NULL)
    {
        dumplog(DumpLog_Error, "PackLen[%u]NOT EXIST dump, [%s:%d]->[%s:%d]\n",
            uLen, szSrcIP, wSrcPort, szDstIP, wRecvPort);
        return 0;
    }
      
    BOOL32 bIsNeedProc = TRUE;  //�Ƿ�Ŀ��RTP��

    TRtpInfo tRtpInfo;
    bIsNeedProc = GetRtpInfo(pData, uLen, &tRtpInfo);
    u16 wRecvSeqNO = tRtpInfo.m_wSeqNO;

    if ( bIsNeedProc )
    {
        if (IsNeedPrint(wRecvPort))
        {
            dumplog(DumpLog_Info, "[RTP] SeqNO %lu, SSRC 0x%08x, PackLen %u, [%s:%d]->[%s:%d]\n",
                tRtpInfo.m_wSeqNO, tRtpInfo.m_dwSSRC, uLen, 
                szSrcIP, wSrcPort, szDstIP, wRecvPort);
        }     
    }
    else
    {
        dumplog(DumpLog_Error, "PackLen %u is NOT RTP, [%s:%d]->[%s:%d]\n",
            uLen, szSrcIP, wSrcPort, szDstIP, wRecvPort);
        return 0;
    }

    ptDumpStat->m_dwTotal++;

    if ( ptDumpStat->m_bIsFirstRun )
    {
        ptDumpStat->m_dwSSRC = tRtpInfo.m_dwSSRC;
        ptDumpStat->m_wExpectedSeqNO = wRecvSeqNO;
        ptDumpStat->m_tSrcAddr.SetNetIpv4(dwSrcIP);
        ptDumpStat->m_tSrcAddr.SetNetPort(wSrcPort);
        ptDumpStat->m_ddwProcDataTime = GetTimeMsByTicks();
        ptDumpStat->m_bIsFirstRun = FALSE;
    }
    else
    {
        if ( ptDumpStat->m_dwSSRC != tRtpInfo.m_dwSSRC )
        {
            //���ܴ�����
            dumplog(DumpLog_Error, "[Proc RTP] RTP pack SSRC 0x%08x is DIFF from last 0x%08x, [%s:%d]->[%s:%d]\n", 
                tRtpInfo.m_dwSSRC, ptDumpStat->m_dwSSRC,
                szSrcIP, wSrcPort, szDstIP, wRecvPort);
            ptDumpStat->m_dwSSRC = tRtpInfo.m_dwSSRC;
        }
    }

    //���ж��Ƿ��ش���
    if ( IsRepeatPack(ptDumpStat->m_wExpectedSeqNO, wRecvSeqNO) )
    {
        ptDumpStat->m_dwRTPRepeat++;
        if (IsNeedPrint(wRecvPort))
        {
            dumplog(DumpLog_Warning, "[Proc RTP] RTP RETRANSMIT pack [(ExpectedSeq)%05hu, (RecvSeq)%05hu], [%s:%d]->[%s:%d]\n", 
                ptDumpStat->m_wExpectedSeqNO, wRecvSeqNO, szSrcIP, wSrcPort, szDstIP, wRecvPort);    
        }     
    }
    else
    {
        if ( IsLostPack(ptDumpStat->m_wExpectedSeqNO, wRecvSeqNO) )
        {
            ptDumpStat->m_dwRTPLost += wRecvSeqNO - ptDumpStat->m_wExpectedSeqNO;

            if (IsNeedPrint(wRecvPort))
            {             
                dumplog(DumpLog_Error, "[Proc RTP] RTP LOST pack [(ExpectedSeq)%05hu, (RecvSeq)%05hu] [%s:%d]->[%s:%d]\n", 
                    ptDumpStat->m_wExpectedSeqNO, wRecvSeqNO, szSrcIP, wSrcPort, szDstIP, wRecvPort);
            }    
        }

        //ֻҪ������������ģ����ش������ش�����������ExpectedSeqNO
        ptDumpStat->m_wExpectedSeqNO = wRecvSeqNO + 1;      
    }

    return 1;
}

void CInviteTask::InitStateMachine()
{
    CStateProc cReqProc;
    cReqProc.ProcMsg = (CTask::PFProcMsg)&CInviteTask::OnReq;
    cReqProc.ProcErrMsg = (CTask::PFProcMsg)&CInviteTask::OnReq;
    cReqProc.TimerPoll = (CTask::PFTimerPoll)&CInviteTask::OnReqTimer;
    AddRuleProc(Req, cReqProc);

    CStateProc cQryAddrProc;
    cQryAddrProc.ProcMsg = (CTask::PFProcMsg)&CInviteTask::OnQryAddr;
    cQryAddrProc.ProcErrMsg = (CTask::PFProcMsg)&CInviteTask::OnQryAddr;
    cQryAddrProc.TimerPoll = (CTask::PFTimerPoll)&CInviteTask::OnQryAddrTimer;
    AddRuleProc(QryAddr, cQryAddrProc);

    CStateProc cWaitRspProc;
    cWaitRspProc.ProcMsg = (CTask::PFProcMsg)&CInviteTask::OnWaitRsp;
    cWaitRspProc.ProcErrMsg = (CTask::PFProcMsg)&CInviteTask::OnWaitRsp;
    cWaitRspProc.TimerPoll = (CTask::PFTimerPoll)&CInviteTask::OnWaitRspTimer;
    AddRuleProc(WaitRsp, cWaitRspProc);

    CStateProc cHoldProc;
    cHoldProc.ProcMsg = (CTask::PFProcMsg)&CInviteTask::OnService;
    cHoldProc.ProcErrMsg = (CTask::PFProcMsg)&CInviteTask::OnService;
    cHoldProc.TimerPoll = (CTask::PFTimerPoll)&CInviteTask::OnServiceTimer;
    AddRuleProc(Service, cHoldProc);

    CStateProc cWaitByeRspProc;
    cWaitByeRspProc.ProcMsg = (CTask::PFProcMsg)&CInviteTask::OnWaitByeRsp;
    cWaitByeRspProc.ProcErrMsg = (CTask::PFProcMsg)&CInviteTask::OnWaitByeRsp;
    cWaitByeRspProc.TimerPoll = (CTask::PFTimerPoll)&CInviteTask::OnWaitByeRspTimer;
    AddRuleProc(WaitByeRsp, cWaitByeRspProc);

    NextState(Req);
}


CInviteTask::~CInviteTask()
{
    DelDsDump();

    CLoginSession* ptTask = NULL;
    g_cCuList.Find(m_tReq.GetSession(), ptTask);
    if (ptTask != NULL)
    {
        ptTask->DelTask(this);
    }

    CPuData* ptPuData = g_cPuList.FindPuData(m_tReq.GetSrcChn().GetDevUri());
    if (ptPuData != NULL)
    {
        ptPuData->SubInvitedNum();
    }

    if (m_tSipDlgId != INVALID_DIALOG_ID)
    {
        OspSipReleaseDlg(m_tSipDlgId);
        m_tSipDlgId = INVALID_DIALOG_ID;
    }
}

u32 CInviteTask::OnReq(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_OK;

    TInviteReqMsg* ptInviteReqMsg = (TInviteReqMsg*)pcMsg->content;
    s32 nPuChn = ptInviteReqMsg->m_nSrcChn;
    s32 nCuChn = ptInviteReqMsg->m_nDstChn;

    m_tNetAddr.SetNetIp(g_cCmuConf.m_tLocalAddr.tIpAddr);
    m_tNetAddr.SetNetPort(LOCAL_INVITE_START_PORT+nCuChn*4);

	//�豸Uri
    string strDevUri;
    if (strlen(ptInviteReqMsg->m_szSrcUri) == 0)
    {
        strDevUri = TEST_PU_URI;
    }
    else
    {
        strDevUri = ptInviteReqMsg->m_szSrcUri;
    }

	//�û�Session
    CCuLoginTask* ptCuLoginTask = NULL;

    Iterator pPos;
    string strSession;
    CLoginSession* pTask = NULL;
    while (!pPos.End())
    {
        if (g_cCuList.Iterate(pPos, strSession, pTask) && pTask != NULL)
        {
            ptCuLoginTask = (CCuLoginTask*)(pTask);
            break;
        }
    }

    if (ptCuLoginTask == NULL)
    {
        TASKLOG(ERROR_LEV, "û��cu�û���¼\n");
        dwRet = PROCMSG_DEL;
        return dwRet;
    }

	//�豸��Ϣ
    CPuData* ptPuData = g_cPuList.FindPuData(strDevUri);
    if (ptPuData == NULL)
    {
        TASKLOG(ERROR_LEV, "�豸[%s]������\n",
            strDevUri.c_str());
        dwRet = PROCMSG_DEL;
        return dwRet;
    }

    //�Ƿ�����
    if (!ptPuData->GetPuStatus().GetIsOnline())
    {
        TASKLOG(ERROR_LEV, "�豸[%s]������\n",
            strDevUri.c_str());
        dwRet = PROCMSG_DEL;
        return dwRet;
    }

    TChannel tSrcChn;    //Դͨ��
    tSrcChn.SetDevUri(strDevUri);
    tSrcChn.SetChnNO(nPuChn);
    
    TChannel tDstChn;    //Ŀ���豸ͨ��
    tDstChn.SetDevUri(strSession);
    tDstChn.SetChnNO(nCuChn);

    //��������
    CreateInviteReq(strSession, tSrcChn, tDstChn);

    //�Ƿ���Ҫ��ѯ������ַ
    if (NAStunServerStat())
    {
        //��ѯ������ַ
        u32 dwSeqNum = 0;
        u16 wVidRtpPort = LOCAL_INVITE_START_PORT+nCuChn*4;
        u16 awPort[4] = {wVidRtpPort, wVidRtpPort+1, wVidRtpPort+2, wVidRtpPort+3};

        //�Ƿ�ʹ��emDataSwitchBind��ʽ
        EmNetWorkType eType = emSelfBind;
        if (g_cCmuConf.m_nNAQryAddrMode != 0)
        {
            eType = emDataSwitchBind;
            //��Ҫ����NA�ص�
            SetDsFilter(&NAOnAck);
        }
        
        NAQueryMappedAddr(
            eType,
            4,
            awPort,
            (s8*)g_cCmuConf.m_tLocalAddr.tIpAddr.c_str(),
            &dwSeqNum,
            (void*)(GetTaskNO()));

        GLOBELOG(CRITICAL_LEV, "��ʼ��ѯ������ַ,NAģʽ[%d]\n", eType);
        NextState(QryAddr);
    }
    else
    {
        SendInviteReq();
        //�ȴ�Ӧ��
        NextState(WaitRsp);
    }  

    return dwRet;
}

u32 CInviteTask::OnReqTimer()
{
    return TIMERPOLL_DONE;
}

u32 CInviteTask::OnQryAddr(CMessage *const pcMsg)
{
    switch(pcMsg->event)
    {
    case CMU_STUN_QRY_ADDR_NTF:
        {
            TStunResultParam* ptStunReslut = (TStunResultParam*)pcMsg->content;

            if (ptStunReslut->m_emResult == emOK)
            {               
                //��ַ��ѯ�ɹ���ɾ��QryAddr��Filter
                if (g_cCmuConf.m_nNAQryAddrMode != 0)
                {
                    SetDsFilter(NULL);
                }
                
                TASKLOG(CRITICAL_LEV, "��ȡ������ַ�ɹ�����ʼ�����������\n");
                SendInviteReq();

                //�ȴ�Ӧ��
                NextState(WaitRsp);
            }
            else
            {
                TASKLOG(ERROR_LEV, "Punchʧ�ܣ�errcode[%d]\n", ptStunReslut->m_emResult);
                return PROCMSG_DEL;
            }
        }
        break;
    
    default:
        {
            TASKLOG(MOD_SWITCH, WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

            return PROCMSG_FAIL;
        }
        break;
    }

    return PROCMSG_OK;
}

u32 CInviteTask::OnQryAddrTimer()
{
    return TIMERPOLL_DONE;
}

u32 CInviteTask::OnWaitRsp(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_DEL;

    COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
    if (pcOspSipMsg == NULL)
    {
        TASKLOG(ERROR_LEV, "ospsip ��ϢΪ��\n");
        return dwRet;
    }

    CRsp cRsp;
    pcOspSipMsg->GetMsgBody(cRsp);
	m_tSipDlgId = pcOspSipMsg->GetSipDlgID();

    if (cRsp.GetErrorCode() == CMS_SUCCESS)
    {
        TASKLOG(CRITICAL_LEV, "invite�豸[%s-%d]��[%s-%d]�ɹ�\n", 
            m_tReq.GetSrcChn().GetDevUri().c_str(), m_tReq.GetSrcChn().GetChnNO(),
            m_tReq.GetDstChn().GetDevUri().c_str(), m_tReq.GetDstChn().GetChnNO());

        //����һ��������
        CPuData* ptPuData = g_cPuList.FindPuData(m_tReq.GetSrcChn().GetDevUri());
        if (ptPuData != NULL)
        {
            ptPuData->AddInvitedNum();
        }

        CLoginSession* ptTask = NULL;
        g_cCuList.Find(m_tReq.GetSession(), ptTask);
        if (ptTask != NULL)
        {
            ptTask->AddTask(this);
        }

		CInviteACK cInviteAck;
		cInviteAck.SetSession(m_tReq.GetSession());
		if(PROCMSG_OK!= PostInDlgReq(KDSIP_EVENT_INVITE_ACK, cInviteAck, m_tSipDlgId))
		{
			TASKLOG(ERROR_LEV, "����KDSIP_EVENT_INVITE_ACKʧ�ܣ�\n");
		}

        if (cRsp.GetSdp().IsAllMediaChnKeyEmpty())
        {
            //���dump��ͳ��������Ϣ
            AddDsDump(); 

            //���͹ؼ�֡
            SendKeyFrameReq();
        }
        else
        {
            //��Ҫpunch��punch�ɹ����ٷ��ؼ�֡
            u32 dwTaskNo = GetTaskNO();
            SamStartPunch(cRsp.GetSdp(), m_tNetAddr, (void*)dwTaskNo);
        }
        
		NextState(Service);

        return PROCMSG_OK;
    }
    else
    {
        TASKLOG(ERROR_LEV, "���豸[%s]Invite��[%s]ʧ�ܣ�������[%d]\n", 
            m_tReq.GetSrcChn().ToString().c_str(), m_tReq.GetDstChn().ToString().c_str(), 
			cRsp.GetErrorCode());

        return PROCMSG_DEL;
    }
}

u32 CInviteTask::OnWaitRspTimer()
{
    //Ӧ��ʱ
    if (GetCurStateHoldTime() > CMU_REQ_RSP_TASK_TIMEOUT)
    {
        TASKLOG(ERROR_LEV, "Dev[%s]Ӧ��ʱ, Msg[%s-%d]����ʧ��\n", 
            m_tReq.GetSrcChn().ToString().c_str(), m_tReq.GetEventStr().c_str(), m_tReq.GetEventId());

        return TIMERPOLL_DEL;
    }

    return TIMERPOLL_DONE;
}

u32 CInviteTask::OnService(CMessage *const pcMsg)
{
    switch(pcMsg->event)
    {
    case CMU_PUNCH_RESULT_NTF:
        {
            TPunchResultParam* ptPunchReslut = (TPunchResultParam*)pcMsg->content;

            if (ptPunchReslut->m_emResult == emOK)
            {
                TASKLOG(CRITICAL_LEV, "Punch�ɹ�����ʼ���͹ؼ�֡����\n");

                //���dump��ͳ��������Ϣ
                AddDsDump();              

                //����ؼ�֡
                SendKeyFrameReq();
            }
            else
            {
                TASKLOG(ERROR_LEV, "Punchʧ�ܣ�errcode[%d]\n", ptPunchReslut->m_emResult);
                return PROCMSG_DEL;
            }
        }
        break;
     case KEY_FRAME_RSP:
         {
             COspSipMsg*const pcOspSipMsg = (COspSipMsg*)pcMsg->content;

             CKeyFrameRsp cRsp;
             pcOspSipMsg->GetMsgBody(cRsp);

             if (cRsp.GetErrorCode() != CMS_SUCCESS)
             {
                 TASKLOG(ERROR_LEV, "����ؼ�֡ʧ�ܣ�Error[%d]\n", cRsp.GetErrorCode());
             }
             else
             {
                  TASKLOG(CRITICAL_LEV, "����ؼ�֡�ɹ�\n");
             }
         }
         break;

        //cmustubtest�ͻ����Լ����͵��ڲ�����bye����
    case BYE_REQ:
        {
            TASKLOG(EVENT_LEV, "bye devDev[%s], dlg[%d]\n", 
                m_tReq.GetSrcChn().ToString().c_str(), m_tSipDlgId);

            if (m_tSipDlgId != INVALID_DIALOG_ID)
            {
                string strNull;
                PostInDlgReq(KDSIP_EVENT_INVITE_BYE_REQ, strNull, m_tSipDlgId);
                m_tSipDlgId = INVALID_DIALOG_ID;
            }

            NextState(WaitByeRsp);
            
            return PROCMSG_OK;
        }
        break;

        //�յ�����˵�BYE����
    case OSP_SIP_BYE_REQ:
        {
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;

            s32 nByeReason = CMS_SUCCESS;
            if (pcOspSipMsg->GetMsgBodyLen() != 0)
            {
                CByeReq cByeReq;
                pcOspSipMsg->GetMsgBody(cByeReq);

                nByeReason = cByeReq.GetReason();
            }

            TKDSIP_DIALOG_ID tReqDlgId = pcOspSipMsg->GetSipDlgID();

            TASKLOG(MOD_SWITCH, CRITICAL_LEV, "�յ��豸DlgID[%lu]��ByeReq, Reason[%d]��\n", 
                tReqDlgId, nByeReason);

            ////////////////////////////////////////////////////////////
            //�����󷽻�ӦӦ��
            CByeRsp cRsp;

            u32 dwResult = PostRsp(KDSIP_EVENT_INVITE_BYE_RSP, pcOspSipMsg->GetSipTransID(), cRsp);
            TASKLOG(MOD_SWITCH, CRITICAL_LEV, "����KDSIP_EVENT_INVITE_BYE_RSP, ResultCode[%lu].\n", dwResult);

            m_tSipDlgId = INVALID_DIALOG_ID;

            return PROCMSG_DEL;
        }

    default:
        {
            TASKLOG(MOD_SWITCH, WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

            return PROCMSG_FAIL;
        }
        break;
    }

    return PROCMSG_OK;
}

u32 CInviteTask::OnServiceTimer()
{
    return TIMERPOLL_DONE;
}

u32 CInviteTask::OnWaitByeRsp(CMessage *const pcMsg)
{
    switch(pcMsg->event)
    {
    case OSP_SIP_BYE_RSP:
        {
            TASKLOG(MOD_SWITCH, ERROR_LEV, "bye[%s-%d]�ɹ�\n", 
                m_tReq.GetSrcChn().GetDevUri().c_str(), m_tReq.GetSrcChn().GetChnNO());
            return PROCMSG_DEL;
        }
        break;

    default:
        {
            TASKLOG(MOD_SWITCH, WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

            return PROCMSG_FAIL;
        }
        break;
    }

    return PROCMSG_OK;
}

u32 CInviteTask::OnWaitByeRspTimer()
{
    if (GetCurStateHoldTime() > CMU_REQ_RSP_TASK_TIMEOUT)
    {
        TASKLOG(MOD_SWITCH, ERROR_LEV, "bye[%s-%d]��ʱ��ɾ��\n", 
            m_tReq.GetSrcChn().GetDevUri().c_str(), m_tReq.GetSrcChn().GetChnNO());
        return TIMERPOLL_DEL;
    }

    return TIMERPOLL_DONE;
}

bool CInviteTask::CreateInviteReq(const string& strUserSess, const TChannel& tSrcChn, const TChannel& tDstChn)
{
    m_tReq.SetSession(strUserSess);

    m_tReq.SetSrcChn(tSrcChn);
    m_tReq.SetDstChn(tDstChn);

    TSdp tSdp;
    tSdp.SetSessionName(SDP_SESSION_PLAY);
    tSdp.SetOwner(tDstChn);	

    TNetAddrList tAddrList;
    tAddrList.push_back(m_tNetAddr);

    TMediaDesc tVideoMediaDesc;
    tVideoMediaDesc.SetMediaType(MEDIA_TYPE_VIDEO);
    tVideoMediaDesc.AddMediaTransChn(TMediaTransChannel(MEDIA_TRANS_CHN_RTP, tAddrList));

    tAddrList[0].SetNetPort(m_tNetAddr.GetNetPort()+1);
    tVideoMediaDesc.AddMediaTransChn(TMediaTransChannel(MEDIA_TRANS_CHN_RTCP, tAddrList));
    tSdp.AddMediaDesc(tVideoMediaDesc);

    TMediaDesc tAudioMediaDesc;
    tAudioMediaDesc.SetMediaType(MEDIA_TYPE_AUDIO);
    tAddrList[0].SetNetPort(m_tNetAddr.GetNetPort()+2);
    tAudioMediaDesc.AddMediaTransChn(TMediaTransChannel(MEDIA_TRANS_CHN_RTP, tAddrList));

    tAddrList[0].SetNetPort(m_tNetAddr.GetNetPort()+3);
    tAudioMediaDesc.AddMediaTransChn(TMediaTransChannel(MEDIA_TRANS_CHN_RTCP, tAddrList));
    tSdp.AddMediaDesc(tAudioMediaDesc);

    m_tReq.SetSdp(tSdp);

    return true;
}

bool CInviteTask::SendInviteReq()
{
    //ת����Ϣ
    return (PROCMSG_OK == PostReq(KDSIP_EVENT_INVITE_REQ, m_tReq, GetCmuUri()));
}

 bool CInviteTask::SendKeyFrameReq()
 {
     CKeyFrameReq cReq;
     cReq.SetSession(m_tReq.GetSession());
     cReq.SetDevChn(m_tReq.GetSrcChn());
     PostMsgReq(cReq, GetCmuUri());

     return true;
 }

 void CInviteTask::AddDsDump()
 {
     if (m_tNetAddr.GetNetIpv4() != 0 && m_tNetAddr.GetNetPort() != 0)
     {
         for (int i=0; i<4; i++)
         {
             dsAddDump(m_tNetAddr.GetNetIpv4(), m_tNetAddr.GetNetPort()+i);
         }

         TDumpStat newStat;
         newStat.m_ddwPowerUpTime = GetTimeMsByTicks();
         newStat.m_dwTaskNO = GetTaskNO();
         newStat.m_dwMediaType = EDumpMediaVideo;
         newStat.m_tDstAddr.SetNetIp(m_tNetAddr.GetNetIp());
         newStat.m_tDstAddr.SetNetPort(m_tNetAddr.GetNetPort());
         u64 ddwAddrID = MakeAddrID(m_tNetAddr.GetNetIpv4(), m_tNetAddr.GetNetPort());
         g_cDumpStatMap.Insert(ddwAddrID, newStat);

         newStat.m_ddwPowerUpTime = GetTimeMsByTicks();
         newStat.m_dwMediaType = EDumpMediaAudio;
         newStat.m_tDstAddr.SetNetIp(m_tNetAddr.GetNetIp());
         newStat.m_tDstAddr.SetNetPort(m_tNetAddr.GetNetPort()+2);
         ddwAddrID = MakeAddrID(m_tNetAddr.GetNetIpv4(), m_tNetAddr.GetNetPort()+2);
         g_cDumpStatMap.Insert(ddwAddrID, newStat);

         SetDsFilter(&CBDsFilterDumpStat);
     }   
 }

 void CInviteTask::DelDsDump()
 {
     if (m_tNetAddr.GetNetIpv4() != 0 && m_tNetAddr.GetNetPort() != 0)
     {
         for (int i=0; i<4; i++)
         {
             dsRemoveDump(m_tNetAddr.GetNetIpv4(), m_tNetAddr.GetNetPort()+i);
         }

         SetDsFilter(NULL);

         u64 ddwAddrID = MakeAddrID(m_tNetAddr.GetNetIpv4(), m_tNetAddr.GetNetPort());
         g_cDumpStatMap.Erase(ddwAddrID);
         ddwAddrID = MakeAddrID(m_tNetAddr.GetNetIpv4(), m_tNetAddr.GetNetPort()+2);
         g_cDumpStatMap.Erase(ddwAddrID);
     }  
 }

 void CInviteTask::SetDsFilter(FilterFunc pfDsFilter)
 {
     if (m_tNetAddr.GetNetIpv4() != 0 && m_tNetAddr.GetNetPort() != 0)
     {
         for (int i=0; i<4; i++)
         {
             dsSetFilterFunc(m_tNetAddr.GetNetIpv4(), m_tNetAddr.GetNetPort()+i, pfDsFilter);
         }  
     }
 }

