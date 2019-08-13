
#include "cms/cms_errorcode.h"
#include "cms/cmu/cmu_event.h"
#include "cms/cmu/cmu_struct.h"
#include "cms/cmu/cmu_proto.h"
#include "cms/tas/tas_proto.h"

#include "puregtask.h"
#include "pubatchregtask.h"
#include "puiregtask.h"
#include "cmuconst.h"
#include "cmuconf.h"
#include "cmudata.h"
#include "cmucoreapp.h"
#include "regtastask.h"
#include "cuiregtask.h"
#include "puinotifytask.h"

void CLoginSessionTask::InitStateMachine()
{ 
    CStateProc cServiceProc;
    cServiceProc.ProcMsg = (CTask::PFProcMsg)&CLoginSessionTask::OnService;
    cServiceProc.ProcErrMsg = (CTask::PFProcMsg)&CLoginSessionTask::OnService;
    cServiceProc.TimerPoll = (CTask::PFTimerPoll)&CLoginSessionTask::TimerPollSkip;
    AddRuleProc(Service, cServiceProc);

    NextState(Service); 
}

u32 CLoginSessionTask::OnService(CMessage *const pcMsg)
{
    return PROCMSG_OK;
}


CPuBatchRegTask::CPuBatchRegTask(CInstExt *pcInst, CPuiRegTask *pcTask) : CPuRegBaseTask(pcInst), m_pcPuiRegTask(pcTask)
{
    CMS_ASSERT(NULL != m_pcPuiRegTask && "m_pcPuiRegTask = NULL!!!");
}

CPuBatchRegTask::~CPuBatchRegTask()
{
    ReleaseResource();
}

void CPuBatchRegTask::InitStateMachine()
{
    CStateProc cServiceProc;
    cServiceProc.ProcMsg = (CTask::PFProcMsg)&CPuBatchRegTask::OnService;
    cServiceProc.ProcErrMsg = (CTask::PFProcMsg)&CPuBatchRegTask::OnService;
    cServiceProc.TimerPoll = (CTask::PFTimerPoll)&CPuBatchRegTask::OnServiceTimer;
    AddRuleProc(Service, cServiceProc);

    SetDevType(DEV_TYPE_PU);

    NextState(Service);
}

void CPuBatchRegTask::PrintData() const
{
	CTask::PrintData();

	if (!m_cPuSet.empty())
    {
        if (g_bDebugOn)
        {
            OspPrintf(TRUE, FALSE, "---------------------------------------------------------------\n");

            u32 dwDevCnt = 0;
            for (map<string, TRegExtInfo>::const_iterator it = m_cPuSet.begin(); m_cPuSet.end() != it; ++it)
            {
                set<u32> cTasks;
                const string &strDevId    = it->first;
                CLoginSessionTask *pcTask = it->second.pcLoginTask;
                u32 dwLoginSessionTaskNO  = INVALID_TASKNO;
                if (NULL != pcTask)
                {
                    dwLoginSessionTaskNO = pcTask->GetTaskNO();
                }

                string strPuiIp = it->second.strPuiIp;

                CPuData *pcPuData = GetPuData(strDevId);
                CMS_ASSERT(NULL != pcPuData && "pcPuData = NULL!!!");

                OspPrintf(TRUE, FALSE, "\t��%u��Dev[%s], State[%d], Name[%s], Ip[%s], GB[%d], PuiIp[%s], LoginSessionTask[%u]\n", 
                    ++dwDevCnt, strDevId.c_str(), pcPuData->GetDevState(), pcPuData->GetDevName().c_str(), 
                    pcPuData->GetPuConnectAddr().c_str(), pcPuData->GetDevGBFlag(), strPuiIp.c_str(), dwLoginSessionTaskNO);
            }
        }
    }

    OspPrintf(TRUE, FALSE, "\tTotalNum[%d]\n", m_cPuSet.size());
}

