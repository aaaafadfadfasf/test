/*****************************************************************************
   文件实现功能: 实现license的管理
   作者        : huzhiyun
   版本        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人      修改内容
   2011/11/11   1.0        huzhiyun        创建
******************************************************************************/
#ifndef _LICENSE_MNG_H_
#define _LICENSE_MNG_H_
#include "inner_common.h"

#define APP_GET_LICENSE                  "./getlicenseinfo"
#define DEF_LICENSE_PATH                 "./kedalicense.key"
//之前文件中的宏定义
#define KEY_VALUE_SEPARETE					(LPCSTR)"&"							// Key-Value对分隔符
#define KEY_VALUE_CONNECTOR					(LPCSTR)"="							// Key-Value连接符

struct TLicenceInfo
{
    TLicenceInfo()
    {
        m_dwTotalPUMaxNum = 0;
        m_dwTotalNRUMaxNum = 0;
        m_dwTotalVideoMaxNum = 0;
        m_dwTotalRecMaxNum = 0;
        m_dwUserID = 0;
        m_tExpireDate = 0;
        m_dwIntelligenceNum = 0;
        m_tCreateDate = 0;
        m_bEnableOtherManufactureDirectAccess = false;
        m_dwOtherPUNum = 0;
        m_dwManagePUMaxNum = 0;
        m_dwOtherStorageMaxSize = 0;
    }

    u32 GetSomeOtherPuLimit(const string &strManufacture);

    u32  m_dwTotalPUMaxNum;    //本地3AS授权的PU的最大允许个数
    u32  m_dwTotalNRUMaxNum;   //本地3AS授权的NRU的最大允许个数
    u32  m_dwTotalVideoMaxNum; //本地3AS授权的视频浏览并发路数的最大允许个数
    u32  m_dwTotalRecMaxNum;   //本地3AS授权的录像并发路数的最大允许个数
    u32  m_dwUserID;           //使用Key的用户ID
    time_t  m_tExpireDate;     //使用Key的截止日期
    string   m_strProductId;   //USKEY的产品编号
    string   m_strLicenseId;   //USKEY的许可证编码字串
    string   m_strHostId;      //主机序列号
    string   m_strVersion;     //版本号
    u32  m_dwIntelligenceNum;  //智能功能的License路数
    time_t  m_tCreateDate;     //使用Key的创建日期

    bool m_bEnableOtherManufactureDirectAccess; //是否允许外厂商直接接入
    u32  m_dwOtherPUNum;       //本地3AS授权的其它厂商PU的最大允许个数
    vector<TOtherPULicenceInfo>   m_atOtherPULicenceInfo; //外厂商接入PU允许数目限制规则
    u32 m_dwManagePUMaxNum;      // 最大可管理监控点数                
    u32 m_dwOtherStorageMaxSize; // 最大可管理外厂商存储空间, 单位:T
};

class CLicenseAppData
{
public:
    enum TKeyType
    {
        enInvalidKey,
        enUsbKey,
        enFileKey,
        enNetKey
    };
    enum TLicenseServerStatus
    {
        enLicenseServerUnkownStatus = 0,
        enLicenseServerUnConnected,
        enLicenseServerConnectedUnAuth,
        enLicenseServerConnectedAuth
    };
    void Init();
    void Dump(u32 dwCode);
    void UpdateLicenseKey(bool bOK, TKeyType tKeyType, const TLicenceInfo &tInfo);
    bool IsLicenseKeyOk() const
    {
        return m_bLicenseKeyOK;
    }
    TLicenceInfo GetLicenseKey() const;
    TKeyType GetLicenseKeyType() const
    {
        return m_tLicenseKeyType;
    }
    u32 GetLicenseServerStatus() const ;
    void UpdateLicenseServerStatus(TLicenseServerStatus tLicenseServerStatus);
    void UpdateLicenseUsage(const TLicenseUsageInfo &tInfo);
    TLicenseUsageInfo GetLicenseUsage() const;

