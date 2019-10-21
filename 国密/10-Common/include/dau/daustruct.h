#ifndef _DAU_STRUCT_H_
#define _DAU_STRUCT_H_

#include "kdvtype.h"
#include "dauconst.h"

#include <vector>
#include <string>

class TiXmlDocument;
class TiXmlNode;


// ����ͨ�ţ�ÿ����Ԫ������
struct TNEResult
{
private:
	std::string m_strNEID;	    // ��ԪID
	std::string m_strAgentID;	// ��Ԫ����ID����������AgentIDΪ����ID��
	u32 m_dwErrorCode;	        // �����룬 0--�ɹ�������--����
	std::string m_strErrorDesc;	// ��������	

public:
    TNEResult()
    {
        m_strNEID.clear();
        m_strAgentID.clear();
        m_dwErrorCode = 0;
        m_strErrorDesc.clear();
    }

	void SetNEID(std::string strNEID){ m_strNEID = strNEID; };
	std::string GetNEID(){ return m_strNEID; };
	void SetAgentID(std::string strAgentID){ m_strAgentID = strAgentID; };
	std::string GetAgentID(){ return m_strAgentID; };
	void SetErrorCode(u32 dwErrorCode){ m_dwErrorCode = dwErrorCode; };
	u32 GetErrorCode(){ return m_dwErrorCode; };
	void SetErrorDesc(std::string strDesc){ m_strErrorDesc = strDesc; };
	std::string GetErrorDesc(){ return m_strErrorDesc; };

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

// ��Ԫע��
struct TNERegisterInfo
{
private:
	std::string m_strNEID;	// �豸ID
	std::string m_strAgentID;	// �豸����AgentID
	std::string m_strNEName;	// �豸����
	std::string m_strProtocol;	// Э�����ƣ�ע��ʱЭ����ʵЭ�����ƣ��ɲ������DAU����
    std::string m_strSoftwareVer;   //����汾
    std::string m_strHardwareVer;   //Ӳ���汾
	u32 m_dwIP;	// �豸IP��������
	std::vector<std::string> m_vctCapability;	// ���������ַ�����ʾ���оͼ�������
	// AgentCap		������������ʶ����Ԫ�Ƿ�Ϊ��������Ԫ����(�����ڲ�ʹ��)
	// UpgradeCap	������������Ԫ�Ƿ��ܹ�ͨ�����ܽ�������
	// TopoCap		�ϱ�topo������3AS��Ҫ�ϱ�
	// IscsiCap		�Ƿ�Я��ISCSI������NRU�����ϱ�

public:
    TNERegisterInfo()
    {
        m_strNEID.clear();
        m_strAgentID.clear();
		m_strProtocol.clear();
        m_strNEName.clear();
        m_dwIP = 0;
        m_vctCapability.clear();
    }
	void SetNEID(std::string strNEID){ m_strNEID = strNEID; };
	std::string GetNEID(){ return m_strNEID; };
	void SetAgentID(std::string strAgentID){ m_strAgentID = strAgentID; };
	std::string GetAgentID(){ return m_strAgentID; };
	void SetProtocol(std::string strProtocol) { m_strProtocol = strProtocol; };
	std::string GetProtocol() { return m_strProtocol; };
	void SetNEName(std::string strName){ m_strNEName = strName; };
	std::string GetNEName(){ return m_strNEName; };
    void SetSoftwareVer(std::string strValue){ m_strSoftwareVer = strValue; };
    std::string GetSoftwareVer(){ return m_strSoftwareVer; };
    void SetHardwareVer(std::string strValue){ m_strHardwareVer = strValue; };
    std::string GetHardwareVer(){ return m_strHardwareVer; };
	void SetIP( u32 dwIP ){ m_dwIP = dwIP; };
	u32 GetIP(){ return m_dwIP; };
	void SetCapabilitySet( std::vector<std::string>& vctCS ){ m_vctCapability = vctCS; };
	std::vector<std::string>& GetCapabilitySet(){return m_vctCapability;}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

// ��Ԫע�����󣬿ɶ����Ԫһ��ע��
struct TNEReportRegisterReq
{
private:
	std::vector<TNERegisterInfo> m_vctRegisterInfo;

public:
    TNEReportRegisterReq()
    {
        m_vctRegisterInfo.clear();
    }
	void SetRegisterInfo( std::vector<TNERegisterInfo>& vctRegisterInfo ) { m_vctRegisterInfo = vctRegisterInfo; };
	std::vector<TNERegisterInfo>& GetRegisterInfo(){ return m_vctRegisterInfo; };
	void Clear() { m_vctRegisterInfo.clear(); };
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};
// ��Ԫע������ظ�
struct TNEReportRegisterRsp
{
private:
	std::vector<TNEResult> m_vctRegisterResult;

public:
    TNEReportRegisterRsp()
    {
        m_vctRegisterResult.clear();
    }
	void SetRegisterResult(std::vector<TNEResult>& vctRet){ m_vctRegisterResult = vctRet; };
	std::vector<TNEResult>& GetRegisterResult(){ return m_vctRegisterResult; };
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

// ����TOPO��Ϣ
struct TNETopoInfo
{	
private:
	std::string m_strNEID;		// �豸ID
	std::string m_strAgentID;	// ��Ԫ����ID����������AgentIDΪ����ID��
	std::string m_strDomainID;	// �豸������ID��CMUID��

