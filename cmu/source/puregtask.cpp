
#include "cms/cms_errorcode.h"
#include "cms/cmu/cmu_event.h"
#include "cms/cmu/cmu_struct.h"
#include "cms/cmu/cmu_proto.h"
#include "cms/tas/tas_proto.h"

#include "puregtask.h"
#include "puiregtask.h"
#include "cmuconst.h"
#include "cmuconf.h"
#include "cmudata.h"
#include "cmucoreapp.h"
#include "regtastask.h"
#include "cuiregtask.h"

extern CRegTasTask* g_pcRegTasTask;

CPuRegBaseTask::CPuRegBaseTask(CInstExt *pcInst) : CLoginSession(pcInst)
{

}

CPuRegBaseTask::~CPuRegBaseTask()
{
    Iterator pPos;
	TChannel nVidSrcChn;
    CTask* pTask = NULL;

    while (!pPos.End())
    {
        if (m_tPuPtzCtrlTaskList.Iterate(pPos, nVidSrcChn, pTask) && pTask != NULL)
        {
            delete pTask;
            pTask = NULL;
        }
    }

    m_tPuPtzCtrlTaskList.Empty();
}

CTask* CPuRegBaseTask::GetPtzCtrlTask(const TChannel &nVidSrcChn) const
{
    CTask* ptTask = NULL;
    m_tPuPtzCtrlTaskList.Find(nVidSrcChn, ptTask);
    return ptTask;
}
    
void CPuRegBaseTask::SetPtzCtrlTask(const TChannel &nVidSrcChn, CTask* ptTask)
{
    CTask* ptExistTask = NULL;
    m_tPuPtzCtrlTaskList.Find(nVidSrcChn, ptExistTask);
    if (ptExistTask != NULL)
    {
        //存在的事务先被删除
        delete ptExistTask;
        ptExistTask = NULL;
    }

    m_tPuPtzCtrlTaskList[nVidSrcChn] = ptTask;
}

void CPuRegBaseTask::ErasePtzCtrlTask(const TChannel &nVidSrcChn)
{
   //只清除表中的指针，不负责删除事务
   m_tPuPtzCtrlTaskList.Erase(nVidSrcChn);
}


s32 CPuRegBaseTask::GetPuLicenseNum(const vector<TDeviceCapInfo> &vctCapInfo)
{
   typedef vector<TDeviceCapInfo>::const_iterator VIT;
   for (VIT vecIt = vctCapInfo.begin(); vecIt != vctCapInfo.end(); ++vecIt)
   {
       if (vecIt->type == DEVICE_CAP_VIDEOSOURCE)
       {
           return vecIt->license;
       }
   }

   return 0;
}

u32 CPuRegBaseTask::ValidLicense(const string &strDevId, TCreater tCreater, s32 nLicenseNum, s32 nVidEncNum, s32 nFreeLicenseNum)
{
    /*
       编码设备注册的检验策略：目前采用策略1
       策略1：编码通道数和licence数同时校验。不校验视频源数
       ####存在问题：
                1.不支持NVR64通道只有32个可用通道的设备注册
                2.有主辅流的设备对于开/关将对应两种设备类型，需要在tas中修改设备类型

       策略2：视频源数和licence数同时校验。不校验编码通道数
       ####存在问题：
                1. 这种方法虽然可以解决策略1的问题2，但要求用户在设备入网时填写设备的
                最大通道数，这个要求具有不确定性，一旦填错，可能使视频源和编码通道的关系
                出现错乱，造成一系列的不良后果。比如单视频源多通道设备可能出现浏览失败的情况。
    */

    //通道数检验
    //todo
    //liangli 柒牌男装项目中，NVR不收License（一贯如此），但是可以上报4通道或者8通道给平台
    //干脆和平台1.0保持一致策略，只校验License，不判断编码通道
    
    //licence校验
    /*
      2014-02-20 fanxg
      licence校验规则变更：
      1. 外厂商设备辅流也收费。
      2. 上报的licence数只要不大于入网licence即可，不要求相等
      3. 通过GBS模拟的异域设备无需要进行licence校验
      4. (2014-03-18)又确定 外厂商设备辅流不强制收费，是否收费由freeLicence字段确定
    */

    if (tCreater != CREATE_BY_GBS_FOREIGN_DOMAIN)
    {
        s32 nPuLicenseNum = nVidEncNum - nFreeLicenseNum;

        //上报的licence数只要不大于入网licence即可，不要求相等
        if (nPuLicenseNum > nLicenseNum)
        {
            GLOBELOG(CMU_MOD_PU, ERROR_LEV, "pu[%s]监控点数不匹配，期望[<=%d],实际[%d]\n",
                strDevId.c_str(), nLicenseNum, nPuLicenseNum);
            return ERR_CMU_PU_LICENSE_NOT_MATCH;
        }
    }

    return CMS_SUCCESS;
}

u32 CPuRegBaseTask::ValidLicense(const CDeviceInfo &cDevInfo, s32 nVidEncNum, s32 nFreeLicenseNum)
{
    s32 nLicenseNum = GetPuLicenseNum(cDevInfo.GetDeviceModelCapDetail());

    return ValidLicense(cDevInfo.GetDeviceUUID(), cDevInfo.GetCreater(), nLicenseNum, nVidEncNum, nFreeLicenseNum);
}

CPuRegTask::CPuRegTask(CInstExt *pcInst) : CPuRegBaseTask(pcInst)
{
    m_pcPuData = NULL;
}

CPuRegTask::~CPuRegTask()
{
    ReleaseResource();
}

void CPuRegTask::InitStateMachine()
{
    CStateProc cWaitRegProc;
    cWaitRegProc.ProcMsg = (CTask::PFProcMsg)&CPuRegTask::OnWaitReg;
    cWaitRegProc.ProcErrMsg = (CTask::PFProcMsg)&CPuRegTask::OnWaitReg;
    cWaitRegProc.TimerPoll = (CTask::PFTimerPoll)&CPuRegTask::OnWaitRegTimer;
    AddRuleProc(WaitReg, cWaitRegProc);

    CStateProc cWaitCfgNtfProc;
    cWaitCfgNtfProc.ProcMsg = (CTask::PFProcMsg)&CPuRegTask::OnWaitCfgNtf;
    cWaitCfgNtfProc.ProcErrMsg = (CTask::PFProcMsg)&CPuRegTask::OnWaitCfgNtf;
    cWaitCfgNtfProc.TimerPoll = (CTask::PFTimerPoll)&CPuRegTask::OnWaitCfgNtfTimer;
    AddRuleProc(WaitCfgNtf, cWaitCfgNtfProc);

    CStateProc cServiceProc;
    cServiceProc.ProcMsg = (CTask::PFProcMsg)&CPuRegTask::OnService;
    cServiceProc.ProcErrMsg = (CTask::PFProcMsg)&CPuRegTask::OnService;
    cServiceProc.TimerPoll = (CTask::PFTimerPoll)&CPuRegTask::OnServiceTimer;
    AddRuleProc(Service, cServiceProc);

    NextState(WaitReg);
}

void CPuRegTask::PrintData() const
{
	CLoginSession::PrintData();

	OspPrintf(TRUE, FALSE, "PuiIP：%s\n",
		GetPuiIp().c_str());

	if (!m_cVidSrcNameBak.empty())
	{
		OspPrintf(TRUE, FALSE, "缓存的前端视频源别名信息如下：\n");
		for (map<s32, string>::const_iterator it = m_cVidSrcNameBak.begin(); m_cVidSrcNameBak.end() != it; ++it)
		{
			OspPrintf(TRUE, FALSE, "  ChnNO[%d], Name[%s]\n", it->first, UTF8ToGBK(it->second).c_str());
		}
	}
}

