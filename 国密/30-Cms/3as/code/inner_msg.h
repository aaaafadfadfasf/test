/*
内部消息定义头文件
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

/* 内部osp消息定义 */
enum
{
    TAS_INNER_MSG_BEGIN = USER_INNER_MSG_BEGIN, 
    TAS_MODULE_SELF_MSG_BEGIN = TAS_INNER_MSG_BEGIN,   // TAS的模块自身的消息范围（不在模块间流通）

    msgTaskMsgTransfer4Db,     // DbTaskPool消息传输器(用于CCtrl与DbTaskPool之间SIP消息的封装传输), 消息体 CTaskMsgTransfer 地址
    msgDbMsgTransfer,          // DbTaskPool消息传输器(用于内部模块与DbTaskPool之间消息的传输)，消息体 CDbMsgTransfer 地址

    msgPoweronReadyNtf,        // 模块上电就绪通知, 消息体 模块标识
    msgPoweroffReadyNtf,       // 模块去电就绪通知，消息体 模块标识
    msgTasExitCmd,             // tas退出消息, 消息体 空
    msgDump,                   // dump请求，消息体视应用场景而定

    msgConnectedNMSNtf,        // NMS建链通知，消息体 空
    msgDisconnectNMSNtf,       // NMS断链通知，消息体 空

    msgLicenseKeyCheckTimer,   // license key检测定时器
    msgLicenseUsageCheckTimer, // license 使用校验定时器
    msgLicenseSDKCbReq,        // license SDK服务器请求，消息体 CInnerLicenseSDKCBReq
    msgLicenseServertatusRefreshReq, //刷新license服务器状态请求 CInnerLicenseServerStatusRefreshReq
    msgLicenseUsageGetReq,     // license 使用情况获取请求, 消息体 空
    msgLicenseUsageGetRsp,     // license 使用情况获取应答，消息体 CInnerLicenseUsageGetRsp

    msgCuserOnlineReq,  // CU用户上线请求, 消息体 CCuOnlineReq
    msgCuserOnlineRsp,  // CU用户上线应答，消息体 TCuOnlineRspExtern

    msgSyncDataRecordStoreReq,  // 同步数据记录存储请求，消息体 TSyncDataRecordStoreReq
    msgSyncDataRecordStoreRsp,  // 同步数据记录存储应答, 消息体 TSyncDataRecordStoreRsp
    msgSyncDataRecordDelReq,    // 同步数据记录删除请求, 消息体 TSyncDataRecordDelReq
    msgSyncDataRecordDelRsp,    // 同步数据记录删除应答, 消息体 TSyncDataRecordDelRsp
    msgSyncDataConstructReq,    // 同步数据构造请求，消息体 TSyncDataConstructReq
    msgSyncDataConstructRsp,    // 同步数据构造应答，消息体 TSyncDataConstructRsp
    msgSyncDataStoreReq,        // 同步数据存储请求，消息体 TSyncDataStoreReq
    msgSyncDataStoreRsp,        // 同步数据存储应答，消息体 TSyncDataStoreRsp
    msgSyncDataChangeNtf,       // 同步数据变更通知（接收到别的平台同步过来的数据，进行变更通知），消息体 变更通知消息体的地址
    msgSyncDomainAuthorizationDependenceGetReq,  // 同步时跨域授权记录依赖信息获取请求, 消息体 设备编号
    msgSyncDomainAuthorizationDependenceGetRsp,  // 同步时跨域授权记录依赖信息获取应答, 消息体 TDomainAuthorizationDependenceGetRsp
    msgSyncDevInfoRelateDataGetReq,  // 同步时设备相关数据获取请求， 消息体 设备编号
    msgSyncDevInfoRelateDataGetRsp,  // 同步时设备相关数据获取应答， 消息体 TSyncDevInfoRelateDataGetRsp

    msgPrivateDevIdListGetReq,  // 私有设备获取请求, 消息体 空
    msgPrivateDevIdListGetRsp,  // 私有设备获取应答，消息体  TPrivateDevIdListGetRsp
    msgDomainRelationListGetReq, // 域关系列表获取请求，消息体 空
    msgDomainRelationListGetRsp, // 域关系列表获取应答，消息体 TDomainRelationListGetRsp

    msgSyncDstDomainListGetReq,  // 同步目标域列表获取请求, 消息体 空
    msgSyncDstDomainListGetRsp,  // 同步目标域列表获取应答，消息体 TSyncDstDomainListGetRsp
    msgDomainAuthorizationDevIdListGetReq,  // 跨域授权设备号列表获取请求, 消息体 指定的域名
    msgDomainAuthorizationDevIdListGetRsp,  // 跨域授权设备号列表获取应答，消息体 TDomainAuthorizationDevIdListGetRsp

    msgParentDomainSyncDataConfigAddReq, // 增加父域同步配置请求, 消息体 TParentDomainSyncDataConfigAddReq
    msgParentDomainSyncDataConfigAddRsp, // 增加父域同步配置应答, 消息体 CInnerRspBase
    msgParentDomainSyncDataConfigDelReq, // 删除父域同步配置请求，消息体 空
    msgParentDomainSyncDataConfigDelRsp, // 删除父域同步配置应答，消息体 CInnerRspBase
    msgDeleteAllDataFromSubDomainReq,    // 删除指定域及其下级的所有数据, 消息体 指定域编号
    msgDeleteAllDataFromSubDomainRsp,    // 删除指定域及其下级的所有数据, 消息体 CInnerRspBase

    msgLicenseUpdateNtfReq, // license更新通知，消息体 CLicenseUpdateNtfReq序列化

	msgNtfSendTimer,                     // Ntf消息分批发送定时器消息

