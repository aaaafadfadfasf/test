#include "license_mng.h"
#include "utility/cnetmanager.h"
#include "config_util/tas_config.h"
#include "inner_common.h"
#include "nmsconn.h"
#include "commonconfig/cmcfg.h"
#include "cms/utility/mischelper.h"
#include "cms/utility/md5.h"
#include "tasks/license_server_mng_task.h"
bool g_bNeedValidateManageLicense = false;


CLicenseApp g_cLicenseApp;
ENTasErrno g_enLicenseValidateResult = CMS_SUCCESS;

ENTasErrno AllocPULicense( const string &strManufacture, u32 dwLicenseNum,const TCreater &tCreater )
{
    CLicenseAppData *pData = &g_cLicenseApp;
    return pData->AllocPULicense(strManufacture, dwLicenseNum,tCreater);
}

void DeallocPULicense( const string &strManufacture, u32 dwLicenseNum )
{
    CLicenseAppData *pData = &g_cLicenseApp;
    pData->DeallocPULicense(strManufacture, dwLicenseNum);
}

ENTasErrno AllocManagePULicense(u32 dwVideoNum)
{
    CLicenseAppData *pData = &g_cLicenseApp;
    return pData->AllocManagePULicense(dwVideoNum);
}
void DeallocManagePULicense(u32 dwVideoNum)
{
    CLicenseAppData *pData = &g_cLicenseApp;
    pData->DeallocManagePULicense(dwVideoNum);
}
ENTasErrno AllocLicense( const CDeviceModelInfo &cModel ,const TCreater &tCreater)
{
    if (cModel.GetDeviceTypeId() != DEVICE_TYPE_ENCODER)
    {
        return CMS_SUCCESS;
    }

    for (vector<TDeviceCapInfo>::const_iterator it = cModel.GetDeviceCapInfo().begin();
        it != cModel.GetDeviceCapInfo().end(); ++it)
    {
        if (it->type == DEVICE_CAP_VIDEOSOURCE)
        {
            ENTasErrno enError = AllocPULicense(cModel.GetManufacturer(), it->license , tCreater);
            if (enError != CMS_SUCCESS)
            {
                return enError;
            }
            enError = AllocManagePULicense(it->num);
            if (enError != CMS_SUCCESS)
            {
                DeallocPULicense(cModel.GetManufacturer(), it->license);
                return enError;
            }
        }
    }

    return CMS_SUCCESS;
}

void DeallocLicense( const CDeviceModelInfo &cModel )
{
    if (cModel.GetDeviceTypeId() != DEVICE_TYPE_ENCODER)
    {
        return;
    }

    for (vector<TDeviceCapInfo>::const_iterator it = cModel.GetDeviceCapInfo().begin();
        it != cModel.GetDeviceCapInfo().end(); ++it)
    {
        if (it->type == DEVICE_CAP_VIDEOSOURCE)
        {
            DeallocPULicense(cModel.GetManufacturer(), it->license);
            DeallocManagePULicense(it->num);
            break;
        }
    }
}
ENTasErrno AllocManagePULicense(const CDeviceModelInfo &cModel)
{
    if (cModel.GetDeviceTypeId() != DEVICE_TYPE_ENCODER)
    {
        return CMS_SUCCESS;
    }
    for (vector<TDeviceCapInfo>::const_iterator it = cModel.GetDeviceCapInfo().begin();
        it != cModel.GetDeviceCapInfo().end(); ++it)
    {
        if (it->type == DEVICE_CAP_VIDEOSOURCE)
        {
            return AllocManagePULicense(it->num);
        }
    }
    return CMS_SUCCESS;
}
void DeallocManagePULicense(const CDeviceModelInfo &cModel)
{
    for (vector<TDeviceCapInfo>::const_iterator it = cModel.GetDeviceCapInfo().begin();
        it != cModel.GetDeviceCapInfo().end(); ++it)
    {
        if (it->type == DEVICE_CAP_VIDEOSOURCE)
        {
            DeallocManagePULicense(it->num);
            return;
        }
    }
}

bool CreatHostFile()
{
    bool bRet = false;
    FILE *fp = fopen("host.txt", "w+");
    do 
    {
        if ( NULL == fp)
        {
            break;
        }

        string strVal;
        if (!GetMachineNO(strVal))
        {
            break;
        }

        fputs(strVal.c_str(), fp);

        bRet = true;
    } while (false);
    if (NULL != fp)
    {
        fclose(fp);
        fp = NULL;
    }
    
    return bRet;
}

bool GetMachineNO(string &strVal)
{
    s8 achMachineNO[MAX_MAC_LEN] = {0};
    CNetManager cNetMgr;
    if (!cNetMgr.GetMacAddr("eth0", achMachineNO, sizeof(achMachineNO)))
    {
        GLOBELOG(enLogSrcLicense, ERROR_LEV, "Get eth0's mac address failed\n");
        return false;
    }
    else
    {
        strVal = achMachineNO;
        return true;
    }
}

#ifdef WIN32
#define strtok_r(str, sep, lasts) pSepLast=NULL;strtok(str, sep)
#endif // WIN32

CLicenseInst::~CLicenseInst()
{
    //删除Task
    vector<CTask*> vctLicenseServerMngTasks = GetTasksArray("CLicenseServerMngTask");
    if (!vctLicenseServerMngTasks.empty())
    {
        vector<CTask*>::iterator vctLicenseServerMngTasksItem = vctLicenseServerMngTasks.begin();
        while (vctLicenseServerMngTasksItem != vctLicenseServerMngTasks.end())
        {
            CLicenseServerMngTask *pcLicenseServerMngTask = (CLicenseServerMngTask *)(*vctLicenseServerMngTasksItem);
            CMS_ASSERT(0 == strcmp(pcLicenseServerMngTask->GetObjName(), "CLicenseServerMngTask"));

            delete pcLicenseServerMngTask;
            pcLicenseServerMngTask = NULL;

            vctLicenseServerMngTasksItem++;
        }
    }
}