	std::string m_strProtocol;	// Э�����ͣ�˽��Э��Ϊ��kedasnmp��

	std::string m_strNEName;	// �豸����
	std::string m_strNEModeInfo;	// �豸�ͺ�
	std::string m_strMatainState;	// �豸ά��״̬��(����/����/�����Զ���. . )
	std::string m_strNetAddress;		// �豸�����ַ��IP������(www.**)
	std::string m_strFixAddress;		// �豸��װλ��
	std::string m_strManufacturer;	// �豸������
	std::string m_strDescription;	// �豸����
	std::string m_strDomainFullName;	// ���ȫ�� ƽ̨�豸����ʾ����
	std::string m_strDomainShortName;// ��ļ��
	std::string m_strGBID;	// ����ID

	std::string m_strMainType;	// �豸������
	std::string m_strSubType;	// �豸������

	u32 m_dwExpireDate;                           // ʹ�ý�ֹ����
	u32 m_dwCreateDate;                           // ��������	
public:
    TNETopoInfo()
    {
        m_strNEID.clear();
        m_strAgentID.clear();
        m_strDomainID.clear();
        m_strProtocol.clear();
        m_strNEName.clear();
        m_strNEModeInfo.clear();
        m_strMatainState.clear();
        m_strNetAddress.clear();
        m_strFixAddress.clear();
        m_strManufacturer.clear();
        m_strDescription.clear();
        m_strDomainFullName.clear();
        m_strDomainShortName.clear();
        m_strGBID.clear();
        m_strMainType.clear();
        m_strSubType.clear();
        m_dwExpireDate = 0;
        m_dwCreateDate = 0;
    }
	void SetNEID(std::string strNEID){ m_strNEID = strNEID; };
	std::string GetNEID(){ return m_strNEID; };
	void SetAgentID(std::string strAgentID){ m_strAgentID = strAgentID; };
	std::string GetAgentID(){ return m_strAgentID; };
	void SetDomainID(std::string strDomainID){ m_strDomainID = strDomainID; };
	std::string GetDomainID(){ return m_strDomainID; };
	void SetProtocol(std::string strProtocal){ m_strProtocol = strProtocal; };
	std::string GetProtocol(){ return m_strProtocol; };
	void SetNEName(std::string strName){ m_strNEName = strName; };
	std::string GetNEName(){ return m_strNEName; };
	void SetModeInfo(std::string strModeInfo){ m_strNEModeInfo = strModeInfo; };
	std::string GetModeInfo(){return m_strNEModeInfo;};
	void SetMatainState(std::string strState){m_strMatainState = strState;}
	std::string GetMatainState(){return m_strMatainState;};
	void SetNetAddress(std::string strAddress){m_strNetAddress = strAddress;};
	std::string GetNetAddress(){return m_strNetAddress;};
	void SetFixAddress(std::string strAddress){m_strFixAddress = strAddress;}
	std::string GetFixAddress(){return m_strFixAddress;};
	void SetManufacturer(std::string strManu){m_strManufacturer = strManu;};
	std::string GetManufacutrer(){return m_strManufacturer;};
	void SetDescription(std::string strDesc){m_strDescription = strDesc;};
	std::string GetDescription(){return m_strDescription;};
	void SetDomainFullName(std::string strFullName){m_strDomainFullName = strFullName;};
	std::string GetDomainFullName(){return m_strDomainFullName;};
	void SetDomainShortName(std::string strName){m_strDomainShortName = strName;};
	std::string GetDomainShowtName(){return m_strDomainShortName;};
	void SetGBID(std::string strGbID){m_strGBID = strGbID;};
	std::string GetGBID(){return m_strGBID;};
	void SetMainType(std::string strType){m_strMainType = strType;};
	std::string GetMaintType(){return m_strMainType;};
	void SetSubType(std::string strSubType){m_strSubType = strSubType;};
	std::string GetSubType(){return m_strSubType;};
	void SetExpireDate(u32 dwDate){m_dwExpireDate = dwDate;};
	u32 GetExpireDate(){return m_dwExpireDate;}
	void SetCreateDate(u32 dwDate){m_dwCreateDate = dwDate;};
	u32 GetCreateDate(){return m_dwCreateDate;};
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

// topo��Ϣ�ϱ���ͬ��/��/ɾ/��
struct TNEReportTopoInfoNty
{
private:
	u32 m_dwTotaoNum;
	u32 m_bIsFinished;	// �Ƿ����
	std::vector<TNETopoInfo> m_vctTopoInfoList;	// �ϱ�topo�б�
	u32 m_dwFilterNum;	//������Ԫ����

public:
    TNEReportTopoInfoNty()
    {
        m_dwTotaoNum = 0;
        m_bIsFinished = 0;
        m_dwFilterNum = 0;
        m_vctTopoInfoList.clear();
    }
	void SetTotalNum(u32 dwNum){m_dwTotaoNum = dwNum;};
	u32 GetTotalNum(){return m_dwTotaoNum;};
	void SetFinished(u32 bFinished){m_bIsFinished = bFinished;};
	u32 GetFinished(){return m_bIsFinished;};
	void SetTopoList(std::vector<TNETopoInfo>& tInfo){m_vctTopoInfoList = tInfo;};
	std::vector<TNETopoInfo>& GetTopoList(){return m_vctTopoInfoList;};
	void SetFilterNum(u32 dwNum){m_dwFilterNum = dwNum;};
	u32 GetFilterNum(){return m_dwFilterNum;};
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

// topo��Ϣ�ϱ����---------del����ʹ��
struct TNEReportTopoInfoRsp
{
private:

