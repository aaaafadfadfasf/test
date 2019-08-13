
#include "cms/cms_errorcode.h"
#include "cms/cmu/cmu_event.h"
#include "cms/cmu/cmu_struct.h"
#include "cms/cmu/cmu_proto.h"

#include "cmuconst.h"
#include "culogintask.h"
#include "cuiregtask.h"
#include "cmucoreapp.h"

#include "cmuconf.h"

CCuiList g_cCuiList;
extern CCmuConf g_cCmuConf;
extern TMappingTable g_tMappingTable;

CCuiRegTask::CCuiRegTask(CInstExt *pcInst) : CLoginSession(pcInst)
{
	m_tNetSegAllInfo.Clear();
}

CCuiRegTask::~CCuiRegTask()
{
    ReleaseResource();
}

void CCuiRegTask::InitStateMachine()
{
    CStateProc cWaitRegProc;
    cWaitRegProc.ProcMsg = (CTask::PFProcMsg)&CCuiRegTask::OnWaitReg;
    cWaitRegProc.ProcErrMsg = (CTask::PFProcMsg)&CCuiRegTask::OnWaitReg;
    cWaitRegProc.TimerPoll = (CTask::PFTimerPoll)&CCuiRegTask::OnWaitRegTimer;
    AddRuleProc(WaitReg, cWaitRegProc);

    CStateProc cServiceProc;
    cServiceProc.ProcMsg = (CTask::PFProcMsg)&CCuiRegTask::OnService;
    cServiceProc.ProcErrMsg = (CTask::PFProcMsg)&CCuiRegTask::OnService;
    cServiceProc.TimerPoll = (CTask::PFTimerPoll)&CCuiRegTask::OnServiceTimer;
    AddRuleProc(Service, cServiceProc);

    NextState(WaitReg);
}

void CCuiRegTask::ReleaseResource()
{
    TASKLOG(CMU_MOD_CU, EVENT_LEV, "cui[%s]登录事务被销毁\n",
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
            cui断链后，通知该cui上所有cu下线
            目前直接遍历删除所有的登录事务，
            后续如果发现有性能问题，可以通过发消息手拉手方式清除cu登录事务
            */
            DestroyChildTasks();

            //从列表中删除
            g_cCuiList.Erase(GetDevUri());

        }
        break;

    default:
        {
            TASKLOG(ERROR_LEV, "未知事务状态[%d]\n", GetState());
        }
        break;
    }

	m_tNetSegAllInfo.Clear();
}