void CLicenseInst::DaemonEntry( CMessage * const pcMsg )
{
	static bool s_bPoweronReadyNtfSent = false;
    switch(pcMsg->event)
    {
    case OSP_POWERON:
        {
            CreatHostFile();
            int status = system("/opt/kdm/system/e2prom");
            if (-1 != status)
            {
                if (WEXITSTATUS(status) != 0)
                {
                    g_bNeedValidateManageLicense = true;
                }
            }
            else
            {
                exit(-1);
            }

            GetAppData().Init();
            //初始化查询task
            CLicenseServerMngTask *pcTask = new CLicenseServerMngTask(this);
            pcTask->InitStateMachine();
            //使用联网验证或本地时都调用
            pcTask->InitSDK();
            NextState(enStateIdle);
            CheckLicenseUsage();
            SetCheckLicenseUsageTimer();
            break;
        }
        //获取到license信息后实际在这里调用checkLicenseKey函数
    case msgLicenseSDKCbReq:
    case TAS_LICENSE_SET_REQ:
        {   
            OnProcSDKMsg(pcMsg);        
        }        
        break;
    case msgLicenseServertatusRefreshReq:
        {
             CInnerLicenseServerStatusRefreshReq *pcReq = *reinterpret_cast<CInnerLicenseServerStatusRefreshReq**>(pcMsg->content);
             CLicenseAppData::TLicenseServerStatus tStatus = (CLicenseAppData::TLicenseServerStatus)pcReq->nMsg;
             GetAppData().UpdateLicenseServerStatus(tStatus);   
        }
    case msgLicenseUsageCheckTimer:
        {
			CCMSTime cCMSTime;
			tm tCurrentTime; 
			tCurrentTime = cCMSTime.GetTM(&tCurrentTime);
			if (!s_bPoweronReadyNtfSent || tCurrentTime.tm_hour >= 2 && tCurrentTime.tm_hour <= 4)
			{
				 CheckLicenseUsage();
			}
            SetCheckLicenseUsageTimer();
        }
        break;
    case msgLicenseUsageGetRsp:
        {
            CInnerLicenseUsageGetRsp *pcRsp = *reinterpret_cast<CInnerLicenseUsageGetRsp**>(pcMsg->content);

            if (pcRsp->GetErrno() == CMS_SUCCESS)
            {
                if (!g_bNeedValidateManageLicense)
                {
                    pcRsp->m_tLicenseUsageInfo.m_dwManagePUNum = 0;
                }
                GetAppData().UpdateLicenseUsage(pcRsp->m_tLicenseUsageInfo);

                if (!s_bPoweronReadyNtfSent)
                {
                    u32 dwModuleId = GetAppID();
                    post(MAKEIID(AID_CCtrl, CInstance::DAEMON), msgPoweronReadyNtf, &dwModuleId, sizeof(dwModuleId));

                    s_bPoweronReadyNtfSent = true;
                }
            }
            else
            {
                // 失败了立即重试
                SetCheckLicenseUsageTimer(1000*5);
            }

            delete pcRsp;
        }
        break;
    case msgDump:
        GetAppData().Dump(*reinterpret_cast<u32*>(pcMsg->content));
        break;
    case TAS_LICENSE_UPDATE_REQ:
        UpdateLicenseFile(reinterpret_cast<COspSipMsg* const>(pcMsg->content), &GetAppData());
        break;
    default:
        break;
    }
}
LPCSTR CLicenseInst::GetStrState() const
{
    switch (CurState())
    {
    case enStateIdle:
    return "enStateIdle";
    default:
    return "未知状态";
    }

}

CLicenseAppData& CLicenseInst::GetAppData()
{
    return g_cLicenseApp.GetAppData();
}


//处理license sdk相关消息，都由m_pcLicenseServerMngTask处理
void CLicenseInst::OnProcSDKMsg(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_OK;
    CLicenseServerMngTask *pcTask = NULL;
    vector<CTask*> vctGbsAgents = GetTasksArray("CLicenseServerMngTask");
    if (!vctGbsAgents.empty())
    {
        if (vctGbsAgents.size() == 1)
        {
            pcTask = static_cast<CLicenseServerMngTask *>(*vctGbsAgents.begin());
        }
        else
        {
            CLASSLOG(enLogSrcLicense, ERROR_LEV, "Too Many Agent tasks[%u] Fail\n", pcMsg->event);
        }
    }

    if (NULL == pcTask)
    {
        CLASSLOG(enLogSrcLicense, ERROR_LEV, "CLicenseServerMngTask Not Exist!\n");
    }

    dwRet = pcTask->ProcMsg(pcMsg);
    
    if (PROCMSG_FAIL == dwRet)
    {
        CLASSLOG(enLogSrcLicense, ERROR_LEV, "CLicenseServerMngTask ProcMsg[%u] Fail\n", pcMsg->event);
    }
    else if (PROCMSG_OK == dwRet)
    {
        CLASSLOG(enLogSrcLicense, EVENT_LEV, "CLicenseServerMngTask ProcMsg[%u] OK\n", pcMsg->event);
    }
    else
    {
        CLASSLOG(enLogSrcLicense, ERROR_LEV, "Unknown Message [%u]\n", pcMsg->event);       
    }
}


void CLicenseInst::CheckLicenseKey(CLicenseAppData *pcData)
{
    //zx 修改，把检测license的步骤拆分为更新和检测，更新放在task中，检测也在task中调用

    g_enLicenseValidateResult = pcData->ValidateLicense();

    static bool s_bKeyDisconnectionAlarmed = false;
    if (!pcData->IsLicenseKeyOk())
    {
        KeyDisconnectionAlarm(true);
        s_bKeyDisconnectionAlarmed = true;
    }
    else
    {
        if (s_bKeyDisconnectionAlarmed)
        {
            KeyDisconnectionAlarm(false);
            s_bKeyDisconnectionAlarmed = false;
        }
    }

    static bool s_bKeyExpireRemindAlarmed = false;
    if (!pcData->IsLicenseKeyOk())
    {
        return;
    }

    CCMSTime cTimeNow;
    CCMSTime cExpireTime(pcData->GetLicenseKey().m_tExpireDate);
    CCMSTime cExpireTimeNew(cExpireTime);
    cExpireTimeNew += -3600*24*7;
    if (cTimeNow < cExpireTime  && cExpireTimeNew < cTimeNow)
    {
        KeyExpireRemindAlarm(true);
        s_bKeyExpireRemindAlarmed = true;
    }
    else
    {
        if (s_bKeyExpireRemindAlarmed)
        {
            KeyExpireRemindAlarm(false);
            s_bKeyExpireRemindAlarmed = false;
        }
    }

    static bool s_bKeyValidateNotpassAlarmed = false;
    if (g_enLicenseValidateResult != CMS_SUCCESS)
    {
        if (pcData->IsLicenseKeyOk())
        {
            KeyValidateNotpass(true);
            s_bKeyValidateNotpassAlarmed = true;
        }
    }
    else
    {
        if (s_bKeyValidateNotpassAlarmed)
        {
            KeyValidateNotpass(false);
            s_bKeyValidateNotpassAlarmed = false;
        }
    }
}

