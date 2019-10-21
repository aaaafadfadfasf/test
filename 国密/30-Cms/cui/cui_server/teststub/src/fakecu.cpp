#include "fakecu.h"
//#include "soapStub.h"
#include "cuiproto.h"
#include "serviceappimp.h"

string g_strUserName = "admin";
string g_strCuUserId = "acf9c72660df4574909dac20daa58af6";
string g_strDomainId = "6e503463ef9e4dd7b3f933f3888aaede";
string g_strDomainName = "china";
string g_strGroupId = "4f4d84d9ebbd4036ac19482679b66173";
string g_strDevURI = "d5a00519995e4ca7b60a0349a77781d2@china";
string g_strUserPswd = "888888";

int nType = 0;

extern string g_strCuInCmuSess;
extern bool g_bLogin;

string GetCuiServerIpStr()
{
    //std::string strCuiIp("172.16.251.6");
    std::string strCuiIp("127.0.0.1");
    return strCuiIp;
}

u16 GetCuiServerTcpListenPort()
{
    return CUI_TCP_DAT_RECV_PORT;
}
u32 GetCuiServerTimeout()
{
    return 90;
}

API void n()
{
    g_bLogin = !g_bLogin;
}

/*====================================================================
��������login
���ܣ�
�㷨ʵ�֣�����ѡ�
����ȫ�ֱ�����
�������˵����

����ֵ˵�����ɹ�����TRUE��ʧ�ܷ���FALSE
====================================================================*/
/// Web service operation 'login' (returns error code or SOAP_OK)
void login()
{
    CCuiCuLoginReq cLoginReq;
    cLoginReq.SetUserName(g_strUserName + "@" + g_strDomainName);
    cLoginReq.SetPassword(g_strUserPswd);
    cLoginReq.SetVersion("2012.2.29");

    CCuiCuLoginRsp cLoginRsp;
    CuLogin(cLoginReq, cLoginRsp);
    g_strCuInCmuSess = cLoginRsp.GetSession();
    if ( cLoginRsp.GetErrorCode() == CMS_SUCCESS )
    {
        g_bLogin = true;
        GetUserInfo();
    }
    else
    {
        OspPrintf(TRUE, FALSE, "��¼ʧ��\n");
    }

    return;
}

/*====================================================================
��������logout
���ܣ�
�㷨ʵ�֣�����ѡ�
����ȫ�ֱ�����
�������˵����

����ֵ˵�����ɹ�����TRUE��ʧ�ܷ���FALSE
====================================================================*/
/// Web service operation 'logout' (returns error code or SOAP_OK)
void logout(const char *pSess = NULL)
{
    CCuLogoutReq cCuLogoutReq;
    if ( NULL == pSess )
    {
        cCuLogoutReq.SetSession(g_strCuInCmuSess);
    }
    else
    {
        cCuLogoutReq.SetSession(pSess);
    }

    CCuLogoutRsp cCuLogoutRsp;
    CuLogout(cCuLogoutReq, cCuLogoutRsp);
    g_bLogin = false;
    return;
}


/*====================================================================
������      : SubscribePuStatus
����        : ����ĳ�����͵�PU״̬
�㷨ʵ��    : ���������������һ��ʧ�ܾ���Ϊ��ʧ�ܣ����ز��ֲ���ʧ�ܵ��б�
             ������ɹ�������Ϊ�ɹ�
             ���ﷵ�ص���δ�ܶ��ĵ��б�
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/7/             liangli
====================================================================*/
/// Web service operation 'SubscribePuStatus' (returns error code or SOAP_OK)
void SubscribePuStatus( )
{
    CCuSubscribePuStatusReq cCuSubscribePuStatusReq;
    cCuSubscribePuStatusReq.SetSession(g_strCuInCmuSess);

    CPuStatusSubscription cSub;
    cSub.SetAllSubscribeType();
    cSub.SetDeviceURI(g_strDevURI);
    cCuSubscribePuStatusReq.GetSubscribeList().push_back(cSub);

    CCuSubscribePuStatusRsp cCuSubscribePuStatusRsp;
    CuSubscribePuStatus(cCuSubscribePuStatusReq, cCuSubscribePuStatusRsp);
    return;
}

/*====================================================================
������      : UnsubscribePuStatus
����        : ȡ��ĳ��״̬��PU����
�㷨ʵ��    : ���������������һ��ʧ�ܾ���Ϊ��ʧ�ܣ����ز��ֲ���ʧ�ܵ��б�
             ������ɹ�������Ϊ�ɹ�
             ���ﷵ�ص���δ��ȡ�����ĵ��б�
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/7/             liangli
====================================================================*/
/// Web service operation 'UnsubscribePuStatus' (returns error code or SOAP_OK)
void UnsubscribePuStatus( )
{
    CCuUnSubscribePuStatusReq cCuUnSubscribePuStatusReq;
    cCuUnSubscribePuStatusReq.SetSession(g_strCuInCmuSess);

    CPuStatusSubscription cSub;
    cSub.SetAllSubscribeType();
    cSub.SetDeviceURI(g_strDevURI);
    cCuUnSubscribePuStatusReq.GetUnSubscribeList().push_back(cSub);

    CCuUnSubscribePuStatusRsp cCuUnSubscribePuStatusRsp;
    CuUnsubscribePuStatus(cCuUnSubscribePuStatusReq, cCuUnSubscribePuStatusRsp);
    return;
}


/*====================================================================
��������GetNotify
���ܣ������ж���1��CU���2����ȡƽ̨����֪ͨ��Ϣ
�㷨ʵ�֣�
����ȫ�ֱ�����
�������˵����std::string &session  �û���SessionID
             kdcusdk__CNotifyList &rsp  ���ص�������Ϣ�����û��������Ϣ��Ϊ�սṹ��
����ֵ˵�����ɹ�����TRUE��ʧ�ܷ���FALSE
====================================================================*/
/// Web service operation 'GetNotify' (returns error code or SOAP_OK)
void GetNotify()
{
    CGetNotifyReq cGetNotifyReq;
    cGetNotifyReq.SetSession(g_strCuInCmuSess);

    CGetNotifyRsp cGetNotifyRsp;
    CuGetNotify(cGetNotifyReq, cGetNotifyRsp);

    list< CInviteStopNotify >::const_iterator pItem = cGetNotifyRsp.notifyPool.inviteStopNotify.begin();
    while ( pItem != cGetNotifyRsp.notifyPool.inviteStopNotify.end() )
    {
        OspPrintf(TRUE, FALSE, "�յ�ƽ̨��Invite֪ͨ��ֹͣ���Ŵ��� %d��ԭ���� %d\n",
            pItem->playId, pItem->errorCode);
        pItem++;
    }

    return;
}

/*====================================================================
������      : GetCUIList
����        : ��ȡ������������CUI�б�
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/1/7/             liangli
====================================================================*/
/// Web service operation 'GetCUIList' (returns error code or SOAP_OK)
void GetCUIList()
{
    CCuGetCuiListReq cCuGetCuiListReq;
    cCuGetCuiListReq.SetSession(g_strCuInCmuSess);

    CCuGetCuiListRsp cCuGetCuiListRsp;
    CuGetCuiList(cCuGetCuiListReq, cCuGetCuiListRsp);

    return;
}

