
#include "cms/cms_const.h"
#include "cms/cms_errorcode.h"

#include "cms/cmu/cmu_event.h"
#include "cms/cmu/cmu_struct.h"
#include "cms/cmu/cmu_proto.h"

#include "cms/tas/tas_event.h"
#include "cms/tas/tas_struct.h"
#include "cms/tas/tas_proto.h"

#include "cms/natagent/natagent.h"

#include "cmustubtestcoreapp.h"
#include "cmustubtestconf.h"
#include "cmustubtestdbg.h"
#include "cmustubtestdata.h"

#include "loginsession.h"
#include "cuiregtasktest.h"
#include "culogintasktest.h"
#include "pusstasktest.h"
#include "puptzctrltasktest.h"
#include "sysparamgettasktest.h"
#include "sysparamsettasktest.h"
#include "videncparamgettasktest.h"
#include "videncparamsettasktest.h"
#include "invitetasktest.h"
#include "devlogintask.h"
#include "alarmlinksettasktest.h"
#include "alarmlinkgettasktest.h"
#include "twschemesavetasktest.h"
#include "invite3pctasktest.h"
#include "reqrsptasktest.h"
#include "punormalcfgtasktest.h"
#include "purecqrytasktest.h"
#include "pusnapshottasktest.h"

CCmuStubTestCoreApp g_cCmuStubTestCoreApp;

TCmuStat g_tCmuStat;

CCmuStubTestCoreInst::CCmuStubTestCoreInst()
{
    ClearInst();
}

CCmuStubTestCoreInst::~CCmuStubTestCoreInst()
{
    ClearInst();
}

CAppData& CCmuStubTestCoreInst::GetAppData()
{
    return GETAPPDATA(CCmuStubTestCoreApp);
}

void CCmuStubTestCoreInst::ClearInst()
{
    EmptyTask();
}

void CCmuStubTestCoreInst::DisProcess()
{
    //断链处理
    ClearInst();
    NextState(Idle);
}

LPCSTR CCmuStubTestCoreInst::GetStrState() const
{
    switch(CurState())
    {
    case Idle:
        return "Idle";
    case RegProxy:
        return "RegProxy";
    case Service:
        return "Service";
    default:
        return "未知状态";
    }
}


