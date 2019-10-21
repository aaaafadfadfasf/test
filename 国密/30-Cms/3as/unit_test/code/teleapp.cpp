#include "teleapp.h"
#include "configfile.h"
#include <fstream>
using std::fstream;
CTeleApp g_cTeleApp;

void CTeleInst::DaemonEntry( CMessage* const pcMsg )
{
    switch (pcMsg->event)
    {
    case OSP_POWERON:
        OnPowerOn();
        break;
    case OSP_PROXY_CONNECT:
        printf("proxy connected\n");
        break;
    case OSP_PROXY_DISCONNECT:
        printf("proxy disconnected\n");
        break;
    case INNER_LOGIN:
        {
            TLoginData *ptLoginData = reinterpret_cast<TLoginData*>(pcMsg->content);
            TasLogin(ptLoginData->loginType, ptLoginData->szName, ptLoginData->szPwd, ptLoginData->szExtra);
        }
        break;
    case TAS_LOGIN_RSP:
        TasLoginRsp(pcMsg);
        break;
    case INNER_LOGOUT:
        TasLogout();
        break;
    case TAS_LOGOUT_RSP:
        {
            CLogoutTasRsp cRsp;
            reinterpret_cast<COspSipMsg*>(pcMsg->content)->GetMsgBody(cRsp);

            TelePrint("logout, result=%d", cRsp.GetErrorCode());
        }
        break;
    case INNER_ONLINE:
        {
            TOnlineInfo *pt = reinterpret_cast<TOnlineInfo*>(pcMsg->content);
            TasOnline(*pt);
        }
        break;
    case TAS_CU_ONLINE_RSP:
        {
            CCuOnlineRsp cRsp;
            reinterpret_cast<COspSipMsg*>(pcMsg->content)->GetMsgBody(cRsp);
            TelePrint("Online result:%d, cuSessid:%s", cRsp.GetErrorCode(), cRsp.GetCuSessionId().c_str());
        }
        break;
    case INNER_OFFLINE:
        {
            TasOffline(reinterpret_cast<const s8*>(pcMsg->content));
        }
        break;
    case TAS_CU_OFFLINE_RSP:
        {
            CCuOfflineRsp cRsp;
            reinterpret_cast<COspSipMsg*>(pcMsg->content)->GetMsgBody(cRsp);
            TelePrint("Offline result:%d. msgbody:%s", cRsp.GetErrorCode(), 
                       reinterpret_cast<COspSipMsg*>(pcMsg->content)->GetMsgBody());
        }
        break;
    case INNER_DEVMODELADD:
        {
            TDevModelAdd *ptInfo = reinterpret_cast<TDevModelAdd*>(pcMsg->content);
            DevModelAddReq(*ptInfo);
        }
        break;
    case TAS_DEVICE_MODEL_ADD_RSP:
        DevModelAddRsp(pcMsg);
        break;
    case INNER_DEVMODELDEL:
        {
            DevModelDelReq(reinterpret_cast<const s8*>(pcMsg->content));
        }
        break;
    case TAS_DEVICE_MODEL_DEL_RSP:
        DevModelDelRsp(pcMsg);
        break;
    case INNER_DEVMODELQRY:
        {
            DevModelQryReq();
        }
        break;
    case TAS_DEVICE_MODEL_QRY_RSP:
        DevModelQryRsp(pcMsg);
        break;
    case TAS_DEVICE_MODEL_QRY_NTF_REQ:
        DevModelQryNtfReq(pcMsg);
        break;
    case INNER_DEVADD:
        {
            TDevAddInfo *ptAddInfo = reinterpret_cast<TDevAddInfo*>(pcMsg->content);
            DevAddReq(*ptAddInfo);
        }
        break;
    case TAS_DEVICE_ADD_RSP:
        DevAddRsp(pcMsg);
        break;
    case INNER_DEVDEL:
        {
            DevDelReq((const s8*)pcMsg->content);
        }
        break;
    case TAS_DEVICE_DEL_RSP:
        DevDelRsp(pcMsg);
        break;
    case INNER_DEVMOD:
        {
            TDevModInfo *ptModInfo = (TDevModInfo*)(pcMsg->content);
            DevModReq(*ptModInfo);
        }
        break;
    case TAS_DEVICE_MOD_RSP:
        DevModRsp(pcMsg);
        break;
    case INNER_DEVQRY:
        {
            TDevQryInfo *ptQryInfo = (TDevQryInfo*)(pcMsg->content);
            DevQryReq(*ptQryInfo);
        }
        break;
    case TAS_DEVICE_QRY_RSP:
        DevQryRsp(pcMsg);
        break;
    case TAS_DEVICE_QRY_NTF_REQ:
        DevQryNtfReq(pcMsg);
        break;

    case INNER_AUTOTEST:
        AutoTest(reinterpret_cast<const s8*>(pcMsg->content));
        break;
    default:
        if (AutoTest(pcMsg)) break;
        DefaultMsgHandler(pcMsg);
        break;
    }
}