	std::vector<TNEResult> m_vctReportResult;	// �ϱ�topo�����ÿһ����¼��Ӧһ�����

public:
    TNEReportTopoInfoRsp()
    {
        m_vctReportResult.clear();
    }
	void SetReportTopoResult(std::vector<TNEResult>& vctRet){m_vctReportResult = vctRet;};
	std::vector<TNEResult>& GetReportTopoResult(){return m_vctReportResult;};
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

struct TNEAlarm
{
private:
	std::string m_strNEID;
	std::string m_strAgentID;
	u32 m_dwAlarmCode;
	u32 m_dwChannel;		// �澯ͨ�����Ǳ����ֶΣ���IPC���ڸ澯����Ҫ����ֶΣ�
    u32 m_dwSubChannel;     // ��ͨ����svr��ͨ�����滹������ͨ��
	u32 m_dwOccurTime;		// �澯����ʱ��
	u32 m_dwAlarmStatus;	//  �澯״̬��0-�ָ�  1-����
	std::string m_strAlarmDesc;

    //�����ֶ���dau��д �����ط�����Ҫ��
    std::string m_strID;        //���������ɵĶ�Ӧ��ID
    std::string m_strDevName;   //�豸�� 
    std::string m_strActualIP;  //�豸��ʵIP
    std::string m_strChildType;//������
public:
    TNEAlarm()
    {
        m_strNEID.clear();
        m_strAgentID.clear();
        m_dwAlarmCode = 0;
        m_dwAlarmStatus = 0;
		m_dwChannel = 0;
        m_dwSubChannel = 0;
		m_dwOccurTime = 0;
        m_strAlarmDesc.clear();
        m_strID.clear();
        m_strDevName.clear();
        m_strActualIP.clear();
        m_strChildType.clear();
    }
	void SetNEID(std::string strNEID){ m_strNEID = strNEID; };
	std::string GetNEID(){ return m_strNEID; };
 	void SetAgentID(std::string strAgentID) { m_strAgentID = strAgentID; };
 	std::string GetAgentID(){ return m_strAgentID; };
	void SetAlarmCode(u32 dwAlarmCode){m_dwAlarmCode = dwAlarmCode;};
	u32 GetAlarmCode(){return m_dwAlarmCode;};
	void SetAlarmStatus(u32 dwStatus){ m_dwAlarmStatus = dwStatus; };
	u32 GetAlarmStaus(){ return m_dwAlarmStatus; };
	void SetAlarmDesc(std::string strDesc){m_strAlarmDesc = strDesc;};
	std::string GetAlarmDesc(){return m_strAlarmDesc;}
	void SetChannel( u32 dwChannel ){ m_dwChannel = dwChannel; };
	u32 GetChannel() { return m_dwChannel; };

