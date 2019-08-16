#ifndef _DAU_STRUCT_H_
#define _DAU_STRUCT_H_

#include "kdvtype.h"
#include "dauconst.h"

#include <vector>
#include <string>


#define CMD_TYPE_GET_NE_COMMON_SYSINFO  "GetNeCommonSysInfo"    // TNECommonSysInfo


class TiXmlDocument;
class TiXmlNode;

struct TCommonMsg
{
private:
    std::string m_strCmdType;
    std::string m_strBody;

public:
    TCommonMsg()
    {
        m_strCmdType.clear();
        m_strBody.clear();
    }

    void SetCmdType(std::string strCmdType) { m_strCmdType = strCmdType; };
    std::string GetCmdType() { return m_strCmdType; };
    void SetBody( std::string strBody ) { m_strBody = strBody; };
    std::string GetBody() { return m_strBody; };

public:
    const std::string ToXml()const;
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const std::string& xml_string);
    void FromXml(const TiXmlNode& xmlNode);
};

struct TOpeNEAgentParam
{
    std::string CmdType;
    std::string MsgInfo;

public:
    TOpeNEAgentParam()
    {
        CmdType = "";
        MsgInfo = "";
    }

    void SetCmdType(std::string strValue) { CmdType = strValue; }
    std::string GetCmdType() { return CmdType; }

    void SetMsgInfo(std::string strValue) { MsgInfo = strValue;}
    std::string GetMsgInfo() { return MsgInfo; }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

// 批量通信，每个网元请求结果
struct TNEResult
{
private:
	std::string m_strNEID;	    // 网元ID
	std::string m_strAgentID;	// 网元代理ID（顶级代理，AgentID为自身ID）
	u32 m_dwErrorCode;	        // 错误码， 0--成功，其它--错误
	std::string m_strErrorDesc;	// 错误描述	

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

// 网元注册
struct TNERegisterInfo
{
private:
	std::string m_strNEID;	// 设备ID
	std::string m_strAgentID;	// 设备所属AgentID
	std::string m_strNEName;	// 设备名称
	std::string m_strProtocol;	// 协议名称，注册时协议真实协议名称，可不用填，由DAU填入
    std::string m_strSoftwareVer;   //软件版本
    std::string m_strHardwareVer;   //硬件版本
	u32 m_dwIP;	// 设备IP，网络序
	std::vector<std::string> m_vctCapability;	// 能力集，字符串表示，有就加入数组
	// AgentCap		代理能力，标识该网元是否为服务器网元代理(网管内部使用)
	// UpgradeCap	升级能力，网元是否能够通过网管进行升级
	// TopoCap		上报topo能力，3AS需要上报
	// IscsiCap		是否携带ISCSI能力，NRU按需上报