void CCmuStubTestCoreInst::DaemonEntry(CMessage *const pcMsg)
{
    u8 byLogLev = EVENT_LEV;

    INSTLOG(byLogLev, "Receive Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

    switch(CurState())
    {
    case Idle:
        {
            OnIdle(pcMsg);
        }
        break;

    case RegProxy:
        {
            OnRegProxy(pcMsg);        
        }
        break;

    case Service:
        {
            OnService(pcMsg);
        }
        break;

    default:
        {            
            INSTLOG(WARNING_LEV, "unkown state recv Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }    
}

void CCmuStubTestCoreInst::NormalEntry(CMessage *const pcMsg)
{
    INSTLOG(ERROR_LEV, "Receive Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
}

BOOL32 CCmuStubTestCoreInst::OnUserInnerCommonMsg(CMessage* const pcMsg)
{
    BOOL32 bIsProcessMsg = TRUE;

    //内部消息处理

    switch(pcMsg->event)
    {
    case CMU_PRINT_CONF:
        {
            g_cCmuConf.PrintData();
        }
        break;

    case CMU_PRINT_STAT:
        {
            if (g_pcCuiRegCmuTask != NULL)
            {
                g_tCmuStat.strRegCmuState = g_pcCuiRegCmuTask->GetStateName(g_pcCuiRegCmuTask->GetState());
            }
            else
            {
                g_tCmuStat.strRegCmuState = "NULL";
            }

            g_tCmuStat.nSsNum = GetTasksArray("CPuSubscribeTask").size();
            g_tCmuStat.nIvtNum = GetTasksArray("CInviteTask").size();
            g_tCmuStat.PrintData();
        }
        break;

    case CMU_PRINT_CUI_LIST:
        {
            g_cCuiList.PrintData();
        }
        break;

    case CMU_PRINT_PU_LIST:
        {
            u8 byFlag = *(u8*)(pcMsg->content);
            g_cPuList.PrintData(byFlag);
        }
        break;

    case CMU_PRINT_CU_LIST:
        {
            g_cCuList.PrintData();
        }
        break;

    default:
        {
            bIsProcessMsg = FALSE;
        }
        break;
    }  

    return bIsProcessMsg;
}

void CCmuStubTestCoreInst::OnIdle(CMessage* const pcMsg)
{
    switch(pcMsg->event)
    {
    case CMU_START:
        {
            g_tCmuStat.strStartTime = COspTime::GetCurrStrTime();
            RegSipProxy();
            NextState(RegProxy);
        }
        break;

    default:
        {
            INSTLOG(WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }    
}

void CCmuStubTestCoreInst::OnRegProxy(CMessage* const pcMsg)
{
    switch(pcMsg->event)
    {
    case OSP_PROXY_CONNECT:
        {
            NextState(Service);

            g_tCmuStat.bIsRegSipProxy = true;
        }
        break;

    case OSP_PROXY_DISCONNECT:
        {
            ProxyDisProc();
        }
        break;
        
    default:
        {
            INSTLOG(WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }
}


void CCmuStubTestCoreInst::OnService(CMessage* const pcMsg)
{
    switch(pcMsg->event)
    {
    case CUI_REG_REQ:
        {
            if (g_pcCuiRegCmuTask == NULL)
            {
                CTask* pTask = CreateTask(pcMsg->event);
                if (pTask == NULL)
                {
                    INSTLOG(ERROR_LEV, "事务创建失败Msg[%s-%d]\n",
                        OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
                    return;
                }

                u32 dwRet = pTask->ProcMsg(pcMsg);
                if (dwRet != PROCMSG_OK)
                {
                    delete pTask;
                    pTask = NULL;
                }

                g_pcCuiRegCmuTask = (CCuiRegTask*)pTask;
            }
            else
            {
                INSTLOG(ERROR_LEV, "已经存在cui登录Msg[%s-%d]\n",
                    OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            }         
        }
        break;

    case CU_LOGIN_REQ:
    case PU_STATUS_SS_REQ:
    case PU_PTZ_CTRL_REQ:
    case CMU_CONNECT_REQ:
    case PU_VID_ENC_PARAM_GET_REQ:
    case PU_VID_ENC_PARAM_SET_REQ:
    case PU_SYS_PARAM_GET_REQ:
    case PU_SYS_PARAM_SET_REQ:
	case INVITE_REQ:
    case INVITE_THIRD_PARTY_CALL_REQ:
	case START_THIRD_PARTY_CALL_REQ:
	case STOP_THIRD_PARTY_CALL_REQ:
    case ALARMLINK_CFG_SET_REQ:
    case ALARMLINK_CFG_GET_REQ:
    case ALARMLINK_QUERY_REQ:
    case PU_REC_QRY_REQ:
    case PU_PIC_QRY_REQ:

    case TVWALL_SCHEME_SAVE_REQ:
    case TVWALL_SCHEME_DEL_REQ:
    case TVWALL_SCHEME_LOAD_REQ:
    case REAL_MEDIA_ON_TV_REQ:
    case REAL_MEDIA_OFF_TV_REQ:
    case TV_POLL_OPERATE_REQ:
    case TVWALL_SCHEME_GROUP_SET_REQ:
    case TVWALL_SCHEME_GROUP_GET_REQ:
    case TVWALL_SCHEME_POLL_OPERATE_REQ:
    case TVWALL_STATUS_SS_REQ:

    case PU_MOTION_DETECT_SET_REQ:
    case PU_MOTION_DETECT_GET_REQ:
    case PU_AREA_SHADE_SET_REQ:
    case PU_AREA_SHADE_GET_REQ:
    case PU_PIC_SNAPSHOT_REQ:
        {
            CTask* pTask = CreateTask(pcMsg->event);
            if (pTask == NULL)
            {
                INSTLOG(ERROR_LEV, "事务创建失败Msg[%s-%d]\n",
                    OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
                return;
            }

            u32 dwRet = pTask->ProcMsg(pcMsg);
            if (dwRet != PROCMSG_OK)
            {
                delete pTask;
                pTask = NULL;
            }
        }
        break;

    case DEV_LOGIN_REQ:
        {
            if (pcMsg->content == NULL)
            {
                INSTLOG(ERROR_LEV, "pcmsg 消息为空\n");
                return;
            }

            LPCSTR strServerId = (LPCSTR)pcMsg->content;

            CLoginSenderTask* pTask = (CLoginSenderTask*)CreateTask(pcMsg->event);
            if (pTask == NULL)
            {
                INSTLOG(ERROR_LEV, "事务创建失败Msg[%s-%d]\n",
                    OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
                return;
            }

            //本地地址列表
            TNetAddrList tLocalAddrList;
            TNetAddr tLocalAddr;
            tLocalAddr.SetNetIp(g_cCmuConf.m_tLocalAddr.GetIpStr());
            tLocalAddr.SetNetPort(g_cCmuConf.m_tLocalAddr.wPort);
            tLocalAddrList.push_back(tLocalAddr);

            //登录
            TSipURI tServerUri;
            tServerUri.SetDomain(g_cCmuConf.m_tLocalUri.GetDomain());
            tServerUri.SetUser(strServerId);
            pTask->SetServerUri(tServerUri.GetURIString());
            pTask->SetDevType("cui");
            pTask->SetDevUri(g_cCmuConf.m_tLocalUri.GetURIString());
            pTask->SetDevAddrList(tLocalAddrList);
            pTask->StartLogin();
        }
        break;

    case CU_LOGOUT_REQ:
    case BYE_REQ:
    case PLAYBACK_PROGRESS_QUERY_REQ:
        {
            if (pcMsg->content == NULL)
            {
                INSTLOG(ERROR_LEV, "pcmsg 消息为空\n");
                return;
            }

            u32 dwTask = *(u32*)pcMsg->content;
            CTask* pTask = FindTask(dwTask);
            if (pTask != NULL)
            {
                if (pTask->ProcMsg(pcMsg) == PROCMSG_DEL)
                {
                    delete pTask;
                    pTask = NULL;
                }          
            }
            else
            {
                INSTLOG(ERROR_LEV, "taskNO[%u]没有对应事务---msg[%s-%u]\n", 
                    dwTask, OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            }
        }
        break;

    case VCR_CTRL_REQ:
        {
            if (pcMsg->content == NULL)
            {
                INSTLOG(ERROR_LEV, "pcmsg 消息为空\n");
                return;
            }

            TVcrCtrlParam tVcrParam = *(TVcrCtrlParam*)pcMsg->content;
            u32 dwTask = tVcrParam.m_nTaskNO;
            CTask* pTask = FindTask(dwTask);
            if (pTask != NULL)
            {
                if (pTask->ProcMsg(pcMsg) == PROCMSG_DEL)
                {
                    delete pTask;
                    pTask = NULL;
                }          
            }
            else
            {
                INSTLOG(ERROR_LEV, "taskNO[%u]没有对应事务---msg[%s-%u]\n", 
                    dwTask, OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            }
        }
        break;

    case ALARMLINK_NTF_REQ:
    case PU_REC_QRY_NTF_REQ:
    case PU_PIC_QRY_NTF_REQ:
        {
            COspSipMsg* pSipMsg = (COspSipMsg*)pcMsg->content;
            if (pSipMsg == NULL)
            {
                INSTLOG(ERROR_LEV, "ospsip 消息为空\n");
                return;
            }
            string strQryTag = GetQueryTag((LPCSTR)pSipMsg->GetMsgBody());

            CTask* pTask = NULL;
            g_cStubQryTaskList.Find(strQryTag, pTask);
            
            if (pTask != NULL)
            {
                if (pTask->ProcMsg(pcMsg) == PROCMSG_DEL)
                {
                    delete pTask;
                    pTask = NULL;
                }          
            }
            else
            {
                INSTLOG(ERROR_LEV, "queryTag[%s]没有对应事务---msg[%s-%u]\n", 
                    strQryTag.c_str(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            }
        }
        break;

    case CU_LOGOUT_RSP:
    case PU_STATUS_REFRESH_SS_REQ:
    case PU_STATUS_UNSS_REQ:
    case PU_STATUS_NTF_RSP:
    case PU_CONFIG_NTF_RSP:
    case PU_SERVICE_STATUS_NTF_RSP:
    case PU_ALARM_STATUS_NTF_RSP:
    case PU_STATUS_SS_RSP:
    case PU_STATUS_NTF_REQ:
    case PU_CONFIG_NTF_REQ:
    case PU_SERVICE_STATUS_NTF_REQ:
    case PU_ALARM_STATUS_NTF_REQ:
    case PU_GPS_STATUS_NTF_REQ:
    case CMU_CONNECT_RSP:
    case CUI_REG_RSP:
    case CU_LOGIN_RSP:
    case PU_PTZ_CTRL_RSP:
    case PU_VID_ENC_PARAM_GET_RSP:
    case PU_VID_ENC_PARAM_SET_RSP:
    case PU_SYS_PARAM_GET_RSP:
    case PU_SYS_PARAM_SET_RSP:
    case CU_GETCUILIST_RSP:

        //{{交换相关
    case INVITE_RSP:
    case INVITE_ACK:
    case OSP_SIP_BYE_REQ:
    //case BYE_REQ:
    case OSP_SIP_BYE_RSP:
    case BYE_RSP:
    case INVITE_THIRD_PARTY_CALL_RSP:
    case INVITE_THIRD_PARTY_CALL_ACK:
    case START_THIRD_PARTY_CALL_RSP:
    case STOP_THIRD_PARTY_CALL_RSP:
    case END_OF_FILE_NTF_REQ:
        //}}交换相关

    case DEV_LOGIN_RSP:

    case ALARMLINK_CFG_SET_RSP:
    case ALARMLINK_CFG_GET_RSP:
    case ALARMLINK_QUERY_RSP:
    case PU_REC_QRY_RSP:

    case TVWALL_SCHEME_SAVE_RSP:
    case TVWALL_SCHEME_DEL_RSP:
    case TVWALL_SCHEME_LOAD_RSP:
    case REAL_MEDIA_ON_TV_RSP:
    case REAL_MEDIA_OFF_TV_RSP:
    case TV_POLL_OPERATE_RSP:
    case TVWALL_SCHEME_GROUP_SET_RSP:
    case TVWALL_SCHEME_GROUP_GET_RSP:
    case TVWALL_SCHEME_POLL_OPERATE_RSP:
    case TVWALL_STATUS_SS_RSP:
    case TVWALL_STATUS_NTF_REQ:

    case PU_MOTION_DETECT_SET_RSP:
    case PU_MOTION_DETECT_GET_RSP:
    case PU_AREA_SHADE_SET_RSP:
    case PU_AREA_SHADE_GET_RSP:
    case PU_PIC_SNAPSHOT_RSP:
    case PU_PIC_DOWNLOAD_RSP:
    case PU_PIC_QRY_RSP:

    case PLAYBACK_PROGRESS_QUERY_RSP:
    case VCR_CTRL_RSP:
    case PLAYBACK_PROGRESS_NTF_REQ:

    case KEY_FRAME_RSP:

    case PU_RECORD_DEL_RSP:
        {
            COspSipMsg* pSipMsg = (COspSipMsg*)pcMsg->content;
            if (pSipMsg == NULL)
            {
                INSTLOG(ERROR_LEV, "ospsip 消息为空\n");
                return;
            }

            CTask* pTask = FindTask(pSipMsg->GetOspTaskNO());
            if (pTask != NULL)
            {
                if (pTask->ProcMsg(pcMsg) == PROCMSG_DEL)
                {
                    delete pTask;
                    pTask = NULL;
                }          
            }
            else
            {
                INSTLOG(ERROR_LEV, "taskNO[%u]没有对应事务---msg[%s-%u]\n", 
                    pSipMsg->GetOspTaskNO(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            }
        }
        break;

    case CMU_TASK_TIMER_TEST:
    case CMU_TASK_TIMER_UPDATE:
    case CMU_TASK_TIMER_KILL:
        {
            //事务定时器处理
            u32 dwTaskNO = *(u32*)pcMsg->content;
            CTask* pcTask = FindTask(dwTaskNO);
            if (pcTask != NULL)
            {
                ProcTaskMsg(pcTask, pcMsg);
            }
            else
            {
                INSTLOG(ERROR_LEV, "taskNO[%u]没有对应事务---msg[%s-%u]\n", 
                    dwTaskNO, OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            }
        }
        break;

    case STUN_UPDATE_REQ:
        {
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;

            CStunUpdateReq cReq;
            pcOspSipMsg->GetMsgBody(cReq);
            const TNetAddr& tNetAddr = cReq.GetStunAddr();

            //调用na接口，变更stun地址
            TNatAddr tStunNatAddr;
            strncpy(tStunNatAddr.m_achIP, tNetAddr.GetNetIp().c_str(), sizeof(tStunNatAddr.m_achIP));
            tStunNatAddr.m_wPort = tNetAddr.GetNetPort();

            if (NATAGENT_OK != NAUpdateStunAddr(&tStunNatAddr))
            {
                INSTLOG(ERROR_LEV, "NAUpdateStunAddr failed!\n");
            }


            INSTLOG(CRITICAL_LEV, "stun地址变更为[%s:%d]---msg[%s-%u]\n", 
                tNetAddr.GetNetIp().c_str(), tNetAddr.GetNetPort(), 
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

            TOspTransID tOspTrasnID;
            tOspTrasnID.dwIID = GetOspIID();
            CStunUpdateRsp cRsp;
            cRsp.SetHeadFromReq(cReq);
            cRsp.SetErrorCode(CMS_SUCCESS);
            OspSipPostRsp(pcOspSipMsg->GetSipRsp(), pcOspSipMsg->GetSipTransID(), cRsp, tOspTrasnID);
        }
        break;

    case CMU_PUNCH_RESULT_NTF:
        {
            TPunchResultParam* ptPunchReslut = (TPunchResultParam*)pcMsg->content;
            if (ptPunchReslut == NULL)
            {
                INSTLOG(ERROR_LEV, "ospsip 消息为空\n");
                return;
            }

            CTask* pTask = FindTask(ptPunchReslut->m_dwTaskNO);;
            if (pTask != NULL)
            {
                if (pTask->ProcMsg(pcMsg) == PROCMSG_DEL)
                {
                    delete pTask;
                    pTask = NULL;
                }          
            }
            else
            {
                INSTLOG(ERROR_LEV, "taskNO[%u]没有对应事务---msg[%s-%u]\n", 
                    ptPunchReslut->m_dwTaskNO, OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            }
        }
        break;

    case CMU_STUN_QRY_ADDR_NTF:
        {
            TStunResultParam* ptPunchReslut = (TStunResultParam*)pcMsg->content;
            if (ptPunchReslut == NULL)
            {
                INSTLOG(ERROR_LEV, "ospsip 消息为空\n");
                return;
            }

            CTask* pTask = FindTask(ptPunchReslut->m_dwTaskNO);;
            if (pTask != NULL)
            {
                if (pTask->ProcMsg(pcMsg) == PROCMSG_DEL)
                {
                    delete pTask;
                    pTask = NULL;
                }          
            }
            else
            {
                INSTLOG(ERROR_LEV, "taskNO[%u]没有对应事务---msg[%s-%u]\n", 
                    ptPunchReslut->m_dwTaskNO, OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            }
        }
        break;

    case OSP_SIP_DISCONNECT:
    case CMU_CUI_DISCONNECT:
    case CMU_PUI_DISCONNECT:
    case CMU_CMU_DISCONNECT:
        {
            COspSipMsg* pSipMsg = (COspSipMsg*)pcMsg->content;
            if (pSipMsg == NULL)
            {
                INSTLOG(ERROR_LEV, "ospsip 消息为空\n");
                return;
            }

            CTask* pTask = FindTask(pSipMsg->GetOspTaskNO());
            if (pTask != NULL)
            {
                pTask->ProcMsg(pcMsg);
                delete pTask;
                pTask = NULL;
            }
            else
            {
                INSTLOG(ERROR_LEV, "taskNO[%u]没有对应事务---msg[%s-%u]\n", 
                    pSipMsg->GetOspTaskNO(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            }
        }
        break;

    case OSP_PROXY_CONNECT:
        {
            NextState(Service);

            g_tCmuStat.bIsRegSipProxy = true;
        }
        break;

    case OSP_PROXY_DISCONNECT:
        {
            ProxyDisProc();
        }
        break;
        
    default:
        {
            INSTLOG(WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }
}

void CCmuStubTestCoreInst::RegSipProxy(s32 nExpires)
{
    OspSipRegProxy(nExpires);
}

//proxy断链处理
void CCmuStubTestCoreInst::ProxyDisProc()
{
    g_tCmuStat.bIsRegSipProxy = false;
}

//事务创建
CTask* CCmuStubTestCoreInst::CreateTask(u16 wEvent)
{
    CTask* pTask = NULL;

    switch(wEvent)
    {
    case CUI_REG_REQ:
        {
            pTask = new CCuiRegTask(this);
        }
        break;

    case CU_LOGIN_REQ:
        {
            pTask = new CCuLoginTask(this);
        }
        break;


    case PU_STATUS_SS_REQ:
        {
            pTask = new CPuSubscribeTask(this);
        }
        break;

    case PU_PTZ_CTRL_REQ:
        {
            pTask = new CPuPtzCtrlTask(this);
        }
        break;

    case PU_SYS_PARAM_GET_REQ:
        {
            pTask = new CPuSysParamGetTask(this);
        }
        break;

    case PU_SYS_PARAM_SET_REQ:
        {
            pTask = new CPuSysParamSetTask(this);
        }
        break;

    case PU_VID_ENC_PARAM_GET_REQ:
        {
            pTask = new CVidEncParamGetTask(this);
        }
        break;

    case PU_VID_ENC_PARAM_SET_REQ:
        {
            pTask = new CVidEncParamSetTask(this);
        }
        break;

        /*

    case PU_SAVE_CFG_REQ:
        {
            pTask = new CSavePuCfgTask(this);
        }
        break;
        */

    case INVITE_REQ:
        {
            pTask = new CInviteTask(this);
        }
        break;
    case INVITE_THIRD_PARTY_CALL_REQ:
        {
            pTask = new CInvite3PCTask(this);
        }
        break;
    case START_THIRD_PARTY_CALL_REQ:
        {
            pTask = new CStart3pcTask(this);
        }
        break;
    case STOP_THIRD_PARTY_CALL_REQ:
        {
            pTask = new CStop3pcTask(this);
        }
        break;

    case DEV_LOGIN_REQ:
        {
            pTask = new CLoginSenderTask(this);
        }
        break;

    case ALARMLINK_CFG_SET_REQ:
        {
            pTask = new CAlarmLinkSetTask(this);
        }
        break;

    case ALARMLINK_CFG_GET_REQ:
        {
            pTask = new CAlarmLinkGetTask(this);
        }
        break;

    case ALARMLINK_QUERY_REQ:
        {
            pTask = new CAlarmLinkGetTask(this);
        }
        break;

    case TVWALL_SCHEME_SAVE_REQ:
    case TVWALL_SCHEME_DEL_REQ:
    case TVWALL_SCHEME_LOAD_REQ:
    case REAL_MEDIA_ON_TV_REQ:
    case REAL_MEDIA_OFF_TV_REQ:
    case TV_POLL_OPERATE_REQ:
    case TVWALL_SCHEME_GROUP_SET_REQ:
    case TVWALL_SCHEME_GROUP_GET_REQ:
    case TVWALL_SCHEME_POLL_OPERATE_REQ:
    case TVWALL_STATUS_SS_REQ:
        {
            pTask = new CTwSchemeSaveTask(this);
        }
        break;

    case PU_MOTION_DETECT_SET_REQ:
    case PU_MOTION_DETECT_GET_REQ:
    case PU_AREA_SHADE_SET_REQ:
    case PU_AREA_SHADE_GET_REQ:
        {
            pTask = new CPuNormalCfgTask(this);
        }

    case PU_REC_QRY_REQ:
        {
            pTask = new CPuRecQryTask(this);
        }
        break;

    case PU_PIC_SNAPSHOT_REQ:
        {
            pTask = new CPuSnapshotTask(this);
        }
        break;

    case PU_PIC_QRY_REQ:
        {
            pTask = new CPuPicQryTask(this);
        }
        break;

    default:
        {
            INSTLOG(ERROR_LEV, "Msg[%s-%d]创建事务失败\n", 
                OspExtEventDesc(wEvent).c_str(), wEvent);
        }
        break;
    }

    if (pTask != NULL)
    {
        pTask->InitStateMachine();
    }

    return pTask;
}


//获取登录session
CLoginSession* CCmuStubTestCoreInst::GetLoginSession(const string& strSession)
{
    CLoginSession* ptLoginSession = NULL;
    if (IsLocalDomain(strSession))
    {
        if (g_cCuList.Find(strSession, ptLoginSession))
        {
            return ptLoginSession;
        }

        if (g_cCuiList.Find(strSession, ptLoginSession))
        {
            return ptLoginSession;
        }
    }
    else
    {
        return ptLoginSession;
    }

    return ptLoginSession;
}