    void SetSubChannel( u32 dwChannel ){ m_dwSubChannel = dwChannel; };
    u32 GetSubChannel() { return m_dwSubChannel; };

	void SetOccurTime( u32 dwTime ){ m_dwOccurTime = dwTime; };
	u32 GetOccurTime() { return m_dwOccurTime; };
    void SetID(std::string strValue) { m_strID = strValue; }
    std::string GetID() { return m_strID; }
    void SetDevName(std::string strValue) { m_strDevName = strValue; }
    std::string GetDevName() { return m_strDevName; }
    void SeActualIP(std::string strValue) { m_strActualIP = strValue; }
    std::string GeActualIP() { return m_strActualIP; }
    void SetChildType(std::string strValue) { m_strChildType = strValue; }
    std::string GetChildType() { return m_strChildType; }

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};
// �澯֪ͨ���ɶ���һ���ϱ�
struct TNEAlarmReport
{
private:
	std::vector<TNEAlarm> m_vctAlarmList;

public:
    TNEAlarmReport()
    {
        m_vctAlarmList.clear();
    }
	void SetAlarmInfo(std::vector<TNEAlarm>& vctAlarm){m_vctAlarmList = vctAlarm;};
	std::vector<TNEAlarm>& GetAlarmInfo(){return m_vctAlarmList;};

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

struct TNEAgentInfo		// cmdtype�� AddAgent
{
private:
	std::string m_strNEID;
	std::string m_strNEName;	// agent����
	std::string m_strProtocol;	// ����Э������
	std::string m_strDauIP;		// ע��DAU  IP��ַ
	u16 m_wDauPort;	// ע��DAU  Port
	std::string m_strNEAgentIP;	// Agent IP
	u16 m_wNEAgentPort;	// Agent Port

public:
    TNEAgentInfo()
    {
        m_strNEID.clear();
        m_strNEName.clear();
        m_strDauIP.clear();
        m_wDauPort = 0;
        m_strNEAgentIP.clear();
        m_wNEAgentPort = 0;
		m_strProtocol.clear();
    }
	void SetNEID( std::string strNEID ) { m_strNEID = strNEID; };
	std::string GetNEID() { return m_strNEID; };
	void SetNEName( std::string strNEName ) { m_strNEName = strNEName; };
	std::string GetNEName() { return m_strNEName; };
	void SetDauIP( std::string strIP ) {  m_strDauIP = strIP; };
	std::string GetDauIP() { return m_strDauIP; };
	void SetDAUPort( u16 wDauPort  ){ m_wDauPort = wDauPort; };
	u16 GetDauPort() { return m_wDauPort; };
	void SetAgentIP( std::string strIP ) { m_strNEAgentIP = strIP; };
	std::string GetAgentIP() { return m_strNEAgentIP; };
	void SetAgentPort( u16 wPort ) { m_wNEAgentPort = wPort; };
	u16 GetAgentPort() { return m_wNEAgentPort; };
	void SetProtocol( std::string strProtocol ) { m_strProtocol = strProtocol; };
	std::string GetProtocol() { return m_strProtocol; };
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

struct TNECpuInfo
{
private:
    std::string m_strVendor;     //����
    std::string m_strName;      //����
    u32         m_dwFreq;       //Ƶ��
    u32         m_dwCpuNum;     
    u32         m_dwCpuUsePercent;

public:
    TNECpuInfo()
    {
        m_strVendor.clear();
        m_strName.clear();
        m_dwFreq = 0;
        m_dwCpuNum = 0;
        m_dwCpuUsePercent = 0;
    }
    void SetVendor(std::string strValue) { m_strVendor = strValue; }
    std::string GetVendor() { return m_strVendor; }

