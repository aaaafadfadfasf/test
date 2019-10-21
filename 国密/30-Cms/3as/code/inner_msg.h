/*
�ڲ���Ϣ����ͷ�ļ�
*/

#ifndef _INNER_MSG_H_
#define _INNER_MSG_H_

#include "kdvtype.h"
#include "ospsip/ospsip.h"
#include "ospext/ospext.h"
#include "tas/tas_event.h"
#include "tas/tas_proto.h"
#include "tas/tas_struct.h"
#include "cmu/cmu_proto.h"

/* �ڲ�osp��Ϣ���� */
enum
{
    TAS_INNER_MSG_BEGIN = USER_INNER_MSG_BEGIN, 
    TAS_MODULE_SELF_MSG_BEGIN = TAS_INNER_MSG_BEGIN,   // TAS��ģ���������Ϣ��Χ������ģ�����ͨ��

    msgTaskMsgTransfer4Db,     // DbTaskPool��Ϣ������(����CCtrl��DbTaskPool֮��SIP��Ϣ�ķ�װ����), ��Ϣ�� CTaskMsgTransfer ��ַ
    msgDbMsgTransfer,          // DbTaskPool��Ϣ������(�����ڲ�ģ����DbTaskPool֮����Ϣ�Ĵ���)����Ϣ�� CDbMsgTransfer ��ַ

    msgPoweronReadyNtf,        // ģ���ϵ����֪ͨ, ��Ϣ�� ģ���ʶ
    msgPoweroffReadyNtf,       // ģ��ȥ�����֪ͨ����Ϣ�� ģ���ʶ
    msgTasExitCmd,             // tas�˳���Ϣ, ��Ϣ�� ��
    msgDump,                   // dump������Ϣ����Ӧ�ó�������

    msgConnectedNMSNtf,        // NMS����֪ͨ����Ϣ�� ��
    msgDisconnectNMSNtf,       // NMS����֪ͨ����Ϣ�� ��

    msgLicenseKeyCheckTimer,   // license key��ⶨʱ��
    msgLicenseUsageCheckTimer, // license ʹ��У�鶨ʱ��
    msgLicenseSDKCbReq,        // license SDK������������Ϣ�� CInnerLicenseSDKCBReq
    msgLicenseServertatusRefreshReq, //ˢ��license������״̬���� CInnerLicenseServerStatusRefreshReq
    msgLicenseUsageGetReq,     // license ʹ�������ȡ����, ��Ϣ�� ��
    msgLicenseUsageGetRsp,     // license ʹ�������ȡӦ����Ϣ�� CInnerLicenseUsageGetRsp

    msgCuserOnlineReq,  // CU�û���������, ��Ϣ�� CCuOnlineReq
    msgCuserOnlineRsp,  // CU�û�����Ӧ����Ϣ�� TCuOnlineRspExtern

    msgSyncDataRecordStoreReq,  // ͬ�����ݼ�¼�洢������Ϣ�� TSyncDataRecordStoreReq
    msgSyncDataRecordStoreRsp,  // ͬ�����ݼ�¼�洢Ӧ��, ��Ϣ�� TSyncDataRecordStoreRsp
    msgSyncDataRecordDelReq,    // ͬ�����ݼ�¼ɾ������, ��Ϣ�� TSyncDataRecordDelReq
    msgSyncDataRecordDelRsp,    // ͬ�����ݼ�¼ɾ��Ӧ��, ��Ϣ�� TSyncDataRecordDelRsp
    msgSyncDataConstructReq,    // ͬ�����ݹ���������Ϣ�� TSyncDataConstructReq
    msgSyncDataConstructRsp,    // ͬ�����ݹ���Ӧ����Ϣ�� TSyncDataConstructRsp
    msgSyncDataStoreReq,        // ͬ�����ݴ洢������Ϣ�� TSyncDataStoreReq
    msgSyncDataStoreRsp,        // ͬ�����ݴ洢Ӧ����Ϣ�� TSyncDataStoreRsp
    msgSyncDataChangeNtf,       // ͬ�����ݱ��֪ͨ�����յ����ƽ̨ͬ�����������ݣ����б��֪ͨ������Ϣ�� ���֪ͨ��Ϣ��ĵ�ַ
    msgSyncDomainAuthorizationDependenceGetReq,  // ͬ��ʱ������Ȩ��¼������Ϣ��ȡ����, ��Ϣ�� �豸���
    msgSyncDomainAuthorizationDependenceGetRsp,  // ͬ��ʱ������Ȩ��¼������Ϣ��ȡӦ��, ��Ϣ�� TDomainAuthorizationDependenceGetRsp
    msgSyncDevInfoRelateDataGetReq,  // ͬ��ʱ�豸������ݻ�ȡ���� ��Ϣ�� �豸���
    msgSyncDevInfoRelateDataGetRsp,  // ͬ��ʱ�豸������ݻ�ȡӦ�� ��Ϣ�� TSyncDevInfoRelateDataGetRsp