void CPuRegTask::ReleaseResource(bool bStay)
{
    TASKLOG(CMU_MOD_PU, EVENT_LEV, "pu[%s]注册事务被销毁\n", m_strDevId.c_str());

    switch(GetState())
    {
    case WaitReg:
        {
        }
        break;
    case WaitCfgNtf:
    case Service:
        {
			ProcOffline();

            // 清除PuData中记录的注册Task指针
            m_pcPuData->SetPuRegTask(NULL);

            //删除pui中登记的pu注册事务
            CPuiRegTask* ptPuiRegTask = GetPuiRegTask();
            if (ptPuiRegTask != NULL)
            {
                ptPuiRegTask->DelTask(this);
            }   
        }
        break;
    default:
        {
            TASKLOG(CMU_MOD_PUI, ERROR_LEV, "未知事务状态[%d]\n", GetState());
        }
        break;
    } 
}

u32 CPuRegTask::OnWaitReg(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_FAIL;
    switch(pcMsg->event)
    {
    case PU_REG_REQ:
        {
            int dwErrorCode = CMS_SUCCESS;
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
            if (pcOspSipMsg == NULL)
            {
                TASKLOG(CMU_MOD_PU, ERROR_LEV, "ospsip 消息为空\n");
                dwErrorCode = ERR_CMU_SIP_BODY_NULL;
                dwRet = PROCMSG_DEL;
                return dwRet;
            }

            CPuRegReq cReq;
            pcOspSipMsg->GetMsgBody(cReq);

            SetSession(cReq.GetSession());
            SetPuiUri(pcOspSipMsg->GetSipFromUri());
            SetDevType(DEV_TYPE_PU);
            SetPuiIp(cReq.GetUsedPuiIp());

			TSipURI tUri;
			tUri.SetURIString(cReq.GetBaseConfig().GetDevUri());
			m_strDevId = tUri.GetUser();

            TASKLOG(CMU_MOD_PU, CRITICAL_LEV, "PU[%s]通过PUI[%s]PuiIP[%s]向CMU注册\n",
                m_strDevId.c_str(), GetPuiUri().GetURIString().c_str(), GetPuiIp().c_str());

            int nPuNetType = (int)DEVICE_LINEMODE_INTRANET;

			CPuiRegTask* ptPuiRegTask = GetPuiRegTask();
            do 
            {
                if (ptPuiRegTask == NULL)			//途经的pui是否注册
                {
                    TASKLOG(CMU_MOD_PU, ERROR_LEV, "pu登录使用的pui[%s]未向cmu注册\n",
                        GetPuiUri().GetURIString().c_str());
                    dwErrorCode = ERR_CMU_PUI_NOT_REG;
                    break;
                }

				//是否入网
                m_pcPuData = CCmuData::GetInstance()->GetPuData(m_strDevId, true);
				if (NULL == m_pcPuData)
				{
					TASKLOG(CMU_MOD_PU, ERROR_LEV, "pu[%s]未入网，注册ip[%s]\n",
						m_strDevId.c_str(), cReq.GetBaseConfig().GetDevIp().c_str());
					dwErrorCode = ERR_CMU_PU_NOT_IN_TOP;
					break;
				}

				//是否启用
				if (!m_pcPuData->IsPuInUse())
				{
					TASKLOG(CMU_MOD_PU, ERROR_LEV, "pu[%s]未启用\n", m_strDevId.c_str());
					dwErrorCode = ERR_CMU_PU_NOT_USED;
					break;
				}

				//是否过期
				if (m_pcPuData->IsPuExpired())
				{
					TASKLOG(CMU_MOD_PU, ERROR_LEV, "pu[%s]已经过期\n", m_strDevId.c_str());
					dwErrorCode = ERR_CMU_PU_EXPIRE;
					break;
				}

                //是否已经注册
                if (m_pcPuData->IsPuRegistered())
                {
                    TASKLOG(CMU_MOD_PU, ERROR_LEV, "pu[%s]已经注册\n", m_strDevId.c_str());
                    dwErrorCode = ERR_CMU_PU_ALREADY_REG;
                    break;
                }

                CDeviceInfo cDevInfo = m_pcPuData->GetDevInfo();
                s32 nVidEncNum = cReq.GetBaseConfig().GetBaseCapSet().GetVidEncNum();
                s32 nFreeLicenseNum = cReq.GeBaseConfig().GetFreeLicenceNum();
                dwErrorCode = ValidLicense(cDevInfo, nVidEncNum, nFreeLicenseNum);
                if (CMS_SUCCESS != dwErrorCode)
                {
                    TASKLOG(CMU_MOD_PU, ERROR_LEV, "pu[%s]license验证失败，VidEncNum[%d], FreeNum[%d]\n", 
                        m_strDevId.c_str(), nVidEncNum, nFreeLicenseNum);
                    break;
                }
               
                //网络类型
                nPuNetType = cDevInfo.GetNetType();

                //设置punch属性
                if (cReq.GetIsSupportNA())
                {
                    SetIsNeedPunch(true);

                    //这种情况完全不关心pu和平台的网络关系
                }
                else
                {
                    SetIsNeedPunch(false);
                }
            } while (0);          
            
            CPuRegRsp cRsp;
            cRsp.SetErrorCode(dwErrorCode);
            cRsp.SetSeqNum(cReq.GetSeqNum());
            cRsp.SetSession(GetSession());
            cRsp.SetNetType(nPuNetType);
            PostMsgRsp(pcOspSipMsg->GetSipTransID(), cRsp);

            if (dwErrorCode != CMS_SUCCESS || ptPuiRegTask == NULL)
            {
                //注册失败
                TASKLOG(CMU_MOD_PU, ERROR_LEV, "pu[%s]登录cmu失败, 错误码[%d]!\n", m_strDevId.c_str(), dwErrorCode);

                return PROCMSG_DEL;
            }
  
            //登录成功
            TASKLOG(CMU_MOD_PU, CRITICAL_LEV, "pu[%s]获取基本配置成功，开始获取所有通道配置!\n", m_strDevId.c_str());

			//校验BaseConfig，有变化则通知3as
			CDevCapInfoModReq cModReq;
			if (!ptPuiRegTask->GetIsSupportRedis() && g_cCmuConf.m_bModDevModel && CheckBaseConfig(cReq.GetBaseConfig(), cModReq.GetDeviceModelCapDetail()))
			{
                TASKLOG(CMU_MOD_PU, CRITICAL_LEV, "pu[%s]基本配置和入网不一致，需要修改。\n", m_strDevId.c_str());
                //通知到3as，基本能力集变化
                cModReq.SetDeviceUUID(m_strDevId);
                cModReq.SetSession(g_pcRegTasTask->GetSession());
                dwRet = PostMsgReq(cModReq, g_cCmuConf.m_t3asUri);
                if (dwRet != PROCMSG_OK)
                {
                    TASKLOG(CMU_MOD_3AS, ERROR_LEV, "发送SIP消息失败!\n");
                    return PROCMSG_DEL;
                }
			}

            if (!ptPuiRegTask->GetIsSupportRedis())     // 对于不支持redis的pui来说，先缓存其信令上报的PuConfig，注册成功后一次写入redis
            {
                // pui上报的数据中，如设备厂商等需要使用3as的数据
                TPuBaseConfig &tPuBaseConfig = cReq.GetBaseConfig();
                FilterBaseConfig(tPuBaseConfig);
                m_tPuConfig.SetBaseConfig(tPuBaseConfig);
            }

            //注册设备
			m_pcPuData->SetPuRegTask(this);
			
            //添加到PUI中，PUI断链时需要使用
            ptPuiRegTask->AddTask(this);           

            //进入pu配置通知状态
            NextState(WaitCfgNtf);

            dwRet = PROCMSG_OK;
        }
        break;

    default:
        {
            TASKLOG(CMU_MOD_PU, WARNING_LEV, "Receive unknown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }

    return dwRet;
}

u32 CPuRegTask::OnWaitRegTimer()
{
    return TIMERPOLL_DONE;
}

u32 CPuRegTask::OnWaitCfgNtf(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_FAIL;
    switch(pcMsg->event)
    {
	case PU_UNREG_REQ:
		{
			UnregProc(pcMsg);
			dwRet = PROCMSG_DEL;
		}
		break;

	case PU_ONLINE_NTF_REQ:					//支持redis的pui将设备配置信息写入redis后，发送该信令来通知cmu可以让设备上线了
		{
            CPuiRegTask *ptPuiRegTask = GetPuiRegTask();
            CMS_ASSERT(NULL != ptPuiRegTask && "pcPuiRegTask = NULL");
            if (ptPuiRegTask->GetIsSupportRedis())
            {
                COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;

                CPuOnlineNtfReq cReq;
                pcOspSipMsg->GetMsgBody(cReq);

                // 接入模块是pui或者其他接入模块下有部分通道在线
                if (ptPuiRegTask->GetDevType() == DEV_TYPE_PUI
					|| ptPuiRegTask->GetDevType() == DEV_TYPE_PUI_GB
                    || DEVICE_TYPE_ENCODER != m_pcPuData->GetDevType()
                    || !IsAllChnNotOnline())
                {
                    ProcOnline();
                }

                // 缓存前端上报的视频源别名信息，以便后续同步
                m_cVidSrcNameBak = m_pcPuData->GetPuVidSrcAliasMap();

                //进入服务态
                NextState(Service);

                // 最后应答，以保证cmu先发布上下线通知
                CPuOnlineNtfRsp cRsp;
                cRsp.SetHeadFromReq(cReq);
                PostMsgRsp(pcOspSipMsg->GetSipTransID(), cRsp);

                //注册成功
                TASKLOG(CMU_MOD_PU, CRITICAL_LEV, "收到pu[%s]的上线通知，设备注册cmu成功!\n", m_strDevId.c_str());

                // 最后才发送开启GPS请求，避免pui认为设备没上线而不处理该请求
                m_pcPuData->EnableGps();

                dwRet = PROCMSG_OK;
            }
            else
            {
                //注册失败
                TASKLOG(CMU_MOD_PU, WARNING_LEV, "收到pu[%s]的上线通知，但是pui[%u]不支持redis，忽略该通知!\n", 
                    m_strDevId.c_str(), ptPuiRegTask->GetTaskNO());

                dwRet = PROCMSG_FAIL;
            }
		}
		break;

    case PU_DEV_CONFIG_NTF_REQ:				//不支持redis的pui，需要继续上报设备的配置信息
        {
            CPuiRegTask *ptPuiRegTask = GetPuiRegTask();
            CMS_ASSERT(NULL != ptPuiRegTask && "pcPuiRegTask = NULL");
            if (!ptPuiRegTask->GetIsSupportRedis())
            {
                COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;

                CPuDevConfigNtyReq cReq;
                pcOspSipMsg->GetMsgBody(cReq);
                TPuDevConfig& tPuDevConfig = cReq.GetConfig();

                // pui上报的数据中，有一些需要用3as的数据进行覆盖
                FilterPuDevConfig(tPuDevConfig);

                TASKLOG(CMU_MOD_PU, CRITICAL_LEV, "设备[%s]配置通知\n", 
                    cReq.GetDevURI().c_str());

                int dwErrorCode = CMS_SUCCESS;

                //更新配置收集器
                m_tPuConfig.MergeDevConfig(tPuDevConfig);
                if (m_tPuConfig.IsCompleted())
                {
                    //登录成功
                    TASKLOG(CMU_MOD_PU, CRITICAL_LEV, "pu[%s]所有配置获取完成，登录cmu成功!\n",
                        m_strDevId.c_str());

                    // 将完整的PuConfig写入redis
                    m_pcPuData->SetPuConfig(m_tPuConfig);

                    // 接入模块是pui
                    if (ptPuiRegTask->GetDevType() == DEV_TYPE_PUI
						|| ptPuiRegTask->GetDevType() == DEV_TYPE_PUI_GB
						|| !IsAllChnNotOnline())
                    {
                        ProcOnline();
                    }

                    //进入服务态
                    NextState(Service);

                    //注册成功
                    TASKLOG(CMU_MOD_PU, CRITICAL_LEV, "收到pu[%s]的配置通知，设备注册cmu成功!\n", m_strDevId.c_str());

                    // 最后才发送开启GPS请求，避免pui认为设备没上线而不处理该请求
                    m_pcPuData->EnableGps();
                }
                else
                {
                    //等待下一个通知
                    NextState(WaitCfgNtf);
                }

                UpdateVidSrcName2Tas(tPuDevConfig.GetVidSrcCfgList());

                CPuDevConfigNtyRsp cRsp;
                cRsp.SetEvent(PU_DEV_CONFIG_NTF_RSP);
                cRsp.SetHeadFromReq(cReq);
                cRsp.SetErrorCode(dwErrorCode);
                PostMsgRsp(pcOspSipMsg->GetSipTransID(), cRsp);

                dwRet = PROCMSG_OK;
            }
            else
            {
                TASKLOG(CMU_MOD_PU, WARNING_LEV, "收到pu[%s]的配置上报通知，但是pui[%u]支持redis，忽略该通知!\n", 
                    m_strDevId.c_str(), ptPuiRegTask->GetTaskNO());

                dwRet = PROCMSG_FAIL;
            }
        }
        break;

	case TAS_DEVICE_CAP_INDEX_NAME_SET_RSP:
		{
			COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;

			CDeviceCapIndexNameSetRsp cRsp;
			pcOspSipMsg->GetMsgBody(cRsp);

			TASKLOG(CMU_MOD_PU, EVENT_LEV, "收到3as返回的视频源别名同步应答!\n");

			if (CMS_SUCCESS != cRsp.GetErrorCode())
			{
				TASKLOG(CMU_MOD_PU, ERROR_LEV, "同步前端视频源别名信息到3as失败!\n");
			}

			// 通知流控管理Task该任务已经处理完毕
			((CCmuCoreInst *)GetInstance())->DelFlowCtrlJob(GetTaskNO());
			
			dwRet = PROCMSG_OK;
		}
		break;

    default:
        {
            TASKLOG(CMU_MOD_PU, WARNING_LEV, "Receive unknown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }

    return dwRet;
}

u32 CPuRegTask::OnWaitCfgNtfTimer()
{
    if (GetCurStateHoldTime() > (u32)CMU_PU_CONFIG_NTF_TIMEOUT)
    {
        TASKLOG(CMU_MOD_PU, WARNING_LEV, "pu[%s]配置通知超时，达到[%u]s, 注册失败\n", 
            m_strDevId.c_str(), (u32)GetCurStateHoldTime());

		// 获取设备配置超时，给注册者发送强制下线通知，发送完毕直接退出，不处理应答
		DisconnectPu();

        return TIMERPOLL_DEL;
    }

    return TIMERPOLL_DONE;
}

u32 CPuRegTask::OnService(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_FAIL;
    switch(pcMsg->event)
    {
	//{{{{{{{{{{{{{{{ add by Royan 20140509 智能告警主机
	case DOMAIN_ALARM_REQ:
		{
			u32 dwErrorCode = CMS_SUCCESS;

			COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
			CDomainAlarmReq cReq;
			pcOspSipMsg->GetMsgBody(cReq);
			TAlarmInput tAlarm = cReq.GetAlarm();

			string strDevUri = tAlarm.GetDevUri();
			if (FALSE == IsSipUri(strDevUri))
			{
				if (g_cPuIdUriMapList.end() == g_cPuIdUriMapList.find(strDevUri))
				{
					CDomainAlarmRsp cRsp;
					cRsp.SetHeadFromReq(cReq);
					cRsp.SetErrorCode(ERR_CMU_PU_NOT_IN_TOP);
					PostMsgRsp(pcOspSipMsg->GetSipTransID(), cRsp);

					TASKLOG(CMU_MOD_PU, ERROR_LEV, "DOMAIN_ALARM_REQ 未找到PUID[%s]对应的URI\n",
						strDevUri.c_str());

					break;
				}

				tAlarm.SetDevUri(g_cPuIdUriMapList.find(strDevUri)->second);
			}

			TASKLOG(CMU_MOD_PU, CRITICAL_LEV, "设备[%s][%s]告警通知:[inputId-%d] [flag-%s] [data-%s]\n", 
				tAlarm.GetDevUri().c_str(), tAlarm.GetAlarmType().c_str(),
				tAlarm.GetInputId(), tAlarm.GetAlarmFlag().c_str(), 
				tAlarm.GetAlarmData().c_str());

			if (m_pcPuData != NULL)
			{
				//更新告警别名
				UpdateAlarmAlias(tAlarm);
				//更新告警收集器
				if (m_pcPuData->UpdateAlarmInput(tAlarm))
				{
					//发布当前告警（todo by xiongxh 这个告警类型比较特殊，pui目前没有向redis发布，暂由cmu发布）
					m_pcPuData->PublishRedisAlarm(tAlarm);

					//向uas写告警日志
					((CCmuCoreInst*)GetInstance())->GetUasLogTask()->AddAlarmLog(GetAlarmLogInfo(tAlarm));
				}
				else
				{
					dwErrorCode = ERR_CMU_ALARM_ILLEGAL;
				}           
			}
			else
			{
				TASKLOG(CMU_MOD_PU, ERROR_LEV, "pu[%s]未入网\n",
					m_strDevId.c_str());
				dwErrorCode = ERR_CMU_PU_NOT_IN_TOP;
			}

			CDomainAlarmRsp cRsp;
			cRsp.SetHeadFromReq(cReq);
			cRsp.SetErrorCode(dwErrorCode);
			PostMsgRsp(pcOspSipMsg->GetSipTransID(), cRsp);
		}
		break;
	//}}}}}}}}}}}}}}} add by Royan 20140509 智能告警主机

    case PU_DEV_CONFIG_NTF_REQ:
        {
            CPuiRegTask *ptPuiRegTask = GetPuiRegTask();
            CMS_ASSERT(NULL != ptPuiRegTask && "pcPuiRegTask = NULL");
            if (!ptPuiRegTask->GetIsSupportRedis())
            {
                COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;

                CPuDevConfigNtyReq cReq;
                pcOspSipMsg->GetMsgBody(cReq);
                //modify by Royan 20140303 采纳fanxg建议，改成临时对象，而非直接修改协议引用
                //const TPuDevConfig& tPuDevConfig = cReq.GetConfig();
                TPuDevConfig tPuDevConfig = cReq.GetConfig();

                // pui上报的数据中，有一些需要用3as的数据进行覆盖
                FilterPuDevConfig(tPuDevConfig);

                TASKLOG(CMU_MOD_PU, CRITICAL_LEV, "设备[%s]配置通知\n", 
                    cReq.GetDevURI().c_str());

                int dwErrorCode = CMS_SUCCESS;

                //更新配置收集器
                m_tPuConfig.MergeDevConfig(tPuDevConfig);

                //将更新后的PuConfig写入redis
                m_pcPuData->SetPuConfig(m_tPuConfig);

                //对于非pui接入的设备需要调整设备在线状态
                if (ptPuiRegTask->GetDevType() != DEV_TYPE_PUI
					&& ptPuiRegTask->GetDevType() != DEV_TYPE_PUI_GB)
                {
                    if (m_pcPuData->GetPuIsOnline())
                    {
						m_pcPuData->PublishRedisConfig();
                        if(IsAllChnNotOnline())         // 由上线转为下线状态
                        {
                            ProcOffline();
                        }
                    }
                    else
                    {
                        if(!IsAllChnNotOnline())        // 由下线转为上线状态
                        {
                            ProcOnline();
                        }
                    }  
                }

                UpdateVidSrcName2Tas(tPuDevConfig.GetVidSrcCfgList());

                CPuDevConfigNtyRsp cRsp;
                cRsp.SetEvent(PU_DEV_CONFIG_NTF_RSP);
                cRsp.SetHeadFromReq(cReq);
                cRsp.SetErrorCode(dwErrorCode);
                PostMsgRsp(pcOspSipMsg->GetSipTransID(), cRsp);

                dwRet = PROCMSG_OK;
            }
            else
            {
                TASKLOG(CMU_MOD_PU, WARNING_LEV, "收到pu[%s]的配置上报通知，但是pui[%u]支持redis，忽略该通知!\n", 
                    m_strDevId.c_str(), ptPuiRegTask->GetTaskNO());

                dwRet = PROCMSG_FAIL;
            }
        }
        break;

    case PU_ALARM_INPUT_NTF_REQ:
        {
            CPuiRegTask *ptPuiRegTask = GetPuiRegTask();
            CMS_ASSERT(NULL != ptPuiRegTask && "pcPuiRegTask = NULL");
            
            // 注意，这里没有判断pui是否支持redis，一般情况下，如果pui支持redis，其告警信息应该是通过redis来发布的，
            // 但是有一部分比较特殊的告警（比如告警主机等），仍然需要通过sip信令来发布，因为这一类告警中的DevUri不是
            // 有效的SipUri（其实是1.0的KdmNO），如果通过sip信令发布，则其他的模块也需要将KdmNO转换为SipUri，为了减少
            // 这类告警对其他模块的影响，故而直接走信令发布到cmu，再由cmu将这类告警中的KdmNO转换为SipUri后发布
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
            if (pcOspSipMsg == NULL)
            {
                CMS_ASSERT(pcOspSipMsg && "[pcOspSipMsg指针为空]");

                TASKLOG(CMU_MOD_PU, ERROR_LEV, "ospsip 消息为空\n");

                dwRet = PROCMSG_DEL;
                return dwRet;
            }

            CPuAlarmInputNtyReq cReq;
            pcOspSipMsg->GetMsgBody(cReq);
            TAlarmInput &tAlarmInput = cReq.GetAlarm();

            u32 dwErrorCode = CMS_SUCCESS;

            // 有部分告警数据需要更新一下
            dwErrorCode = UpdateAlarm(tAlarmInput);
            if (CMS_SUCCESS == dwErrorCode)
            {
                //更新告警收集器
                if (m_pcPuData->UpdateAlarmInput(tAlarmInput))
                {
                    //发布当前告警
                    m_pcPuData->PublishRedisAlarm(tAlarmInput);
                }
                else
                {
                    dwErrorCode = ERR_CMU_ALARM_ILLEGAL;
                } 
            }

            CPuAlarmInputNtyRsp cRsp;
            cRsp.SetHeadFromReq(cReq);
            cRsp.SetErrorCode(dwErrorCode);
            PostMsgRsp(pcOspSipMsg->GetSipTransID(), cRsp);

            dwRet = PROCMSG_OK;
        }
        break;

	//{{{{{{{{{{{{{{{ add by Royan 20140527 前端录像状态通知
	case PU_RECORED_STATUS_NTF_REQ:
		{
            CPuiRegTask *ptPuiRegTask = GetPuiRegTask();
            CMS_ASSERT(NULL != ptPuiRegTask && "pcPuiRegTask = NULL");
            if (!ptPuiRegTask->GetIsSupportRedis())
            {
                COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;

                CPuRecordStatusNtfReq cReq;
                pcOspSipMsg->GetMsgBody(cReq);

                TASKLOG(CMU_MOD_PU, CRITICAL_LEV, "设备[%s:%d]前端录像状态[%d]通知\n", 
                    cReq.GetDevUri().c_str(),
                    cReq.GetDevChn().GetChnNO(),
                    cReq.GetChnRecStat().GetManualRecStat());

                int dwErrorCode = CMS_SUCCESS;

                m_pcPuData->SetIsPuRec(cReq.GetDevChn().GetChnNO(), cReq.GetChnRecStat().GetManualRecStat());

                TPuServiceStatus tPuServiceStatus;
                tPuServiceStatus.GetIsPuRec().insert(map<int,bool>::value_type(cReq.GetDevChn().GetChnNO(), cReq.GetChnRecStat().GetManualRecStat()));

                m_pcPuData->PublishRedisService(tPuServiceStatus);

                CPuRecordStatusNtfRsp cRsp;
                cRsp.SetHeadFromReq(cReq);
                cRsp.SetErrorCode(dwErrorCode);
                PostMsgRsp(pcOspSipMsg->GetSipTransID(), cRsp);

                dwRet = PROCMSG_OK;
            }
            else
            {
                TASKLOG(CMU_MOD_PU, WARNING_LEV, "收到pu[%s]的服务状态上报通知，但是pui[%u]支持redis，忽略该通知!\n", 
                    m_strDevId.c_str(), ptPuiRegTask->GetTaskNO());

                dwRet = PROCMSG_FAIL;
            }
		}
		break;
	//}}}}}}}}}}}}}}} add by Royan 20140527 前端录像状态通知

    case PU_GPS_DATA_NTF_REQ:
        {
            CPuiRegTask *ptPuiRegTask = GetPuiRegTask();
            CMS_ASSERT(NULL != ptPuiRegTask && "pcPuiRegTask = NULL");
            if (!ptPuiRegTask->GetIsSupportRedis())
            {
                COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;

                CPuGpsDataNtyReq cReq;
                pcOspSipMsg->GetMsgBody(cReq);
                const TGpsData& tGpsData = cReq.GetData();

                TASKLOG(CMU_MOD_PU, CRITICAL_LEV, "设备[%s]GPS数据通知\n", 
                    m_strDevId.c_str());

                int dwErrorCode = CMS_SUCCESS;

                m_pcPuData->SetPuGpsData(tGpsData);

                //发布GPS数据
                m_pcPuData->PublishRedisGpsData(tGpsData);

                CPuGpsDataNtyRsp cRsp;
                cRsp.SetHeadFromReq(cReq);
                cRsp.SetErrorCode(dwErrorCode);
                PostMsgRsp(pcOspSipMsg->GetSipTransID(), cRsp);

                dwRet = PROCMSG_OK;
            }
            else
            {
                TASKLOG(CMU_MOD_PU, WARNING_LEV, "收到pu[%s]的GPS上报通知，但是pui[%u]支持redis，忽略该通知!\n", 
                    m_strDevId.c_str(), ptPuiRegTask->GetTaskNO());

                dwRet = PROCMSG_FAIL;
            }
        }
        break;

    case PU_TRANSPARENT_DATA_NTF_REQ:
        {
            CPuiRegTask *ptPuiRegTask = GetPuiRegTask();
            CMS_ASSERT(NULL != ptPuiRegTask && "pcPuiRegTask = NULL");
            if (!ptPuiRegTask->GetIsSupportRedis())
            {
                COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;

                CPuTransparentDataNtfReq cReq;
                pcOspSipMsg->GetMsgBody(cReq);
                const TTransparentData& tTransData = cReq.GetData();

                TASKLOG(CMU_MOD_PU, CRITICAL_LEV, "设备[%s]-[%s]数据通知\n", 
                    m_strDevId.c_str(), tTransData.GetType().c_str());

                int dwErrorCode = CMS_SUCCESS;

                m_pcPuData->SetPuTransparentData(tTransData);

                //发布数据
                m_pcPuData->PublishRedisTransData(tTransData);

                CPuTransparentDataNtfRsp cRsp;
                cRsp.SetHeadFromReq(cReq);
                cRsp.SetErrorCode(dwErrorCode);
                PostMsgRsp(pcOspSipMsg->GetSipTransID(), cRsp);

                dwRet = PROCMSG_OK;
            }
            else
            {
                TASKLOG(CMU_MOD_PU, WARNING_LEV, "收到pu[%s]的透明数据上报通知，但是pui[%u]支持redis，忽略该通知!\n", 
                    m_strDevId.c_str(), ptPuiRegTask->GetTaskNO());

                dwRet = PROCMSG_FAIL;
            }
        }
        break;

    case PU_UNREG_REQ:
        {
            UnregProc(pcMsg);
            dwRet = PROCMSG_DEL;
        }
        break;

	case TAS_DEVICE_CAP_INDEX_NAME_SET_RSP:
		{
			COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;

			CDeviceCapIndexNameSetRsp cRsp;
			pcOspSipMsg->GetMsgBody(cRsp);

			TASKLOG(CMU_MOD_PU, EVENT_LEV, "收到3as返回的视频源别名同步应答!\n");

			if (CMS_SUCCESS != cRsp.GetErrorCode())
			{
				TASKLOG(CMU_MOD_PU, ERROR_LEV, "同步前端视频源别名信息到3as失败!\n");
			}

			// 通知流控管理Task该任务已经处理完毕
			((CCmuCoreInst *)GetInstance())->DelFlowCtrlJob(GetTaskNO());

			dwRet = PROCMSG_OK;
		}
		break;

    default:
        {
            TASKLOG(CMU_MOD_PU, WARNING_LEV, "Receive unknown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }

    return dwRet;
}

u32 CPuRegTask::OnServiceTimer()
{
    return TIMERPOLL_DONE;
}

void CPuRegTask::UnregProc(CMessage *const pcMsg)
{
    int dwErrorCode = CMS_SUCCESS;
    COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
    if (pcOspSipMsg == NULL)
    {
        TASKLOG(CMU_MOD_PU, ERROR_LEV, "ospsip 消息为空\n");
        return;
    }

    CPuUnRegReq cReq;
    pcOspSipMsg->GetMsgBody(cReq);

    CPuUnRegRsp cRsp;
    cRsp.SetErrorCode(dwErrorCode);
    cRsp.SetSeqNum(cReq.GetSeqNum());
    cRsp.SetSession(GetSession());
    PostMsgRsp(pcOspSipMsg->GetSipTransID(), cRsp);
}

void CPuRegTask::DisconnectPu()
{
    if ((GetState() == Service || GetState() == WaitCfgNtf)
        && NULL != m_pcPuData)
    {
        //已经注册的pu才需要强制断开
        CCmuDisconnectPuReq cReq;
        cReq.SetSession(GetSession());
		cReq.SetDevUri(m_pcPuData->GetDevUri().GetURIString());

        PostMsgReq(cReq, GetPuiUri());
    }
}

void CPuRegTask::ForceOffline(const string &strDevId)
{
    if (m_strDevId == strDevId)
    {
        DisconnectPu();

        delete this;
    }
}

CPuiRegTask* CPuRegTask::GetPuiRegTask()
{
    CLoginSession* ptPuiRegTask = NULL;
    g_cPuiList.Find(GetPuiUri().GetURIString(), ptPuiRegTask);

    return (CPuiRegTask*)ptPuiRegTask;
}

void CPuRegTask::FilterBaseConfig(TPuBaseConfig &tPuBaseConfig)
{
	if (NULL != m_pcPuData)
	{
		//设置设备名称
		tPuBaseConfig.SetDevName(m_pcPuData->GetDevName());

		//设置设备厂商
		tPuBaseConfig.SetManufacturer(m_pcPuData->GetDevManufacturer());

		//设置国标标签
		tPuBaseConfig.SetIsGBDevice(m_pcPuData->GetDevGBFlag());
	}
}

void CPuRegTask::FilterPuDevConfig(TPuDevConfig &tPuDevConfig)
{
	if (NULL != m_pcPuData)		// 对于以下数据，直接取3as的数据，不接受pui的变更
	{
		CPuBaseCfgList &cBaseCfg = tPuDevConfig.GetBaseCfgList();
		if (!cBaseCfg.empty())
		{
			FilterBaseConfig(cBaseCfg.at(0));
		}

		CPuVidSrcCfgList &cVidSrcCfg = tPuDevConfig.GetVidSrcCfgList();
		if (!cVidSrcCfg.empty())
		{
			// 备份好前端上报的视频源别名信息
			for (map<int, TPuVidSrcConfig>::const_iterator it = cVidSrcCfg.begin(); cVidSrcCfg.end() != it; ++it)
			{
				m_cVidSrcNameBak[it->first] = it->second.GetName();
			}
		}

		if (!g_cCmuConf.m_bSyncVidSrcName2Plat)
		{
			for (map<int, TPuVidSrcConfig>::iterator it = cVidSrcCfg.begin(); cVidSrcCfg.end() != it; ++it)
			{
				it->second.SetName(m_pcPuData->GetVidSrcAlias(it->first));
			}
		}
	}
}

void CPuRegTask::UpdateVidSrcName2Tas(const CPuVidSrcCfgList& cPuCfg)
{
	for (CPuVidSrcCfgList::const_iterator it = cPuCfg.begin(); cPuCfg.end() != it; ++it)
	{
		m_cVidSrcNameBak[it->first] = it->second.GetName();
	}

    ((CCmuCoreInst *)GetInstance())->AddFlowCtrlJob(GetTaskNO());
}

void CPuRegTask::UpdateVidSrcName2Tas()
{
	//如果不需要同步到tas，直接return
	if (!g_cCmuConf.m_bSyncVidSrcName2Plat)
	{
		return;
	}

	CDeviceCapIndexNameSetReq cReq;
	cReq.SetSession(g_pcRegTasTask->GetSession());
	cReq.GetDeviceCapName().deviceId = m_strDevId;
	cReq.GetDeviceCapName().capId = DEVICE_CAP_VIDEOSOURCE;

	TDeviceCapIndexName tDeviceCapIndexName;

	//覆盖3as中的视频源名称同步
	if (!m_cVidSrcNameBak.empty())
	{
        map<s32, string> cVidSrcAlias = m_pcPuData->GetVidSrcAliasMap();

        //对于视频源名称为空或者上报不支持同步的g100前端来说，不需要同步
        map<int,int> cExtCapSet = m_pcPuData->GetExtCapSet();
        map<int,int>::iterator iter = cExtCapSet.find(VSIP_ENTITY_G100_SUPPORT_VIDEOINPUTNAME_SYNC);
        if (iter == cExtCapSet.end())
        {
            for (map<s32, string>::const_iterator it = m_cVidSrcNameBak.begin(); m_cVidSrcNameBak.end() != it; ++it)
            {
                if (!it->second.empty())
                {
                    map<s32, string>::iterator it4Mod = cVidSrcAlias.find(it->first);
                    if (cVidSrcAlias.end() != it4Mod && it4Mod->second == it->second)       // 两个名字相同，则跳过该同步
                    {
                        continue;
                    }
                    else
                    {
                        tDeviceCapIndexName.index = it->first;
                        //modify by ljn 20170720
                        tDeviceCapIndexName.name = GetUTF8String(it->second);
                        cReq.GetDeviceCapName().deviceCapIndexNames.push_back(tDeviceCapIndexName);
                    }
                }
            }
        }
	}

	if (!cReq.GetDeviceCapName().deviceCapIndexNames.empty())
	{
		PostMsgReq(cReq,GetPuiUri().Get3ASURI());
	}
}

void CPuRegTask::SendPuOfflineTooBrief()
{
	string strContent;

	if (m_pcPuData == NULL || !m_pcPuData->IsOfflineTooBrief(strContent))
	{
		return;
	}

// 	if ( strContent.empty() )
// 	{
// 		TASKLOG(CMU_MOD_PU, ERROR_LEV, "设备频繁下线了，但是没有记录？ DevId[%s]\n", m_strDevId.c_str());
// 	}

	CAlarmEventNtfReq cReq;

	cReq.SetEventType(GetLanDesc(LAN_DESC_PU_OFFLINE_TOO_BRIEF));
	cReq.SetEventOccurTime(time(NULL));
	cReq.SetEventContent(strContent);

	cReq.SetDevUri(m_pcPuData->GetDevUri().GetURIString());
	cReq.SetDevType(m_pcPuData->GetDevTypeStr());

	Iterator pPos;
	string strUri;
	CLoginSession* pTask = NULL;

	while (!pPos.End())
	{
		if (g_cCuiList.Iterate(pPos, strUri, pTask) && pTask != NULL)
		{
			CCuiRegTask* pCuiRegTask = (CCuiRegTask*)pTask;

			this->PostMsgReq(cReq, pCuiRegTask->GetDevUri());
		}
	}

	// UAS日志
	((CCmuCoreInst*)GetInstance())->GetUasLogTask()->AddDeviceLog(this->GetDevLogInfoOfflineTooBrief());
}

TDeviceLogInfo CPuRegTask::GetDevLogInfoOfflineTooBrief()
{
	TDeviceLogInfo tLogInfo = this->GetDevLogInfo(false);
	tLogInfo.logDesc = GetLanDesc(LAN_DESC_PU_OFFLINE_TOO_BRIEF);
	return tLogInfo;
}

TDeviceLogInfo CPuRegTask::GetDevLogInfo(bool bOnOffState)
{
    TDeviceLogInfo tLogInfo;

    if (m_pcPuData != NULL)
    {
        tLogInfo.devUri = m_pcPuData->GetDevUri().GetURIString();

        CDeviceInfo cDeviceInfo = m_pcPuData->GetDevInfo();
        //modify by ljn 20170720
        tLogInfo.devName  = GetUTF8String(cDeviceInfo.GetDeviceName());
        tLogInfo.devAlias = GetUTF8String(cDeviceInfo.GetDeviceAlias());
    }

    tLogInfo.happenedTime = CCMSTime().ToString();
    tLogInfo.logType = (bOnOffState ? enDeviceLogTypeOnline : enDeviceLogTypeOffline);

	if (NULL != m_pcPuData)
	{
		tLogInfo.netAddr = m_pcPuData->GetPuConnectAddr();
	}

    if (bOnOffState)
    {
        tLogInfo.logDesc = GetLanDesc(LAN_DESC_ONLINE);
    }
    else
    {
        tLogInfo.logDesc = GetLanDesc(LAN_DESC_OFFLINE);
    }
    
    return tLogInfo;
}

u32 CPuRegTask::UpdateAlarm(TAlarmInput &tAlarmInput)
{
    u32 dwRet = CMS_SUCCESS;

    do 
    {
        if (NULL == m_pcPuData)
        {
            dwRet = ERR_CMU_PU_NOT_IN_TOP;
            break;
        }

        //如果是智能告警或视质轮询，则把对应的编码通道号转换为视频源通道号，应对告警上报辅流通道号的情况。
        int nVidSrcIdx = tAlarmInput.GetInputId();
        if ((ALARM_TYPE_INTELLIGENT == tAlarmInput.GetAlarmType() || ALARM_TYPE_VIDEOPOLL == tAlarmInput.GetAlarmType()))
        {
            CVidSrcSet cVidSrcSet = m_pcPuData->GetChnVidSrc(nVidSrcIdx);
            if (!cVidSrcSet.empty())
            {
                nVidSrcIdx = *cVidSrcSet.begin();
                TASKLOG(CMU_MOD_PU, EVENT_LEV, "告警通道号[%d]转换为视频源号[%d]\n",
                    tAlarmInput.GetInputId(), nVidSrcIdx);
                tAlarmInput.SetInputId(nVidSrcIdx);
            }
        }

        if ((ALARM_TYPE_VIDEOPOLL == tAlarmInput.GetAlarmType() || ALARM_TYPE_INTELLIGENT == tAlarmInput.GetAlarmType()) 
            && FALSE == IsSipUri(tAlarmInput.GetDevUri()))
        {
            TPuIdUriMapList::const_iterator itor = g_cPuIdUriMapList.find(tAlarmInput.GetDevUri());
            if (g_cPuIdUriMapList.end() == itor)
            {
                TASKLOG(CMU_MOD_PU, ERROR_LEV, "PU_ALARM_INPUT_NTF_REQ 未找到PUID[%s]对应的URI\n",
                    tAlarmInput.GetDevUri().c_str());

                dwRet = ERR_CMU_PU_NOT_IN_TOP;
                break;
            }

            tAlarmInput.SetDevUri(itor->second);
        }

        TASKLOG(CMU_MOD_PU, CRITICAL_LEV, "设备[%s][%s]告警通知:[inputId-%d] [flag-%s] [data-%s]\n", 
            tAlarmInput.GetDevUri().c_str(), tAlarmInput.GetAlarmType().c_str(),
            tAlarmInput.GetInputId(), tAlarmInput.GetAlarmFlag().c_str(), 
            tAlarmInput.GetAlarmData().c_str());

        //更新告警别名
        UpdateAlarmAlias(tAlarmInput);

    } while (false);

    return dwRet;
}

void CPuRegTask::WriteAlarmLog2Uas(const TAlarmInput& tAlarmInput)
{
    ((CCmuCoreInst*)GetInstance())->GetUasLogTask()->AddAlarmLog(GetAlarmLogInfo(tAlarmInput));
}

TAlarmLogInfo CPuRegTask::GetAlarmLogInfo(const TAlarmInput& tAlarmInput)
{
    CMS_ASSERT(m_pcPuData!=NULL&&"m_ptPuData==NULL\n");

    TAlarmLogInfo tLogInfo;
    tLogInfo.devUri = m_pcPuData->GetDevUri().GetURIString();
    tLogInfo.devName = GetUTF8String(m_pcPuData->GetDevName());
    tLogInfo.devAlias = GetUTF8String(m_pcPuData->GetDevAlias());
	tLogInfo.alias = GetUTF8String(tAlarmInput.GetAlarmAlias());
    tLogInfo.channId = tAlarmInput.GetInputId();
    tLogInfo.type = tAlarmInput.GetAlarmType();;
    tLogInfo.flag = tAlarmInput.GetAlarmFlag();
    if (tAlarmInput.GetAlarmFlag() == ALARM_FLAG_OCCUR)
    {
        tLogInfo.occurTime = tAlarmInput.GetAlarmTime();       
    }
    else
    {
        tLogInfo.resumeTime = tAlarmInput.GetAlarmTime();
    }

    //tLogInfo.desc = tAlarmInput.GetAlarmData();//日志描述不应该是告警数据，而应该是固定格式的描述信息。
    
    return tLogInfo;
}

size_t CPuRegTask::GetDevNetSegNO()
{
    size_t nPuNetSegNO = 0;
    CPuiRegTask* pcPuiTask = GetPuiRegTask();
    if (pcPuiTask == NULL)
    {
        TASKLOG(CMU_MOD_PU, ERROR_LEV, "pcPuiTask == NULL\n");
        return nPuNetSegNO;
    }

    const CIpAddrList& cPuiAddrList = pcPuiTask->GetDevAddrList();
    for (size_t i=0; i<cPuiAddrList.size(); i++)
    {
        const TNetAddr& tPuiAddr = cPuiAddrList.at(i);
        if (tPuiAddr.GetNetIp() == GetPuiIp())
        {
            nPuNetSegNO = i;
            TASKLOG(CMU_MOD_PU, CRITICAL_LEV, "pu[%s]-usedPuiIp[%s]使用pui[%s]的NetSegNO[%u]接入\n", 
                m_strDevId.c_str(), GetPuiIp().c_str(), 
                pcPuiTask->GetDevUri().c_str(), nPuNetSegNO);

            return nPuNetSegNO;
        }
    }

    TASKLOG(CMU_MOD_PU, ERROR_LEV, "pu[%s]-usedPuiIp[%s]没有找到pui[%s]对应的NetSegNO\n", 
        m_strDevId.c_str(), GetPuiIp().c_str(), pcPuiTask->GetDevUri().c_str());

    return nPuNetSegNO;
}

string CPuRegTask::GetJoinInterfaceIp(const string &strSession)
{
    return GetPuiIp();
}

BOOL32 CPuRegTask::IsSipUri(const string& strDevUri)
{
    TSipURI tDevUri;
    tDevUri.SetURIString(strDevUri);

    return tDevUri.IsValidURI();
}

bool CPuRegTask::IsAllChnNotOnline() const
{
    if (m_pcPuData == NULL)
    {
        return true;
    }

    return m_pcPuData->IsAllChnNotOnline();  
}

bool CPuRegTask::StartWork()
{
	if (!g_cCmuConf.m_bSyncVidSrcName2Plat || m_cVidSrcNameBak.empty())
	{
		return false;
	}
	
	// 将缓存的视频源信息同步到3as
	UpdateVidSrcName2Tas();

	return true;
}

bool CPuRegTask::CheckBaseConfig(IN TPuBaseConfig& tBaseConfig, OUT vector<TDeviceCapInfo>& vctDevCapInfo)
{
	bool bChg = false;

    CDeviceInfo cDeviceInfo = m_pcPuData->GetDevInfo();
    vctDevCapInfo = cDeviceInfo.GetDeviceModelCapDetail();

    const TPuBaseCapSet &tBaseCapSet = tBaseConfig.GetBaseCapSet();
    s32 nDevType = cDeviceInfo.GetDeviceType();
	if (DEVICE_TYPE_ENCODER == nDevType)
	{
        for (vector<TDeviceCapInfo>::iterator it = vctDevCapInfo.begin(); it != vctDevCapInfo.end(); ++it)
        {
            if (DEVICE_CAP_VIDEOSOURCE == it->type)
            {
                if (tBaseCapSet.GetVideoInPortNum() != it->num)
                {
                    it->num = tBaseCapSet.GetVideoInPortNum();
                    bChg = true;
                }
            }
            else if (DEVICE_CAP_ENCODE_CHANNEL == it->type)
            {
                if (tBaseCapSet.GetVidEncNum() != it->num)
                {
                    it->num = tBaseCapSet.GetVidEncNum();
                    bChg = true;
                }
            }
            else if (DEVICE_CAP_PARALLEL_PORT_ALARM_INPUT == it->type)
            {
                if (tBaseCapSet.GetAlarmInNum() != it->num)
                {
                    it->num = tBaseCapSet.GetAlarmInNum();
 			        //取消并口输入数变化判断
                    //bChg = true;
                }
            }
        }
	}
	else if (DEVICE_TYPE_DECODER == nDevType)
	{
        //编码器判断 视频解码通道数/HDMI数 是否有变化
        u16 wHdmiVidDecNum = 1;
        const map<int,int> &tExtCapSet = tBaseConfig.GetExtCapSet();
        map<int,int>::const_iterator it4Dec = tExtCapSet.find(VSIP_ENTITY_HMDI_COUNT);
        if (tExtCapSet.end() != it4Dec)
        {
            wHdmiVidDecNum = it4Dec->second;
        }

        for (vector<TDeviceCapInfo>::iterator it = vctDevCapInfo.begin(); it != vctDevCapInfo.end(); ++it)
        {
            if (DEVICE_CAP_DECODE_CHANNEL == it->type)
            {
                if (tBaseCapSet.GetVidDecNum() != it->num)
                {
                    it->num = tBaseCapSet.GetVidDecNum();
                    bChg = true;
                }
            }
            else if (DEVICE_CAP_DECODE_OUTPUTCHANNEL == it->type)
            {
                if (wHdmiVidDecNum != it->num)
                {
                    it->num = wHdmiVidDecNum;
                    bChg = true;
                }
            }
        }
	}
	else
	{
		TASKLOG(CMU_MOD_PU, WARNING_LEV, "Unknown Device Type[%d]\n", m_pcPuData->GetDevInfo().GetDeviceType());
	}

	return bChg;
}

void CPuRegTask::UpdateAlarmAlias(TAlarmInput& tAlarmInput)
{
	/*
    1.并口告警需要从3as获取并口别名，根据规则生成告警别名；如果用户没有设置过别名，则用告警通道生成
    2.移动侦测和视频源丢失需要从3as获取视频源别名，根据规则生成告警别名；如果用户没有设置过别名，则用视频源通道生成
    3.生成别名时需要支持多国语言
    */
	string strAlarmAlias;
    string strAlarmType = tAlarmInput.GetAlarmType();
    if (strAlarmType == ALARM_TYPE_PIN)
    {
        // todo by xiongxh strAlarmAlias = m_pcPuData->GetInputPinAlias(tAlarmInput.GetInputId());
    }
    else if (strAlarmType == ALARM_TYPE_MOVE)
    {
        strAlarmAlias = m_pcPuData->GetVidSrcAlias(tAlarmInput.GetInputId());
    }
    else if (strAlarmType == ALARM_TYPE_VIDLOST)
    {
        strAlarmAlias = m_pcPuData->GetVidSrcAlias(tAlarmInput.GetInputId());
    }
    else if (strAlarmType == ALARM_TYPE_DISKFULL)
    {
        strAlarmAlias = GetLanDesc(LAN_DESC_DISK_FULL);
    }
	else if (strAlarmType == ALARM_TYPE_INTELLIGENT)
	{
		strAlarmAlias = m_pcPuData->GetVidSrcAlias(tAlarmInput.GetInputId());
	}
	else if (strAlarmType == ALARM_TYPE_VIDEOPOLL)
	{
		strAlarmAlias = m_pcPuData->GetVidSrcAlias(tAlarmInput.GetInputId());
	}
	//添加基础智能告警子类型
	else if (strAlarmType == ALARM_TYPE_EXCEPTION_EVENT
		|| strAlarmType == ALARM_TYPE_DEFOCUS
		|| strAlarmType == ALARM_TYPE_SCENE_CHANGE
		|| strAlarmType == ALARM_TYPE_REGION_INVASION
		|| strAlarmType == ALARM_TYPE_REGION_LEAVING
		|| strAlarmType == ALARM_TYPE_OBJECT_TAKEN
		|| strAlarmType == ALARM_TYPE_OBJECT_LEFT
		|| strAlarmType == ALARM_TYPE_PEOPLE_GATHER
		|| strAlarmType == ALARM_TYPE_CORGON
		|| strAlarmType == ALARM_TYPE_AUDIO_ABNORMAL
		|| strAlarmType == ALARM_TYPE_LUMA_CHANGE)
	{
		strAlarmAlias = m_pcPuData->GetVidSrcAlias(tAlarmInput.GetInputId());
	}

    else
    {
        strAlarmAlias = "";
    }

	tAlarmInput.SetAlarmAlias(strAlarmAlias);
}

CLoginSession *CPuRegTask::GetLoginSession(const string &strDevId)
{
    if (m_strDevId == strDevId)
    {
        return this;
    }

    return NULL;
}

void CPuRegTask::ProcOnline()
{
    CPuiRegTask* ptPuiRegTask = GetPuiRegTask();
    CMS_ASSERT(NULL != ptPuiRegTask && "ptPuiRegTask = NULL!!!");

    m_pcPuData->SetPuIsOnline(true);

    // 状态发布（上下线肯定是要发布到redis和sip订阅者的）
    m_pcPuData->PublishOnOff();

    // 如果pui不支持redis，则cmu还需要额外代替pui发送其他类型的通知
    if (!ptPuiRegTask->GetIsSupportRedis())
    {
        m_pcPuData->PublishRedisConfig();

        // 设备上线后，清除上一次的服务状态PuServiceStatus和告警信息PuAlarmStatus
        // 至于GpsData和TransData，则由于不写入redis，只是发布，所以无需处理
        m_pcPuData->ClearPuServiceStatus();
        m_pcPuData->ClearAlarmStatus();
    }    

	////上线需要触发GPS激活
	//m_pcPuData->EnableGps();

    //向uas写设备日志
    ((CCmuCoreInst*)GetInstance())->GetUasLogTask()->AddDeviceLog(GetDevLogInfo(true));
}

void CPuRegTask::ProcOffline()
{
    switch(GetState())
    {
    case WaitReg:
        {

        }
        break;
    case WaitCfgNtf:
        {
            DestroyChildTasks();
        }
        break;
    case Service:
        {
            //向uas写设备告警恢复日志，正在告警的才恢复
            TPuAlarmStatus tPuAlarmStatus = m_pcPuData->GetPuAlarmStatus();
            const CAlarmInputArray& tAlarmInputArray = tPuAlarmStatus.GetAlarmArray();
            for (size_t i = 0; i < tAlarmInputArray.size(); i++)
            {
                TAlarmInput tAlarmInput = tAlarmInputArray.at(i);

                if (ALARM_FLAG_OCCUR == tAlarmInput.GetAlarmFlag())
                {
                    //设置恢复状态
                    tAlarmInput.SetAlarmFlag(ALARM_FLAG_RESTORE);

                    //向uas写告警日志
                    ((CCmuCoreInst*)GetInstance())->GetUasLogTask()->AddAlarmLog(GetAlarmLogInfo(tAlarmInput));
                }
            }

            bool bOnline = m_pcPuData->GetPuIsOnline();

            // 设置下线状态等信息
            m_pcPuData->SetPuIsOnline(false);

            m_pcPuData->CleanupDevChnState();

            this->SendPuOfflineTooBrief();

            //向uas写设备下线日志
            ((CCmuCoreInst*)GetInstance())->GetUasLogTask()->AddPuLog(GetDevLogInfo(false));

            // 发布状态通知（由上线到下线才发送通知，如果之前就是下线，现在也是下线，则没有必要发送）
            if (bOnline)
            {
                m_pcPuData->PublishOnOff();
            }

            //清除该登录会话下所有事务
            DestroyChildTasks();
        }
        break;
    default:
        {
            TASKLOG(CMU_MOD_PUI, ERROR_LEV, "ProcOffline未知事务状态[%d]\n", GetState());
        }
        break;
    }
}

bool CPuRegTask::IsUTF8(IN unsigned char *pBuf, IN int strLen)
{
    const unsigned char byMask6 = 0xFC;//11111100//6个字节
    const unsigned char byMask5 = 0xF8;//11111000//5个字节
    const unsigned char byMask4 = 0xF0;//11110000//4个字节
    const unsigned char byMask3 = 0xE0;//11100000//3个字节
    const unsigned char byMask2 = 0xC0;//11000000//2个字节
    const unsigned char byMask1 = 0x80;//10000000//小于此值的1个字节
    for (int i = 0; i < strLen; i++)
    {
        unsigned char uch = pBuf[i];//首字节
        if (uch == 0 && i < strLen - 1)
        {
            //不是结尾发现'\0'，那肯定不是utf8了
            return false;
        }
        if (uch < byMask1)
        {
            continue;
        }
        if ((uch & 0xC0) == 0x80)//首字节在 0x80 和0xc0之间，就是非法
        {
            return false;
        }
        for (int n = 2; n <= 6; n++)
        {
            //这个掩码表示的后面还有几个字节数
            int nNeedChar = 7 - n;
            //生成掩码
            unsigned char byMask = ((0xff >> n) << n);
            if (uch >= byMask)
            {
                //检测剩余字节的数量是否符合utf8规则
                if (strLen - i - 1 < nNeedChar)
                {
                    return false;
                }
                //检测剩余字节数的数值是否符合utf8规则
                for (int k = 0; k < nNeedChar; k++)
                {
                    //每个字节都必须符合 10xxxxxx 格式
                    if ((pBuf[i + k + 1] & 0xC0) != 0x80)
                    {
                        return false;
                    }
                }
                //都符合了
                i += nNeedChar;
                break;
            }
        }

    }
    return true;
}

string CPuRegTask::GetUTF8String(const string& strOri)
{
    bool bUTF8 = IsUTF8((u8 *)strOri.c_str(), strOri.length());
    return bUTF8 ? strOri : GBKToUTF8(strOri);;
}

