/*****************************************************************************
模块名      : cmu 
文件名      : vtduregtask.cpp
相关文件    : 
文件实现功能: CVtduRegTask实现
作者        : 
版本        : vs_phoenix  Copyright(C) 2009-2015 KDDACOM, All rights reserved.
-----------------------------------------------------------------------------
修改记录:
日  期         版本        修改人      修改内容
2011/11/04     1.0        黄至春        创建
******************************************************************************/

#include "cms/cms_errorcode.h"
#include "cmuconst.h"
#include "vtduregtask.h"
#include "cmucoreapp.h"
#include "cuiregtask.h"
#include "cmuconf.h"

CVtduList g_cVtduList;
extern TMappingTable g_tMappingTable;

CVtduRegTask::CVtduRegTask(CInstExt *pcInst) : CLoginSession(pcInst)
{
    m_dwCurSwitchNum = 0;
    m_nRegTime = 0;
    m_dwSerialNo = 0;
	m_tNetSegAllInfo.Clear();
	m_tMappingTable.clear();
}

CVtduRegTask::~CVtduRegTask()
{
    ReleaseResource();
}

void CVtduRegTask::InitStateMachine()
{
    CStateProc cWaitRegProc;
    cWaitRegProc.ProcMsg = (CTask::PFProcMsg)&CVtduRegTask::OnWaitReg;
    cWaitRegProc.ProcErrMsg = (CTask::PFProcMsg)&CVtduRegTask::OnWaitReg;
    cWaitRegProc.TimerPoll = (CTask::PFTimerPoll)&CVtduRegTask::OnWaitRegTimer;
    AddRuleProc(WaitReg, cWaitRegProc);

    CStateProc cServiceProc;
    cServiceProc.ProcMsg = (CTask::PFProcMsg)&CVtduRegTask::OnService;
    cServiceProc.ProcErrMsg = (CTask::PFProcMsg)&CVtduRegTask::OnService;
    cServiceProc.TimerPoll = (CTask::PFTimerPoll)&CVtduRegTask::OnServiceTimer;
    AddRuleProc(Service, cServiceProc);

    NextState(WaitReg);
}

void CVtduRegTask::ReleaseResource()
{
    TASKLOG(CMU_MOD_VTDU, EVENT_LEV, "vtdu[%s]登录事务被销毁\n",
        GetDevUri().GetURIString().c_str());

    switch(GetState())
    {
    case WaitReg:
        {
        }
        break;

    case Service:
        {
            //向uas写设备下线日志
            ((CCmuCoreInst*)GetInstance())->GetUasLogTask()->AddDeviceLog(GetDevLogInfo(false));

            DestroyChildTasks();

            //从列表中删除
            g_cVtduList.Erase(m_cVtduRegReq.GetDevUri());

			for (TMappingTable::iterator it=m_tMappingTable.begin(); it!=m_tMappingTable.end();it++)
			{
				TMappingItem tItem = it->second;
				tItem.SetOperateType(MAPPING_OP_TYPE_DEL);
				m_tMappingTable[it->first] = tItem;
				g_tMappingTable.erase(it->first);
			}
			//通知所有的CUI
			NotifyAllCuiMappingTable();
			m_tMappingTable.clear();
        }
        break;

    default:
        {
            TASKLOG(CMU_MOD_VTDU, ERROR_LEV, "未知事务状态[%d]\n", GetState());
        }
        break;
    }

	m_tNetSegAllInfo.Clear();
}

