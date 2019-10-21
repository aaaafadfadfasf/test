#ifndef _PMS_STRUCT_H_
#define _PMS_STRUCT_H_

#include "kdvtype.h"
#include "cms/cmsassert.h"
#include "cms/cms_proto.h"
#include "cms/utility/uuid.h"
#include "cms/cms_const.h"
#include "pms_specialstruct.h"
#include <string>
#include <vector>
#include <map>
#include <algorithm>
using std::string;
using std::vector;
using std::map;

class TiXmlDocument;
class TiXmlNode;

typedef string TAlarmEndMethod;
#define ALARM_END_METHOD_DEV_RESTORE    "DEV_RESTORE"
#define ALARM_END_METHOD_SYSTEM_CLEAR   "SYSTEM_CLEAR"
#define ALARM_END_METHOD_USER_CLEAR     "USER_CLEAR"

typedef string TDevCategory;
#define DEV_CAT_PLATFORM    "DEV_CAT_PLATFORM"
#define DEV_CAT_PU          "DEV_CAT_PU"

typedef string TAlarmState;
#define ALARM_STATE_OCCUR   "ALARM_OCCUR"
#define ALARM_STATE_END     "ALARM_END"

typedef string TAlarmOp;
#define ALARM_OP_CLEAR      "CLEAR"


#define PLATFORM_TYPE_MASTER "master"
#define PLATFORM_TYPE_SLAVE "slave"

class CProgressNtfBase
{
public:
	CProgressNtfBase()
	{
		progressPercent = 0;
	}

    const int GetProgressPercent() const
    {
        return progressPercent;
    }
    int GetProgressPercent()
    {
        return progressPercent;
    }
    void SetProgressPercent(const int dwProgressPercent)
    {
        progressPercent = dwProgressPercent;
    }
    const string& GetProgressTip() const
    {
        return progressTip;
    }
    string& GetProgressTip()
    {
        return progressTip;
    }
    void SetProgressTip(const string& strProgressTip)
    {
        progressTip = strProgressTip;
    }
private:
    int progressPercent;// -1��ʾʧ��,������Χ [0-100]
    string progressTip; // ������ʾ, ����ʱΪ��������
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

enum TPmsUserType
{
    enPmsUserType_Invalid = 0,
    enPmsUserType_SuperAdmin = 1,
    enPmsUserType_Admin = 2,
};
class CPmsUserInfo
{
public:
	CPmsUserInfo()
	{
		type = enPmsUserType_Invalid;
		ipListEnabledFlag = false;
		pwdExpireEnabledFlag = false;
		pwdExpireTime = 90;
	}

    const UUIDString& GetId() const
    {
        return id;
    }
    UUIDString& GetId()
    {
        return id;
    }
	void SetId(const UUIDString &tUUID)
	{
		id = tUUID;
	}
    const TPmsUserType GetType()const
    {
        return type;
    }
    TPmsUserType GetType()
    {
        return type;
    }
    void SetType(const TPmsUserType enType)
    {
        type = enType;
    }
    const string& GetName()const
    {
        return name;
    }
    string& GetName()
    {
        return name;
    }
    void SetName(const string& strName)
    {
        name = strName;
    }
    const string& GetPassword()const
    {
        return password;
    }
    string& GetPassword()
    {
        return password;
    }
    void SetPassword(const string& strPassword)
    {
        password = strPassword;
    }
    const string& GetDesc() const
    {
        return desc;
    }
    string& GetDesc()
    {
        return desc;
    }
    void SetDesc(const string& strDesc)
    {
        desc = strDesc;
    }

    const string& GetSecuremail() const
    {
        return securemail;
    }
    string&  GetSecuremail()
    {
        return securemail;
    }
    void SetSecuremail(const string& SetSecuremail)
    {
        securemail = SetSecuremail;
    }

	void SetPwdExpireDate(const string& strPwdExpireDate)
	{
		pwdExpireDate = strPwdExpireDate;
	}
	string& GetPwdExpireDate()
	{
		return pwdExpireDate;
	}
	const string& GetPwdExpireDate() const
	{
		return pwdExpireDate;
	}

	void SetPwdExpireTime(int nPwdExpireTime)
	{
		pwdExpireTime = nPwdExpireTime;
	}
	int GetPwdExpireTime() const
	{
		return pwdExpireTime;
	}

	void SetPwdExpireEnabledFlag(bool bPwdExpireEnabledFlag)
	{
		pwdExpireEnabledFlag = bPwdExpireEnabledFlag;
	}
	bool GetPwdExpireEnabledFlag() const
	{
		return pwdExpireEnabledFlag;
	}

	void SetLoginIpList(const string& strLoginIpList)
	{
		loginIpList = strLoginIpList;
	}
	string& GetLoginIpList()
	{
		return loginIpList;
	}
	const string& GetLoginIpList() const
	{
		return loginIpList;
	}

	void SetIpListEnabledFlag(bool bIpListEnabledFlag)
	{
		ipListEnabledFlag = bIpListEnabledFlag;
	}
	bool GetIpListEnabledFlag() const
	{
		return ipListEnabledFlag;
	}

private:
    UUIDString id;     // �û�Id
    TPmsUserType type; // �û�����
    string name;
    string password;             
    string desc;
	string pwdExpireDate;             //������Ч�ڵ���ʱ��               
	int pwdExpireTime;                //������Ч��ʱ��              
	bool pwdExpireEnabledFlag;        //������Ч���Ƿ�����
	string loginIpList;               //�󶨵�IP                
	bool ipListEnabledFlag;           //��IP�Ƿ����� 
    string securemail;
	bool   isDecrypt;               //�Ƿ���ܣ�ie8�����֮ǰ�Ĳ����ܣ�
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

public:
    void SetIsDecrypt(bool bIsDecrypt)
    {
        isDecrypt = bIsDecrypt;
    }
    bool GetIsDecrypt() const
    {
        return isDecrypt;
    }

};
class CPmsUserInfoKey
{
public:

