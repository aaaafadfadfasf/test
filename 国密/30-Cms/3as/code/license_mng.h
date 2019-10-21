/*****************************************************************************
   �ļ�ʵ�ֹ���: ʵ��license�Ĺ���
   ����        : huzhiyun
   �汾        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
   2011/11/11   1.0        huzhiyun        ����
******************************************************************************/
#ifndef _LICENSE_MNG_H_
#define _LICENSE_MNG_H_
#include "inner_common.h"

#define APP_GET_LICENSE                  "./getlicenseinfo"
#define DEF_LICENSE_PATH                 "./kedalicense.key"
//֮ǰ�ļ��еĺ궨��
#define KEY_VALUE_SEPARETE					(LPCSTR)"&"							// Key-Value�Էָ���
#define KEY_VALUE_CONNECTOR					(LPCSTR)"="							// Key-Value���ӷ�

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

    u32  m_dwTotalPUMaxNum;    //����3AS��Ȩ��PU������������
    u32  m_dwTotalNRUMaxNum;   //����3AS��Ȩ��NRU������������
    u32  m_dwTotalVideoMaxNum; //����3AS��Ȩ����Ƶ�������·��������������
    u32  m_dwTotalRecMaxNum;   //����3AS��Ȩ��¼�񲢷�·��������������
    u32  m_dwUserID;           //ʹ��Key���û�ID
    time_t  m_tExpireDate;     //ʹ��Key�Ľ�ֹ����
    string   m_strProductId;   //USKEY�Ĳ�Ʒ���
    string   m_strLicenseId;   //USKEY�����֤�����ִ�
    string   m_strHostId;      //�������к�
    string   m_strVersion;     //�汾��
    u32  m_dwIntelligenceNum;  //���ܹ��ܵ�License·��
    time_t  m_tCreateDate;     //ʹ��Key�Ĵ�������

    bool m_bEnableOtherManufactureDirectAccess; //�Ƿ������⳧��ֱ�ӽ���
    u32  m_dwOtherPUNum;       //����3AS��Ȩ����������PU������������
    vector<TOtherPULicenceInfo>   m_atOtherPULicenceInfo; //�⳧�̽���PU������Ŀ���ƹ���
    u32 m_dwManagePUMaxNum;      // ���ɹ����ص���                
    u32 m_dwOtherStorageMaxSize; // ���ɹ����⳧�̴洢�ռ�, ��λ:T
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
    bool m_bLicenseKeyOK;        // key�Ƿ���������
    u32 m_dwLicenseKeyErrTimes;  // key���������
    TKeyType m_tLicenseKeyType;  // key����
    TLicenceInfo m_tLicenseInfo; // license��Ϣ

    bool m_bCurrentUsageInit;           // license��ǰʹ������Ѿ���ʼ��
    TLicenseUsageInfo m_tCurrentUsage;  // �Ѿ������豸��license��Ϣ

    SEMHANDLE m_hLicenseInfoLock;       // license��Ϣ������
    TLicenseServerStatus m_tLicenseServerStatus; //����license��������״̬
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

    //��ȡdata
    CLicenseAppData& GetAppData();

    // ��ʱ���licenseʹ�����
    void CheckLicenseUsage();

    //���Ӳ�������������д�������
    void CheckLicenseKey(CLicenseAppData *pcData);
    bool ReadLicenseContent(TLicenceInfo &tLicense, CLicenseAppData::TKeyType &tKeyType, const string &strFileKey = string(),bool bVerifyMacNo=true);

	//��ָ����Buf�л�ȡ������Key��Ӧ��Value
	u16 GetLicenseValueFromBuf(const string &strBuf, const s8 *const pchKey, s8 *pchValue, s32 &nValueLen);
    //��key��Ϣд��USB Key
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

// ����host�ļ�
bool CreatHostFile();

bool GetMachineNO(string &strVal);

#endif // _LICENSE_MNG_H_