    msgPrivateDevIdListGetReq,  // ˽���豸��ȡ����, ��Ϣ�� ��
    msgPrivateDevIdListGetRsp,  // ˽���豸��ȡӦ����Ϣ��  TPrivateDevIdListGetRsp
    msgDomainRelationListGetReq, // ���ϵ�б��ȡ������Ϣ�� ��
    msgDomainRelationListGetRsp, // ���ϵ�б��ȡӦ����Ϣ�� TDomainRelationListGetRsp

    msgSyncDstDomainListGetReq,  // ͬ��Ŀ�����б��ȡ����, ��Ϣ�� ��
    msgSyncDstDomainListGetRsp,  // ͬ��Ŀ�����б��ȡӦ����Ϣ�� TSyncDstDomainListGetRsp
    msgDomainAuthorizationDevIdListGetReq,  // ������Ȩ�豸���б��ȡ����, ��Ϣ�� ָ��������
    msgDomainAuthorizationDevIdListGetRsp,  // ������Ȩ�豸���б��ȡӦ����Ϣ�� TDomainAuthorizationDevIdListGetRsp

    msgParentDomainSyncDataConfigAddReq, // ���Ӹ���ͬ����������, ��Ϣ�� TParentDomainSyncDataConfigAddReq
    msgParentDomainSyncDataConfigAddRsp, // ���Ӹ���ͬ������Ӧ��, ��Ϣ�� CInnerRspBase
    msgParentDomainSyncDataConfigDelReq, // ɾ������ͬ������������Ϣ�� ��
    msgParentDomainSyncDataConfigDelRsp, // ɾ������ͬ������Ӧ����Ϣ�� CInnerRspBase
    msgDeleteAllDataFromSubDomainReq,    // ɾ��ָ�������¼�����������, ��Ϣ�� ָ������
    msgDeleteAllDataFromSubDomainRsp,    // ɾ��ָ�������¼�����������, ��Ϣ�� CInnerRspBase

    msgLicenseUpdateNtfReq, // license����֪ͨ����Ϣ�� CLicenseUpdateNtfReq���л�

	msgNtfSendTimer,                     // Ntf��Ϣ�������Ͷ�ʱ����Ϣ

	msgSetDevGbidReq,                    // �����豸��gbid������Ϣ�壺TSetDevGbidReq
	msgSetDevGbidRsp,                    // �����豸��gbid������Ϣ�壺TSetDevGbidRsp

	msgSetDevGrpGbidReq,              // �����豸���gbid������Ϣ�壺TSetDevGrpGbidReq
	msgSetDevGrpGbidRsp,              // �����豸���gbidӦ����Ϣ�壺TSetDevGrpGbidRsp

	msgMatchChildrenGrpGbidReq,          // ���ݸ�����gbidƥ��������������gbid����Ϣ�壺TMatchChildrenGrpGbidReq
	msgMatchChildrenGrpGbidRsp,          // ���ݸ�����gbidƥ��������������gbid, ��Ϣ�壺TMatchChildrenGrpGbidRsp

	msgQryGbidEmptyDevsReq,              // ��ѯָ���豸����gbid�ֶ�Ϊ�յ���ƵԴ,��Ϣ��:TQryGbidEmptyDevsReq
	msgQryGbidEmptyDevsRsp,              // ��ѯָ���豸����gbid�ֶ�Ϊ�յ���ƵԴ,��Ϣ��:TQryGbidEmptyDevsRsp
	msgQryGbidEmptyDevsNtf,              // ��ѯָ���豸����gbid�ֶ�Ϊ�յ���ƵԴ,��Ϣ��:TQryGbidEmptyDevsNtf


