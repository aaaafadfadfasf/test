#include "license_server_mng_task.h"
#include "utility/cnetmanager.h"
#include "config_util/tas_config.h"

CLicenseServerMngTask::CLicenseServerMngTask(CInstExt *pcInst) : CTask(pcInst)
{
    if (!OspSemBCreate(&m_hLicSDKInfoLock))
    {
        CLASSLOG(enLogSrcLicense, ERROR_LEV, "create m_hLicSDKInfoLock failed\n");
    }
    //�������ڳ�ʼ��SDK�Ľṹ��
    s8	achCfgPath[MAX_PATH] = "/opt/kdm/tas/conf/";
    strcpy(m_tLicenseServerInfo.m_achIp, CTasConfig::GetLsIP().c_str());   //SDK��֤������IP
    strcpy(m_tLicenseServerInfo.m_achCfgPath, achCfgPath);                 //SDK�����ļ�����·��
    m_tLicenseServerInfo.m_wPort = CTasConfig::GetLsPort();                //SDK��֤�������˿�
    m_tLicenseServerInfo.m_dwCheckCount = LICENSESDK_MAX_RETRY_COUNT;      //SDK�����ٴκ�����Ӧ�϶�Ϊ����
    m_tLicenseServerInfo.m_dwCheckTime = LICENSESDK_CHECK_TIME;            //SDKÿ�μ����ʱ��
    m_tLicenseServerInfo.m_pfEvent = (pfEventCB)&CLicenseServerMngTask::PFSDKEventCB;   //SDK�ص�����������ʱ��ֹͣ��Ȩ
    m_dwLicenseSDKInitHandle = NULL;


    //��������license��ѡ��
    m_tLicenseReqParam.m_dwApplylicNum = 1;                         //һ��ƽֻ̨����һ��license key
    m_tLicenseReqParam.m_dwFreeTime = LICENSESDK_FREETIME;          //����ʧЧʱ��
    strcpy(m_tLicenseReqParam.m_achModel, LICENSESDK_PRODUCT_NAME); //License SDK�У�key��������ƣ�Ϊ"���ƽ̨V2 1.0 "  

}

CLicenseServerMngTask::~CLicenseServerMngTask()
{
    if (enLsKeyTypeNet == CTasConfig::GetLsKeyType())
    {
        if (m_dwLicenseSDKInitHandle != NULL) 
        {
           SDK_Quit(m_dwLicenseSDKInitHandle);
           m_dwLicenseSDKInitHandle = NULL;
        }
    }
}


void CLicenseServerMngTask::InitStateMachine()
{
    CStateProc cProc;

    cProc.ProcMsg = (CTask::PFProcMsg)&CLicenseServerMngTask::OnIdle;
    cProc.ProcErrMsg = (CTask::PFProcMsg)&CLicenseServerMngTask::OnIdle;
    cProc.TimerPoll = (CTask::PFTimerPoll)&CLicenseServerMngTask::OnIdleTimerPoll;
    AddRuleProc(enStateIdle, cProc);

    cProc.ProcMsg = (CTask::PFProcMsg)&CLicenseServerMngTask::OnApply;
    cProc.ProcErrMsg = (CTask::PFProcMsg)&CLicenseServerMngTask::OnApply;
    cProc.TimerPoll = (CTask::PFTimerPoll)&CLicenseServerMngTask::OnApplyTimerPoll;
    AddRuleProc(enStateApply, cProc);

    cProc.ProcMsg = (CTask::PFProcMsg)&CLicenseServerMngTask::OnServer;
    cProc.ProcErrMsg = (CTask::PFProcMsg)&CLicenseServerMngTask::OnServer;
    cProc.TimerPoll = (CTask::PFTimerPoll)&CLicenseServerMngTask::OnServerTimerPoll;
    AddRuleProc(enStateServer, cProc);

    NextState(enStateIdle);
    
}