    void SetName(std::string strValue) { m_strName = strValue; }
    std::string GetName() { return m_strName; }

    void SetFreq(u32 dwValue) { m_dwFreq = dwValue; }
    u32 GetFreq() { return m_dwFreq; }

    void SetCpuNum(u32 dwValue){ m_dwCpuNum = dwValue; };
    u32 GetCpuNum(){ return m_dwCpuNum; };

    void SetCpuUsePercent(u32 dwValue){ m_dwCpuUsePercent = dwValue; };
    u32 GetCpuUsePercent(){ return m_dwCpuUsePercent; };
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

struct TNEMemInfo
{
private:
    u32 m_dwPhysicsSize;    //�����ڴ��С(KB)
    u32 m_dwAllocSize;      //�����ڴ��С(KB)
    u32 m_dwFreeSize;       //�����ڴ��С(KB)
    u32 m_dwBuffers;        //������(KB)
    u32 m_dwCached;         //����(KB)
    u32 m_dwSwapCached;     //Swap����(KB)

public:
    TNEMemInfo()
    {
        m_dwPhysicsSize = 0;
        m_dwAllocSize = 0;
        m_dwFreeSize = 0;
        m_dwBuffers = 0;
        m_dwCached = 0;
        m_dwSwapCached = 0;
    }
    void SetPhysicsSize(u32 dwValue){ m_dwPhysicsSize = dwValue; };
    u32 GetPhysicsSize(){ return m_dwPhysicsSize; };

    void SetAllocSize(u32 dwValue){ m_dwAllocSize = dwValue; };
    u32 GetAllocSize(){ return m_dwAllocSize; };

    void SetFreeSize(u32 dwValue){ m_dwFreeSize = dwValue; };
    u32 GetFreeSize(){ return m_dwFreeSize; };

    void SetBuffer(u32 dwValue){ m_dwBuffers = dwValue; };
    u32 GetBuffer(){ return m_dwBuffers; };

    void SetCached(u32 dwValue){ m_dwCached = dwValue; };
    u32 GetCached(){ return m_dwCached; };

    void SetSwapCached(u32 dwValue){ m_dwSwapCached = dwValue; };
    u32 GetSwapCached(){ return m_dwSwapCached; };
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

struct TNEEthernetAdapterInfo
{
private:
    u32         m_dwId;             //������
    u32         m_dwState; 		    //����״̬
    std::string m_strName;          //��������
    std::string m_strDescription;   //��������
    std::string m_strMacAddr;       //���������ַ
    u32         m_dwDefaultGateWay; //Ĭ������

    u32         m_dwIpNum;          //IP��ַ����
    std::vector<u32> m_vecIpAddrList;   //ÿ�������ϵ�ip�б�
    std::vector<u32> m_vecSubNetMaskList;   //ÿ�������ϵ����������б�

    u32         m_dwInOctets;     //������
    u32         m_dwOutOctets;    //������
public:
    TNEEthernetAdapterInfo()
    {
        m_dwId = 0;
        m_dwState = 0;
        m_strName.clear();
        m_strDescription.clear();
        m_strMacAddr="00-00-00-00-00-00";
        m_dwDefaultGateWay = 0;
        m_dwIpNum = 0;
        m_vecIpAddrList.clear();
        m_vecSubNetMaskList.clear();
        m_dwInOctets = 0;
        m_dwOutOctets = 0;
    }
    void SetId(u32 dwValue){ m_dwId = dwValue; };
    u32 GetId(){ return m_dwId; };

    void SetState(u32 dwValue){ m_dwState = dwValue; };
    u32 GetState(){ return m_dwState; };

    void SetName(std::string strName){ m_strName = strName; }
    std::string GetName(){ return m_strName; };

    void SetDescription(std::string strDesc){ m_strDescription = strDesc; }
    std::string GetDescription(){ return m_strDescription; };