	msgDevGrp2DevGbidOperateReq,         //�����豸��gbid������Ϣ��CDevGrp2DevGbidOperateReq
	msgDevGrp2DevGbidOperateRsp,         //�����豸��gbid������Ϣ��CDevGrp2DevGbidOperateRsp

	msgDevGrpGbidOperateReq,             //�����豸�����gbid������Ϣ��CDevGrpGbidOperateReq
	msgDevGrpGbidOperateRsp,             //�����豸�����gbid������Ϣ��CDevGrpGbidOperateReq

	msgDevGrpAllChildDevQryReq,          //���������������豸��ѯ����Ϣ��CDevGrpAllChildDevQryReq
	msgDevGrpAllChildDevQryRsp,          //���������������豸��ѯ����Ϣ��CDevGrpAllChildDevQryRsp

	msgTableDataToPushXmlReq,           
	msgTableDataToPushXmlRsp,

    msgQryDevidReq, // ��ѯ�豸id�б� ��Ϣ�� CTaskMsgTransfer<CDeviceQryReq>
    msgQryDevidRsp, // ��ѯ�豸id�б� ��Ϣ�� CTaskMsgTransfer< pair< int, vector<string> > >

    msgSyncAnalyzeCmd,  // ����ͬ�������Ϣ��ΪTInnerSyncDataInfo��ַ
    msgSyncAnalyzerReadyExitDetectTimer, // ͬ��������ϵͳ�˳�����̽�ⶨʱ��, ��Ϣ��Ϊ��
    msgSyncAnalyzerParentDel,
    msgSyncAnalyzerParentAdd,  // ��Ϣ�� UUIDString��ַ
    msgSyncMngSubdomainAddNtf, // ��Ϣ�� subdomainId�ַ�������
    msgSyncMngSubdomainDelNtf, // ��Ϣ�� subdomainId�ַ�������

    TAS_INNER_MSG_END = TAS_INNER_MSG_BEGIN + 1000
};