u32 CLicenseServerMngTask::OnIdle(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_OK;
    SDK_ERRCODE enSdkErrCode = SDK_NO_ERR;

    switch (pcMsg->event)
    {
    case msgLicenseSDKCbReq:
        {
            //����ص�����������
            ProcCBMsg(pcMsg);
        }
        break;
    case TAS_LICENSE_SET_REQ:
        {
            ProcLicenseCfgSet(pcMsg);
        }
        break;

    default:      
        {
            CLASSLOG(enLogSrcLicense, ERROR_LEV, "CLicensServerMngTask Unkown event,[%d]\n", pcMsg->event);
            dwRet = PROCMSG_FAIL;
        }
        break;
    }
    return dwRet;
}
//��ʱ����
u32 CLicenseServerMngTask::OnIdleTimerPoll()
{
    if (GetCurStateHoldTime() >= LICENSE_KEY_INITSDK_CHECK_TIME)
    {
        //��ʼ��ʧ��Ҳ����
        InitSDK();
    }
    return TIMERPOLL_DONE;

}

u32 CLicenseServerMngTask::OnApply(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_OK;

    switch (pcMsg->event)
    {
    case msgLicenseSDKCbReq:
        {
            ProcCBMsg(pcMsg);
        }
        break;
    case TAS_LICENSE_SET_REQ:
        {
            ProcLicenseCfgSet(pcMsg);
        }
        break;
    default:
        {
            CLASSLOG(enLogSrcLicense, ERROR_LEV, "CLicensServerMngTask Unkown event,[%d]\n", pcMsg->event);
            dwRet = PROCMSG_FAIL;
         }
        break;
    }
   
    return dwRet;
}

u32 CLicenseServerMngTask::OnApplyTimerPoll()
{
    //����apply
    if (GetCurStateHoldTime() >= LICENSE_KEY_CHECK_TIME)
    {
        ApplyLicenseKey(); //���Ի�ȡ��Ȩ����ȡ�ɹ����л�״̬,ʧ�ܺ󷵻ص�ǰ״̬���ȴ���һ��
    }   
    return TIMERPOLL_DONE;
}


u32 CLicenseServerMngTask::OnServer(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_OK;
    switch (pcMsg->event)
    {
    case msgLicenseSDKCbReq:
        {
            ProcCBMsg(pcMsg);
        }
        break;
    case TAS_LICENSE_SET_REQ:
        {
            ProcLicenseCfgSet(pcMsg);
        }
        break;
           
    default:
        {
            CLASSLOG(enLogSrcLicense, ERROR_LEV, "CLicensServerMngTask Unkown event,[%d]\n", pcMsg->event);
            dwRet = PROCMSG_FAIL;
        }
        break;
    }
    return dwRet;
}

//ѭ����ȡ����licensekey
u32 CLicenseServerMngTask::OnServerTimerPoll()
{
    if (GetCurStateHoldTime() >  LICENSE_KEY_CHECK_TIME+10)
    {
        UpdateLicenseKey();
        NextState(enStateServer);//���ó�ʱ��ѯ
    }
    return TIMERPOLL_DONE;
}

/***����Ϊ���ܺ���*************************************************/


#ifdef WIN32
#ifndef strtok_r
#define strtok_r(str, sep, lasts) pSepLast=NULL;strtok(str, sep)
#endif
#endif // WIN32


//��ʼ��sdk������License
u32 CLicenseServerMngTask::InitSDK()
{
    SDK_ERRCODE enSdkErrCode = SDK_NO_ERR;

    if (enLsKeyTypeLocal == CTasConfig::GetLsKeyType())
    {
        //ʹ�ñ���Licʱ��ֱ�ӽ���server״̬
        NextState(enStateServer);
        //�ɹ�������update
        UpdateLicenseKey();
    }
    else  if (enLsKeyTypeNet == CTasConfig::GetLsKeyType())
    {
        OspSemTake(m_hLicSDKInfoLock);
        //SDK_Init��ʱʱ��Ϊ10s
        enSdkErrCode = SDK_Init(m_tLicenseServerInfo, &m_dwLicenseSDKInitHandle);
        OspSemGive(m_hLicSDKInfoLock);
        //��ʼ��sdk�ɹ�ʱ������ȴ�Ӧ��״̬
        if (SDK_NO_ERR == enSdkErrCode)
        {
            UpdateLicenseServerConnectState((s32)CLicenseAppData::enLicenseServerConnectedUnAuth);
            NextState(enStateApply);
            //��ʼ���ɹ������Ͽ�ʼapply
            ApplyLicenseKey();
        }
        else
        {
            CLASSLOG(enLogSrcLicense, ERROR_LEV, "[License SDK]CLicensServerMngTask InitSDK Failed, handle[%d], errno[%d]\n", m_dwLicenseSDKInitHandle,enSdkErrCode);
            TLicenceInfo tLicense;
            CLicenseAppData::TKeyType tKeyType = CLicenseAppData::enInvalidKey;
            bool  bOK = false;
            //��ȡʧ�ܱ���1��
            AppData().UpdateLicenseKey(bOK, tKeyType, tLicense);      
            UpdateLicenseServerConnectState((s32)CLicenseAppData::enLicenseServerUnConnected);
            //����ʱ�䣬�ȴ�����
            NextState(enStateIdle);
          //  SDK_Quit();
        }

    }
    else
    {
        CLASSLOG(enLogSrcLicense, ERROR_LEV, "[License SDK] Wrong License Type [%d]! Check Conf file!\n", CTasConfig::GetLsKeyType());
    }
    return enSdkErrCode;
}