/*====================================================================
������      : PtzCtrl
����        : ��ǰ������ͷ���п���
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/1/31/             liangli
====================================================================*/
/// Web service operation 'PtzCtrl' (returns error code or SOAP_OK)
void PtzCtrl()
{
    CPuPtzCtrlReq cCuPtzCtrlReq;
    cCuPtzCtrlReq.SetSession(g_strCuInCmuSess);
    TChannel tDevChn;
    tDevChn.SetDevUri(g_strDevURI);
    tDevChn.SetChnNO(1);
    cCuPtzCtrlReq.SetDevChn(tDevChn);
    TCamCtrl tCamCtrl;
    tCamCtrl.SetCommandId(1);
    tCamCtrl.GetParams().push_back(1);
    cCuPtzCtrlReq.SetCamCtrl(tCamCtrl);

    CPuPtzCtrlRsp cCuPtzCtrlRsp;
    CuPtzCtrl(cCuPtzCtrlReq, cCuPtzCtrlRsp);
    return;
}

/*====================================================================
������      : PtzLock
����        : ����ǰ������ͷ
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/1/             liangli
====================================================================*/
/// Web service operation 'PtzLock' (returns error code or SOAP_OK)
void PtzLock()
{
    CPuPtzLockReq cCuPtzLockReq;
    cCuPtzLockReq.SetSession(g_strCuInCmuSess);
    TChannel tPuChn;
    tPuChn.SetDevUri(g_strDevURI);
    tPuChn.SetChnNO(1);
    cCuPtzLockReq.SetPuChn(tPuChn);
    cCuPtzLockReq.SetLockPassword("123");
    time_t tCurTime;
    time(&tCurTime);
    cCuPtzLockReq.SetBeginLockTime(tCurTime);
    cCuPtzLockReq.SetEndLockTime(tCurTime + 1000);

    CPuPtzLockRsp cCuPtzLockRsp;
    CuPtzLock(cCuPtzLockReq, cCuPtzLockRsp);


    return;
}

/*====================================================================
������      : PtzUnlock
����        : ��ǰ������ͷ����
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/1/             liangli
====================================================================*/
/// Web service operation 'PtzUnlock' (returns error code or SOAP_OK)
void PtzUnlock()
{
    CPuPtzUnlockReq cCuPtzUnlockReq;
    cCuPtzUnlockReq.SetSession(g_strCuInCmuSess);
    TChannel tPuChn;
    tPuChn.SetDevUri(g_strDevURI);
    tPuChn.SetChnNO(1);
    cCuPtzUnlockReq.SetPuChn(tPuChn);
    cCuPtzUnlockReq.SetUnlockPassword("123");

    CPuPtzUnlockRsp cCuPtzUnlockRsp;
    CuPtzUnlock(cCuPtzUnlockReq, cCuPtzUnlockRsp);

    return;
}

/*====================================================================
������      : SendTransData
����        : ����͸����������
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/1/             liangli
====================================================================*/
/// Web service operation 'SendTransData' (returns error code or SOAP_OK)
void SendTransData()
{
    CPuSendTransparentDataReq cDataReq;
    cDataReq.SetSession(g_strCuInCmuSess);
    TChannel tPuChn;
    tPuChn.SetDevUri(g_strDevURI);
    tPuChn.SetChnNO(1);
    cDataReq.SetDevChn(tPuChn);
    cDataReq.SetHexTransparentData("abc");

    CPuSendTransparentDataRsp cDataRsp;
    CuSendTransData(cDataReq, cDataRsp);
    return;
}

/*====================================================================
������      : GetUserInfo
����        : ��ȡ��ǰ��¼CU���û���Ϣ, ������CU�����¼�û�
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/2/             liangli
====================================================================*/
/// Web service operation 'GetUserInfo' (returns error code or SOAP_OK)
void GetUserInfo()
{
    CCuQueryUserReq cCuQueryUserReq;
    cCuQueryUserReq.SetSession(g_strCuInCmuSess);
    CUserInfoQueryCondition cCondition;
    string strUserName = GetNameWithoutDomainFromURI(g_strUserName); //��ѯCU������Ϣ, ligang@nanjing ת��Ϊligang
    cCondition.SetUserName(strUserName);
    cCuQueryUserReq.SetUserInfoQueryCondition(cCondition);

    CCuQueryUserRsp cCuQueryUserRsp;
    CuGetUserInfo(cCuQueryUserReq, cCuQueryUserRsp);  //CuGetSelfUserInfo
    if ( cCuQueryUserRsp.GetUserInfo().empty() )
    {
        OspPrintf(TRUE, FALSE, "���棬��ȡ�û���Ϣʧ��\n");
    }
    else
    {
        g_strCuUserId = cCuQueryUserRsp.GetUserInfo().at(0).GetUserID();
        g_strDomainId = cCuQueryUserRsp.GetUserInfo().at(0).GetDomainID();
        GetUserDev();
    }
    return;
}

/*====================================================================
������      : GetUserList
����        : ��ȡ��ǰ��¼CU�����û���Ϣ�������û�����
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/14/             liangli
====================================================================*/
/// Web service operation 'GetUserList' (returns error code or SOAP_OK)
void GetUserList()
{
    CCuQueryUserReq cCuQueryUserReq;
    cCuQueryUserReq.SetSession(g_strCuInCmuSess);
    cCuQueryUserReq.SetStartEntryIndex(0);
    cCuQueryUserReq.SetExpectEntryNum(1000);
    CUserInfoQueryCondition cCondition;
    cCondition.SetCuserID(g_strCuUserId);
    cCuQueryUserReq.SetUserInfoQueryCondition(cCondition);

    CCuQueryUserRsp cCuQueryUserRsp;
    CuGetUserInfo(cCuQueryUserReq, cCuQueryUserRsp);  //CuGetSelfUserInfo
    printf("cCuQueryUserRsp.GetTotalEntryNum() = %d\n",
        cCuQueryUserRsp.GetTotalEntryNum());
    printf("cCuQueryUserRsp.GetUserInfo().size() = %d\n",
        cCuQueryUserRsp.GetUserInfo().size());
    printf("cCuQueryUserRsp.GetActualResultNum() = %d\n",
        cCuQueryUserRsp.GetActualResultNum());
    return;
}

/*====================================================================
������      : GetUserGroupInfo
����        : ��ȡ��ǰ��¼CU���û�����Ϣ, ������CU�����¼�û�
�㷨ʵ��    : CU��ͨ���˽ӿڻ�ȡ�����DomainUUID
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/5/             liangli
====================================================================*/
/// Web service operation 'GetUserGroupInfo' (returns error code or SOAP_OK)
void GetUserGroupInfo()
{
    CCuUserGroupQryReq cCuQueryUserGroupReq;
    cCuQueryUserGroupReq.SetSession(g_strCuInCmuSess);
    CUserGroupInfoQueryCondition cCondition;
    cCondition.SetGroupNO("123");
    cCuQueryUserGroupReq.SetUserGroupInfoQueryCondition(cCondition);

    CCuUserGroupQryRsp cCuQueryUserGroupRsp;
    CuGetUserGroupInfo(cCuQueryUserGroupReq, cCuQueryUserGroupRsp);

    return;
}

