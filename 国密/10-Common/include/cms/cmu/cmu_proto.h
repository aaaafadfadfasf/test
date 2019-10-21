/*****************************************************************************
   模块名      : cmu_proto
   文件名      : cmu_proto.h
   相关文件    : 
   文件实现功能: cmu_proto.h 定义所有与CMU打交道的模块所有消息的消息体
   作者        : liangli
   版本        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人      修改内容
   2011/10/22   1.0         liangli        创建
******************************************************************************/

#ifndef __CMU_PROTO_H__
#define __CMU_PROTO_H__

#include "cms/cmu/cmu_event.h"
#include "cms/cmu/cmu_struct.h"
#include "cms/cms_proto.h"
#include "cms/uas/uas_struct.h"


//通用的设备登录
class CDevLoginReq : public CEventReq
{
public:
    CDevLoginReq()
    {
        Clear();
        event = OspExtEventDesc(DEV_LOGIN_REQ);
    }

    void Clear()
    {
		//add by Royan 20130828 设置punch属性
		isSupportNA = false;

        CEventReq::Clear();
        devUri.clear();
        devType.clear();
        devAddrList.clear();
        svrUri.clear();
        userName.clear();
        password.clear();
    }

private:
    string           devUri;        //设备URI
    TDevType         devType;       //设备类型
    TNetAddrList     devAddrList;   //设备地址列表
    string           svrUri;        //设备登录的接口模块Uri
    string           userName;      //登录用户名。模块注册时一般不验证，为空
    string           password;      //登录密码。模块注册时一般不验证，为空
	//add by Royan 20130828 设置punch属性
	bool isSupportNA;                //pu是否支持natagent
    CTransparentData   extraInfo;    //具体模块的扩展信息。登录uas时为TLoginExternInfo4Uas序列化的内容
    
public:
    //devUri
    void SetDevUri(const string& strDevUri)
    {
        devUri = strDevUri;
    }
    string& GetDevUri()
    {
        return devUri;
    }
    const string& GetDevUri() const
    {
        return devUri;
    }

    //devType
    void SetDevType(const TDevType& strDevType)
    {
        devType = strDevType;
    }
    TDevType& GetDevType()
    {
        return devType;
    }
    const TDevType& GetDevType() const
    {
        return devType;
    }

    //devAddrList
    void SetDevAddrList(const TNetAddrList& cDevAddrList)
    {
        devAddrList = cDevAddrList;
    }
    TNetAddrList& GetDevAddrList()
    {
        return devAddrList;
    }
    const TNetAddrList& GetDevAddrList() const
    {
        return devAddrList;
    }

    //svrUri
    void SetSvrUri(const string& strSvrUri)
    {
        svrUri = strSvrUri;
    }
    string& GetSvrUri()
    {
        return svrUri;
    }
    const string& GetSvrUri() const
    {
        return svrUri;
    }

    //userName
    void SetUserName(const string& strUserName)
    {
        userName = strUserName;
    }
    string& GetUserName()
    {
        return userName;
    }
    const string& GetUserName() const
    {
        return userName;
    }

    //password
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

//add by Royan 20130828 设置punch属性
public:
	void SetIsSupportNA(bool bIsSupportNA)
	{
		isSupportNA = bIsSupportNA;
	}
	bool GetIsSupportNA() const
	{
		return isSupportNA;
	}