u32 CCuiRegTask::OnWaitReg(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_FAIL;
    switch(pcMsg->event)
    {
    case CUI_REG_REQ:
        {
            int dwErrorCode = CMS_SUCCESS;
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
            if (pcOspSipMsg == NULL)
            {
                TASKLOG(CMU_MOD_CUI, ERROR_LEV, "ospsip 消息为空\n");
                dwErrorCode = ERR_CMU_SIP_BODY_NULL;
                dwRet = PROCMSG_DEL;
                return dwRet;
            }

            CCuiRegReq cReq;
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

            SetDevUri(cReq.GetDevUri());
            if (g_cCuiList.Exist(GetDevUri()))
            {
                TASKLOG(CMU_MOD_CUI, ERROR_LEV, "已存在的cui[%s]登录cmu\n",
                    GetDevUri().c_str());
                dwErrorCode = ERR_CMU_CUI_ALREADY_REG;
            }

            CCuiRegRsp cRsp;
            cRsp.SetErrorCode(dwErrorCode);
            cRsp.SetSeqNum(cReq.GetSeqNum());
            //具有唯一UUID的设备，其session就是设备URI，cmu设置后由rsp回带给登录设备
            //目前只有cu的session是由cmu动态分配的。
            //cRsp.SetSession(cReq.GetSession());
            cRsp.SetSession(GetDevUri());
            PostMsgRsp(pcOspSipMsg->GetSipTransID(), cRsp);

            if (dwErrorCode == CMS_SUCCESS)
            {
                //注册成功
                TASKLOG(CMU_MOD_CUI, CRITICAL_LEV, "cui[%s]登录cmu成功!\n",
                    GetDevUri().c_str());

                SetSession(GetDevUri());
                SetDevType(DEV_TYPE_CUI);
                SetServerUri(OspSipGetLocalURI().GetURIString());

                //获取ipaddlist
                SetIpAddrList(cReq.GetIpAddrList());

				//保存网段信息
				SetNetSegAllInfo(cReq.GetNetSegmentInfo());

                //开始心跳
                SetHBParam(GetDevSipUri());

                //添加到列表中
                g_cCuiList.Insert(GetDevUri(), this);

                //进入服务态
                NextState(Service);

                //通知stun地址
                ((CCmuCoreInst*)GetInstance())->UpdateDevStunAddr(GetDevSipUri());

                //向uas写设备日志
                 ((CCmuCoreInst*)GetInstance())->GetUasLogTask()->AddDeviceLog(GetDevLogInfo(true));

				//通知CUI映射表信息
				NotifyCuiMappingTable();

                dwRet = PROCMSG_OK;
            }
            else
            {
                //注册失败
                TASKLOG(CMU_MOD_CUI, ERROR_LEV, "cui[%s]登录cmu失败!\n",
                    GetDevUri().c_str());

                dwRet = PROCMSG_DEL;
            }
        }
        break;

    default:
        {
            TASKLOG(CMU_MOD_CUI, WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }

    return dwRet;
}

u32 CCuiRegTask::OnWaitRegTimer()
{
    return TIMERPOLL_DONE;
}

u32 CCuiRegTask::OnService(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_FAIL;
    switch(pcMsg->event)
    {
	case MAPPING_TABLE_NTF_RSP:
		{
			TASKLOG(CMU_MOD_VTDU, EVENT_LEV, "MAPPING_TABLE_NTF_RSP\n");
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
            TASKLOG(CMU_MOD_CUI, WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }

    return dwRet;
}

u32 CCuiRegTask::OnServiceTimer()
{
    return TIMERPOLL_DONE;
}

//通知CUI映射表信息
void CCuiRegTask::NotifyCuiMappingTable()
{
	//TODO 应该做成NotifyTask，保证CUI收到
	CMappingTableNtfReq cReq;
	cReq.SetSession(g_cCmuConf.m_tLocalUri.GetURIString());
	cReq.SetMappingTable(g_tMappingTable);

	PostMsgReq(cReq, GetDevSipUri());
}


bool CCuiRegTask::DisProc(CMessage *const pcMsg)
{
    //断链处理

    COspSipMsg* pOspSipMsg = (COspSipMsg*)pcMsg->content;
    if (pOspSipMsg == NULL)
    {
        TASKLOG(CMU_MOD_CUI, ERROR_LEV, "ospsip 消息为空\n");
        return false;
    }

    //是否真断链
    TSipURI tDevUri;
    tDevUri.SetNOString(pOspSipMsg->GetMsgBody());
    if (tDevUri != GetDevSipUri())
    {
        TASKLOG(CMU_MOD_CUI, WARNING_LEV, "收到无效断链消息，断链设备[%s]\n", tDevUri.GetNOString());
        return false;
    }

    TASKLOG(CMU_MOD_CUI, CRITICAL_LEV, "cui[%s]和cmu断链\n", tDevUri.GetNOString());
    return true;
}

void CCuiList::PrintData()
{
    Iterator pPos;
    string strUri;
    CLoginSession* pTask = NULL;

    OspPrintf(TRUE, FALSE, "当前在线的cui：[%u]个\n", GetSize());    
    u32 i = 0;

    while (!pPos.End())
    {
        if (this->Iterate(pPos, strUri, pTask))
        {
            i++;
            OspPrintf(TRUE, FALSE, "  %4u. cui[%s]-session[%s]-taskNO[%u]\n", i, 
                strUri.c_str(), pTask->GetSession().c_str(), pTask->GetTaskNO());

			CCuiRegTask *pCuiRegTask=(CCuiRegTask*)pTask;
			OspPrintf(TRUE, FALSE, "\tCUI网段列表，Size[%d]：\n",
				pCuiRegTask->GetNetSegAllInfo().GetNetSegmentList().size());

			TNetSegmentList::const_iterator it = pCuiRegTask->GetNetSegAllInfo().GetNetSegmentList().begin();
			for(; it != pCuiRegTask->GetNetSegAllInfo().GetNetSegmentList().end(); it++)
			{
				OspPrintf(TRUE, FALSE, "\t\tNic[%s] IP[%s] Mask[%s]\n",
					it->GetNicName().c_str(), it->GetNetIp().c_str(), it->GetNetMask().c_str());
			}
			OspPrintf(TRUE, FALSE, "\t\tDefaultRouteIP:[%s]\n", pCuiRegTask->GetNetSegAllInfo().GetDafaultRoute().c_str());
        }
    }

    OspPrintf(TRUE, FALSE, "当前在线的cui：[%u]个\n", GetSize());
}
