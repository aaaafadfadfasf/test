#include "license_server_mng_task.h"
#include "utility/cnetmanager.h"
#include "config_util/tas_config.h"

CLicenseServerMngTask::CLicenseServerMngTask(CInstExt *pcInst) : CTask(pcInst)
{
    if (!OspSemBCreate(&m_hLicSDKInfoLock))
    {
        CLASSLOG(enLogSrcLicense, ERROR_LEV, "create m_hLicSDKInfoLock failed\n");
    }
    //配置用于初始化SDK的结构体
    s8	achCfgPath[MAX_PATH] = "/opt/kdm/tas/conf/";
    strcpy(m_tLicenseServerInfo.m_achIp, CTasConfig::GetLsIP().c_str());   //SDK验证服务器IP
    strcpy(m_tLicenseServerInfo.m_achCfgPath, achCfgPath);                 //SDK配置文件放置路径
    m_tLicenseServerInfo.m_wPort = CTasConfig::GetLsPort();                //SDK验证服务器端口
    m_tLicenseServerInfo.m_dwCheckCount = LICENSESDK_MAX_RETRY_COUNT;      //SDK检测多少次后无响应认定为断链
    m_tLicenseServerInfo.m_dwCheckTime = LICENSESDK_CHECK_TIME;            //SDK每次检测间隔时间
    m_tLicenseServerInfo.m_pfEvent = (pfEventCB)&CLicenseServerMngTask::PFSDKEventCB;   //SDK回调函数，处理超时和停止授权
    m_dwLicenseSDKInitHandle = NULL;


    //配置申请license的选项
    m_tLicenseReqParam.m_dwApplylicNum = 1;                         //一个平台只申请一个license key
    m_tLicenseReqParam.m_dwFreeTime = LICENSESDK_FREETIME;          //设置失效时间
    strcpy(m_tLicenseReqParam.m_achModel, LICENSESDK_PRODUCT_NAME); //License SDK中，key规则的名称，为"监控平台V2 1.0 "  

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
            //处理回调函数的请求
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
//超时重试
u32 CLicenseServerMngTask::OnIdleTimerPoll()
{
    if (GetCurStateHoldTime() >= LICENSE_KEY_INITSDK_CHECK_TIME)
    {
        //初始化失败也重试
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
    //重试apply
    if (GetCurStateHoldTime() >= LICENSE_KEY_CHECK_TIME)
    {
        ApplyLicenseKey(); //尝试获取授权，获取成功后切换状态,失败后返回当前状态，等待下一次
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

//循环获取更新licensekey
u32 CLicenseServerMngTask::OnServerTimerPoll()
{
    if (GetCurStateHoldTime() >  LICENSE_KEY_CHECK_TIME+10)
    {
        UpdateLicenseKey();
        NextState(enStateServer);//设置超时轮询
    }
    return TIMERPOLL_DONE;
}

/***以下为功能函数*************************************************/


#ifdef WIN32
#ifndef strtok_r
#define strtok_r(str, sep, lasts) pSepLast=NULL;strtok(str, sep)
#endif
#endif // WIN32


//初始化sdk，本地License
u32 CLicenseServerMngTask::InitSDK()
{
    SDK_ERRCODE enSdkErrCode = SDK_NO_ERR;

    if (enLsKeyTypeLocal == CTasConfig::GetLsKeyType())
    {
        //使用本地Lic时候，直接进入server状态
        NextState(enStateServer);
        //成功后马上update
        UpdateLicenseKey();
    }
    else  if (enLsKeyTypeNet == CTasConfig::GetLsKeyType())
    {
        OspSemTake(m_hLicSDKInfoLock);
        //SDK_Init超时时间为10s
        enSdkErrCode = SDK_Init(m_tLicenseServerInfo, &m_dwLicenseSDKInitHandle);
        OspSemGive(m_hLicSDKInfoLock);
        //初始化sdk成功时，进入等待应用状态
        if (SDK_NO_ERR == enSdkErrCode)
        {
            UpdateLicenseServerConnectState((s32)CLicenseAppData::enLicenseServerConnectedUnAuth);
            NextState(enStateApply);
            //初始化成功后马上开始apply
            ApplyLicenseKey();
        }
        else
        {
            CLASSLOG(enLogSrcLicense, ERROR_LEV, "[License SDK]CLicensServerMngTask InitSDK Failed, handle[%d], errno[%d]\n", m_dwLicenseSDKInitHandle,enSdkErrCode);
            TLicenceInfo tLicense;
            CLicenseAppData::TKeyType tKeyType = CLicenseAppData::enInvalidKey;
            bool  bOK = false;
            //获取失败报错1次
            AppData().UpdateLicenseKey(bOK, tKeyType, tLicense);      
            UpdateLicenseServerConnectState((s32)CLicenseAppData::enLicenseServerUnConnected);
            //重置时间，等待重试
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

//获取授权，本地license不会进入该状态，直接进入server态，只处理网络license
void CLicenseServerMngTask::ApplyLicenseKey()
{
    SDK_ERRCODE enSdkErrCode = SDK_NO_ERR;
    bool bOK = true;
    //使用网络license时
    if (enLsKeyTypeNet == CTasConfig::GetLsKeyType())
    {
        s32 nApplyNum = 0;
        //使用LicenseSDK
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
            //获取key失败的情况
            bOK = false;
            CLASSLOG(enLogSrcLicense, ERROR_LEV, "[License SDK] Apply SDK Fail! SDK errCode %d, ApplyNum is %d\n", enSdkErrCode,nApplyNum);
            NextState(enStateApply);
            //错误则设置update失败，错误4次后告警
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

//更新LicenseKey的值，为无限重试，因此不需要返回值
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
    //license信息通过直接调用修改，不通过消息返回
    AppData().UpdateLicenseKey(bOK, tKeyType, tLicense);
    static_cast<CLicenseInst *>(this->GetInstance())->CheckLicenseKey(&AppData());
}



//调用SDK_GetValue获取LicenseInfo
//参照ReadLicenseContent函数
//不校验license
bool CLicenseServerMngTask::GetLicenseValue(TLicenceInfo &tLicense, CLicenseAppData::TKeyType &tKeyType)
{
    bool bRet = true;
    u16 wSdkErrCode = SDK_NO_ERR;
    
    s8 achTmp[1024];
    s32 nLicenseLen;
    CCMSTime cTime;

    nLicenseLen = sizeof(achTmp);
    //读取Key类型
    //网络key,增加类型enNetKey
    tKeyType = CLicenseAppData::enNetKey;

    //读取KEY_RESULT
    //网络key无KEY_RESULT字段
    nLicenseLen = sizeof(achTmp);
    wSdkErrCode = SDK_GetValue("KEY_RESLUT", achTmp, nLicenseLen, m_dwLicenseSDKInitHandle); 

    //读取KEY_LICENSE_TYPE
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
        // 未成功读取时表示无此限制
        tLicense.m_bEnableOtherManufactureDirectAccess = true;
    }
    //外厂商PU允许接入数目信息
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

        // 解释外厂商PU允许接入数目信息，内容格式为 dahua=90;haikang=30;xx=value
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
        // 未成功读取时什么也不做
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


/*回调函数*******************/
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
    //断链消息无需处理
    if (MESG_DISSCONNECT == enSdkMesgType)
    {
        CLASSLOG(enLogSrcLicense, WARNING_LEV, "[License SDK] Disconnected from License Server!\n");               
    }
    //停止授权，或超过freetime时处理该消息。等待重新申请授权
    else if (MESG_STOP_AUTHORIZE == enSdkMesgType)
    {
        TLicenceInfo tLicense;
        CLicenseAppData::TKeyType tKeyType = CLicenseAppData::enInvalidKey;
        bool  bOK = false;
        //updateLicenseKey函数刷新key，失败4次后认定key连接失败并提示，这里手动让它失败4次
        for (int i = 0; i < 4; i++)
        {
            AppData().UpdateLicenseKey(bOK, tKeyType, tLicense);
        }
        static_cast<CLicenseInst *>(this->GetInstance())->CheckLicenseKey(&AppData());
        //需要重新申请，返回apply状态
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

    //需要更改到的验证方式,以前用KeyType来表示，其实不太清晰，用验证方式表示比较准确
    u32 dwAuthMethod = cReq.GetAuthenticationMethod();
    string strLicenseServerIpAddr = cReq.GetLicenseServerIpAddr();
    u32 dwLicenseServerPort = cReq.GetLicenseServerPort();

    CLASSLOG(enLogSrcLicense, EVENT_LEV, "[License SDK]Change AuthMethod to %d!\n", dwAuthMethod); 

    do
    {
        if (enLsKeyTypeLocal == CTasConfig::GetLsKeyType() )
        {
            //验证方式为本地，且未修改，就直接返回成功了
            if (enLsKeyTypeLocal == dwAuthMethod) 
            {
                cRsp.SetErrorCode(CMS_SUCCESS);
                break;
            }
                //这里之后就处理了，修改重新初始化License服务器
        }
        else if ( enLsKeyTypeNet == CTasConfig::GetLsKeyType() )
        {
            //参数没变的时候，就不重新验证了
            if (enLsKeyTypeNet == dwAuthMethod)
            {
                //这边就不在相同时候不验证了，爱刷就刷吧，手贱自找
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
        //之后修改3a license相关配置的操作了，并应用
        //先修改配置
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
        //更新sdk配置，之后初始化就用这些配置了,之后init和apply都是用这些了
        strcpy(m_tLicenseServerInfo.m_achIp, CTasConfig::GetLsIP().c_str());   //SDK验证服务器IP
        m_tLicenseServerInfo.m_wPort = CTasConfig::GetLsPort();                //SDK验证服务器端口
        if (!cReq.GetLicenseServerLicenseId().empty() && cReq.GetLicenseServerLicenseId().length() <=NAME_LEN)
        {
            CLASSLOG(enLogSrcLicense, EVENT_LEV, "[License SDK] 指定id [%s]入网!\n", cReq.GetLicenseServerLicenseId().c_str() );
            strcpy(m_tLicenseReqParam.m_achKeyId, cReq.GetLicenseServerLicenseId().c_str());//指定id获取
        }
        else
        {
            memset(m_tLicenseReqParam.m_achKeyId,0,sizeof(m_tLicenseReqParam.m_achKeyId));
        }
       OspSemGive(m_hLicSDKInfoLock);

         //先停止授权，再更换授权
         TLicenceInfo tLicense;
         CLicenseAppData::TKeyType tKeyType = CLicenseAppData::enInvalidKey;
         bool  bOK = false;
          //updateLicenseKey函数刷新key，失败4次后认定key连接失败并提示，这里手动让它失败4次
         for (int i = 0; i < 4; i++)
         {
             AppData().UpdateLicenseKey(bOK, tKeyType, tLicense);
          }
         static_cast<CLicenseInst *>(this->GetInstance())->CheckLicenseKey(&AppData());
         //需要重新申请，返回apply状态
         UpdateLicenseServerConnectState((s32)CLicenseAppData::enLicenseServerUnConnected);

         //0 是成功
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

//刷新当前连接状态，只有使用服务器验证时候调用，其他时候不调用，不刷新状态的。
void CLicenseServerMngTask::UpdateLicenseServerConnectState(s32 nConnectStatus)
{
    CInnerLicenseServerStatusRefreshReq cReq;
    cReq.nMsg = nConnectStatus;
    PostMsg2License(msgLicenseServertatusRefreshReq, cReq);
}


