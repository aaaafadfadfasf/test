#include "license_mng_task.h"
#include "license_mng.h"
#include "config_util/tas_config.h"
extern bool g_bNeedValidateManageLicense;

CLicenseMngTask::CLicenseMngTask( CInstExt *pcInst ): CSipTask(pcInst)
{

}

CLicenseMngTask::~CLicenseMngTask()
{

}

void CLicenseMngTask::InitStateMachine()
{
    NextState(enStateServer);

    CStateProc cProc;
    cProc.TimerPoll = &CTask::TimerPollSkip;

    cProc.ProcMsg = (CTask::PFProcMsg)&CLicenseMngTask::OnServer;
    cProc.ProcErrMsg = cProc.ProcMsg;
    AddRuleProc(enStateServer, cProc);
}

u32 CLicenseMngTask::OnServer( CMessage *const pcMsg )
{
    COspSipMsg* const pcSipMsg = reinterpret_cast<COspSipMsg* const>(pcMsg->content);
    if (TAS_LICENSE_UPDATE_REQ == pcMsg->event || TAS_LICENSE_SET_REQ == pcMsg->event)
    {
        OspPost(MAKEIID(AID_License, CInstance::DAEMON), pcMsg->event, pcSipMsg->GetMsgBuf(), pcSipMsg->GetMsgLen());
    }
    else if (TAS_LICENSE_GET_REQ == pcMsg->event)
    {
        CLicenseGetReq cReq;
        pcSipMsg->GetMsgBody(cReq);
        CLicenseGetRsp cRsp;
        cRsp.SetSession(cReq.GetSession());
        cRsp.SetSeqNum(cReq.GetSeqNum());

        const CLicenseAppData &cAppData = static_cast<const CLicenseAppData&>(g_cLicenseApp);
        const TLicenceInfo &tLicenseInfo = cAppData.GetLicenseKey();
        const TLicenseUsageInfo &tLicenseUsage = cAppData.GetLicenseUsage();
        u32 tLicenseServerStatus = cAppData.GetLicenseServerStatus();
        CCMSTime cTm;
        
        cTm.SetTime(tLicenseInfo.m_tExpireDate);
        cRsp.SetExpireDate(cTm.ToString());
        GetMachineNO(cRsp.GetHostId());
        cRsp.SetLicenseId(tLicenseInfo.m_strLicenseId);

        cRsp.SetOtherManufactureNum(tLicenseInfo.m_atOtherPULicenceInfo.size());
        vector<int> vecLimits;
        vector<string> vecManufactuers;
        u32 dwOtherLimits = 0;
        for (vector<TOtherPULicenceInfo>::const_iterator it = tLicenseInfo.m_atOtherPULicenceInfo.begin();
            it != tLicenseInfo.m_atOtherPULicenceInfo.end(); ++it)
        {
            dwOtherLimits += it->m_dwLicenseNum;
            vecLimits.push_back(it->m_dwLicenseNum);
            vecManufactuers.push_back(it->m_strManufacture);
        }

        if (dwOtherLimits < tLicenseInfo.m_dwOtherPUNum)
        {
            vecLimits.push_back(tLicenseInfo.m_dwOtherPUNum-dwOtherLimits);
            vecManufactuers.push_back("other");
            cRsp.SetOtherManufactureNum(cRsp.GetOtherManufactureNum()+1);
        }
        cRsp.SetOtherManufactureLicenseNum(vecLimits);
        cRsp.SetOtherManufacturName(vecManufactuers);

        cRsp.SetProductId(tLicenseInfo.m_strProductId);
        cRsp.SetTotalNRUMaxNum(tLicenseInfo.m_dwTotalNRUMaxNum);
        cRsp.SetTotalPUMaxNum(tLicenseInfo.m_dwTotalPUMaxNum);
        cRsp.SetTotalRecMaxNum(tLicenseInfo.m_dwTotalRecMaxNum);
        cRsp.SetTotalVideoMaxNum(tLicenseInfo.m_dwTotalVideoMaxNum);
        cRsp.SetUsedPUNum(tLicenseUsage.m_dwPUNum);
        cRsp.SetUsedRecNum(0);
        cRsp.SetUserID(tLicenseInfo.m_dwUserID);
        cRsp.SetNeedValidateManageLicense(g_bNeedValidateManageLicense);
        cRsp.SetManagePUMaxNum(tLicenseInfo.m_dwManagePUMaxNum);
        cRsp.SetOtherStorageMaxSize(tLicenseInfo.m_dwOtherStorageMaxSize);
        cRsp.SetUsedManagePUNum(tLicenseUsage.m_dwManagePUNum);

        CLicenseAppData::TKeyType tkeyType = cAppData.GetLicenseKeyType();
        if (tkeyType == CLicenseAppData::enFileKey)
        {
            cRsp.SetLicenseType("FileKey");
        }
        else if (tkeyType == CLicenseAppData::enUsbKey)
        {
            cRsp.SetLicenseType("UsbKey");
        }
        else if (tkeyType == CLicenseAppData::enNetKey)
        {
            cRsp.SetLicenseType("NetKey");
        }
        cRsp.SetLicenseVer(tLicenseInfo.m_strVersion);
        //zxtodo这里要改一下，把enum拿出来好比吧。。
         cRsp.SetAuthenticationMethod(CTasConfig::GetLsKeyType());
        if (CTasConfig::GetLsKeyType() != 0 ) 
        {
            cRsp.SetServerConnectState(tLicenseServerStatus);
            cRsp.SetLicenseServerIpAddr(CTasConfig::GetLsIP()); 
            cRsp.SetLicenseServerPort(CTasConfig::GetLsPort());
        }
        PostMsgRsp(pcSipMsg->GetSipTransID(), cRsp);
    }
    else
    {
        Log(ERROR_LEV, "CLicenseMngTask recved unknown msg, msgid=%d\n", pcMsg->event);
    }
    
    return PROCMSG_DEL;
}