	msgSetDevGbidReq,                    // 设置设备的gbid请求，消息体：TSetDevGbidReq
	msgSetDevGbidRsp,                    // 设置设备的gbid请求，消息体：TSetDevGbidRsp

	msgSetDevGrpGbidReq,              // 设置设备组的gbid请求，消息体：TSetDevGrpGbidReq
	msgSetDevGrpGbidRsp,              // 设置设备组的gbid应答，消息体：TSetDevGrpGbidRsp

	msgMatchChildrenGrpGbidReq,          // 根据父分组gbid匹配所有子孙分组的gbid，消息体：TMatchChildrenGrpGbidReq
	msgMatchChildrenGrpGbidRsp,          // 根据父分组gbid匹配所有子孙分组的gbid, 消息体：TMatchChildrenGrpGbidRsp

	msgQryGbidEmptyDevsReq,              // 查询指定设备树中gbid字段为空的视频源,消息体:TQryGbidEmptyDevsReq
	msgQryGbidEmptyDevsRsp,              // 查询指定设备树中gbid字段为空的视频源,消息体:TQryGbidEmptyDevsRsp
	msgQryGbidEmptyDevsNtf,              // 查询指定设备树中gbid字段为空的视频源,消息体:TQryGbidEmptyDevsNtf


	msgDevGrp2DevGbidOperateReq,         //操作设备的gbid请求，消息体CDevGrp2DevGbidOperateReq
	msgDevGrp2DevGbidOperateRsp,         //操作设备的gbid请求，消息体CDevGrp2DevGbidOperateRsp

	msgDevGrpGbidOperateReq,             //操作设备分组的gbid请求，消息体CDevGrpGbidOperateReq
	msgDevGrpGbidOperateRsp,             //操作设备分组的gbid请求，消息体CDevGrpGbidOperateReq

	msgDevGrpAllChildDevQryReq,          //分组下所有子孙设备查询，消息体CDevGrpAllChildDevQryReq
	msgDevGrpAllChildDevQryRsp,          //分组下所有子孙设备查询，消息体CDevGrpAllChildDevQryRsp

	msgTableDataToPushXmlReq,           
	msgTableDataToPushXmlRsp,

    msgQryDevidReq, // 查询设备id列表 消息体 CTaskMsgTransfer<CDeviceQryReq>
    msgQryDevidRsp, // 查询设备id列表 消息体 CTaskMsgTransfer< pair< int, vector<string> > >

    msgSyncAnalyzeCmd,  // 数据同步命令，消息体为TInnerSyncDataInfo地址
    msgSyncAnalyzerReadyExitDetectTimer, // 同步分析子系统退出就绪探测定时器, 消息体为空
    msgSyncAnalyzerParentDel,
    msgSyncAnalyzerParentAdd,  // 消息体 UUIDString地址
    msgSyncMngSubdomainAddNtf, // 消息体 subdomainId字符串内容
    msgSyncMngSubdomainDelNtf, // 消息体 subdomainId字符串内容

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
    s32 m_nErrno; // 默认0表示成功
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
    u16 m_tMsgObj;  // 无消息对象时使用u16作为一个占位符
};

struct TOtherPULicenceInfo
{
    TOtherPULicenceInfo(const string &strManufacture="", u32 dwLicenseNum=0)
        :m_strManufacture(strManufacture), m_dwLicenseNum(dwLicenseNum)
    {}
    string m_strManufacture; // 厂商名称
    u32 m_dwLicenseNum;      // license点数
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
    // 第二个参数是为License厂商限制
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

    ENSyncDataType dataType;       // 同步数据的类型
    ENSyncOperateType operateType; // 操作类型
    string domainId;               // 同步数据所属域编号
    string dataId;                 // 同步数据编号字符串
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

    string dstDomainId;    // 目的域编号
    ENSyncType enSyncType; // 同步类型
    TSyncDataRecordInfo info;
};

struct TSyncDataRecordStoreRsp: public CInnerRspBase
{
};

struct TSyncDataRecordDelReq
{
    string dstDomainId;    // 目标域编号
    ENSyncType enSyncType; // 同步类型
    vector<u32> sns;       // sn数组
};

struct TSyncDataRecordDelRsp: public CInnerRspBase
{
    // 失败时只返回一个总体结果，不具体细化到哪些失败了; 后续整体进行重做
};

struct TSyncDataConstructReq
{
    ENSyncType enSyncType;
    string dstDomainId;     // 目标域编号
    u32 dwExpectedDataNum;  // 期望构造的数据条数
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
    vector<TDeviceCapIndexs> devCapIndexs;           // 设备能力索引名称
    vector<CDeviceGroupDeviceInfoKey> devGrpDevKeys; // 设备分组设备划归
};

struct TParentDomainSyncDataConfigAddReq
{
    string parentDomainId;
    string parentDomainName;
};


struct TDevGbId
{
	UUIDString devId;
	int vidSrcIdx;    //设备ID
	int operType;     //修改类型：0-仅修改主流gbid，1-仅修改辅流gbid，2-主辅流gbid都修改
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
	//暂无需要字段
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
	UUIDString devTreeId;      //用于指定需更新的数据表
	UUIDString devGrpId;       //用于指定需更新的分组(为空表示根分组)
	double     devGrpSeq;      //用于指定需更新的分组grpseq字段（无法修改）
	string     devGrpGbid;     //原始分组对应的gbid
};

struct TSetDevGrpGbidRsp:public CInnerRspBase
{
	//暂无需要字段
};

struct TMatchChildrenGrpGbidReq
{
	UUIDString devTreeId;
	UUIDString devParentGrpId;
	string     devParentGrpGbid;
};

struct TMatchChildrenGrpGbidRsp:public CInnerRspBase
{
	//暂无需要字段
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