//��ȡ��Ȩ������license��������״̬��ֱ�ӽ���server̬��ֻ��������license
void CLicenseServerMngTask::ApplyLicenseKey()
{
    SDK_ERRCODE enSdkErrCode = SDK_NO_ERR;
    bool bOK = true;
    //ʹ������licenseʱ
    if (enLsKeyTypeNet == CTasConfig::GetLsKeyType())
    {
        s32 nApplyNum = 0;
        //ʹ��LicenseSDK
         CLASSLOG(enLogSrcLicense, EVENT_LEV, "[License SDK] ApplyKey Id is  %s\n", m_tLicenseReqParam.m_achKeyId); 
        enSdkErrCode = SDK_ApplyLicense(m_dwLicenseSDKInitHandle,m_tLicenseReqParam, nApplyNum);
        if (SDK_NO_ERR == enSdkErrCode && 1 == nApplyNum)
        {
            CLASSLOG(enLogSrcLicense, EVENT_LEV, "[License SDK] Apply SDK SUCCESS!\n");
            UpdateLicenseServerConnectState((s32)CLicenseAppData::enLicenseServerConnectedAuth);
            NextState(enStateServer);           
        }       
        else
        {
            //��ȡkeyʧ�ܵ����
            bOK = false;
            CLASSLOG(enLogSrcLicense, ERROR_LEV, "[License SDK] Apply SDK Fail! SDK errCode %d, ApplyNum is %d\n", enSdkErrCode,nApplyNum);
            NextState(enStateApply);
            //����������updateʧ�ܣ�����4�κ�澯
            TLicenceInfo tLicense;
            CLicenseAppData::TKeyType tKeyType = CLicenseAppData::enInvalidKey;
            AppData().UpdateLicenseKey(bOK, tKeyType, tLicense);
            static_cast<CLicenseInst *>(this->GetInstance())->CheckLicenseKey(&AppData());
        }
    }
    else
    {
        CLASSLOG(enLogSrcLicense, ERROR_LEV, "[License SDK] Wrong License Type [%d]! Check Conf file!\n", CTasConfig::GetLsKeyType());
        NextState(enStateApply);
    }
}

//����LicenseKey��ֵ��Ϊ�������ԣ���˲���Ҫ����ֵ
void CLicenseServerMngTask::UpdateLicenseKey()
{
    SDK_ERRCODE enSdkErrCode = SDK_NO_ERR;
    bool bOK = true;
    TLicenceInfo tLicense;
    CLicenseAppData::TKeyType tKeyType;
    if (enLsKeyTypeLocal == CTasConfig::GetLsKeyType())
    {
        bOK = static_cast<CLicenseInst*>(this->GetInstance())->ReadLicenseContent(tLicense, tKeyType);
    }
    else if (enLsKeyTypeNet == CTasConfig::GetLsKeyType())
    {
        bOK = GetLicenseValue(tLicense, tKeyType);
    }
    else
    {
        CLASSLOG(enLogSrcLicense, ERROR_LEV, "[License SDK] Wrong License Type [%d]! Check Conf file!\n", CTasConfig::GetLsKeyType());
    }
    //license��Ϣͨ��ֱ�ӵ����޸ģ���ͨ����Ϣ����
    AppData().UpdateLicenseKey(bOK, tKeyType, tLicense);
    static_cast<CLicenseInst *>(this->GetInstance())->CheckLicenseKey(&AppData());
}



