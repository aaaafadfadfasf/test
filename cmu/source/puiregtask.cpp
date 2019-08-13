
#include "cms/cms_errorcode.h"
#include "cms/cmu/cmu_event.h"
#include "cms/cmu/cmu_struct.h"
#include "cms/cmu/cmu_proto.h"

#include "cmuconst.h"
#include "puiregtask.h"
#include "puregtask.h"
#include "cmucoreapp.h"
#include "cmuconf.h"
#include "pubatchregtask.h"
#include "puinotifytask.h"


CPuiList g_cPuiList;

CPuiRegTask::CPuiRegTask(CInstExt *pcInst) : CLoginSession(pcInst)
{
	m_tNetSegAllInfo.Clear();
    m_bIsSupportRedis = false;
    m_pcBatchRegTask  = NULL;
}

CPuiRegTask::~CPuiRegTask()
{
    ReleaseResource();
}

void CPuiRegTask::InitStateMachine()
{
    CStateProc cWaitRegProc;
    cWaitRegProc.ProcMsg = (CTask::PFProcMsg)&CPuiRegTask::OnWaitReg;
    cWaitRegProc.ProcErrMsg = (CTask::PFProcMsg)&CPuiRegTask::OnWaitReg;
    cWaitRegProc.TimerPoll = (CTask::PFTimerPoll)&CPuiRegTask::OnWaitRegTimer;
    AddRuleProc(WaitReg, cWaitRegProc);

    CStateProc cServiceProc;
    cServiceProc.ProcMsg = (CTask::PFProcMsg)&CPuiRegTask::OnService;
    cServiceProc.ProcErrMsg = (CTask::PFProcMsg)&CPuiRegTask::OnService;
    cServiceProc.TimerPoll = (CTask::PFTimerPoll)&CPuiRegTask::OnServiceTimer;
    AddRuleProc(Service, cServiceProc);

    NextState(WaitReg);
}

void CPuiRegTask::ReleaseResource()
{
    TASKLOG(CMU_MOD_CU, EVENT_LEV, "pui[%s]登录事务被销毁\n",
        GetDevUri().c_str());

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

            /* 
            pui断链后，通知该pui上所有pu下线
            目前直接遍历删除所有的登录事务，
            后续如果发现有性能问题，可以通过发消息手拉手方式清除pu登录事务
            */
            DestroyChildTasks();

            //从列表中删除
            g_cPuiList.Erase(GetDevUri());

        }
        break;

    default:
        {
            TASKLOG(CMU_MOD_PUI, ERROR_LEV, "未知事务状态[%d]\n", GetState());
        }
        break;
    }

	m_tNetSegAllInfo.Clear();

    if (NULL != m_pcBatchRegTask)
    {
        delete m_pcBatchRegTask;
        m_pcBatchRegTask = NULL;
    }
}

CPuBatchRegTask *CPuiRegTask::GetBatchRegTask()
{
    if (NULL == m_pcBatchRegTask)
    {
        m_pcBatchRegTask = new CPuBatchRegTask(GetInstance(), this);
        CMS_ASSERT(NULL != m_pcBatchRegTask && "m_pcBatchRegTask = NULL!!!");
        m_pcBatchRegTask->InitStateMachine();
    }
    
    return m_pcBatchRegTask;
}

void CPuiRegTask::PrintData() const
{
	CLoginSession::PrintData();

    if (NULL != m_pcBatchRegTask)
    {
        OspPrintf(TRUE, FALSE, "%-20s:[%u]\n", "BatchRegTask", m_pcBatchRegTask->GetTaskNO());
    }

	OspPrintf(TRUE, FALSE, "%-20s:[%d]\n", "NetSegListSize", m_tNetSegAllInfo.GetNetSegmentList().size());
	TNetSegmentList::const_iterator it = m_tNetSegAllInfo.GetNetSegmentList().begin();
	for(; it != m_tNetSegAllInfo.GetNetSegmentList().end(); it++)
	{
		OspPrintf(TRUE, FALSE, "\tNic[%s] IP[%s] Mask[%s]\n",
			it->GetNicName().c_str(), it->GetNetIp().c_str(), it->GetNetMask().c_str());
	}
	OspPrintf(TRUE, FALSE, "%-20s:[%s]\n", "DefaultRouteIP", m_tNetSegAllInfo.GetDafaultRoute().c_str());
    OspPrintf(TRUE, FALSE, "%-20s:[%d]\n", "SupportRedis", m_bIsSupportRedis);
}

