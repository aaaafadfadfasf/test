
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

                OspPrintf(TRUE, FALSE, "\t【%u】Dev[%s], State[%d], Name[%s], Ip[%s], GB[%d], PuiIp[%s], LoginSessionTask[%u]\n", 
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
                     * CPuBatchRegTask被析构后，这里暂时只将PuData中的PuRegTask设置为NULL，没有立即将缓存的设备状态设置为offline
                     * 因为接入的设备太多，要将所有设备全部更新为offline有一段时间，如果这里立即将缓存的状态设置为offline，在这一段
                     * 时间内浏览的话，cmu会告知设备不在线，而此时cu上设备是在线的，可能造成用户的误解，所以还是等到redis中的状态更新
                     * 完毕之后，再将设备状态设置为offline，以达到统一
                     */
                    pcPuData->SetPuRegTask(NULL);

                    // 只有已经上线的设备，才有必要发送下线通知
                    if (pcPuData->GetPuIsOnline())
                    {
                        cIdSet.insert(it->first);
                    }
                }

                // 删除和该设备相关的Task
                if (NULL != it->second.pcLoginTask)
                {
                    it->second.pcLoginTask->DestroyChildTasks();

                    delete it->second.pcLoginTask;
                    it->second.pcLoginTask = NULL;
                }

                m_cPuSet.erase(it++);
            }

            // CPuBatchRegTask被析构时，需要将所有已注册设备设置为下线，同时清理对应的交换
            // 由于CPuBatchRegTask注册设备太多，直接循环处理会导致阻塞，故而状态等处理设置为异步处理
            if (!cIdSet.empty())
            {
                CPuiNotifyTask *pcTask = g_cPuiNotifyList.GetPuiNotifyTask(m_pcPuiRegTask->GetDevSipUri());
                CMS_ASSERT(NULL != pcTask && "pcTask = NULL!!!");
                pcTask->Add(cIdSet);
            }

            // 国标域下线日志
            TDeviceLogInfo tLogInfo = m_pcPuiRegTask->GetDevLogInfo(false);

            // gbu使用cmssdk的方式非常特别，puireg是cmssdk完成的，但是后续操作又是使用的gbu的uri，这里为了便于排查问题，使用gbu的uri
            tLogInfo.devUri = GetPuiUri().GetURIString();
            ((CCmuCoreInst*)GetInstance())->GetUasLogTask()->AddDeviceLog(tLogInfo);
        }
        break;
    default:
        {
            TASKLOG(CMU_MOD_BATCH, ERROR_LEV, "未知事务状态[%d]\n", GetState());
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

            // 说明：
            // 针对批量接口中连续的类型进行批量化处理，比如针对AAAAABBBBB，可以优化为两次来处理，第一次处理区间为：[0, 5)，第二次为：[5, 10)
            // 同时当前只针对注册请求进行这种优化（后续完全可以按照这种思路进行扩展），其他类型的请求则还是按照单个来处理
            string strLastType;
            u32 dwStartIndex = 0;
            u32 dwEndIndex   = dwStartIndex;
            const vector<TGeneralData> &cData = cReq.GetData();
            for (u32 i = 0; i < cData.size(); ++i)
            {
                const TGeneralData &tData = cReq.GetData().at(i);
                if (BATCH_PU_REG_TYPE_REG == strLastType && strLastType != tData.type)      // 从注册请求切换为其他类型请求
                {
                    strLastType  = tData.type;
                    dwEndIndex   = i;
                    
                    // 先将之前累积的批量请求给处理掉
                    BatchRegProc(cData, dwStartIndex, dwEndIndex, cResult);

                    // 再处理本次的请求
                    cResult[i] = NonBatchProc(tData.type, tData.content);
                    
                }
                else if (BATCH_PU_REG_TYPE_REG != strLastType && BATCH_PU_REG_TYPE_REG == tData.type)   // 从其他类型请求切换为注册请求
                {
                    strLastType  = BATCH_PU_REG_TYPE_REG;
                    dwStartIndex = i;
                    dwEndIndex   = dwStartIndex;
                }
                else
                {
                    if (BATCH_PU_REG_TYPE_REG == tData.type)                    // 连续注册请求
                    {
                        dwEndIndex = i;
                    }
                    else                                                        // 其他则直接处理（目前的优化只针对注册）
                    {
                        cResult[i] = NonBatchProc(tData.type, tData.content);
                    }
                }
            }

            // 最后一次
            if (BATCH_PU_REG_TYPE_REG == strLastType)
            {
                // 先将之前累积的批量请求给处理掉(注意，BatchRegProc的处理区间是[dwStartIndex, dwEndIndex)，所以这里要使用dwEndIndex + 1)
                BatchRegProc(cData, dwStartIndex, dwEndIndex + 1, cResult);
            }

            PostMsgRsp(pcOspSipMsg->GetSipTransID(), cRsp);

            // 这里要注意，在注册应答之后，对于PUI接入的设备，还需要发送开启GPS的请求，但是由于批量注册请求目前只有gbu使用，故这里暂时没做该处理
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
        //已经注册的pu需要强制断开
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
        TASKLOG(CMU_MOD_BATCH, ERROR_LEV, "暂不接受注册服务!\n");
        return ERR_CMU_NOT_SERVICE_TEMPORARILY;
    }

    if (!m_pcPuiRegTask->GetIsSupportRedis())
    {
        TASKLOG(CMU_MOD_BATCH, ERROR_LEV, "pui[%u]不支持redis!\n", m_pcPuiRegTask->GetTaskNO());
        return ERR_CMU_UNCOMPATIBLE_REQ;
    }

    //是否入网
    CPuData *pcPuData = CCmuData::GetInstance()->GetPuData(strDevId, true);
    if (NULL == pcPuData)
    {
        TASKLOG(CMU_MOD_BATCH, ERROR_LEV, "pu[%s]未入网\n", strDevId.c_str());
        return ERR_CMU_PU_NOT_IN_TOP;
    }

    //是否已经注册
    if (pcPuData->IsPuRegistered())
    {
        TASKLOG(CMU_MOD_BATCH, ERROR_LEV, "pu[%s]已经注册\n", strDevId.c_str());
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

			// 之前是上线，则现在需要发送下线通知
            pcPuData->PublishOnOff();

            //向uas写设备下线日志
            ((CCmuCoreInst*)GetInstance())->GetUasLogTask()->AddDeviceLog(pcPuData->GetDevLog());
        }
    }

    // 删除和该设备相关的Task
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

    // 1. 先内部处理
    vector<string> cOffline;
    cOffline.reserve(cDevId.size());
    for (vector<string>::iterator it4Dev = cDevId.begin(); cDevId.end() != it4Dev; ++it4Dev)
    {
        map<string, TRegExtInfo>::iterator it = m_cPuSet.find(*it4Dev);
        if (m_cPuSet.end() != it)                   // 已注册或模拟注册
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

            // 删除和该设备相关的Task
            if (NULL != it->second.pcLoginTask)
            {
                it->second.pcLoginTask->DestroyChildTasks();

                delete it->second.pcLoginTask;
                it->second.pcLoginTask = NULL;
            }

            m_cPuSet.erase(it++);
        }
    }

    // 2. 下线状态处理
    if (!cOffline.empty())
    {
        CPuiNotifyTask *pcTask = g_cPuiNotifyList.GetPuiNotifyTask(m_pcPuiRegTask->GetDevSipUri());
        CMS_ASSERT(NULL != pcTask && "pcTask = NULL!!!");
        pcTask->Add(cOffline);
    }

    // 3. 下线日志 todo by xiongxh 国标子域下线，日志写不写？
    //TDeviceLogInfo tLogInfo = m_pcPuiRegTask->GetDevLogInfo(false);

    // gbu使用cmssdk的方式非常特别，puireg是cmssdk完成的，但是后续操作又是使用的gbu的uri，这里为了便于排查问题，使用gbu的uri
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

        // 先做一次预注册处理
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

    // 开始处理
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

    // 在线设备处理
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
    //是否启用
    if (!tInfo.use)
    {
        TASKLOG(CMU_MOD_BATCH, ERROR_LEV, "pu[%s]未启用\n", strDevId.c_str());
        return ERR_CMU_PU_NOT_USED;
    }

    //是否过期
    if (tInfo.expire)
    {
        TASKLOG(CMU_MOD_BATCH, ERROR_LEV, "pu[%s]已经过期\n", strDevId.c_str());
        return ERR_CMU_PU_EXPIRE;
    }

    u32 dwErr = ValidLicense(strDevId, tInfo.creator, tInfo.license, tInfo.chnnum, tInfo.freeLicense);
    if (CMS_SUCCESS != dwErr)
    {
        TASKLOG(CMU_MOD_BATCH, ERROR_LEV, "pu[%s]license验证失败\n", strDevId.c_str());
        return dwErr;
    }

    CPuData *pcPuData = CCmuData::GetInstance()->GetPuData(strDevId);
    CMS_ASSERT(NULL != pcPuData && "pcPuData == NULL!!!");

    // 走到这里说明设备已经验证通过，可以上线了
    pcPuData->SetPuRegTask(this);

    TRegExtInfo tExtInfo;
    tExtInfo.strPuiIp = tInfo.puiIp;
    m_cPuSet.insert(map<string, TRegExtInfo>::value_type(strDevId, tExtInfo));

    // 接入模块是pui或者其他接入模块下有部分通道在线
    if (m_pcPuiRegTask->GetDevType() == DEV_TYPE_PUI || m_pcPuiRegTask->GetDevType() == DEV_TYPE_PUI_GB || tInfo.chnStatusFlag)
    {
        bOnline = true;

        //向uas写设备日志
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
    if (BATCH_PU_REG_TYPE_UNREG == strType)              // 注销设备
    {
        TSipURI tDevUri(strId);

        return UnRegProc(tDevUri.GetUser());
    }
    else if (BATCH_PU_REG_TYPE_OFFLINE == strType)       // 国标子域下线
    {
        return OfflineProc(strId);
    }
    else
    {
        return ERR_CMU_BATCH_REG_TYPE_INVALID;
    }
}