//����SDK_GetValue��ȡLicenseInfo
//����ReadLicenseContent����
//��У��license
bool CLicenseServerMngTask::GetLicenseValue(TLicenceInfo &tLicense, CLicenseAppData::TKeyType &tKeyType)
{
    bool bRet = true;
    u16 wSdkErrCode = SDK_NO_ERR;
    
    s8 achTmp[1024];
    s32 nLicenseLen;
    CCMSTime cTime;

    nLicenseLen = sizeof(achTmp);
    //��ȡKey����
    //����key,��������enNetKey
    tKeyType = CLicenseAppData::enNetKey;

    //��ȡKEY_RESULT
    //����key��KEY_RESULT�ֶ�
    nLicenseLen = sizeof(achTmp);
    wSdkErrCode = SDK_GetValue("KEY_RESLUT", achTmp, nLicenseLen, m_dwLicenseSDKInitHandle); 

    //��ȡKEY_LICENSE_TYPE
    nLicenseLen = sizeof(achTmp);
    wSdkErrCode = SDK_GetValue( "KEY_LICENSE_TYPE", achTmp, nLicenseLen,m_dwLicenseSDKInitHandle);
    if (SUCCESS_LICENSE != wSdkErrCode)
    {
        CLASSLOG(enLogSrcLicense, WARNING_LEV, "GetValue KEY_LICENSE_TYPE failed, RetVal=%u\n", (u32)wSdkErrCode);
        return false;
    }
    if (strcmp(achTmp, "KDMAAA") != 0)
    {
        achTmp[sizeof(achTmp)-1] = '\0';
        CLASSLOG(enLogSrcLicense, WARNING_LEV, "KEY_LICENSE_TYPE wrong value:%s\n", achTmp);
        return false;
    }

    nLicenseLen = sizeof(achTmp);
    wSdkErrCode = SDK_GetValue("KEY_LICENSE_KEY", achTmp, nLicenseLen,m_dwLicenseSDKInitHandle);
    if (SUCCESS_LICENSE != wSdkErrCode)
    {
        CLASSLOG(enLogSrcLicense, WARNING_LEV, "GetValue KEY_LICENSE_KEY failed, RetVal=%u\n", (u32)wSdkErrCode);
        return false;
    }
    tLicense.m_strLicenseId = achTmp;

    nLicenseLen = sizeof(achTmp);
    wSdkErrCode = SDK_GetValue("KEY_LICENSE_VERSION", achTmp, nLicenseLen,m_dwLicenseSDKInitHandle);
    if (SUCCESS_LICENSE != wSdkErrCode)
    {
        CLASSLOG(enLogSrcLicense, WARNING_LEV, "GetValue KEY_LICENSE_VERSION fail, RetVal=%u\n", (u32)wSdkErrCode);
        return false;
    }
    tLicense.m_strVersion = achTmp;

    nLicenseLen = sizeof(achTmp);
    wSdkErrCode = SDK_GetValue("KEY_LICENSE_MAX_VIDEO_NUM", achTmp, nLicenseLen,m_dwLicenseSDKInitHandle);
    if (SUCCESS_LICENSE != wSdkErrCode)
    {
        CLASSLOG(enLogSrcLicense, WARNING_LEV, "GetValue KEY_LICENSE_MAX_VIDEO_NUM fail, RetVal=%u\n", (u32)wSdkErrCode);
        tLicense.m_dwTotalVideoMaxNum = 0;
    }
    else
    {
        tLicense.m_dwTotalVideoMaxNum = atoi(achTmp);
    }

    nLicenseLen = sizeof(achTmp);
    wSdkErrCode = SDK_GetValue("KEY_LICENSE_MAX_ENCODER_CHANNEL_NUM", achTmp, nLicenseLen,m_dwLicenseSDKInitHandle);
    if (SUCCESS_LICENSE != wSdkErrCode)
    {
        CLASSLOG(enLogSrcLicense, WARNING_LEV, "GetValue KEY_LICENSE_MAX_ENCODER_CHANNEL_NUM fail, RetVal=%u\n", (u32)wSdkErrCode);
        tLicense.m_dwTotalPUMaxNum = 0;
    }
    else
    {
        tLicense.m_dwTotalPUMaxNum = atoi(achTmp);
    }

    nLicenseLen = sizeof(achTmp);
    wSdkErrCode = SDK_GetValue("KEY_LICENSE_MAX_RECORD_NUM", achTmp, nLicenseLen,m_dwLicenseSDKInitHandle);
    if (SUCCESS_LICENSE != wSdkErrCode)
    {
        CLASSLOG(enLogSrcLicense, WARNING_LEV, "GetValue KEY_LICENSE_MAX_RECORD_NUM fail, RetVal=%u\n", (u32)wSdkErrCode);
        tLicense.m_dwTotalRecMaxNum = 0;
    }
    else
    {
        tLicense.m_dwTotalRecMaxNum = atoi(achTmp);
    }

    nLicenseLen = sizeof(achTmp);
    wSdkErrCode = SDK_GetValue("KEY_LICENSE_MAX_INTELLIGENCE_NUM", achTmp, nLicenseLen,m_dwLicenseSDKInitHandle);
    if (SUCCESS_LICENSE != wSdkErrCode)
    {
        CLASSLOG(enLogSrcLicense, EVENT_LEV, "GetValue KEY_LICENSE_MAX_INTELLIGENCE_NUM fail, RetVal=%u\n", (u32)wSdkErrCode);
        tLicense.m_dwIntelligenceNum = 0;
    }
    else
    {
        tLicense.m_dwIntelligenceNum = atoi(achTmp);
    }

    u32 dwYear, dwMonth, dwDay;
    nLicenseLen = sizeof(achTmp);
    wSdkErrCode = SDK_GetValue("KEY_LICENSE_CREATE_DATE", achTmp, nLicenseLen,m_dwLicenseSDKInitHandle);
    if (SUCCESS_LICENSE != wSdkErrCode)
    {
        CLASSLOG(enLogSrcLicense, EVENT_LEV, "GetValue KEY_LICENSE_CREATE_DATE fail, RetVal=%u\n", (u32)wSdkErrCode);
        tLicense.m_tCreateDate = time(NULL);
    }
    else
    {
        sscanf(achTmp, "%u-%u-%u", &dwYear, &dwMonth, &dwDay);
        cTime.SetLocalTime(dwYear, dwMonth, dwDay, 0, 0, 0);
        tLicense.m_tCreateDate = cTime.GetTime();
    }

    nLicenseLen = sizeof(achTmp);
    wSdkErrCode = SDK_GetValue("KEY_LICENSE_MAX_DATE", achTmp, nLicenseLen,m_dwLicenseSDKInitHandle);
    if (SUCCESS_LICENSE != wSdkErrCode)
    {
        CLASSLOG(enLogSrcLicense, WARNING_LEV, "GetValue KEY_LICENSE_MAX_DATE fail, RetVal=%u\n", (u32)wSdkErrCode);
        return false;
    }
    else
    {
        sscanf(achTmp, "%u-%u-%u", &dwYear, &dwMonth, &dwDay);
        cTime.SetLocalTime(dwYear, dwMonth, dwDay, 0, 0, 0);
        tLicense.m_tExpireDate = cTime.GetTime();
    }

    tLicense.m_dwTotalNRUMaxNum = 0x7FFFFFFF;

    nLicenseLen = sizeof(achTmp);
    wSdkErrCode = SDK_GetValue("KEY_LICENSE_OTHER_ENCODER_DIRECT", achTmp, nLicenseLen,m_dwLicenseSDKInitHandle);
    if (SUCCESS_LICENSE == wSdkErrCode)
    {
        achTmp[sizeof(achTmp)-1] = '\0';
        CLASSLOG(enLogSrcLicense, ALL_LEV, "KEY_LICENSE_OTHER_ENCODER_DIRECT's value:%s\n", achTmp);
        if (Stricmp(achTmp, "Yes"))
        {
            tLicense.m_bEnableOtherManufactureDirectAccess = true;
        }
        else
        {
            tLicense.m_bEnableOtherManufactureDirectAccess = false;
        }
    }
    else
    {
        CLASSLOG(enLogSrcLicense, ALL_LEV, "GetValue KEY_LICENSE_OTHER_ENCODER_DIRECT failed, RetVal=%u\n", (u32)wSdkErrCode);
        // δ�ɹ���ȡʱ��ʾ�޴�����
        tLicense.m_bEnableOtherManufactureDirectAccess = true;
    }
    //�⳧��PU���������Ŀ��Ϣ
    nLicenseLen = sizeof(achTmp);
    wSdkErrCode = SDK_GetValue("KEY_LICENSE_MAX_OTHER_ENCODER_NUM", achTmp, nLicenseLen,m_dwLicenseSDKInitHandle);
    if (SUCCESS_LICENSE != wSdkErrCode)
    {
        CLASSLOG(enLogSrcLicense, WARNING_LEV, "GetValue KEY_LICENSE_MAX_OTHER_ENCODER_NUM fail, RetVal=%u\n", (u32)wSdkErrCode);
        tLicense.m_dwOtherPUNum = 0;
    }
    else
    {
        tLicense.m_dwOtherPUNum = atoi(achTmp);
    }

    nLicenseLen = sizeof(achTmp);
    wSdkErrCode = SDK_GetValue("KEY_LICENSE_MAX_OTHER_ENCODER_CHANNEL", achTmp, nLicenseLen,m_dwLicenseSDKInitHandle);
    if (SUCCESS_LICENSE == wSdkErrCode)
    {
        if (static_cast<u32>(nLicenseLen) >= sizeof(achTmp))
        {
            CLASSLOG(enLogSrcLicense, ALL_LEV, "GetValue KEY_LICENSE_MAX_OTHER_ENCODER_CHANNEL Fail:content too long\n");
            return false;
        }
        achTmp[sizeof(achTmp)-1] = '\0';
        CLASSLOG(enLogSrcLicense, ALL_LEV, "KEY_LICENSE_MAX_OTHER_ENCODER_CHANNEL's value:%s\n", achTmp);

        // �����⳧��PU���������Ŀ��Ϣ�����ݸ�ʽΪ dahua=90;haikang=30;xx=value
        TOtherPULicenceInfo tOtherPULicenceInfo;
        s8 *pSep = ";";
        s8 *pSepLast;
        s8 *pToken;
        s8 *pString = &achTmp[0];

        s8 szOtherManufacture[1024];
        u32 dwOtherPuLicenseLimit = 0;
        bool bNeedAppendOtherLicense = true;
        while (true)
        {
            pToken = strtok_r(pString, pSep, &pSepLast);
            if (NULL == pToken)
            {
                break;
            }
            pString = NULL;

            s32 nRet = sscanf(pToken, "%[^=]=%d", szOtherManufacture, &tOtherPULicenceInfo.m_dwLicenseNum);
            if (2 != nRet)
            {
                break;
            }
            tOtherPULicenceInfo.m_strManufacture = szOtherManufacture;
            if (tOtherPULicenceInfo.m_strManufacture == "other")
            {
                bNeedAppendOtherLicense = false;
            }

            dwOtherPuLicenseLimit += tOtherPULicenceInfo.m_dwLicenseNum;
            tLicense.m_atOtherPULicenceInfo.push_back(tOtherPULicenceInfo);
        }

        if (bNeedAppendOtherLicense && dwOtherPuLicenseLimit < tLicense.m_dwOtherPUNum)
        {
            tOtherPULicenceInfo.m_strManufacture = "other";
            tOtherPULicenceInfo.m_dwLicenseNum = tLicense.m_dwOtherPUNum - dwOtherPuLicenseLimit;
            tLicense.m_atOtherPULicenceInfo.push_back(tOtherPULicenceInfo);
        }
    }
    else
    {
        CLASSLOG(enLogSrcLicense, ALL_LEV, "GetValue KEY_LICENSE_MAX_OTHER_ENCODER_CHANNEL fail, RetVal=%u\n", (u32)wSdkErrCode);
        // δ�ɹ���ȡʱʲôҲ����
    }

    nLicenseLen = sizeof(achTmp);
    wSdkErrCode = SDK_GetValue("KEY_LICENSE_MAX_MANAGE_CHANNEL_NUM", achTmp, nLicenseLen,m_dwLicenseSDKInitHandle);
    if (SUCCESS_LICENSE == wSdkErrCode)
    {
        tLicense.m_dwManagePUMaxNum = atoi(achTmp);
    }
    nLicenseLen = sizeof(achTmp);
    wSdkErrCode = SDK_GetValue("KEY_LICENSE_MAX_OTHER_STORAGE_SIZE", achTmp, nLicenseLen,m_dwLicenseSDKInitHandle);
    if (SUCCESS_LICENSE == wSdkErrCode)
    {
        tLicense.m_dwOtherStorageMaxSize = atoi(achTmp);
    }
    return true;
}