/*====================================================================
������      : GetUserGroupList
����        : ��ȡ��ǰ��¼CU�����û���
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/14/             liangli
====================================================================*/
/// Web service operation 'GetUserGroupList' (returns error code or SOAP_OK)
void GetUserGroupList()
{
    CCuUserGroupQryReq cCuQueryUserGroupReq;
    cCuQueryUserGroupReq.SetSession(g_strCuInCmuSess);
    cCuQueryUserGroupReq.SetStartEntryIndex(0);
    cCuQueryUserGroupReq.SetExpectEntryNum(1);
    CUserGroupInfoQueryCondition cCondition;
    cCondition.SetCuserID(g_strCuUserId);
    cCuQueryUserGroupReq.SetUserGroupInfoQueryCondition(cCondition);

    CCuUserGroupQryRsp cCuQueryUserGroupRsp;
    CuGetUserGroupInfo(cCuQueryUserGroupReq, cCuQueryUserGroupRsp);

    return;
}

/*====================================================================
������      : GetUserData
����        : ��ȡ�û��Զ�������
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/5/             liangli
====================================================================*/
/// Web service operation 'GetUserData' (returns error code or SOAP_OK)
void GetUserData()
{
    CCuGetUserDataReq cCuGetUserDataReq;
    cCuGetUserDataReq.SetSession(g_strCuInCmuSess);
    cCuGetUserDataReq.SetKey("123");

    CCuGetUserDataRsp cCuGetUserDataRsp;
    CuGetUserData(cCuGetUserDataReq, cCuGetUserDataRsp);

    return;
}

/*====================================================================
������      : SetUserData
����        : ����CU�Զ�������
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/5/             liangli
====================================================================*/
/// Web service operation 'SetUserData' (returns error code or SOAP_OK)
void SetUserData()
{
    CCuSetUserDataReq cCuSetUserDataReq;
    cCuSetUserDataReq.SetSession(g_strCuInCmuSess);
    cCuSetUserDataReq.SetKey("123");
    cCuSetUserDataReq.SetUserData("123");

    CCuSetUserDataRsp cCuSetUserDataRsp;
    CuSetUserData(cCuSetUserDataReq, cCuSetUserDataRsp);

    return;
}

/*====================================================================
������      : GetUserDevGroupByGroup
����        : ���ݸ��豸���ȡ��һ���豸�飬һ��CU�����豸��ʱ�õ��˽ӿ�
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/6/             liangli
====================================================================*/
/// Web service operation 'GetUserDevGroupByGroup' (returns error code or SOAP_OK)
void GetUserDevGroupByGroup()
{
    CCuUserDevGroupQryReq cCuGetUserDevGroupReq;
    cCuGetUserDevGroupReq.SetSession(g_strCuInCmuSess);
    cCuGetUserDevGroupReq.SetExpectEntryNum(1);
    cCuGetUserDevGroupReq.SetStartEntryIndex(0);
    CDeviceGroupInfoQueryCondition cCondition;
    cCondition.SetCuserID(g_strCuUserId);
    cCondition.SetDeviceParentGroupID("123");
    cCuGetUserDevGroupReq.SetDeviceGroupInfoQueryCondition(cCondition);

    CCuUserDevGroupQryRsp cCuGetUserDevGroupRsp;
    CuGetUserDevGroup(cCuGetUserDevGroupReq, cCuGetUserDevGroupRsp);
    return;
}

/*====================================================================
������      : GetUserDevGroup
����        : ��ȡ�Ե�ǰCU������Ϊ���ڵ�������豸�飬һ����ο������õ��˽ӿ�
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/10/             liangli
====================================================================*/
/// Web service operation 'GetUserDevGroup' (returns error code or SOAP_OK)
void GetUserDevGroup()
{
    CCuUserDevGroupQryReq cCuGetUserDevGroupReq;
    cCuGetUserDevGroupReq.SetSession(g_strCuInCmuSess);
    cCuGetUserDevGroupReq.SetExpectEntryNum(1);
    cCuGetUserDevGroupReq.SetStartEntryIndex(0);
    CDeviceGroupInfoQueryCondition cCondition;
    cCondition.SetCuserID(g_strCuUserId);
    cCuGetUserDevGroupReq.SetDeviceGroupInfoQueryCondition(cCondition);

    CCuUserDevGroupQryRsp cCuGetUserDevGroupRsp;
    CuGetUserDevGroup(cCuGetUserDevGroupReq, cCuGetUserDevGroupRsp);

    return;
}

/*====================================================================
������      : GetDomainDevGroup
����        : ��ȡ����������豸�飬һ����ο������õ��˽ӿ�
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/9/             liangli
====================================================================*/
/// Web service operation 'GetDomainDevGroup' (returns error code or SOAP_OK)
void GetDomainDevGroup()
{
    CCuUserDevGroupQryReq cCuGetUserDevGroupReq;
    cCuGetUserDevGroupReq.SetSession(g_strCuInCmuSess);
    cCuGetUserDevGroupReq.SetExpectEntryNum(1);
    cCuGetUserDevGroupReq.SetStartEntryIndex(0);
    CDeviceGroupInfoQueryCondition cCondition;
    cCondition.SetCuserID(g_strCuUserId);
    cCondition.SetDomainUUID(g_strDomainId);
    cCuGetUserDevGroupReq.SetDeviceGroupInfoQueryCondition(cCondition);

    CCuUserDevGroupQryRsp cCuGetUserDevGroupRsp;
    CuGetUserDevGroup(cCuGetUserDevGroupReq, cCuGetUserDevGroupRsp);
    return;
}

/*====================================================================
������      : GetUserDevByGroup
����        : �����豸���ȡ�û��豸��һ��CU�õ��˽ӿ�
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/6/             liangli
====================================================================*/
/// Web service operation 'GetUserDevByGroup' (returns error code or SOAP_OK)
void GetUserDevByGroup()
{
    CCuUserDevFullInfoQryReq cCuGetUserDevFullInfoReq;
    cCuGetUserDevFullInfoReq.SetSession(g_strCuInCmuSess);
    cCuGetUserDevFullInfoReq.SetStartEntryIndex(0);
    cCuGetUserDevFullInfoReq.SetExpectEntryNum(1);
    cCuGetUserDevFullInfoReq.SetCuserId(g_strCuUserId);
    CCuserDevicefullinfoAndPrivilegeQueryCondition cCondition;
    cCondition.SetDevicegroupId("123");
    cCuGetUserDevFullInfoReq.SetCondition(cCondition);

    CCuUserDevFullInfoQryRsp cCuGetUserDevFullInfoRsp;
    CuGetUserDevFullInfo(cCuGetUserDevFullInfoReq, cCuGetUserDevFullInfoRsp);

    return;
}