u16 CLicenseInst::GetLicenseValueFromBuf(const string &strBuf, const s8 *const pchKey, s8 *pchValue, s32 &nValueLen)
{
	//查找到Key的位置
	string::size_type nTmp = strBuf.find(pchKey);

	if (string::npos != nTmp)
	{
		//从Key的位置开始查找Key-Value的分隔符，获得Value的起始位置
		string::size_type nStartPos = strBuf.find(KEY_VALUE_CONNECTOR, nTmp) + strlen(KEY_VALUE_CONNECTOR);
		if (string::npos == nStartPos)
		{
			return ERR_LICENSE_FORMAT_FAILED;
		}

		//从Value的起始位置开始查找Key-Value对的分隔符，获得Value的结束位置
		string::size_type nEndPos   = strBuf.find(KEY_VALUE_SEPARETE, nStartPos);
		if (string::npos == nEndPos)
		{
			return ERR_LICENSE_FORMAT_FAILED;
		}

		//获取Key对应的Value
		string strValue = strBuf.substr(nStartPos, nEndPos - nStartPos);

		//将取到的Value传回调用者
		strncpy(pchValue, strValue.c_str(), nValueLen);

		//确保返回的是一个C字符串
		pchValue[nValueLen-1] = '\0';

		//返回Value的长度
		nValueLen = strValue.length();

		//成功
		return SUCCESS_LICENSE;
	}
	else
	{
		//格式不对
		return ERR_LICENSE_FORMAT_FAILED;
	}
}