    std::string m_strMainType;
    std::string m_strSubType;
public:
    TNERegisterInfo()
    {
        m_strNEID.clear();
        m_strAgentID.clear();
		m_strProtocol.clear();
        m_strNEName.clear();
        m_dwIP = 0;
        m_vctCapability.clear();
        m_strMainType.clear();
        m_strSubType.clear();
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

    void SetMainType(std::string strValue){ m_strMainType = strValue; };
    std::string GetMainType(){ return m_strMainType; };

    void SetSubType(std::string strValue){ m_strSubType = strValue; };
    std::string GetSubType(){ return m_strSubType; };

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

// 网元注册请求，可多个网元一起注册
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
// 网元注册请求回复
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

// 入网TOPO信息
struct TNETopoInfo
{	
private:
	std::string m_strNEID;		// 设备ID
	std::string m_strAgentID;	// 网元代理ID（顶级代理，AgentID为自身ID）
	std::string m_strDomainID;	// 设备所属域ID（CMUID）

	std::string m_strProtocol;	// 协议类型，私有协议为（kedasnmp）

	std::string m_strNEName;	// 设备名称
	std::string m_strNEModeInfo;	// 设备型号
	std::string m_strMatainState;	// 设备维护状态，(正常/报修/其他自定义. . )
	std::string m_strNetAddress;		// 设备网络地址，IP或域名(www.**)
	std::string m_strFixAddress;		// 设备安装位置
	std::string m_strManufacturer;	// 设备制造商
	std::string m_strDescription;	// 设备描述
	std::string m_strDomainFullName;	// 域的全称 平台设备的显示域名
	std::string m_strDomainShortName;// 域的简称
	std::string m_strGBID;	// 国标ID

	std::string m_strMainType;	// 设备主类型
	std::string m_strSubType;	// 设备子类型

	u32 m_dwExpireDate;                           // 使用截止日期
	u32 m_dwCreateDate;                           // 入网日期	
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

// topo信息上报：同步/增/删/改
struct TNEReportTopoInfoNty
{
private:
	u32 m_dwTotaoNum;
	u32 m_bIsFinished;	// 是否结束
	std::vector<TNETopoInfo> m_vctTopoInfoList;	// 上报topo列表
	u32 m_dwFilterNum;	//过滤网元个数

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

// topo信息上报结果---------del，不使用
struct TNEReportTopoInfoRsp
{
private:

	std::vector<TNEResult> m_vctReportResult;	// 上报topo结果，每一条记录对应一个结果

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
	u32 m_dwChannel;		// 告警通道，非必须字段（如IPC并口告警，需要填该字段）
    u32 m_dwSubChannel;     // 子通道，svr的通道下面还存在子通道
	u32 m_dwOccurTime;		// 告警产生时间
	u32 m_dwAlarmStatus;	//  告警状态：0-恢复  1-产生
	std::string m_strAlarmDesc;

    //以下字段在dau填写 其他地方不需要填
    std::string m_strID;        //在网管生成的对应的ID
    std::string m_strDevName;   //设备名 
    std::string m_strActualIP;  //设备真实IP
    std::string m_strChildType; //子类型
    u32 m_dwVirtualAlarm;          //表示是否模拟告警，
                                //当neagent下线时，对应挂载的设备一起下线，那这个字段的值就是1，表示是模拟的告警
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
        m_dwVirtualAlarm = 0;
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

    void SetVirtualAlarm( u32 dwValue ){ m_dwVirtualAlarm = dwValue; };
    u32 GetVirtualAlarm() { return m_dwVirtualAlarm; };

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};
// 告警通知，可多条一起上报
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

struct TNEAgentInfo		// cmdtype： AddAgent
{
private:
	std::string m_strNEID;
	std::string m_strNEName;	// agent名称
	std::string m_strProtocol;	// 代理协议名称
	std::string m_strDauIP;		// 注册DAU  IP地址
	u16 m_wDauPort;	// 注册DAU  Port
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
    std::string m_strVendor;     //厂商
    std::string m_strName;      //名称
    u32         m_dwFreq;       //频率
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
    u32 m_dwPhysicsSize;    //物理内存大小(KB)
    u32 m_dwAllocSize;      //已用内存大小(KB)
    u32 m_dwFreeSize;       //可用内存大小(KB)
    u32 m_dwBuffers;        //缓冲区(KB)
    u32 m_dwCached;         //缓存(KB)
    u32 m_dwSwapCached;     //Swap缓存(KB)

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
    u32         m_dwId;             //网卡号
    u32         m_dwState; 		    //网卡状态
    std::string m_strName;          //网卡名称
    std::string m_strDescription;   //网卡描述
    std::string m_strMacAddr;       //网卡物理地址
    u32         m_dwDefaultGateWay; //默认网关

    u32         m_dwIpNum;          //IP地址个数
    std::vector<u32> m_vecIpAddrList;   //每个网卡上的ip列表
    std::vector<u32> m_vecSubNetMaskList;   //每个网卡上的子网掩码列表

    u32         m_dwInOctets;     //接收数
    u32         m_dwOutOctets;    //发送数

    u32         m_dwDropNum;      //丢包数
    u32         m_dwSpeed;        //网卡带宽
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

        m_dwDropNum = 0;
        m_dwSpeed = 0;
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

    void SetDropNum(u32 dwValue) { m_dwDropNum = dwValue; };
    u32 GetDropNum() { return m_dwDropNum; };

    void SetSpeed(u32 dwValue) { m_dwSpeed = dwValue; };
    u32 GetSpeed() { return m_dwSpeed; };

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


//系统信息，包含cpu 内存 网卡
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

struct TNESysInfoEx
{
private:
    TNECpuInfo  m_tCpuInfo;
    TNEMemInfo  m_tMenInfo;

public:
    TNESysInfoEx()
    {
        
    }
    void SetCpuInfo(TNECpuInfo tCpuInfo) { m_tCpuInfo = tCpuInfo; }
    TNECpuInfo GetCpuInfo() { return m_tCpuInfo; }

    void SetMemInfo(TNEMemInfo tMemInfo) { m_tMenInfo = tMemInfo; }
    TNEMemInfo GetMemInfo() { return m_tMenInfo; }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

struct TNEEthernet
{
private:

    std::vector<TNEEthernetAdapterInfo> m_vecEthernetAdapterList;
public:
    TNEEthernet()
    {
        m_vecEthernetAdapterList.clear();
    }

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

// 网元升级信息
struct TNEUpdateInfo
{
    enum 
    {
        FILE_TYPE_INVALID,
        FILE_TYPE_VERSION,  //版本
        FILE_TYPE_CONFIG,   //配置文件
        FILE_TYPE_IMAGE,    //镜像
    };
private:
    std::string m_strServerIP;	        // 版本服务器地址
    u16 m_wPort;	                    // 版本服务器端口
    std::string m_strServerUrl;	        // 升级地址
    std::string m_strServerUserName;	// 用户名
    std::string m_strServerPassword;	// 密码

    u32 m_dwAdminStatus;		        // 默认为1-upgradeFromMgt.		如果adminStatus被设置为upgradeFromMgt，则从imageUrl获取版本并升级
    u32 m_dwOperStatus;		            // 暂时不用
                                        // 只读，返回升级操作的状态。
                                        //	inProgress(1)：升级中；
                                        //	completed(2)：升级已完成；
                                        //	failed(3)：升级失败；
                                        //	other(4)：其它

    u32 m_dwUpdateTime;	                // 升级需要花费时间，单位：秒
    std::string m_strCheckString;	    // 升级校验字符串，为空，不支持校验
                                        // 文件导入导出时，填文件的MD5

    u32         m_dwFileType;           // 文件类型 
    u32         m_dwFileSize;           // 文件大小 单位byte
public:
    TNEUpdateInfo()
    {
        m_strServerIP.clear();
        m_wPort = 21;
        m_strServerUrl.clear();
        m_strServerUserName.clear();
        m_strServerPassword.clear();
        m_dwAdminStatus = 1;
        m_dwOperStatus = 0;
        m_dwUpdateTime = 300;
        m_strCheckString.clear();

        m_dwFileType = FILE_TYPE_INVALID;
        m_dwFileSize = 0;
    }
    void SetServerIP( std::string strIP ) { m_strServerIP = strIP; };
    std::string GetServerIP() { return m_strServerIP; };

    void SetPort( u16 wPort )  { m_wPort = wPort; };
    u16 GetPort() { return m_wPort; };

    void SetServerUrl( std::string strUrl ) { m_strServerUrl = strUrl; };
    std::string GetServerUrl() { return m_strServerUrl; };

    void SetUserName( std::string strUserName )  { m_strServerUserName = strUserName; };
    std::string GetUserName() { return m_strServerUserName; };

    void SetPassword( std::string strPwd ) { m_strServerPassword = strPwd; };
    std::string GetPassword() { return m_strServerPassword; };

    void SetAdminStatus( u32 dwAdminStatus ) { m_dwAdminStatus = dwAdminStatus; };
    u32 GetAdminStatus() { return m_dwAdminStatus; };

    void SetOperStatus( u32 dwStatus )  { m_dwOperStatus = dwStatus; };
    u32 GetOperStauts() { return m_dwOperStatus;};

    void SetUpdateTime( u32 dwTime ) { m_dwUpdateTime = dwTime; };
    u32 GetUpdateTime() { return m_dwUpdateTime; };

    void SetCheckString( std::string strCheckString ) { m_strCheckString = strCheckString; };
    std::string GetCheckString() { return m_strCheckString; };

    void SetFileType(u32 dwType) { m_dwFileType = dwType; }
    u32 GetFileType() { return m_dwFileType; }

    void SetFileSize(u32 dwValue) { m_dwFileSize = dwValue; }
    u32 GetFileSize() { return m_dwFileSize; }

public:
    const std::string ToXml()const;
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const std::string& xml_string);
    void FromXml(const TiXmlNode& xmlNode);
};

struct TServicesInfo
{
private:
private:
    std::string m_strID;		    // 服务ID
//     std::string m_strAgentID;	// 网元代理ID（顶级代理，AgentID为自身ID）
//     std::string m_strDomainID;	// 设备所属域ID（CMUID）
// 
//     std::string m_strProtocol;	// 协议类型，私有协议为（kedasnmp）

    std::string m_strName;	        // 服务名称
    //std::string m_strNEModeInfo;	// 设备型号
    std::string m_strState;	        // 服务状态
    std::string m_strNetAddress;	// 服务地址，IP或域名(www.**)
//     std::string m_strFixAddress;		// 设备安装位置
//     std::string m_strManufacturer;	// 设备制造商
    std::string m_strDescription;	// 设备描述
//     std::string m_strDomainFullName;	// 域的全称 平台设备的显示域名
//     std::string m_strDomainShortName;// 域的简称
//     std::string m_strGBID;	// 国标ID

    std::string m_strMainType;	// 设备主类型
    std::string m_strSubType;	// 设备子类型

//     u32 m_dwExpireDate;                           // 使用截止日期
//     u32 m_dwCreateDate;                           // 入网日期	

public:
    TServicesInfo()
    {
        Clear();
    }
    void Clear()
    {
        m_strID.clear();
        m_strName.clear();
        m_strState.clear();
        m_strNetAddress.clear();
        m_strDescription.clear();
        m_strMainType.clear();
        m_strSubType.clear();
    }

    void SetID( std::string strValue )  { m_strID = strValue; };
    std::string GetID() { return m_strID; };

    void SetName( std::string strValue )  { m_strName = strValue; };
    std::string GetName() { return m_strName; };

    void SetState( std::string strValue )  { m_strState = strValue; };
    std::string GetState() { return m_strState; };

    void SetNetAddress( std::string strValue )  { m_strNetAddress = strValue; };
    std::string GetNetAddress() { return m_strNetAddress; };

    void SetDescription( std::string strValue )  { m_strDescription = strValue; };
    std::string GetDescription() { return m_strDescription; };

    void SetMainType( std::string strValue )  { m_strMainType = strValue; };
    std::string GetMainType() { return m_strMainType; };

    void SetSubType( std::string strValue )  { m_strSubType = strValue; };
    std::string GetSubType() { return m_strSubType; };

public:
    const std::string ToJson() const;
    void FromJson(const std::string& json_string);
};

struct TServicesList
{
private:
    std::vector<TServicesInfo>  m_vecList;

public:
    TServicesList()
    {
        Clear();
    }
    void Clear()
    {
        m_vecList.clear();
    }

    void SetServicesList(std::vector<TServicesInfo> vecList) { m_vecList = vecList; }
    std::vector<TServicesInfo> GetServicesList() { return m_vecList; }

public:
    const std::string ToJson() const;
    void FromJson(const std::string& json_string);
};


struct TNECommonSysInfo
{
private:
    std::string m_strMainType;	// 设备主类型
    std::string m_strSubType;	// 设备子类型
    std::string m_strModel;      //型号
    std::string m_strSoftwareVer;   //软件版本
    std::string m_strHardwareVer;   //硬件版本
    std::string m_strFixAddress;		// 设备安装位置
public:

    TNECommonSysInfo()
    {
        Clear();
    }
    void Clear()
    {
        m_strMainType.clear();
        m_strSubType.clear();
        m_strModel.clear();
        m_strSoftwareVer.clear();
        m_strHardwareVer.clear();
        m_strFixAddress.clear();
    }

    void SetMainType(std::string strValue){m_strMainType = strValue;};
    std::string GetMaintType(){return m_strMainType;};

    void SetSubType(std::string strValue){m_strSubType = strValue;};
    std::string GetSubType(){return m_strSubType;};

    void SetModel(std::string strValue){m_strModel = strValue;};
    std::string GetModel(){return m_strModel;};

    void SetSoftwareVer(std::string strValue){ m_strSoftwareVer = strValue; };
    std::string GetSoftwareVer(){ return m_strSoftwareVer; };
    
    void SetHardwareVer(std::string strValue){ m_strHardwareVer = strValue; };
    std::string GetHardwareVer(){ return m_strHardwareVer; };

    void SetFixAddress(std::string strValue){m_strFixAddress = strValue;}
    std::string GetFixAddress(){return m_strFixAddress;};

public:
    const std::string ToJson() const;
    void FromJson(const std::string& json_string);
};

#endif
