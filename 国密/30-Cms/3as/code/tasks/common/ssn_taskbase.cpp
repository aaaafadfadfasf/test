#include "ssn_taskbase.h"
#include "query_task.h"
#include "long_query_task.h"
#include "tasks/user_devgrp_vidsrc_query_task.h"
#include "tasks/devgrp_vidsrc_totalnum_get_task.h"
#include "cctrl.h"
#include "config_util/tas_config.h"
#include "db_task_pool.h"
#include "tasks/g300_del_task.h"
#include "tasks/license_mng_task.h"
#include "tasks/devtree_add_task.h"
#include "tasks/devgrp_dev_add_task.h"
#include "tasks/devtree_devgrp_assign_task.h"
#include "tasks/devgrp_mod .h"
#include "tasks/devgrp2dev_mod.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "malloc_extension.h"
#include "cms/commonconfig/cmcfg.h"

extern CDbTaskPool g_cDbTaskPool;

static int parseLine(char* line){
    // This assumes that a digit will be found and the line ends in " Kb".
    int i = strlen(line);
    const char* p = line;
    while (*p <'0' || *p > '9') p++;
    line[i-3] = '\0';
    i = atoi(p);
    return i;
}

static int getVmsize(){ //Note: this value is in KB!
    FILE* file = fopen("/proc/self/status", "r");
    int result = -1;
    char line[128];

    while (fgets(line, 128, file) != NULL){
        if (strncmp(line, "VmSize:", 7) == 0){
            result = parseLine(line);
            break;
        }
    }
    fclose(file);
    return result;
}

static int getRsssize(){ //Note: this value is in KB!
    FILE* file = fopen("/proc/self/status", "r");
    int result = -1;
    char line[128];

    while (fgets(line, 128, file) != NULL){
        if (strncmp(line, "VmRSS:", 6) == 0){
            result = parseLine(line);
            break;
        }
    }
    fclose(file);
    return result;
}


CSsnTaskbase::CSsnTaskbase( CInstExt *pcInst ): CSipTask(pcInst)
{
    m_bHbSetResult = false;
    m_bDestroying=false;
    AppData().AddSsnId2TaskMap(m_tSessionId, this);
    m_dwPollCount = 0;
    m_dwPollDetectCount=0;
	m_bNeedDevGrpGbId=false;
}

CSsnTaskbase::~CSsnTaskbase()
{
    m_bDestroying = true;

    for (set<CQueryTask*>::iterator it = m_cQueryTaskSet.begin(); it != m_cQueryTaskSet.end(); ++it)
    {
        delete (*it);
    }

	for (set<CLongQueryTask*>::iterator it = m_cLongQueryTaskSet.begin(); it != m_cLongQueryTaskSet.end(); ++it)
	{
		delete (*it);
	}

	for (set<CUserDevGrpVidsrcQryTask*>::iterator it = m_cUserDevGrpVidsrcQryTaskSet.begin(); it != m_cUserDevGrpVidsrcQryTaskSet.end(); ++it)
	{
		delete (*it);
	}

	for (set<CDevGrpVidsrcTotalnumGetTask*>::iterator it = m_cDevGrpVidsrcNumGetTaskSet.begin(); it != m_cDevGrpVidsrcNumGetTaskSet.end(); ++it)
	{
		delete (*it);
	}

    AppData().DelSsnId2TaskMap(m_tSessionId);
    
    string strModuleUri = m_tClientUri.GetURIString();
    if (!strModuleUri.empty())
    {
        AppData().DelClientModuleUri(strModuleUri);
    }

    if (m_bHbSetResult)
    {
        OspSipStopHB(m_tClientUri);
    }

    if (!m_cChangeNotifyLst.empty())
    {
        m_cChangeNotifyLst.clear();

#ifndef _USEASAN_
        MallocExtension::instance()->ReleaseFreeMemory();
#endif
    }
}


CCtrlAppData& CSsnTaskbase::AppData()
{
    return static_cast<CCtrlInst*>(GetInstance())->GetAppData();
}

