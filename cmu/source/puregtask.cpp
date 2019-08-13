
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
        //���ڵ������ȱ�ɾ��
        delete ptExistTask;
        ptExistTask = NULL;
    }

    m_tPuPtzCtrlTaskList[nVidSrcChn] = ptTask;
}

void CPuRegBaseTask::ErasePtzCtrlTask(const TChannel &nVidSrcChn)
{
   //ֻ������е�ָ�룬������ɾ������
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
       �����豸ע��ļ�����ԣ�Ŀǰ���ò���1
       ����1������ͨ������licence��ͬʱУ�顣��У����ƵԴ��
       ####�������⣺
                1.��֧��NVR64ͨ��ֻ��32������ͨ�����豸ע��
                2.�����������豸���ڿ�/�ؽ���Ӧ�����豸���ͣ���Ҫ��tas���޸��豸����

       ����2����ƵԴ����licence��ͬʱУ�顣��У�����ͨ����
       ####�������⣺
                1. ���ַ�����Ȼ���Խ������1������2����Ҫ���û����豸����ʱ��д�豸��
                ���ͨ���������Ҫ����в�ȷ���ԣ�һ���������ʹ��ƵԴ�ͱ���ͨ���Ĺ�ϵ
                ���ִ��ң����һϵ�еĲ�����������絥��ƵԴ��ͨ���豸���ܳ������ʧ�ܵ������
    */

    //ͨ��������
    //todo
    //liangli ������װ��Ŀ�У�NVR����License��һ����ˣ������ǿ����ϱ�4ͨ������8ͨ����ƽ̨
    //�ɴ��ƽ̨1.0����һ�²��ԣ�ֻУ��License�����жϱ���ͨ��
    
    //licenceУ��
    /*
      2014-02-20 fanxg
      licenceУ���������
      1. �⳧���豸����Ҳ�շѡ�
      2. �ϱ���licence��ֻҪ����������licence���ɣ���Ҫ�����
      3. ͨ��GBSģ��������豸����Ҫ����licenceУ��
      4. (2014-03-18)��ȷ�� �⳧���豸������ǿ���շѣ��Ƿ��շ���freeLicence�ֶ�ȷ��
    */

    if (tCreater != CREATE_BY_GBS_FOREIGN_DOMAIN)
    {
        s32 nPuLicenseNum = nVidEncNum - nFreeLicenseNum;

        //�ϱ���licence��ֻҪ����������licence���ɣ���Ҫ�����
        if (nPuLicenseNum > nLicenseNum)
        {
            GLOBELOG(CMU_MOD_PU, ERROR_LEV, "pu[%s]��ص�����ƥ�䣬����[<=%d],ʵ��[%d]\n",
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

	OspPrintf(TRUE, FALSE, "PuiIP��%s\n",
		GetPuiIp().c_str());

	if (!m_cVidSrcNameBak.empty())
	{
		OspPrintf(TRUE, FALSE, "�����ǰ����ƵԴ������Ϣ���£�\n");
		for (map<s32, string>::const_iterator it = m_cVidSrcNameBak.begin(); m_cVidSrcNameBak.end() != it; ++it)
		{
			OspPrintf(TRUE, FALSE, "  ChnNO[%d], Name[%s]\n", it->first, UTF8ToGBK(it->second).c_str());
		}
	}
}

void CPuRegTask::ReleaseResource(bool bStay)
{
    TASKLOG(CMU_MOD_PU, EVENT_LEV, "pu[%s]ע����������\n", m_strDevId.c_str());

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

            // ���PuData�м�¼��ע��Taskָ��
            m_pcPuData->SetPuRegTask(NULL);

            //ɾ��pui�еǼǵ�puע������
            CPuiRegTask* ptPuiRegTask = GetPuiRegTask();
            if (ptPuiRegTask != NULL)
            {
                ptPuiRegTask->DelTask(this);
            }   
        }
        break;
    default:
        {
            TASKLOG(CMU_MOD_PUI, ERROR_LEV, "δ֪����״̬[%d]\n", GetState());
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
                TASKLOG(CMU_MOD_PU, ERROR_LEV, "ospsip ��ϢΪ��\n");
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

            TASKLOG(CMU_MOD_PU, CRITICAL_LEV, "PU[%s]ͨ��PUI[%s]PuiIP[%s]��CMUע��\n",
                m_strDevId.c_str(), GetPuiUri().GetURIString().c_str(), GetPuiIp().c_str());

            int nPuNetType = (int)DEVICE_LINEMODE_INTRANET;

			CPuiRegTask* ptPuiRegTask = GetPuiRegTask();
            do 
            {
                if (ptPuiRegTask == NULL)			//;����pui�Ƿ�ע��
                {
                    TASKLOG(CMU_MOD_PU, ERROR_LEV, "pu��¼ʹ�õ�pui[%s]δ��cmuע��\n",
                        GetPuiUri().GetURIString().c_str());
                    dwErrorCode = ERR_CMU_PUI_NOT_REG;
                    break;
                }

				//�Ƿ�����
                m_pcPuData = CCmuData::GetInstance()->GetPuData(m_strDevId, true);
				if (NULL == m_pcPuData)
				{
					TASKLOG(CMU_MOD_PU, ERROR_LEV, "pu[%s]δ������ע��ip[%s]\n",
						m_strDevId.c_str(), cReq.GetBaseConfig().GetDevIp().c_str());
					dwErrorCode = ERR_CMU_PU_NOT_IN_TOP;
					break;
				}

				//�Ƿ�����
				if (!m_pcPuData->IsPuInUse())
				{
					TASKLOG(CMU_MOD_PU, ERROR_LEV, "pu[%s]δ����\n", m_strDevId.c_str());
					dwErrorCode = ERR_CMU_PU_NOT_USED;
					break;
				}

				//�Ƿ����
				if (m_pcPuData->IsPuExpired())
				{
					TASKLOG(CMU_MOD_PU, ERROR_LEV, "pu[%s]�Ѿ�����\n", m_strDevId.c_str());
					dwErrorCode = ERR_CMU_PU_EXPIRE;
					break;
				}

                //�Ƿ��Ѿ�ע��
                if (m_pcPuData->IsPuRegistered())
                {
                    TASKLOG(CMU_MOD_PU, ERROR_LEV, "pu[%s]�Ѿ�ע��\n", m_strDevId.c_str());
                    dwErrorCode = ERR_CMU_PU_ALREADY_REG;
                    break;
                }

                CDeviceInfo cDevInfo = m_pcPuData->GetDevInfo();
                s32 nVidEncNum = cReq.GetBaseConfig().GetBaseCapSet().GetVidEncNum();
                s32 nFreeLicenseNum = cReq.GeBaseConfig().GetFreeLicenceNum();
                dwErrorCode = ValidLicense(cDevInfo, nVidEncNum, nFreeLicenseNum);
                if (CMS_SUCCESS != dwErrorCode)
                {
                    TASKLOG(CMU_MOD_PU, ERROR_LEV, "pu[%s]license��֤ʧ�ܣ�VidEncNum[%d], FreeNum[%d]\n", 
                        m_strDevId.c_str(), nVidEncNum, nFreeLicenseNum);
                    break;
                }
               
                //��������
                nPuNetType = cDevInfo.GetNetType();

                //����punch����
                if (cReq.GetIsSupportNA())
                {
                    SetIsNeedPunch(true);

                    //���������ȫ������pu��ƽ̨�������ϵ
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
                //ע��ʧ��
                TASKLOG(CMU_MOD_PU, ERROR_LEV, "pu[%s]��¼cmuʧ��, ������[%d]!\n", m_strDevId.c_str(), dwErrorCode);

                return PROCMSG_DEL;
            }
  
            //��¼�ɹ�
            TASKLOG(CMU_MOD_PU, CRITICAL_LEV, "pu[%s]��ȡ�������óɹ�����ʼ��ȡ����ͨ������!\n", m_strDevId.c_str());

			//У��BaseConfig���б仯��֪ͨ3as
			CDevCapInfoModReq cModReq;
			if (!ptPuiRegTask->GetIsSupportRedis() && g_cCmuConf.m_bModDevModel && CheckBaseConfig(cReq.GetBaseConfig(), cModReq.GetDeviceModelCapDetail()))
			{
                TASKLOG(CMU_MOD_PU, CRITICAL_LEV, "pu[%s]�������ú�������һ�£���Ҫ�޸ġ�\n", m_strDevId.c_str());
                //֪ͨ��3as�������������仯
                cModReq.SetDeviceUUID(m_strDevId);
                cModReq.SetSession(g_pcRegTasTask->GetSession());
                dwRet = PostMsgReq(cModReq, g_cCmuConf.m_t3asUri);
                if (dwRet != PROCMSG_OK)
                {
                    TASKLOG(CMU_MOD_3AS, ERROR_LEV, "����SIP��Ϣʧ��!\n");
                    return PROCMSG_DEL;
                }
			}

            if (!ptPuiRegTask->GetIsSupportRedis())     // ���ڲ�֧��redis��pui��˵���Ȼ����������ϱ���PuConfig��ע��ɹ���һ��д��redis
            {
                // pui�ϱ��������У����豸���̵���Ҫʹ��3as������
                TPuBaseConfig &tPuBaseConfig = cReq.GetBaseConfig();
                FilterBaseConfig(tPuBaseConfig);
                m_tPuConfig.SetBaseConfig(tPuBaseConfig);
            }

            //ע���豸
			m_pcPuData->SetPuRegTask(this);
			
            //��ӵ�PUI�У�PUI����ʱ��Ҫʹ��
            ptPuiRegTask->AddTask(this);           

            //����pu����֪ͨ״̬
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

	case PU_ONLINE_NTF_REQ:					//֧��redis��pui���豸������Ϣд��redis�󣬷��͸�������֪ͨcmu�������豸������
		{
            CPuiRegTask *ptPuiRegTask = GetPuiRegTask();
            CMS_ASSERT(NULL != ptPuiRegTask && "pcPuiRegTask = NULL");
            if (ptPuiRegTask->GetIsSupportRedis())
            {
                COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;

                CPuOnlineNtfReq cReq;
                pcOspSipMsg->GetMsgBody(cReq);

                // ����ģ����pui������������ģ�����в���ͨ������
                if (ptPuiRegTask->GetDevType() == DEV_TYPE_PUI
					|| ptPuiRegTask->GetDevType() == DEV_TYPE_PUI_GB
                    || DEVICE_TYPE_ENCODER != m_pcPuData->GetDevType()
                    || !IsAllChnNotOnline())
                {
                    ProcOnline();
                }

                // ����ǰ���ϱ�����ƵԴ������Ϣ���Ա����ͬ��
                m_cVidSrcNameBak = m_pcPuData->GetPuVidSrcAliasMap();

                //�������̬
                NextState(Service);

                // ���Ӧ���Ա�֤cmu�ȷ���������֪ͨ
                CPuOnlineNtfRsp cRsp;
                cRsp.SetHeadFromReq(cReq);
                PostMsgRsp(pcOspSipMsg->GetSipTransID(), cRsp);

                //ע��ɹ�
                TASKLOG(CMU_MOD_PU, CRITICAL_LEV, "�յ�pu[%s]������֪ͨ���豸ע��cmu�ɹ�!\n", m_strDevId.c_str());

                // ���ŷ��Ϳ���GPS���󣬱���pui��Ϊ�豸û���߶������������
                m_pcPuData->EnableGps();

                dwRet = PROCMSG_OK;
            }
            else
            {
                //ע��ʧ��
                TASKLOG(CMU_MOD_PU, WARNING_LEV, "�յ�pu[%s]������֪ͨ������pui[%u]��֧��redis�����Ը�֪ͨ!\n", 
                    m_strDevId.c_str(), ptPuiRegTask->GetTaskNO());

                dwRet = PROCMSG_FAIL;
            }
		}
		break;

    case PU_DEV_CONFIG_NTF_REQ:				//��֧��redis��pui����Ҫ�����ϱ��豸��������Ϣ
        {
            CPuiRegTask *ptPuiRegTask = GetPuiRegTask();
            CMS_ASSERT(NULL != ptPuiRegTask && "pcPuiRegTask = NULL");
            if (!ptPuiRegTask->GetIsSupportRedis())
            {
                COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;

                CPuDevConfigNtyReq cReq;
                pcOspSipMsg->GetMsgBody(cReq);
                TPuDevConfig& tPuDevConfig = cReq.GetConfig();

                // pui�ϱ��������У���һЩ��Ҫ��3as�����ݽ��и���
                FilterPuDevConfig(tPuDevConfig);

                TASKLOG(CMU_MOD_PU, CRITICAL_LEV, "�豸[%s]����֪ͨ\n", 
                    cReq.GetDevURI().c_str());

                int dwErrorCode = CMS_SUCCESS;

                //���������ռ���
                m_tPuConfig.MergeDevConfig(tPuDevConfig);
                if (m_tPuConfig.IsCompleted())
                {
                    //��¼�ɹ�
                    TASKLOG(CMU_MOD_PU, CRITICAL_LEV, "pu[%s]�������û�ȡ��ɣ���¼cmu�ɹ�!\n",
                        m_strDevId.c_str());

                    // ��������PuConfigд��redis
                    m_pcPuData->SetPuConfig(m_tPuConfig);

                    // ����ģ����pui
                    if (ptPuiRegTask->GetDevType() == DEV_TYPE_PUI
						|| ptPuiRegTask->GetDevType() == DEV_TYPE_PUI_GB
						|| !IsAllChnNotOnline())
                    {
                        ProcOnline();
                    }

                    //�������̬
                    NextState(Service);

                    //ע��ɹ�
                    TASKLOG(CMU_MOD_PU, CRITICAL_LEV, "�յ�pu[%s]������֪ͨ���豸ע��cmu�ɹ�!\n", m_strDevId.c_str());

                    // ���ŷ��Ϳ���GPS���󣬱���pui��Ϊ�豸û���߶������������
                    m_pcPuData->EnableGps();
                }
                else
                {
                    //�ȴ���һ��֪ͨ
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
                TASKLOG(CMU_MOD_PU, WARNING_LEV, "�յ�pu[%s]�������ϱ�֪ͨ������pui[%u]֧��redis�����Ը�֪ͨ!\n", 
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

			TASKLOG(CMU_MOD_PU, EVENT_LEV, "�յ�3as���ص���ƵԴ����ͬ��Ӧ��!\n");

			if (CMS_SUCCESS != cRsp.GetErrorCode())
			{
				TASKLOG(CMU_MOD_PU, ERROR_LEV, "ͬ��ǰ����ƵԴ������Ϣ��3asʧ��!\n");
			}

			// ֪ͨ���ع���Task�������Ѿ��������
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
        TASKLOG(CMU_MOD_PU, WARNING_LEV, "pu[%s]����֪ͨ��ʱ���ﵽ[%u]s, ע��ʧ��\n", 
            m_strDevId.c_str(), (u32)GetCurStateHoldTime());

		// ��ȡ�豸���ó�ʱ����ע���߷���ǿ������֪ͨ���������ֱ���˳���������Ӧ��
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
	//{{{{{{{{{{{{{{{ add by Royan 20140509 ���ܸ澯����
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

					TASKLOG(CMU_MOD_PU, ERROR_LEV, "DOMAIN_ALARM_REQ δ�ҵ�PUID[%s]��Ӧ��URI\n",
						strDevUri.c_str());

					break;
				}

				tAlarm.SetDevUri(g_cPuIdUriMapList.find(strDevUri)->second);
			}

			TASKLOG(CMU_MOD_PU, CRITICAL_LEV, "�豸[%s][%s]�澯֪ͨ:[inputId-%d] [flag-%s] [data-%s]\n", 
				tAlarm.GetDevUri().c_str(), tAlarm.GetAlarmType().c_str(),
				tAlarm.GetInputId(), tAlarm.GetAlarmFlag().c_str(), 
				tAlarm.GetAlarmData().c_str());

			if (m_pcPuData != NULL)
			{
				//���¸澯����
				UpdateAlarmAlias(tAlarm);
				//���¸澯�ռ���
				if (m_pcPuData->UpdateAlarmInput(tAlarm))
				{
					//������ǰ�澯��todo by xiongxh ����澯���ͱȽ����⣬puiĿǰû����redis����������cmu������
					m_pcPuData->PublishRedisAlarm(tAlarm);

					//��uasд�澯��־
					((CCmuCoreInst*)GetInstance())->GetUasLogTask()->AddAlarmLog(GetAlarmLogInfo(tAlarm));
				}
				else
				{
					dwErrorCode = ERR_CMU_ALARM_ILLEGAL;
				}           
			}
			else
			{
				TASKLOG(CMU_MOD_PU, ERROR_LEV, "pu[%s]δ����\n",
					m_strDevId.c_str());
				dwErrorCode = ERR_CMU_PU_NOT_IN_TOP;
			}

			CDomainAlarmRsp cRsp;
			cRsp.SetHeadFromReq(cReq);
			cRsp.SetErrorCode(dwErrorCode);
			PostMsgRsp(pcOspSipMsg->GetSipTransID(), cRsp);
		}
		break;
	//}}}}}}}}}}}}}}} add by Royan 20140509 ���ܸ澯����

    case PU_DEV_CONFIG_NTF_REQ:
        {
            CPuiRegTask *ptPuiRegTask = GetPuiRegTask();
            CMS_ASSERT(NULL != ptPuiRegTask && "pcPuiRegTask = NULL");
            if (!ptPuiRegTask->GetIsSupportRedis())
            {
                COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;

                CPuDevConfigNtyReq cReq;
                pcOspSipMsg->GetMsgBody(cReq);
                //modify by Royan 20140303 ����fanxg���飬�ĳ���ʱ���󣬶���ֱ���޸�Э������
                //const TPuDevConfig& tPuDevConfig = cReq.GetConfig();
                TPuDevConfig tPuDevConfig = cReq.GetConfig();

                // pui�ϱ��������У���һЩ��Ҫ��3as�����ݽ��и���
                FilterPuDevConfig(tPuDevConfig);

                TASKLOG(CMU_MOD_PU, CRITICAL_LEV, "�豸[%s]����֪ͨ\n", 
                    cReq.GetDevURI().c_str());

                int dwErrorCode = CMS_SUCCESS;

                //���������ռ���
                m_tPuConfig.MergeDevConfig(tPuDevConfig);

                //�����º��PuConfigд��redis
                m_pcPuData->SetPuConfig(m_tPuConfig);

                //���ڷ�pui������豸��Ҫ�����豸����״̬
                if (ptPuiRegTask->GetDevType() != DEV_TYPE_PUI
					&& ptPuiRegTask->GetDevType() != DEV_TYPE_PUI_GB)
                {
                    if (m_pcPuData->GetPuIsOnline())
                    {
						m_pcPuData->PublishRedisConfig();
                        if(IsAllChnNotOnline())         // ������תΪ����״̬
                        {
                            ProcOffline();
                        }
                    }
                    else
                    {
                        if(!IsAllChnNotOnline())        // ������תΪ����״̬
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
                TASKLOG(CMU_MOD_PU, WARNING_LEV, "�յ�pu[%s]�������ϱ�֪ͨ������pui[%u]֧��redis�����Ը�֪ͨ!\n", 
                    m_strDevId.c_str(), ptPuiRegTask->GetTaskNO());

                dwRet = PROCMSG_FAIL;
            }
        }
        break;

    case PU_ALARM_INPUT_NTF_REQ:
        {
            CPuiRegTask *ptPuiRegTask = GetPuiRegTask();
            CMS_ASSERT(NULL != ptPuiRegTask && "pcPuiRegTask = NULL");
            
            // ע�⣬����û���ж�pui�Ƿ�֧��redis��һ������£����pui֧��redis����澯��ϢӦ����ͨ��redis�������ģ�
            // ������һ���ֱȽ�����ĸ澯������澯�����ȣ�����Ȼ��Ҫͨ��sip��������������Ϊ��һ��澯�е�DevUri����
            // ��Ч��SipUri����ʵ��1.0��KdmNO�������ͨ��sip���������������ģ��Ҳ��Ҫ��KdmNOת��ΪSipUri��Ϊ�˼���
            // ����澯������ģ���Ӱ�죬�ʶ�ֱ�����������cmu������cmu������澯�е�KdmNOת��ΪSipUri�󷢲�
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
            if (pcOspSipMsg == NULL)
            {
                CMS_ASSERT(pcOspSipMsg && "[pcOspSipMsgָ��Ϊ��]");

                TASKLOG(CMU_MOD_PU, ERROR_LEV, "ospsip ��ϢΪ��\n");

                dwRet = PROCMSG_DEL;
                return dwRet;
            }

            CPuAlarmInputNtyReq cReq;
            pcOspSipMsg->GetMsgBody(cReq);
            TAlarmInput &tAlarmInput = cReq.GetAlarm();

            u32 dwErrorCode = CMS_SUCCESS;

            // �в��ָ澯������Ҫ����һ��
            dwErrorCode = UpdateAlarm(tAlarmInput);
            if (CMS_SUCCESS == dwErrorCode)
            {
                //���¸澯�ռ���
                if (m_pcPuData->UpdateAlarmInput(tAlarmInput))
                {
                    //������ǰ�澯
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

	//{{{{{{{{{{{{{{{ add by Royan 20140527 ǰ��¼��״̬֪ͨ
	case PU_RECORED_STATUS_NTF_REQ:
		{
            CPuiRegTask *ptPuiRegTask = GetPuiRegTask();
            CMS_ASSERT(NULL != ptPuiRegTask && "pcPuiRegTask = NULL");
            if (!ptPuiRegTask->GetIsSupportRedis())
            {
                COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;

                CPuRecordStatusNtfReq cReq;
                pcOspSipMsg->GetMsgBody(cReq);

                TASKLOG(CMU_MOD_PU, CRITICAL_LEV, "�豸[%s:%d]ǰ��¼��״̬[%d]֪ͨ\n", 
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
                TASKLOG(CMU_MOD_PU, WARNING_LEV, "�յ�pu[%s]�ķ���״̬�ϱ�֪ͨ������pui[%u]֧��redis�����Ը�֪ͨ!\n", 
                    m_strDevId.c_str(), ptPuiRegTask->GetTaskNO());

                dwRet = PROCMSG_FAIL;
            }
		}
		break;
	//}}}}}}}}}}}}}}} add by Royan 20140527 ǰ��¼��״̬֪ͨ

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

                TASKLOG(CMU_MOD_PU, CRITICAL_LEV, "�豸[%s]GPS����֪ͨ\n", 
                    m_strDevId.c_str());

                int dwErrorCode = CMS_SUCCESS;

                m_pcPuData->SetPuGpsData(tGpsData);

                //����GPS����
                m_pcPuData->PublishRedisGpsData(tGpsData);

                CPuGpsDataNtyRsp cRsp;
                cRsp.SetHeadFromReq(cReq);
                cRsp.SetErrorCode(dwErrorCode);
                PostMsgRsp(pcOspSipMsg->GetSipTransID(), cRsp);

                dwRet = PROCMSG_OK;
            }
            else
            {
                TASKLOG(CMU_MOD_PU, WARNING_LEV, "�յ�pu[%s]��GPS�ϱ�֪ͨ������pui[%u]֧��redis�����Ը�֪ͨ!\n", 
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

                TASKLOG(CMU_MOD_PU, CRITICAL_LEV, "�豸[%s]-[%s]����֪ͨ\n", 
                    m_strDevId.c_str(), tTransData.GetType().c_str());

                int dwErrorCode = CMS_SUCCESS;

                m_pcPuData->SetPuTransparentData(tTransData);

                //��������
                m_pcPuData->PublishRedisTransData(tTransData);

                CPuTransparentDataNtfRsp cRsp;
                cRsp.SetHeadFromReq(cReq);
                cRsp.SetErrorCode(dwErrorCode);
                PostMsgRsp(pcOspSipMsg->GetSipTransID(), cRsp);

                dwRet = PROCMSG_OK;
            }
            else
            {
                TASKLOG(CMU_MOD_PU, WARNING_LEV, "�յ�pu[%s]��͸�������ϱ�֪ͨ������pui[%u]֧��redis�����Ը�֪ͨ!\n", 
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

			TASKLOG(CMU_MOD_PU, EVENT_LEV, "�յ�3as���ص���ƵԴ����ͬ��Ӧ��!\n");

			if (CMS_SUCCESS != cRsp.GetErrorCode())
			{
				TASKLOG(CMU_MOD_PU, ERROR_LEV, "ͬ��ǰ����ƵԴ������Ϣ��3asʧ��!\n");
			}

			// ֪ͨ���ع���Task�������Ѿ��������
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
        TASKLOG(CMU_MOD_PU, ERROR_LEV, "ospsip ��ϢΪ��\n");
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
        //�Ѿ�ע���pu����Ҫǿ�ƶϿ�
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
		//�����豸����
		tPuBaseConfig.SetDevName(m_pcPuData->GetDevName());

		//�����豸����
		tPuBaseConfig.SetManufacturer(m_pcPuData->GetDevManufacturer());

		//���ù����ǩ
		tPuBaseConfig.SetIsGBDevice(m_pcPuData->GetDevGBFlag());
	}
}

void CPuRegTask::FilterPuDevConfig(TPuDevConfig &tPuDevConfig)
{
	if (NULL != m_pcPuData)		// �����������ݣ�ֱ��ȡ3as�����ݣ�������pui�ı��
	{
		CPuBaseCfgList &cBaseCfg = tPuDevConfig.GetBaseCfgList();
		if (!cBaseCfg.empty())
		{
			FilterBaseConfig(cBaseCfg.at(0));
		}

		CPuVidSrcCfgList &cVidSrcCfg = tPuDevConfig.GetVidSrcCfgList();
		if (!cVidSrcCfg.empty())
		{
			// ���ݺ�ǰ���ϱ�����ƵԴ������Ϣ
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
	//�������Ҫͬ����tas��ֱ��return
	if (!g_cCmuConf.m_bSyncVidSrcName2Plat)
	{
		return;
	}

	CDeviceCapIndexNameSetReq cReq;
	cReq.SetSession(g_pcRegTasTask->GetSession());
	cReq.GetDeviceCapName().deviceId = m_strDevId;
	cReq.GetDeviceCapName().capId = DEVICE_CAP_VIDEOSOURCE;

	TDeviceCapIndexName tDeviceCapIndexName;

	//����3as�е���ƵԴ����ͬ��
	if (!m_cVidSrcNameBak.empty())
	{
        map<s32, string> cVidSrcAlias = m_pcPuData->GetVidSrcAliasMap();

        //������ƵԴ����Ϊ�ջ����ϱ���֧��ͬ����g100ǰ����˵������Ҫͬ��
        map<int,int> cExtCapSet = m_pcPuData->GetExtCapSet();
        map<int,int>::iterator iter = cExtCapSet.find(VSIP_ENTITY_G100_SUPPORT_VIDEOINPUTNAME_SYNC);
        if (iter == cExtCapSet.end())
        {
            for (map<s32, string>::const_iterator it = m_cVidSrcNameBak.begin(); m_cVidSrcNameBak.end() != it; ++it)
            {
                if (!it->second.empty())
                {
                    map<s32, string>::iterator it4Mod = cVidSrcAlias.find(it->first);
                    if (cVidSrcAlias.end() != it4Mod && it4Mod->second == it->second)       // ����������ͬ����������ͬ��
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
// 		TASKLOG(CMU_MOD_PU, ERROR_LEV, "�豸Ƶ�������ˣ�����û�м�¼�� DevId[%s]\n", m_strDevId.c_str());
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

	// UAS��־
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

        //��������ܸ澯��������ѯ����Ѷ�Ӧ�ı���ͨ����ת��Ϊ��ƵԴͨ���ţ�Ӧ�Ը澯�ϱ�����ͨ���ŵ������
        int nVidSrcIdx = tAlarmInput.GetInputId();
        if ((ALARM_TYPE_INTELLIGENT == tAlarmInput.GetAlarmType() || ALARM_TYPE_VIDEOPOLL == tAlarmInput.GetAlarmType()))
        {
            CVidSrcSet cVidSrcSet = m_pcPuData->GetChnVidSrc(nVidSrcIdx);
            if (!cVidSrcSet.empty())
            {
                nVidSrcIdx = *cVidSrcSet.begin();
                TASKLOG(CMU_MOD_PU, EVENT_LEV, "�澯ͨ����[%d]ת��Ϊ��ƵԴ��[%d]\n",
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
                TASKLOG(CMU_MOD_PU, ERROR_LEV, "PU_ALARM_INPUT_NTF_REQ δ�ҵ�PUID[%s]��Ӧ��URI\n",
                    tAlarmInput.GetDevUri().c_str());

                dwRet = ERR_CMU_PU_NOT_IN_TOP;
                break;
            }

            tAlarmInput.SetDevUri(itor->second);
        }

        TASKLOG(CMU_MOD_PU, CRITICAL_LEV, "�豸[%s][%s]�澯֪ͨ:[inputId-%d] [flag-%s] [data-%s]\n", 
            tAlarmInput.GetDevUri().c_str(), tAlarmInput.GetAlarmType().c_str(),
            tAlarmInput.GetInputId(), tAlarmInput.GetAlarmFlag().c_str(), 
            tAlarmInput.GetAlarmData().c_str());

        //���¸澯����
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

    //tLogInfo.desc = tAlarmInput.GetAlarmData();//��־������Ӧ���Ǹ澯���ݣ���Ӧ���ǹ̶���ʽ��������Ϣ��
    
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
            TASKLOG(CMU_MOD_PU, CRITICAL_LEV, "pu[%s]-usedPuiIp[%s]ʹ��pui[%s]��NetSegNO[%u]����\n", 
                m_strDevId.c_str(), GetPuiIp().c_str(), 
                pcPuiTask->GetDevUri().c_str(), nPuNetSegNO);

            return nPuNetSegNO;
        }
    }

    TASKLOG(CMU_MOD_PU, ERROR_LEV, "pu[%s]-usedPuiIp[%s]û���ҵ�pui[%s]��Ӧ��NetSegNO\n", 
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
	
	// ���������ƵԴ��Ϣͬ����3as
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
 			        //ȡ�������������仯�ж�
                    //bChg = true;
                }
            }
        }
	}
	else if (DEVICE_TYPE_DECODER == nDevType)
	{
        //�������ж� ��Ƶ����ͨ����/HDMI�� �Ƿ��б仯
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
    1.���ڸ澯��Ҫ��3as��ȡ���ڱ��������ݹ������ɸ澯����������û�û�����ù����������ø澯ͨ������
    2.�ƶ�������ƵԴ��ʧ��Ҫ��3as��ȡ��ƵԴ���������ݹ������ɸ澯����������û�û�����ù�������������ƵԴͨ������
    3.���ɱ���ʱ��Ҫ֧�ֶ������
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
	//��ӻ������ܸ澯������
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

    // ״̬�����������߿϶���Ҫ������redis��sip�����ߵģ�
    m_pcPuData->PublishOnOff();

    // ���pui��֧��redis����cmu����Ҫ�������pui�����������͵�֪ͨ
    if (!ptPuiRegTask->GetIsSupportRedis())
    {
        m_pcPuData->PublishRedisConfig();

        // �豸���ߺ������һ�εķ���״̬PuServiceStatus�͸澯��ϢPuAlarmStatus
        // ����GpsData��TransData�������ڲ�д��redis��ֻ�Ƿ������������账��
        m_pcPuData->ClearPuServiceStatus();
        m_pcPuData->ClearAlarmStatus();
    }    

	////������Ҫ����GPS����
	//m_pcPuData->EnableGps();

    //��uasд�豸��־
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
            //��uasд�豸�澯�ָ���־�����ڸ澯�ĲŻָ�
            TPuAlarmStatus tPuAlarmStatus = m_pcPuData->GetPuAlarmStatus();
            const CAlarmInputArray& tAlarmInputArray = tPuAlarmStatus.GetAlarmArray();
            for (size_t i = 0; i < tAlarmInputArray.size(); i++)
            {
                TAlarmInput tAlarmInput = tAlarmInputArray.at(i);

                if (ALARM_FLAG_OCCUR == tAlarmInput.GetAlarmFlag())
                {
                    //���ûָ�״̬
                    tAlarmInput.SetAlarmFlag(ALARM_FLAG_RESTORE);

                    //��uasд�澯��־
                    ((CCmuCoreInst*)GetInstance())->GetUasLogTask()->AddAlarmLog(GetAlarmLogInfo(tAlarmInput));
                }
            }

            bool bOnline = m_pcPuData->GetPuIsOnline();

            // ��������״̬����Ϣ
            m_pcPuData->SetPuIsOnline(false);

            m_pcPuData->CleanupDevChnState();

            this->SendPuOfflineTooBrief();

            //��uasд�豸������־
            ((CCmuCoreInst*)GetInstance())->GetUasLogTask()->AddPuLog(GetDevLogInfo(false));

            // ����״̬֪ͨ�������ߵ����߲ŷ���֪ͨ�����֮ǰ�������ߣ�����Ҳ�����ߣ���û�б�Ҫ���ͣ�
            if (bOnline)
            {
                m_pcPuData->PublishOnOff();
            }

            //����õ�¼�Ự����������
            DestroyChildTasks();
        }
        break;
    default:
        {
            TASKLOG(CMU_MOD_PUI, ERROR_LEV, "ProcOfflineδ֪����״̬[%d]\n", GetState());
        }
        break;
    }
}

bool CPuRegTask::IsUTF8(IN unsigned char *pBuf, IN int strLen)
{
    const unsigned char byMask6 = 0xFC;//11111100//6���ֽ�
    const unsigned char byMask5 = 0xF8;//11111000//5���ֽ�
    const unsigned char byMask4 = 0xF0;//11110000//4���ֽ�
    const unsigned char byMask3 = 0xE0;//11100000//3���ֽ�
    const unsigned char byMask2 = 0xC0;//11000000//2���ֽ�
    const unsigned char byMask1 = 0x80;//10000000//С�ڴ�ֵ��1���ֽ�
    for (int i = 0; i < strLen; i++)
    {
        unsigned char uch = pBuf[i];//���ֽ�
        if (uch == 0 && i < strLen - 1)
        {
            //���ǽ�β����'\0'���ǿ϶�����utf8��
            return false;
        }
        if (uch < byMask1)
        {
            continue;
        }
        if ((uch & 0xC0) == 0x80)//���ֽ��� 0x80 ��0xc0֮�䣬���ǷǷ�
        {
            return false;
        }
        for (int n = 2; n <= 6; n++)
        {
            //��������ʾ�ĺ��滹�м����ֽ���
            int nNeedChar = 7 - n;
            //��������
            unsigned char byMask = ((0xff >> n) << n);
            if (uch >= byMask)
            {
                //���ʣ���ֽڵ������Ƿ����utf8����
                if (strLen - i - 1 < nNeedChar)
                {
                    return false;
                }
                //���ʣ���ֽ�������ֵ�Ƿ����utf8����
                for (int k = 0; k < nNeedChar; k++)
                {
                    //ÿ���ֽڶ�������� 10xxxxxx ��ʽ
                    if ((pBuf[i + k + 1] & 0xC0) != 0x80)
                    {
                        return false;
                    }
                }
                //��������
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