inline void InitTasInnerEventDesc()
{
    static bool bInit = false;
    if(bInit)
    {
        return;
    }
    bInit = true;

    OSP_ADD_EVENT_DESC(msgTaskMsgTransfer4Db);
    OSP_ADD_EVENT_DESC(msgDbMsgTransfer);

    OSP_ADD_EVENT_DESC(msgPoweronReadyNtf);
    OSP_ADD_EVENT_DESC(msgPoweroffReadyNtf);
    OSP_ADD_EVENT_DESC(msgTasExitCmd);
    OSP_ADD_EVENT_DESC(msgDump);

    OSP_ADD_EVENT_DESC(msgConnectedNMSNtf);
    OSP_ADD_EVENT_DESC(msgDisconnectNMSNtf);

    OSP_ADD_EVENT_DESC(msgLicenseKeyCheckTimer);
    OSP_ADD_EVENT_DESC(msgLicenseUsageCheckTimer);

    OSP_ADD_EVENT_DESC(msgLicenseSDKCbReq);
    OSP_ADD_EVENT_DESC(msgLicenseServertatusRefreshReq);
     
    OSP_ADD_EVENT_DESC(msgLicenseUsageGetReq);
    OSP_ADD_EVENT_DESC(msgLicenseUsageGetRsp);

    OSP_ADD_EVENT_DESC(msgCuserOnlineReq);
    OSP_ADD_EVENT_DESC(msgCuserOnlineRsp);

    OSP_ADD_EVENT_DESC(msgSyncDataRecordStoreReq);
    OSP_ADD_EVENT_DESC(msgSyncDataRecordStoreRsp);
    OSP_ADD_EVENT_DESC(msgSyncDataRecordDelReq);
    OSP_ADD_EVENT_DESC(msgSyncDataRecordDelRsp);
    OSP_ADD_EVENT_DESC(msgSyncDataConstructReq);
    OSP_ADD_EVENT_DESC(msgSyncDataConstructRsp);
    OSP_ADD_EVENT_DESC(msgSyncDataStoreReq);
    OSP_ADD_EVENT_DESC(msgSyncDataStoreRsp);
    OSP_ADD_EVENT_DESC(msgSyncDomainAuthorizationDependenceGetReq);
    OSP_ADD_EVENT_DESC(msgSyncDomainAuthorizationDependenceGetRsp);
    OSP_ADD_EVENT_DESC(msgSyncDevInfoRelateDataGetReq);
    OSP_ADD_EVENT_DESC(msgSyncDevInfoRelateDataGetRsp);

    OSP_ADD_EVENT_DESC(msgPrivateDevIdListGetReq);
    OSP_ADD_EVENT_DESC(msgPrivateDevIdListGetRsp);
    OSP_ADD_EVENT_DESC(msgDomainRelationListGetReq);
    OSP_ADD_EVENT_DESC(msgDomainRelationListGetRsp);

    OSP_ADD_EVENT_DESC(msgSyncDstDomainListGetReq);
    OSP_ADD_EVENT_DESC(msgSyncDstDomainListGetRsp);
    OSP_ADD_EVENT_DESC(msgDomainAuthorizationDevIdListGetReq);
    OSP_ADD_EVENT_DESC(msgDomainAuthorizationDevIdListGetRsp);

    OSP_ADD_EVENT_DESC(msgParentDomainSyncDataConfigAddReq);
    OSP_ADD_EVENT_DESC(msgParentDomainSyncDataConfigAddRsp);
    OSP_ADD_EVENT_DESC(msgParentDomainSyncDataConfigDelReq);
    OSP_ADD_EVENT_DESC(msgParentDomainSyncDataConfigDelRsp);
    OSP_ADD_EVENT_DESC(msgDeleteAllDataFromSubDomainReq);
    OSP_ADD_EVENT_DESC(msgDeleteAllDataFromSubDomainRsp);
    OSP_ADD_EVENT_DESC(msgLicenseUpdateNtfReq);
	OSP_ADD_EVENT_DESC(msgNtfSendTimer);
	OSP_ADD_EVENT_DESC(msgSetDevGbidReq);
	OSP_ADD_EVENT_DESC(msgSetDevGbidRsp);
	OSP_ADD_EVENT_DESC(msgSetDevGrpGbidReq);
	OSP_ADD_EVENT_DESC(msgSetDevGrpGbidRsp);
	OSP_ADD_EVENT_DESC(msgMatchChildrenGrpGbidReq);
	OSP_ADD_EVENT_DESC(msgMatchChildrenGrpGbidRsp);
	OSP_ADD_EVENT_DESC(msgQryGbidEmptyDevsReq);
	OSP_ADD_EVENT_DESC(msgQryGbidEmptyDevsRsp);
	OSP_ADD_EVENT_DESC(msgQryGbidEmptyDevsNtf);

	OSP_ADD_EVENT_DESC(msgDevGrp2DevGbidOperateReq);
	OSP_ADD_EVENT_DESC(msgDevGrp2DevGbidOperateRsp);
	OSP_ADD_EVENT_DESC(msgDevGrpGbidOperateReq);
	OSP_ADD_EVENT_DESC(msgDevGrpGbidOperateRsp);
	OSP_ADD_EVENT_DESC(msgDevGrpAllChildDevQryReq);
	OSP_ADD_EVENT_DESC(msgDevGrpAllChildDevQryRsp);

	OSP_ADD_EVENT_DESC(msgTableDataToPushXmlReq);
	OSP_ADD_EVENT_DESC(msgTableDataToPushXmlRsp);

    OSP_ADD_EVENT_DESC(msgQryDevidReq);
    OSP_ADD_EVENT_DESC(msgQryDevidRsp);

    OSP_ADD_EVENT_DESC(msgSyncAnalyzeCmd);
    OSP_ADD_EVENT_DESC(msgSyncAnalyzerReadyExitDetectTimer);
    OSP_ADD_EVENT_DESC(msgSyncAnalyzerParentDel);
    OSP_ADD_EVENT_DESC(msgSyncAnalyzerParentAdd);
    OSP_ADD_EVENT_DESC(msgSyncMngSubdomainAddNtf);
    OSP_ADD_EVENT_DESC(msgSyncMngSubdomainDelNtf);
}

class CInnerRspBase
{
public:
    CInnerRspBase():m_nErrno(0){}
    s32 GetErrno() const{return m_nErrno;}
    void SetErrno(s32 nErrno){m_nErrno = nErrno;}
private:
    s32 m_nErrno; // Ĭ��0��ʾ�ɹ�
};