u32 CSsnTaskbase::PollChangeNotifyLst()
{
    if (++m_dwPollDetectCount < 60 && m_dwPollDetectCount%10==0)
    {
        if(getRsssize()>3145728) // rss> 3G
        {
            // tcmalloc时测试一天虚拟内存无法降下去，真的遇到这种情况让tas oom退出算了，不保护 getVmsize()>3565158 vm>3.4G
            // 测试遇到过tcmalloc虚拟内存和物理内存占用3G多，调用ReleaseFreeMemory后物理内存立即释放至367M但是2小时后
            //（再长时间未观察了）虚拟内存仍然不动，因此改为判断物理内存占用
            if(m_cChangeNotifyLst.size()>2000){
                Log(WARNING_LEV, "Tas's Rsssize > 3G, the task will be deleted(ntfnum:%d). module uri=%s\n", 
                    (s32)m_cChangeNotifyLst.size(), GetClientSipURI().GetURIString().c_str());

                return TIMERPOLL_DEL;
            }
        }else {
            if(m_cChangeNotifyLst.size()>1000){ // 表明系统负载比较大，尝试释放
#ifndef _USEASAN_
                MallocExtension::instance()->ReleaseFreeMemory();
#endif
            }
        }


        if (m_cChangeNotifyLst.size() > CTasConfig::GetSessionChangeNtfMaxNum())
        {
            Log(WARNING_LEV, "ChangeNotify message is too many, the task will be deleted(ntfnum:%d). module uri=%s\n", 
                (s32)m_cChangeNotifyLst.size(), GetClientSipURI().GetURIString().c_str());

            return TIMERPOLL_DEL; // 达到最大容量时直接删除任务
        }
    }

    if (++m_dwPollCount<60)
    {
        return TIMERPOLL_DONE;
    }
    return DoPollChangeNotifyLst();
}

u32 CSsnTaskbase::DoPollChangeNotifyLst()
{
    m_dwPollCount = 0;
    /*if (!m_cQueryTaskSet.empty() || m_cChangeNotifyLst.empty())
    {
        return TIMERPOLL_DONE;
    }*/
    // 由于CU那边一个req的处理不合理地依赖于ntf了，得不到及时的回复CU处理流程会挂起；
    // 为了让CU不改动，NTF发送不等查询任务了，这样可能导致数据获取者得到不正确的数据
    if (m_cChangeNotifyLst.empty())
    {
        return TIMERPOLL_DONE;
    }

    TChangeNtfItem &tItem = m_cChangeNotifyLst.front();
    if (PROCMSG_OK != PostMsgReq(tItem, m_tClientUri))
    {
        Log(WARNING_LEV, "CSsnTaskbase post ChangeNotify message failed");
    }

    return TIMERPOLL_DONE;
}

void CSsnTaskbase::SaveNewChangeNotify(u16 wMsgId,TChangeNtfItem &cItem)
{
    if (ChangeNotifyFilter(wMsgId, cItem))
    {
        RelaceSessionId(cItem, m_tSessionId);

		// 丑陋的特殊处理，过滤掉设备组名里面的国标编码 [10/16/2015 pangubing]
		if ((TAS_DEVICE_GROUP_ADD_NTF_REQ==wMsgId||TAS_DEVICE_GROUP_MOD_NTF_REQ==wMsgId)
			&&!GetIsNeedDevGrpGbId())
		{
			GLOBELOG(EVENT_LEV, "OnChangeNtfReq m_tClientUri[%s]need cut group gb id.\n",m_tClientUri.GetURIString().c_str());
			TChangeNtfItem cNewItem; 
			if (TAS_DEVICE_GROUP_ADD_NTF_REQ==wMsgId)
			{
				CDeviceGroupAddNtfReq cNtfReq;
				cNtfReq.FromXml(cItem);
				CDeviceGroupInfo &info=cNtfReq.GetDeviceGroupInfo();
				info.GetDeviceGroupName()=CutDevGrpGbId(info.GetDeviceGroupName());
				cNewItem = cNtfReq.ToXml();
			} 
			else if(TAS_DEVICE_GROUP_MOD_NTF_REQ==wMsgId)
			{
				CDeviceGroupModNtfReq cNtfReq;
				cNtfReq.FromXml(cItem);
				CDeviceGroupInfo &info=cNtfReq.GetDeviceGroupInfo();
				info.GetDeviceGroupName()=CutDevGrpGbId(info.GetDeviceGroupName());
				cNewItem = cNtfReq.ToXml();
			}

            bool bFirstOne=m_cChangeNotifyLst.empty();
			m_cChangeNotifyLst.push_back(cNewItem);
			if (bFirstOne) DoPollChangeNotifyLst();
		}
		else
		{
            bool bFirstOne=m_cChangeNotifyLst.empty();
			m_cChangeNotifyLst.push_back(cItem);
			if (bFirstOne) DoPollChangeNotifyLst();
		}

    }

}

void CSsnTaskbase::OnChangeNtfReq( const CTaskMsgTransferBase* pcMsg )
{
	string strMsg;

	const CSipEvent *pcEvent = reinterpret_cast<const CSipEvent*>(pcMsg->GetMsgObjAddr());
	strMsg = pcEvent->ToXml();
	
    AppData().HandleChangeNtfReq(pcMsg->GetEventId(), strMsg);
}