void CTeleInst::DefaultMsgHandler( CMessage* const pcMsg )
{
    TelePrint("recv unknown msg, msgid=%d", pcMsg->event);
}

void CTeleInst::OnPowerOn()
{
    m_tTasSipUri.SetURIString(GetTasUri());

    m_tSrcOspTrans.dwIID = MAKEIID(GetAppID(), GetInsID());
    m_tSrcOspTrans.dwTaskID = 0;
    m_tSrcOspTrans.dwSeqNO = 0;
}

#define PostMsgReq(msgObj) \
    msgObj.SetSession(m_strSessId); \
    if (!OspSipPostReq(KDSIP_EVENT_MESSAGE_REQ, msgObj, m_tTasSipUri, m_tSrcOspTrans)) \
    TelePrint("send msg req failed, msgid=%s", msgObj.GetEventStr().c_str())

#define PostMsgRsp(msqreq, msgObj) \
    msgObj.SetSeqNum(msqreq.GetSeqNum()); \
    msgObj.SetSession(m_strSessId); \
    if (!OspSipPostRsp(KDSIP_EVENT_MESSAGE_RSP, reinterpret_cast<COspSipMsg*>(pcMsg->content)->GetSipTransID(), msgObj, m_tSrcOspTrans)) \
    TelePrint("send msg rsp failed, msgid=%s", msgObj.GetEventStr().c_str())

void CTeleInst::TasLogin(s32 logintype, const s8* pszName, const s8* pszPwd, const s8* pszExtra)
{
    if (!(1 <= logintype && logintype <= 3))
    {
        TelePrint("invalid logintype %d", logintype);
        return;
    }

    CLoginTasReq cLoginTasReq;
    cLoginTasReq.SetLoginType(static_cast<CLoginTasReq::TLoginType>(logintype));
    cLoginTasReq.SetUserName(pszName);
    cLoginTasReq.SetPassword(pszPwd);
    cLoginTasReq.SetExtraInfo(pszExtra);

    PostMsgReq(cLoginTasReq);
}

void CTeleInst::TasLoginRsp( CMessage* const pcMsg )
{
    CLoginTasRsp cLoginTasRsp;
    COspSipMsg *pcSip = reinterpret_cast<COspSipMsg*>(pcMsg->content);

    pcSip->GetMsgBody(cLoginTasRsp);
    if (cLoginTasRsp.GetErrorCode() == 0)
    {
        TOspTransID tOspTransId;
        tOspTransId.dwIID = MAKEIID(GetAppID(), GetInsID());
        tOspTransId.dwTaskID = INVALID_TASKNO;
        tOspTransId.dwSeqNO = INVALID_SEQ_NO;
        OspSipSetHBParam(m_tTasSipUri, tOspTransId, NULL, 10, 3);
        m_strSessId = cLoginTasRsp.GetSession();
    }
    TelePrint("login result=%d", cLoginTasRsp.GetErrorCode());
}

void CTeleInst::TasLogout()
{
    CLogoutTasReq cReq;
    PostMsgReq(cReq);
}


void CTeleInst::DevAddReq( const TDevAddInfo &tAddInfo )
{
    CDeviceAddReq cAddReq;
    cAddReq.GetDeviceInfo().SetDeviceName(tAddInfo.szDevName);
    cAddReq.GetDeviceInfo().SetDomainUUID(tAddInfo.szDomainId);
    cAddReq.GetDeviceInfo().SetDeviceModel(tAddInfo.szModelId);
    CCMSTime cTm;
    cTm.SetLocalTime(2014, 1, 10, 0, 0, 0);
    u8 abyData[] = {0xe7, 0xa7, 0x91, 0xe8, 0xbe, 0xbe, 0x0}; // 科达utf8编码数据
    cAddReq.GetDeviceInfo().SetDescription(reinterpret_cast<const s8*>(abyData));
    cAddReq.GetDeviceInfo().SetExpireDate(cTm.GetXSDDateTime());

    string strData = cAddReq.ToXml();
    CDeviceAddReq cAddReqTmp;
    cAddReqTmp.FromXml(strData);
    TelePrint("dev raw_data:%s, convert_data:%s", abyData, cAddReqTmp.GetDeviceInfo().GetDescription().c_str());

    PostMsgReq(cAddReq);
}