/*====================================================================
������      : GetUserDev
����        : ��ȡ�����û��豸��һ����ο����õ��˽ӿ�
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/10/             liangli
====================================================================*/
/// Web service operation 'GetUserDev' (returns error code or SOAP_OK)
void GetUserDev()
{
    CCuUserDevFullInfoQryReq cCuGetUserDevFullInfoReq;
    cCuGetUserDevFullInfoReq.SetSession(g_strCuInCmuSess);
    cCuGetUserDevFullInfoReq.SetStartEntryIndex(0);
    cCuGetUserDevFullInfoReq.SetExpectEntryNum(1);
    cCuGetUserDevFullInfoReq.SetCuserId(g_strCuUserId);

    CCuUserDevFullInfoQryRsp cCuGetUserDevFullInfoRsp;
    CuGetUserDevFullInfo(cCuGetUserDevFullInfoReq, cCuGetUserDevFullInfoRsp);
    if ( cCuGetUserDevFullInfoRsp.GetErrorCode() != CMS_SUCCESS )
    {
        OspPrintf(TRUE, FALSE, "��ȡ�û��豸ʧ�ܣ�������[%d]\n", 
            cCuGetUserDevFullInfoRsp.GetErrorCode());
    }
    else
    {
        OspPrintf(TRUE, FALSE, "cCuGetUserDevFullInfoRsp.GetTotalEntryNum() = %d\n",
            cCuGetUserDevFullInfoRsp.GetTotalEntryNum());
        OspPrintf(TRUE, FALSE, "cCuGetUserDevFullInfoRsp.GetUserDevFullInfo().size() = %d\n",
            cCuGetUserDevFullInfoRsp.GetUserDevFullInfo().size());
        OspPrintf(TRUE, FALSE, "cCuGetUserDevFullInfoRsp.GetActualResultNum() = %d\n",
            cCuGetUserDevFullInfoRsp.GetActualResultNum());
    }

    return;
}

/*====================================================================
������      : GetUserDevByDomain
����        : ���ݵ�ǰ���ȡ�û��豸��һ����ο����õ��˽ӿ�
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/10/             liangli
====================================================================*/
/// Web service operation 'GetUserDevByDomain' (returns error code or SOAP_OK)
void GetUserDevByDomain()
{
    CCuUserDevFullInfoQryReq cCuGetUserDevFullInfoReq;
    cCuGetUserDevFullInfoReq.SetSession(g_strCuInCmuSess);
    cCuGetUserDevFullInfoReq.SetStartEntryIndex(0);
    cCuGetUserDevFullInfoReq.SetExpectEntryNum(1);
    cCuGetUserDevFullInfoReq.SetCuserId(g_strCuUserId);
    CCuserDevicefullinfoAndPrivilegeQueryCondition cCondition;
    cCondition.SetDomainId(g_strDomainId);
    cCuGetUserDevFullInfoReq.SetCondition(cCondition);

    CCuUserDevFullInfoQryRsp cCuGetUserDevFullInfoRsp;
    CuGetUserDevFullInfo(cCuGetUserDevFullInfoReq, cCuGetUserDevFullInfoRsp);

    return;
}

/*====================================================================
������      : GetDeviceGroupDeviceRelations
����        : ��ȡ�豸���豸��Ķ�Ӧ��ϵ��һ����ο����õ��˽ӿ�
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/10/             liangli
====================================================================*/
/// Web service operation 'GetDeviceGroupDeviceRelations' (returns error code or SOAP_OK)
void GetDeviceGroupDeviceRelations()
{
    CCuQryDevGroupOfDevInfoReq cCuQryDevGroupOfDevInfoReq;
    cCuQryDevGroupOfDevInfoReq.SetSession(g_strCuInCmuSess);
    cCuQryDevGroupOfDevInfoReq.SetExpectEntryNum(1);
    cCuQryDevGroupOfDevInfoReq.SetStartEntryIndex(0);
    CDeviceGroupDeviceInfoQueryCondition cCondition;
    cCondition.SetCuserID(g_strCuUserId);

    CCuQryDevGroupOfDevInfoRsp cCuQryDevGroupOfDevInfoRsp;
    CuGetDeviceGroupOfDeviceInfo(cCuQryDevGroupOfDevInfoReq, cCuQryDevGroupOfDevInfoRsp);

    return;
}

/*====================================================================
������      : CreateDeviceGroup
����        : �����豸�飬CU�Ͷ��ο��������õ��˹���
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/13/             liangli
====================================================================*/
/// Web service operation 'CreateDeviceGroup' (returns error code or SOAP_OK)
void CreateDeviceGroup()
{
    CCuCreateDeviceGroupReq cCuCreateDeviceGroupReq;
    cCuCreateDeviceGroupReq.SetSession(g_strCuInCmuSess);
    CDeviceGroupInfo cDevGrpInfo;
    cDevGrpInfo.SetDeviceGroupName("devgroup");
    cDevGrpInfo.SetDeviceParentGroupID("123");
    cDevGrpInfo.SetDomainUUID(g_strDomainId);
    cDevGrpInfo.SetDeviceGroupDesc("desc");
    cCuCreateDeviceGroupReq.SetDeviceGroupInfo(cDevGrpInfo);

    CCuCreateDeviceGroupRsp cCuCreateDeviceGroupRsp;
    CuCreateDeviceGroup(cCuCreateDeviceGroupReq, cCuCreateDeviceGroupRsp);

    return;
}

/*====================================================================
������      : DelDeviceGroup
����        : ɾ���豸��
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/13/             liangli
====================================================================*/
/// Web service operation 'DelDeviceGroup' (returns error code or SOAP_OK)
void DelDeviceGroup()
{
    CCuDeleteDeviceGroupReq cCuDeleteDeviceGroupReq;
    cCuDeleteDeviceGroupReq.SetSession(g_strCuInCmuSess);
    CDeviceGroupInfoKey cDevGroupId;
    cDevGroupId.SetDeviceGroupID(g_strGroupId);
    cCuDeleteDeviceGroupReq.SetDeviceGroupInfoKey(cDevGroupId);

    CCuDeleteDeviceGroupRsp cCuDeleteDeviceGroupRsp;
    CuDeleteDeviceGroup(cCuDeleteDeviceGroupReq, cCuDeleteDeviceGroupRsp);

    return;
}

/*====================================================================
������      : ModifyDeviceGroup
����        : �޸��豸����Ϣ
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/13/             liangli
====================================================================*/
/// Web service operation 'ModifyDeviceGroup' (returns error code or SOAP_OK)
void ModifyDeviceGroup()
{
    CCuModifyDeviceGroupReq cCuModifyDeviceGroupReq;
    cCuModifyDeviceGroupReq.SetSession(g_strCuInCmuSess);
    cCuModifyDeviceGroupReq.SetDeviceGroupID(g_strGroupId);
    cCuModifyDeviceGroupReq.SetDeviceGroupName(g_strGroupId);
    cCuModifyDeviceGroupReq.SetDeviceParentGroupID(g_strGroupId);
    cCuModifyDeviceGroupReq.SetDeviceGroupDesc(g_strGroupId);

    CCuModifyDeviceGroupRsp cCuModifyDeviceGroupRsp;
    CuModifyDeviceGroup(cCuModifyDeviceGroupReq, cCuModifyDeviceGroupRsp);
    return;
}

