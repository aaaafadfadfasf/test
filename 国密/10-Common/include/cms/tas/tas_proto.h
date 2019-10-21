/*****************************************************************************
模块名      : tas_proto
文件名      : tas_proto.h
相关文件    : 
文件实现功能: tas_proto.h 定义3as外部消息的消息体
作者        : liangli
版本        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2011/10/22   1.0         liangli        创建
2011/10/31   1.0         huzhiyun         
******************************************************************************/
#ifndef _TAS_PROTO_H_
#define _TAS_PROTO_H_

#include "cms/tas/tas_event.h"
#include "cms/tas/tas_struct.h"


class CLoginTasReq: public CEventReq
{
public:
    CLoginTasReq()
    {
        event = OspExtEventDesc(TAS_LOGIN_REQ);
        loginType = enLoginType_CMU;
    }
public:
    enum TLoginType
    {
        enLoginType_CMU = 1,  // cmu登录，用户名(cmu的uri)
        enLoginType_CUI = 2,  // cui登录，用户名(cui的uri)
        enLoginType_TAC = 3,  // tac登录，用户名(tac用户名) 密码(tac密码) 扩展信息(登录模块(比如pmcs)的uri)
        enLoginType_SubTAS = 4, // 下级tas登录, 用户名(下级3as的uri)，密码(下级3as的domainId)
        enLoginType_P2PTAS = 5, // 点对点tas登录, 用户名(客户端的uri), 密码(客户端的domainId), 扩展参数(ENSyncType数值字符串)
        enLoginType_Peripheral = 6,  // 外设模块登录，用户名(外设的uri) 扩展信息(TPSLoginExternInfo的序列化内容)
    };
    void SetLoginType(TLoginType val)
    {
        loginType = val;
    }
    TLoginType GetLoginType() const
    {
        return loginType;
    }
    string& GetUserName()
    {
        return userName;
    }
    const string& GetUserName() const
    {
        return userName;
    }
    void SetUserName(const string &val)
    {
        userName = val;
    }
    string& GetPassword()
    {
        return password;
    }
    const string& GetPassword() const
    {
        return password;
    }
    void SetPassword(const string &val)
    {
        password = val;
    }
    CTransparentData& GetExtraInfo()
    {
        return extraInfo;
    }
    const CTransparentData& GetExtraInfo() const
    {
        return extraInfo;
    }
    void SetExtraInfo(const CTransparentData &val)
    {
        extraInfo = val;
    }
private:
    TLoginType  loginType;      // 登录类型
    string      userName;       // 模块uri或者用户名，由登录类型决定
    string      password;
    CTransparentData   extraInfo; // 扩展信息

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CLoginTasRsp: public CEventRsp
{
public:
    CLoginTasRsp()
    {
        event = OspExtEventDesc(TAS_LOGIN_RSP);
    }
    // 登录成功时在CEventRsp中返回服务端生成的会话号

public:
	CTransparentData& GetExtraInfo()
	{
		return extraInfo;
	}
	const CTransparentData& GetExtraInfo() const
	{
		return extraInfo;
	}
	void SetExtraInfo(const CTransparentData &val)
	{
		extraInfo = val;
	}

private:
	CTransparentData   extraInfo; // 扩展信息

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CLogoutTasReq: public CEventReq
{
public:
    CLogoutTasReq()
    {
        event = OspExtEventDesc(TAS_LOGOUT_REQ);
    }
    // 在CEventReq中提供登录成功时返回的会话号
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CLogoutTasRsp: public CEventRsp
{
public:
    CLogoutTasRsp()
    {
        event = OspExtEventDesc(TAS_LOGOUT_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CCuOnlineReq:public CEventReq
{
public:
    CCuOnlineReq()
    {
        event = OspExtEventDesc(TAS_CU_ONLINE_REQ);
    }
    const string& GetCuUserName() const
    {
        return userName;
    }
    string& GetCuUserName()
    {
        return userName;
    }
    void SetCuUserName(const string &strUserName)
    {
        userName = strUserName;
    }
    const string& GetCuUserPassword() const
    {
        return password;
    }
    string& GetCuUserPassword()
    {
        return password;
    }
    void SetCuUserPassword(const string &strPassword)
    {
        password = strPassword;
    }
private:
    string userName;     // cu用户名
    string password;     // cu密码
	string loginIp;      //登陆IP
	string clientType;   //客户端类型
    string loginSession; //由cui指定cu登录session，注意时空不能重复，建议采用uuid	               
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetLoginIp(const string& strLoginIp)
    {
        loginIp = strLoginIp;
    }
    string& GetLoginIp()
    {
        return loginIp;
    }
    const string& GetLoginIp() const
    {
        return loginIp;
    }
    void SetClientType(const string& strClientType)
    {
        clientType = strClientType;
    }
    string& GetClientType()
    {
        return clientType;
    }
    const string& GetClientType() const
    {
        return clientType;
    }

public:
    void SetLoginSession(const string& strLoginSession)
    {
        loginSession = strLoginSession;
    }
    string& GetLoginSession()
    {
        return loginSession;
    }
    const string& GetLoginSession() const
    {
        return loginSession;
    }

};

class CCuOnlineRsp:public CEventRsp
{
public:
    CCuOnlineRsp()
    {
        event = OspExtEventDesc(TAS_CU_ONLINE_RSP);
		pwdLockedLeftTime = 0;
		pwdExpireLeftTime = 0;

    }
    const string& GetCuSessionId() const
    {
        return cuSessionId;
    }
    string& GetCuSessionId()
    {
        return cuSessionId;
    }
    void SetCuSessionId(const string &strID)
    {
        cuSessionId = strID;
    }
    void SetDeviceTreeVer(const string& strDeviceTreeVer)
    {
        deviceTreeVer = strDeviceTreeVer;
    }
    string& GetDeviceTreeVer()
    {
        return deviceTreeVer;
    }
    const string& GetDeviceTreeVer() const
    {
        return deviceTreeVer;
    }
	void SetPwdLockedLeftTime(s32 nPwdLockedLeftTime)
	{
		pwdLockedLeftTime = nPwdLockedLeftTime;
	}
	s32 GetPwdLockedLeftTime() const
	{
		return pwdLockedLeftTime;
	}

	void SetPwdExpireLeftTime(s32 nPwdExpireLeftTime)
	{
		pwdExpireLeftTime = nPwdExpireLeftTime;
	}
	s32 GetPwdExpireLeftTime() const
	{
		return pwdExpireLeftTime;
	}

private:
    string cuSessionId;   // tas为cu生成的会话号
    string deviceTreeVer; // 设备树版本号
	s32 pwdLockedLeftTime;//密码被锁定剩余时间，单位：秒               
	s32 pwdExpireLeftTime;//密码有效期到期剩余时间               
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

};

class CCuOfflineReq: public CEventReq
{
public:
    CCuOfflineReq()
    {
        event = OspExtEventDesc(TAS_CU_OFFLINE_REQ);
    }
    const string& GetCuSessionId() const
    {
        return cuSessionId;
    }
    string& GetCuSessionId()
    {
        return cuSessionId;
    }
    void SetCuSessionId(const string &strID)
    {
        cuSessionId = strID;
    }
private:
    string cuSessionId;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CCuOfflineRsp: public CEventRsp
{
public:
    CCuOfflineRsp()
    {
        event = OspExtEventDesc(TAS_CU_OFFLINE_RSP); 
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDeviceAddReq: public CEventReq
{
public:
    CDeviceAddReq()
    {
        event = OspExtEventDesc(TAS_DEVICE_ADD_REQ);
    }

    CDeviceInfo& GetDeviceInfo() 
    {
        return info;
    }
    const CDeviceInfo& GetDeviceInfo() const
    {
        return info;
    }
    void SetDeviceInfo(const CDeviceInfo &val)
    {
        info = val;
    }
private:
    CDeviceInfo info;

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDeviceAddRsp: public CEventRsp
{
public:
    CDeviceAddRsp()
    {
        event = OspExtEventDesc(TAS_DEVICE_ADD_RSP);
    }

    UUIDString& GetDeviceUUID()
    {
        return deviceId;
    }
    const UUIDString& GetDeviceUUID() const
    {
        return deviceId;
    }
    void SetDeviceUUID(const UUIDString &val)
    {
        deviceId = val;
    }

private:
    UUIDString deviceId; // 设备UUIDString

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDeviceAddNtfReq: public CEventReq
{
public:
    CDeviceAddNtfReq()
    {
        event = OspExtEventDesc(TAS_DEVICE_ADD_NTF_REQ);
    }

    CDeviceInfo& GetDeviceInfo() 
    {
        return info;
    }
    const CDeviceInfo& GetDeviceInfo() const
    {
        return info;
    }
    void SetDeviceInfo(const CDeviceInfo &val)
    {
        info = val;
    }

private:
    CDeviceInfo info;

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDeviceDelReq: public CEventReq
{
public:
    CDeviceDelReq()
    {
        event = OspExtEventDesc(TAS_DEVICE_DEL_REQ);
    }

    UUIDString& GetDeviceUUID() 
    {
        return deviceId;
    }
    const UUIDString& GetDeviceUUID() const
    {
        return deviceId;
    }
    void SetDeviceUUID(const UUIDString &val)
    {
        deviceId = val;
    }

private:
    UUIDString deviceId;

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDeviceDelRsp: public CEventRsp
{
public:
    CDeviceDelRsp()
    {
        event = OspExtEventDesc(TAS_DEVICE_DEL_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDeviceDelNtfReq: public CEventReq
{
public:
    CDeviceDelNtfReq()
    {
        event = OspExtEventDesc(TAS_DEVICE_DEL_NTF_REQ);
    }

    UUIDString& GetDeviceUUID() 
    {
        return deviceId;
    }
    const UUIDString& GetDeviceUUID() const
    {
        return deviceId;
    }
    void SetDeviceUUID(const UUIDString &val)
    {
        deviceId = val;
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

private:
    UUIDString deviceId;
    UUIDString domainId;
	TCreater   creater;               

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetCreater(TCreater nCreater)
    {
        creater = nCreater;
    }
    TCreater GetCreater() const
    {
        return creater;
    }

};

class CDeviceModReq: public CEventReq
{
public:
    CDeviceModReq()
    {
        event = OspExtEventDesc(TAS_DEVICE_MOD_REQ);
        usedFlag = true;
        netType = DEVICE_LINEMODE_CMU_OUTNAT_PU_OUTNAT;
    }
    const UUIDString& GetDeviceUUID() const
    {
        return deviceId;
    }
    bool GetDeviceUUID(UUIDString &val) const
    {
        if (!optionField.HasOption("deviceId"))
            return false;
        val = deviceId;
        return true;
    }
    void SetDeviceUUID(const UUIDString &val)
    {
        deviceId = val;
        optionField.SetOption("deviceId");
    }
    bool GetDeviceModelId(UUIDString &val) const
    {
        if (!optionField.HasOption("deviceModelId"))
            return false;
        val = deviceModelId;
        return true;
    }
    void SetDeviceModelId(const UUIDString &val)
    {
        deviceModelId = val;
        optionField.SetOption("deviceModelId");
    }
    bool GetDevicePropertyFlags(TDevicePropertyFlags &val) const
    {
        if (!optionField.HasOption("propertyFlags"))
            return false;
        val = propertyFlags;
        return true;
    }
    void SetDevicePropertyFlags(const TDevicePropertyFlags &val)
    {
        propertyFlags = val;
        optionField.SetOption("propertyFlags");
    }
    bool GetDeviceName(string &val) const
    {
        if (!optionField.HasOption("deviceName"))
            return false;
        val = deviceName;
        return true;
    }
    void SetDeviceName(const string &val)
    {
        deviceName = val;
        optionField.SetOption("deviceName");
    }
    bool GetDeviceAlias(string &val) const
    {
        if (!optionField.HasOption("deviceAlias"))
            return false;
        val = deviceAlias;
        return true;
    }
    void SetDeviceAlias(const string &val)
    {
        deviceAlias = val;
        optionField.SetOption("deviceAlias");
    }
    bool GetUsedFlag(bool &val) const
    {
        if (!optionField.HasOption("usedFlag"))
            return false;
        val = usedFlag;
        return true;
    }
    void SetUsedFlag(bool val)
    {
        usedFlag = val;
        optionField.SetOption("usedFlag");
    }
    bool GetNetType(int &val) const
    {
        if (!optionField.HasOption("netType"))
            return false;
        val = netType;
        return true;
    }
    void SetNetType(int val)
    {
        netType = val;
        optionField.SetOption("netType");
    }
    bool GetNetAddress(string &val) const
    {
        if (!optionField.HasOption("netAddress"))
            return false;
        val = netAddress;
        return true;
    }
    void SetNetAddress(const string &val)
    {
        netAddress = val;
        optionField.SetOption("netAddress");
    }
    bool GetDescription(string &val) const
    {
        if (!optionField.HasOption("description"))
            return false;
        val = description;
        return true;
    }
    void SetDescription(const string &val)
    {
        description = val;
        optionField.SetOption("description");
    }
    bool GetExpireDate(string &val) const
    {
        if (!optionField.HasOption("expireDate"))
            return false;
        val = expireDate;
        return true;
    }
    void SetExpireDate(const string &val)
    {
        expireDate = val;
        optionField.SetOption("expireDate");
    }
	bool GetDevData(TDevExtData &val) const
	{
		if (!optionField.HasOption("devData"))
			return false;
		val = devData;
		return true;
	}
	void SetDevData(const TDevExtData &val)
	{
		devData = val;
		optionField.SetOption("devData");
	}
private:
    CModOptionField optionField;
    UUIDString deviceId; // 设备UUIDString
    UUIDString deviceModelId; // 设备型号Id
    TDevicePropertyFlags propertyFlags; // 设备属性标志
    string deviceName;     // 设备名
    string deviceAlias;    // 设备别名
    bool usedFlag; // 启停状态标志：0停用，非0启用
    int netType;   // 接入线路方式：参见常量定义
    string netAddress;   // 设备网络地址，IP或域名
    string description;  // 设备描述
    string expireDate;   // 使用截止日期
	TDevExtData devData;	// 设备扩展信息，该变量中只需要设置需要修改的字段，其他的仍做保留
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDeviceModRsp: public CEventRsp
{
public:
    CDeviceModRsp()
    {
        event = OspExtEventDesc(TAS_DEVICE_MOD_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDeviceModNtfReq: public CEventReq
{
public:
    CDeviceModNtfReq()
    {
        event = OspExtEventDesc(TAS_DEVICE_MOD_NTF_REQ);
    }

    CDeviceInfo& GetDeviceInfo() 
    {
        return info;
    }
    const CDeviceInfo& GetDeviceInfo() const
    {
        return info;
    }
    void SetDeviceInfo(const CDeviceInfo &val)
    {
        info = val;
    }

private:
    CDeviceInfo info;

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

struct TDeviceGeneraData
{
	TDeviceGeneraData()
	{

	}
	string           type;               
	string           content;               
public:
    void SetType(const string& strType)
    {
        type = strType;
    }
    string& GetType()
    {
        return type;
    }
    const string& GetType() const
    {
        return type;
    }

    void SetContent(const string& strContent)
    {
        content = strContent;
    }
    string& GetContent()
    {
        return content;
    }
    const string& GetContent() const
    {
        return content;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDeviceQryReq: public CQueryReqBase
{
public:
    CDeviceQryReq()
    {
        event = OspExtEventDesc(TAS_DEVICE_QRY_REQ);
    }

    CDeviceInfoQueryCondition& GetDeviceInfoQueryCondition() 
    {
        return condition;
    }
    const CDeviceInfoQueryCondition& GetDeviceInfoQueryCondition() const
    {
        return condition;
    }
    void SetDeviceInfoQueryCondition(const CDeviceInfoQueryCondition &val)
    {
        condition = val;
    }
private:
    CDeviceInfoQueryCondition condition;

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDeviceQryRsp: public CQueryRspBase
{
public:
    CDeviceQryRsp()
    {
        event = OspExtEventDesc(TAS_DEVICE_QRY_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDeviceQryNtfReq: public CQueryNtfReqBase
{
public:
    CDeviceQryNtfReq()
    {
        event = OspExtEventDesc(TAS_DEVICE_QRY_NTF_REQ);
    }

    vector<CDeviceInfo>& GetDeviceInfo()
    {
        return infos;
    }
    const vector<CDeviceInfo>& GetDeviceInfo() const
    {
        return infos;
    }
    void SetDeviceInfo(const vector<CDeviceInfo> &val)
    {
        infos = val;
    }

private:
    vector<CDeviceInfo> infos;

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDeviceModelAddReq: public CEventReq
{
public:
    CDeviceModelAddReq()
    {
        event = OspExtEventDesc(TAS_DEVICE_MODEL_ADD_REQ);
    }

    CDeviceModelInfo& GetDeviceModelInfo() 
    {
        return info;
    }
    const CDeviceModelInfo& GetDeviceModelInfo() const
    {
        return info;
    }
    void SetDeviceModelInfo(const CDeviceModelInfo &val)
    {
        info = val;
    }
private:
    CDeviceModelInfo info;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
class CDeviceModelAddRsp: public CEventRsp
{
public:
    CDeviceModelAddRsp()
    {
        event = OspExtEventDesc(TAS_DEVICE_MODEL_ADD_RSP);
    }

    UUIDString& GetDeviceModelId()
    {
        return deviceModelId;
    }
    const UUIDString& GetDeviceModelId() const
    {
        return deviceModelId;
    }
    void SetDeviceModelId(const UUIDString &val)
    {
        deviceModelId = val;
    }
private:
    UUIDString deviceModelId;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
class CDeviceModelAddNtfReq: public CEventReq
{
public:
    CDeviceModelAddNtfReq()
    {
        event = OspExtEventDesc(TAS_DEVICE_MODEL_ADD_NTF_REQ);
    }

    CDeviceModelInfo& GetDeviceModelInfo()
    {
        return info;
    }
    const CDeviceModelInfo& GetDeviceModelInfo() const
    {
        return info;
    }
    void SetDeviceModelInfo(const CDeviceModelInfo &val)
    {
        info = val;
    }
private:
    CDeviceModelInfo info;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
class CDeviceModelDelReq: public CEventReq
{
public:
    CDeviceModelDelReq()
    {
        event = OspExtEventDesc(TAS_DEVICE_MODEL_DEL_REQ);
    }

    UUIDString& GetDeviceModelId() 
    {
        return deviceModelId;
    }
    const UUIDString& GetDeviceModelId() const
    {
        return deviceModelId;
    }
    void SetDeviceModelId(const UUIDString &val)
    {
        deviceModelId = val;
    }
private:
    UUIDString deviceModelId;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
class CDeviceModelDelRsp: public CEventRsp
{
public:
    CDeviceModelDelRsp()
    {
        event = OspExtEventDesc(TAS_DEVICE_MODEL_DEL_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
class CDeviceModelDelNtfReq: public CEventReq
{
public:
    CDeviceModelDelNtfReq()
    {
        event = OspExtEventDesc(TAS_DEVICE_MODEL_DEL_NTF_REQ);
    }

    UUIDString& GetDeviceModelId()
    {
        return deviceModelId;
    }
    const UUIDString& GetDeviceModelId() const
    {
        return deviceModelId;
    }
    void SetDeviceModelId(const UUIDString &val)
    {
        deviceModelId = val;
    }
private:
    UUIDString deviceModelId;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
class CDeviceModelQryReq: public CQueryReqBase
{
public:
    CDeviceModelQryReq()
    {
        event = OspExtEventDesc(TAS_DEVICE_MODEL_QRY_REQ);
    }

    CDeviceModelQueryCondition& GetDeviceModelQryCondition() 
    {
        return condition;
    }
    const CDeviceModelQueryCondition& GetDeviceModelQryCondition() const
    {
        return condition;
    }
    void SetDeviceModelQryCondition(const CDeviceModelQueryCondition &val)
    {
        condition = val;
    }
private:
    CDeviceModelQueryCondition condition;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
class CDeviceModelQryRsp: public CQueryRspBase
{
public:
    CDeviceModelQryRsp()
    {
        event = OspExtEventDesc(TAS_DEVICE_MODEL_QRY_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
class CDeviceModelQryNtfReq: public CQueryNtfReqBase
{
public:
    CDeviceModelQryNtfReq()
    {
        event = OspExtEventDesc(TAS_DEVICE_MODEL_QRY_NTF_REQ);
    }

    vector<CDeviceModelInfo>& GetDeviceModelInfo() 
    {
        return infos;
    }
    const vector<CDeviceModelInfo>& GetDeviceModelInfo() const
    {
        return infos;
    }
    void SetDeviceModelInfo(const vector<CDeviceModelInfo> &val)
    {
        infos = val;
    }
private:
    vector<CDeviceModelInfo> infos;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDeviceCapIndexNameGetReq: public CEventReq
{
public:
    CDeviceCapIndexNameGetReq()
    {
        event = OspExtEventDesc(TAS_DEVICE_CAP_INDEX_NAME_GET_REQ);
        capId = DEVICE_CAP_INVALID;
        capIndex = -1;
    }

    UUIDString& GetDeviceId() 
    {
        return deviceId;
    }
    const UUIDString& GetDeviceId() const
    {
        return deviceId;
    }
    void SetDeviceId(const UUIDString &val)
    {
        deviceId = val;
    }
    int GetCapId() const
    {
        return capId;
    }
    void SetCapId(int val)
    {
        capId = val;
    }
    int GetCapIndex() const 
    {
        return capIndex;
    }
    void SetCapIndex(int val)
    {
        capIndex = val;
    }
private:
    UUIDString deviceId;
    int capId;
    int capIndex; // -1表示某设备某种能力的所有索引名称
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDeviceCapIndexNameGetRsp: public CEventRsp
{
public:
    CDeviceCapIndexNameGetRsp()
    {
        event = OspExtEventDesc(TAS_DEVICE_CAP_INDEX_NAME_GET_RSP);
    }

    TDeviceCapName& GetDeviceCapName() 
    {
        return deviceCapName;
    }
    const TDeviceCapName& GetDeviceCapName() const
    {
        return deviceCapName;
    }
    void SetDeviceCapName(const TDeviceCapName &val)
    {
        deviceCapName = val;
    }
private:
    TDeviceCapName deviceCapName;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDeviceCapIndexNameSetReq: public CEventReq
{
public:
    CDeviceCapIndexNameSetReq()
    {
        event = OspExtEventDesc(TAS_DEVICE_CAP_INDEX_NAME_SET_REQ);
    }

    TDeviceCapName& GetDeviceCapName() 
    {
        return deviceCapName;
    }
    const TDeviceCapName& GetDeviceCapName() const
    {
        return deviceCapName;
    }
    void SetDeviceCapName(const TDeviceCapName &val)
    {
        deviceCapName = val;
    }
private:
    TDeviceCapName deviceCapName;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDeviceCapIndexNameSetRsp: public CEventRsp
{
public:
    CDeviceCapIndexNameSetRsp()
    {
        event = OspExtEventDesc(TAS_DEVICE_CAP_INDEX_NAME_SET_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDeviceCapIndexNameModNtfReq: public CEventReq
{
public:
    CDeviceCapIndexNameModNtfReq()
    {
        event = OspExtEventDesc(TAS_DEVICE_CAP_INDEX_NAME_MOD_NTF_REQ);
    }

    TDeviceCapName& GetDeviceCapName() 
    {
        return deviceCapName;
    }
    const TDeviceCapName& GetDeviceCapName() const
    {
        return deviceCapName;
    }
    void SetDeviceCapName(const TDeviceCapName &val)
    {
        deviceCapName = val;
    }
private:
    TDeviceCapName deviceCapName;
    UUIDString domainId;

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
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

};

class CDeviceCapIndexNameQryReq: public CQueryReqBase
{
public:
    CDeviceCapIndexNameQryReq()
    {
        event = OspExtEventDesc(TAS_DEVICE_CAP_INDEX_NAME_QRY_REQ); 
    }

    CDeviceCapIndexNameQryCondition& GetDeviceCapIndexNameQryCondition() 
    {
        return condition;
    }
    const CDeviceCapIndexNameQryCondition& GetDeviceCapIndexNameQryCondition() const
    {
        return condition;
    }
    void SetDeviceCapIndexNameQryCondition(const CDeviceCapIndexNameQryCondition &val)
    {
        condition = val;
    }
private:
    CDeviceCapIndexNameQryCondition condition;

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDeviceCapIndexNameQryRsp: public CQueryRspBase
{
public:
    CDeviceCapIndexNameQryRsp()
    {
        event = OspExtEventDesc(TAS_DEVICE_CAP_INDEX_NAME_QRY_RSP); 
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDeviceCapIndexNameQryNtfReq: public CQueryNtfReqBase
{
public:
    CDeviceCapIndexNameQryNtfReq()
    {
        event = OspExtEventDesc(TAS_DEVICE_CAP_INDEX_NAME_QRY_NTF_REQ); 
    }

    void SetInfos(const vector<TDeviceCapName>& cInfos)
    {
        infos = cInfos;
    }
    vector<TDeviceCapName>& GetInfos()
    {
        return infos;
    }
    const vector<TDeviceCapName>& GetInfos() const
    {
        return infos;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

private:
    vector<TDeviceCapName> infos;
};

class CDeviceManufacturerAddReq: public CEventReq
{
public:
    CDeviceManufacturerAddReq()
    {
        event = OspExtEventDesc(TAS_DEVICE_MANUFACTURER_ADD_REQ);
    }
    string& GetManufacturer()
    {
        return manufacturer;
    }
    const string& GetManufacturer() const
    {
        return manufacturer;
    }
	void SetManufacturer(const string& val)
	{
		manufacturer = val;
    }
private:
    string manufacturer; // 厂商名称
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDeviceManufacturerAddRsp: public CEventRsp
{
public:
    CDeviceManufacturerAddRsp()
    {
        event = OspExtEventDesc(TAS_DEVICE_MANUFACTURER_ADD_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDeviceManufacturerDelReq: public CEventReq
{
public:
    CDeviceManufacturerDelReq()
    {
        event = OspExtEventDesc(TAS_DEVICE_MANUFACTURER_DEL_REQ);
    }
    string& GetManufacturer()
    {
        return manufacturer;
    }
    const string& GetManufacturer() const
    {
        return manufacturer;
    }
	void SetManufacturer(const string& val)
	{
		manufacturer = val;
    }
private:
    string manufacturer; // 厂商名称
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDeviceManufacturerDelRsp: public CEventRsp
{
public:
    CDeviceManufacturerDelRsp()
    {
        event = OspExtEventDesc(TAS_DEVICE_MANUFACTURER_DEL_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDeviceManufacturerQryReq: public CQueryReqBase
{
public:
    CDeviceManufacturerQryReq()
    {
        event = OspExtEventDesc(TAS_DEVICE_MANUFACTURER_QRY_REQ);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDeviceManufacturerQryRsp: public CQueryRspBase
{
public:
    CDeviceManufacturerQryRsp()
    {
        event = OspExtEventDesc(TAS_DEVICE_MANUFACTURER_QRY_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDeviceManufacturerQryNtfReq: public CQueryNtfReqBase
{
public:
    CDeviceManufacturerQryNtfReq()
    {
        event = OspExtEventDesc(TAS_DEVICE_MANUFACTURER_QRY_NTF_REQ);
    }
    vector<CDeviceManufacturerInfo>& GetDeviceManufacturerInfo()
    {
        return infos;
    }
    const vector<CDeviceManufacturerInfo>& GetDeviceManufacturerInfo() const
    {
        return infos;
    }
	void SetDeviceManufacturerInfo(const vector<CDeviceManufacturerInfo>& val)
	{
		infos = val;
	}
private:
    vector<CDeviceManufacturerInfo> infos;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUserAddReq: public CEventReq
{
public:
    CUserAddReq()
    {
        event = OspExtEventDesc(TAS_CUSER_ADD_REQ);
    }

    CUserInfo& GetUserInfo() 
    {
        return info;
    }
    const CUserInfo& GetUserInfo() const
    {
        return info;
    }
    void SetUserInfo(const CUserInfo &val)
    {
        info = val;
    }
private:
    CUserInfo info;

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUserAddRsp: public CEventRsp
{
public:
    CUserAddRsp()
    {
        event = OspExtEventDesc(TAS_CUSER_ADD_RSP);
    }

    UUIDString& GetUserID()
    {
        return userID;
    }
    const UUIDString& GetUserID() const
    {
        return userID;
    }
    void SetUserID(const UUIDString &val)
    {
        userID = val;
    }

private:
    UUIDString userID; // 用户编号

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUserAddNtfReq: public CEventReq
{
public:
    CUserAddNtfReq()
    {
        event = OspExtEventDesc(TAS_CUSER_ADD_NTF_REQ);
    }
    void GetUserInfo(CUserInfo &val) const
    {
        val = info;
    }
    CUserInfo& GetUserInfo()
    {
        return info;
    }
    const CUserInfo& GetUserInfo() const
    {
        return info;
    }
    void SetUserInfo(const CUserInfo &val)
    {
        info = val;
    }
private:
    CUserInfo info;

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUserDelReq: public CEventReq
{
public:
    CUserDelReq()
    {
        event = OspExtEventDesc(TAS_CUSER_DEL_REQ);
    }

    UUIDString& GetUserID()
    {
        return userID;
    }
    const UUIDString& GetUserID() const
    {
        return userID;
    }
    void SetUserID(const UUIDString &val)
    {
        userID = val;
    }
private:
    UUIDString userID;

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUserDelRsp: public CEventRsp
{
public:
    CUserDelRsp()
    {
        event = OspExtEventDesc(TAS_CUSER_DEL_RSP);
    }

    void SetOperUuid(const UUIDString &val)
    {
        operUuid = val;
    }
    const UUIDString &GetOperUuid() const
    {
        return operUuid;
    }
    UUIDString &GetOperUuid()
    {
        return operUuid;
    }

private:
    UUIDString operUuid;	// 操作Id，标志唯一的一次操作，用于后续查询进度

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUserDelNtfReq: public CEventReq
{
public:
    CUserDelNtfReq()
    {
        event = OspExtEventDesc(TAS_CUSER_DEL_NTF_REQ);
    }

    UUIDString& GetUserID() 
    {
        return userID;
    }
    const UUIDString& GetUserID() const
    {
        return userID;
    }
    void SetUserID(const UUIDString &val)
    {
        userID = val;
    }
private:
    UUIDString userID;

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUserModReq: public CEventReq
{
public:
    CUserModReq()
    {
        event = OspExtEventDesc(TAS_CUSER_MOD_REQ);
        type = CUSER_TYPE_SUPERADMIN;
        mLoginFlag = true;
        enabledFlag = true;
		pwdExpireEnabledFlag = false;
		ipListEnabledFlag = false;
		ipBindFlagForMobileClient = false;
		switchNum = -1;
		waterMarkEnabled = false;
		waterMarkFontSize = 1;
		waterMarkRotationAngle = 30;
		waterMarkTransparent = 30;
		waterMarkTiled = true;
		waterMarkColor = 16711680;
		waterMarkFont = "";

    }

    bool GetUserID(UUIDString &val) const
    {
        if (!optionField.HasOption("id"))
            return false;
        val = id;
        return true;
    }
    void SetUserID(const UUIDString &val)
    {
        id = val;
        optionField.SetOption("id");
    }
    bool GetUsergrpID(UUIDString &val) const
    {
        if (!optionField.HasOption("groupId"))
            return false;
        val = groupId;
        return true;
    }
    void SetUsergrpID(const UUIDString &val)
    {
        groupId = val;
        optionField.SetOption("groupId");
    }
    bool GetUserName(string &val) const
    {
        if (!optionField.HasOption("name"))
            return false;
        val = name;
        return true;
    }
    void SetUserName(const string &val)
    {
        name = val;
        optionField.SetOption("name");
    }
    bool GetUserPwd(string &val) const
    {
        if (!optionField.HasOption("password"))
            return false;
        val = password;
        return true;
    }
    void SetUserPwd(const string &val)
    {
        password = val;
        optionField.SetOption("password");
    }
    bool GetUserOldPwd(string &val) const
    {
        if (!optionField.HasOption("oldpassword"))
            return false;
        val = oldpassword;
        return true;
    }
    void SetUserOldPwd(const string &val)
    {
        oldpassword = val;
        optionField.SetOption("oldpassword");
    }
    bool GetUserType(int &val) const
    {
        if (!optionField.HasOption("type"))
            return false;
        val = type;
        return true;
    }
    void SetUserType(int val)
    {
        type = val;
        optionField.SetOption("type");
    }
    bool GetMLoginFlag(bool &val) const
    {
        if (!optionField.HasOption("mLoginFlag"))
            return false;
        val = mLoginFlag;
        return true;
    }
    void SetMLoginFlag(bool val)
    {
        mLoginFlag = val;
        optionField.SetOption("mLoginFlag");
    }
    bool GetEnabledFlag(bool &val) const
    {
        if (!optionField.HasOption("enabledFlag"))
            return false;
        val = enabledFlag;
        return true;
    }
    void SetEnabledFlag(bool val)
    {
        enabledFlag = val;
        optionField.SetOption("enabledFlag");
    }
    bool GetDescription(string &val) const
    {
        if (!optionField.HasOption("description"))
            return false;
        val = description;
        return true;
    }
    void SetDescription(const string &val)
    {
        description = val;
        optionField.SetOption("description");
    }
    bool GetNameExpireDate(string &val) const
    {
        if (!optionField.HasOption("nameExpireDate"))
            return false;
        val = nameExpireDate;
        return true;
    }
    void SetNameExpireDate(const string &val)
    {
        nameExpireDate = val;
        optionField.SetOption("nameExpireDate");
    }
    bool GetPwdExpireDate(string &val) const
    {
        if (!optionField.HasOption("pwdExpireDate"))
            return false;
        val = pwdExpireDate;
        return true;
    }
    void SetPwdExpireDate(const string &val)
    {
        pwdExpireDate = val;
        optionField.SetOption("pwdExpireDate");
    }
    bool GetUserPrivilege(CUserPrivilege &val) const
    {
        if (!optionField.HasOption("userPrivilege"))
            return false;
        val = userPrivilege;
        return true;
    }
    void SetUserPrivilege(const CUserPrivilege &val)
    {
        userPrivilege = val;
        optionField.SetOption("userPrivilege");
    }

	void SetLoginIpList(const string& strLoginIpList)
	{
		loginIpList = strLoginIpList;
		optionField.SetOption("loginIpList");
	}

	bool GetLoginIpList(string &val) const
	{
		if (!optionField.HasOption("loginIpList"))
			return false;
		val = loginIpList;
		return true;
	}

	void SetPwdExpireEnabledFlag(bool bPwdExpireEnabledFlag)
	{
		pwdExpireEnabledFlag = bPwdExpireEnabledFlag;
		optionField.SetOption("pwdExpireEnabledFlag");
	}
	bool GetPwdExpireEnabledFlag(bool &val) const
	{
		if (!optionField.HasOption("pwdExpireEnabledFlag"))
			return false;
		val = pwdExpireEnabledFlag;
		return true;
	}
	void SetPwdExpireTime(int nPwdExpireTime)
	{
		pwdExpireTime = nPwdExpireTime;
		optionField.SetOption("pwdExpireTime");
	}
	bool GetPwdExpireTime(int &val) const
	{
		if (!optionField.HasOption("pwdExpireTime"))
			return false;
		val = pwdExpireTime;
		return true;
	}
	void SetIpListEnabledFlag(bool bIpListEnabledFlag)
	{
		ipListEnabledFlag = bIpListEnabledFlag;
		optionField.SetOption("ipListEnabledFlag");
	}
	bool GetIpListEnabledFlag(bool &val) const
	{
		if (!optionField.HasOption("ipListEnabledFlag"))
			return false;
		val = ipListEnabledFlag;
		return true;
	}

	void SetIpBindFlagForMobileClient(bool bIpBindFlagForMobileClient)
	{
		ipBindFlagForMobileClient = bIpBindFlagForMobileClient;
		optionField.SetOption("ipBindFlagForMobileClient");
	}
	bool GetIpBindFlagForMobileClient(bool &val) const
	{
		if (!optionField.HasOption("ipBindFlagForMobileClient"))
			return false;
		val = ipBindFlagForMobileClient;
		return true;
	}
	void SetSwitchNum(int nSwitchNum)
	{
		switchNum = nSwitchNum;
		optionField.SetOption("switchNum");
	}
	bool GetSwitchNum(int &val) const
	{
		if (!optionField.HasOption("switchNum"))
			return false;
		val = switchNum;
		return true;
	}

	void SetWaterMarkEnabled(bool bWaterMarkEnabled)
	{
		waterMarkEnabled = bWaterMarkEnabled;
		optionField.SetOption("waterMarkEnabled");
	}
	bool GetWaterMarkEnabled(bool &val) const
	{
		if (!optionField.HasOption("waterMarkEnabled"))
			return false;
		val = waterMarkEnabled;
		return true;
	}

	void SetWaterMarkFont(const string& strWaterMarkFont)
	{
		waterMarkFont = strWaterMarkFont;
		optionField.SetOption("waterMarkFont");
	}

	bool GetWaterMarkFont(string &val) const
	{
		if (!optionField.HasOption("waterMarkFont"))
			return false;
		val = waterMarkFont;
		return true;
	}

	void SetWaterMarkFontSize(int nWaterMarkFontSize)
	{
		waterMarkFontSize = nWaterMarkFontSize;
		optionField.SetOption("waterMarkFontSize");
	}
	bool GetWaterMarkFontSize(int &val) const
	{
		if (!optionField.HasOption("waterMarkFontSize"))
			return false;
		val = waterMarkFontSize;
		return true;
	}

	void SetWaterMarkRotationAngle(int nWaterMarkRotationAngle)
	{
		waterMarkRotationAngle = nWaterMarkRotationAngle;
		optionField.SetOption("waterMarkRotationAngle");
	}
	bool GetWaterMarkRotationAngle(int &val) const
	{
		if (!optionField.HasOption("waterMarkRotationAngle"))
			return false;
		val = waterMarkRotationAngle;
		return true;
	}

	void SetWaterMarkColor(int nWaterMarkColor)
	{
		waterMarkColor = nWaterMarkColor;
		optionField.SetOption("waterMarkColor");
	}
	bool GetWaterMarkColor(int &val) const
	{
		if (!optionField.HasOption("waterMarkColor"))
			return false;
		val = waterMarkColor;
		return true;
	}

	void SetWaterMarkTransparent(int nWaterMarkTransparent)
	{
		waterMarkTransparent = nWaterMarkTransparent;
		optionField.SetOption("waterMarkTransparent");
	}
	bool GetWaterMarkTransparent(int &val) const
	{
		if (!optionField.HasOption("waterMarkTransparent"))
			return false;
		val = waterMarkTransparent;
		return true;
	}

	void SetWaterMarkTiled(bool bWaterMarkTiled)
	{
		waterMarkTiled = bWaterMarkTiled;
		optionField.SetOption("waterMarkTiled");
	}
	bool GetWaterMarkTiled(bool &val) const
	{
		if (!optionField.HasOption("waterMarkTiled"))
			return false;
		val = waterMarkTiled;
		return true;
	}

private:
    CModOptionField optionField;
    UUIDString id;    // 用户编号
    UUIDString groupId; // 用户所属组
    string name;     // 用户名
    string password; // 登陆密码, 密文形式
    string oldpassword; // 老密码，密文形式 提供老密码时同时校验老密码
    int type;    // CU用户类型：参见常量定义
    bool mLoginFlag; // 多次登陆标志, 1表示允许多人登录
    bool enabledFlag; // 启停状态标志
    string description; // 描述
    string nameExpireDate; // 帐户使用截止日期
    string pwdExpireDate;  // 密码使用截止日期
	int pwdExpireTime;       //密码有效期时长，单位：天
	bool pwdExpireEnabledFlag;   //密码有效期是否启用
	string loginIpList;       //绑定的IP地址列表
	bool  ipListEnabledFlag; //绑定IP是否启用              
    CUserPrivilege userPrivilege;
	bool ipBindFlagForMobileClient; //移动客户端ip绑定是否启用               
	int switchNum;                //码流并发
		//视频水印功能
	bool waterMarkEnabled;                    //是否启用水印
	string waterMarkFont;                     //水印字体
	int waterMarkFontSize;                    //字体大小
	int waterMarkRotationAngle;               //旋转角度
	int waterMarkColor;                       //颜色
	int waterMarkTransparent;                 //透明度
	bool waterMarkTiled;                      //是否平铺

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUserModRsp: public CEventRsp
{
public:
    CUserModRsp()
    {
        event = OspExtEventDesc(TAS_CUSER_MOD_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUserModNtfReq: public CEventReq
{
public:
    CUserModNtfReq()
    {
        event = OspExtEventDesc(TAS_CUSER_MOD_NTF_REQ);
    }

    CUserInfo& GetUserInfo() 
    {
        return info;
    }
    const CUserInfo& GetUserInfo() const
    {
        return info;
    }
    void SetUserInfo(const CUserInfo &val)
    {
        info = val;
    }

private:
    CUserInfo info;

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUserQryReq: public CQueryReqBase
{
public:
    CUserQryReq()
    {
        event = OspExtEventDesc(TAS_CUSER_QRY_REQ);
    }

    CUserInfoQueryCondition& GetUserInfoQueryCondition() 
    {
        return condition;
    }
    const CUserInfoQueryCondition& GetUserInfoQueryCondition() const
    {
        return condition;
    }
    void SetUserInfoQueryCondition(const CUserInfoQueryCondition &val)
    {
        condition = val;
    }

private:
    CUserInfoQueryCondition condition;

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUserQryRsp: public CQueryRspBase
{
public:
    CUserQryRsp()
    {
        event = OspExtEventDesc(TAS_CUSER_QRY_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUserQryNtfReq: public CQueryNtfReqBase
{
public:
    CUserQryNtfReq()
    {
        event = OspExtEventDesc(TAS_CUSER_QRY_NTF_REQ);
    }

    vector<CUserInfo>& GetUserInfo() 
    {
        return infos;
    }
    const vector<CUserInfo>& GetUserInfo() const
    {
        return infos;
    }
    void SetUserInfo(const vector<CUserInfo> &val)
    {
        infos = val;
    }

private:
    vector<CUserInfo> infos;

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CUserDeviceAddReq: public CEventReq
{
public:
    CUserDeviceAddReq()
    {
        event = OspExtEventDesc(TAS_CUSER_DEVICE_ADD_REQ);
    }

    CUserDeviceInfo& GetUserDeviceInfo() 
    {
        return info;
    }
    const CUserDeviceInfo& GetUserDeviceInfo() const
    {
        return info;
    }
    void SetUserDeviceInfo(const CUserDeviceInfo &val)
    {
        info = val;
    }

private:
    CUserDeviceInfo info;

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUserDeviceAddRsp: public CEventRsp
{
public:
    CUserDeviceAddRsp()
    {
        event = OspExtEventDesc(TAS_CUSER_DEVICE_ADD_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUserDeviceAddNtfReq: public CEventReq
{
public:
    CUserDeviceAddNtfReq()
    {
        event = OspExtEventDesc(TAS_CUSER_DEVICE_ADD_NTF_REQ);
    }

    CUserDeviceInfo& GetUserDeviceInfo()
    {
        return info;
    }
    const CUserDeviceInfo& GetUserDeviceInfo() const
    {
        return info;
    }
    void SetUserDeviceInfo(const CUserDeviceInfo &val)
    {
        info = val;
    }

private:
    CUserDeviceInfo info;

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUserDeviceDelReq: public CEventReq
{
public:
    CUserDeviceDelReq()
    {
        event = OspExtEventDesc(TAS_CUSER_DEVICE_DEL_REQ);
    }

    CUserDeviceInfoKey& GetUserDeviceInfoKey()
    {
        return key;
    }
    const CUserDeviceInfoKey& GetUserDeviceInfoKey() const
    {
        return key;
    }
    void SetUserDeviceInfoKey(const CUserDeviceInfoKey &val)
    {
        key = val;
    }

private:
    // 具体能力信息为空时表示删除整个设备，另外若TAS发现所有的能力信息都已经被删除时也会自动删除整个设备
    CUserDeviceInfoKey key;

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUserDeviceDelRsp: public CEventRsp
{
public:
    CUserDeviceDelRsp()
    {
        event = OspExtEventDesc(TAS_CUSER_DEVICE_DEL_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUserDeviceDelNtfReq: public CEventReq
{
public:
    CUserDeviceDelNtfReq()
    {
        event = OspExtEventDesc(TAS_CUSER_DEVICE_DEL_NTF_REQ);
    }

    CUserDeviceInfoKey& GetUserDeviceInfoKey() 
    {
        return key;
    }
    const CUserDeviceInfoKey& GetUserDeviceInfoKey() const
    {
        return key;
    }
    void SetUserDeviceInfoKey(const CUserDeviceInfoKey &val)
    {
        key = val;
    }

private:
    // 具体能力信息为空时表示删除整个设备
    CUserDeviceInfoKey key;
    UUIDString devDomainId;

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetDevDomainId(const UUIDString& strDevDomainId)
    {
        devDomainId = strDevDomainId;
    }
    UUIDString& GetDevDomainId()
    {
        return devDomainId;
    }
    const UUIDString& GetDevDomainId() const
    {
        return devDomainId;
    }

};

class CUserDeviceModReq: public CEventReq
{
public:
    CUserDeviceModReq()
    {
        event = OspExtEventDesc(TAS_CUSER_DEVICE_MOD_REQ);
    }
    const UUIDString& GetUserID() const
    {
        return userID;
    }
    const UUIDString& GetDeviceID() const
    {
        return deviceID;
    }
    bool GetUserID(UUIDString &val) const
    {
        if (!optionField.HasOption("userID"))
            return false;
        val = userID;
        return true;
    }
    void SetUserID(const UUIDString &val)
    {
        userID = val;
        optionField.SetOption("userID");
    }
    bool GetDeviceID(UUIDString &val) const
    {
        if (!optionField.HasOption("deviceID"))
            return false;
        val = deviceID;
        return true;
    }
    void SetDeviceID(const UUIDString &val)
    {
        deviceID = val;
        optionField.SetOption("deviceID");
    }
    bool GetDevicePrivilege(CDevicePrivilege &val) const
    {
        if (!optionField.HasOption("devPrivilege"))
            return false;
        val = devPrivilege;
        return true;
    }
    void SetDevicePrivilege(const CDevicePrivilege &val)
    {
        devPrivilege = val;
        optionField.SetOption("devPrivilege");
    }
    bool GetDeviceCapPrivilege(vector<TDeviceCapPrivilege> &val) const
    {
        if (!optionField.HasOption("capPrivileges"))
            return false;
        val = capPrivileges;
        return true;
    }
    void SetDeviceCapPrivilege(const vector<TDeviceCapPrivilege> &val)
    {
        capPrivileges = val;
        optionField.SetOption("capPrivileges");
    }
private:
    CModOptionField optionField;
    UUIDString userID;
    UUIDString deviceID;
    CDevicePrivilege devPrivilege;
    vector<TDeviceCapPrivilege> capPrivileges;  // 注意，此处不能对能力索引划归关系进行增删操作
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUserDeviceModRsp: public CEventRsp
{
public:
    CUserDeviceModRsp()
    {
        event = OspExtEventDesc(TAS_CUSER_DEVICE_MOD_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUserDeviceModNtfReq: public CEventReq
{
public:
    CUserDeviceModNtfReq()
    {
        event = OspExtEventDesc(TAS_CUSER_DEVICE_MOD_NTF_REQ);
    }

    CUserDeviceInfo& GetUserDeviceInfo() 
    {
        return info;
    }
    const CUserDeviceInfo& GetUserDeviceInfo() const
    {
        return info;
    }
    void SetUserDeviceInfo(const CUserDeviceInfo &val)
    {
        info = val;
    }

private:
    CUserDeviceInfo info;

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUserDeviceQryReq: public CQueryReqBase
{
public:
    CUserDeviceQryReq()
    {
        event = OspExtEventDesc(TAS_CUSER_DEVICE_QRY_REQ);
    }

    CUserDeviceInfoQueryCondition& GetUserDeviceInfoQueryCondition()
    {
        return condition;
    }
    const CUserDeviceInfoQueryCondition& GetUserDeviceInfoQueryCondition() const
    {
        return condition;
    }
    void SetUserDeviceInfoQueryCondition(const CUserDeviceInfoQueryCondition &val)
    {
        condition = val;
    }

private:
    CUserDeviceInfoQueryCondition condition;

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUserDeviceQryRsp: public CQueryRspBase
{
public:
    CUserDeviceQryRsp()
    {
        event = OspExtEventDesc(TAS_CUSER_DEVICE_QRY_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUserDeviceQryNtfReq: public CQueryNtfReqBase
{
public:
    CUserDeviceQryNtfReq()
    {
        event = OspExtEventDesc(TAS_CUSER_DEVICE_QRY_NTF_REQ);
    }
    
    vector<CUserDeviceInfo>& GetCuserDevInfo() 
    {
        return infos;
    }
    const vector<CUserDeviceInfo>& GetCuserDevInfo() const
    {
        return infos;
    }
    void SetCuserDevInfo(const vector<CUserDeviceInfo> &val)
    {
        infos = val;
    }

private:
    vector<CUserDeviceInfo> infos;

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CDeviceGroupAddReq: public CEventReq
{
public:
    CDeviceGroupAddReq()
    {
        event = OspExtEventDesc(TAS_DEVICE_GROUP_ADD_REQ);
    }

    CDeviceGroupInfo& GetDeviceGroupInfo() 
    {
        return info;
    }
    const CDeviceGroupInfo& GetDeviceGroupInfo() const
    {
        return info;
    }
    void SetDeviceGroupInfo(const CDeviceGroupInfo &val)
    {
        info = val;
    }

private:
    CDeviceGroupInfo info;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

};

class CDeviceGroupAddRsp: public CEventRsp
{
public:
    CDeviceGroupAddRsp()
    {
        event = OspExtEventDesc(TAS_DEVICE_GROUP_ADD_RSP);
    }

    CDeviceGroupInfoKey& GetDeviceGroupInfoKey()
    {
        return key;
    }
    const CDeviceGroupInfoKey& GetDeviceGroupInfoKey() const
    {
        return key;
    }
    void SetDeviceGroupInfoKey(const CDeviceGroupInfoKey &val)
    {
        key = val;
    }

private:
    CDeviceGroupInfoKey key;

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDeviceGroupAddNtfReq: public CEventReq
{
public:
    CDeviceGroupAddNtfReq()
    {
        event = OspExtEventDesc(TAS_DEVICE_GROUP_ADD_NTF_REQ);
    }

    CDeviceGroupInfo& GetDeviceGroupInfo() 
    {
        return info;
    }
    const CDeviceGroupInfo& GetDeviceGroupInfo() const
    {
        return info;
    }
    void SetDeviceGroupInfo(const CDeviceGroupInfo &val)
    {
        info = val;
    }

private:
    CDeviceGroupInfo info;

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDeviceGroupDelReq: public CEventReq
{
public:
    CDeviceGroupDelReq()
    {
        event = OspExtEventDesc(TAS_DEVICE_GROUP_DEL_REQ);
    }

    CDeviceGroupInfoKey& GetDeviceGroupInfoKey() 
    {
        return key;
    }
    const CDeviceGroupInfoKey& GetDeviceGroupInfoKey() const
    {
        return key;
    }
    void SetDeviceGroupInfoKey(const CDeviceGroupInfoKey &val)
    {
        key = val;
    }

private:
    CDeviceGroupInfoKey key;

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDeviceGroupDelRsp: public CEventRsp
{
public:
    CDeviceGroupDelRsp()
    {
        event = OspExtEventDesc(TAS_DEVICE_GROUP_DEL_RSP);
    }
    vector<string>& GetConflictedNames()
    {
        return conflictedNames;
    }
    const vector<string>& GetConflictedNames() const
    {
        return conflictedNames;
    }
    void SetConflictedNames(const vector<string> &val)
    {
        conflictedNames = val;
    }
private:
    // 删除设备组时将导致重名的组名列表, 最多提示10个. 错误码为ERR_TAS_EXIST_SAMENAME_RECORD时有效
    vector<string> conflictedNames;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDeviceGroupDelNtfReq: public CEventReq
{
public:
    CDeviceGroupDelNtfReq()
    {
        event = OspExtEventDesc(TAS_DEVICE_GROUP_DEL_NTF_REQ);
    }

    CDeviceGroupInfoKey& GetDeviceGroupInfoKey() 
    {
        return key;
    }
    const CDeviceGroupInfoKey& GetDeviceGroupInfoKey() const
    {
        return key;
    }
    void SetDeviceGroupInfoKey(const CDeviceGroupInfoKey &val)
    {
        key = val;
    }

private:
    CDeviceGroupInfoKey key;
    UUIDString domainId;

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
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

};

class CDeviceGroupModReq: public CEventReq
{
public:
    CDeviceGroupModReq()
    {
        event = OspExtEventDesc(TAS_DEVICE_GROUP_MOD_REQ);
    }

    bool GetDeviceGroupID(UUIDString &val) const
    {
        if (!optionField.HasOption("deviceGroupID"))
            return false;
        val = deviceGroupID;
        return true;
    }
    void SetDeviceGroupID(const UUIDString &val)
    {
        deviceGroupID = val;
        optionField.SetOption("deviceGroupID");
    }
    bool GetDeviceParentGroupID(UUIDString &val) const
    {
        if (!optionField.HasOption("deviceParentGroupID"))
            return false;
        val = deviceParentGroupID;
        return true;
    }
    void SetDeviceParentGroupID(const UUIDString &val)
    {
        deviceParentGroupID = val;
        optionField.SetOption("deviceParentGroupID");
    }
    bool GetDeviceGroupName(string &val) const
    {
        if (!optionField.HasOption("deviceGroupName"))
            return false;
        val = deviceGroupName;
        return true;
    }
    void SetDeviceGroupName(const string &val)
    {
        deviceGroupName = val;
        optionField.SetOption("deviceGroupName");
    }
    bool GetDeviceGroupDesc(string &val) const
    {
        if (!optionField.HasOption("deviceGroupDesc"))
            return false;
        val = deviceGroupDesc;
        return true;
    }
    void SetDeviceGroupDesc(const string &val)
    {
        deviceGroupDesc = val;
        optionField.SetOption("deviceGroupDesc");
    }

	bool GetGrpSeq(double &val) const
	{
		if (!optionField.HasOption("grpseq"))
			return false;
		val = grpseq;
		return true;
	}
	void SetGrpSeq(const double &val)
	{
		grpseq = val;
		optionField.SetOption("grpseq");
	}

	bool GetDevTreeId(string &val) const
	{
		if (!optionField.HasOption("devtreeId"))
			return false;
		val = devtreeId;
		return true;
	}
	void SetDevTreeId(const string &val)
	{
		devtreeId = val;
		optionField.SetOption("devtreeId");
	}
private:
    CModOptionField optionField;
    UUIDString deviceGroupID;
    UUIDString deviceParentGroupID; // 域内此组为顶级组时，该值为空
    string deviceGroupName;
    string deviceGroupDesc;
	double grpseq;
	UUIDString devtreeId;    // 设备组所属设备树ID [3/10/2016 pangubing]

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDeviceGroupModRsp: public CEventRsp
{
public:
    CDeviceGroupModRsp()
    {
        event = OspExtEventDesc(TAS_DEVICE_GROUP_MOD_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDeviceGroupModNtfReq: public CEventReq
{
public:
    CDeviceGroupModNtfReq()
    {
        event = OspExtEventDesc(TAS_DEVICE_GROUP_MOD_NTF_REQ);
    }

    CDeviceGroupInfo& GetDeviceGroupInfo() 
    {
        return info;
    }
    const CDeviceGroupInfo& GetDeviceGroupInfo() const
    {
        return info;
    }
    void SetDeviceGroupInfo(const CDeviceGroupInfo &val)
    {
        info = val;
    }

private:
    CDeviceGroupInfo info;

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDeviceGroupQryReq: public CQueryReqBase
{
public:
    CDeviceGroupQryReq()
    {
        event = OspExtEventDesc(TAS_DEVICE_GROUP_QRY_REQ);
    }

    CDeviceGroupInfoQueryCondition& GetDeviceGroupInfoQueryCondition() 
    {
        return condition;
    }
    const CDeviceGroupInfoQueryCondition& GetDeviceGroupInfoQueryCondition() const
    {
        return condition;
    }
    void SetDeviceGroupInfoQueryCondition(const CDeviceGroupInfoQueryCondition &cDbQueryBase)
    {
        condition = cDbQueryBase;
    }

private:
    CDeviceGroupInfoQueryCondition condition;

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDeviceGroupQryRsp: public CQueryRspBase
{
public:
    CDeviceGroupQryRsp()
    {
        event = OspExtEventDesc(TAS_DEVICE_GROUP_QRY_RSP);
        hasDevice = false;
    }
    bool HasDevice() const
    {
        return hasDevice;
    }
    void SetDevice(bool val)
    {
        hasDevice = val;
    }

private:
    bool hasDevice; // 设备组下是否拥有设备(查询时指定CU用户，则指特定CU用户在该设备组下是否拥有设备)
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDeviceGroupQryNtfReq: public CQueryNtfReqBase
{
public:
    CDeviceGroupQryNtfReq()
    {
        event = OspExtEventDesc(TAS_DEVICE_GROUP_QRY_NTF_REQ);
    }

    vector<CDeviceGroupInfo>& GetDeviceGroupInfo() 
    {
        return infos;
    }
    const vector<CDeviceGroupInfo>& GetDeviceGroupInfo() const
    {
        return infos;
    }
    void SetDeviceGroupInfo(const vector<CDeviceGroupInfo> &val)
    {
        infos = val;
    }
    vector<bool>& GetDeviceGroupHasDevice() 
    {
        return containDevices;
    }
    const vector<bool>& GetDeviceGroupHasDevice() const
    {
        return containDevices;
    }
    void SetDeviceGroupHasDevice(const vector<bool> &val)
    {
        containDevices = val;
    }

private:
    vector<CDeviceGroupInfo> infos;
    vector<bool> containDevices; // 组是否拥有设备标志, 利用数据下标与组信息对应

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CDeviceGroupDeviceAddReq: public CEventReq
{
public:
    CDeviceGroupDeviceAddReq()
    {
        event = OspExtEventDesc(TAS_DEVICE_GROUP_DEVICE_ADD_REQ);
    }

    CDeviceGroupDeviceInfo& GetDeviceGroupDeviceInfo() 
    {
        return info;
    }
    const CDeviceGroupDeviceInfo& GetDeviceGroupDeviceInfo() const
    {
        return info;
    }
    void SetDeviceGroupDeviceInfo(const CDeviceGroupDeviceInfo &val)
    {
        info = val;
    }
private:
    CDeviceGroupDeviceInfo info;

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDeviceGroupDeviceAddRsp: public CEventRsp
{
public:
    CDeviceGroupDeviceAddRsp()
    {
        event = OspExtEventDesc(TAS_DEVICE_GROUP_DEVICE_ADD_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDeviceGroupDeviceAddNtfReq: public CEventReq
{
public:
    CDeviceGroupDeviceAddNtfReq()
    {
        event = OspExtEventDesc(TAS_DEVICE_GROUP_DEVICE_ADD_NTF_REQ);
    }

    CDeviceGroupDeviceInfo& GetDeviceGroupDeviceInfo() 
    {
        return info;
    }
    const CDeviceGroupDeviceInfo& GetDeviceGroupDeviceInfo() const
    {
        return info;
    }
    void SetDeviceGroupDeviceInfo(const CDeviceGroupDeviceInfo &val)
    {
        info = val;
    }
private:
    CDeviceGroupDeviceInfo info;

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

enum EnumModType
{
	TAS_OPERATE_TYPE_NULL = 0,
	TAS_OPERATE_TYPE_ADD_PRIMARY,
	TAS_OPERATE_TYPE_ADD_SECOND,
	TAS_OPERATE_TYPE_DEL_PRIMARY,
	TAS_OPERATE_TYPE_DEL_SECOND,
	TAS_OPERATE_TYPE_MOD_PRIMARY,  
    TAS_OPERATE_TYPE_MOD_SECOND 
};

class CDeviceGroupMod4GbReq : public CEventReq
{
public:
	CDeviceGroupMod4GbReq()
	{
		event = OspExtEventDesc(TAS_DEVICE_GROUP_MOD_4GB_REQ);
	}
private:
	UUIDString devTreeId;               
	UUIDString devGrpId;               
	UUIDString devGroupGbid; //修改gbid,为空时无效
	int        prisec;     //修改主辅流，0无效；1主流；2辅流；3全部

public:
    void SetDevTreeId(const UUIDString& strDevTreeId)
    {
        devTreeId = strDevTreeId;
    }
    UUIDString& GetDevTreeId()
    {
        return devTreeId;
    }
    const UUIDString& GetDevTreeId() const
    {
        return devTreeId;
    }

    void SetDevGrpId(const UUIDString& strDevGrpId)
    {
        devGrpId = strDevGrpId;
    }
    UUIDString& GetDevGrpId()
    {
        return devGrpId;
    }
    const UUIDString& GetDevGrpId() const
    {
        return devGrpId;
    }

    void SetDevGroupGbid(const UUIDString& strDevGroupGbid)
    {
        devGroupGbid = strDevGroupGbid;
    }
    UUIDString& GetDevGroupGbid()
    {
        return devGroupGbid;
    }
    const UUIDString& GetDevGroupGbid() const
    {
        return devGroupGbid;
    }

    void SetPrisec(int nPrisec)
    {
        prisec = nPrisec;
    }
    int GetPrisec() const
    {
        return prisec;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDeviceGroupMod4GbRsp : public CEventRsp
{
public:
	CDeviceGroupMod4GbRsp()
	{
		event = OspExtEventDesc(TAS_DEVICE_GROUP_MOD_4GB_RSP);
	}
private:
	UUIDString operUuid;                // 操作Id，标志唯一的一次操作，用于后续查询进度

public:
    void SetOperUuid(const UUIDString& strOperUuid)
    {
        operUuid = strOperUuid;
    }
    UUIDString& GetOperUuid()
    {
        return operUuid;
    }
    const UUIDString& GetOperUuid() const
    {
        return operUuid;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDeviceGroupDeviceMod4GbReq: public CEventReq
{
public:
	CDeviceGroupDeviceMod4GbReq()
	{
		event = OspExtEventDesc(TAS_DEVICE_GROUP_DEVICE_MOD_4GB_REQ);
		operatetype = TAS_OPERATE_TYPE_NULL;
	}
private:
	UUIDString devTreeId;               
	UUIDString deviceGroupID;               
	UUIDString deviceId;               
	int vidIndex;               //视频源序号
	int operatetype;               //添加主流、添加辅流，删除主流、删除辅流、修改主流、修改辅流
	UUIDString gbid;               //修改的gbid
public:
    void SetDevTreeId(const UUIDString& strDevTreeId)
    {
        devTreeId = strDevTreeId;
    }
    UUIDString& GetDevTreeId()
    {
        return devTreeId;
    }
    const UUIDString& GetDevTreeId() const
    {
        return devTreeId;
    }

    void SetDeviceGroupID(const UUIDString& strDeviceGroupID)
    {
        deviceGroupID = strDeviceGroupID;
    }
    UUIDString& GetDeviceGroupID()
    {
        return deviceGroupID;
    }
    const UUIDString& GetDeviceGroupID() const
    {
        return deviceGroupID;
    }

    void SetDeviceId(const UUIDString& strDeviceId)
    {
        deviceId = strDeviceId;
    }
    UUIDString& GetDeviceId()
    {
        return deviceId;
    }
    const UUIDString& GetDeviceId() const
    {
        return deviceId;
    }

    void SetVidIndex(int nVidIndex)
    {
        vidIndex = nVidIndex;
    }
    int GetVidIndex() const
    {
        return vidIndex;
    }

    void SetOperatetype(int nOperatetype)
    {
        operatetype = nOperatetype;
    }
    int GetOperatetype() const
    {
        return operatetype;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetGbid(const UUIDString& strGbid)
    {
        gbid = strGbid;
    }
    UUIDString& GetGbid()
    {
        return gbid;
    }
    const UUIDString& GetGbid() const
    {
        return gbid;
    }

};

class CDeviceGroupDeviceMod4GbRsp: public CEventRsp
{
public:
	CDeviceGroupDeviceMod4GbRsp()
	{
		event = OspExtEventDesc(TAS_DEVICE_GROUP_DEVICE_MOD_4GB_RSP);
	}
private:
	UUIDString devtreeId;               
	UUIDString devgroupId;                              
	UUIDString deviceId;                              
	int vidIndex;                              
	TDeviceGbid4Gb gbid;                 
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetDevtreeId(const UUIDString& strDevtreeId)
    {
        devtreeId = strDevtreeId;
    }
    UUIDString& GetDevtreeId()
    {
        return devtreeId;
    }
    const UUIDString& GetDevtreeId() const
    {
        return devtreeId;
    }

    void SetDevgroupId(const UUIDString& strDevgroupId)
    {
        devgroupId = strDevgroupId;
    }
    UUIDString& GetDevgroupId()
    {
        return devgroupId;
    }
    const UUIDString& GetDevgroupId() const
    {
        return devgroupId;
    }

    void SetDeviceId(const UUIDString& strDeviceId)
    {
        deviceId = strDeviceId;
    }
    UUIDString& GetDeviceId()
    {
        return deviceId;
    }
    const UUIDString& GetDeviceId() const
    {
        return deviceId;
    }

    void SetVidIndex(int nVidIndex)
    {
        vidIndex = nVidIndex;
    }
    int GetVidIndex() const
    {
        return vidIndex;
    }

    void SetGbid(const TDeviceGbid4Gb& cGbid)
    {
        gbid = cGbid;
    }
    TDeviceGbid4Gb& GetGbid()
    {
        return gbid;
    }
    const TDeviceGbid4Gb& GetGbid() const
    {
        return gbid;
    }

};


class CDeviceGroupDeviceDelReq: public CEventReq
{
public:
    CDeviceGroupDeviceDelReq()
    {
        event = OspExtEventDesc(TAS_DEVICE_GROUP_DEVICE_DEL_REQ);
    }

    CDeviceGroupDeviceInfoKey& GetDeviceGroupDeviceInfoKey() 
    {
        return key;
    }
    const CDeviceGroupDeviceInfoKey& GetDeviceGroupDeviceInfoKey() const
    {
        return key;
    }
    void SetDeviceGroupDeviceInfoKey(const CDeviceGroupDeviceInfoKey &val)
    {
        key = val;
    }
private:
    CDeviceGroupDeviceInfoKey key;

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDeviceGroupDeviceDelRsp: public CEventRsp
{
public:
    CDeviceGroupDeviceDelRsp()
    {
        event = OspExtEventDesc(TAS_DEVICE_GROUP_DEVICE_DEL_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDeviceGroupDeviceDelNtfReq: public CEventReq
{
public:
    CDeviceGroupDeviceDelNtfReq()
    {
        event = OspExtEventDesc(TAS_DEVICE_GROUP_DEVICE_DEL_NTF_REQ);
    }

    CDeviceGroupDeviceInfoKey& GetDeviceGroupDeviceInfoKey() 
    {
        return key;
    }
    const CDeviceGroupDeviceInfoKey& GetDeviceGroupDeviceInfoKey() const
    {
        return key;
    }
    void SetDeviceGroupDeviceInfoKey(const CDeviceGroupDeviceInfoKey &val)
    {
        key = val;
    }
private:
    CDeviceGroupDeviceInfoKey key;
    UUIDString domainId;               // 组或设备所属的域Id(业务上设备只能被划归到同域的设备组中)

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
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

};

class CDeviceGroupDeviceOrderReq:public CEventReq
{
public:
	CDeviceGroupDeviceOrderReq()
	{
		event = OspExtEventDesc(TAS_DEVICE_GROUP_DEVICE_ORDER_REQ);
	}

private:
	UUIDString devTreeId;               
	TVidSrcOrderInfo vidsrcOrderInfo;               
public:
    void SetDevTreeId(const UUIDString& strDevTreeId)
    {
        devTreeId = strDevTreeId;
    }
    UUIDString& GetDevTreeId()
    {
        return devTreeId;
    }
    const UUIDString& GetDevTreeId() const
    {
        return devTreeId;
    }

    void SetVidsrcOrderInfo(const TVidSrcOrderInfo& cVidsrcOrderInfo)
    {
        vidsrcOrderInfo = cVidsrcOrderInfo;
    }
    TVidSrcOrderInfo& GetVidsrcOrderInfo()
    {
        return vidsrcOrderInfo;
    }
    const TVidSrcOrderInfo& GetVidsrcOrderInfo() const
    {
        return vidsrcOrderInfo;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDeviceGroupDeviceOrderRsp:public CEventRsp
{
public:
	CDeviceGroupDeviceOrderRsp()
	{
		event = OspExtEventDesc(TAS_DEVICE_GROUP_DEVICE_ORDER_RSP);
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDeviceGroupDeviceOrderNtfReq:public CEventReq
{
public:
	CDeviceGroupDeviceOrderNtfReq()
	{
		event = OspExtEventDesc(TAS_DEVICE_GROUP_DEVICE_ORDER_NTF_REQ);

	}
private:
	UUIDString domainId;               
	TVidSrcOrderInfo vidsrcOrderInfo;               
public:
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

    void SetVidsrcOrderInfo(const TVidSrcOrderInfo& cVidsrcOrderInfo)
    {
        vidsrcOrderInfo = cVidsrcOrderInfo;
    }
    TVidSrcOrderInfo& GetVidsrcOrderInfo()
    {
        return vidsrcOrderInfo;
    }
    const TVidSrcOrderInfo& GetVidsrcOrderInfo() const
    {
        return vidsrcOrderInfo;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDeviceGroupDeviceQryReq: public CQueryReqBase
{
public:
    CDeviceGroupDeviceQryReq()
    {
        event = OspExtEventDesc(TAS_DEVICE_GROUP_DEVICE_QRY_REQ);
    }

    CDeviceGroupDeviceInfoQueryCondition& GetDeviceGroupDeviceInfoQueryCondition() 
    {
        return condition;
    }
    const CDeviceGroupDeviceInfoQueryCondition& GetDeviceGroupDeviceInfoQueryCondition() const
    {
        return condition;
    }
    void SetDeviceGroupDeviceInfoQueryCondition(const CDeviceGroupDeviceInfoQueryCondition &val)
    {
        condition = val;
    }
private:
    CDeviceGroupDeviceInfoQueryCondition condition;

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDeviceGroupDeviceQryRsp: public CQueryRspBase
{
public:
    CDeviceGroupDeviceQryRsp()
    {
        event = OspExtEventDesc(TAS_DEVICE_GROUP_DEVICE_QRY_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDeviceGroupDeviceQryNtfReq: public CQueryNtfReqBase
{
public:
    CDeviceGroupDeviceQryNtfReq()
    {
        event = OspExtEventDesc(TAS_DEVICE_GROUP_DEVICE_QRY_NTF_REQ);
    }

    vector<CDeviceGroupDeviceInfo>& GetDeviceGroupDeviceInfo()
    {
        return infos;
    }
    const vector<CDeviceGroupDeviceInfo>& GetDeviceGroupDeviceInfo() const
    {
        return infos;
    }
    void SetDeviceGroupDeviceInfo(const vector<CDeviceGroupDeviceInfo> &val)
    {
        infos = val;
    }
private:
    vector<CDeviceGroupDeviceInfo> infos;

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDeviceGroupDeviceQry4GbReq: public CQueryReqBase
{
public:
	CDeviceGroupDeviceQry4GbReq()
	{
		event = OspExtEventDesc(TAS_DEVICE_GROUP_DEVICE_QRY_4GB_REQ);
	}
private:
	CDeviceGroupDeviceInfoQuery4GbCondition condition;                                          
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

public:
    void SetCondition(const CDeviceGroupDeviceInfoQuery4GbCondition& cCondition)
    {
        condition = cCondition;
    }
    CDeviceGroupDeviceInfoQuery4GbCondition& GetCondition()
    {
        return condition;
    }
    const CDeviceGroupDeviceInfoQuery4GbCondition& GetCondition() const
    {
        return condition;
    }

};

class CDeviceGroupDeviceQry4GbRsp: public CQueryRspBase
{
public:
	CDeviceGroupDeviceQry4GbRsp()
	{
		event = OspExtEventDesc(TAS_DEVICE_GROUP_DEVICE_QRY_4GB_RSP);
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDeviceGroupDeviceQry4GbNtfReq: public CQueryNtfReqBase
{
public:
	CDeviceGroupDeviceQry4GbNtfReq()
	{
		event = OspExtEventDesc(TAS_DEVICE_GROUP_DEVICE_QRY_4GB_NTF_REQ);
	}
private:
	vector<CDeviceGroupDeviceInfo4Gb> infos;               
public:
    void SetInfos(const vector<CDeviceGroupDeviceInfo4Gb>& cInfos)
    {
        infos = cInfos;
    }
    vector<CDeviceGroupDeviceInfo4Gb>& GetInfos()
    {
        return infos;
    }
    const vector<CDeviceGroupDeviceInfo4Gb>& GetInfos() const
    {
        return infos;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDeviceGroupChildrenQryReq:public CEventReq
{
public:
	CDeviceGroupChildrenQryReq()
	{
		event = OspExtEventDesc(TAS_DEVICE_GROUP_CHILDREN_QRY_REQ);
	}

private:
	UUIDString devTreeId;               
	UUIDString devGrpId;               
public:
    void SetDevTreeId(const UUIDString& strDevTreeId)
    {
        devTreeId = strDevTreeId;
    }
    UUIDString& GetDevTreeId()
    {
        return devTreeId;
    }
    const UUIDString& GetDevTreeId() const
    {
        return devTreeId;
    }

    void SetDevGrpId(const UUIDString& strDevGrpId)
    {
        devGrpId = strDevGrpId;
    }
    UUIDString& GetDevGrpId()
    {
        return devGrpId;
    }
    const UUIDString& GetDevGrpId() const
    {
        return devGrpId;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDeviceGroupChildrenQryRsp:public CQueryRspBase
{
public:
	CDeviceGroupChildrenQryRsp()
	{
		event = OspExtEventDesc(TAS_DEVICE_GROUP_CHILDREN_QRY_RSP);
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDeviceGroupChildrenQryNtfReq:public CQueryNtfReqBase
{
public:
	CDeviceGroupChildrenQryNtfReq()
	{
		event = OspExtEventDesc(TAS_DEVICE_GROUP_CHILDREN_QRY_NTF_REQ);
	}

private:
	vector<CDeviceGroupInfo> devGrpInfos;                              
	vector<CDeviceGroupDeviceInfo4Gb> devInfos;                              

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetDevInfos(const vector<CDeviceGroupDeviceInfo4Gb>& cDevInfos)
    {
        devInfos = cDevInfos;
    }
    vector<CDeviceGroupDeviceInfo4Gb>& GetDevInfos()
    {
        return devInfos;
    }
    const vector<CDeviceGroupDeviceInfo4Gb>& GetDevInfos() const
    {
        return devInfos;
    }

public:
    void SetDevGrpInfos(const vector<CDeviceGroupInfo>& cDevGrpInfos)
    {
        devGrpInfos = cDevGrpInfos;
    }
    vector<CDeviceGroupInfo>& GetDevGrpInfos()
    {
        return devGrpInfos;
    }
    const vector<CDeviceGroupInfo>& GetDevGrpInfos() const
    {
        return devGrpInfos;
    }

};


class CDeviceGbidErrGetReq:public CEventReq
{
public:
	CDeviceGbidErrGetReq()
	{
		 event = OspExtEventDesc(TAS_DEVICE_GBID_ERROR_GET_REQ);
	}

private:
	UUIDString   devTreeId;                
	UUIDString   groupId;                //为空时查询整棵树
	int          errIndex;               //上次错误设备的序号

public:
    void SetDevTreeId(const UUIDString& strDevTreeId)
    {
        devTreeId = strDevTreeId;
    }
    UUIDString& GetDevTreeId()
    {
        return devTreeId;
    }
    const UUIDString& GetDevTreeId() const
    {
        return devTreeId;
    }

    void SetGroupId(const UUIDString& strGroupId)
    {
        groupId = strGroupId;
    }
    UUIDString& GetGroupId()
    {
        return groupId;
    }
    const UUIDString& GetGroupId() const
    {
        return groupId;
    }
	void SetErrIndex(int nErrIndex)
	{
		errIndex = nErrIndex;
	}
	int GetErrIndex() const
	{
		return errIndex;
	}

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDeviceGbidErrGetRsp:public CEventRsp
{
public:
	CDeviceGbidErrGetRsp()
	{
		event = OspExtEventDesc(TAS_DEVICE_GBID_ERROR_GET_RSP);
	}

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

private:
	bool existErr;                 //false-表中已无错误gbid(本条目TDevGbidErrItem亦无效)，true-本条目TDevGbidErrItem，表中是否有其他错误gbid需看下次查询
	TDevGbidErrItem errItem;               
public:
    void SetExistErr(bool bExistErr)
    {
        existErr = bExistErr;
    }
    bool GetExistErr() const
    {
        return existErr;
    }

    void SetErrItem(const TDevGbidErrItem& cErrItem)
    {
        errItem = cErrItem;
    }
    TDevGbidErrItem& GetErrItem()
    {
        return errItem;
    }
    const TDevGbidErrItem& GetErrItem() const
    {
        return errItem;
    }

};


class CDeviceMatchGbidReq:public CEventReq
{
public:
	CDeviceMatchGbidReq()
	{
		matchLastBits = false;
		event = OspExtEventDesc(TAS_DEVICE_MATCH_GBID_REQ);
	}

private:
	UUIDString devTreeId;               
	UUIDString devGrpId;               
	UUIDString devId;     //为空时，需匹配分组下所有设备的国标ID
	int        devVidIndex;               
	int        devMatchType; //主流 0，辅流 1  
	bool       matchLastBits;                //是否匹配ID的后六位，默认false
public:
    void SetDevTreeId(const UUIDString& strDevTreeId)
    {
        devTreeId = strDevTreeId;
    }
    UUIDString& GetDevTreeId()
    {
        return devTreeId;
    }
    const UUIDString& GetDevTreeId() const
    {
        return devTreeId;
    }

    void SetDevGrpId(const UUIDString& strDevGrpId)
    {
        devGrpId = strDevGrpId;
    }
    UUIDString& GetDevGrpId()
    {
        return devGrpId;
    }
    const UUIDString& GetDevGrpId() const
    {
        return devGrpId;
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

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetDevVidIndex(int nDevVidIndex)
    {
        devVidIndex = nDevVidIndex;
    }
    int GetDevVidIndex() const
    {
        return devVidIndex;
    }

    void SetDevMatchType(int nDevMatchType)
    {
        devMatchType = nDevMatchType;
    }
    int GetDevMatchType() const
    {
        return devMatchType;
    }

public:
    void SetMatchLastBits(bool bMatchLastBits)
    {
        matchLastBits = bMatchLastBits;
    }
    bool GetMatchLastBits() const
    {
        return matchLastBits;
    }

};

class CDeviceMatchGbidRsp:public CEventRsp
{
public:
	CDeviceMatchGbidRsp()
	{
		event = OspExtEventDesc(TAS_DEVICE_MATCH_GBID_RSP);
	}
	UUIDString gbid;   //设备匹配后的gbid，分组匹配为空
	UUIDString operId;                
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetGbid(const UUIDString& strGbid)
    {
        gbid = strGbid;
    }
    UUIDString& GetGbid()
    {
        return gbid;
    }
    const UUIDString& GetGbid() const
    {
        return gbid;
    }

public:
    void SetOperId(const UUIDString& strOperId)
    {
        operId = strOperId;
    }
    UUIDString& GetOperId()
    {
        return operId;
    }
    const UUIDString& GetOperId() const
    {
        return operId;
    }

};

class CDeviceGrpDevSumNumReq:public CEventReq
{
public:
	CDeviceGrpDevSumNumReq()
	{
		event = OspExtEventDesc(TAS_DEVICE_GROUP_DEVICE_SUMNUM_REQ);
	}

private:
	UUIDString devTreeId;               
	UUIDString devGrpId;               
public:
    void SetDevTreeId(const UUIDString& strDevTreeId)
    {
        devTreeId = strDevTreeId;
    }
    UUIDString& GetDevTreeId()
    {
        return devTreeId;
    }
    const UUIDString& GetDevTreeId() const
    {
        return devTreeId;
    }

    void SetDevGrpId(const UUIDString& strDevGrpId)
    {
        devGrpId = strDevGrpId;
    }
    UUIDString& GetDevGrpId()
    {
        return devGrpId;
    }
    const UUIDString& GetDevGrpId() const
    {
        return devGrpId;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDeviceGrpDevSumNumRsp:public CEventRsp
{
public:
	CDeviceGrpDevSumNumRsp()
	{
		event = OspExtEventDesc(TAS_DEVICE_GROUP_DEVICE_SUMNUM_RSP);
	}
	int totalNum;                //指定分组下所有通道个数（子分组递归）
public:
    void SetTotalNum(int nTotalNum)
    {
        totalNum = nTotalNum;
    }
    int GetTotalNum() const
    {
        return totalNum;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDeviceGbinfoQryReq:public CQueryReqBase
{
public:
	CDeviceGbinfoQryReq()
	{
		event = OspExtEventDesc(TAS_DEVICE_GBINFO_QRY_REQ);
	}

private:
	UUIDString devTreeId;               
	string gbid;
	string name;               
public:
    void SetDevTreeId(const UUIDString& strDevTreeId)
    {
        devTreeId = strDevTreeId;
    }
    UUIDString& GetDevTreeId()
    {
        return devTreeId;
    }
    const UUIDString& GetDevTreeId() const
    {
        return devTreeId;
    }

    void SetGbid(const string& strGbid)
    {
        gbid = strGbid;
    }
    string& GetGbid()
    {
        return gbid;
    }
    const string& GetGbid() const
    {
        return gbid;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetName(const string& strName)
    {
        name = strName;
    }
    string& GetName()
    {
        return name;
    }
    const string& GetName() const
    {
        return name;
    }

};

class CDeviceGbinfoQryRsp:public CQueryRspBase
{
public:
	CDeviceGbinfoQryRsp()
	{
		event = OspExtEventDesc(TAS_DEVICE_GBINFO_QRY_RSP);
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDeviceGbinfoQryNtfReq:public CQueryNtfReqBase
{
public:
	CDeviceGbinfoQryNtfReq()
	{
		event = OspExtEventDesc(TAS_DEVICE_GBINFO_QRY_NTF_REQ);
	}
private:
	vector<CDeviceFullGbInfo> devGbinfos;               
public:
    void SetDevGbinfos(const vector<CDeviceFullGbInfo>& cDevGbinfos)
    {
        devGbinfos = cDevGbinfos;
    }
    vector<CDeviceFullGbInfo>& GetDevGbinfos()
    {
        return devGbinfos;
    }
    const vector<CDeviceFullGbInfo>& GetDevGbinfos() const
    {
        return devGbinfos;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDevgrpGbinfoQryReq:public CQueryReqBase
{
public:
	CDevgrpGbinfoQryReq()
	{
		event = OspExtEventDesc(TAS_DEVGRP_GBINFO_QRY_REQ);
	}

private:
	UUIDString devTreeId;               
	string civilCode; 
	string name;               
public:
    void SetDevTreeId(const UUIDString& strDevTreeId)
    {
        devTreeId = strDevTreeId;
    }
    UUIDString& GetDevTreeId()
    {
        return devTreeId;
    }
    const UUIDString& GetDevTreeId() const
    {
        return devTreeId;
    }

    void SetCivilCode(const string& strCivilCode)
    {
        civilCode = strCivilCode;
    }
    string& GetCivilCode()
    {
        return civilCode;
    }
    const string& GetCivilCode() const
    {
        return civilCode;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetName(const string& strName)
    {
        name = strName;
    }
    string& GetName()
    {
        return name;
    }
    const string& GetName() const
    {
        return name;
    }

};

class CDevgrpGbinfoQryRsp:public CQueryRspBase
{
public:
	CDevgrpGbinfoQryRsp()
	{
		event = OspExtEventDesc(TAS_DEVGRP_GBINFO_QRY_RSP);
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDevgrpGbinfoQryNtfReq:public CQueryNtfReqBase
{
public:
	CDevgrpGbinfoQryNtfReq()
	{
		event = OspExtEventDesc(TAS_DEVGRP_GBINFO_QRY_NTF_REQ);
	}
private:

	vector<TDevGrpFullGbInfos> devGbinfos;                
public:
    void SetDevGbinfos(const vector<TDevGrpFullGbInfos>& cDevGbinfos)
    {
        devGbinfos = cDevGbinfos;
    }
    vector<TDevGrpFullGbInfos>& GetDevGbinfos()
    {
        return devGbinfos;
    }
    const vector<TDevGrpFullGbInfos>& GetDevGbinfos() const
    {
        return devGbinfos;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUserGroupAddReq: public CEventReq
{
public:
    CUserGroupAddReq()
    {
        event = OspExtEventDesc(TAS_CUSER_GROUP_ADD_REQ);
    }

    CUserGroupInfo& GetUserGroupInfo() 
    {
        return info;
    }
    const CUserGroupInfo& GetUserGroupInfo() const
    {
        return info;
    }
    void SetUserGroupInfo(const CUserGroupInfo &val)
    {
        info = val;
    }
private:
    CUserGroupInfo info;

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUserGroupAddRsp: public CEventRsp
{
public:
    CUserGroupAddRsp()
    {
        event = OspExtEventDesc(TAS_CUSER_GROUP_ADD_RSP);
    }

    CUserGroupInfoKey& GetUserGroupInfoKey() 
    {
        return key;
    }
    const CUserGroupInfoKey& GetUserGroupInfoKey() const
    {
        return key;
    }
    void SetUserGroupInfoKey(const CUserGroupInfoKey &val)
    {
        key = val;
    }
private:
    CUserGroupInfoKey key;

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUserGroupAddNtfReq: public CEventReq
{
public:
    CUserGroupAddNtfReq()
    {
        event = OspExtEventDesc(TAS_CUSER_GROUP_ADD_NTF_REQ);
    }

    CUserGroupInfo& GetUserGroupInfo()
    {
        return info;
    }
    const CUserGroupInfo& GetUserGroupInfo() const
    {
        return info;
    }
    void SetUserGroupInfo(const CUserGroupInfo &val)
    {
        info = val;
    }
private:
    CUserGroupInfo info;

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUserGroupDelReq: public CEventReq
{
public:
    CUserGroupDelReq()
    {
        event = OspExtEventDesc(TAS_CUSER_GROUP_DEL_REQ);
    }

    CUserGroupInfoKey& GetUserGroupInfoKey() 
    {
        return key;
    }
    const CUserGroupInfoKey& GetUserGroupInfoKey() const
    {
        return key;
    }
    void SetUserGroupInfoKey(const CUserGroupInfoKey &val)
    {
        key = val;
    }
private:
    CUserGroupInfoKey key;

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUserGroupDelRsp: public CEventRsp
{
public:
    CUserGroupDelRsp()
    {
        event = OspExtEventDesc(TAS_CUSER_GROUP_DEL_RSP);
    }

    void SetOperUuid(const UUIDString &val)
    {
        operUuid = val;
    }
    const UUIDString &GetOperUuid() const
    {
        return operUuid;
    }
    UUIDString &GetOperUuid()
    {
        return operUuid;
    }

private:
    UUIDString operUuid;				// 操作Id，标志唯一的一次操作，用于后续查询进度

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUserGroupDelNtfReq: public CEventReq
{
public:
    CUserGroupDelNtfReq()
    {
        event = OspExtEventDesc(TAS_CUSER_GROUP_DEL_NTF_REQ);
    }

    CUserGroupInfoKey& GetUserGroupInfoKey()
    {
        return key;
    }
    const CUserGroupInfoKey& GetUserGroupInfoKey() const
    {
        return key;
    }
    void SetUserGroupInfoKey(const CUserGroupInfoKey &val)
    {
        key = val;
    }
private:
    CUserGroupInfoKey key;

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUserGroupModReq: public CEventReq
{
public:
    CUserGroupModReq()
    {
        event = OspExtEventDesc(TAS_CUSER_GROUP_MOD_REQ);
		switchNum = -1;
		waterMarkEnabled = false;
		waterMarkFontSize = 1;
		waterMarkRotationAngle = 30;
		waterMarkTransparent = 30;
		waterMarkTiled = true;
		waterMarkColor = 16711680;
		waterMarkFont = "";
    }

    bool GetGroupNO(UUIDString &val) const
    {
        if (!optionField.HasOption("id"))
            return false;
        val = id;
        return true;
    }
    void SetGroupNO(const UUIDString &val)
    {
        id = val;
        optionField.SetOption("id");
    }
    bool GetGroupName(string &val) const
    {
        if (!optionField.HasOption("name"))
            return false;
        val = name;
        return true;
    }
    void SetGroupName(const string &val)
    {
        name = val;
        optionField.SetOption("name");
    }

    bool GetDescription(string &val) const
    {
        if (!optionField.HasOption("description"))
            return false;
        val = description;
        return true;
    }
    void SetDescription(const string &val)
    {
        description = val;
        optionField.SetOption("description");
    }
    bool GetUserPrivilege(CUserPrivilege &val) const
    {
        if (!optionField.HasOption("userPrivilege"))
            return false;
        val = userPrivilege;
        return true;
    }
    void SetUserPrivilege(const CUserPrivilege &val)
    {
        userPrivilege = val;
        optionField.SetOption("userPrivilege");
    }
	void SetSwitchNum(int nSwitchNum)
	{
		switchNum = nSwitchNum;
		optionField.SetOption("switchNum");
	}
	bool GetSwitchNum(int &val) const
	{
		if (!optionField.HasOption("switchNum"))
			return false;
		val = switchNum;
		return true;
	}

	void SetWaterMarkEnabled(bool bWaterMarkEnabled)
	{
		waterMarkEnabled = bWaterMarkEnabled;
		optionField.SetOption("waterMarkEnabled");
	}
	bool GetWaterMarkEnabled(bool &val) const
	{
		if (!optionField.HasOption("waterMarkEnabled"))
			return false;
		val = waterMarkEnabled;
		return true;
	}

	void SetWaterMarkFont(const string& strWaterMarkFont)
	{
		waterMarkFont = strWaterMarkFont;
		optionField.SetOption("waterMarkFont");
	}
	bool GetWaterMarkFont(string &val) const
	{
		if (!optionField.HasOption("waterMarkFont"))
			return false;
		val = waterMarkFont;
		return true;
	}

	void SetWaterMarkFontSize(int nWaterMarkFontSize)
	{
		waterMarkFontSize = nWaterMarkFontSize;
		optionField.SetOption("waterMarkFontSize");
	}
	bool GetWaterMarkFontSize(int &val) const
	{
		if (!optionField.HasOption("waterMarkFontSize"))
			return false;
		val = waterMarkFontSize;
		return true;
	}

	void SetWaterMarkRotationAngle(int nWaterMarkRotationAngle)
	{
		waterMarkRotationAngle = nWaterMarkRotationAngle;
		optionField.SetOption("waterMarkRotationAngle");
	}
	bool GetWaterMarkRotationAngle(int &val) const
	{
		if (!optionField.HasOption("waterMarkRotationAngle"))
			return false;
		val = waterMarkRotationAngle;
		return true;
	}

	void SetWaterMarkColor(int nWaterMarkColor)
	{
		waterMarkColor = nWaterMarkColor;
		optionField.SetOption("waterMarkColor");
	}
	bool GetWaterMarkColor(int &val) const
	{
		if (!optionField.HasOption("waterMarkColor"))
			return false;
		val = waterMarkColor;
		return true;
	}

	void SetWaterMarkTransparent(int nWaterMarkTransparent)
	{
		waterMarkTransparent = nWaterMarkTransparent;
		optionField.SetOption("waterMarkTransparent");
	}
	bool GetWaterMarkTransparent(int &val) const
	{
		if (!optionField.HasOption("waterMarkTransparent"))
			return false;
		val = waterMarkTransparent;
		return true;
	}

	void SetWaterMarkTiled(bool bWaterMarkTiled)
	{
		waterMarkTiled = bWaterMarkTiled;
		optionField.SetOption("waterMarkTiled");
	}
	bool GetWaterMarkTiled(bool &val) const
	{
		if (!optionField.HasOption("waterMarkTiled"))
			return false;
		val = waterMarkTiled;
		return true;
	}
private:
    CModOptionField optionField;
    UUIDString id;
    string name;
    string description; // 描述
    CUserPrivilege userPrivilege;
	int switchNum;                //码流并发数
	//视频水印功能
	bool waterMarkEnabled;                       //是否启用水印
	string waterMarkFont;                        //水印字体
	int waterMarkFontSize;                       //字体大小
	int waterMarkRotationAngle;                  //旋转角度
	int waterMarkColor;                          //颜色
	int waterMarkTransparent;                    //透明度
	bool waterMarkTiled;                         //是否平铺
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUserGroupModRsp: public CEventRsp
{
public:
    CUserGroupModRsp()
    {
        event = OspExtEventDesc(TAS_CUSER_GROUP_MOD_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUserGroupModNtfReq: public CEventReq
{
public:
    CUserGroupModNtfReq()
    {
        event = OspExtEventDesc(TAS_CUSER_GROUP_MOD_NTF_REQ);
    }

    CUserGroupInfo& GetUserGroupInfo()
    {
        return info;
    }
    const CUserGroupInfo& GetUserGroupInfo() const
    {
        return info;
    }
    void SetUserGroupInfo(const CUserGroupInfo &val)
    {
        info = val;
    }
private:
    CUserGroupInfo info;

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUserGroupQryReq: public CQueryReqBase
{
public:
    CUserGroupQryReq()
    {
        event = OspExtEventDesc(TAS_CUSER_GROUP_QRY_REQ);
    }

    CUserGroupInfoQueryCondition& GetUserGroupInfoQueryCondition() 
    {
        return condition;
    }
    const CUserGroupInfoQueryCondition& GetUserGroupInfoQueryCondition() const
    {
        return condition;
    }
    void SetUserGroupInfoQueryCondition(const CUserGroupInfoQueryCondition &val)
    {
        condition = val;
    }
private:
    CUserGroupInfoQueryCondition condition;

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUserGroupQryRsp: public CQueryRspBase
{
public:
    CUserGroupQryRsp()
    {
        event = OspExtEventDesc(TAS_CUSER_GROUP_QRY_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUserGroupQryNtfReq: public CQueryNtfReqBase
{
public:
    CUserGroupQryNtfReq()
    {
        event = OspExtEventDesc(TAS_CUSER_GROUP_QRY_NTF_REQ);
    }

    vector<CUserGroupInfo>& GetUserGroupInfo()
    {
        return infos;
    }
    const vector<CUserGroupInfo>& GetUserGroupInfo() const
    {
        return infos;
    }
    void SetUserGroupInfo(const vector<CUserGroupInfo> &val)
    {
        infos = val;
    }
private:
    vector<CUserGroupInfo> infos;

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUserGroupDeviceAddReq: public CEventReq
{
public:
    CUserGroupDeviceAddReq()
    {
        event = OspExtEventDesc(TAS_CUSER_GROUP_DEVICE_ADD_REQ);
    }

    CUserGroupDeviceInfo& GetUserGroupDeviceInfo() 
    {
        return info;
    }
    const CUserGroupDeviceInfo& GetUserGroupDeviceInfo() const
    {
        return info;
    }
    void SetUserGroupDeviceInfo(const CUserGroupDeviceInfo &val)
    {
        info = val;
    }
private:
    CUserGroupDeviceInfo info;

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUserGroupDeviceAddRsp: public CEventRsp
{
public:
    CUserGroupDeviceAddRsp()
    {
        event = OspExtEventDesc(TAS_CUSER_GROUP_DEVICE_ADD_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUserGroupDeviceAddNtfReq: public CEventReq
{
public:
    CUserGroupDeviceAddNtfReq()
    {
        event = OspExtEventDesc(TAS_CUSER_GROUP_DEVICE_ADD_NTF_REQ);
    }

    CUserGroupDeviceInfo& GetUserGroupDeviceInfo() 
    {
        return info;
    }
    const CUserGroupDeviceInfo& GetUserGroupDeviceInfo() const
    {
        return info;
    }
    void SetUserGroupDeviceInfo(const CUserGroupDeviceInfo &val)
    {
        info = val;
    }
private:
    CUserGroupDeviceInfo info;

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUserGroupDeviceDelReq: public CEventReq
{
public:
    CUserGroupDeviceDelReq()
    {
        event = OspExtEventDesc(TAS_CUSER_GROUP_DEVICE_DEL_REQ);
    }

    CUserGroupDeviceInfoKey& GetUserGroupDeviceInfoKey() 
    {
        return key;
    }
    const CUserGroupDeviceInfoKey& GetUserGroupDeviceInfoKey() const
    {
        return key;
    }
    void SetUserGroupDeviceInfoKey(const CUserGroupDeviceInfoKey &val)
    {
        key = val;
    }
private:
    CUserGroupDeviceInfoKey key;

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUserGroupDeviceDelRsp: public CEventRsp
{
public:
    CUserGroupDeviceDelRsp()
    {
        event = OspExtEventDesc(TAS_CUSER_GROUP_DEVICE_DEL_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUserGroupDeviceDelNtfReq: public CEventReq
{
public:
    CUserGroupDeviceDelNtfReq()
    {
        event = OspExtEventDesc(TAS_CUSER_GROUP_DEVICE_DEL_NTF_REQ);
    }

    CUserGroupDeviceInfoKey& GetUserGroupDeviceInfoKey() 
    {
        return key;
    }
    const CUserGroupDeviceInfoKey& GetUserGroupDeviceInfoKey() const
    {
        return key;
    }
    void SetUserGroupDeviceInfoKey(const CUserGroupDeviceInfoKey &val)
    {
        key = val;
    }
    void SetDevDomainId(const UUIDString& strDevDomainId)
    {
        devDomainId = strDevDomainId;
    }
    UUIDString& GetDevDomainId()
    {
        return devDomainId;
    }
    const UUIDString& GetDevDomainId() const
    {
        return devDomainId;
    }
private:
    CUserGroupDeviceInfoKey key;
    UUIDString devDomainId;

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUserGroupDeviceModReq: public CEventReq
{
public:
    CUserGroupDeviceModReq()
    {
        event = OspExtEventDesc(TAS_CUSER_GROUP_DEVICE_MOD_REQ);
    }

    bool GetGroupID(UUIDString &val) const
    {
        if (!optionField.HasOption("groupID"))
            return false;
        val = groupID;
        return true;
    }
    void SetGroupID(const UUIDString &val)
    {
        groupID = val;
        optionField.SetOption("groupID");
    }
    bool GetDeviceID(UUIDString &val) const
    {
        if (!optionField.HasOption("deviceID"))
            return false;
        val = deviceID;
        return true;
    }
    void SetDeviceID(const UUIDString &val)
    {
        deviceID = val;
        optionField.SetOption("deviceID");
    }
    bool GetDevicePrivilege(CDevicePrivilege &val) const
    {
        if (!optionField.HasOption("privilege"))
            return false;
        val = privilege;
        return true;
    }
    void SetDevicePrivilege(const CDevicePrivilege &val)
    {
        privilege = val;
        optionField.SetOption("privilege");
    }
    bool GetDeviceCapPrivilege(vector<TDeviceCapPrivilege> &val) const
    {
        if (!optionField.HasOption("capPrivileges"))
            return false;
        val = capPrivileges;
        return true;
    }
    void SetDeviceCapPrivilege(const vector<TDeviceCapPrivilege> &val)
    {
        capPrivileges = val;
        optionField.SetOption("capPrivileges");
    }
private:
    CModOptionField optionField;
    UUIDString groupID;
    UUIDString deviceID;
    CDevicePrivilege privilege;
    vector<TDeviceCapPrivilege> capPrivileges;  // 注意，此处不能对能力索引划归关系进行增删操作
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUserGroupDeviceModRsp: public CEventRsp
{
public:
    CUserGroupDeviceModRsp()
    {
        event = OspExtEventDesc(TAS_CUSER_GROUP_DEVICE_MOD_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUserGroupDeviceModNtfReq: public CEventReq
{
public:
    CUserGroupDeviceModNtfReq()
    {
        event = OspExtEventDesc(TAS_CUSER_GROUP_DEVICE_MOD_NTF_REQ);
    }

    CUserGroupDeviceInfo& GetUserGroupDeviceInfo() 
    {
        return info;
    }
    const CUserGroupDeviceInfo& GetUserGroupDeviceInfo() const
    {
        return info;
    }
    void SetUserGroupDeviceInfo(const CUserGroupDeviceInfo &val)
    {
        info = val;
    }
private:
    CUserGroupDeviceInfo info;

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUserGroupDeviceQryReq: public CQueryReqBase
{
public:
    CUserGroupDeviceQryReq()
    {
        event = OspExtEventDesc(TAS_CUSER_GROUP_DEVICE_QRY_REQ);
    }

    CUserGroupDeviceInfoQueryCondition& GetUserGroupDeviceInfoQueryCondition() 
    {
        return condition;
    }
    const CUserGroupDeviceInfoQueryCondition& GetUserGroupDeviceInfoQueryCondition() const
    {
        return condition;
    }
    void SetUserGroupDeviceInfoQueryCondition(const CUserGroupDeviceInfoQueryCondition &val)
    {
        condition = val;
    }
private:
    CUserGroupDeviceInfoQueryCondition condition;

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUserGroupDeviceQryRsp: public CQueryRspBase
{
public:
    CUserGroupDeviceQryRsp()
    {
        event = OspExtEventDesc(TAS_CUSER_GROUP_DEVICE_QRY_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUserGroupDeviceQryNtfReq: public CQueryNtfReqBase
{
public:
    CUserGroupDeviceQryNtfReq()
    {
        event = OspExtEventDesc(TAS_CUSER_GROUP_DEVICE_QRY_NTF_REQ);
    }

    vector<CUserGroupDeviceInfo>& GetUserGroupDeviceInfo() 
    {
        return infos;
    }
    const vector<CUserGroupDeviceInfo>& GetUserGroupDeviceInfo() const
    {
        return infos;
    }
    void SetUserGroupDeviceInfo(const vector<CUserGroupDeviceInfo> &val)
    {
        infos = val;
    }
private:
    vector<CUserGroupDeviceInfo> infos;

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CLicenseSetReq: public CEventReq
{
public:
    CLicenseSetReq()
    {
        event = OspExtEventDesc(TAS_LICENSE_SET_REQ); 
        authenticationMethod = 0;
        serverPort = 0;
    }
    void SetAuthenticationMethod(int dwAuthenticationMethod)
    {
        authenticationMethod = dwAuthenticationMethod;
    }
    int GetAuthenticationMethod() const
    {
        return authenticationMethod;
    }
    void SetLicenseServerIpAddr(const string& strLicenseServerIpAddr)
    {
        serverIpAddr = strLicenseServerIpAddr;
    }
    const string& GetLicenseServerIpAddr() const
    {
        return serverIpAddr;
    }
    string& GetLicenseServerIpAddr()
    {
        return serverIpAddr;
    }

    void SetLicenseServerPort(int dwLicenseServerPort)
    {
        serverPort = dwLicenseServerPort;
    }
    int GetLicenseServerPort()
    {
        return serverPort;
    }

    void SetLicenseServerLicenseId(const string& strLicenseServerLicenseId)
    {
        serverLicenseId = strLicenseServerLicenseId;
    }
    const string& GetLicenseServerLicenseId() const
    {
        return serverLicenseId;
    }
    string& GetLicenseServerLicenseId()
    {
        return serverLicenseId;
    }

private:
    int authenticationMethod;  //必填，参照CLicenseServerMngTask里，0为local,1为net
    string serverIpAddr;         //为0时不填，为1时必填
    int serverPort;             //为0是不填，为1时必填
    string serverLicenseId;      //为0时不填，为1时选填
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CLicenseSetRsp: public CEventRsp
{
public:
	CLicenseSetRsp()
	{
		event = OspExtEventDesc(TAS_LICENSE_SET_RSP);
	}

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CLicenseUpdateReq: public CEventReq
{
public:
    CLicenseUpdateReq()
    {
        event = OspExtEventDesc(TAS_LICENSE_UPDATE_REQ);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CLicenseUpdateRsp: public CEventRsp
{
public:
    CLicenseUpdateRsp()
    {
        event = OspExtEventDesc(TAS_LICENSE_UPDATE_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CLicenseUpdateNtfReq: public CEventReq
{
public:
    CLicenseUpdateNtfReq()
    {
        event = OspExtEventDesc(TAS_LICENSE_UPDATE_NTF_REQ);
    }
	
private:
    int otherStorageMaxSize; // 最大可管理外厂商存储空间, 单位:T 
	
public:
    void SetOtherStorageMaxSize(int nOtherStorageMaxSize)
    {
        otherStorageMaxSize = nOtherStorageMaxSize;
    }
    int GetOtherStorageMaxSize() const
    {
        return otherStorageMaxSize;
    }
	
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
class CLicenseGetReq: public CEventReq
{
public:
    CLicenseGetReq()
    {
        event = OspExtEventDesc(TAS_LICENSE_GET_REQ);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CLicenseGetRsp: public CEventRsp
{
public:
    CLicenseGetRsp()
    {
        event = OspExtEventDesc(TAS_LICENSE_GET_RSP);
        totalPUMaxNum = 0;
        totalNRUMaxNum = 0;
        totalVideoMaxNum = 0;
        totalRecMaxNum = 0;
        userID = 0;
        usedPUNum = 0;
        usedRecNum = 0;
        otherManufactureNum = 0;
        authenticationMethod = 0;
        serverPort = 0;
        serverConnectState=0;
		otherStorageUsedSize=0;
        needValidateManageLicense = false;
        managePUMaxNum=0;                  
        otherStorageMaxSize=0;
        usedManagePUNum=0;
    }
    void Clear()
    {
        totalPUMaxNum = 0;
        totalNRUMaxNum = 0;
        totalVideoMaxNum = 0;
        totalRecMaxNum = 0;
        userID = 0;
        usedPUNum = 0;
        usedRecNum = 0;
        otherManufactureNum = 0;
        authenticationMethod = 0;
        serverPort = 0;
        serverConnectState=0;
		otherStorageUsedSize=0;
		
        needValidateManageLicense = false;
        managePUMaxNum=0;                  
        otherStorageMaxSize=0;
        usedManagePUNum=0;
        otherManufacturNames.clear();
        otherManufactureLicenseNums.clear();
        productId.clear();
        licenseId.clear();
        hostId.clear();
        expireDate.clear();
        licenseType.clear();
        licenseVer.clear();
        serverIpAddr.clear();
        serverLicenseId.clear();
    }

    int GetTotalPUMaxNum() const 
    {
        return totalPUMaxNum;
    }
    void SetTotalPUMaxNum(int val)
    {
        totalPUMaxNum = val;
    }
    int GetTotalNRUMaxNum() const 
    {
        return totalNRUMaxNum;
    }
    void SetTotalNRUMaxNum(int val)
    {
        totalNRUMaxNum = val;
    }
    int GetTotalVideoMaxNum() const 
    {
        return totalVideoMaxNum;
    }
    void SetTotalVideoMaxNum(int val)
    {
        totalVideoMaxNum = val;
    }
    int GetTotalRecMaxNum() const
    {
        return totalRecMaxNum;
    }
    void SetTotalRecMaxNum(int val)
    {
        totalRecMaxNum = val;
    }
    int GetUserID() const 
    {
        return userID;
    }
    void SetUserID(int val)
    {
        userID = val;
    }
    string& GetExpireDate()
    {
        return expireDate;
    }
    const string& GetExpireDate() const
    {
        return expireDate;
    }
    void SetExpireDate(const string &val)
    {
        expireDate = val;
    }
    string& GetProductId() 
    {
        return productId;
    }
    const string& GetProductId() const
    {
        return productId;
    }
    void SetProductId(const string &val)
    {
        productId = val;
    }
    string& GetLicenseId() 
    {
        return licenseId;
    }
    const string& GetLicenseId() const
    {
        return licenseId;
    }
    void SetLicenseId(const string &val)
    {
        licenseId = val;
    }
    string& GetHostId() 
    {
        return hostId;
    }
    const string& GetHostId() const
    {
        return hostId;
    }
    void SetHostId(const string &val)
    {
        hostId = val;
    }
    int GetUsedPUNum() const 
    {
        return usedPUNum;
    }
    void SetUsedPUNum(int val)
    {
        usedPUNum = val;
    }
    int GetUsedRecNum() const
    {
        return usedRecNum;
    }
    void SetUsedRecNum(int val)
    {
        usedRecNum = val;
    }
    int GetOtherManufactureNum() const 
    {
        return otherManufactureNum;
    }
    void SetOtherManufactureNum(int val)
    {
        otherManufactureNum = val;
    }
    vector<string>& GetOtherManufacturName() 
    {
        return otherManufacturNames;
    }
    const vector<string>& GetOtherManufacturName() const
    {
        return otherManufacturNames;
    }
    void SetOtherManufacturName(const vector<string> &val)
    {
        otherManufacturNames = val;
    }
    vector<int>& GetOtherManufactureLicenseNum() 
    {
        return otherManufactureLicenseNums;
    }
    const vector<int>& GetOtherManufactureLicenseNum() const
    {
        return otherManufactureLicenseNums;
    }
    void SetOtherManufactureLicenseNum(const vector<int> &val)
    {
        otherManufactureLicenseNums = val;
    }
    void SetLicenseType(const string& strLicenseType)
    {
        licenseType = strLicenseType;
    }
    string& GetLicenseType()
    {
        return licenseType;
    }
    const string& GetLicenseType() const
    {
        return licenseType;
    }

    void SetLicenseVer(const string& strLicenseVer)
    {
        licenseVer = strLicenseVer;
    }
    string& GetLicenseVer()
    {
        return licenseVer;
    }
    const string& GetLicenseVer() const
    {
        return licenseVer;
    }

    void  SetOtherStorageUsedSize(int dwOtherStorageUsedSize)
	{
		otherStorageUsedSize = dwOtherStorageUsedSize;
	}
	int GetOtherStorageUsedSize() const
	{
		return otherStorageUsedSize;
	}
	
    void SetAuthenticationMethod(int dwAuthenticationMethod)
    {
        authenticationMethod = dwAuthenticationMethod;
    }
    int GetAuthenticationMethod() const
    {
        return authenticationMethod;
    }

    void SetServerConnectState(int dwServerConnectState)
    {
        serverConnectState = dwServerConnectState;
    }
    int GetServerConnectState() const
    {
        return serverConnectState;
    }
    void SetLicenseServerIpAddr(const string& strLicenseServerIpAddr)
    {
        serverIpAddr = strLicenseServerIpAddr;
    }
    const string& GetLicenseServerIpAddr() const
    {
        return serverIpAddr;
    }
    string& GetLicenseServerIpAddr()
    {
        return serverIpAddr;
    }
    void SetLicenseServerPort(int dwLicenseServerPort)
    {
        serverPort = dwLicenseServerPort;
    }
    int GetLicenseServerPort()
    {
        return serverPort;
    }

    void SetLicenseServerLicenseId(const string& strLicenseServerLicenseId)
    {
        serverLicenseId = strLicenseServerLicenseId;
    }
    const string& GetLicenseServerLicenseId() const
    {
        return serverLicenseId;
    }
    string& GetLicenseServerLicenseId()
    {
        return serverLicenseId;
    }

private:
    int totalPUMaxNum;       // 本地3AS授权的PU的最大允许个数
    int totalNRUMaxNum;      // 本地3AS授权的NRU的最大允许个数
    int totalVideoMaxNum;    // 本地3AS授权的视频浏览并发路数的最大允许个数
    int totalRecMaxNum;      // 本地3AS授权的录像并发路数的最大允许个数
    int otherManufactureNum; // 外厂商限制数目
    vector<string> otherManufacturNames;     // 外厂商名称数组
    vector<int> otherManufactureLicenseNums; // 外厂商license数目限制

    string productId;     // USKEY的产品编号
    string licenseId;     // USKEY的许可证编码字串
    string hostId;        // 主机编号，也即机器码
    int userID;           // 使用Key的用户ID
    string expireDate;    // 使用Key的截止日期

    string licenseType;   // license类型， "UsbKey"或者"FileKey"或“NetKey”
    string licenseVer;    // license版本

    int usedPUNum;  // 已接入的终端数
    int usedRecNum; // 已使用的并发录像数

    bool needValidateManageLicense; // 需要校验管理license                
    int managePUMaxNum;      // 最大可管理监控点数                
    int otherStorageMaxSize; // 最大可管理外厂商存储空间, 单位:T
    int usedManagePUNum;     // 已管理的监控点数                

    int otherStorageUsedSize; //单位：T
	
    int authenticationMethod;   //必填，获取授权方式，参照CLicenseServerMngTask里，0为local,1为net
    string serverIpAddr;           //license服务器地址
    int serverPort;                  //license服务器端口
    string serverLicenseId;       //licenseId
    int serverConnectState;    //license服务器连接状态，1为连接失败，2为连接成功正在获取授权，3为获取授权成功

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetManagePUMaxNum(int nManagePUMaxNum)
    {
        managePUMaxNum = nManagePUMaxNum;
    }
    int GetManagePUMaxNum() const
    {
        return managePUMaxNum;
    }
	
    void SetOtherStorageMaxSize(int nOtherStorageMaxSize)
    {
        otherStorageMaxSize = nOtherStorageMaxSize;
    }
    int GetOtherStorageMaxSize() const
    {
        return otherStorageMaxSize;
    }

public:
    void SetNeedValidateManageLicense(bool bNeedValidateManageLicense)
    {
        needValidateManageLicense = bNeedValidateManageLicense;
    }
    bool GetNeedValidateManageLicense() const
    {
        return needValidateManageLicense;
    }

public:
    void SetUsedManagePUNum(int nUsedManagePUNum)
    {
        usedManagePUNum = nUsedManagePUNum;
    }
    int GetUsedManagePUNum() const
    {
        return usedManagePUNum;
    }
};

class CDomainRelationQryReq: public CQueryReqBase
{
public:
    CDomainRelationQryReq()
    {
        event = OspExtEventDesc(TAS_DOMAIN_RELATION_QRY_REQ);
    }

    void SetCondition(const CDomainRelationQueryCondition& cCondition)
    {
        condition = cCondition;
    }
    CDomainRelationQueryCondition& GetCondition()
    {
        return condition;
    }
    const CDomainRelationQueryCondition& GetCondition() const
    {
        return condition;
    }
private:
    CDomainRelationQueryCondition condition;              
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDomainRelationQryRsp: public CQueryRspBase
{
public:
    CDomainRelationQryRsp()
    {
        event = OspExtEventDesc(TAS_DOMAIN_RELATION_QRY_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDomainRelationQryNtfReq: public CQueryNtfReqBase
{
public:
    CDomainRelationQryNtfReq()
    {
        event = OspExtEventDesc(TAS_DOMAIN_RELATION_QRY_NTF_REQ);
    }

    vector<CDomainRelationInfo>& GetDomainRelationInfo()
    {
        return infos;
    }
    const vector<CDomainRelationInfo>& GetDomainRelationInfo() const
    {
        return infos;
    }
    void SetDomainRelationInfo(const vector<CDomainRelationInfo> &val)
    {
        infos = val;
    }
private:
    vector<CDomainRelationInfo> infos;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDomainRelationAddNtfReq: public CEventReq
{
public:
    CDomainRelationAddNtfReq()
    {
        event = OspExtEventDesc(TAS_DOMAIN_RELATION_ADD_NTF_REQ);
    }

    CDomainRelationInfo& GetDomainRelation() 
    {
        return info;
    }
    const CDomainRelationInfo& GetDomainRelation() const
    {
        return info;
    }
    void SetDomainRelation(const CDomainRelationInfo &val)
    {
        info = val;
    }
private:
    CDomainRelationInfo info;

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDomainRelationDelNtfReq: public CEventReq
{
public:
    CDomainRelationDelNtfReq()
    {
        event = OspExtEventDesc(TAS_DOMAIN_RELATION_DEL_NTF_REQ);
    }

    UUIDString& GetDomainId()
    {
        return domainId;
    }
    const UUIDString& GetDomainId() const
    {
        return domainId;
    }
    void SetDomainId(const UUIDString &val)
    {
        domainId = val;
    }
private:
    UUIDString domainId;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDomainRelationModNtfReq: public CEventReq
{
public:
    CDomainRelationModNtfReq()
    {
        event = OspExtEventDesc(TAS_DOMAIN_RELATION_MOD_NTF_REQ);
    }

    CDomainRelationInfo& GetDomainRelation()
    {
        return info;
    }
    const CDomainRelationInfo& GetDomainRelation() const
    {
        return info;
    }
    void SetDomainRelation(const CDomainRelationInfo &val)
    {
        info = val;
    }
private:
    CDomainRelationInfo info;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDomainNameGetReq: public CEventReq
{
public:
    CDomainNameGetReq()
    {
        event = OspExtEventDesc(TAS_DOMAIN_NAME_GET_REQ);
    }

    UUIDString& GetDomainId()
    {
        return domainId;
    }
    const UUIDString& GetDomainId() const
    {
        return domainId;
    }
    void SetDomainId(const UUIDString &val)
    {
        domainId = val;
    }
private:
    UUIDString domainId;    //为空则查3AS所在域，不为空则查指定域
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDomainNameGetRsp: public CEventRsp
{
public:
    CDomainNameGetRsp()
    {
        event = OspExtEventDesc(TAS_DOMAIN_NAME_GET_RSP);
    }

    UUIDString& GetDomainId()
    {
        return domainId;
    }
    const UUIDString& GetDomainId() const
    {
        return domainId;
    }
    void SetDomainId(const UUIDString &val)
    {
        domainId = val;
    }
    UUIDString& GetDomainName()
    {
        return domainName;
    }
    const UUIDString& GetDomainName() const
    {
        return domainName;
    }
    void SetDomainName(const UUIDString &val)
    {
        domainName = val;
    }
private:
    UUIDString domainId;
    UUIDString domainName;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CGatewayDelReq : public CEventReq
{
public:
	CGatewayDelReq()
	{
		event = OspExtEventDesc(TAS_GATEWAY_DEL_REQ);
	}

	string& GetUri()
	{
		return uri;
	}
	const string& GetUri() const
	{
		return uri;
	}
	void SetUri(const string &val)
	{
		uri = val;
	}
private:
	string uri;
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
protected:
	void ToXml(TiXmlDocument& xmlDoc)const;
public:
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class CGatewayDelRsp : public CEventRsp
{
public:
	CGatewayDelRsp()
	{
		event = OspExtEventDesc(TAS_GATEWAY_DEL_RSP);
	}

	string& GetUri()
	{
		return uri;
	}
	const string& GetUri() const
	{
		return uri;
	}
	void SetUri(const string &val)
	{
		uri = val;
	}
private:
	string uri;

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
protected:
	void ToXml(TiXmlDocument& xmlDoc)const;
public:
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class CUserDeviceGroupAddReq : public CEventReq
{
public:
	CUserDeviceGroupAddReq()
	{
		event = OspExtEventDesc(TAS_CUSER_DEVICE_GROUP_ADD_REQ);
        assignPriv = false;
	}

	void SetOperId(const UUIDString &val)
	{
		operId = val;
	}
	const UUIDString &GetOperId() const
	{
		return operId;
	}
	UUIDString &GetOperId()
	{
		return operId;
	}

	void SetUserId(const UUIDString &val)
	{
		userId = val;
	}
	const UUIDString &GetUserId() const
	{
		return userId;
	}
	UUIDString &GetUserId()
	{
		return userId;
	}

	void SetDevGrpId(const UUIDString &val)
	{
		devGrpId = val;
	}
	const UUIDString &GetDevGrpId() const
	{
		return devGrpId;
	}
	UUIDString &GetDevGrpId()
	{
		return devGrpId;
	}

private:
	UUIDString operId;					// 操作者用户id
	UUIDString userId;					// 用户id
	UUIDString devGrpId;				// 设备组id
    bool assignPriv;                    // 划归时使用指定的设备和视频源权限                
    CEncoderPrivilege devicePrivilege;                 
    CVideosourcePrivilege deviceCapPrivilege;                

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetAssignPriv(bool bAssignPriv)
    {
        assignPriv = bAssignPriv;
    }
    bool GetAssignPriv() const
    {
        return assignPriv;
    }

    void SetDevicePrivilege(const CEncoderPrivilege& cDevicePrivilege)
    {
        devicePrivilege = cDevicePrivilege;
    }
    CEncoderPrivilege& GetDevicePrivilege()
    {
        return devicePrivilege;
    }
    const CEncoderPrivilege& GetDevicePrivilege() const
    {
        return devicePrivilege;
    }

    void SetDeviceCapPrivilege(const CVideosourcePrivilege& cDeviceCapPrivilege)
    {
        deviceCapPrivilege = cDeviceCapPrivilege;
    }
    CVideosourcePrivilege& GetDeviceCapPrivilege()
    {
        return deviceCapPrivilege;
    }
    const CVideosourcePrivilege& GetDeviceCapPrivilege() const
    {
        return deviceCapPrivilege;
    }

};

class CUserDeviceGroupAddRsp : public CEventRsp
{
public:
	CUserDeviceGroupAddRsp()
	{
		event = OspExtEventDesc(TAS_CUSER_DEVICE_GROUP_ADD_RSP);
	}

	void SetOperUuid(const UUIDString &val)
	{
		operUuid = val;
	}
	const UUIDString &GetOperUuid() const
	{
		return operUuid;
	}
	UUIDString &GetOperUuid()
	{
		return operUuid;
	}

private:
	UUIDString operUuid;				// 操作Id，标志唯一的一次操作，用于后续查询进度
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUserDeviceGroupDelReq : public CEventReq
{
public:
	CUserDeviceGroupDelReq()
	{
		event = OspExtEventDesc(TAS_CUSER_DEVICE_GROUP_DEL_REQ);
	}

	void SetOperId(const UUIDString &val)
	{
		operId = val;
	}
	const UUIDString &GetOperId() const
	{
		return operId;
	}
	UUIDString &GetOperId()
	{
		return operId;
	}

	void SetUserId(const UUIDString &val)
	{
		userId = val;
	}
	const UUIDString &GetUserId() const
	{
		return userId;
	}
	UUIDString &GetUserId()
	{
		return userId;
	}

	void SetDevGrpId(const UUIDString &val)
	{
		devGrpId = val;
	}
	const UUIDString &GetDevGrpId() const
	{
		return devGrpId;
	}
	UUIDString &GetDevGrpId()
	{
		return devGrpId;
	}

private:
	UUIDString operId;					// 操作者用户id
	UUIDString userId;					// 用户id
	UUIDString devGrpId;				// 设备组id
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUserDeviceGroupDelRsp : public CEventRsp
{
public:
	CUserDeviceGroupDelRsp()
	{
		event = OspExtEventDesc(TAS_CUSER_DEVICE_GROUP_DEL_RSP);
	}

	void SetOperUuid(const UUIDString &val)
	{
		operUuid = val;
	}
	const UUIDString &GetOperUuid() const
	{
		return operUuid;
	}
	UUIDString &GetOperUuid()
	{
		return operUuid;
	}

private:
	UUIDString operUuid;				// 操作Id，标志唯一的一次操作，用于后续查询进度
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUserGroupDeviceGroupAddReq : public CEventReq
{
public:
	CUserGroupDeviceGroupAddReq()
	{
		event = OspExtEventDesc(TAS_CUSER_GROUP_DEVICE_GROUP_ADD_REQ);
        assignPriv = false;
	}

	void SetOperId(const UUIDString &val)
	{
		operId = val;
	}
	const UUIDString &GetOperId() const
	{
		return operId;
	}
	UUIDString &GetOperId()
	{
		return operId;
	}

	void SetUserGroupId(const UUIDString &val)
	{
		userGroupId = val;
	}
	const UUIDString &GetUserGroupId() const
	{
		return userGroupId;
	}
	UUIDString &GetUserGroupId()
	{
		return userGroupId;
	}

	void SetDevGrpId(const UUIDString &val)
	{
		devGrpId = val;
	}
	const UUIDString &GetDevGrpId() const
	{
		return devGrpId;
	}
	UUIDString &GetDevGrpId()
	{
		return devGrpId;
	}
	
private:
	UUIDString operId;					// 操作者用户id
	UUIDString userGroupId;				// 用户组id
	UUIDString devGrpId;				// 设备组id
    bool assignPriv;                    // 划归时使用指定的设备和视频源权限                
    CEncoderPrivilege devicePrivilege;                 
    CVideosourcePrivilege deviceCapPrivilege;                

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetAssignPriv(bool bAssignPriv)
    {
        assignPriv = bAssignPriv;
    }
    bool GetAssignPriv() const
    {
        return assignPriv;
    }

    void SetDevicePrivilege(const CEncoderPrivilege& cDevicePrivilege)
    {
        devicePrivilege = cDevicePrivilege;
    }
    CEncoderPrivilege& GetDevicePrivilege()
    {
        return devicePrivilege;
    }
    const CEncoderPrivilege& GetDevicePrivilege() const
    {
        return devicePrivilege;
    }

    void SetDeviceCapPrivilege(const CVideosourcePrivilege& cDeviceCapPrivilege)
    {
        deviceCapPrivilege = cDeviceCapPrivilege;
    }
    CVideosourcePrivilege& GetDeviceCapPrivilege()
    {
        return deviceCapPrivilege;
    }
    const CVideosourcePrivilege& GetDeviceCapPrivilege() const
    {
        return deviceCapPrivilege;
    }

};

class CUserGroupDeviceGroupAddRsp : public CEventRsp
{
public:
	CUserGroupDeviceGroupAddRsp()
	{
		event = OspExtEventDesc(TAS_CUSER_GROUP_DEVICE_GROUP_ADD_RSP);
	}

	void SetOperUuid(const UUIDString &val)
	{
		operUuid = val;
	}
	const UUIDString &GetOperUuid() const
	{
		return operUuid;
	}
	UUIDString &GetOperUuid()
	{
		return operUuid;
	}

private:
	UUIDString operUuid;				// 操作Id，标志唯一的一次操作，用于后续查询进度
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUserGroupDeviceGroupDelReq : public CEventReq
{
public:
	CUserGroupDeviceGroupDelReq()
	{
		event = OspExtEventDesc(TAS_CUSER_GROUP_DEVICE_GROUP_DEL_REQ);
	}

	void SetOperId(const UUIDString &val)
	{
		operId = val;
	}
	const UUIDString &GetOperId() const
	{
		return operId;
	}
	UUIDString &GetOperId()
	{
		return operId;
	}

	void SetUserGroupId(const UUIDString &val)
	{
		userGroupId = val;
	}
	const UUIDString &GetUserGroupId() const
	{
		return userGroupId;
	}
	UUIDString &GetUserGroupId()
	{
		return userGroupId;
	}

	void SetDevGrpId(const UUIDString &val)
	{
		devGrpId = val;
	}
	const UUIDString &GetDevGrpId() const
	{
		return devGrpId;
	}
	UUIDString &GetDevGrpId()
	{
		return devGrpId;
	}

private:
	UUIDString operId;					// 操作者用户id
	UUIDString userGroupId;				// 用户组id
	UUIDString devGrpId;				// 设备组id
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUserGroupDeviceGroupDelRsp : public CEventRsp
{
public:
	CUserGroupDeviceGroupDelRsp()
	{
		event = OspExtEventDesc(TAS_CUSER_GROUP_DEVICE_GROUP_DEL_RSP);
	}

	void SetOperUuid(const UUIDString &val)
	{
		operUuid = val;
	}
	const UUIDString &GetOperUuid() const
	{
		return operUuid;
	}
	UUIDString &GetOperUuid()
	{
		return operUuid;
	}

private:
	UUIDString operUuid;				// 操作Id，标志唯一的一次操作，用于后续查询进度
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDeviceAutoAssignToUserSetReq : public CEventReq
{
public:
	CDeviceAutoAssignToUserSetReq()
	{
		event = OspExtEventDesc(TAS_DEVICE_AUTO_ASSIGN_TO_USER_SET_REQ);

		autoAssign = false;
	}

	void SetOperId(const UUIDString &val)
	{
		operId = val;
	}
	const UUIDString &GetOperId() const
	{
		return operId;
	}
	UUIDString &GetOperId()
	{
		return operId;
	}

	void SetDevGrpId(const UUIDString &val)
	{
		devGrpId = val;
	}
	const UUIDString &GetDevGrpId() const
	{
		return devGrpId;
	}
	UUIDString &GetDevGrpId()
	{
		return devGrpId;
	}

	void SetAutoAssign(bool val)
	{
		autoAssign = val;
	}
	bool GetAutoAssign() const
	{
		return autoAssign;
	}
	bool &GetAutoAssign()
	{
		return autoAssign;
	}

	void SetUserId(const UUIDString &val)
	{
		userId = val;
	}
	const UUIDString &GetUserId() const
	{
		return userId;
	}
	UUIDString &GetUserId()
	{
		return userId;
	}
private:
	UUIDString operId;				// 操作者用户id
	UUIDString devGrpId;			// 设备组Id
	bool       autoAssign;			// 是否开启自动划归
	UUIDString userId;				// userId指定划归给指定的用户
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDeviceAutoAssignToUserSetRsp : public CEventRsp
{
public:
	CDeviceAutoAssignToUserSetRsp()
	{
		event = OspExtEventDesc(TAS_DEVICE_AUTO_ASSIGN_TO_USER_SET_RSP);
	}

	void SetOperUuid(const UUIDString &val)
	{
		operUuid = val;
	}
	const UUIDString &GetOperUuid() const
	{
		return operUuid;
	}
	UUIDString &GetOperUuid()
	{
		return operUuid;
	}

private:
	UUIDString operUuid;				// 操作Id，标志唯一的一次操作，用于后续查询进度
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDeviceAutoAssignToUserQryReq : public CQueryReqBase
{
public:
	CDeviceAutoAssignToUserQryReq()
	{
		event = OspExtEventDesc(TAS_DEVICE_AUTO_ASSIGN_TO_USER_QRY_REQ);
	}

	void SetUserId(const UUIDString &val)
	{
		userId = val;
	}
	const UUIDString &GetUserId() const
	{
		return userId;
	}
	UUIDString &GetUserId()
	{
		return userId;
	}

private:
	UUIDString userId;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDeviceAutoAssignToUserQryRsp : public CQueryRspBase
{
public:
	CDeviceAutoAssignToUserQryRsp()
	{
		event = OspExtEventDesc(TAS_DEVICE_AUTO_ASSIGN_TO_USER_QRY_RSP);
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDeviceAutoAssignToUserQryNtfReq: public CQueryNtfReqBase
{
public:
	CDeviceAutoAssignToUserQryNtfReq()
	{
		event = OspExtEventDesc(TAS_DEVICE_AUTO_ASSIGN_TO_USER_QRY_NTF_REQ);
	}

	vector<CDeviceAutoAssignInfo>& GetDeviceAutoAssignInfo() 
	{
		return infos;
	}
	const vector<CDeviceAutoAssignInfo>& GetDeviceAutoAssignInfo() const
	{
		return infos;
	}
	void SetDeviceAutoAssignInfo(const vector<CDeviceAutoAssignInfo> &val)
	{
		infos = val;
	}

private:
	vector<CDeviceAutoAssignInfo> infos;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDeviceAutoAssignToUserQryNtfRsp : public CEventRsp
{
public:
	CDeviceAutoAssignToUserQryNtfRsp()
	{
		event = OspExtEventDesc(TAS_DEVICE_AUTO_ASSIGN_TO_USER_QRY_NTF_RSP);
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDeviceAutoAssignToUserGroupSetReq : public CEventReq
{
public:
	CDeviceAutoAssignToUserGroupSetReq()
	{
		event = OspExtEventDesc(TAS_DEVICE_AUTO_ASSIGN_TO_USER_GROUP_SET_REQ);

		autoAssign = false;
	}

	void SetOperId(const UUIDString &val)
	{
		operId = val;
	}
	const UUIDString &GetOperId() const
	{
		return operId;
	}
	UUIDString &GetOperId()
	{
		return operId;
	}

	void SetDevGrpId(const UUIDString &val)
	{
		devGrpId = val;
	}
	const UUIDString &GetDevGrpId() const
	{
		return devGrpId;
	}
	UUIDString &GetDevGrpId()
	{
		return devGrpId;
	}

	void SetAutoAssign(bool val)
	{
		autoAssign = val;
	}
	bool GetAutoAssign() const
	{
		return autoAssign;
	}
	bool &GetAutoAssign()
	{
		return autoAssign;
	}

	void SetUserGroupId(const UUIDString &val)
	{
		userGroupId = val;
	}
	const UUIDString &GetUserGroupId() const
	{
		return userGroupId;
	}
	UUIDString &GetUserGroupId()
	{
		return userGroupId;
	}

private:
	UUIDString operId;				// 操作者用户id
	UUIDString devGrpId;			// 设备组Id
	bool       autoAssign;			// 是否开启自动划归
	UUIDString userGroupId;			// 当autoAssign为true时，userId指定划归给指定的用户组
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDeviceAutoAssignToUserGroupSetRsp : public CEventRsp
{
public:
	CDeviceAutoAssignToUserGroupSetRsp()
	{
		event = OspExtEventDesc(TAS_DEVICE_AUTO_ASSIGN_TO_USER_GROUP_SET_RSP);
	}

	void SetOperUuid(const UUIDString &val)
	{
		operUuid = val;
	}
	const UUIDString &GetOperUuid() const
	{
		return operUuid;
	}
	UUIDString &GetOperUuid()
	{
		return operUuid;
	}

private:
	UUIDString operUuid;				// 操作Id，标志唯一的一次操作，用于后续查询进度
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDeviceAutoAssignToUserGroupQryReq : public CQueryReqBase
{
public:
	CDeviceAutoAssignToUserGroupQryReq()
	{
		event = OspExtEventDesc(TAS_DEVICE_AUTO_ASSIGN_TO_USER_GROUP_QRY_REQ);
	}

	void SetUserGroupId(const UUIDString &val)
	{
		userGroupId = val;
	}
	const UUIDString &GetUserGroupId() const
	{
		return userGroupId;
	}
	UUIDString &GetUserGroupId()
	{
		return userGroupId;
	}

private:
	UUIDString userGroupId;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDeviceAutoAssignToUserGroupQryRsp : public CQueryRspBase
{
public:
	CDeviceAutoAssignToUserGroupQryRsp()
	{
		event = OspExtEventDesc(TAS_DEVICE_AUTO_ASSIGN_TO_USER_GROUP_QRY_RSP);
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDeviceAutoAssignToUserGroupQryNtfReq: public CQueryNtfReqBase
{
public:
	CDeviceAutoAssignToUserGroupQryNtfReq()
	{
		event = OspExtEventDesc(TAS_DEVICE_AUTO_ASSIGN_TO_USER_GROUP_QRY_NTF_REQ);
	}

	vector<CDeviceAutoAssignInfo>& GetDeviceAutoAssignInfo() 
	{
		return infos;
	}
	const vector<CDeviceAutoAssignInfo>& GetDeviceAutoAssignInfo() const
	{
		return infos;
	}
	void SetDeviceAutoAssignInfo(const vector<CDeviceAutoAssignInfo> &val)
	{
		infos = val;
	}

private:
	vector<CDeviceAutoAssignInfo> infos;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDeviceAutoAssignToUserGroupQryNtfRsp : public CEventRsp
{
public:
	CDeviceAutoAssignToUserGroupQryNtfRsp()
	{
		event = OspExtEventDesc(TAS_DEVICE_AUTO_ASSIGN_TO_USER_GROUP_QRY_NTF_RSP);
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CTransactionProgressGetReq : public CEventReq
{
public:
	CTransactionProgressGetReq()
	{
		event = OspExtEventDesc(TAS_TRANSACTION_PROGRESS_GET_REQ);
	}

	void SetOperUuid(const UUIDString &val)
	{
		operUuid = val;
	}
	const UUIDString &GetOperUuid() const
	{
		return operUuid;
	}
	UUIDString &GetOperUuid()
	{
		return operUuid;
	}

private:
	UUIDString operUuid;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CTransactionProgressGetRsp : public CEventRsp
{
public:
	CTransactionProgressGetRsp()
	{
		event = OspExtEventDesc(TAS_TRANSACTION_PROGRESS_GET_RSP);
	}

	void SetProgress(const TTransProgress &val)
	{
		progress = val;
	}
	const TTransProgress &GetProgress() const
	{
		return progress;
	}
	TTransProgress &GetProgress()
	{
		return progress;
	}

private:
	TTransProgress progress;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CConfigTasSetReq: public CEventReq
{
public:
    CConfigTasSetReq()
    {
        event = OspExtEventDesc(TAS_CONFIG_TAS_SET_REQ);
    }
    bool GetDomainName(string &val) const
    {
        if (!optionField.HasOption("domainName"))
            return false;
        val = domainName;
        return true;
    }
    void SetDomainName(const string &val)
    {
        domainName = val;
        optionField.SetOption("domainName");
    }
	bool GetDomainAlias(TDomainExtData &val) const
	{
		if (!optionField.HasOption("domainAlias"))
			return false;
		val = domainAlias;
		return true;
	}
    //mod by liangli 2014.3.21
	void SetDomainAlias(const TDomainExtData &val)
	{
		domainAlias = val;
		optionField.SetOption("domainAlias");
	}
    bool GetParentDomainId(UUIDString &val) const
    {
        if (!optionField.HasOption("parentDomainId"))
            return false;
        val = parentDomainId;
        return true;
    }
    void SetParentDomainId(const UUIDString &val)
    {
        parentDomainId = val;
        optionField.SetOption("parentDomainId");
    }
    bool GetParentDomainName(string &val) const
    {
        if (!optionField.HasOption("parentDomainName"))
            return false;
        val = parentDomainName;
        return true;
    }
    void SetParentDomainName(const string &val)
    {
        parentDomainName = val;
        optionField.SetOption("parentDomainName");
    }
	
private:
    CModOptionField optionField; // 修改字段标识
	string  domainName;          // 本域的域名
	TDomainExtData domainAlias;			// 本域的域别名 mod by liangli 2014.3.21 将domainAlias改为map
	UUIDString parentDomainId;   // 父域的编号
	string  parentDomainName;    // 父域的域名
	
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CConfigTasSetRsp: public CEventRsp
{
public:
    CConfigTasSetRsp()
    {
        event = OspExtEventDesc(TAS_CONFIG_TAS_SET_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CConfigTasGetReq: public CEventReq
{
public:
    CConfigTasGetReq()
    {
        event = OspExtEventDesc(TAS_CONFIG_TAS_GET_REQ);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CConfigTasGetRsp: public CEventRsp
{
public:
    CConfigTasGetRsp()
    {
        event = OspExtEventDesc(TAS_CONFIG_TAS_GET_RSP);
    }

private:
    UUIDString selfDomainId;                  // 本域的编号
    string  domainName;                       // 本域的域名
	TDomainExtData  domainAlias;					  // 本域的域别名 mod by liangli 2014.3.21 将domainAlias改为map
    UUIDString parentDomainId;                // 父域的编号
    string  parentDomainName;                 // 父域的域名
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetSelfDomainId(const UUIDString& strSelfDomainId)
    {
        selfDomainId = strSelfDomainId;
    }
    UUIDString& GetSelfDomainId()
    {
        return selfDomainId;
    }
    const UUIDString& GetSelfDomainId() const
    {
        return selfDomainId;
    }

    void SetDomainName(const string& strDomainName)
    {
        domainName = strDomainName;
    }
    string& GetDomainName()
    {
        return domainName;
    }
    const string& GetDomainName() const
    {
        return domainName;
    }
    //mod by liangli 2014.3.21 将domainAlias改为map
	void SetDomainAlias(const TDomainExtData& tDomainAlias)
	{
		domainAlias = tDomainAlias;
	}
	TDomainExtData& GetDomainAlias()
	{
		return domainAlias;
	}
	const TDomainExtData& GetDomainAlias() const
	{
		return domainAlias;
	}

    void SetParentDomainId(const UUIDString& strParentDomainId)
    {
        parentDomainId = strParentDomainId;
    }
    UUIDString& GetParentDomainId()
    {
        return parentDomainId;
    }
    const UUIDString& GetParentDomainId() const
    {
        return parentDomainId;
    }

    void SetParentDomainName(const string& strParentDomainName)
    {
        parentDomainName = strParentDomainName;
    }
    string& GetParentDomainName()
    {
        return parentDomainName;
    }
    const string& GetParentDomainName() const
    {
        return parentDomainName;
    }

};

class CCuserDataVersionGetReq: public CEventReq
{
public:
    CCuserDataVersionGetReq()
    {
        event = OspExtEventDesc(TAS_CUSER_DATA_VERSION_GET_REQ);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CCuserDataVersionGetRsp: public CEventRsp
{
public:
    CCuserDataVersionGetRsp()
    {
        event = OspExtEventDesc(TAS_CUSER_DATA_VERSION_GET_RSP);
    }

private:
    string dataVersion;                
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetDataVersion(const string& strDataVersion)
    {
        dataVersion = strDataVersion;
    }
    string& GetDataVersion()
    {
        return dataVersion;
    }
    const string& GetDataVersion() const
    {
        return dataVersion;
    }

};

class CChildtasAddReq: public CEventReq
{
public:
    CChildtasAddReq()
    {
        event = OspExtEventDesc(TAS_CHILDTAS_ADD_REQ);
    }
private:
    UUIDString childDomainId;                // 子域的编号
    string  childDomainName;                 // 子域的域名
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetChildDomainId(const UUIDString& strChildDomainId)
    {
        childDomainId = strChildDomainId;
    }
    UUIDString& GetChildDomainId()
    {
        return childDomainId;
    }
    const UUIDString& GetChildDomainId() const
    {
        return childDomainId;
    }

    void SetChildDomainName(const string& strChildDomainName)
    {
        childDomainName = strChildDomainName;
    }
    string& GetChildDomainName()
    {
        return childDomainName;
    }
    const string& GetChildDomainName() const
    {
        return childDomainName;
    }

};

class CChildtasAddRsp: public CEventRsp
{
public:
    CChildtasAddRsp()
    {
        event = OspExtEventDesc(TAS_CHILDTAS_ADD_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CChildtasDelReq: public CEventReq
{
public:
    CChildtasDelReq()
    {
        event = OspExtEventDesc(TAS_CHILDTAS_DEL_REQ);
    }
private:
    UUIDString childDomainId;                // 子域的编号
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetChildDomainId(const UUIDString& strChildDomainId)
    {
        childDomainId = strChildDomainId;
    }
    UUIDString& GetChildDomainId()
    {
        return childDomainId;
    }
    const UUIDString& GetChildDomainId() const
    {
        return childDomainId;
    }

};

class CChildtasDelRsp: public CEventRsp
{
public:
    CChildtasDelRsp()
    {
        event = OspExtEventDesc(TAS_CHILDTAS_DEL_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CCtrlTasReq: public CEventReq
{
public:
    CCtrlTasReq()
    {
        event = OspExtEventDesc(TAS_CTRL_TAS_REQ);
    }
    bool GetReSyncData(bool &val) const
    {
        if (!optionField.HasOption("reSyncData"))
            return false;
        val = reSyncData;
        return true;
    }
    void SetReSyncData(bool val)
    {
        reSyncData = val;
        optionField.SetOption("reSyncData");
    }
private:
    CModOptionField optionField; // 控制字段标识
    bool reSyncData;             // 重做数据同步
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CCtrlTasRsp: public CEventRsp
{
public:
    CCtrlTasRsp()
    {
        event = OspExtEventDesc(TAS_CTRL_TAS_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};




class CTacUserAddReq: public CEventReq
{
public:
    CTacUserAddReq()
    {
        event = OspExtEventDesc(TAS_TACUSER_ADD_REQ);
    }

    CTacUserInfo& GetTasUserInfo() 
    {
        return info;
    }
    const CTacUserInfo& GetTasUserInfo() const
    {
        return info;
    }
    void SetTasUserInfo(const CTacUserInfo &val)
    {
        info = val;
    }
private:
    CTacUserInfo info;

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CTacUserAddRsp: public CEventRsp
{
public:
    CTacUserAddRsp()
    {
        event = OspExtEventDesc(TAS_TACUSER_ADD_RSP);
    }

    CTacUserInfoKey& GetTasUserInfoKey()
    {
        return key;
    }
    const CTacUserInfoKey& GetTasUserInfoKey() const
    {
        return key;
    }
    void SetTasUserInfoKey(const CTacUserInfoKey &val)
    {
        key = val;
    }
private:
    CTacUserInfoKey key;

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CTacUserAddNtfReq: public CEventReq
{
public:
    CTacUserAddNtfReq()
    {
        event = OspExtEventDesc(TAS_TACUSER_ADD_NTF_REQ);
    }

    CTacUserInfo& GetTasUserInfo() 
    {
        return info;
    }
    const CTacUserInfo& GetTasUserInfo() const
    {
        return info;
    }
    void SetTasUserInfo(const CTacUserInfo &val)
    {
        info = val;
    }
private:
    CTacUserInfo info;

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CTacUserDelReq: public CEventReq
{
public:
    CTacUserDelReq()
    {
        event = OspExtEventDesc(TAS_TACUSER_DEL_REQ);
    }

    CTacUserInfoKey& GetTasUserInfoKey()
    {
        return key;
    }
    const CTacUserInfoKey& GetTasUserInfoKey() const
    {
        return key;
    }
    void SetTasUserInfoKey(const CTacUserInfoKey &val)
    {
        key = val;
    }
private:
    CTacUserInfoKey key;

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CTacUserDelRsp: public CEventRsp
{
public:
    CTacUserDelRsp()
    {
        event = OspExtEventDesc(TAS_TACUSER_DEL_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CTacUserDelNtfReq: public CEventReq
{
public:
    CTacUserDelNtfReq()
    {
        event = OspExtEventDesc(TAS_TACUSER_DEL_NTF_REQ);
    }

    CTacUserInfoKey& GetTasUserInfoKey()
    {
        return key;
    }
    const CTacUserInfoKey& GetTasUserInfoKey() const
    {
        return key;
    }
    void SetTasUserInfoKey(const CTacUserInfoKey &val)
    {
        key = val;
    }
private:
    CTacUserInfoKey key;

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CTacUserModReq: public CEventReq
{
public:
    CTacUserModReq()
    {
        event = OspExtEventDesc(TAS_TACUSER_MOD_REQ);
        userType = enTacUserType_SuperAdmin;
        mLoginFlag = true;
        enabledFlag = true;
    }

    bool GetUserID(UUIDString &val) const
    {
        if (!optionField.HasOption("userID"))
            return false;
        val = userID;
        return true;
    }
    void SetUserID(const UUIDString &val)
    {
        userID = val;
        optionField.SetOption("userID");
    }
    bool GetUserName(string &val) const
    {
        if (!optionField.HasOption("userName"))
            return false;
        val = userName;
        return true;
    }
    void SetUserName(const string &val)
    {
        userName = val;
        optionField.SetOption("userName");
    }
    bool GetUserPwd(string &val) const
    {
        if (!optionField.HasOption("userPwd"))
            return false;
        val = userPwd;
        return true;
    }
    void SetUserPwd(const string &val)
    {
        userPwd = val;
        optionField.SetOption("userPwd");
    }
    bool GetUserType(TTacUserType &val) const
    {
        if (!optionField.HasOption("userType"))
            return false;
        val = userType;
        return true;
    }
    void SetUserType(TTacUserType val)
    {
        userType = val;
        optionField.SetOption("userType");
    }
    bool GetMLoginFlag(bool &val) const
    {
        if (!optionField.HasOption("mLoginFlag"))
            return false;
        val = mLoginFlag;
        return true;
    }
    void SetMLoginFlag(bool val)
    {
        mLoginFlag = val;
        optionField.SetOption("mLoginFlag");
    }
    bool GetEnabledFlag(bool &val) const
    {
        if (!optionField.HasOption("enabledFlag"))
            return false;
        val = enabledFlag;
        return true;
    }
    void SetEnabledFlag(bool val)
    {
        enabledFlag = val;
        optionField.SetOption("enabledFlag");
    }
    bool GetDescription(string &val) const
    {
        if (!optionField.HasOption("description"))
            return false;
        val = description;
        return true;
    }
    void SetDescription(const string &val)
    {
        description = val;
        optionField.SetOption("description");
    }
    bool GetNameExpireDate(string &val) const
    {
        if (!optionField.HasOption("nameExpireDate"))
            return false;
        val = nameExpireDate;
        return true;
    }
    void SetNameExpireDate(const string &val)
    {
        nameExpireDate = val;
        optionField.SetOption("nameExpireDate");
    }
    bool GetPwdExpireDate(string &val) const
    {
        if (!optionField.HasOption("pwdExpireDate"))
            return false;
        val = pwdExpireDate;
        return true;
    }
    void SetPwdExpireDate(const string &val)
    {
        pwdExpireDate = val;
        optionField.SetOption("pwdExpireDate");
    }
    bool GetCreateDate(string &val) const
    {
        if (!optionField.HasOption("createDate"))
            return false;
        val = createDate;
        return true;
    }
    void SetCreateDate(const string &val)
    {
        createDate = val;
        optionField.SetOption("createDate");
    }
    bool GetCreatorID(UUIDString &val) const
    {
        if (!optionField.HasOption("creatorID"))
            return false;
        val = creatorID;
        return true;
    }
    void SetCreatorID(const UUIDString &val)
    {
        creatorID = val;
        optionField.SetOption("creatorID");
    }
    bool GetTasUserPrivilege(CTacUserPrivilege &val) const
    {
        if (!optionField.HasOption("privilege"))
            return false;
        val = privilege;
        return true;
    }
    void SetTasUserPrivilege(const CTacUserPrivilege &val)
    {
        privilege = val;
        optionField.SetOption("privilege");
    }
private:
    CModOptionField optionField;
    UUIDString userID; // 用户编号
    string userName; // 用户名
    string userPwd; // 登陆密码, 密文形式
    TTacUserType userType; // 用户类型
    bool mLoginFlag; // 多次登陆标志
    bool enabledFlag; // 启停状态标志
    string description; // 描述
    string nameExpireDate; // 帐户使用截止日期
    string pwdExpireDate; // 密码使用截止日期
    string createDate; // 创建日期
    UUIDString creatorID; // 创建者编号
    CTacUserPrivilege privilege;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CTacUserModRsp: public CEventRsp
{
public:
    CTacUserModRsp()
    {
        event = OspExtEventDesc(TAS_TACUSER_MOD_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CTacUserModNtfReq: public CEventReq
{
public:
    CTacUserModNtfReq()
    {
        event = OspExtEventDesc(TAS_TACUSER_MOD_NTF_REQ);
    }

    CTacUserInfo& GetTasUserInfo() 
    {
        return info;
    }
    const CTacUserInfo& GetTasUserInfo() const
    {
        return info;
    }
    void SetTasUserInfo(const CTacUserInfo &val)
    {
        info = val;
    }
private:
    CTacUserInfo info;

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CTacUserQryReq: public CQueryReqBase
{
public:
    CTacUserQryReq()
    {
        event = OspExtEventDesc(TAS_TACUSER_QRY_REQ);
    }

    CTacUserInfoQueryCondition& GetTasUserInfoQueryCondition()
    {
        return condition;
    }
    const CTacUserInfoQueryCondition& GetTasUserInfoQueryCondition() const
    {
        return condition;
    }
    void SetTasUserInfoQueryCondition(const CTacUserInfoQueryCondition &val)
    {
        condition = val;
    }
private:
    CTacUserInfoQueryCondition condition;

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CTacUserQryRsp: public CQueryRspBase
{
public:
    CTacUserQryRsp()
    {
        event = OspExtEventDesc(TAS_TACUSER_QRY_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CTacUserQryNtfReq: public CQueryNtfReqBase
{
public:
    CTacUserQryNtfReq()
    {
        event = OspExtEventDesc(TAS_TACUSER_QRY_NTF_REQ);
    }

    vector<CTacUserInfo>& GetTasUserInfo()
    {
        return infos;
    }
    const vector<CTacUserInfo>& GetTasUserInfo() const
    {
        return infos;
    }
    void SetTasUserInfo(const vector<CTacUserInfo> &val)
    {
        infos = val;
    }
private:
    vector<CTacUserInfo> infos;

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CTempAuthorizationAddReq: public CEventReq
{
public:
    CTempAuthorizationAddReq()
    {
        event = OspExtEventDesc(TAS_TEMP_AUTHORIZATION_ADD_REQ);
    }

    CTempAuthorizationInfo& GetTempAuthorizationInfo() 
    {
        return info;
    }
    const CTempAuthorizationInfo& GetTempAuthorizationInfo() const
    {
        return info;
    }
    void SetTempAuthorizationInfo(const CTempAuthorizationInfo &val)
    {
        info = val;
    }
private:
    CTempAuthorizationInfo info;

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CTempAuthorizationAddRsp: public CEventRsp
{
public:
    CTempAuthorizationAddRsp()
    {
        event = OspExtEventDesc(TAS_TEMP_AUTHORIZATION_ADD_RSP);
    }

    CTempAuthorizationInfoKey& GetTempAuthorizationInfoKey()
    {
        return key;
    }
    const CTempAuthorizationInfoKey& GetTempAuthorizationInfoKey() const
    {
        return key;
    }
    void SetTempAuthorizationInfoKey(const CTempAuthorizationInfoKey &val)
    {
        key = val;
    }
private:
    CTempAuthorizationInfoKey key;

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CTempAuthorizationAddNtfReq: public CEventReq
{
public:
    CTempAuthorizationAddNtfReq()
    {
        event = OspExtEventDesc(TAS_TEMP_AUTHORIZATION_ADD_NTF_REQ);
    }

    CTempAuthorizationInfo& GetTempAuthorizationInfo()
    {
        return info;
    }
    const CTempAuthorizationInfo& GetTempAuthorizationInfo() const
    {
        return info;
    }
    void SetTempAuthorizationInfo(const CTempAuthorizationInfo &val)
    {
        info = val;
    }
private:
    CTempAuthorizationInfo info;

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CTempAuthorizationDelReq: public CEventReq
{
public:
    CTempAuthorizationDelReq()
    {
        event = OspExtEventDesc(TAS_TEMP_AUTHORIZATION_DEL_REQ);
    }

    CTempAuthorizationInfoKey& GetTempAuthorizationInfoKey()
    {
        return key;
    }
    const CTempAuthorizationInfoKey& GetTempAuthorizationInfoKey() const
    {
        return key;
    }
    void SetTempAuthorizationInfoKey(const CTempAuthorizationInfoKey &val)
    {
        key = val;
    }
private:
    CTempAuthorizationInfoKey key;

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CTempAuthorizationDelRsp: public CEventRsp
{
public:
    CTempAuthorizationDelRsp()
    {
        event = OspExtEventDesc(TAS_TEMP_AUTHORIZATION_DEL_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CTempAuthorizationDelNtfReq: public CEventReq
{
public:
    CTempAuthorizationDelNtfReq()
    {
        event = OspExtEventDesc(TAS_TEMP_AUTHORIZATION_DEL_NTF_REQ);
    }

    CTempAuthorizationInfoKey& GetTempAuthorizationInfoKey()
    {
        return key;
    }
    const CTempAuthorizationInfoKey& GetTempAuthorizationInfoKey() const
    {
        return key;
    }
    void SetTempAuthorizationInfoKey(const CTempAuthorizationInfoKey &val)
    {
        key = val;
    }
private:
    CTempAuthorizationInfoKey key;
    UUIDString devDomainId;   // hzytodo 未实现

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetDevDomainId(const UUIDString& strDevDomainId)
    {
        devDomainId = strDevDomainId;
    }
    UUIDString& GetDevDomainId()
    {
        return devDomainId;
    }
    const UUIDString& GetDevDomainId() const
    {
        return devDomainId;
    }

};

class CTempAuthorizationModReq: public CEventReq
{
public:
    CTempAuthorizationModReq()
    {
        event = OspExtEventDesc(TAS_TEMP_AUTHORIZATION_MOD_REQ);
    }

    bool GetDeviceID(UUIDString &val) const
    {
        if (!optionField.HasOption("deviceID"))
            return false;
        val = deviceID;
        return true;
    }
    void SetDeviceID(const UUIDString &val)
    {
        deviceID = val;
        optionField.SetOption("deviceID");
    }
    bool GetDesc(string &val) const
    {
        if (!optionField.HasOption("desc"))
            return false;
        val = desc;
        return true;
    }
    void SetDesc(const string &val)
    {
        desc = val;
        optionField.SetOption("desc");
    }
private:
    CModOptionField optionField;
    UUIDString deviceID;
    string desc;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CTempAuthorizationModRsp: public CEventRsp
{
public:
    CTempAuthorizationModRsp()
    {
        event = OspExtEventDesc(TAS_TEMP_AUTHORIZATION_MOD_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CTempAuthorizationModNtfReq: public CEventReq
{
public:
    CTempAuthorizationModNtfReq()
    {
        event = OspExtEventDesc(TAS_TEMP_AUTHORIZATION_MOD_NTF_REQ);
    }

    CTempAuthorizationInfo& GetTempAuthorizationInfo()
    {
        return info;
    }
    const CTempAuthorizationInfo& GetTempAuthorizationInfo() const
    {
        return info;
    }
    void SetTempAuthorizationInfo(const CTempAuthorizationInfo &val)
    {
        info = val;
    }
private:
    CTempAuthorizationInfo info;

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CTempAuthorizationQryReq: public CQueryReqBase
{
public:
    CTempAuthorizationQryReq()
    {
        event = OspExtEventDesc(TAS_TEMP_AUTHORIZATION_QRY_REQ);
    }

    CTempAuthorizationInfoQueryCondition& GetTempAuthorizationInfoQueryCondition()
    {
        return condition;
    }
    const CTempAuthorizationInfoQueryCondition& GetTempAuthorizationInfoQueryCondition() const
    {
        return condition;
    }
    void SetTempAuthorizationInfoQueryCondition(const CTempAuthorizationInfoQueryCondition &val)
    {
        condition = val;
    }
private:
    CTempAuthorizationInfoQueryCondition condition;

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CTempAuthorizationQryRsp: public CQueryRspBase
{
public:
    CTempAuthorizationQryRsp()
    {
        event = OspExtEventDesc(TAS_TEMP_AUTHORIZATION_QRY_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CTempAuthorizationQryNtfReq: public CQueryNtfReqBase
{
public:
    CTempAuthorizationQryNtfReq()
    {
        event = OspExtEventDesc(TAS_TEMP_AUTHORIZATION_QRY_NTF_REQ);
    }

    vector<CTempAuthorizationInfo>& GetTempAuthorizationInfo()
    {
        return infos;
    }
    const vector<CTempAuthorizationInfo>& GetTempAuthorizationInfo() const
    {
        return infos;
    }
    void SetTempAuthorizationInfo(const vector<CTempAuthorizationInfo> &val)
    {
        infos = val;
    }
private:
    vector<CTempAuthorizationInfo> infos;

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CSyncTasDataReq: public CEventReq
{
public:
    CSyncTasDataReq()
    {
        event = OspExtEventDesc(TAS_SYNC_TAS_DATA_REQ);
    }

    vector<TSyncDataInfo>& GetSyncDataInfo() 
    {
        return syncDataInfos;
    }
    const vector<TSyncDataInfo>& GetSyncDataInfo() const
    {
        return syncDataInfos;
    }
    void SetSyncDataInfo(const vector<TSyncDataInfo> &val)
    {
        syncDataInfos = val;
    }
private:
    vector<TSyncDataInfo> syncDataInfos;

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CSyncTasDataRsp: public CEventRsp
{// 出现一个失败就整体失败，后续自动尝试重做；具体记录的处理结果只有整体成功时才有意义。
 // 比如跨域授权同步时对端用户被删除之后，可能通知授权方该同步数据无效了。
public:
    CSyncTasDataRsp()
    {
        event = OspExtEventDesc(TAS_SYNC_TAS_DATA_RSP);
    }

    vector<TSyncDataResult>& GetSyncDataResults() 
    {
        return syncDataResults;
    }
    const vector<TSyncDataResult>& GetSyncDataResults() const
    {
        return syncDataResults;
    }
    void SetSyncDataResults(const vector<TSyncDataResult> &val)
    {
        syncDataResults = val;
    }

private:
    vector<TSyncDataResult> syncDataResults;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CCuserDevicefullinfoAndPrivilegeQryReq: public CQueryReqBase
{
public:
    CCuserDevicefullinfoAndPrivilegeQryReq()
    {
        event = OspExtEventDesc(TAS_CUSER_DEVICEFULLINFO_AND_PRIVILEGE_QRY_REQ);
    }

    UUIDString& GetCuserId()
    {
        return cuserId;
    }
    const UUIDString& GetCuserId() const
    {
        return cuserId;
    }
    void SetCuserId(const UUIDString &val)
    {
        cuserId = val;
    }
    CCuserDevicefullinfoAndPrivilegeQueryCondition& GetCondition() 
    {
        return condition;
    }
    const CCuserDevicefullinfoAndPrivilegeQueryCondition& GetCondition() const
    {
        return condition;
    }
    void SetCondition(const CCuserDevicefullinfoAndPrivilegeQueryCondition &val)
    {
        condition = val;
    }
private:
    UUIDString cuserId;
	string     queryHint;                               //查询提示用于分页查询. 为空时, 按数量和序号分页
    CCuserDevicefullinfoAndPrivilegeQueryCondition condition;

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetQueryHint(const string& strQueryHint)
    {
        queryHint = strQueryHint;
    }
    string& GetQueryHint()
    {
        return queryHint;
    }
    const string& GetQueryHint() const
    {
        return queryHint;
    }
};

class CCuserDevicefullinfoAndPrivilegeQryRsp: public CQueryRspBase
{
public:
    CCuserDevicefullinfoAndPrivilegeQryRsp()
    {
        event = OspExtEventDesc(TAS_CUSER_DEVICEFULLINFO_AND_PRIVILEGE_QRY_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CCuserDevicefullinfoAndPrivilegeQryNtfReq: public CQueryNtfReqBase
{
public:
    CCuserDevicefullinfoAndPrivilegeQryNtfReq()
    {
        event = OspExtEventDesc(TAS_CUSER_DEVICEFULLINFO_AND_PRIVILEGE_QRY_NTF_REQ);
    }
    const vector<TDevicefullinfoAndPrivilege>& GetDevicefullinfoAndPrivileges() const
    {
        return devicefullinfoAndPrivileges;
    }
    vector<TDevicefullinfoAndPrivilege>& GetDevicefullinfoAndPrivileges()
    {
        return devicefullinfoAndPrivileges;
    }
	void SetDevicefullinfoAndPrivileges(const vector<TDevicefullinfoAndPrivilege>& val)
	{
		devicefullinfoAndPrivileges = val;
	}
private:
	string     queryHint;                               //查询提示用于分页查询. 为空时, 按数量和序号分页
    vector<TDevicefullinfoAndPrivilege> devicefullinfoAndPrivileges;

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetQueryHint(const string& strQueryHint)
    {
        queryHint = strQueryHint;
    }
    string& GetQueryHint()
    {
        return queryHint;
    }
    const string& GetQueryHint() const
    {
        return queryHint;
    }
};

class CTvwallAddReq: public CEventReq
{
public:
	CTvwallAddReq()
	{
		event = OspExtEventDesc(TAS_TVWALL_ADD_REQ);
	}
private:
	CTvwallInfo tvwallInfo;              

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetTvwallInfo(const CTvwallInfo& cTvwallInfo)
    {
        tvwallInfo = cTvwallInfo;
    }
    CTvwallInfo& GetTvwallInfo()
    {
        return tvwallInfo;
    }
    const CTvwallInfo& GetTvwallInfo() const
    {
        return tvwallInfo;
    }

};

class CTvwallAddRsp: public CEventRsp
{
public:
	CTvwallAddRsp()
	{
		event = OspExtEventDesc(TAS_TVWALL_ADD_RSP);
	}

private:
    UUIDString tvwallId;

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetTvwallId(const UUIDString& strTvwallId)
    {
        tvwallId = strTvwallId;
    }
    UUIDString& GetTvwallId()
    {
        return tvwallId;
    }
    const UUIDString& GetTvwallId() const
    {
        return tvwallId;
    }

};

class CTvwallAddNtfReq: public CEventReq
{
public:
	CTvwallAddNtfReq()
	{
		event = OspExtEventDesc(TAS_TVWALL_ADD_NTF_REQ);
	}
private:
	CTvwallInfo tvwallInfo;              

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetTvwallInfo(const CTvwallInfo& cTvwallInfo)
    {
        tvwallInfo = cTvwallInfo;
    }
    CTvwallInfo& GetTvwallInfo()
    {
        return tvwallInfo;
    }
    const CTvwallInfo& GetTvwallInfo() const
    {
        return tvwallInfo;
    }

};

class CTvwallDelReq: public CEventReq
{
public:
	CTvwallDelReq()
	{
		event = OspExtEventDesc(TAS_TVWALL_DEL_REQ);
	}
private:
	UUIDString tvwallId;              

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetTvwallId(const UUIDString& strTvwallId)
    {
        tvwallId = strTvwallId;
    }
    UUIDString& GetTvwallId()
    {
        return tvwallId;
    }
    const UUIDString& GetTvwallId() const
    {
        return tvwallId;
    }

};

class CTvwallDelRsp: public CEventRsp
{
public:
	CTvwallDelRsp()
	{
		event = OspExtEventDesc(TAS_TVWALL_DEL_RSP);
	}

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CTvwallDelNtfReq: public CEventReq
{
public:
	CTvwallDelNtfReq()
	{
		event = OspExtEventDesc(TAS_TVWALL_DEL_NTF_REQ);
	}
private:
	UUIDString tvwallId;
    UUIDString domainId;

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetTvwallId(const UUIDString& strTvwallId)
    {
        tvwallId = strTvwallId;
    }
    UUIDString& GetTvwallId()
    {
        return tvwallId;
    }
    const UUIDString& GetTvwallId() const
    {
        return tvwallId;
    }

public:
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

};

class CTvwallModReq: public CEventReq
{
public:
	CTvwallModReq()
	{
		event = OspExtEventDesc(TAS_TVWALL_MOD_REQ);
	}
    bool GetId(UUIDString &val) const
    {
        if (!optionField.HasOption("id"))
            return false;
        val = id;
        return true;
    }
    void SetId(const UUIDString &val)
    {
        id = val;
        optionField.SetOption("id");
    }
    bool GetName(string &val) const
    {
        if (!optionField.HasOption("name"))
            return false;
        val = name;
        return true;
    }
    void SetName(const string &val)
    {
        name = val;
        optionField.SetOption("name");
    }
    bool GetTvNum(int &val) const
    {
        if (!optionField.HasOption("tvNum"))
            return false;
        val = tvNum;
        return true;
    }
    void SetTvNum(int val)
    {
        tvNum = val;
        optionField.SetOption("tvNum");
    }
    bool GetStyle(int &val) const
    {
        if (!optionField.HasOption("style"))
            return false;
        val = style;
        return true;
    }
    void SetStyle(int val)
    {
        style = val;
        optionField.SetOption("style");
    }
    bool GetCustomStyleData(string &val) const
    {
        if (!optionField.HasOption("customStyleData"))
            return false;
        val = customStyleData;
        return true;
    }
    void SetCustomStyleData(const string &val)
    {
        customStyleData = val;
        optionField.SetOption("customStyleData");
    }
    bool GetDesc(string &val) const
    {
        if (!optionField.HasOption("desc"))
            return false;
        val = desc;
        return true;
    }
    void SetDesc(const string &val)
    {
        desc = val;
        optionField.SetOption("desc");
    }
    bool GetTvDecoderBinders(vector<TvDecoderBinder> &val) const
    {
        if (!optionField.HasOption("tvDecoderBinders"))
            return false;
        val = tvDecoderBinders;
        return true;
    }
    void SetTvDecoderBinders(const vector<TvDecoderBinder> &val)
    {
        tvDecoderBinders = val;
        optionField.SetOption("tvDecoderBinders");
    }
private:
	CModOptionField optionField;
	UUIDString id;        // 电视墙编号
	string name;          // 电视墙名称
	int tvNum;            // 电视机数目
	int style;            // 电视墙风格, 不使用枚举，因为TAS不关心具体的内容，否则到时风格类型添加时TAS也要改了
	string customStyleData; // 电视墙自定义风格数据
	string desc;          // 电视墙描述
	vector<TvDecoderBinder> tvDecoderBinders; // 电视与解码器绑定关系

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CTvwallModRsp: public CEventRsp
{
public:
	CTvwallModRsp()
	{
		event = OspExtEventDesc(TAS_TVWALL_MOD_RSP);
	}

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CTvwallModNtfReq: public CEventReq
{
public:
	CTvwallModNtfReq()
	{
		event = OspExtEventDesc(TAS_TVWALL_MOD_NTF_REQ);
	}
private:
	CTvwallInfo tvwallInfo;              

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetTvwallInfo(const CTvwallInfo& cTvwallInfo)
    {
        tvwallInfo = cTvwallInfo;
    }
    CTvwallInfo& GetTvwallInfo()
    {
        return tvwallInfo;
    }
    const CTvwallInfo& GetTvwallInfo() const
    {
        return tvwallInfo;
    }

};

class CTvwallQryReq: public CQueryReqBase
{
public:
	CTvwallQryReq()
	{
		event = OspExtEventDesc(TAS_TVWALL_QRY_REQ);
	}
private:
	CTvwallInfoQueryCondition condition;              

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetCondition(const CTvwallInfoQueryCondition& cCondition)
    {
        condition = cCondition;
    }
    CTvwallInfoQueryCondition& GetCondition()
    {
        return condition;
    }
    const CTvwallInfoQueryCondition& GetCondition() const
    {
        return condition;
    }

};

class CTvwallQryRsp: public CQueryRspBase
{
public:
	CTvwallQryRsp()
	{
		event = OspExtEventDesc(TAS_TVWALL_QRY_RSP);
	}

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CTvwallQryNtfReq: public CQueryNtfReqBase
{
public:
	CTvwallQryNtfReq()
	{
		event = OspExtEventDesc(TAS_TVWALL_QRY_NTF_REQ);
	}
private:
	vector<CTvwallInfo> infos;              

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetInfos(const vector<CTvwallInfo>& cInfos)
    {
        infos = cInfos;
    }
    vector<CTvwallInfo>& GetInfos()
    {
        return infos;
    }
    const vector<CTvwallInfo>& GetInfos() const
    {
        return infos;
    }

};

class CSyncDstDomainAddReq: public CEventReq
{
public:
    CSyncDstDomainAddReq()
    {
        event = OspExtEventDesc(TAS_SYNCDSTDOMAIN_ADD_REQ);
    }
private:
    TSyncDstDomainInfo info;              
public:
    void SetInfo(const TSyncDstDomainInfo& cInfo)
    {
        info = cInfo;
    }
    TSyncDstDomainInfo& GetInfo()
    {
        return info;
    }
    const TSyncDstDomainInfo& GetInfo() const
    {
        return info;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CSyncDstDomainAddRsp: public CEventRsp
{
public:
    CSyncDstDomainAddRsp()
    {
        event = OspExtEventDesc(TAS_SYNCDSTDOMAIN_ADD_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CSyncDstDomainAddNtfReq: public CEventReq
{
public:
    CSyncDstDomainAddNtfReq()
    {
        event = OspExtEventDesc(TAS_SYNCDSTDOMAIN_ADD_NTF_REQ);
    }
private:
    TSyncDstDomainInfo info;              
public:
    void SetInfo(const TSyncDstDomainInfo& cInfo)
    {
        info = cInfo;
    }
    TSyncDstDomainInfo& GetInfo()
    {
        return info;
    }
    const TSyncDstDomainInfo& GetInfo() const
    {
        return info;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CSyncDstDomainDelReq: public CEventReq
{
public:
    CSyncDstDomainDelReq()
    {
        event = OspExtEventDesc(TAS_SYNCDSTDOMAIN_DEL_REQ);
    }
private:
    UUIDString domainId;  
    ENSyncType syncType;              
public:
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

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetSyncType(ENSyncType eSyncType)
    {
        syncType = eSyncType;
    }
    ENSyncType GetSyncType() const
    {
        return syncType;
    }

};

class CSyncDstDomainDelRsp: public CEventRsp
{
public:
    CSyncDstDomainDelRsp()
    {
        event = OspExtEventDesc(TAS_SYNCDSTDOMAIN_DEL_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CSyncDstDomainDelNtfReq: public CEventReq
{
public:
    CSyncDstDomainDelNtfReq()
    {
        event = OspExtEventDesc(TAS_SYNCDSTDOMAIN_DEL_NTF_REQ);
    }
private:
    UUIDString domainId;
    ENSyncType syncType;              
public:
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

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetSyncType(ENSyncType eSyncType)
    {
        syncType = eSyncType;
    }
    ENSyncType GetSyncType() const
    {
        return syncType;
    }

};

class CSyncDstDomainModReq: public CEventReq
{
public:
    CSyncDstDomainModReq()
    {
        event = OspExtEventDesc(TAS_SYNCDSTDOMAIN_MOD_REQ);
    }
private:
    UUIDString domainId;              
    string domainName;              
public:
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

    void SetDomainName(const string& strDomainName)
    {
        domainName = strDomainName;
    }
    string& GetDomainName()
    {
        return domainName;
    }
    const string& GetDomainName() const
    {
        return domainName;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CSyncDstDomainModRsp: public CEventRsp
{
public:
    CSyncDstDomainModRsp()
    {
        event = OspExtEventDesc(TAS_SYNCDSTDOMAIN_MOD_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CSyncDstDomainModNtfReq: public CEventReq
{
public:
    CSyncDstDomainModNtfReq()
    {
        event = OspExtEventDesc(TAS_SYNCDSTDOMAIN_MOD_NTF_REQ);
    }
private:
    TSyncDstDomainInfo info;              
public:
    void SetInfo(const TSyncDstDomainInfo& cInfo)
    {
        info = cInfo;
    }
    TSyncDstDomainInfo& GetInfo()
    {
        return info;
    }
    const TSyncDstDomainInfo& GetInfo() const
    {
        return info;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CSyncDstDomainQryReq: public CQueryReqBase
{
public:
    CSyncDstDomainQryReq()
    {
        event = OspExtEventDesc(TAS_SYNCDSTDOMAIN_QRY_REQ);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CSyncDstDomainQryRsp: public CQueryRspBase
{
public:
    CSyncDstDomainQryRsp()
    {
        event = OspExtEventDesc(TAS_SYNCDSTDOMAIN_QRY_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CSyncDstDomainQryNtfReq: public CQueryNtfReqBase
{
public:
    CSyncDstDomainQryNtfReq()
    {
        event = OspExtEventDesc(TAS_SYNCDSTDOMAIN_QRY_NTF_REQ);
    }
private:
    vector<TSyncDstDomainInfo> infos;              
public:
    void SetInfos(const vector<TSyncDstDomainInfo>& cInfos)
    {
        infos = cInfos;
    }
    vector<TSyncDstDomainInfo>& GetInfos()
    {
        return infos;
    }
    const vector<TSyncDstDomainInfo>& GetInfos() const
    {
        return infos;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CSyncSrcDomainDelReq: public CEventReq
{
public:
    CSyncSrcDomainDelReq()
    {
        event = OspExtEventDesc(TAS_SYNCSRCDOMAIN_DEL_REQ);
    }
private:
    UUIDString domainId;              
public:
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

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CSyncSrcDomainDelRsp: public CEventRsp
{
public:
    CSyncSrcDomainDelRsp()
    {
        event = OspExtEventDesc(TAS_SYNCSRCDOMAIN_DEL_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CDevTreeAddReq: public CEventReq
{
public:
	CDevTreeAddReq()
	{
		event = OspExtEventDesc(TAS_DEVTREE_ADD_REQ);
		canBeUsed4Gb = false;
        type=CDevTreeInfo::SystemCustomTree;
	}

private:
	string devtreeName;
	bool   canBeUsed4Gb;
	bool   copyFromOtherTree;               
	UUIDString srcTreeId;                //新建设备树可从指定树拷贝
    CDevTreeInfo::TreeType type;                 
public:
    void SetDevtreeName(const string& strDevtreeName)
    {
        devtreeName = strDevtreeName;
    }
    string& GetDevtreeName()
    {
        return devtreeName;
    }
    const string& GetDevtreeName() const
    {
        return devtreeName;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetCanBeUsed4Gb(bool bCanBeUsed4Gb)
    {
        canBeUsed4Gb = bCanBeUsed4Gb;
    }
    bool GetCanBeUsed4Gb() const
    {
        return canBeUsed4Gb;
    }

public:
    void SetSrcTreeId(const UUIDString& strSrcTreeId)
    {
        srcTreeId = strSrcTreeId;
    }
    UUIDString& GetSrcTreeId()
    {
        return srcTreeId;
    }
    const UUIDString& GetSrcTreeId() const
    {
        return srcTreeId;
    }

public:
    void SetCopyFromOtherTree(bool bCopyFromOtherTree)
    {
        copyFromOtherTree = bCopyFromOtherTree;
    }
    bool GetCopyFromOtherTree() const
    {
        return copyFromOtherTree;
    }

public:
    void SetType(CDevTreeInfo::TreeType eType)
    {
        type = eType;
    }
    CDevTreeInfo::TreeType GetType() const
    {
        return type;
    }

};

class CDevTreeAddRsp: public CEventRsp
{
public:
	CDevTreeAddRsp()
	{
		event = OspExtEventDesc(TAS_DEVTREE_ADD_RSP);
	}

private:
	UUIDString devtreeId;
	UUIDString operUuid;                
public:
    void SetDevtreeId(const UUIDString& strDevtreeId)
    {
        devtreeId = strDevtreeId;
    }
    UUIDString& GetDevtreeId()
    {
        return devtreeId;
    }
    const UUIDString& GetDevtreeId() const
    {
        return devtreeId;
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetOperUuid(const UUIDString& strOperUuid)
    {
        operUuid = strOperUuid;
    }
    UUIDString& GetOperUuid()
    {
        return operUuid;
    }
    const UUIDString& GetOperUuid() const
    {
        return operUuid;
    }

};

class CDevTreeModReq: public CEventReq
{
public:
	CDevTreeModReq()
	{
		event = OspExtEventDesc(TAS_DEVTREE_MOD_REQ);
	}

private:
	UUIDString devtreeId;               
	string devtreeName;                                    

public:
    void SetDevtreeId(const UUIDString& strDevtreeId)
    {
        devtreeId = strDevtreeId;
    }
    UUIDString& GetDevtreeId()
    {
        return devtreeId;
    }
    const UUIDString& GetDevtreeId() const
    {
        return devtreeId;
    }

    void SetDevtreeName(const string& strDevtreeName)
    {
        devtreeName = strDevtreeName;
    }
    string& GetDevtreeName()
    {
        return devtreeName;
    }
    const string& GetDevtreeName() const
    {
        return devtreeName;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDevTreeModRsp: public CEventRsp
{
public:
	CDevTreeModRsp()
	{
		event = OspExtEventDesc(TAS_DEVTREE_MOD_RSP);
	}

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDevTreeDelReq: public CEventReq
{
public:
	CDevTreeDelReq()
	{
		event = OspExtEventDesc(TAS_DEVTREE_DEL_REQ);
	}

private:
	UUIDString devtreeId;                            

public:
    void SetDevtreeId(const UUIDString& strDevtreeId)
    {
        devtreeId = strDevtreeId;
    }
    UUIDString& GetDevtreeId()
    {
        return devtreeId;
    }
    const UUIDString& GetDevtreeId() const
    {
        return devtreeId;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDevTreeDelRsp: public CEventRsp
{
public:
	CDevTreeDelRsp()
	{
		event = OspExtEventDesc(TAS_DEVTREE_DEL_RSP);
	}

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CDevTreeQryReq: public CQueryReqBase
{
public:
	CDevTreeQryReq()
	{
		event = OspExtEventDesc(TAS_DEVTREE_QRY_REQ);
        type=CDevTreeInfo::SystemCustomTree;
        includeFavorites = false;
	}

private:
    CDevTreeInfo::TreeType type; 
    bool includeFavorites; // 包括用户自己的收藏夹

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

public:
    void SetType(CDevTreeInfo::TreeType eType)
    {
        type = eType;
    }
    CDevTreeInfo::TreeType GetType() const
    {
        return type;
    }
    void SetIncludeFavorites(bool bIncludeFavorites)
    {
        includeFavorites = bIncludeFavorites;
    }
    bool GetIncludeFavorites() const
    {
        return includeFavorites;
    }
};

class CDevTreeQryRsp: public CQueryRspBase
{
public:
	CDevTreeQryRsp()
	{
		event = OspExtEventDesc(TAS_DEVTREE_QRY_RSP);
	}

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDevTreeQryNtfReq: public CQueryNtfReqBase
{
public:
	CDevTreeQryNtfReq()
	{
		event = OspExtEventDesc(TAS_DEVTREE_QRY_NTF_REQ);
	}

private:
	vector<CDevTreeInfo> infos;               
public:
    void SetInfos(const vector<CDevTreeInfo>& cInfos)
    {
        infos = cInfos;
    }
    vector<CDevTreeInfo>& GetInfos()
    {
        return infos;
    }
    const vector<CDevTreeInfo>& GetInfos() const
    {
        return infos;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDevTreeDevGrpAssignReq: public CEventReq
{
public:
	CDevTreeDevGrpAssignReq()
	{
		event = OspExtEventDesc(TAS_DEVTREE_DEVGROUP_ASSIGN_REQ);
	}

private:
	UUIDString srcGroupId;     //来源组ID             
	UUIDString srcDevTreeId;   //来源设备树ID 
	UUIDString dstParentGrpId; //移动目的父组ID
	UUIDString dstDevTreeId;   //移动目的设备树ID
             
	bool onlyOnline;               
	bool onlyUnAssigned;               
    bool onlyEnable;                   

	TDevGroupType grpType;                   //分组类型，国标选择推送树有效 
public:
    void SetSrcGroupId(const UUIDString& strSrcGroupId)
    {
        srcGroupId = strSrcGroupId;
    }
    UUIDString& GetSrcGroupId()
    {
        return srcGroupId;
    }
    const UUIDString& GetSrcGroupId() const
    {
        return srcGroupId;
    }

    void SetSrcDevTreeId(const UUIDString& strSrcDevTreeId)
    {
        srcDevTreeId = strSrcDevTreeId;
    }
    UUIDString& GetSrcDevTreeId()
    {
        return srcDevTreeId;
    }
    const UUIDString& GetSrcDevTreeId() const
    {
        return srcDevTreeId;
    }

    void SetDstParentGrpId(const UUIDString& strDstParentGrpId)
    {
        dstParentGrpId = strDstParentGrpId;
    }
    UUIDString& GetDstParentGrpId()
    {
        return dstParentGrpId;
    }
    const UUIDString& GetDstParentGrpId() const
    {
        return dstParentGrpId;
    }

    void SetDstDevTreeId(const UUIDString& strDstDevTreeId)
    {
        dstDevTreeId = strDstDevTreeId;
    }
    UUIDString& GetDstDevTreeId()
    {
        return dstDevTreeId;
    }
    const UUIDString& GetDstDevTreeId() const
    {
        return dstDevTreeId;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetOnlyOnline(bool bOnlyOnline)
    {
        onlyOnline = bOnlyOnline;
    }
    bool GetOnlyOnline() const
    {
        return onlyOnline;
    }

    void SetOnlyUnAssigned(bool bOnlyUnAssigned)
    {
        onlyUnAssigned = bOnlyUnAssigned;
    }
    bool GetOnlyUnAssigned() const
    {
        return onlyUnAssigned;
    }

public:
    void SetGrpType(TDevGroupType eGrpType)
    {
        grpType = eGrpType;
    }
    TDevGroupType GetGrpType() const
    {
        return grpType;
    }
public:
    void SetOnlyEnable(bool bOnlyEnable)
    {
        onlyEnable = bOnlyEnable;
    }
    bool GetOnlyEnable() const
    {
        return onlyEnable;
    }

};

class CDevTreeDevGrpAssignRsp: public CEventRsp
{
public:
	CDevTreeDevGrpAssignRsp()
	{
		event = OspExtEventDesc(TAS_DEVTREE_DEVGROUP_ASSIGN_RSP);
	}

	void SetOperUuid(const UUIDString &val)
	{
		operUuid = val;
	}
	const UUIDString &GetOperUuid() const
	{
		return operUuid;
	}
	UUIDString &GetOperUuid()
	{
		return operUuid;
	}

private:
	UUIDString operUuid;				// 操作Id，标志唯一的一次操作，用于后续查询进度
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDeviceGrpPathQryReq: public CQueryReqBase
{
public:
	CDeviceGrpPathQryReq()
	{
		event = OspExtEventDesc(TAS_DEVICE_GROUPPATH_QRY_REQ);
	}
private:
	UUIDString devTreeId;                  //设备树ID
	UUIDString deviceId;                  //设备ID
	int  videoSrcId;                   //视频源Id
public:
    void SetDevTreeId(const UUIDString& strDevTreeId)
    {
        devTreeId = strDevTreeId;
    }
    UUIDString& GetDevTreeId()
    {
        return devTreeId;
    }
    const UUIDString& GetDevTreeId() const
    {
        return devTreeId;
    }

    void SetDeviceId(const UUIDString& strDeviceId)
    {
        deviceId = strDeviceId;
    }
    UUIDString& GetDeviceId()
    {
        return deviceId;
    }
    const UUIDString& GetDeviceId() const
    {
        return deviceId;
    }

    void SetVideoSrcId(int nVideoSrcId)
    {
        videoSrcId = nVideoSrcId;
    }
    int GetVideoSrcId() const
    {
        return videoSrcId;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDeviceGrpPathQryRsp: public CQueryRspBase
{
public:
	CDeviceGrpPathQryRsp()
	{
		event = OspExtEventDesc(TAS_DEVICE_GROUPPATH_QRY_RSP);
	}

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDeviceGrpPathQryNtfReq: public CQueryNtfReqBase
{
public:
	CDeviceGrpPathQryNtfReq()
	{
		event = OspExtEventDesc(TAS_DEVICE_GROUPPATH_QRY_NTF_REQ);
	}

private:
	vector<CDeviceGroupInfo> infos;                       
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetInfos(const vector<CDeviceGroupInfo>& cInfos)
    {
        infos = cInfos;
    }
    vector<CDeviceGroupInfo>& GetInfos()
    {
        return infos;
    }
    const vector<CDeviceGroupInfo>& GetInfos() const
    {
        return infos;
    }

};


class CDevCapInfoModReq: public CEventReq
{
public:
	CDevCapInfoModReq()
	{
		event = OspExtEventDesc(TAS_DEV_CAPINFO_MOD_REQ);
	}

	UUIDString& GetDeviceUUID() 
	{
		return deviceId;
	}
	const UUIDString& GetDeviceUUID() const
	{
		return deviceId;
	}
	void SetDeviceUUID(const UUIDString &val)
	{
		deviceId = val;
	}

	vector<TDeviceCapInfo>& GetDeviceModelCapDetail()
	{
		return modelCapDetails;
	}
	const vector<TDeviceCapInfo>& GetDeviceModelCapDetail() const
	{
		return modelCapDetails;
	}
	void SetDeviceModelCapDetail(const vector<TDeviceCapInfo>& val)
	{
		modelCapDetails = val;
	}
private:
	 UUIDString deviceId; // 设备UUIDString
	 vector<TDeviceCapInfo> modelCapDetails; // 设备型号能力
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDevCapInfoModRsp: public CEventRsp
{
public:
	CDevCapInfoModRsp()
	{
		event = OspExtEventDesc(TAS_DEV_CAPINFO_MOD_RSP);
	}

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CPtzLockAddReq: public CEventReq
{
public:
	CPtzLockAddReq()
	{
		event = OspExtEventDesc(TAS_PTZLOCK_ADD_REQ);
	}
private:
	CPtzLockInfo cPtzLockInfo;               
public:
    void SetCPtzLockInfo(const CPtzLockInfo& cCPtzLockInfo)
    {
        cPtzLockInfo = cCPtzLockInfo;
    }
    CPtzLockInfo& GetCPtzLockInfo()
    {
        return cPtzLockInfo;
    }
    const CPtzLockInfo& GetCPtzLockInfo() const
    {
        return cPtzLockInfo;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPtzLockAddRsp: public CEventRsp
{
public:
	CPtzLockAddRsp()
	{
		event = OspExtEventDesc(TAS_PTZLOCK_ADD_RSP);
	}

private:  
	string beginTime;               	// 锁定开始时间.
	string endTime;                 	// 自动解锁时间.
	string userId;	               	// 加锁用户Id
	string username;                   // 加锁用户名;
	int priPtzLevel;               	//加锁用户PTZ锁定权限
public:
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

    void SetUserId(const string& strUserId)
    {
        userId = strUserId;
    }
    string& GetUserId()
    {
        return userId;
    }
    const string& GetUserId() const
    {
        return userId;
    }

    void SetUsername(const string& strUsername)
    {
        username = strUsername;
    }
    string& GetUsername()
    {
        return username;
    }
    const string& GetUsername() const
    {
        return username;
    }

    void SetPriPtzLevel(int nPriPtzLevel)
    {
        priPtzLevel = nPriPtzLevel;
    }
    int GetPriPtzLevel() const
    {
        return priPtzLevel;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

typedef string TPtzLockOprType;
#define PTZLOCK_UNLOCK					"unlock"	// 解锁
#define PTZLOCK_DELETE					"delete"	// cui超时直接删除锁
#define PTZLOCK_UNLOCKALL				"unlockall"	// 批量解锁所有
class CPtzLockDelReq: public CEventReq
{
public:
	CPtzLockDelReq()
	{
		event = OspExtEventDesc(TAS_PTZLOCK_DEL_REQ);
	}

private:
	string deviceId;               	 // 设备id
	int videoSrcId;               	 // 视频源id
	string userId;               	 // 解锁用户
	string password;                // 手动解锁密码.
	TPtzLockOprType operateType;    // 操作类型 解锁/删除/批量删除 unlock/delete/unlockall
public:
    void SetDeviceId(const string& strDeviceId)
    {
        deviceId = strDeviceId;
    }
    string& GetDeviceId()
    {
        return deviceId;
    }
    const string& GetDeviceId() const
    {
        return deviceId;
    }

    void SetVideoSrcId(int nVideoSrcId)
    {
        videoSrcId = nVideoSrcId;
    }
    int GetVideoSrcId() const
    {
        return videoSrcId;
    }

    void SetUserId(const string& strUserId)
    {
        userId = strUserId;
    }
    string& GetUserId()
    {
        return userId;
    }
    const string& GetUserId() const
    {
        return userId;
    }

    void SetPassword(const string& strPassword)
    {
        password = strPassword;
    }
    string& GetPassword()
    {
        return password;
    }
    const string& GetPassword() const
    {
        return password;
    }

    void SetOperateType(const TPtzLockOprType& strOperateType)
    {
        operateType = strOperateType;
    }
    TPtzLockOprType& GetOperateType()
    {
        return operateType;
    }
    const TPtzLockOprType& GetOperateType() const
    {
        return operateType;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPtzLockDelRsp: public CEventRsp
{
public:
	CPtzLockDelRsp()
	{
		event = OspExtEventDesc(TAS_PTZLOCK_DEL_RSP);
	}
private:  
	string beginTime; 	// 锁定开始时间.
	string endTime;   	// 自动解锁时间.
	string userId;	  // 加锁用户Id
	string username;     // 加锁用户名;
	int priPtzLevel;  //加锁用户PTZ锁定权限
public:
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

    void SetUserId(const string& strUserId)
    {
        userId = strUserId;
    }
    string& GetUserId()
    {
        return userId;
    }
    const string& GetUserId() const
    {
        return userId;
    }

    void SetUsername(const string& strUsername)
    {
        username = strUsername;
    }
    string& GetUsername()
    {
        return username;
    }
    const string& GetUsername() const
    {
        return username;
    }

    void SetPriPtzLevel(int nPriPtzLevel)
    {
        priPtzLevel = nPriPtzLevel;
    }
    int GetPriPtzLevel() const
    {
        return priPtzLevel;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CPtzLockQryReq: public CQueryReqBase
{
public:
	CPtzLockQryReq()
	{
		event = OspExtEventDesc(TAS_PTZLOCK_QRY_REQ);
	}
private:
	 CPtzLockQueryCondition condition;                
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
public:
    void SetCondition(const CPtzLockQueryCondition& cCondition)
    {
        condition = cCondition;
    }
    CPtzLockQueryCondition& GetCondition()
    {
        return condition;
    }
    const CPtzLockQueryCondition& GetCondition() const
    {
        return condition;
    }

};

class CPtzLockQryRsp: public CQueryRspBase
{
public:
	CPtzLockQryRsp()
	{
		event = OspExtEventDesc(TAS_PTZLOCK_QRY_RSP);
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class CPtzLockQryNtfReq: public CQueryNtfReqBase
{
public:
	CPtzLockQryNtfReq()
	{
		event = OspExtEventDesc(TAS_PTZLOCK_QRY_NTF_REQ);
	}

private:
	vector<CPtzLockInfo> infos;                              
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
public:
    void SetInfos(const vector<CPtzLockInfo>& cInfos)
    {
        infos = cInfos;
    }
    vector<CPtzLockInfo>& GetInfos()
    {
        return infos;
    }
    const vector<CPtzLockInfo>& GetInfos() const
    {
        return infos;
    }

};


class CPtzLockAddNtfReq: public CEventReq
{
public:
	CPtzLockAddNtfReq()
	{
		event = OspExtEventDesc(TAS_PTZLOCK_ADD_NTF_REQ);
	}

private:
	CPtzLockInfo info;               

public:
    void SetInfo(const CPtzLockInfo& cInfo)
    {
        info = cInfo;
    }
    CPtzLockInfo& GetInfo()
    {
        return info;
    }
    const CPtzLockInfo& GetInfo() const
    {
        return info;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CPtzLockDelNtfReq: public CEventReq
{
public:
	CPtzLockDelNtfReq()
	{
		event = OspExtEventDesc(TAS_PTZLOCK_DEL_NTF_REQ);
	}

private:
	UUIDString deviceId;               
	int     videoSrcId;                              	 // 视频源id   
	TPtzLockOprType operateType;                   // 操作类型 解锁/删除/批量删除 unlock/delete/unlockall
public:
    void SetDeviceId(const UUIDString& strDeviceId)
    {
        deviceId = strDeviceId;
    }
    UUIDString& GetDeviceId()
    {
        return deviceId;
    }
    const UUIDString& GetDeviceId() const
    {
        return deviceId;
    }

    void SetVideoSrcId(int nVideoSrcId)
    {
        videoSrcId = nVideoSrcId;
    }
    int GetVideoSrcId() const
    {
        return videoSrcId;
    }

	void SetOperateType(const TPtzLockOprType& strOperateType)
	{
		operateType = strOperateType;
	}
	TPtzLockOprType& GetOperateType()
	{
		return operateType;
	}
	const TPtzLockOprType& GetOperateType() const
	{
		return operateType;
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUserDevGrpVidsrcQryReq:public CEventReq
{
public:
	CUserDevGrpVidsrcQryReq()
	{
		event = OspExtEventDesc(TAS_CUSER_DEVGRP_VIDSRC_QRY_REQ);
		CUUID cUUID;
		queryTag = cUUID;
		recursive = true;
	}
private:
	UUIDString devtreeId;                           // 设备树Id（为空时指系统设备树）
	UUIDString cuserId;                             // 用户id（非空）
	UUIDString devGrpId;               				// 设备组id（非空）
	UUIDString queryTag;                            // 非空
	bool       recursive;                           // 是否递归查询（默认递归查询--true，false-不递归）
public:
    void SetDevtreeId(const UUIDString& strDevtreeId)
    {
        devtreeId = strDevtreeId;
    }
    UUIDString& GetDevtreeId()
    {
        return devtreeId;
    }
    const UUIDString& GetDevtreeId() const
    {
        return devtreeId;
    }

    void SetCuserId(const UUIDString& strCuserId)
    {
        cuserId = strCuserId;
    }
    UUIDString& GetCuserId()
    {
        return cuserId;
    }
    const UUIDString& GetCuserId() const
    {
        return cuserId;
    }

    void SetDevGrpId(const UUIDString& strDevGrpId)
    {
        devGrpId = strDevGrpId;
    }
    UUIDString& GetDevGrpId()
    {
        return devGrpId;
    }
    const UUIDString& GetDevGrpId() const
    {
        return devGrpId;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetQueryTag(const UUIDString& strQueryTag)
    {
        queryTag = strQueryTag;
    }
    UUIDString& GetQueryTag()
    {
        return queryTag;
    }
    const UUIDString& GetQueryTag() const
    {
        return queryTag;
    }
public:
    void SetRecursive(bool bRecursive)
    {
        recursive = bRecursive;
    }
    bool GetRecursive() const
    {
        return recursive;
    }

};

class CUserDevGrpVidsrcQryRsp:public CEventRsp
{
public:
	CUserDevGrpVidsrcQryRsp()
	{
		event = OspExtEventDesc(TAS_CUSER_DEVGRP_VIDSRC_QRY_RSP);
		hasRecords = false;
	}
private:
	bool hasRecords;                      //是否有记录（也即是否有NtfReq）                        
	UUIDString queryTag;                 
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetHasRecords(bool bHasRecords)
    {
        hasRecords = bHasRecords;
    }
    bool GetHasRecords() const
    {
        return hasRecords;
    }

    void SetQueryTag(const UUIDString& strQueryTag)
    {
        queryTag = strQueryTag;
    }
    UUIDString& GetQueryTag()
    {
        return queryTag;
    }
    const UUIDString& GetQueryTag() const
    {
        return queryTag;
    }

};

class CUserDevGrpVidsrcQryNtfReq:public CEventReq
{
public:
	CUserDevGrpVidsrcQryNtfReq()
	{
		event = OspExtEventDesc(TAS_CUSER_DEVGRP_VIDSRC_QRY_NTF_REQ);
		lastNtf = false;
	}

private:            
	bool lastNtf;                                       //是否最后一条Ntf（查询业务最后一条Ntf，即所有分页查询都通知结束）
	UUIDString queryTag;               
	vector<CUserDevGrpVidSrcInfo> infos;                                               
  
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetLastNtf(bool bLastNtf)
    {
        lastNtf = bLastNtf;
    }
    bool GetLastNtf() const
    {
        return lastNtf;
    }

    void SetQueryTag(const UUIDString& strQueryTag)
    {
        queryTag = strQueryTag;
    }
    UUIDString& GetQueryTag()
    {
        return queryTag;
    }
    const UUIDString& GetQueryTag() const
    {
        return queryTag;
    }

    void SetInfos(const vector<CUserDevGrpVidSrcInfo>& cInfos)
    {
        infos = cInfos;
    }
    vector<CUserDevGrpVidSrcInfo>& GetInfos()
    {
        return infos;
    }
    const vector<CUserDevGrpVidSrcInfo>& GetInfos() const
    {
        return infos;
    }

};


class CDevGrpVidsrcTotalnumGetReq:public CEventReq
{
public:
	CDevGrpVidsrcTotalnumGetReq()
	{
		event = OspExtEventDesc(TAS_DEVGRP_VIDSRC_TOTALNUM_GET_REQ);
	}
private:
	UUIDString userId;                           //用户ID
	vector<UUIDString> devGrpIds;                //待查询分组Id列表
public:
    void SetDevGrpIds(const vector<UUIDString>& cDevGrpIds)
    {
        devGrpIds = cDevGrpIds;
    }
    vector<UUIDString>& GetDevGrpIds()
    {
        return devGrpIds;
    }
    const vector<UUIDString>& GetDevGrpIds() const
    {
        return devGrpIds;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetUserId(const UUIDString& strUserId)
    {
        userId = strUserId;
    }
    UUIDString& GetUserId()
    {
        return userId;
    }
    const UUIDString& GetUserId() const
    {
        return userId;
    }

};

class CDevGrpVidsrcTotalnumGetRsp:public CEventRsp
{
public:
	CDevGrpVidsrcTotalnumGetRsp()
	{
		event = OspExtEventDesc(TAS_DEVGRP_VIDSRC_TOTALNUM_GET_RSP);
	}
private:
	vector<CDevGrpVidSrcNum> devGrpVidsrcNum;                 //分组下视频源总数
public:
    void SetDevGrpVidsrcNum(const vector<CDevGrpVidSrcNum>& cDevGrpVidsrcNum)
    {
        devGrpVidsrcNum = cDevGrpVidsrcNum;
    }
    vector<CDevGrpVidSrcNum>& GetDevGrpVidsrcNum()
    {
        return devGrpVidsrcNum;
    }
    const vector<CDevGrpVidSrcNum>& GetDevGrpVidsrcNum() const
    {
        return devGrpVidsrcNum;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CVidsrcInfoQryReq:public CQueryReqBase
{
public:
	CVidsrcInfoQryReq()
	{
		event = OspExtEventDesc(TAS_VIDSRC_INFO_QRY_REQ);
	}
private:           
	vector<CDevVidsrc> devVidsrcs;               
public:

    void SetDevVidsrcs(const vector<CDevVidsrc>& cDevVidsrcs)
    {
        devVidsrcs = cDevVidsrcs;
    }
    vector<CDevVidsrc>& GetDevVidsrcs()
    {
        return devVidsrcs;
    }
    const vector<CDevVidsrc>& GetDevVidsrcs() const
    {
        return devVidsrcs;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CVidsrcInfoQryRsp:public CQueryRspBase
{
public:
	CVidsrcInfoQryRsp()
	{
		event = OspExtEventDesc(TAS_VIDSRC_INFO_QRY_RSP);
	}

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CVidsrcInfoQryNtfReq:public CQueryNtfReqBase
{
public:
	CVidsrcInfoQryNtfReq()
	{
		event = OspExtEventDesc(TAS_VIDSRC_INFO_QRY_NTF_REQ);
	}

private:
	vector<CDevVidSrcInfo> devVidsrcInfo;               
public:

    void SetDevVidsrcInfo(const vector<CDevVidSrcInfo>& cDevVidsrcInfo)
    {
        devVidsrcInfo = cDevVidsrcInfo;
    }
    vector<CDevVidSrcInfo>& GetDevVidsrcInfo()
    {
        return devVidsrcInfo;
    }
    const vector<CDevVidSrcInfo>& GetDevVidsrcInfo() const
    {
        return devVidsrcInfo;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


#endif // _TAS_PROTO_H_








































