bool CLicenseInst::ReadLicenseContent(TLicenceInfo &tLicense, CLicenseAppData::TKeyType &tKeyType, const string &strFileKey, bool bVerifyMacNo)
{
    string  strMD5Value;
	s8 szBuf[1024 * 10] = {'\0'};

    s32 nRet = 0;
#ifdef _LINUX_
    static bool s_bGetlicensetoolValidatedNotpassAlarmed = false;
    nRet = MD5Sum(APP_GET_LICENSE, strMD5Value); 
    if ( 0 != nRet || 0 != strcmp(LICENSE_GETLICENSEINFO_MD5SUM,strMD5Value.c_str())) 
    {  
        KeyLicenseGetFileValidateNotpass(true);
        s_bGetlicensetoolValidatedNotpassAlarmed = true;
        CLASSLOG(enLogSrcLicense, ERROR_LEV, "Invlid getlicenseinfo file, wrong md5sum[%s]!\n", strMD5Value.c_str()); 
        return false;
    }
    else
    {
        if (s_bGetlicensetoolValidatedNotpassAlarmed)
        {
        KeyLicenseGetFileValidateNotpass(false);
            s_bGetlicensetoolValidatedNotpassAlarmed = false;
        }
    }
#endif
	//调用getlicenseinfo读取License信息
	if (strFileKey.empty())
	{
		nRet = PGets(szBuf, sizeof(szBuf), "%s", APP_GET_LICENSE);
	}
	else
	{
		nRet = PGets(szBuf, sizeof(szBuf), "%s -f %s", APP_GET_LICENSE, strFileKey.c_str());
	}
	
    if (0 != nRet)
	{
		CLASSLOG(enLogSrcLicense, WARNING_LEV, "Execute getkeyinfo failed, errno=%u, nRet=%d, szBuf:[%s]\n", 
			(u32)errno, nRet, szBuf);
		return false;
	}

	//读取License成功，相关数据已经传入szBuf
	string strResult(szBuf);

	s8 achTmp[1024];
	s32 nLicenceLen;
	CCMSTime cTime;

    u16 wRet;
	nLicenceLen = sizeof(achTmp);
	wRet = GetLicenseValueFromBuf(strResult, "KEY_TYPE", achTmp, nLicenceLen);
	if (SUCCESS_LICENSE != wRet)
	{
		CLASSLOG(enLogSrcLicense, WARNING_LEV, "GetValue KEY_TYPE failed, RetVal=%u\n", (u32)wRet);
		return false;
	}

	//读取Key类型成功，设置Key类型
	tKeyType = (CLicenseAppData::TKeyType)(atoi(achTmp));

	nLicenceLen = sizeof(achTmp);
	wRet = GetLicenseValueFromBuf(strResult, "KEY_RESULT", achTmp, nLicenceLen);
	if (SUCCESS_LICENSE != wRet)
	{
		CLASSLOG(enLogSrcLicense, WARNING_LEV, "GetValue KEY_RESULT failed, RetVal=%u\n", (u32)wRet);
		return false;
	}

	//检查结果是否成功
	if (0 != strcmp("READ_LICENSE_SUCCESS", achTmp))
	{
		CLASSLOG(enLogSrcLicense, WARNING_LEV, "ReadLicense failed, incorrect field=%s\n", achTmp);
		return false;
	}

    nLicenceLen = sizeof(achTmp);
    wRet = GetLicenseValueFromBuf(strResult, "KEY_LICENSE_TYPE", achTmp, nLicenceLen);
    if (SUCCESS_LICENSE != wRet)
    {
        CLASSLOG(enLogSrcLicense, WARNING_LEV, "GetValue KEY_LICENSE_TYPE failed, RetVal=%u\n", (u32)wRet);
        return false;
    }
    if (strcmp(achTmp, "KDMAAA") != 0)
    {
        achTmp[sizeof(achTmp)-1] = '\0';
        CLASSLOG(enLogSrcLicense, WARNING_LEV, "KEY_LICENSE_TYPE wrong value:%s\n", achTmp);
        return false;
    }

    if (bVerifyMacNo&&CLicenseAppData::enFileKey == tKeyType)
    {
        nLicenceLen = sizeof(achTmp);
        wRet = GetLicenseValueFromBuf(strResult, "KEY_LICENSE_MACHINENO", achTmp, nLicenceLen);
        if (SUCCESS_LICENSE != wRet)
        {
            CLASSLOG(enLogSrcLicense, WARNING_LEV, "GetValue KEY_LICENSE_MACHINENO Failed, RetVal=%u\n", (u32)wRet);
            return false;
        }

        string strMachineNO;
        if (!GetMachineNO(strMachineNO))
        {
            return false;
        }
        
        if (memcmp(strMachineNO.c_str(), achTmp, MAX_MAC_LEN) != 0)
        {
            achTmp[sizeof(achTmp)-1] = '\0';
            CLASSLOG(enLogSrcLicense, WARNING_LEV, "KEY_LICENSE_MACHINENO wrong value:%s\n", achTmp);
            return false;
        }

        tLicense.m_strHostId = achTmp;
    }

    nLicenceLen = sizeof(achTmp);
    wRet = GetLicenseValueFromBuf(strResult, "KEY_LICENSE_KEY", achTmp, nLicenceLen);
    if (SUCCESS_LICENSE != wRet)
    {
        CLASSLOG(enLogSrcLicense, WARNING_LEV, "GetValue KEY_LICENSE_KEY failed, RetVal=%u\n", (u32)wRet);
        return false;
    }
    tLicense.m_strLicenseId = achTmp;

    nLicenceLen = sizeof(achTmp);
    wRet = GetLicenseValueFromBuf(strResult, "KEY_LICENSE_VERSION", achTmp, nLicenceLen);
    if (SUCCESS_LICENSE != wRet)
    {
        CLASSLOG(enLogSrcLicense, WARNING_LEV, "GetValue KEY_LICENSE_VERSION fail, RetVal=%u\n", (u32)wRet);
        return false;
    }
    tLicense.m_strVersion = achTmp;

    nLicenceLen = sizeof(achTmp);
    wRet = GetLicenseValueFromBuf(strResult, "KEY_LICENSE_MAX_VIDEO_NUM", achTmp, nLicenceLen);
    if (SUCCESS_LICENSE != wRet)
    {
        CLASSLOG(enLogSrcLicense, WARNING_LEV, "GetValue KEY_LICENSE_MAX_VIDEO_NUM fail, RetVal=%u\n", (u32)wRet);
        tLicense.m_dwTotalVideoMaxNum = 0;
    }
    else
    {
        tLicense.m_dwTotalVideoMaxNum = atoi(achTmp);
    }

    nLicenceLen = sizeof(achTmp);
    wRet = GetLicenseValueFromBuf(strResult, "KEY_LICENSE_MAX_ENCODER_CHANNEL_NUM", achTmp, nLicenceLen);
    if (SUCCESS_LICENSE != wRet)
    {
        CLASSLOG(enLogSrcLicense, WARNING_LEV, "GetValue KEY_LICENSE_MAX_ENCODER_CHANNEL_NUM fail, RetVal=%u\n", (u32)wRet);
        tLicense.m_dwTotalPUMaxNum = 0;
    }
    else
    {
        tLicense.m_dwTotalPUMaxNum = atoi(achTmp);
    }

    nLicenceLen = sizeof(achTmp);
    wRet = GetLicenseValueFromBuf(strResult, "KEY_LICENSE_MAX_RECORD_NUM", achTmp, nLicenceLen);
    if (SUCCESS_LICENSE != wRet)
    {
        CLASSLOG(enLogSrcLicense, WARNING_LEV, "GetValue KEY_LICENSE_MAX_RECORD_NUM fail, RetVal=%u\n", (u32)wRet);
        tLicense.m_dwTotalRecMaxNum = 0;
    }
    else
    {
        tLicense.m_dwTotalRecMaxNum = atoi(achTmp);
    }

    nLicenceLen = sizeof(achTmp);
    wRet = GetLicenseValueFromBuf(strResult, "KEY_LICENSE_MAX_INTELLIGENCE_NUM", achTmp, nLicenceLen);
    if (SUCCESS_LICENSE != wRet)
    {
        CLASSLOG(enLogSrcLicense, WARNING_LEV, "GetValue KEY_LICENSE_MAX_INTELLIGENCE_NUM fail, RetVal=%u\n", (u32)wRet);
        tLicense.m_dwIntelligenceNum = 0;
    }
    else
    {
        tLicense.m_dwIntelligenceNum = atoi(achTmp); 
    }

    u32 dwYear, dwMonth, dwDay;
    nLicenceLen = sizeof(achTmp);
    wRet = GetLicenseValueFromBuf(strResult, "KEY_LICENSE_CREATE_DATE", achTmp, nLicenceLen);
    if (SUCCESS_LICENSE != wRet)
    {
        CLASSLOG(enLogSrcLicense, EVENT_LEV, "GetValue KEY_LICENSE_CREATE_DATE fail, RetVal=%u\n", (u32)wRet);
        tLicense.m_tCreateDate = time(NULL);
    }
    else
    {
        sscanf(achTmp, "%u-%u-%u", &dwYear, &dwMonth, &dwDay);
        cTime.SetLocalTime(dwYear, dwMonth, dwDay, 0, 0, 0);
        tLicense.m_tCreateDate = cTime.GetTime();
    }

    nLicenceLen = sizeof(achTmp);
    wRet = GetLicenseValueFromBuf(strResult, "KEY_LICENSE_MAX_DATE", achTmp, nLicenceLen);
    if (SUCCESS_LICENSE != wRet)
    {
        CLASSLOG(enLogSrcLicense, WARNING_LEV, "GetValue KEY_LICENSE_MAX_DATE fail, RetVal=%u\n", (u32)wRet);
        return false;
    }
    else
    {
        sscanf(achTmp, "%u-%u-%u", &dwYear, &dwMonth, &dwDay);
        cTime.SetLocalTime(dwYear, dwMonth, dwDay, 0, 0, 0);
        tLicense.m_tExpireDate = cTime.GetTime();
    }

    tLicense.m_dwTotalNRUMaxNum = 0x7FFFFFFF;

    //是否允许外厂商的PU直接接入
    nLicenceLen = sizeof(achTmp);
    wRet = GetLicenseValueFromBuf(strResult, "KEY_LICENSE_OTHER_ENCODER_DIRECT", achTmp, nLicenceLen);
    if (SUCCESS_LICENSE == wRet)
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
        CLASSLOG(enLogSrcLicense, ALL_LEV, "GetValue KEY_LICENSE_OTHER_ENCODER_DIRECT failed, RetVal=%u\n", (u32)wRet);
        // 未成功读取时表示无此限制
        tLicense.m_bEnableOtherManufactureDirectAccess = true;
    }
    //外厂商PU允许接入数目信息
    nLicenceLen = sizeof(achTmp);
    wRet = GetLicenseValueFromBuf(strResult, "KEY_LICENSE_MAX_OTHER_ENCODER_NUM", achTmp, nLicenceLen);
    if (SUCCESS_LICENSE != wRet)
    {
        CLASSLOG(enLogSrcLicense, WARNING_LEV, "GetValue KEY_LICENSE_MAX_OTHER_ENCODER_NUM fail, RetVal=%u\n", (u32)wRet);
        tLicense.m_dwOtherPUNum = 0;
    }
    else
    {
        tLicense.m_dwOtherPUNum = atoi(achTmp);
    }

    nLicenceLen = sizeof(achTmp);
    wRet = GetLicenseValueFromBuf(strResult, "KEY_LICENSE_MAX_OTHER_ENCODER_CHANNEL", achTmp, nLicenceLen);
    if (SUCCESS_LICENSE == wRet)
    {
        if (static_cast<u32>(nLicenceLen) >= sizeof(achTmp))
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
        while(true) 
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
        CLASSLOG(enLogSrcLicense, ALL_LEV, "GetValue KEY_LICENSE_MAX_OTHER_ENCODER_CHANNEL fail, RetVal=%u\n", (u32)wRet);
        // 未成功读取时什么也不做
    }

    nLicenceLen = sizeof(achTmp);
    wRet = GetLicenseValueFromBuf(strResult, "KEY_LICENSE_MAX_MANAGE_CHANNEL_NUM", achTmp, nLicenceLen);
    if (SUCCESS_LICENSE == wRet)
    {
        tLicense.m_dwManagePUMaxNum = atoi(achTmp);
    }
    nLicenceLen = sizeof(achTmp);
    wRet = GetLicenseValueFromBuf(strResult, "KEY_LICENSE_MAX_OTHER_STORAGE_SIZE", achTmp, nLicenceLen);
    if (SUCCESS_LICENSE == wRet)
    {
        tLicense.m_dwOtherStorageMaxSize = atoi(achTmp);
    }
    return true;
}