/*====================================================================
������      : DeviceGroupAddDevice
����        : �豸������豸
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/13/             liangli
====================================================================*/
/// Web service operation 'DeviceGroupAddDevice' (returns error code or SOAP_OK)
void DeviceGroupAddDevice()
{
    CCuDeviceGroupAddDeviceReq cCuDeviceGroupAddDeviceReq;
    cCuDeviceGroupAddDeviceReq.SetSession(g_strCuInCmuSess);
    CDeviceGroupDeviceInfo cInfo;
    cInfo.SetDeviceGroupID(g_strGroupId);
    cInfo.SetDeviceID(g_strGroupId);
    cInfo.SetDomainId(g_strDomainId);
    TDeviceCapIndexs Indexs;
    Indexs.id = 1;
    Indexs.indexs.push_back(1);
    cInfo.GetDeviceCapIndexs().push_back(Indexs);
    cCuDeviceGroupAddDeviceReq.GetDeviceGroupDeviceInfo().push_back(cInfo);

    CCuDeviceGroupAddDeviceRsp cCuDeviceGroupAddDeviceRsp;
    CuDeviceGroupAddDevice( cCuDeviceGroupAddDeviceReq, cCuDeviceGroupAddDeviceRsp );

    return;
}

/*====================================================================
������      : DeviceGroupDelDevice
����        : �豸��ɾ���豸
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/13/             liangli
====================================================================*/
/// Web service operation 'DeviceGroupDelDevice' (returns error code or SOAP_OK)
void DeviceGroupDelDevice()
{
    CCuDeviceGroupDelDeviceReq cCuDeviceGroupDelDeviceReq;
    cCuDeviceGroupDelDeviceReq.SetSession(g_strCuInCmuSess);
    CDeviceGroupDeviceInfo cInfo;
    cInfo.SetDeviceGroupID(g_strGroupId);
    cInfo.SetDeviceID(g_strGroupId);
    cInfo.SetDomainId(g_strDomainId);
    TDeviceCapIndexs Indexs;
    Indexs.id = 1;
    Indexs.indexs.push_back(1);
    cInfo.GetDeviceCapIndexs().push_back(Indexs);
    cCuDeviceGroupDelDeviceReq.GetDeviceGroupDeviceInfo().push_back(cInfo);

    CCuDeviceGroupDelDeviceRsp cCuDeviceGroupDelDeviceRsp;
    CuDeviceGroupDelDevice( cCuDeviceGroupDelDeviceReq, cCuDeviceGroupDelDeviceRsp );
    return;
}

/*====================================================================
������      : CreateUserGroup
����        : �����µ��û���
�㷨ʵ��    : �û���û�����Ĳ㼶�ṹ�����û��������û���ֻ�����û�����
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/14/             liangli
====================================================================*/
void CreateUserGroup()
{
    CCuCreateUserGroupReq cCuCreateUserGroupReq;
    cCuCreateUserGroupReq.SetSession(g_strCuInCmuSess);
    CUserGroupInfo cUserGroupInfo;
    cUserGroupInfo.SetGroupName(g_strUserName);
    cUserGroupInfo.SetDescription(g_strUserName);
    cUserGroupInfo.SetUserType(1);
    CUserPrivilege cUserPri;
    cUserPri.SetPrivilege(CUserPrivilege::en_Alarm_Manage);
    cUserGroupInfo.SetUserPrivilege(cUserPri);
    cUserGroupInfo.SetCreateUser(g_strCuUserId);

    CCuCreateUserGroupRsp cCuCreateUserGroupRsp;
    CuCreateUserGroup(cCuCreateUserGroupReq, cCuCreateUserGroupRsp);

    return;
}

/*====================================================================
������      : DelUserGroup
����        : ɾ���û���
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/14/             liangli
====================================================================*/
void DelUserGroup()
{
    CCuDeleteUserGroupReq cCuDeleteUserGroupReq;
    cCuDeleteUserGroupReq.SetSession(g_strCuInCmuSess);
    CUserGroupInfoKey cUserGroupInfoKey;
    cUserGroupInfoKey.SetGroupNO(g_strGroupId);
    cCuDeleteUserGroupReq.SetUserGroupInfoKey(cUserGroupInfoKey);

    CCuDeleteUserGroupRsp cCuDeleteUserGroupRsp;
    CuDeleteUserGroup(cCuDeleteUserGroupReq, cCuDeleteUserGroupRsp);

    return;
}

/*====================================================================
������      : ModifyUserGroup
����        : �޸��û��������Ϣ
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/14/             liangli
====================================================================*/
void ModifyUserGroup()
{
    CCuModifyUserGroupReq cCuModifyUserGroupReq;
    cCuModifyUserGroupReq.SetSession(g_strCuInCmuSess);
    cCuModifyUserGroupReq.SetGroupNO(g_strGroupId);
    cCuModifyUserGroupReq.SetGroupName(g_strGroupId);
    cCuModifyUserGroupReq.SetDescription(g_strGroupId);
    CUserPrivilege cUserPri;
    cUserPri.SetPrivilege(CUserPrivilege::en_Alarm_Manage);

    cCuModifyUserGroupReq.SetUserPrivilege(cUserPri);
    //�������޸�ID���������ڣ��û������ͺʹ�����ID��������IDֻ�ܺ�̨��TAS�Զ��޸�

    CCuModifyUserGroupRsp cCuModifyUserGroupRsp;
    CuModifyUserGroup(cCuModifyUserGroupReq, cCuModifyUserGroupRsp);

    return;
}

/*====================================================================
������      : CreateUser
����        : �������û�
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/14/             liangli
====================================================================*/
void CreateUser()
{
    CCuCreateUserReq cCuCreateUserReq;
    cCuCreateUserReq.SetSession(g_strCuInCmuSess);
    CCuUserInfo cUserInfo;
	string strNewUser = "aaa";
    cUserInfo.SetUserName(strNewUser + "@" + g_strDomainName);
    CUserPrivilege cPri;
    cPri.SetPrivilege(CUserPrivilege::en_Call_Ctrl);
    cUserInfo.SetUserPrivilege(cPri);
    cCuCreateUserReq.SetUserInfo(cUserInfo);

    CCuCreateUserRsp cCuCreateUserRsp;
    CuCreateUser(cCuCreateUserReq, cCuCreateUserRsp);
    return;
}

/*====================================================================
������      : DelUser
����        : ɾ�����û�
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/14/             liangli
====================================================================*/
void DelUser()
{
    CCuDeleteUserReq cCuDeleteUserReq;
    cCuDeleteUserReq.SetSession(g_strCuInCmuSess);
    cCuDeleteUserReq.SetUserID(g_strCuUserId);

    CCuDeleteUserRsp cCuDeleteUserRsp;
    CuDeleteUser(cCuDeleteUserReq, cCuDeleteUserRsp);

    return;
}