void CPuBatchRegTask::ReleaseResource()
{
    switch(GetState())
    {
    case Service:
        {
            DestroyChildTasks();

            set<string> cIdSet;
            for (map<string, TRegExtInfo>::iterator it = m_cPuSet.begin(); m_cPuSet.end() != it;)
            {
                CPuData *pcPuData = CCmuData::GetInstance()->GetPuData(it->first);
                if (NULL != pcPuData)
                {
                    /*
                     * CPuBatchRegTask��������������ʱֻ��PuData�е�PuRegTask����ΪNULL��û��������������豸״̬����Ϊoffline
                     * ��Ϊ������豸̫�࣬Ҫ�������豸ȫ������Ϊoffline��һ��ʱ�䣬������������������״̬����Ϊoffline������һ��
                     * ʱ��������Ļ���cmu���֪�豸�����ߣ�����ʱcu���豸�����ߵģ���������û�����⣬���Ի��ǵȵ�redis�е�״̬����
                     * ���֮���ٽ��豸״̬����Ϊoffline���Դﵽͳһ
                     */
                    pcPuData->SetPuRegTask(NULL);

                    // ֻ���Ѿ����ߵ��豸�����б�Ҫ��������֪ͨ
                    if (pcPuData->GetPuIsOnline())
                    {
                        cIdSet.insert(it->first);
                    }
                }

                // ɾ���͸��豸��ص�Task
                if (NULL != it->second.pcLoginTask)
                {
                    it->second.pcLoginTask->DestroyChildTasks();

                    delete it->second.pcLoginTask;
                    it->second.pcLoginTask = NULL;
                }

                m_cPuSet.erase(it++);
            }

            // CPuBatchRegTask������ʱ����Ҫ��������ע���豸����Ϊ���ߣ�ͬʱ�����Ӧ�Ľ���
            // ����CPuBatchRegTaskע���豸̫�ֱ࣬��ѭ������ᵼ���������ʶ�״̬�ȴ�������Ϊ�첽����
            if (!cIdSet.empty())
            {
                CPuiNotifyTask *pcTask = g_cPuiNotifyList.GetPuiNotifyTask(m_pcPuiRegTask->GetDevSipUri());
                CMS_ASSERT(NULL != pcTask && "pcTask = NULL!!!");
                pcTask->Add(cIdSet);
            }

            // ������������־
            TDeviceLogInfo tLogInfo = m_pcPuiRegTask->GetDevLogInfo(false);

            // gbuʹ��cmssdk�ķ�ʽ�ǳ��ر�puireg��cmssdk��ɵģ����Ǻ�����������ʹ�õ�gbu��uri������Ϊ�˱����Ų����⣬ʹ��gbu��uri
            tLogInfo.devUri = GetPuiUri().GetURIString();
            ((CCmuCoreInst*)GetInstance())->GetUasLogTask()->AddDeviceLog(tLogInfo);
        }
        break;
    default:
        {
            TASKLOG(CMU_MOD_BATCH, ERROR_LEV, "δ֪����״̬[%d]\n", GetState());
        }
        break;
    } 
}