void CTeleInst::DevAddRsp( CMessage* const pcMsg )
{
    CDeviceAddRsp cRsp;
    reinterpret_cast<COspSipMsg*>(pcMsg->content)->GetMsgBody(cRsp);
    TelePrint("DevAddRsp result=%d", cRsp.GetErrorCode());
}

void CTeleInst::DevDelReq( const string &strDevId )
{
    CDeviceDelReq cReq;
    cReq.SetDeviceUUID(strDevId);
    PostMsgReq(cReq);
}

void CTeleInst::DevDelRsp( CMessage* const pcMsg )
{
    CDeviceDelRsp cRsp;
    reinterpret_cast<COspSipMsg*>(pcMsg->content)->GetMsgBody(cRsp);
    TelePrint("DevDelRsp result=%d", cRsp.GetErrorCode());
}

void CTeleInst::DevModReq( const TDevModInfo &tModInfo )
{
    CDeviceModReq cReq;
    cReq.SetDeviceUUID(tModInfo.szDevId);
    cReq.SetDeviceName(tModInfo.szDevNewName);
    PostMsgReq(cReq);
}

void CTeleInst::DevModRsp( CMessage* const pcMsg )
{
    CDeviceModRsp cRsp;
    reinterpret_cast<COspSipMsg*>(pcMsg->content)->GetMsgBody(cRsp);
    TelePrint("DevModRsp result=%d", cRsp.GetErrorCode());
}

void CTeleInst::DevQryReq( const TDevQryInfo &tQryInfo )
{
    CDeviceQryReq cReq;
    cReq.SetStartEntryIndex(tQryInfo.nStartIndex);
    cReq.SetExpectEntryNum(tQryInfo.nCount);
    PostMsgReq(cReq);
}

void CTeleInst::DevQryRsp( CMessage* const pcMsg )
{
    CDeviceQryRsp cRsp;
    reinterpret_cast<COspSipMsg*>(pcMsg->content)->GetMsgBody(cRsp);
    TelePrint("DevQryRsp result=%d, count=%d", cRsp.GetErrorCode(), cRsp.GetTotalEntryNum());
}

void CTeleInst::DevQryNtfReq( CMessage* const pcMsg )
{
    CDeviceQryNtfReq cNtfReq;
    reinterpret_cast<COspSipMsg*>(pcMsg->content)->GetMsgBody(cNtfReq);
    CNtfRsp cNtfRsp;
    cNtfRsp.SetEvent(TAS_DEVICE_QRY_NTF_RSP);

    PostMsgRsp(cNtfReq, cNtfRsp);
    for (u32 i = 0; i < cNtfReq.GetDeviceInfo().size(); ++i)
    {
        TelePrint("Device info: devid=%s, devname=%s", 
            cNtfReq.GetDeviceInfo()[i].GetDeviceUUID().c_str(), 
            cNtfReq.GetDeviceInfo()[i].GetDeviceName().c_str());
    }
}

void CTeleInst::DevModelAddReq( const TDevModelAdd &tInfo )
{
    CDeviceModelAddReq cReq;
    CDeviceModelInfo &cModelInfo = cReq.GetDeviceModelInfo();
    cModelInfo.SetDeviceModelName(tInfo.szName);
    cModelInfo.SetDeviceTypeId(tInfo.dwDevType);
    cModelInfo.SetDomainId(tInfo.szDomainId);
    cModelInfo.SetManufacturer("kedacom");
    
    vector<TDeviceCapInfo> &atCaps = cModelInfo.GetDeviceCapInfo();
    if (1 == tInfo.dwDevType)
    {
        TDeviceCapInfo tCap;

        tCap.type = 1;
        tCap.num = 2;
        tCap.license = 2;
        atCaps.push_back(tCap);

        tCap.type = 2;
        tCap.num = 1;
        tCap.license = 0;
        atCaps.push_back(tCap);

        tCap.type = 3;
        tCap.num = 2;
        tCap.license = 0;
        atCaps.push_back(tCap);

        tCap.type = 4;
        tCap.num = 2;
        tCap.license = 0;
        atCaps.push_back(tCap);
    }
    else
    {
        TelePrint("tas_test tool not support typeid...");
        return;
    }

    PostMsgReq(cReq);
}

void CTeleInst::DevModelAddRsp( CMessage* const pcMsg )
{
    CDeviceModelAddRsp cRsp;
    reinterpret_cast<COspSipMsg*>(pcMsg->content)->GetMsgBody(cRsp);
    TelePrint("DevModelAddRsp result=%d", cRsp.GetErrorCode());
}