/*====================================================================
������      : ModifyUser
����        : �޸����û�������Ϣ�������޸��û���������
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/14/             liangli
====================================================================*/
void ModifyUser()
{
    CCuModifyUserReq cCuModifyUserReq;
    cCuModifyUserReq.SetSession(g_strCuInCmuSess);
    cCuModifyUserReq.SetUserID(g_strCuInCmuSess);
    cCuModifyUserReq.SetUsergrpID(g_strCuInCmuSess);
    cCuModifyUserReq.SetUserName(g_strCuInCmuSess);
    cCuModifyUserReq.SetUserPwd(g_strCuInCmuSess);
    cCuModifyUserReq.SetUserType(1);
    cCuModifyUserReq.SetMLoginFlag(1);
    cCuModifyUserReq.SetEnabledFlag(1);
    cCuModifyUserReq.SetDescription(g_strCuInCmuSess);
    time_t tCurTime;
    time(&tCurTime);
    CCMSTime cNameExpireData(tCurTime);
    cCuModifyUserReq.SetNameExpireDate(cNameExpireData.ToString());
    CCMSTime cPwdExpireData(tCurTime+1000);
    cCuModifyUserReq.SetPwdExpireDate(cPwdExpireData.ToString());
    CUserPrivilege cPri;
    cPri.SetPrivilege(CUserPrivilege::en_Alarm_Manage);
    cCuModifyUserReq.SetUserPrivilege(cPri);

    CCuModifyUserRsp cCuModifyUserRsp;
    CuModifyUser(cCuModifyUserReq, cCuModifyUserRsp);

    return;
}

/*====================================================================
������      : UserGetDeviceByGroup
����        : ��ȡ�û������û�����豸�����б�
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/14/             liangli
====================================================================*/
void UserGetDeviceByGroup()
{
    if ( nType == cusdk__TypeUser )
    {
        CCuGetUserDeviceReq cCuGetUserDeviceReq;
        cCuGetUserDeviceReq.SetSession(g_strCuInCmuSess);
        cCuGetUserDeviceReq.SetStartEntryIndex(0);
        cCuGetUserDeviceReq.SetExpectEntryNum(1);
        CUserDeviceInfoQueryCondition cCondition;
        cCondition.SetUserID(g_strCuUserId);
        cCondition.SetDevGrpID(g_strGroupId);
        cCuGetUserDeviceReq.SetUserDeviceInfoQueryCondition(cCondition);

        CCuGetUserDeviceRsp cCuGetUserDeviceRsp;
        CuGetUserDevice( cCuGetUserDeviceReq, cCuGetUserDeviceRsp );
    }
    else if ( nType == cusdk__TypeUserGroup )
    {
        CCuGetUserGroupDeviceReq cCuGetUserGroupDeviceReq;
        cCuGetUserGroupDeviceReq.SetSession(g_strCuInCmuSess);
        cCuGetUserGroupDeviceReq.SetStartEntryIndex(0);
        cCuGetUserGroupDeviceReq.SetExpectEntryNum(1);
        CUserGroupDeviceInfoQueryCondition cCondition;
        cCondition.SetCuserGrpID(g_strCuUserId);
        cCondition.SetDevGrpID(g_strGroupId);
        cCuGetUserGroupDeviceReq.SetUserGroupDeviceInfoQueryCondition(cCondition);

        CCuGetUserGroupDeviceRsp cCuGetUserGroupDeviceRsp;
        CuGetUserGroupDevice( cCuGetUserGroupDeviceReq, cCuGetUserGroupDeviceRsp );
    }
    return;
}

/*====================================================================
������      : UserAddDevice
����        : �û�����֮ ����û����飩�豸
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/15/             liangli
====================================================================*/
void UserAddDevice()
{
    if ( nType == cusdk__TypeUser )
    {
        CCuUserAddDeviceReq cCuUserAddDeviceReq;
        cCuUserAddDeviceReq.SetSession(g_strCuInCmuSess);
        CUserDeviceInfo cUserDevInfo;
        cUserDevInfo.SetUserID(g_strCuUserId);
        cCuUserAddDeviceReq.GetUserDeviceInfoList().push_back(cUserDevInfo);

        CCuUserAddDeviceRsp cCuUserAddDeviceRsp;
        CuUserAddDevice(cCuUserAddDeviceReq, cCuUserAddDeviceRsp);
    }
    else if ( nType == cusdk__TypeUserGroup )
    {
        CCuUserGroupAddDeviceReq cCuUserGroupAddDeviceReq;
        cCuUserGroupAddDeviceReq.SetSession(g_strCuInCmuSess);
        CUserGroupDeviceInfo cUserGroupDevInfo;
        cUserGroupDevInfo.SetDeviceID("123");
        cCuUserGroupAddDeviceReq.GetUserGroupDeviceInfoList().push_back(cUserGroupDevInfo);

        CCuUserGroupAddDeviceRsp cCuUserGroupAddDeviceRsp;
        CuUserGroupAddDevice(cCuUserGroupAddDeviceReq, cCuUserGroupAddDeviceRsp);
    }
    return;
}

/*====================================================================
������      : UserDelDevice
����        : �û�����֮ ɾ���û����飩�豸
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/15/             liangli
====================================================================*/
void UserDelDevice()
{
    if ( nType == cusdk__TypeUser )
    {
        CCuUserDelDeviceReq cCuUserDelDeviceReq;
        cCuUserDelDeviceReq.SetSession(g_strCuInCmuSess);
        CUserDeviceInfo cUserDevInfo;
        cUserDevInfo.SetDeviceID("123");
        cCuUserDelDeviceReq.GetUserDeviceInfoList().push_back(cUserDevInfo);

        CCuUserDelDeviceRsp cCuUserDelDeviceRsp;
        CuUserDelDevice(cCuUserDelDeviceReq, cCuUserDelDeviceRsp);
    }
    else if ( nType == cusdk__TypeUserGroup )
    {
        CCuUserGroupDelDeviceReq cCuUserGroupDelDeviceReq;
        cCuUserGroupDelDeviceReq.SetSession(g_strCuInCmuSess);
        CUserGroupDeviceInfo cUserGroupDevInfo;
        cUserGroupDevInfo.SetDeviceID("123");
        cCuUserGroupDelDeviceReq.GetUserGroupDeviceInfoList().push_back(cUserGroupDevInfo);

        CCuUserGroupDelDeviceRsp cCuUserGroupDelDeviceRsp;
        CuUserGroupDelDevice(cCuUserGroupDelDeviceReq, cCuUserGroupDelDeviceRsp);
    }
    return;
}