void CLicenseInst::CheckLicenseUsage()
{
    PostTask2DbPool(msgLicenseUsageGetReq, NULL);
}


void CLicenseInst::SetCheckLicenseUsageTimer(u32 dwTimerInterval)
{
    u32 dwIntervalTmp = CTasConfig::GetLicenseUsageInterval()*1000;
    SetTimer(msgLicenseUsageCheckTimer, (dwTimerInterval == 0)? dwIntervalTmp: dwTimerInterval);
}

void CLicenseInst::PostMsg2CCtrl( u32 dwEventId, void* pContent, u32 dwLen )
{
    post(MAKEIID(AID_CCtrl, CInstance::DAEMON), static_cast<u16>(dwEventId), pContent, static_cast<u16>(dwLen));
}

void CLicenseInst::UpdateLicenseFile( COspSipMsg* const pcSipMsg, CLicenseAppData *pcData )
{
    CLicenseUpdateReq cReq;
    pcSipMsg->GetMsgBody(cReq);
    CLicenseUpdateRsp cRsp;
    cRsp.SetSession(cReq.GetSession());
    cRsp.SetSeqNum(cReq.GetSeqNum());
    cRsp.SetErrorCode(ERR_TAS_LICENSE_KEY_INVALID_LICENSE_FILE);

    do 
    {
		// 读取新Key的信息
        string strNewKeyFile("/opt/kdm/cpconf/");
        strNewKeyFile.append(FILE_KEY_NAME);
		TLicenceInfo tNewLicense;
		CLicenseAppData::TKeyType tNewKeyType;


		if (pcData->GetLicenseKeyType()==CLicenseAppData::enUsbKey)
		{
			// usb key不用校验机器码 [7/8/2015 pangubing]
			if (!ReadLicenseContent(tNewLicense, tNewKeyType, strNewKeyFile,false))
			{
				TelePrint("read or validate new license failed, path:[%s]\n", strNewKeyFile.c_str());
				break;
			}
		} 
		else
		{
			if (!ReadLicenseContent(tNewLicense, tNewKeyType, strNewKeyFile))
			{
				TelePrint("read or validate new license failed, path:[%s]\n", strNewKeyFile.c_str());
				break;
			}
		}


		// 检查新Key中最大pu接入点数是否大于外厂商监控点数
		if (tNewLicense.m_dwOtherPUNum > tNewLicense.m_dwTotalPUMaxNum)
		{
			TelePrint("正在上传的License中外厂商最大监控点数[%d]大于允许的PU最大接入数[%d]!", tNewLicense.m_dwOtherPUNum, tNewLicense.m_dwTotalPUMaxNum);
			cRsp.SetErrorCode(ERR_TAS_LICENSE_OTHER_MORE_THAN_ALL);
			break;
		}

		// 读取原Key的信息
		TLicenceInfo tOldLicense;
		CLicenseAppData::TKeyType tOldKeyType = CLicenseAppData::enInvalidKey;
		s32 nOldLicenseRet = ReadLicenseContent(tOldLicense, tOldKeyType);

		CLASSLOG(enLogSrcLicense, WARNING_LEV, "读取原license信息，nRet[%d], tOldKeyType[%d]\n", 
			nOldLicenseRet, tOldKeyType);

		string strBakFileKey("./");
		strBakFileKey.append(FILE_KEY_NAME);
		strBakFileKey.append(".bak");

		string strRealFileKey("./");
		strRealFileKey.append(FILE_KEY_NAME);

		if (tOldKeyType != CLicenseAppData::enUsbKey)			// FileKey or InvalidKey
		{
			if (IsFileExist(strRealFileKey))			// 存在着FileKey，本次操作是扩容原有的FileKey
			{
				if (0 != rename(strRealFileKey.c_str(), strBakFileKey.c_str()))
				{
					TelePrint("backup license file failed, errno=%d, [%s --> %s]", errno, strRealFileKey.c_str(), strBakFileKey.c_str());
					break;
				}
			}

			s32 nCmdRet = ExecCmd("/bin/mv %s %s", strNewKeyFile.c_str(), strRealFileKey.c_str());
			if (-1 == nCmdRet)
			{
				TelePrint("save license file failed, errno=%d, [%s --> %s]", errno, strNewKeyFile.c_str(), strRealFileKey.c_str());
				break;
			}
			if (0 != nCmdRet)
			{
				TelePrint("save license file failed, shellRet=%d, [%s --> %s]", nCmdRet, strNewKeyFile.c_str(), strRealFileKey.c_str());
				break;
			}
		}
		else													// UsbKey
		{
			if (tNewLicense.m_strLicenseId != tOldLicense.m_strLicenseId)		// 扩容原有的UsbKey，需要校验许可证ID
			{
				TelePrint("扩容UsbKey时，新FileKey[%s]和原UsbKey[%s]的许可证ID不同!", 
					tNewLicense.m_strLicenseId.c_str(), tOldLicense.m_strLicenseId.c_str());
				cRsp.SetErrorCode(ERR_TAS_LICENSE_ID_DIFF);
				break;
			}      

			cRsp.SetErrorCode(WriteUsbKey(strNewKeyFile));
			if (cRsp.GetErrorCode() != CMS_SUCCESS)
			{
				TelePrint("write usb key failed, errorcode=%d", cRsp.GetErrorCode());
				break;
			}

            ExecCmd("/bin/rm -f %s", strNewKeyFile.c_str());
		}

        cRsp.SetErrorCode(CMS_SUCCESS);
        CheckLicenseKey(pcData);
    } while (false);

    COspSipMsg cSipRspMsg;
    cSipRspMsg.SetSipTransID(pcSipMsg->GetSipTransID());
    string strMsgBody = cRsp.ToXml();
    cSipRspMsg.SetMsgBody(strMsgBody.c_str(), strMsgBody.size());
    OspPost(MAKEIID(AID_CCtrl, CInstance::DAEMON), cRsp.GetEventId(), cSipRspMsg.GetMsgBuf(), cSipRspMsg.GetMsgLen());
}