u32 CPuBatchRegTask::OnService(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_FAIL;
    switch(pcMsg->event)
    {
	case BATCH_PU_REG_REQ:
		{
			COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;

            CBatchPuRegReq cReq;
            pcOspSipMsg->GetMsgBody(cReq);

            SetPuiUri(pcOspSipMsg->GetSipFromUri());

            CBatchPuRegRsp cRsp;
            vector<int> &cResult = cRsp.GetResult();

            s32 nDataSize = cReq.GetData().size();
            cResult.resize(nDataSize, CMS_SUCCESS);

            // ˵����
            // ��������ӿ������������ͽ��������������������AAAAABBBBB�������Ż�Ϊ������������һ�δ�������Ϊ��[0, 5)���ڶ���Ϊ��[5, 10)
            // ͬʱ��ǰֻ���ע��������������Ż���������ȫ���԰�������˼·������չ�����������͵��������ǰ��յ���������
            string strLastType;
            u32 dwStartIndex = 0;
            u32 dwEndIndex   = dwStartIndex;
            const vector<TGeneralData> &cData = cReq.GetData();
            for (u32 i = 0; i < cData.size(); ++i)
            {
                const TGeneralData &tData = cReq.GetData().at(i);
                if (BATCH_PU_REG_TYPE_REG == strLastType && strLastType != tData.type)      // ��ע�������л�Ϊ������������
                {
                    strLastType  = tData.type;
                    dwEndIndex   = i;
                    
                    // �Ƚ�֮ǰ�ۻ�����������������
                    BatchRegProc(cData, dwStartIndex, dwEndIndex, cResult);

                    // �ٴ����ε�����
                    cResult[i] = NonBatchProc(tData.type, tData.content);
                    
                }
                else if (BATCH_PU_REG_TYPE_REG != strLastType && BATCH_PU_REG_TYPE_REG == tData.type)   // ���������������л�Ϊע������
                {
                    strLastType  = BATCH_PU_REG_TYPE_REG;
                    dwStartIndex = i;
                    dwEndIndex   = dwStartIndex;
                }
                else
                {
                    if (BATCH_PU_REG_TYPE_REG == tData.type)                    // ����ע������
                    {
                        dwEndIndex = i;
                    }
                    else                                                        // ������ֱ�Ӵ���Ŀǰ���Ż�ֻ���ע�ᣩ
                    {
                        cResult[i] = NonBatchProc(tData.type, tData.content);
                    }
                }
            }

            // ���һ��
            if (BATCH_PU_REG_TYPE_REG == strLastType)
            {
                // �Ƚ�֮ǰ�ۻ�����������������(ע�⣬BatchRegProc�Ĵ���������[dwStartIndex, dwEndIndex)����������Ҫʹ��dwEndIndex + 1)
                BatchRegProc(cData, dwStartIndex, dwEndIndex + 1, cResult);
            }

            PostMsgRsp(pcOspSipMsg->GetSipTransID(), cRsp);

            // ����Ҫע�⣬��ע��Ӧ��֮�󣬶���PUI������豸������Ҫ���Ϳ���GPS�����󣬵�����������ע������Ŀǰֻ��gbuʹ�ã���������ʱû���ô���
            for (vector<TGeneralData>::const_iterator it = cData.begin(); cData.end() != it; ++it)
            {
                if (BATCH_PU_REG_TYPE_REG == it->type)
                {
                    TSipURI tDevUri(it->content);
                    CPuData *pcPuData = GetPuDataByUri(tDevUri);
                    if (NULL != pcPuData)
                    {
                        pcPuData->EnableGps();
                    }
                }
            }

            dwRet = PROCMSG_OK;
		}
		break;
    case CMU_DISCONNECT_PU_RSP:
        {
            COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;

            CCmuDisconnectPuRsp cRsp;
            pcOspSipMsg->GetMsgBody(cRsp);

            TASKLOG(CMU_MOD_BATCH, EVENT_LEV, "Receive CCmuDisconnectPuRsp, TaskNO[%u]\n", GetTaskNO());

            dwRet = PROCMSG_OK;
        }
        break;
    default:
        {
            TASKLOG(CMU_MOD_BATCH, WARNING_LEV, "Receive unknown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }

    return dwRet;
}

u32 CPuBatchRegTask::OnServiceTimer()
{
    return TIMERPOLL_DONE;
}

CLoginSessionTask *CPuBatchRegTask::GetLoginSession(const string &strDevId)
{
    map<string, TRegExtInfo>::iterator it = m_cPuSet.find(strDevId);
    if (m_cPuSet.end() != it)
    {
        if (NULL == it->second.pcLoginTask)
        {
            it->second.pcLoginTask = new CLoginSessionTask(GetInstance());
            it->second.pcLoginTask->InitStateMachine();
            it->second.pcLoginTask->SetDevType(DEV_TYPE_PU);
			it->second.pcLoginTask->SetPuiIp(it->second.strPuiIp);
        }

        return it->second.pcLoginTask;
    }

    return NULL;
}

CPuiRegTask *CPuBatchRegTask::GetPuiRegTask()
{
    return m_pcPuiRegTask;
}

void CPuBatchRegTask::ForceOffline(const string &strDevId)
{
    map<string, TRegExtInfo>::iterator it = m_cPuSet.find(strDevId);
    if (m_cPuSet.end() != it)
    {
        //�Ѿ�ע���pu��Ҫǿ�ƶϿ�
        CCmuDisconnectPuReq cReq;
        cReq.SetSession(GetSession());
        cReq.SetDevUri(GetDevUriByDevId(strDevId).GetURIString());
        PostMsgReq(cReq, GetPuiUri());

        UnRegProc(strDevId);
    }
}

string CPuBatchRegTask::GetJoinInterfaceIp(const string &strSession)
{
    map<string, TRegExtInfo>::const_iterator it = m_cPuSet.find(strSession);
    if (m_cPuSet.end() != it)
    {
        return it->second.strPuiIp;
    }

    return string();
}

u32 CPuBatchRegTask::PreRegProc(const string &strDevId)
{
    if (g_cPuiNotifyList.IsExistPuiNotifyTask(m_pcPuiRegTask->GetDevSipUri()))
    {
        TASKLOG(CMU_MOD_BATCH, ERROR_LEV, "�ݲ�����ע�����!\n");
        return ERR_CMU_NOT_SERVICE_TEMPORARILY;
    }

    if (!m_pcPuiRegTask->GetIsSupportRedis())
    {
        TASKLOG(CMU_MOD_BATCH, ERROR_LEV, "pui[%u]��֧��redis!\n", m_pcPuiRegTask->GetTaskNO());
        return ERR_CMU_UNCOMPATIBLE_REQ;
    }

    //�Ƿ�����
    CPuData *pcPuData = CCmuData::GetInstance()->GetPuData(strDevId, true);
    if (NULL == pcPuData)
    {
        TASKLOG(CMU_MOD_BATCH, ERROR_LEV, "pu[%s]δ����\n", strDevId.c_str());
        return ERR_CMU_PU_NOT_IN_TOP;
    }

    //�Ƿ��Ѿ�ע��
    if (pcPuData->IsPuRegistered())
    {
        TASKLOG(CMU_MOD_BATCH, ERROR_LEV, "pu[%s]�Ѿ�ע��\n", strDevId.c_str());
        return ERR_CMU_PU_ALREADY_REG;
    }

    return CMS_SUCCESS;
}

u32 CPuBatchRegTask::UnRegProc(const string &strDevId)
{
    CPuData *pcPuData = CCmuData::GetInstance()->GetPuData(strDevId);
    if (NULL != pcPuData)
    {
        pcPuData->SetPuRegTask(NULL);
        
        if (pcPuData->GetPuIsOnline())
        {
            pcPuData->SetPuIsOnline(false);
            pcPuData->CleanupDevChnState();

			// ֮ǰ�����ߣ���������Ҫ��������֪ͨ
            pcPuData->PublishOnOff();

            //��uasд�豸������־
            ((CCmuCoreInst*)GetInstance())->GetUasLogTask()->AddDeviceLog(pcPuData->GetDevLog());
        }
    }

    // ɾ���͸��豸��ص�Task
    map<string, TRegExtInfo>::iterator it = m_cPuSet.find(strDevId);
    if (m_cPuSet.end() != it)
    {
        if (NULL != it->second.pcLoginTask)
        {
            it->second.pcLoginTask->DestroyChildTasks();

            delete it->second.pcLoginTask;
            it->second.pcLoginTask = NULL;
        }

        m_cPuSet.erase(it);
    }

    return CMS_SUCCESS;
}

u32 CPuBatchRegTask::OfflineProc(const string &strKey)
{
    CDomainData *pcDomainData = GetLocalDomainData();
    CMS_ASSERT(NULL != pcDomainData && "pcDomainData = NULL!!!");

    vector<string> cDevId;
    pcDomainData->GetRedisPipe().GetFastDBInstance().SMembers(strKey, cDevId);
    pcDomainData->GetRedisPipe().GetFastDBInstance().Del(strKey);

    // 1. ���ڲ�����
    vector<string> cOffline;
    cOffline.reserve(cDevId.size());
    for (vector<string>::iterator it4Dev = cDevId.begin(); cDevId.end() != it4Dev; ++it4Dev)
    {
        map<string, TRegExtInfo>::iterator it = m_cPuSet.find(*it4Dev);
        if (m_cPuSet.end() != it)                   // ��ע���ģ��ע��
        {
            CPuData *pcPuData = CCmuData::GetInstance()->GetPuData(*it4Dev);
            if (NULL != pcPuData)
            {
                pcPuData->SetPuRegTask(NULL);

                if (pcPuData->GetPuIsOnline())
                {
                    cOffline.push_back(*it4Dev);
                }
            }

            // ɾ���͸��豸��ص�Task
            if (NULL != it->second.pcLoginTask)
            {
                it->second.pcLoginTask->DestroyChildTasks();

                delete it->second.pcLoginTask;
                it->second.pcLoginTask = NULL;
            }

            m_cPuSet.erase(it++);
        }
    }

    // 2. ����״̬����
    if (!cOffline.empty())
    {
        CPuiNotifyTask *pcTask = g_cPuiNotifyList.GetPuiNotifyTask(m_pcPuiRegTask->GetDevSipUri());
        CMS_ASSERT(NULL != pcTask && "pcTask = NULL!!!");
        pcTask->Add(cOffline);
    }

    // 3. ������־ todo by xiongxh �����������ߣ���־д��д��
    //TDeviceLogInfo tLogInfo = m_pcPuiRegTask->GetDevLogInfo(false);

    // gbuʹ��cmssdk�ķ�ʽ�ǳ��ر�puireg��cmssdk��ɵģ����Ǻ�����������ʹ�õ�gbu��uri������Ϊ�˱����Ų����⣬ʹ��gbu��uri
    //tLogInfo.devUri = GetPuiUri().GetURIString();
    //((CCmuCoreInst*)GetInstance())->GetUasLogTask()->AddDeviceLog(tLogInfo);
    
    return CMS_SUCCESS;
}

void CPuBatchRegTask::BatchRegProc(const vector<TGeneralData> &cData, u32 dwStartIndex, u32 dwEndIndex, vector<int> &cResult)
{
    CMS_ASSERT(dwEndIndex <= cData.size() && "impossible!!!");

    vector<string> cDevId;
    vector<u32> cIndex;
    cDevId.reserve(dwEndIndex - dwStartIndex);
    cIndex.reserve(dwEndIndex - dwStartIndex);
    for (u32 i = dwStartIndex; i < dwEndIndex; ++i)
    {
        TSipURI tDevUri(cData.at(i).content);

        // ����һ��Ԥע�ᴦ��
        u32 dwRet = PreRegProc(tDevUri.GetUser());
        if (CMS_SUCCESS == dwRet)
        {
            cDevId.push_back(tDevUri.GetUser());
            cIndex.push_back(i);
        }
        else
        {
            cResult[i] = dwRet;
        }
    }

    CDomainData *pcDomainData = GetLocalDomainData();
    CMS_ASSERT(NULL != pcDomainData && "pcDomainData = NULL!!!");

    // ��ʼ����
    vector<TPuRegInfo> cInfo;
    vector<string> cOnline;
    cOnline.reserve(cDevId.size());
    if (pcDomainData->GetRedisPipe().BatchGetPuRegInfo(cDevId, cInfo))
    {
        bool bOnline = false;
        for (u32 i = 0; i < cDevId.size(); ++i)
        {
            cResult[cIndex[i]] = RegProc(cDevId[i], cInfo[i], bOnline);
            if (CMS_SUCCESS == cResult[cIndex[i]] && bOnline)
            {
                cOnline.push_back(cDevId[i]);
            }
        }
    }
    else
    {
        for (u32 i = 0; i < cDevId.size(); ++i)
        {
            cResult[cIndex[i]] = ERR_CMU_DATA_UNAVAILABLE_TEMPORARILY;
        }
    }

    // �����豸����
    pcDomainData->GetRedisPipe().BatchPuOnline(cOnline, true);
    for (vector<string>::const_iterator it = cOnline.begin(); cOnline.end() != it; ++it)
    {
        CPuData *pcPuData = CCmuData::GetInstance()->GetPuData(*it);
        if (NULL != pcPuData)
        {
            pcPuData->SetPuIsOnline(true, false);
            pcPuData->PublishSipOnOff(ALL_SS, INVALID_TASKNO, false);
			//pcPuData->EnableGps();
        }
    }
}

u32 CPuBatchRegTask::RegProc(const string &strDevId, const TPuRegInfo &tInfo, bool &bOnline)
{
    //�Ƿ�����
    if (!tInfo.use)
    {
        TASKLOG(CMU_MOD_BATCH, ERROR_LEV, "pu[%s]δ����\n", strDevId.c_str());
        return ERR_CMU_PU_NOT_USED;
    }

    //�Ƿ����
    if (tInfo.expire)
    {
        TASKLOG(CMU_MOD_BATCH, ERROR_LEV, "pu[%s]�Ѿ�����\n", strDevId.c_str());
        return ERR_CMU_PU_EXPIRE;
    }

    u32 dwErr = ValidLicense(strDevId, tInfo.creator, tInfo.license, tInfo.chnnum, tInfo.freeLicense);
    if (CMS_SUCCESS != dwErr)
    {
        TASKLOG(CMU_MOD_BATCH, ERROR_LEV, "pu[%s]license��֤ʧ��\n", strDevId.c_str());
        return dwErr;
    }

    CPuData *pcPuData = CCmuData::GetInstance()->GetPuData(strDevId);
    CMS_ASSERT(NULL != pcPuData && "pcPuData == NULL!!!");

    // �ߵ�����˵���豸�Ѿ���֤ͨ��������������
    pcPuData->SetPuRegTask(this);

    TRegExtInfo tExtInfo;
    tExtInfo.strPuiIp = tInfo.puiIp;
    m_cPuSet.insert(map<string, TRegExtInfo>::value_type(strDevId, tExtInfo));

    // ����ģ����pui������������ģ�����в���ͨ������
    if (m_pcPuiRegTask->GetDevType() == DEV_TYPE_PUI || m_pcPuiRegTask->GetDevType() == DEV_TYPE_PUI_GB || tInfo.chnStatusFlag)
    {
        bOnline = true;

        //��uasд�豸��־
        ((CCmuCoreInst*)GetInstance())->GetUasLogTask()->AddDeviceLog(
            pcPuData->GetDevLog(tInfo.name, tInfo.alias, tInfo.connectIp));
    }
    else
    {
        bOnline = false;
    }

    return CMS_SUCCESS;
}

u32 CPuBatchRegTask::NonBatchProc(const string &strType, const string &strId)
{
    if (BATCH_PU_REG_TYPE_UNREG == strType)              // ע���豸
    {
        TSipURI tDevUri(strId);

        return UnRegProc(tDevUri.GetUser());
    }
    else if (BATCH_PU_REG_TYPE_OFFLINE == strType)       // ������������
    {
        return OfflineProc(strId);
    }
    else
    {
        return ERR_CMU_BATCH_REG_TYPE_INVALID;
    }
}