/*====================================================================
������      : UserModifyDevice
����        : �û�����֮ �޸��û����飩�豸Ȩ��
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/15/             liangli
====================================================================*/
void UserModifyDevice()
{
    if ( nType == cusdk__TypeUser )
    {
        CCuUserModDeviceReq cCuUserModDeviceReq;
        cCuUserModDeviceReq.SetSession(g_strCuInCmuSess);
        CUserDeviceInfo cUserDevInfo;
        cUserDevInfo.SetUserID(g_strCuUserId);
        cCuUserModDeviceReq.GetUserDeviceInfoList().push_back(cUserDevInfo);
        CCuUserModDeviceRsp cCuUserModDeviceRsp;
        CuUserModDevice(cCuUserModDeviceReq, cCuUserModDeviceRsp);
    }
    else if ( nType == cusdk__TypeUserGroup )
    {
        CCuUserGroupModDeviceReq cCuUserGroupModDeviceReq;
        cCuUserGroupModDeviceReq.SetSession(g_strCuInCmuSess);
        CUserGroupDeviceInfo cUserGroupDevInfo;
        cUserGroupDevInfo.SetGroupID(g_strCuUserId);
        cCuUserGroupModDeviceReq.GetUserGroupDeviceInfoList().push_back(cUserGroupDevInfo);

        CCuUserGroupModDeviceRsp cCuUserGroupModDeviceRsp;
        CuUserGroupModDevice(cCuUserGroupModDeviceReq, cCuUserGroupModDeviceRsp);
    }
    return;
}

/*====================================================================
������      : GetAuthorizationDevice
����        : ��ʱ��Ȩ����ֻ�г�������Ա����ʹ�ã�������ʱ��Ȩ������򣬲���������û�
              ��ȡ�������ʱ��Ȩ�豸�б�ӿ���ͨCU�û�Ҳ��ʹ�ã������ӿڶ�ֻ�ܳ�������Աʹ��
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/16/             liangli
====================================================================*/
void GetAuthorizationDevice()
{
    CCuGetAuthorizationDeviceReq cCuGetAuthorizationDeviceReq;
    cCuGetAuthorizationDeviceReq.SetSession(g_strCuInCmuSess);
    cCuGetAuthorizationDeviceReq.SetStartEntryIndex(0);
    cCuGetAuthorizationDeviceReq.SetExpectEntryNum(1);

    CCuGetAuthorizationDeviceRsp cCuGetAuthorizationDeviceRsp;
    CuGetGetAuthorizationDevice(cCuGetAuthorizationDeviceReq, cCuGetAuthorizationDeviceRsp);

    return;
}

/*====================================================================
������      : AddAuthorizationDevice
����        : CU��Ŀ����ָ���û���ʱ��Ȩĳ�豸
�㷨ʵ��    : ֻ�ܳ�������Աʹ��
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/17/             liangli
====================================================================*/
void AddAuthorizationDevice()
{
    CCuAddAuthorizationDeviceReq cCuAddAuthorizationDeviceReq;
    cCuAddAuthorizationDeviceReq.SetSession(g_strCuInCmuSess);
    CTempAuthorizationInfo cTempAuthorizationInfo;
    cTempAuthorizationInfo.SetDeviceID("123");
    cCuAddAuthorizationDeviceReq.SetTempAuthorizationInfo(cTempAuthorizationInfo);

    CCuAddAuthorizationDeviceRsp cCuAddAuthorizationDeviceRsp;
    CuAddAuthorizationDevice(cCuAddAuthorizationDeviceReq, cCuAddAuthorizationDeviceRsp);
    return;
}

/*====================================================================
������      : DelAuthorizationDevice
����        : ɾ����ʱ��Ȩ�豸������������Ա��ʹ��
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/17/             liangli
====================================================================*/
void DelAuthorizationDevice()
{
    CCuDelAuthorizationDeviceReq cCuDelAuthorizationDeviceReq;
    cCuDelAuthorizationDeviceReq.SetSession(g_strCuInCmuSess);
    CTempAuthorizationInfoKey cTempAuthorizationInfoKey;
    cTempAuthorizationInfoKey.SetDeviceID("123");
    cTempAuthorizationInfoKey.SetUserName(GetNameWithoutDomainFromURI(g_strUserName));
    cTempAuthorizationInfoKey.SetDestinationDomain(GetDomainNameWithoutPreNameFromURI(g_strUserName));
    cCuDelAuthorizationDeviceReq.SetTempAuthorizationInfoKey(cTempAuthorizationInfoKey);

    CCuDelAuthorizationDeviceRsp cCuDelAuthorizationDeviceRsp;
    CuDelAuthorizationDevice(cCuDelAuthorizationDeviceReq, cCuDelAuthorizationDeviceRsp);
    return;
}

/*====================================================================
������      : ModifyAuthorizationDevice
����        : �޸���Ȩ�豸��Ϣ������������Ա��ʹ�ã�Ŀǰֻ���޸�����
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/17/             liangli
====================================================================*/
void ModifyAuthorizationDevice()
{
    CCuModifyAuthorizationDeviceReq cCuModifyAuthorizationDeviceReq;
    cCuModifyAuthorizationDeviceReq.SetSession(g_strCuInCmuSess);
    cCuModifyAuthorizationDeviceReq.SetDeviceID("123");
    cCuModifyAuthorizationDeviceReq.SetDesc("123");

    CCuModifyAuthorizationDeviceRsp cCuModifyAuthorizationDeviceRsp;
    CuModifyAuthorizationDevice(cCuModifyAuthorizationDeviceReq, cCuModifyAuthorizationDeviceRsp);
    return;
}

/*====================================================================
������      : GetDeviceBaseInfo
����        : ��ѯ�����豸�Ļ�����Ϣ
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/17/             liangli
====================================================================*/
void GetDeviceBaseInfo()
{
    CCuGetDeviceBaseInfoReq cCuGetDeviceBaseInfoReq;
    cCuGetDeviceBaseInfoReq.SetSession(g_strCuInCmuSess);
    CDeviceInfoQueryCondition cCondition;
    cCondition.SetDeviceUUID(GetNameWithoutDomainFromURI(g_strDevURI));
    cCuGetDeviceBaseInfoReq.SetDeviceInfoQueryCondition(cCondition);

    CCuGetDeviceBaseInfoRsp cCuGetDeviceBaseInfoRsp;
    CuGetDeviceBaseInfo(cCuGetDeviceBaseInfoReq, cCuGetDeviceBaseInfoRsp);
    return;
}

void ConvertNetAddrFromCuToCui(const cusdk__NetworkAddr &tFrom, TNetAddr &tTo)
{
    tTo.SetNetIp(tFrom.ip);
    tTo.SetNetPort(tFrom.port);
}

