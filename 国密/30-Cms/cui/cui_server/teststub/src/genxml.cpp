#include <fstream>
#include <string>
using namespace std;
#include "genxml.h"
#include "fakesvrapp.h"

extern string g_strUserName;
extern string g_strCuiInTasSess;
extern string g_strCuInTasSess;
extern string g_strCuInCmuSess;
extern string g_strCuUserId;
extern string g_strDomainId;
extern string g_strGroupId;
extern string g_strDevURI;
extern string g_strDomainName;
string g_strQueryTag = "f5f142818aa54deb8af3628bc829208b";

template<typename CMsg>
void WriteXml(CMsg &cMsg)
{
    cMsg.SetSession(g_strCuInCmuSess);
    string strFileName = "E:\\phoenix\\30-Cms\\cui\\cui_server\\teststub\\prj_win\\xmlexp\\" + cMsg.GetEventStr() + ".xml";
    std::ofstream out(strFileName);
    std::string str1 = cMsg.ToXml();
    if ( strFileName.find("TAS_") != string::npos )
    {
        str1 = ReplaceSessid(str1.c_str(), g_strCuInTasSess);
    }
    int lenth = strlen(str1.c_str())+1;
    out<<str1;
    out.close();
}

#define WRITEXML(x) do \
{ \
    x cUserQryRsp; \
    WriteXml(cUserQryRsp); \
} while (0)

void GenClassName()
{
    string strFile = "E:\\phoenix\\30-Cms\\cui\\cui_server\\unittest\\prj_win\\proto\\";
    string strFile0 = strFile;strFile0 += "cmu_proto.h";
    string strFile1 = strFile;strFile1 += "tas_proto.h";
    ifstream iRead0(strFile0);
    ifstream iRead1(strFile1);
    string strWrite;

    for ( int i=0;i<2;i++ )
    {
        string Name;
        if ( i == 0 )
        {
            Name = strFile0;
        }
        else if ( i == 1 )
        {
            Name = strFile1;
        }
        ifstream iRead(Name);
        while ( ! iRead.eof() )
        {
            char szBuf[1024] = {0};
            iRead.getline(szBuf, 1024);
            char *pBegin = strstr(szBuf, "class ");
            if ( NULL != pBegin )
            {
                pBegin += strlen("class ");
                char *pEnd = strstr(pBegin, ":");
                *pEnd = '\0';
                strWrite += pBegin;
                strWrite += "\r\n";
            }
        }
        iRead.close();
    }

    
    ofstream iWrite("proto.txt");
    iWrite << strWrite;
    iWrite.close();
}