void CSsnTaskbase::OnChangeNtfRsp( CMessage *const pcMsg )
{
    COspSipMsg *pcSipMsg = reinterpret_cast<COspSipMsg*>(pcMsg->content);
    CNtfRsp cRsp;
    pcSipMsg->GetMsgBody(cRsp);

    HandleChangeNtfRsp(cRsp.GetSeqNum());
}

void CSsnTaskbase::HandleChangeNtfRsp( s32 nSeqNo )
{
    if (!m_cChangeNotifyLst.empty() && 
        nSeqNo == atoi(GetSipSeqNO(m_cChangeNotifyLst.front().c_str()).c_str()))
    {
        m_cChangeNotifyLst.pop_front();
        DoPollChangeNotifyLst();
    }
}

void CSsnTaskbase::OnQryReq( CMessage *const pcMsg )
{

	if (TAS_CUSER_DEVICE_QRY_REQ == pcMsg->event)
	{
		CLongQueryTask *pcLongTask = new CLongQueryTask(this);
		pcLongTask->InitStateMachine();
		if (PROCMSG_OK == pcLongTask->ProcMsg(pcMsg))
		{
			m_cLongQueryTaskSet.insert(pcLongTask);
		}
		else
		{
			delete pcLongTask;
		}
	}
	else if (TAS_CUSER_DEVGRP_VIDSRC_QRY_REQ == pcMsg->event)
	{
		CUserDevGrpVidsrcQryTask *pcNoNumQryTask = new CUserDevGrpVidsrcQryTask(this);
		pcNoNumQryTask->InitStateMachine();
		if (PROCMSG_OK == pcNoNumQryTask->ProcMsg(pcMsg))
		{
			m_cUserDevGrpVidsrcQryTaskSet.insert(pcNoNumQryTask);
		}
		else
		{
			delete pcNoNumQryTask;
		}
	}
	else if(TAS_DEVGRP_VIDSRC_TOTALNUM_GET_REQ == pcMsg->event)
	{
		CDevGrpVidsrcTotalnumGetTask *pTask = new CDevGrpVidsrcTotalnumGetTask(this);
		pTask->InitStateMachine();
		if (PROCMSG_OK == pTask->ProcMsg(pcMsg))
		{
			m_cDevGrpVidsrcNumGetTaskSet.insert(pTask);
		}
		else
		{
			delete pTask;
		}
	}
	else
	{
		CQueryTask *pcTask = new CQueryTask(this);
		pcTask->InitStateMachine();

		if (PROCMSG_OK == pcTask->ProcMsg(pcMsg))
		{
			m_cQueryTaskSet.insert(pcTask);
		}
		else
		{
			delete pcTask;
		}
	}
}

u32 CSsnTaskbase::OnLogout( CMessage *const pcMsg )
{
    COspSipMsg *pcSipMsg = reinterpret_cast<COspSipMsg*>(pcMsg->content);
    CLogoutTasReq cReq;
    pcSipMsg->GetMsgBody(cReq);
    CLogoutTasRsp cRsp;
    CopySeqnoAndSsnid(cReq, cRsp);
    PostMsgRsp(pcSipMsg->GetSipTransID(), cRsp);

    Log(CRITICAL_LEV, "Logout the session, the task will be deleted. module uri=%s\n", 
        GetClientSipURI().GetURIString().c_str());
    return PROCMSG_DEL;
}

u32 CSsnTaskbase::OnDisconnect( CMessage *const pcMsg )
{
    m_bHbSetResult = false; // 断链之后底层自动删除了心跳记录，上层无需自行删除
    Log(WARNING_LEV, "Disconnection happened in the session, the task will be deleted. module uri=%s\n", 
        GetClientSipURI().GetURIString().c_str());
    return PROCMSG_DEL;
}

u32 CSsnTaskbase::OnSipMsgProcFail( CMessage *const pcMsg )
{
    COspSipMsg *pcSipMsg = reinterpret_cast<COspSipMsg*>(pcMsg->content);

    // 检查是否为对端不认识的变更通知
    if (KDSIP_RESPONSE_403_FORBIDDEN == pcSipMsg->GetSipErrorCode())
    {
        Log(EVENT_LEV, "recved 403 forbidden, seqno=%u\n", pcSipMsg->GetOspTransID().dwSeqNO);
        HandleChangeNtfRsp(pcSipMsg->GetOspTransID().dwSeqNO);
    }

    // 服务端一般不负责重传等，SIP消息出错就出错
    return PROCMSG_OK;
}

void CSsnTaskbase::ReverseDeleteQueryTaskRecord( CQueryTask* pcTask )
{
    // 正在销毁时无需进行后续处理
    if (m_bDestroying)
    {
        return;
    }

    m_cQueryTaskSet.erase(pcTask);
    /*
    if (m_cQueryTaskSet.empty())
    {
        DoPollChangeNotifyLst();
    }
    */
    // 由于CU那边一个req的处理不合理地依赖于ntf了，得不到及时的回复CU处理流程会挂起；
    // 为了让CU不改动，NTF发送不等查询任务了，这样可能导致数据获取者得到不正确的数据
}