/*====================================================================
������      : SetupStream
����        : Invite��Ƶ����
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/22/             liangli
====================================================================*/
void SetupStream()
{
    CCuSetupStreamReq cCuSetupStreamReq;
    cCuSetupStreamReq.SetSession(g_strCuInCmuSess);
    //Դ��PU
    TChannel tSrcChn;
    tSrcChn.SetDevUri(g_strDevURI);
    tSrcChn.SetChnNO(0);
    cCuSetupStreamReq.SetSrcChn(tSrcChn);
    //Ŀ����CU
    TChannel tCuChn;
    tCuChn.SetDevUri(g_strCuInCmuSess);
    tCuChn.SetChnNO(1);
    cCuSetupStreamReq.SetDstChn(tCuChn);

    int playMode = cusdk__VideoAudioBoth;

    //����ÿ��CU��IP
    cusdk__MediaNetInfo MediaNetInfo;
    cusdk__NetworkAddr NetWorkAddr;
    NetWorkAddr.ip = "172.16.64.95";
    NetWorkAddr.port = 12345;
    MediaNetInfo.videoRtpNatInfo.localAddr = NetWorkAddr;
    NetWorkAddr.port += 1;
    MediaNetInfo.videoRtcpNatInfo.localAddr = NetWorkAddr;
    NetWorkAddr.port += 1;
    MediaNetInfo.audioRtpNatInfo.localAddr = NetWorkAddr;
    NetWorkAddr.port += 1;
    MediaNetInfo.audioRtcpNatInfo.localAddr = NetWorkAddr;
    cusdk__MediaNetInfo *pCuMediaNetItem = &MediaNetInfo;
    if ( cusdk__VideoAudioBoth == playMode || cusdk__VideoOnly == playMode )
    {
        TMediaTransDesc tVideoMediaTransDesc;
        tVideoMediaTransDesc.SetMediaType(MEDIA_TYPE_VIDEO);
        //            TMediaTransProto tMediaTransProto;
        //            tMediaTransProto.SetTransport(MEDIA_TRANS_RTP);
        //            tVideoMediaTransDesc.SetMediaTransProto();

        TMediaTransChannel tVideoRtpChan;
        tVideoRtpChan.SetTransChnType(MEDIA_TRANS_CHN_RTP);
        tVideoRtpChan.SetMediaChannelKey(pCuMediaNetItem->videoRtpNatInfo.natData);
        TNetAddr tVideoRtpAddr;
        ConvertNetAddrFromCuToCui(pCuMediaNetItem->videoRtpNatInfo.localAddr, tVideoRtpAddr);
        tVideoRtpChan.GetAddrList().push_back(tVideoRtpAddr);
        if ( (! pCuMediaNetItem->videoRtpNatInfo.natAddr.ip.empty()) &&
            pCuMediaNetItem->videoRtpNatInfo.natAddr.port != 0 )
        {
            ConvertNetAddrFromCuToCui(pCuMediaNetItem->videoRtpNatInfo.natAddr, tVideoRtpAddr);
            tVideoRtpChan.GetAddrList().push_back(tVideoRtpAddr);
        }
        tVideoMediaTransDesc.GetMediaTransChnList().push_back(tVideoRtpChan);

        TMediaTransChannel tVideoRtcpChan;
        tVideoRtcpChan.SetTransChnType(MEDIA_TRANS_CHN_RTCP);
        tVideoRtcpChan.SetMediaChannelKey(pCuMediaNetItem->videoRtcpNatInfo.natData);
        TNetAddr tVideoRtcpAddr;
        ConvertNetAddrFromCuToCui(pCuMediaNetItem->videoRtcpNatInfo.localAddr, tVideoRtcpAddr);
        tVideoRtcpChan.GetAddrList().push_back(tVideoRtcpAddr);
        if ( ( ! pCuMediaNetItem->videoRtcpNatInfo.natAddr.ip.empty()) &&
            pCuMediaNetItem->videoRtcpNatInfo.natAddr.port != 0 )
        {
            ConvertNetAddrFromCuToCui(pCuMediaNetItem->videoRtcpNatInfo.natAddr, tVideoRtcpAddr);
            tVideoRtcpChan.GetAddrList().push_back(tVideoRtcpAddr);
        }

        tVideoMediaTransDesc.GetMediaTransChnList().push_back(tVideoRtcpChan);
        cCuSetupStreamReq.AddDstMediaTransDesc(tVideoMediaTransDesc);
    }
    if ( cusdk__VideoAudioBoth == playMode || cusdk__AudioOnly == playMode )
    {
        TMediaTransDesc tAudioMediaTransDesc;
        tAudioMediaTransDesc.SetMediaType(MEDIA_TYPE_AUDIO);

        TMediaTransChannel tAudioRtpChan;
        tAudioRtpChan.SetTransChnType(MEDIA_TRANS_CHN_RTP);
        tAudioRtpChan.SetMediaChannelKey(pCuMediaNetItem->audioRtpNatInfo.natData);
        TNetAddr tAudioRtpAddr;
        ConvertNetAddrFromCuToCui(pCuMediaNetItem->audioRtpNatInfo.localAddr, tAudioRtpAddr);
        tAudioRtpChan.GetAddrList().push_back(tAudioRtpAddr);
        if ( ( ! pCuMediaNetItem->audioRtpNatInfo.natAddr.ip.empty() ) &&
            pCuMediaNetItem->audioRtpNatInfo.natAddr.port != 0 )
        {
            ConvertNetAddrFromCuToCui(pCuMediaNetItem->audioRtpNatInfo.natAddr, tAudioRtpAddr);
            tAudioRtpChan.GetAddrList().push_back(tAudioRtpAddr);
        }
        tAudioMediaTransDesc.GetMediaTransChnList().push_back(tAudioRtpChan);

        TMediaTransChannel tAudioRtcpChan;
        tAudioRtcpChan.SetTransChnType(MEDIA_TRANS_CHN_RTCP);
        tAudioRtcpChan.SetMediaChannelKey(pCuMediaNetItem->audioRtcpNatInfo.natData);
        TNetAddr tAudioRtcpAddr;
        ConvertNetAddrFromCuToCui(pCuMediaNetItem->audioRtcpNatInfo.localAddr, tAudioRtcpAddr);
        tAudioRtcpChan.GetAddrList().push_back(tAudioRtcpAddr);
        if ( ( ! pCuMediaNetItem->audioRtcpNatInfo.natAddr.ip.empty() ) &&
            pCuMediaNetItem->audioRtcpNatInfo.natAddr.port != 0 )
        {
            ConvertNetAddrFromCuToCui(pCuMediaNetItem->audioRtcpNatInfo.natAddr, tAudioRtcpAddr);
            tAudioRtcpChan.GetAddrList().push_back(tAudioRtcpAddr);
        }
        tAudioMediaTransDesc.GetMediaTransChnList().push_back(tAudioRtcpChan);
        cCuSetupStreamReq.AddDstMediaTransDesc(tAudioMediaTransDesc);
    }

    CCuSetupStreamRsp cCuSetupStreamRsp;
    CuSetupStream( cCuSetupStreamReq, cCuSetupStreamRsp );
    if ( 0 == cCuSetupStreamRsp.GetErrorCode() )
    {
        //PlayStream();
        StopStream();
    }
    return;
}

void PlayStream()
{
    CCuPlayStreamReq cCuPlayStreamReq;
    cCuPlayStreamReq.SetSession(g_strCuInCmuSess);
    cCuPlayStreamReq.SetPlayId(1);

    CCuPlayStreamRsp cCuPlayStreamRsp;
    CuPlayStream(cCuPlayStreamReq, cCuPlayStreamRsp);
    return;
}

void StopStream()
{
    CCuStopStreamReq cCuStopStreamReq;
    cCuStopStreamReq.SetSession(g_strCuInCmuSess);
    cCuStopStreamReq.SetPlayId(1);

    CCuStopStreamRsp cCuStopStreamRsp;
    CuStopStream( cCuStopStreamReq, cCuStopStreamRsp );
    return;
}