class CTaskMsgTransferBase: public COspObj
{
public:
    CTaskMsgTransferBase(u32 dwTaskNo, const TKDSIP_TRANS_ID &tTransId, u16 wEventId)
        : m_pMsgObj(NULL), m_dwOspTaskNo(dwTaskNo), m_tSipTransID(tTransId), m_wEventId(wEventId)
    {
    }
    void SetTransfer(const CTaskMsgTransferBase &cSrcTransfer, u16 wNewEventId)
    {
        m_dwOspTaskNo = cSrcTransfer.m_dwOspTaskNo;
        m_tSipTransID = cSrcTransfer.m_tSipTransID;
        m_wEventId = wNewEventId;
    }
    u32 GetOspTaskNo() const{return m_dwOspTaskNo;}
    TKDSIP_TRANS_ID GetSipTransId() const{return m_tSipTransID;}
    u16 GetEventId() const{return m_wEventId;}
    void* GetMsgObjAddr() const {return m_pMsgObj;}
protected:
    void *m_pMsgObj;
private:
    u32 m_dwOspTaskNo;
    TKDSIP_TRANS_ID m_tSipTransID;
    u16 m_wEventId;
};

template<typename msgtype>
class CTaskMsgTransfer: public CTaskMsgTransferBase
{
public:
    CTaskMsgTransfer(): CTaskMsgTransferBase(0, TKDSIP_TRANS_ID(), 0)
    {
        m_pMsgObj = &m_tMsgObj;
    }
    CTaskMsgTransfer(u32 dwTaskNo, const TKDSIP_TRANS_ID &tTransId, u16 wEventId)
        : CTaskMsgTransferBase(dwTaskNo, tTransId, wEventId)
    {
        m_pMsgObj = &m_tMsgObj;
    }
    CTaskMsgTransfer<msgtype>& operator=(const CTaskMsgTransfer<msgtype> &rh)
    {
        if (this == &rh)
        {
            return *this;
        }

        CTaskMsgTransferBase::operator=(rh);
        m_pMsgObj = &m_tMsgObj;
        m_tMsgObj = rh.m_tMsgObj;
        return *this;
    }
    msgtype& GetMsgObj() {return m_tMsgObj;}
    const msgtype& GetMsgObj() const{return m_tMsgObj;}
	void SetMsgObj(const msgtype &obj) {m_tMsgObj = obj;}
private:
    msgtype m_tMsgObj;
};

class CDbMsgTransferBase: public COspObj
{
public:
    CDbMsgTransferBase(u32 dwOspIID, u32 dwTaskNo, u16 wEventId)
        : m_pMsgObj(NULL), m_dwOspIID(dwOspIID), m_dwOspTaskNo(dwTaskNo), m_wEventId(wEventId)
    {
    }
    CDbMsgTransferBase(const CDbMsgTransferBase &rh)
        : m_pMsgObj(rh.m_pMsgObj), m_dwOspIID(rh.m_dwOspIID), m_dwOspTaskNo(rh.m_dwOspTaskNo), m_wEventId(rh.m_wEventId)
    {
    }

    u32 GetOspIID() const{return m_dwOspIID;}
    u32 GetOspTaskNo() const{return m_dwOspTaskNo;}
    u16 GetEventId() const{return m_wEventId;}
    void* GetMsgObjAddr() const {return m_pMsgObj;}
protected:
    void *m_pMsgObj;
private:
    u32 m_dwOspIID;
    u32 m_dwOspTaskNo;
    u16 m_wEventId;
};

template<typename msgtype>
class CDbMsgTransfer: public CDbMsgTransferBase
{
public:
    CDbMsgTransfer(): CDbMsgTransferBase(0, 0, 0)
    {
        m_pMsgObj = &m_tMsgObj;
    }
    CDbMsgTransfer(u32 dwOspIID, u32 dwTaskNo, u16 wEventId)
        : CDbMsgTransferBase(dwOspIID, dwTaskNo, wEventId)
    {
        m_pMsgObj = &m_tMsgObj;
    }
    CDbMsgTransfer(const CDbMsgTransfer &rh)
        : CDbMsgTransferBase(rh), m_tMsgObj(rh.m_tMsgObj)
    {
    }
    CDbMsgTransfer(const CDbMsgTransferBase &reqTrans, u16 wNewEventId)
        : CDbMsgTransferBase(reqTrans.GetOspIID(), reqTrans.GetOspTaskNo(), wNewEventId)
    {
    }
    CDbMsgTransfer<msgtype>& operator=(const CDbMsgTransfer<msgtype> &rh)
    {
        if (this == &rh)
        {
            return *this;
        }

        CDbMsgTransferBase::operator=(rh);
        m_pMsgObj = &m_tMsgObj;
        m_tMsgObj = rh.m_tMsgObj;
        return *this;
    }
    msgtype& GetMsgObj() {return m_tMsgObj;}
    const msgtype& GetMsgObj() const{return m_tMsgObj;}
private:
    msgtype m_tMsgObj;
};

