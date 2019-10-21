/*****************************************************************************
模块名      : uas_proto
文件名      : uas_proto.h
相关文件    : 
文件实现功能: uas_proto.h 定义uas外部消息的消息体
作者        : 胡志云
版本        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2012/05/28  1.0        huzhiyun    创建
        
******************************************************************************/
#ifndef _UAS_PROTO_H_
#define _UAS_PROTO_H_

#include "cms/uas/uas_event.h"
#include "cms/uas/uas_struct.h"

class CLoginUasReq: public CEventReq
{
public:
    CLoginUasReq()
    {
        event = OspExtEventDesc(UAS_LOGIN_REQ);
        loginType = enLoginType_DEFAULT;
    }
public:
    enum TLoginType
    {
        enLoginType_DEFAULT = 1,  // 平台模块登录，用户名（小写形式的模块名称，比如cmu模块登录时名称就是cmu）
        enLoginType_CUI = 2,      // CUI模块（相比其它平台模块，CUI上面需要挂CU用户会话）
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
    string& GetExtraInfo()
    {
        return extraInfo;
    }
    const string& GetExtraInfo() const
    {
        return extraInfo;
    }
    void SetExtraInfo(const string &val)
    {
        extraInfo = val;
    }
private:
    TLoginType  loginType;      // 登录类型
    string      userName;       // 模块名称或者用户名，由登录类型决定
    string      password;
    string      extraInfo;      // 扩展信息

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CLoginUasRsp: public CEventRsp
{
public:
    CLoginUasRsp()
    {
        event = OspExtEventDesc(UAS_LOGIN_RSP);
    }
    // 登录成功时在CEventRsp中返回服务端生成的会话号

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CLogoutUasReq: public CEventReq
{
public:
    CLogoutUasReq()
    {
        event = OspExtEventDesc(UAS_LOGOUT_REQ);
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

class CLogoutUasRsp: public CEventRsp
{
public:
    CLogoutUasRsp()
    {
        event = OspExtEventDesc(UAS_LOGOUT_RSP);
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

class COperateLogAddReq: public CEventReq
{
public:
    COperateLogAddReq()
    {
        event = OspExtEventDesc(UAS_OPERATE_LOG_ADD_REQ);
    }
private:
    TOperateLogInfo info;              
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetInfo(const TOperateLogInfo& cInfo)
    {
        info = cInfo;
    }
    TOperateLogInfo& GetInfo()
    {
        return info;
    }
    const TOperateLogInfo& GetInfo() const
    {
        return info;
    }

};

class COperateLogAddRsp: public CEventRsp
{
public:
    COperateLogAddRsp()
    {
        event = OspExtEventDesc(UAS_OPERATE_LOG_ADD_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class COperateLogQryReq: public CQueryReqBase
{
public:
    COperateLogQryReq()
    {
        event = OspExtEventDesc(UAS_OPERATE_LOG_QRY_REQ);
    }
private:
    COperateLogInfoQueryCondition condition;             
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetCondition(const COperateLogInfoQueryCondition& cCondition)
    {
        condition = cCondition;
    }
    COperateLogInfoQueryCondition& GetCondition()
    {
        return condition;
    }
    const COperateLogInfoQueryCondition& GetCondition() const
    {
        return condition;
    }

};

class COperateLogQryRsp: public CQueryRspBase
{
public:
    COperateLogQryRsp()
    {
        event = OspExtEventDesc(UAS_OPERATE_LOG_QRY_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class COperateLogQryNtfReq: public CQueryNtfReqBase
{
public:
    COperateLogQryNtfReq()
    {
        event = OspExtEventDesc(UAS_OPERATE_LOG_QRY_NTF_REQ);
    }
private:
    vector<TOperateLogInfo> infos;             
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetInfos(const vector<TOperateLogInfo>& cInfos)
    {
        infos = cInfos;
    }
    vector<TOperateLogInfo>& GetInfos()
    {
        return infos;
    }
    const vector<TOperateLogInfo>& GetInfos() const
    {
        return infos;
    }

};

class CDeviceLogAddReq: public CEventReq
{
public:
    CDeviceLogAddReq()
    {
        event = OspExtEventDesc(UAS_DEVICE_LOG_ADD_REQ);
    }
private:
    TDeviceLogInfo info;              
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
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

};

class CDeviceLogAddRsp: public CEventRsp
{
public:
    CDeviceLogAddRsp()
    {
        event = OspExtEventDesc(UAS_DEVICE_LOG_ADD_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDeviceLogQryReq: public CQueryReqBase
{
public:
    CDeviceLogQryReq()
    {
        event = OspExtEventDesc(UAS_DEVICE_LOG_QRY_REQ);
    }
private:
    CDeviceLogInfoQueryCondition condition;             
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetCondition(const CDeviceLogInfoQueryCondition& cCondition)
    {
        condition = cCondition;
    }
    CDeviceLogInfoQueryCondition& GetCondition()
    {
        return condition;
    }
    const CDeviceLogInfoQueryCondition& GetCondition() const
    {
        return condition;
    }

};

class CDeviceLogQryRsp: public CQueryRspBase
{
public:
    CDeviceLogQryRsp()
    {
        event = OspExtEventDesc(UAS_DEVICE_LOG_QRY_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDeviceLogQryNtfReq: public CQueryNtfReqBase
{
public:
    CDeviceLogQryNtfReq()
    {
        event = OspExtEventDesc(UAS_DEVICE_LOG_QRY_NTF_REQ);
    }
private:
    vector<TDeviceLogInfo> infos;             
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetInfos(const vector<TDeviceLogInfo>& cInfos)
    {
        infos = cInfos;
    }
    vector<TDeviceLogInfo>& GetInfos()
    {
        return infos;
    }
    const vector<TDeviceLogInfo>& GetInfos() const
    {
        return infos;
    }

};

class CAlarmLogAddReq: public CEventReq
{
public:
    CAlarmLogAddReq()
    {
        event = OspExtEventDesc(UAS_ALARM_LOG_ADD_REQ);
    }
private:
    TAlarmLogInfo info;              // AlarmFlag表示告警恢复时只有devId，channId，type, flag和resumeTime字段有效
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetInfo(const TAlarmLogInfo& cInfo)
    {
        info = cInfo;
    }
    TAlarmLogInfo& GetInfo()
    {
        return info;
    }
    const TAlarmLogInfo& GetInfo() const
    {
        return info;
    }

};

class CAlarmLogAddRsp: public CEventRsp
{
public:
    CAlarmLogAddRsp()
    {
        event = OspExtEventDesc(UAS_ALARM_LOG_ADD_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CAlarmLogQryReq: public CQueryReqBase
{
public:
    CAlarmLogQryReq()
    {
        event = OspExtEventDesc(UAS_ALARM_LOG_QRY_REQ);
    }
private:
    CAlarmLogInfoQueryCondition condition;             
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetCondition(const CAlarmLogInfoQueryCondition& cCondition)
    {
        condition = cCondition;
    }
    CAlarmLogInfoQueryCondition& GetCondition()
    {
        return condition;
    }
    const CAlarmLogInfoQueryCondition& GetCondition() const
    {
        return condition;
    }

};

class CAlarmLogQryRsp: public CQueryRspBase
{
public:
    CAlarmLogQryRsp()
    {
        event = OspExtEventDesc(UAS_ALARM_LOG_QRY_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CAlarmLogQryNtfReq: public CQueryNtfReqBase
{
public:
    CAlarmLogQryNtfReq()
    {
        event = OspExtEventDesc(UAS_ALARM_LOG_QRY_NTF_REQ);
    }
private:
    vector<TAlarmLogInfo> infos;             
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetInfos(const vector<TAlarmLogInfo>& cInfos)
    {
        infos = cInfos;
    }
    vector<TAlarmLogInfo>& GetInfos()
    {
        return infos;
    }
    const vector<TAlarmLogInfo>& GetInfos() const
    {
        return infos;
    }

};

class CUserCustomDataAddReq: public CEventReq
{
public:
    CUserCustomDataAddReq()
    {
        event = OspExtEventDesc(UAS_CUSER_CUSTOMDATA_ADD_REQ);
    }

private:
    TUserCustomDataInfo info;              
    bool lastFrag;               // 最后一个分片标志位；最后一个分片时，该值为真；uas收到最后一个分片时才进行数据写操作 
    string dataTag;              // 数据标签；标识一次自定义数据操作，一次操作的不同分片使用相同的标签. 
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetInfo(const TUserCustomDataInfo& cInfo)
    {
        info = cInfo;
    }
    TUserCustomDataInfo& GetInfo()
    {
        return info;
    }
    const TUserCustomDataInfo& GetInfo() const
    {
        return info;
    }

    void SetLastFrag(bool bLastFrag)
    {
        lastFrag = bLastFrag;
    }
    bool GetLastFrag() const
    {
        return lastFrag;
    }

    void SetDataTag(const string& strDataTag)
    {
        dataTag = strDataTag;
    }
    string& GetDataTag()
    {
        return dataTag;
    }
    const string& GetDataTag() const
    {
        return dataTag;
    }

};

class CUserCustomDataAddRsp: public CEventRsp
{
public:
    CUserCustomDataAddRsp()
    {
        event = OspExtEventDesc(UAS_CUSER_CUSTOMDATA_ADD_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUserCustomDataAddNtfReq: public CEventReq
{
public:
    CUserCustomDataAddNtfReq()
    {
        event = OspExtEventDesc(UAS_CUSER_CUSTOMDATA_ADD_NTF_REQ);
    }
private:
    CUserCustomDataInfoKey key;              // 由于用户自定义数据的数据比较大，因此只通知一个key
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetKey(const CUserCustomDataInfoKey& cKey)
    {
        key = cKey;
    }
    CUserCustomDataInfoKey& GetKey()
    {
        return key;
    }
    const CUserCustomDataInfoKey& GetKey() const
    {
        return key;
    }

};

class CUserCustomDataDelReq: public CEventReq
{
public:
    CUserCustomDataDelReq()
    {
        event = OspExtEventDesc(UAS_CUSER_CUSTOMDATA_DEL_REQ);
    }
private:
    CUserCustomDataInfoKey key;              
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetKey(const CUserCustomDataInfoKey& cKey)
    {
        key = cKey;
    }
    CUserCustomDataInfoKey& GetKey()
    {
        return key;
    }
    const CUserCustomDataInfoKey& GetKey() const
    {
        return key;
    }

};

class CUserCustomDataDelRsp: public CEventRsp
{
public:
    CUserCustomDataDelRsp()
    {
        event = OspExtEventDesc(UAS_CUSER_CUSTOMDATA_DEL_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUserCustomDataDelNtfReq: public CEventReq
{
public:
    CUserCustomDataDelNtfReq()
    {
        event = OspExtEventDesc(UAS_CUSER_CUSTOMDATA_DEL_NTF_REQ);
    }
private:
    CUserCustomDataInfoKey key;              
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetKey(const CUserCustomDataInfoKey& cKey)
    {
        key = cKey;
    }
    CUserCustomDataInfoKey& GetKey()
    {
        return key;
    }
    const CUserCustomDataInfoKey& GetKey() const
    {
        return key;
    }

};

class CUserCustomDataQryReq: public CQueryReqBase
{
public:
    CUserCustomDataQryReq()
    {
        event = OspExtEventDesc(UAS_CUSER_CUSTOMDATA_QRY_REQ);
    }
private:
    CUserCustomDataInfoQueryCondition condition;              
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetCondition(const CUserCustomDataInfoQueryCondition& cCondition)
    {
        condition = cCondition;
    }
    CUserCustomDataInfoQueryCondition& GetCondition()
    {
        return condition;
    }
    const CUserCustomDataInfoQueryCondition& GetCondition() const
    {
        return condition;
    }

};

class CUserCustomDataQryRsp: public CQueryRspBase
{
public:
    CUserCustomDataQryRsp()
    {
        event = OspExtEventDesc(UAS_CUSER_CUSTOMDATA_QRY_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUserCustomDataQryNtfReq: public CQueryNtfReqBase
{
public:
    CUserCustomDataQryNtfReq()
    {
        event = OspExtEventDesc(UAS_CUSER_CUSTOMDATA_QRY_NTF_REQ);
    }
private:
    vector<TUserCustomDataInfo> infos;              
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetInfos(const vector<TUserCustomDataInfo>& cInfos)
    {
        infos = cInfos;
    }
    vector<TUserCustomDataInfo>& GetInfos()
    {
        return infos;
    }
    const vector<TUserCustomDataInfo>& GetInfos() const
    {
        return infos;
    }

};


class CVideosourceBasiccfgAddReq: public CEventReq
{
public:
    CVideosourceBasiccfgAddReq()
    {
        event = OspExtEventDesc(UAS_VIDEOSOURCE_BASICCFG_ADD_REQ);
    }

private:
    TVideosourceBasicCfgInfo info;              
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetInfo(const TVideosourceBasicCfgInfo& cInfo)
    {
        info = cInfo;
    }
    TVideosourceBasicCfgInfo& GetInfo()
    {
        return info;
    }
    const TVideosourceBasicCfgInfo& GetInfo() const
    {
        return info;
    }

};

class CVideosourceBasiccfgAddRsp: public CEventRsp
{
public:
    CVideosourceBasiccfgAddRsp()
    {
        event = OspExtEventDesc(UAS_VIDEOSOURCE_BASICCFG_ADD_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CVideosourceBasiccfgAddNtfReq: public CEventReq
{
public:
    CVideosourceBasiccfgAddNtfReq()
    {
        event = OspExtEventDesc(UAS_VIDEOSOURCE_BASICCFG_ADD_NTF_REQ);
    }
private:
    TVideosourceBasicCfgInfo info;              

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetInfo(const TVideosourceBasicCfgInfo& cInfo)
    {
        info = cInfo;
    }
    TVideosourceBasicCfgInfo& GetInfo()
    {
        return info;
    }
    const TVideosourceBasicCfgInfo& GetInfo() const
    {
        return info;
    }

};

class CVideosourceBasiccfgDelReq: public CEventReq
{
public:
    CVideosourceBasiccfgDelReq()
    {
        event = OspExtEventDesc(UAS_VIDEOSOURCE_BASICCFG_DEL_REQ);
    }
private:
    CVideosourceBasicCfgInfoKey key;                           
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetKey(const CVideosourceBasicCfgInfoKey& cKey)
    {
        key = cKey;
    }
    CVideosourceBasicCfgInfoKey& GetKey()
    {
        return key;
    }
    const CVideosourceBasicCfgInfoKey& GetKey() const
    {
        return key;
    }

};

class CVideosourceBasiccfgDelRsp: public CEventRsp
{
public:
    CVideosourceBasiccfgDelRsp()
    {
        event = OspExtEventDesc(UAS_VIDEOSOURCE_BASICCFG_DEL_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CVideosourceBasiccfgDelNtfReq: public CEventReq
{
public:
    CVideosourceBasiccfgDelNtfReq()
    {
        event = OspExtEventDesc(UAS_VIDEOSOURCE_BASICCFG_DEL_NTF_REQ);
    }
private:
    CVideosourceBasicCfgInfoKey key;                           
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetKey(const CVideosourceBasicCfgInfoKey& cKey)
    {
        key = cKey;
    }
    CVideosourceBasicCfgInfoKey& GetKey()
    {
        return key;
    }
    const CVideosourceBasicCfgInfoKey& GetKey() const
    {
        return key;
    }

};

class CVideosourceBasiccfgQryReq: public CQueryReqBase
{
public:
    CVideosourceBasiccfgQryReq()
    {
        event = OspExtEventDesc(UAS_VIDEOSOURCE_BASICCFG_QRY_REQ);
    }
private:
    CVideosourceBasicCfgInfoQueryCondition condition;                         
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetCondition(const CVideosourceBasicCfgInfoQueryCondition& cCondition)
    {
        condition = cCondition;
    }
    CVideosourceBasicCfgInfoQueryCondition& GetCondition()
    {
        return condition;
    }
    const CVideosourceBasicCfgInfoQueryCondition& GetCondition() const
    {
        return condition;
    }

};

class CVideosourceBasiccfgQryRsp: public CQueryRspBase
{
public:
    CVideosourceBasiccfgQryRsp()
    {
        event = OspExtEventDesc(UAS_VIDEOSOURCE_BASICCFG_QRY_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CVideosourceBasiccfgQryNtfReq: public CQueryNtfReqBase
{
public:
    CVideosourceBasiccfgQryNtfReq()
    {
        event = OspExtEventDesc(UAS_VIDEOSOURCE_BASICCFG_QRY_NTF_REQ);
    }
private:
    vector<TVideosourceBasicCfgInfo> infos;                           
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetInfos(const vector<TVideosourceBasicCfgInfo>& cInfos)
    {
        infos = cInfos;
    }
    vector<TVideosourceBasicCfgInfo>& GetInfos()
    {
        return infos;
    }
    const vector<TVideosourceBasicCfgInfo>& GetInfos() const
    {
        return infos;
    }

};

class CVideosourcePresetcfgAddReq: public CEventReq
{
public:
    CVideosourcePresetcfgAddReq()
    {
        event = OspExtEventDesc(UAS_VIDEOSOURCE_PRESETCFG_ADD_REQ);
    }

private:
    TVideosourcePresetCfgInfo info; // 注意：预置位别名，一次只能添加一个             
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetInfo(const TVideosourcePresetCfgInfo& cInfo)
    {
        info = cInfo;
    }
    TVideosourcePresetCfgInfo& GetInfo()
    {
        return info;
    }
    const TVideosourcePresetCfgInfo& GetInfo() const
    {
        return info;
    }

};

class CVideosourcePresetcfgAddRsp: public CEventRsp
{
public:
    CVideosourcePresetcfgAddRsp()
    {
        event = OspExtEventDesc(UAS_VIDEOSOURCE_PRESETCFG_ADD_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CVideosourcePresetcfgAddNtfReq: public CEventReq
{
public:
    CVideosourcePresetcfgAddNtfReq()
    {
        event = OspExtEventDesc(UAS_VIDEOSOURCE_PRESETCFG_ADD_NTF_REQ);
    }
private:
    TVideosourcePresetCfgInfo info;              

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetInfo(const TVideosourcePresetCfgInfo& cInfo)
    {
        info = cInfo;
    }
    TVideosourcePresetCfgInfo& GetInfo()
    {
        return info;
    }
    const TVideosourcePresetCfgInfo& GetInfo() const
    {
        return info;
    }

};

class CVideosourcePresetcfgDelReq: public CEventReq
{
public:
    CVideosourcePresetcfgDelReq()
    {
        event = OspExtEventDesc(UAS_VIDEOSOURCE_PRESETCFG_DEL_REQ);
    }
private:
    CVideosourcePresetCfgInfoKey key;                           
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetKey(const CVideosourcePresetCfgInfoKey& cKey)
    {
        key = cKey;
    }
    CVideosourcePresetCfgInfoKey& GetKey()
    {
        return key;
    }
    const CVideosourcePresetCfgInfoKey& GetKey() const
    {
        return key;
    }

};

class CVideosourcePresetcfgDelRsp: public CEventRsp
{
public:
    CVideosourcePresetcfgDelRsp()
    {
        event = OspExtEventDesc(UAS_VIDEOSOURCE_PRESETCFG_DEL_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CVideosourcePresetcfgDelNtfReq: public CEventReq
{
public:
    CVideosourcePresetcfgDelNtfReq()
    {
        event = OspExtEventDesc(UAS_VIDEOSOURCE_PRESETCFG_DEL_NTF_REQ);
    }
private:
    CVideosourcePresetCfgInfoKey key;                           
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetKey(const CVideosourcePresetCfgInfoKey& cKey)
    {
        key = cKey;
    }
    CVideosourcePresetCfgInfoKey& GetKey()
    {
        return key;
    }
    const CVideosourcePresetCfgInfoKey& GetKey() const
    {
        return key;
    }

};

class CVideosourcePresetcfgQryReq: public CQueryReqBase
{
public:
    CVideosourcePresetcfgQryReq()
    {
        event = OspExtEventDesc(UAS_VIDEOSOURCE_PRESETCFG_QRY_REQ);
    }
private:
    CVideosourcePresetCfgInfoQueryCondition condition;                         
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetCondition(const CVideosourcePresetCfgInfoQueryCondition& cCondition)
    {
        condition = cCondition;
    }
    CVideosourcePresetCfgInfoQueryCondition& GetCondition()
    {
        return condition;
    }
    const CVideosourcePresetCfgInfoQueryCondition& GetCondition() const
    {
        return condition;
    }

};

class CVideosourcePresetcfgQryRsp: public CQueryRspBase
{
public:
    CVideosourcePresetcfgQryRsp()
    {
        event = OspExtEventDesc(UAS_VIDEOSOURCE_PRESETCFG_QRY_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CVideosourcePresetcfgQryNtfReq: public CQueryNtfReqBase
{
public:
    CVideosourcePresetcfgQryNtfReq()
    {
        event = OspExtEventDesc(UAS_VIDEOSOURCE_PRESETCFG_QRY_NTF_REQ);
    }
private:
    vector<TVideosourcePresetCfgInfo> infos;                           
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetInfos(const vector<TVideosourcePresetCfgInfo>& cInfos)
    {
        infos = cInfos;
    }
    vector<TVideosourcePresetCfgInfo>& GetInfos()
    {
        return infos;
    }
    const vector<TVideosourcePresetCfgInfo>& GetInfos() const
    {
        return infos;
    }

};

class CConfigUasSetReq: public CEventReq
{
public:
    CConfigUasSetReq()
    {
        event = OspExtEventDesc(UAS_CONFIG_UAS_SET_REQ);
    }
    bool GetUserLogSavedMaxDays(int &val) const
    {
        if (!optionField.HasOption("userLogSavedMaxDays"))
            return false;
        val = userLogSavedMaxDays;
        return true;
    }
    void SetUserLogSavedMaxDays(int val)
    {
        userLogSavedMaxDays = val;
        optionField.SetOption("userLogSavedMaxDays");
    }
    bool GetUserLogSavedMaxNumber(int &val) const
    {
        if (!optionField.HasOption("userLogSavedMaxNumber"))
            return false;
        val = userLogSavedMaxNumber;
        return true;
    }
    void SetUserLogSavedMaxNumber(int val)
    {
        userLogSavedMaxNumber = val;
        optionField.SetOption("userLogSavedMaxNumber");
    }
    bool GetDeviceLogSavedMaxNumber(int &val) const
    {
        if (!optionField.HasOption("deviceLogSavedMaxNumber"))
            return false;
        val = deviceLogSavedMaxNumber;
        return true;
    }
    void SetDeviceLogSavedMaxNumber(int val)
    {
        deviceLogSavedMaxNumber = val;
        optionField.SetOption("deviceLogSavedMaxNumber");
    }
    bool GetDeviceLogSavedMaxDays(int &val) const
    {
        if (!optionField.HasOption("deviceLogSavedMaxDays"))
            return false;
        val = deviceLogSavedMaxDays;
        return true;
    }
    void SetDeviceLogSavedMaxDays(int val)
    {
        deviceLogSavedMaxDays = val;
        optionField.SetOption("deviceLogSavedMaxDays");
    }
    bool GetAlarmLogSavedMaxNumber(int &val) const
    {
        if (!optionField.HasOption("alarmLogSavedMaxNumber"))
            return false;
        val = alarmLogSavedMaxNumber;
        return true;
    }
    void SetAlarmLogSavedMaxNumber(int val)
    {
        alarmLogSavedMaxNumber = val;
        optionField.SetOption("alarmLogSavedMaxNumber");
    }
    bool GetAlarmLogSavedMaxDays(int &val) const
    {
        if (!optionField.HasOption("alarmLogSavedMaxDays"))
            return false;
        val = alarmLogSavedMaxDays;
        return true;
    }
    void SetAlarmLogSavedMaxDays(int val)
    {
        alarmLogSavedMaxDays = val;
        optionField.SetOption("alarmLogSavedMaxDays");
    }
    bool GetUserLogEnabled(bool &val) const
    {
        if (!optionField.HasOption("userLogEnabled"))
            return false;
        val = userLogEnabled;
        return true;
    }
    void SetUserLogEnabled(bool val)
    {
        userLogEnabled = val;
        optionField.SetOption("userLogEnabled");
    }
    bool GetDeviceLogEnabled(bool &val) const
    {
        if (!optionField.HasOption("deviceLogEnabled"))
            return false;
        val = deviceLogEnabled;
        return true;
    }
    void SetDeviceLogEnabled(bool val)
    {
        deviceLogEnabled = val;
        optionField.SetOption("deviceLogEnabled");
    }
private:
    CModOptionField optionField; // 修改字段标识

    int userLogSavedMaxDays;	 // 最大的用户日志保存天数
    int userLogSavedMaxNumber;	 // 最大的用户日志保存条数
    int deviceLogSavedMaxNumber; // 最大的设备日志保存条数
    int deviceLogSavedMaxDays;	 // 最大的设备日志保存天数
    int alarmLogSavedMaxNumber;	 // 最大的告警日志保存条数
    int alarmLogSavedMaxDays;	 // 最大的告警日志保存天数

    bool userLogEnabled;         // 用户日志启停标志
    bool deviceLogEnabled;       // 设备日志启停标志
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CConfigUasSetRsp: public CEventRsp
{
public:
    CConfigUasSetRsp()
    {
        event = OspExtEventDesc(UAS_CONFIG_UAS_SET_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CConfigUasGetReq: public CEventReq
{
public:
    CConfigUasGetReq()
    {
        event = OspExtEventDesc(UAS_CONFIG_UAS_GET_REQ);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CConfigUasGetRsp: public CEventRsp
{
public:
    CConfigUasGetRsp()
    {
        event = OspExtEventDesc(UAS_CONFIG_UAS_GET_RSP);
    }
private:
    int userLogSavedMaxDays;	  // 最大的用户日志保存天数
    int userLogSavedMaxNumber;	  // 最大的用户日志保存条数
    int deviceLogSavedMaxNumber;  // 最大的设备日志保存条数
    int deviceLogSavedMaxDays;	  // 最大的设备日志保存天数
    int alarmLogSavedMaxNumber;	  // 最大的告警日志保存条数
    int alarmLogSavedMaxDays;	  // 最大的告警日志保存天数

    bool userLogEnabled;          // 用户日志启停标志
    bool deviceLogEnabled;        // 设备日志启停标志
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetUserLogSavedMaxDays(int nUserLogSavedMaxDays)
    {
        userLogSavedMaxDays = nUserLogSavedMaxDays;
    }
    int GetUserLogSavedMaxDays() const
    {
        return userLogSavedMaxDays;
    }

    void SetUserLogSavedMaxNumber(int nUserLogSavedMaxNumber)
    {
        userLogSavedMaxNumber = nUserLogSavedMaxNumber;
    }
    int GetUserLogSavedMaxNumber() const
    {
        return userLogSavedMaxNumber;
    }

    void SetDeviceLogSavedMaxNumber(int nDeviceLogSavedMaxNumber)
    {
        deviceLogSavedMaxNumber = nDeviceLogSavedMaxNumber;
    }
    int GetDeviceLogSavedMaxNumber() const
    {
        return deviceLogSavedMaxNumber;
    }

    void SetDeviceLogSavedMaxDays(int nDeviceLogSavedMaxDays)
    {
        deviceLogSavedMaxDays = nDeviceLogSavedMaxDays;
    }
    int GetDeviceLogSavedMaxDays() const
    {
        return deviceLogSavedMaxDays;
    }

    void SetAlarmLogSavedMaxNumber(int nAlarmLogSavedMaxNumber)
    {
        alarmLogSavedMaxNumber = nAlarmLogSavedMaxNumber;
    }
    int GetAlarmLogSavedMaxNumber() const
    {
        return alarmLogSavedMaxNumber;
    }

    void SetAlarmLogSavedMaxDays(int nAlarmLogSavedMaxDays)
    {
        alarmLogSavedMaxDays = nAlarmLogSavedMaxDays;
    }
    int GetAlarmLogSavedMaxDays() const
    {
        return alarmLogSavedMaxDays;
    }

    void SetUserLogEnabled(bool bUserLogEnabled)
    {
        userLogEnabled = bUserLogEnabled;
    }
    bool GetUserLogEnabled() const
    {
        return userLogEnabled;
    }

    void SetDeviceLogEnabled(bool bDeviceLogEnabled)
    {
        deviceLogEnabled = bDeviceLogEnabled;
    }
    bool GetDeviceLogEnabled() const
    {
        return deviceLogEnabled;
    }

};

#endif // _UAS_PROTO_H_