CLicenseAppData& CLicenseServerMngTask::AppData()
{
    return static_cast<CLicenseInst*>(GetInstance())->GetAppData();
}


/*�ص�����*******************/
void CLicenseServerMngTask::PFSDKEventCB(s32 dwMsg, s8* pchParam, void *pUser)
{
    CInnerLicenseSDKCBReq cReq;
    cReq.nMsg = dwMsg;
    PostMsg2License(msgLicenseSDKCbReq, cReq);
}

void CLicenseServerMngTask::ProcCBMsg(CMessage *const pcMsg)
{
    CInnerLicenseSDKCBReq *pcReq = *reinterpret_cast<CInnerLicenseSDKCBReq**>(pcMsg->content);
    SDK_MESG_TYPE enSdkMesgType = (SDK_MESG_TYPE)(pcReq->nMsg);
    //������Ϣ���账��
    if (MESG_DISSCONNECT == enSdkMesgType)
    {
        CLASSLOG(enLogSrcLicense, WARNING_LEV, "[License SDK] Disconnected from License Server!\n");               
    }
    //ֹͣ��Ȩ���򳬹�freetimeʱ�������Ϣ���ȴ�����������Ȩ
    else if (MESG_STOP_AUTHORIZE == enSdkMesgType)
    {
        TLicenceInfo tLicense;
        CLicenseAppData::TKeyType tKeyType = CLicenseAppData::enInvalidKey;
        bool  bOK = false;
        //updateLicenseKey����ˢ��key��ʧ��4�κ��϶�key����ʧ�ܲ���ʾ�������ֶ�����ʧ��4��
        for (int i = 0; i < 4; i++)
        {
            AppData().UpdateLicenseKey(bOK, tKeyType, tLicense);
        }
        static_cast<CLicenseInst *>(this->GetInstance())->CheckLicenseKey(&AppData());
        //��Ҫ�������룬����apply״̬
        UpdateLicenseServerConnectState((s32)CLicenseAppData::enLicenseServerConnectedUnAuth);
        NextState(enStateApply);
        CLASSLOG(enLogSrcLicense, ERROR_LEV, "[License SDK] Authorize stopped by License Server!\n");
    }

}