void CSsnTaskbase::ReverseDeleteQueryTaskRecord( CLongQueryTask* pcTask )
{
	// 正在销毁时无需进行后续处理
    if (m_bDestroying)
    {
        return;
    }

    m_cLongQueryTaskSet.erase(pcTask);
}

void CSsnTaskbase::ReverseDeleteQueryTaskNoNumRecord( CUserDevGrpVidsrcQryTask* pcTask )
{
	// 正在销毁时无需进行后续处理
	if (m_bDestroying)
	{
		return;
	}

	m_cUserDevGrpVidsrcQryTaskSet.erase(pcTask);
}

void CSsnTaskbase::ReverseDelGetDevGrpVidSrcNumRecord( CDevGrpVidsrcTotalnumGetTask* pcTask )
{
	// 正在销毁时无需进行后续处理
	if (m_bDestroying)
	{
		return;
	}

	m_cDevGrpVidsrcNumGetTaskSet.erase(pcTask);
}

TSipURI CSsnTaskbase::GetClientSipURI() const
{
    return m_tClientUri;
}

void CSsnTaskbase::SetClientSipURI( const TSipURI& tUri )
{
    m_tClientUri = tUri; 
    AppData().AddClientModuleUri(tUri.GetURIString());
}

#define DECODE_SIP_AND_POST2DbTaskPool(msgid, msgtype) \
    case msgid:\
{\
    CTaskMsgTransfer<msgtype> cTrans(GetTaskNO(), pcSipMsg->GetSipTransID(), pcMsg->event);\
    pcSipMsg->GetMsgBody(cTrans.GetMsgObj());\
    if (!PostTaskMsgTransfer2DbTaskPool(cTrans))\
    {\
    CEventRsp cRsp;\
    cRsp.SetEvent(pcMsg->event+1);\
    cRsp.SetErrorCode(ERR_TAS_SYSTEM_BUSY);\
    PostMsgRsp(pcSipMsg->GetSipTransID(), cRsp);\
    }\
}break