template<>
class CDbMsgTransfer<void>: public CDbMsgTransferBase
{
public:
    CDbMsgTransfer(): CDbMsgTransferBase(0, 0, 0)
    {
        m_tMsgObj = 0;
    }
    CDbMsgTransfer(u32 dwOspIID, u32 dwTaskNo, u16 wEventId)
        : CDbMsgTransferBase(dwOspIID, dwTaskNo, wEventId)
    {
        m_tMsgObj = 0;
    }
    CDbMsgTransfer<void>& operator=(const CDbMsgTransfer<void> &rh)
    {
        if (this == &rh)
        {
            return *this;
        }

        CDbMsgTransferBase::operator=(rh);
        m_pMsgObj = NULL;
        m_tMsgObj = 0;
        return *this;
    }
private:
    u16& GetMsgObj();
    const u16& GetMsgObj() const;
private:
    u16 m_tMsgObj;  // ����Ϣ����ʱʹ��u16��Ϊһ��ռλ��
};

struct TOtherPULicenceInfo
{
    TOtherPULicenceInfo(const string &strManufacture="", u32 dwLicenseNum=0)
        :m_strManufacture(strManufacture), m_dwLicenseNum(dwLicenseNum)
    {}
    string m_strManufacture; // ��������
    u32 m_dwLicenseNum;      // license����
};

struct TLicenseUsageInfo
{
    TLicenseUsageInfo(){Init();}
    void Init()
    {
        m_dwPUNum = 0;
        m_atOtherPULicenseInfos.clear();
        m_dwManagePUNum = 0;
    }
    bool IsNotLessLicense(const TLicenseUsageInfo &tLicense);
    u32  OtherPUTotalLicense();
    // �ڶ���������ΪLicense��������
    u32  OtherPULicense(const string &strManufacture, const vector<TOtherPULicenceInfo> &vecOtherPuLimit);
    void AddOtherPULicense(const string &strManufacture, s32 nLicenseNum);

    u32 m_dwPUNum;    
    vector<TOtherPULicenceInfo> m_atOtherPULicenseInfos;
    u32 m_dwManagePUNum;
};

struct TCuOnlineRspExtern
{
    CCuOnlineRsp cRsp;
    string UserName;
    string UserId;
    bool MultiLogin;
};

struct TSyncDataAnalyzeInfo
{
    TSyncDataAnalyzeInfo(){}
    TSyncDataAnalyzeInfo(ENSyncDataType dataType_, ENSyncOperateType operateType_, 
        const string &domainId_, const string &dataId_)
        :dataType(dataType_), operateType(operateType_), domainId(domainId_), dataId(dataId_){}

    ENSyncDataType dataType;       // ͬ�����ݵ�����
    ENSyncOperateType operateType; // ��������
    string domainId;               // ͬ��������������
    string dataId;                 // ͬ�����ݱ���ַ���
};

struct TSyncDataRecordInfo
{
    TSyncDataRecordInfo(){}
    TSyncDataRecordInfo(int sn_, ENSyncDataType dataType_, const string &dataId_,
        ENSyncOperateType operateType_, const string &dataDomainId_)
    :sn(sn_), dataType(dataType_), dataId(dataId_), operateType(operateType_), dataDomainId(dataDomainId_)
    {}

    int sn;
    ENSyncDataType dataType;
    string dataId;
    ENSyncOperateType operateType;
    string dataDomainId;
};

struct TSyncDataRecordStoreReq
{
    TSyncDataRecordStoreReq(){}
    TSyncDataRecordStoreReq(const string &dstDomainId_, ENSyncType enSyncType_, ENSyncDataType dataType_, 
        const string &dataId_, ENSyncOperateType operateType_, const string &dataDomainId_)
        :dstDomainId(dstDomainId_), enSyncType(enSyncType_), info(0, dataType_, dataId_, operateType_, dataDomainId_)
    {
    }

