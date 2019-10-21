/*****************************************************************************
模块名      : pms_proto.h 
文件名      : pms_proto.h
相关文件    : 
文件实现功能: pms_proto.h 定义PMC_PMS消息体
作者        : liangli
版本        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2011/10/24   1.0         liangli        创建
******************************************************************************/

#ifndef _PMS_PROTO_H_
#define _PMS_PROTO_H_

#include "cms/pms/pms_event.h"
#include "cms/pms/pms_struct.h"

//class CLoginPmsReq: public CEventReq
//{
//public:
//    enum TLoginType
//    {
//        enLoginType_PMCS = 1,     // pmcs登录, 提供pmc用户名和密码以及登录模块(比如pmcs)的uri
//        enLoginType_SlavePMS = 2  // 从平台pms登录，提供pms的模块uri
//    };
//    CLoginPmsReq()
//    {
//        event = OspExtEventDesc(PMS_LOGIN_PMS_REQ);
//		loginType = enLoginType_PMCS;
//    }
//    const TLoginType GetLoginType()const
//    {
//        return loginType;
//    }
//    TLoginType GetLoginType()
//    {
//        return loginType;
//    }
//    void SetLoginType(const TLoginType enLoginType)
//    {
//        loginType = enLoginType;
//    }
//    const string& GetUserName()const
//    {
//        return userName;
//    }
//    string& GetUserName()
//    {
//        return userName;
//    }
//    void SetUserName(const string& strUserName)
//    {
//        userName = strUserName;
//    }
//    const string& GetPassword()const
//    {
//        return password;
//    }
//    string& GetPassword()
//    {
//        return password;
//    }
//    void SetPassword(const string& strPassword)
//    {
//        password = strPassword;
//    }
//
//private:
//    TLoginType loginType; // 登录类型
//    string userName;      // 模块UUIDString或者用户名，由登录类型决定
//    string password;
//public:
//    const std::string ToXml()const;
//    void FromXml(const std::string& xml_string);
//protected:
//    void ToXml(TiXmlDocument& xmlDoc)const;
//public:
//    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
//    void FromXml(const TiXmlNode& xmlNode);
//};
//
//class CLoginPmsRsp: public CEventRsp
//{
//public:
//    CLoginPmsRsp()
//    {
//        event = OspExtEventDesc(PMS_LOGIN_PMS_RSP);
//    }
//    // 登录成功时在CEventRsp中返回服务端生成的会话号
//public:
//    const std::string ToXml()const;
//    void FromXml(const std::string& xml_string);
//protected:
//    void ToXml(TiXmlDocument& xmlDoc)const;
//public:
//    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
//    void FromXml(const TiXmlNode& xmlNode);
//};
//
//class CLogoutPmsReq: public CEventReq
//{
//public:
//    CLogoutPmsReq()
//    {
//        event = OspExtEventDesc(PMS_LOGOUT_PMS_REQ);
//    }
//    // 在CEventReq中提供登录成功时返回的会话号
//public:
//    const std::string ToXml()const;
//    void FromXml(const std::string& xml_string);
//protected:
//    void ToXml(TiXmlDocument& xmlDoc)const;
//public:
//    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
//    void FromXml(const TiXmlNode& xmlNode);
//};
//
//class CLogoutPmsRsp: public CEventRsp
//{
//public:
//    CLogoutPmsRsp()
//    {
//        event = OspExtEventDesc(PMS_LOGOUT_PMS_RSP);
//    }
//public:
//    const std::string ToXml()const;
//    void FromXml(const std::string& xml_string);
//protected:
//    void ToXml(TiXmlDocument& xmlDoc)const;
//public:
//    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
//    void FromXml(const TiXmlNode& xmlNode);
//};
//
//class CPmcOnlineReq: public CEventReq
//{
//public:
//    CPmcOnlineReq()
//    {
//        event = OspExtEventDesc(PMS_PMC_ONLINE_REQ);
//    }
//
//    const string& GetUserName()const
//    {
//        return userName;
//    }
//    string& GetUserName()
//    {
//        return userName;
//    }
//    void SetUserName(const string& strUserName)
//    {
//        userName = strUserName;
//    }
//    const string& GetPassword()const
//    {
//        return password;
//    }
//    string& GetPassword()
//    {
//        return password;
//    }
//    void SetPassword(const string& strPassword)
//    {
//        password = strPassword;
//    }
//
//private:
//    string userName;     // pmc用户名
//    string password;     // pmc密码
//public:
//    const std::string ToXml()const;
//    void FromXml(const std::string& xml_string);
//protected:
//    void ToXml(TiXmlDocument& xmlDoc)const;
//public:
//    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
//    void FromXml(const TiXmlNode& xmlNode);
//};
//
//class CPmcOnlineRsp: public CEventRsp
//{
//public:
//    CPmcOnlineRsp()
//    {
//        event = OspExtEventDesc(PMS_PMC_ONLINE_RSP);
//    }
//
//    const string& GetPmcSessionID()const
//    {
//        return pmcSessionID;
//    }
//    string& GetPmcSessionID()
//    {
//        return pmcSessionID;
//    }
//    void SetPmcSessionID(const string &val)
//    {
//        pmcSessionID = val;
//    }
//private:
//    string pmcSessionID; 
//
//public:
//    const std::string ToXml()const;
//    void FromXml(const std::string& xml_string);
//protected:
//    void ToXml(TiXmlDocument& xmlDoc)const;
//public:
//    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
//    void FromXml(const TiXmlNode& xmlNode);
//};
//
//class CPmcOfflineReq: public CEventReq
//{
//public:
//    CPmcOfflineReq()
//    {
//        event = OspExtEventDesc(PMS_PMC_OFFLINE_REQ);
//    }
//    const string& GetPmcSessionID()const
//    {
//        return pmcSessionID;
//    }
//    string& GetPmcSessionID()
//    {
//        return pmcSessionID;
//    }
//    void SetPmcSessionID(const string &val)
//    {
//        pmcSessionID = val;
//    }
//private:
//    string pmcSessionID;
//
//public:
//    const std::string ToXml()const;
//    void FromXml(const std::string& xml_string);
//protected:
//    void ToXml(TiXmlDocument& xmlDoc)const;
//public:
//    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
//    void FromXml(const TiXmlNode& xmlNode);
//};
//
//class CPmcOfflineRsp: public CEventRsp
//{
//public:
//    CPmcOfflineRsp()
//    {
//        event = OspExtEventDesc(PMS_PMC_OFFLINE_RSP);
//    }
//
//public:
//    const std::string ToXml()const;
//    void FromXml(const std::string& xml_string);
//protected:
//    void ToXml(TiXmlDocument& xmlDoc)const;
//public:
//    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
//    void FromXml(const TiXmlNode& xmlNode);
//};