void GenXml()
{
    WRITEXML(CPuiRegReq );

    WRITEXML(CPuiRegRsp );

    WRITEXML(CPuRegReq );

    WRITEXML(CPuRegRsp );

    WRITEXML(CPuUnRegReq );

    WRITEXML(CPuUnRegRsp );

    WRITEXML(CCuiRegReq );

    WRITEXML(CCuiRegRsp );

    WRITEXML(CCuLoginReq );

    WRITEXML(CCuLoginRsp );

    WRITEXML(CCuLogoutReq );

    WRITEXML(CCuLogoutRsp );

    //WRITEXML(CCuDisconnectNtfReq );

    WRITEXML(CCuDisconnectNtfRsp );

    WRITEXML(CCuGetCuiListReq );

    CCuGetCuiListRsp cCuGetCuiListRsp;
    TNetAddr tCuiAddr;
    string strIp = "172.16.64.95";
    tCuiAddr.SetNetIp(strIp);
    tCuiAddr.SetNetPort(1230);
    cCuGetCuiListRsp.GetIpAddrList().push_back(tCuiAddr);
    WriteXml(cCuGetCuiListRsp);
    //WRITEXML(CCuGetCuiListRsp );

    WRITEXML(CCuGetUserDataReq );

    WRITEXML(CCuGetUserDataRsp);

    WRITEXML(CCuSetUserDataReq);

    WRITEXML(CCuSetUserDataRsp);

    WRITEXML(CPuStatusSsReq);

    WRITEXML(CPuStatusSsRsp);

    WRITEXML(CPuStatusRefreshSsReq);

    WRITEXML(CPuStatusRefreshSsRsp);

    WRITEXML(CPuStatusUnSsReq);

    WRITEXML(CPuStatusUnSsRsp);

    WRITEXML(CPuStatusNtyReq);

    WRITEXML(CPuConfigNtyReq);

    WRITEXML(CPuServiceStatusNtyReq);

    WRITEXML(CPuAlarmStatusNtyReq);

    WRITEXML(CPuPtzCtrlReq);

    WRITEXML(CPuPtzCtrlRsp);

    WRITEXML(CPuSendTransparentDataReq);

    WRITEXML(CPuSendTransparentDataRsp);

    WRITEXML(CPuPtzLockReq);

    WRITEXML(CPuPtzLockRsp);

    WRITEXML(CPuPtzUnlockReq);

    WRITEXML(CPuPtzUnlockRsp);

    WRITEXML(CPuVidEncParamSetReq);

    WRITEXML(CPuVidEncParamSetRsp);

    WRITEXML(CPuVidEncParamGetReq);

    WRITEXML(CPuVidEncParamGetRsp);

    WRITEXML(CPuSaveCfgReq);

    WRITEXML(CPuSaveCfgRsp);

    WRITEXML(CPuSysParamSetReq);

    WRITEXML(CPuSysParamSetRsp);

    WRITEXML(CPuSysParamGetReq);

    WRITEXML(CPuSysParamGetRsp);

    WRITEXML(CPuSendRealMediaReq);

    WRITEXML(CPuSendRealMediaRsp);

    WRITEXML(CPuSendRealMediaACK);

    WRITEXML(CPuStopSendRealMediaReq);

    WRITEXML(CPuRebootReq);

    WRITEXML(CPuRebootRsp);

    WRITEXML(CInviteReq);

    WRITEXML(CInviteRsp);

    WRITEXML(CInviteACK);

    WRITEXML(CByeReq);

    WRITEXML(CByeRsp);

    WRITEXML(CVtduRegReq);

    WRITEXML(CVtduRegRsp);

    WRITEXML(CVtduAddSwitchReq);

    WRITEXML(CVtduAddSwitchRsp);

    WRITEXML(CVtduRemoveSwitchReq);

    WRITEXML(CVtduRemoveSwitchRsp);

    WRITEXML(CVtduPunchResultNtfReq);

    WRITEXML(CVtduPunchResultNtfRsp);

    WRITEXML(CVtduAddrAssignReq);

    WRITEXML(CVtduAddrAssignRsp);

    WRITEXML(CCmuConnectReq);

    WRITEXML(CCmuConnectRsp);

    WRITEXML(CLoginTasReq);

    WRITEXML(CLoginTasRsp);

    WRITEXML(CLogoutTasReq);

    WRITEXML(CLogoutTasRsp);

    WRITEXML(CCuOnlineReq);

    CCuOnlineRsp cCuOnlineRsp;
    cCuOnlineRsp.SetCuSessionId(g_strCuInTasSess);
    WriteXml(cCuOnlineRsp);

    WRITEXML(CCuOfflineReq);

    WRITEXML(CCuOfflineRsp);

    WRITEXML(CDeviceAddReq);

    WRITEXML(CDeviceAddRsp);

    WRITEXML(CDeviceAddNtfReq);

    WRITEXML(CDeviceDelReq);

    WRITEXML(CDeviceDelRsp);

    WRITEXML(CDeviceDelNtfReq);

    WRITEXML(CDeviceModReq);

    WRITEXML(CDeviceModRsp);

    WRITEXML(CDeviceModNtfReq);

    WRITEXML(CDeviceQryReq);

    WRITEXML(CDeviceQryRsp);

    WRITEXML(CDeviceQryNtfReq);

    WRITEXML(CDeviceModelAddReq);

    WRITEXML(CDeviceModelAddRsp);

    WRITEXML(CDeviceModelAddNtfReq);

    WRITEXML(CDeviceModelDelReq);

    WRITEXML(CDeviceModelDelRsp);

    WRITEXML(CDeviceModelDelNtfReq);

    WRITEXML(CDeviceModelQryReq);

    WRITEXML(CDeviceModelQryRsp);

    WRITEXML(CDeviceModelQryNtfReq);

    WRITEXML(CDeviceCapIndexNameGetReq);

    WRITEXML(CDeviceCapIndexNameGetRsp);

    WRITEXML(CDeviceCapIndexNameSetReq);

    WRITEXML(CDeviceCapIndexNameSetRsp);

    WRITEXML(CDeviceCapIndexNameModNtfReq);

    WRITEXML(CDeviceManufacturerAddReq);

    WRITEXML(CDeviceManufacturerAddRsp);

    WRITEXML(CDeviceManufacturerDelReq);

    WRITEXML(CDeviceManufacturerDelRsp);

    WRITEXML(CDeviceManufacturerQryReq);

    WRITEXML(CDeviceManufacturerQryRsp);

    WRITEXML(CDeviceManufacturerQryNtfReq);

    WRITEXML(CUserAddReq);

    WRITEXML(CUserAddRsp);

    WRITEXML(CUserAddNtfReq);

    WRITEXML(CUserDelReq);

    WRITEXML(CUserDelRsp);

    WRITEXML(CUserDelNtfReq);

    WRITEXML(CUserModReq);

    WRITEXML(CUserModRsp);

    WRITEXML(CUserModNtfReq);

    WRITEXML(CUserQryReq);

    CUserQryRsp cUserQryRsp;
    string cUserQryTag = "UserQry";
    cUserQryRsp.SetQueryTag(cUserQryTag);
    cUserQryRsp.SetTotalEntryNum(1);
    WriteXml(cUserQryRsp);
    //WRITEXML(CUserQryRsp);

    CUserQryNtfReq cUserQryNtfReq;
    cUserQryNtfReq.SetEntryNum(1);
    cUserQryNtfReq.SetLastNtf(true);
    cUserQryNtfReq.SetQueryTag(cUserQryTag);
    CUserInfo cUserInfo;
    cUserInfo.SetUserName(g_strUserName);
    cUserInfo.SetUserID("123");
    cUserQryNtfReq.GetUserInfo().push_back(cUserInfo);
    WriteXml(cUserQryNtfReq);
    //WRITEXML(CUserQryNtfReq);

    WRITEXML(CUserDeviceAddReq);

    WRITEXML(CUserDeviceAddRsp);

    WRITEXML(CUserDeviceAddNtfReq);

    WRITEXML(CUserDeviceDelReq);

    WRITEXML(CUserDeviceDelRsp);

    WRITEXML(CUserDeviceDelNtfReq);

    WRITEXML(CUserDeviceModReq);

    WRITEXML(CUserDeviceModRsp);

    WRITEXML(CUserDeviceModNtfReq);

    WRITEXML(CUserDeviceQryReq);

    WRITEXML(CUserDeviceQryRsp);

    WRITEXML(CUserDeviceQryNtfReq);

    WRITEXML(CDeviceGroupAddReq);

    WRITEXML(CDeviceGroupAddRsp);

    WRITEXML(CDeviceGroupAddNtfReq);

    WRITEXML(CDeviceGroupDelReq);

    WRITEXML(CDeviceGroupDelRsp);

    WRITEXML(CDeviceGroupDelNtfReq);

    WRITEXML(CDeviceGroupModReq);

    WRITEXML(CDeviceGroupModRsp);

    WRITEXML(CDeviceGroupModNtfReq);

    WRITEXML(CDeviceGroupQryReq);

    WRITEXML(CDeviceGroupQryRsp);

    WRITEXML(CDeviceGroupQryNtfReq);

    WRITEXML(CDeviceGroupDeviceAddReq);

    WRITEXML(CDeviceGroupDeviceAddRsp);

    WRITEXML(CDeviceGroupDeviceAddNtfReq);

    WRITEXML(CDeviceGroupDeviceDelReq);

    WRITEXML(CDeviceGroupDeviceDelRsp);

    WRITEXML(CDeviceGroupDeviceDelNtfReq);

    WRITEXML(CDeviceGroupDeviceModReq);

    WRITEXML(CDeviceGroupDeviceModRsp);

    WRITEXML(CDeviceGroupDeviceModNtfReq);

    WRITEXML(CDeviceGroupDeviceQryReq);

    WRITEXML(CDeviceGroupDeviceQryRsp);

    WRITEXML(CDeviceGroupDeviceQryNtfReq);

    WRITEXML(CUserGroupAddReq);

    WRITEXML(CUserGroupAddRsp);

    WRITEXML(CUserGroupAddNtfReq);

    WRITEXML(CUserGroupDelReq);

    WRITEXML(CUserGroupDelRsp);

    WRITEXML(CUserGroupDelNtfReq);

    WRITEXML(CUserGroupModReq);

    WRITEXML(CUserGroupModRsp);

    WRITEXML(CUserGroupModNtfReq);

    WRITEXML(CUserGroupQryReq);

    WRITEXML(CUserGroupQryRsp);

    WRITEXML(CUserGroupQryNtfReq);

    WRITEXML(CUserGroupDeviceAddReq);

    WRITEXML(CUserGroupDeviceAddRsp);

    WRITEXML(CUserGroupDeviceAddNtfReq);

    WRITEXML(CUserGroupDeviceDelReq);

    WRITEXML(CUserGroupDeviceDelRsp);

    WRITEXML(CUserGroupDeviceDelNtfReq);

    WRITEXML(CUserGroupDeviceModReq);

    WRITEXML(CUserGroupDeviceModRsp);

    WRITEXML(CUserGroupDeviceModNtfReq);

    WRITEXML(CUserGroupDeviceQryReq);

    WRITEXML(CUserGroupDeviceQryRsp);

    WRITEXML(CUserGroupDeviceQryNtfReq);

    WRITEXML(CLicenseUpdateReq);

    WRITEXML(CLicenseUpdateRsp);

    WRITEXML(CLicenseQryReq);

    WRITEXML(CLicenseQryRsp);

    WRITEXML(CDomainRelationQryReq);

    CDomainRelationQryRsp cDomainRelationQryRsp;
    cDomainRelationQryRsp.SetTotalEntryNum(1);
    cDomainRelationQryRsp.SetQueryTag("DomainRelationQry");
    WriteXml(cDomainRelationQryRsp);
    //WRITEXML(CDomainRelationQryRsp);

    CDomainRelationQryNtfReq cDomainRelationQryNtfReq;
    cDomainRelationQryNtfReq.SetEntryNum(1);
    cDomainRelationQryNtfReq.SetLastNtf(true);
    cDomainRelationQryNtfReq.SetQueryTag("DomainRelationQry");
    CDomainRelationInfo cDomainRelationInfo;
    cDomainRelationInfo.SetDomainID(g_strDomainId);
    cDomainRelationInfo.SetDomainName(g_strDomainName);
    cDomainRelationQryNtfReq.GetDomainRelationInfo().push_back(cDomainRelationInfo);
    WriteXml(cDomainRelationQryNtfReq);

    WRITEXML(CDomainRelationAddNtfReq);

    WRITEXML(CDomainRelationDelNtfReq);

    WRITEXML(CDomainRelationModNtfReq);

    WRITEXML(CDomainNameGetReq);

    CDomainNameGetRsp cDomainNameGetRsp;
    cDomainNameGetRsp.SetDomainId(g_strDomainId);
    cDomainNameGetRsp.SetDomainName(g_strDomainName);
    WriteXml(cDomainNameGetRsp);
//    WRITEXML(CDomainNameGetRsp);

    WRITEXML(CConfigTasModReq);

    WRITEXML(CConfigTasModRsp);

    WRITEXML(CConfigTasQryReq);

    WRITEXML(CConfigTasQryRsp);

    WRITEXML(CCtrlTasReq);

    WRITEXML(CCtrlTasRsp);

    WRITEXML(CTacUserAddReq);

    WRITEXML(CTacUserAddRsp);

    WRITEXML(CTacUserAddNtfReq);

    WRITEXML(CTacUserDelReq);

    WRITEXML(CTacUserDelRsp);

    WRITEXML(CTacUserDelNtfReq);

    WRITEXML(CTacUserModReq);

    WRITEXML(CTacUserModRsp);

    WRITEXML(CTacUserModNtfReq);

    WRITEXML(CTacUserQryReq);

    WRITEXML(CTacUserQryRsp);

    WRITEXML(CTacUserQryNtfReq);

    WRITEXML(CTempAuthorizationAddReq);

    WRITEXML(CTempAuthorizationAddRsp);

    WRITEXML(CTempAuthorizationAddNtfReq);

    WRITEXML(CTempAuthorizationDelReq);

    WRITEXML(CTempAuthorizationDelRsp);

    WRITEXML(CTempAuthorizationDelNtfReq);

    WRITEXML(CTempAuthorizationModReq);

    WRITEXML(CTempAuthorizationModRsp);

    WRITEXML(CTempAuthorizationModNtfReq);

    WRITEXML(CTempAuthorizationQryReq);

    WRITEXML(CTempAuthorizationQryRsp);

    WRITEXML(CTempAuthorizationQryNtfReq);

    WRITEXML(CSyncTasDataReq);

    WRITEXML(CSyncTasDataRsp);

    WRITEXML(CCuserDevicefullinfoAndPrivilegeQryReq);

    CCuserDevicefullinfoAndPrivilegeQryRsp cCCuserDevicefullinfoAndPrivilegeQryRsp;
    cCCuserDevicefullinfoAndPrivilegeQryRsp.SetTotalEntryNum(2);
    WriteXml(cCCuserDevicefullinfoAndPrivilegeQryRsp);
    //WRITEXML(CCuserDevicefullinfoAndPrivilegeQryRsp);

    CCuserDevicefullinfoAndPrivilegeQryNtfReq cCuserDevicefullinfoAndPrivilegeQryNtfReq;
    for ( int i=0;i<10;i++ )
    {
        char szBuf[128] = {0};
        sprintf(szBuf, "%d", i);
        TDevicefullinfoAndPrivilege tDev;
        tDev.deviceInfo.SetDeviceUUID(szBuf);
        tDev.deviceInfo.SetDeviceName("aaa");

        TUserVideoSourceCapInfo tCap;
        tCap.name = szBuf;
        tCap.privilege.SetPrivilege((CVideosourcePrivilege::TPrivilege)10);
        tDev.videosourceinfos.insert(make_pair<int, TUserVideoSourceCapInfo>(0, tCap));
        cCuserDevicefullinfoAndPrivilegeQryNtfReq.GetDevicefullinfoAndPrivileges().push_back(tDev);
    }
    cCuserDevicefullinfoAndPrivilegeQryNtfReq.SetEntryNum(10);
    WriteXml(cCuserDevicefullinfoAndPrivilegeQryNtfReq);
    //WRITEXML(CCuserDevicefullinfoAndPrivilegeQryNtfReq);
}