s32 CLicenseInst::WriteUsbKey( const string &strKeyPath )
{
#ifndef _LINUX_
    return ERR_TAS_LICENSE_WRITE_USB_KEY_ERROR;
#endif

    char szBuf[100];
    return (PGets(szBuf, sizeof(szBuf), "./update_usbkey %s", strKeyPath.c_str())==0)?CMS_SUCCESS:ERR_TAS_LICENSE_WRITE_USB_KEY_ERROR;
}

void CLicenseAppData::Adapt2901OspParameter(const string& str2901DevType)
{
#ifdef _LINUX_
    s8 szBuf[1024];
    sprintf(szBuf, "%s %s", ADAPT_2901_SAN_KDSIP_VALUE_TOOL, str2901DevType.c_str()); 
    s32 nRet = system(szBuf); 
    if ( nRet != 0)
    {
        TelePrint("adapt kdsip value for 2901SAN failed!, errno=%d, Ret=%d", errno,nRet);
        return;
    }
    system("sync;reboot");
#endif
    return; 
}
void CLicenseAppData::UpdateLicenseKey( bool bOK, TKeyType tKeyType, const TLicenceInfo &tInfo )
{
    OspSemTake(m_hLicenseInfoLock);

    if (bOK)
    {
		CLASSLOG(enLogSrcLicense, EVENT_LEV, "Read licensekey successfully.\n");
        if (tInfo.m_dwOtherStorageMaxSize != m_tLicenseInfo.m_dwOtherStorageMaxSize)
        {
            CLicenseUpdateNtfReq cNtfReq;
            cNtfReq.SetOtherStorageMaxSize(tInfo.m_dwOtherStorageMaxSize);
            string strMsg = cNtfReq.ToXml();
            OspPost(MAKEIID(AID_CCtrl, CInstance::DAEMON), 
                msgLicenseUpdateNtfReq, strMsg.c_str(), strlen(strMsg.c_str())+1);
        }
        m_bLicenseKeyOK = true;
        m_dwLicenseKeyErrTimes = 0;
        m_tLicenseKeyType = tKeyType;
        m_tLicenseInfo = tInfo;
        if (g_bNeedValidateManageLicense  && tInfo.m_dwManagePUMaxNum != CTasConfig::GetManagePuMaxNum() && tInfo.m_dwManagePUMaxNum!=0) 
        {
            TelePrint("will change MangePuMaxNum from %d to %d", CTasConfig::GetManagePuMaxNum(), tInfo.m_dwManagePUMaxNum);
            CTasConfig::SetManagePuMaxNum(tInfo.m_dwManagePUMaxNum );
            string str2901DevType = GetMachineTypeByPUMaxNum(tInfo.m_dwManagePUMaxNum);
            TelePrint("will change 2901DevType to %s", str2901DevType.c_str());       
            if (str2901DevType.empty()) 
            {
                Adapt2901OspParameter(MACHINE_TYPE_2901_G2_S);
            }
            else
            {
                Adapt2901OspParameter(str2901DevType);
            }
        }       
    }
    else
    {
        if ((++m_dwLicenseKeyErrTimes) > 3)
        {
            m_bLicenseKeyOK = false;
            m_tLicenseKeyType = enInvalidKey;
            TLicenceInfo tEmpty;
            m_tLicenseInfo = tEmpty;
            CLicenseUpdateNtfReq cNtfReq;
            cNtfReq.SetOtherStorageMaxSize(0);
            string strMsg = cNtfReq.ToXml();
            OspPost(MAKEIID(AID_CCtrl, CInstance::DAEMON), 
                msgLicenseUpdateNtfReq, strMsg.c_str(), strlen(strMsg.c_str())+1);
        }
		CLASSLOG(enLogSrcLicense, ERROR_LEV, "Failed to read licensekey for %u consecutive times! Tas will refuse to server when it is more than 3 times.\n", m_dwLicenseKeyErrTimes);
    }

    OspSemGive(m_hLicenseInfoLock);
}

extern int volatile g_g300_del_task_num;
static int s_databaseHasMoreLicenseCount=0; // 为了防止退网期间检测误报引起重启，连接几次都超标才重启
void CLicenseAppData::UpdateLicenseUsage( const TLicenseUsageInfo &tInfo )
{
    if (!m_bCurrentUsageInit)
    {
        OspSemTake(m_hLicenseInfoLock);
        m_tCurrentUsage = tInfo;
        m_bCurrentUsageInit = true;
        OspSemGive(m_hLicenseInfoLock);
    }
    else
    {
        if (g_g300_del_task_num > 0) return; // 大量退网期间比较无效，直接跳过

        // 简单比较使用情况，没有超标即可。否则数据库中的数据与3AS之间不匹配，重启3AS
        if (m_tCurrentUsage.IsNotLessLicense(tInfo))
        {
            s_databaseHasMoreLicenseCount=0;
        }
        else
        {
            if (++s_databaseHasMoreLicenseCount >= 3)
            {
                CLASSLOG(enLogSrcLicense, ERROR_LEV, "LicenseUsage is more in the database, tas will reboot.\n");
                TelePrint("LicenseUsage is more in the database, tas will reboot.");
                TasReboot();
            }
        }
    }
}
void CLicenseAppData::UpdateLicenseServerStatus(TLicenseServerStatus tLicenseServerStatus)
{
    m_tLicenseServerStatus = tLicenseServerStatus;
}
u32 CLicenseAppData::GetLicenseServerStatus() const 
{
     return m_tLicenseServerStatus;
}

