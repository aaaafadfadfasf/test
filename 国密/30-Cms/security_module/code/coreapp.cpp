#include "coreapp.h"

CCoreApp *g_pcCoreApp;

extern SEMHANDLE g_hSecurityModuleExitSem;

TSecurityStat g_tSecurityStat;

CCoreInst::CCoreInst()
{

}



CCoreInst::~CCoreInst()
{

}



LPCSTR CCoreInst::GetStrState() const
{
	switch(CurState())
	{
	case enStateIdle:
		return "enStateIdle";
	case enStateServer:
		return "enStateServer";
	default:
		return "未知状态";
	}
}



 void CCoreInst::DaemonEntry(CMessage* const pcMsg)
 {
	 
	 switch(CurState())
	 {
	 case enStateIdle:
		 {
			 IdleStateHandle(pcMsg);
		 }
		 break;
	 case enStateServer:
		 {
			 ServerStateHandle(pcMsg);
		 }
		 break;
	 case DBIX_RESULT_CB:
		 {
			 CDbixMsg* pcDbixMsg = (CDbixMsg*)pcMsg->content;
			 if (pcDbixMsg == NULL)
			 {
				 INSTLOG(ERROR_LEV, "Dbix 消息为空\n");
				 return;
			 }

			 CTask *pcTask = FindTask(pcDbixMsg->GetOspTaskNO());
			 if (pcTask != NULL)
			 {
				 ProcTaskMsg(pcTask, pcMsg);
			 }
			 else
			 {
				 INSTLOG(ERROR_LEV, "taskNO[%u]没有对应事务---msg[%s-%u]\n", 
					 pcDbixMsg->GetOspTaskNO(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
			 }
		 }
		 break;

	 default:
		 {            
			 INSTLOG(WARNING_LEV, "unkown state recv Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
		 }
		 break;
	 }   

 }


 void CCoreInst::IdleStateHandle(CMessage* const pcMsg)
 {
	 switch(pcMsg->event)
	 {
	 case ModulePowon:
		 {
			 g_pcDelVkekTask = new CDelVkekTask(this);
			 g_pcDelVkekTask->InitStateMachine();

			 g_tSecurityStat.strStartTime = COspTime::GetCurrStrTime();
			 RegSipProxy();
			 NextState(enStateServer);	 
		 }
		 break;
	
	 default:
		 {
			 INSTLOG(WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
		 }
		 break;
	 }
 }


 void CCoreInst::ServerStateHandle(CMessage* const pcMsg)
 {
	 INSTLOG(EVENT_LEV, "收到消息 event[%s-%d]\n\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

	 COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
	 CTask *pcTask = NULL;

	 switch(pcMsg->event)
	 {
	case OSP_PROXY_CONNECT:
		{
			INSTLOG(EVENT_LEV,"已连接上proxy!\n");
			g_tSecurityStat.bIsRegSipProxy = true;
		}
		break;
	case OSP_PROXY_DISCONNECT:
		{
			INSTLOG(EVENT_LEV,"\n\n Proxy已断链\n\n");
			g_tSecurityStat.bIsRegSipProxy = false;
		}
		break;
	case ModulePowoff:
		{
			OspSemGive(g_hSecurityModuleExitSem);
		}
		break;
	case SECURITY_MODULE_NEW_VKEK_REQ:
	case SECURITY_MODULE_VKEK_SAVE_REQ:
	case SECURITY_MODULE_VKEK_QUERY_REQ:
	case SECURITY_MODULE_CERTIFICATE_QUERY_REQ:
	case SECURITY_MODULE_NVKEK_QUERY_REQ:
	case SECURITY_MODULE_CERTIFICATE_SAVE_REQ:
	case SECURITY_MODULE_CERTIFICATE_GET_REQ:
		{
			pcTask = CreateTask(pcMsg->event);
			if (pcTask == NULL)
			{
				INSTLOG(ERROR_LEV, "事务创建失败Msg[%s-%d]\n",
					OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
				return;
			}

			ProcTaskMsg(pcTask, pcMsg);
		}
		break;
	case DBIX_RESULT_CB:
		{
			CDbixMsg* pcDbixMsg = (CDbixMsg*)pcMsg->content;
			if (pcDbixMsg == NULL)
			{
				INSTLOG(ERROR_LEV, "Dbix 消息为空\n");
				return;
			}

			pcTask = FindTask(pcDbixMsg->GetOspTaskNO());
			if (pcTask != NULL)
			{
				ProcTaskMsg(pcTask, pcMsg);
			}
			else
			{
				INSTLOG(ERROR_LEV, "taskNO[%u]没有对应事务---msg[%s-%u]\n", 
					pcDbixMsg->GetOspTaskNO(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
			}
		}
		break;
	default:
		{
			INSTLOG(WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
		}
		break;
	 }
 }

	 
 //事务创建
 CTask* CCoreInst::CreateTask(u16 wEvent)
 {
	 CTask* pcTask = NULL;

	 switch(wEvent)
	 {
	 case SECURITY_MODULE_NEW_VKEK_REQ:
		 {
			pcTask = new CVkekCreateTask(this);
		 }
		 break;
	 case SECURITY_MODULE_VKEK_SAVE_REQ:
		 {
			 pcTask = new CVkekSaveTask(this);
		 }
		 break;
	 case SECURITY_MODULE_VKEK_QUERY_REQ:
		 {
			 pcTask = new CVkekQueryTask(this);
		 }
		 break;
	 case SECURITY_MODULE_CERTIFICATE_QUERY_REQ:
		 {
			 pcTask = new CCertificateQueryTask(this);
		 }
		 break;
	 case SECURITY_MODULE_NVKEK_QUERY_REQ:
		 {
			 pcTask = new CNVkekQueryTask(this);
		 }
		 break;
	 case SECURITY_MODULE_CERTIFICATE_SAVE_REQ:
		 {
			pcTask = new CCertificateSaveTask(this);
		 }
		break;
	 case SECURITY_MODULE_CERTIFICATE_GET_REQ:
		{
		 pcTask = new CCertificateGetTask(this);
		}
	 default:
		 {
			 INSTLOG(ERROR_LEV, "Msg[%s-%d]创建事务失败\n", 
				 OspExtEventDesc(wEvent).c_str(), wEvent);
		 }
		 break;
	 }
	 if (pcTask != NULL)
	 {
		 pcTask->InitStateMachine();
	 }

	 return pcTask;
 }



 void CCoreInst::ProcTaskMsg(CTask* pTask, CMessage* const pcMsg)
 {
	 if (pTask == NULL)
	 {
		 INSTLOG(ERROR_LEV, "pTask == NULL---msg[%s-%u]\n", 
			 OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
		 return;
	 }

	 u32 dwRet = pTask->ProcMsg(pcMsg);
	 if (dwRet == PROCMSG_DEL)
	 {
		 INSTLOG(ALL_LEV, "事务[%s-%u]处理后需要删除---msg[%s-%u]\n", 
			 pTask->GetObjName(), pTask->GetTaskNO(),
			 OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

		 delete pTask;
		 pTask = NULL;
	 }
	 else if (dwRet == PROCMSG_FAIL)
	 {
		 INSTLOG(ERROR_LEV, "事务[%s-%u]处理失败---msg[%s-%u]\n", 
			 pTask->GetObjName(), pTask->GetTaskNO(),
			 OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
	 }
	 else if (dwRet == PROCMSG_OK)
	 {
		 INSTLOG(ALL_LEV, "事务[%s-%u]处理成功---msg[%s-%u]\n", 
			 pTask->GetObjName(), pTask->GetTaskNO(),
			 OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
	 }
	 else
	 {
		 INSTLOG(ERROR_LEV, "事务[%s-%u]处理结果未知---msg[%s-%u]\n", 
			 pTask->GetObjName(), pTask->GetTaskNO(),
			 OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
	 }
 }


 void CCoreInst::RegSipProxy(s32 nExpires)
 {
	 OspSipRegProxy(nExpires);
 }


 BOOL32 CCoreInst::OnUserInnerCommonMsg(CMessage* const pcMsg)
 {
	 switch(pcMsg->event)
	 {
	 case ModulePrintStat:
		 {
			g_tSecurityStat.strSecurityStat = GetStrState();
			g_tSecurityStat.PrintData();
			return TRUE;
		 }
		 break;
	 default:
		 {
			 return FALSE;
		 }
		 break;
	 }


	 return TRUE;
 }