    string dstDomainId;    // Ŀ������
    ENSyncType enSyncType; // ͬ������
    TSyncDataRecordInfo info;
};

struct TSyncDataRecordStoreRsp: public CInnerRspBase
{
};

struct TSyncDataRecordDelReq
{
    string dstDomainId;    // Ŀ������
    ENSyncType enSyncType; // ͬ������
    vector<u32> sns;       // sn����
};

struct TSyncDataRecordDelRsp: public CInnerRspBase
{
    // ʧ��ʱֻ����һ����������������ϸ������Щʧ����; ���������������
};

struct TSyncDataConstructReq
{
    ENSyncType enSyncType;
    string dstDomainId;     // Ŀ������
    u32 dwExpectedDataNum;  // �����������������
};

struct TSyncDataConstructRsp: public CInnerRspBase
{
    vector<TSyncDataInfo> results;
};

struct TSyncDataStoreReq
{
    ENSyncType enSyncType;
    vector<TSyncDataInfo> data;
};

struct TSyncDataStoreRsp: public CInnerRspBase
{
   vector<TSyncDataResult> results; 
};

struct TPrivateDevIdListGetRsp: public CInnerRspBase
{
    set<string> devIds;
};

struct TDomainRelationListGetRsp: public CInnerRspBase
{
    vector<CDomainRelationInfo> domainRelationList;
};

struct TSyncDstDomainListGetRsp: public CInnerRspBase
{
    vector<TSyncDstDomainInfo> dstDomainList;
};

struct TDomainAuthorizationDevIdListGetRsp: public CInnerRspBase
{
    vector<string> devIdList;
};

struct CInnerLicenseSDKCBReq
{
    s32 nMsg;
};

struct CInnerLicenseServerStatusRefreshReq
{
    s32 nMsg;
};
class CInnerLicenseUsageGetRsp: public CInnerRspBase
{
public:
    TLicenseUsageInfo m_tLicenseUsageInfo;
};

struct TSyncDomainAuthorizationDependenceGetRsp: public CInnerRspBase
{
    UUIDString devModelId;
    UUIDString devId;
    vector<TDeviceCapIndexs> devCapIndexs;
};

struct TSyncDevInfoRelateDataGetRsp: public CInnerRspBase
{
    UUIDString devId;
    vector<TDeviceCapIndexs> devCapIndexs;           // �豸������������
    vector<CDeviceGroupDeviceInfoKey> devGrpDevKeys; // �豸�����豸����
};

struct TParentDomainSyncDataConfigAddReq
{
    string parentDomainId;
    string parentDomainName;
};


struct TDevGbId
{
	UUIDString devId;
	int vidSrcIdx;    //�豸ID
	int operType;     //�޸����ͣ�0-���޸�����gbid��1-���޸ĸ���gbid��2-������gbid���޸�
	string priGbid;
	string secGbid;
};

struct TSetDevGbidReq
{
	UUIDString devTreeId;
	UUIDString devGrpId;
	vector<TDevGbId> DevGbids;
};

struct TSetDevGbidRsp:public CInnerRspBase
{
	//������Ҫ�ֶ�
};

struct TSetDevGrpGbidReq
{
	TSetDevGrpGbidReq()
	{
		clear();
	}
	void clear()
	{
		devTreeId.clear();
		devGrpGbid.clear();
		devGrpSeq = 0;
		devGrpGbid.clear();
	}
	UUIDString devTreeId;      //����ָ������µ����ݱ�
	UUIDString devGrpId;       //����ָ������µķ���(Ϊ�ձ�ʾ������)
	double     devGrpSeq;      //����ָ������µķ���grpseq�ֶΣ��޷��޸ģ�
	string     devGrpGbid;     //ԭʼ�����Ӧ��gbid
};

struct TSetDevGrpGbidRsp:public CInnerRspBase
{
	//������Ҫ�ֶ�
};

struct TMatchChildrenGrpGbidReq
{
	UUIDString devTreeId;
	UUIDString devParentGrpId;
	string     devParentGrpGbid;
};

struct TMatchChildrenGrpGbidRsp:public CInnerRspBase
{
	//������Ҫ�ֶ�
};

struct TQryGbidEmptyDevsReq
{
	UUIDString devTreeId;
};

struct TQryGbidEmptyDevsRsp:public CInnerRspBase
{
	TQryGbidEmptyDevsRsp()
	{
		totalNum = -1;
	}
	int totalNum;
};