    void SetExtraInfo(const CTransparentData& cExtraInfo)
    {
        extraInfo = cExtraInfo;
    }
    CTransparentData& GetExtraInfo()
    {
        return extraInfo;
    }
    const CTransparentData& GetExtraInfo() const
    {
        return extraInfo;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDevLoginRsp : public CEventRsp
{
public:
    CDevLoginRsp()
    {
        event = OspExtEventDesc(DEV_LOGIN_RSP);
    }

private:
    string rspData; // 模块之间约定的应答数据
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetRspData(const string& strRspData)
    {
        rspData = strRspData;
    }
    string& GetRspData()
    {
        return rspData;
    }
    const string& GetRspData() const
    {
        return rspData;
    }
};


class CDevLogoutReq : public CEventReq
{
public:
    CDevLogoutReq()
    {
        event = OspExtEventDesc(DEV_LOGOUT_REQ);
    }

    void Clear()
    {
        devUri.clear();
        CEventReq::Clear();
    }

private:
    string           devUri;                                //设备URI
public:
    void SetDevUri(const string& strDevUri)
    {
        devUri = strDevUri;
    }
    string& GetDevUri()
    {
        return devUri;
    }
    const string& GetDevUri() const
    {
        return devUri;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDevLogoutRsp : public CEventRsp
{
public:
    CDevLogoutRsp()
    {
        event = OspExtEventDesc(DEV_LOGOUT_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

//获取模块的IP地址信息
class CNetAddrQueryReq : public CEventReq
{
public:
    CNetAddrQueryReq()
    {
        event = OspExtEventDesc(NETADDR_QUERY_REQ);
    } 

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CNetAddrQueryRsp : public CEventRsp
{
public:
    CNetAddrQueryRsp()
    {
        event = OspExtEventDesc(NETADDR_QUERY_RSP);
    }

    void Clear()
    {
        CEventRsp::Clear();
        devAddrList.clear();
    }

private:
    TNetAddrList     devAddrList;   //设备地址列表

public:
    void SetDevAddrList(const TNetAddrList& cDevAddrList)
    {
        devAddrList = cDevAddrList;
    }
    TNetAddrList& GetDevAddrList()
    {
        return devAddrList;
    }
    const TNetAddrList& GetDevAddrList() const
    {
        return devAddrList;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

//使用INVITE方法作为通用文件上传、下载请求
typedef string TFileSvcType;
#define FILE_EXPORT_PU_PIC       "ExportPuPic"       //前端图片下载
#define FILE_EXPORT_DAILY        "ExportDaily"       //日志导出
#define FILE_EXPORT_MIRROR       "ExportMirror"      //镜像导出
#define FILE_EXPORT_CUSEROPERATELOG  "ExportCuserOperateLog"      //导出CU用户操作日志，fileToken为COperateLogInfoQueryCondition的序列化结果
#define FILE_EXPORT_ALARMLOG         "ExportAlarmLog"             //导出设备告警日志，fileToken为CAlarmLogInfoQueryCondition的序列化结果
#define FILE_EXPORT_DEVICELOG        "ExportDeviceLog"            //导出设备日志，fileToken为CDeviceLogInfoQueryCondition的序列化结果
#define FILE_EXPORT_PMCUSEROPERATELOG  "ExportPmcuserOperateLog"  //导出PMC用户操作日志，fileToken为CUserOperationLogQryCondition的序列化结果
#define FILE_EXPORT_PMSPLATFORMLOG   "ExportPmsPlatformLog"       //导出PMS平台日志，fileToken为CPlatformLogInfoQryCondition的序列化结果
#define FILE_EXPORT_NMSHISTORYALARM      "ExportNmsHistoryAlarm"  //导出网管的历史告警，fileToken为CNmsAlarmQueryCondition的序列化结果
#define FILE_EXPORT_CUSER_DEVICEDATA  "ExportCuserDeviceData"     //导出CU用户的设备数据
#define FILE_EXPORT_GB_CATALOG    "ExportGbCatalog"               //导出国标目录信息，用于国标目录重组配置
#define FILE_EXPORT_GB_CATALOG_REPORT_CFG "ExportGbCatalogReportCfg" //导出国标目录重组上报配置  
#define FILE_EXPORT_DEVICE_SATATISTICS "ExportDeviceSatatistics"	// 导出设备统计信息.
#define FILE_EXPORT_MODULELOG           "ExportModuleLog"             //导出平台模块日记 
#define FILE_EXPORT_GB_DEVINFO_EXT "ExportGbDevInfoExt"          //导出国标扩展信息 

#define FILE_IMPORT_MIRROR       "ImportMirror"      //镜像导入     
#define FILE_IMPORT_UPDATE       "ImportUpdate"      //升级包上传
#define FILE_IMPORT_MAP_PIC      "ImportMapPic"      //地图图片上传
#define FILE_IMPORT_MAP_ELE      "ImportMapEle"      //地图图元上传
#define FILE_IMPORT_GB_CATALOG_REPORT_CFG "ImportGbCatalogReportCfg" //国标目录重组上报配置上传
#define FILE_IMPORT_GB_DEVINFO_EXT "ImportGbDevInfoExt"          //导出国标扩展信息 

class CFileExportReq : public CEventReq
{
public:
    CFileExportReq()
    {
        event = OspExtEventDesc(FILE_EXPORT_REQ);
        fileCharacterset = "GBK";
    }

    void Clear()
    {
        srvName.clear();
        fileReceiverUri.clear();
        fileSenderUri.clear();
        fileToken.clear();
        CEventReq::Clear();
    }

private: 
    TFileSvcType        srvName;                            //业务类型
    string              fileReceiverUri;                    //文件接受者：pmc、cu等
    string              fileSenderUri;                      //文件发送者: pu、pms、uas等
    CTransparentData    fileToken;                          //文件标识：表示文件标识或者导出条件（表示导出条件时内容参见具体的业务类型说明）
    string              fileCharacterset;                   //文件的字符集：字符集名称表示参见linux "iconv --list"的输出
	string              fileTimeZone;                       //文件中时间的时区

	std::vector<string> customParams;						// 自定义参数,每种业务类型不同.
public:
    void SetSrvName(const TFileSvcType& strSrvName)
    {
        srvName = strSrvName;
    }
    TFileSvcType& GetSrvName()
    {
        return srvName;
    }
    const TFileSvcType& GetSrvName() const
    {
        return srvName;
    }

    void SetFileReceiverUri(const string& strFileReceiverUri)
    {
        fileReceiverUri = strFileReceiverUri;
    }
    string& GetFileReceiverUri()
    {
        return fileReceiverUri;
    }
    const string& GetFileReceiverUri() const
    {
        return fileReceiverUri;
    }

    void SetFileSenderUri(const string& strFileSenderUri)
    {
        fileSenderUri = strFileSenderUri;
    }
    string& GetFileSenderUri()
    {
        return fileSenderUri;
    }
    const string& GetFileSenderUri() const
    {
        return fileSenderUri;
    }

    void SetFileToken(const CTransparentData& strFileToken)
    {
        fileToken = strFileToken;
    }
    CTransparentData& GetFileToken()
    {
        return fileToken;
    }
    const CTransparentData& GetFileToken() const
    {
        return fileToken;
    }
    void SetFileCharacterset(const string& strFileCharacterset)
    {
        fileCharacterset = strFileCharacterset;
    }
    string& GetFileCharacterset()
    {
        return fileCharacterset;
    }
    const string& GetFileCharacterset() const
    {
        return fileCharacterset;
    }
	void SetFileTimeZone(const string& strTimeZone)
	{
		fileTimeZone = strTimeZone;
	}
	string& GetFileTimeZone()
	{
		return fileTimeZone;
	}
	const string& GetFileTimeZone() const
	{
		return fileTimeZone;
	}

	std::vector<string>& CustomParames(){ return this->customParams;  }
	const std::vector<string>& CustomParames() const{ return this->customParams; }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CFileExportRsp : public CEventRsp
{
public:
    CFileExportRsp()
    {
        event = OspExtEventDesc(FILE_EXPORT_RSP);
    }

    void Clear()
    {
        fileURL.clear();
        CEventRsp::Clear();
    }

private: 
    string              fileURL;                    //文件URL
public:
    void SetFileURL(const string& strFileURL)
    {
        fileURL = strFileURL;
    }
    string& GetFileURL()
    {
        return fileURL;
    }
    const string& GetFileURL() const
    {
        return fileURL;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CFileExportAck : public CEventRsp
{
public:
    CFileExportAck()
    {
        event = OspExtEventDesc(FILE_EXPORT_ACK);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CFileCollectProgressNtfReq: public CEventReq
{
public:
    CFileCollectProgressNtfReq()
    {
        event = OspExtEventDesc(FILE_COLLECT_PROGRESS_NTF_REQ);
    }

    void Clear()
    {
        percent = 0;
        CEventReq::Clear();
    }

private:
    int percent;                          //文件收集进度百分比[0-100]，达到100时，可以开启http下载
public:
    void SetPercent(int nPercent)
    {
        percent = nPercent;
    }
    int GetPercent() const
    {
        return percent;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CFileCollectProgressNtfRsp : public CEventRsp
{
public:
    CFileCollectProgressNtfRsp()
    {
        event = OspExtEventDesc(FILE_COLLECT_PROGRESS_NTF_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CFileImportReq : public CEventReq
{
public:
    CFileImportReq()
    {
        event = OspExtEventDesc(FILE_IMPORT_REQ);
    }

    void Clear()
    {
        srvName.clear();
        fileReceiverUri.clear();
        fileSenderUri.clear();
        fileToken.clear();
        CEventReq::Clear();
    }

private: 
    TFileSvcType        srvName;                            //业务类型
    string              fileReceiverUri;                    //文件接受者：pmc、cu等
    string              fileSenderUri;                      //文件发送者: pu、pms、uas等
    string              fileToken;                          //文件标识
public:
    void SetSrvName(const TFileSvcType& strSrvName)
    {
        srvName = strSrvName;
    }
    TFileSvcType& GetSrvName()
    {
        return srvName;
    }
    const TFileSvcType& GetSrvName() const
    {
        return srvName;
    }

    void SetFileReceiverUri(const string& strFileReceiverUri)
    {
        fileReceiverUri = strFileReceiverUri;
    }
    string& GetFileReceiverUri()
    {
        return fileReceiverUri;
    }
    const string& GetFileReceiverUri() const
    {
        return fileReceiverUri;
    }

    void SetFileSenderUri(const string& strFileSenderUri)
    {
        fileSenderUri = strFileSenderUri;
    }
    string& GetFileSenderUri()
    {
        return fileSenderUri;
    }
    const string& GetFileSenderUri() const
    {
        return fileSenderUri;
    }

    void SetFileToken(const string& strFileToken)
    {
        fileToken = strFileToken;
    }
    string& GetFileToken()
    {
        return fileToken;
    }
    const string& GetFileToken() const
    {
        return fileToken;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CFileImportRsp : public CEventRsp
{
public:
    CFileImportRsp()
    {
        event = OspExtEventDesc(FILE_IMPORT_RSP);
    }

    void Clear()
    {
        dirURL.clear();
        CEventRsp::Clear();
    }

private: 
    string              dirURL;                    //目录URL，给出一个目录，请求端将文件上传到该目录，多个文件可以在一个会话内
public:
    void SetDirURL(const string& strDirURL)
    {
        dirURL = strDirURL;
    }
    string& GetDirURL()
    {
        return dirURL;
    }
    const string& GetDirURL() const
    {
        return dirURL;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CFileImportAck : public CEventRsp
{
public:
    CFileImportAck()
    {
        event = OspExtEventDesc(FILE_IMPORT_ACK);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CFileTransEndReq: public CEventReq
{
public:
    CFileTransEndReq()
    {
        event = OspExtEventDesc(FILE_TRANS_END_REQ);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CFileTransEndRsp : public CEventRsp
{
public:
    CFileTransEndRsp()
    {
        event = OspExtEventDesc(FILE_TRANS_END_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

//////////////////////////////////////////////////////////////////////////
//PUI注册平台

typedef vector<TNetAddr> CIpAddrList;
class CPuiRegReq : public CEventReq
{
public:
    CPuiRegReq()
    {
        event = OspExtEventDesc(PUI_REG_REQ);
        Clear();
    }

    void Clear()
    {
		//add by Royan 20130828 设置punch属性
		isSupportNA = false;
        devUri.clear();
        ipAddrList.clear();
		netSegmentInfo.Clear();
        isSupportRedis = false;
        CEventReq::Clear();
    }

public:
    const string& GetDevUri() const
    {
        return devUri;
    }

    string& GetDevUri()
    {
        return devUri;
    }

    void SetDevUri(const string& tSipUri)
    {
        devUri = tSipUri;
    }

    const CIpAddrList& GetIpAddrList() const
    {
        return ipAddrList;
    }

    CIpAddrList& GetIpAddrList()
    {
        return ipAddrList;
    }

    void SetIpAddrList(const CIpAddrList& cIpAddrList)
    {
        ipAddrList = cIpAddrList;
    }

	const TNetSegmentAllInfo& GetNetSegInfo() const
	{
		return netSegmentInfo;
	}
	TNetSegmentAllInfo& GetNetSegInfo()
	{
		return netSegmentInfo;
	}
	void SetNetSegInfo(const TNetSegmentAllInfo& tNetSegmentAllInfo)
	{
		netSegmentInfo = tNetSegmentAllInfo;
	}
//add by Royan 20130828 设置punch属性
public:
	void SetIsSupportNA(bool bIsSupportNA)
	{
		isSupportNA = bIsSupportNA;
	}
	bool GetIsSupportNA() const
	{
		return isSupportNA;
	}

	void SetDevType(const TDevType& strDevType)
	{
		devType = strDevType;
	}
	TDevType& GetDevType()
	{
		return devType;
	}
	const TDevType& GetDevType() const
	{
		return devType;
	}

    void SetIsSupportRedis(bool bIsSupportRedis)
    {
        isSupportRedis = bIsSupportRedis;
    }
    bool GetIsSupportRedis() const
    {
        return isSupportRedis;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
private:
    string           devUri;          //pui设备URI
    CIpAddrList      ipAddrList;      //pui地址列表
	TNetSegmentAllInfo netSegmentInfo;   //pui地址列表
	//add by Royan 20130828 设置punch属性
	bool isSupportNA;             //pu是否支持natagent
    bool isSupportRedis;

    TDevType         devType;                //使用pui身份登录时使用的设备类型:gbs，cui1.0，G网关等
};

//用于设置pu用于加密码流的vkek( 可解密vek )
//以便解码时使用
typedef std::vector<TSecurityModuleVkek> SECURITY_VKEK_LIST;

class CUpdatePuVkeksNtfReq : public CEventReq
{
public:
	CUpdatePuVkeksNtfReq()
	{
		event = OspExtEventDesc(UPDATE_PU_VKEKS_NTF_REQ);
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);

public:
	void Clear()
	{
		CEventReq::Clear();
		vkeks.clear();
		tm_from.clear();
		tPuChannel.Clear();
	}

public:
	void SetVkeks(const SECURITY_VKEK_LIST &vkekList)
	{
		vkeks = vkekList;
	}
	const SECURITY_VKEK_LIST& GetVkeks() const
	{
		return vkeks;
	}
	SECURITY_VKEK_LIST& GetVkeks()
	{
		return vkeks;
	}


	void SetBeginTime(const std::string &strBeginTime)
	{
		tm_from = strBeginTime;
	}
	std::string& GetBeginTime()
	{
		return tm_from;
	}
	const std::string& GetBeginTime() const
	{
		return tm_from;
	}


	void SetPuChannel(const TChannel &tPuChl)
	{
		tPuChannel = tPuChl;
	}
	TChannel& GetPuChannel()
	{
		return tPuChannel;
	}
	const TChannel& GetPuChannel() const
	{
		return tPuChannel;
	}

public:
	SECURITY_VKEK_LIST vkeks;
	std::string tm_from;                //该组vkek的启用时间, 注意 结束 永远是未知的
	//std::string tm_to;                //该组vkek的结束时间
										//时间格式参见 CSecurityModuleVkekQueryReq  的开始结束时间
	TChannel    tPuChannel;
};
class CUpdatePuVkeksNtfRsp : public CEventRsp
{
public:
	CUpdatePuVkeksNtfRsp()
	{
		event = OspExtEventDesc(UPDATE_PU_VKEKS_NTF_RSP);
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class CPuSignatrueCtrlReq : public CEventReq
{
public:
	CPuSignatrueCtrlReq()
	{
		event = OspExtEventDesc(PU_SIGNATURE_CTRL_REQ);
	}

public:
	const string& GetDevUri() const
	{
		return devUri;
	}

	string& GetDevUri()
	{
		return devUri;
	}

	void SetDevUri(const string& tSipUri)
	{
		devUri = tSipUri;
	}


	bool GetEnabled() const
	{
		return enabled;
	}
	void SetEnabled(bool enabled)
	{
		this->enabled = enabled;
	}

protected:
	std::string devUri;
	bool		enabled;
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};
class CPuSignatrueCtrlRsp : public CEventRsp
{
public:
	CPuSignatrueCtrlRsp()
	{
		event = OspExtEventDesc(PU_SIGNATURE_CTRL_RSP);
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class CPuEncryptionCtrlReq : public CEventReq
{
public:
	CPuEncryptionCtrlReq()
	{
		event = OspExtEventDesc(PU_ENCRYPTION_CTRL_REQ);
	}

public:
	const string& GetDevUri() const
	{
		return devUri;
	}

	string& GetDevUri()
	{
		return devUri;
	}

	void SetDevUri(const string& tSipUri)
	{
		devUri = tSipUri;
	}

	bool GetEnabled() const
	{
		return enabled;
	}
	void SetEnabled(bool enabled)
	{
		this->enabled = enabled;
	}
protected:
	std::string devUri;
	bool		enabled;
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};
class CPuEncryptionCtrlRsp : public CEventRsp
{
public:
	CPuEncryptionCtrlRsp()
	{
		event = OspExtEventDesc(PU_ENCRYPTION_CTRL_RSP);
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

typedef int TBackendCapId;
#define VIDEO_CLOCK_RATE_NEGO              1     // 后端能力集---采样率协商（值为PLAT_ENABLE或PLAT_DISABLE）

typedef int TBackendCapValue;
#define CAP_VALUE_PLAT_ENABLE              1
#define CAP_VALUE_PLAT_DISABLE             0

typedef map<TBackendCapId, TBackendCapValue> TBackendCapList;

struct TBackendCapabilities
{
public:
	TBackendCapabilities()
	{
		Clear();
	}

	~TBackendCapabilities()
	{
		Clear();
	}

	void Clear()
	{
		backendCapList.clear();
	}

	const TBackendCapList &GetBackendCapList() const
	{
		return backendCapList;
	}
	TBackendCapList &GetBackendCapList()
	{
		return backendCapList;
	}
	void SetBackendCapList(const TBackendCapList &tCapList)
	{
		backendCapList = tCapList;
	}

private:
	TBackendCapList backendCapList;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPuiRegRsp : public CEventRsp
{
public:
    CPuiRegRsp()
    {
		Clear();
        event = OspExtEventDesc(PUI_REG_RSP);
    }

	~CPuiRegRsp()
	{
		Clear();
	}

	void Clear()
	{
		backendCapabilities.Clear();
	}

private:
	TBackendCapabilities backendCapabilities;

public:
	const TBackendCapabilities &GetBackendCapabilities() const
	{
		return backendCapabilities;
	}
	TBackendCapabilities &GetBackendCapabilities()
	{
		return backendCapabilities;
	}
	void SetBackendCapabilities(const TBackendCapabilities &tCapabilities)
	{
		backendCapabilities = tCapabilities;
	}

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


//////////////////////////////////////////////////////////////////////////
//pu注册平台

class CPuRegReq : public CEventReq
{
public:
    CPuRegReq()
    {
        event = OspExtEventDesc(PU_REG_REQ);
    }

    void Clear()
    {
        isSupportNA = false;
        baseConfig.Clear();
        CEventReq::Clear();
    }

public:
    const TPuBaseConfig& GeBaseConfig() const
    {
        return baseConfig;
    }

    TPuBaseConfig& GetBaseConfig()
    {
        return baseConfig;
    }

    void SetBaseConfig(const TPuBaseConfig& tBaseConfig)
    {
        baseConfig = tBaseConfig;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

private:
    TPuBaseConfig baseConfig;
    string usedPuiIp;                //pu使用的PUI的IP，可用于选择VTDU的码流地址
    bool isSupportNA;                //pu是否支持natagent
public:
    void SetUsedPuiIp(const string& strUsedPuiIp)
    {
        usedPuiIp = strUsedPuiIp;
    }
    string& GetUsedPuiIp()
    {
        return usedPuiIp;
    }
    const string& GetUsedPuiIp() const
    {
        return usedPuiIp;
    }

public:
    void SetIsSupportNA(bool bIsSupportNA)
    {
        isSupportNA = bIsSupportNA;
    }
    bool GetIsSupportNA() const
    {
        return isSupportNA;
    }

};

class CPuRegRsp : public CEventRsp
{
public:
    CPuRegRsp()
    {
        event = OspExtEventDesc(PU_REG_RSP);
        netType = DEVICE_LINEMODE_INTRANET;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

private:
    int          netType;                          //该pu和平台的网络关系
    /*
    const u32 DEVICE_LINEMODE_INTRANET = 0;    //CMU与PU在同一网络
    const u32 DEVICE_LINEMODE_EXTRANET = 1;    //CMU与PU在不同网络
    */
public:
    void SetNetType(int nNetType)
    {
        netType = nNetType;
    }
    int GetNetType() const
    {
        return netType;
    }

};

class CPuUnRegReq : public CEventReq
{
public:
    CPuUnRegReq()
    {
        event = OspExtEventDesc(PU_UNREG_REQ);
    }

    void Clear()
    {
        devUri.clear();
        CEventReq::Clear();
    }
public:
    const string& GetDevUri() const
    {
        return devUri;
    }

    string& GetDevUri()
    {
        return devUri;
    }

    void SetDevUri(const string& tSipUri)
    {
        devUri = tSipUri;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
private:
    string           devUri;  //前端设备URI
};

class CPuUnRegRsp : public CEventRsp
{
public:
    CPuUnRegRsp()
    {
        event = OspExtEventDesc(PU_UNREG_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPuOnlineNtfReq : public CEventReq
{
public:
    CPuOnlineNtfReq()
    {
        event = OspExtEventDesc(PU_ONLINE_NTF_REQ);
    }

    void Clear()
    {
		devUri.clear();
        CEventReq::Clear();
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

private:
    string devUri; //前端设备URI：puid@domain
public:
    void SetDevUri(const string& strUri)
    {
        devUri = strUri;
    }
    string& GetDevUri()
    {
        return devUri;
    }
    const string& GetDevUri() const
    {
        return devUri;
    }
};

class CPuOnlineNtfRsp : public CEventRsp
{
public:
    CPuOnlineNtfRsp()
    {
        event = OspExtEventDesc(PU_ONLINE_NTF_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPuVidSrcOsdSetReq : public CEventReq
{
public:
    CPuVidSrcOsdSetReq()
    {
		Clear();
        event = OspExtEventDesc(PU_VIDSRC_OSD_SET_REQ);
    }

    void Clear()
    {
		puChn.Clear();
		puVidSrcOsdCfg.clear();
        CEventReq::Clear();
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

private:
    TChannel        puChn;          //设备视频源通道
	TPuVidSrcOsdCfg puVidSrcOsdCfg; //视频源的OSD字幕配置

public:
	void SetPuChn(const TChannel& var)
	{
		puChn = var;
	}
	TChannel& GetPuChn()
	{
		return puChn;
	}
	const TChannel& GetPuChn() const
	{
		return puChn;
	}

	void SetPuVidSrcOsdCfg(const TPuVidSrcOsdCfg& var)
	{
		puVidSrcOsdCfg = var;
	}
	TPuVidSrcOsdCfg& GetPuVidSrcOsdCfg()
	{
		return puVidSrcOsdCfg;
	}
	const TPuVidSrcOsdCfg& GetPuVidSrcOsdCfg() const
	{
		return puVidSrcOsdCfg;
	}

	//事务模板会使用
	const string& GetDevUri() const
	{
		return puChn.GetDevUri();
	}
};

class CPuVidSrcOsdSetRsp : public CEventRsp
{
public:
    CPuVidSrcOsdSetRsp()
    {
		Clear();
        event = OspExtEventDesc(PU_VIDSRC_OSD_SET_RSP);
    }

public:
	void Clear()
	{
		ret.clear();
	}

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

private:
	TPuVidSrcOsdRet ret;

public:
	void SetPuVidSrcOsdRet(const TPuVidSrcOsdRet& var)
	{
		ret = var;
	}
	TPuVidSrcOsdRet& GetPuVidSrcOsdRet()
	{
		return ret;
	}
	const TPuVidSrcOsdRet& GetPuVidSrcOsdRet() const
	{
		return ret;
	}
};

class CPuVidSrcOsdGetReq : public CEventReq
{
public:
    CPuVidSrcOsdGetReq()
    {
		Clear();
        event = OspExtEventDesc(PU_VIDSRC_OSD_GET_REQ);
    }

    void Clear()
    {
		puChn.Clear();
        CEventReq::Clear();
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

private:
    TChannel puChn; //设备视频源通道

public:
	void SetPuChn(const TChannel& var)
	{
		puChn = var;
	}
	TChannel& GetPuChn()
	{
		return puChn;
	}
	const TChannel& GetPuChn() const
	{
		return puChn;
	}

	//事务模板会使用
	const string& GetDevUri() const
	{
		return puChn.GetDevUri();
	}
};

class CPuVidSrcOsdGetRsp : public CEventRsp
{
public:
    CPuVidSrcOsdGetRsp()
    {
		Clear();
        event = OspExtEventDesc(PU_VIDSRC_OSD_GET_RSP);
    }

public:
	void Clear()
	{
		puVidSrcOsdCfg.clear();
	}

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

private:
	TPuVidSrcOsdCfg puVidSrcOsdCfg; //视频源的OSD字幕配置

public:
	void SetPuVidSrcOsdCfg(const TPuVidSrcOsdCfg& var)
	{
		puVidSrcOsdCfg = var;
	}
	TPuVidSrcOsdCfg& GetPuVidSrcOsdCfg()
	{
		return puVidSrcOsdCfg;
	}
	const TPuVidSrcOsdCfg& GetPuVidSrcOsdCfg() const
	{
		return puVidSrcOsdCfg;
	}
};

class CCmuDisconnectPuReq : public CEventReq
{
public:
    CCmuDisconnectPuReq()
    {
        event = OspExtEventDesc(CMU_DISCONNECT_PU_REQ);
    }

    void Clear()
    {
        devUri.clear();
        CEventReq::Clear();
    }
public:
    const string& GetDevUri() const
    {
        return devUri;
    }

    string& GetDevUri()
    {
        return devUri;
    }

    void SetDevUri(const string& tSipUri)
    {
        devUri = tSipUri;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
private:
    string           devUri;  //前端设备URI
};

class CCmuDisconnectPuRsp : public CEventRsp
{
public:
    CCmuDisconnectPuRsp()
    {
        event = OspExtEventDesc(CMU_DISCONNECT_PU_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

//////////////////////////////////////////////////////////////////////////
//cmu通知pui_ovf模块probe onvif前端
class CProbeOnvifPuReq : public CEventReq
{
public:
	CProbeOnvifPuReq()
	{
		event = OspExtEventDesc(PROBE_ONVIFPU_REQ);
	}

private:
	TProbeOnvifPuList probeOnvifPuList;

public:
	TProbeOnvifPuList& GetProbeOnvifPuList()
	{
		return probeOnvifPuList;
	}

	const TProbeOnvifPuList& GetProbeOnvifPuList() const
	{
		return probeOnvifPuList;
	}

	void SetProbeOnvifPuList(const TProbeOnvifPuList& tProbeOnvifPuList)
	{
		probeOnvifPuList = tProbeOnvifPuList;
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CProbeOnvifPuRsp : public CEventRsp
{
public:
	CProbeOnvifPuRsp()
	{
		event = OspExtEventDesc(PROBE_ONVIFPU_RSP);
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
//////////////////////////////////////////////////////////////////////////
//cui注册平台

class CCuiRegReq : public CEventReq
{
public:
    CCuiRegReq()
    {
        event = OspExtEventDesc(CUI_REG_REQ);
    }

    void Clear()
    {
		isSupportNA = false;

        devUri.clear();
        ipAddrList.clear();
		netSegmentInfo.Clear();
        CEventReq::Clear();
    }

public:
    const string& GetDevUri() const
    {
        return devUri;
    }

    string& GetDevUri()
    {
        return devUri;
    }

    void SetDevUri(const string& strSipUri)
    {
        devUri = strSipUri;
    }

    const CIpAddrList& GetIpAddrList() const
    {
        return ipAddrList;
    }

    CIpAddrList& GetIpAddrList()
    {
        return ipAddrList;
    }

    void SetIpAddrList(const CIpAddrList& cIpAddrList)
    {
        ipAddrList = cIpAddrList;
    }

	void SetNetSegmentInfo(const TNetSegmentAllInfo& cNetSegment)
	{
		netSegmentInfo = cNetSegment;
	}
	TNetSegmentAllInfo& GetNetSegmentInfo()
	{
		return netSegmentInfo;
	}
	const TNetSegmentAllInfo& GetNetSegmentInfo() const
	{
		return netSegmentInfo;
	}

	void SetIsSupportNA(bool bIsSupportNA)
	{
		isSupportNA = bIsSupportNA;
	}
	bool GetIsSupportNA() const
	{
		return isSupportNA;
	}

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

private:
    string            devUri;       //cui设备URI
    CIpAddrList       ipAddrList;   //cui网络地址列表, SIP没有端口概念, 这里的端口是指CUI的WebService监听端口, 默认80
	TNetSegmentAllInfo   netSegmentInfo; //cui网段地址列表
	bool              isSupportNA;  //是否支持natagent
};

class CCuiRegRsp : public CEventRsp
{
public:
    CCuiRegRsp()
    {
        event = OspExtEventDesc(CUI_REG_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

//获取方位角信息
class CCuiGetAzimuthReq : public CEventReq
{
public:
	CCuiGetAzimuthReq()
	{
		event = OspExtEventDesc(CUI_GET_AZIMUTH_REQ);
		Clear();
	}

	void Clear()
	{
		vidSrcChn.Clear();
		CEventReq::Clear();
	}

public:
	void SetVidSrcChn(const TChannel& cVidSrcChn)
	{
		vidSrcChn = cVidSrcChn;
	}
	TChannel& GetVidSrcChn()
	{
		return vidSrcChn;
	}
	const TChannel& GetVidSrcChn() const
	{
		return vidSrcChn;
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);

private:
	TChannel vidSrcChn; //视频源通道
};

class CCuiGetAzimuthRsp : public CEventRsp
{
public:
	CCuiGetAzimuthRsp()
	{
		event = OspExtEventDesc(CUI_GET_AZIMUTH_RSP);
		Clear();
	}

	void Clear()
	{
		azimuthInfo.clear();
		CEventRsp::Clear();
	}

public:
	const string& GetAzimuthInfo() const
	{
		return azimuthInfo;
	}

	string& GetAzimuthInfo()
	{
		return azimuthInfo;
	}

	void SetAzimuthInfo(const string& strAzimuthInfo)
	{
		azimuthInfo = strAzimuthInfo;
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);

private:
	string azimuthInfo; //方位角信息，base64加密后的密文
};

#define BATCH_OPERATE_TYPE_REG               "reg"
#define BATCH_OPERATE_TYPE_UNREG             "unreg"
#define BATCH_OPERATE_TYPE_CHN_STAT          "stat"
#define BATCH_OPERATE_TYPE_GB_DOMAIN_OFFLINE "domainoff"
#define BATCH_OPERATE_TYPE_LOG_ALARM         "alarmlog"
#define BATCH_OPERATE_TYPE_LOG_DEVICE        "devicelog"
#define BATCH_OPERATE_TYPE_LOG_OPERATE       "operatelog"
class CBatchOperateReq : public CEventReq
{
public:
	CBatchOperateReq()
	{
		Clear();
		event = OspExtEventDesc(BATCH_OPERATE_REQ);
	}

	void Clear()
	{
		data.clear();
	}

	const vector<TGeneralData> &GetData() const
	{
		return data;
	}
	vector<TGeneralData> &GetData()
	{
		return data;
	}
	void SetData(const vector<TGeneralData> &val)
	{
		data = val;
	}

private:
	vector<TGeneralData> data;
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};
class CBatchOperateRsp : public CEventRsp
{
public:
	CBatchOperateRsp()
	{
		event = OspExtEventDesc(BATCH_OPERATE_RSP);
	}

	const vector<int> &GetResult() const
	{
		return result;
	}
	vector<int> &GetResult()
	{
		return result;
	}
	void SetResult(const vector<int> &cResult)
	{
		result = cResult;
	}

private:
	vector<int> result;
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

#define BATCH_PU_REG_TYPE_REG "reg"
#define BATCH_PU_REG_TYPE_UNREG "unreg"
#define BATCH_PU_REG_TYPE_OFFLINE "offline"
class CBatchPuRegReq : public CEventReq
{
public:
    CBatchPuRegReq()
    {
        Clear();
        event = OspExtEventDesc(BATCH_PU_REG_REQ);
    }

    void Clear()
    {
        data.clear();
    }

    const vector<TGeneralData> &GetData() const
    {
        return data;
    }
    vector<TGeneralData> &GetData()
    {
        return data;
    }
    void SetData(const vector<TGeneralData> &val)
    {
        data = val;
    }

private:

    // 对于批量事务来说，TGeneralData的type为reg、unreg，TGeneralData的content为对应CEventReq的ToXml结果
    // 并非所有的事务都支持批量事务，使用之前最好和cmu的维护人员确认下目前哪些事务支持批量处理
    // 目前pu的批量注册事务包括三条基本操作，对应的TGeneralData的type分别为：BATCH_PU_REG_TYPE_REG、BATCH_PU_REG_TYPE_UNREG
    // BATCH_PU_REG_TYPE_REG对应的content为DevUri，BATCH_PU_REG_TYPE_UNREG对应的content也是DevUri
    // BATCH_PU_REG_TYPE_OFFLINE对应的content为redis中保存国标域下的设备的set对应的key
    vector<TGeneralData> data;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CBatchPuRegRsp : public CEventRsp
{
public:
    CBatchPuRegRsp()
    {
        event = OspExtEventDesc(BATCH_PU_REG_RSP);
    }

    const vector<int> &GetResult() const
    {
        return result;
    }
    vector<int> &GetResult()
    {
        return result;
    }
    void SetResult(const vector<int> &cResult)
    {
        result = cResult;
    }
    
private:
    vector<int> result;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

//CU登录、登出
class CCuLoginReq : public CEventReq
{
public:
    CCuLoginReq()
    {
        event = OspExtEventDesc(CU_LOGIN_REQ);
        isSupportNA = true;
    }

    void Clear()
    {
        userName.clear();
        password.clear();
        version.clear();
        usedCuiIp.clear();
        isSupportNA = false;
        CEventReq::Clear();
    }

public:
    const string& GetUserName() const
    {
        return userName;
    }

    string& GetUserName()
    {
        return userName;
    }

    void SetUserName(const string& strUserName)
    {
        userName = strUserName;
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

    const string& GetVersion() const
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

    void SetUsedCuiIp(const string& strUsedCuiIp)
    {
        usedCuiIp = strUsedCuiIp;
    }
    string& GetUsedCuiIp()
    {
        return usedCuiIp;
    }
    const string& GetUsedCuiIp() const
    {
        return usedCuiIp;
    }
    void SetIsSupportNA(bool bIsSupportNA)
    {
        isSupportNA = bIsSupportNA;
    }
    bool GetIsSupportNA() const
    {
        return isSupportNA;
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

private:
    string          userName;       //用户名(未加密),例如ligang@nanjing
    string          password;       //密码(密文)
    string          version;        //CU版本号
    string          usedCuiIp;      //cu接入的CUI本地IP（存在NAT时使用NAT内地址）
    bool            isSupportNA;    //是否支持natagent
};

class CCuLoginRsp : public CEventRsp
{
public:
    CCuLoginRsp()
    {
        event = OspExtEventDesc(CU_LOGIN_RSP);
    }

    void Clear()
    {
        cmsVer.clear();
        CEventRsp::Clear();
    }

public:
    const string& GetCmsVer() const
    {
        return cmsVer;
    }

    string& GetCmsVer()
    {
        return cmsVer;
    }

    void SetCmsVer(const string& strCmsVer)
    {
        cmsVer = strCmsVer;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

private:
    string          cmsVer;             //平台版本信息
};

class CCuLogoutReq : public CEventReq
{
public:
    CCuLogoutReq()
    {
        event = OspExtEventDesc(CU_LOGOUT_REQ);
    }

    void Clear()
    {
        CEventReq::Clear();
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CCuLogoutRsp : public CEventRsp
{
public:
    CCuLogoutRsp()
    {
        event = OspExtEventDesc(CU_LOGOUT_RSP);
    }

    void Clear()
    {
        CEventRsp::Clear();
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUpdateCuLoginInfoReq : public CEventReq
{
public:
	CUpdateCuLoginInfoReq()
	{
		event = OspExtEventDesc(UPDATE_CULOGIN_INFO_REQ);
		isSupportNA = false;
	}
public:
	void Clear()
	{
		userName.clear();
		password.clear();
		version.clear();
		usedCuiIp.clear();
		isSupportNA = false;
		CEventReq::Clear();
	}
public:
	const string& GetUserName() const
	{
		return userName;
	}

	string& GetUserName()
	{
		return userName;
	}

	void SetUserName(const string& strUserName)
	{
		userName = strUserName;
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

	const string& GetVersion() const
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

	void SetUsedCuiIp(const string& strUsedCuiIp)
	{
		usedCuiIp = strUsedCuiIp;
	}
	string& GetUsedCuiIp()
	{
		return usedCuiIp;
	}
	const string& GetUsedCuiIp() const
	{
		return usedCuiIp;
	}
	void SetIsSupportNA(bool bIsSupportNA)
	{
		isSupportNA = bIsSupportNA;
	}
	bool GetIsSupportNA() const
	{
		return isSupportNA;
	}
private:
	string          userName;       //用户名(未加密),例如ligang@nanjing
	string          password;       //密码(密文)
	string          version;        //CU版本号
	string          usedCuiIp;      //cu接入的CUI本地IP（存在NAT时使用NAT内地址）
	bool            isSupportNA;    //是否支持natagent
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUpdateCuLoginInfoRsp : public CEventRsp
{
public:
	CUpdateCuLoginInfoRsp()
	{
		event = OspExtEventDesc(UPDATE_CULOGIN_INFO_RSP);
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CCuDisconnectNtfReq : public CEventReq
{
public:
    CCuDisconnectNtfReq()
    {
        event = OspExtEventDesc(CU_DISCONNECT_NTF_REQ);
    }
    void Clear()
    {
        CEventReq::Clear();
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CCuDisconnectNtfRsp : public CEventRsp
{
public:
    CCuDisconnectNtfRsp()
    {
        event = OspExtEventDesc(CU_DISCONNECT_NTF_RSP);
    }
    void Clear()
    {
        CEventRsp::Clear();
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CCuGetCuiListReq : public CEventReq
{
public:
    CCuGetCuiListReq()
    {
        event = OspExtEventDesc(CU_GETCUILIST_REQ);
    }
    ~CCuGetCuiListReq()
    {
        
    }
    void Clear()
    {
        CEventReq::Clear();
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
private:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CCuGetCuiListRsp : public CEventRsp
{
public:
    CCuGetCuiListRsp()
    {
        event = OspExtEventDesc(CU_GETCUILIST_RSP);
    }
    ~CCuGetCuiListRsp()
    {

    }
    void Clear()
    {
        CEventRsp::Clear();
    }

public:
    const CIpAddrList& GetIpAddrList() const
    {
        return addrList;
    }

    CIpAddrList& GetIpAddrList()
    {
        return addrList;
    }

    void SetIpAddrList(const CIpAddrList& cIpAddrList)
    {
        addrList = cIpAddrList;
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
private:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
private:
    CIpAddrList addrList;   //返回CUI的地址列表, 考虑CUI可能不在80端口监听, 所以也算上端口, 最后组成URL返回给CU
    //遗留问题是，每个CUI 有多网段, 是否要把CUI和网段地址列表打包在一起
};

//todo 2011.12.12 交换接口, 涉及媒体能力协商, 等待VTDU部分

class CUpdateRegNetSegReq : public CEventReq
{
public:
	CUpdateRegNetSegReq()
	{
		event = OspExtEventDesc(UPDATE_REG_NETSEG_REQ);
	}
	void Clear()
	{
		netSegmentAllInfo.Clear();
		CEventReq::Clear();
	}

public:
	void SetNetSegmentAllInfo(const TNetSegmentAllInfo& tNetSegmentAllInfo) 
	{
		netSegmentAllInfo = tNetSegmentAllInfo;
	}
	const TNetSegmentAllInfo& GetNetSegmentAllInfo() const
	{
		return netSegmentAllInfo;
	}
	TNetSegmentAllInfo& GetNetSegmentAllInfo()
	{
		return netSegmentAllInfo;
	}
private:
	TNetSegmentAllInfo netSegmentAllInfo;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUpdateRegNetSegRsp : public CEventRsp
{
public:
	CUpdateRegNetSegRsp()
	{
		event = OspExtEventDesc(UPDATE_REG_NETSEG_RSP);
	}
	void Clear()
	{
		CEventRsp::Clear();
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

//////////////////////////////////////////////////////////////////////////
//pu状态订阅

//fanxg-20131209
//整体订阅支持两种方式：使用时二选其一。
//1. 指定域名：订阅该域的所有设备，可以通过刷新订阅增删设备。主要用于国标28181的订阅
//2. 指定设备列表：只订阅指定的设备，可以通过刷新订阅增删状态类型。主要用于平台内部模块之间的订阅

typedef vector<string>  TDevUriList;

//整体订阅(目录订阅)
class CCatalogStatusSsReq : public CCatalogSsReq
{
public:
    CCatalogStatusSsReq()
    {
        event = OspExtEventDesc(CATALOG_STATUS_SS_REQ);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CCatalogStatusSsRsp : public CCatalogSsRsp
{
public:
    CCatalogStatusSsRsp()
    {
        event = OspExtEventDesc(CATALOG_STATUS_SS_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CCatalogStatusRefreshSsReq : public CCatalogRefreshSsReq
{
public:
    CCatalogStatusRefreshSsReq()
    {
        event = OspExtEventDesc(CATALOG_STATUS_REFRESH_SS_REQ);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CCatalogStatusRefreshSsRsp : public CCatalogRefreshSsRsp
{
public:
    CCatalogStatusRefreshSsRsp()
    {
        event = OspExtEventDesc(CATALOG_STATUS_REFRESH_SS_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CCatalogStatusUnSsReq : public CCatalogUnSsReq
{
public:
    CCatalogStatusUnSsReq()
    {
        event = OspExtEventDesc(CATALOG_STATUS_UNSS_REQ);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CCatalogStatusUnSsRsp : public CCatalogUnSsRsp
{
public:
    CCatalogStatusUnSsRsp()
    {
        event = OspExtEventDesc(CATALOG_STATUS_UNSS_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

//整体订阅(列表订阅)
class CPuListStatusSsReq : public CListSsReq
{
public:
    CPuListStatusSsReq()
    {
        event = OspExtEventDesc(PULIST_STATUS_SS_REQ);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPuListStatusSsRsp : public CListSsRsp
{
public:
    CPuListStatusSsRsp()
    {
        event = OspExtEventDesc(PULIST_STATUS_SS_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPuListStatusRefreshSsReq : public CListRefreshSsReq
{
public:
    CPuListStatusRefreshSsReq()
    {
        event = OspExtEventDesc(PULIST_STATUS_REFRESH_SS_REQ);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPuListStatusRefreshSsRsp : public CListRefreshSsRsp
{
public:
    CPuListStatusRefreshSsRsp()
    {
        event = OspExtEventDesc(PULIST_STATUS_REFRESH_SS_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPuListStatusUnSsReq : public CListUnSsReq
{
public:
    CPuListStatusUnSsReq()
    {
        event = OspExtEventDesc(PULIST_STATUS_UNSS_REQ);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPuListStatusUnSsRsp : public CListUnSsRsp
{
public:
    CPuListStatusUnSsRsp()
    {
        event = OspExtEventDesc(PULIST_STATUS_UNSS_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

/*
 * 单点订阅即将被废弃，停止使用【xiongxh】
 */
class CPuStatusSsReq : public CSsReq
{
public:
    CPuStatusSsReq()
    {
        event = OspExtEventDesc(PU_STATUS_SS_REQ);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPuStatusSsRsp : public CSsRsp
{
public:
    CPuStatusSsRsp()
    {
        event = OspExtEventDesc(PU_STATUS_SS_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPuStatusRefreshSsReq : public CRefreshSsReq
{
public:
    CPuStatusRefreshSsReq()
    {
        event = OspExtEventDesc(PU_STATUS_REFRESH_SS_REQ);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPuStatusRefreshSsRsp : public CRefreshSsRsp
{
public:
    CPuStatusRefreshSsRsp()
    {
        event = OspExtEventDesc(PU_STATUS_REFRESH_SS_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPuStatusUnSsReq : public CUnSsReq
{
public:
    CPuStatusUnSsReq()
    {
        event = OspExtEventDesc(PU_STATUS_UNSS_REQ);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPuStatusUnSsRsp : public CUnSsRsp
{
public:
    CPuStatusUnSsRsp()
    {
        event = OspExtEventDesc(PU_STATUS_UNSS_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPuStatusNtyReq : public CEventReq
{
public:
    CPuStatusNtyReq()
    {
        event = OspExtEventDesc(PU_STATUS_NTF_REQ);
    };

    void Clear()
    {
        onOff.Clear();
    }

public:
    const string& GetDevURI() const
    {
        return devURI;
    }
    const string& GetDevURI()
    {
        return devURI;
    }
    void SetDevURI(const string& strDevURI)
    {
        devURI = strDevURI;
    }
    const TPuStatus& GetOnOffStatus() const
    {
        return onOff;
    }
    TPuStatus& GetOnOffStatus()
    {
        return onOff;
    }
    void SetOnOffStatus(const TPuStatus& tOnOff)
    {
        onOff = tOnOff;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

private:
    string              devURI;     //pu uuid@domain
    TPuStatus           onOff;      //上下线状态
};

typedef CNtfRsp CPuStatusNtyRsp;

//pui通知cmu使用
class CPuDevConfigNtyReq : public CEventReq
{
public:
    CPuDevConfigNtyReq()
    {
        event = OspExtEventDesc(PU_DEV_CONFIG_NTF_REQ);
    };

    void Clear()
    {
        devURI.clear();
        config.Clear();
    }

public:

    const string& GetDevURI() const
    {
        return devURI;
    }
    const string& GetDevURI()
    {
        return devURI;
    }
    void SetDevURI(const string& strDevURI)
    {
        devURI = strDevURI;
    }

    const TPuDevConfig& GetConfig() const
    {
        return config;
    }
    TPuDevConfig& GetConfig()
    {
        return config;
    }
    void SetConfig(const TPuDevConfig& tConfig)
    {
        config = tConfig;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

private:
    string              devURI;       //pu uuid@domain
    TPuDevConfig        config;       //前端属性、能力集、配置
};

typedef CNtfRsp CPuDevConfigNtyRsp;

//状态订阅中使用
class CPuConfigNtyReq : public CEventReq
{
public:
    CPuConfigNtyReq()
    {
        event = OspExtEventDesc(PU_CONFIG_NTF_REQ);
    }

    void Clear()
    {
        devURI.clear();
        config.Clear();
    }

public:

    const string& GetDevURI() const
    {
        return devURI;
    }
    const string& GetDevURI()
    {
        return devURI;
    }
    void SetDevURI(const string& strDevURI)
    {
        devURI = strDevURI;
    }

    const TPuDevConfig& GetConfig() const
    {
        return config;
    }
    TPuDevConfig& GetConfig()
    {
        return config;
    }
    void SetConfig(const TPuDevConfig& tConfig)
    {
        config = tConfig;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

private:
    string                 devURI;    //pu uuid@domain
    TPuDevConfig           config;    //前端属性、能力集、配置
};

typedef CNtfRsp CPuConfigNtyRsp;

class CPuServiceStatusNtyReq : public CEventReq
{
public:
    CPuServiceStatusNtyReq()
    {
        event = OspExtEventDesc(PU_SERVICE_STATUS_NTF_REQ);
    };

    void Clear()
    {
        service.Clear();
    }

public:
    const string & GetDevURI() const
    {
        return devURI;
    }
    const string & GetDevURI()
    {
        return devURI;
    }
    void SetDevURI(const string &strDevURI)
    {
        devURI = strDevURI;
    }
    const TPuServiceStatus& GetServiceStatus() const
    {
        return service;
    }
    TPuServiceStatus& GetServiceStatus()
    {
        return service;
    }
    void SetServiceStatus(const TPuServiceStatus& tService)
    {
        service = tService;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

private:
    string              devURI;     //pu uuid @domain
    TPuServiceStatus    service;    //业务状态
};

typedef CNtfRsp CPuServiceStatusNtyRsp;

class CPuAlarmStatusNtyReq : public CEventReq
{
public:
    CPuAlarmStatusNtyReq()
    {
        event = OspExtEventDesc(PU_ALARM_STATUS_NTF_REQ);
    };

    void Clear()
    {
        alarm.Clear();
    }

public:
    const string & GetDevURI() const
    {
        return devURI;
    }
    string & GetDevURI()
    {
        return devURI;
    }
    void SetDevURI(const string &strDevURI)
    {
        devURI = strDevURI;
    }
    const TPuAlarmStatus& GetAlarmStatus() const
    {
        return alarm;
    }
    TPuAlarmStatus& GetAlarmStatus()
    {
        return alarm;
    }
    void SetAlarmStatus(const TPuAlarmStatus& tAlarm)
    {
        alarm = tAlarm;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

private:
    string              devURI;     //pu uuid @domain
    TPuAlarmStatus      alarm;    //告警状态
};

typedef CNtfRsp CPuAlarmStatusNtyRsp;

class CPuGpsStatusNtyReq : public CEventReq
{
public:
    CPuGpsStatusNtyReq()
    {
        event = OspExtEventDesc(PU_GPS_STATUS_NTF_REQ);
    };

    void Clear()
    {
        gps.Clear();
    }

public:
	const string& GetDevURI() const
	{
		return devURI;
	}
	string& GetDevURI()
	{
		return devURI;
	}
	void SetDevURI(const string& strVal)
	{
		devURI = strVal;
	}
	const TGpsData& GetGpsData() const
	{
		return gps;
	}
	TGpsData& GetGpsData()
	{
		return gps;
	}
	void SetGpsData(const TGpsData& tVal)
	{
		gps = tVal;
	}

private:
    string              devURI;     //pu uuid @domain
    TGpsData            gps;        //gps状态
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

typedef CNtfRsp CPuGpsStatusNtyRsp;

class CPuTransDataStatusNtyReq : public CEventReq
{
public:
    CPuTransDataStatusNtyReq()
    {
        event = OspExtEventDesc(PU_TRANSDATA_STATUS_NTF_REQ);
    };

    void Clear()
    {
        data.Clear();
    }

private:
    string              devURI;                     //pu uuid @domain
    TTransparentData    data;                       //透明数据状态
public:
    void SetDevURI(const string& strDevURI)
    {
        devURI = strDevURI;
    }
    string& GetDevURI()
    {
        return devURI;
    }
    const string& GetDevURI() const
    {
        return devURI;
    }

    void SetData(const TTransparentData& cData)
    {
        data = cData;
    }
    TTransparentData& GetData()
    {
        return data;
    }
    const TTransparentData& GetData() const
    {
        return data;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

typedef CNtfRsp CPuTransDataStatusNtyRsp;


class CPuBatchNtyReq : public CEventReq
{
public:
	CPuBatchNtyReq()
	{
		Clear();
		event = OspExtEventDesc(PU_BATCH_NTF_REQ);
	}

	void Clear()
	{
		data.clear();
	}

	const vector<TGeneralData> &GetData() const
	{
		return data;
	}
	vector<TGeneralData> &GetData()
	{
		return data;
	}
	void SetData(const vector<TGeneralData> &val)
	{
		data = val;
	}
	
private:
	vector<TGeneralData> data;			// 对于批量通知来说，TGeneralData的type为TSsType，TGeneralData的content为对应ntf的序列化结果
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPuBatchNtyRsp : public CEventRsp
{
public:
	CPuBatchNtyRsp()
	{
		Clear();
		event = OspExtEventDesc(PU_BATCH_NTF_RSP);
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

//////////////////////////////////////////////////////////////////////////
//ptz控制操作
class CPuPtzCtrlReq : public CEventReq
{
public:
    CPuPtzCtrlReq()
    {
        event = OspExtEventDesc(PU_PTZ_CTRL_REQ);
    }

    void Clear()
    {
        userName.clear();
        devChn.Clear();
        camCtrl.Clear();
        CEventReq::Clear();
    }

public:
    const string &GetUserName() const
    {
        return userName;
    }
    string &GetUserName()
    {
        return userName;
    }
    void SetUserName(const string &strUserName)
    {
        userName = strUserName;
    }

    const TChannel& GetDevChn() const
    {
        return devChn;
    }

    TChannel& GetDevChn()
    {
        return devChn;
    }

    void SetDevChn(const TChannel& tChn)
    {
        devChn = tChn;
    }

    const TCamCtrl& GetCamCtrl() const
    {
        return camCtrl;
    }

    TCamCtrl& GetCamCtrl()
    {
        return camCtrl;
    }

    void SetCamCtrl(const TCamCtrl& tCamCtrl)
    {
        camCtrl = tCamCtrl;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

private:
    string          userName;   //cu登录用户名, 例如 ligang@nanjing
    TChannel         devChn;    //设备通道
    TCamCtrl         camCtrl;   //控制信息
};

class CPuPtzCtrlWithLockRsp : public CEventRsp
{
public:
	CPuPtzCtrlWithLockRsp()
	{
		Clear();
	}

	void Clear()
	{
		errorDesc.clear();
		userId.clear();
		username.clear();
		beginTime.clear();
		endTime.clear();
		priPtzLevel = 0;

		CEventRsp::Clear();
	}
public:
	const string &GetErrorDesc() const
	{
		return errorDesc;
	}
	string &GetErrorDesc() 
	{
		return errorDesc;
	}
	void SetErrorDesc(const string &strErrorDesc)
	{
		errorDesc = strErrorDesc;
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

private:
	string errorDesc;       //有可能摄像头被锁定而给出抢占提示
	string userId;	        // 加锁用户Id
	string username;        // 加锁用户名;
	string beginTime;    	// 锁定开始时间.
	string endTime;        	// 自动解锁时间.
	int priPtzLevel;        //加锁用户PTZ锁定权限
};

class CPuPtzCtrlRsp : public CPuPtzCtrlWithLockRsp
{
public:
	CPuPtzCtrlRsp()
	{
		event = OspExtEventDesc(PU_PTZ_CTRL_RSP);
		Clear();
	}

	void Clear()
	{	
		CPuPtzCtrlWithLockRsp::Clear();
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);	
};

//透明串口
class CPuSendTransparentDataReq : public CEventReq
{
public:
    CPuSendTransparentDataReq()
    {
        event = OspExtEventDesc(PU_SEND_TRANSPARENT_DATA_REQ);
    }
    ~CPuSendTransparentDataReq(){}
    void Clear()
    {
        userName.clear();
        devChn.Clear();
        hexData.clear();
        CEventReq::Clear();
    }
public:
    const string &GetUserName() const
    {
        return userName;
    }
    string &GetUserName()
    {
        return userName;
    }
    void SetUserName(const string &strUserName)
    {
        userName = strUserName;
    }

    const TChannel& GetDevChn() const
    {
        return devChn;
    }

    TChannel& GetDevChn()
    {
        return devChn;
    }

    void SetDevChn(const TChannel& tPuChn)
    {
        devChn = tPuChn;
    }

    const string &GetHexTransparentData() const
    {
        return hexData;
    }
    string &GetHexTransparentData()
    {
        return hexData;
    }
    void SetHexTransparentData(const string &strData)
    {
        hexData = strData;
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
private:
    string          userName;   //cu登录用户名, 例如 ligang@nanjing
    TChannel        devChn;     //设备通道, chnNO为视频源
    string          hexData;    //十六进制的透明串口数据
};

class CPuSendTransparentDataRsp : public CEventRsp
{
public:
    CPuSendTransparentDataRsp()
    {
        event = OspExtEventDesc(PU_SEND_TRANSPARENT_DATA_RSP);
    }
    ~CPuSendTransparentDataRsp(){}
    void Clear()
    {
        errorDesc.clear();
        CEventRsp::Clear();
    }
public:
    const string &GetErrorDesc() const
    {
        return errorDesc;
    }
    string &GetErrorDesc() 
    {
        return errorDesc;
    }
    void SetErrorDesc(const string &strErrorDesc)
    {
        errorDesc = strErrorDesc;
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

private:
    string errorDesc;       //有可能摄像头被锁定而给出抢占提示
};

//PTZ 锁定
class CPuPtzLockReq : public CEventReq
{
public:
    CPuPtzLockReq()
    {
        event = OspExtEventDesc(PU_PTZ_LOCK_REQ);
    }
    ~CPuPtzLockReq(){}
    void Clear()
    {
        devChn.Clear();
        begin.clear();
        end.clear();
        userName.clear();
        password.clear();
        CEventReq::Clear();
    }
public:
    const TChannel &GetPuChn() const
    {
        return devChn;
    }
    TChannel &GetPuChn()
    {
        return devChn;
    }
    void SetPuChn(const TChannel& tPuChn)
    {
        devChn = tPuChn;
    }
    const string &GetBeginLockTimeStr() const
    {
        return begin;
    }
    string &GetBeginLockTimeStr()
    {
        return begin;
    }
    const time_t GetBeginLockTime() const
    {
        CCMSTime cmsTime(begin);
        return cmsTime.GetTime();
    }
    time_t GetBeginLockTime()
    {
        CCMSTime cmsTime(begin);
        return cmsTime.GetTime();
    }
    void SetBeginLockTime(const string& strTime)
    {
        begin = strTime;
    }
    void SetBeginLockTime(const time_t tTime)
    {
        CCMSTime cmsTime(tTime);
        begin = cmsTime.GetXSDDateTime();
    }
    const string &GetEndLockTimeStr() const
    {
        return end;
    }
	string &GetEndLockTimeStr()
	{
		return end;
	}
    const time_t GetEndLockTime() const
    {
        CCMSTime cmsTime(end);
        return cmsTime.GetTime();
    }
    time_t GetEndLockTime()
    {
        CCMSTime cmsTime(end);
        return cmsTime.GetTime();
    }
    void SetEndLockTime(const string& strTime)
    {
        end = strTime;
    }
    void SetEndLockTime(const time_t tTime)
    {
        CCMSTime cmsTime(tTime);
        end = cmsTime.GetXSDDateTime();
    }
    const string &GetLockUserName() const
    {
        return userName;
    }
    string &GetLockUserName()
    {
        return userName;
    }
    void SetLockUserName(const string &strUserName)
    {
        userName = strUserName;
    }
    const string &GetLockPassword() const
    {
        return password;
    }
    string &GetLockPassword()
    {
        return password;
    }
    void SetLockPassword(const string& strPassword)
    {
        password = strPassword;
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

private:
    TChannel devChn;    //chnNO是视频源
    string   begin;     //起始时间
    string   end;       //解锁时间
    string   userName;  //加锁用户
    string   password;  //加锁密码
};

class CPuPtzLockRsp : public CEventRsp
{
public:
    CPuPtzLockRsp()
    {
        event = OspExtEventDesc(PU_PTZ_LOCK_RSP);
    }
    ~CPuPtzLockRsp(){}
    void Clear()
    {
        CEventRsp::Clear();
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPuPtzUnlockReq : public CEventReq
{
public:
    CPuPtzUnlockReq()
    {
        event = OspExtEventDesc(PU_PTZ_UNLOCK_REQ);
    }
    ~CPuPtzUnlockReq(){}
    void Clear()
    {
        devChn.Clear();
        userName.clear();
        password.clear();
        CEventReq::Clear();
    }
public:
    const TChannel &GetPuChn() const
    {
        return devChn;
    }
    TChannel &GetPuChn()
    {
        return devChn;
    }
    void SetPuChn(const TChannel& tPuChn)
    {
        devChn = tPuChn;
    }
    const string &GetUnlockUserName() const
    {
        return userName;
    }
    string &GetUnlockUserName()
    {
        return userName;
    }
    void SetUnlockUserName(const string &strUserName)
    {
        userName = strUserName;
    }
    const string &GetUnlockPassword() const
    {
        return password;
    }
    string &GetUnlockPassword()
    {
        return password;
    }
    void SetUnlockPassword(const string& strPassword)
    {
        password = strPassword;
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
private:
    TChannel devChn;    //chnNO为视频源
    string   userName;  //加锁用户
    string   password;  //加锁密码
};

class CPuPtzUnlockRsp : public CEventRsp
{
public:
    CPuPtzUnlockRsp()
    {
        event = OspExtEventDesc(PU_PTZ_UNLOCK_RSP);
    }
    ~CPuPtzUnlockRsp(){}
    void Clear()
    {
        CEventRsp::Clear();
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

//////////////////////////////////////////////////////////////////////////
//视频编码参数配置

class CPuVidEncParamSetReq : public CEventReq
{
public:
    CPuVidEncParamSetReq()
    {
        event = OspExtEventDesc(PU_VID_ENC_PARAM_SET_REQ);
    }

    void Clear()
    {
        userUri.clear();
        devChn.Clear();
        param.Clear();
    }

public:
    const string& GetUserUri() const
    {
        return userUri;
    }

    string& GetUserUri()
    {
        return userUri;
    }

    void SetUserUri(const string& tSipUri)
    {
        userUri = tSipUri;
    }

    const TChannel& GetDevChn() const
    {
        return devChn;
    }

    TChannel& GetDevChn()
    {
        return devChn;
    }

    void SetDevChn(const TChannel& tDevChn)
    {
        devChn = tDevChn;
    }

    const TVideoEncodeParam& GetVidEncParam() const
    {
        return param;
    }

    TVideoEncodeParam& GetVidEncParam()
    {
        return param;
    }

    void SetVidEncParam(const TVideoEncodeParam& tVidEncParam)
    {
        param = tVidEncParam;
    }

    //事务模板会使用
    const string& GetDevUri() const
    {
        return devChn.GetDevUri();
    }

//     const TSurportMediaType& GetSupportedMediaType() const 
//     {
//         return surportedMediaType;
//     }
// 
//     TSurportMediaType& GetSupportedMediaType() 
//     {
//         return surportedMediaType;
//     }
// 
//     void SetSupportedMediaType(const TSurportMediaType& tSupportedMediaType) 
//     {
//         surportedMediaType = tSupportedMediaType;
//     }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

private:
    string             userUri;       //操作用户
    TChannel           devChn;        //设备通道
    TVideoEncodeParam  param;         //视频编码参数
    //TSurportMediaType  surportedMediaType;  //所有支持的媒体格式
};

class CPuVidEncParamSetExReq : public CEventReq
{
public:
	CPuVidEncParamSetExReq()
	{
		event = OspExtEventDesc(PU_VID_ENC_PARAM_SET_EX_REQ);
	}

	void Clear()
	{
		userUri.clear();
		devChn.Clear();
		param.Clear();
	}

public:
	const string& GetUserUri() const
	{
		return userUri;
	}

	string& GetUserUri()
	{
		return userUri;
	}

	void SetUserUri(const string& tSipUri)
	{
		userUri = tSipUri;
	}

	const TChannel& GetDevChn() const
	{
		return devChn;
	}

	TChannel& GetDevChn()
	{
		return devChn;
	}

	void SetDevChn(const TChannel& tDevChn)
	{
		devChn = tDevChn;
	}

	const TVideoEncodeParam& GetVidEncParam() const
	{
		return param;
	}

	TVideoEncodeParam& GetVidEncParam()
	{
		return param;
	}

	void SetVidEncParam(const TVideoEncodeParam& tVidEncParam)
	{
		param = tVidEncParam;
	}

	//事务模板会使用
	const string& GetDevUri() const
	{
		return devChn.GetDevUri();
	}

	//     const TSurportMediaType& GetSupportedMediaType() const 
	//     {
	//         return surportedMediaType;
	//     }
	// 
	//     TSurportMediaType& GetSupportedMediaType() 
	//     {
	//         return surportedMediaType;
	//     }
	// 
	//     void SetSupportedMediaType(const TSurportMediaType& tSupportedMediaType) 
	//     {
	//         surportedMediaType = tSupportedMediaType;
	//     }

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);

private:
	string             userUri;       //操作用户
	TChannel           devChn;        //设备通道
	TVideoEncodeParam  param;         //视频编码参数
	//TSurportMediaType  surportedMediaType;  //所有支持的媒体格式
};

class CPuVidEncParamSetRsp : public CEventRsp
{
public:
    CPuVidEncParamSetRsp()
    {
        event = OspExtEventDesc(PU_VID_ENC_PARAM_SET_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPuVidEncParamSetExRsp : public CEventRsp
{
public:
	CPuVidEncParamSetExRsp()
	{
		event = OspExtEventDesc(PU_VID_ENC_PARAM_SET_EX_RSP);
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class CPuVidEncParamGetReq : public CEventReq
{
public:
    CPuVidEncParamGetReq()
    {
        event = OspExtEventDesc(PU_VID_ENC_PARAM_GET_REQ);
    }

    void Clear()
    {
        userUri.clear();
        devChn.Clear();
    }

public:
    const string& GetUserUri() const
    {
        return userUri;
    }

    string& GetUserUri()
    {
        return userUri;
    }

    void SetUserUri(const string& tSipUri)
    {
        userUri = tSipUri;
    }

    const TChannel& GetDevChn() const
    {
        return devChn;
    }

    TChannel& GetDevChn()
    {
        return devChn;
    }

    void SetDevChn(const TChannel& tDevChn)
    {
        devChn = tDevChn;
    }

    //事务模板会使用
    const string& GetDevUri() const
    {
        return devChn.GetDevUri();
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

private:
    string             userUri;   //操作用户
    TChannel           devChn;    //设备通道

};

class CPuVidEncParamGetExReq : public CEventReq
{
public:
	CPuVidEncParamGetExReq()
	{
		event = OspExtEventDesc(PU_VID_ENC_PARAM_GET_EX_REQ);
	}

	void Clear()
	{
		userUri.clear();
		devChn.Clear();
	}
	
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);

private:
	string             userUri;   //操作用户
	TChannel           devChn;    //设备通道

public:
    void SetUserUri(const string& strUserUri)
    {
        userUri = strUserUri;
    }
    string& GetUserUri()
    {
        return userUri;
    }
    const string& GetUserUri() const
    {
        return userUri;
    }

    void SetDevChn(const TChannel& cDevChn)
    {
        devChn = cDevChn;
    }
    TChannel& GetDevChn()
    {
        return devChn;
    }
    const TChannel& GetDevChn() const
    {
        return devChn;
    }

};

class CPuVidEncParamGetRsp : public CEventRsp
{
public:
    CPuVidEncParamGetRsp()
    {
        event = OspExtEventDesc(PU_VID_ENC_PARAM_GET_RSP);
    }

    void Clear()
    {
        param.Clear();
        surportedMediaType.Clear();
        CEventRsp::Clear();
    }

public:
    const TVideoEncodeParam& GetVidEncParam() const
    {
        return param;
    }

    TVideoEncodeParam& GetVidEncParam()
    {
        return param;
    }

    void SetVidEncParam(const TVideoEncodeParam& tVidEncParam)
    {
        param = tVidEncParam;
    }

    const TSupportMediaType& GetSupportedMediaType() const 
    {
        return surportedMediaType;
    }

    TSupportMediaType& GetSupportedMediaType() 
    {
        return surportedMediaType;
    }

    void SetSupportedMediaType(const TSupportMediaType& tSupportedMediaType) 
    {
        surportedMediaType = tSupportedMediaType;
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

private:
    TVideoEncodeParam  param;               //当前视频编码参数
    TSupportMediaType  surportedMediaType;  //所有支持的媒体格式
};

class CPuVidEncParamGetExRsp : public CEventRsp
{
public:
	CPuVidEncParamGetExRsp()
	{
		event = OspExtEventDesc(PU_VID_ENC_PARAM_GET_EX_RSP);
	}

	void Clear()
	{
		param.Clear();
		surportedMediaType.Clear();
		CEventRsp::Clear();
	}

public:
	const TVideoEncodeParam& GetVidEncParam() const
	{
		return param;
	}

	TVideoEncodeParam& GetVidEncParam()
	{
		return param;
	}

	void SetVidEncParam(const TVideoEncodeParam& tVidEncParam)
	{
		param = tVidEncParam;
	}

	const TSupportMediaType& GetSupportedMediaType() const 
	{
		return surportedMediaType;
	}

	TSupportMediaType& GetSupportedMediaType() 
	{
		return surportedMediaType;
	}

	void SetSupportedMediaType(const TSupportMediaType& tSupportedMediaType) 
	{
		surportedMediaType = tSupportedMediaType;
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);

private:
	TVideoEncodeParam  param;               //当前视频编码参数
	TSupportMediaType  surportedMediaType;  //所有支持的媒体格式
};

//音频编码参数配置
class CPuAudEncParamSetReq : public CEventReq
{
public:
    CPuAudEncParamSetReq()
    {
        event = OspExtEventDesc(PU_AUD_ENC_PARAM_SET_REQ);
    }

    void Clear()
    {
        userUri.clear();
        devChn.Clear();
        param.Clear();
    }

public:
    //事务模板会使用
    const string& GetDevUri() const
    {
        return devChn.GetDevUri();
    }


private:
    string             userUri;                  //操作用户
    TChannel           devChn;                   //设备通道
    TAudioEncodeParam  param;                    //音频编码参数
public:
    void SetUserUri(const string& strUserUri)
    {
        userUri = strUserUri;
    }
    string& GetUserUri()
    {
        return userUri;
    }
    const string& GetUserUri() const
    {
        return userUri;
    }

    void SetDevChn(const TChannel& cDevChn)
    {
        devChn = cDevChn;
    }
    TChannel& GetDevChn()
    {
        return devChn;
    }
    const TChannel& GetDevChn() const
    {
        return devChn;
    }

    void SetParam(const TAudioEncodeParam& cParam)
    {
        param = cParam;
    }
    TAudioEncodeParam& GetParam()
    {
        return param;
    }
    const TAudioEncodeParam& GetParam() const
    {
        return param;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPuAudEncParamSetRsp : public CEventRsp
{
public:
    CPuAudEncParamSetRsp()
    {
        event = OspExtEventDesc(PU_AUD_ENC_PARAM_SET_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPuAudEncParamGetReq : public CEventReq
{
public:
    CPuAudEncParamGetReq()
    {
        event = OspExtEventDesc(PU_AUD_ENC_PARAM_GET_REQ);
    }

    void Clear()
    {
        userUri.clear();
        devChn.Clear();
    }
    //事务模板会使用
    const string& GetDevUri() const
    {
        return devChn.GetDevUri();
    }

private:
    string             userUri;                  //操作用户
    TChannel           devChn;                   //设备通道

public:
    void SetUserUri(const string& strUserUri)
    {
        userUri = strUserUri;
    }
    string& GetUserUri()
    {
        return userUri;
    }
    const string& GetUserUri() const
    {
        return userUri;
    }

    void SetDevChn(const TChannel& cDevChn)
    {
        devChn = cDevChn;
    }
    TChannel& GetDevChn()
    {
        return devChn;
    }
    const TChannel& GetDevChn() const
    {
        return devChn;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPuAudEncParamGetRsp : public CEventRsp
{
public:
    CPuAudEncParamGetRsp()
    {
        event = OspExtEventDesc(PU_AUD_ENC_PARAM_GET_RSP);
    }

    void Clear()
    {
        param.Clear();
        surportedMediaType.Clear();
        CEventRsp::Clear();
    }

private:
    TAudioEncodeParam  param;                              //当前音频编码参数
    TSupportMediaType  surportedMediaType;                 //所有支持的媒体格式
public:
    void SetParam(const TAudioEncodeParam& cParam)
    {
        param = cParam;
    }
    TAudioEncodeParam& GetParam()
    {
        return param;
    }
    const TAudioEncodeParam& GetParam() const
    {
        return param;
    }

    void SetSurportedMediaType(const TSupportMediaType& cSurportedMediaType)
    {
        surportedMediaType = cSurportedMediaType;
    }
    TSupportMediaType& GetSurportedMediaType()
    {
        return surportedMediaType;
    }
    const TSupportMediaType& GetSurportedMediaType() const
    {
        return surportedMediaType;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


//平台透传VSIP信令对pu进行控制
#define   INVALID_PU_SUB_DEV_NO    (-1)   //无效子设备号，表示直接控制设备本身
class CPuVsipOperateReq : public CEventReq
{
public:
    CPuVsipOperateReq()
    {
        event = OspExtEventDesc(PU_VSIP_OPERATE_REQ);
    }

    void Clear()
    {
        userUri.clear();
        devUri.clear();
        subDevNo = INVALID_PU_SUB_DEV_NO;
        vsipData.clear();
        CEventReq::Clear();
    }

public:
    const string& GetUserUri() const
    {
        return userUri;
    }

    string& GetUserUri()
    {
        return userUri;
    }

    void SetUserUri(const string& tSipUri)
    {
        userUri = tSipUri;
    }

    const string& GetDevUri() const
    {
        return devUri;
    }

    string& GetDevUri()
    {
        return devUri;
    }

    void SetDevUri(const string& strDevUri)
    {
        devUri = strDevUri;
    }

    void SetSubDevNo(int nSubDevNo)
    {
        subDevNo = nSubDevNo;
    }
    int GetSubDevNo() const
    {
        return subDevNo;
    }

    const string& GetVsipData() const
    {
        return vsipData;
    }

    string& GetVsipData()
    {
        return vsipData;
    }

    void SetVsipData(const string& strVsipData)
    {
        vsipData = strVsipData;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

private:
    string            userUri;   //操作用户
    string            devUri;    //操作设备
    int               subDevNo;  //子设备号：nvr中接的IPC序号，
    string            vsipData;  //base64格式的vsip数据
};

class CPuVsipOperateRsp : public CEventRsp
{
public:
    CPuVsipOperateRsp()
    {
        event = OspExtEventDesc(PU_VSIP_OPERATE_RSP);
    }

    void Clear()
    {
        vsipData.clear();
        CEventRsp::Clear();
    }

public:
    const string& GetVsipData() const
    {
        return vsipData;
    }

    string& GetVsipData()
    {
        return vsipData;
    }

    void SetVsipData(const string& strVsipData)
    {
        vsipData = strVsipData;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

private:
    string             vsipData;  //base64格式的vsip数据
};

//平台对pu进行透明操作
typedef  string TPuTransOprType;
#define PU_TRANS_OPR_SEND_COMDATA       "SendComData"       //发送串口数据
#define PU_TRANS_OPR_EXT_PTZ_CTRL       "ExtPtzCtrl"        //扩展PTZ操作
#define PU_TRANS_OPR_SEND_EXT_DATA      "SendExtData"       //发送透明数据

class CPuTransparentOperateReq : public CEventReq
{
public:
    CPuTransparentOperateReq()
    {
        event = OspExtEventDesc(PU_TRANSPARENT_OPERATE_REQ);
    }

    void Clear()
    {
        
        srvName.clear();
        userUri.clear();
        devChn.Clear();
		msgType = 0;
        data.clear();
        CEventReq::Clear();
    }

public:
    //事务模板会使用
    const string& GetDevUri() const
    {
        return devChn.GetDevUri();
    }

private:
    TPuTransOprType   srvName;                 //业务类型
    string            userUri;                 //操作用户
    TChannel          devChn;                  //设备通道
	int               msgType;                 //子消息类型
    string            data;                    //base64格式的透明数据
public:
    void SetSrvName(const TPuTransOprType& strSrvName)
    {
        srvName = strSrvName;
    }
    TPuTransOprType& GetSrvName()
    {
        return srvName;
    }
    const TPuTransOprType& GetSrvName() const
    {
        return srvName;
    }

    void SetUserUri(const string& strUserUri)
    {
        userUri = strUserUri;
    }
    string& GetUserUri()
    {
        return userUri;
    }
    const string& GetUserUri() const
    {
        return userUri;
    }

    void SetDevChn(const TChannel& cDevChn)
    {
        devChn = cDevChn;
    }
    TChannel& GetDevChn()
    {
        return devChn;
    }
    const TChannel& GetDevChn() const
    {
        return devChn;
    }

	void SetMsgType(int nMsgType)
	{
		msgType = nMsgType;
	}

	int GetMsgType() const
	{
		return msgType;
	}

    void SetData(const string& strData)
    {
        data = strData;
    }
    string& GetData()
    {
        return data;
    }
    const string& GetData() const
    {
        return data;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPuTransparentOperateRsp : public CPuPtzCtrlWithLockRsp
{
public:
    CPuTransparentOperateRsp()
    {
        event = OspExtEventDesc(PU_TRANSPARENT_OPERATE_RSP);
		Clear();
    }

    void Clear()
    {
        data.clear();
		msgType = 0;
        CPuPtzCtrlWithLockRsp::Clear();
    }

private:
	int                msgType;            //子消息类型
    string             data;               //base64格式的透明数据
public:
	void SetMsgType(int nMsgType)
	{
		msgType = nMsgType;
	}

	int GetMsgType() const
	{
		return msgType;
	}

    void SetData(const string& strData)
    {
        data = strData;
    }
    string& GetData()
    {
        return data;
    }
    const string& GetData() const
    {
        return data;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

//该接口支持req-rsp-ntf模型的透明数据(比如支持IPCV7的HTTP隧道)
class CPuTransparentQryReq : public CQueryReqBase
{
public:
    CPuTransparentQryReq()
    {
        event = OspExtEventDesc(PU_TRANSPARENT_QRY_REQ);
    }

    void Clear()
    {
        srvName.clear();
        userUri.clear();
        devChn.Clear();
        msgType = 0;
        data.clear();
        CEventReq::Clear();
    }

private:
    TPuTransOprType   srvName;                         //业务类型
    string            userUri;                         //操作用户
    TChannel          devChn;                          //设备通道
    int               msgType;                         //子消息类型
    string            data;                            //base64格式的透明数据
public:
    //事务模板会使用
    const string& GetDevUri() const
    {
        return devChn.GetDevUri();
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetSrvName(const TPuTransOprType& strSrvName)
    {
        srvName = strSrvName;
    }
    TPuTransOprType& GetSrvName()
    {
        return srvName;
    }
    const TPuTransOprType& GetSrvName() const
    {
        return srvName;
    }

    void SetUserUri(const string& strUserUri)
    {
        userUri = strUserUri;
    }
    string& GetUserUri()
    {
        return userUri;
    }
    const string& GetUserUri() const
    {
        return userUri;
    }

    void SetDevChn(const TChannel& cDevChn)
    {
        devChn = cDevChn;
    }
    TChannel& GetDevChn()
    {
        return devChn;
    }
    const TChannel& GetDevChn() const
    {
        return devChn;
    }

    void SetMsgType(int nMsgType)
    {
        msgType = nMsgType;
    }
    int GetMsgType() const
    {
        return msgType;
    }

    void SetData(const string& strData)
    {
        data = strData;
    }
    string& GetData()
    {
        return data;
    }
    const string& GetData() const
    {
        return data;
    }

};

class CPuTransparentQryRsp : public CQueryRspBase
{
public:
    CPuTransparentQryRsp()
    {
        event = OspExtEventDesc(PU_TRANSPARENT_QRY_RSP);
    }

    void Clear()
    {
        msgType = 0;
        data.clear();
        CEventRsp::Clear();
    }

private:
    int               msgType;                       //子消息类型
    string            data;                          //base64格式的透明数据

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetMsgType(int nMsgType)
    {
        msgType = nMsgType;
    }
    int GetMsgType() const
    {
        return msgType;
    }

    void SetData(const string& strData)
    {
        data = strData;
    }
    string& GetData()
    {
        return data;
    }
    const string& GetData() const
    {
        return data;
    }

};

class CPuTransparentQryNtfReq : public CQueryNtfReqBase
{
public:
    CPuTransparentQryNtfReq()
    {
        event = OspExtEventDesc(PU_TRANSPARENT_QRY_NTF_REQ);
    }

    void Clear()
    {
        msgType = 0;
        data.clear();
        CEventReq::Clear();
    }

private:
    int               msgType;                    //子消息类型
    string            data;                       //base64格式的透明数据

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetMsgType(int nMsgType)
    {
        msgType = nMsgType;
    }
    int GetMsgType() const
    {
        return msgType;
    }

    void SetData(const string& strData)
    {
        data = strData;
    }
    string& GetData()
    {
        return data;
    }
    const string& GetData() const
    {
        return data;
    }

};

class CPuTransparentQryNtfRsp : public CEventRsp
{
public:
    CPuTransparentQryNtfRsp()
    {
        event = OspExtEventDesc(PU_TRANSPARENT_QRY_NTF_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

//前端透明数据通知
class CPuTransparentDataNtfReq : public CEventReq
{
public:
    CPuTransparentDataNtfReq()
    {
        event = OspExtEventDesc(PU_TRANSPARENT_DATA_NTF_REQ);
    }

    void Clear()
    {
        data.Clear();
        CEventReq::Clear();
    }

private:
    TTransparentData  data;                         //透明数据
public:
    void SetData(const TTransparentData& cData)
    {
        data = cData;
    }
    TTransparentData& GetData()
    {
        return data;
    }
    const TTransparentData& GetData() const
    {
        return data;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPuTransparentDataNtfRsp : public CEventRsp
{
public:
    CPuTransparentDataNtfRsp()
    {
        event = OspExtEventDesc(PU_TRANSPARENT_DATA_NTF_RSP);
    }

    void Clear()
    {
        CEventRsp::Clear();
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

//////////////////////////////////////////////////////////////////////////
//保存前端配置

class CPuSaveCfgReq : public CEventReq
{
public:
    CPuSaveCfgReq()
    {
        event = OspExtEventDesc(PU_SAVE_CFG_REQ);
    }

    void Clear()
    {
        userUri.clear();
        devUri.clear();
    }

public:
    const string& GetUserUri() const
    {
        return userUri;
    }

    string& GetUserUri()
    {
        return userUri;
    }

    void SetUserUri(const string& tSipUri)
    {
        userUri = tSipUri;
    }

    const string& GetDevUri() const
    {
        return devUri;
    }

    string& GetDevUri()
    {
        return devUri;
    }

    void SetDevUri(const string& tSipUri)
    {
        devUri = tSipUri;
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

private:
    string            userUri;   //操作用户
    string            devUri;    //操作设备
};

class CPuSaveCfgRsp : public CEventRsp
{
public:
    CPuSaveCfgRsp()
    {
        event = OspExtEventDesc(PU_SAVE_CFG_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


//////////////////////////////////////////////////////////////////////////
//前端系统参数配置

class CPuSysParamSetReq : public CEventReq
{
public:
    CPuSysParamSetReq()
    {
        event = OspExtEventDesc(PU_SYS_PARAM_SET_REQ);
    }

    void Clear()
    {
        userUri.clear();
        devUri.clear();
        param.Clear();
    }

public:
    const string& GetUserUri() const
    {
        return userUri;
    }

    string& GetUserUri()
    {
        return userUri;
    }

    void SetUserUri(const string& tSipUri)
    {
        userUri = tSipUri;
    }

    const string& GetDevUri() const
    {
        return devUri;
    }

    string& GetDevUri()
    {
        return devUri;
    }

    void SetDevUri(const string& tSipUri)
    {
        devUri = tSipUri;
    }

    const TPuSysParam& GetPuSysParam() const
    {
        return param;
    }

    TPuSysParam& GetPuSysParam()
    {
        return param;
    }

    void SetPuSysParam(const TPuSysParam& tPuSysParam)
    {
        param = tPuSysParam;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

private:
    string            userUri;       //操作用户
    string            devUri;        //操作设备
    TPuSysParam       param;         //前端系统参数
};

class CPuSysParamSetRsp : public CEventRsp
{
public:
    CPuSysParamSetRsp()
    {
        event = OspExtEventDesc(PU_SYS_PARAM_SET_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPuSysParamGetReq : public CEventReq
{
public:
    CPuSysParamGetReq()
    {
        event = OspExtEventDesc(PU_SYS_PARAM_GET_REQ);
    }

    void Clear()
    {
        userUri.clear();
        devUri.clear();
    }

public:
    const string& GetUserUri() const
    {
        return userUri;
    }

    string& GetUserUri()
    {
        return userUri;
    }

    void SetUserUri(const string& tSipUri)
    {
        userUri = tSipUri;
    }

    const string& GetDevUri() const
    {
        return devUri;
    }

    string& GetDevUri()
    {
        return devUri;
    }

    void SetDevUri(const string& tSipUri)
    {
        devUri = tSipUri;
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

private:
    string            userUri;       //操作用户
    string            devUri;        //操作设备

};

class CPuSysParamGetRsp : public CEventRsp
{
public:
    CPuSysParamGetRsp()
    {
        event = OspExtEventDesc(PU_SYS_PARAM_GET_RSP);
    }

    void Clear()
    {
        param.Clear();
        CEventRsp::Clear();
    }

public:
    const TPuSysParam& GetPuSysParam() const
    {
        return param;
    }

    TPuSysParam& GetPuSysParam()
    {
        return param;
    }

    void SetPuSysParam(const TPuSysParam& tPuSysParam)
    {
        param = tPuSysParam;
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

private:
    TPuSysParam        param;    //前端系统参数

};


//////////////////////////////////////////////////////////////////////////
//实时浏览

class CPuSendRealMediaReq : public CEventReq
{
public:
    CPuSendRealMediaReq()
    {
        event = OspExtEventDesc(PU_SEND_REAL_MEDIA_REQ);
    }

    void Clear()
    {
        srcChn.Clear();
        dstChn.Clear();
        dstMediaDescList.clear();
        CEventReq::Clear();
    }

public:

    //源通道
    const TChannel& GetSrcChn() const 
    {
        return srcChn;
    }
    TChannel& GetSrcChn() 
    {
        return srcChn;
    }
    void SetSrcChn(const TChannel& tSrcChn) 
    {
        srcChn = tSrcChn;
    }

    //目的通道
    const TChannel& GetDstChn() const 
    {
        return dstChn;
    }
    TChannel& GetDstChn() 
    {
        return dstChn;
    }
    void SetDstChn(const TChannel& tDstChn) 
    {
        dstChn = tDstChn;
    }

    //媒体传输通道接口
    s32 GetDstMediaDescNum() const
    {
        return dstMediaDescList.size();
    }
    const TMediaDesc& GetDstMediaDesc(s32 nIdx) const
    {
        return dstMediaDescList[nIdx];
    }
    void SetDstMediaDescn(s32 nIdx, const TMediaDesc& tValue)
    {
        dstMediaDescList[nIdx] = tValue;
    }

    const TMediaDescList& GetDstMediaDesc() const
    {
        return dstMediaDescList;
    }
    TMediaDescList& GetDstMediaDesc()
    {
        return dstMediaDescList;
    }
    void SetDstMediaDesc(const TMediaDescList& tMediaDesc)
    {
        dstMediaDescList = tMediaDesc;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

private:
    TChannel                srcChn;   //源通道
    TChannel                dstChn;   //目的通道
    TMediaDescList     		dstMediaDescList;  //媒体参数
};

class CPuSendRealMediaRsp : public CEventRsp
{
public:
    CPuSendRealMediaRsp()
    {
        event = OspExtEventDesc(PU_SEND_REAL_MEDIA_RSP);
    }

    void Clear()
    {
        srcChn.Clear();
        dstChn.Clear();
        srcMediaDescList.clear();
        CEventRsp::Clear();
    }

public:
    //源通道
    const TChannel& GetSrcChn() const 
    {
        return srcChn;
    }
    TChannel& GetSrcChn() 
    {
        return srcChn;
    }
    void SetSrcChn(const TChannel& tSrcChn) 
    {
        srcChn = tSrcChn;
    }

    //目的通道
    const TChannel& GetDstChn() const 
    {
        return dstChn;
    }
    TChannel& GetDstChn() 
    {
        return dstChn;
    }
    void SetDstChn(const TChannel& tDstChn) 
    {
        dstChn = tDstChn;
    }

    //媒体传输通道接口
    s32 GetSrcMediaDescNum() const
    {
        return srcMediaDescList.size();
    }
    const TMediaDesc& GetSrcMediaDesc(s32 nIdx) const
    {
        return srcMediaDescList[nIdx];
    }
    void SetSrcMediaDescn(s32 nIdx, const TMediaDesc& tValue)
    {
        srcMediaDescList[nIdx] = tValue;
    }

    const TMediaDescList& GetSrcMediaDesc() const
    {
        return srcMediaDescList;
    }
    TMediaDescList& GetSrcMediaDesc()
    {
        return srcMediaDescList;
    }
    void SetSrcMediaDesc(const TMediaDescList& tMediaDesc)
    {
        srcMediaDescList = tMediaDesc;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

private:
    TChannel                srcChn;
    TChannel                dstChn;
    TMediaDescList     srcMediaDescList;
};

class CPuSendRealMediaACK: public CEventRsp
{
protected:

public:
    CPuSendRealMediaACK(void) 
    {
        event = OspExtEventDesc(PU_SEND_REAL_MEDIA_ACK);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPuStopSendRealMediaReq : public CEventReq
{
public:
    CPuStopSendRealMediaReq()
    {
        event = OspExtEventDesc(PU_STOP_SEND_REAL_MEDIA_REQ);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

//////////////////////////////////////////////////////////////////////////
//前端重启

class CPuRebootReq : public CEventReq
{
public:
    CPuRebootReq()
    {
        event = OspExtEventDesc(PU_REBOOT_REQ);
    }

    void Clear()
    {
        userUri.clear();
        devUri.clear();
    }

public:
    const string& GetUserUri() const
    {
        return userUri;
    }

    string& GetUserUri()
    {
        return userUri;
    }

    void SetUserUri(const string& tSipUri)
    {
        userUri = tSipUri;
    }

    const string& GetDevUri() const
    {
        return devUri;
    }

    string& GetDevUri()
    {
        return devUri;
    }

    void SetDevUri(const string& tSipUri)
    {
        devUri = tSipUri;
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

private:
    string            userUri;       //操作用户
    string            devUri;        //操作设备
};

class CPuRebootRsp : public CEventRsp
{
public:
    CPuRebootRsp()
    {
        event = OspExtEventDesc(PU_REBOOT_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

////视频源参数设置
typedef std::map<string, string> TVidSrcInfo;
typedef std::set<string> TVidSrcItem;
static const string VID_SRC_INFO_SRCNAME = "VidSrcName";			//视频源名称; 

class CPuVideosrcSetReq : public CEventReq
{
public:
	CPuVideosrcSetReq()
	{
		event = OspExtEventDesc(PU_VIDEOSRC_SET_REQ);
		Clear();
	}

	void Clear()
	{
		vidSrcInfo.clear();
	}
//数据操作
public:
	void SetVidSrcInfo(const TVidSrcInfo& tInf)
	{
		vidSrcInfo = tInf;
	}
	TVidSrcInfo& GetVidSrcInfo()
	{
		return vidSrcInfo;
	}
	const TVidSrcInfo& GetVidSrcInfo() const
	{
		return vidSrcInfo;
	}

	void SetVidSrcInfo(const string& strKey, const string& strValue)
	{
		vidSrcInfo[strKey] = strValue;
	}
	void SetVidSrcInfo(const string& strKey, int nValue)
	{
		char buf[256]={0};
		sprintf( buf, "%d", nValue );
		vidSrcInfo[strKey] = buf;
	}
	void SetVidSrcInfo(const string& strKey, double lfValue)
	{
		char buf[256]={0};
		sprintf( buf, "%lf", lfValue );
		vidSrcInfo[strKey] = buf;
	}

	void SetVidSrcInfo(const string& strKey, bool bValue)
	{
		vidSrcInfo[strKey] = BoolStr(bValue);
	}

	bool GetVidSrcInfo(const string& strKey, string& strValue) const
	{
		TExtInfo::const_iterator itr = vidSrcInfo.find(strKey);
		if (itr != vidSrcInfo.end())
		{
			strValue = itr->second;

			return true;
		}

		return false;
	}
	bool GetVidSrcInfo(const string& strKey, int& nValue) const
	{
		TExtInfo::const_iterator itr = vidSrcInfo.find(strKey);
		if (itr != vidSrcInfo.end())
		{
			nValue = atoi(itr->second.c_str());

			return true;
		}

		return false;
	}
	bool GetVidSrcInfo(const string& strKey, double& lfValue) const
	{
		TExtInfo::const_iterator itr = vidSrcInfo.find(strKey);
		if (itr != vidSrcInfo.end())
		{
			lfValue = atof(itr->second.c_str());

			return true;
		}

		return false;
	}

	bool GetVidSrcInfo(const string& strKey, bool& bValue) const
	{
		TExtInfo::const_iterator itr = vidSrcInfo.find(strKey);
		if (itr != vidSrcInfo.end())
		{
			bValue = (itr->second == TRUE_STR);

			return true;
		}

		return false;
	}

private:
	TVidSrcInfo vidSrcInfo;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPuVideosrcSetRsp : public CEventRsp
{
public:
	CPuVideosrcSetRsp()
	{
		event = OspExtEventDesc(PU_VIDEOSRC_SET_RSP);
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPuVidSrcCfgRefreshReq : public CEventReq
{
public:
	CPuVidSrcCfgRefreshReq()
	{
		event = OspExtEventDesc(PU_VIDSRCCFG_REFRESH_REQ);
		Clear();
	}

	void Clear()
	{
		vidSrcItem.clear();
	}

public:
	void SetVidSrcItem(const TVidSrcItem& tInf)
	{
		vidSrcItem = tInf;
	}
	TVidSrcItem& GetVidSrcItem()
	{
		return vidSrcItem;
	}
	const TVidSrcItem& GetVidSrcItem() const
	{
		return vidSrcItem;
	}

	void SetVidSrcItem(const string& strKey)
	{
		vidSrcItem.insert(strKey);
	}

	bool GetVidSrcItem(const string& strKey) const
	{
		TVidSrcItem::const_iterator itr = vidSrcItem.find(strKey);
		if (itr != vidSrcItem.end())
		{
			return true;
		}
		return false;
	}

private:
	TVidSrcItem vidSrcItem;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPuVidSrcCfgRefreshRsp : public CEventRsp
{
public:
	CPuVidSrcCfgRefreshRsp()
	{
		event = OspExtEventDesc(PU_VIDSRCCFG_REFRESH_RSP);
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

////视频源同步参数设置
typedef std::map<string, bool> TSwichSets;
static const string SYNC_PUSRCNAME_TO_PLAT = "SyncPuSrcNameToPlat";			//同步视频源名称到平台; 
static const string SYNC_PUSRCNAME_TO_PU = "SyncPuSrcNameToPu";			    //视频源名称到前端; 
static const string DISPLAY_AREA_INFO = "DisplayAreaInfo";          	    //叠加辖区信息; 

class CCfgSyncSwitchSetReq : public CEventReq
{
public:
	CCfgSyncSwitchSetReq()
	{
		event = OspExtEventDesc(CONFIG_SYNC_SWITCH_SET_REQ);
	}

//数据操作
public:
	void SetSwichSets(const TSwichSets& tInf)
	{
		switchSets = tInf;
	}
	TSwichSets& GetSwichSets()
	{
		return switchSets;
	}
	const TSwichSets& GetSwichSets() const
	{
		return switchSets;
	}

	void SetSwichSets(const string& strKey, bool bValue)
	{
		switchSets[strKey] = bValue;
	}
	bool GetSwichSets(const string& strKey, bool& bValue)
	{
		bValue = false;
		TSwichSets::const_iterator itr = switchSets.find(strKey);
		if (itr != switchSets.end())
		{
			bValue = itr->second;
			return true;
		}

		return false;
	}
private:
	TSwichSets switchSets;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CCfgSyncSwitchSetRsp : public CEventRsp
{
public:
	CCfgSyncSwitchSetRsp()
	{
		event = OspExtEventDesc(CONFIG_SYNC_SWITCH_SET_RSP);
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CCfgSyncSwitchGetReq : public CEventReq
{
public:
	CCfgSyncSwitchGetReq()
	{
		event = OspExtEventDesc(CONFIG_SYNC_SWITCH_GET_REQ);
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CCfgSyncSwitchGetRsp : public CEventRsp
{
public:
	CCfgSyncSwitchGetRsp()
	{
		event = OspExtEventDesc(CONFIG_SYNC_SWITCH_GET_RSP);
	}

	//数据操作
public:
	void SetSwichSets(const TSwichSets& tInf)
	{
		switchSets = tInf;
	}
	TSwichSets& GetSwichSets()
	{
		return switchSets;
	}
	const TSwichSets& GetSwichSets() const
	{
		return switchSets;
	}

	void SetSwichSets(const string& strKey, bool bValue)
	{
		switchSets[strKey] = bValue;
	}
	void GetSwichSets(const string& strKey, bool& bValue) const
	{
		bValue = false;
		TSwichSets::const_iterator itr = switchSets.find(strKey);
		if (itr != switchSets.end())
		{
			bValue = itr->second;
		}
	}
private:
	TSwichSets switchSets;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
/////////////////////////////////////////////////////////////////////
//{{交换相关
//huangzhichun
/////////////////////////////////////////////////////////////////////

//Invite请求
class CInviteReq : public CEventReq
{
protected:
    TChannel    srcChn;     //平台放像时，为RCS uri。
    TChannel    dstChn;
    TSdp        sdp;

public:

    CInviteReq(void) 
    {
        Clear();
        event = OspExtEventDesc(INVITE_REQ);
    }

    void Clear()
    {
        srcChn.Clear();
        dstChn.Clear();
        sdp.Clear();
    }

public:
    //srcChn
    const TChannel& GetSrcChn() const 
    {
        return srcChn;
    }
    TChannel& GetSrcChn() 
    {
        return srcChn;
    }
    void SetSrcChn(const TChannel& tSrcChn) 
    {
        srcChn = tSrcChn;
    }

    //dstChn
    const TChannel& GetDstChn() const 
    {
        return dstChn;
    }
    TChannel& GetDstChn() 
    {
        return dstChn;
    }
    void SetDstChn(const TChannel& tDstChn) 
    {
        dstChn = tDstChn;
    }

    //sdp
    const TSdp& GetSdp() const
    {
        return sdp;
    }
    TSdp& GetSdp()
    {
        return sdp;
    }
    void SetSdp(const TSdp& var)
    {
        sdp = var;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

//Invite响应
class CInviteRsp : public CEventRsp
{
protected:
    TSdp        sdp;

public:
    CInviteRsp(void) 
    {
        event = OspExtEventDesc(INVITE_RSP);
    }
    void Clear()
    {
        sdp.Clear();
    }

public:
    //sdp
    const TSdp& GetSdp() const
    {
        return sdp;
    }
    TSdp& GetSdp()
    {
        return sdp;
    }
    void SetSdp(const TSdp& var)
    {
        sdp = var;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

//Invite响应的确认
class CInviteACK : public CEventRsp
{
protected:
    TSdp        sdp;

public:
    CInviteACK(void) 
    {
        event = OspExtEventDesc(INVITE_ACK);
    }

    void Clear()
    {
        sdp.Clear();
    }

public:
    //sdp
    const TSdp& GetSdp() const
    {
        return sdp;
    }
    TSdp& GetSdp()
    {
        return sdp;
    }
    void SetSdp(const TSdp& var)
    {
        sdp = var;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

//所有INVITE的业务共用这个BYE消息
class CByeReq : public CEventReq
{
public:
    CByeReq()
    {
        Clear();
        event = OspExtEventDesc(BYE_REQ);
    }

    void Clear()
    {
        reason = 0;  //默认是0，对应CMS_SUCCESS，表示正常bye
    }

private:
    int  reason;                    //bye对话的原因，使用cms_errorcode.h中的错误码

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetReason(int nReason)
    {
        reason = nReason;
    }
    int GetReason() const
    {
        return reason;
    }

};

class CByeRsp : public CEventRsp
{
public:
    CByeRsp()
    {
        event = OspExtEventDesc(BYE_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


//第三方呼叫请求
class CStartThirdPartyCallReq : public CEventReq
{
protected:
    TChannel    srcChn;     //平台放像时，为RCS uri。
    TChannel    dstChn;
    TSdp        sdp;

public:

    CStartThirdPartyCallReq(void) 
    {
        Clear();
        event = OspExtEventDesc(START_THIRD_PARTY_CALL_REQ);
    }

    void Clear()
    {
        srcChn.Clear();
        dstChn.Clear();
    }
public:

    //srcChn
    const TChannel& GetSrcChn() const 
    {
        return srcChn;
    }
    TChannel& GetSrcChn() 
    {
        return srcChn;
    }
    void SetSrcChn(const TChannel& tSrcChn) 
    {
        srcChn = tSrcChn;
    }

    //dstChn
    const TChannel& GetDstChn() const 
    {
        return dstChn;
    }
    TChannel& GetDstChn() 
    {
        return dstChn;
    }
    void SetDstChn(const TChannel& tDstChn) 
    {
        dstChn = tDstChn;
    }

    //sdp
    const TSdp& GetSdp() const
    {
        return sdp;
    }
    TSdp& GetSdp()
    {
        return sdp;
    }
    void SetSdp(const TSdp& var)
    {
        sdp = var;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CStartThirdPartyCallRsp : public CEventRsp
{
protected:
    TSdp    sdp;

public:

    CStartThirdPartyCallRsp(void) 
    {
        Clear();
        event = OspExtEventDesc(START_THIRD_PARTY_CALL_RSP);
    }

    void Clear()
    {
        sdp.Clear();
    }
public:

    //sdp
    const TSdp& GetSdp() const
    {
        return sdp;
    }
    TSdp& GetSdp()
    {
        return sdp;
    }
    void SetSdp(const TSdp& var)
    {
        sdp = var;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CStopThirdPartyCallReq : public CEventReq
{
protected:
    TChannel    srcChn;     //对于放像，填的是playbackSessUri
    TChannel    dstChn;

public:

    CStopThirdPartyCallReq(void) 
    {
        Clear();
        event = OspExtEventDesc(STOP_THIRD_PARTY_CALL_REQ);
    }

    void Clear()
    {
        srcChn.Clear();
        dstChn.Clear();
    }
public:

    //srcChn
    const TChannel& GetSrcChn() const 
    {
        return srcChn;
    }
    TChannel& GetSrcChn() 
    {
        return srcChn;
    }
    void SetSrcChn(const TChannel& tSrcChn) 
    {
        srcChn = tSrcChn;
    }

    //dstChn
    const TChannel& GetDstChn() const 
    {
        return dstChn;
    }
    TChannel& GetDstChn() 
    {
        return dstChn;
    }
    void SetDstChn(const TChannel& tDstChn) 
    {
        dstChn = tDstChn;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CStopThirdPartyCallRsp : public CEventRsp
{
public:

    CStopThirdPartyCallRsp(void) 
    {
        Clear();
        event = OspExtEventDesc(STOP_THIRD_PARTY_CALL_RSP);
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

//Invite请求
class CInviteThirdPartyCallReq : public CEventReq
{
protected:
	TChannel    srcChn;     //平台放像时，为RCS uri。
	TChannel    dstChn;
	TSdp        sdp;

public:

	CInviteThirdPartyCallReq(void) 
	{
		Clear();
		event = OspExtEventDesc(INVITE_THIRD_PARTY_CALL_REQ);
	}

	void Clear()
	{
		srcChn.Clear();
		dstChn.Clear();
		sdp.Clear();
	}

public:
	//srcChn
	const TChannel& GetSrcChn() const 
	{
		return srcChn;
	}
	TChannel& GetSrcChn() 
	{
		return srcChn;
	}
	void SetSrcChn(const TChannel& tSrcChn) 
	{
		srcChn = tSrcChn;
	}

	//dstChn
	const TChannel& GetDstChn() const 
	{
		return dstChn;
	}
	TChannel& GetDstChn() 
	{
		return dstChn;
	}
	void SetDstChn(const TChannel& tDstChn) 
	{
		dstChn = tDstChn;
	}

	//sdp
	const TSdp& GetSdp() const
	{
		return sdp;
	}
	TSdp& GetSdp()
	{
		return sdp;
	}
	void SetSdp(const TSdp& var)
	{
		sdp = var;
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

//Invite响应
class CInviteThirdPartyCallRsp : public CEventRsp
{
protected:
	TSdp        sdp;

public:
	CInviteThirdPartyCallRsp(void) 
	{
		event = OspExtEventDesc(INVITE_THIRD_PARTY_CALL_RSP);
	}
	void Clear()
	{
		sdp.Clear();
	}

public:
	//sdp
	const TSdp& GetSdp() const
	{
		return sdp;
	}
	TSdp& GetSdp()
	{
		return sdp;
	}
	void SetSdp(const TSdp& var)
	{
		sdp = var;
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

//Invite响应的确认
class CInviteThirdPartyCallAck : public CEventRsp
{
protected:
	TSdp        sdp;

public:
	CInviteThirdPartyCallAck(void) 
	{
		event = OspExtEventDesc(INVITE_THIRD_PARTY_CALL_ACK);
	}

	void Clear()
	{
		sdp.Clear();
	}

public:
	//sdp
	const TSdp& GetSdp() const
	{
		return sdp;
	}
	TSdp& GetSdp()
	{
		return sdp;
	}
	void SetSdp(const TSdp& var)
	{
		sdp = var;
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};


//Invite请求
class CInviteDecoderCallReq : public CEventReq
{
protected:
	TChannel    srcChn;     //平台放像时，为RCS uri。
	TChannel    dstChn;
	TSdp        sdp;

public:

	CInviteDecoderCallReq(void) 
	{
		Clear();
		event = OspExtEventDesc(INVITE_DECORDER_CALL_REQ);
	}

	void Clear()
	{
		srcChn.Clear();
		dstChn.Clear();
		sdp.Clear();
	}

public:
	//srcChn
	const TChannel& GetSrcChn() const 
	{
		return srcChn;
	}
	TChannel& GetSrcChn() 
	{
		return srcChn;
	}
	void SetSrcChn(const TChannel& tSrcChn) 
	{
		srcChn = tSrcChn;
	}

	//dstChn
	const TChannel& GetDstChn() const 
	{
		return dstChn;
	}
	TChannel& GetDstChn() 
	{
		return dstChn;
	}
	void SetDstChn(const TChannel& tDstChn) 
	{
		dstChn = tDstChn;
	}

	//sdp
	const TSdp& GetSdp() const
	{
		return sdp;
	}
	TSdp& GetSdp()
	{
		return sdp;
	}
	void SetSdp(const TSdp& var)
	{
		sdp = var;
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

//Invite响应
class CInviteDecoderCallRsp : public CEventRsp
{
protected:
	TSdp        sdp;

public:
	CInviteDecoderCallRsp(void) 
	{
		event = OspExtEventDesc(INVITE_DECORDER_CALL_RSP);
	}
	void Clear()
	{
		sdp.Clear();
	}

public:
	//sdp
	const TSdp& GetSdp() const
	{
		return sdp;
	}
	TSdp& GetSdp()
	{
		return sdp;
	}
	void SetSdp(const TSdp& var)
	{
		sdp = var;
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

//Invite响应的确认
class CInviteDecoderCallAck : public CEventRsp
{
protected:
	TSdp        sdp;

public:
	CInviteDecoderCallAck(void) 
	{
		event = OspExtEventDesc(INVITE_DECORDER_CALL_ACK);
	}

	void Clear()
	{
		sdp.Clear();
	}

public:
	//sdp
	const TSdp& GetSdp() const
	{
		return sdp;
	}
	TSdp& GetSdp()
	{
		return sdp;
	}
	void SetSdp(const TSdp& var)
	{
		sdp = var;
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

//更新交换地址请求
class CUpdateSwitchAddressReq : public CEventReq
{
private:
	TMidaType mediaType;
	TTransChannel transChnType;     //RTP,RTCP,DATA
	TNetAddr previousAddr;          //原有的交换地址
	TNetAddr presentAddr;           //现在的需要更新交换地址

public:
	CUpdateSwitchAddressReq()
	{
		Clear();
		event = OspExtEventDesc(UPDATE_SWITCH_ADDRESS_REQ);
	}

	void Clear()
	{
		mediaType.clear();
		previousAddr.Clear();
		presentAddr.Clear();
	}
public:
	const TMidaType& GetMediaType() const
	{
		return mediaType;
	}
	TMidaType& GetMediaType()
	{
		return mediaType;
	}
	void SetMediaType(const TMidaType& strValue)
	{
		mediaType = strValue;
	}

	const TTransChannel& GetTransChannel() const
	{
		return transChnType;
	}
	TTransChannel& GetTransChannel()
	{
		return transChnType;
	}
	void SetTransChannel(const TTransChannel& tTransChannel)
	{
		transChnType = tTransChannel;
	}

	const TNetAddr& GetPreviousAddr() const
	{
		return previousAddr;
	}
	TNetAddr& GetPreviousAddr()
	{
		return previousAddr;
	}
	void SetPreviousAddr(const TNetAddr& tNetAddr)
	{
		previousAddr = tNetAddr;
	}
	void SetPreviousAddr(u32 nIp,int nPort)
	{
		previousAddr.SetNetIpv4(nIp);
		previousAddr.SetNetPort(nPort);
	}

	const TNetAddr& GetPresentAddr() const
	{
		return presentAddr;
	}
	TNetAddr& GetPresentAddr()
	{
		return presentAddr;
	}
	void SetPresentAddr(const TNetAddr& tNetAddr)
	{
		presentAddr = tNetAddr;
	}
	void SetPresentAddr(u32 nIp,int nPort)
	{
		presentAddr.SetNetIpv4(nIp);
		presentAddr.SetNetPort(nPort);
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

//更新交换地址应答
class CUpdateSwitchAddressRsp : public CEventRsp
{
public:
	CUpdateSwitchAddressRsp()
	{
		event = OspExtEventDesc(UPDATE_SWITCH_ADDRESS_RSP);
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
/////////////////////////////////////////////////////////////////////
//}}交换相关
/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
//{{VTDU协议体
//huangzhichun
/////////////////////////////////////////////////////////////////////

//VTDU注册请求
class CVtduRegReq : public CEventReq
{
protected:
    string      devUri;         //vtdu设备的URI
    CIpList     ipAddrList;
    int         mediaStartPort; //码流交换起始端口
	int         mediaM2PPort;   //按源交换端口
    int         maxSwitchNum;   //最大交换路数
    int         maxRcvGroup;    //最大接收组
    int         maxSndMmbPerRcvGrp;  //每组最大发送目标数
    bool        isTravGAP;      //是否穿越网闸
    string      creditDomainUri;   //信任域
	bool        isSupportNA;           //pu是否支持natagent
	TNetSegmentAllInfo netSegInfo;

public:

    CVtduRegReq(void) 
    {
        Clear();
        event = OspExtEventDesc(VTDU_REG_REQ);
    }

    void Clear()
    {
		isSupportNA = false;
        devUri.clear();
        ipAddrList.clear();
		mediaStartPort = 0;
		mediaM2PPort = 0;
        maxSwitchNum = 0;
        maxRcvGroup = 0;
        maxSndMmbPerRcvGrp = 0;
        isTravGAP = false;
        creditDomainUri.clear();
		netSegInfo.Clear();
    }

public:
    //Vtdu ID
	const string& GetDevUri() const
	{
		return devUri;
	}
	string& GetDevUri()
	{
		return devUri;
	}
	void SetDevUri(const string& tSipUri)
	{
		devUri = tSipUri;
	}

    //地址列表
	const CIpList& GetIpAddrList() const
	{
		return ipAddrList;
	}
	CIpList& GetIpAddrList()
	{
		return ipAddrList;
	}
	void SetIpAddrList(const CIpList& cIpAddrList)
	{
		ipAddrList = cIpAddrList;
	}

    //码流交换起始端口
	void SetMediaStartPort(int var)
	{
		mediaStartPort = var;
	}
	int GetMediaStartPort() const
	{
		return mediaStartPort;
	}
    
    //按源转发端口
	void SetMediaM2PPort(int var)
	{
		mediaM2PPort = var;
	}
	int GetMediaM2PPort() const
	{
		return mediaM2PPort;
	}
    
    //最大交换路数
	void SetMaxSwitchNum(int dwValue)
	{
		maxSwitchNum = dwValue;
	}
	int GetMaxSwitchNum() const
	{
		return maxSwitchNum;
	}

    //最大接收组
	void SetMaxRcvGroup(int dwValue)
	{
		maxRcvGroup = dwValue;
	}
	int GetMaxRcvGroup() const
	{
		return maxRcvGroup;
	}

    //每组最大发送目标数
	void SetMaxSndMmbPerRcvGrp(int dwValue)
	{
		maxSndMmbPerRcvGrp = dwValue;
	}
	int GetMaxSndMmbPerRcvGrp() const
	{
		return maxSndMmbPerRcvGrp;
	}

	//是否穿越网闸
	void SetIsTravGAP(bool bValue)
	{
		isTravGAP = bValue;
	}
	bool GetIsTravGAP() const
	{
		return isTravGAP;
	}

	//设置信任域
	const string& GetCreditDomainUri() const
	{
		return creditDomainUri;
	}
	string& GetCreditDomainUri()
	{
		return creditDomainUri;
	}
	void SetCreditDomainUri(const string& tValue)
	{
		creditDomainUri = tValue;
	}

public:
	void SetIsSupportNA(bool bIsSupportNA)
	{
		isSupportNA = bIsSupportNA;
	}
	bool GetIsSupportNA() const
	{
		return isSupportNA;
	}

	const TNetSegmentAllInfo& GetNetSegInfo() const
	{
		return netSegInfo;
	}
	TNetSegmentAllInfo& GetNetSegInfo()
	{
		return netSegInfo;
	}
	void SetNetSegInfo(const TNetSegmentAllInfo& tValue)
	{
		netSegInfo = tValue;
	}
	
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

//VTDU注册应答
class CVtduRegRsp : public CEventRsp
{
protected:
	bool isStreamKeepaliveEnable;
	int  streamKeepaliveInterval;
	bool isDefenseCrossStreamEnable;

public:
	CVtduRegRsp()
	{
		Clear();
		event = OspExtEventDesc(VTDU_REG_RSP);
	}

public:
	void Clear()
	{
		isStreamKeepaliveEnable = false;
		streamKeepaliveInterval = 0;
		isDefenseCrossStreamEnable = false;
	}

public:
	void SetIsStreamKeepaliveEnable(bool bVar)
	{
		isStreamKeepaliveEnable = bVar;
	}
	bool GetIsStreamKeepaliveEnable() const
	{
		return isStreamKeepaliveEnable;
	}

	void SetStreamKeepaliveInterval(int nVar)
	{
		streamKeepaliveInterval = nVar;
	}
	int GetStreamKeepaliveInterval() const
	{
		return streamKeepaliveInterval;
	}

	void SetIsDefenseCrossStreamEnable(bool bVar)
	{
		isDefenseCrossStreamEnable = bVar;
	}
	bool GetIsDefenseCrossStreamEnable() const
	{
		return isDefenseCrossStreamEnable;
	}

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

//添加交换
class CVtduAddSwitchReq : public CEventReq
{
protected:
    TSwitchChannel switchChannel;

public:
    CVtduAddSwitchReq()
    {
        event = OspExtEventDesc(VTDU_ADDSWITCH_REQ);
    }

    const TSwitchChannel& GetSwitchChn() const
    {
        return switchChannel;
    }
    TSwitchChannel& GetSwitchChn()
    {
        return switchChannel;
    }
    void SetSwitchChn(const TSwitchChannel& cValue)
    {
        switchChannel = cValue;
    }
	
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CVtduAddSwitchRsp : public CEventRsp
{
protected:
    TSwitchSsnID switchSsnID;

public:
    CVtduAddSwitchRsp()
    {
        switchSsnID = 0;
		event = OspExtEventDesc(VTDU_ADDSWITCH_RSP);
    }

    const TSwitchSsnID& GetSwitchSsnID() const
    {
        return switchSsnID;
    }
    TSwitchSsnID& GetSwitchSsnID()
    {
        return switchSsnID;
    }
    void SetSwitchSsnID(const TSwitchSsnID& tValue)
    {
        switchSsnID = tValue;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

//删除交换
class CVtduRemoveSwitchReq : public CEventReq
{
protected:
    TSwitchSsnID switchSsnID;

public:
    CVtduRemoveSwitchReq()
    {
        switchSsnID = 0;
		event = OspExtEventDesc(VTDU_REMOVESWITCH_REQ);
    }

    const TSwitchSsnID& GetSwitchSsnID() const
    {
        return switchSsnID;
    }
    TSwitchSsnID& GetSwitchSsnID()
    {
        return switchSsnID;
    }
    void SetSwitchSsnID(const TSwitchSsnID& tValue)
    {
        switchSsnID = tValue;
    }
	
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);	
};
class CVtduRemoveSwitchRsp : public CEventRsp
{
protected:
    TSwitchSsnID switchSsnID;

public:
    CVtduRemoveSwitchRsp()
    {
        switchSsnID = 0;
		event = OspExtEventDesc(VTDU_REMOVESWITCH_RSP);
    }

    const TSwitchSsnID& GetSwitchSsnID() const
    {
        return switchSsnID;
    }
    TSwitchSsnID& GetSwitchSsnID()
    {
        return switchSsnID;
    }
    void SetSwitchSsnID(const TSwitchSsnID& tValue)
    {
        switchSsnID = tValue;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

//探测穿越结果通知 消息体
class CVtduPunchResultNtfReq : public CEventReq
{
protected:
    TSwitchSsnID switchSsnID;
    TMediaChannelKey mediaChnKey;   //udp传输时，Nat打洞用的key
    int punchResult;

public:
    CVtduPunchResultNtfReq()
    {
        switchSsnID = 0;
        punchResult = 0;
		
		event = OspExtEventDesc(VTDU_PUNCH_RESULT_NTF_REQ);
    }

    const TSwitchSsnID& GetSwitchSsnID() const
    {
        return switchSsnID;
    }
    TSwitchSsnID& GetSwitchSsnID()
    {
        return switchSsnID;
    }
    void SetSwitchSsnID(const TSwitchSsnID& tValue)
    {
        switchSsnID = tValue;
    }

    const TMediaChannelKey& GetMediaChannelKey() const
    {
        return mediaChnKey;
    }
    TMediaChannelKey& GetMediaChannelKey()
    {
        return mediaChnKey;
    }
    void SetMediaChannelKey(const TMediaChannelKey& cValue)
    {
        mediaChnKey = cValue;
    }

    int GetPunchResult() const
    {
        return punchResult;
    }

    void SetPunchResult(int nValue)
    {
        punchResult = nValue;
    }
	

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);	
};

class CVtduPunchResultNtfRsp : public CNtfRsp
{
protected:
    TSwitchSsnID switchSsnID;
    TMediaChannelKey mediaChnKey;   //udp传输时，Nat打洞用的key

public:
    CVtduPunchResultNtfRsp()
    {
        switchSsnID = 0;

        event = OspExtEventDesc(VTDU_PUNCH_RESULT_NTF_RSP);
    }

    const TSwitchSsnID& GetSwitchSsnID() const
    {
        return switchSsnID;
    }
    TSwitchSsnID& GetSwitchSsnID()
    {
        return switchSsnID;
    }
    void SetSwitchSsnID(const TSwitchSsnID& tValue)
    {
        switchSsnID = tValue;
    }

    const TMediaChannelKey& GetMediaChannelKey() const
    {
        return mediaChnKey;
    }
    TMediaChannelKey& GetMediaChannelKey()
    {
        return mediaChnKey;
    }
    void SetMediaChannelKey(const TMediaChannelKey& cValue)
    {
        mediaChnKey = cValue;
    }
	
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);	
};

class CVtduAddrAssignReq : public CEventReq
{
protected:
    TMediaTransChannelList  mediaTransChnList;

public:
    CVtduAddrAssignReq()
    {
		event = OspExtEventDesc(VTDU_ADDR_ASSIGN_REQ);
    }

    //媒体传输通道接口

	//s32 GetMediaTransChnNum() const
	//{
	//	return mediaTransChnList.size();
	//}
	//const TMediaTransChannel& GetMediaTransChn(s32 nIdx) const
	//{
	//	return mediaTransChnList[nIdx];
	//}
	//void SetMediaTransChn(s32 nIdx, const TMediaTransChannel& tValue)
	//{
	//	mediaTransChnList[nIdx] = tValue;
	//}

    const TMediaTransChannelList& GetMediaTransChnList() const
    {
        return mediaTransChnList;
    }
    TMediaTransChannelList& GetMediaTransChnList()
    {
        return mediaTransChnList;
    }
    void SetMediaTransChnList(const TMediaTransChannelList& tValue)
    {
        mediaTransChnList = tValue;
    }

	
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);	
};
class CVtduAddrAssignRsp : public CEventRsp
{
protected:
    TMediaTransChannelList  mediaTransChnList;

public:
    CVtduAddrAssignRsp()
    {
		event = OspExtEventDesc(VTDU_ADDR_ASSIGN_RSP);
    }

    //媒体传输通道接口

	s32 GetMediaTransChnNum() const
	{
		return mediaTransChnList.size();
	}

	//const TMediaTransChannel& GetMediaTransChn(s32 nIdx) const
	//{
	//	return mediaTransChnList[nIdx];
	//}
	//void SetMediaTransChn(s32 nIdx, const TMediaTransChannel& tValue)
	//{
	//	mediaTransChnList[nIdx] = tValue;
	//}

    const TMediaTransChannelList& GetMediaTransChnList() const
    {
        return mediaTransChnList;
    }
    TMediaTransChannelList& GetMediaTransChnList()
    {
        return mediaTransChnList;
    }
    void SetMediaTransChnList(const TMediaTransChannelList& tValue)
    {
        mediaTransChnList = tValue;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

/////////////////////////////////////////////////////////////////////
//}}VTDU协议体
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//CMU互联
class CCmuConnectReq : public CEventReq
{
public:
    CCmuConnectReq()
    {
        event = OspExtEventDesc(CMU_CONNECT_REQ);
    }

    void Clear()
    {
        srcUri.clear();
        dstUri.clear();
		localIp.clear();
        CEventReq::Clear();
    }

public:
    const string& GetSrcUri() const
    {
        return srcUri;
    }
    string& GetSrcUri()
    {
        return srcUri;
    }
    void SetSrcUri(const string& tUri)
    {
        srcUri = tUri;
    }

    const string& GetDstUri() const
    {
        return dstUri;
    }
    string& GetDstUri()
    {
        return dstUri;
    }
    void SetDstUri(const string& tUri)
    {
        dstUri = tUri;
    }

	const TIpAddr& GetLocalIp() const
	{
		return localIp;
	}
	TIpAddr& GetLocalIp()
	{
		return localIp;
	}
	void SetLocalIp(const TIpAddr& tIp)
	{
		localIp = tIp;
	}

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
private:
    string           srcUri;      //发起连接的cmu uri
    string           dstUri;      //接受连接的cmu uri
	TIpAddr          localIp;     //当前CMU的IP
};

class CCmuConnectRsp : public CEventRsp
{
public:
    CCmuConnectRsp()
    {
        Clear();
        event = OspExtEventDesc(CMU_CONNECT_RSP);
    }

	void Clear()
	{
		localIp.clear();
        supportRedis = false;
        redisIp.clear();
        redisPort = 0;
        redisPassword.clear();
		CEventRsp::Clear();
	}

public:
	const TIpAddr& GetLocalIp() const
	{
		return localIp;
	}
	TIpAddr& GetLocalIp()
	{
		return localIp;
	}
	void SetLocalIp(const TIpAddr& tIp)
	{
		localIp = tIp;
	}

    bool GetSupportRedis() const
    {
        return supportRedis;
    }
    void SetSupportRedis(bool bSupportRedis)
    {
        supportRedis = bSupportRedis;
    }

    const string &GetRedisIp() const
    {
        return redisIp;
    }
    string &GetRedisIp()
    {
        return redisIp;
    }
    void SetRedisIp(const string &strIp)
    {
        redisIp = strIp;
    }

    u16 GetRedisPort()
    {
        return redisPort;
    }
    void SetRedisPort(u16 wPort)
    {
        redisPort = wPort;
    }

    const string &GetRedisPassword() const
    {
        return redisPassword;
    }
    string &GetRedisPassword()
    {
        return redisPassword;
    }
    void SetRedisPassword(const string &strPassword)
    {
        redisPassword = strPassword;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

private:
	TIpAddr          localIp;     //当前CMU的IP
    bool             supportRedis;//是否支持redis
    string           redisIp;     //supportRedis为true时该字段有效
    u16              redisPort;   //supportRedis为true时该字段有效
    string           redisPassword;//supportRedis为true时该字段有效
};

//CMU断链
class CCmuDisconnectNtfReq : public CEventReq
{
public:
    CCmuDisconnectNtfReq()
    {
        event = OspExtEventDesc(CMU_DISCONNECT_NTF_REQ);
    }

    void Clear()
    {
        cmuUri.clear();
        CEventReq::Clear();
    }

private:
    string           cmuUri;                     //断链的cmu uri
public:
    void SetCmuUri(const string& strCmuUri)
    {
        cmuUri = strCmuUri;
    }
    string& GetCmuUri()
    {
        return cmuUri;
    }
    const string& GetCmuUri() const
    {
        return cmuUri;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CCmuDisconnectNtfRsp : public CEventRsp
{
public:
    CCmuDisconnectNtfRsp()
    {
        event = OspExtEventDesc(CMU_DISCONNECT_NTF_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

//告警输入通知
class CPuAlarmInputNtyReq : public CEventReq
{
public:
    CPuAlarmInputNtyReq()
    {
        event = OspExtEventDesc(PU_ALARM_INPUT_NTF_REQ);
    }

    void Clear()
    {
        alarm.Clear();
    }

private:
    TAlarmInput      alarm;                 //告警输入
public:
    void SetAlarm(const TAlarmInput& cAlarm)
    {
        alarm = cAlarm;
    }
    TAlarmInput& GetAlarm()
    {
        return alarm;
    }
    const TAlarmInput& GetAlarm() const
    {
        return alarm;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPuAlarmInputNtyRsp : public CEventRsp
{
public:
    CPuAlarmInputNtyRsp()
    {
        event = OspExtEventDesc(PU_ALARM_INPUT_NTF_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPuAlarmResetReq : public CEventReq
{
public:
	CPuAlarmResetReq()
	{
		event = OspExtEventDesc(PU_ALARM_RESET_REQ);
		Clear();
	}

private:
	TChannel        devChn;                      //设备编码通道，设备本身时用-1
public:
	void SetDevChn(const TChannel& cDevChn)
	{
		devChn = cDevChn;
	}
	TChannel& GetDevChn()
	{
		return devChn;
	}
	const TChannel& GetDevChn() const
	{
		return devChn;
	}
	//事务模板会使用
	const string& GetDevUri() const
	{
		return devChn.GetDevUri();
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class CPuAlarmResetRsp : public CEventRsp
{
public:
	CPuAlarmResetRsp()
	{
		event = OspExtEventDesc(PU_ALARM_RESET_RSP);
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

//前端读秒
class CPuReadSecReq : public CEventReq
{
public:
    CPuReadSecReq()
    {
        event = OspExtEventDesc(PU_READ_SEC_REQ);
        Clear();
    }

    void Clear()
    {
        devChn.Clear();
        alarmInputId = 0;
        alarmType.clear();
        readTime = 0;
        CEventReq::Clear();
    }

private:
    TChannel        devChn;                      //设备通道
    int             alarmInputId;             	 //告警输入通道
    string          alarmType;             	     //告警类型
    int             readTime;                    //读秒时长,开始告警计时有效
public:
    void SetDevChn(const TChannel& cDevChn)
    {
        devChn = cDevChn;
    }
    TChannel& GetDevChn()
    {
        return devChn;
    }
    const TChannel& GetDevChn() const
    {
        return devChn;
    }

    void SetAlarmInputId(int nAlarmInputId)
    {
        alarmInputId = nAlarmInputId;
    }
    int GetAlarmInputId() const
    {
        return alarmInputId;
    }

    void SetAlarmType(const string& strAlarmType)
    {
        alarmType = strAlarmType;
    }
    string& GetAlarmType()
    {
        return alarmType;
    }
    const string& GetAlarmType() const
    {
        return alarmType;
    }

    void SetReadTime(int nReadTime)
    {
        readTime = nReadTime;
    }
    int GetReadTime() const
    {
        return readTime;
    }

    //事务模板会使用
    const string& GetDevUri() const
    {
        return devChn.GetDevUri();
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPuReadSecRsp : public CEventRsp
{
public:
    CPuReadSecRsp()
    {
        event = OspExtEventDesc(PU_READ_SEC_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


//前端开关量输出
class CPuIoOutReq : public CEventReq
{
public:
    CPuIoOutReq()
    {
        event = OspExtEventDesc(PU_IO_OUT_REQ);
    }

    void Clear()
    {
        devUri.clear();
        ioOutPut.Clear();
        CEventReq::Clear();
    }

private:
    string          devUri;                      //设备URI
    TIoOutPut       ioOutPut;                    //开关量输出
public:
    void SetDevUri(const string& strDevUri)
    {
        devUri = strDevUri;
    }
    string& GetDevUri()
    {
        return devUri;
    }
    const string& GetDevUri() const
    {
        return devUri;
    }

    void SetIoOutPut(const TIoOutPut& cIoOutPut)
    {
        ioOutPut = cIoOutPut;
    }
    TIoOutPut& GetIoOutPut()
    {
        return ioOutPut;
    }
    const TIoOutPut& GetIoOutPut() const
    {
        return ioOutPut;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPuIoOutRsp : public CEventRsp
{
public:
    CPuIoOutRsp()
    {
        event = OspExtEventDesc(PU_IO_OUT_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};




//告警联动设置
class CAlarmLinkCfgSetReq : public CEventReq
{
public:
    CAlarmLinkCfgSetReq()
    {
        event = OspExtEventDesc(ALARMLINK_CFG_SET_REQ);
    }

    void Clear()
    {
        cfg.Clear();
        CEventReq::Clear();
    }

private:
    TAlarmLinkage      cfg;                      //联动配置
public:
    void SetCfg(const TAlarmLinkage& cCfg)
    {
        cfg = cCfg;
    }
    TAlarmLinkage& GetCfg()
    {
        return cfg;
    }
    const TAlarmLinkage& GetCfg() const
    {
        return cfg;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CAlarmLinkCfgSetRsp : public CEventRsp
{
public:
    CAlarmLinkCfgSetRsp()
    {
        event = OspExtEventDesc(ALARMLINK_CFG_SET_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

//告警联动获取
class CAlarmLinkCfgGetReq : public CEventReq
{
public:
    CAlarmLinkCfgGetReq()
    {
        event = OspExtEventDesc(ALARMLINK_CFG_GET_REQ);
    }

    void Clear()
    {
        alarmSrc.Clear();
        CEventReq::Clear();
    }

private:
    TAlarmLinkSrc      alarmSrc;                      //告警源
public:
    void SetAlarmSrc(const TAlarmLinkSrc& cAlarmSrc)
    {
        alarmSrc = cAlarmSrc;
    }
    TAlarmLinkSrc& GetAlarmSrc()
    {
        return alarmSrc;
    }
    const TAlarmLinkSrc& GetAlarmSrc() const
    {
        return alarmSrc;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CAlarmLinkCfgGetRsp : public CEventRsp
{
public:
    CAlarmLinkCfgGetRsp()
    {
        event = OspExtEventDesc(ALARMLINK_CFG_GET_RSP);
    }

    void Clear()
    {
        cfg.Clear();
        CEventRsp::Clear();
    }

private:
    TAlarmLinkage      cfg;                      //联动配置
public:
    void SetCfg(const TAlarmLinkage& cCfg)
    {
        cfg = cCfg;
    }
    TAlarmLinkage& GetCfg()
    {
        return cfg;
    }
    const TAlarmLinkage& GetCfg() const
    {
        return cfg;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CAlarmLinkVersionGetReq : public CEventReq
{
public:
    CAlarmLinkVersionGetReq()
    {
        event = OspExtEventDesc(ALARMLINK_VERSION_GET_REQ);
    }

    ~CAlarmLinkVersionGetReq()
    {

    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CAlarmLinkVersionGetRsp : public CEventRsp
{
public:
    CAlarmLinkVersionGetRsp()
    {
        event = OspExtEventDesc(ALARMLINK_VERSION_GET_RSP);
    }

    ~CAlarmLinkVersionGetRsp()
    {

    }

public:
    const string &GetVersion() const
    {
        return version;
    }

    void SetVersion(const string &strVersion)
    {
        version = strVersion;
    }

private:
    string version;                    //版本信息
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

typedef     vector<string>                CDevUriList;
typedef     vector<TAlarmLinkSrc>         CAlarmLinkSrcList;

class CAlarmLinkQueryReq : public CQueryReqBase
{
public:
    CAlarmLinkQueryReq()
    {
        event = OspExtEventDesc(ALARMLINK_QUERY_REQ);
    }

    void Clear()
    {
        devUriList.clear();
        linkSrcList.clear();
        CEventReq::Clear();
    }

private:
    CDevUriList          devUriList;                          //根据设备查询告警联动信息，查询所有时忽略
    CAlarmLinkSrcList    linkSrcList;                         //根据联动源查询告警联动信息，查询所有时忽略
public:
    void SetDevUriList(const CDevUriList& cDevUriList)
    {
        devUriList = cDevUriList;
    }
    CDevUriList& GetDevUriList()
    {
        return devUriList;
    }
    const CDevUriList& GetDevUriList() const
    {
        return devUriList;
    }

    void SetLinkSrcList(const CAlarmLinkSrcList& cLinkSrcList)
    {
        linkSrcList = cLinkSrcList;
    }
    CAlarmLinkSrcList& GetLinkSrcList()
    {
        return linkSrcList;
    }
    const CAlarmLinkSrcList& GetLinkSrcList() const
    {
        return linkSrcList;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CAlarmLinkQueryRsp : public CQueryRspBase
{
public:
    CAlarmLinkQueryRsp()
    {
        event = OspExtEventDesc(ALARMLINK_QUERY_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CAlarmLinkNtfReq : public CQueryNtfReqBase
{
public:
    CAlarmLinkNtfReq()
    {
        event = OspExtEventDesc(ALARMLINK_NTF_REQ);
    }

    void Clear()
    {
        cfgList.clear();
        CEventReq::Clear();
    }

private:
    CAlarmLinkageList  cfgList;                                     //配置列表
public:
    void SetCfgList(const CAlarmLinkageList& cCfgList)
    {
        cfgList = cCfgList;
    }
    CAlarmLinkageList& GetCfgList()
    {
        return cfgList;
    }
    const CAlarmLinkageList& GetCfgList() const
    {
        return cfgList;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CAlarmLinkNtfRsp : public CEventRsp
{
public:
    CAlarmLinkNtfRsp()
    {
        event = OspExtEventDesc(ALARMLINK_NTF_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CAlarmLinkModNtfReq : public CEventReq
{
public:
    CAlarmLinkModNtfReq()
    {
        event = OspExtEventDesc(ALARMLINK_MOD_NTF_REQ);
    }

    void Clear()
    {
        cfg.Clear();
        CEventReq::Clear();
    }

private:
    TAlarmLinkage  cfg;                                     //配置列表
public:
    void SetCfg(const TAlarmLinkage& cCfg)
    {
        cfg = cCfg;
    }
    TAlarmLinkage& GetCfg()
    {
        return cfg;
    }
    const TAlarmLinkage& GetCfg() const
    {
        return cfg;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CAlarmLinkModNtfRsp : public CEventRsp
{
public:
    CAlarmLinkModNtfRsp()
    {
        event = OspExtEventDesc(ALARMLINK_MOD_NTF_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


//电视墙相关

//电视操作类型
#define   TV_OCCUR_TYPE_ALARM       "alarm"             //告警上墙
#define   TV_OCCUR_TYPE_HAND        "hand"              //手动上墙

//实时码流上墙
class CRealMediaOnTvReq : public CEventReq
{
public:
    CRealMediaOnTvReq()
    {
        event = OspExtEventDesc(REAL_MEDIA_ON_TV_REQ);
    }

    void Clear()
    {
        encChn.Clear();
        tvChn.Clear();
        tvOccurType.clear();
        CEventReq::Clear();
    }

private:
    TChannel         encChn;                              //编码器通道，码流的源
    TTvChannel       tvChn;                               //电视通道
    string           tvOccurType;                         //电视操作类型: TV_OCCUR_TYPE_ALRAM, TV_OCCUR_TYPE_HAND
public:
    void SetEncChn(const TChannel& cEncChn)
    {
        encChn = cEncChn;
    }
    TChannel& GetEncChn()
    {
        return encChn;
    }
    const TChannel& GetEncChn() const
    {
        return encChn;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetTvChn(const TTvChannel& cTvChn)
    {
        tvChn = cTvChn;
    }
    TTvChannel& GetTvChn()
    {
        return tvChn;
    }
    const TTvChannel& GetTvChn() const
    {
        return tvChn;
    }

public:
    void SetTvOccurType(const string& strTvOccurType)
    {
        tvOccurType = strTvOccurType;
    }
    string& GetTvOccurType()
    {
        return tvOccurType;
    }
    const string& GetTvOccurType() const
    {
        return tvOccurType;
    }

};

class CRealMediaOnTvRsp : public CEventRsp
{
public:
    CRealMediaOnTvRsp()
    {
        event = OspExtEventDesc(REAL_MEDIA_ON_TV_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

//实时码流下墙
class CRealMediaOffTvReq : public CEventReq
{
public:
    CRealMediaOffTvReq()
    {
        event = OspExtEventDesc(REAL_MEDIA_OFF_TV_REQ);
    }

    void Clear()
    {
        encChn.Clear();
        tvChn.Clear();
        tvOccurType.clear();
        CEventReq::Clear();
    }

private:
    TChannel         encChn;                              //编码器通道，码流的源
    TTvChannel       tvChn;                               //电视通道
    string           tvOccurType;                         //电视操作类型: TV_OCCUR_TYPE_ALRAM, TV_OCCUR_TYPE_HAND

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetEncChn(const TChannel& cEncChn)
    {
        encChn = cEncChn;
    }
    TChannel& GetEncChn()
    {
        return encChn;
    }
    const TChannel& GetEncChn() const
    {
        return encChn;
    }

    void SetTvChn(const TTvChannel& cTvChn)
    {
        tvChn = cTvChn;
    }
    TTvChannel& GetTvChn()
    {
        return tvChn;
    }
    const TTvChannel& GetTvChn() const
    {
        return tvChn;
    }

public:
    void SetTvOccurType(const string& strTvOccurType)
    {
        tvOccurType = strTvOccurType;
    }
    string& GetTvOccurType()
    {
        return tvOccurType;
    }
    const string& GetTvOccurType() const
    {
        return tvOccurType;
    }

};

class CRealMediaOffTvRsp : public CEventRsp
{
public:
    CRealMediaOffTvRsp()
    {
        event = OspExtEventDesc(REAL_MEDIA_OFF_TV_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CRealMediaSwitchOccurTypeReq: public CEventReq
{
public:
    CRealMediaSwitchOccurTypeReq()
    {
        Clear();
        event = OspExtEventDesc(REAL_MEDIA_SWITCH_OCCUR_TYPE_REQ);
    }

public://xml utils
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

public://formatter
    void Clear()
    {
        CEventReq::Clear();

        tvChn.Clear();
        dstOccurType.clear();
    }

public://property
    void SetDstOccurType( const string &occurType )
    {
        dstOccurType = occurType;
    }

    const string& GetDstOccurType() const
    {
        return dstOccurType;
    }
    string& GetDstOccurType() 
    {
        return dstOccurType;
    }
    
    void SetTvChn(const TTvChannel& cTvChn)
    {
        tvChn = cTvChn;
    }
    TTvChannel& GetTvChn()
    {
        return tvChn;
    }
    const TTvChannel& GetTvChn() const
    {
        return tvChn;
    }

private:
    TTvChannel  tvChn;              //电视通道
    string      dstOccurType;       //需要切换到的电视操作类型: TV_OCCUR_TYPE_ALRAM, TV_OCCUR_TYPE_HAND...
};


class CRealMediaSwitchOccurTypeRsp: public CEventRsp
{
public:
    CRealMediaSwitchOccurTypeRsp()
    {
        event = OspExtEventDesc(REAL_MEDIA_SWITCH_OCCUR_TYPE_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

};


class CRecOnTvReq : public CEventReq
{
public:
    CRecOnTvReq()
    {
        event = OspExtEventDesc(REC_ON_TV_REQ);
    }

    void Clear()
    {
        recUri.Clear();
        timeRange.Clear();
        tvChn.Clear();
        tvOccurType.clear();
        CEventReq::Clear();
    }

private:

    TSdpUri          recUri;                              //放像文件
    TTimeRange       timeRange;                           //放像时间段
    TTvChannel       tvChn;                               //电视通道
    string           tvOccurType;                         //电视操作类型: TV_OCCUR_TYPE_ALRAM, TV_OCCUR_TYPE_HAND

public:

    void SetRecUri(const TSdpUri& tUri)
    {
        recUri = tUri;
    }
    TSdpUri& GetRecUri()
    {
        return recUri;
    }
    const TSdpUri& GetRecUri() const
    {
        return recUri;
    }

    void SetTimeRange(const TTimeRange& cTimeRange)
    {
        timeRange = cTimeRange;
    }
    TTimeRange& GetTimeRange()
    {
        return timeRange;
    }
    const TTimeRange& GetTimeRange() const
    {
        return timeRange;
    }

    void SetTvChn(const TTvChannel& cTvChn)
    {
        tvChn = cTvChn;
    }
    TTvChannel& GetTvChn()
    {
        return tvChn;
    }
    const TTvChannel& GetTvChn() const
    {
        return tvChn;
    }

    void SetTvOccurType(const string& strTvOccurType)
    {
        tvOccurType = strTvOccurType;
    }
    string& GetTvOccurType()
    {
        return tvOccurType;
    }
    const string& GetTvOccurType() const
    {
        return tvOccurType;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CRecOnTvRsp : public CEventRsp
{
public:
    CRecOnTvRsp()
    {
        event = OspExtEventDesc(REC_ON_TV_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CRecOnTvAck : public CEventRsp
{
public:
    CRecOnTvAck()
    {
        event = OspExtEventDesc(REC_ON_TV_ACK);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


//电视窗口轮询

#define         POLL_OPERATE_CONFIG    "config"
#define         POLL_OPERATE_START     "start"
#define         POLL_OPERATE_PAUSE     "pause"
#define         POLL_OPERATE_RESUME    "resume"
#define         POLL_OPERATE_STOP      "stop"

class CTvPollOperateReq : public CEventReq
{
public:
    CTvPollOperateReq()
    {
        event = OspExtEventDesc(TV_POLL_OPERATE_REQ);
    }

    void Clear()
    {
        tvChn.Clear();
        oprType.clear();
        pollList.clear();
        CEventReq::Clear();
    }

private:
    TTvChannel       tvChn;                                             //电视通道
    string           oprType;                                           //轮询操作类型:POLL_OPERATE_START等
    CTvPollList      pollList;                                          //轮询信息列表: 开启操作时需要该项
public:

    void SetOprType(const string& strOprType)
    {
        oprType = strOprType;
    }
    string& GetOprType()
    {
        return oprType;
    }
    const string& GetOprType() const
    {
        return oprType;
    }

    void SetPollList(const CTvPollList& cPollList)
    {
        pollList = cPollList;
    }
    CTvPollList& GetPollList()
    {
        return pollList;
    }
    const CTvPollList& GetPollList() const
    {
        return pollList;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetTvChn(const TTvChannel& cTvChn)
    {
        tvChn = cTvChn;
    }
    TTvChannel& GetTvChn()
    {
        return tvChn;
    }
    const TTvChannel& GetTvChn() const
    {
        return tvChn;
    }

};

class CTvPollOperateRsp : public CEventRsp
{
public:
    CTvPollOperateRsp()
    {
        event = OspExtEventDesc(TV_POLL_OPERATE_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


// class CTvStartPollReq : public CEventReq
// {
// public:
//     CTvStartPollReq()
//     {
//         event = OspExtEventDesc(TV_START_POLL_REQ);
//     }
// 
//     void Clear()
//     {
//         decChn.Clear();
//         pollList.clear();
//         CEventReq::Clear();
//     }
// 
// private:
//     TChannel         decChn;                               //解码器通道，码流目的
//     CTvPollList      pollList;                             //轮询信息列表
// public:
//     void SetDecChn(const TChannel& cDecChn)
//     {
//         decChn = cDecChn;
//     }
//     TChannel& GetDecChn()
//     {
//         return decChn;
//     }
//     const TChannel& GetDecChn() const
//     {
//         return decChn;
//     }
// 
//     void SetPollList(const CTvPollList& cPollList)
//     {
//         pollList = cPollList;
//     }
//     CTvPollList& GetPollList()
//     {
//         return pollList;
//     }
//     const CTvPollList& GetPollList() const
//     {
//         return pollList;
//     }
// 
// public:
//     const std::string ToXml()const;
//     void FromXml(const std::string& xml_string);
//     void ToXml(TiXmlDocument& xmlDoc)const;
//     void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
//     void FromXml(const TiXmlNode& xmlNode);
// };
// 
// class CTvStartPollRsp : public CEventRsp
// {
// public:
//     CTvStartPollRsp()
//     {
//         event = OspExtEventDesc(TV_START_POLL_RSP);
//     }
// public:
//     const std::string ToXml()const;
//     void FromXml(const std::string& xml_string);
//     void ToXml(TiXmlDocument& xmlDoc)const;
//     void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
//     void FromXml(const TiXmlNode& xmlNode);
// };
// 
// class CTvPausePollReq : public CEventReq
// {
// public:
//     CTvPausePollReq()
//     {
//         event = OspExtEventDesc(TV_PAUSE_POLL_REQ);
//     }
// 
//     void Clear()
//     {
//         decChn.Clear();
//         CEventReq::Clear();
//     }
// 
// private:
//     TChannel         decChn;                               //解码器通道，码流目的
// public:
//     void SetDecChn(const TChannel& cDecChn)
//     {
//         decChn = cDecChn;
//     }
//     TChannel& GetDecChn()
//     {
//         return decChn;
//     }
//     const TChannel& GetDecChn() const
//     {
//         return decChn;
//     }
// 
// public:
//     const std::string ToXml()const;
//     void FromXml(const std::string& xml_string);
//     void ToXml(TiXmlDocument& xmlDoc)const;
//     void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
//     void FromXml(const TiXmlNode& xmlNode);
// };
// 
// class CTvPausePollRsp : public CEventRsp
// {
// public:
//     CTvPausePollRsp()
//     {
//         event = OspExtEventDesc(TV_PAUSE_POLL_RSP);
//     }
// public:
//     const std::string ToXml()const;
//     void FromXml(const std::string& xml_string);
//     void ToXml(TiXmlDocument& xmlDoc)const;
//     void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
//     void FromXml(const TiXmlNode& xmlNode);
// };
// 
// class CTvResumePollReq : public CEventReq
// {
// public:
//     CTvResumePollReq()
//     {
//         event = OspExtEventDesc(TV_RESUME_POLL_REQ);
//     }
// 
//     void Clear()
//     {
//         decChn.Clear();
//         CEventReq::Clear();
//     }
// 
// private:
//     TChannel         decChn;                               //解码器通道，码流目的
// public:
//     void SetDecChn(const TChannel& cDecChn)
//     {
//         decChn = cDecChn;
//     }
//     TChannel& GetDecChn()
//     {
//         return decChn;
//     }
//     const TChannel& GetDecChn() const
//     {
//         return decChn;
//     }
// 
// public:
//     const std::string ToXml()const;
//     void FromXml(const std::string& xml_string);
//     void ToXml(TiXmlDocument& xmlDoc)const;
//     void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
//     void FromXml(const TiXmlNode& xmlNode);
// };
// 
// class CTvResumePollRsp : public CEventRsp
// {
// public:
//     CTvResumePollRsp()
//     {
//         event = OspExtEventDesc(TV_RESUME_POLL_REQ);
//     }
// public:
//     const std::string ToXml()const;
//     void FromXml(const std::string& xml_string);
//     void ToXml(TiXmlDocument& xmlDoc)const;
//     void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
//     void FromXml(const TiXmlNode& xmlNode);
// };
// 
// class CTvStopPollReq : public CEventReq
// {
// public:
//     CTvStopPollReq()
//     {
//         event = OspExtEventDesc(TV_STOP_POLL_REQ);
//     }
// 
//     void Clear()
//     {
//         decChn.Clear();
//         CEventReq::Clear();
//     }
// 
// private:
//     TChannel         decChn;                               //解码器通道，码流目的
// public:
//     void SetDecChn(const TChannel& cDecChn)
//     {
//         decChn = cDecChn;
//     }
//     TChannel& GetDecChn()
//     {
//         return decChn;
//     }
//     const TChannel& GetDecChn() const
//     {
//         return decChn;
//     }
// 
// public:
//     const std::string ToXml()const;
//     void FromXml(const std::string& xml_string);
//     void ToXml(TiXmlDocument& xmlDoc)const;
//     void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
//     void FromXml(const TiXmlNode& xmlNode);
// };
// 
// class CTvStopPollRsp : public CEventRsp
// {
// public:
//     CTvStopPollRsp()
//     {
//         event = OspExtEventDesc(TV_STOP_POLL_RSP);
//     }
// public:
//     const std::string ToXml()const;
//     void FromXml(const std::string& xml_string);
//     void ToXml(TiXmlDocument& xmlDoc)const;
//     void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
//     void FromXml(const TiXmlNode& xmlNode);
// };


//电视窗口画面风格设置
class CTvWinStyleSetReq : public CEventReq
{
public:
    CTvWinStyleSetReq()
    {
        Clear();
        event = OspExtEventDesc(TV_WIN_STYLE_SET_REQ);
    }

    void Clear()
    {
        tvWallId.clear();
        tvId = 0;
        style = ETV_WIN_STYLE_1;
        CEventReq::Clear();
    }

private:
    string  tvWallId;                                 //电视墙ID
    int     tvId;                                     //电视机ID
    ETvWinStype style;                                //画面风格
public:
    void SetTvWallId(const string& strTvWallId)
    {
        tvWallId = strTvWallId;
    }
    string& GetTvWallId()
    {
        return tvWallId;
    }
    const string& GetTvWallId() const
    {
        return tvWallId;
    }

    void SetTvId(int nTvId)
    {
        tvId = nTvId;
    }
    int GetTvId() const
    {
        return tvId;
    }

    void SetStyle(ETvWinStype eStyle)
    {
        style = eStyle;
    }
    ETvWinStype GetStyle() const
    {
        return style;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CTvWinStyleSetRsp : public CEventRsp
{
public:
    CTvWinStyleSetRsp()
    {
        event = OspExtEventDesc(TV_WIN_STYLE_SET_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CTvWinStyleGetReq : public CEventReq
{
public:
    CTvWinStyleGetReq()
    {
        event = OspExtEventDesc(TV_WIN_STYLE_GET_REQ);
    }

    void Clear()
    {
        tvWallId.clear();
        tvId = 0;
        CEventReq::Clear();
    }

private:
    string  tvWallId;                                 //电视墙ID
    int     tvId;                                     //电视机ID
public:
    void SetTvWallId(const string& strTvWallId)
    {
        tvWallId = strTvWallId;
    }
    string& GetTvWallId()
    {
        return tvWallId;
    }
    const string& GetTvWallId() const
    {
        return tvWallId;
    }

    void SetTvId(int nTvId)
    {
        tvId = nTvId;
    }
    int GetTvId() const
    {
        return tvId;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CTvWinStyleGetRsp : public CEventRsp
{
public:
    CTvWinStyleGetRsp()
    {
        Clear();
        event = OspExtEventDesc(TV_WIN_STYLE_GET_RSP);
    }

    void Clear()
    {
        tvWallId.clear();
        tvId = 0;
        style = ETV_WIN_STYLE_1;
        CEventRsp::Clear();
    }

private:
    string  tvWallId;                                 //电视墙ID
    int     tvId;                                     //电视机ID
    ETvWinStype style;                                //画面风格
public:
    void SetTvWallId(const string& strTvWallId)
    {
        tvWallId = strTvWallId;
    }
    string& GetTvWallId()
    {
        return tvWallId;
    }
    const string& GetTvWallId() const
    {
        return tvWallId;
    }

    void SetTvId(int nTvId)
    {
        tvId = nTvId;
    }
    int GetTvId() const
    {
        return tvId;
    }

    void SetStyle(ETvWinStype eStyle)
    {
        style = eStyle;
    }
    ETvWinStype GetStyle() const
    {
        return style;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

//电视墙预案配置、查询
class CTvWallSchemeSaveReq : public CEventReq
{
public:
    CTvWallSchemeSaveReq()
    {
        event = OspExtEventDesc(TVWALL_SCHEME_SAVE_REQ);
    }

    void Clear()
    {
        scheme.clear();
        CEventReq::Clear();
    }

private:
    string         scheme;                 //预案详细信息
public:
    void SetScheme(const string& cScheme)
    {
        scheme = cScheme;
    }
    string& GetScheme()
    {
        return scheme;
    }
    const string& GetScheme() const
    {
        return scheme;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CTvWallSchemeSaveRsp : public CEventRsp
{
public:
    CTvWallSchemeSaveRsp()
    {
        event = OspExtEventDesc(TVWALL_SCHEME_SAVE_RSP);
    }

    void Clear()
    {
        //schemeId.clear();
        CEventRsp::Clear();
    }

private:
    //string  schemeId;                        //预案ID
public:
//     void SetSchemeId(const string& strSchemeId)
//     {
//         schemeId = strSchemeId;
//     }
//     string& GetSchemeId()
//     {
//         return schemeId;
//     }
//     const string& GetSchemeId() const
//     {
//         return schemeId;
//     }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CTvWallSchemeDelReq : public CEventReq
{
public:
    CTvWallSchemeDelReq()
    {
        event = OspExtEventDesc(TVWALL_SCHEME_DEL_REQ);
    }

    void Clear()
    {
        tvWallId.clear();
        schemeNameList.clear();
        CEventReq::Clear();
    }

private:
    //CTvWallSchemeIdList  schemeIdList;                        //预案ID列表
    string  tvWallId;                                           //电视墙ID
    CTvWallSchemeNameList schemeNameList;                       //预案列表
public:
//     void SetSchemeIdList(const CTvWallSchemeIdList& cSchemeIdList)
//     {
//         schemeIdList = cSchemeIdList;
//     }
//     CTvWallSchemeIdList& GetSchemeIdList()
//     {
//         return schemeIdList;
//     }
//     const CTvWallSchemeIdList& GetSchemeIdList() const
//     {
//         return schemeIdList;
//     }

    void SetTvWallId(const string& strTvWallId)
    {
        tvWallId = strTvWallId;
    }
    string& GetTvWallId()
    {
        return tvWallId;
    }
    const string& GetTvWallId() const
    {
        return tvWallId;
    }

    void SetSchemeNameList(const CTvWallSchemeNameList& cSchemeNameList)
    {
        schemeNameList = cSchemeNameList;
    }
    CTvWallSchemeNameList& GetSchemeNameList()
    {
        return schemeNameList;
    }
    const CTvWallSchemeNameList& GetSchemeNameList() const
    {
        return schemeNameList;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CTvWallSchemeDelRsp : public CEventRsp
{
public:
    CTvWallSchemeDelRsp()
    {
        event = OspExtEventDesc(TVWALL_SCHEME_DEL_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CTvWallSchemeQueryReq : public CQueryReqBase
{
public:
    CTvWallSchemeQueryReq()
    {
        event = OspExtEventDesc(TVWALL_SCHEME_QUERY_REQ);
    }

    void Clear()
    {
        //userId.clear();
        tvWallId.clear();
        CEventReq::Clear();
    }

private:
    //string  userId;                        //用户ID
    string  tvWallId;                      //电视墙ID
public:
//     void SetUserId(const string& strUserId)
//     {
//         userId = strUserId;
//     }
//     string& GetUserId()
//     {
//         return userId;
//     }
//     const string& GetUserId() const
//     {
//         return userId;
//     }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetTvWallId(const string& strTvWallId)
    {
        tvWallId = strTvWallId;
    }
    string& GetTvWallId()
    {
        return tvWallId;
    }
    const string& GetTvWallId() const
    {
        return tvWallId;
    }

};

class CTvWallSchemeQueryRsp : public CQueryRspBase
{
public:
    CTvWallSchemeQueryRsp()
    {
        event = OspExtEventDesc(TVWALL_SCHEME_QUERY_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CTvWallSchemeNtfReq : public CQueryNtfReqBase
{
public:
    CTvWallSchemeNtfReq()
    {
        event = OspExtEventDesc(TVWALL_SCHEME_NTF_REQ);
    }

    void Clear()
    {
        schemeList.clear();
        CEventReq::Clear();
    }

private:
    CTTvWallSchemeList  schemeList;                        //预案列表
public:
    void SetSchemeList(const CTTvWallSchemeList& cSchemeList)
    {
        schemeList = cSchemeList;
    }
    CTTvWallSchemeList& GetSchemeList()
    {
        return schemeList;
    }
    const CTTvWallSchemeList& GetSchemeList() const
    {
        return schemeList;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CTvWallSchemeNtfRsp : public CEventRsp
{
public:
    CTvWallSchemeNtfRsp()
    {
        event = OspExtEventDesc(TVWALL_SCHEME_NTF_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

//电视墙预案加载
class CTvWallSchemeLoadReq : public CEventReq
{
public:
    CTvWallSchemeLoadReq()
    {
        event = OspExtEventDesc(TVWALL_SCHEME_LOAD_REQ);
    }

    void Clear()
    {
        //schemeId.clear();
        tvWallId.clear();
        schemeName.clear();
        CEventReq::Clear();
    }

private:
    //string  schemeId;                        //预案ID
    string  tvWallId;                          //电视墙ID
    string  schemeName;                        //预案名称
public:
//     void SetSchemeId(const string& strSchemeId)
//     {
//         schemeId = strSchemeId;
//     }
//     string& GetSchemeId()
//     {
//         return schemeId;
//     }
//     const string& GetSchemeId() const
//     {
//         return schemeId;
//     }

    void SetTvWallId(const string& strTvWallId)
    {
        tvWallId = strTvWallId;
    }
    string& GetTvWallId()
    {
        return tvWallId;
    }
    const string& GetTvWallId() const
    {
        return tvWallId;
    }

    void SetSchemeName(const string& strSchemeName)
    {
        schemeName = strSchemeName;
    }
    string& GetSchemeName()
    {
        return schemeName;
    }
    const string& GetSchemeName() const
    {
        return schemeName;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CTvWallSchemeLoadRsp : public CEventRsp
{
public:
    CTvWallSchemeLoadRsp()
    {
        event = OspExtEventDesc(TVWALL_SCHEME_LOAD_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CTvWallSchemeGroupSetReq : public CEventReq
{
public:
    CTvWallSchemeGroupSetReq()
    {
        event = OspExtEventDesc(TVWALL_SCHEME_GROUP_SET_REQ);
    }

    void Clear()
    {
        schemeGroup.Clear();
        CEventReq::Clear();
    }

private:
    TTvWallSchemeGroup         schemeGroup;                 //预案组
public:
    void SetSchemeGroup(const TTvWallSchemeGroup& cSchemeGroup)
    {
        schemeGroup = cSchemeGroup;
    }
    TTvWallSchemeGroup& GetSchemeGroup()
    {
        return schemeGroup;
    }
    const TTvWallSchemeGroup& GetSchemeGroup() const
    {
        return schemeGroup;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CTvWallSchemeGroupSetRsp : public CEventRsp
{
public:
    CTvWallSchemeGroupSetRsp()
    {
        event = OspExtEventDesc(TVWALL_SCHEME_GROUP_SET_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CTvWallSchemeGroupGetReq : public CEventReq
{
public:
    CTvWallSchemeGroupGetReq()
    {
        event = OspExtEventDesc(TVWALL_SCHEME_GROUP_GET_REQ);
    }

    void Clear()
    {
        tvWallId.clear();
        CEventReq::Clear();
    }

private:
    string      tvWallId;                               //电视墙ID
public:
    void SetTvWallId(const string& strTvWallId)
    {
        tvWallId = strTvWallId;
    }
    string& GetTvWallId()
    {
        return tvWallId;
    }
    const string& GetTvWallId() const
    {
        return tvWallId;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CTvWallSchemeGroupGetRsp : public CEventRsp
{
public:
    CTvWallSchemeGroupGetRsp()
    {
        event = OspExtEventDesc(TVWALL_SCHEME_GROUP_GET_RSP);
    }

    void Clear()
    {
        schemeGroup.Clear();
        CEventRsp::Clear();
    }

private:
    TTvWallSchemeGroup         schemeGroup;                 //预案组
public:
    void SetSchemeGroup(const TTvWallSchemeGroup& cSchemeGroup)
    {
        schemeGroup = cSchemeGroup;
    }
    TTvWallSchemeGroup& GetSchemeGroup()
    {
        return schemeGroup;
    }
    const TTvWallSchemeGroup& GetSchemeGroup() const
    {
        return schemeGroup;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


//电视墙预案轮询

class CTvWallSchemePollOperateReq : public CEventReq
{
public:
    CTvWallSchemePollOperateReq()
    {
        event = OspExtEventDesc(TVWALL_SCHEME_POLL_OPERATE_REQ);
    }

    void Clear()
    {       
        tvWallId.clear();
        oprType.clear();
        CEventReq::Clear();
    }

private:
    string      tvWallId;                                            //电视墙ID，一个电视墙只有一个预案组，所以只要给出电视墙ID即可轮询
    string      oprType;                                             //轮询操作类型:POLL_OPERATE_START等
public:
    void SetTvWallId(const string& strTvWallId)
    {
        tvWallId = strTvWallId;
    }
    string& GetTvWallId()
    {
        return tvWallId;
    }
    const string& GetTvWallId() const
    {
        return tvWallId;
    }

    void SetOprType(const string& strOprType)
    {
        oprType = strOprType;
    }
    string& GetOprType()
    {
        return oprType;
    }
    const string& GetOprType() const
    {
        return oprType;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CTvWallSchemePollOperateRsp : public CEventRsp
{
public:
    CTvWallSchemePollOperateRsp()
    {
        event = OspExtEventDesc(TVWALL_SCHEME_POLL_OPERATE_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};



// class CTvWallSchemeStartPollReq : public CEventReq
// {
// public:
//     CTvWallSchemeStartPollReq()
//     {
//         event = OspExtEventDesc(TVWALL_SCHEME_START_POLL_REQ);
//     }
// 
//     void Clear()
//     {
//         tvWallId.clear();
//         CEventReq::Clear();
//     }
// 
// private:
//     string      tvWallId;                               //电视墙ID，一个电视墙只有一个预案组，所以只要给出电视墙ID即可轮询
// public:
//     void SetTvWallId(const string& strTvWallId)
//     {
//         tvWallId = strTvWallId;
//     }
//     string& GetTvWallId()
//     {
//         return tvWallId;
//     }
//     const string& GetTvWallId() const
//     {
//         return tvWallId;
//     }
// 
// public:
//     const std::string ToXml()const;
//     void FromXml(const std::string& xml_string);
//     void ToXml(TiXmlDocument& xmlDoc)const;
//     void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
//     void FromXml(const TiXmlNode& xmlNode);
// };
// 
// class CTvWallSchemeStartPollRsp : public CEventRsp
// {
// public:
//     CTvWallSchemeStartPollRsp()
//     {
//         event = OspExtEventDesc(TVWALL_SCHEME_START_POLL_RSP);
//     }
// public:
//     const std::string ToXml()const;
//     void FromXml(const std::string& xml_string);
//     void ToXml(TiXmlDocument& xmlDoc)const;
//     void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
//     void FromXml(const TiXmlNode& xmlNode);
// };
// 
// class CTvWallSchemePausePollReq : public CEventReq
// {
// public:
//     CTvWallSchemePausePollReq()
//     {
//         event = OspExtEventDesc(TVWALL_SCHEME_PAUSE_POLL_REQ);
//     }
// 
//     void Clear()
//     {
//         tvWallId.clear();
//         CEventReq::Clear();
//     }
// 
// private:
//     string           tvWallId;                        //电视墙ID
// public:
//     void SetTvWallId(const string& strTvWallId)
//     {
//         tvWallId = strTvWallId;
//     }
//     string& GetTvWallId()
//     {
//         return tvWallId;
//     }
//     const string& GetTvWallId() const
//     {
//         return tvWallId;
//     }
// 
// public:
//     const std::string ToXml()const;
//     void FromXml(const std::string& xml_string);
//     void ToXml(TiXmlDocument& xmlDoc)const;
//     void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
//     void FromXml(const TiXmlNode& xmlNode);
// };
// 
// class CTvWallSchemePausePollRsp : public CEventRsp
// {
// public:
//     CTvWallSchemePausePollRsp()
//     {
//         event = OspExtEventDesc(TVWALL_SCHEME_PAUSE_POLL_RSP);
//     }
// public:
//     const std::string ToXml()const;
//     void FromXml(const std::string& xml_string);
//     void ToXml(TiXmlDocument& xmlDoc)const;
//     void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
//     void FromXml(const TiXmlNode& xmlNode);
// };
// 
// 
// class CTvWallSchemeResumePollReq : public CEventReq
// {
// public:
//     CTvWallSchemeResumePollReq()
//     {
//         event = OspExtEventDesc(TVWALL_SCHEME_RESUME_POLL_REQ);
//     }
// 
//     void Clear()
//     {
//         tvWallId.clear();
//         CEventReq::Clear();
//     }
// 
// private:
//     string           tvWallId;                        //电视墙ID
// public:
//     void SetTvWallId(const string& strTvWallId)
//     {
//         tvWallId = strTvWallId;
//     }
//     string& GetTvWallId()
//     {
//         return tvWallId;
//     }
//     const string& GetTvWallId() const
//     {
//         return tvWallId;
//     }
// 
// public:
//     const std::string ToXml()const;
//     void FromXml(const std::string& xml_string);
//     void ToXml(TiXmlDocument& xmlDoc)const;
//     void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
//     void FromXml(const TiXmlNode& xmlNode);
// };
// 
// class CTvWallSchemeResumePollRsp : public CEventRsp
// {
// public:
//     CTvWallSchemeResumePollRsp()
//     {
//         event = OspExtEventDesc(TVWALL_SCHEME_RESUME_POLL_RSP);
//     }
// public:
//     const std::string ToXml()const;
//     void FromXml(const std::string& xml_string);
//     void ToXml(TiXmlDocument& xmlDoc)const;
//     void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
//     void FromXml(const TiXmlNode& xmlNode);
// };
// 
// 
// class CTvWallSchemeStopPollReq : public CEventReq
// {
// public:
//     CTvWallSchemeStopPollReq()
//     {
//         event = OspExtEventDesc(TVWALL_SCHEME_STOP_POLL_REQ);
//     }
// 
//     void Clear()
//     {
//         tvWallId.clear();
//         CEventReq::Clear();
//     }
// 
// private:
//     string           tvWallId;                        //电视墙ID
// public:
//     void SetTvWallId(const string& strTvWallId)
//     {
//         tvWallId = strTvWallId;
//     }
//     string& GetTvWallId()
//     {
//         return tvWallId;
//     }
//     const string& GetTvWallId() const
//     {
//         return tvWallId;
//     }
// 
// public:
//     const std::string ToXml()const;
//     void FromXml(const std::string& xml_string);
//     void ToXml(TiXmlDocument& xmlDoc)const;
//     void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
//     void FromXml(const TiXmlNode& xmlNode);
// };
// 
// class CTvWallSchemeStopPollRsp : public CEventRsp
// {
// public:
//     CTvWallSchemeStopPollRsp()
//     {
//         event = OspExtEventDesc(TVWALL_SCHEME_STOP_POLL_RSP);
//     }
// public:
//     const std::string ToXml()const;
//     void FromXml(const std::string& xml_string);
//     void ToXml(TiXmlDocument& xmlDoc)const;
//     void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
//     void FromXml(const TiXmlNode& xmlNode);
// };


//平台录像上墙
class CPlatRecOnTvReq : public CEventReq
{
public:
    CPlatRecOnTvReq()
    {
        event = OspExtEventDesc(PLAT_REC_ON_TV_REQ);
    }

    void Clear()
    {
        recDurationToken.clear();
        timeRange.Clear();
        encChn.Clear();
        tvChn.Clear();
        CEventReq::Clear();
    }

private:
    TRecToken           recDurationToken;                      //录像Token
    TTimeRange          timeRange;                             //录像时间段
    TChannel            encChn;                                //编码器通道
    TTvChannel          tvChn;                                 //电视通道
public:
    void SetRecDurationToken(const TRecToken& strRecDurationToken)
    {
        recDurationToken = strRecDurationToken;
    }
    TRecToken& GetRecDurationToken()
    {
        return recDurationToken;
    }
    const TRecToken& GetRecDurationToken() const
    {
        return recDurationToken;
    }

    void SetTimeRange(const TTimeRange& cTimeRange)
    {
        timeRange = cTimeRange;
    }
    TTimeRange& GetTimeRange()
    {
        return timeRange;
    }
    const TTimeRange& GetTimeRange() const
    {
        return timeRange;
    }

    void SetEncChn(const TChannel& cEncChn)
    {
        encChn = cEncChn;
    }
    TChannel& GetEncChn()
    {
        return encChn;
    }
    const TChannel& GetEncChn() const
    {
        return encChn;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetTvChn(const TTvChannel& cTvChn)
    {
        tvChn = cTvChn;
    }
    TTvChannel& GetTvChn()
    {
        return tvChn;
    }
    const TTvChannel& GetTvChn() const
    {
        return tvChn;
    }

};

class CPlatRecOnTvRsp : public CEventRsp
{
public:
    CPlatRecOnTvRsp()
    {
        event = OspExtEventDesc(PLAT_REC_ON_TV_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

//平台录像下墙
class CPlatRecOffTvReq : public CEventReq
{
public:
    CPlatRecOffTvReq()
    {
        event = OspExtEventDesc(PLAT_REC_OFF_TV_REQ);
    }

    void Clear()
    {
        tvChn.Clear();
        CEventReq::Clear();
    }

private:
    TTvChannel          tvChn;                                 //电视通道
public:

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetTvChn(const TTvChannel& cTvChn)
    {
        tvChn = cTvChn;
    }
    TTvChannel& GetTvChn()
    {
        return tvChn;
    }
    const TTvChannel& GetTvChn() const
    {
        return tvChn;
    }

};

class CPlatRecOffTvRsp : public CEventRsp
{
public:
    CPlatRecOffTvRsp()
    {
        event = OspExtEventDesc(PLAT_REC_OFF_TV_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

//前端录像上墙
class CPuRecOnTvReq : public CEventReq
{
public:
	CPuRecOnTvReq()
	{
		event = OspExtEventDesc(PU_REC_ON_TV_REQ);
	}

	void Clear()
	{
		recDurationToken.clear();
		timeRange.Clear();
		encChn.Clear();
		tvChn.Clear();
		CEventReq::Clear();
	}

private:
	TRecToken           recDurationToken;                      //录像Token
	TTimeRange          timeRange;                             //录像时间段
	TChannel            encChn;                                //编码器通道
	TTvChannel          tvChn;                                 //电视通道
public:
	void SetRecDurationToken(const TRecToken& strRecDurationToken)
	{
		recDurationToken = strRecDurationToken;
	}
	TRecToken& GetRecDurationToken()
	{
		return recDurationToken;
	}
	const TRecToken& GetRecDurationToken() const
	{
		return recDurationToken;
	}

	void SetTimeRange(const TTimeRange& cTimeRange)
	{
		timeRange = cTimeRange;
	}
	TTimeRange& GetTimeRange()
	{
		return timeRange;
	}
	const TTimeRange& GetTimeRange() const
	{
		return timeRange;
	}

	void SetEncChn(const TChannel& cEncChn)
	{
		encChn = cEncChn;
	}
	TChannel& GetEncChn()
	{
		return encChn;
	}
	const TChannel& GetEncChn() const
	{
		return encChn;
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
public:
	void SetTvChn(const TTvChannel& cTvChn)
	{
		tvChn = cTvChn;
	}
	TTvChannel& GetTvChn()
	{
		return tvChn;
	}
	const TTvChannel& GetTvChn() const
	{
		return tvChn;
	}

};

class CPuRecOnTvRsp : public CEventRsp
{
public:
	CPuRecOnTvRsp()
	{
		event = OspExtEventDesc(PU_REC_ON_TV_RSP);
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

//前端录像下墙
class CPuRecOffTvReq : public CEventReq
{
public:
	CPuRecOffTvReq()
	{
		event = OspExtEventDesc(PU_REC_OFF_TV_REQ);
	}

	void Clear()
	{
		tvChn.Clear();
		CEventReq::Clear();
	}

private:
	TTvChannel          tvChn;                                 //电视通道
public:

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
public:
	void SetTvChn(const TTvChannel& cTvChn)
	{
		tvChn = cTvChn;
	}
	TTvChannel& GetTvChn()
	{
		return tvChn;
	}
	const TTvChannel& GetTvChn() const
	{
		return tvChn;
	}

};

class CPuRecOffTvRsp : public CEventRsp
{
public:
	CPuRecOffTvRsp()
	{
		event = OspExtEventDesc(PU_REC_OFF_TV_RSP);
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

//录像(前端、平台)上墙
class CRecordOnTvReq : public CEventReq
{
public:
	CRecordOnTvReq()
	{
		event = OspExtEventDesc(RECORD_ON_TV_REQ);
	}

	void Clear()
	{
		encChn.Clear();
		tvChn.Clear();
		CEventReq::Clear();
	}

private:
	TSdp                sdp;                                   //录像上墙时源媒体信息
	TChannel            encChn;                                //编码器通道
	TTvChannel          tvChn;                                 //电视通道
public:
	const TSdp& GetSdp() const
	{
		return sdp;
	}
	TSdp& GetSdp()
	{
		return sdp;
	}
	void SetSdp(const TSdp& var)
	{
		sdp = var;
	}

	void SetEncChn(const TChannel& cEncChn)
	{
		encChn = cEncChn;
	}
	TChannel& GetEncChn()
	{
		return encChn;
	}
	const TChannel& GetEncChn() const
	{
		return encChn;
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
public:
	void SetTvChn(const TTvChannel& cTvChn)
	{
		tvChn = cTvChn;
	}
	TTvChannel& GetTvChn()
	{
		return tvChn;
	}
	const TTvChannel& GetTvChn() const
	{
		return tvChn;
	}

};

class CRecordOnTvRsp : public CEventRsp
{
public:
	CRecordOnTvRsp()
	{
		event = OspExtEventDesc(RECORD_ON_TV_RSP);
	}
private:
	TSdp sdp;         //录像上墙时解码器媒体信息
public:
	//sdp
	const TSdp& GetSdp() const
	{
		return sdp;
	}
	TSdp& GetSdp()
	{
		return sdp;
	}
	void SetSdp(const TSdp& var)
	{
		sdp = var;
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

//录像(前端、平台)下墙
class CRecordOffTvReq : public CEventReq
{
public:
	CRecordOffTvReq()
	{
		event = OspExtEventDesc(RECORD_OFF_TV_REQ);
	}

	void Clear()
	{
		tvChn.Clear();
		CEventReq::Clear();
	}

private:
	TTvChannel          tvChn;                                 //电视通道
public:

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
public:
	void SetTvChn(const TTvChannel& cTvChn)
	{
		tvChn = cTvChn;
	}
	TTvChannel& GetTvChn()
	{
		return tvChn;
	}
	const TTvChannel& GetTvChn() const
	{
		return tvChn;
	}

};

class CRecordOffTvRsp : public CEventRsp
{
public:
	CRecordOffTvRsp()
	{
		event = OspExtEventDesc(RECORD_OFF_TV_RSP);
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

//电视墙状态订阅类型
#define  TVWALL_ALL_STATUS    "TVWALL_ALL"

class CTvWallStatusSsReq : public CEventReq
{
public:
    CTvWallStatusSsReq()
    {
        event = OspExtEventDesc(TVWALL_STATUS_SS_REQ);
    }

    void Clear()
    {
        userUri.clear();
        devUri.clear();
        ssTypes.clear();
        CEventReq::Clear();
    }

public:
    const string& GetUserUri() const
    {
        return userUri;
    }

    string& GetUserUri()
    {
        return userUri;
    }

    void SetUserUri(const string& tSipUri)
    {
        userUri = tSipUri;
    }

    const string& GetDevUri() const
    {
        return devUri;
    }

    string& GetDevUri()
    {
        return devUri;
    }

    void SetDevUri(const string& tSipUri)
    {
        devUri = tSipUri;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

private:
    string          userUri;                  //订阅者
    string          devUri;                   //被订阅者
    TSsTypes        ssTypes;                  //订阅的状态种类
public:
    void SetSsTypes(const TSsTypes& cSsTypes)
    {
        ssTypes = cSsTypes;
    }
    TSsTypes& GetSsTypes()
    {
        return ssTypes;
    }
    const TSsTypes& GetSsTypes() const
    {
        return ssTypes;
    }
};

class CTvWallStatusSsRsp : public CEventRsp
{
public:
    CTvWallStatusSsRsp()
    {
        event = OspExtEventDesc(TVWALL_STATUS_SS_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CTvWallStatusUnSsReq : public CEventReq
{
public:
    CTvWallStatusUnSsReq()
    {
        event = OspExtEventDesc(TVWALL_STATUS_UNSS_REQ);
    }

    void Clear()
    {
        userUri.clear();
        devUri.clear();
    }

public:
    const string& GetUserUri() const
    {
        return userUri;
    }

    string& GetUserUri()
    {
        return userUri;
    }

    void SetUserUri(const string& tSipUri)
    {
        userUri = tSipUri;
    }

    const string& GetDevUri() const
    {
        return devUri;
    }

    string& GetDevUri()
    {
        return devUri;
    }

    void SetDevUri(const string& tSipUri)
    {
        devUri = tSipUri;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

private:
    string          userUri;     //订阅者
    string          devUri;      //被订阅者
};

class CTvWallStatusUnSsRsp : public CEventRsp
{
public:
    CTvWallStatusUnSsRsp()
    {
        event = OspExtEventDesc(TVWALL_STATUS_UNSS_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CTvWallStatusNtyReq : public CEventReq
{
public:
    CTvWallStatusNtyReq()
    {
        event = OspExtEventDesc(TVWALL_STATUS_NTF_REQ);
    };

    void Clear()
    {
        devURI.clear();
        state.clear();
        CEventReq::Clear();
    }

private:
    string              devURI;                  //tvwall: uuid@domain
    string        state;                   //tvwall状态
public:
    void SetDevURI(const string& strDevURI)
    {
        devURI = strDevURI;
    }
    string& GetDevURI()
    {
        return devURI;
    }
    const string& GetDevURI() const
    {
        return devURI;
    }

    void SetState(const string& cState)
    {
        state = cState;
    }
    string& GetState()
    {
        return state;
    }
    const string& GetState() const
    {
        return state;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

typedef CNtfRsp CTvWallStatusNtyRsp;

//电视墙通道锁定、备份
class CTvChannelLockReq : public CEventReq
{
public:
    CTvChannelLockReq()
    {
        event = OspExtEventDesc(TV_CHANNEL_LOCK_REQ);
    }

    void Clear()
    {
        tvChn.Clear();
        lockTime.Clear();
        password.clear();
    }

private:
    TTvChannel    tvChn;                       //被锁定的电视通道
    TTimeRange    lockTime;                    //被锁定时间
    string        password;                    //锁定密码
public:
    void SetTvChn(const TTvChannel& cTvChn)
    {
        tvChn = cTvChn;
    }
    TTvChannel& GetTvChn()
    {
        return tvChn;
    }
    const TTvChannel& GetTvChn() const
    {
        return tvChn;
    }

    void SetLockTime(const TTimeRange& cLockTime)
    {
        lockTime = cLockTime;
    }
    TTimeRange& GetLockTime()
    {
        return lockTime;
    }
    const TTimeRange& GetLockTime() const
    {
        return lockTime;
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

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CTvChannelLockRsp : public CEventRsp
{
public:
    CTvChannelLockRsp()
    {
        event = OspExtEventDesc(TV_CHANNEL_LOCK_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CTvChannelUnlockReq : public CEventReq
{
public:
    CTvChannelUnlockReq()
    {
        event = OspExtEventDesc(TV_CHANNEL_UNLOCK_REQ);
    }

    void Clear()
    {
        tvChn.Clear();
        password.clear();
    }

private:
    TTvChannel    tvChn;                       //被锁定的电视通道
    string        password;                    //锁定密码
public:
    void SetTvChn(const TTvChannel& cTvChn)
    {
        tvChn = cTvChn;
    }
    TTvChannel& GetTvChn()
    {
        return tvChn;
    }
    const TTvChannel& GetTvChn() const
    {
        return tvChn;
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

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CTvChannelUnlockRsp : public CEventRsp
{
public:
    CTvChannelUnlockRsp()
    {
        event = OspExtEventDesc(TV_CHANNEL_UNLOCK_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CTvChannelWatchPointSaveReq : public CEventReq
{
public:
    CTvChannelWatchPointSaveReq()
    {
        event = OspExtEventDesc(TV_CHANNEL_WATCH_POINT_SAVE_REQ);
    }

    void Clear()
    {
        tvChn.Clear();
        saveTime.Clear();
    }

private:
    TTvChannel    tvChn;                       //备份电视通道
    TTimeRange    saveTime;                    //备份时间
public:
    void SetTvChn(const TTvChannel& cTvChn)
    {
        tvChn = cTvChn;
    }
    TTvChannel& GetTvChn()
    {
        return tvChn;
    }
    const TTvChannel& GetTvChn() const
    {
        return tvChn;
    }

    void SetSaveTime(const TTimeRange& cSaveTime)
    {
        saveTime = cSaveTime;
    }
    TTimeRange& GetSaveTime()
    {
        return saveTime;
    }
    const TTimeRange& GetSaveTime() const
    {
        return saveTime;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CTvChannelWatchPointSaveRsp : public CEventRsp
{
public:
    CTvChannelWatchPointSaveRsp()
    {
        event = OspExtEventDesc(TV_CHANNEL_WATCH_POINT_SAVE_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CTvChannelWatchPointLoadReq : public CEventReq
{
public:
    CTvChannelWatchPointLoadReq()
    {
        event = OspExtEventDesc(TV_CHANNEL_WATCH_POINT_LOAD_REQ);
    }

    void Clear()
    {
        tvChn.Clear();
    }

private:
    TTvChannel    tvChn;                       //加载电视通道
public:
    void SetTvChn(const TTvChannel& cTvChn)
    {
        tvChn = cTvChn;
    }
    TTvChannel& GetTvChn()
    {
        return tvChn;
    }
    const TTvChannel& GetTvChn() const
    {
        return tvChn;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CTvChannelWatchPointLoadRsp : public CEventRsp
{
public:
    CTvChannelWatchPointLoadRsp()
    {
        event = OspExtEventDesc(TV_CHANNEL_WATCH_POINT_LOAD_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CTvChannelWatchInfoGetReq : public CEventReq
{
public:
    CTvChannelWatchInfoGetReq()
    {
        event = OspExtEventDesc(TV_CHANNEL_WATCH_INFO_GET_REQ);
    }

    void Clear()
    {
        tvChn.Clear();
    }

private:
    TTvChannel    tvChn;                       //电视通道    
public:
    void SetTvChn(const TTvChannel& cTvChn)
    {
        tvChn = cTvChn;
    }
    TTvChannel& GetTvChn()
    {
        return tvChn;
    }
    const TTvChannel& GetTvChn() const
    {
        return tvChn;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CTvChannelWatchInfoGetRsp : public CEventRsp
{
public:
    CTvChannelWatchInfoGetRsp()
    {
        Clear();
        event = OspExtEventDesc(TV_CHANNEL_WATCH_INFO_GET_RSP);
    }

    void Clear()
    {
        tvChn.Clear();
        encChn.Clear();
        saveTime.Clear();
        isLock = false;
        lockTime.Clear();
    }

private:
    TTvChannel    tvChn;                       //电视通道
    TChannel      encChn;                      //编码通道
    TTimeRange    saveTime;                    //备份时间
    bool          isLock;                      //是否锁定
    TTimeRange    lockTime;                    //锁定时间
public:
    void SetTvChn(const TTvChannel& cTvChn)
    {
        tvChn = cTvChn;
    }
    TTvChannel& GetTvChn()
    {
        return tvChn;
    }
    const TTvChannel& GetTvChn() const
    {
        return tvChn;
    }

    void SetEncChn(const TChannel& cEncChn)
    {
        encChn = cEncChn;
    }
    TChannel& GetEncChn()
    {
        return encChn;
    }
    const TChannel& GetEncChn() const
    {
        return encChn;
    }

    void SetSaveTime(const TTimeRange& cSaveTime)
    {
        saveTime = cSaveTime;
    }
    TTimeRange& GetSaveTime()
    {
        return saveTime;
    }
    const TTimeRange& GetSaveTime() const
    {
        return saveTime;
    }

    void SetIsLock(bool bIsLock)
    {
        isLock = bIsLock;
    }
    bool GetIsLock() const
    {
        return isLock;
    }

    void SetLockTime(const TTimeRange& cLockTime)
    {
        lockTime = cLockTime;
    }
    TTimeRange& GetLockTime()
    {
        return lockTime;
    }
    const TTimeRange& GetLockTime() const
    {
        return lockTime;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

//电视墙预案锁定
class CTvWallSchemeLockReq : public CEventReq
{
public:
    CTvWallSchemeLockReq()
    {
        event = OspExtEventDesc(TVWALL_SCHEME_LOCK_REQ);
    }

    void Clear()
    {
        tvWallId.clear();
        lockTime.Clear();
        password.clear();
    }

private:
    string        tvWallId;                    //被锁定的电视墙
    TTimeRange    lockTime;                    //被锁定时间
    string        password;                    //锁定密码
public:
    void SetTvWallId(const string& strTvWallId)
    {
        tvWallId = strTvWallId;
    }
    string& GetTvWallId()
    {
        return tvWallId;
    }
    const string& GetTvWallId() const
    {
        return tvWallId;
    }

    void SetLockTime(const TTimeRange& cLockTime)
    {
        lockTime = cLockTime;
    }
    TTimeRange& GetLockTime()
    {
        return lockTime;
    }
    const TTimeRange& GetLockTime() const
    {
        return lockTime;
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

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CTvWallSchemeLockRsp : public CEventRsp
{
public:
    CTvWallSchemeLockRsp()
    {
        event = OspExtEventDesc(TVWALL_SCHEME_LOCK_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CTvWallSchemeUnlockReq : public CEventReq
{
public:
    CTvWallSchemeUnlockReq()
    {
        event = OspExtEventDesc(TVWALL_SCHEME_UNLOCK_REQ);
    }

    void Clear()
    {
        tvWallId.clear();
        password.clear();
    }

private:
    string        tvWallId;                    //被锁定的电视墙
    string        password;                    //锁定密码
public:
    void SetTvWallId(const string& strTvWallId)
    {
        tvWallId = strTvWallId;
    }
    string& GetTvWallId()
    {
        return tvWallId;
    }
    const string& GetTvWallId() const
    {
        return tvWallId;
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

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CTvWallSchemeUnlockRsp : public CEventRsp
{
public:
    CTvWallSchemeUnlockRsp()
    {
        event = OspExtEventDesc(TVWALL_SCHEME_UNLOCK_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


//{{{{{{{{{{{{{{{ modify by Royan 20140520 SetCameraType新增CameraId项
//摄像头类型参数配置
class CPuCameraTypeSetReq : public CEventReq
{
public:
	CPuCameraTypeSetReq()
	{
		event = OspExtEventDesc(PU_CAMERA_TYPE_SET_REQ);
	}

	void Clear()
	{
		userUri.clear();
		devChn.Clear();
		cameraType = 0;
		cameraId   = 0;
		CEventReq::Clear();
	}

	//事务模板会使用
	const string& GetDevUri() const
	{
		return devChn.GetDevUri();
	}

private:
	string              userUri;                    //操作用户
	TChannel            devChn;                     //设备通道
	int                 cameraType;                 //摄像头类型
	int                 cameraId;                   //摄像头地址码
public:
	void SetUserUri(const string& strUserUri)
	{
		userUri = strUserUri;
	}
	string& GetUserUri()
	{
		return userUri;
	}
	const string& GetUserUri() const
	{
		return userUri;
	}

	void SetDevChn(const TChannel& cDevChn)
	{
		devChn = cDevChn;
	}
	TChannel& GetDevChn()
	{
		return devChn;
	}
	const TChannel& GetDevChn() const
	{
		return devChn;
	}

	void SetCameraType(const int nCameraType)
	{
		cameraType = nCameraType;
	}
	int GetCameraType()
	{
		return cameraType;
	}

	void SetCameraId(const int nCameraId)
	{
		cameraId = nCameraId;
	}
	int GetCameraId()
	{
		return cameraId;
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};
//}}}}}}}}}}}}}}} modify by Royan 20140520 SetCameraType新增CameraId项

class CPuCameraTypeSetRsp : public CEventRsp
{
public:
	CPuCameraTypeSetRsp()
	{
		event = OspExtEventDesc(PU_CAMERA_TYPE_SET_RSP);
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};


//移动侦测参数配置
class CPuMotionDetectSetReq : public CEventReq
{
public:
    CPuMotionDetectSetReq()
    {
        event = OspExtEventDesc(PU_MOTION_DETECT_SET_REQ);
    }

    void Clear()
    {
        userUri.clear();
        devChn.Clear();
        param.clear();
        CEventReq::Clear();
    }

    //事务模板会使用
    const string& GetDevUri() const
    {
        return devChn.GetDevUri();
    }

private:
    string              userUri;                    //操作用户
    TChannel            devChn;                     //设备通道
    TMotionDetectList   param;                      //移动侦测参数
public:
    void SetUserUri(const string& strUserUri)
    {
        userUri = strUserUri;
    }
    string& GetUserUri()
    {
        return userUri;
    }
    const string& GetUserUri() const
    {
        return userUri;
    }

    void SetDevChn(const TChannel& cDevChn)
    {
        devChn = cDevChn;
    }
    TChannel& GetDevChn()
    {
        return devChn;
    }
    const TChannel& GetDevChn() const
    {
        return devChn;
    }

    void SetParam(const TMotionDetectList& cParam)
    {
        param = cParam;
    }
    TMotionDetectList& GetParam()
    {
        return param;
    }
    const TMotionDetectList& GetParam() const
    {
        return param;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPuMotionDetectSetRsp : public CEventRsp
{
public:
    CPuMotionDetectSetRsp()
    {
        event = OspExtEventDesc(PU_MOTION_DETECT_SET_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPuMotionDetectGetReq : public CEventReq
{
public:
    CPuMotionDetectGetReq()
    {
        event = OspExtEventDesc(PU_MOTION_DETECT_GET_REQ);
    }

    void Clear()
    {
        userUri.clear();
        devChn.Clear();
        CEventReq::Clear();
    }

    //事务模板会使用
    const string& GetDevUri() const
    {
        return devChn.GetDevUri();
    }

private:
    string             userUri;                //操作用户
    TChannel           devChn;                 //设备通道
public:
    void SetUserUri(const string& strUserUri)
    {
        userUri = strUserUri;
    }
    string& GetUserUri()
    {
        return userUri;
    }
    const string& GetUserUri() const
    {
        return userUri;
    }

    void SetDevChn(const TChannel& cDevChn)
    {
        devChn = cDevChn;
    }
    TChannel& GetDevChn()
    {
        return devChn;
    }
    const TChannel& GetDevChn() const
    {
        return devChn;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPuMotionDetectGetRsp : public CEventRsp
{
public:
    CPuMotionDetectGetRsp()
    {
        event = OspExtEventDesc(PU_MOTION_DETECT_GET_RSP);
    }

    void Clear()
    {
        param.clear();
        CEventRsp::Clear();
    }

private:
    TMotionDetectList   param;                      //移动侦测参数
public:
    void SetParam(const TMotionDetectList& cParam)
    {
        param = cParam;
    }
    TMotionDetectList& GetParam()
    {
        return param;
    }
    const TMotionDetectList& GetParam() const
    {
        return param;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


//图像遮蔽参数配置
class CPuAreaShadeSetReq : public CEventReq
{
public:
    CPuAreaShadeSetReq()
    {
        event = OspExtEventDesc(PU_AREA_SHADE_SET_REQ);
    }

    void Clear()
    {
        userUri.clear();
        devChn.Clear();
        param.clear();
        CEventReq::Clear();
    }

    //事务模板会使用
    const string& GetDevUri() const
    {
        return devChn.GetDevUri();
    }

private:
    string              userUri;                    //操作用户
    TChannel            devChn;                     //设备通道
    TAreaShadeList      param;                      //图像遮蔽参数
public:
    void SetUserUri(const string& strUserUri)
    {
        userUri = strUserUri;
    }
    string& GetUserUri()
    {
        return userUri;
    }
    const string& GetUserUri() const
    {
        return userUri;
    }

    void SetDevChn(const TChannel& cDevChn)
    {
        devChn = cDevChn;
    }
    TChannel& GetDevChn()
    {
        return devChn;
    }
    const TChannel& GetDevChn() const
    {
        return devChn;
    }

    void SetParam(const TAreaShadeList& cParam)
    {
        param = cParam;
    }
    TAreaShadeList& GetParam()
    {
        return param;
    }
    const TAreaShadeList& GetParam() const
    {
        return param;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPuAreaShadeSetRsp : public CEventRsp
{
public:
    CPuAreaShadeSetRsp()
    {
        event = OspExtEventDesc(PU_AREA_SHADE_SET_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPuAreaShadeGetReq : public CEventReq
{
public:
    CPuAreaShadeGetReq()
    {
        event = OspExtEventDesc(PU_AREA_SHADE_GET_REQ);
    }

    void Clear()
    {
        userUri.clear();
        devChn.Clear();
        CEventReq::Clear();
    }

    //事务模板会使用
    const string& GetDevUri() const
    {
        return devChn.GetDevUri();
    }

private:
    string             userUri;                //操作用户
    TChannel           devChn;                 //设备通道
public:
    void SetUserUri(const string& strUserUri)
    {
        userUri = strUserUri;
    }
    string& GetUserUri()
    {
        return userUri;
    }
    const string& GetUserUri() const
    {
        return userUri;
    }

    void SetDevChn(const TChannel& cDevChn)
    {
        devChn = cDevChn;
    }
    TChannel& GetDevChn()
    {
        return devChn;
    }
    const TChannel& GetDevChn() const
    {
        return devChn;
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPuAreaShadeGetRsp : public CEventRsp
{
public:
    CPuAreaShadeGetRsp()
    {
        event = OspExtEventDesc(PU_AREA_SHADE_GET_RSP);
    }

    void Clear()
    {
        param.clear();
        CEventRsp::Clear();
    }

private:
    TAreaShadeList   param;                      //图像遮蔽参数
public:
    void SetParam(const TAreaShadeList& cParam)
    {
        param = cParam;
    }
    TAreaShadeList& GetParam()
    {
        return param;
    }
    const TAreaShadeList& GetParam() const
    {
        return param;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


//GPS订阅相关
class CPuGpsCfgSetReq : public CEventReq
{
public:
    CPuGpsCfgSetReq()
    {
        event = OspExtEventDesc(PU_GPS_CFG_SET_REQ);
    }

    void Clear()
    {
        userUri.clear();
        devUri.clear();
        param.Clear();
        CEventReq::Clear();
    }

private:
    string              userUri;                  //操作用户
    string              devUri;                   //操作设备
    TGpsCfgParam        param;                    //GPS配置参数
public:
    void SetUserUri(const string& strUserUri)
    {
        userUri = strUserUri;
    }
    string& GetUserUri()
    {
        return userUri;
    }
    const string& GetUserUri() const
    {
        return userUri;
    }

    void SetDevUri(const string& strDevUri)
    {
        devUri = strDevUri;
    }
    string& GetDevUri()
    {
        return devUri;
    }
    const string& GetDevUri() const
    {
        return devUri;
    }

    void SetParam(const TGpsCfgParam& cParam)
    {
        param = cParam;
    }
    TGpsCfgParam& GetParam()
    {
        return param;
    }
    const TGpsCfgParam& GetParam() const
    {
        return param;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPuGpsCfgSetRsp : public CEventRsp
{
public:
    CPuGpsCfgSetRsp()
    {
        event = OspExtEventDesc(PU_GPS_CFG_SET_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPuGpsCfgGetReq : public CEventReq
{
public:
    CPuGpsCfgGetReq()
    {
        event = OspExtEventDesc(PU_GPS_CFG_GET_REQ);
    }

    void Clear()
    {
        userUri.clear();
        devUri.clear();
        CEventReq::Clear();
    }

private:
    string              userUri;                  //操作用户
    string              devUri;                   //操作设备
public:
    void SetUserUri(const string& strUserUri)
    {
        userUri = strUserUri;
    }
    string& GetUserUri()
    {
        return userUri;
    }
    const string& GetUserUri() const
    {
        return userUri;
    }

    void SetDevUri(const string& strDevUri)
    {
        devUri = strDevUri;
    }
    string& GetDevUri()
    {
        return devUri;
    }
    const string& GetDevUri() const
    {
        return devUri;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPuGpsCfgGetRsp : public CEventRsp
{
public:
    CPuGpsCfgGetRsp()
    {
        event = OspExtEventDesc(PU_GPS_CFG_GET_RSP);
    }

    void Clear()
    {
        param.Clear();
        CEventRsp::Clear();
    }

private:
    TGpsCfgParam        param;                    //GPS配置参数
public:
    void SetParam(const TGpsCfgParam& cParam)
    {
        param = cParam;
    }
    TGpsCfgParam& GetParam()
    {
        return param;
    }
    const TGpsCfgParam& GetParam() const
    {
        return param;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPuGpsSsSetReq : public CEventReq
{
public:
    CPuGpsSsSetReq()
    {
        event = OspExtEventDesc(PU_GPS_SS_SET_REQ);
    }

    void Clear()
    {
        userUri.clear();
        devUri.clear();
        param.Clear();
        CEventReq::Clear();
    }

private:
    string              userUri;                  //操作用户
    string              devUri;                   //操作设备
    TGpsSsParam         param;                    //GPS订阅参数
public:
    void SetUserUri(const string& strUserUri)
    {
        userUri = strUserUri;
    }
    string& GetUserUri()
    {
        return userUri;
    }
    const string& GetUserUri() const
    {
        return userUri;
    }

    void SetDevUri(const string& strDevUri)
    {
        devUri = strDevUri;
    }
    string& GetDevUri()
    {
        return devUri;
    }
    const string& GetDevUri() const
    {
        return devUri;
    }

    void SetParam(const TGpsSsParam& cParam)
    {
        param = cParam;
    }
    TGpsSsParam& GetParam()
    {
        return param;
    }
    const TGpsSsParam& GetParam() const
    {
        return param;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPuGpsSsSetRsp : public CEventRsp
{
public:
    CPuGpsSsSetRsp()
    {
        event = OspExtEventDesc(PU_GPS_SS_SET_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPuGpsSsGetReq : public CEventReq
{
public:
    CPuGpsSsGetReq()
    {
        event = OspExtEventDesc(PU_GPS_SS_GET_REQ);
    }

    void Clear()
    {
        userUri.clear();
        devUri.clear();
        CEventReq::Clear();
    }

private:
    string              userUri;                  //操作用户
    string              devUri;                   //操作设备
    TGpsSsInquiry       param;                    //GPS查询参数
public:
    void SetUserUri(const string& strUserUri)
    {
        userUri = strUserUri;
    }
    string& GetUserUri()
    {
        return userUri;
    }
    const string& GetUserUri() const
    {
        return userUri;
    }

    void SetDevUri(const string& strDevUri)
    {
        devUri = strDevUri;
    }
    string& GetDevUri()
    {
        return devUri;
    }
    const string& GetDevUri() const
    {
        return devUri;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetParam(const TGpsSsInquiry& cParam)
    {
        param = cParam;
    }
    TGpsSsInquiry& GetParam()
    {
        return param;
    }
    const TGpsSsInquiry& GetParam() const
    {
        return param;
    }

};

class CPuGpsSsGetRsp : public CEventRsp
{
public:
    CPuGpsSsGetRsp()
    {
        event = OspExtEventDesc(PU_GPS_SS_GET_RSP);
    }

    void Clear()
    {
        param.Clear();
        CEventRsp::Clear();
    }

private:
    TGpsSsParam        param;                    //GPS配置参数
public:
    void SetParam(const TGpsSsParam& cParam)
    {
        param = cParam;
    }
    TGpsSsParam& GetParam()
    {
        return param;
    }
    const TGpsSsParam& GetParam() const
    {
        return param;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPuGpsDataNtyReq : public CEventReq
{
public:
    CPuGpsDataNtyReq()
    {
        event = OspExtEventDesc(PU_GPS_DATA_NTF_REQ);
    }

    void Clear()
    {
        data.Clear();
        CEventReq::Clear();
    }

private:
    TGpsData      data;                              //gps数据
public:
    void SetData(const TGpsData& cData)
    {
        data = cData;
    }
    TGpsData& GetData()
    {
        return data;
    }
    const TGpsData& GetData() const
    {
        return data;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPuGpsDataNtyRsp : public CEventRsp
{
public:
    CPuGpsDataNtyRsp()
    {
        event = OspExtEventDesc(PU_GPS_DATA_NTF_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

//透明数据配置的设置、获取
class CPuTransdataCfgSetReq : public CEventReq
{
public:
    CPuTransdataCfgSetReq()
    {
        event = OspExtEventDesc(PU_TRANSDATA_CFG_SET_REQ);
    }

    void Clear()
    {
        userUri.clear();
        devUri.clear();
        param.Clear();
        CEventReq::Clear();
    }

private:
    string              userUri;                  //操作用户
    string              devUri;                   //操作设备
    TTransDataCfgParam  param;                    //配置参数
public:
    void SetUserUri(const string& strUserUri)
    {
        userUri = strUserUri;
    }
    string& GetUserUri()
    {
        return userUri;
    }
    const string& GetUserUri() const
    {
        return userUri;
    }

    void SetDevUri(const string& strDevUri)
    {
        devUri = strDevUri;
    }
    string& GetDevUri()
    {
        return devUri;
    }
    const string& GetDevUri() const
    {
        return devUri;
    }

    void SetParam(const TTransDataCfgParam& cParam)
    {
        param = cParam;
    }
    TTransDataCfgParam& GetParam()
    {
        return param;
    }
    const TTransDataCfgParam& GetParam() const
    {
        return param;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPuTransdataCfgSetRsp : public CEventRsp
{
public:
    CPuTransdataCfgSetRsp()
    {
        event = OspExtEventDesc(PU_TRANSDATA_CFG_SET_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPuTransdataCfgGetReq : public CEventReq
{
public:
    CPuTransdataCfgGetReq()
    {
        event = OspExtEventDesc(PU_TRANSDATA_CFG_GET_REQ);
    }

    void Clear()
    {
        userUri.clear();
        devUri.clear();
        param.Clear();
        CEventReq::Clear();
    }

private:
    string              userUri;                  //操作用户
    string              devUri;                   //操作设备
    TTransDataCfgParam  param;                    //配置参数
public:
    void SetUserUri(const string& strUserUri)
    {
        userUri = strUserUri;
    }
    string& GetUserUri()
    {
        return userUri;
    }
    const string& GetUserUri() const
    {
        return userUri;
    }

    void SetDevUri(const string& strDevUri)
    {
        devUri = strDevUri;
    }
    string& GetDevUri()
    {
        return devUri;
    }
    const string& GetDevUri() const
    {
        return devUri;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetParam(const TTransDataCfgParam& cParam)
    {
        param = cParam;
    }
    TTransDataCfgParam& GetParam()
    {
        return param;
    }
    const TTransDataCfgParam& GetParam() const
    {
        return param;
    }

};

class CPuTransdataCfgGetRsp : public CEventRsp
{
public:
    CPuTransdataCfgGetRsp()
    {
        event = OspExtEventDesc(PU_TRANSDATA_CFG_GET_RSP);
    }

    void Clear()
    {
        param.Clear();
        CEventRsp::Clear();
    }

private:
    TTransDataCfgParam        param;                //配置参数
public:
    void SetParam(const TTransDataCfgParam& cParam)
    {
        param = cParam;
    }
    TTransDataCfgParam& GetParam()
    {
        return param;
    }
    const TTransDataCfgParam& GetParam() const
    {
        return param;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

//前端录像开启、停止
class CPuRecordStartReq : public CEventReq
{
public:
    CPuRecordStartReq()
    {
        event = OspExtEventDesc(PU_RECORD_START_REQ);
    }

    void Clear()
    {
        devChn.Clear();
        CEventReq::Clear();
    }

private:
    TChannel           devChn;                            //设备通道

public:
    //事务模板会使用
    const string& GetDevUri() const
    {
        return devChn.GetDevUri();
    }
public:
    void SetDevChn(const TChannel& cDevChn)
    {
        devChn = cDevChn;
    }
    TChannel& GetDevChn()
    {
        return devChn;
    }
    const TChannel& GetDevChn() const
    {
        return devChn;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPuRecordStartRsp : public CEventRsp
{
public:
    CPuRecordStartRsp()
    {
        event = OspExtEventDesc(PU_RECORD_START_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

//{{{{{{{{{{{{{{{ add by Royan 20140526 前端录像状态通知
//前端录像状态通知
class CPuRecordStatusNtfReq : public CEventReq
{
public:
	CPuRecordStatusNtfReq()
	{
		event = OspExtEventDesc(PU_RECORED_STATUS_NTF_REQ);
	}

	void Clear()
	{
		devChn.Clear();
		recStat.Clear();
		CEventReq::Clear();
	}

private:
	TChannel           devChn;                            //设备通道
	TPuChnRecStatInfo  recStat;                           //通道录像状态

public:
	//事务模板会使用
	const string& GetDevUri() const
	{
		return devChn.GetDevUri();
	}
public:
	void SetDevChn(const TChannel& tDevChn)
	{
		devChn = tDevChn;
	}
	TChannel& GetDevChn()
	{
		return devChn;
	}
	const TChannel& GetDevChn() const
	{
		return devChn;
	}

	void SetChnRecStat(const TPuChnRecStatInfo& tRecStat)
	{
		recStat = tRecStat;
	}
	TPuChnRecStatInfo& GetChnRecStat()
	{
		return recStat;
	}
	const TPuChnRecStatInfo& GetChnRecStat() const
	{
		return recStat;
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class CPuRecordStatusNtfRsp : public CEventRsp
{
public:
	CPuRecordStatusNtfRsp()
	{
		event = OspExtEventDesc(PU_RECORED_STATUS_NTF_RSP);
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};
//}}}}}}}}}}}}}}} add by Royan 20140526 前端录像状态通知

class CPuRecordStopReq : public CEventReq
{
public:
    CPuRecordStopReq()
    {
        event = OspExtEventDesc(PU_RECORD_STOP_REQ);
    }

    void Clear()
    {
        devChn.Clear();
        CEventReq::Clear();
    }

private:
    TChannel           devChn;                            //设备通道

public:
    //事务模板会使用
    const string& GetDevUri() const
    {
        return devChn.GetDevUri();
    }
public:
    void SetDevChn(const TChannel& cDevChn)
    {
        devChn = cDevChn;
    }
    TChannel& GetDevChn()
    {
        return devChn;
    }
    const TChannel& GetDevChn() const
    {
        return devChn;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPuRecordStopRsp : public CEventRsp
{
public:
    CPuRecordStopRsp()
    {
        event = OspExtEventDesc(PU_RECORD_STOP_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

//前端录像删除
class CPuRecordDelReq : public CEventReq
{
public:
    CPuRecordDelReq()
    {
        event = OspExtEventDesc(PU_RECORD_DEL_REQ);
    }

    void Clear()
    {
        devChn.Clear();
        recInfo.Clear();
        CEventReq::Clear();
    }

private:
    TChannel           devChn;                            //设备通道
    TPuRecInfo         recInfo;                           //录像信息

public:
    //事务模板会使用
    const string& GetDevUri() const
    {
        return devChn.GetDevUri();
    }
public:
    void SetDevChn(const TChannel& cDevChn)
    {
        devChn = cDevChn;
    }
    TChannel& GetDevChn()
    {
        return devChn;
    }
    const TChannel& GetDevChn() const
    {
        return devChn;
    }

    void SetRecInfo(const TPuRecInfo& cRecInfo)
    {
        recInfo = cRecInfo;
    }
    TPuRecInfo& GetRecInfo()
    {
        return recInfo;
    }
    const TPuRecInfo& GetRecInfo() const
    {
        return recInfo;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPuRecordDelRsp : public CEventRsp
{
public:
    CPuRecordDelRsp()
    {
        event = OspExtEventDesc(PU_RECORD_DEL_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

//前端录像查询
/*
//录象文件/事件类型/图片事件
#define MTI_TYPE_H264					   0x10 //h264文件类型
#define MTI_QUIEY_ALL_EVENT                0    //查询所有事件
#define MTI_TYPE_PIN_ALARM                 1    //并口告警录象
#define MTI_TYPE_MOTION_DETECT_ALARM       2    //移动侦测告警录象
#define MTI_TYPE_MANUAL                    3    //手动录象
#define MTI_TYPE_TIMELY                    4    //定时录象
#define MTI_TYPE_PERIODLY                  5    //周期录象
#define MTI_QUIEY_FILE                     6    //按文件查询
*/

#define PU_EVENT_TYPE_ALL                        "all"              //查询所有事件
#define PU_EVENT_TYPE_PIN_ALARM                  "pin_alarm"        //并口告警录象
#define PU_EVENT_TYPE_MOTION_DETECT_ALARM        "motion_detect"    //移动侦测告警录象
#define PU_EVENT_TYPE_MANUAL                     "manual"           //手动录象
#define PU_EVENT_TYPE_TIMELY                     "timely"           //定时录象
#define PU_EVENT_TYPE_PERIODLY                   "periodly"         //周期录象
#define PU_EVENT_TYPE_FILE                       "file"             //按文件查询
#define PU_EVENT_TYPE_H264					   "h264"             //h264文件类型

class CPuRecQryReq : public CQueryReqBase
{
public:
    CPuRecQryReq()
	{
		Clear();
		event = OspExtEventDesc(PU_REC_QRY_REQ);
	}

	void Clear()
	{
		devChn.Clear();
		recType.clear();
		timeRange.Clear();
		extInfo.clear();
		CEventReq::Clear();
	}

private:
    TChannel           devChn;                     //设备通道
    string             recType;                    //录像查询条件, 根据vsip定义
    TTimeRange         timeRange;                  //时间段
	TExtInfo           extInfo;                    //扩展信息
public:
    void SetDevChn(const TChannel& cDevChn)
    {
        devChn = cDevChn;
    }
    TChannel& GetDevChn()
    {
        return devChn;
    }
    const TChannel& GetDevChn() const
    {
        return devChn;
    }

    void SetRecType(const string& strRecType)
    {
        recType = strRecType;
    }
    string& GetRecType()
    {
        return recType;
    }
    const string& GetRecType() const
    {
        return recType;
    }

    void SetTimeRange(const TTimeRange& cTimeRange)
    {
        timeRange = cTimeRange;
    }
    TTimeRange& GetTimeRange()
    {
        return timeRange;
    }
    const TTimeRange& GetTimeRange() const
    {
        return timeRange;
    }

	void SetExtInfo(const TExtInfo& tExtInfo)
	{
		extInfo = tExtInfo;
	}
	TExtInfo& GetExtInfo()
	{
		return extInfo;
	}
	const TExtInfo& GetExtInfo() const
	{
		return extInfo;
	}

    //是否支持夏令时GMT格式
    void SetExtInfo(const string& strKey, bool bValue)
    {
        extInfo[strKey] = BoolStr(bValue);
    }
    bool GetExtInfo(const string& strKey, bool& bValue) const
    {
        TExtInfo::const_iterator itr = extInfo.find(strKey);
        if (itr != extInfo.end())
        {
            bValue = (itr->second == TRUE_STR);

            return true;
        }

        return false;
    }
    void SetGMTLabel(bool bEnable)
    {
        SetExtInfo(SDP_EXT_INFO_GMT_LABEL, bEnable);
    }
    bool GetGMTLabel() const
    {
        bool bVar = false;

        GetExtInfo(SDP_EXT_INFO_GMT_LABEL, bVar);

        return bVar;
    }


    //事务模板会使用
    const string& GetDevUri() const
    {
        return devChn.GetDevUri();
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPuRecQryRsp : public CQueryRspBase
{
public:
    CPuRecQryRsp()
    {
		Clear();
        event = OspExtEventDesc(PU_REC_QRY_RSP);
    }

	void Clear()
	{
		extInfo.clear();
	}
private:
	TExtInfo           extInfo;                    //扩展信息
public:
	//是否支持夏令时GMT格式
	void SetExtInfo(const string& strKey, bool bValue)
	{
		extInfo[strKey] = BoolStr(bValue);
	}
	bool GetExtInfo(const string& strKey, bool& bValue) const
	{
		TExtInfo::const_iterator itr = extInfo.find(strKey);
		if (itr != extInfo.end())
		{
			bValue = (itr->second == TRUE_STR);

			return true;
		}

		return false;
	}
	void SetGMTLabel(bool bEnable)
	{
		SetExtInfo(SDP_EXT_INFO_GMT_LABEL, bEnable);
	}
	bool GetGMTLabel() const
	{
		bool bVar = false;

		GetExtInfo(SDP_EXT_INFO_GMT_LABEL, bVar);

		return bVar;
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPuRecQryNtfReq : public CQueryNtfReqBase
{
public:
    CPuRecQryNtfReq()
    {
		Clear();
        event = OspExtEventDesc(PU_REC_QRY_NTF_REQ);
    }

    void Clear()
    {
        puChn.Clear();
        recInfoList.clear();
        CEventReq::Clear();
    }

private:
    TChannel            puChn;                         //设备通道
    CPuRecInfoList      recInfoList;                   //录像信息列表
public:
    void SetPuChn(const TChannel& cPuChn)
    {
        puChn = cPuChn;
    }
    TChannel& GetPuChn()
    {
        return puChn;
    }
    const TChannel& GetPuChn() const
    {
        return puChn;
    }

    void SetRecInfoList(const CPuRecInfoList& cRecInfoList)
    {
        recInfoList = cRecInfoList;
    }
    CPuRecInfoList& GetRecInfoList()
    {
        return recInfoList;
    }
    const CPuRecInfoList& GetRecInfoList() const
    {
        return recInfoList;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPuRecQryNtfRsp : public CEventRsp
{
public:
    CPuRecQryNtfRsp()
    {
        event = OspExtEventDesc(PU_REC_QRY_NTF_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

//前端录像日历查询
class CPuRecDaysQryReq : public CEventReq
{
public:
    CPuRecDaysQryReq()
    {
        event = OspExtEventDesc(PU_REC_DAYS_QRY_REQ);
        Clear();
    }

    void Clear()
    {
        devChn.Clear();
        timeCond.clear();
        CEventReq::Clear();
    }

private:
    TChannel        devChn;                                   //设备通道
    /*
    1、查询全年每月录像分布情况（比如，年=2013，月=0，日=0）
    2、查询指定月份录像分布情况（比如，年=2013，月=1，日=0）
    3、查询指定日期录像分布情况（比如，年=2013，月=1，日=3）
    年为公元年份号，月取值范围为0~12，日取值范围为0~31。
    */
    string          timeCond;                                 //时间条件

public:
    //事务模板会使用
    const string& GetDevUri() const
    {
        return devChn.GetDevUri();
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetDevChn(const TChannel& cDevChn)
    {
        devChn = cDevChn;
    }
    TChannel& GetDevChn()
    {
        return devChn;
    }
    const TChannel& GetDevChn() const
    {
        return devChn;
    }

    void SetTimeCond(const string& strTimeCond)
    {
        timeCond = strTimeCond;
    }
    string& GetTimeCond()
    {
        return timeCond;
    }
    const string& GetTimeCond() const
    {
        return timeCond;
    }

};

class CPuRecDaysQryRsp : public CEventRsp
{
public:
    CPuRecDaysQryRsp()
    {
        event = OspExtEventDesc(PU_REC_DAYS_QRY_RSP);
    }

    void Clear()
    {
        resultBitMask = 0;
        CEventRsp::Clear();
    }

private:
    int        resultBitMask;                                   //查询结果

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetResultBitMask(int nResultBitMask)
    {
        resultBitMask = nResultBitMask;
    }
    int GetResultBitMask() const
    {
        return resultBitMask;
    }

};

//解码器风格配置、获取
class CPuDecStyleSetReq : public CEventReq
{
public:
    CPuDecStyleSetReq()
    {
        event = OspExtEventDesc(PU_DEC_STYLE_SET_REQ);
    }

    void Clear()
    {
        userUri.clear();
        devUri.clear();
        decOutputId = 0;
        style = 0;
        CEventReq::Clear();
    }

private:
    string              userUri;                                 //操作用户
    string              devUri;                                  //设备URI
    int                 decOutputId;                             //解码器HDMI输出通道
    int                 style;                                   //解码器风格
public:
    void SetUserUri(const string& strUserUri)
    {
        userUri = strUserUri;
    }
    string& GetUserUri()
    {
        return userUri;
    }
    const string& GetUserUri() const
    {
        return userUri;
    }

    void SetDevUri(const string& strDevUri)
    {
        devUri = strDevUri;
    }
    string& GetDevUri()
    {
        return devUri;
    }
    const string& GetDevUri() const
    {
        return devUri;
    }

    void SetStyle(int nStyle)
    {
        style = nStyle;
    }
    int GetStyle() const
    {
        return style;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetDecOutputId(int nDecOutputId)
    {
        decOutputId = nDecOutputId;
    }
    int GetDecOutputId() const
    {
        return decOutputId;
    }

};

class CPuDecStyleSetRsp : public CEventRsp
{
public:
    CPuDecStyleSetRsp()
    {
        event = OspExtEventDesc(PU_DEC_STYLE_SET_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPuDecStyleGetReq : public CEventReq
{
public:
    CPuDecStyleGetReq()
    {
        event = OspExtEventDesc(PU_DEC_STYLE_GET_REQ);
    }

    void Clear()
    {
        userUri.clear();
        devUri.clear();
        decOutputId = 0;
        CEventReq::Clear();
    }

private:
    string              userUri;                                 //操作用户
    string              devUri;                                  //设备URI
    int                 decOutputId;                             //解码器HDMI输出通道
public:
    void SetUserUri(const string& strUserUri)
    {
        userUri = strUserUri;
    }
    string& GetUserUri()
    {
        return userUri;
    }
    const string& GetUserUri() const
    {
        return userUri;
    }

    void SetDevUri(const string& strDevUri)
    {
        devUri = strDevUri;
    }
    string& GetDevUri()
    {
        return devUri;
    }
    const string& GetDevUri() const
    {
        return devUri;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetDecOutputId(int nDecOutputId)
    {
        decOutputId = nDecOutputId;
    }
    int GetDecOutputId() const
    {
        return decOutputId;
    }

};

class CPuDecStyleGetRsp : public CEventRsp
{
public:
    CPuDecStyleGetRsp()
    {
        event = OspExtEventDesc(PU_DEC_STYLE_GET_RSP);
    }

    void Clear()
    {
        style = 0;
        CEventRsp::Clear();
    }

private:
    int                 style;                                   //解码器风格
public:
    void SetStyle(int nStyle)
    {
        style = nStyle;
    }
    int GetStyle() const
    {
        return style;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CVcrCtrlReq : public CEventReq
{
public:
    CVcrCtrlReq()
    {
        Clear();
        event = OspExtEventDesc(VCR_CTRL_REQ);
    }

    void Clear()
    {
        playbackSessUri.clear();
        vcrCmd.clear();
        playRate = 0;
        seekTime = 0;
    }

public:
    //playbackSessUri
    void SetPlaybackSessUri(const string& var)
    {
        playbackSessUri = var;
    }
    string& GetPlaybackSessUri()
    {
        return playbackSessUri;
    }
    const string& GetPlaybackSessUri() const
    {
        return playbackSessUri;
    }

    //vcrCmd
    const TVcrCmd& GetVcrCmd() const
    {
        return vcrCmd;
    }
    TVcrCmd& GetVcrCmd()
    {
        return vcrCmd;
    }
    void SetVcrCmd(const TVcrCmd& var)
    {
        vcrCmd = var;
    }

    //playRate
    int GetPlayRate() const
    {
        return playRate;
    }
    void SetPlayRate(int var)
    {
        playRate = var;
    }

    //seekTime
    int GetSeekTime() const
    {
        return seekTime;
    }
    void SetSeekTime(int var)
    {
        seekTime = var;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

private:
	string  playbackSessUri;
    TVcrCmd vcrCmd;
    int     playRate;       //放像速率，正数为正反，负数为倒放
    int     seekTime;       //相对时间
};

class CVcrCtrlRsp : public CEventRsp
{
public:
    CVcrCtrlRsp()
    {
        event = OspExtEventDesc(VCR_CTRL_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CPlaybackProgressQueryReq : public CEventReq
{
private:
	string  playbackSessUri;

public:
    CPlaybackProgressQueryReq()
    {
        Clear();
        event = OspExtEventDesc(PLAYBACK_PROGRESS_QUERY_REQ);
    }

    void Clear()
    {
        playbackSessUri.clear();
    }

public:
    void SetPlaybackSessUri(const string& var)
    {
        playbackSessUri = var;
    }
    string& GetPlaybackSessUri()
    {
        return playbackSessUri;
    }
    const string& GetPlaybackSessUri() const
    {
        return playbackSessUri;
    }


public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPlaybackProgressQueryRsp : public CEventRsp
{
private:
	u32         curPlayTime;                             //当前放像时间，单位：秒
	u32         totalPlayTime;                           //放像总时长
	u32         startPlayTime;                           //起始放像时间，单位：秒
	bool        isPlayFinished;                          //放像是否结束

public:
    CPlaybackProgressQueryRsp()
    {
        event = OspExtEventDesc(PLAYBACK_PROGRESS_QUERY_RSP);
    }

    void SetCurPlayTime(u32 nCurPlayTime)
    {
        curPlayTime = nCurPlayTime;
    }
    u32 GetCurPlayTime() const
    {
        return curPlayTime;
    }

    void SetTotalPlayTime(u32 nTotalPlayTime)
    {
        totalPlayTime = nTotalPlayTime;
    }
    u32 GetTotalPlayTime() const
    {
        return totalPlayTime;
    }

    void SetStartPlayTime(u32 nStartPlayTime)
    {
        startPlayTime = nStartPlayTime;
    }
    u32 GetStartPlayTime() const
    {
        return startPlayTime;
    }

    void SetIsPlayFinished(bool bIsPlayFinished)
    {
        isPlayFinished = bIsPlayFinished;
    }
    bool GetIsPlayFinished() const
    {
        return isPlayFinished;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

//平台内部使用进度主动通知的方式
class CPlaybackProgressNtfReq : public CEventReq
{
private:
    u32         curPlayTime;                             //当前放像时间，单位：秒
    u32         totalPlayTime;                           //放像总时长, 单位：秒
    u32         startPlayTime;                           //起始放像时间，单位：秒
    bool        isPlayFinished;                          //放像是否结束

public:
    CPlaybackProgressNtfReq()
    {
        Clear();
        event = OspExtEventDesc(PLAYBACK_PROGRESS_NTF_REQ);
    }

    void Clear()
    {
        curPlayTime = 0;    
        totalPlayTime = 0;   
        startPlayTime = 0;   
        isPlayFinished = 0;  
    }

public:
    void SetCurPlayTime(u32 nCurPlayTime)
    {
        curPlayTime = nCurPlayTime;
    }
    u32 GetCurPlayTime() const
    {
        return curPlayTime;
    }

    void SetTotalPlayTime(u32 nTotalPlayTime)
    {
        totalPlayTime = nTotalPlayTime;
    }
    u32 GetTotalPlayTime() const
    {
        return totalPlayTime;
    }

    void SetStartPlayTime(u32 nStartPlayTime)
    {
        startPlayTime = nStartPlayTime;
    }
    u32 GetStartPlayTime() const
    {
        return startPlayTime;
    }

    void SetIsPlayFinished(bool bIsPlayFinished)
    {
        isPlayFinished = bIsPlayFinished;
    }
    bool GetIsPlayFinished() const
    {
        return isPlayFinished;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPlaybackProgressNtfRsp : public CEventRsp
{
public:
    CPlaybackProgressNtfRsp()
    {
        event = OspExtEventDesc(PLAYBACK_PROGRESS_NTF_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};



class CEndOfFileNtfReq : public CEventReq
{
private:
    string  playbackSessUri;

public:
    CEndOfFileNtfReq()
    {
        event = OspExtEventDesc(END_OF_FILE_NTF_REQ);
    }

public:
    void SetPlaybackSessUri(const string& var)
    {
        playbackSessUri = var;
    }
    string& GetPlaybackSessUri()
    {
        return playbackSessUri;
    }
    const string& GetPlaybackSessUri() const
    {
        return playbackSessUri;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CEndOfFileNtfRsp : public CEventRsp
{
public:
    CEndOfFileNtfRsp()
    {
        event = OspExtEventDesc(END_OF_FILE_NTF_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CKeyFrameReq : public CEventReq
{
private:
    TChannel    devChn;

public:
    CKeyFrameReq()
    {
        event = OspExtEventDesc(KEY_FRAME_REQ);
    }

public:
    void SetDevChn(const TChannel& var)
    {
        devChn = var;
    }
    TChannel& GetDevChn()
    {
        return devChn;
    }
    const TChannel& GetDevChn() const
    {
        return devChn;
    }

    const string& GetDevUri() const
    {
        return devChn.GetDevUri();
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CKeyFrameRsp : public CEventRsp
{
public:
    CKeyFrameRsp()
    {
        event = OspExtEventDesc(KEY_FRAME_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

//前端图片抓拍
class CPuPicSnapshotReq : public CEventReq
{
public:
    CPuPicSnapshotReq()
    {
        event = OspExtEventDesc(PU_PIC_SNAPSHOT_REQ);
    }

    void Clear()
    {
        devChn.Clear();
    }

private:
    TChannel           devChn;                                  //设备通道

public:
    //事务模板会使用
    const string& GetDevUri() const
    {
        return devChn.GetDevUri();
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetDevChn(const TChannel& cDevChn)
    {
        devChn = cDevChn;
    }
    TChannel& GetDevChn()
    {
        return devChn;
    }
    const TChannel& GetDevChn() const
    {
        return devChn;
    }

};

class CPuPicSnapshotRsp : public CEventRsp
{
public:
    CPuPicSnapshotRsp()
    {
        event = OspExtEventDesc(PU_PIC_SNAPSHOT_RSP);
    }

private:
    string  picID;                        //图片标识
    string  picName;                      //图片名称，门店管理使用。

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetPicID(const string& strPicID)
    {
        picID = strPicID;
    }
    string& GetPicID()
    {
        return picID;
    }
    const string& GetPicID() const
    {
        return picID;
    }

public:
    void SetPicName(const string& strPicName)
    {
        picName = strPicName;
    }
    string& GetPicName()
    {
        return picName;
    }
    const string& GetPicName() const
    {
        return picName;
    }

};

//前端图片下载
class CPuPicDownloadReq : public CEventReq
{
public:
    CPuPicDownloadReq()
    {
        Clear();
        event = OspExtEventDesc(PU_PIC_DOWNLOAD_REQ);
    }

    void Clear()
    {
        devChn.Clear();
        picID.clear();
        packIndex = 1;     //前端要求从1开始计数

        CEventReq::Clear();
    }

private:
    TChannel            devChn;                              //设备通道
    string              picID;                              //图片标识
    int                 packIndex;                          //图片数据包序号，从1开始计数

public:
    //事务模板会使用
    const string& GetDevUri() const
    {
        return devChn.GetDevUri();
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetDevChn(const TChannel& cDevChn)
    {
        devChn = cDevChn;
    }
    TChannel& GetDevChn()
    {
        return devChn;
    }
    const TChannel& GetDevChn() const
    {
        return devChn;
    }

    void SetPicID(const string& strPicID)
    {
        picID = strPicID;
    }
    string& GetPicID()
    {
        return picID;
    }
    const string& GetPicID() const
    {
        return picID;
    }

    void SetPackIndex(int nPackIndex)
    {
        packIndex = nPackIndex;
    }
    int GetPackIndex() const
    {
        return packIndex;
    }

};

class CPuPicDownloadRsp : public CEventRsp
{
public:
    CPuPicDownloadRsp()
    {
        event = OspExtEventDesc(PU_PIC_DOWNLOAD_RSP);
    }

    void Clear()
    {
        picData.Clear();
        CEventRsp::Clear();
    }

private:
    TBase64DataPack     picData;                            //图片数据                           

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetPicData(const TBase64DataPack& cPicData)
    {
        picData = cPicData;
    }
    TBase64DataPack& GetPicData()
    {
        return picData;
    }
    const TBase64DataPack& GetPicData() const
    {
        return picData;
    }

};

//前端图片查询
#define PU_PIC_CHAN_QUERY					(1)	    //通道号查询
#define PU_PIC_TIME_QUERY					(1<<1)	//时间查询
#define PU_PIC_EVENT_QUERY					(1<<2)	//事件查询
class CPuPicQryReq : public CQueryReqBase
{
public:
    CPuPicQryReq()
    {
        event = OspExtEventDesc(PU_PIC_QRY_REQ);
    }

    void Clear()
    {
        devChn.Clear();
        qryWay = 0;
        qryEvent.clear();
        timeRange.Clear();
        CEventReq::Clear();
    }

private:
    TChannel           devChn;                                  //设备通道
    int                qryWay;                                  //查询方式:第0位按通道-第1位按时间-第2位按事件
    string             qryEvent;                                //图片事件类型
    TTimeRange         timeRange;                               //时间段
public:
    //事务模板会使用
    const string& GetDevUri() const
    {
        return devChn.GetDevUri();
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetDevChn(const TChannel& cDevChn)
    {
        devChn = cDevChn;
    }
    TChannel& GetDevChn()
    {
        return devChn;
    }
    const TChannel& GetDevChn() const
    {
        return devChn;
    }

    void SetTimeRange(const TTimeRange& cTimeRange)
    {
        timeRange = cTimeRange;
    }
    TTimeRange& GetTimeRange()
    {
        return timeRange;
    }
    const TTimeRange& GetTimeRange() const
    {
        return timeRange;
    }

public:
    void SetQryWay(int nQryWay)
    {
        qryWay = nQryWay;
    }
    int GetQryWay() const
    {
        return qryWay;
    }

public:
    void SetQryEvent(const string& strQryEvent)
    {
        qryEvent = strQryEvent;
    }
    string& GetQryEvent()
    {
        return qryEvent;
    }
    const string& GetQryEvent() const
    {
        return qryEvent;
    }

};

class CPuPicQryRsp : public CQueryRspBase
{
public:
    CPuPicQryRsp()
    {
        event = OspExtEventDesc(PU_PIC_QRY_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPuPicQryNtfReq : public CQueryNtfReqBase
{
public:
    CPuPicQryNtfReq()
    {
        event = OspExtEventDesc(PU_PIC_QRY_NTF_REQ);
    }

    void Clear()
    {
        picInfoList.clear();
        CEventReq::Clear();
    }

private:
    CPuPicInfoList      picInfoList;                                //图片信息列表

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetPicInfoList(const CPuPicInfoList& cPicInfoList)
    {
        picInfoList = cPicInfoList;
    }
    CPuPicInfoList& GetPicInfoList()
    {
        return picInfoList;
    }
    const CPuPicInfoList& GetPicInfoList() const
    {
        return picInfoList;
    }

};

class CPuPicQryNtfRsp : public CEventRsp
{
public:
    CPuPicQryNtfRsp()
    {
        event = OspExtEventDesc(PU_PIC_QRY_NTF_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

//前端长字幕设置、获取
class CPuLongOsdSetReq : public CEventReq
{
public:
    CPuLongOsdSetReq()
    {
        event = OspExtEventDesc(PU_LONG_OSD_SET_REQ);
    }

    void Clear()
    {
        devChn.Clear();
        param.Clear();
    }

private:
    TChannel           devChn;                       //设备通道
    TLongOsdParam      param;                        //配置参数

public:
    //事务模板会使用
    const string& GetDevUri() const
    {
        return devChn.GetDevUri();
    }
public:
    void SetDevChn(const TChannel& cDevChn)
    {
        devChn = cDevChn;
    }
    TChannel& GetDevChn()
    {
        return devChn;
    }
    const TChannel& GetDevChn() const
    {
        return devChn;
    }

    void SetParam(const TLongOsdParam& cParam)
    {
        param = cParam;
    }
    TLongOsdParam& GetParam()
    {
        return param;
    }
    const TLongOsdParam& GetParam() const
    {
        return param;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPuLongOsdSetRsp : public CEventRsp
{
public:
    CPuLongOsdSetRsp()
    {
        event = OspExtEventDesc(PU_LONG_OSD_SET_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPuLongOsdGetReq : public CEventReq
{
public:
    CPuLongOsdGetReq()
    {
        event = OspExtEventDesc(PU_LONG_OSD_GET_REQ);
    }

    void Clear()
    {
        devChn.Clear();
    }

private:
    TChannel           devChn;                       //设备通道

public:
    //事务模板会使用
    const string& GetDevUri() const
    {
        return devChn.GetDevUri();
    }
public:
    void SetDevChn(const TChannel& cDevChn)
    {
        devChn = cDevChn;
    }
    TChannel& GetDevChn()
    {
        return devChn;
    }
    const TChannel& GetDevChn() const
    {
        return devChn;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPuLongOsdGetRsp : public CEventRsp
{
public:
    CPuLongOsdGetRsp()
    {
        event = OspExtEventDesc(PU_LONG_OSD_GET_RSP);
    }

    void Clear()
    {
        devChn.Clear();
        param.Clear();
    }

private:
    TChannel           devChn;                       //设备通道
    TLongOsdParam      param;                        //配置参数
public:
    void SetDevChn(const TChannel& cDevChn)
    {
        devChn = cDevChn;
    }
    TChannel& GetDevChn()
    {
        return devChn;
    }
    const TChannel& GetDevChn() const
    {
        return devChn;
    }

    void SetParam(const TLongOsdParam& cParam)
    {
        param = cParam;
    }
    TLongOsdParam& GetParam()
    {
        return param;
    }
    const TLongOsdParam& GetParam() const
    {
        return param;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

//{{{{{{{{{{{{{{{ add by Royan 20140418 多行字幕
//前端多行字幕设置、获取
class CPuMultiOsdSetReq : public CEventReq
{
public:
	CPuMultiOsdSetReq()
	{
		event = OspExtEventDesc(PU_MULTI_OSD_SET_REQ);
	}

	void Clear()
	{
		devChn.Clear();
		param.Clear();
	}

private:
	TChannel           devChn;                       //设备通道
	TMultiOsdParam     param;                        //配置参数

public:
	//事务模板会使用
	const string& GetDevUri() const
	{
		return devChn.GetDevUri();
	}
public:
	void SetDevChn(const TChannel& cDevChn)
	{
		devChn = cDevChn;
	}
	TChannel& GetDevChn()
	{
		return devChn;
	}
	const TChannel& GetDevChn() const
	{
		return devChn;
	}

	void SetParam(const TMultiOsdParam& cParam)
	{
		param = cParam;
	}
	TMultiOsdParam& GetParam()
	{
		return param;
	}
	const TMultiOsdParam& GetParam() const
	{
		return param;
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};


class CPuMultiOsdSetRsp : public CEventRsp
{
public:
	CPuMultiOsdSetRsp()
	{
		event = OspExtEventDesc(PU_MULTI_OSD_SET_RSP);
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class CPuMultiOsdGetReq : public CEventReq
{
public:
	CPuMultiOsdGetReq()
	{
		event = OspExtEventDesc(PU_MULTI_OSD_GET_REQ);
	}

	void Clear()
	{
		devChn.Clear();
	}

private:
	TChannel           devChn;                       //设备通道

public:
	//事务模板会使用
	const string& GetDevUri() const
	{
		return devChn.GetDevUri();
	}
public:
	void SetDevChn(const TChannel& cDevChn)
	{
		devChn = cDevChn;
	}
	TChannel& GetDevChn()
	{
		return devChn;
	}
	const TChannel& GetDevChn() const
	{
		return devChn;
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class CPuMultiOsdGetRsp : public CEventRsp
{
public:
	CPuMultiOsdGetRsp()
	{
		event = OspExtEventDesc(PU_MULTI_OSD_GET_RSP);
	}

	void Clear()
	{
		devChn.Clear();
		param.Clear();
	}

private:
	TChannel           devChn;                       //设备通道
	TMultiOsdParam     param;                        //配置参数
public:
	void SetDevChn(const TChannel& cDevChn)
	{
		devChn = cDevChn;
	}
	TChannel& GetDevChn()
	{
		return devChn;
	}
	const TChannel& GetDevChn() const
	{
		return devChn;
	}

	void SetParam(const TMultiOsdParam& cParam)
	{
		param = cParam;
	}
	TMultiOsdParam& GetParam()
	{
		return param;
	}
	const TMultiOsdParam& GetParam() const
	{
		return param;
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};
//}}}}}}}}}}}}}}} add by Royan 20140418 多行字幕

//传输台标
class CPuLogoBmpOsdTransmitReq : public CEventReq
{
public:
    CPuLogoBmpOsdTransmitReq()
    {
        event = OspExtEventDesc(PU_LOGO_BMPOSD_TRANSMIT_REQ);
    }

    void Clear()
    {
        devChn.Clear();
        param.Clear();
    }

private:
    TChannel           devChn;                       //设备通道
    TLogoBmpOsdParam   param;                        //配置参数

public:
    //事务模板会使用
    const string& GetDevUri() const
    {
        return devChn.GetDevUri();
    }
public:
    void SetDevChn(const TChannel& cDevChn)
    {
        devChn = cDevChn;
    }
    TChannel& GetDevChn()
    {
        return devChn;
    }
    const TChannel& GetDevChn() const
    {
        return devChn;
    }

    void SetParam(const TLogoBmpOsdParam& cParam)
    {
        param = cParam;
    }
    TLogoBmpOsdParam& GetParam()
    {
        return param;
    }
    const TLogoBmpOsdParam& GetParam() const
    {
        return param;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPuLogoBmpOsdTransmitRsp : public CEventRsp
{
public:
    CPuLogoBmpOsdTransmitRsp()
    {
        Clear();
        event = OspExtEventDesc(PU_LOGO_BMPOSD_TRANSMIT_RSP);
    }

    void Clear()
    {
        devChn.Clear();
        curPackIndex = 0;
        totalPack = 0;
    }

private:
    TChannel devChn;                       //设备通道  
    int curPackIndex;               	   //当前包的索引号,从1开始
    int totalPack;               		   //总包数
public:
    void SetDevChn(const TChannel& cDevChn)
    {
        devChn = cDevChn;
    }
    TChannel& GetDevChn()
    {
        return devChn;
    }
    const TChannel& GetDevChn() const
    {
        return devChn;
    }

    void SetCurPackIndex(int nCurPackIndex)
    {
        curPackIndex = nCurPackIndex;
    }
    int GetCurPackIndex() const
    {
        return curPackIndex;
    }

    void SetTotalPack(int nTotalPack)
    {
        totalPack = nTotalPack;
    }
    int GetTotalPack() const
    {
        return totalPack;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

//STUN服务器地址更新
class CStunUpdateReq: public CEventReq
{
public:
    CStunUpdateReq()
    {
        event = OspExtEventDesc(STUN_UPDATE_REQ);
    }

    void Clear()
    {
        stunAddr.Clear();
    }

private:
    TNetAddr    stunAddr;              

public:
    void SetStunAddr(const TNetAddr& cStunAddr)
    {
        stunAddr = cStunAddr;
    }
    TNetAddr& GetStunAddr()
    {
        return stunAddr;
    }
    const TNetAddr& GetStunAddr() const
    {
        return stunAddr;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CStunUpdateRsp : public CEventRsp
{
public:
    CStunUpdateRsp()
    {
        event = OspExtEventDesc(STUN_UPDATE_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

//{{{{{{{{{{{{{{{ add by Royan 20140509 智能告警主机
//异域告警
class CDomainAlarmReq: public CEventReq
{
public:
	CDomainAlarmReq()
	{
		event = OspExtEventDesc(DOMAIN_ALARM_REQ);
	}

	void Clear()
	{
		tAlarm.Clear();
	}

private:
	TAlarmInput tAlarm;              

public:
	void SetAlarm(const TAlarmInput& var)
	{
		tAlarm = var;
	}
	TAlarmInput& GetAlarm()
	{
		return tAlarm;
	}
	const TAlarmInput& GetAlarm() const
	{
		return tAlarm;
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class CDomainAlarmRsp : public CEventRsp
{
public:
	CDomainAlarmRsp()
	{
		event = OspExtEventDesc(DOMAIN_ALARM_RSP);
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};
//}}}}}}}}}}}}}}} add by Royan 20140509 智能告警主机

typedef map<string, string> TUaExtInfo;
#define UaExtInfoKey_UAType          ("UAType")                     // 值为：TSipUAType

//UA向proxy进行路由的登记和订阅
class CUaRouterRegReq : public CEventReq
{
public:
    CUaRouterRegReq()
    {
        event = OspExtEventDesc(UA_ROUTER_REG_REQ);
    }

    void Clear()
    {
        CEventReq::Clear();
        devUri.clear();
        netParamList.clear();
    }

private:
    string           devUri;                     //设备URI
    TNetParamList   netParamList;               //该设备的网络参数列表
    TUaExtInfo      extInfo;                    //sip注册信令的扩展数据
public:
    void SetDevUri(const string& strDevUri)
    {
        devUri = strDevUri;
    }
    string& GetDevUri()
    {
        return devUri;
    }
    const string& GetDevUri() const
    {
        return devUri;
    }

    void SetNetParamList(const TNetParamList& cNetParamList)
    {
        netParamList = cNetParamList;
    }
    TNetParamList& GetNetParamList()
    {
        return netParamList;
    }
    const TNetParamList& GetNetParamList() const
    {
        return netParamList;
    }
    
    string GetUAType() const
    {
        TUaExtInfo::const_iterator it = extInfo.find(UaExtInfoKey_UAType);
        if (extInfo.end() != it)
        {
            return it->second;
        }

        return string();
    }

    void SetUAType(const string &strUAType)
    {
        extInfo[UaExtInfoKey_UAType] = strUAType;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUaRouterRegRsp : public CEventRsp
{
public:
    CUaRouterRegRsp()
    {
        event = OspExtEventDesc(UA_ROUTER_REG_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUaRouterSsReq : public CEventReq
{
public:
    CUaRouterSsReq()
    {
        event = OspExtEventDesc(UA_ROUTER_SS_REQ);
    }

    void Clear()
    {
        CEventReq::Clear();
        devUri.clear();
        devUriList.clear();
    }

private:
    string       devUri;                     //订阅者设备URI
    CDevUriList  devUriList;                 //需要订阅的UA列表，空表示订阅所有UA
public:
    void SetDevUri(const string& strDevUri)
    {
        devUri = strDevUri;
    }
    string& GetDevUri()
    {
        return devUri;
    }
    const string& GetDevUri() const
    {
        return devUri;
    }

    void SetDevUriList(const CDevUriList& cDevUriList)
    {
        devUriList = cDevUriList;
    }
    CDevUriList& GetDevUriList()
    {
        return devUriList;
    }
    const CDevUriList& GetDevUriList() const
    {
        return devUriList;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUaRouterSsRsp : public CEventRsp
{
public:
    CUaRouterSsRsp()
    {
        event = OspExtEventDesc(UA_ROUTER_SS_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUaRouterNtfReq : public CEventReq
{
public:
    CUaRouterNtfReq()
    {
        event = OspExtEventDesc(UA_ROUTER_NTF_REQ);
    };

    void Clear()
    {
        opType.clear();
        uaRouteTable.clear();
    }

private:
    TNtfOpType opType;             
    CUaRouteTable  uaRouteTable;                  //UA的路由表
public:
    void SetUaRouteTable(const CUaRouteTable& cUaRouteTable)
    {
        uaRouteTable = cUaRouteTable;
    }
    CUaRouteTable& GetUaRouteTable()
    {
        return uaRouteTable;
    }
    const CUaRouteTable& GetUaRouteTable() const
    {
        return uaRouteTable;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetOpType(const TNtfOpType& strOpType)
    {
        opType = strOpType;
    }
    TNtfOpType& GetOpType()
    {
        return opType;
    }
    const TNtfOpType& GetOpType() const
    {
        return opType;
    }

};

typedef CNtfRsp CUaRouterNtfRsp;


//前端抓怕参数设置
class CPuSnapShotCfgGetReq : public CEventReq
{
public:
	CPuSnapShotCfgGetReq()
	{
		event = OspExtEventDesc(PU_SNAPSHOT_CFG_GET_REQ);
	}

	void Clear()
	{
		devChn.Clear();		
	}


private:
	TChannel           devChn; 

public:
	//事务模板会使用
	const string& GetDevUri() const
	{
		return devChn.GetDevUri();
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
public:
    void SetDevChn(const TChannel& cDevChn)
    {
        devChn = cDevChn;
    }
    TChannel& GetDevChn()
    {
        return devChn;
    }
    const TChannel& GetDevChn() const
    {
        return devChn;
    }

};


class CPuSnapShotCfgGetRsp : public CEventRsp
{
public:
	CPuSnapShotCfgGetRsp()
	{
		event = OspExtEventDesc(PU_SNAPSHOT_CFG_GET_RSP);
	}

	void Clear()
	{
		devChn.Clear();		
	}

private:
	TChannel  devChn; 
	TPuSnapShotCfg puSnapShotCfg;

public:
	//事务模板会使用
	const string& GetDevUri() const
	{
		return devChn.GetDevUri();
	}


public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
public:
    void SetDevChn(const TChannel& cDevChn)
    {
        devChn = cDevChn;
    }
    TChannel& GetDevChn()
    {
        return devChn;
    }
    const TChannel& GetDevChn() const
    {
        return devChn;
    }

    void SetPuSnapShotCfg(const TPuSnapShotCfg& cPuSnapShotCfg)
    {
        puSnapShotCfg = cPuSnapShotCfg;
    }
    TPuSnapShotCfg& GetPuSnapShotCfg()
    {
        return puSnapShotCfg;
    }
    const TPuSnapShotCfg& GetPuSnapShotCfg() const
    {
        return puSnapShotCfg;
    }

};


class CPuSnapShotCfgSetReq : public CEventReq
{
public:
	CPuSnapShotCfgSetReq()
	{
		event = OspExtEventDesc(PU_SNAPSHOT_CFG_SET_REQ);
	}

	void Clear()
	{
		devChn.Clear();		
	}

private:
	TChannel           devChn;
	TPuSnapShotCfg puSnapShotCfg;

public:
	//事务模板会使用
	const string& GetDevUri() const
	{
		return devChn.GetDevUri();
	}


public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
public:
    void SetDevChn(const TChannel& cDevChn)
    {
        devChn = cDevChn;
    }
    TChannel& GetDevChn()
    {
        return devChn;
    }
    const TChannel& GetDevChn() const
    {
        return devChn;
    }

    void SetPuSnapShotCfg(const TPuSnapShotCfg& cPuSnapShotCfg)
    {
        puSnapShotCfg = cPuSnapShotCfg;
    }
    TPuSnapShotCfg& GetPuSnapShotCfg()
    {
        return puSnapShotCfg;
    }
    const TPuSnapShotCfg& GetPuSnapShotCfg() const
    {
        return puSnapShotCfg;
    }

};


class CPuSnapShotCfgSetRsp : public CEventRsp
{
public:
	CPuSnapShotCfgSetRsp()
	{
		event = OspExtEventDesc(PU_SNAPSHOT_CFG_SET_RSP);
	}

	void Clear()
	{
		devChn.Clear();		
	}

private:
	TChannel  devChn; 
public:
	//事务模板会使用
	const string& GetDevUri() const
	{
		return devChn.GetDevUri();
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);


public:
    void SetDevChn(const TChannel& cDevChn)
    {
        devChn = cDevChn;
    }
    TChannel& GetDevChn()
    {
        return devChn;
    }
    const TChannel& GetDevChn() const
    {
        return devChn;
    }

};


//串口参数配置消息协议类
class CPuSerialParamCfgGetReq : public CEventReq
{
public:
	CPuSerialParamCfgGetReq()
	{
		event = OspExtEventDesc(PU_SERIALPARAM_CFG_GET_REQ);
	}

	void Clear()
	{
		devUri.clear();		
	}


private:
	string devUri;
	TPuSerialPortParam puSerPortParam;

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
public:
    void SetDevUri(const string& strDevUri)
    {
        devUri = strDevUri;
    }
    string& GetDevUri()
    {
        return devUri;
    }
    const string& GetDevUri() const
    {
        return devUri;
    }

    void SetPuSerPortParam(const TPuSerialPortParam& cPuSerPortParam)
    {
        puSerPortParam = cPuSerPortParam;
    }
    TPuSerialPortParam& GetPuSerPortParam()
    {
        return puSerPortParam;
    }
    const TPuSerialPortParam& GetPuSerPortParam() const
    {
        return puSerPortParam;
    }

};


class CPuSerialParamCfgGetRsp : public CEventRsp
{
public:
	CPuSerialParamCfgGetRsp()
	{
		event = OspExtEventDesc(PU_SERIALPARAM_CFG_GET_RSP);
	}

	void Clear()
	{
		devUri.clear();		
	}


private:
	string devUri;
	TPuSerialPortParam puSerPortParam;

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
 

public:
    void SetDevUri(const string& strDevUri)
    {
        devUri = strDevUri;
    }
    string& GetDevUri()
    {
        return devUri;
    }
    const string& GetDevUri() const
    {
        return devUri;
    }

    void SetPuSerPortParam(const TPuSerialPortParam& cPuSerPortParam)
    {
        puSerPortParam = cPuSerPortParam;
    }
    TPuSerialPortParam& GetPuSerPortParam()
    {
        return puSerPortParam;
    }
    const TPuSerialPortParam& GetPuSerPortParam() const
    {
        return puSerPortParam;
    }

};


class CPuSerialParamCfgSetReq : public CEventReq
{
public:
	CPuSerialParamCfgSetReq()
	{
		event = OspExtEventDesc(PU_SERIALPARAM_CFG_SET_REQ);
	}

	void Clear()
	{
		devUri.clear();		
	}


private:
	string devUri;
	TPuSerialPortParam puSerPortParam;

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
public:
    void SetDevUri(const string& strDevUri)
    {
        devUri = strDevUri;
    }
	string& GetDevUri()
	{
		return devUri;
	}
    const string& GetDevUri() const
    {
        return devUri;
    }

    void SetPuSerPortParam(const TPuSerialPortParam& cPuSerPortParam)
    {
        puSerPortParam = cPuSerPortParam;
    }
    TPuSerialPortParam& GetPuSerPortParam()
    {
        return puSerPortParam;
    }
    const TPuSerialPortParam& GetPuSerPortParam() const
    {
        return puSerPortParam;
    }

};


class CPuSerialParamCfgSetRsp : public CEventRsp
{
public:
	CPuSerialParamCfgSetRsp()
	{
		event = OspExtEventDesc(PU_SERIALPARAM_CFG_SET_RSP);
	}

	void Clear()
	{
		devUri.clear();		
	}


private:
	string devUri;

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);

    
public:
    void SetDevUri(const string& strDevUri)
    {
        devUri = strDevUri;
    }
    string& GetDevUri()
    {
        return devUri;
    }
    const string& GetDevUri() const
    {
        return devUri;
    }

};

//同步平台时间到pu
class CPuSyncTimeReq : public CEventReq
{
public:
	CPuSyncTimeReq()
	{
		event = OspExtEventDesc(PU_TIME_SYNC_REQ);
	}

public:
	void SetDateTime(const string& strDateTime)
	{
		dateTime = strDateTime;
	}
	string& GetDateTime()
	{
		return dateTime;
	}
	const string& GetDateTime() const
	{
		return dateTime;
	}
private:
	string dateTime;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPuSyncTimeRsp : public CEventRsp
{
public:
	CPuSyncTimeRsp()
	{
		event = OspExtEventDesc(PU_TIME_SYNC_RSP);
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class COffLineDevQryReq : public CQueryReqBase
{
public:
	COffLineDevQryReq()
	{
		event = OspExtEventDesc(CMU_PU_OFFLINEDEV_QRY_REQ);
	}

	void Clear()
	{
		CEventReq::Clear();
	}

private:
	string            domanName;                                        
public:
    void SetDomanName(const string& strDomanName)
    {
        domanName = strDomanName;
    }
    string& GetDomanName()
    {
        return domanName;
    }
    const string& GetDomanName() const
    {
        return domanName;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class COffLineDevQryRsp : public CQueryRspBase
{
public:
	COffLineDevQryRsp()
	{
		event = OspExtEventDesc(CMU_PU_OFFLINEDEV_QRY_RSP);
		onlineDevNum = 0;
		offlineDevNum = 0;
		idleDevNum = 0;
	}

	void Clear()
	{
		onlineDevNum = 0;
		offlineDevNum = 0;
		idleDevNum = 0;
		CEventRsp::Clear();
	}
public:
	void SetOnlineDevNum(int nOnlineDevNum)
	{
		onlineDevNum = nOnlineDevNum;
	}
	int GetOnlineDevNum()
	{
		return onlineDevNum;
	}

	void SetOfflineDevNum(int nOfflineDevNum)
	{
		offlineDevNum = nOfflineDevNum;
	}
	int GetOfflineDevNum()
	{
		return offlineDevNum;
	}

	void SetIdleDevNum(int nIdleDevNum)
	{
		idleDevNum = nIdleDevNum;
	}
	int GetIdleDevNum()
	{
		return idleDevNum;
	}

private:
	int onlineDevNum;        //在线设备数目
	int offlineDevNum;       //离线设备数目
	int idleDevNum;          //未启用设备数目
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class COffLineDevQryNtfReq : public CQueryNtfReqBase
{
public:
	COffLineDevQryNtfReq()
	{
		event = OspExtEventDesc(CMU_PU_OFFLINEDEV_QRY_NTF_REQ);
		devVideoSrcList.clear();
		onlineVideoSrcNum=0;
	}

	void Clear()
	{
		CEventReq::Clear();
	}

private:
	TDeviceVideoSrcIdList devVideoSrcList;
	int onlineVideoSrcNum;               
public:
    void SetDevVideoSrcList(const TDeviceVideoSrcIdList& cDevVideoSrcList)
    {
        devVideoSrcList = cDevVideoSrcList;
    }
    TDeviceVideoSrcIdList& GetDevVideoSrcList()
    {
        return devVideoSrcList;
    }
    const TDeviceVideoSrcIdList& GetDevVideoSrcList() const
    {
        return devVideoSrcList;
    }

	void SetOnlineVideoSrcNum(int nOnlineVideoSrcNum)
	{
		onlineVideoSrcNum = nOnlineVideoSrcNum;
	}
	int GetOnlineVideoSrcNum() const
	{
		return onlineVideoSrcNum;
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

};

class COffLineDevQryNtfRsp : public CEventRsp
{
public:
	COffLineDevQryNtfRsp()
	{
		event = OspExtEventDesc(CMU_PU_OFFLINEDEV_QRY_NTF_RSP);
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CModuleOnOffLogNtfReq : public CEventReq
{
public:
	CModuleOnOffLogNtfReq()
	{
		event = OspExtEventDesc(MODULE_ON_OFF_LOG_NTF_REQ);
	}

	void Clear()
	{
		//info.Clear();
		CEventReq::Clear();
	}
public:
	void SetInfo(const TDeviceLogInfo& cInfo)
	{
		info = cInfo;
	}
	TDeviceLogInfo& GetInfo()
	{
		return info;
	}
	const TDeviceLogInfo& GetInfo() const
	{
		return info;
	}

private:
	TDeviceLogInfo info;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CModuleOnOffLogNtfRsp : public CEventRsp
{
public:
	CModuleOnOffLogNtfRsp()
	{
		event = OspExtEventDesc(MODULE_ON_OFF_LOG_NTF_RSP);
	}

	void Clear()
	{
		CEventRsp::Clear();
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class CAlarmEventNtfReq : public CEventReq
{
public:
	CAlarmEventNtfReq()
	{
		event = OspExtEventDesc(ALARM_EVENT_NTF_REQ);
		chnNO = 0;
	}

	void Clear()
	{
		eventOccurTime.clear();
		eventType.clear();
		eventContent.clear();
		devUri.clear();
		devType.clear();
		chnNO = 0;
		CEventReq::Clear();
	}

public:
	const string &GetEventOccurTimeStr() const
	{
		return eventOccurTime;
	}
	string &GetEventOccurTimeStr()
	{
		return eventOccurTime;
	}
	const time_t GetEventOccurTime() const
	{
		CCMSTime cmsTime(eventOccurTime);
		return cmsTime.GetTime();
	}
	time_t GetEventOccurTime()
	{
		CCMSTime cmsTime(eventOccurTime);
		return cmsTime.GetTime();
	}
	void SetEventOccurTime(const string& strTime)
	{
		eventOccurTime = strTime;
	}
	void SetEventOccurTime(const time_t tTime)
	{
		CCMSTime cmsTime(tTime);
		eventOccurTime = cmsTime.GetXSDDateTime();
	}

	void SetEventType(const string& cType)
	{
		eventType = cType;
	}
	string& GetEventType()
	{
		return eventType;
	}
	const string& GetEventType() const
	{
		return eventType;
	}

	void SetEventContent(const string& cContent)
	{
		eventContent = cContent;
	}
	string& GetEventContent()
	{
		return eventContent;
	}
	const string& GetEventContent() const
	{
		return eventContent;
	}
	
	void SetDevUri(const string& cDevUri)
	{
		devUri = cDevUri;
	}
	const string& GetDevUri() const
	{
		return devUri;
	}
	string& GetDevUri()
	{
		return devUri;
	}
	
	void SetDevType(const TDevType& strDevType)
	{
		devType = strDevType;
	}
	TDevType& GetDevType()
	{
		return devType;
	}
	const TDevType& GetDevType() const
	{
		return devType;
	}

	void SetChnNO(int dwChnNO)
	{
		chnNO = dwChnNO;
	}
	int& GetChnNO()
	{
		return chnNO;
	}
	const int& GetChnNO() const
	{
		return chnNO;
	}

private:
	string eventOccurTime;
	string eventType;
	string eventContent;
	string devUri;          // 设备uri
	TDevType  devType;      //设备类型
	int    chnNO;
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class CAlarmEventNtfRsp : public CEventRsp
{
public:
	CAlarmEventNtfRsp()
	{
		event = OspExtEventDesc(ALARM_EVENT_NTF_RSP);
	}

	void Clear()
	{
		CEventRsp::Clear();
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class CMappingTableNtfReq : public CEventReq
{
public:
	CMappingTableNtfReq()
	{
		event = OspExtEventDesc(MAPPING_TABLE_NTF_REQ);
	}

	void Clear()
	{
		tMappingTable.clear();

		CEventReq::Clear();
	}

public:
	void SetMappingTable(const TMappingTable& tTable)
	{
		tMappingTable = tTable;
	}
	TMappingTable& GetMappingTable()
	{
		return tMappingTable;
	}
	const TMappingTable& GetMappingTable() const
	{
		return tMappingTable;
	}

private:
	TMappingTable tMappingTable;

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class CMappingTableNtfRsp : public CEventRsp
{
public:
	CMappingTableNtfRsp()
	{
		event = OspExtEventDesc(MAPPING_TABLE_NTF_RSP);
	}

	void Clear()
	{
		CEventRsp::Clear();
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class CDomainTableNtfReq : public CEventReq
{
public:
	CDomainTableNtfReq()
	{
		event = OspExtEventDesc(DOMAIN_TABLE_NTF_REQ);
	}

	void Clear()
	{
		tDomainTable.clear();

		CEventReq::Clear();
	}

public:
	void SetDomainTable(const TDomainTable& tTable)
	{
		tDomainTable = tTable;
	}
	TDomainTable& GetDomainTable()
	{
		return tDomainTable;
	}
	const TDomainTable& GetDomainTable() const
	{
		return tDomainTable;
	}

private:
	TDomainTable tDomainTable;

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class CDomainTableNtfRsp : public CEventRsp
{
public:
	CDomainTableNtfRsp()
	{
		event = OspExtEventDesc(DOMAIN_TABLE_NTF_RSP);
	}

	void Clear()
	{
		CEventRsp::Clear();
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class CGetAlarmTypesReq : public CEventReq
{
public:
	CGetAlarmTypesReq()
	{
		event = OspExtEventDesc(ALARM_TYPE_GET_REQ);
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class CGetAlarmTypesRsp : public CEventRsp
{
public:
	CGetAlarmTypesRsp()
	{
		event = OspExtEventDesc(ALARM_TYPE_GET_RSP);
	}

	//数据操作
public:
	void SetAlarmInputArray(const CAlarmTypeArray& ctAlarmTypeArray)
	{
		alarmTypeArray = ctAlarmTypeArray;
	}
	CAlarmTypeArray& GetAlarmTypeArray()
	{
		return alarmTypeArray;
	}
	const CAlarmTypeArray& GetAlarmTypeArray() const
	{
		return alarmTypeArray;
	}
private:
	CAlarmTypeArray alarmTypeArray;       //告警信息
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};


#endif //__CMU_PROTO_H__