u32 CVtduRegTask::OnWaitReg(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_FAIL;
    switch(pcMsg->event)
    {
    case VTDU_REG_REQ:
        {
            int dwErrorCode = CMS_SUCCESS;
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
            if (pcOspSipMsg == NULL)
            {
                TASKLOG(CMU_MOD_VTDU, ERROR_LEV, "ospsip 消息为空\n");
                dwErrorCode = ERR_CMU_SIP_BODY_NULL;
                dwRet = PROCMSG_DEL;
                return dwRet;
            }

            CVtduRegReq cReq;
            pcOspSipMsg->GetMsgBody(cReq);

			//add by Royan 20130828 设置punch属性
			if (cReq.GetIsSupportNA())
			{
				SetIsNeedPunch(true);

				//这种情况完全不关心pu和平台的网络关系
			}
			else
			{
				SetIsNeedPunch(false);
			}

            if (g_cVtduList.Exist(cReq.GetDevUri()))
            {
                TASKLOG(CMU_MOD_VTDU,ERROR_LEV, "已存在的vtdu[%s]登录cmu\n",
                    cReq.GetDevUri().c_str());
                dwErrorCode = ERR_CMU_VTDU_ALREADY_REG;
            }

            if (cReq.GetIpAddrList().size() == 0)
            {
                TASKLOG(CMU_MOD_VTDU,ERROR_LEV, "vtdu[%s]地址列表为空\n",
                    cReq.GetDevUri().c_str());
                dwErrorCode = ERR_CMU_DEV_ADDR_INVALID;
            }

            //发送应答
            CVtduRegRsp cRsp;
            cRsp.SetHeadFromReq(cReq);
            cRsp.SetErrorCode(dwErrorCode);
			cRsp.SetIsStreamKeepaliveEnable(g_cCmuConf.m_bIsStreamKeepaliveEnable);
			cRsp.SetStreamKeepaliveInterval(g_cCmuConf.m_wStreamKeepaliveInterval);
			cRsp.SetIsDefenseCrossStreamEnable(GetCommonConfig().GetDefenseCrossStreamEnable());
 			cRsp.SetIsCheckRecvSRRR(g_cCmuConf.m_bIsCheckRecvSRRR);
			cRsp.SetSRRRCheckTimeout(g_cCmuConf.m_wSRRRCheckTimeout);
			PostMsgRsp(pcOspSipMsg->GetSipTransID(), cRsp);

            if (dwErrorCode == CMS_SUCCESS)
            {
                //保存设备的注册信息
                m_cVtduRegReq = cReq;
                m_nRegTime = time(NULL);
                SetSession(GetDevUri().GetURIString());
                this->SetDevType(DEV_TYPE_VTDU);
                SetDevUri(cReq.GetDevUri());
                SetServerUri(OspSipGetLocalURI().GetURIString());

                TNetAddrList tAddrList;
                const CIpList& cIpList = m_cVtduRegReq.GetIpAddrList();
                for (size_t i=0; i<cIpList.size(); i++)
                {
                    TNetAddr tAddr;
                    tAddr.SetNetIp(cIpList.at(i));
                    tAddrList.push_back(tAddr);
                }
                SetDevAddrList(tAddrList);

                //注册成功
                TASKLOG(CMU_MOD_VTDU,CRITICAL_LEV, "vtdu[%s]登录cmu成功!\n",
                    cReq.GetDevUri().c_str());

				//保存网段信息
				SetNetSegAllInfo(cReq.GetNetSegInfo());

                //开始心跳
                SetHBParam(cReq.GetDevUri());

                //添加到VTDU列表中
                g_cVtduList.Insert(cReq.GetDevUri(), this);

                //进入服务态
                NextState(Service);

                //通知stun地址
                ((CCmuCoreInst*)GetInstance())->UpdateDevStunAddr(GetDevSipUri());

                //向uas写设备日志
                ((CCmuCoreInst*)GetInstance())->GetUasLogTask()->AddDeviceLog(GetDevLogInfo(true));

                dwRet = PROCMSG_OK;
            }
            else
            {
                //注册失败
                TASKLOG(CMU_MOD_VTDU, ERROR_LEV, "vtdu[%s]登录cmu失败!\n",
                    cReq.GetDevUri().c_str());

                dwRet = PROCMSG_DEL;
            }
        }
        break;

    default:
        {
            TASKLOG(CMU_MOD_VTDU, WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }

    return dwRet;
}

u32 CVtduRegTask::OnWaitRegTimer()
{
    return TIMERPOLL_DONE;
}

u32 CVtduRegTask::OnService(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_FAIL;
    switch(pcMsg->event)
    {
	case MAPPING_TABLE_NTF_REQ:
		{
			COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
			if (pcOspSipMsg == NULL)
			{
				TASKLOG(CMU_MOD_VTDU, ERROR_LEV, "ospsip 消息为空\n");
				dwRet = PROCMSG_DEL;
				return dwRet;
			}

			CMappingTableNtfReq cReq;
			pcOspSipMsg->GetMsgBody(cReq);
			m_tMappingTable = cReq.GetMappingTable();
			for (TMappingTable::const_iterator it=m_tMappingTable.begin(); it!=m_tMappingTable.end(); it++)
			{
				g_tMappingTable[it->first] = it->second;
			}

			//发送应答
			CMappingTableNtfRsp cRsp;
			cRsp.SetHeadFromReq(cReq);
			cRsp.SetErrorCode(CMS_SUCCESS);
			PostMsgRsp(pcOspSipMsg->GetSipTransID(), cRsp);

			//通知所有的CUI
			NotifyAllCuiMappingTable();

			dwRet= PROCMSG_OK;
		}
		break;

	case MAPPING_TABLE_NTF_RSP:
		{
			//只打印一下
			COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
			if (pcOspSipMsg == NULL)
			{
				TASKLOG(CMU_MOD_VTDU, ERROR_LEV, "ospsip 消息为空\n");
				dwRet = PROCMSG_DEL;
				return dwRet;
			}

			CMappingTableNtfRsp cRsp;
			pcOspSipMsg->GetMsgBody(cRsp);

			TASKLOG(CMU_MOD_VTDU, EVENT_LEV, "收到CUI(%s)的MappingTableNtfRsp，ErrCode(%d)\n",
				cRsp.GetSession().c_str(), cRsp.GetErrorCode());
		}
		break;

	case UPDATE_REG_NETSEG_REQ:
		{
			COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
			if (pcOspSipMsg == NULL)
			{
				TASKLOG(CMU_MOD_VTDU, ERROR_LEV, "ospsip 消息为空\n");
				dwRet = PROCMSG_DEL;
				return dwRet;
			}

			CUpdateRegNetSegReq cReq;
			pcOspSipMsg->GetMsgBody(cReq);
			m_tNetSegAllInfo = cReq.GetNetSegmentAllInfo();

			//发送应答
			CUpdateRegNetSegRsp cRsp;
			cRsp.SetHeadFromReq(cReq);
			cRsp.SetErrorCode(CMS_SUCCESS);
			PostMsgRsp(pcOspSipMsg->GetSipTransID(), cRsp);

			dwRet= PROCMSG_OK;
		}
		break;

    case OSP_SIP_DISCONNECT:
        {
            if (DisProc(pcMsg))
            {
                dwRet = PROCMSG_DEL;
            }
            else
            {
                dwRet = PROCMSG_FAIL;
            }
        }
        break;

    default:
        {
            TASKLOG(CMU_MOD_VTDU, WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }

    return dwRet;
}

u32 CVtduRegTask::OnServiceTimer()
{
    return TIMERPOLL_DONE;
}

bool CVtduRegTask::DisProc(CMessage *const pcMsg)
{
    //断链处理

    COspSipMsg* pOspSipMsg = (COspSipMsg*)pcMsg->content;
    if (pOspSipMsg == NULL)
    {
        TASKLOG(CMU_MOD_VTDU, ERROR_LEV, "ospsip 消息为空\n");
        return false;
    }

    //是否真断链
    TSipURI tDevUri;
    tDevUri.SetNOString(pOspSipMsg->GetMsgBody());
    if (tDevUri != m_cVtduRegReq.GetDevUri())
    {
        TASKLOG(CMU_MOD_VTDU, WARNING_LEV, "收到无效断链消息，断链设备[%s]\n", tDevUri.GetNOString());
        return false;
    }

    TASKLOG(CMU_MOD_VTDU, CRITICAL_LEV, "vtdu[%s]和cmu断链\n", tDevUri.GetNOString());            

    return true;
}

//通知所有的CUI映射表信息
void CVtduRegTask::NotifyAllCuiMappingTable()
{
	//TODO 应该做成NotifyTask，保证CUI收到
	Iterator pPos;
	string strUri;
	CLoginSession* pTask = NULL;
	while (!pPos.End())
	{
		if (g_cCuiList.Iterate(pPos, strUri, pTask) && pTask != NULL)
		{
			CMappingTableNtfReq cReq;
			cReq.SetSession(g_cCmuConf.m_tLocalUri.GetURIString());
			cReq.SetMappingTable(m_tMappingTable);

			PostMsgReq(cReq, pTask->GetDevSipUri());
		}
	}
}

void CVtduRegTask::PrintData() const
{
    CLoginSession::PrintData();

    OspPrintf(TRUE, FALSE, "%-20s:[%s]\n", "devUri", m_cVtduRegReq.GetDevUri().c_str());

    OspPrintf(TRUE, FALSE, "%-20s:[", "ipAddrList");
    CIpList::const_iterator itr = m_cVtduRegReq.GetIpAddrList().begin();
    for(; itr != m_cVtduRegReq.GetIpAddrList().end(); itr++)
    {
        OspPrintf(TRUE, FALSE, "%s ", itr->c_str());
    }
    OspPrintf(TRUE, FALSE, "]\n");

	OspPrintf(TRUE, FALSE, "%-20s:[%lu]\n", "mediaStartPort", m_cVtduRegReq.GetMediaStartPort());
	OspPrintf(TRUE, FALSE, "%-20s:[%lu]\n", "mediaM2PPort", m_cVtduRegReq.GetMediaM2PPort());
	OspPrintf(TRUE, FALSE, "%-20s:[%lu]\n", "mappingTable", m_tMappingTable.size());
	int nCount = 0;
	for (TMappingTable::const_iterator it=m_tMappingTable.begin(); it!=m_tMappingTable.end(); it++)
	{
		nCount++;
		OspPrintf(TRUE, FALSE, "  [%d][%s][Local-%s:%d -> Mapping-%s:%d]\n",
			nCount, it->second.GetProtocol().c_str(),
			it->second.GetLocalIP().c_str(), it->second.GetLocalPort(),
			it->second.GetMappingIP().c_str(), it->second.GetMappingPort());
	}
    OspPrintf(TRUE, FALSE, "%-20s:[%lu]\n", "maxSwitchNum", m_cVtduRegReq.GetMaxSwitchNum());
    OspPrintf(TRUE, FALSE, "%-20s:[%lu]\n", "maxRcvGroup", m_cVtduRegReq.GetMaxRcvGroup());
    OspPrintf(TRUE, FALSE, "%-20s:[%lu]\n", "maxSndMmbPerRcvGrp", m_cVtduRegReq.GetMaxSndMmbPerRcvGrp());
    OspPrintf(TRUE, FALSE, "%-20s:[%s]\n", "isTravGAP", m_cVtduRegReq.GetIsTravGAP()?"true":"false");
    OspPrintf(TRUE, FALSE, "%-20s:[%s]\n", "creditDomainUri", m_cVtduRegReq.GetCreditDomainUri().c_str());
    OspPrintf(TRUE, FALSE, "\n");

    OspPrintf(TRUE, FALSE, "%-20s:[%lu]\n", "curSwitchNum", m_dwCurSwitchNum);
    OspPrintf(TRUE, FALSE, "%-20s:[%lu]\n", "serialNo", m_dwSerialNo);
    OspPrintf(TRUE, FALSE, "%-20s:[%s]\n", "regTime", CCMSTime(m_nRegTime).ToString().c_str());

	OspPrintf(TRUE, FALSE, "%-20s:[%d]\n", "NetSegListSize", m_tNetSegAllInfo.GetNetSegmentList().size());
	TNetSegmentList::const_iterator it = m_tNetSegAllInfo.GetNetSegmentList().begin();
	for(; it != m_tNetSegAllInfo.GetNetSegmentList().end(); it++)
	{
		OspPrintf(TRUE, FALSE, "\tNic[%s] IP[%s] Mask[%s]\n",
			it->GetNicName().c_str(), it->GetNetIp().c_str(), it->GetNetMask().c_str());
	}
	OspPrintf(TRUE, FALSE, "%-20s:[%s]\n", "DefaultRouteIP", m_tNetSegAllInfo.GetDafaultRoute().c_str());
}

void CVtduList::PrintData()
{
    Iterator pPos;
    string strUri;
    CLoginSession* pTask = NULL;

    OspPrintf(TRUE, FALSE, "当前在线的vtdu：[%u]个\n", GetSize());	
    u32 i = 0;

    while (!pPos.End())
    {
        if (this->Iterate(pPos, strUri, pTask) && pTask)
        {
            CVtduRegTask* pVtduTask = (CVtduRegTask*)pTask;
            i++;
            string strAddrs = IpListToStr(pVtduTask->GetIpAddrList());

            OspPrintf(TRUE, FALSE, "  %4u. vtdu[%s]-IP[%s]-session[%s]-taskNO[%lu]\n", i,
                strUri.c_str(), strAddrs.c_str(),  pTask->GetSession().c_str(), pTask->GetTaskNO());
        }
    }

    OspPrintf(TRUE, FALSE, "当前在线的vtdu：[%u]个\n", GetSize());
}

u32 CVtduList::GetTotalSwitchNum()
{
    Iterator pPos;
    string strUri;
    CLoginSession* pTask = NULL;

    u32 dwTotalSwitchNum = 0;

    while (!pPos.End())
    {
        if (this->Iterate(pPos, strUri, pTask) && pTask)
        {
            CVtduRegTask* pVtduTask = (CVtduRegTask*)pTask;
            dwTotalSwitchNum += pVtduTask->GetCurSwitchNum();
        }
    }

    return dwTotalSwitchNum;
}

u32 CVtduList::GetMaxSwitchNum()
{
    Iterator pPos;
    string strUri;
    CLoginSession* pTask = NULL;

    u32 dwMaxSwitchNum = 0;

    while (!pPos.End())
    {
        if (this->Iterate(pPos, strUri, pTask) && pTask)
        {
            CVtduRegTask* pVtduTask = (CVtduRegTask*)pTask;
            dwMaxSwitchNum += pVtduTask->GetMaxSwitchNum();
        }
    }

    return dwMaxSwitchNum;
}