ENTasErrno CLicenseAppData::ValidateLicense()
{
    ENTasErrno enErrno = CMS_SUCCESS;
    OspSemTake(m_hLicenseInfoLock);
    do 
    {
        if (!m_bLicenseKeyOK)
        {
            enErrno = ERR_TAS_LICENSE_KEY_ERROR;
            break;
        }

        time_t tNow = time(NULL);
        if (m_tLicenseInfo.m_tExpireDate < tNow)
        {
            enErrno = ERR_TAS_LICENSE_KEY_EXPIRE;
            break;
        }

        /* pu license总数 */
        if (m_tLicenseInfo.m_dwTotalPUMaxNum < m_tCurrentUsage.m_dwPUNum)
        {
            enErrno = ERR_TAS_LICENSE_NOT_ENOUGH;
            break;
        }

        /* 外厂商具体限制 */
        TLicenceInfo tLicense = m_tLicenseInfo;
        enErrno = ValidateOtherPULicense(tLicense, m_tCurrentUsage);
        if (g_bNeedValidateManageLicense)
        {
            if (m_tLicenseInfo.m_dwManagePUMaxNum < m_tCurrentUsage.m_dwManagePUNum)
            {
                enErrno = ERR_TAS_LICENSE_MANAGE_PU_TOO_MANY;
                break;
            }
        }
    } while (false);
    OspSemGive(m_hLicenseInfoLock);
    return enErrno;
}

void CLicenseAppData::Init()
{
    m_hLicenseInfoLock = NULL;
    if (!OspSemBCreate(&m_hLicenseInfoLock))
    {
        CLASSLOG(enLogSrcLicense, ERROR_LEV, "create m_hLicenseInfoLock failed\n");
    }

    m_bLicenseKeyOK = false;
    m_dwLicenseKeyErrTimes = 0;
    m_tLicenseKeyType = enInvalidKey;
    m_bCurrentUsageInit = false;
    m_tLicenseServerStatus = enLicenseServerUnkownStatus;
}

ENTasErrno CLicenseAppData::ValidateOtherPULicense( TLicenceInfo &tLicense, const TLicenseUsageInfo &tUsage )
{
    ENTasErrno enErrno = CMS_SUCCESS;
    bool bEnableDirectAccess = tLicense.m_bEnableOtherManufactureDirectAccess;

    string strManufacture;
    bool bIsOtherPuDirectAccess;
    u32 dwOtherPULicenseNum = 0;
    TOtherPULicenceInfo tOtherPUInfo;
    for (u32 i = 0; i < tUsage.m_atOtherPULicenseInfos.size(); ++i)
    {
        tOtherPUInfo = tUsage.m_atOtherPULicenseInfos[i];

        strManufacture = tOtherPUInfo.m_strManufacture;
        AdjustManufactureName(strManufacture, &bIsOtherPuDirectAccess);
        if (!bEnableDirectAccess && bIsOtherPuDirectAccess)
        {
            enErrno = ERR_TAS_LICENSE_NO_DIRECT_ACCESS_OTHER_PU;
            break;
        }

        /* 查找 */
        u32 j;
        for (j = 0; j < tLicense.m_atOtherPULicenceInfo.size(); ++j)
        {
            if (tLicense.m_atOtherPULicenceInfo[j].m_strManufacture == strManufacture)
            {
                break;
            }
        }
        if (j >= tLicense.m_atOtherPULicenceInfo.size())
        {
            for (j = 0; j < tLicense.m_atOtherPULicenceInfo.size(); ++j)
            {
                if (tLicense.m_atOtherPULicenceInfo[j].m_strManufacture == "other")
                {
                    break;
                }
            }
        }

        if (j < tLicense.m_atOtherPULicenceInfo.size())
        {
            /* 比较 */
            if (tLicense.m_atOtherPULicenceInfo[j].m_dwLicenseNum >= tOtherPUInfo.m_dwLicenseNum)
            {
                tLicense.m_atOtherPULicenceInfo[j].m_dwLicenseNum -= tOtherPUInfo.m_dwLicenseNum;
            }
            else
            {
                enErrno = ERR_TAS_LICENSE_SOME_OTHER_PU_TOO_MANY;
                break;
            }
        }
        else
        {
            enErrno = ERR_TAS_LICENSE_SOME_OTHER_PU_TOO_MANY;
            break;
        }

        // 累加总数
        dwOtherPULicenseNum += tOtherPUInfo.m_dwLicenseNum;
    }

    if (enErrno == CMS_SUCCESS && dwOtherPULicenseNum > tLicense.m_dwOtherPUNum)
    {
        enErrno = ERR_TAS_LICENSE_OTHER_PU_TOTAL_TOO_MANY;
    }

    return enErrno;
}

ENTasErrno CLicenseAppData::AllocPULicense( const string &strManufacture, u32 dwLicenseNum,const TCreater &tCreater )
{
    ENTasErrno enErr;
    string strNewManufacture = strManufacture;
    bool bIsOtherPuDirectAccess;

    AdjustManufactureName(strNewManufacture, &bIsOtherPuDirectAccess);

    OspSemTake(m_hLicenseInfoLock);
    do 
    {
        if (m_tLicenseInfo.m_dwTotalPUMaxNum < m_tCurrentUsage.m_dwPUNum + dwLicenseNum)
        {
            enErr = ERR_TAS_LICENSE_NOT_ENOUGH;
            break;
        }

        if (strNewManufacture != MANUFACTURE_KEDACOM)
        {
			// 国标下级的外厂商不校验，上级是否允许外厂商接入能力 [5/27/2015 pangubing]
			if (CREATE_BY_GBS_FOREIGN_DOMAIN!=tCreater)
			{
				if (bIsOtherPuDirectAccess  && !m_tLicenseInfo.m_bEnableOtherManufactureDirectAccess)
				{
					enErr = ERR_TAS_LICENSE_NO_DIRECT_ACCESS_OTHER_PU;
					break;
				}
			}

            if (m_tLicenseInfo.m_dwOtherPUNum < m_tCurrentUsage.OtherPUTotalLicense()+dwLicenseNum)
            {
                enErr = ERR_TAS_LICENSE_OTHER_PU_TOTAL_TOO_MANY;
                break;
            }

            if (m_tLicenseInfo.GetSomeOtherPuLimit(strNewManufacture) < 
                m_tCurrentUsage.OtherPULicense(strNewManufacture, m_tLicenseInfo.m_atOtherPULicenceInfo)+dwLicenseNum)
            {
                enErr = ERR_TAS_LICENSE_SOME_OTHER_PU_TOO_MANY;
                break;
            }

            m_tCurrentUsage.AddOtherPULicense(strManufacture, dwLicenseNum);
        }

        m_tCurrentUsage.m_dwPUNum += dwLicenseNum;

        enErr = CMS_SUCCESS;
    } while (false);
    OspSemGive(m_hLicenseInfoLock);

    return enErr;
}