    void SetMacAddr(std::string strMacAddr){ m_strMacAddr = strMacAddr; }
    std::string GetMacAddr(){ return m_strMacAddr; };

    void SetDefaultGateWay(u32 dwValue){ m_dwDefaultGateWay = dwValue; };
    u32 GetDefaultGateWay(){ return m_dwDefaultGateWay; };

    void SetIpNum(u32 dwValue){ m_dwIpNum = dwValue; };
    u32 GetIpNum(){ return m_dwIpNum; };

    void SetIpAddrList(std::vector<u32>& vecIpList){ m_vecIpAddrList = vecIpList; };
    std::vector<u32>& GetIpAddrList(){ return m_vecIpAddrList; };

    void SetSubNetMaskList(std::vector<u32>& vecList){ m_vecSubNetMaskList = vecList; };
    std::vector<u32>& GetSubNetMaskList(){ return m_vecSubNetMaskList; };

    void SetInOctets(u32 dwValue){ m_dwInOctets = dwValue; };
    u32 GetInOctets(){ return m_dwInOctets; };

    void SetOutOctets(u32 dwValue){ m_dwOutOctets = dwValue; };
    u32 GetOutOctets(){ return m_dwOutOctets; };

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


//ϵͳ��Ϣ������cpu �ڴ� ����
struct TNESysInfo
{
private:
    TNECpuInfo  m_tCpuInfo;
    TNEMemInfo  m_tMenInfo;
    
    u32 m_dwEthernetAdapterNum;
    std::vector<TNEEthernetAdapterInfo> m_vecEthernetAdapterList;
public:
    TNESysInfo()
    {
        m_dwEthernetAdapterNum = 0;
        m_vecEthernetAdapterList.clear();
    }
    void SetCpuInfo(TNECpuInfo tCpuInfo) { m_tCpuInfo = tCpuInfo; }
    TNECpuInfo GetCpuInfo() { return m_tCpuInfo; }

    void SetMemInfo(TNEMemInfo tMemInfo) { m_tMenInfo = tMemInfo; }
    TNEMemInfo GetMemInfo() { return m_tMenInfo; }

    void SetEthernetAdapterNum(u32 dwValue){ m_dwEthernetAdapterNum = dwValue; };
    u32 GetEthernetAdapterNum(){ return m_dwEthernetAdapterNum; };

    void SetEthernetAdapterList(std::vector<TNEEthernetAdapterInfo>& vecEthernetAdapterList){ m_vecEthernetAdapterList = vecEthernetAdapterList; }
    std::vector<TNEEthernetAdapterInfo>& GetEthernetAdapterList(){ return m_vecEthernetAdapterList; }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

struct TNEDisconnectNtf
{
private:
    std::vector<std::string> m_vecNeList;
public:
    TNEDisconnectNtf()
    {
        m_vecNeList.clear();
    }

    void SetNeList(std::vector<std::string>& vecList){ m_vecNeList = vecList; }
    std::vector<std::string>& GetNeList(){ return m_vecNeList; }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

struct TNEFileBeatCfg
{
private:
    std::string m_strID;
    std::string m_strIpAddr;
    u32         m_dwIsUsed;
    std::string m_strServerIpAddr;
    u16         m_wPort;
public:
    TNEFileBeatCfg()
    {
        m_strID.clear();
        m_strIpAddr.clear();
        m_dwIsUsed = 0;
        m_strServerIpAddr.clear();
        m_wPort = 0;
    }

    std::string GetID() { return m_strID; }
    void SetID(std::string strValue) { m_strID = strValue; }

    std::string GetIpAddr() { return m_strIpAddr; }
    void SetIpAddr(std::string strValue) { m_strIpAddr = strValue; }

    u32 GetIsUsed() { return m_dwIsUsed; }
    void SetIsUsed(BOOL32 bUsed)
    {
        if (FALSE == bUsed)
        {
            m_dwIsUsed = 0;
        }
        else
        {
            m_dwIsUsed = 1;
        }
    }
    
    std::string GetServerIpAddr() { return m_strServerIpAddr; }
    void SetServerIpAddr(std::string strValue) { m_strServerIpAddr = strValue; }

    u16 GetPort() { return m_wPort; }
    void SetPort(u16 wPort) { m_wPort = wPort; }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

#endif