void CTeleInst::DevModelDelReq( const string &strModelId )
{
    CDeviceModelDelReq cReq;
    cReq.SetDeviceModelId(strModelId);
    PostMsgReq(cReq);
}

void CTeleInst::DevModelDelRsp( CMessage* const pcMsg )
{
    CDeviceModelDelRsp cRsp;
    reinterpret_cast<COspSipMsg*>(pcMsg->content)->GetMsgBody(cRsp);
    TelePrint("DevModelDelRsp result=%d", cRsp.GetErrorCode());
}

void CTeleInst::DevModelQryReq()
{
    CDeviceModelQryReq cReq;
    PostMsgReq(cReq);
}

void CTeleInst::DevModelQryRsp( CMessage* const pcMsg )
{
    CDeviceModelQryRsp cRsp;
    reinterpret_cast<COspSipMsg*>(pcMsg->content)->GetMsgBody(cRsp);
    TelePrint("DevModelQryRsp result=%d", cRsp.GetErrorCode());
}

void CTeleInst::DevModelQryNtfReq( CMessage* const pcMsg )
{
    CDeviceModelQryNtfReq cNtfReq;
    reinterpret_cast<COspSipMsg*>(pcMsg->content)->GetMsgBody(cNtfReq);
    CNtfRsp cNtfRsp;
    cNtfRsp.SetEvent(TAS_DEVICE_MODEL_QRY_NTF_RSP);
    TelePrint("DevModelQryNtfRsp:%s", cNtfRsp.ToXml().c_str());

    PostMsgRsp(cNtfReq, cNtfRsp);
    for (u32 i = 0; i < cNtfReq.GetDeviceModelInfo().size(); ++i)
    {
        TelePrint("DeviceModel info: modelid=%s, modelname=%s", 
            cNtfReq.GetDeviceModelInfo()[i].GetDeviceModelId().c_str(), 
            cNtfReq.GetDeviceModelInfo()[i].GetDeviceModelName().c_str());
    }
}

void CTeleInst::TasOnline( const TOnlineInfo &tInfo )
{
    CCuOnlineReq cReq;
    cReq.SetCuUserName(tInfo.szName);
    cReq.SetCuUserPassword(tInfo.szPwd);
    PostMsgReq(cReq);
}

void CTeleInst::TasOffline( const s8* cstrCuSessid )
{
    CCuOfflineReq cReq;
    cReq.SetCuSessionId(cstrCuSessid);
    PostMsgReq(cReq);
}


void CTeleInst::AutoTest( const s8* cstrfile )
{
    if (NULL == cstrfile)
    {
        TelePrint("invalid argument in autotest");
        return;
    }
    
    s8 szBuff[1024*64];

    FILE *pfile = fopen(cstrfile, "r");
    if (NULL == pfile)
    {
        TelePrint("fopen failed in autotest, errno=%d", errno);
        return;
    }
    do 
    {
        if (NULL == fgets(szBuff, sizeof(szBuff), pfile)) break;
        string strReq = ReplaceSessid(szBuff, m_strSessId);
        TelePrint("autotest req content:%s\n", strReq.c_str());
        
        if (!OspSipPostReq(KDSIP_EVENT_MESSAGE_REQ, strReq, m_tTasSipUri, m_tSrcOspTrans))
            TelePrint("send msg req failed in autotest");
    } while (true);

    fclose(pfile);
}

bool CTeleInst::AutoTest( CMessage* const pcMsg )
{
    COspSipMsg* pcSipMsg = reinterpret_cast<COspSipMsg*>(pcMsg->content);
    if (IsNtfReqType(pcMsg->event))
    {
        TelePrint("QryNtfReq content begin----------------");
        TelePrint("%s", pcSipMsg->GetMsgBody());
        TelePrint("QryNtfReq content end------------------\n");

        CNtfRsp cRsp;
        cRsp.SetSession(m_strSessId);
        cRsp.SetSeqNum(atoi(GetSipSeqNO(pcSipMsg->GetMsgBody()).c_str()));
        cRsp.SetEvent(pcMsg->event+1); // NtfReq对应的NtfRsp消息号大1

        if (!OspSipPostRsp(KDSIP_EVENT_MESSAGE_RSP, pcSipMsg->GetSipTransID(), cRsp, m_tSrcOspTrans))
            TelePrint("send msg rsp failed in autotest");

        return true;
    }
    if (IsRspType(pcMsg->event))
    {
        TelePrint("Rsp Content begin-----------------");
        TelePrint("%s", pcSipMsg->GetMsgBody());
        TelePrint("Rsp Content end-----------------\n");
        return true;
    }

    return false;
}