    const UUIDString& GetId() const
    {
        return id;
    }
    UUIDString &GetId()
    {
        return id;
    }
    void SetId(const UUIDString& strID)
    {
        id = strID;
    }
private:
    UUIDString id;  // �û�ID
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
class CPmsUserInfoQueryCondition
{
public:
    CPmsUserInfoQueryCondition()
    {
        type = enPmsUserType_Invalid;
    }
    bool GetUserUUID(UUIDString & tUUID) const
    {
        if (!optionField.HasOption("id"))
            return false;
        tUUID = id;
        return true;
    }
    void SetUserUUID(const UUIDString &tUUID)
    {
        optionField.SetOption("id");
        id = tUUID;
    }
    bool GetUserType(TPmsUserType & val) const
    {
        if (!optionField.HasOption("type"))
            return false;
        val = type;
        return true;
    }
    void SetUserType(const TPmsUserType &val)
    {
        optionField.SetOption("type");
        type = val;
    }
    bool GetUserName(string &strUserName) const
    {      
        if (!optionField.HasOption("userName"))
            return false;
        strUserName = userName;
        return true;
    }
    void SetUserName(const string &strUserName)
    {
        optionField.SetOption("userName");
        userName = strUserName;
    }
private:
    CQryOptionField optionField;
    UUIDString id;     // �û�Id
    TPmsUserType type; // �û�����
    string userName;   // �û���, ��֧��ģ����ѯ
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPlatformModuleStatus
{
public:
	CPlatformModuleStatus()
	{
		status = enStatus_Conn;
	}

    enum TStatus
    {
        enStatus_Conn,
        enStatus_Discon
    };
    const string& GetName() const
    {
        return name;
    }
    string& GetName()
    {
        return name;
    }
    void SetName(const string& strName)
    {
        name = strName;
    }
    const string& GetAddress() const
    {
        return address;
    }
    string& GetAddress()
    {
        return address;
    }
    void SetAddress(const string& strAddress)
    {
        address = strAddress;
    }
	const string& GetUri()const
	{
		return uri;
	}
	string& GetUri()
	{
		return uri;
	}
	void SetUri(const string& strUri)
	{
		uri = strUri;
	}
    const TStatus GetStatus()const
    {
        return status;
    }
    TStatus GetStatus()
    {
        return status;
    }
    void SetStatus(const TStatus enStatus)
    {
        status = enStatus;
    }
private:
    string name;    // ģ������
    string address; // ģ���ַ
	string uri;
    TStatus status; // ģ��״̬
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

// enum TPlatformType
// {
//     enPlatformType_Master, // ��ƽ̨
//     enPlatformType_Slave   // ��ƽ̨
// };

class CPlatformSoftwareInfo
{
public:
	CPlatformSoftwareInfo()
	{
		platformType = PLATFORM_TYPE_MASTER;
		hbStatus     = enHBSTATUS_NOT_CONFIG;
        httpPort     = 80;
	}

    enum THeartbeatStatus
    {
        enHBSTATUS_NOT_CONFIG,           // δ����
        enHBSTATUS_HOST_DISCONNECT,      // ��������(δ�����ϱ���)
        enHBSTATUS_HOST_CONNECT,         // ��������(�������ϱ���)
        enHBSTATUS_BACKUP_NOT_REPLACING, // ��������(δ��������)
        enHBSTATUS_BACKUP_REPLACING,     // ��������(��������)
    };
    const string& GetVersion()const 
    {
        return version;
    }
    string &GetVersion()
    {
        return version;
    }
    void SetVersion(const string& strVersion)
    {
        version = strVersion;
    }
    const string& GetDomainName()const  
    {
        return domainName;
    }
    string& GetDomainName()
    {
        return domainName;
    }
    void SetDomainName(const string& strDomainName)
    {
        domainName = strDomainName;
    }
    const string& GetPlatformType()const  
    {
        return platformType;
    }
    string& GetPlatformType()
    {
        return platformType;
    }
    void SetPlatformType(const string& strPlatformType)
    {
        platformType = strPlatformType;
    }
    const map<string,string >& GetPlatformCapacitys()const
    {
        return platCapacitys;
    }
    map<string,string>& GetPlatformCapacitys()
    {
        return platCapacitys;
    }
    void SetPlatformCapacitys(const map<string,string >& tPlatformCapacitys)
    {
        platCapacitys = tPlatformCapacitys;
    }
    const THeartbeatStatus GetHbStatus()const
    {
        return hbStatus;
    }
    THeartbeatStatus GetHbStatus()
    {
        return hbStatus;
    }
    void SetHbStatus(const THeartbeatStatus enHbStatus)
    {
        hbStatus = enHbStatus;
    }
    const vector<CPlatformModuleStatus>& GetModuleStatus()const
    {
        return moduleStatus;
    }
    vector<CPlatformModuleStatus>& GetModuleStatus()
    {
        return moduleStatus;
    }
    void SetModuleStatus(const vector<CPlatformModuleStatus>& tModuleStatus)
    {
        moduleStatus = tModuleStatus;
    }
    void SetDomainId(const UUIDString& strDomainId)
    {
        domainId = strDomainId;
    }
    UUIDString& GetDomainId()
    {
        return domainId;
    }
    const UUIDString& GetDomainId() const
    {
        return domainId;
    }
    void SetPmsId(const UUIDString& strPmsId)
    {
        pmsId = strPmsId;
    }
    UUIDString& GetPmsId()
    {
        return pmsId;
    }
    const UUIDString& GetPmsId() const
    {
        return pmsId;
    }

private:
	string localNruId;       //����nruId              
    string version;          // ����汾
    string domainName;       // ƽ̨��������
	TNetAddr proxyAddr;             			             
	TNetAddr parentProxyAddr;             
    TNetAddr stunServerAddr;  // stun��������ַ
    int httpPort;             // http�����˿� 
	int httpsPort;            //https�����˿�
    UUIDString domainId;                 
    UUIDString pmsId;  
	bool isIscsiHide;        //iscsi�Ƿ�����               
    string platformType;                    //ֻ����ƽ̨����ƽ̨���֣���ƽ̨��master,��ƽ̨��slave
    map<string,string> platCapacitys;  	 	// ƽ̨��������
    										// platCapacitys["recTaskNum"]  ¼������·�� //rcs
				    						// platCapacitys["recBitRate]  ¼�������� //rcs
							    			// platCapacitys["recVodNum]  ¼��㲥·�� //rcs
							   				// platCapacitys["recVodTotalBitRate]  ¼��㲥������ //rcs
                                            // platCapacitys["switchNum]   ת������·�� //cmu
                                            // platCapacitys["switchTotalBitRate]  ת�������� //cmu
                                            // platCapacitys["onlineVideosourceNum]  ������ƵԴ���� //cmu
                                            // platCapacitys["onlineCuNum]  ����CU���� //cmu
											// platCapacitys["onlineMobileClientPercent]  �����ƶ��ͻ�����ռ���� //cmu
    THeartbeatStatus hbStatus;
    vector<CPlatformModuleStatus> moduleStatus;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

public:
    void SetProxyAddr(const TNetAddr& cProxyAddr)
    {
        proxyAddr = cProxyAddr;
    }
    TNetAddr& GetProxyAddr()
    {
        return proxyAddr;
    }
    const TNetAddr& GetProxyAddr() const
    {
        return proxyAddr;
    }

    void SetParentProxyAddr(const TNetAddr& cParentProxyAddr)
    {
        parentProxyAddr = cParentProxyAddr;
    }
    TNetAddr& GetParentProxyAddr()
    {
        return parentProxyAddr;
    }
    const TNetAddr& GetParentProxyAddr() const
    {
        return parentProxyAddr;
    }

    void SetStunServerAddr(const TNetAddr& cStunServerAddr)
    {
        stunServerAddr = cStunServerAddr;
    }
    TNetAddr& GetStunServerAddr()
    {
        return stunServerAddr;
    }
    const TNetAddr& GetStunServerAddr() const
    {
        return stunServerAddr;
    }

public:
    void SetHttpPort(int nHttpPort)
    {
        httpPort = nHttpPort;
    }
    int GetHttpPort() const
    {
        return httpPort;
    }

    void SetLocalNruId(const string& strLocalNruId)
    {
        localNruId = strLocalNruId;
    }
    string& GetLocalNruId()
    {
        return localNruId;
    }
    const string& GetLocalNruId() const
    {
        return localNruId;
    }

    void SetIsIscsiHide(bool bIsIscsiHide)
    {
        isIscsiHide = bIsIscsiHide;
    }
    bool GetIsIscsiHide() const
    {
        return isIscsiHide;
    }

    void SetHttpsPort(int nHttpsPort)
    {
        httpsPort = nHttpsPort;
    }
    int GetHttpsPort() const
    {
        return httpsPort;
    }

};

struct TSystemPartInfo
{
public:
    string		     		partUsage;             			//������;, sys|opt|db|log|usbbak
    int 		 			totalSize;             			//������С����λ��M��
    int	    	 			freeSize;             			//���ÿռ��С ����λ��M�� 
    // int						abnNum;             			//�쳣����

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPlatformTimeInfo
{
public:
	CPlatformTimeInfo()
	{
		type = enTimeSourceType_Local;
		syncInterval = 0;
	}

    enum TTimeSourceType
    {
        enTimeSourceType_Local, // ����ʱ��Դ�������������ͬ��
        enTimeSourceType_NTP    // NTP
    };
    const TTimeSourceType GetType()const   
    {
        return type;
    }
    TTimeSourceType GetType()
    {
        return type;
    }
    void SetType(const TTimeSourceType enType)
    {
        type = enType;
    }
    const string& GetTimeSource()const
    {
        return timeSource;
    }
    string& GetTimeSource()
    {
        return timeSource;
    }
    void SetTimeSource(const string& strTimeSource)
    {
        timeSource = strTimeSource;
    }
    const int GetSyncInterval()const
    {
        return syncInterval;
    }
    int GetSyncInterval()
    {
        return syncInterval;
    }
    void SetSyncInterval(const int dwSyncInterval)
    {
        syncInterval = dwSyncInterval;
    }
    const string& GetDateTime()const
    {
        return dateTime;
    }
    string& GetDateTime()
    {
        return dateTime;
    }
    void SetDateTime(const string& strNewDateTime)
    {
        dateTime = strNewDateTime;
    }

private:
    TTimeSourceType type;  // ʱ��Դ����
    // �����ֶ���NTPʱ��Դʱ��Ч
    string timeSource;     // NTP��������ַ
    int syncInterval;      // ͬ������(��λ:����)
    // �����ֶ��ڱ���ʱ��Դʱ��Ч
    string dateTime;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

struct TIPInfo
{
    string IP;
    string IPMask;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CNetcardInfo
{
public:
	CNetcardInfo()
	{
		nicIndex = 0;
		nicSpeed = 0; 
		nicMode = enNICMode_Autoneg;

	}
    enum ConStatus
    {
        e_CONNECTION ,
        e_DISCONNECTION
    };
    enum TNICMode
    {
        enNICMode_Unknow,
        enNICMode_Autoneg,
        enNICMode_10baseT_Half,
        enNICMode_10baseT_Full,
        enNICMode_100baseT_Half,
        enNICMode_100baseT_Full,
        enNICMode_1000baseT_Half,
        enNICMode_1000baseT_Full,
        enNICMode_10000baseT_Full
    };
    const int GetNicIndex()const
    {
        return nicIndex;
    }
    int GetNicIndex()
    {
        return nicIndex;
    }
    void SetNicIndex(const int dwNicIndex)
    {
        nicIndex = dwNicIndex;
    }
    const vector<TIPInfo>& GetIpInfos()const
    {
        return ipInfos;
    }
    vector<TIPInfo>& GetIpInfos()
    {
        return ipInfos;
    }
    void SetIpInfos(const vector<TIPInfo>& tIpInfos)
    {
        ipInfos = tIpInfos;
    }
    const int GetNicSpeed()const
    {
        return nicSpeed;
    }
    int GetNicSpeed()
    {
        return nicSpeed;
    }
    void SetNicSpeed(const int dwNicSpeed)
    {
        nicSpeed = dwNicSpeed;
    }
    const TNICMode GetNicMode()const
    {
        return nicMode;
    }
    TNICMode GetNicMode()
    {
        return nicMode;
    }
    void SetNicMode(const TNICMode enNicMode)
    {
        nicMode = enNicMode;
    }
 
private:
    int nicIndex; // �����ڲ���ϵͳ�е�˳��(��0��ʼ), nicҲ��NIC(network interface card)
    vector<TIPInfo> ipInfos;
    int nicSpeed; // ��λ��M
    TNICMode nicMode;
    int connectStatus;  //e_CONNECTION/e_DISCONNECTION
    double netIO; //��¼���������� (Mb/s)
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetConnectStatus(int cConnectStatus)
    {
        connectStatus = cConnectStatus;
    }
    int& GetConnectStatus()
    {
        return connectStatus;
    }
    const int& GetConnectStatus() const
    {
        return connectStatus;
    }

public:
    void SetNetIO(double nNetIO)
    {
        netIO = nNetIO;
    }
    double GetNetIO() const
    {
        return netIO;
    }

};
class CNetRouteInfo
{
public:
	CNetRouteInfo()
	{
		nicIndex = 0;
	}

    const string& GetDestIP()const
    {
        return destIP;
    }
    string& GetDestIP()
    {
        return destIP;
    }
    void SetDestIP(const string& strDestIP)
    {
        destIP = strDestIP;
    }
    const string& GetDestIPMask() const
    {
        return destIPMask;
    }
    string& GetDestIPMask()
    {
        return destIPMask;
    }
    void SetDestIPMask(const string& strDestIPMask)
    {
        destIPMask = strDestIPMask;
    }
    const string& GetGateway()const
    {
        return gateway;
    }
    string& GetGateway()
    {
        return gateway;
    }
    void SetGateway(const string& strGateway)
    {
        gateway = strGateway;
    }
    const int GetNicIndex() const
    {
        return nicIndex;
    }
    int GetNicIndex()
    {
        return nicIndex;
    }
    void SetNicIndex(const int dwNicIndex)
    {
        nicIndex = dwNicIndex;
    }
 
private:
    string destIP;
    string destIPMask;
    string gateway;
    int nicIndex;  // (֧��ָ��NIC) �����ڲ���ϵͳ�е�˳��(��0��ʼ), -1��ʾ�Զ�ѡ��
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
class CNetRouteInfoKey
{
public:
    const string& GetDestIP()const
    {
        return destIP;
    }
    string& GetDestIP()
    {
        return destIP;
    }
    void SetDestIP(const string& strDestIP)
    {
        destIP = strDestIP;
    }
    const string& GetDestIPMask()const
    {
        return destIPMask;
    }
    string& GetDestIPMask()
    {
        return destIPMask;
    }
    void SetDestIPMask(const string& strDestIPMask)
    {
        destIPMask = strDestIPMask;
    }

private:
    string destIP;
    string destIPMask;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
class CNetDefaultRoute
{
public:
	CNetDefaultRoute()
	{
		nicIndex = 0;
	}

    const string& GetDefaultGateway()const
    {
        return defaultGateway;
    }
    string& GetDefaultGateway()
    {
        return defaultGateway;
    }
    void SetDefaultGateway(const string& strDefaultGateway)
    {
        defaultGateway = strDefaultGateway;
    }
    const int GetNicIndex()const
    {
        return nicIndex;
    }
    int   GetNicIndex()
    {
        return nicIndex;
    }
    void SetNicIndex(const int dwNicIndex)
    {
        nicIndex = dwNicIndex;
    }

private:
    string defaultGateway;
    int nicIndex; // �����ڲ���ϵͳ�е�˳��(��0��ʼ), -1��ʾ�Զ�ѡ��
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
class CNetNatInfo
{
public:
	CNetNatInfo()
	{
		platformPort = 0;
		platformNatPort = 0;
	}

    const string& GetPlatformIP()const
    {
        return platformIP;
    }
    string& GetPlatformIP()
    {
        return platformIP;
    }
    void SetPlatformIP(const string& strPlatformIP)
    {
        platformIP = strPlatformIP;
    }
    const int GetPlatformPort()const
    {
        return platformPort;
    }
    int GetPlatformPort()
    {
        return platformPort;
    }
    void SetPlatformPort(const int dwPlatformPort)
    {
        platformPort = dwPlatformPort;
    }
    const string& GetPlatformNatIP()const
    {
        return platformNatIP;
    }
    string& GetPlatformNatIP()
    {
        return platformNatIP;
    }
    void SetPlatformNatIP(const string& strPlatformNatIP)
    {
        platformNatIP = strPlatformNatIP;
    }
    const int GetPlatformNatPort()const
    {
        return platformNatPort;
    }
    int GetPlatformNatPort()
    {
        return platformNatPort;
    }
    void SetPlatformNatPort(const int dwPlatformNatPort)
    {
        platformNatPort = dwPlatformNatPort;
    }

private:
    string platformIP;
    int platformPort;
    string platformNatIP;
    int platformNatPort;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
typedef CNetNatInfo CNetNatInfoKey;

enum TPlatformRelation 
{
    enPlatformRelation_Up,   // �ϼ�ƽ̨
    enPlatformRelation_Down, // �¼�ƽ̨
    enPlatformRelation_Slave,// ��ƽ̨
    enPlatformRelation_Master// ��ƽ̨
};

enum TPlatformNetType
{
    enPmsTopoNat_NoNat = 0,
    enPmsTopoNat_UpNat = 1,
    enPmsTopoNat_DownNat = 2,
    enPmsTopoNat_AllNat = 3,
};

class CPlatformTOPO
{
public:
	CPlatformTOPO()
	{
		platformRelation = enPlatformRelation_Up;
        platformNetType = enPmsTopoNat_NoNat;
        localNicIndex = -1;
        httpPort = 80;
		httpsPort = 443;
	}

    bool IsValid() const
    {
        return (enPlatformRelation_Up == platformRelation || enPlatformRelation_Down == platformRelation 
            || enPlatformRelation_Slave == platformRelation || enPlatformRelation_Master == platformRelation )
            && !platformName.empty() && !pmsId.empty() && !platformAddr.empty();
    }

    const TPlatformRelation GetPlatformRelation()const
    {
        return platformRelation;
    }
    TPlatformRelation GetPlatformRelation()
    {
        return platformRelation;
    }
    void SetPlatformRelation(const TPlatformRelation enPlatformRelation)
    {
        platformRelation = enPlatformRelation;
    }
    const string& GetPlatformName()const
    {
        return platformName;
    }
    string& GetPlatformName()
    {
        return platformName;
    }
    void SetPlatformName(const string& strName)
    {
        platformName = strName;
    }
    const string& GetPmsId()const
    {
        return pmsId;
    }
    string& GetPmsId()
    {
        return pmsId;
    }
    void SetPmsId(const string& strPmsId)
    {
        pmsId = strPmsId;
    }
    void SetPlatformAddr(const string& cPlatformAddr)
    {
        platformAddr = cPlatformAddr;
    }
    string& GetPlatformAddr()
    {
        return platformAddr;
    }
    const string& GetPlatformAddr() const
    {
        return platformAddr;
    }
    void SetPlatformNetType(TPlatformNetType ePlatformNetType)
    {
        platformNetType = ePlatformNetType;
    }
    TPlatformNetType GetPlatformNetType() const
    {
        return platformNetType;
    }
    void SetLocalNicIndex(int nLocalNicIndex)
    {
        localNicIndex = nLocalNicIndex;
    }
    int GetLocalNicIndex() const
    {
        return localNicIndex;
    }

private:
    TPlatformRelation platformRelation; // ��ϵ��վ��TOPO����ƽ̨����ĽǶ�������
    string platformName;                // ƽ̨��TOPO�е�����, platformRelationΪ��ƽ̨ʱ��Ϊ��ƽ̨������������Ϊƽ̨����
    string pmsId;                       // ƽ̨PMS��sipͨ�ű�ʶ����������Ϣ
    string platformAddr;                // ƽ̨PMS�ĵ�ַ
    int httpPort;                       // ƽ̨Http�ļ����˿�
	int httpsPort;                      // ƽ̨Https�ļ����˿�
    TPlatformNetType platformNetType;
    int localNicIndex;                  // ��ƽ̨������������������,��0��ʼ, -1��ʾ�Զ�ѡ�����ߵ�����ƥ�����  ע�⣺Ŀǰֻ֧������Ӧ
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetHttpPort(int nHttpPort)
    {
        httpPort = nHttpPort;
    }
    int GetHttpPort() const
    {
        return httpPort;
    }

    void SetHttpsPort(int nHttpsPort)
    {
        httpsPort = nHttpsPort;
    }
    int GetHttpsPort() const
    {
        return httpsPort;
    }

};
class CPlatformModuleInfo
{
public:
	CPlatformModuleInfo()
	{
		enabled = false;
		status  = enModuleStatus_Runing;
	}

    enum TModuleStatus
    {
        enModuleStatus_Runing,
        enModuleStatus_NotRuning
    };
    const string& GetName()const
    {
        return name;
    }
    string& GetName()
    {
        return name;
    }
    void SetName(const string& strName)
    {
        name = strName;
    }
    const UUIDString& GetUuid()const
    {
        return uuid;
    }
    UUIDString& GetUuid()
    {
        return uuid;
    }
    void SetUuid(const UUIDString& strUuid)
    {
        uuid = strUuid;
    }
    const bool GetEnabled()const
    {
        return enabled;
    }
    bool GetEnabled()
    {
        return enabled;
    }
    void SetEnabled(const bool bEnabled)
    {
        enabled = bEnabled;
    }
    const TModuleStatus GetStatus()const
    {
        return status;
    }
    TModuleStatus GetStatus()
    {
        return status;
    }
    void SetStatus(const TModuleStatus enStatus)
    {
        status = enStatus;
    }
    const string& GetDescLink()const
    {
        return descLink;
    }
    string& GetDescLink()
    {
        return descLink;
    }
    void SetDescLink(const string& strDescLink)
    {
        descLink = strDescLink;
    }
    const string& GetSettingLink()const
    {
        return settingLink;
    }
    string& GetSettingLink()
    {
        return settingLink;
    }
    void SetSettingLink(const string& strSettingLink)
    {
        settingLink = strSettingLink;
    }
private:
    string name; 
    UUIDString uuid;
    bool enabled;
	string descLink;
	string settingLink;
    TModuleStatus status;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPlatformLogInfoQryCondition
{
public:
//     bool GetDateTime(string &strDateTime) const
//     {      
//         if (!optionField.HasOption("dateTime"))
//             return false;
//         strDateTime = dateTime;
//         return true;
//     }
//     void SetDateTime(const string &strDateTime)
//     {
//         optionField.SetOption("dateTime");
//         dateTime = strDateTime;
//     }

//     bool GetPlatformName(string &strPlatformName) const
//     {      
//         if (!optionField.HasOption("platformName"))
//             return false;
//         strPlatformName = platformName;
//         return true;
//     }
//     void SetPlatformName(const string &strPlatformName)
//     {
//         optionField.SetOption("platformName");
//         platformName = strPlatformName;
//     }
// 
//     bool GetPlatformID(string &strPlatformID) const
//     {      
//         if (!optionField.HasOption("platformID"))
//             return false;
//         strPlatformID = platformID;
//         return true;
//     }
//     void SetPlatformID(const string &strPlatformID)
//     {
//         optionField.SetOption("platformID");
//         platformID = strPlatformID;
//     }
// 
//     bool GetDesc(string &strDesc) const
//     {      
//         if (!optionField.HasOption("desc"))
//             return false;
//         strDesc = desc;
//         return true;
//     }
//     void SetDesc(const string &strDesc)
//     {
//         optionField.SetOption("desc");
//         desc = strDesc;
//     }
private:
    CQryOptionField optionField;
    string startTime;              //��־��ʼʱ��
    string endTime;             //��־����ʱ��
//    string dateTime; // ��־����ʱ��
//     string platformName; // �������û�, ����Ϊϵͳ��ĳpmc�û�
//     string platformID;
//    string desc;// ��Ϊ����
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetStartTime(const string& strStartTime)
    {
        optionField.SetOption("startTime");
        startTime = strStartTime;
    }
    bool GetStartTime(string& strStartTime) const
    {
        if (!optionField.HasOption("startTime"))
            return false;
        strStartTime = startTime;
        return true;
    }

    void SetEndTime(const string& strEndTime)
    {
        optionField.SetOption("endTime");
        endTime = strEndTime;
    }
    bool GetEndTime(string& strEndTime) const
    {
        if (!optionField.HasOption("endTime"))
            return false;
        strEndTime = endTime;
        return true;
    }
//     void SetStartTime(const string& strStartTime)
//     {
//         startTime = strStartTime;
//     }
//     string& GetStartTime()
//     {
//         return startTime;
//     }
//     const string& GetStartTime() const
//     {
//         return startTime;
//     }
// 
//     void SetEndTime(const string& strEndTime)
//     {
//         endTime = strEndTime;
//     }
//     string& GetEndTime()
//     {
//         return endTime;
//     }
//     const string& GetEndTime() const
//     {
//         return endTime;
//     }

};

class CPlatformLogInfo
{
public:
    const string& GetDateTime()const
    {
        return dateTime;
    }
    string& GetDateTime()
    {
        return dateTime;
    }
    void SetDateTime(const string& strDateTime)
    {
        dateTime = strDateTime;
    }

//     const string& GetPlatformName()const
//     {
//         return platformName;
//     }
//     string& GetPlatformName()
//     {
//         return platformName;
//     }
//     void SetPlatformName(const string& strPlatformName)
//     {
//         platformName = strPlatformName;
//     }
// 
//     const string& GetPlatformID()const
//     {
//         return platformID;
//     }
//     string& GetPlatformID()
//     {
//         return platformID;
//     }
//     void SetPlatformID(const string& strPlatformID)
//     {
//         platformID = strPlatformID;
//     }

    const string& GetDesc()const
    {
        return desc;
    }
    string& GetDesc()
    {
        return desc;
    }
    void SetDesc(const string& strDesc)
    {
        desc = strDesc;
    }
private:
    string dateTime; // ��־����ʱ��
//     string platformName; // �������û�, ����Ϊϵͳ��ĳpmc�û�
//     string platformID;
    string desc;// ��Ϊ����
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUserOperationLogQryCondition
{
public:

//     bool GetDateTime(string &strDateTime) const
//     {      
//         if (!optionField.HasOption("dateTime"))
//             return false;
//         strDateTime = dateTime;
//         return true;
//     }
//     void SetDateTime(const string &strDateTime)
//     {
//         optionField.SetOption("dateTime");
//         dateTime = strDateTime;
//     }

    bool GetUserName(string &strUserName) const
    {      
        if (!optionField.HasOption("userName"))
            return false;
        strUserName = userName;
        return true;
    }
    void SetUserName(const string &strUserName)
    {
        optionField.SetOption("userName");
        userName = strUserName;
    }

    bool GetUserID(string &strUserID) const
    {      
        if (!optionField.HasOption("userID"))
            return false;
        strUserID = userID;
        return true;
    }
    void SetUserID(const string &strUserID)
    {
        optionField.SetOption("userID");
        userID = strUserID;
    }

    bool GetIp(string &strIp) const
    {      
        if (!optionField.HasOption("ip"))
            return false;
        strIp = ip;
        return true;
    }
    void SetIp(const string &strIp)
    {
        optionField.SetOption("ip");
        ip = strIp;
    }

//     bool GetOperation(string &strOperation) const
//     {      
//         if (!optionField.HasOption("operation"))
//             return false;
//         strOperation = operation;
//         return true;
//     }
//     void SetOperation(const string &strOperation)
//     {
//         optionField.SetOption("operation");
//         operation = strOperation;
//     }
private:
    CQryOptionField optionField;
    string startTime;             //��־����ʱ��
    string endTime;              //��־����ʱ��
//    string dateTime; // ��־����ʱ��
    string userName; // �������û�, ĳpmc�û�
    string userID;
    string ip; //��ǰ�û�ʹ�õ�ip��ַ
//    string operation;// ��������Ϊ����
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetStartTime(const string& strStartTime)
    {
        optionField.SetOption("startTime");
        startTime = strStartTime;
    }
    bool GetStartTime(string& strStartTime) const
    {
        if (!optionField.HasOption("startTime"))
            return false;
        strStartTime = startTime;
        return true;
    }

    void SetEndTime(const string& strEndTime)
    {
        optionField.SetOption("endTime");
        endTime = strEndTime;
    }
    bool GetEndTime(string& strEndTime) const
    {
        if (!optionField.HasOption("endTime"))
            return false;
        strEndTime = endTime;
        return true;
    }
};

class CUserOperationLogInfo
{
public:
    const string& GetDateTime()const
    {
        return dateTime;
    }
    string& GetDateTime()
    {
        return dateTime;
    }
    void SetDateTime(const string& strDateTime)
    {
        dateTime = strDateTime;
    }
    const string& GetUserName()const
    {
        return userName;
    }
    string& GetUserName()
    {
        return userName;
    }
    void SetUserName(const string& strUserNmae)
    {
        userName = strUserNmae;
    }
    const string& GetUserID()const
    {
        return userID;
    }
    string& GetUserID()
    {
        return userID;
    }
    void SetUserID(const string& strUserID)
    {
        userID = strUserID;
    }
    const string& GetOperation()const
    {
        return operation;
    }
    string& GetOperation()
    {
        return operation;
    }
    void SetOperation(const string& strOperation)
    {
        operation = strOperation;
    }
    const string& GetIp()const
    {
        return ip;
    }
    string& GetIp()
    {
        return ip;
    }
    void SetIp(const string& strIp)
    {
        ip = strIp;
    }
private:
    string dateTime; // ��־����ʱ��
    string userName; // �������û�, ĳpmc�û�
    string userID;
    string ip; //��ǰ�û�ʹ�õ�ip��ַ
    string operation;// ��������Ϊ����
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

// ��Ԫ�豸����
enum TNMSDevType
{
    enNmsDevTypeInvalid = 0,
    enNmsDevTypeServer = 1,  // ƽ̨����
    enNmsDevTypePu = 2       // ǰ������
}; 
enum TNmsAlarmLevel
{
    enNMS_ALARM_LEVEL_NORMAL                    =0x00,       // �豸����
    enNMS_ALARM_LEVEL_GENERAL                   =0x01,       // ��ͨ�澯
    enNMS_ALARM_LEVEL_IMPORTANT                 =0x02,       // ��Ҫ�澯
    enNMS_ALARM_LEVEL_CRITICAL                  =0x04,       // ���ظ澯
    enNMS_ALARM_LEVEL_UNKNOW                    =0x08        // δ֪�澯
};

class CNMSDeviceQueryCondition
{
public:
    CNMSDeviceQueryCondition()
    {
        type = enNmsDevTypeInvalid;
        isOnline = false;
    }
    bool GetID(UUIDString& strId)const
    {
        if (!optionField.HasOption("id"))
            return false;
        strId = id;
        return true;
    }
    void SetID(const UUIDString& strId)
    {
        optionField.SetOption("id");
        id = strId;
    }

    bool GetParentID(UUIDString& strParentId)const
    {
        if (!optionField.HasOption("parentId"))
            return false;
        strParentId = parentId;
        return true;
    }
    void SetParentID(const UUIDString& strParentId)
    {
        optionField.SetOption("parentId");
        parentId = strParentId;
    }

    bool GetNMSDevType(TNMSDevType & val) const
    {
        if (!optionField.HasOption("type"))
            return false;
        val = type;
        return true;
    }
    void SetNMSDevType(const TNMSDevType &val)
    {
        optionField.SetOption("type");
        type = val;
    }

    bool GetName(string &strName) const
    {      
        if (!optionField.HasOption("name"))
            return false;
        strName = name;
        return true;
    }
    void SetName(const string &strName)
    {
        optionField.SetOption("name");
        name = strName;
    }

    bool GetAddress(string &strAddress) const
    {      
        if (!optionField.HasOption("address"))
            return false;
        strAddress = address;
        return true;
    }
    void SetAddress(const string &strAddress)
    {
        optionField.SetOption("address");
        address = strAddress;
    }

    bool GetIsOnline(bool &bIsOnline) const
    {      
        if (!optionField.HasOption("isOnline"))
            return false;
        bIsOnline = isOnline;
        return true;
    }
    void SetIsOnline(const bool &bIsOnline)
    {
        optionField.SetOption("isOnline");
        isOnline = bIsOnline;
    }
private: // parentId, address�ֶ���ʱ��֧����Ϊ��������
    CQryOptionField optionField; 
    UUIDString id;
    UUIDString parentId;
    TNMSDevType type;
    string name;
    string address;

    bool isOnline;    // �豸�Ƿ�����
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CNMSDeviceInfo
{
public:
	CNMSDeviceInfo()
	{
		type = enNmsDevTypeInvalid;
		isOnline = false;
        highestAlarmLevel = enNMS_ALARM_LEVEL_NORMAL;
	}

    const UUIDString& GetId()const
    {
        return id;
    }
    UUIDString& GetId()
    {
        return id;
    }
    void SetId(const UUIDString& strId)
    {
        id = strId;
    }
    const UUIDString& GetParentId()const
    {
        return parentId;
    }
    UUIDString& GetParentId()
    {
        return parentId;
    }
    void SetParentId(const UUIDString& strParentId)
    {
        parentId = strParentId;
    }
    const TNMSDevType GetType()const
    {
        return type;
    }
    TNMSDevType GetType()
    {
        return type;
    }
    void SetType(const TNMSDevType enType)
    {
        type = enType;
    }
    void SetSubType(const string& strSubType)
    {
        subType = strSubType;
    }
    string& GetSubType()
    {
        return subType;
    }
    const string& GetSubType() const
    {
        return subType;
    }
    const string& GetName()const
    {
        return name;
    }
    string& GetName()
    {
        return name;
    }
    void SetName(const string& strName)
    {
        name = strName;
    }
    const string& GetAddress()const
    {
        return address;
    }
    string& GetAddress()
    {
        return address;
    }
    void SetAddress(const string& strAddress)
    {
        address = strAddress;
    }
    const bool GetIsOnline()const
    {
        return isOnline;
    }
    bool GetIsOnline()
    {
        return isOnline;
    }
    void SetIsOnline(const bool bIsOnline)
    {
        isOnline = bIsOnline;
    }
    TNmsAlarmLevel GetHighestAlarmLevel() const
    {
        return highestAlarmLevel;
    }
    void SetHighestAlarmLevel(TNmsAlarmLevel tLevel)
    {
        highestAlarmLevel = tLevel;
    }
 
private:
    UUIDString id;
    UUIDString parentId;  // ������ʵ����Ԫ�Ĳ�ι�ϵ��Ϊ��ʱ��ʾû�и���Ԫ
    TNMSDevType type;     // ��Ԫ���࣬�������Ϊƽ̨�豸С��ΪCMU������Ϊǰ���豸С��ΪIPC112
    string subType;       // ��Ԫ����
    string name;
    string address;

    bool isOnline;    // �豸�Ƿ�����
    TNmsAlarmLevel highestAlarmLevel; // ��߸澯����
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CNmsAlarmQueryCondition
{
public:
    CNmsAlarmQueryCondition()
    {
    }

    //devId
    bool GetDevId(UUIDString & val) const
    {
        if (!optionField.HasOption("devId"))
            return false;
        val = devId;
        return true;
    }
    void SetDevId(const UUIDString &val)
    {
        optionField.SetOption("devId");
        devId = val;
    }

    //devCategory
    bool GetDevCategory(TDevCategory & val) const
    {
        if (!optionField.HasOption("devCategory"))
            return false;
        val = devCategory;
        return true;
    }
    void SetDevCategory(const TDevCategory &val)
    {
        optionField.SetOption("devCategory");
        devCategory = val;
    }

    //alarmState
    bool GetAlarmState(TAlarmState & val) const
    {
        if (!optionField.HasOption("alarmState"))
            return false;
        val = alarmState;
        return true;
    }
    void SetAlarmState(const TAlarmState &val)
    {
        optionField.SetOption("alarmState");
        alarmState = val;
    }

    //timeRange
    bool GetTimeRange(TTimeRange & val) const
    {
        if (!optionField.HasOption("timeRange"))
            return false;
        val = timeRange;
        return true;
    }
    void SetTimeRange(const TTimeRange &val)
    {
        optionField.SetOption("timeRange");
        timeRange = val;
    }

private:
    CQryOptionField optionField;
    UUIDString      devId;
    string          devName;             
    TDevCategory    devCategory;  // �豸����
    TAlarmState     alarmState;
    TTimeRange      timeRange;

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetDevName(const string& strDevName)
    {
        optionField.SetOption("devName");
        devName = strDevName;
    }
    bool GetDevName(string & val) const
    {
        if (!optionField.HasOption("devName"))
        {
            return false;
        }
        val = devName;
        return true;
    }
};

class CNMSAlarmInfo
{
public:
	CNMSAlarmInfo()
	{
		alarmCode = 0;
        alarmLevel = enNMS_ALARM_LEVEL_NORMAL;
        endTime = ALARM_RESUME_TIME_INVALIDVAL;
	}

private:
    string          devAlias;                
    UUIDString      devId;                   
    string          chnId;                      //�澯ͨ�������ݲ�һ�������֣��п������ַ�����
    int             alarmCode;               
    int             serialNo;             
    string          alarmDesc;               
    string          devType;                    //�����澯���豸����
    TNmsAlarmLevel  alarmLevel;              
    string          beginTime;               
    string          endTime;                    //endʱ����Ч��ʾ�澯δ��ֹ
    TAlarmEndMethod endMethod;                  //��ֹ�澯��ʽ
    UUIDString      endUserId;                  //��ֹ�澯���û�
    string          alarmResume;                //�澯�������

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetDevAlias(const string& strDevAlias)
    {
        devAlias = strDevAlias;
    }
    string& GetDevAlias()
    {
        return devAlias;
    }
    const string& GetDevAlias() const
    {
        return devAlias;
    }

    void SetDevId(const UUIDString& strDevId)
    {
        devId = strDevId;
    }
    UUIDString& GetDevId()
    {
        return devId;
    }
    const UUIDString& GetDevId() const
    {
        return devId;
    }

    void SetChnId(const string& strChnId)
    {
        chnId = strChnId;
    }
    string& GetChnId()
    {
        return chnId;
    }
    const string& GetChnId() const
    {
        return chnId;
    }

    void SetAlarmCode(int nAlarmCode)
    {
        alarmCode = nAlarmCode;
    }
    int GetAlarmCode() const
    {
        return alarmCode;
    }

    void SetAlarmDesc(const string& strAlarmDesc)
    {
        alarmDesc = strAlarmDesc;
    }
    string& GetAlarmDesc()
    {
        return alarmDesc;
    }
    const string& GetAlarmDesc() const
    {
        return alarmDesc;
    }

    void SetAlarmLevel(TNmsAlarmLevel tAlarmLevel)
    {
        alarmLevel = tAlarmLevel;
    }
    TNmsAlarmLevel GetAlarmLevel() const
    {
        return alarmLevel;
    }

    void SetBeginTime(const string& strBeginTime)
    {
        beginTime = strBeginTime;
    }
    string& GetBeginTime()
    {
        return beginTime;
    }
    const string& GetBeginTime() const
    {
        return beginTime;
    }

    void SetEndTime(const string& strEndTime)
    {
        endTime = strEndTime;
    }
    string& GetEndTime()
    {
        return endTime;
    }
    const string& GetEndTime() const
    {
        return endTime;
    }

    void SetEndMethod(const TAlarmEndMethod& strEndMethod)
    {
        endMethod = strEndMethod;
    }
    TAlarmEndMethod& GetEndMethod()
    {
        return endMethod;
    }
    const TAlarmEndMethod& GetEndMethod() const
    {
        return endMethod;
    }

    void SetEndUserId(const UUIDString& strEndUserId)
    {
        endUserId = strEndUserId;
    }
    UUIDString& GetEndUserId()
    {
        return endUserId;
    }
    const UUIDString& GetEndUserId() const
    {
        return endUserId;
    }

public:
    void SetSerialNo(int nSerialNo)
    {
        serialNo = nSerialNo;
    }
    int GetSerialNo() const
    {
        return serialNo;
    }

public:
    void SetDevType(const string& strDevType)
    {
        devType = strDevType;
    }
    string& GetDevType()
    {
        return devType;
    }
    const string& GetDevType() const
    {
        return devType;
    }

public:
    void SetAlarmResume(const string& strAlarmResume)
    {
        alarmResume = strAlarmResume;
    }
    string& GetAlarmResume()
    {
        return alarmResume;
    }
    const string& GetAlarmResume() const
    {
        return alarmResume;
    }

};

class CProxyConfigInfo
{
public:
	CProxyConfigInfo()
	{
		port = 0;
	}

    const string& GetIp() const 
    {
        return ip;
    }
    string& GetIp()
    {
        return ip;
    }
    void SetIp(const string& tIp) 
    {
        ip = tIp;
    }

    const int GetPort() const 
    {
        return port;
    }
    int GetPort()
    {
        return port;
    }
    void SetPort(const int dwPort) 
    {
        port = dwPort;
    }
private:
    string ip;
    int port;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CSTUNServerConfigInfo
{
public:
	CSTUNServerConfigInfo()
	{
		port = 0;
	}
    const string& GetIp() const 
    {
        return ip;
    }
    string& GetIp()
    {
        return ip;
    }
    void SetIp(const string& tIp) 
    {
        ip = tIp;
    }

    int GetPort() const 
    {
        return port;
    }
    int GetPort()
    {
        return port;
    }
    void SetPort(const int dwPort) 
    {
        port = dwPort;
    }
private:
    string ip;
    int port;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CMailCfgInfo
{
public:
    CMailCfgInfo()
    {

    }
private:
    string mailServer;  //�ʼ���������ַ
    string mailUser;    //�ʻ���
    string passwd;      //����
    vector<string> userList; //�������б�
public:
    void SetMailServer(const string& strMailServer)
    {
        mailServer = strMailServer;
    }
    string& GetMailServer()
    {
        return mailServer;
    }
    const string& GetMailServer() const
    {
        return mailServer;
    }

    void SetMailUser(const string& strMailUser)
    {
        mailUser = strMailUser;
    }
    string& GetMailUser()
    {
        return mailUser;
    }
    const string& GetMailUser() const
    {
        return mailUser;
    }

    void SetPasswd(const string& strPasswd)
    {
        passwd = strPasswd;
    }
    string& GetPasswd()
    {
        return passwd;
    }
    const string& GetPasswd() const
    {
        return passwd;
    }

    void SetUserList(const vector<string>& cUserList)
    {
        userList = cUserList;
    }
    vector<string>& GetUserList()
    {
        return userList;
    }
    const vector<string>& GetUserList() const
    {
        return userList;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


typedef string TUPnPProtocolType;
#define UPNP_PROTOCOL_TCP						NAT_PROTOCOL_TCP
#define UPNP_PROTOCOL_UDP						NAT_PROTOCOL_UDP

typedef TUPnPProtocolType TPortMapType;
#define PORTMAP_PROTOCAL_TCP NAT_PROTOCOL_TCP
#define PORTMAP_PROTOCAL_UDP NAT_PROTOCOL_UDP

typedef int TUPnPItemFlag;
#define UPNP_ITEM_PRESET   0
#define UPNP_ITEM_CUSTOM   1

typedef int TUPnPOperMode;
#define UPNP_OPER_MODE_MANUAL  0
#define UPNP_OPER_MODE_HALF    1
#define UPNP_OPER_MODE_AUTO    2




typedef TUPnPProtocolType TPortMapType;
#define  PORTMAP_PROTOCAL_TCP NAT_PROTOCOL_TCP
#define  PORTMAP_PROTOCAL_UDP NAT_PROTOCOL_UDP



struct TPortMapItem
{
private:
    string platIp;
    int platPort;
    TPortMapType protocol;
    string wanIp;
    long wanPort;
public:
    TPortMapItem()
    {
        Clear();
    }
    void Clear()
    {
        platIp.clear();
        platPort = 0;
        protocol.clear();
        wanIp.clear();
        wanPort = 0;
    }


public:
    void SetPlatIp(const string& strPlatIp)
    {
        platIp = strPlatIp;
    }
    const string& GetPlatIp() const 
    {
        return platIp;
    }
    string& GetPlatIp()
    {
        return platIp;
    }

    const int GetPlatPort() const
    {
        return platPort;
    }
    int GetPlatPort()
    {
        return platPort;
    }
    void SetPlatPort(int nPlatPort)
	{
		platPort = nPlatPort;
	}

	const TUPnPProtocolType &GetProtocol() const
	{
		return protocol;
	}
	TUPnPProtocolType &GetProtocol()
	{
		return protocol;
	}
	void SetProtocol(const TUPnPProtocolType &tProtocol)
	{
		protocol = tProtocol;
	}

	const string &GetWanIp() const
	{
		return wanIp;
	}
	string &GetWanIp()
	{
		return wanIp;
	}
	void SetWanIp(const string &strWanIp)
	{
		wanIp = strWanIp;
	}

	int GetWanPort() const
	{
		return wanPort;
	}
	void SetWanPort(int nWanPort)
	{
		wanPort = nWanPort;
	}
    
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

typedef vector<TPortMapItem> TPortMapList;



struct TUPnPMapItem
{
private:
	int                platPort;
	TUPnPProtocolType  protocol;
	string wanIp;
	long   wanPort;
    bool   enable;
	int    status;
	string remark;
    TUPnPItemFlag flag;

public:
    TUPnPMapItem()
    {
        platPort = 0;
        protocol = UPNP_PROTOCOL_UDP;
        wanPort  = 0;
        enable   = false;
		status   = 0;
        flag     = UPNP_ITEM_CUSTOM;
    }

public:
	const int GetPlatPort() const
	{
		return platPort;
	}
    int GetPlatPort()
    {
        return platPort;
    }
	void SetPlatPort(int nPlatPort)
	{
		platPort = nPlatPort;
	}

	const TUPnPProtocolType &GetProtocol() const
	{
		return protocol;
	}
	TUPnPProtocolType &GetProtocol()
	{
		return protocol;
	}
	void SetProtocol(const TUPnPProtocolType &tProtocol)
	{
		protocol = tProtocol;
	}

	const string &GetWanIp() const
	{
		return wanIp;
	}
	string &GetWanIp()
	{
		return wanIp;
	}
	void SetWanIp(const string &strWanIp)
	{
		wanIp = strWanIp;
	}

    const int GetWanPort() const
	{
		return wanPort;
	}
    int GetWanPort()
	{
		return wanPort;
	}
	void SetWanPort(int nWanPort)
	{
		wanPort = nWanPort;
	}

	bool GetEnable() const
	{
		return enable;
	}
	void SetEnable(bool bEnable)
	{
		enable = bEnable;
	}

	int GetStatus() const
	{
		return status;
	}
	void SetStatus(int nStatus)
	{
		status = nStatus;
	}

	const string &GetRemark() const
	{
		return remark;
	}
	string &GetRemark()
	{
		return remark;
	}
	void SetRemark(const string &strRemark)
	{
		remark = strRemark;
	}

	TUPnPItemFlag GetFlag() const
	{
		return flag;
	}
	void SetFlag(TUPnPItemFlag nFlag)
	{
		flag = nFlag;
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

typedef vector<TUPnPMapItem> TUPnPMapList;


struct TModuleConf
{
public:
	TModuleConf()
	{
		
	}

	void Clear()
	{
        moduleName.clear();
		netAddr.Clear();
	}
    string moduleName;
	TNetAddr netAddr;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

#endif // _PMS_STRUCT_H_