class CNruInfoQryReq:public CEventReq
{
public:
	CNruInfoQryReq()
	{
		event = OspExtEventDesc(PMS_NRU_INFO_QRY_REQ);
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CNruInfoQryRsp:public CEventRsp
{
public:
	CNruInfoQryRsp()
	{
		event = OspExtEventDesc(PMS_NRU_INFO_QRY_RSP);
	}
private:
	string svrIp;               
	string nruId;               
public:
    void SetSvrIp(const string& strSvrIp)
    {
        svrIp = strSvrIp;
    }
    string& GetSvrIp()
    {
        return svrIp;
    }
    const string& GetSvrIp() const
    {
        return svrIp;
    }

    void SetNruId(const string& strNruId)
    {
        nruId = strNruId;
    }
    string& GetNruId()
    {
        return nruId;
    }
    const string& GetNruId() const
    {
        return nruId;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CNruInfoUpdateReq:public CEventReq
{
public:
	CNruInfoUpdateReq()
	{
		event = OspExtEventDesc(PMS_NRU_INFO_UPDATE_REQ);
	}
private:
	string svrIp;               
public:
    void SetSvrIp(const string& strSvrIp)
    {
        svrIp = strSvrIp;
    }
    string& GetSvrIp()
    {
        return svrIp;
    }
    const string& GetSvrIp() const
    {
        return svrIp;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CNruInfoUpdateRsp:public CEventRsp
{
public:
	CNruInfoUpdateRsp()
	{
		event = OspExtEventDesc(PMS_NRU_INFO_UPDATE_RSP);
	}

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};



class CTcpdumpBeginAckReq:public CEventReq
{
public:
	CTcpdumpBeginAckReq()
	{
		event = OspExtEventDesc(PMS_TCPDUMP_BEGIN_ACK);
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CTcpdumpStopNtfReq:public CEventReq
{
public:
	CTcpdumpStopNtfReq()
	{
		event = OspExtEventDesc(PMS_TCPDUMP_STOP_NTF_REQ);
	}
private:
	bool isShowUser;               //pmc是否提示给用户               

public:
    void SetIsShowUser(bool bIsShowUser)
    {
        isShowUser = bIsShowUser;
    }
    bool GetIsShowUser() const
    {
        return isShowUser;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CTcpdumpStopNtfRsp:public CEventRsp
{
public:
	CTcpdumpStopNtfRsp()
	{
		event = OspExtEventDesc(PMS_TCPDUMP_STOP_NTF_RSP);
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CPmsHeartbeatConfGetReq:public CEventReq
{
public:
	CPmsHeartbeatConfGetReq()
	{
		event = OspExtEventDesc(PMS_HEARTBEAT_CONF_GET_REQ);
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CPmsHeartbeatConfGetRsp:public CEventRsp
{
public:
	CPmsHeartbeatConfGetRsp()
	{
		event = OspExtEventDesc(PMS_HEARTBEAT_CONF_GET_RSP);
	}
private:
	string backupPcIp;         //备机IP               
	string referencePcIp;      //参照机IP               
	string hostTmpIp;          //主机恢复时采用的临时IP               
	string hostTmpMask;        //主机恢复时采用的临时掩码               
	string hostTmpGateway;     //主机恢复时采用的临时网关             
	vector<string> hostIpList; //主机IP列表              
public:
    void SetBackupPcIp(const string& strBackupPcIp)
    {
        backupPcIp = strBackupPcIp;
    }
    string& GetBackupPcIp()
    {
        return backupPcIp;
    }
    const string& GetBackupPcIp() const
    {
        return backupPcIp;
    }

    void SetReferencePcIp(const string& strReferencePcIp)
    {
        referencePcIp = strReferencePcIp;
    }
    string& GetReferencePcIp()
    {
        return referencePcIp;
    }
    const string& GetReferencePcIp() const
    {
        return referencePcIp;
    }

    void SetHostTmpIp(const string& strHostTmpIp)
    {
        hostTmpIp = strHostTmpIp;
    }
    string& GetHostTmpIp()
    {
        return hostTmpIp;
    }
    const string& GetHostTmpIp() const
    {
        return hostTmpIp;
    }

    void SetHostTmpMask(const string& strHostTmpMask)
    {
        hostTmpMask = strHostTmpMask;
    }
    string& GetHostTmpMask()
    {
        return hostTmpMask;
    }
    const string& GetHostTmpMask() const
    {
        return hostTmpMask;
    }

    void SetHostTmpGateway(const string& strHostTmpGateway)
    {
        hostTmpGateway = strHostTmpGateway;
    }
    string& GetHostTmpGateway()
    {
        return hostTmpGateway;
    }
    const string& GetHostTmpGateway() const
    {
        return hostTmpGateway;
    }

    void SetHostIpList(const vector<string>& cHostIpList)
    {
        hostIpList = cHostIpList;
    }
    vector<string>& GetHostIpList()
    {
        return hostIpList;
    }
    const vector<string>& GetHostIpList() const
    {
        return hostIpList;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};



class CMirrorBakConfSetReq:public CEventReq
{
public:
	CMirrorBakConfSetReq()
	{
		event = OspExtEventDesc(PMS_MIRROR_BAK_CONF_SET_REQ);
	}
private:
	int mirrorNum; //镜像备份个数              
	string mirrorPath; //镜像备份路径，如果是USB备份，路径则为：/opt/kdm/cpconf              
public:
    void SetMirrorNum(int nMirrorNum)
    {
        mirrorNum = nMirrorNum;
    }
    int GetMirrorNum() const
    {
        return mirrorNum;
    }

    void SetMirrorPath(const string& strMirrorPath)
    {
        mirrorPath = strMirrorPath;
    }
    string& GetMirrorPath()
    {
        return mirrorPath;
    }
    const string& GetMirrorPath() const
    {
        return mirrorPath;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CMirrorBakConfSetRsp:public CEventRsp
{
public:
	CMirrorBakConfSetRsp()
	{
		event = OspExtEventDesc(PMS_MIRROR_BAK_CONF_SET_RSP);
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CMirrorBakConfGetReq:public CEventReq
{
public:
	CMirrorBakConfGetReq()
	{
		event = OspExtEventDesc(PMS_MIRROR_BAK_CONF_GET_REQ);
	}

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CMirrorBakConfGetRsp:public CEventRsp
{
public:
	CMirrorBakConfGetRsp()
	{
		event = OspExtEventDesc(PMS_MIRROR_BAK_CONF_GET_RSP);
	}
private:
	int mirrorNum;    //镜像备份个数                         
	string mirrorPath;//镜像备份路径，如果是/opt/kdm/cpconf,则为USB备份                             
public:
    void SetMirrorNum(int nMirrorNum)
    {
        mirrorNum = nMirrorNum;
    }
    int GetMirrorNum() const
    {
        return mirrorNum;
    }

    void SetMirrorPath(const string& strMirrorPath)
    {
        mirrorPath = strMirrorPath;
    }
    string& GetMirrorPath()
    {
        return mirrorPath;
    }
    const string& GetMirrorPath() const
    {
        return mirrorPath;
    }


public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CMirrorBakFileListGetReq:public CEventReq
{
public:
	CMirrorBakFileListGetReq()
	{
		event = OspExtEventDesc(PMS_MIRROR_BAK_FILE_LIST_GET_REQ);
	}

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CMirrorBakFileListGetRsp:public CEventRsp
{
public:
	CMirrorBakFileListGetRsp()
	{
		event = OspExtEventDesc(PMS_MIRROR_BAK_FILE_LIST_GET_RSP);
	}
private:
	vector<string> mirrorBakFileList;//备份的镜像文件列表               
public:
    void SetMirrorBakFileList(const vector<string>& cMirrorBakFileList)
    {
        mirrorBakFileList = cMirrorBakFileList;
    }
    vector<string>& GetMirrorBakFileList()
    {
        return mirrorBakFileList;
    }
    const vector<string>& GetMirrorBakFileList() const
    {
        return mirrorBakFileList;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CPmsTcpdumpStartReq:public CEventReq
{
public:
	CPmsTcpdumpStartReq()
	{
		event = OspExtEventDesc(PMS_TCPDUMP_START_REQ);
	}
private:
	string method;//抓取方式：alltcpdump（全部抓取）、parttcpdump（信令抓取）                              

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetMethod(const string& strMethod)
    {
        method = strMethod;
    }
    string& GetMethod()
    {
        return method;
    }
    const string& GetMethod() const
    {
        return method;
    }

};


class CPmsTcpdumpStartRsp:public CEventRsp
{
public:
	CPmsTcpdumpStartRsp()
	{
		event = OspExtEventDesc(PMS_TCPDUMP_START_RSP);
	}

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CPmsTcpdumpStopReq:public CEventReq
{
public:
	CPmsTcpdumpStopReq()
	{
		event = OspExtEventDesc(PMS_TCPDUMP_STOP_REQ);
	}

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CPmsTcpdumpStopRsp:public CEventRsp
{
public:
	CPmsTcpdumpStopRsp()
	{
		event = OspExtEventDesc(PMS_TCPDUMP_STOP_RSP);
	}

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CPmsTcpdumpDownloadReq:public CEventReq
{
public:
	CPmsTcpdumpDownloadReq()
	{
		event = OspExtEventDesc(PMS_TCPDUMP_DOWNLOAD_REQ);
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CPmsTcpdumpDownloadRsp:public CEventRsp
{
public:
	CPmsTcpdumpDownloadRsp()
	{
		event = OspExtEventDesc(PMS_TCPDUMP_DOWNLOAD_RSP);
	}

private:
	string filepath;//抓包文件的路径               
public:
    void SetFilepath(const string& strFilepath)
    {
        filepath = strFilepath;
    }
    string& GetFilepath()
    {
        return filepath;
    }
    const string& GetFilepath() const
    {
        return filepath;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CPmsHeartbeatHostConfReq: public CEventReq
{
public:
	CPmsHeartbeatHostConfReq()
	{
		event = OspExtEventDesc(PMS_HEARTBEAT_HOST_CONF_REQ);
	}

private:
	string backupPcIp; //备机IP              

public:

    void SetBackupPcIp(const string& strBackupPcIp)
    {
        backupPcIp = strBackupPcIp;
    }
    string& GetBackupPcIp()
    {
        return backupPcIp;
    }
    const string& GetBackupPcIp() const
    {
        return backupPcIp;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CPmsHeartbeatHostConfRsp: public CEventRsp
{
public:
	CPmsHeartbeatHostConfRsp()
	{
		event = OspExtEventDesc(PMS_HEARTBEAT_HOST_CONF_RSP);
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CPmsHeartbeatBakpcConfReq: public CEventReq
{
public:
	CPmsHeartbeatBakpcConfReq()
	{
		event = OspExtEventDesc(PMS_HEARTBEAT_BAKPC_CONF_REQ);
	}
private:
	string referencePcIp;      //参照机IP               
	string hostTmpIp;          //主机恢复时采用的临时IP               
	string hostTmpMask;        //主机恢复时采用的临时掩码     
	string hostTmpGateway;     //主机恢复时采用的临时网关          
	vector<string> hostIpList; //主机IP              
public:
    void SetReferencePcIp(const string& strReferencePcIp)
    {
        referencePcIp = strReferencePcIp;
    }
    string& GetReferencePcIp()
    {
        return referencePcIp;
    }
    const string& GetReferencePcIp() const
    {
        return referencePcIp;
    }

    void SetHostTmpIp(const string& strHostTmpIp)
    {
        hostTmpIp = strHostTmpIp;
    }
    string& GetHostTmpIp()
    {
        return hostTmpIp;
    }
    const string& GetHostTmpIp() const
    {
        return hostTmpIp;
    }

    void SetHostTmpMask(const string& strHostTmpMask)
    {
        hostTmpMask = strHostTmpMask;
    }
    string& GetHostTmpMask()
    {
        return hostTmpMask;
    }
    const string& GetHostTmpMask() const
    {
        return hostTmpMask;
    }

    void SetHostTmpGateway(const string& strHostTmpGateway)
    {
        hostTmpGateway = strHostTmpGateway;
    }
    string& GetHostTmpGateway()
    {
        return hostTmpGateway;
    }
    const string& GetHostTmpGateway() const
    {
        return hostTmpGateway;
    }

    void SetHostIplist(const vector<string>& cHostIpList)
    {
        hostIpList = cHostIpList;
    }
    vector<string>& GetHostIplist()
    {
        return hostIpList;
    }
    const vector<string>& GetHostIplist() const
    {
        return hostIpList;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CPmsHeartbeatBakpcConfRsp: public CEventRsp
{
public:
	CPmsHeartbeatBakpcConfRsp()
	{
		event = OspExtEventDesc(PMS_HEARTBEAT_BAKPC_CONF_RSP);
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPmsDefaultMirrorMakeProgressNtfReq: public CEventReq
{
private:
	CProgressNtfBase progressNtf;                                
public:
	CPmsDefaultMirrorMakeProgressNtfReq()
	{
		event = OspExtEventDesc(PMS_DEFAULT_MIRROR_MAKE_PROGRESS_NTF_REQ);
	}

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetProgressNtf(const CProgressNtfBase& cProgressNtf)
    {
        progressNtf = cProgressNtf;
    }
    CProgressNtfBase& GetProgressNtf()
    {
        return progressNtf;
    }
    const CProgressNtfBase& GetProgressNtf() const
    {
        return progressNtf;
    }

};


class CPmsDefaultMirrorMakeProgressNtfRsp: public CEventRsp
{
public:
	CPmsDefaultMirrorMakeProgressNtfRsp()
	{
		event = OspExtEventDesc(PMS_DEFAULT_MIRROR_MAKE_PROGRESS_NTF_RSP);
	}

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CPmsDefaultMirrorMakeAck: public CEventReq
{
public:
	CPmsDefaultMirrorMakeAck()
	{
		event = OspExtEventDesc(PMS_DEFAULT_MIRROR_MAKE_ACK);
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CPmsDefaultMirrorMakeEndReq: public CEventReq 
{
public:
	CPmsDefaultMirrorMakeEndReq()
	{
		event = OspExtEventDesc(PMS_DEFAULT_MIRROR_MAKE_END_REQ);
	}

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CPmsDefaultMirrorMakeEndRsp: public CEventRsp
{
public:
	CPmsDefaultMirrorMakeEndRsp()
	{
		event = OspExtEventDesc(PMS_DEFAULT_MIRROR_MAKE_END_RSP);
	}

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CPmsIptablesSetReq: public CEventReq
{
public:
	CPmsIptablesSetReq()
	{
		event = OspExtEventDesc(PMS_IPTABLES_SET_REQ);
	}
private:
	string        service;   //白名单需要配置的服务：ssh、mysql                             
	vector<string> ipList;   //加入白名单的IP列表                          

public:
    void SetService(const string& strService)
    {
        service = strService;
    }
    string& GetService()
    {
        return service;
    }
    const string& GetService() const
    {
        return service;
    }

    void SetIpList(const vector<string>& cIpList)
    {
        ipList = cIpList;
    }
    vector<string>& GetIpList()
    {
        return ipList;
    }
    const vector<string>& GetIpList() const
    {
        return ipList;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CPmsIptablesSetRsp: public CEventRsp
{
public:
	CPmsIptablesSetRsp()
	{
		event = OspExtEventDesc(PMS_IPTABLES_SET_RSP);
	}

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CPmsIptablesGetReq: public CEventReq
{
public:
	CPmsIptablesGetReq()
	{
		event = OspExtEventDesc(PMS_IPTABLES_GET_REQ);
	}
private:
	string   service;   //获取的白名单服务名：ssh、mysql                              

public:
    void SetService(const string& strService)
    {
        service = strService;
    }
    string& GetService()
    {
        return service;
    }
    const string& GetService() const
    {
        return service;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CPmsIptablesGetRsp: public CEventRsp
{
public:
	CPmsIptablesGetRsp()
	{
		event = OspExtEventDesc(PMS_IPTABLES_GET_RSP);
	}
private:
	string         service;  //白名单服务名：ssh、mysql                             
	vector<string> ipList;   //加入该白名单的IP列表                          

public:
    void SetService(const string& strService)
    {
        service = strService;
    }
    string& GetService()
    {
        return service;
    }
    const string& GetService() const
    {
        return service;
    }

    void SetIpList(const vector<string>& cIpList)
    {
        ipList = cIpList;
    }
    vector<string>& GetIpList()
    {
        return ipList;
    }
    const vector<string>& GetIpList() const
    {
        return ipList;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};




class CPmsQrcodeImgUpdateReq: public CEventReq
{
public:
	CPmsQrcodeImgUpdateReq()
	{
		event = OspExtEventDesc(PMS_QRCODE_IMG_UPDATE_REQ);
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CPmsQrcodeImgUpdateRsp: public CEventRsp
{	
public:
	CPmsQrcodeImgUpdateRsp()
	{
		event = OspExtEventDesc(PMS_QRCODE_IMG_UPDATE_RSP);
	}
private:
	string filePath;                              

public:
    void SetFilePath(const string& strFilePath)
    {
        filePath = strFilePath;
    }
    string& GetFilePath()
    {
        return filePath;
    }
    const string& GetFilePath() const
    {
        return filePath;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};



class CPmsUserAddReq: public CEventReq
{
public:
    CPmsUserAddReq()
    {
        event = OspExtEventDesc(PMS_USER_ADD_REQ);
    }
    const CPmsUserInfo& GetInfo()const
    {
        return info;
    }
    CPmsUserInfo& GetInfo()
    {
        return info;
    }
    void SetInfo(const CPmsUserInfo& tInfo)
    {
        info = tInfo;
    }
private:
    CPmsUserInfo info;

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPmsUserAddRsp: public CEventRsp
{
public:
    CPmsUserAddRsp()
    {
        event = OspExtEventDesc(PMS_USER_ADD_RSP);
    }

    const CPmsUserInfoKey& GetKey()const
    {
        return key;
    }
    CPmsUserInfoKey& GetKey()
    {
        return key;
    }
    void SetKey(const CPmsUserInfoKey& tKey)
    {
        key = tKey;
    }

private:
    CPmsUserInfoKey key;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPmsUserDelReq: public CEventReq
{
public:
    CPmsUserDelReq()
    {
        event = OspExtEventDesc(PMS_USER_DEL_REQ);
    }

    const CPmsUserInfoKey& GetKey()const
    {
        return key;
    }
    CPmsUserInfoKey& GetKey()
    {
        return key;
    }
    void SetKey(const CPmsUserInfoKey& tKey)
    {
        key = tKey;
    }
private:
    CPmsUserInfoKey key;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPmsUserDelRsp: public CEventRsp
{
public:
    CPmsUserDelRsp()
    {
        event = OspExtEventDesc(PMS_USER_DEL_RSP);
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

class CPmsUserModReq: public CEventReq
{
public:
    CPmsUserModReq()
    {
        event = OspExtEventDesc(PMS_USER_MOD_REQ);
    } 
    const CPmsUserInfo& GetInfo()const    
    {
        return info;
    }
    CPmsUserInfo& GetInfo()
    {
        return info;
    }
    void SetInfo(const CPmsUserInfo& tInfo)
    {
        info = tInfo;
    }

private:
    CPmsUserInfo info;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPmsUserModRsp: public CEventRsp
{
public:
    CPmsUserModRsp()
    {
        event = OspExtEventDesc(PMS_USER_MOD_RSP);
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

class CPmsUserQryReq: public CQueryReqBase
{
public:
    CPmsUserQryReq()
    {
        event = OspExtEventDesc(PMS_USER_QRY_REQ);
    } 
    const CPmsUserInfoQueryCondition& GetCondition() const
    {
        return condition;
    }
    CPmsUserInfoQueryCondition& GetCondition()
    {
        return condition;
    }
    void SetConditon(const CPmsUserInfoQueryCondition& tCondition)
    {
        condition = tCondition;
    }

private:
    CPmsUserInfoQueryCondition condition;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPmsUserQryRsp: public CQueryRspBase
{
public:
    CPmsUserQryRsp()
    {
        event = OspExtEventDesc(PMS_USER_QRY_RSP);
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

class CPmsUserQryNtfReq: public CQueryNtfReqBase
{
public:
    CPmsUserQryNtfReq()
    {
        event = OspExtEventDesc(PMS_USER_QRY_NTF_REQ);
    }
    const vector<CPmsUserInfo>& GetInfos()const
    {
        return infos;
    }
    vector<CPmsUserInfo>& GetInfos()
    {
        return infos;
    }
    void SetInfos(const vector<CPmsUserInfo>& tInfos)
    {
        infos = tInfos;
    }

private:
    vector<CPmsUserInfo> infos;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPlatformHardwareInfoQryReq: public CEventReq
{
public:
    CPlatformHardwareInfoQryReq()
    {
        event = OspExtEventDesc(PMS_PLATFORM_HARDWAREINFO_QRY_REQ);
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

class CPlatformHardwareInfoQryRsp: public CEventRsp
{
public:
    CPlatformHardwareInfoQryRsp()
    {
        event = OspExtEventDesc(PMS_PLATFORM_HARDWAREINFO_QRY_RSP);
        cpuUsePercent  = 0;
        memUsePercent  = 0;
        sysLoadPercent = 0;
        cpuNum = 0;
    } 

    const string& GetType()const
    {
        return type;
    }
    string& GetType()
    {
        return type;
    }
    void SetType(const string& strType)
    {
        type = strType;
    }

    const string& GetVersion()const
    {
        return version;
    }
    string& GetVersion()
    {
        return version;
    }
    void SetVersion(const string& strVersion)
    {
        version = strVersion;
    }

    const int GetCpuUserPercent()const
    {
        return cpuUsePercent;
    }
    int GetCpuUserPrecent()
    {
        return cpuUsePercent;
    }
    void SetCpuUsePercent(const int dwCpuUsePrecent)
    {
        cpuUsePercent = dwCpuUsePrecent;
    }

    const int GetMemUsePrecent()const
    {
        return memUsePercent;
    }
    int GetMemUsePercent()
    {
        return memUsePercent;
    }
    void SetMemUsePercent(const int dwMemUsePercent)
    {
        memUsePercent = dwMemUsePercent;
    }

    const int GetSysLoadPercent()const
    {
        return sysLoadPercent;
    }
    int GetSysLoadPercent()
    {
        return sysLoadPercent;
    }
    void SetSysLoadPercent(const int dwSysLoadPercent)
    {
        sysLoadPercent = dwSysLoadPercent;
    }

    const string& GetCurrentTime() const
    {
        return currentTime;
    }
    string& GetCurrentTime()
    {
        return currentTime;
    }
    void SetCurrentTime(const string& strCurrentTime)
    {
        currentTime = strCurrentTime;
    }

private:    
    string type;         // 硬件类型
    string version;      // 硬件版本
    int cpuUsePercent;   // CPU使用率(0-100)
    int memUsePercent;   // 内存使用率(0-100)
    int sysLoadPercent;	 // 系统负荷(0-100)
    string currentTime;  // 系统时间
    int cpuNum;          // 内核个数
    vector<int> cpuUseRateArr; // 每个内核的使用率
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetCpuNum(int nCpuNum)
    {
        cpuNum = nCpuNum;
    }
    int GetCpuNum() const
    {
        return cpuNum;
    }

    void SetCpuUseRateArr(const vector<int>& cCpuUseRateArr)
    {
        cpuUseRateArr = cCpuUseRateArr;
    }
    vector<int>& GetCpuUseRateArr()
    {
        return cpuUseRateArr;
    }
    const vector<int>& GetCpuUseRateArr() const
    {
        return cpuUseRateArr;
    }

};



class CPlatformSoftwareInfoQryReq: public CEventReq
{
public:
    CPlatformSoftwareInfoQryReq()
    {
        event = OspExtEventDesc(PMS_PLATFORM_SOFTWAREINFO_QRY_REQ);
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

class CPlatformSoftwareInfoQryRsp: public CEventRsp
{
public:
    CPlatformSoftwareInfoQryRsp()
    {
        event = OspExtEventDesc(PMS_PLATFORM_SOFTWAREINFO_QRY_RSP);
    }
    const CPlatformSoftwareInfo& GetInfo()const 
    {
        return info;
    }
    CPlatformSoftwareInfo& GetInfo() 
    {
        return info;
    }
    void SetInfo(const CPlatformSoftwareInfo& tPlatformSoftwareInfo)
    {
        info = tPlatformSoftwareInfo;
    }

private:    
    CPlatformSoftwareInfo info;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPlatformSoftwareInfoModReq: public CEventReq
{
public:
    CPlatformSoftwareInfoModReq()
    {
        event = OspExtEventDesc(PMS_PLATFORM_SOFTWAREINFO_MOD_REQ);
    }

    bool GetDomainName(string &val)const
    {
        if (optionField.HasOption("domainName"))
        {
            val = domainName;
            return true;
        }
        else
        {
            return false;
        }
    }
    void SetDomainName(const string& val)
    {
        optionField.SetOption("domainName");
        domainName = val;
    }
    bool GetProxyAddr(TNetAddr &val)const
    {
        if (optionField.HasOption("proxyAddr"))
        {
            val = proxyAddr;
            return true;
        }
        else
        {
            return false;
        }
    }
    void SetProxyAddr(const TNetAddr& val)
    {
        optionField.SetOption("proxyAddr");
        proxyAddr = val;
    }
    bool GetParentProxyAddr(TNetAddr &val)const
    {
        if (optionField.HasOption("parentProxyAddr"))
        {
            val = parentProxyAddr;
            return true;
        }
        else
        {
            return false;
        }
    }
    void SetParentProxyAddr(const TNetAddr& val)
    {
        optionField.SetOption("parentProxyAddr");
        parentProxyAddr = val;
    }
    bool GetStunServerAddr(TNetAddr &val)const
    {
        if (optionField.HasOption("stunServerAddr"))
        {
            val = stunServerAddr;
            return true;
        }
        else
        {
            return false;
        }
    }
    void SetStunServerAddr(const TNetAddr& val)
    {
        optionField.SetOption("stunServerAddr");
        stunServerAddr = val;
    }
    bool GetHttpPort(int &val)const
    {
        if (optionField.HasOption("httpPort"))
        {
            val = httpPort;
            return true;
        }
        else
        {
            return false;
        }
    }
    void SetHttpPort(int val)
    {
        optionField.SetOption("httpPort");
        httpPort = val;
    }

private:
    CModOptionField optionField;

    string domainName;       // 平台所属域名
    TNetAddr proxyAddr;		             
    TNetAddr parentProxyAddr;
    TNetAddr stunServerAddr;
    string domainAlias;
    int httpPort;

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

public:
    void SetDomainAlias(const string& strDomainAlias)
    {
        optionField.SetOption("domainAlias");
        domainAlias = strDomainAlias;
    }
    bool GetDomainAlias(string &val)const
    {
        if (optionField.HasOption("domainAlias"))
        {
            val = domainName;
            return true;
        }
        else
        {
            return false;
        }
    }

};

class CPlatformSoftwareInfoModRsp: public CEventRsp
{
public:
    CPlatformSoftwareInfoModRsp()
    {
        event = OspExtEventDesc(PMS_PLATFORM_SOFTWAREINFO_MOD_RSP);
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

class CModuleStatusItemDelReq: public CEventReq
{
public:
    CModuleStatusItemDelReq()
    {
        event = OspExtEventDesc(PMS_MODULE_STATUS_ITEM_DEL_REQ);
    }

private:
    string moduleUri;                
public:
    void SetModuleUri(const string& strModuleUri)
    {
        moduleUri = strModuleUri;
    }
    string& GetModuleUri()
    {
        return moduleUri;
    }
    const string& GetModuleUri() const
    {
        return moduleUri;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CModuleStatusItemDelRsp: public CEventRsp
{
public:
    CModuleStatusItemDelRsp()
    {
        event = OspExtEventDesc(PMS_MODULE_STATUS_ITEM_DEL_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CSystemDiskInfoGetReq: public CEventReq
{
public:
    CSystemDiskInfoGetReq()
    {
        event = OspExtEventDesc(PMS_SYSTEM_DISK_INFO_GET_REQ);
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

class CSystemDiskInfoGetRsp: public CEventRsp
{
public:
    CSystemDiskInfoGetRsp()
    {
        event = OspExtEventDesc(PMS_SYSTEM_DISK_INFO_GET_RSP);
    }

    void SetInfos(const vector<TSystemPartInfo>& cSystemDisks)
    {
        infos = cSystemDisks;
    }
    vector<TSystemPartInfo>& GetInfos()
    {
        return infos;
    }
    const vector<TSystemPartInfo>& GetInfos() const
    {
        return infos;
    }

private:
    // 硬件型号为自家设备的时，返回CF卡和备份盘，否则只返回根文件系统所在磁盘的信息
    vector<TSystemPartInfo> infos;              

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CCtrlPmsReq: public CEventReq
{
public:    
    enum TCtrlCode
    {
        enCtrlType_Halt,        // 机器关机
        enCtrlType_Reboot,      // 机器重启
        enCtrlType_RemoveUsbBak // 移除Usb备份盘
    };
    CCtrlPmsReq()
    {
        event = OspExtEventDesc(PMS_CTRL_PMS_REQ);
        code = enCtrlType_Halt;
    }
    const TCtrlCode GetCode()const
    {
        return code;
    }
    TCtrlCode GetCode()
    {
        return code;
    }
    void SetCode(const TCtrlCode enTCtrlCode)
    {
        code = enTCtrlCode;
    }
    const string& GetArgument()const
    {
        return argument;
    }
    string& GetArgument()
    {
        return argument;
    }
    void SetArgument(const string& strArgument)
    {
        argument = strArgument;
    }

private:
    TCtrlCode code;  // 控制码
    string argument; // 控制参数
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CCtrlPmsRsp: public CEventRsp
{
public:
    CCtrlPmsRsp()
    {
        event = OspExtEventDesc(PMS_CTRL_PMS_RSP);
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


class CPlatformTimeInfoQryReq: public CEventReq
{
public:
    CPlatformTimeInfoQryReq()
    {
        event = OspExtEventDesc(PMS_PLATFORM_TIMEINFO_QRY_REQ);
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

class CPlatformTimeInfoQryRsp: public CEventRsp
{
public:
    CPlatformTimeInfoQryRsp()
    {
        event = OspExtEventDesc(PMS_PLATFORM_TIMEINFO_QRY_RSP);
    } 
    const string& GetCurrentDateTime()const
    {
        return currentDateTime;
    }
    string& GetCurrentDateTime()
    {
        return currentDateTime;
    }
    void SetCurrentDateTime(const string& strCurrentDateTime)
    {
        currentDateTime = strCurrentDateTime;
    }

    void SetPlatformTimeCfg(const CPlatformTimeInfo& cPlatformTimeCfg)
    {
        platformTimeCfg = cPlatformTimeCfg;
    }
    CPlatformTimeInfo& GetPlatformTimeCfg()
    {
        return platformTimeCfg;
    }
    const CPlatformTimeInfo& GetPlatformTimeCfg() const
    {
        return platformTimeCfg;
    }

private:    
    string currentDateTime;
    CPlatformTimeInfo platformTimeCfg;             
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPlatformTimeInfoModReq: public CEventReq
{
public:
    CPlatformTimeInfoModReq()
    {
        event = OspExtEventDesc(PMS_PLATFORM_TIMEINFO_MOD_REQ);
    } 

    void SetPlatformTimeCfg(const CPlatformTimeInfo& cPlatformTimeCfg)
    {
        platformTimeCfg = cPlatformTimeCfg;
    }
    CPlatformTimeInfo& GetPlatformTimeCfg()
    {
        return platformTimeCfg;
    }
    const CPlatformTimeInfo& GetPlatformTimeCfg() const
    {
        return platformTimeCfg;
    }

private:    
    CPlatformTimeInfo platformTimeCfg;             
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPlatformTimeInfoModRsp: public CEventRsp
{
public:
    CPlatformTimeInfoModRsp()
    {
        event = OspExtEventDesc(PMS_PLATFORM_TIMEINFO_MOD_RSP);
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


class CNetcardInfoQryReq: public CEventReq
{
public:
    CNetcardInfoQryReq()
    {
        event = OspExtEventDesc(PMS_NETCARDINFO_QRY_REQ);
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

class CNetcardInfoQryRsp: public CEventRsp
{
public:
    CNetcardInfoQryRsp()
    {
        event = OspExtEventDesc(PMS_NETCARDINFO_QRY_RSP);
    }    
    const vector<CNetcardInfo>& GetInfos()const
    {
        return infos;
    }
    vector<CNetcardInfo>& GetInfos()
    {
        return infos;
    }
    void SetInfos(const vector<CNetcardInfo>& tInfos)
    {
        infos = tInfos;
    }

private:    
    vector<CNetcardInfo> infos;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CNetcardInfoModReq: public CEventReq
{
public:
    CNetcardInfoModReq()
    {
        event = OspExtEventDesc(PMS_NETCARDINFO_MOD_REQ);
    } 
    const CNetcardInfo& GetInfo()const
    {
        return info;
    }
    CNetcardInfo& GetInfo()
    {
        return info;
    }
    void SetInfo(const CNetcardInfo& tInfo)
    {
        info = tInfo;
    }

private:    
    CNetcardInfo info; // 设置时CNetcardInfo中的网速信息无效，网速由工作模式决定
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CNetcardInfoModRsp: public CEventRsp
{
public:
    CNetcardInfoModRsp()
    {
        event = OspExtEventDesc(PMS_NETCARDINFO_MOD_RSP);
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


class CNetRouteAddReq: public CEventReq
{
public:
    CNetRouteAddReq()
    {
        event = OspExtEventDesc(PMS_NETROUTEINFO_ADD_REQ);
    } 
    const CNetRouteInfo& GetInfo()const
    {
        return info;
    }
    CNetRouteInfo& GetInfo()
    {
        return info;
    }
    void SetInfo(const CNetRouteInfo& tInfo)
    {
        info = tInfo;
    }

private:
    CNetRouteInfo info;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CNetRouteAddRsp: public CEventRsp
{
public:
    CNetRouteAddRsp()
    {
        event = OspExtEventDesc(PMS_NETROUTEINFO_ADD_RSP);
    } 
    const CNetRouteInfoKey& GetKey()const
    {
        return key;
    }
    CNetRouteInfoKey& GetKey()
    {
        return key;
    }
    void SetKey(const CNetRouteInfoKey&  tNetRouteInfoKey)
    {
        key = tNetRouteInfoKey;
    }

private:
    CNetRouteInfoKey key;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CNetRouteDelReq: public CEventReq
{
public:
    CNetRouteDelReq()
    {
        event = OspExtEventDesc(PMS_NETROUTEINFO_DEL_REQ);
    } 
    const CNetRouteInfoKey& GetKey()const
    {
        return key;
    }
    CNetRouteInfoKey& GetKey()
    {
        return key;
    }
    void SetKey(const CNetRouteInfoKey& tNetRouteInfoKey)
    {
        key = tNetRouteInfoKey;
    }

private:
    CNetRouteInfoKey key;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CNetRouteDelRsp: public CEventRsp
{
public:
    CNetRouteDelRsp()
    {
        event = OspExtEventDesc(PMS_NETROUTEINFO_DEL_RSP);
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

class CNetRouteQryReq: public CQueryReqBase
{
public:
    CNetRouteQryReq()
    {
        event = OspExtEventDesc(PMS_NETROUTEINFO_QRY_REQ);
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

class CNetRouteQryRsp: public CQueryRspBase
{
public:
    CNetRouteQryRsp()
    {
        event = OspExtEventDesc(PMS_NETROUTEINFO_QRY_RSP);
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

class CNetRouteQryNtfReq: public CQueryNtfReqBase
{
public:
    CNetRouteQryNtfReq()
    {
        event = OspExtEventDesc(PMS_NETROUTEINFO_QRY_NTF_REQ);
    }
    const vector<CNetRouteInfo>& GetInfos()const
    {
        return infos;
    }
    vector<CNetRouteInfo>& GetInfos()
    {
        return infos;
    }
    void SetInfos(const vector<CNetRouteInfo>& tNetRouteInfo)
    {
        infos = tNetRouteInfo;
    }

private:
    vector<CNetRouteInfo> infos;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CNetDefaultRouteGetReq: public CEventReq
{
public:
    CNetDefaultRouteGetReq()
    {
        event = OspExtEventDesc(PMS_NETDEFAULTROUTE_GET_REQ);
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

class CNetDefaultRouteGetRsp: public CEventRsp
{
public:
    CNetDefaultRouteGetRsp()
    {
        event = OspExtEventDesc(PMS_NETDEFAULTROUTE_GET_RSP);
    } 
    const CNetDefaultRoute& GetInfo() const
    {
        return info;
    }
    CNetDefaultRoute& GetInfo() 
    {
        return info;
    }
    void SetInfo(const CNetDefaultRoute& tInfo)
    {
        info = tInfo;
    }

private:    
    CNetDefaultRoute info;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CNetDefaultRouteSetReq: public CEventReq
{
public:
    CNetDefaultRouteSetReq()
    {
        event = OspExtEventDesc(PMS_NETDEFAULTROUTE_SET_REQ);
    }
    const CNetDefaultRoute& GetInfo() const
    {
        return info;
    }
    CNetDefaultRoute& GetInfo() 
    {
        return info;
    }
    void SetInfo(const CNetDefaultRoute& tInfo)
    {
        info = tInfo;
    }

private:    
    CNetDefaultRoute info;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CNetDefaultRouteSetRsp: public CEventRsp
{
public:
    CNetDefaultRouteSetRsp()
    {
        event = OspExtEventDesc(PMS_NETDEFAULTROUTE_SET_RSP);
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

class CNetNatAddReq: public CEventReq
{
public:
    CNetNatAddReq()
    {
        event = OspExtEventDesc(PMS_NETNAT_ADD_REQ);
    } 
    const CNetNatInfo& GetInfo() const
    {
        return info;
    }
    CNetNatInfo& GetInfo() 
    {
        return info;
    }
    void SetInfo(const CNetNatInfo& tInfo)
    {
        info = tInfo;
    }

private:
    CNetNatInfo info;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CNetNatAddRsp: public CEventRsp
{
public:
    CNetNatAddRsp()
    {
        event = OspExtEventDesc(PMS_NETNAT_ADD_RSP);
    }
    const CNetNatInfoKey& GetKey() const
    {
        return key;
    }
    CNetNatInfoKey& GetKey() 
    {
        return key;
    }
    void SetKey(const CNetNatInfoKey& tkey)
    {
        key = tkey;
    }

private:
    CNetNatInfoKey key;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CNetNatDelReq: public CEventReq
{
public:
    CNetNatDelReq()
    {
        event = OspExtEventDesc(PMS_NETNAT_DEL_REQ);
    }    
    const CNetNatInfoKey& GetKey() const
    {
        return key;
    }
    CNetNatInfoKey& GetKey() 
    {
        return key;
    }
    void SetKey(const CNetNatInfoKey& tkey)
    {
        key = tkey;
    }

private:
    CNetNatInfoKey key;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CNetNatDelRsp: public CEventRsp
{
public:
    CNetNatDelRsp()
    {
        event = OspExtEventDesc(PMS_NETNAT_DEL_RSP);
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

class CNetNatQryReq: public CQueryReqBase
{
public:
    CNetNatQryReq()
    {
        event = OspExtEventDesc(PMS_NETNAT_QRY_REQ);
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

class CNetNatQryRsp: public CQueryRspBase
{
public:
    CNetNatQryRsp()
    {
        event = OspExtEventDesc(PMS_NETNAT_QRY_RSP);
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

class CNetNatQryNtfReq: public CQueryNtfReqBase
{
public:
    CNetNatQryNtfReq()
    {
        event = OspExtEventDesc(PMS_NETNAT_QRY_NTF_REQ);
    }
    const vector<CNetNatInfo>& GetInfos() const
    {
        return infos;
    }
    vector<CNetNatInfo>& GetInfos() 
    {
        return infos;
    }
    void SetInfos(const vector<CNetNatInfo>& tInfos)
    {
        infos = tInfos;
    }

private:
    vector<CNetNatInfo> infos;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CPlatformTOPOQryReq: public CQueryReqBase
{
public:
    CPlatformTOPOQryReq()
    {
        event = OspExtEventDesc(PMS_PLATFORM_TOPO_QRY_REQ);
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

class CPlatformTOPOQryRsp: public CQueryRspBase
{
public:
    CPlatformTOPOQryRsp()
    {
        event = OspExtEventDesc(PMS_PLATFORM_TOPO_QRY_RSP);
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

class CPlatformTOPOQryNtfReq: public CQueryNtfReqBase
{
public:
    CPlatformTOPOQryNtfReq()
    {
        event = OspExtEventDesc(PMS_PLATFORM_TOPO_QRY_NTF_REQ);
    }
    const vector<CPlatformTOPO>& GetInfos() const
    {
        return infos;
    }
    vector<CPlatformTOPO>& GetInfos() 
    {
        return infos;
    }
    void SetInfos(const vector<CPlatformTOPO>& tInfos)
    {
        infos = tInfos;
    }

private:
    vector<CPlatformTOPO> infos;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPlatformTopoExternInfoGetReq: public CEventReq
{
public:
    CPlatformTopoExternInfoGetReq()
    {
        event = OspExtEventDesc(PMS_PLATFORM_TOPO_EXTERNINFO_GET_REQ);
        platformNetType = enPmsTopoNat_NoNat;
    }

    void SetPmsUserName(const string& strPmsUserName)
    {
        pmsUserName = strPmsUserName;
    }
    string& GetPmsUserName()
    {
        return pmsUserName;
    }
    const string& GetPmsUserName() const
    {
        return pmsUserName;
    }

    void SetPmsUserPassword(const string& strPmsUserPassword)
    {
        pmsUserPassword = strPmsUserPassword;
    }
    string& GetPmsUserPassword()
    {
        return pmsUserPassword;
    }
    const string& GetPmsUserPassword() const
    {
        return pmsUserPassword;
    }

private:
    string pmsUserName;           // 用户名称
    string pmsUserPassword;       // 用户密码

    TPlatformNetType platformNetType;             

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
public:
    void SetPlatformNetType(TPlatformNetType ePlatformNetType)
    {
        platformNetType = ePlatformNetType;
    }
    TPlatformNetType GetPlatformNetType() const
    {
        return platformNetType;
    }

};

class CPlatformTopoExternInfoGetRsp: public CEventRsp
{
public:
    CPlatformTopoExternInfoGetRsp()
    {
        event = OspExtEventDesc(PMS_PLATFORM_TOPO_EXTERNINFO_GET_RSP);
        httpPort = 80;
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
    string domainName;      // 域名
    UUIDString domainId;    // 域号
    UUIDString pmsId;       // pms用于SIP通信的模块Id
    int httpPort;           // http的监听端口                 
    
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
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

};

class CPlatformTOPOAddReq: public CEventReq
{
public:
    CPlatformTOPOAddReq()
    {
        event = OspExtEventDesc(PMS_PLATFORM_TOPO_ADD_REQ);
    } 
    const CPlatformTOPO& GetInfo() const
    {
        return info;
    }
    CPlatformTOPO& GetInfo() 
    {
        return info;
    }
    void SetInfo(const CPlatformTOPO& tInfo)
    {
        info = tInfo;
    }

private:
    CPlatformTOPO info;

    // 下面的字段适用于添加下级平台, 基本信息存储于info
    UUIDString subDoaminId;              // 下级域号

    // 下面的字段适用于设置下级平台的上级平台, 基本信息存储于info
    TNetAddr supperProxyAddr;               // 上级proxy地址 
    UUIDString supperDomainId;              // 上级域号
    string currentDomainName;               // 下级域名，用于组网时同时修改域名

    // 下面的字段适用于设置从平台的主平台, 基本信息存储于info
    TNetAddr masterProxyAddr;               // 主平台上的proxy地址
    UUIDString masterDoaminId;              // 主平台域号
    TNetAddr masterRedisAddr;               //主平台的redis地址


public:
    void SetSubDoaminId(const UUIDString& strSubDoaminId)
    {
        subDoaminId = strSubDoaminId;
    }
    UUIDString& GetSubDoaminId()
    {
        return subDoaminId;
    }
    const UUIDString& GetSubDoaminId() const
    {
        return subDoaminId;
    }

    void SetSupperProxyAddr(const TNetAddr& cSupperProxyAddr)
    {
        supperProxyAddr = cSupperProxyAddr;
    }
    TNetAddr& GetSupperProxyAddr()
    {
        return supperProxyAddr;
    }
    const TNetAddr& GetSupperProxyAddr() const
    {
        return supperProxyAddr;
    }

    void SetSupperDomainId(const UUIDString& strSupperDomainId)
    {
        supperDomainId = strSupperDomainId;
    }
    UUIDString& GetSupperDomainId()
    {
        return supperDomainId;
    }
    const UUIDString& GetSupperDomainId() const
    {
        return supperDomainId;
    }
    void SetCurrentDomainName(const string& strCurrentDomainName)
    {
        currentDomainName = strCurrentDomainName;
    }
    string& GetCurrentDomainName()
    {
        return currentDomainName;
    }
    const string& GetCurrentDomainName() const
    {
        return currentDomainName;
    }

    void SetMasterProxyAddr(const TNetAddr& cMasterProxyAddr)
    {
        masterProxyAddr = cMasterProxyAddr;
    }
    TNetAddr& GetMasterProxyAddr()
    {
        return masterProxyAddr;
    }
    const TNetAddr& GetMasterProxyAddr() const
    {
        return masterProxyAddr;
    }
    void SetMasterRedisAddr(const TNetAddr& cMasterRedisAddr)
    {
        masterRedisAddr = cMasterRedisAddr;
    }
    TNetAddr& GetMasterRedisAddr()
    {
        return masterRedisAddr;
    }
    const TNetAddr& GetMasterRedisAddr() const
    {
        return masterRedisAddr;
    }
    void SetMasterDoaminId(const UUIDString& val)
    {
        masterDoaminId = val;
    }
    UUIDString& GetMasterDoaminId()
    {
        return masterDoaminId;
    }
    const UUIDString& GetMasterDoaminId() const
    {
        return masterDoaminId;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

};

class CPlatformTOPOAddRsp:public CEventRsp
{
public:
    CPlatformTOPOAddRsp()
    {
        event = OspExtEventDesc(PMS_PLATFORM_TOPO_ADD_RSP);
    } 

private:
    // 上级平台添加下级平台或主平台添加从平台时返回本平台的一些信息
    // 用于设置下级平台的上级平台信息或者从平台的主平台信息
    TNetAddr proxyAddr;              
    UUIDString domainId;              
    TNetAddr  redisAddr;           
    string domainName;              
    string pmsId;              
    string platformIP; // ip:port形式的
    int httpPort;                

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
     void SetRedisAddr(const TNetAddr& cRedisAddr)
    {
        redisAddr = cRedisAddr;
    }
    TNetAddr& GetRedisAddr()
    {
        return redisAddr;
    }
    const TNetAddr& GetRedisAddr() const
    {
        return redisAddr;
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

    void SetPmsId(const string& strPmsId)
    {
        pmsId = strPmsId;
    }
    string& GetPmsId()
    {
        return pmsId;
    }
    const string& GetPmsId() const
    {
        return pmsId;
    }

    void SetPlatformIP(const string& strPlatformIP)
    {
        platformIP = strPlatformIP;
    }
    string& GetPlatformIP()
    {
        return platformIP;
    }
    const string& GetPlatformIP() const
    {
        return platformIP;
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

};

class CPlatformTOPOAddNtfReq: public CEventReq
{
public:
    CPlatformTOPOAddNtfReq()
    {
        event = OspExtEventDesc(PMS_PLATFORM_TOPO_ADD_NTF_REQ);
    } 
    const CPlatformTOPO& GetInfo() const
    {
        return info;
    }
    CPlatformTOPO& GetInfo() 
    {
        return info;
    }
    void SetInfo(const CPlatformTOPO& tInfo)
    {
        info = tInfo;
    }

private:    
    CPlatformTOPO info;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPlatformTOPOModReq: public CEventReq
{
public:
    CPlatformTOPOModReq()
    {
        event = OspExtEventDesc(PMS_PLATFORM_TOPO_MOD_REQ);
    } 

private:    
    string platformName;     // 待修改TOPO记录的老名字, 下面两个字段未改时请使用原值
    string newPlatformName;  // 支持修改：主平台上从平台TOPO记录的平台名称，上级的域名或主平台的域名
    string newPlatformIP;                 

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetPlatformName(const string& strPlatformName)
    {
        platformName = strPlatformName;
    }
    string& GetPlatformName()
    {
        return platformName;
    }
    const string& GetPlatformName() const
    {
        return platformName;
    }

    void SetNewPlatformName(const string& strNewPlatformName)
    {
        newPlatformName = strNewPlatformName;
    }
    string& GetNewPlatformName()
    {
        return newPlatformName;
    }
    const string& GetNewPlatformName() const
    {
        return newPlatformName;
    }

    void SetNewPlatformIP(const string& strNewPlatformIP)
    {
        newPlatformIP = strNewPlatformIP;
    }
    string& GetNewPlatformIP()
    {
        return newPlatformIP;
    }
    const string& GetNewPlatformIP() const
    {
        return newPlatformIP;
    }

};

class CPlatformTOPOModRsp: public CEventRsp
{
public:
    CPlatformTOPOModRsp()
    {
        event = OspExtEventDesc(PMS_PLATFORM_TOPO_MOD_RSP);
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

class CPlatformTOPOModNtfReq : public CEventReq
{
public:
    CPlatformTOPOModNtfReq()
    {
        event = OspExtEventDesc(PMS_PLATFORM_TOPO_MOD_NTF_REQ);
    }

private:    
    CPlatformTOPO platformTOPO;              
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetPlatformTOPO(const CPlatformTOPO& cPlatformTOPO)
    {
        platformTOPO = cPlatformTOPO;
    }
    CPlatformTOPO& GetPlatformTOPO()
    {
        return platformTOPO;
    }
    const CPlatformTOPO& GetPlatformTOPO() const
    {
        return platformTOPO;
    }

};

class CPlatformTOPODelReq : public CEventReq
{
public:
    CPlatformTOPODelReq()
    {
        event = OspExtEventDesc(PMS_PLATFORM_TOPO_DEL_REQ);
    }  

    const string& GetPlatformName() const
    {
        return platformName;
    }
    string& GetPlatformName() 
    {
        return platformName;
    }
    void SetPlatformName(const string &strName)
    {
        platformName = strName;
    }
private:    
    string  platformName;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPlatformTOPODelRsp : public CEventRsp
{
public:
    CPlatformTOPODelRsp()
    {
        event = OspExtEventDesc(PMS_PLATFORM_TOPO_DEL_RSP);
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

class CPlatformTOPODelNtfReq : public CEventReq
{
public:
    CPlatformTOPODelNtfReq()
    {
        event = OspExtEventDesc(PMS_PLATFORM_TOPO_DEL_NTF_REQ);
    }
    const string& GetPlatformName() const
    {
        return platformName;
    }
    string& GetPlatformName() 
    {
        return platformName;
    }
    void SetPlatformName(const string &strName)
    {
        platformName = strName;
    }
	void SetResult(int dwResult)
	{
		result=dwResult;
	}
	int GetResult()
	{
		return result;
	}
    const string& GetErrorDesc() const
    {
        return errordesc;
    }
    string& GetErrorDesc()
    {
        return errordesc;
    }
    void SetErrorDesc(const string& strErrorDesc)
    {
        errordesc = strErrorDesc;
    }
private:
    string  platformName;
    int result;
    string errordesc;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPlatformTOPOSyncReq : public CEventReq
{
public:
    CPlatformTOPOSyncReq()
    {
        event = OspExtEventDesc(PMS_PLATFORM_TOPO_SYNC_REQ);
    }
    void SetPlatformTOPO(const CPlatformTOPO& cPlatformTOPO)
    {
        platformTOPO = cPlatformTOPO;
    }
    CPlatformTOPO& GetPlatformTOPO()
    {
        return platformTOPO;
    }
    const CPlatformTOPO& GetPlatformTOPO() const
    {
        return platformTOPO;
    }
private:
    CPlatformTOPO platformTOPO;  // 利用pmsId作为标识

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPlatformTOPOSyncRsp : public CEventRsp
{
public:
    CPlatformTOPOSyncRsp()
    {
        event = OspExtEventDesc(PMS_PLATFORM_TOPO_SYNC_RSP);
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

class CPlatformModuleInfoQryReq: public CQueryReqBase
{
public:
    CPlatformModuleInfoQryReq()
    {
        event = OspExtEventDesc(PMS_PLATFORM_MODULEINFO_QRY_REQ);
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

class CPlatformModuleInfoQryRsp: public CQueryRspBase
{
public:
    CPlatformModuleInfoQryRsp()
    {
        event = OspExtEventDesc(PMS_PLATFORM_MODULEINFO_QRY_RSP);
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

class CPlatformModuleInfoQryNtfReq: public CQueryNtfReqBase
{
public:
    CPlatformModuleInfoQryNtfReq()
    {
        event = OspExtEventDesc(PMS_PLATFORM_MODULEINFO_QRY_NTF_REQ);
    }  
    const vector<CPlatformModuleInfo>& GetInfos()const
    {
        return infos;
    }
    vector<CPlatformModuleInfo>& GetInfos()
    {
        return infos;
    }
    void SetInfos(const vector<CPlatformModuleInfo>& tInfos)
    {
        infos = tInfos;
    }

private:  
    vector<CPlatformModuleInfo> infos;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPlatformModuleInfoModReq: public CEventReq
{
public:
    CPlatformModuleInfoModReq()
    {
        event = OspExtEventDesc(PMS_PLATFORM_MODULEINFO_MOD_REQ);
    }  

    const vector<CPlatformModuleInfo>& GetInfos()const
    {
        return infos;
    }
    vector<CPlatformModuleInfo>& GetInfos()
    {
        return infos;
    }
    void SetInfo(const vector<CPlatformModuleInfo>& tInfos)
    {
        infos = tInfos;
    }
private:    
    // 只允许修改模块启停状态
    vector<CPlatformModuleInfo> infos;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPlatformModuleInfoModRsp: public CEventRsp
{
public:
    CPlatformModuleInfoModRsp()
    {
        event = OspExtEventDesc(PMS_PLATFORM_MODULEINFO_MOD_RSP);
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

//打包文名是DevCfg.kcf,存放在/opt/kdm/cpconf目录下
class CMirrorExportBeginReq: public CEventReq
{
public:
    CMirrorExportBeginReq()
    {
        event = OspExtEventDesc(PMS_MIRROR_EXPORT_BEGIN_REQ);
        type  = enExportType_Trans;
    } 
    enum TExportType
    {
        enExportType_Trans,       // 导出并发送至客户端，镜像不在服务器上保留
        enExportType_Trans_Leave, // 导出并发送至客户端，镜像在服务器上保留
        enExportType_NoTrans      // 导出在服务器，镜像不发送至客户端(网速较慢时可以直接利用U盘本地导出镜像)
    };
    const TExportType GetType()const 
    {
        return type;
    }
    TExportType GetType()
    {
        return type;
    }
    void SetType(const TExportType enType)
    {
        type = enType;
    }   

private:
    TExportType type;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CMirrorExportBeginRsp: public CEventRsp
{
public:
    CMirrorExportBeginRsp()
    {
        event = OspExtEventDesc(PMS_MIRROR_EXPORT_BEGIN_RSP);
    }    
private:
    string filename;             
public:
    void SetFilename(const string& strFilename)
    {
        filename = strFilename;
    }
    string& GetFilename()
    {
        return filename;
    }
    const string& GetFilename() const
    {
        return filename;
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

class CMirrorExportBeginAckReq: public CEventReq
{
public:
    CMirrorExportBeginAckReq()
    {
        event = OspExtEventDesc(PMS_MIRROR_EXPORT_BEGIN_ACK);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CMirrorExportEndReq: public CEventReq
{
public:
    CMirrorExportEndReq()
    {
        event = OspExtEventDesc(PMS_MIRROR_EXPORT_END_REQ);
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

class CMirrorExportEndRsp: public CEventRsp
{
public:
    CMirrorExportEndRsp()
    {
        event = OspExtEventDesc(PMS_MIRROR_EXPORT_END_RSP);
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

class CMirrorExportProgressNtfReq: public CEventReq
{
public:
    CMirrorExportProgressNtfReq()
    {
        event = OspExtEventDesc(PMS_MIRROR_EXPORT_PROGRESS_NTF_REQ);
    } 

    const CProgressNtfBase& GetProgressNtf()const 
    {
        return progressNtf;
    }
    CProgressNtfBase& GetProgressNtf()
    {
        return progressNtf;
    }
    void SetProgressNtf(const CProgressNtfBase& tProgressNtf)
    {
        progressNtf = tProgressNtf;
    }

private:
    CProgressNtfBase progressNtf;       
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CMirrorImportBeginReq: public CEventReq
{
public:
    CMirrorImportBeginReq()
    {
        event = OspExtEventDesc(PMS_MIRROR_IMPORT_BEGIN_REQ);
        type = enImportType_file;
    }
    enum TImportType
    {
        enImportType_file,       // 从文件导入
        enImportType_backup_disk, // 从备份盘导入
    };
private:
    TImportType type;
	string mirrorBakFile;               
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetType(TImportType eType)
    {
        type = eType;
    }
    TImportType GetType() const
    {
        return type;
    }

    void SetMirrorBakFile(const string& strMirrorBakFile)
    {
        mirrorBakFile = strMirrorBakFile;
    }
    string& GetMirrorBakFile()
    {
        return mirrorBakFile;
    }
    const string& GetMirrorBakFile() const
    {
        return mirrorBakFile;
    }

};

class CMirrorImportBeginRsp: public CEventRsp
{
public:
    CMirrorImportBeginRsp()
    {
        event = OspExtEventDesc(PMS_MIRROR_IMPORT_BEGIN_RSP);
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

class CMirrorImportBeginAckReq: public CEventReq
{
public:
    CMirrorImportBeginAckReq()
    {
        event = OspExtEventDesc(PMS_MIRROR_IMPORT_BEGIN_ACK);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CMirrorImportEndReq: public CEventReq
{
public:
    CMirrorImportEndReq()
    {
        event = OspExtEventDesc(PMS_MIRROR_IMPORT_END_REQ);
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

class CMirrorImportEndRsp: public CEventRsp
{
public:
    CMirrorImportEndRsp()
    {
        event = OspExtEventDesc(PMS_MIRROR_IMPORT_END_RSP);
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

class CMirrorImportProgressNtfReq: public CEventReq
{
public:
    CMirrorImportProgressNtfReq()
    {
        event = OspExtEventDesc(PMS_MIRROR_IMPORT_PROGRESS_NTF_REQ);
    } 
    const CProgressNtfBase& GetProgressNtf()const 
    {
        return progressNtf;
    }
    CProgressNtfBase& GetProgressNtf()
    {
        return progressNtf;
    }
    void SetProgressNtf(const CProgressNtfBase& tProgressNtf)
    {
        progressNtf = tProgressNtf;
    }

private:
    CProgressNtfBase progressNtf;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CMirrorResetReq: public CEventReq
{
public:
    CMirrorResetReq()
    {
        event = OspExtEventDesc(PMS_MIRROR_RESET_REQ);
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

class CMirrorResetRsp: public CEventRsp
{
public:
    CMirrorResetRsp()
    {
        event = OspExtEventDesc(PMS_MIRROR_RESET_RSP);
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

class CMirrorResetAckReq : public CEventReq 
{
public:
    CMirrorResetAckReq()
    {
        event = OspExtEventDesc(PMS_MIRROR_RESET_ACK);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CMirrorResetProgressNtfReq: public CEventReq
{
public:
    CMirrorResetProgressNtfReq()
    {
        event = OspExtEventDesc(PMS_MIRROR_RESET_PROGRESS_NTF_REQ);
    }  
    const CProgressNtfBase& GetProgressNtf()const 
    {
        return progressNtf;
    }
    CProgressNtfBase& GetProgressNtf()
    {
        return progressNtf;
    }
    void SetProgressNtf(const CProgressNtfBase& tProgressNtf)
    {
        progressNtf = tProgressNtf;
    }

private:
    CProgressNtfBase progressNtf;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CMirrorUpdateBeginReq: public CEventReq
{
public:
    CMirrorUpdateBeginReq()
    {
        event = OspExtEventDesc(PMS_MIRROR_UPDATE_BEGIN_REQ);
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

class CMirrorUpdateBeginRsp: public CEventRsp
{
public:
    CMirrorUpdateBeginRsp()
    {
        event = OspExtEventDesc(PMS_MIRROR_UPDATE_BEGIN_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CMirrorUpdateBeginAckReq: public CEventReq
{
public:
	CMirrorUpdateBeginAckReq()
	{
		event = OspExtEventDesc(PMS_MIRROR_UPDATE_BEGIN_ACK);
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CMirrorUpdateEndReq: public CEventReq
{
public:
    CMirrorUpdateEndReq()
    {
        event = OspExtEventDesc(PMS_MIRROR_UPDATE_END_REQ);
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

class CMirrorUpdateEndRsp: public CEventRsp
{
public:
    CMirrorUpdateEndRsp()
    {
        event = OspExtEventDesc(PMS_MIRROR_UPDATE_END_RSP);
        rebootMachineFlag = false;
        rebootServiceFlag = false;
    }

private:
    bool rebootMachineFlag; // 机器需要重启
    bool rebootServiceFlag; // 服务需要重启

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetRebootMachineFlag(bool bRebootMachineFlag)
    {
        rebootMachineFlag = bRebootMachineFlag;
    }
    bool GetRebootMachineFlag() const
    {
        return rebootMachineFlag;
    }

    void SetRebootServiceFlag(bool bRebootServiceFlag)
    {
        rebootServiceFlag = bRebootServiceFlag;
    }
    bool GetRebootServiceFlag() const
    {
        return rebootServiceFlag;
    }

};

class CMirrorUpdateProgressNtfReq: public CEventReq
{
public:
    CMirrorUpdateProgressNtfReq()
    {
        event = OspExtEventDesc(PMS_MIRROR_UPDATE_PROGRESS_NTF_REQ);
    } 
    const CProgressNtfBase& GetProgressNtf()const 
    {
        return progressNtf;
    }
    CProgressNtfBase& GetProgressNtf()
    {
        return progressNtf;
    }
    void SetProgressNtf(const CProgressNtfBase& tProgressNtf)
    {
        progressNtf = tProgressNtf;
    }

private:
    CProgressNtfBase progressNtf;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPlatformLogInfoQryReq: public CQueryReqBase
{
public:
    CPlatformLogInfoQryReq()
    {
        event = OspExtEventDesc(PMS_PLATFORM_LOG_QRY_REQ);
    }

    const CPlatformLogInfoQryCondition& GetCondition()const
    {
        return condition;
    }
    CPlatformLogInfoQryCondition& GetCondition()
    {
        return condition;
    }
    void SetCondition(const CPlatformLogInfoQryCondition& tCondition)
    {
        condition = tCondition;
    }

private:
    CPlatformLogInfoQryCondition condition;

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CPlatformLogInfoQryRsp: public CQueryRspBase
{
public:
    CPlatformLogInfoQryRsp()
    {
        event = OspExtEventDesc(PMS_PLATFORM_LOG_QRY_RSP);
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

class CPlatformLogInfoNtfReq: public CQueryNtfReqBase
{
public:
    CPlatformLogInfoNtfReq() 
    {
        event = OspExtEventDesc(PMS_PLATFORM_LOG_QRY_NTF_REQ);
    } 
    const vector<CPlatformLogInfo>& GetInfos()const
    {
        return infos;
    }
    vector<CPlatformLogInfo>& GetInfos()
    {
        return infos;
    }
    void SetInfos(const vector<CPlatformLogInfo>& tInfos)
    {
        infos = tInfos;
    }
private:  
    vector<CPlatformLogInfo> infos;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUserOperationLogAddReq : public CEventReq
{
public:
    CUserOperationLogAddReq()
    {
        event = OspExtEventDesc(PMS_USER_OPERATION_LOG_ADD_REQ);
        CCMSTime cTime;
        strTime = cTime.ToString();
    }

private:
    string strTime;             
    string strDesc;             

public:
    void SetStrTime(const string& strStrTime)
    {
        strTime = strStrTime;
    }
    string& GetStrTime()
    {
        return strTime;
    }
    const string& GetStrTime() const
    {
        return strTime;
    }

    void SetStrDesc(const string& strStrDesc)
    {
        strDesc = strStrDesc;
    }
    string& GetStrDesc()
    {
        return strDesc;
    }
    const string& GetStrDesc() const
    {
        return strDesc;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUserOperationLogAddRsp: public CEventRsp
{
public:
    CUserOperationLogAddRsp()
    {
        event = OspExtEventDesc(PMS_USER_OPERATION_LOG_ADD_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUserOperationLogQryReq: public CQueryReqBase
{
public:
    CUserOperationLogQryReq()
    {
        event = OspExtEventDesc(PMS_USER_OPERATION_LOG_QRY_REQ);
    }

    const CUserOperationLogQryCondition& GetCondition() const
    {
        return condition;
    }
    CUserOperationLogQryCondition& GetCondition()
    {
        return condition;
    }
    void SetConditon(const CUserOperationLogQryCondition& tCondition)
    {
        condition = tCondition;
    }
private:
    CUserOperationLogQryCondition condition;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUserOperationLogQryRsp: public CQueryRspBase
{
public:
    CUserOperationLogQryRsp()
    {
        event = OspExtEventDesc(PMS_USER_OPERATION_LOG_QRY_RSP);
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

class CUserOperationLogNtfReq: public CQueryNtfReqBase
{
public:
    CUserOperationLogNtfReq() 
    {
        event = OspExtEventDesc(PMS_USER_OPERATION_LOG_QRY_NTF_REQ);
    } 
    const vector<CUserOperationLogInfo>& GetInfos()const
    {
        return infos;
    }
    vector<CUserOperationLogInfo>& GetInfos()
    {
        return infos;
    }
    void SetInfos(const vector<CUserOperationLogInfo>& tInfos)
    {
        infos = tInfos;
    }

private:  
    vector<CUserOperationLogInfo> infos;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CNMSDeviceQryReq: public CQueryReqBase
{
public:
    CNMSDeviceQryReq()
    {
        event = OspExtEventDesc(PMS_NMS_DEVICE_QRY_REQ);
    }
    void SetCondition(const CNMSDeviceQueryCondition& cCondition)
    {
        condition = cCondition;
    }
    CNMSDeviceQueryCondition& GetCondition()
    {
        return condition;
    }
    const CNMSDeviceQueryCondition& GetCondition() const
    {
        return condition;
    }
private:
    CNMSDeviceQueryCondition condition;              
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CNMSDeviceQryRsp: public CQueryRspBase
{
public:
    CNMSDeviceQryRsp()
    {
        event = OspExtEventDesc(PMS_NMS_DEVICE_QRY_RSP);
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

class CNMSDeviceQryNtfReq: public CQueryNtfReqBase
{
public:
    CNMSDeviceQryNtfReq()
    {
        event = OspExtEventDesc(PMS_NMS_DEVICE_QRY_NTF_REQ);
    } 
    const vector<CNMSDeviceInfo>& GetInfos()const
    {
        return infos;
    }
    vector<CNMSDeviceInfo>& GetInfos()
    {
        return infos;
    }
    void SetInfos(const vector<CNMSDeviceInfo>& tInfos)
    {
        infos = tInfos;
    }

private:
    vector<CNMSDeviceInfo> infos;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CNMSDeviceSyncNtfReq: public CEventReq
{
public:
    CNMSDeviceSyncNtfReq()
    {
        event = OspExtEventDesc(PMS_NMS_DEVICE_SYNC_NTF_REQ);
    }

    //ntfOpType
    const TNtfOpType& GetType()const
    {
        return ntfOpType;
    }
    TNtfOpType& GetType()
    {
        return ntfOpType;
    }
    void SetType(const TNtfOpType& val)
    {
        ntfOpType = val;
    }

    //info
    const CNMSDeviceInfo& GetInfo() const
    {
        return info;
    }
    CNMSDeviceInfo& GetInfo()
    {
        return info;
    }
    void SetInfo(const CNMSDeviceInfo& tInfo)
    {
        info = tInfo;
    }

private:    
    TNtfOpType       ntfOpType;
    CNMSDeviceInfo   info;

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CNMSAlarmQryReq: public CQueryReqBase
{
public:
    CNMSAlarmQryReq()
    {
        event = OspExtEventDesc(PMS_NMS_ALARM_QRY_REQ);
    }

    const CNmsAlarmQueryCondition& GetCondition()const
    {
        return condition;
    }
    CNmsAlarmQueryCondition& GetCondition()
    {
        return condition;
    }
    void SetCondition(const CNmsAlarmQueryCondition& tCondition)
    {
        condition = tCondition;
    }
    void SetRealTime(bool bRealTime)
    {
        realTime = bRealTime;
    }
    bool GetRealTime() const
    {
        return realTime;
    }

private:
    CNmsAlarmQueryCondition     condition;
    bool realTime;  // true表示实时告警，否则表示历史告警
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CNMSAlarmQryRsp: public CQueryRspBase
{
public:
    CNMSAlarmQryRsp()
    {
        event = OspExtEventDesc(PMS_NMS_ALARM_QRY_RSP);
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

class CNMSAlarmQryNtfReq: public CQueryNtfReqBase
{
public:
    CNMSAlarmQryNtfReq()
    {
        event = OspExtEventDesc(PMS_NMS_ALARM_QRY_NTF_REQ);
    } 
    const vector<CNMSAlarmInfo>& GetInfos()const
    {
        return infos;
    }
    vector<CNMSAlarmInfo>& GetInfos()
    {
        return infos;
    }
    void SetInfos(const vector<CNMSAlarmInfo>& tInfos)
    {
        infos = tInfos;
    }

private: 
    vector<CNMSAlarmInfo> infos;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CNMSAlarmModReq: public CEventReq
{
public:
    CNMSAlarmModReq()
    {
        event = OspExtEventDesc(PMS_NMS_ALARM_MOD_REQ);
    }

    //devId
    const UUIDString& GetDevId()const
    {
        return devId;
    }
    UUIDString& GetDevId()
    {
        return devId;
    }
    void SetDevID(const UUIDString& val)
    {
        devId = val;
    }
    void SetAlarmSn(int nAlarmSn)
    {
        alarmSn = nAlarmSn;
    }
    int GetAlarmSn() const
    {
        return alarmSn;
    }

    //alarmOp
    const TAlarmOp& GetAlarmOp()const
    {
        return alarmOp;
    }
    TAlarmOp& GetAlarmOp()
    {
        return alarmOp;
    }
    void SetAlarmOp(const TAlarmOp& val)
    {
        alarmOp = val;
    }

private:
    UUIDString  devId;
    int         alarmSn;              
    TAlarmOp    alarmOp;

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CNMSAlarmModRsp: public CEventRsp
{
public:
    CNMSAlarmModRsp()
    {
        event = OspExtEventDesc(PMS_NMS_ALARM_MOD_RSP);
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

class CNMSAlarmSyncNtfReq: public CEventReq
{
public:
    CNMSAlarmSyncNtfReq()
    {
        event = OspExtEventDesc(PMS_NMS_ALARM_SYNC_NTF_REQ);
    }

    //ntfOpType
    const TNtfOpType& GetType()const
    {
        return ntfOpType;
    }
    TNtfOpType& GetType()
    {
        return ntfOpType;
    }
    void SetType(const TNtfOpType& val)
    {
        ntfOpType = val;
    }

    //info
    const CNMSAlarmInfo& GetInfo()const
    {
        return info;
    }
    CNMSAlarmInfo& GetInfo()
    {
        return info;
    }
    void SetInfo(const CNMSAlarmInfo& tInfo)
    {
        info = tInfo;
    }

private:
    TNtfOpType      ntfOpType;
    CNMSAlarmInfo   info;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CProxyConfigInfoQryReq: public CEventReq
{
public:
    CProxyConfigInfoQryReq()
    {
        event = OspExtEventDesc(PMS_PROXY_CONFIGINFO_QRY_REQ);
    }
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CProxyConfigInfoQryRsp: public CEventRsp
{
public:
    CProxyConfigInfoQryRsp()
    {
        event = OspExtEventDesc(PMS_PROXY_CONFIGINFO_QRY_RSP);
    }
    const CProxyConfigInfo& GetInfo()const
    {
        return info;
    }
    CProxyConfigInfo& GetInfo()
    {
        return info;
    }
    void SetInfo(const CProxyConfigInfo& tInfo)
    {
        info = tInfo;
    }
private:
    CProxyConfigInfo info;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CProxyConfigInfoModReq: public CEventReq
{
public:
    CProxyConfigInfoModReq()
    {
        event = OspExtEventDesc(PMS_PROXY_CONFIGINFO_MOD_REQ);
    }
    const CProxyConfigInfo& GetInfo()const
    {
        return info;
    }
    CProxyConfigInfo& GetInfo()
    {
        return info;
    }
    void SetInfo(const CProxyConfigInfo& tInfo)
    {
        info = tInfo;
    }
private:
    CProxyConfigInfo info;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CProxyConfigInfoModRsp: public CEventRsp
{
public:
    CProxyConfigInfoModRsp()
    {
        event = OspExtEventDesc(PMS_PROXY_CONFIGINFO_MOD_RSP);
    }
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CSTUNServerConfigInfoQryReq: public CEventReq
{
public:
    CSTUNServerConfigInfoQryReq()
    {
        event = OspExtEventDesc(PMS_STUN_SERVER_CONFIGINFO_QRY_REQ);
    }
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CSTUNServerConfigInfoQryRsp: public CEventRsp
{
public:
    CSTUNServerConfigInfoQryRsp()
    {
        event = OspExtEventDesc(PMS_STUN_SERVER_CONFIGINFO_QRY_RSP);
    }
    const CSTUNServerConfigInfo& GetInfo()const
    {
        return info;
    }
    CSTUNServerConfigInfo& GetInfo()
    {
        return info;
    }
    void SetInfo(const CSTUNServerConfigInfo& tInfo)
    {
        info = tInfo;
    }
private:
    CSTUNServerConfigInfo info;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CSTUNServerConfigInfoModReq: public CEventReq
{
public:
    CSTUNServerConfigInfoModReq()
    {
        event = OspExtEventDesc(PMS_STUN_SERVER_CONFIGINFO_MOD_REQ);
    }
    const CSTUNServerConfigInfo& GetInfo()const
    {
        return info;
    }
    CSTUNServerConfigInfo& GetInfo()
    {
        return info;
    }
    void SetInfo(const CSTUNServerConfigInfo& tInfo)
    {
        info = tInfo;
    }
private:
    CSTUNServerConfigInfo info;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CSTUNServerConfigInfoModRsp: public CEventRsp
{
public:
    CSTUNServerConfigInfoModRsp()
    {
        event = OspExtEventDesc(PMS_STUN_SERVER_CONFIGINFO_MOD_RSP);
    }
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

typedef CPlatformTimeInfo CNTPServerConfigInfo;


class CNTPServerConfigInfoQryReq: public CEventReq
{
public:
    CNTPServerConfigInfoQryReq()
    {
        event = OspExtEventDesc(PMS_NTP_SERVER_CONFIGINFO_QRY_REQ);
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

class CNTPServerConfigInfoQryRsp: public CEventRsp
{
public:
    CNTPServerConfigInfoQryRsp()
    {
        event = OspExtEventDesc(PMS_NTP_SERVER_CONFIGINFO_QRY_RSP);
    }
    const CNTPServerConfigInfo& GetInfo()const
    {
        return info;
    }
    CNTPServerConfigInfo& GetInfo()
    {
        return info;
    }
    void SetInfo(CNTPServerConfigInfo& tInfo)
    {
        info = tInfo;
    }
private:
    CNTPServerConfigInfo info;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CNTPServerConfigInfoModReq: public CEventReq
{
public:
    CNTPServerConfigInfoModReq()
    {
        event = OspExtEventDesc(PMS_STUN_SERVER_CONFIGINFO_MOD_REQ);
    }
    const CNTPServerConfigInfo& GetInfo()const
    {
        return info;
    }
    CNTPServerConfigInfo& GetInfo()
    {
        return info;
    }
    void SetInfo(CNTPServerConfigInfo& tInfo)
    {
        info = tInfo;
    }
private:
    CNTPServerConfigInfo info;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CNTPServerConfigInfoModRsp: public CEventRsp
{
public:
    CNTPServerConfigInfoModRsp()
    {
        event = OspExtEventDesc(PMS_STUN_SERVER_CONFIGINFO_MOD_RSP);
    }
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CMailCfgSetReq: public CEventReq
{
public:
    CMailCfgSetReq()
    {
        event = OspExtEventDesc(PMS_MAIL_CFG_SET_REQ);
    }
private:
    CMailCfgInfo info;//邮箱服务配置信息
public:
    void SetInfo(const CMailCfgInfo& cInfo)
    {
        info = cInfo;
    }
    CMailCfgInfo& GetInfo()
    {
        return info;
    }
    const CMailCfgInfo& GetInfo() const
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

class CMailCfgSetRsp: public CEventRsp
{
public:
    CMailCfgSetRsp()
    {
        event = OspExtEventDesc(PMS_MAIL_CFG_SET_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CMailCfgQryReq: public CEventReq
{
public:
    CMailCfgQryReq()
    {
        event = OspExtEventDesc(PMS_MAIL_CFG_QRY_REQ);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CMailCfgQryRsp: public CEventRsp
{
public:
    CMailCfgQryRsp()
    {
        event = OspExtEventDesc(PMS_MAIL_CFG_QRY_RSP);
    }
    CMailCfgInfo info;/*               */
public:
    void SetInfo(const CMailCfgInfo& cInfo)
    {
        info = cInfo;
    }
    CMailCfgInfo& GetInfo()
    {
        return info;
    }
    const CMailCfgInfo& GetInfo() const
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

class CSendTestMailReq: public CEventReq
{
public:
    CSendTestMailReq()
    {
        event = OspExtEventDesc(PMS_SEND_TEST_MAIL_REQ);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CSendTestMailRsp: public CEventRsp
{
public:
    CSendTestMailRsp()
    {
        event = OspExtEventDesc(PMS_SEND_TEST_MAIL_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUPnPMapInfoGetModeReq : public CEventReq
{
public:
	CUPnPMapInfoGetModeReq()
	{
		Clear();
		event = OspExtEventDesc(PMS_UPNP_MAP_INFO_GET_MODE_REQ);
	}

	void Clear()
	{
		
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUPnPMapInfoGetModeRsp : public CEventRsp
{
public:
	CUPnPMapInfoGetModeRsp()
	{
		Clear();
		event = OspExtEventDesc(PMS_UPNP_MAP_INFO_GET_MODE_RSP);
	}

	void Clear()
	{
		netCardId = 0;
		platIp.clear();
		operMode = UPNP_OPER_MODE_MANUAL;
	}

private:
	int    netCardId;
	string platIp;
	TUPnPOperMode operMode;
	string wanIp;

public:

	int GetNetCardId() const
	{
		return netCardId;
	}
	void SetNetCardId(int nNetCardId)
	{
		netCardId = nNetCardId;
	}

	const string &GetPlatIp() const
	{
		return platIp;
	}
	string &GetPlatIp()
	{
		return platIp;
	}
	void SetPlatIp(const string &strPlatIp)
	{
		platIp = strPlatIp;
	}

	const TUPnPOperMode &GetOperMode() const
	{
		return operMode;
	}
	TUPnPOperMode &GetOperMode()
	{
		return operMode;
	}
	void SetOperMode(const TUPnPOperMode &tOperMode)
	{
		operMode = tOperMode;
	}

	const string &GetWanIp() const
	{
		return wanIp;
	}
	string &GetWanIp()
	{
		return wanIp;
	}
	void SetWanIp(const string &strIp)

	{
		wanIp = strIp;
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUPnPMapInfoSetModeReq : public CEventReq
{
public:
	CUPnPMapInfoSetModeReq()
	{
		Clear();
		event = OspExtEventDesc(PMS_UPNP_MAP_INFO_SET_MODE_REQ);
	}

	void Clear()
	{
		netCardId = 0;
		operMode = UPNP_OPER_MODE_MANUAL;
	}

private:
	int    netCardId;
	TUPnPOperMode operMode;

public:

	int GetNetCardId() const
	{
		return netCardId;
	}
	void SetNetCardId(int nNetCardId)
	{
		netCardId = nNetCardId;
	}

	const TUPnPOperMode &GetOperMode() const
	{
		return operMode;
	}
	TUPnPOperMode &GetOperMode()
	{
		return operMode;
	}
	void SetOperMode(const TUPnPOperMode &tOperMode)
	{
		operMode = tOperMode;
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUPnPMapInfoSetModeRsp : public CEventRsp
{
public:
	CUPnPMapInfoSetModeRsp()
	{
		Clear();
		event = OspExtEventDesc(PMS_UPNP_MAP_INFO_SET_MODE_RSP);
	}

	void Clear()
	{

	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUPnPMapInfoQryReq : public CQueryReqBase
{
public:
	CUPnPMapInfoQryReq()
	{
		Clear();
		event = OspExtEventDesc(PMS_UPNP_MAP_INFO_QRY_REQ);
	}

	void Clear()
	{
		netCardId = 0;
	}

private:
	int netCardId;

public:
	int GetNetCardId() const
	{
		return netCardId;
	}
	void SetNetCardId(int nId)
	{
		netCardId = nId;
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUPnPMapInfoQryRsp : public CQueryRspBase
{
public:
	CUPnPMapInfoQryRsp()
	{
		Clear();
		event = OspExtEventDesc(PMS_UPNP_MAP_INFO_QRY_RSP);
	}

	void Clear()
	{

	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUPnPMapInfoQryNtfReq : public CQueryNtfReqBase
{
public:
	CUPnPMapInfoQryNtfReq() 
	{
		Clear();
		event = OspExtEventDesc(PMS_UPNP_MAP_INFO_QRY_NTF_REQ);
	} 

	void Clear()
	{
		upnpMapList.clear();
	}
	
private:
	TUPnPMapList upnpMapList;

public:
	const TUPnPMapList &GetUPnPMapList() const
	{
		return upnpMapList;
	}
	TUPnPMapList &GetUPnPMapList()
	{
		return upnpMapList;
	}
	void SetUPnPMapList(const TUPnPMapList &tMapList)
	{
		upnpMapList = tMapList;
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUPnPMapInfoQryNtfRsp : public CEventRsp
{
public:
	CUPnPMapInfoQryNtfRsp() 
	{
		Clear();
		event = OspExtEventDesc(PMS_UPNP_MAP_INFO_QRY_NTF_RSP);
	}

	void Clear()
	{

	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUPnPMapInfoAddReq : public CEventReq
{
public:
	CUPnPMapInfoAddReq()
	{
		Clear();
		event = OspExtEventDesc(PMS_UPNP_MAP_INFO_ADD_REQ);
	}

	void Clear()
	{
		netCardId = 0;
		operMode  = UPNP_OPER_MODE_MANUAL;
	}

private:
	int netCardId;
	TUPnPOperMode operMode;
	TUPnPMapItem upnpMapItem;

public:
	int GetNetCardId() const
	{
		return netCardId;
	}
	void SetNetCardId(int nId)
	{
		netCardId = nId;
	}

	TUPnPOperMode GetOperMode() const
	{
		return operMode;
	}
	void SetOperMode(TUPnPOperMode tMode)
	{
		operMode = tMode;
	}

	const TUPnPMapItem &GetUPnPMapItem() const
	{
		return upnpMapItem;
	}
	TUPnPMapItem &GetUPnPMapItem()
	{
		return upnpMapItem;
	}
	void SetUPnPMapItem(const TUPnPMapItem &tItem)
	{
		upnpMapItem = tItem;
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUPnPMapInfoAddRsp : public CEventRsp
{
public:
	CUPnPMapInfoAddRsp()
	{
		Clear();
		event = OspExtEventDesc(PMS_UPNP_MAP_INFO_ADD_RSP);
	}

	void Clear()
	{
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUPnPMapInfoDelReq : public CEventReq
{
public:
	CUPnPMapInfoDelReq()
	{
		Clear();
		event = OspExtEventDesc(PMS_UPNP_MAP_INFO_DEL_REQ);
	}

	void Clear()
	{
		netCardId = 0;
		operMode  = UPNP_OPER_MODE_MANUAL;
		platPort = 0;
		protocol = UPNP_PROTOCOL_UDP;
	}

private:
	int netCardId;
	TUPnPOperMode operMode;
	int    platPort;
	TUPnPProtocolType protocol;

public:
	int GetNetCardId() const
	{
		return netCardId;
	}
	void SetNetCardId(int nId)
	{
		netCardId = nId;
	}

	TUPnPOperMode GetOperMode() const
	{
		return operMode;
	}
	void SetOperMode(TUPnPOperMode tMode)
	{
		operMode = tMode;
	}

	int GetPlatPort()
	{
		return platPort;
	}
	void SetPlatPort(int nPlatPort)
	{
		platPort = nPlatPort;
	}

	TUPnPProtocolType GetProtocol()
	{
		return protocol;
	}
	void SetProtocol(TUPnPProtocolType nProtocol)
	{
		protocol = nProtocol;
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUPnPMapInfoDelRsp : public CEventRsp
{
public:
	CUPnPMapInfoDelRsp()
	{
		Clear();
		event = OspExtEventDesc(PMS_UPNP_MAP_INFO_DEL_RSP);
	}

	void Clear()
	{

	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUPnPMapInfoModReq : public CEventReq
{
public:
	CUPnPMapInfoModReq()
	{
		Clear();
		event = OspExtEventDesc(PMS_UPNP_MAP_INFO_MOD_REQ);
	}

	void Clear()
	{
		netCardId = 0;
		operMode  = UPNP_OPER_MODE_MANUAL;
	}

private:
	int netCardId;
	TUPnPOperMode operMode;
	TUPnPMapItem  upnpMapItem;

public:
	int GetNetCardId() const
	{
		return netCardId;
	}
	void SetNetCardId(int nId)
	{
		netCardId = nId;
	}

	TUPnPOperMode GetOperMode() const
	{
		return operMode;
	}
	void SetOperMode(TUPnPOperMode tMode)
	{
		operMode = tMode;
	}

	const TUPnPMapItem &GetUPnPMapItem() const
	{
		return upnpMapItem;
	}
	TUPnPMapItem &GetUPnPMapItem()
	{
		return upnpMapItem;
	}
	void SetUPnPMapItem(const TUPnPMapItem &tItem)
	{
		upnpMapItem = tItem;
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUPnPMapInfoModRsp : public CEventRsp
{
public:
	CUPnPMapInfoModRsp()
	{
		Clear();
		event = OspExtEventDesc(PMS_UPNP_MAP_INFO_MOD_RSP);
	}

	void Clear()
	{
		
	}

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUPnPMapInfoSyncReq : public CEventReq
{
public:
	CUPnPMapInfoSyncReq()
	{
		Clear();
		event = OspExtEventDesc(PMS_UPNP_MAP_INFO_SYNC_REQ);
	}

	void Clear()
	{
		netCardId = 0;
		platIp.clear();
		upnpMapList.clear();
	}

private:
	int    netCardId;
	string platIp;
	TUPnPMapList upnpMapList;

public:
	int GetNetCardId() const
	{
		return netCardId;
	}
	void SetNetCardId(int nNetCardId)
	{
		netCardId = nNetCardId;
	}

	const string &GetPlatIp() const
	{
		return platIp;
	}
	string &GetPlatIp()
	{
		return platIp;
	}
	void SetPlatIp(const string &strPlatIp)
	{
		platIp = strPlatIp;
	}

	const TUPnPMapList &GetUPnPMapList() const
	{
		return upnpMapList;
	}
	TUPnPMapList &GetUPnPMapList()
	{
		return upnpMapList;
	}
	void SetUPnPMapList(const TUPnPMapList &tMapList)
	{
		upnpMapList = tMapList;
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUPnPMapInfoSyncRsp : public CEventRsp
{
public:
	CUPnPMapInfoSyncRsp()
	{
		event = OspExtEventDesc(PMS_UPNP_MAP_INFO_SYNC_RSP);
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPortMapInfoQryReq : public CQueryReqBase
{
public:
	CPortMapInfoQryReq()
	{
		event = OspExtEventDesc(PMS_PORT_MAP_INFO_QRY_REQ);
	}

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPortMapInfoQryRsp : public CQueryRspBase
{
public:
	CPortMapInfoQryRsp()
	{
		event = OspExtEventDesc(PMS_PORT_MAP_INFO_QRY_RSP);
	}

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPortMapInfoQryNtfReq : public CQueryNtfReqBase
{
public:
	CPortMapInfoQryNtfReq() 
	{
		Clear();
		event = OspExtEventDesc(PMS_PORT_MAP_INFO_QRY_NTF_REQ);
	} 

	void Clear()
	{
		portMapList.clear();
	}
	
private:
	TPortMapList portMapList;

public:
	const TPortMapList &GetPortMapList() const
	{
		return portMapList;
	}
	TPortMapList &GetPortMapList()
	{
		return portMapList;
	}
	void SetPortMapList(const TPortMapList &tMapList)
	{
		portMapList = tMapList;
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPortMapInfoQryNtfRsp : public CEventRsp
{
public:
	CPortMapInfoQryNtfRsp() 
	{
		event = OspExtEventDesc(PMS_PORT_MAP_INFO_QRY_NTF_RSP);
	}

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPortMapInfoAddReq : public CEventReq
{
public:
	CPortMapInfoAddReq()
	{
		event = OspExtEventDesc(PMS_PORT_MAP_INFO_ADD_REQ);
	}


private:
	TPortMapItem portMapItem;
public:
    const TPortMapItem& GetPortMapItem() const 
    {
        return portMapItem;
    }
	TPortMapItem& GetPortMapItem()
    {
        return portMapItem;
    }
	void SetPortMapItem(TPortMapItem tPortMapItem)
	{
		portMapItem = tPortMapItem;
	}



public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPortMapInfoAddRsp : public CEventRsp
{
public:
	CPortMapInfoAddRsp()
	{
		event = OspExtEventDesc(PMS_PORT_MAP_INFO_ADD_RSP);
	}

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPortMapInfoDelReq : public CEventReq
{
public:
	CPortMapInfoDelReq()
	{
		Clear();
		event = OspExtEventDesc(PMS_PORT_MAP_INFO_DEL_REQ);
	}

	void Clear()
	{
        platIp.clear();
		platPort = 0;
        protocol.clear();
	}

private:
    string platIp;
	int    platPort;
    string protocol;

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

    void SetPlatPort(int dwPlatPort)
    {
        platPort = dwPlatPort;
    }

    const  int GetPlatPort() const
    {
        return platPort;
    }
    int GetPlatPort()
    {
        return platPort;
    }

    void SetProtocol(const string& strProtocol)
    {
        protocol = strProtocol;
    }
    string GetProtocol() 
    {
        return protocol;
    }
    const string& GetProtocol() const
    {
        return protocol;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPortMapInfoDelRsp : public CEventRsp
{
public:
	CPortMapInfoDelRsp()
	{
		event = OspExtEventDesc(PMS_PORT_MAP_INFO_DEL_RSP);
	}


public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPortMapInfoModReq : public CEventReq
{
public:
	CPortMapInfoModReq()
	{
		event = OspExtEventDesc(PMS_PORT_MAP_INFO_MOD_REQ);
	}

private:
	TPortMapItem  portMapItem;

public:
    const TPortMapItem& GetPortMapItem() const
    {
        return portMapItem;
    }
    TPortMapItem& GetPortMapItem()
    {
        return portMapItem;
    }
    void SetPortMapItem(const TPortMapItem& tPortMapItem)
    {
        portMapItem = tPortMapItem;
    }


public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPortMapInfoModRsp : public CEventRsp
{
public:
	CPortMapInfoModRsp()
	{
		Clear();
		event = OspExtEventDesc(PMS_PORT_MAP_INFO_MOD_RSP);
	}

	void Clear()
	{
		
	}

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDdnsStatusGetReq : public CEventReq
{
public:
	CDdnsStatusGetReq()
	{
		Clear();
		event = OspExtEventDesc(PMS_DDNS_STATUS_GET_REQ);
	}

	void Clear()
	{

	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

typedef int TDDNSStatus;
#define DDNS_CONN_STATUS_OFFLINE      0
#define DDNS_CONN_STATUS_ONLINE       1

class CDdnsStatusGetRsp : public CEventRsp
{
public:
	CDdnsStatusGetRsp()
	{
		Clear();
		event = OspExtEventDesc(PMS_DDNS_STATUS_GET_RSP);
	}

	void Clear()
	{
		serverAddr.clear();
		domain.clear();
		serverport = 0;
		webPort    = 0;
		state      = DDNS_CONN_STATUS_OFFLINE;
	}

private:
	string serverAddr;
	int    serverport;
	string domain;
	int    webPort;
	TDDNSStatus state;

public:
	const string &GetServerAddr() const
	{
		return serverAddr;
	}
	string &GetServerAddr()
	{
		return serverAddr;
	}
	void SetServerAddr(const string &strServerAddr)
	{
		serverAddr = strServerAddr;
	}

	int GetServerPort() const
	{
		return serverport;
	}
	void SetServerPort(int nPort)
	{
		serverport = nPort;
	}

	const string &GetDomain() const
	{
		return domain;
	}
	string &GetDomain()
	{
		return domain;
	}
	void SetDomain(const string &strDomain)
	{
		domain = strDomain;
	}

	int GetWebPort() const
	{
		return webPort;
	}
	void SetWebPort(int nWebPort)
	{
		webPort = nWebPort;
	}

	TDDNSStatus GetState() const
	{
		return state;
	}
	void SetState(TDDNSStatus tState)
	{
		state = tState;
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDdnsStatusSetReq : public CEventReq
{
public:
	CDdnsStatusSetReq()
	{
		Clear();
		event = OspExtEventDesc(PMS_DDNS_STATUS_SET_REQ);
	}

	void Clear()
	{
		domain.clear();
	}

private:
	string domain;

public:
	const string &GetDomain() const
	{
		return domain;
	}
	string &GetDomain()
	{
		return domain;
	}
	void SetDomain(const string &strDomain)
	{
		domain = strDomain;
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDdnsStatusSetRsp : public CEventRsp
{
public:
	CDdnsStatusSetRsp()
	{
		Clear();
		event = OspExtEventDesc(PMS_DDNS_STATUS_SET_RSP);
	}

	void Clear()
	{

	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CModuleConfGetReq : public CEventReq
{
public:
	CModuleConfGetReq()
	{
		Clear();
		event = OspExtEventDesc(PMS_MODULE_CONF_GET_REQ);
	}

	void Clear()
	{

	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CModuleConfGetRsp : public CEventRsp
{
public:
	CModuleConfGetRsp()
	{
		Clear();
		event = OspExtEventDesc(PMS_MODULE_CONF_GET_RSP);
	}

	void Clear()
	{
        moduleConfList.clear();
	}

private:
	vector<TModuleConf> moduleConfList;

public:
	const vector<TModuleConf> &GetModuleConfList() const
	{
		return moduleConfList;
	}
	vector<TModuleConf> &GetModuleConfList()
	{
		return moduleConfList;
	}
	void SetModuleConfList(const vector<TModuleConf> &cModuleConfList)
	{
		moduleConfList = cModuleConfList;
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CDefenseCrossStreamSetReq : public CEventReq
{
public:
	CDefenseCrossStreamSetReq()
	{
		Clear();
		event = OspExtEventDesc(PMS_DEFENSE_CROSS_STREAM_SET_REQ);
	}

	void Clear()
	{
		enable=true;
	}

private:
	bool enable ;

public:
	const bool GetEnable() const
	{
		return enable;
	}
	bool GetEnable()
	{
		return enable;
	}
	void SetEnable(const bool & bEnable)
	{
		enable = bEnable;
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDefenseCrossStreamSetRsp : public CEventRsp
{
public:
	CDefenseCrossStreamSetRsp()
	{
		Clear();
		event = OspExtEventDesc(PMS_DEFENSE_CROSS_STREAM_SET_RSP);
	}

	void Clear()
	{

	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDefenseCrossStreamGetReq : public CEventReq
{
public:
	CDefenseCrossStreamGetReq()
	{
		Clear();
		event = OspExtEventDesc(PMS_DEFENSE_CROSS_STREAM_GET_REQ);
	}

	void Clear()
	{

	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDefenseCrossStreamGetRsp : public CEventRsp
{
public:
	CDefenseCrossStreamGetRsp()
	{
		Clear();
		event = OspExtEventDesc(PMS_DEFENSE_CROSS_STREAM_GET_RSP);
	}

	void Clear()
	{
		enable = false;
	}

private:
	bool enable;

public:
	const bool GetEnable() const
	{
		return enable;
	}
	bool GetEnable()
	{
		return enable;
	}
	void SetEnable(const bool & bEnable)
	{
		enable = bEnable;
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};



class CPmsSecuremailInfoGetReq : public CEventReq
{
public:
	CPmsSecuremailInfoGetReq()
	{
		Clear();
		event = OspExtEventDesc(PMS_SECUREMAIL_INFO_GET_REQ);
	}

	void Clear()
	{

	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPmsSecuremailInfoGetRsp : public CEventRsp
{
public:
	CPmsSecuremailInfoGetRsp()
	{
		Clear();
		event = OspExtEventDesc(PMS_SECUREMAIL_INFO_GET_RSP);
	}

	void Clear()
	{
		securemail.clear();
	}

private:
	string securemail;

public:
	const string& GetSecuremail() const
	{
		return securemail;
	}
    string& GetSecuremail()
    {
        return securemail;
    }

	void SetSecuremail(const string& strSecuremail)
	{
		securemail = strSecuremail;
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};



class CPmsResetCuPasswordReq : public CEventReq
{
public:
	CPmsResetCuPasswordReq()
	{
		Clear();
		event = OspExtEventDesc(PMS_RESET_CU_PASSWORD_REQ);
	}

	void Clear()
	{

	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPmsResetCuPasswordRsp : public CEventRsp
{
public:
	CPmsResetCuPasswordRsp()
	{
		Clear();
		event = OspExtEventDesc(PMS_RESET_CU_PASSWORD_RSP);
	}

	void Clear()
	{
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};



class CPmsPmcPasswordResetInfoGetReq : public CEventReq
{
public:
	CPmsPmcPasswordResetInfoGetReq()
	{
		Clear();
		event = OspExtEventDesc(PMS_PMC_PASSWORD_RESET_INFO_GET_REQ);
	}

	void Clear()
	{
	}

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPmsPmcPasswordResetInfoGetRsp : public CEventRsp
{
public:
	CPmsPmcPasswordResetInfoGetRsp()
	{
		Clear();
		event = OspExtEventDesc(PMS_PMC_PASSWORD_RESET_INFO_GET_RSP);
	}

	void Clear()
	{
	}
private:
    string serialnumber;
    string securemail;
public:
	const string& GetSerialnumber() const
	{
		return serialnumber;
	}
    string& GetSerialnumber()
    {
        return serialnumber;
    }
	void SetSerialnumber(const string& strSerialnumber)
	{
		serialnumber = strSerialnumber;
	}
    const string& GetSecuremail() const
	{
		return securemail;
	}
    string& GetSecuremail()
    {
        return securemail;
    }

	void SetSecuremail(const string& strSecuremail)
	{
		securemail = strSecuremail;
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CPmsPmcPasswordResetReq : public CEventReq
{
public:
	CPmsPmcPasswordResetReq()
	{
		Clear();
		event = OspExtEventDesc(PMS_PMC_PASSWORD_RESET_REQ);
	}

	void Clear()
	{
        safecode.clear();
        password.clear();
	}
private:
    string safecode;
    string password;
public:
    const string& GetSafecode() const
	{
		return safecode;
	}
    string& GetSafecode()
    {
        return safecode;
    }
	void SetSafecode(const string& strSafecode)
	{
		safecode = strSafecode;
	}
    const string& GetPassword() const
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
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPmsPmcPasswordResetRsp : public CEventRsp
{
public:
	CPmsPmcPasswordResetRsp()
	{
		Clear();
		event = OspExtEventDesc(PMS_PMC_PASSWORD_RESET_RSP);
	}

	void Clear()
	{
	}
private:
    string errorDesc;
public:
    const string& GetErrorDesc()const
    {
        return errorDesc;
    }
    string& GetErrorDesc()
    {
        return errorDesc;
    }
    void SetErrorDesc(const string& strErrorDesc)
    {
        errorDesc= strErrorDesc;
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};



#endif // _PMS_PROTO_H_



























