struct TVidSrcGbInfo
{
	TVidSrcGbInfo()
	{
		devId.clear();
		vidIndex = -1;
	}
	UUIDString devGrpId;
	UUIDString devId;
	int vidIndex;
};

struct TErrItem
{
	UUIDString devTreeId;
	UUIDString devGrpId;
	UUIDString devId;
	int        vidIdx;
	TDevGbidErrType errType;

	bool operator==(const TErrItem& rh) const
	{
		return (devTreeId == rh.devTreeId && devGrpId == rh.devGrpId && 
			devId == rh.devId && vidIdx == rh.vidIdx && errType == rh.errType);
	}

};

struct TVidSrcGbInfoExt
{
	TVidSrcGbInfoExt()
	{
		clear();
	}
	void clear()
	{
		devId.clear();
		vidIndex = -1;
		newPrimaryGbid.clear();
		oldPrimaryGbid.clear();
		newSecondaryGbid.clear();
		oldSecondaryGbid.clear();
	}

	UUIDString devId;
	int vidIndex;
	string newPrimaryGbid;
	string oldPrimaryGbid;
	string newSecondaryGbid;
	string oldSecondaryGbid;
};

struct TQryGbidEmptyDevsNtf
{
	TQryGbidEmptyDevsNtf()
	{
		lastNtf = false;
	}
	bool lastNtf;
	vector<TVidSrcGbInfo> vidSrcGbInfos;
};


struct TQryDevGbidReq
{
	vector<UUIDString> devIds; 
};

struct TQryDevGbidRsp:CInnerRspBase
{
	map<UUIDString, string> devGbIds;//devIds--->devGbid;
};

struct TCheckErrGbTree
{
	UUIDString devTreeId;
};

struct TCheckErrGbDev
{
	UUIDString devTreeId;
	UUIDString devGrpId;
	UUIDString devId;
	vector<int> vidSrcIdx;
};

enum EnumGbidOperateType
{
	OPERATE_TYPE_NULL,
	OPERATE_TYPE_ADD_PRIMARY,
	OPERATE_TYPE_DEL_PRIMARY,
	OPERATE_TYPE_MOD_PRIMARY,
	OPERATE_TYPE_ADD_SECOND,
	OPERATE_TYPE_DEL_SECOND,
	OPERATE_TYPE_MOD_SECOND,
	OPERATE_TYPE_BATCH_DEL_PRIMARY,
	OPERATE_TYPE_BATCH_DEL_SECOND,
};
struct TOperateGbDevInfo
{
	UUIDString devGrpId;
	UUIDString devId;
	int vidSrcIdx;   
	EnumGbidOperateType operType;     
	string gbid;
};
class CDevGrp2DevGbidOperateReq
{
public:
	UUIDString devTreeId;
	vector<TOperateGbDevInfo> devs;

};
class CDevGrp2DevGbidOperateRsp : public CInnerRspBase
{
public:
	vector<string> gbids;
};
class CDevGrpGbidOperateReq
{
public:
	UUIDString devTreeId;
	UUIDString devGrpId;
	UUIDString devGrpGbid;

};
class CDevGrpGbidOperateRsp : public CInnerRspBase
{
};

class CDevGrpAllChildDevQryReq
{
public:
	UUIDString devTreeId;
	UUIDString devGrpId;
};
class CDevGrpAllChildDevQryRsp : public CInnerRspBase
{
public:                             
	vector<CDeviceGroupDeviceInfo4Gb> devInfos;  
};


struct TUserLoginInfo
{
	TUserLoginInfo()
	{
		nInputPwdErrorTimes = 0;
	}

	string    loginIp;
	CCMSTime  cLoginCMSTime;
	s32       nInputPwdErrorTimes;
	CCMSTime  cLockCMSTime;	
};

class CUserPwdLockMng
{
public:
	void UnlockAllUserPwd();
	void EraseFromLoginInfo(string strUserName, string strLoginIp);
	void ProcPwdError(string strUserName, string strLoginIp);
	u32 IsPwdLocked(const string &strUserName, const string &strLoginIp);
private:
	map<string, vector<TUserLoginInfo> > m_mapUserLoginInfo;
};

extern CUserPwdLockMng g_cUserPwdLockMng;

#endif // _INNER_MSG_H_
