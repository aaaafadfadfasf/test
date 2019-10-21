/*****************************************************************************
模块名      : gbs_proto
文件名      : gbs_proto.h
相关文件    : 
文件实现功能: gbs_proto.h 定义gbs外部消息的消息体
作者        : 胡志云
版本        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2013/11/25  1.0        huzhiyun    创建
        
******************************************************************************/
#ifndef _GBS_PROTO_H_
#define _GBS_PROTO_H_

#include "cms/gbs/gbs_event.h"
#include "cms/gbs/gbs_struct.h"

class CGbsProgressNtfReq: public CEventReq
{
public:
    CGbsProgressNtfReq()
    {
        event = OspExtEventDesc(GBS_PROGRESS_NTF_REQ);
        progressPercent = 0;
    }

private:
    int progressPercent;// -1表示失败,正常范围 [0-100]                
    string progressTip; // 进度提示, 出错时为错误描述                
public:
    void SetProgressPercent(int nProgressPercent)
    {
        progressPercent = nProgressPercent;
    }
    int GetProgressPercent() const
    {
        return progressPercent;
    }

    void SetProgressTip(const string& strProgressTip)
    {
        progressTip = strProgressTip;
    }
    string& GetProgressTip()
    {
        return progressTip;
    }
    const string& GetProgressTip() const
    {
        return progressTip;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGbsConfSetReq: public CEventReq
{
public:
    CGbsConfSetReq()
    {
        event = OspExtEventDesc(GBS_CONF_SET_REQ);
    }
private:
    TGbsConf conf; 
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetConf(const TGbsConf& cConf)
    {
        conf = cConf;
    }
    TGbsConf& GetConf()
    {
        return conf;
    }
    const TGbsConf& GetConf() const
    {
        return conf;
    }

};

class CGbsConfSetRsp: public CEventRsp
{
public:
    CGbsConfSetRsp()
    {
        event = OspExtEventDesc(GBS_CONF_SET_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGbsConfGetReq: public CEventReq
{
public:
    CGbsConfGetReq()
    {
        event = OspExtEventDesc(GBS_CONF_GET_REQ);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGbsConfGetRsp: public CEventRsp
{
public:
    CGbsConfGetRsp()
    {
        event = OspExtEventDesc(GBS_CONF_GET_RSP);
        g800_connection_status = "disconnect";
    }

private:
    TGbsConf conf;
    string g800_connection_status; // g800连接状态 值为connect或disconnect                 

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetConf(const TGbsConf& cConf)
    {
        conf = cConf;
    }
    TGbsConf& GetConf()
    {
        return conf;
    }
    const TGbsConf& GetConf() const
    {
        return conf;
    }

public:
    void SetG800_connection_status(const string& strG800_connection_status)
    {
        g800_connection_status = strG800_connection_status;
    }
    string& GetG800_connection_status()
    {
        return g800_connection_status;
    }
    const string& GetG800_connection_status() const
    {
        return g800_connection_status;
    }

};

class CGbsAdvancedConfSetReq: public CEventReq
{
public:
    CGbsAdvancedConfSetReq()
    {
        event = OspExtEventDesc(GBS_ADVANCED_CONF_SET_REQ);
    }
private:
    TGbsAdvanceConf conf; 
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetConf(const TGbsAdvanceConf& cConf)
    {
        conf = cConf;
    }
    TGbsAdvanceConf& GetConf()
    {
        return conf;
    }
    const TGbsAdvanceConf& GetConf() const
    {
        return conf;
    }

};

class CGbsAdvancedConfSetRsp: public CEventRsp
{
public:
    CGbsAdvancedConfSetRsp()
    {
        event = OspExtEventDesc(GBS_ADVANCED_CONF_SET_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGbsAdvancedConfGetReq: public CEventReq
{
public:
    CGbsAdvancedConfGetReq()
    {
        event = OspExtEventDesc(GBS_ADVANCED_CONF_GET_REQ);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGbsAdvancedConfGetRsp: public CEventRsp
{
public:
    CGbsAdvancedConfGetRsp()
    {
        event = OspExtEventDesc(GBS_ADVANCED_CONF_GET_RSP);
    }

private:
    TGbsAdvanceConf conf;                

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetConf(const TGbsAdvanceConf& cConf)
    {
        conf = cConf;
    }
    TGbsAdvanceConf& GetConf()
    {
        return conf;
    }
    const TGbsAdvanceConf& GetConf() const
    {
        return conf;
    }
};

class CGbsOptionConfSetReq: public CEventReq
{
public:
	CGbsOptionConfSetReq()
	{
		event = OspExtEventDesc(GBS_OPTION_CONF_SET_REQ);
	}

	void SetSyncGbpuLongLati(bool sync)
	{
		optionField.SetOption("SyncGbpuLongLati");
		conf.syncGbpuLongLati=sync;
	}
	bool GetSyncGbpuLongLati(bool &sync)
	{
		if (optionField.HasOption("SyncGbpuLongLati"))
		{
			sync=conf.syncGbpuLongLati;
			return true;
		}
		return false;
	}

private:
	CModOptionField optionField; // 修改字段标识
	TGbsOptionConf conf;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGbsOptionConfSetRsp: public CEventRsp
{
public:
	CGbsOptionConfSetRsp()
	{
		event = OspExtEventDesc(GBS_OPTION_CONF_SET_RSP);
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGbsOptionConfGetReq: public CEventReq
{
public:
	CGbsOptionConfGetReq()
	{
		event = OspExtEventDesc(GBS_OPTION_CONF_GET_REQ);
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGbsOptionConfGetRsp: public CEventRsp
{
public:
	CGbsOptionConfGetRsp()
	{
		event = OspExtEventDesc(GBS_OPTION_CONF_GET_RSP);
	}

private:
	TGbsOptionConf conf;                
public:
    void SetConf(const TGbsOptionConf& cConf)
    {
        conf = cConf;
    }
    TGbsOptionConf& GetConf()
    {
        return conf;
    }
    const TGbsOptionConf& GetConf() const
    {
        return conf;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGbsKddomainAddReq: public CEventReq
{
public:
    CGbsKddomainAddReq()
    {
        event = OspExtEventDesc(GBS_KDDOMAIN_ADD_REQ);
    }

private:
    TGbsKdDomainConf conf; 
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetConf(const TGbsKdDomainConf& cConf)
    {
        conf = cConf;
    }
    TGbsKdDomainConf& GetConf()
    {
        return conf;
    }
    const TGbsKdDomainConf& GetConf() const
    {
        return conf;
    }

};

class CGbsKddomainAddRsp: public CEventRsp
{
public:
    CGbsKddomainAddRsp()
    {
        event = OspExtEventDesc(GBS_KDDOMAIN_ADD_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGbsKddomainDelReq: public CEventReq
{
public:
    CGbsKddomainDelReq()
    {
        event = OspExtEventDesc(GBS_KDDOMAIN_DEL_REQ);
    }

private:
    string kdDomainId; 
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetKdDomainId(const string& strKdDomainId)
    {
        kdDomainId = strKdDomainId;
    }
    string& GetKdDomainId()
    {
        return kdDomainId;
    }
    const string& GetKdDomainId() const
    {
        return kdDomainId;
    }

};

class CGbsKddomainDelRsp: public CEventRsp
{
public:
    CGbsKddomainDelRsp()
    {
        event = OspExtEventDesc(GBS_KDDOMAIN_DEL_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGbsKddomainQryReq: public CQueryReqBase
{
public:
    CGbsKddomainQryReq()
    {
        event = OspExtEventDesc(GBS_KDDOMAIN_QRY_REQ);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGbsKddomainQryRsp: public CQueryRspBase
{
public:
    CGbsKddomainQryRsp()
    {
        event = OspExtEventDesc(GBS_KDDOMAIN_QRY_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGbsKddomainQryNtfReq: public CQueryNtfReqBase
{
public:
    CGbsKddomainQryNtfReq()
    {
        event = OspExtEventDesc(GBS_KDDOMAIN_QRY_NTF_REQ);
    }

private:
    vector<TGbsKdDomainConf> confs; 
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetConfs(const vector<TGbsKdDomainConf>& cConfs)
    {
        confs = cConfs;
    }
    vector<TGbsKdDomainConf>& GetConfs()
    {
        return confs;
    }
    const vector<TGbsKdDomainConf>& GetConfs() const
    {
        return confs;
    }

};

class CGbsPuAddReq: public CEventReq
{
public:
    CGbsPuAddReq()
    {
        event = OspExtEventDesc(GBS_PU_ADD_REQ);
    }

private:
    TGbsGbPuConf conf;
	string gbEncoderType;                //设备GBID第11~13位，由入网设备者指定，默认为"112"
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetConf(const TGbsGbPuConf& cConf)
    {
        conf = cConf;
    }
    TGbsGbPuConf& GetConf()
    {
        return conf;
    }
    const TGbsGbPuConf& GetConf() const
    {
        return conf;
    }

public:
    void SetGbEncoderType(const string& strGbEncoderType)
    {
        gbEncoderType = strGbEncoderType;
    }
    string& GetGbEncoderType()
    {
        return gbEncoderType;
    }
    const string& GetGbEncoderType() const
    {
        return gbEncoderType;
    }

};

class CGbsPuAddRsp: public CEventRsp
{
public:
    CGbsPuAddRsp()
    {
        event = OspExtEventDesc(GBS_PU_ADD_RSP);
    }

private:
    string gbDevId;          // 为编码设备分配的编号
    vector<string> gbDevIds; // 为编码通道分配的编码器编号列表
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetGbDevId(const string& strGbDevId)
    {
        gbDevId = strGbDevId;
    }
    string& GetGbDevId()
    {
        return gbDevId;
    }
    const string& GetGbDevId() const
    {
        return gbDevId;
    }

    void SetGbDevIds(const vector<string>& cGbDevIds)
    {
        gbDevIds = cGbDevIds;
    }
    vector<string>& GetGbDevIds()
    {
        return gbDevIds;
    }
    const vector<string>& GetGbDevIds() const
    {
        return gbDevIds;
    }

};

class CGbsPuDelReq: public CEventReq
{
public:
    CGbsPuDelReq()
    {
        event = OspExtEventDesc(GBS_PU_DEL_REQ);
    }

private:
    string gbDevId; // 按照gbdevId删除
    string kdDevId; // 按照kddevId删除

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetGbDevId(const string& strGbDevId)
    {
        gbDevId = strGbDevId;
    }
    string& GetGbDevId()
    {
        return gbDevId;
    }
    const string& GetGbDevId() const
    {
        return gbDevId;
    }

public:
    void SetKdDevId(const string& strKdDevId)
    {
        kdDevId = strKdDevId;
    }
    string& GetKdDevId()
    {
        return kdDevId;
    }
    const string& GetKdDevId() const
    {
        return kdDevId;
    }

};

class CGbsPuDelRsp: public CEventRsp
{
public:
    CGbsPuDelRsp()
    {
        event = OspExtEventDesc(GBS_PU_DEL_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGbsPuModReq: public CEventReq
{
public:
    CGbsPuModReq()
    {
        event = OspExtEventDesc(GBS_PU_MOD_REQ);
        useFlag = true;
        videoSourceNum = 0;
        encodeChannelNum = 0;
        licenseNum = 0;
        localStorageCap = false;
		streamType = GBS_STREAM_TYPE_UDP;
    }

private:
    string gbDevId;          // 按照国标编号指定修改设备                
    string kdDevId;          // 按照科达编号指定修改设备                


    string devName;          // 设备名称                
    string devModel;         // 设备型号                
    string manufacturer;     // 设备厂商                
    string userName;         // 为空时使用PU编号                
    string userPassword;     // 为空时使用统一的登录密码                
    string ip;               // 设备IP地址                
    bool useFlag;            // 启用停用                

    int  videoSourceNum;     // 视频源数目                
    int  encodeChannelNum;   // 编码通道数目                
    int  licenseNum;         // license收取数目                
    bool localStorageCap;    // 本地存储能力
    vector<int> capLabel;    // 设备能力标签，参见TAS TCmsDeviceCapType
    
    string devAddr;          // 设备地址                
    string devDesc;          // 设备描述
    string kdmDevId;         // 1.0的kdmno                
    TStreamType streamType;                 // 码流类型：UDP--默认，TCP
    string gbNatId;                     //前端natId
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetGbDevId(const string& strGbDevId)
    {
        gbDevId = strGbDevId;
    }
    string& GetGbDevId()
    {
        return gbDevId;
    }
    const string& GetGbDevId() const
    {
        return gbDevId;
    }

    void SetKdDevId(const string& strKdDevId)
    {
        kdDevId = strKdDevId;
    }
    string& GetKdDevId()
    {
        return kdDevId;
    }
    const string& GetKdDevId() const
    {
        return kdDevId;
    }

    void SetDevName(const string& strDevName)
    {
        devName = strDevName;
    }
    string& GetDevName()
    {
        return devName;
    }
    const string& GetDevName() const
    {
        return devName;
    }

    void SetDevModel(const string& strDevModel)
    {
        devModel = strDevModel;
    }
    string& GetDevModel()
    {
        return devModel;
    }
    const string& GetDevModel() const
    {
        return devModel;
    }

    void SetManufacturer(const string& strManufacturer)
    {
        manufacturer = strManufacturer;
    }
    string& GetManufacturer()
    {
        return manufacturer;
    }
    const string& GetManufacturer() const
    {
        return manufacturer;
    }

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

    void SetUserPassword(const string& strUserPassword)
    {
        userPassword = strUserPassword;
    }
    string& GetUserPassword()
    {
        return userPassword;
    }
    const string& GetUserPassword() const
    {
        return userPassword;
    }

    void SetIp(const string& strIp)
    {
        ip = strIp;
    }
    string& GetIp()
    {
        return ip;
    }
    const string& GetIp() const
    {
        return ip;
    }

    void SetUseFlag(bool bUseFlag)
    {
        useFlag = bUseFlag;
    }
    bool GetUseFlag() const
    {
        return useFlag;
    }

    void SetVideoSourceNum(int nVideoSourceNum)
    {
        videoSourceNum = nVideoSourceNum;
    }
    int GetVideoSourceNum() const
    {
        return videoSourceNum;
    }

    void SetEncodeChannelNum(int nEncodeChannelNum)
    {
        encodeChannelNum = nEncodeChannelNum;
    }
    int GetEncodeChannelNum() const
    {
        return encodeChannelNum;
    }

    void SetLicenseNum(int nLicenseNum)
    {
        licenseNum = nLicenseNum;
    }
    int GetLicenseNum() const
    {
        return licenseNum;
    }

    void SetLocalStorageCap(bool bLocalStorageCap)
    {
        localStorageCap = bLocalStorageCap;
    }
    bool GetLocalStorageCap() const
    {
        return localStorageCap;
    }

    void SetCapLabel(const vector<int>& cCapLabel)
    {
        capLabel = cCapLabel;
    }
    vector<int>& GetCapLabel()
    {
        return capLabel;
    }
    const vector<int>& GetCapLabel() const
    {
        return capLabel;
    }


    void SetGbNatId(const string& strGbNatId)
    {
        gbNatId = strGbNatId;
    }
    const string& GetGbNatId() const
    {
        return gbNatId;
    }
    string& GetGbNatId()
    {
        return gbNatId;
    }

public:
    void SetDevAddr(const string& strDevAddr)
    {
        devAddr = strDevAddr;
    }
    string& GetDevAddr()
    {
        return devAddr;
    }
    const string& GetDevAddr() const
    {
        return devAddr;
    }

    void SetDevDesc(const string& strDevDesc)
    {
        devDesc = strDevDesc;
    }
    string& GetDevDesc()
    {
        return devDesc;
    }
    const string& GetDevDesc() const
    {
        return devDesc;
    }

public:
    void SetKdmDevId(const string& strKdmDevId)
    {
        kdmDevId = strKdmDevId;
    }
    string& GetKdmDevId()
    {
        return kdmDevId;
    }
    const string& GetKdmDevId() const
    {
        return kdmDevId;
    }

public:
    void SetStreamType(const TStreamType& strStreamType)
    {
        streamType = strStreamType;
    }
    TStreamType& GetStreamType()
    {
        return streamType;
    }
    const TStreamType& GetStreamType() const
    {
        return streamType;
    }

};

class CGbsPuModRsp: public CEventRsp
{
public:
    CGbsPuModRsp()
    {
        event = OspExtEventDesc(GBS_PU_MOD_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGbsPuQryReq: public CQueryReqBase
{
public:
    CGbsPuQryReq()
    {
        event = OspExtEventDesc(GBS_PU_QRY_REQ);
    }

private:
    string kdDevId;                                

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetKdDevId(const string& strKdDevId)
    {
        kdDevId = strKdDevId;
    }
    string& GetKdDevId()
    {
        return kdDevId;
    }
    const string& GetKdDevId() const
    {
        return kdDevId;
    }

};

class CGbsPuQryRsp: public CQueryRspBase
{
public:
    CGbsPuQryRsp()
    {
        event = OspExtEventDesc(GBS_PU_QRY_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGbsPuQryNtfReq: public CQueryNtfReqBase
{
public:
    CGbsPuQryNtfReq()
    {
        event = OspExtEventDesc(GBS_PU_QRY_NTF_REQ);
    }

private:
    vector<TGbsGbPuConf> confs; 
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetConfs(const vector<TGbsGbPuConf>& cConfs)
    {
        confs = cConfs;
    }
    vector<TGbsGbPuConf>& GetConfs()
    {
        return confs;
    }
    const vector<TGbsGbPuConf>& GetConfs() const
    {
        return confs;
    }

};


class CGbsPuSupportIdTypesGetReq:public CEventReq
{
public:
	CGbsPuSupportIdTypesGetReq()
	{
		event = OspExtEventDesc(GBS_PU_SUPPORT_IDTYPES_GET_REQ);
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGbsPuSupportIdTypesGetRsp:public CEventRsp
{
public:
	CGbsPuSupportIdTypesGetRsp()
	{
		event = OspExtEventDesc(GBS_PU_SUPPORT_IDTYPES_GET_RSP);
	}

private:
	string supportGbEncoderTypes;                //所有支持的类型编码组成字符串，2个类型编码之间以“|”隔开
public:
    void SetSupportGbEncoderTypes(const string& strSupportGbEncoderTypes)
    {
        supportGbEncoderTypes = strSupportGbEncoderTypes;
    }
    string& GetSupportGbEncoderTypes()
    {
        return supportGbEncoderTypes;
    }
    const string& GetSupportGbEncoderTypes() const
    {
        return supportGbEncoderTypes;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGbsDecodeAddReq:public CEventReq
{
public:
	CGbsDecodeAddReq()
	{
		event = OspExtEventDesc(GBS_DECODE_ADD_REQ);
	}
private:
    TGbsGbDecodeConf conf;               	
public:
    void SetConf(const TGbsGbDecodeConf& cConf)
    {
        conf = cConf;
    }
    TGbsGbDecodeConf& GetConf()
    {
        return conf;
    }
    const TGbsGbDecodeConf& GetConf() const
    {
        return conf;
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
class CGbsDecodeAddRsp:public CEventReq
{
public:
	CGbsDecodeAddRsp()
	{
		event = OspExtEventDesc(GBS_DECODE_ADD_RSP);
	}
private:
	string gbDevId;                         // 为编码设备分配的编号
	vector<string> gbDevIds;                // 为编码通道分配的编码器编号列表
public:
    void SetGbDevId(const string& strGbDevId)
    {
        gbDevId = strGbDevId;
    }
    string& GetGbDevId()
    {
        return gbDevId;
    }
    const string& GetGbDevId() const
    {
        return gbDevId;
    }
    void SetGbDevIds(const vector<string>& cGbDevIds)
    {
        gbDevIds = cGbDevIds;
    }
    vector<string>& GetGbDevIds()
    {
        return gbDevIds;
    }
    const vector<string>& GetGbDevIds() const
    {
        return gbDevIds;
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGbsDecodeDelReq: public CEventReq
{
public:
    CGbsDecodeDelReq()
    {
        event = OspExtEventDesc(GBS_DECODE_DEL_REQ);
    }

private:
    string gbDevId; // 按照gbdevId删除
    string kdDevId; // 按照kddevId删除

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetGbDevId(const string& strGbDevId)
    {
        gbDevId = strGbDevId;
    }
    string& GetGbDevId()
    {
        return gbDevId;
    }
    const string& GetGbDevId() const
    {
        return gbDevId;
    }

public:
    void SetKdDevId(const string& strKdDevId)
    {
        kdDevId = strKdDevId;
    }
    string& GetKdDevId()
    {
        return kdDevId;
    }
    const string& GetKdDevId() const
    {
        return kdDevId;
    }

};

class CGbsDecodeDelRsp: public CEventRsp
{
public:
    CGbsDecodeDelRsp()
    {
        event = OspExtEventDesc(GBS_DECODE_DEL_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGbsDecodeModReq: public CEventReq
{
public:
    CGbsDecodeModReq()
    {
        event = OspExtEventDesc(GBS_DECODE_MOD_REQ);
        useFlag = true;
        decodeChnNum = 0;
        decodeOutputChnNum = 0;
    }

private:
    string gbDevId;          // 按照国标编号指定修改设备                
    string kdDevId;          // 按照科达编号指定修改设备                


    string devName;          // 设备名称                
    string devModel;         // 设备型号                
    string manufacturer;     // 设备厂商                
    string userName;         // 为空时使用PU编号                
    string userPassword;     // 为空时使用统一的登录密码                
    string ip;               // 设备IP地址                
    bool useFlag;            // 启用停用                

    int  decodeChnNum;     // 最大解码通道数                
    int  decodeOutputChnNum;   // 解码输出通道数                
    vector<int> capLabel;    // 设备能力标签，参见TAS TCmsDeviceCapType
    
    string devAddr;          // 设备地址                
    string devDesc;          // 设备描述
    string kdmDevId;         // 1.0的kdmno                
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetGbDevId(const string& strGbDevId)
    {
        gbDevId = strGbDevId;
    }
    string& GetGbDevId()
    {
        return gbDevId;
    }
    const string& GetGbDevId() const
    {
        return gbDevId;
    }

    void SetKdDevId(const string& strKdDevId)
    {
        kdDevId = strKdDevId;
    }
    string& GetKdDevId()
    {
        return kdDevId;
    }
    const string& GetKdDevId() const
    {
        return kdDevId;
    }

    void SetDevName(const string& strDevName)
    {
        devName = strDevName;
    }
    string& GetDevName()
    {
        return devName;
    }
    const string& GetDevName() const
    {
        return devName;
    }

    void SetDevModel(const string& strDevModel)
    {
        devModel = strDevModel;
    }
    string& GetDevModel()
    {
        return devModel;
    }
    const string& GetDevModel() const
    {
        return devModel;
    }

    void SetManufacturer(const string& strManufacturer)
    {
        manufacturer = strManufacturer;
    }
    string& GetManufacturer()
    {
        return manufacturer;
    }
    const string& GetManufacturer() const
    {
        return manufacturer;
    }

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

    void SetUserPassword(const string& strUserPassword)
    {
        userPassword = strUserPassword;
    }
    string& GetUserPassword()
    {
        return userPassword;
    }
    const string& GetUserPassword() const
    {
        return userPassword;
    }

    void SetIp(const string& strIp)
    {
        ip = strIp;
    }
    string& GetIp()
    {
        return ip;
    }
    const string& GetIp() const
    {
        return ip;
    }

    void SetUseFlag(bool bUseFlag)
    {
        useFlag = bUseFlag;
    }
    bool GetUseFlag() const
    {
        return useFlag;
    }

    void SetDecodeChnNum(int nDecodeChnNum)
    {
        decodeChnNum = nDecodeChnNum;
    }
    int GetDecodeChnNum() const
    {
        return decodeChnNum;
    }

    void SetDecodeOutputChnNum(int nDecodeOutputChnNum)
    {
        decodeOutputChnNum = nDecodeOutputChnNum;
    }
    int GetDecodeOutputChnNum() const
    {
        return decodeOutputChnNum;
    }

    void SetCapLabel(const vector<int>& cCapLabel)
    {
        capLabel = cCapLabel;
    }
    vector<int>& GetCapLabel()
    {
        return capLabel;
    }
    const vector<int>& GetCapLabel() const
    {
        return capLabel;
    }

    

public:
    void SetDevAddr(const string& strDevAddr)
    {
        devAddr = strDevAddr;
    }
    string& GetDevAddr()
    {
        return devAddr;
    }
    const string& GetDevAddr() const
    {
        return devAddr;
    }

    void SetDevDesc(const string& strDevDesc)
    {
        devDesc = strDevDesc;
    }
    string& GetDevDesc()
    {
        return devDesc;
    }
    const string& GetDevDesc() const
    {
        return devDesc;
    }

public:
    void SetKdmDevId(const string& strKdmDevId)
    {
        kdmDevId = strKdmDevId;
    }
    string& GetKdmDevId()
    {
        return kdmDevId;
    }
    const string& GetKdmDevId() const
    {
        return kdmDevId;
    }

};


class CGbsDecodeModRsp: public CEventRsp
{
public:
    CGbsDecodeModRsp()
    {
        event = OspExtEventDesc(GBS_DECODE_MOD_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};



class CGbsDecodeQryReq: public CQueryReqBase
{
public:
    CGbsDecodeQryReq()
    {
        event = OspExtEventDesc(GBS_DECODE_QRY_REQ);
    }

private:
    string kdDevId;                                

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetKdDevId(const string& strKdDevId)
    {
        kdDevId = strKdDevId;
    }
    string& GetKdDevId()
    {
        return kdDevId;
    }
    const string& GetKdDevId() const
    {
        return kdDevId;
    }
};

class CGbsDecodeQryRsp: public CQueryRspBase
{
public:
    CGbsDecodeQryRsp()
    {
        event = OspExtEventDesc(GBS_DECODE_QRY_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGbsDecodeQryNtfReq: public CQueryNtfReqBase
{
public:
    CGbsDecodeQryNtfReq()
    {
        event = OspExtEventDesc(GBS_DECODE_QRY_NTF_REQ);
    }

private:
    vector<TGbsGbDecodeConf> confs; 
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetConfs(const vector<TGbsGbDecodeConf>& cConfs)
    {
        confs = cConfs;
    }
    vector<TGbsGbDecodeConf>& GetConfs()
    {
        return confs;
    }
    const vector<TGbsGbDecodeConf>& GetConfs() const
    {
        return confs;
    }

};
class CGbsGbPeripheralAddReq: public CEventReq
{
public:
	CGbsGbPeripheralAddReq()
	{
		event = OspExtEventDesc(GBS_GBPERIPHERAL_ADD_REQ);
	}

private:
	TGbsGbPeripheralConf conf;
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
public:
	void SetConf(const TGbsGbPeripheralConf& cConf)
	{
		conf = cConf;
	}
	TGbsGbPeripheralConf& GetConf()
	{
		return conf;
	}
	const TGbsGbPeripheralConf& GetConf() const
	{
		return conf;
	}

};

class CGbsGbPeripheralAddRsp: public CEventRsp
{
public:
	CGbsGbPeripheralAddRsp()
	{
		event = OspExtEventDesc(GBS_GBPERIPHERAL_ADD_RSP);
	}
private:
	string gbDevId;  //Gbs为国标外设分配的国标Id
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
public:
	void SetGbDevId(const string& strGbDevId)
	{
		gbDevId = strGbDevId;
	}
	string& GetGbDevId()
	{
		return gbDevId;
	}
	const string& GetGbDevId() const
	{
		return gbDevId;
	}
};

class CGbsGbPeripheralDelReq: public CEventReq
{
public:
	CGbsGbPeripheralDelReq()
	{
		event = OspExtEventDesc(GBS_GBPERIPHERAL_DEL_REQ);
	}

private:
	string gbDevId;
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
public:
	void SetGbDevId(const string& strGbDevId)
	{
		gbDevId = strGbDevId;
	}
	string& GetGbDevId()
	{
		return gbDevId;
	}
	const string& GetGbDevId() const
	{
		return gbDevId;
	}
};

class CGbsGbPeripheralDelRsp: public CEventRsp
{
public:
	CGbsGbPeripheralDelRsp()
	{
		event = OspExtEventDesc(GBS_GBPERIPHERAL_DEL_RSP);
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class CGbsGbPeripheralDelAck: public CEventReq
{
public:
	CGbsGbPeripheralDelAck()
	{
		event = OspExtEventDesc(GBS_GBPERIPHERAL_DEL_ACK);
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class CGbsGbPeripheralModReq: public CEventReq
{
public:
	CGbsGbPeripheralModReq()
	{
		event = OspExtEventDesc(GBS_GBPERIPHERAL_MOD_REQ);
	}

private:
	string gbDevId;                
	string newName;
	string userName;               
	string userPassword;
	string ip;
public:
	void SetGbDevId(const string& strGbDevId)
	{
		gbDevId = strGbDevId;
	}
	string& GetGbDevId()
	{
		return gbDevId;
	}
	const string& GetGbDevId() const
	{
		return gbDevId;
	}

	void SetNewName(const string& strNewName)
	{
		newName = strNewName;
	}
	string& GetNewName()
	{
		return newName;
	}
	const string& GetNewName() const
	{
		return newName;
	}
	
	void SetIp(const string& strIp)
	{
		ip = strIp;
	}
	string& GetIp()
	{
		return ip;
	}
	const string& GetIp() const
	{
		return ip;
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
public:
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

	void SetUserPassword(const string& strUserPassword)
	{
		userPassword = strUserPassword;
	}
	string& GetUserPassword()
	{
		return userPassword;
	}
	const string& GetUserPassword() const
	{
		return userPassword;
	}
};

class CGbsGbPeripheralModRsp: public CEventRsp
{
public:
	CGbsGbPeripheralModRsp()
	{
		event = OspExtEventDesc(GBS_GBPERIPHERAL_MOD_RSP);
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class CGbsGbPeripheralQryReq: public CQueryReqBase
{
public:
	CGbsGbPeripheralQryReq()
	{
		event = OspExtEventDesc(GBS_GBPERIPHERAL_QRY_REQ);
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class CGbsGbPeripheralQryRsp: public CQueryRspBase
{
public:
	CGbsGbPeripheralQryRsp()
	{
		event = OspExtEventDesc(GBS_GBPERIPHERAL_QRY_RSP);
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class CGbsGbPeripheralQryNtfReq: public CQueryNtfReqBase
{
public:
	CGbsGbPeripheralQryNtfReq()
	{
		event = OspExtEventDesc(GBS_GBPERIPHERAL_QRY_NTF_REQ);
	}

private:
	vector<TGbsGbPeripheralConf> confs;
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
public:
	void SetConfs(const vector<TGbsGbPeripheralConf>& cConfs)
	{
		confs = cConfs;
	}
	vector<TGbsGbPeripheralConf>& GetConfs()
	{
		return confs;
	}
	const vector<TGbsGbPeripheralConf>& GetConfs() const
	{
		return confs;
	}
};

class CGbsGbdomainChildAddReq: public CEventReq
{
public:
    CGbsGbdomainChildAddReq()
    {
        event = OspExtEventDesc(GBS_GBDOMAIN_CHILD_ADD_REQ);
    }

private:
    TGbsGbDomainChildConf conf;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetConf(const TGbsGbDomainChildConf& cConf)
    {
        conf = cConf;
    }
    TGbsGbDomainChildConf& GetConf()
    {
        return conf;
    }
    const TGbsGbDomainChildConf& GetConf() const
    {
        return conf;
    }
};

class CGbsGbdomainChildAddRsp: public CEventRsp
{
public:
    CGbsGbdomainChildAddRsp()
    {
        event = OspExtEventDesc(GBS_GBDOMAIN_CHILD_ADD_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGbsGbdomainChildDelReq: public CEventReq
{
public:
    CGbsGbdomainChildDelReq()
    {
        event = OspExtEventDesc(GBS_GBDOMAIN_CHILD_DEL_REQ);
    }

private:
    string gbDevId;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetGbDevId(const string& strGbDevId)
    {
        gbDevId = strGbDevId;
    }
    string& GetGbDevId()
    {
        return gbDevId;
    }
    const string& GetGbDevId() const
    {
        return gbDevId;
    }

};

class CGbsGbdomainChildDelAck: public CEventReq
{
public:
    CGbsGbdomainChildDelAck()
    {
        event = OspExtEventDesc(GBS_GBDOMAIN_CHILD_DEL_ACK);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGbsGbdomainChildDelRsp: public CEventRsp
{
public:
    CGbsGbdomainChildDelRsp()
    {
        event = OspExtEventDesc(GBS_GBDOMAIN_CHILD_DEL_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGbsGbdomainChildDelSyncReq: public CEventReq
{
public:
    CGbsGbdomainChildDelSyncReq()
    {
        event = OspExtEventDesc(GBS_GBDOMAIN_CHILD_DEL_SYNC_REQ);
    }

private:
    string gbDevId; 
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetGbDevId(const string& strGbDevId)
    {
        gbDevId = strGbDevId;
    }
    string& GetGbDevId()
    {
        return gbDevId;
    }
    const string& GetGbDevId() const
    {
        return gbDevId;
    }

};

class CGbsGbdomainChildDelSyncRsp: public CEventRsp
{
public:
    CGbsGbdomainChildDelSyncRsp()
    {
        event = OspExtEventDesc(GBS_GBDOMAIN_CHILD_DEL_SYNC_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGbsGbdomainChildModReq: public CEventReq
{
public:
    CGbsGbdomainChildModReq()
    {
        event = OspExtEventDesc(GBS_GBDOMAIN_CHILD_MOD_REQ);
		streamType = GBS_STREAM_TYPE_UDP;
		qryCatalogOnReg = "TRUE";
    }

private:
    string gbDevId;                
    string newName;
    string userName;               
    string userPassword;                
    string gbNatId;
	TStreamType streamType;               
	string qryCatalogOnReg;                //注册成功后是否自动查询目录，"TRUE"--自动查询，"FALSE"--不自动查询
public:
    void SetGbDevId(const string& strGbDevId)
    {
        gbDevId = strGbDevId;
    }
    string& GetGbDevId()
    {
        return gbDevId;
    }
    const string& GetGbDevId() const
    {
        return gbDevId;
    }

    void SetGbNatId(const string& strGbNatId)
    {
        gbNatId = strGbNatId;
    }
    string& GetGbNatId()
    {
        return gbNatId;
    }
    const string& GetGbNatId() const
    {
        return gbNatId;
    }

    void SetNewName(const string& strNewName)
    {
        newName = strNewName;
    }
    string& GetNewName()
    {
        return newName;
    }
    const string& GetNewName() const
    {
        return newName;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
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

    void SetUserPassword(const string& strUserPassword)
    {
        userPassword = strUserPassword;
    }
    string& GetUserPassword()
    {
        return userPassword;
    }
    const string& GetUserPassword() const
    {
        return userPassword;
    }

public:
    void SetStreamType(const TStreamType& strStreamType)
    {
        streamType = strStreamType;
    }
    TStreamType& GetStreamType()
    {
        return streamType;
    }
    const TStreamType& GetStreamType() const
    {
        return streamType;
    }

public:
    void SetQryCatalogOnReg(const string& strQryCatalogOnReg)
    {
        qryCatalogOnReg = strQryCatalogOnReg;
    }
    string& GetQryCatalogOnReg()
    {
        return qryCatalogOnReg;
    }
    const string& GetQryCatalogOnReg() const
    {
        return qryCatalogOnReg;
    }
};

class CGbsGbdomainChildModRsp: public CEventRsp
{
public:
    CGbsGbdomainChildModRsp()
    {
        event = OspExtEventDesc(GBS_GBDOMAIN_CHILD_MOD_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGbsGbdomainChildQryReq: public CQueryReqBase
{
public:
    CGbsGbdomainChildQryReq()
    {
        event = OspExtEventDesc(GBS_GBDOMAIN_CHILD_QRY_REQ);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGbsGbdomainChildQryRsp: public CQueryRspBase
{
public:
    CGbsGbdomainChildQryRsp()
    {
        event = OspExtEventDesc(GBS_GBDOMAIN_CHILD_QRY_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGbsGbdomainChildQryNtfReq: public CQueryNtfReqBase
{
public:
    CGbsGbdomainChildQryNtfReq()
    {
        event = OspExtEventDesc(GBS_GBDOMAIN_CHILD_QRY_NTF_REQ);
    }

private:
    vector<TGbsGbDomainChildConf> confs;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetConfs(const vector<TGbsGbDomainChildConf>& cConfs)
    {
        confs = cConfs;
    }
    vector<TGbsGbDomainChildConf>& GetConfs()
    {
        return confs;
    }
    const vector<TGbsGbDomainChildConf>& GetConfs() const
    {
        return confs;
    }

};



class CGbsGbdomainParentAddReq: public CEventReq
{
public:
    CGbsGbdomainParentAddReq()
    {
        event = OspExtEventDesc(GBS_GBDOMAIN_PARENT_ADD_REQ);
    }

private:
    TGbsGbDomainParentConf conf;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetConf(const TGbsGbDomainParentConf& cConf)
    {
        conf = cConf;
    }
    TGbsGbDomainParentConf& GetConf()
    {
        return conf;
    }
    const TGbsGbDomainParentConf& GetConf() const
    {
        return conf;
    }

};

class CGbsGbdomainParentAddRsp: public CEventRsp
{
public:
    CGbsGbdomainParentAddRsp()
    {
        event = OspExtEventDesc(GBS_GBDOMAIN_PARENT_ADD_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGbsGbdomainParentDelReq: public CEventReq
{
public:
    CGbsGbdomainParentDelReq()
    {
        event = OspExtEventDesc(GBS_GBDOMAIN_PARENT_DEL_REQ);
    }

private:
    string gbDevId;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetGbDevId(const string& strGbDevId)
    {
        gbDevId = strGbDevId;
    }
    string& GetGbDevId()
    {
        return gbDevId;
    }
    const string& GetGbDevId() const
    {
        return gbDevId;
    }

};

class CGbsGbdomainParentDelRsp: public CEventRsp
{
public:
    CGbsGbdomainParentDelRsp()
    {
        event = OspExtEventDesc(GBS_GBDOMAIN_PARENT_DEL_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGbsGbdomainParentModReq: public CEventReq
{
public:
    CGbsGbdomainParentModReq()
    {
        event = OspExtEventDesc(GBS_GBDOMAIN_PARENT_MOD_REQ);
        newPort = 0;
		streamType = GBS_STREAM_TYPE_UDP;
    }

private:
    string gbDevId;                
    string newName;
    string newUserName;               
    string newUserPassword;                
    string newIp;                
    u32 newPort;                
    string gbNatId;
	TStreamType streamType;
	string fixGbsIp;               
public:
    void SetGbDevId(const string& strGbDevId)
    {
        gbDevId = strGbDevId;
    }
    string& GetGbDevId()
    {
        return gbDevId;
    }
    const string& GetGbDevId() const
    {
        return gbDevId;
    }

    void SetGbNatId(const string& strGbNatId)
    {
        gbNatId = strGbNatId;
    }
    string& GetGbNatId()
    {
        return gbNatId;
    }
    const string& GetGbNatId() const
    {
        return gbNatId;
    }

    void SetNewName(const string& strNewName)
    {
        newName = strNewName;
    }
    string& GetNewName()
    {
        return newName;
    }
    const string& GetNewName() const
    {
        return newName;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetNewUserName(const string& strNewUserName)
    {
        newUserName = strNewUserName;
    }
    string& GetNewUserName()
    {
        return newUserName;
    }
    const string& GetNewUserName() const
    {
        return newUserName;
    }

    void SetNewUserPassword(const string& strNewUserPassword)
    {
        newUserPassword = strNewUserPassword;
    }
    string& GetNewUserPassword()
    {
        return newUserPassword;
    }
    const string& GetNewUserPassword() const
    {
        return newUserPassword;
    }

    void SetNewIp(const string& strNewIp)
    {
        newIp = strNewIp;
    }
    string& GetNewIp()
    {
        return newIp;
    }
    const string& GetNewIp() const
    {
        return newIp;
    }

    void SetNewPort(u32 nNewPort)
    {
        newPort = nNewPort;
    }
    u32 GetNewPort() const
    {
        return newPort;
    }

public:
    void SetStreamType(const TStreamType& strStreamType)
    {
        streamType = strStreamType;
    }
    TStreamType& GetStreamType()
    {
        return streamType;
    }
    const TStreamType& GetStreamType() const
    {
        return streamType;
    }

public:
    void SetFixGbsIp(const string& strFixGbsIp)
    {
        fixGbsIp = strFixGbsIp;
    }
    string& GetFixGbsIp()
    {
        return fixGbsIp;
    }
    const string& GetFixGbsIp() const
    {
        return fixGbsIp;
    }

};

class CGbsGbdomainParentModRsp: public CEventRsp
{
public:
    CGbsGbdomainParentModRsp()
    {
        event = OspExtEventDesc(GBS_GBDOMAIN_PARENT_MOD_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGbsGbdomainParentQryReq: public CQueryReqBase
{
public:
    CGbsGbdomainParentQryReq()
    {
        event = OspExtEventDesc(GBS_GBDOMAIN_PARENT_QRY_REQ);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGbsGbdomainParentQryRsp: public CQueryRspBase
{
public:
    CGbsGbdomainParentQryRsp()
    {
        event = OspExtEventDesc(GBS_GBDOMAIN_PARENT_QRY_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGbsGbdomainParentQryNtfReq: public CQueryNtfReqBase
{
public:
    CGbsGbdomainParentQryNtfReq()
    {
        event = OspExtEventDesc(GBS_GBDOMAIN_PARENT_QRY_NTF_REQ);
    }

private:
    vector<TGbsGbDomainParentConf> confs;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetConfs(const vector<TGbsGbDomainParentConf>& cConfs)
    {
        confs = cConfs;
    }
    vector<TGbsGbDomainParentConf>& GetConfs()
    {
        return confs;
    }
    const vector<TGbsGbDomainParentConf>& GetConfs() const
    {
        return confs;
    }

};

class CGbsGbdevQryReq: public CQueryReqBase
{
public:
    CGbsGbdevQryReq()
    {
        event = OspExtEventDesc(GBS_GBDEV_QRY_REQ);
    }

    void SetGbDomainId(const string& strGbDomainId)
    {
        gbDomainId = strGbDomainId;
    }
    string& GetGbDomainId()
    {
        return gbDomainId;
    }
    const string& GetGbDomainId() const
    {
        return gbDomainId;
    }
                  
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

private:
    string gbDomainId;  // 返回指定域下面的设备，同时会递归查询它下面域的设备
};

class CGbsGbdevQryRsp: public CQueryRspBase
{
public:
    CGbsGbdevQryRsp()
    {
        event = OspExtEventDesc(GBS_GBDEV_QRY_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGbsGbdevQryNtfReq: public CQueryNtfReqBase
{
public:
    CGbsGbdevQryNtfReq()
    {
        event = OspExtEventDesc(GBS_GBDEV_QRY_NTF_REQ);
    }
    void SetDevs(const vector<TGbsGbdevInfo>& cDevs)
    {
        devs = cDevs;
    }
    vector<TGbsGbdevInfo>& GetDevs()
    {
        return devs;
    }
    const vector<TGbsGbdevInfo>& GetDevs() const
    {
        return devs;
    }

private:
    vector<TGbsGbdevInfo> devs;

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGbsGbPuQryReq: public CQueryReqBase
{
public:
	CGbsGbPuQryReq()
	{
		event = OspExtEventDesc(GBS_GBPU_QRY_REQ);
	}
	

private:
	string GbDevId;                
public:
	void SetGbDevId(const string& strId)
	{
		GbDevId = strId;
	}
	string& GetGbDevId()
	{
		return GbDevId;
	}
	const string& GetGbDevId() const
	{
		return GbDevId;
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class CGbsGbPuQryRsp: public CQueryRspBase
{
public:
	CGbsGbPuQryRsp()
	{
		event = OspExtEventDesc(GBS_GBPU_QRY_RSP);
	}
private:
	string DevGbId;               
	string DevKdid;               
	string DevName;               
	string RegName;               
	string RegPassword;               
	string Manufacturer;               
	string Model;
	int    VidSrcNum;
    int ChannelNum;
	string Status;               
public:
	void SetDevGbId(const string& strId)
	{
		DevGbId = strId;
	}
	string& GetDevGbId()
	{
		return DevGbId;
	}
	const string& GetDevGbId() const
	{
		return DevGbId;
	}
	
	void SetDevKdId(const string& strKdId)
	{
		DevKdid = strKdId;
	}
	string& GetDevKdId()
	{
		return DevKdid;
	}
	const string& GetDevKdId() const
	{
		return DevKdid;
	}

	void SetDevName(const string& strDevName)
	{
		DevName = strDevName;
	}
	string& GetDevName()
	{
		return DevName;
	}
	const string& GetDevName() const
	{
		return DevName;
	}

	void SetRegName(const string& strRegName)
	{
		RegName = strRegName;
	}
	string& GetRegName()
	{
		return RegName;
	}
	const string& GetRegName() const
	{
		return RegName;
	}

	void SetRegPassword(const string& strRegPassword)
	{
		RegPassword = strRegPassword;
	}
	string& GetRegPassword()
	{
		return RegPassword;
	}
	const string& GetRegPassword() const
	{
		return RegPassword;
	}

	void SetManufacturer(const string& strMan)
	{
		Manufacturer = strMan;
	}
	string& GetManufacturer()
	{
		return Manufacturer;
	}
	const string& GetManufacturer() const
	{
		return Manufacturer;
	}

	void SetModel(const string& strModel)
	{
		Model = strModel;
	}
	string& GetModel()
	{
		return Model;
	}
	const string& GetModel() const
	{
		return Model;
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
public:
    void SetVidSrcNum(int nVidSrcNum)
    {
        VidSrcNum = nVidSrcNum;
    }
    int GetVidSrcNum() const
    {
        return VidSrcNum;
    }

public:
    void SetStatus(const string& strStatus)
    {
        Status = strStatus;
    }
    string& GetStatus()
    {
        return Status;
    }
    const string& GetStatus() const
    {
        return Status;
    }
public:
    void SetChannelNum(int nChannelNum)
    {
        ChannelNum = nChannelNum;
    }
    int GetChannelNum() const
    {
        return ChannelNum;
    }

};

class CGbsDiscGbPuReq:public CEventReq
{
public:
	CGbsDiscGbPuReq()
	{
		event = OspExtEventDesc(GBS_DISC_GBPU_REQ);
	}
private:
	string strDevGbid;               
public:
    void SetStrDevGbid(const string& strStrDevGbid)
    {
        strDevGbid = strStrDevGbid;
    }
    string& GetStrDevGbid()
    {
        return strDevGbid;
    }
    const string& GetStrDevGbid() const
    {
        return strDevGbid;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGbsDiscGbPuRsp:public CEventRsp
{
public:
	CGbsDiscGbPuRsp()
	{
		event = OspExtEventDesc(GBS_DISC_GBPU_RSP);
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGbsDevKdidGetReq: public CEventReq
{
public:
	CGbsDevKdidGetReq()
	{
		event = OspExtEventDesc(GBS_DEV_KDID_GET_REQ);
	}
private:
	string strDevGbid;

public:
	void SetDevGbid(const string & strGbid)
	{
		strDevGbid = strGbid;
	}
	string& GetDevGbid()
	{
		return strDevGbid;
	}
	const string& GetDevGbid() const 
	{
		return strDevGbid;
	}
public:
	const std::string ToXml() const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc) const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode) const;
	void FromXml(const TiXmlNode& xmNode);
};

class CGbsDevKdidGetRsp: public CEventRsp
{
public:
	CGbsDevKdidGetRsp()
	{
		event = OspExtEventDesc(GBS_DEV_KDID_GET_RSP);
	}
private:
	string kdDevUri;       // 设备的KdUri         
	int encodeChnIndex;    // 编码通道索引，从0开始，-1表示设备本身

public:
	void SetKdDevUri(const string& strkdDevUri)
	{
		kdDevUri = strkdDevUri;
	}
	string& GetKdDevUri()
	{
		return kdDevUri;
	}
	const string& GetKdDevUri() const
	{
		return kdDevUri;
	}

	void SetEncodeChnIndex(int nEncodeChnIndex)
	{
		encodeChnIndex = nEncodeChnIndex;
	}
	int GetEncodeChnIndex() const
	{
		return encodeChnIndex;
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class CGbsDevGbidGetReq: public CEventReq
{
public:
    CGbsDevGbidGetReq()
    {
        event = OspExtEventDesc(GBS_DEV_GBID_GET_REQ);
    }

private:
    string kdDevUri;       // 设备的KdUri         
    int encodeChnIndex;    // 编码通道索引，从0开始，-1表示设备本身, -2表示分组
public:
    void SetKdDevUri(const string& strKdDevUri)
    {
        kdDevUri = strKdDevUri;
    }
    string& GetKdDevUri()
    {
        return kdDevUri;
    }
    const string& GetKdDevUri() const
    {
        return kdDevUri;
    }

    void SetEncodeChnIndex(int nEncodeChnIndex)
    {
        encodeChnIndex = nEncodeChnIndex;
    }
    int GetEncodeChnIndex() const
    {
        return encodeChnIndex;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CGbsDevGbidGetRsp: public CEventRsp
{
public:
    CGbsDevGbidGetRsp()
    {
        event = OspExtEventDesc(GBS_DEV_GBID_GET_RSP);
    }

private:
    string gbId;  // 对应的国标编号
    string gbsId; // 通信的GBS编号  也即系统目录编号          

public:
    void SetGbId(const string& strGbId)
    {
        gbId = strGbId;
    }
    string& GetGbId()
    {
        return gbId;
    }
    const string& GetGbId() const
    {
        return gbId;
    }

    void SetGbsId(const string& strGbsId)
    {
        gbsId = strGbsId;
    }
    string& GetGbsId()
    {
        return gbsId;
    }
    const string& GetGbsId() const
    {
        return gbsId;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGbsDevGrpGbidBatchGetReq : public CEventReq
{
public:
	CGbsDevGrpGbidBatchGetReq()
	{
		event = OspExtEventDesc(GBS_DEV_GRP_GBID_BATCH_GET_REQ);
	}
private:
	vector<string> vecGroupkdid;               

public:
    void SetVecGroupkdid(const vector<string>& cVecGroupkdid)
    {
        vecGroupkdid = cVecGroupkdid;
    }
    vector<string>& GetVecGroupkdid()
    {
        return vecGroupkdid;
    }
    const vector<string>& GetVecGroupkdid() const
    {
        return vecGroupkdid;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGbsDevGrpGbidBatchGetRsp : public CEventRsp
{
public:
	CGbsDevGrpGbidBatchGetRsp()
	{
		event = OspExtEventDesc(GBS_DEV_GRP_GBID_BATCH_GET_RSP);
	}
private:
	vector<TGbDevGrpGbidBatchGetInfo> vecBatchDevGrpInfo;               

public:
    void SetVecBatchDevGrpInfo(const vector<TGbDevGrpGbidBatchGetInfo>& cVecBatchDevGrpInfo)
    {
        vecBatchDevGrpInfo = cVecBatchDevGrpInfo;
    }
    vector<TGbDevGrpGbidBatchGetInfo>& GetVecBatchDevGrpInfo()
    {
        return vecBatchDevGrpInfo;
    }
    const vector<TGbDevGrpGbidBatchGetInfo>& GetVecBatchDevGrpInfo() const
    {
        return vecBatchDevGrpInfo;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGbsDevGbidBatchGetReq : public CEventReq
{
public:
	CGbsDevGbidBatchGetReq()
	{
		event = OspExtEventDesc(GBS_DEV_GBID_BATCH_GET_REQ);
	}
private:
	vector<TGbDevGbidBatchGetCondition> vecConditions;                
public:
    void SetVecConditions(const vector<TGbDevGbidBatchGetCondition>& cVecConditions)
    {
        vecConditions = cVecConditions;
    }
    vector<TGbDevGbidBatchGetCondition>& GetVecConditions()
    {
        return vecConditions;
    }
    const vector<TGbDevGbidBatchGetCondition>& GetVecConditions() const
    {
        return vecConditions;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGbsDevGbidBatchGetRsp : public CEventRsp
{
public:
	CGbsDevGbidBatchGetRsp()
	{
		event = OspExtEventDesc(GBS_DEV_GBID_BATCH_GET_RSP);
	}
private:
	vector<TGbDevGbidBatchGetInfo> vecBatchDevInfo;               
public:
    void SetVecBatchDevInfo(const vector<TGbDevGbidBatchGetInfo>& cVecBatchDevInfo)
    {
        vecBatchDevInfo = cVecBatchDevInfo;
    }
    vector<TGbDevGbidBatchGetInfo>& GetVecBatchDevInfo()
    {
        return vecBatchDevInfo;
    }
    const vector<TGbDevGbidBatchGetInfo>& GetVecBatchDevInfo() const
    {
        return vecBatchDevInfo;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGbsGbDevTeleBootReq: public CEventReq
{
public:
	CGbsGbDevTeleBootReq()
	{
		event = OspExtEventDesc(GBS_GBDEV_TELEBOOT_REQ);
	}
private:
	string kdDevUri;                //设备的科达Uri
public:
    void SetKdDevUri(const string& strKdDevUri)
    {
        kdDevUri = strKdDevUri;
    }
    string& GetKdDevUri()
    {
        return kdDevUri;
    }
    const string& GetKdDevUri() const
    {
        return kdDevUri;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGbsGbDevTeleBootRsp:public CEventRsp
{
public:
	CGbsGbDevTeleBootRsp()
	{
		event = OspExtEventDesc(GBS_GBDEV_TELEBOOT_RSP);
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class CGbsGbParentDomainGetReq:public CEventReq
{
public:
	CGbsGbParentDomainGetReq()
	{
		event = OspExtEventDesc(GBS_GBPARENTDOMAIN_GET_REQ);
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class CGbsGbParentDomainGetRsp:public CEventRsp
{
public:
	CGbsGbParentDomainGetRsp()
	{
		event = OspExtEventDesc(GBS_GBPARENTDOMAIN_GET_RSP);
	}
private:
	int SumNum;               
	vector< TGbsGbParentDomainInfo > GbParentDomainList;                 //GbParentDomain list
public:
    void SetSumNum(int nSumNum)
    {
        SumNum = nSumNum;
    }
    int GetSumNum() const
    {
        return SumNum;
    }

    void SetGbParentDomainList(const vector< TGbsGbParentDomainInfo >& cGbParentDomainList)
    {
        GbParentDomainList = cGbParentDomainList;
    }
    vector< TGbsGbParentDomainInfo >& GetGbParentDomainList()
    {
        return GbParentDomainList;
    }
    const vector< TGbsGbParentDomainInfo >& GetGbParentDomainList() const
    {
        return GbParentDomainList;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGbsGbPDCatalogLoadReq:public CEventReq
{
public:
	CGbsGbPDCatalogLoadReq()
	{
		event = OspExtEventDesc(GBS_GBPARENTDOMAIN_CATALOG_LOAD_REQ);
	}
private:
	string User;                //用户名
	string GbId;                //国标上级域ID
	string CuSession;           //CU
public:
    void SetUser(const string& strUser)
    {
        User = strUser;
    }
    string& GetUser()
    {
        return User;
    }
    const string& GetUser() const
    {
        return User;
    }

    void SetGbId(const string& strGbId)
    {
        GbId = strGbId;
    }
    string& GetGbId()
    {
        return GbId;
    }
    const string& GetGbId() const
    {
        return GbId;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetCuSession(const string& strCuSession)
    {
        CuSession = strCuSession;
    }
    string& GetCuSession()
    {
        return CuSession;
    }
    const string& GetCuSession() const
    {
        return CuSession;
    }

};

class CGbsGbPDCatalogLoadRsp:public CEventRsp
{
public:
	CGbsGbPDCatalogLoadRsp()
	{
		event = OspExtEventDesc(GBS_GBPARENTDOMAIN_CATALOG_LOAD_RSP);
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGbsGbPDCLProgressNtfReq:public CEventReq
{
public:
	CGbsGbPDCLProgressNtfReq()
	{
		event = OspExtEventDesc(GBS_GBPARENTDOMAIN_CL_PROGRESS_NTF_REQ);
	}
private:
	string CuSession;
	int progressPercent;               // -1表示失败,正常范围 [0-100]                
	string progressTip;                // 进度提示, 出错时为错误描述   
public:
    void SetCuSession(const string& strCuSession)
    {
        CuSession = strCuSession;
    }
    string& GetCuSession()
    {
        return CuSession;
    }
    const string& GetCuSession() const
    {
        return CuSession;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetProgressPercent(int nProgressPercent)
    {
        progressPercent = nProgressPercent;
    }
    int GetProgressPercent() const
    {
        return progressPercent;
    }

    void SetProgressTip(const string& strProgressTip)
    {
        progressTip = strProgressTip;
    }
    string& GetProgressTip()
    {
        return progressTip;
    }
    const string& GetProgressTip() const
    {
        return progressTip;
    }

};

class CGbsGbPDCLProgressNtfRsp:CEventRsp 
{
public:
	CGbsGbPDCLProgressNtfRsp()
	{
		event = OspExtEventDesc(GBS_GBPARENTDOMAIN_CL_PROGRESS_NTF_RSP);
	}
private:
	string CuSession;               
public:
    void SetCuSession(const string& strCuSession)
    {
        CuSession = strCuSession;
    }
    string& GetCuSession()
    {
        return CuSession;
    }
    const string& GetCuSession() const
    {
        return CuSession;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGbsGbDevRecCtrlReq:public CEventReq
{
public:
	CGbsGbDevRecCtrlReq()
	{
		event = OspExtEventDesc(GBS_GBDEV_RECCTRL_REQ); 
	}
private:
	string DevId;                //GBID或kdid@domainName
	string Cmd;                 //Record 开启录像， StopRecord停止录像
	int    ChnIndex;                           //DevId为kdid@domainName时，此字段生效
public:
    void SetDevId(const string& strDevId)
    {
        DevId = strDevId;
    }
    string& GetDevId()
    {
        return DevId;
    }
    const string& GetDevId() const
    {
        return DevId;
    }

    void SetCmd(const string& strCmd)
    {
        Cmd = strCmd;
    }
    string& GetCmd()
    {
        return Cmd;
    }
    const string& GetCmd() const
    {
        return Cmd;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetChnIndex(int nChnIndex)
    {
        ChnIndex = nChnIndex;
    }
    int GetChnIndex() const
    {
        return ChnIndex;
    }

};

class CGbsGbDevRecCtrlRsp:public CEventRsp
{
public:
	CGbsGbDevRecCtrlRsp()
	{
		event = OspExtEventDesc(GBS_GBDEV_RECCTRL_RSP);
	}

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CGbsGbRefreshCatalogReq: public CEventReq
{
public:
	CGbsGbRefreshCatalogReq()
	{
		event = OspExtEventDesc(GBS_REFRESH_CATALOG_REQ);
	}

private:
	string gbDevId;                //请求域的国标ID
public:
    void SetGbDevId(const string& strGbDevId)
    {
        gbDevId = strGbDevId;
    }
    string& GetGbDevId()
    {
        return gbDevId;
    }
    const string& GetGbDevId() const
    {
        return gbDevId;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGbsGbRefreshCatalogRsp: public CEventRsp
{
public:
	CGbsGbRefreshCatalogRsp()
	{
		event = OspExtEventDesc(GBS_REFRESH_CATALOG_RSP);
	}
private:
	string gbDevId;                //请求域的国标ID
public:
	void SetGbDevId(const string& strGbDevId)
	{
		gbDevId = strGbDevId;
	}
	string& GetGbDevId()
	{
		return gbDevId;
	}
	const string& GetGbDevId() const
	{
		return gbDevId;
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CGbsGbRefreshCatalogNtfReq: public CEventReq
{
public:
	CGbsGbRefreshCatalogNtfReq()
	{
		event = OspExtEventDesc(GBS_REFRESH_CATALOG_NTF_REQ);
	}

private:
	string gbDevId;                               //域的国标ID
	int result;                   //结果，0表示成功
public:
    void SetGbDevId(const string& strGbDevId)
    {
        gbDevId = strGbDevId;
    }
    string& GetGbDevId()
    {
        return gbDevId;
    }
    const string& GetGbDevId() const
    {
        return gbDevId;
    }

    void SetResult(int nResult)
    {
        result = nResult;
    }
    int GetResult() const
    {
        return result;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGbsGbRefreshCatalogNtfRsp: public CEventRsp
{
public:
	CGbsGbRefreshCatalogNtfRsp()
	{
		event = OspExtEventDesc(GBS_REFRESH_CATALOG_NTF_RSP);
	}
private:
	string gbDevId;                //请求域的国标ID
public:
	void SetGbDevId(const string& strGbDevId)
	{
		gbDevId = strGbDevId;
	}
	string& GetGbDevId()
	{
		return gbDevId;
	}
	const string& GetGbDevId() const
	{
		return gbDevId;
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};


class CGbsDevGrpCivilCodeGetReq: public CEventReq
{
public:
	CGbsDevGrpCivilCodeGetReq()
	{
		event = OspExtEventDesc(GBS_DEVGRP_CIVILCODE_GET_REQ);
	}

private:
	string gbDevGrpId;                               //请求国标ID
public:
    void SetGbDevGrpId(const string& strGbDevGrpId)
    {
        gbDevGrpId = strGbDevGrpId;
    }
    string& GetGbDevGrpId()
    {
        return gbDevGrpId;
    }
    const string& GetGbDevGrpId() const
    {
        return gbDevGrpId;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGbsDevGrpCivilCodeGetRsp: public CEventRsp
{
public:
	CGbsDevGrpCivilCodeGetRsp()
	{
		event = OspExtEventDesc(GBS_DEVGRP_CIVILCODE_GET_RSP);
	}
private:
	string civilcode;                               //行政区划
public:
    void SetCivilcode(const string& strCivilcode)
    {
        civilcode = strCivilcode;
    }
    string& GetCivilcode()
    {
        return civilcode;
    }
    const string& GetCivilcode() const
    {
        return civilcode;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGbsCatalogSelectReportStartReq : public CEventReq
{
public:
	CGbsCatalogSelectReportStartReq()
	{
		event = OspExtEventDesc(GBS_CATALOG_SELECT_REPORT_START_REQ);
	}
private:
	string parentGbDomainId;        //上级国标域ID
	string devTreeId;               //设备树ID

public:
	void SetParentGbDomainId(const string& strParentGbDomainId)
	{
		parentGbDomainId = strParentGbDomainId;
	}
	string& GetParentGbDomainId()
	{
		return parentGbDomainId;
	}
	const string& GetParentGbDomainId() const
	{
		return parentGbDomainId;
	}

    void SetDevTreeId(const string& strDevTreeId)
    {
        devTreeId = strDevTreeId;
    }
    string& GetDevTreeId()
    {
        return devTreeId;
    }
    const string& GetDevTreeId() const
    {
        return devTreeId;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

};

class CGbsCatalogSelectReportStartRsp : public CEventRsp
{
public:
	CGbsCatalogSelectReportStartRsp()
	{
		event = OspExtEventDesc(GBS_CATALOG_SELECT_REPORT_START_RSP);
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGbsCatalogSelectReportCancelReq : public CEventReq
{
public:
	CGbsCatalogSelectReportCancelReq()
	{
		event = OspExtEventDesc(GBS_CATALOG_SELECT_REPORT_CANCEL_REQ);
	}
private:
	string gbua;               //上级国标域ID
public:
    void SetGbua(const string& strGbua)
    {
        gbua = strGbua;
    }
    string& GetGbua()
    {
        return gbua;
    }
    const string& GetGbua() const
    {
        return gbua;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGbsCatalogSelectReportCancelRsp : public CEventRsp
{
public:
	CGbsCatalogSelectReportCancelRsp()
	{
		event = OspExtEventDesc(GBS_CATALOG_SELECT_REPORT_CANCEL_RSP);
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGbsNatTableAddReq: public CEventReq
{
public:
    CGbsNatTableAddReq()
    {
        event=OspExtEventDesc(GBS_NAT_TABLE_ADD_REQ);
    }

private:
    string name;  // 不允许重复，不允许存在空白字符 
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

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGbsNatTableAddRsp: public CEventRsp
{
public:
    CGbsNatTableAddRsp()
    {
        event=OspExtEventDesc(GBS_NAT_TABLE_ADD_RSP);
    }
    
private:
    string natId; // 平台生成的唯一id                
public:
    void SetNatId(const string& strNatId)
    {
        natId = strNatId;
    }
    string& GetNatId()
    {
        return natId;
    }
    const string& GetNatId() const
    {
        return natId;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGbsNatTableDelReq: public CEventReq
{
public:
    CGbsNatTableDelReq()
    {
        event=OspExtEventDesc(GBS_NAT_TABLE_DEL_REQ);
    }

private:
    string natId;  // 存在使用者时拒绝删除                
public:
    void SetNatId(const string& strNatId)
    {
        natId = strNatId;
    }
    string& GetNatId()
    {
        return natId;
    }
    const string& GetNatId() const
    {
        return natId;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGbsNatTableDelRsp: public CEventRsp
{
public:
    CGbsNatTableDelRsp()
    {
        event=OspExtEventDesc(GBS_NAT_TABLE_DEL_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGbsNatTableModReq: public CEventReq
{
public:
    CGbsNatTableModReq()
    {
        event=OspExtEventDesc(GBS_NAT_TABLE_MOD_REQ);
    }

private:
    string natId; 
    string newName;  // 不允许重复，不允许存在空白字符
public:
    void SetNewName(const string& strNewName)
    {
        newName = strNewName;
    }
    string& GetNewName()
    {
        return newName;
    }
    const string& GetNewName() const
    {
        return newName;
    }
    void SetNatId(const string& strNatId)
    {
        natId =  strNatId;
    }
    string& GetNatId()
    {
        return natId;
    }
    const string& GetNatId() const
    {  
        return natId;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGbsNatTableModRsp: public CEventRsp
{
public:
    CGbsNatTableModRsp()
    {
        event=OspExtEventDesc(GBS_NAT_TABLE_MOD_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGbsNatTableGetReq: public CEventReq
{
public:
    CGbsNatTableGetReq()
    {
        event=OspExtEventDesc(GBS_NAT_TABLE_GET_REQ);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGbsNatTableGetRsp: public CEventRsp
{
public:
    CGbsNatTableGetRsp()
    {
        event=OspExtEventDesc(GBS_NAT_TABLE_GET_RSP);
    }

private:
    vector<string> natIds;                
    vector<string> natNames;                
public:
    void SetNatIds(const vector<string>& cNatIds)
    {
        natIds = cNatIds;
    }
    vector<string>& GetNatIds()
    {
        return natIds;
    }
    const vector<string>& GetNatIds() const
    {
        return natIds;
    }

    void SetNatNames(const vector<string>& cNatNames)
    {
        natNames = cNatNames;
    }
    vector<string>& GetNatNames()
    {
        return natNames;
    }
    const vector<string>& GetNatNames() const
    {
        return natNames;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGbsNatDataAddReq: public CEventReq
{
public:
    CGbsNatDataAddReq()
    {
        event = OspExtEventDesc(GBS_NAT_DATA_SET_REQ);
    }

private:
    string gbNatId; 
    int action;  //0是增加，1是修改
    TGbsNatData natData;
public:
    void SetGbNatId(const string& strGbNatId)
    {
        gbNatId = strGbNatId;
    }
    const string&  GetGbNatId() const
    {
        return gbNatId;
    }
    string & GetGbNatId()
    {
        return gbNatId;
    }
    void SetNatData(const TGbsNatData& tGbsNatData)
    {
        natData = tGbsNatData;
    }
    const TGbsNatData& GetNatData() const
    {
        return natData;
    }
    TGbsNatData& GetNatData()
    {
        return natData;
    }
    void SetAction(const int dwAction)
    {
        action = dwAction;
    }
    const int GetAction() const
    {
        return action;
    }
    int &GetAction()
    {
         return action;
    }


public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGbsNatDataAddRsp: public CEventRsp
{
public:
    CGbsNatDataAddRsp()
    {
        event = OspExtEventDesc(GBS_NAT_DATA_SET_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGbsNatDataDelReq: public CEventReq
{
public:
    CGbsNatDataDelReq()
    {
        event = OspExtEventDesc(GBS_NAT_DATA_DEL_REQ);
    }

private:
    string gbNatId;

    TGbsNatData::ProtoType proto;
    string intraNetIp;
    u16 intraNetPortBegin;
    u16 intraNetPortEnd;
public:
    void SetGbNatId(const string& strGbNatId)
    {
        gbNatId = strGbNatId;
    }
    const string&  GetGbNatId() const
    {
        return gbNatId;
    }
    string & GetGbNatId()
    {
        return gbNatId;
    }

    void SetProto( u16 wProto)
    {
        proto = (TGbsNatData::ProtoType) wProto;
    }
    const  u16 GetProto() const
    {
        return (u16)proto;
    }
    u16 GetProto()
    {
        return (u16)proto;
    }

    void SetIntraNetIp(const string& strIntraNetIp)
    {
        intraNetIp = strIntraNetIp;
    }
    const string& GetIntraNetIp() const
    {
        return intraNetIp;
    }
    string & GetIntraNetIp()
    {
        return intraNetIp;
    }

    void SetIntraNetPortBegin(const u16& nGetIntraNetPortBegin)
    {
        intraNetPortBegin = nGetIntraNetPortBegin;
    }
    const u16  GetIntraNetPortBegin() const
    {
        return intraNetPortBegin;
    }

    u16 GetIntraNetPortBegin()
    {
        return intraNetPortBegin;
    }
    
    void SetIntraNetPortEnd(const u16& nGetIntraNetPortEnd)
    {
        intraNetPortEnd = nGetIntraNetPortEnd;
    }
    const u16 GetIntraNetPortEnd() const
    {
        return intraNetPortEnd;
    }
    u16 GetIntraNetPortEnd()
    {
        return intraNetPortEnd;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGbsNatDataDelRsp: public CEventRsp
{
public:
    CGbsNatDataDelRsp()
    {
        event = OspExtEventDesc(GBS_NAT_DATA_DEL_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGbsNatDataListGetReq: public CEventReq
{
public:
    CGbsNatDataListGetReq()
    {
        event = OspExtEventDesc(GBS_NAT_DATA_LIST_GET_REQ);
    }

private:
    string gbNatId; // 推荐直接使用NAT名称当作id
    TGbsNatData natData;
public:
    void SetGbNatId(const string& strGbNatId)
    {
        gbNatId = strGbNatId;
    }
    const string&  GetGbNatId() const
    {
        return gbNatId;
    }
    string & GetGbNatId()
    {
        return gbNatId;
    }
    void SetNatData(const TGbsNatData& tGbNatData)
    {
        natData = tGbNatData;
    }
    const TGbsNatData& GetNatData() const
    {
        return natData;
    }
    TGbsNatData& GetNatData()
    {
        return natData;
    }


public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGbsNatDataListGetRsp: public CEventRsp
{
public:
    CGbsNatDataListGetRsp()
    {
        event = OspExtEventDesc(GBS_NAT_DATA_LIST_GET_RSP);
    }

private:
    vector<string> gbNatIds;

public:
    void SetGbNatIds(const vector<string>& strGbNatIds)
    {
        gbNatIds = strGbNatIds;
    }
    const vector<string>&  GetGbNatIds() const
    {
        return gbNatIds;
    }
    vector<string> & GetGbNatIds()
    {
        return gbNatIds;
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGbsNatDataContentQryReq: public CQueryReqBase
{
public:
    CGbsNatDataContentQryReq()
    {
        event = OspExtEventDesc(GBS_NAT_DATA_CONTENT_QRY_REQ);
    }

private:
    string gbNatId;
public:
    void SetGbNatId(const string& strGbNatId)
    {
        gbNatId = strGbNatId;
    }
    const string&  GetGbNatId() const
    {
        return gbNatId;
    }
    string & GetGbNatId()
    {
        return gbNatId;
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGbsNatDataContentQryRsp: public CQueryRspBase
{
public:
    CGbsNatDataContentQryRsp()
    {
        event = OspExtEventDesc(GBS_NAT_DATA_CONTENT_QRY_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGbsNatDataContentQryNtfReq: public CQueryNtfReqBase
{
public:
    CGbsNatDataContentQryNtfReq()
    {
        event = OspExtEventDesc(GBS_NAT_DATA_CONTENT_QRY_NTF_REQ);
    }

private:
    vector<TGbsNatData> natData;
public:
    void SetNatData(const vector<TGbsNatData> & tGbNatData)
    {
        natData = tGbNatData;
    }
    const vector<TGbsNatData> & GetNatData() const
    {
        return natData;
    }
    vector<TGbsNatData> & GetNatData()
    {
        return natData;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGbsGbpuNatSetReq: public CEventReq
{
public:
    CGbsGbpuNatSetReq()
    {
        event = OspExtEventDesc(GBS_GBPU_NAT_SET_REQ);
    }

private:
    string gbpuId;
    string gbNatId;

public:
    void SetGbPutId(const string& strGbPutId)
    {
        gbpuId = strGbPutId;
    }
    const string&  GetGbPuId() const
    {
        return gbpuId;
    }
    string & GetGbPuId()
    {
        return gbpuId;
    }
     void SetGbNatId(const string& strGbNatId)
    {
        gbNatId = strGbNatId;
    }
    const string&  GetGbNatId() const
    {
        return gbNatId;
    }
    string & GetGbNatId()
    {
        return gbNatId;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGbsGbpuNatSetRsp: public CEventRsp
{
public:
    CGbsGbpuNatSetRsp()
    {
        event = OspExtEventDesc(GBS_GBPU_NAT_SET_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGbsGbpuNatDelReq: public CEventReq
{
public:
    CGbsGbpuNatDelReq()
    {
        event = OspExtEventDesc(GBS_GBPU_NAT_DEL_REQ);
    }

private:
    string gbpuId;
    string gbNatId;

public:
    void SetGbPutId(const string& strGbPutId)
    {
        gbpuId = strGbPutId;
    }
    const string&  GetGbPuId() const
    {
        return gbpuId;
    }
    string & GetGbPuId()
    {
        return gbpuId;
    }
     void SetGbNatId(const string& strGbNatId)
    {
        gbNatId = strGbNatId;
    }
    const string&  GetGbNatId() const
    {
        return gbNatId;
    }
    string & GetGbNatId()
    {
        return gbNatId;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGbsGbpuNatDelRsp: public CEventRsp
{
public:
    CGbsGbpuNatDelRsp()
    {
        event = OspExtEventDesc(GBS_GBPU_NAT_DEL_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CGbsGbpuNatQryReq: public CQueryReqBase
{
public:
    CGbsGbpuNatQryReq()
    {
        event = OspExtEventDesc(GBS_GBPU_NAT_QRY_REQ);
    }

private:
    string gbNatId;
public:
    void SetGbNatId(const string& strGbNatId)
    {
        gbNatId = strGbNatId;
    }
    const string&  GetGbNatId() const
    {
        return gbNatId;
    }
    string & GetGbNatId()
    {
        return gbNatId;
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGbsGbpuNatQryRsp: public CQueryRspBase
{
public:
    CGbsGbpuNatQryRsp()
    {
        event = OspExtEventDesc(GBS_GBPU_NAT_QRY_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGbsGbpuNatQryNtfReq: public CQueryNtfReqBase
{
public:
    CGbsGbpuNatQryNtfReq()
    {
        event = OspExtEventDesc(GBS_GBPU_NAT_QRY_NTF_REQ);
    }

private:
    map<string, string> gbpuId2gbnatId;

public:
    void SetGbPuId2GbNatId(const map<string, string>& tGbPuId2GbNatId)
    {
        gbpuId2gbnatId = tGbPuId2GbNatId;
    }
    const map<string, string>& GetGbPuId2GbNatId() const
    {
        return gbpuId2gbnatId;
    }
    map<string, string>& GetGbPuId2GbNatId()
    {
        return gbpuId2gbnatId;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CGbsSetPuiGbMtsReq: public CEventReq
{
public:
    CGbsSetPuiGbMtsReq()
    {
        event = OspExtEventDesc(GBS_SET_PUIGB_MTS_REQ);
    }
private:
    string ip;
    int port; 
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetIp(const string& strIp)
    {
        ip = strIp;
    }
    string& GetIp()
    {
        return ip;
    }
    const string& GetIp() const
    {
        return ip;
    }
    void SetPort(const int nPort)
    {
        port = nPort;
    }
    int GetPort()
    {
        return port;
    }
    const int GetPort() const
    {
        return port;
    }
};

class CGbsSetPuiGbMtsRsp : public CEventRsp
{
public:
	CGbsSetPuiGbMtsRsp()
	{
		event = OspExtEventDesc(GBS_SET_PUIGB_MTS_RSP);
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

class CGbsSafeAuthInfoFirstReq : public CEventReq
{
public:
	CGbsSafeAuthInfoFirstReq()
	{
		event = OspExtEventDesc(GBS_SAFE_AUTHINFO_CHECK_FIRST_REQ);
	}
private:
	string UserId;               
	string CapabilityAlgorithm;                
public:
    void SetUserId(const string& strUserId)
    {
        UserId = strUserId;
    }
    string& GetUserId()
    {
        return UserId;
    }
    const string& GetUserId() const
    {
        return UserId;
    }

    void SetCapabilityAlgorithm(const string& strCapabilityAlgorithm)
    {
        CapabilityAlgorithm = strCapabilityAlgorithm;
    }
    string& GetCapabilityAlgorithm()
    {
        return CapabilityAlgorithm;
    }
    const string& GetCapabilityAlgorithm() const
    {
        return CapabilityAlgorithm;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGbsSafeAuthInfoFirstRsp : public CEventRsp
{
public:
	CGbsSafeAuthInfoFirstRsp()
	{
		event = OspExtEventDesc(GBS_SAFE_AUTHINFO_CHECK_FIRST_RSP);
	}

private:
	string BidirectionAlgorithm;               
	string Random1;               
public:
    void SetBidirectionAlgorithm(const string& strBidirectionAlgorithm)
    {
        BidirectionAlgorithm = strBidirectionAlgorithm;
    }
    string& GetBidirectionAlgorithm()
    {
        return BidirectionAlgorithm;
    }
    const string& GetBidirectionAlgorithm() const
    {
        return BidirectionAlgorithm;
    }

    void SetRandom1(const string& strRandom1)
    {
        Random1 = strRandom1;
    }
    string& GetRandom1()
    {
        return Random1;
    }
    const string& GetRandom1() const
    {
        return Random1;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGbsSafeAuthInfoSecondReq : public CEventReq
{
public:
	CGbsSafeAuthInfoSecondReq()
	{
		event = OspExtEventDesc(GBS_SAFE_AUTHINFO_CHECK_SECOND_REQ);
	}
private:
	string Random1;               
	string Random2;               
	string ServerId;               
	string Sign1;               
	string Algorithm;               
public:
    void SetRandom1(const string& strRandom1)
    {
        Random1 = strRandom1;
    }
    string& GetRandom1()
    {
        return Random1;
    }
    const string& GetRandom1() const
    {
        return Random1;
    }

    void SetRandom2(const string& strRandom2)
    {
        Random2 = strRandom2;
    }
    string& GetRandom2()
    {
        return Random2;
    }
    const string& GetRandom2() const
    {
        return Random2;
    }

    void SetServerId(const string& strServerId)
    {
        ServerId = strServerId;
    }
    string& GetServerId()
    {
        return ServerId;
    }
    const string& GetServerId() const
    {
        return ServerId;
    }

    void SetSign1(const string& strSign1)
    {
        Sign1 = strSign1;
    }
    string& GetSign1()
    {
        return Sign1;
    }
    const string& GetSign1() const
    {
        return Sign1;
    }

    void SetAlgorithm(const string& strAlgorithm)
    {
        Algorithm = strAlgorithm;
    }
    string& GetAlgorithm()
    {
        return Algorithm;
    }
    const string& GetAlgorithm() const
    {
        return Algorithm;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGbsSafeAuthInfoSecondRsp : public CEventRsp
{
public:
	CGbsSafeAuthInfoSecondRsp()
	{
		event = OspExtEventDesc(GBS_SAFE_AUTHINFO_CHECK_SECOND_RSP);
	}
private:
	string Random1;               
	string Random2;               
	string CryptKey;                
	string Sign2;               
	string Algorithm;               
public:
    void SetRandom1(const string& strRandom1)
    {
        Random1 = strRandom1;
    }
    string& GetRandom1()
    {
        return Random1;
    }
    const string& GetRandom1() const
    {
        return Random1;
    }

    void SetRandom2(const string& strRandom2)
    {
        Random2 = strRandom2;
    }
    string& GetRandom2()
    {
        return Random2;
    }
    const string& GetRandom2() const
    {
        return Random2;
    }

    void SetCryptKey(const string& strCryptKey)
    {
        CryptKey = strCryptKey;
    }
    string& GetCryptKey()
    {
        return CryptKey;
    }
    const string& GetCryptKey() const
    {
        return CryptKey;
    }

    void SetSign2(const string& strSign2)
    {
        Sign2 = strSign2;
    }
    string& GetSign2()
    {
        return Sign2;
    }
    const string& GetSign2() const
    {
        return Sign2;
    }

    void SetAlgorithm(const string& strAlgorithm)
    {
        Algorithm = strAlgorithm;
    }
    string& GetAlgorithm()
    {
        return Algorithm;
    }
    const string& GetAlgorithm() const
    {
        return Algorithm;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGbsCertificateUploadReq :public CEventReq
{
public:
    CGbsCertificateUploadReq()
    {
        event = OspExtEventDesc(GBS_CERTIFICATE_UPLOAD_REQ);
        type = 0;
    }
    
private:
    int type; // 1:设备证书 2：用户证书 其它值：非法                
    string ownerid; // 证书拥有者id  设备证书时为设备编号，用户证书时为用户编号                
    string validField; // 校验字段  设备证书时为密码模块id，用户证书时为空(检测中心何迪说用户身份证号码不在证书里面)               

    string cert4sign;  // 签名证书PEM格式内容                
    string cert4encry; // 加密证书PEM格式内容                
public:
    void SetType(int nType)
    {
        type = nType;
    }
    int GetType() const
    {
        return type;
    }

    void SetOwnerid(const string& strOwnerid)
    {
        ownerid = strOwnerid;
    }
    string& GetOwnerid()
    {
        return ownerid;
    }
    const string& GetOwnerid() const
    {
        return ownerid;
    }

    void SetValidField(const string& strValidField)
    {
        validField = strValidField;
    }
    string& GetValidField()
    {
        return validField;
    }
    const string& GetValidField() const
    {
        return validField;
    }

    void SetCert4sign(const string& strCert4sign)
    {
        cert4sign = strCert4sign;
    }
    string& GetCert4sign()
    {
        return cert4sign;
    }
    const string& GetCert4sign() const
    {
        return cert4sign;
    }

    void SetCert4encry(const string& strCert4encry)
    {
        cert4encry = strCert4encry;
    }
    string& GetCert4encry()
    {
        return cert4encry;
    }
    const string& GetCert4encry() const
    {
        return cert4encry;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGbsCertificateUploadRsp :public CEventRsp
{
public:
    CGbsCertificateUploadRsp()
    {
        event = OspExtEventDesc(GBS_CERTIFICATE_UPLOAD_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGbsCertificateGetReq : public CEventReq
{
public:
    CGbsCertificateGetReq()
    {
        event = OspExtEventDesc(GBS_CERTIFICATE_GET_REQ);
    }
	void SetOwnerid(const string& strOwnerid)
	{
		ownerid = strOwnerid;
	}
	string& GetOwnerid()
	{
		return ownerid;
	}
	const string& GetOwnerid() const
	{
		return ownerid;
	}
private:
    string ownerid; // 证书拥有者id  设备证书时为设备编号，用户证书时为用户编号 
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGbsCertificateGetRsp : public CEventRsp
{
public:
    CGbsCertificateGetRsp()
    {
        event = OspExtEventDesc(GBS_CERTIFICATE_GET_RSP);
        type = 0;
    }

private:
    int type; // 1:设备证书 2：用户证书 其它值：非法                
    string ownerid; // 证书拥有者id  设备证书时为设备编号，用户证书时为用户编号                
    string validField; // 校验字段  设备证书时为密码模块id，用户证书时为空(检测中心何迪说用户身份证号码不在证书里面)                

    string cert4sign;  // 签名证书PEM格式内容                
    string cert4encry; // 加密证书PEM格式内容                
public:
    void SetType(int nType)
    {
        type = nType;
    }
    int GetType() const
    {
        return type;
    }

    void SetOwnerid(const string& strOwnerid)
    {
        ownerid = strOwnerid;
    }
    string& GetOwnerid()
    {
        return ownerid;
    }
    const string& GetOwnerid() const
    {
        return ownerid;
    }

    void SetValidField(const string& strValidField)
    {
        validField = strValidField;
    }
    string& GetValidField()
    {
        return validField;
    }
    const string& GetValidField() const
    {
        return validField;
    }

    void SetCert4sign(const string& strCert4sign)
    {
        cert4sign = strCert4sign;
    }
    string& GetCert4sign()
    {
        return cert4sign;
    }
    const string& GetCert4sign() const
    {
        return cert4sign;
    }

    void SetCert4encry(const string& strCert4encry)
    {
        cert4encry = strCert4encry;
    }
    string& GetCert4encry()
    {
        return cert4encry;
    }
    const string& GetCert4encry() const
    {
        return cert4encry;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGbsRegUasInfoQryReq: public CQueryReqBase
{
public:
	CGbsRegUasInfoQryReq()
	{
		event = OspExtEventDesc(GBS_REG_UAS_INFO_QRY_REQ);
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGbsRegUasInfoQryRsp:public CQueryRspBase
{
public:
	CGbsRegUasInfoQryRsp()
	{
		event = OspExtEventDesc(GBS_REG_UAS_INFO_QRY_RSP);
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGbsRegUasInfoQryNtfReq:public CQueryNtfReqBase
{
public:
	CGbsRegUasInfoQryNtfReq()
	{
		event = OspExtEventDesc(GBS_REG_UAS_INFO_QRY_NTF_REQ);
	}
private:             
	vector<TGbsRegUasInfo> infos;
public:
    void SetInfos(const vector<TGbsRegUasInfo>& cInfos)
    {
        infos = cInfos;
    }
    vector<TGbsRegUasInfo>& GetInfos()
    {
        return infos;
    }
    const vector<TGbsRegUasInfo>& GetInfos() const
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

class CPrimaryGbsCatalogQryReq:public CEventReq
{
public:
	CPrimaryGbsCatalogQryReq()
	{
		event = OspExtEventDesc(GBS_PRIMARYGBS_CATALOG_QRY_REQ);
	}

private:
	int taskNo;                 //接收目录信息的task号
	string gbsId;                 //从gbs-cmssdk-uuid
public:
    void SetGbsId(const string& strGbsId)
    {
        gbsId = strGbsId;
    }
    string& GetGbsId()
    {
        return gbsId;
    }
    const string& GetGbsId() const
    {
        return gbsId;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetTaskNo(int nTaskNo)
    {
        taskNo = nTaskNo;
    }
    int GetTaskNo() const
    {
        return taskNo;
    }

};
class CPrimaryGbsCatalogQryRsp:public CEventRsp
{
public:
	CPrimaryGbsCatalogQryRsp()
	{
		event = OspExtEventDesc(GBS_PRIMARYGBS_CATALOG_QRY_RSP);
	}
private:
	int totalNum;               
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetTotalNum(int nTotalNum)
    {
        totalNum = nTotalNum;
    }
    int GetTotalNum() const
    {
        return totalNum;
    }

};

class CGbsGbdomainParentPrivilegeGetReq: public CEventReq
{
public:
    CGbsGbdomainParentPrivilegeGetReq()
    {
        event = OspExtEventDesc(GBS_GBDOMAIN_PARENT_PRIVILEGE_GET_REQ);
    }

private:
    string parentid;                
public:
    void SetParentid(const string& strParentid)
    {
        parentid = strParentid;
    }
    string& GetParentid()
    {
        return parentid;
    }
    const string& GetParentid() const
    {
        return parentid;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGbsGbdomainParentPrivilegeGetRsp: public CEventRsp
{
public:
    CGbsGbdomainParentPrivilegeGetRsp()
    {
        event = OspExtEventDesc(GBS_GBDOMAIN_PARENT_PRIVILEGE_GET_RSP);
    }
private:
    string parentid;                
    bool recplay;                
    bool recdownload;                
    bool ptz;                
public:
    void SetParentid(const string& strParentid)
    {
        parentid = strParentid;
    }
    string& GetParentid()
    {
        return parentid;
    }
    const string& GetParentid() const
    {
        return parentid;
    }

    void SetRecplay(bool bRecplay)
    {
        recplay = bRecplay;
    }
    bool GetRecplay() const
    {
        return recplay;
    }

    void SetRecdownload(bool bRecdownload)
    {
        recdownload = bRecdownload;
    }
    bool GetRecdownload() const
    {
        return recdownload;
    }

    void SetPtz(bool bPtz)
    {
        ptz = bPtz;
    }
    bool GetPtz() const
    {
        return ptz;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGbsGbdomainParentPrivilegeSetReq: public CEventReq
{
public:
    CGbsGbdomainParentPrivilegeSetReq()
    {
        event = OspExtEventDesc(GBS_GBDOMAIN_PARENT_PRIVILEGE_SET_REQ);
    }
private:
    string parentid;                
    bool recplay;                
    bool recdownload;                
    bool ptz;                
public:
    void SetParentid(const string& strParentid)
    {
        parentid = strParentid;
    }
    string& GetParentid()
    {
        return parentid;
    }
    const string& GetParentid() const
    {
        return parentid;
    }

    void SetRecplay(bool bRecplay)
    {
        recplay = bRecplay;
    }
    bool GetRecplay() const
    {
        return recplay;
    }

    void SetRecdownload(bool bRecdownload)
    {
        recdownload = bRecdownload;
    }
    bool GetRecdownload() const
    {
        return recdownload;
    }

    void SetPtz(bool bPtz)
    {
        ptz = bPtz;
    }
    bool GetPtz() const
    {
        return ptz;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGbsGbdomainParentPrivilegeSetRsp: public CEventRsp
{
public:
    CGbsGbdomainParentPrivilegeSetRsp()
    {
        event = OspExtEventDesc(GBS_GBDOMAIN_PARENT_PRIVILEGE_SET_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

#endif // _GBS_PROTO_H_

