u32 CSsnTaskbase::DefaultProc( CMessage *const pcMsg )
{
    COspSipMsg *pcSipMsg = reinterpret_cast<COspSipMsg*>(pcMsg->content);

    /* 请求消息处理，包括增删改请求和不带Ntf的查询请求 */
    bool bHandled = true;
    switch (pcMsg->event)
    {
    case TAS_LOGOUT_REQ:
        return OnLogout(pcMsg);
        break;
    case OSP_SIP_DISCONNECT:
        return OnDisconnect(pcMsg);
        break;
    case OSP_SIP_MSG_PROC_FAIL:
        return OnSipMsgProcFail(pcMsg);
		break;
	case TAS_TRANSACTION_PROGRESS_GET_REQ:
		{
			COspSipMsg *pcSipMsg = (COspSipMsg *)pcMsg->content;
			CTransactionProgressGetReq cReq;
			pcSipMsg->GetMsgBody(cReq);

			CTransactionProgressGetRsp cRsp;
			cRsp.SetProgress(g_cDbTaskPool.GetTransProgress(cReq.GetOperUuid()));
			cRsp.SetErrorCode(g_cDbTaskPool.GetTransProErr(cReq.GetOperUuid()));
			PostMsgRsp(pcSipMsg->GetSipTransID(), cRsp);

			CLASSLOG(enLogSrcCCtrl, PROGRESS_LEV, "【进度查询】TransTag[%s]，Progress[totalNum:%d, finishNum:%d, time:%d]\n", 
				cReq.GetOperUuid().c_str(), cRsp.GetProgress().totalNum, cRsp.GetProgress().finishNum, cRsp.GetProgress().startTime);
		}
        break;
	case TAS_GATEWAY_DEL_REQ:
		{
			CG300DelTask *pcTask = new CG300DelTask(GetInstance());
			pcTask->InitStateMachine();

			u32 dwRet = pcTask->ProcMsg(pcMsg);
			if (PROCMSG_DEL == dwRet)
			{
				delete pcTask;
				pcTask = NULL;
			}

			return PROCMSG_OK;
		}
		break;
	case TAS_LICENSE_GET_REQ:
        {
            CLicenseMngTask *pcTask = new CLicenseMngTask(GetInstance());
            pcTask->InitStateMachine();

            u32 dwRet = pcTask->ProcMsg(pcMsg);
            if (PROCMSG_DEL == dwRet)
            {
                delete pcTask;
            }

            return PROCMSG_OK;
        }
        break;
	case TAS_DEVTREE_ADD_REQ:
		{
			CDevtreeAddTask *pcTask = new CDevtreeAddTask(GetInstance());
			pcTask->InitStateMachine();

			u32 dwRet = pcTask->ProcMsg(pcMsg);
			if (PROCMSG_DEL == dwRet)
			{
				delete pcTask;
				pcTask = NULL;
			}

			return PROCMSG_OK;
		}
		break;
	case TAS_DEVICE_GROUP_DEVICE_ADD_REQ:
		{
			CDevgrpDevAddTask *pcTask = new CDevgrpDevAddTask(GetInstance());
			pcTask->InitStateMachine();

			u32 dwRet = pcTask->ProcMsg(pcMsg);
			if (PROCMSG_DEL == dwRet)
			{
				delete pcTask;
				pcTask = NULL;
			}

			return PROCMSG_OK;
		}
		break;
	case TAS_DEVTREE_DEVGROUP_ASSIGN_REQ:
		{
			CDevtreeDevgrpAssignTask *pcTask = new CDevtreeDevgrpAssignTask(GetInstance());
			pcTask->InitStateMachine();

			u32 dwRet = pcTask->ProcMsg(pcMsg);
			if (PROCMSG_DEL == dwRet)
			{
				delete pcTask;
				pcTask = NULL;
			}
			return PROCMSG_OK;
		}
		break;
	case TAS_DEVICE_GROUP_MOD_4GB_REQ:
		{
			CTask* pcTask = new CDevGrpModTask(this);
			pcTask->InitStateMachine();
			u32 dwRet = pcTask->ProcMsg(pcMsg);
			if (PROCMSG_DEL == dwRet)
			{
				delete pcTask;
				pcTask = NULL;
			}
			return PROCMSG_OK;
		}
		break;
	case TAS_DEVICE_GROUP_DEVICE_MOD_4GB_REQ:
		{
			CTask* pcTask = new CDevGrp2DevModTask(this);
			pcTask->InitStateMachine();
			if (PROCMSG_DEL == pcTask->ProcMsg(pcMsg))
			{
				delete pcTask;
				return PROCMSG_DEL;
			}
			return PROCMSG_OK;
		}
		break;
		DECODE_SIP_AND_POST2DbTaskPool(TAS_DEVICE_GBID_ERROR_GET_REQ,CDeviceGbidErrGetReq);
		DECODE_SIP_AND_POST2DbTaskPool(TAS_DEVICE_MATCH_GBID_REQ,CDeviceMatchGbidReq);//分组匹配
		DECODE_SIP_AND_POST2DbTaskPool(TAS_DEVICE_GROUP_DEVICE_SUMNUM_REQ, CDeviceGrpDevSumNumReq);//分组统计
        DECODE_SIP_AND_POST2DbTaskPool(TAS_DEVICE_ADD_REQ, CDeviceAddReq);
        DECODE_SIP_AND_POST2DbTaskPool(TAS_DEVICE_DEL_REQ, CDeviceDelReq);
        DECODE_SIP_AND_POST2DbTaskPool(TAS_DEVICE_MOD_REQ, CDeviceModReq);
        DECODE_SIP_AND_POST2DbTaskPool(TAS_TVWALL_ADD_REQ, CTvwallAddReq);
        DECODE_SIP_AND_POST2DbTaskPool(TAS_TVWALL_DEL_REQ, CTvwallDelReq);
        DECODE_SIP_AND_POST2DbTaskPool(TAS_TVWALL_MOD_REQ, CTvwallModReq);
        DECODE_SIP_AND_POST2DbTaskPool(TAS_DEVICE_MODEL_ADD_REQ, CDeviceModelAddReq);
        DECODE_SIP_AND_POST2DbTaskPool(TAS_DEVICE_MODEL_DEL_REQ, CDeviceModelDelReq);
        DECODE_SIP_AND_POST2DbTaskPool(TAS_DEVICE_CAP_INDEX_NAME_GET_REQ, CDeviceCapIndexNameGetReq);
        DECODE_SIP_AND_POST2DbTaskPool(TAS_DEVICE_CAP_INDEX_NAME_SET_REQ, CDeviceCapIndexNameSetReq);
        DECODE_SIP_AND_POST2DbTaskPool(TAS_DEVICE_MANUFACTURER_ADD_REQ, CDeviceManufacturerAddReq);
        DECODE_SIP_AND_POST2DbTaskPool(TAS_DEVICE_MANUFACTURER_DEL_REQ, CDeviceManufacturerDelReq);
        DECODE_SIP_AND_POST2DbTaskPool(TAS_CUSER_ADD_REQ, CUserAddReq);
        DECODE_SIP_AND_POST2DbTaskPool(TAS_CUSER_DEL_REQ, CUserDelReq);
        DECODE_SIP_AND_POST2DbTaskPool(TAS_CUSER_MOD_REQ, CUserModReq);
        DECODE_SIP_AND_POST2DbTaskPool(TAS_CUSER_DEVICE_ADD_REQ, CUserDeviceAddReq);
        DECODE_SIP_AND_POST2DbTaskPool(TAS_CUSER_DEVICE_DEL_REQ, CUserDeviceDelReq);
        DECODE_SIP_AND_POST2DbTaskPool(TAS_CUSER_DEVICE_MOD_REQ, CUserDeviceModReq);
        DECODE_SIP_AND_POST2DbTaskPool(TAS_DEVICE_GROUP_ADD_REQ, CDeviceGroupAddReq);
        DECODE_SIP_AND_POST2DbTaskPool(TAS_DEVICE_GROUP_DEL_REQ, CDeviceGroupDelReq);
        DECODE_SIP_AND_POST2DbTaskPool(TAS_DEVICE_GROUP_MOD_REQ, CDeviceGroupModReq);
        DECODE_SIP_AND_POST2DbTaskPool(TAS_DEVICE_GROUP_DEVICE_DEL_REQ, CDeviceGroupDeviceDelReq);
        DECODE_SIP_AND_POST2DbTaskPool(TAS_CUSER_GROUP_ADD_REQ, CUserGroupAddReq);
        DECODE_SIP_AND_POST2DbTaskPool(TAS_CUSER_GROUP_DEL_REQ, CUserGroupDelReq);
        DECODE_SIP_AND_POST2DbTaskPool(TAS_CUSER_GROUP_MOD_REQ, CUserGroupModReq);
        DECODE_SIP_AND_POST2DbTaskPool(TAS_CUSER_GROUP_DEVICE_ADD_REQ, CUserGroupDeviceAddReq);
        DECODE_SIP_AND_POST2DbTaskPool(TAS_CUSER_GROUP_DEVICE_DEL_REQ, CUserGroupDeviceDelReq);
        DECODE_SIP_AND_POST2DbTaskPool(TAS_CUSER_GROUP_DEVICE_MOD_REQ, CUserGroupDeviceModReq);
        DECODE_SIP_AND_POST2DbTaskPool(TAS_DOMAIN_NAME_GET_REQ, CDomainNameGetReq);
		DECODE_SIP_AND_POST2DbTaskPool(TAS_CUSER_DEVICE_GROUP_ADD_REQ, CUserDeviceGroupAddReq);
		DECODE_SIP_AND_POST2DbTaskPool(TAS_CUSER_DEVICE_GROUP_DEL_REQ, CUserDeviceGroupDelReq);
		DECODE_SIP_AND_POST2DbTaskPool(TAS_CUSER_GROUP_DEVICE_GROUP_ADD_REQ, CUserGroupDeviceGroupAddReq);
		DECODE_SIP_AND_POST2DbTaskPool(TAS_CUSER_GROUP_DEVICE_GROUP_DEL_REQ, CUserGroupDeviceGroupDelReq);
		DECODE_SIP_AND_POST2DbTaskPool(TAS_DEVICE_AUTO_ASSIGN_TO_USER_SET_REQ, CDeviceAutoAssignToUserSetReq);
		DECODE_SIP_AND_POST2DbTaskPool(TAS_DEVICE_AUTO_ASSIGN_TO_USER_GROUP_SET_REQ, CDeviceAutoAssignToUserGroupSetReq);
        DECODE_SIP_AND_POST2DbTaskPool(TAS_CUSER_DATA_VERSION_GET_REQ, CCuserDataVersionGetReq);
		DECODE_SIP_AND_POST2DbTaskPool(TAS_DEVTREE_DEL_REQ, CDevTreeDelReq);
		DECODE_SIP_AND_POST2DbTaskPool(TAS_DEVTREE_MOD_REQ, CDevTreeModReq);
		DECODE_SIP_AND_POST2DbTaskPool(TAS_DEV_CAPINFO_MOD_REQ, CDevCapInfoModReq);
		DECODE_SIP_AND_POST2DbTaskPool(TAS_PTZLOCK_ADD_REQ, CPtzLockAddReq);
		DECODE_SIP_AND_POST2DbTaskPool(TAS_PTZLOCK_DEL_REQ, CPtzLockDelReq);
		DECODE_SIP_AND_POST2DbTaskPool(TAS_DEVICE_GROUP_DEVICE_ORDER_REQ, CDeviceGroupDeviceOrderReq);
    default:
        bHandled = false;
        break;
    }
    if (bHandled) return PROCMSG_OK;

    /* 带Ntf的查询请求 */
    if (IsQryReqType(pcMsg->event))
    {
        OnQryReq(pcMsg);
        return PROCMSG_OK;
    }

    if (msgTaskMsgTransfer4Db == pcMsg->event)
    {
        ptrint ptr = *reinterpret_cast<ptrint*>(pcMsg->content);
        const CTaskMsgTransferBase* pcMsgTrans = reinterpret_cast<const CTaskMsgTransferBase*>(ptr);

        if (IsRspType(pcMsgTrans->GetEventId()))
        {
            // 应答消息，直接发给请求者
            string strMsg(reinterpret_cast<const CEventRsp*>(pcMsgTrans->GetMsgObjAddr())->ToXml());
            PostMsgRsp(pcMsgTrans->GetSipTransId(), strMsg);
        }
        else
        {
            // 通知消息
            CMS_ASSERT(IsNtfReqType(pcMsgTrans->GetEventId()));
            OnChangeNtfReq(pcMsgTrans);
        }
        return PROCMSG_OK;
    }

    if (IsNtfRspType(pcMsg->event))
    {
        // 此处只可能是变更通知(包括增删改通知)的应答, 查询通知的应答在CCtrl中直接丢给相应query_task了
        OnChangeNtfRsp(pcMsg);
        return PROCMSG_OK;
    }
    // 其余SIP应答消息，在TAS作为客户端时才可能收到，也即他们将在conn_task类中被处理

    Log(WARNING_LEV, "CSsnTaskbase::DefaultProc recved unknown msg, [%u-%s]\n", 
        pcMsg->event, OspExtEventDesc(pcMsg->event).c_str());
    return PROCMSG_FAIL;
}