void CLicenseServerMngTask::ProcLicenseCfgSet(CMessage *const pcMsg)
{
    COspSipMsg* const pcSipMsg = reinterpret_cast<COspSipMsg* const>(pcMsg->content);
    CLicenseSetReq cReq;
    pcSipMsg->GetMsgBody(cReq);
    CLicenseSetRsp cRsp;
    cRsp.SetSession(cReq.GetSession());
    cRsp.SetSeqNum(cReq.GetSeqNum());
    cRsp.SetErrorCode(ERR_TAS_LICENSE_KEY_INVALID_LICENSE_FILE);

    //��Ҫ���ĵ�����֤��ʽ,��ǰ��KeyType����ʾ����ʵ��̫����������֤��ʽ��ʾ�Ƚ�׼ȷ
    u32 dwAuthMethod = cReq.GetAuthenticationMethod();
    string strLicenseServerIpAddr = cReq.GetLicenseServerIpAddr();
    u32 dwLicenseServerPort = cReq.GetLicenseServerPort();

    CLASSLOG(enLogSrcLicense, EVENT_LEV, "[License SDK]Change AuthMethod to %d!\n", dwAuthMethod); 

    do
    {
        if (enLsKeyTypeLocal == CTasConfig::GetLsKeyType() )
        {
            //��֤��ʽΪ���أ���δ�޸ģ���ֱ�ӷ��سɹ���
            if (enLsKeyTypeLocal == dwAuthMethod) 
            {
                cRsp.SetErrorCode(CMS_SUCCESS);
                break;
            }
                //����֮��ʹ����ˣ��޸����³�ʼ��License������
        }
        else if ( enLsKeyTypeNet == CTasConfig::GetLsKeyType() )
        {
            //����û���ʱ�򣬾Ͳ�������֤��
            if (enLsKeyTypeNet == dwAuthMethod)
            {
                //��߾Ͳ�����ͬʱ����֤�ˣ���ˢ��ˢ�ɣ��ּ�����
                /*
                if (strLicenseServerIpAddr == CTasConfig::GetLsIP() && dwLicenseServerPort == CTasConfig::GetLsPort()) 
                {
                    cRsp.SetErrorCode(CMS_SUCCESS);
                    break;
                }
                */
            }
        }
        else
        {
            cRsp.SetErrorCode(ERR_TAS_LICENSE_KEY_INVALID_LICENSE_FILE);
            break;
        }

        if (m_dwLicenseSDKInitHandle != NULL) 
        {
             SDK_ReleaseLicense(m_dwLicenseSDKInitHandle, m_tLicenseReqParam);
             SDK_Quit(m_dwLicenseSDKInitHandle);
             m_dwLicenseSDKInitHandle = NULL;
        }
        //֮���޸�3a license������õĲ����ˣ���Ӧ��
        //���޸�����
        CTasConfig::SetLsKeyType((u16)dwAuthMethod);
        if (!strLicenseServerIpAddr.empty()) 
        {
            CTasConfig::SetLsIp(strLicenseServerIpAddr);
        }
        if (dwLicenseServerPort != 0) 
        {
            CTasConfig::SetLsPort((u16)dwLicenseServerPort);
        }
        OspSemTake(m_hLicSDKInfoLock);
        //����sdk���ã�֮���ʼ��������Щ������,֮��init��apply��������Щ��
        strcpy(m_tLicenseServerInfo.m_achIp, CTasConfig::GetLsIP().c_str());   //SDK��֤������IP
        m_tLicenseServerInfo.m_wPort = CTasConfig::GetLsPort();                //SDK��֤�������˿�
        if (!cReq.GetLicenseServerLicenseId().empty() && cReq.GetLicenseServerLicenseId().length() <=NAME_LEN)
        {
            CLASSLOG(enLogSrcLicense, EVENT_LEV, "[License SDK] ָ��id [%s]����!\n", cReq.GetLicenseServerLicenseId().c_str() );
            strcpy(m_tLicenseReqParam.m_achKeyId, cReq.GetLicenseServerLicenseId().c_str());//ָ��id��ȡ
        }
        else
        {
            memset(m_tLicenseReqParam.m_achKeyId,0,sizeof(m_tLicenseReqParam.m_achKeyId));
        }
       OspSemGive(m_hLicSDKInfoLock);

         //��ֹͣ��Ȩ���ٸ�����Ȩ
         TLicenceInfo tLicense;
         CLicenseAppData::TKeyType tKeyType = CLicenseAppData::enInvalidKey;
         bool  bOK = false;
          //updateLicenseKey����ˢ��key��ʧ��4�κ��϶�key����ʧ�ܲ���ʾ�������ֶ�����ʧ��4��
         for (int i = 0; i < 4; i++)
         {
             AppData().UpdateLicenseKey(bOK, tKeyType, tLicense);
          }
         static_cast<CLicenseInst *>(this->GetInstance())->CheckLicenseKey(&AppData());
         //��Ҫ�������룬����apply״̬
         UpdateLicenseServerConnectState((s32)CLicenseAppData::enLicenseServerUnConnected);

         //0 �ǳɹ�
         if (!InitSDK())
         {
             cRsp.SetErrorCode(CMS_SUCCESS); 
         }
         else
         {
            cRsp.SetErrorCode(ERR_TAS_LICENSE_SDK_INIT_ERROR); 
         }
    }while(false);

    COspSipMsg cSipRspMsg;
    cSipRspMsg.SetSipTransID(pcSipMsg->GetSipTransID());
    string strMsgBody = cRsp.ToXml();
    cSipRspMsg.SetMsgBody(strMsgBody.c_str(), strMsgBody.size());
    OspPost(MAKEIID(AID_CCtrl, CInstance::DAEMON), cRsp.GetEventId(), cSipRspMsg.GetMsgBuf(), cSipRspMsg.GetMsgLen());
}

//ˢ�µ�ǰ����״̬��ֻ��ʹ�÷�������֤ʱ����ã�����ʱ�򲻵��ã���ˢ��״̬�ġ�
void CLicenseServerMngTask::UpdateLicenseServerConnectState(s32 nConnectStatus)
{
    CInnerLicenseServerStatusRefreshReq cReq;
    cReq.nMsg = nConnectStatus;
    PostMsg2License(msgLicenseServertatusRefreshReq, cReq);
}