	ENTasErrno AllocPULicense(const string &strManufacture, u32 dwLicenseNum,const TCreater &tCreater);
    void DeallocPULicense(const string &strManufacture, u32 dwLicenseNum);
    ENTasErrno AllocManagePULicense(u32 dwVideoNum);
    void DeallocManagePULicense(u32 dwVideoNum);
    
    ENTasErrno ValidateLicense();
private:
    ENTasErrno ValidateOtherPULicense(TLicenceInfo &tLicense, const TLicenseUsageInfo &tUsage);
    void Adapt2901OspParameter(const string& str2901DevType);

private:
    bool m_bLicenseKeyOK;        // key是否连接正常
    u32 m_dwLicenseKeyErrTimes;  // key检测出错次数
    TKeyType m_tLicenseKeyType;  // key类型
    TLicenceInfo m_tLicenseInfo; // license信息

    bool m_bCurrentUsageInit;           // license当前使用情况已经初始化
    TLicenseUsageInfo m_tCurrentUsage;  // 已经接入设备的license信息

    SEMHANDLE m_hLicenseInfoLock;       // license信息保护锁
    TLicenseServerStatus m_tLicenseServerStatus; //连接license服务器的状态
};

class CLicenseInst: public CInstExt
{
public:
    enum
    {
        LICENSE_KEY_CHECH_INTERVAL = 1000*5
    };
    enum ENState
    {
        enStateIdle = 1,
    };
public:
    CLicenseInst(){};
    virtual ~CLicenseInst();
    void DaemonEntry(CMessage * const pcMsg);
    void NormalEntry(CMessage * const pcMsg){}

    //获取data
    CLicenseAppData& GetAppData();

    // 定时检测license使用情况
    void CheckLicenseUsage();

    //增加参数，便于主动写入错误码
    void CheckLicenseKey(CLicenseAppData *pcData);
    bool ReadLicenseContent(TLicenceInfo &tLicense, CLicenseAppData::TKeyType &tKeyType, const string &strFileKey = string(),bool bVerifyMacNo=true);

	//从指定的Buf中获取给定的Key对应的Value
	u16 GetLicenseValueFromBuf(const string &strBuf, const s8 *const pchKey, s8 *pchValue, s32 &nValueLen);
    //将key信息写入USB Key
    s32 WriteUsbKey(const string &strKeyPath);

public:
    virtual LPCSTR GetStrState() const;

private:  
    void SetCheckLicenseUsageTimer(u32 dwTimerInterval = 0);
    void UpdateLicenseFile(COspSipMsg* const pcSipMsg, CLicenseAppData *pcData);
    void UpdateLicenseConfig(COspSipMsg* const pcSipMsg, CLicenseAppData *pcData);
    void PostMsg2CCtrl(u32 dwEventId, void* pContent = NULL, u32 dwLen = 0);
    void OnProcSDKMsg(CMessage * const pcMsg);
};

typedef COspApp<CLicenseInst, 1, CLicenseAppData> CLicenseApp;

extern CLicenseApp g_cLicenseApp;
extern ENTasErrno g_enLicenseValidateResult;

ENTasErrno AllocLicense(const CDeviceModelInfo &cModel,const TCreater &tCreater);
void DeallocLicense(const CDeviceModelInfo &cModel);
ENTasErrno AllocPULicense( const string &strManufacture, u32 dwLicenseNum,const TCreater &tCreater );
void DeallocPULicense( const string &strManufacture, u32 dwLicenseNum );
ENTasErrno AllocManagePULicense(const CDeviceModelInfo &cModel);
void DeallocManagePULicense(const CDeviceModelInfo &cModel);
ENTasErrno AllocManagePULicense(u32 dwVideoNum);
void DeallocManagePULicense(u32 dwVideoNum);

// 创建host文件
bool CreatHostFile();

bool GetMachineNO(string &strVal);

#endif // _LICENSE_MNG_H_