void CSsnTaskbase::PrintData() const
{
    CTask::PrintData();

    TelePrint("ModuleUri:%s", m_tClientUri.GetURIString().c_str());
    TelePrint("HbSet result:%s", m_bHbSetResult? "success":"failed");
    TelePrint("SessId:%s", m_tSessionId.c_str());
    TelePrint("QueryTaskSet size:%u", m_cQueryTaskSet.size());
	TelePrint("LongQueryTaskSet size:%u", m_cLongQueryTaskSet.size());
	TelePrint("m_cUserDevGrpVidsrcQryTaskSet size:%u",m_cUserDevGrpVidsrcQryTaskSet.size());
	TelePrint("m_cDevGrpVidsrcNumGetTaskSet size:%u",m_cDevGrpVidsrcNumGetTaskSet.size());
    TelePrint("ChangeNotifyLst size:%u", m_cChangeNotifyLst.size());
}

bool CSsnTaskbase::SetHb()
{
    if (m_bHbSetResult)
    {
        CMS_ASSERT(false);
        return false;
    }

    m_bHbSetResult = SetHBParam(m_tClientUri);
    if (!m_bHbSetResult) Log(ERROR_LEV, "CSsnTaskbase::SetHb failed");
    return m_bHbSetResult;
}

bool CSsnTaskbase::ChangeNotifyFilter( u16 wMsgId, TChangeNtfItem &cItem )
{
    ENTasResourceTypes enResourceType;
	ENTasResourceTypes enResourceTypeSub=TasResourceEnd;
    switch(wMsgId)
    {
    case TAS_DEVICE_ADD_NTF_REQ:
		{
			enResourceTypeSub = TasResourceGeneralDevAdd;
			enResourceType = TasResourceGeneralDev;
			if (m_cNtfFilterTypes.find(TasResourceIgnoreGbChildDomainDevChg) != m_cNtfFilterTypes.end())
			{
				//需要过滤国标子域设备变更信息
				CDeviceAddNtfReq cNtfReq;
				cNtfReq.FromXml(cItem);
				if (cNtfReq.GetDeviceInfo().GetCreater()==CREATE_BY_GBS_FOREIGN_DOMAIN 
					&& cNtfReq.GetDeviceInfo().GetDomainUUID() == GetDomainUUID())
				{
					return false;
				}
			}
			break;
		}
    case TAS_DEVICE_DEL_NTF_REQ:
		{
			enResourceTypeSub = TasResourceGeneralDevDel;
			enResourceType = TasResourceGeneralDev;
			if (m_cNtfFilterTypes.find(TasResourceIgnoreGbChildDomainDevChg) != m_cNtfFilterTypes.end())
			{
				//需要过滤国标子域设备变更信息
				CDeviceDelNtfReq cNtfReq;
				cNtfReq.FromXml(cItem);
				if (cNtfReq.GetCreater()==CREATE_BY_GBS_FOREIGN_DOMAIN 
					&& cNtfReq.GetDomainId() == GetDomainUUID())
				{
					return false;
				}
			}
			break;
		}
    case TAS_DEVICE_MOD_NTF_REQ:
        {
            enResourceTypeSub = TasResourceGeneralDevMod;
			enResourceType = TasResourceGeneralDev;
			if (m_cNtfFilterTypes.find(TasResourceIgnoreGbChildDomainDevChg) != m_cNtfFilterTypes.end())
			{
				//需要过滤国标子域设备变更信息
				CDeviceModNtfReq cNtfReq;
				cNtfReq.FromXml(cItem);
				if (cNtfReq.GetDeviceInfo().GetCreater()==CREATE_BY_GBS_FOREIGN_DOMAIN 
					&& cNtfReq.GetDeviceInfo().GetDomainUUID() == GetDomainUUID())
				{
					return false;
				}
			}
            break;
        }
    case TAS_TVWALL_ADD_NTF_REQ:
    case TAS_TVWALL_DEL_NTF_REQ:
    case TAS_TVWALL_MOD_NTF_REQ:
        {
            enResourceType = TasResourceTvwallDev;
            break;
        }
    case TAS_DEVICE_MODEL_ADD_NTF_REQ:
    case TAS_DEVICE_MODEL_DEL_NTF_REQ:
        {
            enResourceType = TasResourceDevModel;
            break;
        }
    case TAS_DEVICE_CAP_INDEX_NAME_MOD_NTF_REQ:
        {
            enResourceType = TasResourceDevCapIndexName;
            break;
        }
    case TAS_CUSER_ADD_NTF_REQ:
    case TAS_CUSER_DEL_NTF_REQ:
    case TAS_CUSER_MOD_NTF_REQ:
        {
            enResourceType = TasResourceCuser;
            break;
        }
    case TAS_CUSER_DEVICE_ADD_NTF_REQ:
    case TAS_CUSER_DEVICE_DEL_NTF_REQ:
    case TAS_CUSER_DEVICE_MOD_NTF_REQ:
        {
            enResourceType = TasResourceCuser2Dev;
            break;
        }
    case TAS_DEVICE_GROUP_ADD_NTF_REQ:
    case TAS_DEVICE_GROUP_DEL_NTF_REQ:
    case TAS_DEVICE_GROUP_MOD_NTF_REQ:
        {
            enResourceType = TasResourceDevGrp;
            break;
        }
    case TAS_DEVICE_GROUP_DEVICE_ADD_NTF_REQ:
    case TAS_DEVICE_GROUP_DEVICE_DEL_NTF_REQ:
        {
            enResourceType = TasResourceDevGrp2Dev;
            break;
        }
    case TAS_CUSER_GROUP_ADD_NTF_REQ:
    case TAS_CUSER_GROUP_DEL_NTF_REQ:
    case TAS_CUSER_GROUP_MOD_NTF_REQ:
        {
            enResourceType = TasResourceCuserGrp;
            break;
        }
    case TAS_CUSER_GROUP_DEVICE_ADD_NTF_REQ:
    case TAS_CUSER_GROUP_DEVICE_DEL_NTF_REQ:
    case TAS_CUSER_GROUP_DEVICE_MOD_NTF_REQ:
        {
            enResourceType = TasResourceCuserGrp2Dev;
            break;
        }
    case TAS_DOMAIN_RELATION_ADD_NTF_REQ:
    case TAS_DOMAIN_RELATION_DEL_NTF_REQ:
    case TAS_DOMAIN_RELATION_MOD_NTF_REQ:
        {
            enResourceType = TasResourceDomainRelation;
            break;
        }
    case TAS_TACUSER_ADD_NTF_REQ:
    case TAS_TACUSER_DEL_NTF_REQ:
    case TAS_TACUSER_MOD_NTF_REQ:
        {
            enResourceType = TasResourceTuser;
            break;
        }
    case TAS_SYNCDSTDOMAIN_ADD_NTF_REQ:
    case TAS_SYNCDSTDOMAIN_DEL_NTF_REQ:
    case TAS_SYNCDSTDOMAIN_MOD_NTF_REQ:
        {
            enResourceType = TasResourceSyncDstDomainCfg;
            break;
        }
    case TAS_TEMP_AUTHORIZATION_ADD_NTF_REQ:
    case TAS_TEMP_AUTHORIZATION_DEL_NTF_REQ:
    case TAS_TEMP_AUTHORIZATION_MOD_NTF_REQ:
        {
            enResourceType = TasResourceTmpAuthorizationCfg;
            break;
        }
	case TAS_PTZLOCK_ADD_NTF_REQ:
	case TAS_PTZLOCK_DEL_NTF_REQ:
		{
			enResourceType = TasResourcePtzLock;
			break;
		}
    case TAS_LICENSE_UPDATE_NTF_REQ:
        {
            enResourceType = TasLicenseUpdateNotify;
            break;
        }
    default:
        return false;
    }

    return m_cNtfFilterTypes.end() != m_cNtfFilterTypes.find(enResourceType)||m_cNtfFilterTypes.end() != m_cNtfFilterTypes.find(enResourceTypeSub);
}