u32 CPuiRegTask::OnWaitReg(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_FAIL;
    switch(pcMsg->event)
    {
    case PUI_REG_REQ:
        {
            int dwErrorCode = CMS_SUCCESS;
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
            if (pcOspSipMsg == NULL)
            {
                TASKLOG(CMU_MOD_PUI, ERROR_LEV, "ospsip 消息为空\n");
                dwErrorCode = ERR_CMU_SIP_BODY_NULL;
                dwRet = PROCMSG_DEL;
                return dwRet;
            }

            CPuiRegReq cReq;
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

            m_bIsSupportRedis = cReq.GetIsSupportRedis();

            SetDevUri(cReq.GetDevUri());
            if (g_cPuiList.Exist(GetDevUri()))
            {
                TASKLOG(CMU_MOD_PUI, ERROR_LEV, "已存在的pui[%s]登录cmu\n",
                    GetDevUri().c_str());
                dwErrorCode = ERR_CMU_PUI_ALREADY_REG;
            }

            if (g_cPuiNotifyList.IsExistPuiNotifyTask(GetDevSipUri()))
            {
                TASKLOG(CMU_MOD_PUI, ERROR_LEV, "pui[%s]上一次登录事务尚未处理结束，请稍后登录\n",
                    GetDevUri().c_str());
                dwErrorCode = ERR_CMU_PUI_ALREADY_REG; 
            }

            CPuiRegRsp cRsp;
            cRsp.SetErrorCode(dwErrorCode);
            cRsp.SetSeqNum(cReq.GetSeqNum());
            //具有唯一UUID的设备，其session就是设备URI，cmu设置后由rsp回带给登录设备
            //目前只有cu的session是由cmu动态分配的。
            //cRsp.SetSession(cReq.GetSession());
            cRsp.SetSession(GetDevUri());

			TBackendCapList &tBackendCapList = cRsp.GetBackendCapabilities().GetBackendCapList();
			// 支持采样率协商则通知给pui
			if (g_cCmuConf.m_bSupportClockRate)
			{
				tBackendCapList.insert(pair<TBackendCapId, TBackendCapValue>(VIDEO_CLOCK_RATE_NEGO, CAP_VALUE_PLAT_ENABLE));
			}

            PostMsgRsp(pcOspSipMsg->GetSipTransID(), cRsp);

            if (dwErrorCode == CMS_SUCCESS)
            {
                //注册成功
                TASKLOG(CMU_MOD_PUI, CRITICAL_LEV, "pui[%s]登录cmu成功!\n",
                    GetDevUri().c_str());

                SetSession(GetDevUri());

                //使用pui身份登录cmu的默认(字段为空)设备类型是pui，也可能是:gbs，cui1.0，G网关等
                if (cReq.GetDevType().empty())
                {
                    SetDevType(DEV_TYPE_PUI);
                }
                else
                {
                    SetDevType(cReq.GetDevType());
                }
                
                SetServerUri(OspSipGetLocalURI().GetURIString());
                SetDevAddrList(cReq.GetIpAddrList());

				//保存网段信息
				SetNetSegAllInfo(cReq.GetNetSegInfo());

                //开始心跳
                SetHBParam(GetDevSipUri());

                //添加到PUI列表中
                g_cPuiList.Insert(GetDevUri(), this);

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
                TASKLOG(CMU_MOD_PUI, ERROR_LEV, "pui[%s]登录cmu失败!\n",
                    GetDevUri().c_str());

                dwRet = PROCMSG_DEL;
            }
        }
        break;

    default:
        {
            TASKLOG(CMU_MOD_PUI, WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }

    return dwRet;
}

u32 CPuiRegTask::OnWaitRegTimer()
{
    return TIMERPOLL_DONE;
}

u32 CPuiRegTask::OnService(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_FAIL;
    switch(pcMsg->event)
    {
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
            TASKLOG(CMU_MOD_PUI, WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }

    return dwRet;
}

u32 CPuiRegTask::OnServiceTimer()
{
    return TIMERPOLL_DONE;
}

bool CPuiRegTask::DisProc(CMessage *const pcMsg)
{
    //PUI断链处理

    COspSipMsg* pOspSipMsg = (COspSipMsg*)pcMsg->content;
    if (pOspSipMsg == NULL)
    {
        TASKLOG(CMU_MOD_PUI, ERROR_LEV, "ospsip 消息为空\n");
        return false;
    }

    //是否真是PUI断链
    TSipURI tDevUri;
    tDevUri.SetNOString(pOspSipMsg->GetMsgBody());
    if (tDevUri != GetDevSipUri())
    {
        TASKLOG(CMU_MOD_PUI, WARNING_LEV, "收到无效断链消息，断链设备[%s]\n", tDevUri.GetNOString());
        return false;
    }

    TASKLOG(CMU_MOD_PUI, CRITICAL_LEV, "pui[%s]和cmu断链\n", tDevUri.GetNOString());

    return true;
}

u32 CPuiRegTask::GetVidInPortNum()
{
	u32 dwPuNum = 0;
	if (NULL != m_pcBatchRegTask)
	{
		dwPuNum += m_pcBatchRegTask->GetPuSet().size();
	}

	CTask* pTask = NULL;
	Iterator pPos;
	while (!pPos.End())
	{
		if (IterateChildTasks(pPos, pTask) && pTask != NULL)
		{
			if (string("CPuRegTask") == string(pTask->GetObjName()))
			{
				//todo 此处需要计算视频源
				//g_cPuList.Find(((CPuRegTask*)pTask)->GetDevUri())
				dwPuNum++;
			}
		}               
	} 

	return dwPuNum;
}

void CPuiList::PrintData(const string &strPuiType)
{
    Iterator pPos;
    string strUri;
    CLoginSession* pTask = NULL;

    OspPrintf(TRUE, FALSE, "当前在线的pui总数：[%u]个\n", GetSize());
    u32 i = 0;

    while (!pPos.End())
    {
        if (this->Iterate(pPos, strUri, pTask))
        {
            if (strPuiType.empty() || strPuiType == pTask->GetDevType())
            {
                i++;
                OspPrintf(TRUE, FALSE, "  %4u. pui[%s]-type[%s]-session[%s]-taskNO[%u]\n", i, 
                    strUri.c_str(), pTask->GetDevType().c_str(), pTask->GetSession().c_str(), pTask->GetTaskNO());
            }
        }
    }

    OspPrintf(TRUE, FALSE, "当前在线的pui总数：[%u]个，符合条件的pui数量：[%u]个\n", GetSize(), i);
}