void CLicenseAppData::DeallocPULicense( const string &strManufacture, u32 dwLicenseNum )
{
    OspSemTake(m_hLicenseInfoLock);

    m_tCurrentUsage.m_dwPUNum -= dwLicenseNum;
    if (strManufacture != MANUFACTURE_KEDACOM)
    {
        m_tCurrentUsage.AddOtherPULicense(strManufacture, 0-(s32)dwLicenseNum);
    }
    OspSemGive(m_hLicenseInfoLock);
}

ENTasErrno CLicenseAppData::AllocManagePULicense( u32 dwVideoNum )
{
    if (!g_bNeedValidateManageLicense)
    {
        return CMS_SUCCESS;
    }
    ENTasErrno enErr;
    OspSemTake(m_hLicenseInfoLock);
    do 
    {
        if (m_tLicenseInfo.m_dwManagePUMaxNum < m_tCurrentUsage.m_dwManagePUNum + dwVideoNum)
        {
            enErr = ERR_TAS_LICENSE_MANAGE_PU_TOO_MANY;
            break;
        }
        m_tCurrentUsage.m_dwManagePUNum += dwVideoNum;
        enErr = CMS_SUCCESS;
    } while (false);
    OspSemGive(m_hLicenseInfoLock);
    return enErr;
}
void CLicenseAppData::DeallocManagePULicense( u32 dwVideoNum )
{
    if (!g_bNeedValidateManageLicense)
    {
        return;
    }
    OspSemTake(m_hLicenseInfoLock);
    m_tCurrentUsage.m_dwManagePUNum -= dwVideoNum;
    OspSemGive(m_hLicenseInfoLock);
}
void CLicenseAppData::Dump(u32 dwCode)
{
    OspSemTake(m_hLicenseInfoLock);

    TelePrint("\nlicense dump:=============================================================\n");
    TelePrint("LicenseInfo: PUMaxNum(%u), PURealNum(%u), ErrCode(%d)\n", 
        m_tLicenseInfo.m_dwTotalPUMaxNum, m_tCurrentUsage.m_dwPUNum, g_enLicenseValidateResult);
    if (dwCode > 0)
    {
        TelePrint("license detail:\n");
        TelePrint("LicenseKeyOK:%s, LicenseKeyErrTimes:%u, LicenseKeyType:%u\n",
            m_bLicenseKeyOK?"yes":"no", m_dwLicenseKeyErrTimes, (u32)m_tLicenseKeyType);
        TelePrint("EnableOtherManufactureDirectAccess:%s, OtherPUNum:%u, certain Other manufactuer limit list:\n",
            m_tLicenseInfo.m_bEnableOtherManufactureDirectAccess? "true":"false", m_tLicenseInfo.m_dwOtherPUNum);
        if (m_tLicenseInfo.m_atOtherPULicenceInfo.size() > 0)
        {
            for (vector<TOtherPULicenceInfo>::const_iterator it = m_tLicenseInfo.m_atOtherPULicenceInfo.begin();
                it != m_tLicenseInfo.m_atOtherPULicenceInfo.end(); ++it)
            {
                TelePrint("Manufactuer-limitNum: %s-%u\n", it->m_strManufacture.c_str(), it->m_dwLicenseNum);
            }
        }
        else
        {
            TelePrint("empty...\n");
        }

        TelePrint("\nmanufactuer license usage list:\n");
        if (m_tCurrentUsage.m_atOtherPULicenseInfos.size() > 0)
        {
            for (vector<TOtherPULicenceInfo>::const_iterator it = m_tCurrentUsage.m_atOtherPULicenseInfos.begin();
                it != m_tCurrentUsage.m_atOtherPULicenseInfos.end(); ++it)
            {
                TelePrint("Manufactuer-UsageNum: %s-%u\n", it->m_strManufacture.c_str(), it->m_dwLicenseNum);
            }
        }
        else
        {
            TelePrint("empty...\n");
        }
    }
    TelePrint("NeedValidateManagePuLicense:%s, ManagePUMaxNum:%d, CurrentNum:%d(please ignore it when its value is 0)  OtherStorageMaxSize:%dT.\n", 
        g_bNeedValidateManageLicense?"true":"false", m_tLicenseInfo.m_dwManagePUMaxNum,
        m_tCurrentUsage.m_dwManagePUNum, m_tLicenseInfo.m_dwOtherStorageMaxSize);
    
    TelePrint("=============================================================\n");
    OspSemGive(m_hLicenseInfoLock);
}

TLicenceInfo CLicenseAppData::GetLicenseKey() const
{
    TLicenceInfo tInfo;

    OspSemTake(m_hLicenseInfoLock);
    tInfo = m_tLicenseInfo;
    OspSemGive(m_hLicenseInfoLock);

    return tInfo;
}

TLicenseUsageInfo CLicenseAppData::GetLicenseUsage() const
{
    TLicenseUsageInfo tInfo;

    OspSemTake(m_hLicenseInfoLock);
    tInfo = m_tCurrentUsage;
    OspSemGive(m_hLicenseInfoLock);

    return tInfo;
}

u32 TLicenceInfo::GetSomeOtherPuLimit(const string &strManufacture)
{
    u32 i;
    for (i = 0; i < m_atOtherPULicenceInfo.size(); ++i)
    {
        if (m_atOtherPULicenceInfo[i].m_strManufacture == strManufacture)
        {
            break;
        }
    }

    if (i < m_atOtherPULicenceInfo.size())
    {
        return m_atOtherPULicenceInfo[i].m_dwLicenseNum;
    }
    else
    {
        for (i = 0; i < m_atOtherPULicenceInfo.size(); ++i)
        {
            if (m_atOtherPULicenceInfo[i].m_strManufacture == "other")
            {
                return m_atOtherPULicenceInfo[i].m_dwLicenseNum;
            }
        }

        return 0;
    }
}
