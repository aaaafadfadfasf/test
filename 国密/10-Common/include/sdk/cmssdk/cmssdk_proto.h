/*****************************************************************************
   模块名      : CMSSDK
   文件名      : cmssdk_proto.h
   相关文件    : 
   文件实现功能: 监控平台2.0接入协议
   作者        : fanxg
   版本        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人      修改内容
   2013/09/12  1.0         fanxg        创建
******************************************************************************/

#ifndef __CMS_SDK_PROTO_H__
#define __CMS_SDK_PROTO_H__

#include "cmssdk_errorcode.h"
#include "cmssdk_event.h"
#include "cmssdk_struct.h"

//协议定义
class  CCmsEvent
{
public:
    virtual ~CCmsEvent(){}

    virtual const std::string ToXml()const = 0;
    virtual void FromXml(const std::string& xml_string) = 0;
    virtual void ToXml(TiXmlDocument& xmlDoc)const = 0;
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const = 0;
    virtual void FromXml(const TiXmlNode& xmlNode) = 0;
};

class  CCmsEventReq: public CCmsEvent
{
public:
    CCmsEventReq()
    {
        event = 0;
        seqNum = 0;
    }

	~CCmsEventReq()
	{

	}

    void Clear()
    {
        //event和结构体是绑定死的，故不用重新分配
        seqNum = 0;  //重新分配流水号
        session = STRING_NULL;
		userMonitorId.clear();
    }

    //从req获取消息头
    void SetHeadFromReq(const CCmsEventReq& cEvent)
    {
        session = cEvent.GetSession();
        seqNum = cEvent.GetSeqNum();
		userMonitorId = cEvent.GetUserMonitorId();
    }

protected:
    int event;       //事件id      
    int seqNum;      //事件流水号        
    string session;  //会话ID    	 
	string userMonitorId;//用户的400id	
public:
    void SetEvent(int nEvent)
    {		
        event = nEvent;
    }
    int GetEvent() const
    {
        return event;
    }

    void SetSeqNum(int nSeqNum)
    {
        seqNum = nSeqNum;
    }
    int GetSeqNum() const
    {
        return seqNum;
    }

    void SetSession(const string& strSession)
    {
        session = strSession;
    }
    string& GetSession()
    {
        return session;
    }
    const string& GetSession() const
    {
        return session;
    }
	void SetUserMonitorId(const string& strUserMonitorId)
	{
		userMonitorId = strUserMonitorId;
	}

	const string& GetUserMonitorId() const 
	{
		return userMonitorId;
	}
	string& GetUserMonitorId()
	{
		return userMonitorId;
	}

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsEventRsp: public CCmsEvent
{
public:
    CCmsEventRsp()
    {
        event = 0;
        seqNum = 0;
        errorCode = 0;
    }  

	~CCmsEventRsp()
	{

	}

    void Clear()
    {
        //event和结构体是绑定死的，故不用重新分配
        seqNum = 0;
        session = STRING_NULL;
        errorCode = 0;
		userMonitorId.clear();
    }

    //从req获取消息头
    void SetHeadFromReq(const CCmsEventReq& cEvent)
    {
        session = cEvent.GetSession();
        seqNum = cEvent.GetSeqNum();
		userMonitorId = cEvent.GetUserMonitorId();
    }

protected:
    int event;             //事件id     
    int seqNum;            //事件流水号  
    string session;        //会话ID          
    int errorCode;         //错误码    
	string userMonitorId;
public:
    void SetEvent(int nEvent)
    {
        event = nEvent;
    }
    int GetEvent() const
    {
        return event;
    }

    void SetSeqNum(int nSeqNum)
    {
        seqNum = nSeqNum;
    }
    int GetSeqNum() const
    {
        return seqNum;
    }

    void SetSession(const string& strSession)
    {
        session = strSession;
    }
    string& GetSession()
    {
        return session;
    }
    const string& GetSession() const
    {
        return session;
    }

    void SetErrorCode(int nErrorCode)
    {
        errorCode = nErrorCode;
    }
    int GetErrorCode() const
    {
        return errorCode;
    }
	void SetUserMonitorId(const string& strUserMonitorId)
	{
		userMonitorId = strUserMonitorId;
	}

	const string& GetUserMonitorId() const
	{
		return userMonitorId;
	}
	string& GetUserMonitorId()
	{
		return userMonitorId;
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsDevRegReq : public CCmsEventReq
{
private:
    TCmsPuConfig config; 
	string       usedPuiIp;

public:
    CCmsDevRegReq()
    {
        Clear();
        SetEvent(CMS_DEV_REG_REQ);
    }

    ~CCmsDevRegReq()
    {
        Clear();
    }

    void Clear()
    {
        config.Clear();
		usedPuiIp = STRING_NULL;
    }
public:
    void SetConfig(const TCmsPuConfig& cConfig)
    {
        config = cConfig;
    }
    TCmsPuConfig& GetConfig()
    {
        return config;
    }
    const TCmsPuConfig& GetConfig() const
    {
        return config;
    }

	void SetUsedPuiIp(const string& strIp)
	{
		usedPuiIp = strIp;
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
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsDevRegRsp : public CCmsEventRsp
{
public:
    CCmsDevRegRsp()
    {
        Clear();
        SetEvent(CMS_DEV_REG_RSP);
    }

    ~CCmsDevRegRsp()
    {
        Clear();
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

class  CCmsCmuDisconnectNtfReq : public CCmsEventReq
{
public:
	CCmsCmuDisconnectNtfReq()
	{
		Clear();
		SetEvent(CMS_CMU_DISCONNECT_NTF_REQ);
	}

	~CCmsCmuDisconnectNtfReq()
	{
		Clear();
	}

	void Clear()
	{
		cmuUri = STRING_NULL;
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

class  CCmsCmuDisconnectNtfRsp : public CCmsEventRsp
{
public:
	CCmsCmuDisconnectNtfRsp()
	{
		Clear();
		SetEvent(CMS_CMU_DISCONNECT_NTF_RSP);
	}

	~CCmsCmuDisconnectNtfRsp()
	{
		Clear();
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

class  CCmsDevUnRegReq : public CCmsEventReq
{
private:
    string devUri;               
public:
    CCmsDevUnRegReq()
    {
        Clear();
        SetEvent(CMS_DEV_UNREG_REQ);
    }

    ~CCmsDevUnRegReq()
    {
        Clear();
    }

    void Clear()
    {
        devUri = STRING_NULL;
    }
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

class  CCmsDevUnRegRsp : public CCmsEventRsp
{
public:
    CCmsDevUnRegRsp()
    {
        Clear();
        SetEvent(CMS_DEV_UNREG_RSP);
    }

    ~CCmsDevUnRegRsp()
    {
        Clear();
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

//无应答通知
class  CCmsDevOfflineNtf : public CCmsEventReq
{
private:
    string devUri;               
	CCmsPuChnStateList chnStatusSet;         //通道状态集
public:
    CCmsDevOfflineNtf()
    {
        Clear();
        SetEvent(CMS_DEV_OFFLINE_NTF);
    }

    ~CCmsDevOfflineNtf()
    {
        Clear();
    }

    void Clear()
    {
        devUri = STRING_NULL;
    }
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

	const CCmsPuChnStateList &GetChnStatusSet() const
	{
		return chnStatusSet;
	}
	CCmsPuChnStateList &GetChnStatusSet()
	{
		return chnStatusSet;
	}
	void SetChnStatusSet(const CCmsPuChnStateList &cChnStateSet)
	{
		chnStatusSet = cChnStateSet;
	}

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

//无应答通知
class  CCmsDevOnlineNtf : public CCmsEventReq
{
private:
    string devUri;               
	CCmsPuChnStateList chnStatusSet;         //通道状态集
public:
    CCmsDevOnlineNtf()
    {
        Clear();
        SetEvent(CMS_DEV_ONLINE_NTF);
    }

    ~CCmsDevOnlineNtf()
    {
        Clear();
    }

    void Clear()
    {
        devUri = STRING_NULL;
    }
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

	const CCmsPuChnStateList &GetChnStatusSet() const
	{
		return chnStatusSet;
	}
	CCmsPuChnStateList &GetChnStatusSet()
	{
		return chnStatusSet;
	}
	void SetChnStatusSet(const CCmsPuChnStateList &cChnStateSet)
	{
		chnStatusSet = cChnStateSet;
	}

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


//无应答通知
class  CCmsDevAlarmNtf : public CCmsEventReq
{
private:
    TCmsAlarmInput alarmInput;                       //告警信息
public:
    CCmsDevAlarmNtf()
    {
        Clear();
        SetEvent(CMS_DEV_ALARM_NTF);
    }

    ~CCmsDevAlarmNtf()
    {
        Clear();
    }

    void Clear()
    {
        alarmInput.Clear();
    }
public:
    void SetAlarmInput(const TCmsAlarmInput& cAlarmInput)
    {
        alarmInput = cAlarmInput;
    }
    TCmsAlarmInput& GetAlarmInput()
    {
        return alarmInput;
    }
    const TCmsAlarmInput& GetAlarmInput() const
    {
        return alarmInput;
    }

	const string& GetDevUri() const
	{
		return alarmInput.GetDevUri();
	}

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsSubscribeGpsReq : public CCmsEventReq
{
public:
	CCmsSubscribeGpsReq()
	{
		Clear();
		SetEvent(CMS_SUBSCRIBE_GPS_REQ);
	}

	void Clear()
	{
		userUri = STRING_NULL;
		devUri = STRING_NULL;
		param.Clear();
	}

private:
	string              userUri;                  //操作用户
	string              devUri;                   //操作设备
	TCmsGpsSsParam      param;                    //GPS订阅参数
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

	void SetParam(const TCmsGpsSsParam& cParam)
	{
		param = cParam;
	}
	TCmsGpsSsParam& GetParam()
	{
		return param;
	}
	const TCmsGpsSsParam& GetParam() const
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

class  CCmsSubscribeGpsRsp : public CCmsEventRsp
{
public:
	CCmsSubscribeGpsRsp()
	{
		Clear();
		SetEvent(CMS_SUBSCRIBE_GPS_RSP);
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

class  CCmsDevGpsDataNty : public CCmsEventReq
{
private:
	string           devUri;							//pu uuid@domain
	TCmsGpsData      data;                              //gps数据
public:
	CCmsDevGpsDataNty()
	{
		Clear();
		SetEvent(CMS_DEV_GPS_DATA_NTF);
	}

	~CCmsDevGpsDataNty()
	{
		Clear();
	}

	void Clear()
	{
		data.Clear();
	}
public:
	void SetDevUri(const string &strUri)
	{
		devUri = strUri;
	}
	const string GetDevUri() const
	{
		return devUri;
	}
	string &GetDevUri()
	{
		return devUri;
	}

	void SetData(const TCmsGpsData& cData)
	{
		data = cData;
	}
	TCmsGpsData& GetData()
	{
		return data;
	}
	const TCmsGpsData& GetData() const
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


class  CCmsDevInfoModNtf : public CCmsEventReq
{
public:
	CCmsDevInfoModNtf()
	{
		SetEvent(CMS_DEV_INFO_MOD_NTF);
		Clear();
	}

	~CCmsDevInfoModNtf()
	{

	}

	void Clear()
	{
		devURI = STRING_NULL;
		config.Clear();
	}

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

	const TCmsPuDevConfig& GetConfig() const
	{
		return config;
	}
	TCmsPuDevConfig& GetConfig()
	{
		return config;
	}
	void SetConfig(const TCmsPuDevConfig& tConfig)
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
	TCmsPuDevConfig     config;       //前端属性、能力集、配置
};

class  CCmsPuVidEncParamSetReq : public CCmsEventReq
{
public:
	CCmsPuVidEncParamSetReq()
	{
		SetEvent(CMS_PU_VID_ENC_PARAM_SET_REQ);
		Clear();
	}

	~CCmsPuVidEncParamSetReq()
	{

	}

	void Clear()
	{
		userUri = STRING_NULL;
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

	const TCmsChannel& GetDevChn() const
	{
		return devChn;
	}

	TCmsChannel& GetDevChn()
	{
		return devChn;
	}

	void SetDevChn(const TCmsChannel& tDevChn)
	{
		devChn = tDevChn;
	}

	const TCmsVideoEncodeParam& GetVidEncParam() const
	{
		return param;
	}

	TCmsVideoEncodeParam& GetVidEncParam()
	{
		return param;
	}

	void SetVidEncParam(const TCmsVideoEncodeParam& tVidEncParam)
	{
		param = tVidEncParam;
	}

	const string& GetDevUri() const
	{
		return devChn.GetDevUri();
	}

private:
	string				 userUri;       //操作用户
	TCmsChannel			 devChn;        //设备通道
	TCmsVideoEncodeParam param;         //视频编码参数
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsPuVidEncParamSetRsp : public CCmsEventRsp
{
public:
	CCmsPuVidEncParamSetRsp()
	{
		SetEvent(CMS_PU_VID_ENC_PARAM_SET_RSP);
		Clear();
	}

	~CCmsPuVidEncParamSetRsp()
	{

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

class  CCmsPuVidEncParamGetReq : public CCmsEventReq
{
public:
	CCmsPuVidEncParamGetReq()
	{
		SetEvent(CMS_PU_VID_ENC_PARAM_GET_REQ);
		Clear();
	}

	~CCmsPuVidEncParamGetReq()
	{

	}

	void Clear()
	{
		userUri = STRING_NULL;
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

	const TCmsChannel& GetDevChn() const
	{
		return devChn;
	}

	TCmsChannel& GetDevChn()
	{
		return devChn;
	}

	void SetDevChn(const TCmsChannel& tDevChn)
	{
		devChn = tDevChn;
	}

	const string& GetDevUri() const
	{
		return devChn.GetDevUri();
	}

private:
	string             userUri;   //操作用户
	TCmsChannel        devChn;    //设备通道

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsPuVidEncParamGetRsp : public CCmsEventRsp
{
public:
	CCmsPuVidEncParamGetRsp()
	{
		SetEvent(CMS_PU_VID_ENC_PARAM_GET_RSP);
		Clear();
	}

	~CCmsPuVidEncParamGetRsp()
	{

	}

	void Clear()
	{
		param.Clear();
		surportedMediaType.Clear();
	}

public:
	const TCmsVideoEncodeParam& GetVidEncParam() const
	{
		return param;
	}

	TCmsVideoEncodeParam& GetVidEncParam()
	{
		return param;
	}

	void SetVidEncParam(const TCmsVideoEncodeParam& tVidEncParam)
	{
		param = tVidEncParam;
	}

	const TCmsSupportMediaType& GetSupportedMediaType() const 
	{
		return surportedMediaType;
	}

	TCmsSupportMediaType& GetSupportedMediaType() 
	{
		return surportedMediaType;
	}

	void SetSupportedMediaType(const TCmsSupportMediaType& tSupportedMediaType) 
	{
		surportedMediaType = tSupportedMediaType;
	}

private:
	TCmsVideoEncodeParam  param;               //当前视频编码参数
	TCmsSupportMediaType  surportedMediaType;  //所有支持的媒体格式
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

//高级编码参数设置
class  CCmsPuVidEncParamSetExReq : public CCmsEventReq
{
public:
    CCmsPuVidEncParamSetExReq()
    {
        Clear();
        SetEvent(CMS_PU_VID_ENC_PARAM_SET_EX_REQ);
    }

    void Clear()
    {
        userUri = STRING_NULL;
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

    const TCmsChannel& GetDevChn() const
    {
        return devChn;
    }

    TCmsChannel& GetDevChn()
    {
        return devChn;
    }

    void SetDevChn(const TCmsChannel& tDevChn)
    {
        devChn = tDevChn;
    }

    const TCmsVideoEncodeParam& GetVidEncParam() const
    {
        return param;
    }

    TCmsVideoEncodeParam& GetVidEncParam()
    {
        return param;
    }

    void SetVidEncParam(const TCmsVideoEncodeParam& tVidEncParam)
    {
        param = tVidEncParam;
    }

    //事务模板会使用
    const string& GetDevUri() const
    {
        return devChn.GetDevUri();
    }

private:
    string             userUri;       //操作用户
    TCmsChannel           devChn;        //设备通道
    TCmsVideoEncodeParam  param;         //视频编码参数
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsPuVidEncParamSetExRsp : public CCmsEventRsp
{
public:
    CCmsPuVidEncParamSetExRsp()
    {
        SetEvent(CMS_PU_VID_ENC_PARAM_SET_EX_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsPuVidEncParamGetExReq : public CCmsEventReq
{
public:
    CCmsPuVidEncParamGetExReq()
    {
        Clear();
        SetEvent(CMS_PU_VID_ENC_PARAM_GET_EX_REQ);
    }

    void Clear()
    {
        userUri = STRING_NULL;
        devChn.Clear();
    }

private:
    string             userUri;   //操作用户
    TCmsChannel           devChn;    //设备通道

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

    void SetDevChn(const TCmsChannel& cDevChn)
    {
        devChn = cDevChn;
    }
    TCmsChannel& GetDevChn()
    {
        return devChn;
    }
    const TCmsChannel& GetDevChn() const
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

class  CCmsPuVidEncParamGetExRsp : public CCmsEventRsp
{
public:
    CCmsPuVidEncParamGetExRsp()
    {
        Clear();
        SetEvent(CMS_PU_VID_ENC_PARAM_GET_EX_RSP);
    }

    void Clear()
    {
        param.Clear();
        surportedMediaType.Clear();
        CCmsEventRsp::Clear();
    }

public:
    const TCmsVideoEncodeParam& GetVidEncParam() const
    {
        return param;
    }

    TCmsVideoEncodeParam& GetVidEncParam()
    {
        return param;
    }

    void SetVidEncParam(const TCmsVideoEncodeParam& tVidEncParam)
    {
        param = tVidEncParam;
    }

    const TCmsSupportMediaType& GetSupportedMediaType() const 
    {
        return surportedMediaType;
    }

    TCmsSupportMediaType& GetSupportedMediaType() 
    {
        return surportedMediaType;
    }

    void SetSupportedMediaType(const TCmsSupportMediaType& tSupportedMediaType) 
    {
        surportedMediaType = tSupportedMediaType;
    }

private:
    TCmsVideoEncodeParam  param;               //当前视频编码参数
    TCmsSupportMediaType  surportedMediaType;  //所有支持的媒体格式
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class  CCmsPuAudEncParamSetReq : public CCmsEventReq
{
public:
	CCmsPuAudEncParamSetReq()
	{
		Clear();
		SetEvent(CMS_PU_AUD_ENC_PARAM_SET_REQ);
	}

	~CCmsPuAudEncParamSetReq()
	{
		Clear();
	}

	void Clear()
	{
		userUri = STRING_NULL;
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
	string				  userUri;                  //操作用户
	TCmsChannel           devChn;                   //设备通道
	TCmsAudioEncodeParam  param;                    //视频编码参数
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

	void SetDevChn(const TCmsChannel& cDevChn)
	{
		devChn = cDevChn;
	}
	TCmsChannel& GetDevChn()
	{
		return devChn;
	}
	const TCmsChannel& GetDevChn() const
	{
		return devChn;
	}

	void SetParam(const TCmsAudioEncodeParam& cParam)
	{
		param = cParam;
	}
	TCmsAudioEncodeParam& GetParam()
	{
		return param;
	}
	const TCmsAudioEncodeParam& GetParam() const
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

class  CCmsPuAudEncParamSetRsp : public CCmsEventRsp
{
public:
	CCmsPuAudEncParamSetRsp()
	{
		Clear();
		SetEvent(CMS_PU_AUD_ENC_PARAM_SET_RSP);
	}

	~CCmsPuAudEncParamSetRsp()
	{
		Clear();
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

class  CCmsPuAudEncParamGetReq : public CCmsEventReq
{
public:
	CCmsPuAudEncParamGetReq()
	{
		Clear();
		SetEvent(CMS_PU_AUD_ENC_PARAM_GET_REQ);
	}

	~CCmsPuAudEncParamGetReq()
	{
		Clear();
	}

	void Clear()
	{
		userUri = STRING_NULL;
		devChn.Clear();
	}
	//事务模板会使用
	const string& GetDevUri() const
	{
		return devChn.GetDevUri();
	}

private:
	string             userUri;                  //操作用户
	TCmsChannel        devChn;                   //设备通道

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

	void SetDevChn(const TCmsChannel& cDevChn)
	{
		devChn = cDevChn;
	}
	TCmsChannel& GetDevChn()
	{
		return devChn;
	}
	const TCmsChannel& GetDevChn() const
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

class  CCmsPuAudEncParamGetRsp : public CCmsEventRsp
{
public:
	CCmsPuAudEncParamGetRsp()
	{
		Clear();
		SetEvent(CMS_PU_AUD_ENC_PARAM_GET_RSP);
	}

	~CCmsPuAudEncParamGetRsp()
	{
		Clear();
	}

	void Clear()
	{
		param.Clear();
		surportedMediaType.Clear();
	}

private:
	TCmsAudioEncodeParam  param;                              //当前音频编码参数
	TCmsSupportMediaType  surportedMediaType;                 //所有支持的媒体格式
public:
	void SetParam(const TCmsAudioEncodeParam& cParam)
	{
		param = cParam;
	}
	TCmsAudioEncodeParam& GetParam()
	{
		return param;
	}
	const TCmsAudioEncodeParam& GetParam() const
	{
		return param;
	}

	void SetSurportedMediaType(const TCmsSupportMediaType& cSurportedMediaType)
	{
		surportedMediaType = cSurportedMediaType;
	}
	TCmsSupportMediaType& GetSurportedMediaType()
	{
		return surportedMediaType;
	}
	const TCmsSupportMediaType& GetSurportedMediaType() const
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

class  CCmsPuTransparentOperateReq : public CCmsEventReq
{
public:
	CCmsPuTransparentOperateReq()
	{
		SetEvent(CMS_PU_TRANSPARENT_OPERATE_REQ);
		Clear();
	}

	~CCmsPuTransparentOperateReq()
	{

	}

	void Clear()
	{
		srvName = STRING_NULL;
		userUri = STRING_NULL;
		devChn.Clear();
		msgType = 0;
		data = STRING_NULL;
	}
public:
	void SetSrvName(const TCmsPuTransOprType& strSrvName)
	{
		srvName = strSrvName;
	}
	TCmsPuTransOprType& GetSrvName()
	{
		return srvName;
	}
	const TCmsPuTransOprType& GetSrvName() const
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

	void SetDevChn(const TCmsChannel& cDevChn)
	{
		devChn = cDevChn;
	}
	TCmsChannel& GetDevChn()
	{
		return devChn;
	}
	const TCmsChannel& GetDevChn() const
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

private:
	TCmsPuTransOprType	srvName;			//业务类型
    string				userUri;			//操作用户
	TCmsChannel			devChn;				//设备通道
	int                 msgType;            //子消息类型
	string				data;				//base64格式的透明数据
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsPuTransparentOperateRsp : public CCmsEventRsp
{
public:
	CCmsPuTransparentOperateRsp()
	{
		Clear();
		SetEvent(CMS_PU_TRANSPARENT_OPERATE_RSP);
	}

	~CCmsPuTransparentOperateRsp()
	{
		Clear();
	}

	void Clear()
	{
		msgType = 0;
		data = STRING_NULL;
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

class  CCmsPuTransdataCfgSetReq : public CCmsEventReq
{
public:
	CCmsPuTransdataCfgSetReq()
	{
		Clear();
		SetEvent(CMS_PU_TRANSDATA_CFG_SET_REQ);
	}

	~CCmsPuTransdataCfgSetReq()
	{
		Clear();
	}

	void Clear()
	{
		userUri = STRING_NULL;
		devUri = STRING_NULL;
		param.Clear();
	}

private:
	string                 userUri;                  //操作用户
	string                 devUri;                   //操作设备
	TCmsTransDataCfgParam  param;                    //配置参数
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

	void SetParam(const TCmsTransDataCfgParam& cParam)
	{
		param = cParam;
	}
	TCmsTransDataCfgParam& GetParam()
	{
		return param;
	}
	const TCmsTransDataCfgParam& GetParam() const
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

class  CCmsPuTransdataCfgSetRsp : public CCmsEventRsp
{
public:
	CCmsPuTransdataCfgSetRsp()
	{
		Clear();
		SetEvent(CMS_PU_TRANSDATA_CFG_SET_RSP);
	}

	~CCmsPuTransdataCfgSetRsp()
	{
		Clear();
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

class  CCmsPuTransdataCfgGetReq : public CCmsEventReq
{
public:
	CCmsPuTransdataCfgGetReq()
	{
		Clear();
		SetEvent(CMS_PU_TRANSDATA_CFG_GET_REQ);
	}

	~CCmsPuTransdataCfgGetReq()
	{
		Clear();
	}

	void Clear()
	{
		userUri = STRING_NULL;
		devUri = STRING_NULL;
	}

private:
	string              userUri;                  //操作用户
	string              devUri;                   //操作设备
	TCmsTransDataCfgParam  param;                 //配置参数
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

	void SetParam(const TCmsTransDataCfgParam& cParam)
	{
		param = cParam;
	}
	TCmsTransDataCfgParam& GetParam()
	{
		return param;
	}
	const TCmsTransDataCfgParam& GetParam() const
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

class  CCmsPuTransdataCfgGetRsp : public CCmsEventRsp
{
public:
	CCmsPuTransdataCfgGetRsp()
	{
		Clear();
		SetEvent(CMS_PU_TRANSDATA_CFG_GET_RSP);
	}

	~CCmsPuTransdataCfgGetRsp()
	{
		Clear();
	}

	void Clear()
	{
		param.Clear();
	}

private:
	TCmsTransDataCfgParam        param;                //配置参数
public:
	void SetParam(const TCmsTransDataCfgParam& cParam)
	{
		param = cParam;
	}
	TCmsTransDataCfgParam& GetParam()
	{
		return param;
	}
	const TCmsTransDataCfgParam& GetParam() const
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

class  CCmsPuBatchNtyReq : public CCmsEventReq
{
public:
	CCmsPuBatchNtyReq()
	{
		Clear();
		SetEvent(CMS_PU_BATCH_NTF_REQ);
	}

	void Clear()
	{
		data.clear();
	}

	const vector<TCmsGeneralData> &GetData() const
	{
		return data;
	}
	vector<TCmsGeneralData> &GetData()
	{
		return data;
	}
	void SetData(const vector<TCmsGeneralData> &val)
	{
		data = val;
	}

private:
	vector<TCmsGeneralData> data;			// 对于批量通知来说，TCmsGeneralData的type为TCmsSsType，TGeneralData的content为对应ntf的序列化结果
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsPuBatchNtyRsp : public CCmsEventRsp
{
public:
	CCmsPuBatchNtyRsp()
	{
		Clear();
		SetEvent(CMS_PU_BATCH_NTF_RSP);
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

class  CCmsRealMediaInviteReq: public CCmsEventReq
{
public:
	CCmsRealMediaInviteReq()
	{
		SetEvent(CMS_REALMEDIA_INVITE_REQ);
		Clear();
	}

	~CCmsRealMediaInviteReq()
	{

	}

	void Clear()
	{
		srcChn.Clear();
		dstChn.Clear();
		sdp.Clear();
	}

	//srcChn
	const TCmsChannel& GetSrcChn() const 
	{
		return srcChn;
	}
	TCmsChannel& GetSrcChn() 
	{
		return srcChn;
	}
	void SetSrcChn(const TCmsChannel& tSrcChn) 
	{
		srcChn = tSrcChn;
	}

	//dstChn
	const TCmsChannel& GetDstChn() const 
	{
		return dstChn;
	}
	TCmsChannel& GetDstChn() 
	{
		return dstChn;
	}
	void SetDstChn(const TCmsChannel& tDstChn) 
	{
		dstChn = tDstChn;
	}

	//sdp
	const TCmsSdp& GetSdp() const
	{
		return sdp;
	}
	TCmsSdp& GetSdp()
	{
		return sdp;
	}
	void SetSdp(const TCmsSdp& var)
	{
		sdp = var;
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);

private:
	TCmsChannel    srcChn;     //平台放像时，为RCS uri。
	TCmsChannel    dstChn;
	TCmsSdp        sdp;
};

class  CCmsRealMediaInviteRsp: public CCmsEventRsp
{
public:
	CCmsRealMediaInviteRsp()
	{
		SetEvent(CMS_REALMEDIA_INVITE_RSP);
		Clear();
	}

	~CCmsRealMediaInviteRsp()
	{

	}

	void Clear()
	{
		sdp.Clear();
	}
	//sdp
	const TCmsSdp& GetSdp() const
	{
		return sdp;
	}
	TCmsSdp& GetSdp()
	{
		return sdp;
	}
	void SetSdp(const TCmsSdp& var)
	{
		sdp = var;
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);

private:
	TCmsSdp        sdp;
};

class  CCmsRealMediaInviteAck: public CCmsEventReq
{
public:
	CCmsRealMediaInviteAck()
	{
		SetEvent(CMS_REALMEDIA_INVITE_ACK);
	}

	~CCmsRealMediaInviteAck()
	{

	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);

};

class  CCmsRealMediaByeReq: public CCmsEventReq
{
public:
	CCmsRealMediaByeReq()
	{
		Clear();
		SetEvent(CMS_REALMEDIA_BYE_REQ);
	}

	~CCmsRealMediaByeReq()
	{

	}

	void Clear()
	{
		reason = 0;  //默认是0，对应CMS_SUCCESS，表示正常bye
	}

	void SetReason(int nReason)
	{
		reason = nReason;
	}
	int GetReason() const
	{
		return reason;
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);

private:
	int reason;
};

class  CCmsRealMediaByeRsp: public CCmsEventRsp
{
public:
	CCmsRealMediaByeRsp()
	{
		SetEvent(CMS_REALMEDIA_BYE_RSP);
	}

	~CCmsRealMediaByeRsp()
	{

	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};


class  CCmsKeyFrameSendReq: public CCmsEventReq
{
public:
	CCmsKeyFrameSendReq()
	{
		SetEvent(CMS_KEYFREAM_SEND_REQ);
	}

	~CCmsKeyFrameSendReq()
	{

	}
	void Clear()
	{
		devChn.Clear();
	}

	void SetDevChn(const TCmsChannel& cDevChn)
	{
		devChn = cDevChn;
	}
	TCmsChannel& GetDevChn()
	{
		return devChn;
	}
	const TCmsChannel& GetDevChn() const
	{
		return devChn;
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);

private:
	TCmsChannel    devChn;
};

class  CCmsKeyFrameSendRsp : public CCmsEventRsp
{
public:
	CCmsKeyFrameSendRsp()
	{
		SetEvent(CMS_KEYFREAM_SEND_RSP);
	}

	~CCmsKeyFrameSendRsp()
	{

	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsPtzCtrlReq: public CCmsEventReq
{
public:
	CCmsPtzCtrlReq()
	{
		SetEvent(CMS_PTZ_CTRL_REQ);
	}

	~CCmsPtzCtrlReq()
	{

	}
	void Clear()
	{
		userName = STRING_NULL;
		devChn.Clear();
		camCtrl.Clear();
	}

	void SetName(const string &strUserName)
	{
		userName = strUserName;
	}

	string& GetName()
	{
		return userName;
	}

	const string& GetName() const
	{
		return userName;
	}

	void SetDevChn(const TCmsChannel& cDevChn)
	{
		devChn = cDevChn;
	}
	TCmsChannel& GetDevChn()
	{
		return devChn;
	}
	const TCmsChannel& GetDevChn() const
	{
		return devChn;
	}

	void SetCamCtrl(const TCmsCamCtrl& cCamCtrl)
	{
		camCtrl = cCamCtrl;
	}

	TCmsCamCtrl& GetCamCtrl()
	{
		return camCtrl;
	}

	const TCmsCamCtrl& GetCamCtrl() const
	{
		return camCtrl;
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);

private:
	string				userName;   //cu登录用户名, 例如 ligang@nanjing
	TCmsChannel         devChn;    //设备通道
	TCmsCamCtrl         camCtrl;   //控制信息
};

class  CCmsPtzCtrlRsp : public CCmsEventRsp
{
public:
	CCmsPtzCtrlRsp()
	{
		SetEvent(CMS_PTZ_CTRL_RSP);
	}

	~CCmsPtzCtrlRsp()
	{

	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsStartAudioCallReq: public CCmsEventReq
{
public:
	CCmsStartAudioCallReq()
	{
		SetEvent(CMS_START_AUDIO_CALL_REQ);
		Clear();
	}

	~CCmsStartAudioCallReq()
	{

	}
	void Clear()
	{
		srcChn.Clear();
		dstChn.Clear();
		sdp.Clear();
	}

	//srcChn
	const TCmsChannel& GetSrcChn() const 
	{
		return srcChn;
	}
	TCmsChannel& GetSrcChn() 
	{
		return srcChn;
	}
	void SetSrcChn(const TCmsChannel& tSrcChn) 
	{
		srcChn = tSrcChn;
	}

	//dstChn
	const TCmsChannel& GetDstChn() const 
	{
		return dstChn;
	}
	TCmsChannel& GetDstChn() 
	{
		return dstChn;
	}
	void SetDstChn(const TCmsChannel& tDstChn) 
	{
		dstChn = tDstChn;
	}

	//sdp
	const TCmsSdp& GetSdp() const
	{
		return sdp;
	}
	TCmsSdp& GetSdp()
	{
		return sdp;
	}
	void SetSdp(const TCmsSdp& var)
	{
		sdp = var;
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
public:
	TCmsChannel       srcChn;     
	TCmsChannel       dstChn;
	TCmsSdp           sdp;	//cu sdp
};

class  CCmsStartAudioCallRsp: public CCmsEventRsp
{
public:
	CCmsStartAudioCallRsp()
	{
		SetEvent(CMS_START_AUDIO_CALL_RSP);
		Clear();
	}

	~CCmsStartAudioCallRsp()
	{

	}
	void Clear()
	{
		sdp.Clear();
	}

	//sdp
	const TCmsSdp& GetSdp() const
	{
		return sdp;
	}
	TCmsSdp& GetSdp()
	{
		return sdp;
	}
	void SetSdp(const TCmsSdp& var)
	{
		sdp = var;
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
public:
	TCmsSdp           sdp;	//vtdu sdp
};

class  CCmsStopAudioCallReq: public CCmsEventReq
{
public:
	CCmsStopAudioCallReq()
	{
		SetEvent(CMS_STOP_AUDIO_CALL_REQ);
		Clear();
	}

	~CCmsStopAudioCallReq()
	{

	}
	void Clear()
	{
		srcChn.Clear();
		dstChn.Clear();
	}

	//srcChn
	const TCmsChannel& GetSrcChn() const 
	{
		return srcChn;
	}
	TCmsChannel& GetSrcChn() 
	{
		return srcChn;
	}
	void SetSrcChn(const TCmsChannel& tSrcChn) 
	{
		srcChn = tSrcChn;
	}

	//dstChn
	const TCmsChannel& GetDstChn() const 
	{
		return dstChn;
	}
	TCmsChannel& GetDstChn() 
	{
		return dstChn;
	}
	void SetDstChn(const TCmsChannel& tDstChn) 
	{
		dstChn = tDstChn;
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
public:
	TCmsChannel       srcChn;     
	TCmsChannel       dstChn;
};

class  CCmsStopAudioCallRsp: public CCmsEventRsp
{
public:
	CCmsStopAudioCallRsp()
	{
		SetEvent(CMS_STOP_AUDIO_CALL_RSP);
	}

	~CCmsStopAudioCallRsp()
	{

	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsAudioCallDestoryNtfReq: public CCmsEventReq
{
public:
	CCmsAudioCallDestoryNtfReq()
	{
		Clear();
		SetEvent(CMS_AUDIO_CALL_DESTORY_NTF_REQ);
	}

	~CCmsAudioCallDestoryNtfReq()
	{
		Clear();
	}

	void Clear()
	{
		reason = CMS_SDK_SUCCESS;		//默认是CMS_SDK_SUCCESS，表示正常拆除
	}

	void SetReason(int nReason)
	{
		reason = nReason;
	}
	int GetReason() const
	{
		return reason;
	}

	const TCmsChannel &GetSrcChn() const
	{
		return srcChn;
	}

	void SetSrcChn(const TCmsChannel &tSrcChn)
	{
		srcChn = tSrcChn;
	}

	const TCmsChannel &GetDstChn() const
	{
		return dstChn;
	}

	void SetDstChn(const TCmsChannel &tDstChn)
	{
		dstChn = tDstChn;
	}

private:
	int  reason;                    //拆除交换的原因
	TCmsChannel srcChn;				//交换的源通道
	TCmsChannel dstChn;				//交换的目的通道
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsAudioCallDestoryNtfRsp: public CCmsEventRsp
{
public:
	CCmsAudioCallDestoryNtfRsp()
	{
		SetEvent(CMS_AUDIO_CALL_DESTORY_NTF_RSP);
	}

	~CCmsAudioCallDestoryNtfRsp()
	{
		Clear();
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsAudioCallInviteReq: public CCmsEventReq
{
public:
	CCmsAudioCallInviteReq()
	{
		SetEvent(CMS_AUDIOCALL_INVITE_REQ);
		Clear();
	}

	~CCmsAudioCallInviteReq()
	{

	}
	void Clear()
	{
		srcChn.Clear();
		dstChn.Clear();
		sdp.Clear();
	}

	//srcChn
	const TCmsChannel& GetSrcChn() const 
	{
		return srcChn;
	}
	TCmsChannel& GetSrcChn() 
	{
		return srcChn;
	}
	void SetSrcChn(const TCmsChannel& tSrcChn) 
	{
		srcChn = tSrcChn;
	}

	//dstChn
	const TCmsChannel& GetDstChn() const 
	{
		return dstChn;
	}
	TCmsChannel& GetDstChn() 
	{
		return dstChn;
	}
	void SetDstChn(const TCmsChannel& tDstChn) 
	{
		dstChn = tDstChn;
	}

	//sdp
	const TCmsSdp& GetSdp() const
	{
		return sdp;
	}
	TCmsSdp& GetSdp()
	{
		return sdp;
	}
	void SetSdp(const TCmsSdp& var)
	{
		sdp = var;
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);

private:
	TCmsChannel    srcChn;     //平台放像时，为RCS uri。
	TCmsChannel    dstChn;
	TCmsSdp           sdp;
};

class  CCmsAudioCallInviteRsp: public CCmsEventRsp
{
public:
	CCmsAudioCallInviteRsp()
	{
		SetEvent(CMS_AUDIOCALL_INVITE_RSP);
	}

	~CCmsAudioCallInviteRsp()
	{

	}
	//sdp
	const TCmsSdp& GetSdp() const
	{
		return sdp;
	}
	TCmsSdp& GetSdp()
	{
		return sdp;
	}
	void SetSdp(const TCmsSdp& var)
	{
		sdp = var;
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);

private:
	TCmsSdp        sdp;
};

class  CCmsAudioCallInviteAck: public CCmsEventReq
{
public:
	CCmsAudioCallInviteAck()
	{
		SetEvent(CMS_AUDIOCALL_INVITE_ACK);
	}

	~CCmsAudioCallInviteAck()
	{

	}
	//sdp
	const TCmsSdp& GetSdp() const
	{
		return sdp;
	}
	TCmsSdp& GetSdp()
	{
		return sdp;
	}
	void SetSdp(const TCmsSdp& var)
	{
		sdp = var;
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);

private:

	TCmsSdp           sdp;
};

class  CCmsAudioCallByeReq: public CCmsEventReq
{
public:
	CCmsAudioCallByeReq()
	{
		Clear();
		SetEvent(CMS_AUDIOCALL_BYE_REQ);
	}

	~CCmsAudioCallByeReq()
	{

	}

	void Clear()
	{
		reason = 0;  //默认是0，对应CMS_SUCCESS，表示正常bye
	}

	void SetReason(int nReason)
	{
		reason = nReason;
	}
	int GetReason() const
	{
		return reason;
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);

private:
	int  reason;                    //bye对话的原因
};

class  CCmsAudioCallByeRsp: public CCmsEventRsp
{
public:
	CCmsAudioCallByeRsp()
	{
		SetEvent(CMS_AUDIOCALL_BYE_RSP);
	}

	~CCmsAudioCallByeRsp()
	{

	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsBroadcastStartAudioCallReq: public CCmsEventReq
{
public:
	CCmsBroadcastStartAudioCallReq()
	{
		SetEvent(CMS_BROADCAST_START_AUDIO_CALL_REQ);
		Clear();
	}

	~CCmsBroadcastStartAudioCallReq()
	{

	}
	void Clear()
	{
		srcChn.Clear();
		dstChn.Clear();
	}

	//srcChn
	const TCmsChannel& GetSrcChn() const 
	{
		return srcChn;
	}
	TCmsChannel& GetSrcChn() 
	{
		return srcChn;
	}
	void SetSrcChn(const TCmsChannel& tSrcChn) 
	{
		srcChn = tSrcChn;
	}

	//dstChn
	const TCmsChannel& GetDstChn() const 
	{
		return dstChn;
	}
	TCmsChannel& GetDstChn() 
	{
		return dstChn;
	}
	void SetDstChn(const TCmsChannel& tDstChn) 
	{
		dstChn = tDstChn;
	}

	const TCmsSdp& GetSdp() const
	{
		return sdp;
	}
	TCmsSdp& GetSdp()
	{
		return sdp;
	}
	void SetSdp(const TCmsSdp& var)
	{
		sdp = var;
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
public:
	TCmsChannel       srcChn;     
	TCmsChannel       dstChn;
	TCmsSdp           sdp;
};

class  CCmsBroadcastStartAudioCallRsp: public CCmsEventRsp
{
public:
	CCmsBroadcastStartAudioCallRsp()
	{
		SetEvent(CMS_BROADCAST_START_AUDIO_CALL_RSP);
		Clear();
	}

	~CCmsBroadcastStartAudioCallRsp()
	{

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

class  CCmsBroadcastStopAudioCallReq: public CCmsEventReq
{
public:
	CCmsBroadcastStopAudioCallReq()
	{
		SetEvent(CMS_BROADCAST_STOP_AUDIO_CALL_REQ);
		Clear();
	}

	~CCmsBroadcastStopAudioCallReq()
	{
		Clear();
	}
	void Clear()
	{
		srcChn.Clear();
		dstChn.Clear();
	}

	//srcChn
	const TCmsChannel& GetSrcChn() const 
	{
		return srcChn;
	}
	TCmsChannel& GetSrcChn() 
	{
		return srcChn;
	}
	void SetSrcChn(const TCmsChannel& tSrcChn) 
	{
		srcChn = tSrcChn;
	}

	//dstChn
	const TCmsChannel& GetDstChn() const 
	{
		return dstChn;
	}
	TCmsChannel& GetDstChn() 
	{
		return dstChn;
	}
	void SetDstChn(const TCmsChannel& tDstChn) 
	{
		dstChn = tDstChn;
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
public:
	TCmsChannel       srcChn;     
	TCmsChannel       dstChn;
};

class  CCmsBroadcastStopAudioCallRsp: public CCmsEventRsp
{
public:
	CCmsBroadcastStopAudioCallRsp()
	{
		SetEvent(CMS_BROADCAST_STOP_AUDIO_CALL_RSP);
	}

	~CCmsBroadcastStopAudioCallRsp()
	{

	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsBroadcastAudioCallInviteReq: public CCmsEventReq
{
public:
	CCmsBroadcastAudioCallInviteReq()
	{
		SetEvent(CMS_BROADCAST_AUDIOCALL_INVITE_REQ);
		Clear();
	}

	~CCmsBroadcastAudioCallInviteReq()
	{

	}
	void Clear()
	{
		srcChn.Clear();
		dstChn.Clear();
		sdp.Clear();
	}

	//srcChn
	const TCmsChannel& GetSrcChn() const 
	{
		return srcChn;
	}
	TCmsChannel& GetSrcChn() 
	{
		return srcChn;
	}
	void SetSrcChn(const TCmsChannel& tSrcChn) 
	{
		srcChn = tSrcChn;
	}

	//dstChn
	const TCmsChannel& GetDstChn() const 
	{
		return dstChn;
	}
	TCmsChannel& GetDstChn() 
	{
		return dstChn;
	}
	void SetDstChn(const TCmsChannel& tDstChn) 
	{
		dstChn = tDstChn;
	}

	//sdp
	const TCmsSdp& GetSdp() const
	{
		return sdp;
	}
	TCmsSdp& GetSdp()
	{
		return sdp;
	}
	void SetSdp(const TCmsSdp& var)
	{
		sdp = var;
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);

private:
	TCmsChannel    srcChn;     //平台放像时，为RCS uri。
	TCmsChannel    dstChn;
	TCmsSdp           sdp;
};

class  CCmsBroadcastAudioCallInviteRsp: public CCmsEventRsp
{
public:
	CCmsBroadcastAudioCallInviteRsp()
	{
		SetEvent(CMS_BROADCAST_AUDIOCALL_INVITE_RSP);
	}

	~CCmsBroadcastAudioCallInviteRsp()
	{

	}
	//sdp
	const TCmsSdp& GetSdp() const
	{
		return sdp;
	}
	TCmsSdp& GetSdp()
	{
		return sdp;
	}
	void SetSdp(const TCmsSdp& var)
	{
		sdp = var;
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);

private:
	TCmsSdp        sdp;
};

class  CCmsBroadcastAudioCallInviteAck: public CCmsEventRsp
{
public:
	CCmsBroadcastAudioCallInviteAck()
	{
		SetEvent(CMS_BROADCAST_AUDIOCALL_INVITE_ACK);
	}

	~CCmsBroadcastAudioCallInviteAck()
	{

	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsBroadcastAudioCallByeReq: public CCmsEventReq
{
public:
	CCmsBroadcastAudioCallByeReq()
	{
		Clear();
		SetEvent(CMS_BROADCAST_AUDIOCALL_BYE_REQ);
	}

	~CCmsBroadcastAudioCallByeReq()
	{

	}

	void Clear()
	{
		reason = 0;  //默认是0，对应CMS_SUCCESS，表示正常bye
	}

	void SetReason(int nReason)
	{
		reason = nReason;
	}
	int GetReason() const
	{
		return reason;
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);

private:
	int  reason;                    //bye对话的原因
};

class  CCmsBroadcastAudioCallByeRsp: public CCmsEventRsp
{
public:
	CCmsBroadcastAudioCallByeRsp()
	{
		SetEvent(CMS_BROADCAST_AUDIOCALL_BYE_RSP);
	}

	~CCmsBroadcastAudioCallByeRsp()
	{

	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsRecQueryReq: public CCmsEventReq 
{
public:
	CCmsRecQueryReq()
	{
		SetEvent(CMS_REC_QUERY_REQ);
		Clear();
	}

	~CCmsRecQueryReq()
	{
		Clear();
	}
	void Clear()
	{
		startEntryIndex = 0;
		expectEntryNum = 0;
		mediaType = CMS_ALL_REC_MEDIA;
		
		optionFields.Clear();
		devChn.Clear();
		puRecType = STRING_NULL;
		platRecType = STRING_NULL;
		timeRange.Clear();

		extInfo.clear();
        timeout = 35;       // sip信令默认的超时时间
	}

private:
	int				      startEntryIndex;				 //表示从第startEntryIndex条数开始获取(不包括startEntryIndex)，startEntryIndex从0开始，当它为0时同时返回符合条件的记录总数
	int				      expectEntryNum;				 //表示期望得到的查询记录总数，0表示查询者不限制返回的记录总数
	TCmsRecMediaType      mediaType;
    int                   timeout;                       //超时时间（秒）
	
	CCmsOptionField       optionFields;
	string                domain;						 //域名，如果设置了该值且不是本域，则会登录异域rcs查询平台录像，对前端录像查询无意义
	TCmsChannel           devChn;						 //设备通道
	TCmsPuRecType         puRecType;					 //录像查询条件, 根据vsip定义
	TCmsPlatRecType       platRecType;					 //平台录像类型
	TCmsTimeRange         timeRange;					 //时间段
	TCmsRecLockState      lockState;					 //锁定状态，目前仅支持平台录像查询
	TCmsExtInfo           extInfo;                       //扩展信息

public:

	void SetStartEntryIndex(int index)
	{
		startEntryIndex = index;
	}
	
	int GetStartEntryIndex() const
	{
		return startEntryIndex;
	}
	
	void SetExpectEntryNum(int num)
	{
		expectEntryNum = num;
	}
	
	int GetExpectEntryNum() const
	{
		return expectEntryNum;
	}
	
	void SetMediaType(const TCmsRecMediaType &tQryType)
	{
		mediaType = tQryType;
	}

	const TCmsRecMediaType &GetMediaType() const
	{
		return mediaType;
	}

    void SetTimeout(int nTimeout)
    {
        timeout = nTimeout;
    }

    int GetTimeout()
    {
        return timeout;
    }

	void SetDomain(const string& strDomain)
	{
		domain = strDomain;
		optionFields.SetOption("domain");
	}
	bool GetDomain(string& strDomain) const
	{
		if (!optionFields.HasOption("domain"))
			return false;
		strDomain = domain;
		return true;
	}
	
	void SetDevChn(const TCmsChannel& tDevChn)
	{
		devChn = tDevChn;
		optionFields.SetOption("devChn");
	}
	bool GetDevChn(TCmsChannel& tDevChn) const
	{
		if (!optionFields.HasOption("devChn"))
			return false;
		tDevChn = devChn;
		return true;
	}
	
	void SetPuRecType(const TCmsPuRecType& tRecType)
	{
		puRecType = tRecType;
		optionFields.SetOption("puRecType");
	}
	bool GetPuRecType(TCmsPuRecType &tRecType) const
	{
		if (!optionFields.HasOption("puRecType"))
			return false;
		tRecType = puRecType;
		return true;
	}

	void SetPlatRecType(const TCmsPlatRecType& tRecType)
	{
		platRecType = tRecType;
		optionFields.SetOption("platRecType");
	}
	bool GetPlatRecType(TCmsPlatRecType &tRecType) const
	{
		if (!optionFields.HasOption("platRecType"))
			return false;
		tRecType = platRecType;
		return true;
	}
	
	void SetTimeRange(const TCmsTimeRange& cTimeRange)
	{
		timeRange = cTimeRange;
		optionFields.SetOption("timeRange");
	}
	bool GetTimeRange(TCmsTimeRange &tTimeRange) const
	{
		if (!optionFields.HasOption("timeRange"))
			return false;
		tTimeRange = timeRange;
		return true;
	}

	void SetLockState(const TCmsRecLockState &tRecLockState)
	{
		lockState = tRecLockState;
		optionFields.SetOption("lockState");
	}
	bool GetLockState(TCmsRecLockState &tRecLockState) const
	{
		if (!optionFields.HasOption("lockState"))
			return false;
		tRecLockState = lockState;
		return true;
	}

	//tExtInf
	void SetExtInfo(const TCmsExtInfo& tExtInf)
	{
		extInfo = tExtInf;
	}
	TCmsExtInfo& GetExtInfo()
	{
		return extInfo;
	}
	const TCmsExtInfo& GetExtInfo() const
	{
		return extInfo;
	}

	void SetExtInfo(const string& strKey, const string& strValue)
	{
		extInfo[strKey] = strValue;
	}
	void SetExtInfo(const string& strKey, int nValue)
	{
		char buf[256]={0};
		sprintf( buf, "%d", nValue );
		extInfo[strKey] = buf;
	}
	void SetExtInfo(const string& strKey, double lfValue)
	{
		char buf[256]={0};
		sprintf( buf, "%lf", lfValue );
		extInfo[strKey] = buf;
	}

	void SetExtInfo(const string& strKey, bool bValue)
	{
		extInfo[strKey] = bValue? "true" : "false";
	}

	bool GetExtInfo(const string& strKey, string& strValue) const
	{
		TCmsExtInfo::const_iterator itr = extInfo.find(strKey);
		if (itr != extInfo.end())
		{
			strValue = itr->second;

			return true;
		}

		return false;
	}
	bool GetExtInfo(const string& strKey, int& nValue) const
	{
		TCmsExtInfo::const_iterator itr = extInfo.find(strKey);
		if (itr != extInfo.end())
		{
			nValue = atoi(itr->second.c_str());

			return true;
		}

		return false;
	}
	bool GetExtInfo(const string& strKey, double& lfValue) const
	{
		TCmsExtInfo::const_iterator itr = extInfo.find(strKey);
		if (itr != extInfo.end())
		{
			lfValue = atof(itr->second.c_str());

			return true;
		}

		return false;
	}

	bool GetExtInfo(const string& strKey, bool& bValue) const
	{
		TCmsExtInfo::const_iterator itr = extInfo.find(strKey);
		if (itr != extInfo.end())
		{
			bValue = (itr->second == "true");

			return true;
		}

		return false;
	}

	//TCmsExtInfo细化接口
	void SetGBRecLocation(const string& var)
	{
		extInfo[CMS_SDP_EXT_INFO_GB_REC_LOCATION] = var;
	}
	string GetGBRecLocation() const
	{
		TCmsExtInfo::const_iterator itr = extInfo.find(CMS_SDP_EXT_INFO_GB_REC_LOCATION);
		if (itr != extInfo.end())
		{
			return itr->second;
		}

		return "";
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


class  CCmsRecQueryRsp:public CCmsEventRsp
{
public:
	CCmsRecQueryRsp()
	{
		SetEvent(CMS_REC_QUERY_RSP);
		Clear();
	}

	~CCmsRecQueryRsp()
	{
		Clear();
	}

	void Clear()
	{
		totalEntryNum = 0;
	}

	int GetTotalEntryNum() const
	{
		return totalEntryNum;
	}
	void SetTotalEntryNum(int nTotalEntry)
	{
		totalEntryNum = nTotalEntry;
	}

	int GetEntryNum() const
	{
		return entryNum;
	}

	void SetEntryNum(int nEntryNum)
	{
		entryNum = nEntryNum;
	}

private:
	int totalEntryNum;			// 查询结果总记录数
	int entryNum;				// 查询通知中会返回的记录总数
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsRecQueryNtfReq : public CCmsEventReq
{
public:
	CCmsRecQueryNtfReq()
	{
		SetEvent(CMS_REC_QUERY_NTF_REQ);
		Clear();
	}

	~CCmsRecQueryNtfReq()
	{
		Clear();
	}

	void Clear()
	{
		entryNum = 0;
		lastNtf  = false;
		puRecInfoList.clear();
		platRecInfoList.clear();
	}
	int GetEntryNum() const
	{
		return entryNum;
	}

	void SetEntryNum(int nEntryNum)
	{
		entryNum = nEntryNum;
	}
	
	bool IsLastNtf() const
	{
		return lastNtf;
	}

	void SetLastNtf(bool bLastNtf)
	{
		lastNtf = bLastNtf;
	}

	TCmsPuRecInfoList& GetPuRecInfoList()
	{
		return puRecInfoList;
	}
	const TCmsPuRecInfoList& GetPuRecInfoList() const
	{
		return puRecInfoList;
	}

	void SetPuRecInfoList(const TCmsPuRecInfoList &cPuRecInfoList)
	{
		puRecInfoList = cPuRecInfoList;
	}

	TCmsRecDurationList& GetPlatRecInfoList()
	{
		return platRecInfoList;
	}
	const TCmsRecDurationList& GetPlatRecInfoList() const
	{
		return platRecInfoList;
	}

	void SetPlatRecInfoList(const TCmsRecDurationList &cPlatRecInfoList)
	{
		platRecInfoList = cPlatRecInfoList;
	}

private:
	int  entryNum;								//本次通知返回的结果数量
	bool lastNtf;								//标志着是否最后一条通知
	TCmsPuRecInfoList   puRecInfoList;          //前端录像查询结果
	TCmsRecDurationList platRecInfoList;        //平台录像查询结果
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsRecQueryNtfRsp : public CCmsEventRsp
{
public:
	CCmsRecQueryNtfRsp()
	{
		SetEvent(CMS_REC_QUERY_NTF_RSP);
		Clear();
	}

	~CCmsRecQueryNtfRsp()
	{
		Clear();
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

class  CCmsRecPlayInviteReq: public CCmsEventReq
{

public:
	CCmsRecPlayInviteReq()
	{
		SetEvent(CMS_REC_PLAY_INVITE_REQ);
		Clear();
	}

	~CCmsRecPlayInviteReq()
	{

	}
	void Clear()
	{
		srcChn.Clear();
		dstChn.Clear();
		sdp.Clear();
		mediaType = CMS_PU_REC_MEDIA;
		domain = STRING_NULL;
	}

	//srcChn
	const TCmsChannel& GetSrcChn() const 
	{
		return srcChn;
	}
	TCmsChannel& GetSrcChn() 
	{
		return srcChn;
	}
	void SetSrcChn(const TCmsChannel& tSrcChn) 
	{
		srcChn = tSrcChn;
	}

	//dstChn
	const TCmsChannel& GetDstChn() const 
	{
		return dstChn;
	}
	TCmsChannel& GetDstChn() 
	{
		return dstChn;
	}
	void SetDstChn(const TCmsChannel& tDstChn) 
	{
		dstChn = tDstChn;
	}

	//sdp
	const TCmsSdp& GetSdp() const
	{
		return sdp;
	}
	TCmsSdp& GetSdp()
	{
		return sdp;
	}
	void SetSdp(const TCmsSdp& var)
	{
		sdp = var;
	}

	void SetMediaType(const TCmsRecMediaType &tQryType)
	{
		mediaType = tQryType;
	}

	const TCmsRecMediaType &GetMediaType() const
	{
		return mediaType;
	}

	void SetDomain(const string& strDomain)
	{
		domain = strDomain;
	}
	const string &GetDomain() const
	{
		return domain;
	}
	string &GetDomain()
	{
		return domain;
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);

private:
	TCmsChannel    srcChn;     //平台放像时，为RCS uri。
	TCmsChannel    dstChn;
	TCmsSdp           sdp;
	TCmsRecMediaType mediaType;
	string           domain;	// 域名，用于跨域放像，如果设置了该值且不是本域，则会登录异域rcs进行放像，对前端放像无意义
};

class  CCmsRecPlayInviteRsp: public CCmsEventRsp
{
public:
	CCmsRecPlayInviteRsp()
	{
		SetEvent(CMS_REC_PLAY_INVITE_RSP);
		Clear();
	}

	~CCmsRecPlayInviteRsp()
	{

	}

	void Clear()
	{
		sdp.Clear();
	}

	TCmsSdp& GetSdp()
	{
		return sdp;
	}

	const TCmsSdp& GetSdp() const
	{
		return sdp;
	}

	void SetSdp(const TCmsSdp &tsdp)
	{
		sdp = tsdp;
	}

	unsigned long GetPlayId() const
	{
		return playId;
	}

	void SetPlayId(unsigned long dwPlayId) 
	{
		playId = dwPlayId;
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);

private:
	unsigned long         playId;                     //前端返回的ID，用于放像控制, 是否需要待定
	TCmsSdp        sdp;
};

class  CCmsRecPlayInviteAck: public CCmsEventReq
{
public:
	CCmsRecPlayInviteAck()
	{
		SetEvent(CMS_REC_PLAY_INVITE_ACK);
	}

	~CCmsRecPlayInviteAck()
	{

	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsRecPlayProgNtfReq: public CCmsEventReq
{
public:
	CCmsRecPlayProgNtfReq()
	{
		SetEvent(CMS_REC_PLAY_PROG_NTF_REQ);
		Clear();
	}

	void Clear()
	{
		curPlayTime = 0;    
		totalPlayTime = 0;   
		startPlayTime = 0;   
		isPlayFinished = 0;  
		playSession = STRING_NULL;
	}

public:
	void SetCurPlayTime(unsigned long nCurPlayTime)
	{
		curPlayTime = nCurPlayTime;
	}
	unsigned long GetCurPlayTime() const
	{
		return curPlayTime;
	}

	void SetTotalPlayTime(unsigned long nTotalPlayTime)
	{
		totalPlayTime = nTotalPlayTime;
	}
	unsigned long GetTotalPlayTime() const
	{
		return totalPlayTime;
	}

	void SetStartPlayTime(unsigned long nStartPlayTime)
	{
		startPlayTime = nStartPlayTime;
	}
	unsigned long GetStartPlayTime() const
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

	string& GetPlaySession()
	{
		return playSession;
	}

	const string& GetPlaySession() const
	{
		return playSession;
	}

	void SetPlaySession(const string& strPlaySession)
	{
		playSession = strPlaySession;
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);

private:

	unsigned long         curPlayTime;                             //当前放像时间，单位：秒
	unsigned long         totalPlayTime;                           //放像总时长, 单位：秒
	unsigned long         startPlayTime;                           //起始放像时间，单位：秒
	bool        isPlayFinished;                          //放像是否结束
	string		playSession;							 //放像结束时需携带playSession
};

class  CCmsRecPlayProgNtfRsp : public CCmsEventRsp
{
public:
	CCmsRecPlayProgNtfRsp()
	{
		SetEvent(CMS_REC_PLAY_PROG_NTF_RSP);
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsRecPlayEndNtyReq: public CCmsEventReq
{
private:
	string  playbackSessUri;

public:
	CCmsRecPlayEndNtyReq()
	{
		SetEvent(CMS_REC_PLAY_END_NTF_REQ);
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

class  CCmsRecPlayEndNtyRsp : public CCmsEventRsp
{
public:
	CCmsRecPlayEndNtyRsp()
	{
		SetEvent(CMS_REC_PLAY_END_NTF_RSP);
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsRecPlayProgQryReq : public CCmsEventReq
{
private:
	string  playbackSessUri;

public:
	CCmsRecPlayProgQryReq()
	{
		Clear();
		SetEvent(CMS_REC_PLAY_PROG_QRY_REQ);
	}

	void Clear()
	{
		playbackSessUri = STRING_NULL;
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

class  CCmsRecPlayProgQryRsp : public CCmsEventRsp
{
private:
	unsigned long        curPlayTime;                             //当前放像时间，单位：秒
	unsigned long        totalPlayTime;                           //放像总时长
	unsigned long        startPlayTime;                           //起始放像时间，单位：秒
	bool                 isPlayFinished;                          //放像是否结束

public:
	CCmsRecPlayProgQryRsp()
	{
	    Clear();
		SetEvent(CMS_REC_PLAY_PROG_QRY_RSP);
	}

    void Clear()
    {
        curPlayTime = 0;    
        totalPlayTime = 0;   
        startPlayTime = 0;   
        isPlayFinished = false;  
    }

	void SetCurPlayTime(unsigned long nCurPlayTime)
	{
		curPlayTime = nCurPlayTime;
	}
	unsigned long GetCurPlayTime() const
	{
		return curPlayTime;
	}

	void SetTotalPlayTime(unsigned long nTotalPlayTime)
	{
		totalPlayTime = nTotalPlayTime;
	}
	unsigned long GetTotalPlayTime() const
	{
		return totalPlayTime;
	}

	void SetStartPlayTime(unsigned long nStartPlayTime)
	{
		startPlayTime = nStartPlayTime;
	}
	unsigned long GetStartPlayTime() const
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

class  CCmsRecPlayByeReq: public CCmsEventReq
{
public:
	CCmsRecPlayByeReq()
	{
		Clear();
		SetEvent(CMS_REC_PLAY_BYE_REQ);
	}

	~CCmsRecPlayByeReq()
	{

	}

	void Clear()
	{
		reason = 0;  //默认是0，对应CMS_SUCCESS，表示正常bye
	}

	void SetReason(int nReason)
	{
		reason = nReason;
	}
	int GetReason() const
	{
		return reason;
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);

private:
	int reason;
};

class  CCmsRecPlayByeRsp: public CCmsEventRsp
{
public:
	CCmsRecPlayByeRsp()
	{
		SetEvent(CMS_REC_PLAY_BYE_RSP);
	}

	~CCmsRecPlayByeRsp()
	{

	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsRecPlayVcrCtrlReq: public CCmsEventReq
{
public:
	CCmsRecPlayVcrCtrlReq()
	{
		SetEvent(CMS_REC_PLAY_VCR_CTRL_REQ);
		Clear();
	}

	~CCmsRecPlayVcrCtrlReq()
	{

	}
	void Clear()
	{
		playbackSessUri = STRING_NULL;
		vcrCmd = CMS_VCRCMD_UNKNOWN;
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
	const TCmsVcrCmd& GetVcrCmd() const
	{
		return vcrCmd;
	}
	TCmsVcrCmd& GetVcrCmd()
	{
		return vcrCmd;
	}
	void SetVcrCmd(const TCmsVcrCmd& var)
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
	TCmsVcrCmd vcrCmd;
	int     playRate;       //放像速率，正数为正反，负数为倒放
	int     seekTime;       //相对时间
//	unsigned long		m_dwPuPlayId;  //前端返回的ID，用于放像控制
};

class  CCmsRecPlayVcrCtrlRsp: public CCmsEventRsp
{
public:
	CCmsRecPlayVcrCtrlRsp()
	{
		SetEvent(CMS_REC_PLAY_VCR_CTRL_RSP);
	}

	~CCmsRecPlayVcrCtrlRsp()
	{

	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsRecDownloadInviteReq: public CCmsEventReq
{
public:
	CCmsRecDownloadInviteReq()
	{
		SetEvent(CMS_REC_DOWNLOAD_INVITE_REQ);
		Clear();
	}

	~CCmsRecDownloadInviteReq()
	{

	}
	void Clear()
	{
		srcChn.Clear();
		dstChn.Clear();
		sdp.Clear();
	}

	//srcChn
	const TCmsChannel& GetSrcChn() const 
	{
		return srcChn;
	}
	TCmsChannel& GetSrcChn() 
	{
		return srcChn;
	}
	void SetSrcChn(const TCmsChannel& tSrcChn) 
	{
		srcChn = tSrcChn;
	}

	//dstChn
	const TCmsChannel& GetDstChn() const 
	{
		return dstChn;
	}
	TCmsChannel& GetDstChn() 
	{
		return dstChn;
	}
	void SetDstChn(const TCmsChannel& tDstChn) 
	{
		dstChn = tDstChn;
	}

	//sdp
	const TCmsSdp& GetSdp() const
	{
		return sdp;
	}
	TCmsSdp& GetSdp()
	{
		return sdp;
	}
	void SetSdp(const TCmsSdp& var)
	{
		sdp = var;
	}

	void SetMediaType(const TCmsRecMediaType &tQryType)
	{
		mediaType = tQryType;
	}

	const TCmsRecMediaType &GetMediaType() const
	{
		return mediaType;
	}

	void SetDomain(const string& strDomain)
	{
		domain = strDomain;
	}
	const string &GetDomain() const
	{
		return domain;
	}
	string &GetDomain()
	{
		return domain;
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);

private:
	TCmsChannel    srcChn;     //平台放像时，为RCS uri。
	TCmsChannel    dstChn;
	TCmsSdp           sdp;
	TCmsRecMediaType mediaType;
	string           domain;	// 域名，用于跨域下载，如果设置了该值且不是本域，则会登录异域rcs进行下载，对前端录像下载无意义
};

class  CCmsRecDownloadInviteRsp: public CCmsEventRsp
{
public:
	CCmsRecDownloadInviteRsp()
	{
		SetEvent(CMS_REC_DOWNLOAD_INVITE_RSP);
		Clear();
	}

	~CCmsRecDownloadInviteRsp()
	{

	}

	void Clear()
	{
		sdp.Clear();
	}
	//sdp
	const TCmsSdp& GetSdp() const
	{
		return sdp;
	}
	TCmsSdp& GetSdp()
	{
		return sdp;
	}
	void SetSdp(const TCmsSdp& var)
	{
		sdp = var;
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);

private:
	TCmsSdp        sdp;
};

class  CCmsRecDownloadInviteAck: public CCmsEventReq
{
public:
	CCmsRecDownloadInviteAck()
	{
		SetEvent(CMS_REC_DOWNLOAD_INVITE_ACK);
	}

	~CCmsRecDownloadInviteAck()
	{

	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);

};

class  CCmsRecDownloadEndNtyReq: public CCmsEventReq
{
public:
	CCmsRecDownloadEndNtyReq()
	{
		SetEvent(CMS_REC_DOWNLOAD_END_NTF_REQ);
	}

	~CCmsRecDownloadEndNtyReq()
	{
		Clear();
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

class  CCmsRecDownloadEndNtyRsp : public CCmsEventRsp
{
public:
	CCmsRecDownloadEndNtyRsp()
	{
		SetEvent(CMS_REC_DOWNLOAD_END_NTF_RSP);
	}

	~CCmsRecDownloadEndNtyRsp()
	{
		Clear();
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

class  CCmsRecDownloadByeReq: public CCmsEventReq
{
public:
	CCmsRecDownloadByeReq()
	{
		Clear();
		SetEvent(CMS_REC_DOWNLOAD_BYE_REQ);
	}

	~CCmsRecDownloadByeReq()
	{

	}

	void Clear()
	{
		reason = 0;  //默认是0，对应CMS_SUCCESS，表示正常bye
	}

	void SetReason(int nReason)
	{
		reason = nReason;
	}
	int GetReason() const
	{
		return reason;
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);

private:
	int reason;
};

class  CCmsRecDownloadByeRsp: public CCmsEventRsp
{
public:
	CCmsRecDownloadByeRsp()
	{
		SetEvent(CMS_REC_DOWNLOAD_BYE_RSP);
	}

	~CCmsRecDownloadByeRsp()
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
//目录订阅

//整体订阅(目录订阅)
class   CCmsCatalogStatusSsReq : public CCmsEventReq
{
public:
    CCmsCatalogStatusSsReq()
    {
		Clear();
        SetEvent(CMS_CATALOG_STATUS_SS_REQ);
    }

    void Clear()
    {
        userUri = STRING_NULL;
        domain = STRING_NULL;
        ssTypes.clear();
		batchNotify = false;
    }

private:
    string          userUri;                  //订阅者
    string          domain;                   //需要订阅的域名，用于域名方式的整体订阅
    TCmsSsTypes     ssTypes;                  //订阅的状态种类
	bool            batchNotify;			  //是否接收批量通知
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

    void SetDomain(const string& strDomain)
    {
        domain = strDomain;
    }
    string& GetDomain()
    {
        return domain;
    }
    const string& GetDomain() const
    {
        return domain;
    }

    void SetSsTypes(const TCmsSsTypes& cSsTypes)
    {
        ssTypes = cSsTypes;
    }
    TCmsSsTypes& GetSsTypes()
    {
        return ssTypes;
    }
    const TCmsSsTypes& GetSsTypes() const
    {
        return ssTypes;
    }

	bool GetBatchNotify() const
	{
		return batchNotify;
	}
	void SetBatchNotify(bool bFlag)
	{
		batchNotify = bFlag;
	}

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class   CCmsCatalogStatusSsRsp : public CCmsEventRsp
{
public:
    CCmsCatalogStatusSsRsp()
    {
        SetEvent(CMS_CATALOG_STATUS_SS_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class   CCmsCatalogStatusRefreshSsReq : public CCmsEventReq
{
public:
    CCmsCatalogStatusRefreshSsReq()
    {
        SetEvent(CMS_CATALOG_STATUS_REFRESH_SS_REQ);
    };

    void Clear()
    {
        ssOpType = STRING_NULL;
        ssTypes.clear();
    }

private:
    //刷新的是状态类型
    TCmsSsOpType       ssOpType;                      //刷新操作类型：添加或删除，CMS_SS_OPE_ADD，CMS_SS_OPE_ERASE
    TCmsSsTypes        ssTypes;                       //订阅刷新的状态种类
public:
    void SetSsOpType(const TCmsSsOpType& strSsOpType)
    {
        ssOpType = strSsOpType;
    }
    TCmsSsOpType& GetSsOpType()
    {
        return ssOpType;
    }
    const TCmsSsOpType& GetSsOpType() const
    {
        return ssOpType;
    }

    void SetSsTypes(const TCmsSsTypes& cSsTypes)
    {
        ssTypes = cSsTypes;
    }
    TCmsSsTypes& GetSsTypes()
    {
        return ssTypes;
    }
    const TCmsSsTypes& GetSsTypes() const
    {
        return ssTypes;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class   CCmsCatalogStatusRefreshSsRsp : public CCmsEventRsp
{
public:
    CCmsCatalogStatusRefreshSsRsp()
    {
        SetEvent(CMS_CATALOG_STATUS_REFRESH_SS_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class   CCmsCatalogStatusUnSsReq : public CCmsEventReq
{
public:
    CCmsCatalogStatusUnSsReq()
    {
        SetEvent(CMS_CATALOG_STATUS_UNSS_REQ);
    }

    void Clear()
    {
        userUri = STRING_NULL;
    }

private:
    string          userUri;                        //订阅者
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

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class   CCmsCatalogStatusUnSsRsp : public CCmsEventRsp
{
public:
    CCmsCatalogStatusUnSsRsp()
    {
        SetEvent(CMS_CATALOG_STATUS_UNSS_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

//////////////////////////////////////////////////////////////////////////
// 订阅列表，key为设备uri，value为订阅类型
typedef map<string, TCmsSsTypes> TCmsSsDevList;

// 订阅失败列表，key为设备uri，value为错误码
typedef map<string, int> TCmsSsErrorList;

//整体订阅(列表订阅)
class   CCmsPuListStatusSsReq : public CCmsEventReq
{
public:
    CCmsPuListStatusSsReq()
    {
		Clear();
        SetEvent(CMS_PULIST_STATUS_SS_REQ);
    }

    void Clear()
    {
		userUri = STRING_NULL;
		ssDevList.clear();
		batchNotify = false;
    }

private:
	string		  userUri;
	TCmsSsDevList ssDevList;
	bool          batchNotify;
public:
    
	const string &GetUserUri() const
	{
		return userUri;
	}
	string &GetUserUri()
	{
		return userUri;
	}
	void SetUserUri(const string &strUserUri)
	{
		userUri = strUserUri;
	}

	const TCmsSsDevList& GetSsDevList() const
	{
		return ssDevList;
	}
	TCmsSsDevList& GetSsDevList()
	{
		return ssDevList;
	}
	void SetSsDevList(const TCmsSsDevList &tSsUnitList)
	{
		ssDevList = tSsUnitList;
	}

	bool GetBatchNotify() const
	{
		return batchNotify;
	}
	void SetBatchNotify(bool bFlag)
	{
		batchNotify = bFlag;
	}

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class   CCmsPuListStatusSsRsp : public CCmsEventRsp
{
public:
    CCmsPuListStatusSsRsp()
    {
        SetEvent(CMS_PULIST_STATUS_SS_RSP);
    }

	void Clear()
	{
		ssErrorList.clear();
	}

private:
	TCmsSsErrorList ssErrorList;

public:
	const TCmsSsErrorList& GetSsErrorList() const
	{
		return ssErrorList;
	}
	TCmsSsErrorList& GetSsErrorList()
	{
		return ssErrorList;
	}
	void SetSsErrorList(const TCmsSsErrorList &tSsErrorList)
	{
		ssErrorList = tSsErrorList;
	}
	void SetSsErrorItem(const string &strDevUri, int enErrorCode)
	{
		ssErrorList[strDevUri] = enErrorCode;
	}

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class   CCmsPuListStatusRefreshSsReq : public CCmsEventReq
{
public:
    CCmsPuListStatusRefreshSsReq()
    {
        SetEvent(CMS_PULIST_STATUS_REFRESH_SS_REQ);
    }

    void Clear()
    {
        ssOpType = STRING_NULL;
        ssDevList.clear();
    }

private:
	TCmsSsOpType  ssOpType;	 // 刷新操作类型（约定：当被订阅设备的TSsTypes被清空后从订阅设备列表中清除该设备）
	TCmsSsDevList ssDevList; // 需要刷新的设备列表

public:

	const TCmsSsOpType& GetSsOpType() const
	{
		return ssOpType;
	}
	TCmsSsOpType& GetSsOpType()
	{
		return ssOpType;
	}
	void SetSsOpType(const TCmsSsOpType& var)
	{
		ssOpType = var;
	}

	const TCmsSsDevList& GetSsDevList() const
	{
		return ssDevList;
	}
	TCmsSsDevList& GetSsDevList()
	{
		return ssDevList;
	}
	void SetSsDevList(const TCmsSsDevList &tSsUnitList)
	{
		ssDevList = tSsUnitList;
	}

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class   CCmsPuListStatusRefreshSsRsp : public CCmsEventRsp
{
public:
    CCmsPuListStatusRefreshSsRsp()
    {
        SetEvent(CMS_PULIST_STATUS_REFRESH_SS_RSP);
    }

	void Clear()
	{
		ssErrorList.clear();
	}

private:
	TCmsSsErrorList ssErrorList;

public:
	const TCmsSsErrorList& GetSsErrorList() const
	{
		return ssErrorList;
	}
	TCmsSsErrorList& GetSsErrorList()
	{
		return ssErrorList;
	}
	void SetSsErrorList(const TCmsSsErrorList &tSsErrorList)
	{
		ssErrorList = tSsErrorList;
	}
	void SetSsErrorItem(const string &strDevUri, int enErrorCode)
	{
		ssErrorList[strDevUri] = enErrorCode;
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class   CCmsPuListStatusUnSsReq : public CCmsEventReq
{
public:
    CCmsPuListStatusUnSsReq()
    {
        SetEvent(CMS_PULIST_STATUS_UNSS_REQ);
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

class   CCmsPuListStatusUnSsRsp : public CCmsEventRsp
{
public:
    CCmsPuListStatusUnSsRsp()
    {
        SetEvent(CMS_PULIST_STATUS_UNSS_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

//////////////////////////////////////////////////////////////////////////

//单个设备订阅
class   CCmsPuStatusSsReq : public CCmsEventReq
{
public:
    CCmsPuStatusSsReq()
    {
		Clear();
        SetEvent(CMS_PU_STATUS_SS_REQ);
    }

    void Clear()
    {
        userUri = STRING_NULL;
        devUri = STRING_NULL;
        ssTypes.clear();
		batchNotify = false;
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

	bool GetBatchNotify() const
	{
		return batchNotify;
	}
	void SetBatchNotify(bool bFlag)
	{
		batchNotify = bFlag;
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
    TCmsSsTypes     ssTypes;                  //订阅的状态种类
	bool            batchNotify;			  //是否接收批量通知
public:
    void SetSsTypes(const TCmsSsTypes& cSsTypes)
    {
        ssTypes = cSsTypes;
    }
    TCmsSsTypes& GetSsTypes()
    {
        return ssTypes;
    }
    const TCmsSsTypes& GetSsTypes() const
    {
        return ssTypes;
    }

};

class   CCmsPuStatusSsRsp : public CCmsEventRsp
{
public:
    CCmsPuStatusSsRsp()
    {
        SetEvent(CMS_PU_STATUS_SS_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class   CCmsPuStatusRefreshSsReq : public CCmsEventReq
{
public:
    CCmsPuStatusRefreshSsReq()
    {
        SetEvent(CMS_PU_STATUS_REFRESH_SS_REQ);
    };

    void Clear()
    {
        ssOpType = STRING_NULL;
        ssTypes.clear();
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

private: 
    TCmsSsOpType       ssOpType;                //刷新操作类型：添加或删除，SS_OPE_ADD，SS_OPE_ERASE
    TCmsSsTypes        ssTypes;                 //订阅刷新的状态种类
public:
    void SetSsOpType(const TCmsSsOpType& strSsOpType)
    {
        ssOpType = strSsOpType;
    }
    TCmsSsOpType& GetSsOpType()
    {
        return ssOpType;
    }
    const TCmsSsOpType& GetSsOpType() const
    {
        return ssOpType;
    }

    void SetSsTypes(const TCmsSsTypes& cSsTypes)
    {
        ssTypes = cSsTypes;
    }
    TCmsSsTypes& GetSsTypes()
    {
        return ssTypes;
    }
    const TCmsSsTypes& GetSsTypes() const
    {
        return ssTypes;
    }

};

class   CCmsPuStatusRefreshSsRsp : public CCmsEventRsp
{
public:
    CCmsPuStatusRefreshSsRsp()
    {
        SetEvent(CMS_PU_STATUS_REFRESH_SS_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class   CCmsPuStatusUnSsReq : public CCmsEventReq
{
public:
    CCmsPuStatusUnSsReq()
    {
        SetEvent(CMS_PU_STATUS_UNSS_REQ);
    }

    void Clear()
    {
        userUri = STRING_NULL;
        devUri = STRING_NULL;
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

class   CCmsPuStatusUnSsRsp : public CCmsEventRsp
{
public:
    CCmsPuStatusUnSsRsp()
    {
        SetEvent(CMS_PU_STATUS_UNSS_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class   CCmsPuStatusNtyReq : public CCmsEventReq
{
public:
    CCmsPuStatusNtyReq()
    {
        SetEvent(CMS_PU_STATUS_NTF_REQ);
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
    const TCmsPuStatus& GetOnOffStatus() const
    {
        return onOff;
    }
    TCmsPuStatus& GetOnOffStatus()
    {
        return onOff;
    }
    void SetOnOffStatus(const TCmsPuStatus& tOnOff)
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
    TCmsPuStatus           onOff;      //上下线状态
};

class   CCmsPuStatusNtyRsp : public CCmsEventRsp
{
public:
	CCmsPuStatusNtyRsp()
	{
		SetEvent(CMS_PU_STATUS_NTF_RSP);
	}

	~CCmsPuStatusNtyRsp()
	{
		Clear();
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

class   CCmsPuConfigNtyReq : public CCmsEventReq
{
public:
    CCmsPuConfigNtyReq()
    {
        SetEvent(CMS_PU_CONFIG_NTF_REQ);
    }

    void Clear()
    {
        devURI = STRING_NULL;
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

    const TCmsPuDevConfig& GetConfig() const
    {
        return config;
    }
    TCmsPuDevConfig& GetConfig()
    {
        return config;
    }
    void SetConfig(const TCmsPuDevConfig& tConfig)
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
    TCmsPuDevConfig           config;    //前端属性、能力集、配置
};

class   CCmsPuConfigNtyRsp : public CCmsEventRsp
{
public:
	CCmsPuConfigNtyRsp()
	{
		SetEvent(CMS_PU_CONFIG_NTF_RSP);
	}

	~CCmsPuConfigNtyRsp()
	{
		Clear();
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

class   CCmsPuServiceStatusNtyReq : public CCmsEventReq
{
public:
    CCmsPuServiceStatusNtyReq()
    {
        SetEvent(CMS_PU_SERVICE_STATUS_NTF_REQ);
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
    const TCmsPuServiceStatus& GetServiceStatus() const
    {
        return service;
    }
    TCmsPuServiceStatus& GetServiceStatus()
    {
        return service;
    }
    void SetServiceStatus(const TCmsPuServiceStatus& tService)
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
    TCmsPuServiceStatus    service;    //业务状态
};

class   CCmsPuServiceStatusNtyRsp : public CCmsEventRsp
{
public:
	CCmsPuServiceStatusNtyRsp()
	{
		SetEvent(CMS_PU_SERVICE_STATUS_NTF_RSP);
	}

	~CCmsPuServiceStatusNtyRsp()
	{
		Clear();
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

class   CCmsPuAlarmStatusNtyReq : public CCmsEventReq
{
public:
    CCmsPuAlarmStatusNtyReq()
    {
        SetEvent(CMS_PU_ALARM_STATUS_NTF_REQ);
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
    const TCmsPuAlarmStatus& GetAlarmStatus() const
    {
        return alarm;
    }
    TCmsPuAlarmStatus& GetAlarmStatus()
    {
        return alarm;
    }
    void SetAlarmStatus(const TCmsPuAlarmStatus& tAlarm)
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
    TCmsPuAlarmStatus      alarm;    //告警状态
};

class   CCmsPuAlarmStatusNtyRsp : public CCmsEventRsp
{
public:
	CCmsPuAlarmStatusNtyRsp()
	{
		SetEvent(CMS_PU_ALARM_STATUS_NTF_REQ);
	}

	~CCmsPuAlarmStatusNtyRsp()
	{
		Clear();
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

class   CCmsPuTransDataStatusNtyReq : public CCmsEventReq
{
public:
	CCmsPuTransDataStatusNtyReq()
	{
		Clear();
		SetEvent(CMS_PU_TRANSDATA_STATUS_NTF_REQ);
	}

	~CCmsPuTransDataStatusNtyReq()
	{
		Clear();
	}

	void Clear()
	{
		devURI = STRING_NULL;
		data.Clear();
	}

private:
	string				   devURI;                     //pu uuid @domain
	TCmsTransparentData    data;                       //透明数据状态
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

	void SetData(const TCmsTransparentData& cData)
	{
		data = cData;
	}
	TCmsTransparentData& GetData()
	{
		return data;
	}
	const TCmsTransparentData& GetData() const
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

class   CCmsPuTransDataStatusNtyRsp : public CCmsEventRsp
{
public:
	CCmsPuTransDataStatusNtyRsp()
	{
		SetEvent(CMS_PU_TRANSDATA_STATUS_NTF_RSP);
	}

	~CCmsPuTransDataStatusNtyRsp()
	{
		Clear();
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

//前端透明数据通知
class   CCmsPuTransparentDataNtfReq : public CCmsEventReq
{
public:
	CCmsPuTransparentDataNtfReq()
	{
		SetEvent(CMS_PU_TRANSPARENT_DATA_NTF_REQ);
	}

	void Clear()
	{
		data.Clear();
		CCmsEventReq::Clear();
	}

private:
	TCmsTransparentData  data;                         //透明数据
public:
	void SetData(const TCmsTransparentData& cData)
	{
		data = cData;
	}
	TCmsTransparentData& GetData()
	{
		return data;
	}
	const TCmsTransparentData& GetData() const
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

class   CCmsPuTransparentDataNtfRsp : public CCmsEventRsp
{
public:
	CCmsPuTransparentDataNtfRsp()
	{
		SetEvent(CMS_PU_TRANSPARENT_DATA_NTF_RSP);
	}

	void Clear()
	{
		CCmsEventRsp::Clear();
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class   CCmsCmuDisconnectPuNtf : public CCmsEventReq
{
public:
	CCmsCmuDisconnectPuNtf()
	{
		SetEvent(CMS_CMU_DISCONNECT_PU_NTF);
	}

	void Clear()
	{
		devUri = STRING_NULL;
		CCmsEventReq::Clear();
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

private:
	string           devUri;  //前端设备URI
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class   CCmsModuleOnOffLogNtf : public CCmsEventReq
{
public:
	CCmsModuleOnOffLogNtf()
	{
		SetEvent(CMS_MODULE_ON_OFF_LOG_NTF);
	}

	void Clear()
	{
		info.Clear();
		CCmsEventReq::Clear();
	}
public:
	void SetInfo(const TCmsModuleOnOffLogInfo& cInfo)
	{
		info = cInfo;
	}
	TCmsModuleOnOffLogInfo& GetInfo()
	{
		return info;
	}
	const TCmsModuleOnOffLogInfo& GetInfo() const
	{
		return info;
	}

private:
	TCmsModuleOnOffLogInfo info;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CCmsPuAlarmResetReq : public CCmsEventReq
{
public:
    CCmsPuAlarmResetReq()
    {
        SetEvent(CMS_PU_ALARM_RESET_REQ);
    }

private:
    TCmsChannel        devChn;                      //设备编码通道，设备本身时用-1
public:
    void SetDevChn(const TCmsChannel& cDevChn)
    {
        devChn = cDevChn;
    }
    TCmsChannel& GetDevChn()
    {
        return devChn;
    }
    const TCmsChannel& GetDevChn() const
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

class CCmsPuAlarmResetRsp : public CCmsEventRsp
{
public:
    CCmsPuAlarmResetRsp()
    {
        SetEvent(CMS_PU_ALARM_RESET_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class   CCmsPuGpsStatusNtyReq : public CCmsEventReq
{
public:
    CCmsPuGpsStatusNtyReq()
    {
        SetEvent(CMS_PU_GPS_STATUS_NTF_REQ);
    };

    void Clear()
    {
        gps.Clear();
    }

public:
    const string& GetDevUri() const
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
    const TCmsGpsData& GetGpsData() const
    {
        return gps;
    }
    TCmsGpsData& GetGpsData()
    {
        return gps;
    }
    void SetGpsData(const TCmsGpsData& tVal)
    {
        gps = tVal;
    }

private:
    string              devURI;     //pu uuid @domain
    TCmsGpsData            gps;        //gps状态
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class   CCmsPuGpsStatusNtyRsp : public CCmsEventRsp
{
public:
	CCmsPuGpsStatusNtyRsp()
	{
		SetEvent(CMS_PU_GPS_STATUS_NTF_RSP);
	}

	~CCmsPuGpsStatusNtyRsp()
	{
		Clear();
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

class   CCmsPuCameraTypeSetReq : public CCmsEventReq
{
public:
	CCmsPuCameraTypeSetReq()
	{
		Clear();
		SetEvent(CMS_PU_CAMERA_TYPE_SET_REQ);
	}

	~CCmsPuCameraTypeSetReq()
	{
		Clear();
	}

	void Clear()
	{
		userUri = STRING_NULL;
		devChn.Clear();
		cameraType = CMS_CAM_TYPE_BEGIN;
	}

	//事务模板会使用
	const string& GetDevUri() const
	{
		return devChn.GetDevUri();
	}

private:
	string              userUri;                    //操作用户
	TCmsChannel         devChn;                     //设备通道
	TCmsCamType         cameraType;                 //摄像头类型
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

	void SetDevChn(const TCmsChannel& cDevChn)
	{
		devChn = cDevChn;
	}
	TCmsChannel& GetDevChn()
	{
		return devChn;
	}
	const TCmsChannel& GetDevChn() const
	{
		return devChn;
	}

	void SetCameraType(TCmsCamType tCameraType)
	{
		cameraType = tCameraType;
	}
	TCmsCamType GetCameraType() const
	{
		return cameraType;
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class   CCmsPuCameraTypeSetRsp : public CCmsEventRsp
{
public:
	CCmsPuCameraTypeSetRsp()
	{
		Clear();
		SetEvent(CMS_PU_CAMERA_TYPE_SET_RSP);
	}

	~CCmsPuCameraTypeSetRsp()
	{
		Clear();
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

//移动侦测参数配置
class   CCmsPuMotionDetectSetReq : public CCmsEventReq
{
public:
    CCmsPuMotionDetectSetReq()
    {
        Clear();
        SetEvent(CMS_PU_MOTION_DETECT_SET_REQ);
    }

    void Clear()
    {
        userUri = STRING_NULL;
        devChn.Clear();
        param.clear();
        CCmsEventReq::Clear();
    }

    //事务模板会使用
    const string& GetDevUri() const
    {
        return devChn.GetDevUri();
    }

private:
    string              userUri;                    //操作用户
    TCmsChannel            devChn;                     //设备通道
    TCmsMotionDetectList   param;                      //移动侦测参数
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

    void SetDevChn(const TCmsChannel& cDevChn)
    {
        devChn = cDevChn;
    }
    TCmsChannel& GetDevChn()
    {
        return devChn;
    }
    const TCmsChannel& GetDevChn() const
    {
        return devChn;
    }

    void SetParam(const TCmsMotionDetectList& cParam)
    {
        param = cParam;
    }
    TCmsMotionDetectList& GetParam()
    {
        return param;
    }
    const TCmsMotionDetectList& GetParam() const
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

class   CCmsPuMotionDetectSetRsp : public CCmsEventRsp
{
public:
    CCmsPuMotionDetectSetRsp()
    {
        SetEvent(CMS_PU_MOTION_DETECT_SET_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class   CCmsPuMotionDetectGetReq : public CCmsEventReq
{
public:
    CCmsPuMotionDetectGetReq()
    {
        Clear();
        SetEvent(CMS_PU_MOTION_DETECT_GET_REQ);
    }

    void Clear()
    {
        userUri = STRING_NULL;
        devChn.Clear();
        CCmsEventReq::Clear();
    }

    //事务模板会使用
    const string& GetDevUri() const
    {
        return devChn.GetDevUri();
    }

private:
    string             userUri;                //操作用户
    TCmsChannel           devChn;                 //设备通道
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

    void SetDevChn(const TCmsChannel& cDevChn)
    {
        devChn = cDevChn;
    }
    TCmsChannel& GetDevChn()
    {
        return devChn;
    }
    const TCmsChannel& GetDevChn() const
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

class   CCmsPuMotionDetectGetRsp : public CCmsEventRsp
{
public:
    CCmsPuMotionDetectGetRsp()
    {
        Clear();
        SetEvent(CMS_PU_MOTION_DETECT_GET_RSP);
    }

    void Clear()
    {
        param.clear();
        CCmsEventRsp::Clear();
    }

private:
    TCmsMotionDetectList   param;                      //移动侦测参数
public:
    void SetParam(const TCmsMotionDetectList& cParam)
    {
        param = cParam;
    }
    TCmsMotionDetectList& GetParam()
    {
        return param;
    }
    const TCmsMotionDetectList& GetParam() const
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
class   CCmsPuAreaShadeSetReq : public CCmsEventReq
{
public:
    CCmsPuAreaShadeSetReq()
    {
        Clear();
        SetEvent(CMS_PU_AREA_SHADE_SET_REQ);
    }

    void Clear()
    {
        userUri = STRING_NULL;
        devChn.Clear();
        param.clear();
        CCmsEventReq::Clear();
    }

    //事务模板会使用
    const string& GetDevUri() const
    {
        return devChn.GetDevUri();
    }

private:
    string              userUri;                    //操作用户
    TCmsChannel            devChn;                     //设备通道
    TCmsAreaShadeList      param;                      //图像遮蔽参数
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

    void SetDevChn(const TCmsChannel& cDevChn)
    {
        devChn = cDevChn;
    }
    TCmsChannel& GetDevChn()
    {
        return devChn;
    }
    const TCmsChannel& GetDevChn() const
    {
        return devChn;
    }

    void SetParam(const TCmsAreaShadeList& cParam)
    {
        param = cParam;
    }
    TCmsAreaShadeList& GetParam()
    {
        return param;
    }
    const TCmsAreaShadeList& GetParam() const
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

class   CCmsPuAreaShadeSetRsp : public CCmsEventRsp
{
public:
    CCmsPuAreaShadeSetRsp()
    {
        SetEvent(CMS_PU_AREA_SHADE_SET_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class   CCmsPuAreaShadeGetReq : public CCmsEventReq
{
public:
    CCmsPuAreaShadeGetReq()
    {
        Clear();
        SetEvent(CMS_PU_AREA_SHADE_GET_REQ);
    }

    void Clear()
    {
        userUri = STRING_NULL;
        devChn.Clear();
        CCmsEventReq::Clear();
    }

    //事务模板会使用
    const string& GetDevUri() const
    {
        return devChn.GetDevUri();
    }

private:
    string             userUri;                //操作用户
    TCmsChannel           devChn;                 //设备通道
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

    void SetDevChn(const TCmsChannel& cDevChn)
    {
        devChn = cDevChn;
    }
    TCmsChannel& GetDevChn()
    {
        return devChn;
    }
    const TCmsChannel& GetDevChn() const
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

class   CCmsPuAreaShadeGetRsp : public CCmsEventRsp
{
public:
    CCmsPuAreaShadeGetRsp()
    {
        Clear();
        SetEvent(CMS_PU_AREA_SHADE_GET_RSP);
    }

    void Clear()
    {
        param.clear();
        CCmsEventRsp::Clear();
    }

private:
    TCmsAreaShadeList   param;                      //图像遮蔽参数
public:
    void SetParam(const TCmsAreaShadeList& cParam)
    {
        param = cParam;
    }
    TCmsAreaShadeList& GetParam()
    {
        return param;
    }
    const TCmsAreaShadeList& GetParam() const
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


class   CCmsPuPicSnapshotReq : public CCmsEventReq
{
public:
	CCmsPuPicSnapshotReq()
	{
		Clear();
		SetEvent(CMS_PU_PIC_SNAPSHOT_REQ);
	}

	~CCmsPuPicSnapshotReq()
	{
		Clear();
	}

	void Clear()
	{
		devChn.Clear();
	}

private:
	TCmsChannel           devChn;                                  //设备通道

public:
	//事务模板会使用
	const string& GetDevUri() const
	{
		return devChn.GetDevUri();
	}
public:
	void SetDevChn(const TCmsChannel& cDevChn)
	{
		devChn = cDevChn;
	}
	TCmsChannel& GetDevChn()
	{
		return devChn;
	}
	const TCmsChannel& GetDevChn() const
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

class   CCmsPuPicSnapshotRsp : public CCmsEventRsp
{
public:
	CCmsPuPicSnapshotRsp()
	{
		Clear();
		SetEvent(CMS_PU_PIC_SNAPSHOT_RSP);
	}

	~CCmsPuPicSnapshotRsp()
	{
		Clear();
	}

	void Clear()
	{

	}

private:
	string  picID;                        //图片标识
    string  picName;                      //图片名称，门店管理使用。

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
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
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

class   CCmsPuRecDaysQryReq : public CCmsEventReq
{
public:
    CCmsPuRecDaysQryReq()
    {
		Clear();
        SetEvent(CMS_PU_REC_DAYS_QRY_REQ);
    }

	~CCmsPuRecDaysQryReq()
	{
		Clear();
	}

    void Clear()
    {
        devChn.Clear();
        timeCond = STRING_NULL;
    }

private:
    TCmsChannel        devChn;                               //设备通道
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
    void SetDevChn(const TCmsChannel& cDevChn)
    {
        devChn = cDevChn;
    }
    TCmsChannel& GetDevChn()
    {
        return devChn;
    }
    const TCmsChannel& GetDevChn() const
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
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class   CCmsPuRecDaysQryRsp : public CCmsEventRsp
{
public:
    CCmsPuRecDaysQryRsp()
    {
		Clear();
        SetEvent(CMS_PU_REC_DAYS_QRY_RSP);
    }

	~CCmsPuRecDaysQryRsp()
	{
		Clear();
	}

    void Clear()
    {
        resultBitMask = 0;
    }

private:
    int        resultBitMask;                                   //查询结果

public:
    void SetResultBitMask(int nResultBitMask)
    {
        resultBitMask = nResultBitMask;
    }
    int GetResultBitMask() const
    {
        return resultBitMask;
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class   CCmsPuLongOsdSetReq : public CCmsEventReq
{
public:
	CCmsPuLongOsdSetReq()
	{
		Clear();
		SetEvent(CMS_PU_LONG_OSD_SET_REQ);
	}

	~CCmsPuLongOsdSetReq()
	{
		Clear();
	}

	void Clear()
	{
		devChn.Clear();
		param.Clear();
	}

private:
	TCmsChannel           devChn;                       //设备通道
	TCmsLongOsdParam      param;                        //配置参数

public:
	//事务模板会使用
	const string& GetDevUri() const
	{
		return devChn.GetDevUri();
	}
public:
	void SetDevChn(const TCmsChannel& cDevChn)
	{
		devChn = cDevChn;
	}
	TCmsChannel& GetDevChn()
	{
		return devChn;
	}
	const TCmsChannel& GetDevChn() const
	{
		return devChn;
	}

	void SetParam(const TCmsLongOsdParam& cParam)
	{
		param = cParam;
	}
	TCmsLongOsdParam& GetParam()
	{
		return param;
	}
	const TCmsLongOsdParam& GetParam() const
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

class   CCmsPuLongOsdSetRsp : public CCmsEventRsp
{
public:
	CCmsPuLongOsdSetRsp()
	{
		Clear();
		SetEvent(CMS_PU_LONG_OSD_SET_RSP);
	}

	~CCmsPuLongOsdSetRsp()
	{
		Clear();
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

class  CCmsPuLongOsdGetReq : public CCmsEventReq
{
public:
	CCmsPuLongOsdGetReq()
	{
		Clear();
		SetEvent(CMS_PU_LONG_OSD_GET_REQ);
	}

	~CCmsPuLongOsdGetReq()
	{
		Clear();
	}

	void Clear()
	{
		devChn.Clear();
	}

private:
	TCmsChannel           devChn;                       //设备通道

public:
	//事务模板会使用
	const string& GetDevUri() const
	{
		return devChn.GetDevUri();
	}
public:
	void SetDevChn(const TCmsChannel& cDevChn)
	{
		devChn = cDevChn;
	}
	TCmsChannel& GetDevChn()
	{
		return devChn;
	}
	const TCmsChannel& GetDevChn() const
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

class  CCmsPuLongOsdGetRsp : public CCmsEventRsp
{
public:
	CCmsPuLongOsdGetRsp()
	{
		Clear();
		SetEvent(CMS_PU_LONG_OSD_GET_RSP);
	}

	~CCmsPuLongOsdGetRsp()
	{
		Clear();
	}

	void Clear()
	{
		devChn.Clear();
		param.Clear();
	}

private:
	TCmsChannel           devChn;                       //设备通道
	TCmsLongOsdParam      param;                        //配置参数
public:
	void SetDevChn(const TCmsChannel& cDevChn)
	{
		devChn = cDevChn;
	}
	TCmsChannel& GetDevChn()
	{
		return devChn;
	}
	const TCmsChannel& GetDevChn() const
	{
		return devChn;
	}

	void SetParam(const TCmsLongOsdParam& cParam)
	{
		param = cParam;
	}
	TCmsLongOsdParam& GetParam()
	{
		return param;
	}
	const TCmsLongOsdParam& GetParam() const
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

class  CCmsPuLogoBmpOsdTransmitReq : public CCmsEventReq
{
public:
	CCmsPuLogoBmpOsdTransmitReq()
	{
		Clear();
		SetEvent(CMS_PU_LOGO_BMPOSD_TRANSMIT_REQ);
	}

	~CCmsPuLogoBmpOsdTransmitReq()
	{
		Clear();
	}

	void Clear()
	{
		devChn.Clear();
		param.Clear();
	}

private:
	TCmsChannel           devChn;                       //设备通道
	TCmsLogoBmpOsdParam   param;                        //配置参数

public:
	//事务模板会使用
	const string& GetDevUri() const
	{
		return devChn.GetDevUri();
	}
public:
	void SetDevChn(const TCmsChannel& cDevChn)
	{
		devChn = cDevChn;
	}
	TCmsChannel& GetDevChn()
	{
		return devChn;
	}
	const TCmsChannel& GetDevChn() const
	{
		return devChn;
	}

	void SetParam(const TCmsLogoBmpOsdParam& cParam)
	{
		param = cParam;
	}
	TCmsLogoBmpOsdParam& GetParam()
	{
		return param;
	}
	const TCmsLogoBmpOsdParam& GetParam() const
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

class  CCmsPuLogoBmpOsdTransmitRsp : public CCmsEventRsp
{
public:
	CCmsPuLogoBmpOsdTransmitRsp()
	{
		Clear();
		SetEvent(CMS_PU_LOGO_BMPOSD_TRANSMIT_RSP);
	}

	~CCmsPuLogoBmpOsdTransmitRsp()
	{
		Clear();
	}

	void Clear()
	{
		devChn.Clear();
		curPackIndex = 0;
		totalPack = 0;
	}

private:
	TCmsChannel devChn;                    //设备通道  
	int curPackIndex;               	   //当前包的索引号,从1开始
	int totalPack;               		   //总包数
public:
	void SetDevChn(const TCmsChannel& cDevChn)
	{
		devChn = cDevChn;
	}
	TCmsChannel& GetDevChn()
	{
		return devChn;
	}
	const TCmsChannel& GetDevChn() const
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

//前端录像开启、停止
class  CCmsPuRecordStartReq : public CCmsEventReq
{
public:
    CCmsPuRecordStartReq()
    {
        Clear();
        SetEvent(CMS_PU_RECORD_START_REQ);
    }

    void Clear()
    {
        devChn.Clear();
        CCmsEventReq::Clear();
    }

private:
    TCmsChannel           devChn;                            //设备通道

public:
    //事务模板会使用
    const string& GetDevUri() const
    {
        return devChn.GetDevUri();
    }
public:
    void SetDevChn(const TCmsChannel& cDevChn)
    {
        devChn = cDevChn;
    }
    TCmsChannel& GetDevChn()
    {
        return devChn;
    }
    const TCmsChannel& GetDevChn() const
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

class  CCmsPuRecordStartRsp : public CCmsEventRsp
{
public:
    CCmsPuRecordStartRsp()
    {
        SetEvent(CMS_PU_RECORD_START_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsPuRecordStopReq : public CCmsEventReq
{
public:
    CCmsPuRecordStopReq()
    {
        Clear();
        SetEvent(CMS_PU_RECORD_STOP_REQ);
    }

    void Clear()
    {
        devChn.Clear();
        CCmsEventReq::Clear();
    }

private:
    TCmsChannel           devChn;                            //设备通道

public:
    //事务模板会使用
    const string& GetDevUri() const
    {
        return devChn.GetDevUri();
    }
public:
    void SetDevChn(const TCmsChannel& cDevChn)
    {
        devChn = cDevChn;
    }
    TCmsChannel& GetDevChn()
    {
        return devChn;
    }
    const TCmsChannel& GetDevChn() const
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

class  CCmsPuRecordStopRsp : public CCmsEventRsp
{
public:
    CCmsPuRecordStopRsp()
    {
        SetEvent(CMS_PU_RECORD_STOP_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

//前端录像删除
class  CCmsPuRecordDelReq : public CCmsEventReq
{
public:
    CCmsPuRecordDelReq()
    {
        Clear();
        SetEvent(CMS_PU_RECORD_DEL_REQ);
    }

    void Clear()
    {
        devChn.Clear();
        recInfo.Clear();
        CCmsEventReq::Clear();
    }

private:
    TCmsChannel           devChn;                            //设备通道
    TCmsPuRecInfo         recInfo;                           //录像信息

public:
    //事务模板会使用
    const string& GetDevUri() const
    {
        return devChn.GetDevUri();
    }
public:
    void SetDevChn(const TCmsChannel& cDevChn)
    {
        devChn = cDevChn;
    }
    TCmsChannel& GetDevChn()
    {
        return devChn;
    }
    const TCmsChannel& GetDevChn() const
    {
        return devChn;
    }

    void SetRecInfo(const TCmsPuRecInfo& cRecInfo)
    {
        recInfo = cRecInfo;
    }
    TCmsPuRecInfo& GetRecInfo()
    {
        return recInfo;
    }
    const TCmsPuRecInfo& GetRecInfo() const
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

class  CCmsPuRecordDelRsp : public CCmsEventRsp
{
public:
    CCmsPuRecordDelRsp()
    {
        SetEvent(CMS_PU_RECORD_DEL_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsPuSnapShotCfgGetReq : public CCmsEventReq
{
public:
	CCmsPuSnapShotCfgGetReq()
	{
		Clear();
		SetEvent(CMS_PU_SNAPSHOT_CFG_GET_REQ);
	}

	void Clear()
	{
		devChn.Clear();
	}
private:
	TCmsChannel           devChn; 

public:
	//事务模板会使用
	const string& GetDevUri() const
	{
		return devChn.GetDevUri();
	}
public:
	void SetDevChn(const TCmsChannel& cDevChn)
	{
		devChn = cDevChn;
	}
	TCmsChannel& GetDevChn()
	{
		return devChn;
	}
	const TCmsChannel& GetDevChn() const
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

class  CCmsPuSnapShotCfgGetRsp : public CCmsEventRsp
{
public:
	CCmsPuSnapShotCfgGetRsp()
	{
		Clear();
		SetEvent(CMS_PU_SNAPSHOT_CFG_GET_RSP);
	}
	void Clear()
	{
		devChn.Clear();
		puSnapShotCfg.Clear();
	}

private:
	TCmsChannel  devChn; 
	TCmsPuSnapShotCfg puSnapShotCfg;

public:
	const string& GetDevUri() const
	{
		return devChn.GetDevUri();
	}
public:
	void SetDevChn(const TCmsChannel& cDevChn)
	{
		devChn = cDevChn;
	}
	TCmsChannel& GetDevChn()
	{
		return devChn;
	}
	const TCmsChannel& GetDevChn() const
	{
		return devChn;
	}
	void SetSnapShotCfg(const TCmsPuSnapShotCfg& tSnapShotCfg)
	{
		puSnapShotCfg = tSnapShotCfg;
	}
	TCmsPuSnapShotCfg& GetSnapShotCfg()
	{
		return puSnapShotCfg;
	}
	const TCmsPuSnapShotCfg& GetSnapShotCfg() const
	{
		return puSnapShotCfg;
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsPuSnapShotCfgSetReq : public CCmsEventReq
{
public:
	CCmsPuSnapShotCfgSetReq()
	{
		Clear();
		SetEvent(CMS_PU_SNAPSHOT_CFG_SET_REQ);
	}

	void Clear()
	{
		devChn.Clear();
		puSnapShotCfg.Clear();
	}
private:
	TCmsChannel  devChn; 
	TCmsPuSnapShotCfg puSnapShotCfg;
public:
	//事务模板会使用
	const string& GetDevUri() const
	{
		return devChn.GetDevUri();
	}
public:
	void SetDevChn(const TCmsChannel& cDevChn)
	{
		devChn = cDevChn;
	}
	TCmsChannel& GetDevChn()
	{
		return devChn;
	}
	const TCmsChannel& GetDevChn() const
	{
		return devChn;
	}
	void SetSnapShotCfg(const TCmsPuSnapShotCfg& tSnapShotCfg)
	{
		puSnapShotCfg = tSnapShotCfg;
	}
	TCmsPuSnapShotCfg& GetSnapShotCfg()
	{
		return puSnapShotCfg;
	}
	const TCmsPuSnapShotCfg& GetSnapShotCfg() const
	{
		return puSnapShotCfg;
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsPuSnapShotCfgSetRsp : public CCmsEventRsp
{
public:
	CCmsPuSnapShotCfgSetRsp()
	{
		Clear();
		SetEvent(CMS_PU_SNAPSHOT_CFG_SET_RSP);
	}
	void Clear()
	{
		devChn.Clear();
	}
private:
	TCmsChannel           devChn; 

public:
	const string& GetDevUri() const
	{
		return devChn.GetDevUri();
	}
public:
	void SetDevChn(const TCmsChannel& cDevChn)
	{
		devChn = cDevChn;
	}
	TCmsChannel& GetDevChn()
	{
		return devChn;
	}
	const TCmsChannel& GetDevChn() const
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

//串口参数配置消息协议类
class  CCmsPuSerialParamCfgGetReq : public CCmsEventReq
{
public:
	CCmsPuSerialParamCfgGetReq()
	{
		Clear();
		SetEvent(CMS_PU_SERIALPARAM_CFG_GET_REQ);
	}
	void Clear()
	{
		devUri = STRING_NULL;
		puSerPortParam.Clear();
	}
private:
	string devUri;
	TCmsPuSerialPortParam puSerPortParam;

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

	void SetCmsPuSerPortParam(const TCmsPuSerialPortParam& tPuSerPortParam)
	{
		puSerPortParam = tPuSerPortParam;
	}
	TCmsPuSerialPortParam& GetCmsPuSerPortParam()
	{
		return puSerPortParam;
	}
	const TCmsPuSerialPortParam& GetCmsPuSerPortParam() const
	{
		return puSerPortParam;
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsPuSerialParamCfgGetRsp : public CCmsEventRsp
{
public:
	CCmsPuSerialParamCfgGetRsp()
	{
		Clear();
		SetEvent(CMS_PU_SERIALPARAM_CFG_GET_RSP);
	}
	void Clear()
	{
		devUri = STRING_NULL;
		puSerPortParam.Clear();
	}
private:
	string devUri;
	TCmsPuSerialPortParam puSerPortParam;

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

	void SetCmsPuSerPortParam(const TCmsPuSerialPortParam& tPuSerPortParam)
	{
		puSerPortParam = tPuSerPortParam;
	}
	TCmsPuSerialPortParam& GetCmsPuSerPortParam()
	{
		return puSerPortParam;
	}
	const TCmsPuSerialPortParam& GetCmsPuSerPortParam() const
	{
		return puSerPortParam;
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsPuSerialParamCfgSetReq : public CCmsEventReq
{
public:
	CCmsPuSerialParamCfgSetReq()
	{
		Clear();
		SetEvent(CMS_PU_SERIALPARAM_CFG_SET_REQ);
	}
	void Clear()
	{
		devUri = STRING_NULL;
		puSerPortParam.Clear();
	}
private:
	string devUri;
	TCmsPuSerialPortParam puSerPortParam;

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

	void SetCmsPuSerPortParam(const TCmsPuSerialPortParam& tPuSerPortParam)
	{
		puSerPortParam = tPuSerPortParam;
	}
	TCmsPuSerialPortParam& GetCmsPuSerPortParam()
	{
		return puSerPortParam;
	}
	const TCmsPuSerialPortParam& GetCmsPuSerPortParam() const
	{
		return puSerPortParam;
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsPuSerialParamCfgSetRsp : public CCmsEventRsp
{
public:
	CCmsPuSerialParamCfgSetRsp()
	{
		Clear();
		SetEvent(CMS_PU_SERIALPARAM_CFG_SET_RSP);
	}
	void Clear()
	{
		devUri = STRING_NULL;
	}
private:
	string devUri;

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


//告警联动设置，布防撤防
class  CCmsAlarmLinkCfgSetReq : public CCmsEventReq
{
public:
    CCmsAlarmLinkCfgSetReq()
    {
        Clear();
        SetEvent(CMS_ALARMLINK_CFG_SET_REQ);
    }

    void Clear()
    {
        cfg.Clear();
        CCmsEventReq::Clear();
    }

private:
    TCmsAlarmLinkage      cfg;                      //联动配置
public:
    void SetCfg(const TCmsAlarmLinkage& cCfg)
    {
        cfg = cCfg;
    }
    TCmsAlarmLinkage& GetCfg()
    {
        return cfg;
    }
    const TCmsAlarmLinkage& GetCfg() const
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

class  CCmsAlarmLinkCfgSetRsp : public CCmsEventRsp
{
public:
    CCmsAlarmLinkCfgSetRsp()
    {
        SetEvent(CMS_ALARMLINK_CFG_SET_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

//告警联动获取
class  CCmsAlarmLinkCfgGetReq : public CCmsEventReq
{
public:
    CCmsAlarmLinkCfgGetReq()
    {
        Clear();;
        SetEvent(CMS_ALARMLINK_CFG_GET_REQ);
    }

    void Clear()
    {
        alarmSrc.Clear();
        CCmsEventReq::Clear();
    }

private:
    TCmsAlarmLinkSrc      alarmSrc;                      //告警源
public:
    void SetAlarmSrc(const TCmsAlarmLinkSrc& cAlarmSrc)
    {
        alarmSrc = cAlarmSrc;
    }
    TCmsAlarmLinkSrc& GetAlarmSrc()
    {
        return alarmSrc;
    }
    const TCmsAlarmLinkSrc& GetAlarmSrc() const
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

class  CCmsAlarmLinkCfgGetRsp : public CCmsEventRsp
{
public:
    CCmsAlarmLinkCfgGetRsp()
    {
        Clear();
        SetEvent(CMS_ALARMLINK_CFG_GET_RSP);
    }

    void Clear()
    {
        cfg.Clear();
        CCmsEventRsp::Clear();
    }

private:
    TCmsAlarmLinkage      cfg;                      //联动配置
public:
    void SetCfg(const TCmsAlarmLinkage& cCfg)
    {
        cfg = cCfg;
    }
    TCmsAlarmLinkage& GetCfg()
    {
        return cfg;
    }
    const TCmsAlarmLinkage& GetCfg() const
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

//---------------------------------------------------------------------------------
//                                3as功能相关部分
//---------------------------------------------------------------------------------

class  CCmsDeviceAddReq : public CCmsEventReq
{
private:
    TCmsDeviceInfo deviceInfo;

public:
    CCmsDeviceAddReq()
    {
        Clear();
        SetEvent(CMS_3AS_DEVICE_ADD_REQ);
    }

    ~CCmsDeviceAddReq()
    {
        Clear();
    }

    void Clear()
    {
        deviceInfo.Clear();
    }

    const TCmsDeviceInfo &GetDeviceInfo() const
    {
        return deviceInfo;
    }

    void SetDeviceInfo(const TCmsDeviceInfo &val)
    {
        deviceInfo = val;
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsDeviceAddRsp : public CCmsEventRsp
{
private:
	string uri;
public:
    CCmsDeviceAddRsp()
    {
        Clear();
        SetEvent(CMS_3AS_DEVICE_ADD_RSP);
    }

    ~CCmsDeviceAddRsp()
    {
        Clear();
    }

    void Clear()
    {
		uri = STRING_NULL;
    }

	const string &GetUri() const
	{
		return uri;
	}

	void SetUri(const string &val)
	{
		uri = val;
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsDeviceAddNtfReq : public CCmsEventReq
{
private:
	TCmsDeviceInfo deviceInfo;

public:
	CCmsDeviceAddNtfReq()
	{
		Clear();
		SetEvent(CMS_3AS_DEVICE_ADD_NTF_REQ);
	}

	~CCmsDeviceAddNtfReq()
	{
		Clear();
	}

	void Clear()
	{
		deviceInfo.Clear();
	}

	const TCmsDeviceInfo &GetDeviceInfo() const
	{
		return deviceInfo;
	}

	void SetDeviceInfo(const TCmsDeviceInfo &val)
	{
		deviceInfo = val;
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsDeviceAddNtfRsp : public CCmsEventRsp
{
public:
	CCmsDeviceAddNtfRsp()
	{
		Clear();
		SetEvent(CMS_3AS_DEVICE_ADD_NTF_RSP);
	}

	~CCmsDeviceAddNtfRsp()
	{
		Clear();
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

class  CCmsDeviceDelReq : public CCmsEventReq
{
private:
    string uri;                  // 设备uri，devId@domain
public:
    CCmsDeviceDelReq()
    {
        Clear();
        SetEvent(CMS_3AS_DEVICE_DEL_REQ);
    }

    ~CCmsDeviceDelReq()
    {
        Clear();
    }

    void Clear()
    {
        uri = STRING_NULL;
    }

	const string &GetUri() const
	{
		return uri;
	}

	void SetUri(const string &val)
	{
		uri = val;
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsDeviceDelRsp : public CCmsEventRsp
{
public:
    CCmsDeviceDelRsp()
    {
        Clear();
        SetEvent(CMS_3AS_DEVICE_DEL_RSP);
    }

    ~CCmsDeviceDelRsp()
    {
        Clear();
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

class  CCmsDeviceDelNtfReq : public CCmsEventReq
{
private:
	string uri;                  // 设备uri，devId@domain
public:
	CCmsDeviceDelNtfReq()
	{
		Clear();
		SetEvent(CMS_3AS_DEVICE_DEL_NTF_REQ);
	}

	~CCmsDeviceDelNtfReq()
	{
		Clear();
	}

	void Clear()
	{
		uri = STRING_NULL;
	}

	const string &GetUri() const
	{
		return uri;
	}

	void SetUri(const string &val)
	{
		uri = val;
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsDeviceDelNtfRsp : public CCmsEventRsp
{
public:
	CCmsDeviceDelNtfRsp()
	{
		Clear();
		SetEvent(CMS_3AS_DEVICE_DEL_NTF_RSP);
	}

	~CCmsDeviceDelNtfRsp()
	{
		Clear();
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


/*
 * 不支持修改设备的设备类型，即从编码器改为解码器或从解码器改为编码器等
 */
class  CCmsDeviceModReq : public CCmsEventReq
{
private:
    string			  uri;								   // 设备uri，devId@domain
    CCmsOptionField   optionFields;                        // 说明需要修改以下哪些信息
    string            name;                                // 设备名
    string            alias;                               // 设备别名
    string            description;                         // 设备描述信息
    string            modelName;                           // 设备型号名称（为空由cmssdk自动生成）
    string            manufacturer;                        // 设备制造商
    vector<TCmsDeviceCapInfo> capInfo;                     // 设备能力
    TCmsPropertyFlag  propertyFlag;                        // 设备属性标志
    bool              usedFlag;                            // 启用标志：0-停用，1-启用
    TCmsNetType       netType;                             // 接入线路方式：0-CMU和PU在同一网络，1-CMU和PU在不同网络
    string            netAddress;                          // 设备网络地址，IP或域名
    string            expireDate;                          // 使用截止日期
	TCmsDevExtData    devData;							   // 设备扩展信息

public:
    CCmsDeviceModReq()
    {
        Clear();
        SetEvent(CMS_3AS_DEVICE_MOD_REQ);
    }

    ~CCmsDeviceModReq()
    {
        Clear();
    }

    void Clear()
    {
        uri = STRING_NULL;
        optionFields.Clear();
        name = STRING_NULL;
        alias = STRING_NULL;
        description = STRING_NULL;
        modelName = STRING_NULL;
        manufacturer = STRING_NULL;
        capInfo.clear();
        propertyFlag = CMS_UNKNOWN;
        usedFlag = true;
        netType  = CMS_DEVICE_LINEMODE_CMU_OUTNAT_PU_OUTNAT;
        netAddress = STRING_NULL;
        expireDate = STRING_NULL;
		devData.Clear();
    }

	const string &GetUri() const
	{
		return uri;
	}

	void SetUri(const string &val)
	{
		uri = val;
	}

    bool GetName(string &val) const
    {
        if (!optionFields.HasOption("name"))
            return false;
        val = name;
        return true;
    }

    void SetName(const string &val)
    {
        name = val;
        optionFields.SetOption("name");
    }

    bool GetAlias(string &val) const
    {
        if (!optionFields.HasOption("alias"))
            return false;
        val = alias;
        return true;
    }

    void SetAlias(const string &val)
    {
        alias = val;
        optionFields.SetOption("alias");
    }

    bool GetDescription(string &val) const
    {
        if (!optionFields.HasOption("description"))
            return false;
        val = description;
        return true;
    }

    void SetDescription(const string &val)
    {
        description = val;
        optionFields.SetOption("description");
    }

	bool GetModelName(string &val) const
	{
		if (!optionFields.HasOption("modelName"))
			return false;
		val = modelName;
		return true;
	}

	void SetModelName(const string &val)
	{
		modelName = val;
		optionFields.SetOption("modelName");
	}

    bool GetManufacturer(string &val) const
    {
        if (!optionFields.HasOption("manufacturer"))
            return false;
        val = manufacturer;
        return true;
    }

    void SetManufacturer(const string &val)
    {
        manufacturer = val;
        optionFields.SetOption("manufacturer");
    }

    bool GetCapInfo(vector<TCmsDeviceCapInfo> &val) const
    {
        if (!optionFields.HasOption("capInfo"))
            return false;
        val = capInfo;
        return true;
    }

    void SetCapInfo(const vector<TCmsDeviceCapInfo> &val)
    {
        capInfo = val;
        optionFields.SetOption("capInfo");
    }

    bool GetPropertyFlag(TCmsPropertyFlag &val) const
    {
        if (!optionFields.HasOption("propertyFlag"))
            return false;
        val = propertyFlag;
        return true;
    }

    void SetPropertyFlag(TCmsPropertyFlag val)
    {
        propertyFlag = val;
        optionFields.SetOption("propertyFlag");
    }

    bool GetUsedFlag(bool &val) const
    {
        if (!optionFields.HasOption("usedFlag"))
            return false;
        val = usedFlag;
        return true;
    }

    void SetUsedFlag(bool val)
    {
        usedFlag = val;
        optionFields.SetOption("usedFlag");
    }

    bool GetNetType(TCmsNetType &val) const
    {
        if (!optionFields.HasOption("netType"))
            return false;
        val = netType;
        return true;
    }

    void SetNetType(TCmsNetType val)
    {
        netType = val;
        optionFields.SetOption("netType");
    }

    bool GetNetAddress(string &val) const
    {
        if (!optionFields.HasOption("netAddress"))
            return false;
        val = netAddress;
        return true;
    }

    void SetNetAddress(const string &val)
    {
        netAddress = val;
        optionFields.SetOption("netAddress");
    }

    bool GetExpireDate(string &val) const
    {
        if (!optionFields.HasOption("expireDate"))
            return false;
        val = expireDate;
        return true;
    }

    void SetExpireDate(const string &val)
    {
        expireDate = val;
        optionFields.SetOption("expireDate");
    }

	bool GetDevData(TCmsDevExtData &val) const
	{
		if (!optionFields.HasOption("devData"))
			return false;
		val = devData;
		return true;
	}

	void SetDevData(const TCmsDevExtData &val)
	{
		devData = val;
		optionFields.SetOption("devData");
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsDeviceModRsp : public CCmsEventRsp
{
public:
    CCmsDeviceModRsp()
    {
        Clear();
        SetEvent(CMS_3AS_DEVICE_MOD_RSP);
    }

    ~CCmsDeviceModRsp()
    {
        Clear();
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

class  CCmsDeviceModNtfReq : public CCmsEventReq
{
private:
	TCmsDeviceInfo deviceInfo;

public:
	CCmsDeviceModNtfReq()
	{
		Clear();
		SetEvent(CMS_3AS_DEVICE_MOD_NTF_REQ);
	}

	~CCmsDeviceModNtfReq()
	{
		Clear();
	}

	void Clear()
	{
		deviceInfo.Clear();
	}

	const TCmsDeviceInfo &GetDeviceInfo() const
	{
		return deviceInfo;
	}

	void SetDeviceInfo(const TCmsDeviceInfo &val)
	{
		deviceInfo = val;
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsDeviceModNtfRsp : public CCmsEventRsp
{
public:
	CCmsDeviceModNtfRsp()
	{
		Clear();
		SetEvent(CMS_3AS_DEVICE_MOD_NTF_RSP);
	}

	~CCmsDeviceModNtfRsp()
	{
		Clear();
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

class  CCmsDeviceQryReq : public CCmsEventReq
{
private:
    int    startEntryIndex;             // 起始查询索引
    int    expectEntryNum;              // 期望返回记录数量
    vector<TCmsSortItem> sortFields;    // 排序列表

    CCmsOptionField optionFields;       // 说明需要使用以下哪些查询条件
    string uri;							// 设备uri，devId@domain
    string idBegin;                     // 设备Id大于idBegin
	string domainId;					// 域Id
    string name;                        // 设备名
    string alias;                       // 设备别名
    set<TCmsDeviceType> types;          // 设备类型集合（支持一次指定多个类型）
    bool usedFlag;                      // 启停状态标志：0停用，非0启用
    TCmsNetType netType;                // 接入线路方式：参见常量定义
    string netAddress;                  // 设备网络地址，IP或域名
    string description;                 // 设备描述
    string expireDate_From;             // 使用截止日期_查询起点
    string expireDate_To;               // 使用截止日期_查询终点
    string createDate_From;             // 入网日期_查询起点
    string createDate_To;               // 入网日期_查询终点
	set<TCmsCreater> creaters;			// 创建者集合
public:
    CCmsDeviceQryReq()
    {
        Clear();
        SetEvent(CMS_3AS_DEVICE_QRY_REQ);
    }

    ~CCmsDeviceQryReq()
    {
        Clear();
    }

    void Clear()
    {
        startEntryIndex = 0;
        expectEntryNum  = 0;
        sortFields.clear();

        optionFields.Clear();
        uri = STRING_NULL;
		domainId = STRING_NULL;
        name = STRING_NULL;
        alias = STRING_NULL;
        types.clear();
        usedFlag = true;
        netType  = CMS_DEVICE_LINEMODE_CMU_OUTNAT_PU_OUTNAT;
        netAddress = STRING_NULL;
        description = STRING_NULL;
        expireDate_From = STRING_NULL;
        expireDate_To = STRING_NULL;
        createDate_From = STRING_NULL;
        createDate_To = STRING_NULL;
		creaters.clear();
    }

    int GetStartEntryIndex() const
    {
        return startEntryIndex;
    }

    void SetStartEntryIndex(int val)
    {
        startEntryIndex = val;
    }

    int GetExpectEntryNum() const
    {
        return expectEntryNum;
    }

    void SetExpectEntryNum(int val)
    {
        expectEntryNum = val;
    }

    const vector<TCmsSortItem> &GetSort() const
    {
        return sortFields;
    }

    void SetSort(const vector<TCmsSortItem> &val)
    {
        sortFields = val;
    }

    bool GetUri(string &val) const
    {
        if (!optionFields.HasOption("uri"))
            return false;
        val = uri;
        return true;
    }

    void SetId(const string &val)
    {
        uri = val;
        optionFields.SetOption("uri");
    }

	bool GetDomainId(string &val) const
	{
		if (!optionFields.HasOption("domainId"))
			return false;
		val = domainId;
		return true;
	}

	void SetDomainId(const string &val)
	{
		domainId = val;
		optionFields.SetOption("domainId");
	}

    bool GetDeviceUUIDBegin(string &val) const
    {
        if (!optionFields.HasOption("idBegin"))
            return false;
        val = idBegin;
        return true;
    }
    void SetDeviceUUIDBegin(const string &val)
    {
        idBegin = val;
        optionFields.SetOption("idBegin");
    }

    bool GetName(string &val) const
    {
        if (!optionFields.HasOption("name"))
            return false;
        val = name;
        return true;
    }

    void SetName(const string &val)
    {
        name = val;
        optionFields.SetOption("name");
    }

    bool GetAlias(string &val) const
    {
        if (!optionFields.HasOption("alias"))
            return false;
        val = alias;
        return true;
    }

    void SetAlias(const string &val)
    {
        alias = val;
        optionFields.SetOption("alias");
    }

    bool GetTypes(set<TCmsDeviceType> &val) const
    {
        if (!optionFields.HasOption("types"))
            return false;
        val = types;
        return true;
    }

    void SetTypes(const set<TCmsDeviceType> &val)
    {
        types = val;
        optionFields.SetOption("types");
    }

    bool GetUsedFlag(bool &val) const
    {
        if (!optionFields.HasOption("usedFlag"))
            return false;
        val = usedFlag;
        return true;
    }

    void SetUsedFlag(bool val)
    {
        usedFlag = val;
        optionFields.SetOption("usedFlag");
    }

    bool GetNetType(TCmsNetType &val) const
    {
        if (!optionFields.HasOption("netType"))
            return false;
        val = netType;
        return true;
    }

    void SetNetType(TCmsNetType val)
    {
        netType = val;
        optionFields.SetOption("netType");
    }

    bool GetNetAddress(string &val) const
    {
        if (!optionFields.HasOption("netAddress"))
            return false;
        val = netAddress;
        return true;
    }

    void SetNetAddress(const string &val)
    {
        netAddress = val;
        optionFields.SetOption("netAddress");
    }

    bool GetDescription(string &val) const
    {
        if (!optionFields.HasOption("description"))
            return false;
        val = description;
        return true;
    }

    void SetDescription(const string &val)
    {
        description = val;
        optionFields.SetOption("description");
    }

    bool GetExpireDate_From(string &val) const
    {
        if (!optionFields.HasOption("expireDate_From"))
            return false;
        val = expireDate_From;
        return true;
    }

    void SetExpireDate_From(const string &val)
    {
        expireDate_From = val;
        optionFields.SetOption("expireDate_From");
    }

    bool GetExpireDate_To(string &val) const
    {
        if (!optionFields.HasOption("expireDate_To"))
            return false;
        val = expireDate_To;
        return true;
    }

    void SetExpireDate_To(const string &val)
    {
        expireDate_To = val;
        optionFields.SetOption("expireDate_To");
    }

    bool GetCreateDate_From(string &val) const
    {
        if (!optionFields.HasOption("createDate_From"))
            return false;
        val = createDate_From;
        return true;
    }

    void SetCreateDate_From(const string &val)
    {
        createDate_From = val;
        optionFields.SetOption("createDate_From");
    }

    bool GetCreateDate_To(string &val) const
    {
        if (!optionFields.HasOption("createDate_To"))
            return false;
        val = createDate_To;
        return true;
    }

    void SetCreateDate_To(const string &val)
    {
        createDate_To = val;
        optionFields.SetOption("createDate_To");
    }

	bool GetCreaters(set<TCmsCreater> &val) const
	{
		if (!optionFields.HasOption("creaters"))
			return false;
		val = creaters;
		return true;
	}

	void SetCreaters(const set<TCmsCreater> &val)
	{
		creaters = val;
		optionFields.SetOption("creaters");
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsDeviceQryRsp : public CCmsEventRsp
{
private:
	int totalEntryNum;
	int entryNum;
public:
	CCmsDeviceQryRsp()
	{
		Clear();
		SetEvent(CMS_3AS_DEVICE_QRY_RSP);
	}

	~CCmsDeviceQryRsp()
	{
		Clear();
	}

	void Clear()
	{
		totalEntryNum = 0;
		entryNum = 0;
	}

	int GetTotalEntryNum() const
	{
		return totalEntryNum;
	}

	void SetTotalEntryNum(int val)
	{
		totalEntryNum = val;
	}

	int GetEntryNum() const
	{
		return entryNum;
	}

	void SetEntryNum(int val)
	{
		entryNum = val;
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class  CCmsDeviceQryNtfReq : public CCmsEventReq
{
private:
	int  entryNum;
	bool lastNtf;
    vector<TCmsDeviceInfo> deviceInfo;
public:
    CCmsDeviceQryNtfReq()
    {
        Clear();
        SetEvent(CMS_3AS_DEVICE_QRY_NTF_REQ);
    }

    ~CCmsDeviceQryNtfReq()
    {
        Clear();
    }

    void Clear()
    {
		entryNum = 0;
		lastNtf = false;
		deviceInfo.clear();
    }

	int GetEntryNum() const
	{
		return entryNum;
	}

	void SetEntryNum(int val)
	{
		entryNum = val;
	}

	bool IsLastNtf() const
	{
		return lastNtf;
	}
	void SetLastNtf(bool val)
	{
		lastNtf = val;
	}

    const vector<TCmsDeviceInfo> &GetDeviceInfo() const
    {
        return deviceInfo;
    }

    void SetDeviceInfo(const vector<TCmsDeviceInfo> &val)
    {
        deviceInfo = val;
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsDeviceQryNtfRsp : public CCmsEventRsp
{
public:
	CCmsDeviceQryNtfRsp()
	{
		Clear();
		SetEvent(CMS_3AS_DEVICE_QRY_NTF_RSP);
	}

	~CCmsDeviceQryNtfRsp()
	{
		Clear();
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

class  CCmsDeviceGroupAddReq : public CCmsEventReq
{
private:
    TCmsDeviceGroupInfo groupInfo;
public:
    CCmsDeviceGroupAddReq()
    {
        Clear();
        SetEvent(CMS_3AS_DEVICE_GROUP_ADD_REQ);
    }

    ~CCmsDeviceGroupAddReq()
    {
        Clear();
    }

    void Clear()
    {
        groupInfo.Clear();
    }

    const TCmsDeviceGroupInfo &GetGroupInfo() const
    {
        return groupInfo;
    }

    void SetGroupInfo(const TCmsDeviceGroupInfo &val)
    {
        groupInfo = val;
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsDeviceGroupAddRsp : public CCmsEventRsp
{
private:
	string groupUri;
public:
    CCmsDeviceGroupAddRsp()
    {
        Clear();
        SetEvent(CMS_3AS_DEVICE_GROUP_ADD_RSP);
    }

    ~CCmsDeviceGroupAddRsp()
    {
        Clear();
    }

    void Clear()
    {
    }

	const string &GetGroupUri() const
	{
		return groupUri;
	}
	void SetGroupUri(const string &val)
	{
		groupUri = val;
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsDeviceGroupAddNtfReq: public CCmsEventReq
{
public:
	CCmsDeviceGroupAddNtfReq()
	{
		Clear();
		SetEvent(CMS_3AS_DEVICE_GROUP_ADD_NTF_REQ);
	}

	~CCmsDeviceGroupAddNtfReq()
	{
		Clear();
	}

	void Clear()
	{
		info.Clear();
	}

	TCmsDeviceGroupInfo& GetDeviceGroupInfo() 
	{
		return info;
	}
	const TCmsDeviceGroupInfo& GetDeviceGroupInfo() const
	{
		return info;
	}
	void SetDeviceGroupInfo(const TCmsDeviceGroupInfo &val)
	{
		info = val;
	}

private:
	TCmsDeviceGroupInfo info;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsDeviceGroupAddNtfRsp : public CCmsEventRsp
{
public:
	CCmsDeviceGroupAddNtfRsp()
	{
		Clear();
		SetEvent(CMS_3AS_DEVICE_GROUP_ADD_NTF_RSP);
	}

	~CCmsDeviceGroupAddNtfRsp()
	{
		Clear();
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

class  CCmsDeviceGroupDelReq : public CCmsEventReq
{
private:
    string uri;              // 组uri
public:
    CCmsDeviceGroupDelReq()
    {
        Clear();
        SetEvent(CMS_3AS_DEVICE_GROUP_DEL_REQ);
    }

    ~CCmsDeviceGroupDelReq()
    {
        Clear();
    }

    void Clear()
    {
        uri = STRING_NULL;
    }

    const string &GetUri() const
    {
        return uri;
    }

    void SetUri(const string &val)
    {
        uri = val;
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsDeviceGroupDelRsp : public CCmsEventRsp
{
public:
    CCmsDeviceGroupDelRsp()
    {
        Clear();
        SetEvent(CMS_3AS_DEVICE_GROUP_DEL_RSP);
    }

    ~CCmsDeviceGroupDelRsp()
    {
        Clear();
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

class  CCmsDeviceGroupDelNtfReq: public CCmsEventReq
{
public:
	CCmsDeviceGroupDelNtfReq()
	{
		Clear();
		SetEvent(CMS_3AS_DEVICE_GROUP_DEL_NTF_REQ);
	}

	~CCmsDeviceGroupDelNtfReq()
	{
		Clear();
	}

	void Clear()
	{
		groupUri = STRING_NULL;
	}

	const string &GetGroupUri() const
	{
		return groupUri;
	}

	string &GetGroupUri()
	{
		return groupUri;
	}

	void SetGroupUri(const string &val)
	{
		groupUri = val;
	}

private:
	string groupUri;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsDeviceGroupDelNtfRsp : public CCmsEventRsp
{
public:
	CCmsDeviceGroupDelNtfRsp()
	{
		Clear();
		SetEvent(CMS_3AS_DEVICE_GROUP_DEL_NTF_RSP);
	}

	~CCmsDeviceGroupDelNtfRsp()
	{
		Clear();
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

class  CCmsDeviceGroupModReq : public CCmsEventReq
{
private:
    string uri;                         // 组uri
    CCmsOptionField optionFields;       // 说明需要修改以下哪些信息
    string parentUri;                   // 父组uri
    string name;                        // 组名
    string description;                 // 组描述信息

public:
    CCmsDeviceGroupModReq()
    {
        Clear();
        SetEvent(CMS_3AS_DEVICE_GROUP_MOD_REQ);
    }

    ~CCmsDeviceGroupModReq()
    {
        Clear();
    }

    void Clear()
    {
        uri = STRING_NULL;
        optionFields.Clear();
        parentUri = STRING_NULL;
        name = STRING_NULL;
        description = STRING_NULL;
    }

    const string &GetUri() const
    {
        return uri;
    }

    void SetUri(const string &val)
    {
        uri = val;
    }

    bool GetParentUri(string &val) const
    {
        if (!optionFields.HasOption("parentUri"))
            return false;
        val = parentUri;
        return true;
    }

    void SetParentUri(const string &val)
    {
        parentUri = val;
        optionFields.SetOption("parentUri");
    }

    bool GetName(string &val) const
    {
        if (!optionFields.HasOption("name"))
            return false;
        val = name;
        return true;
    }

    void SetName(const string &val)
    {
        name = val;
        optionFields.SetOption("name");
    }

    bool GetDescription(string &val) const
    {
        if (!optionFields.HasOption("description"))
            return false;
        val = description;
        return true;
    }

    void SetDescription(const string &val)
    {
        description = val;
        optionFields.SetOption("description");
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsDeviceGroupModRsp : public CCmsEventRsp
{
public:
    CCmsDeviceGroupModRsp()
    {
        Clear();
        SetEvent(CMS_3AS_DEVICE_GROUP_MOD_RSP);
    }

    ~CCmsDeviceGroupModRsp()
    {
        Clear();
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

class  CCmsDeviceGroupModNtfReq: public CCmsEventReq
{
public:
	CCmsDeviceGroupModNtfReq()
	{
		Clear();
		SetEvent(CMS_3AS_DEVICE_GROUP_MOD_NTF_REQ);
	}

	~CCmsDeviceGroupModNtfReq()
	{
		Clear();
	}

	void Clear()
	{
		info.Clear();
	}

	TCmsDeviceGroupInfo& GetDeviceGroupInfo() 
	{
		return info;
	}
	const TCmsDeviceGroupInfo& GetDeviceGroupInfo() const
	{
		return info;
	}
	void SetDeviceGroupInfo(const TCmsDeviceGroupInfo &val)
	{
		info = val;
	}

private:
	TCmsDeviceGroupInfo info;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsDeviceGroupModNtfRsp : public CCmsEventRsp
{
public:
	CCmsDeviceGroupModNtfRsp()
	{
		Clear();
		SetEvent(CMS_3AS_DEVICE_GROUP_MOD_NTF_RSP);
	}

	~CCmsDeviceGroupModNtfRsp()
	{
		Clear();
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

class  CCmsDeviceGroupQryReq : public CCmsEventReq
{
private:
    int    startEntryIndex;             // 起始查询索引
    int    expectEntryNum;              // 期望返回记录数量
    vector<TCmsSortItem> sortFields;    // 排序列表

    CCmsOptionField optionFields;       // 说明需要使用以下哪些查询条件
    string uri;                         // 设备组uri
	string domainId;					// 设备组域Id
    string parentUri;                   // 父组uri
    string name;                        // 设备名
    string description;                 // 设备描述
    string createDate_From;             // 创建组日期_查询起点
    string createDate_To;               // 创建组日期_查询终点
	set<TCmsCreater> creaters;			// 创建者集合
public:
    CCmsDeviceGroupQryReq()
    {
        Clear();
        SetEvent(CMS_3AS_DEVICE_GROUP_QRY_REQ);
    }

    ~CCmsDeviceGroupQryReq()
    {
        Clear();
    }

    void Clear()
    {
        startEntryIndex = 0;
        expectEntryNum  = 0;
        sortFields.clear();

        optionFields.Clear();
        uri = STRING_NULL;
        parentUri = STRING_NULL;
        name = STRING_NULL;
        description = STRING_NULL;
        createDate_From = STRING_NULL;
        createDate_To = STRING_NULL;
		creaters.clear();
    }

    int GetStartEntryIndex() const
    {
        return startEntryIndex;
    }

    void SetStartEntryIndex(int val)
    {
        startEntryIndex = val;
    }

    int GetExpectEntryNum() const
    {
        return expectEntryNum;
    }

    void SetExpectEntryNum(int val)
    {
        expectEntryNum = val;
    }

    const vector<TCmsSortItem> &GetSort() const
    {
        return sortFields;
    }

    void SetSort(const vector<TCmsSortItem> &val)
    {
        sortFields = val;
    }

    bool GetUri(string &val) const
    {
        if (!optionFields.HasOption("uri"))
            return false;
        val = uri;
        return true;
    }

    void SetUri(const string &val)
    {
        uri = val;
        optionFields.SetOption("uri");
    }

	bool GetDomainId(string &val) const
	{
		if (!optionFields.HasOption("domainId"))
			return false;
		val = domainId;
		return true;
	}

	void SetDomainId(const string &val)
	{
		domainId = val;
		optionFields.SetOption("domainId");
	}

    bool GetParentUri(string &val) const
    {
        if (!optionFields.HasOption("parentUri"))
            return false;
        val = parentUri;
        return true;
    }

    void SetParentUri(const string &val)
    {
        parentUri = val;
        optionFields.SetOption("parentUri");
    }

    bool GetName(string &val) const
    {
        if (!optionFields.HasOption("name"))
            return false;
        val = name;
        return true;
    }

    void SetName(const string &val)
    {
        name = val;
        optionFields.SetOption("name");
    }

    bool GetDescription(string &val) const
    {
        if (!optionFields.HasOption("description"))
            return false;
        val = description;
        return true;
    }

    void SetDescription(const string &val)
    {
        description = val;
        optionFields.SetOption("description");
    }

    bool GetCreateDate_From(string &val) const
    {
        if (!optionFields.HasOption("createDate_From"))
            return false;
        val = createDate_From;
        return true;
    }

    void SetCreateDate_From(const string &val)
    {
        createDate_From = val;
        optionFields.SetOption("createDate_From");
    }

    bool GetCreateDate_To(string &val) const
    {
        if (!optionFields.HasOption("createDate_To"))
            return false;
        val = createDate_To;
        return true;
    }

    void SetCreateDate_To(const string &val)
    {
        createDate_To = val;
        optionFields.SetOption("createDate_To");
    }

	bool GetCreaters(set<TCmsCreater> &val) const
	{
		if (!optionFields.HasOption("creaters"))
			return false;
		val = creaters;
		return true;
	}

	void SetCreaters(const set<TCmsCreater> &val)
	{
		creaters = val;
		optionFields.SetOption("creaters");
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsDeviceGroupQryRsp : public CCmsEventRsp
{
private:
	int totalEntryNum;
	int entryNum;
public:
	CCmsDeviceGroupQryRsp()
	{
		Clear();
		SetEvent(CMS_3AS_DEVICE_GROUP_QRY_RSP);
	}

	~CCmsDeviceGroupQryRsp()
	{
		Clear();
	}

	void Clear()
	{
		totalEntryNum = 0;
		entryNum = 0;
	}

	int GetTotalEntryNum() const
	{
		return totalEntryNum;
	}

	void SetTotalEntryNum(int val)
	{
		totalEntryNum = val;
	}

	int GetEntryNum() const
	{
		return entryNum;
	}

	void SetEntryNum(int val)
	{
		entryNum = val;
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsDeviceGroupQryNtfReq : public CCmsEventReq
{
private:
	int entryNum;
	bool lastNtf;
    vector<TCmsDeviceGroupInfo> groupInfo;
public:
    CCmsDeviceGroupQryNtfReq()
    {
        Clear();
        SetEvent(CMS_3AS_DEVICE_GROUP_QRY_NTF_REQ);
    }

    ~CCmsDeviceGroupQryNtfReq()
    {
        Clear();
    }

	void Clear()
	{
		entryNum = 0;
		lastNtf = false;
		groupInfo.clear();
	}

	int GetEntryNum() const
	{
		return entryNum;
	}

	void SetEntryNum(int val)
	{
		entryNum = val;
	}

	bool IsLastNtf() const
	{
		return lastNtf;
	}
	void SetLastNtf(bool val)
	{
		lastNtf = val;
	}

	const vector<TCmsDeviceGroupInfo> &GetGroupInfo() const
	{
		return groupInfo;
	}

	void SetGroupInfo(const vector<TCmsDeviceGroupInfo> &val)
	{
		groupInfo = val;
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsDeviceGroupQryNtfRsp : public CCmsEventRsp
{
public:
	CCmsDeviceGroupQryNtfRsp()
	{
		Clear();
		SetEvent(CMS_3AS_DEVICE_GROUP_QRY_NTF_RSP);
	}

	~CCmsDeviceGroupQryNtfRsp()
	{
		Clear();
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

class  CCmsDeviceGroupDeviceAddReq : public CCmsEventReq
{
private:
	CCmsDeviceGroupDeviceInfo info;

public:
    CCmsDeviceGroupDeviceAddReq()
    {
        Clear();
        SetEvent(CMS_3AS_DEVICE_GROUP_DEVICE_ADD_REQ);
    }

    ~CCmsDeviceGroupDeviceAddReq()
    {
        Clear();
    }

    void Clear()
    {
        info.Clear();
    }

	CCmsDeviceGroupDeviceInfo& GetDeviceGroupDeviceInfo() 
	{
		return info;
	}
	const CCmsDeviceGroupDeviceInfo& GetDeviceGroupDeviceInfo() const
	{
		return info;
	}
	void SetDeviceGroupDeviceInfo(const CCmsDeviceGroupDeviceInfo &val)
	{
		info = val;
	}

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsDeviceGroupDeviceAddRsp : public CCmsEventRsp
{
public:
    CCmsDeviceGroupDeviceAddRsp()
    {
        Clear();
        SetEvent(CMS_3AS_DEVICE_GROUP_DEVICE_ADD_RSP);
    }

    ~CCmsDeviceGroupDeviceAddRsp()
    {
        Clear();
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

class  CCmsDeviceGroupDeviceAddNtfReq: public CCmsEventReq
{
public:
	CCmsDeviceGroupDeviceAddNtfReq()
	{
		Clear();
		SetEvent(CMS_3AS_DEVICE_GROUP_DEVICE_ADD_NTF_REQ);
	}

	~CCmsDeviceGroupDeviceAddNtfReq()
	{
		Clear();
	}

	void Clear()
	{
		info.Clear();
	}

	CCmsDeviceGroupDeviceInfo& GetDeviceGroupDeviceInfo() 
	{
		return info;
	}
	const CCmsDeviceGroupDeviceInfo& GetDeviceGroupDeviceInfo() const
	{
		return info;
	}
	void SetDeviceGroupDeviceInfo(const CCmsDeviceGroupDeviceInfo &val)
	{
		info = val;
	}
private:
	CCmsDeviceGroupDeviceInfo info;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsDeviceGroupDeviceAddNtfRsp : public CCmsEventRsp
{
public:
	CCmsDeviceGroupDeviceAddNtfRsp()
	{
		Clear();
		SetEvent(CMS_3AS_DEVICE_GROUP_DEVICE_ADD_NTF_RSP);
	}

	~CCmsDeviceGroupDeviceAddNtfRsp()
	{
		Clear();
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

class  CCmsDeviceGroupDeviceDelReq: public CCmsEventReq
{
public:
	CCmsDeviceGroupDeviceDelReq()
	{
		Clear();
		SetEvent(CMS_3AS_DEVICE_GROUP_DEVICE_DEL_REQ);
	}

	~CCmsDeviceGroupDeviceDelReq()
	{
		Clear();
	}

	void Clear()
	{

	}

	CCmsDeviceGroupDeviceInfo& GetDeviceGroupDeviceInfo() 
	{
		return info;
	}
	const CCmsDeviceGroupDeviceInfo& GetDeviceGroupDeviceInfo() const
	{
		return info;
	}
	void SetDeviceGroupDeviceInfo(const CCmsDeviceGroupDeviceInfo &val)
	{
		info = val;
	}
private:
	CCmsDeviceGroupDeviceInfo info;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsDeviceGroupDeviceDelRsp: public CCmsEventRsp
{
public:
	CCmsDeviceGroupDeviceDelRsp()
	{
		Clear();
		SetEvent(CMS_3AS_DEVICE_GROUP_DEVICE_DEL_RSP);
	}

	~CCmsDeviceGroupDeviceDelRsp()
	{
		Clear();
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

class  CCmsDeviceGroupDeviceDelNtfReq: public CCmsEventReq
{
public:
	CCmsDeviceGroupDeviceDelNtfReq()
	{
		Clear();
		SetEvent(CMS_3AS_DEVICE_GROUP_DEVICE_DEL_NTF_REQ);
	}

	~CCmsDeviceGroupDeviceDelNtfReq()
	{
		Clear();
	}

	void Clear()
	{

	}

	CCmsDeviceGroupDeviceInfo& GetDeviceGroupDeviceInfo() 
	{
		return info;
	}
	const CCmsDeviceGroupDeviceInfo& GetDeviceGroupDeviceInfo() const
	{
		return info;
	}
	void SetDeviceGroupDeviceInfo(const CCmsDeviceGroupDeviceInfo &val)
	{
		info = val;
	}
private:
	CCmsDeviceGroupDeviceInfo info;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsDeviceGroupDeviceDelNtfRsp : public CCmsEventRsp
{
public:
	CCmsDeviceGroupDeviceDelNtfRsp()
	{
		Clear();
		SetEvent(CMS_3AS_DEVICE_GROUP_DEVICE_DEL_NTF_RSP);
	}

	~CCmsDeviceGroupDeviceDelNtfRsp()
	{
		Clear();
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

class  CCmsDeviceCapIndexNameSetReq: public CCmsEventReq
{
public:
	CCmsDeviceCapIndexNameSetReq()
	{
		Clear();
		SetEvent(CMS_3AS_DEVICE_CAP_INDEX_NAME_SET_REQ);
	}

	~CCmsDeviceCapIndexNameSetReq()
	{
		Clear();
	}

	void Clear()
	{
		deviceCapName.Clear();
	}

	TCmsDeviceCapName& GetDeviceCapName() 
	{
		return deviceCapName;
	}
	const TCmsDeviceCapName& GetDeviceCapName() const
	{
		return deviceCapName;
	}
	void SetDeviceCapName(const TCmsDeviceCapName &val)
	{
		deviceCapName = val;
	}
private:
	TCmsDeviceCapName deviceCapName;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsDeviceCapIndexNameSetRsp : public CCmsEventRsp
{
public:
	CCmsDeviceCapIndexNameSetRsp()
	{
		Clear();
		SetEvent(CMS_3AS_DEVICE_CAP_INDEX_NAME_SET_RSP);
	}

	~CCmsDeviceCapIndexNameSetRsp()
	{
		Clear();
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

class  CCmsDeviceCapIndexNameGetReq: public CCmsEventReq
{
public:
	CCmsDeviceCapIndexNameGetReq()
	{
		Clear();
		SetEvent(CMS_3AS_DEVICE_CAP_INDEX_NAME_GET_REQ);
	}

	~CCmsDeviceCapIndexNameGetReq()
	{
		Clear();
	}

	void Clear()
	{
		deviceUri = STRING_NULL;
		capId = CMS_DEVICE_CAP_INVALID;
		capIndex = -1;					// 默认获取所有的别名信息
	}

	const string &GetDeviceUri() const
	{
		return deviceUri;
	}
	void SetDeviceUri(const string &val)
	{
		deviceUri = val;
	}

	TCmsDeviceCapType GetCapId() const
	{
		return capId;
	}
	void SetCapId(TCmsDeviceCapType val)
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
	string deviceUri;
	TCmsDeviceCapType capId;
	int capIndex;			// -1表示某设备某种能力的所有索引名称
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsDeviceCapIndexNameGetRsp : public CCmsEventRsp
{
public:
	CCmsDeviceCapIndexNameGetRsp()
	{
		Clear();
		SetEvent(CMS_3AS_DEVICE_CAP_INDEX_NAME_GET_RSP);
	}

	~CCmsDeviceCapIndexNameGetRsp()
	{
		Clear();
	}

	void Clear()
	{
		deviceCapName.Clear();
	}

	TCmsDeviceCapName& GetDeviceCapName() 
	{
		return deviceCapName;
	}
	const TCmsDeviceCapName& GetDeviceCapName() const
	{
		return deviceCapName;
	}
	void SetDeviceCapName(const TCmsDeviceCapName &val)
	{
		deviceCapName = val;
	}
private:
	TCmsDeviceCapName deviceCapName;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsDeviceCapIndexNameModNtfReq: public CCmsEventReq
{
public:
	CCmsDeviceCapIndexNameModNtfReq()
	{
		Clear();
		SetEvent(CMS_3AS_DEVICE_CAP_INDEX_NAME_MOD_NTF_REQ);
	}

	~CCmsDeviceCapIndexNameModNtfReq()
	{
		Clear();
	}

	void Clear()
	{
		deviceCapName.Clear();
	}

	TCmsDeviceCapName& GetDeviceCapName() 
	{
		return deviceCapName;
	}
	const TCmsDeviceCapName& GetDeviceCapName() const
	{
		return deviceCapName;
	}
	void SetDeviceCapName(const TCmsDeviceCapName &val)
	{
		deviceCapName = val;
	}
private:
	TCmsDeviceCapName deviceCapName;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsDeviceCapIndexNameModNtfRsp : public CCmsEventRsp
{
public:
	CCmsDeviceCapIndexNameModNtfRsp()
	{
		Clear();
		SetEvent(CMS_3AS_DEVICE_CAP_INDEX_NAME_MOD_NTF_RSP);
	}

	~CCmsDeviceCapIndexNameModNtfRsp()
	{
		Clear();
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

class CCmsDeviceCapIndexNameQryReq: public CCmsEventReq
{
private:
    int    startEntryIndex;             // 起始查询索引
    int    expectEntryNum;              // 期望返回记录数量

public:
    CCmsDeviceCapIndexNameQryReq()
    {
        Clear();
        SetEvent(CMS_3AS_DEVICE_CAP_INDEX_NAME_QRY_REQ);
    }

    ~CCmsDeviceCapIndexNameQryReq()
    {
        Clear();
    }

    void Clear()
    {
        startEntryIndex = 0;
        expectEntryNum  = 0;
    }

    int GetStartEntryIndex() const
    {
        return startEntryIndex;
    }

    void SetStartEntryIndex(int val)
    {
        startEntryIndex = val;
    }

    int GetExpectEntryNum() const
    {
        return expectEntryNum;
    }

    void SetExpectEntryNum(int val)
    {
        expectEntryNum = val;
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CCmsDeviceCapIndexNameQryRsp: public CCmsEventRsp
{
private:
    int totalEntryNum;
    int entryNum;
public:
    CCmsDeviceCapIndexNameQryRsp()
    {
        Clear();
        SetEvent(CMS_3AS_DEVICE_CAP_INDEX_NAME_QRY_RSP);
    }

    ~CCmsDeviceCapIndexNameQryRsp()
    {
        Clear();
    }

    void Clear()
    {
        totalEntryNum = 0;
        entryNum = 0;
    }

    int GetTotalEntryNum() const
    {
        return totalEntryNum;
    }

    void SetTotalEntryNum(int val)
    {
        totalEntryNum = val;
    }

    int GetEntryNum() const
    {
        return entryNum;
    }

    void SetEntryNum(int val)
    {
        entryNum = val;
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CCmsDeviceCapIndexNameQryNtfReq : public CCmsEventReq
{
public:
    CCmsDeviceCapIndexNameQryNtfReq()
    {
        Clear();
        SetEvent(CMS_3AS_DEVICE_CAP_INDEX_NAME_QRY_NTF_REQ);
    }

    ~CCmsDeviceCapIndexNameQryNtfReq()
    {
        Clear();
    }

    void Clear()
    {
        entryNum = 0;
        lastNtf  = false;
        infos.clear();
    }

    int GetEntryNum() const
    {
        return entryNum;
    }

    void SetEntryNum(int val)
    {
        entryNum = val;
    }

    bool IsLastNtf() const
    {
        return lastNtf;
    }
    void SetLastNtf(bool val)
    {
        lastNtf = val;
    }

    void SetInfos(const vector<TCmsDeviceCapName>& cInfos)
    {
        infos = cInfos;
    }
    vector<TCmsDeviceCapName>& GetInfos()
    {
        return infos;
    }
    const vector<TCmsDeviceCapName>& GetInfos() const
    {
        return infos;
    }

private:
    int entryNum;
    bool lastNtf;
    vector<TCmsDeviceCapName> infos;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CCmsDeviceCapIndexNameQryNtfRsp : public CCmsEventRsp
{
public:
    CCmsDeviceCapIndexNameQryNtfRsp()
    {
        Clear();
        SetEvent(CMS_3AS_DEVICE_CAP_INDEX_NAME_QRY_NTF_RSP);
    }

    ~CCmsDeviceCapIndexNameQryNtfRsp()
    {
        Clear();
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

class  CCmsDeviceGroupDeviceQryReq : public CCmsEventReq
{
private:
	int    startEntryIndex;             // 起始查询索引
	int    expectEntryNum;              // 期望返回记录数量
	vector<TCmsSortItem> sortFields;    // 排序列表

	CCmsOptionField optionFields;       // 说明需要使用以下哪些查询条件
	string groupUri;					// 设备组Id
    string deviceUri;					// 设备Uri
public:
	CCmsDeviceGroupDeviceQryReq()
	{
		Clear();
		SetEvent(CMS_3AS_DEVICE_GROUP_DEVICE_QRY_REQ);
	}

	~CCmsDeviceGroupDeviceQryReq()
	{
		Clear();
	}

	void Clear()
	{
		startEntryIndex = 0;
		expectEntryNum  = 0;
		sortFields.clear();

		optionFields.Clear();
		groupUri = STRING_NULL;
		deviceUri = STRING_NULL;
	}

	int GetStartEntryIndex() const
	{
		return startEntryIndex;
	}

	void SetStartEntryIndex(int val)
	{
		startEntryIndex = val;
	}

	int GetExpectEntryNum() const
	{
		return expectEntryNum;
	}

	void SetExpectEntryNum(int val)
	{
		expectEntryNum = val;
	}

	const vector<TCmsSortItem> &GetSort() const
	{
		return sortFields;
	}

	void SetSort(const vector<TCmsSortItem> &val)
	{
		sortFields = val;
	}

	bool GetGroupUri(string &val) const
	{
		if (!optionFields.HasOption("groupUri"))
			return false;
		val = groupUri;
		return true;
	}

	void SetGroupUri(const string &val)
	{
		groupUri = val;
		optionFields.SetOption("groupUri");
	}

	bool GetDeviceUri(string &val) const
	{
		if (!optionFields.HasOption("deviceUri"))
			return false;
		val = deviceUri;
		return true;
	}

	void SetDeviceUri(const string &val)
	{
		deviceUri = val;
		optionFields.SetOption("deviceUri");
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsDeviceGroupDeviceQryRsp : public CCmsEventRsp
{
private:
	int totalEntryNum;
	int entryNum;
public:
	CCmsDeviceGroupDeviceQryRsp()
	{
		Clear();
		SetEvent(CMS_3AS_DEVICE_GROUP_DEVICE_QRY_RSP);
	}

	~CCmsDeviceGroupDeviceQryRsp()
	{
		Clear();
	}

	void Clear()
	{
		totalEntryNum = 0;
		entryNum = 0;
	}

	int GetTotalEntryNum() const
	{
		return totalEntryNum;
	}

	void SetTotalEntryNum(int val)
	{
		totalEntryNum = val;
	}

	int GetEntryNum() const
	{
		return entryNum;
	}

	void SetEntryNum(int val)
	{
		entryNum = val;
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsDeviceGroupDeviceQryNtfReq : public CCmsEventReq
{
private:
	int entryNum;
	bool lastNtf;
	vector<CCmsDeviceGroupDeviceInfo> infos;
public:
	CCmsDeviceGroupDeviceQryNtfReq()
	{
		Clear();
		SetEvent(CMS_3AS_DEVICE_GROUP_DEVICE_QRY_NTF_REQ);
	}

	~CCmsDeviceGroupDeviceQryNtfReq()
	{
		Clear();
	}

	void Clear()
	{
		entryNum = 0;
		lastNtf = false;
		infos.clear();
	}

	int GetEntryNum() const
	{
		return entryNum;
	}

	void SetEntryNum(int val)
	{
		entryNum = val;
	}

	bool IsLastNtf() const
	{
		return lastNtf;
	}
	void SetLastNtf(bool val)
	{
		lastNtf = val;
	}

	const vector<CCmsDeviceGroupDeviceInfo> &GetInfo() const
	{
		return infos;
	}

	vector<CCmsDeviceGroupDeviceInfo> &GetInfo()
	{
		return infos;
	}

	void SetInfo(const vector<CCmsDeviceGroupDeviceInfo> &val)
	{
		infos = val;
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsDeviceGroupDeviceQryNtfRsp : public CCmsEventRsp
{
public:
	CCmsDeviceGroupDeviceQryNtfRsp()
	{
		Clear();
		SetEvent(CMS_3AS_DEVICE_GROUP_DEVICE_QRY_NTF_RSP);
	}

	~CCmsDeviceGroupDeviceQryNtfRsp()
	{
		Clear();
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

class  CCmsDomainRelationInfo
{
private:
	string domainId;           // 域编号
	string parentDomainId;     // 父域编号
	string domainName;         // 域名称
	TCmsDomainExtData domainAlias;                  // 域别名,采用了map<key, value>的扩展方式，fanxg-20140321

public:
	CCmsDomainRelationInfo()
	{
		Clear();
	}

	~CCmsDomainRelationInfo()
	{
		Clear();
	}

	void Clear()
	{
		domainId = STRING_NULL;
		parentDomainId = STRING_NULL;
		domainName = STRING_NULL;
		domainAlias.Clear();
	}

	string& GetDomainId() 
	{
		return domainId;
	}
	const string& GetDomainId() const
	{
		return domainId;
	}
	void SetDomainId(const string &val)
	{
		domainId = val;
	}

	string& GetParentDomainId() 
	{
		return parentDomainId;
	}
	const string& GetParentDomainId() const
	{
		return parentDomainId;
	}
	void SetParentDomainId(const string &val)
	{
		parentDomainId = val;
	}

	string& GetDomainName() 
	{
		return domainName;
	}
	const string& GetDomainName() const
	{
		return domainName;
	}
	void SetDomainName(const string &val)
	{
		domainName = val;
	}

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetDomainAlias(const TCmsDomainExtData& cDomainAlias)
    {
        domainAlias = cDomainAlias;
    }
    TCmsDomainExtData& GetDomainAlias()
    {
        return domainAlias;
    }
    const TCmsDomainExtData& GetDomainAlias() const
    {
        return domainAlias;
    }

};

class  CCmsDomainRelationQryReq : public CCmsEventReq
{
private:
	int startEntryIndex;
	int expectEntryNum;

	CCmsOptionField optionField;	// 查询字段标识
	string domainId;				// 域编号
	string parentDomainId;			// 父域编号
	string domainName;				// 域名称
public:
	CCmsDomainRelationQryReq()
	{
		Clear();
		SetEvent(CMS_3AS_DOMAIN_RELATION_QRY_REQ);
	}

	~CCmsDomainRelationQryReq()
	{
		Clear();
	}

	void Clear()
	{
		startEntryIndex = 0;
		expectEntryNum  = 0;
	}

	void SetStartEntryIndex(int val)
	{
		startEntryIndex = val;
	}

	int GetStartEntryIndex() const
	{
		return startEntryIndex;
	}

	void SetExpectEntryNum(int val)
	{
		expectEntryNum = val;
	}

	int GetExpectEntryNum() const
	{
		return expectEntryNum;
	}

	bool GetDomainId(string &val) const
	{
		if (!optionField.HasOption("domainId"))
			return false;
		val = domainId;
		return true;
	}
	void SetDomainId(const string &val)
	{
		domainId = val;
		optionField.SetOption("domainId");
	}
	bool GetParentDomainId(string &val) const
	{
		if (!optionField.HasOption("parentDomainId"))
			return false;
		val = parentDomainId;
		return true;
	}
	void SetParentDomainId(const string &val)
	{
		parentDomainId = val;
		optionField.SetOption("parentDomainId");
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
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsDomainRelationQryRsp : public CCmsEventRsp
{
private:
	int totalEntryNum;			// 查询结果总记录数
	int entryNum;				// 查询通知中会返回的记录总数
public:
	CCmsDomainRelationQryRsp()
	{
		Clear();
		SetEvent(CMS_3AS_DOMAIN_RELATION_QRY_RSP);
	}

	~CCmsDomainRelationQryRsp()
	{
		Clear();
	}

	void Clear()
	{
		totalEntryNum = 0;
		entryNum = 0;
	}

	int GetTotalEntryNum() const
	{
		return totalEntryNum;
	}
	void SetTotalEntryNum(int val)
	{
		totalEntryNum = val;
	}

	int GetEntryNum() const
	{
		return entryNum;
	}

	void SetEntryNum(int val)
	{
		entryNum = val;
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsDomainRelationQryNtfReq : public CCmsEventReq
{
private:
	int  entryNum;
	bool lastNtf;
	vector<CCmsDomainRelationInfo> domainRelationInfo;
public:
	CCmsDomainRelationQryNtfReq()
	{
		Clear();
		SetEvent(CMS_3AS_DOMAIN_RELATION_QRY_NTF_REQ);
	}

	~CCmsDomainRelationQryNtfReq()
	{
		Clear();
	}

	void Clear()
	{
		entryNum = 0;
		lastNtf = false;
	}

	int GetEntryNum() const
	{
		return entryNum;
	}

	void SetEntryNum(int val)
	{
		entryNum = val;
	}

	bool IsLastNtf() const
	{
		return lastNtf;
	}
	void SetLastNtf(bool val)
	{
		lastNtf = val;
	}

	const vector<CCmsDomainRelationInfo> &GetDomainRelationInfo() const
	{
		return domainRelationInfo;
	}

	void SetDomainRelationInfo(const vector<CCmsDomainRelationInfo> &val)
	{
		domainRelationInfo = val;
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsDomainRelationQryNtfRsp : public CCmsEventRsp
{
public:
	CCmsDomainRelationQryNtfRsp()
	{
		Clear();
		SetEvent(CMS_3AS_DOMAIN_RELATION_QRY_NTF_RSP);
	}

	~CCmsDomainRelationQryNtfRsp()
	{
		Clear();
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

class  CCmsDomainRelationAddNtfReq : public CCmsEventReq
{
private:
	CCmsDomainRelationInfo domainRelationInfo;
public:
	CCmsDomainRelationAddNtfReq()
	{
		Clear();
		SetEvent(CMS_3AS_DOMAIN_RELATION_ADD_NTF_REQ);
	}

	~CCmsDomainRelationAddNtfReq()
	{
		Clear();
	}

	void Clear()
	{
		domainRelationInfo.Clear();
	}

	const CCmsDomainRelationInfo &GetDomainRelationInfo() const
	{
		return domainRelationInfo;
	}

	CCmsDomainRelationInfo &GetDomainRelationInfo()
	{
		return domainRelationInfo;
	}

	void SetDomainRelationInfo(const CCmsDomainRelationInfo &val)
	{
		domainRelationInfo = val;
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsDomainRelationAddNtfRsp : public CCmsEventRsp
{
public:
	CCmsDomainRelationAddNtfRsp()
	{
		Clear();
		SetEvent(CMS_3AS_DOMAIN_RELATION_ADD_NTF_RSP);
	}

	~CCmsDomainRelationAddNtfRsp()
	{
		Clear();
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

class  CCmsDomainRelationDelNtfReq : public CCmsEventReq
{
private:
	string domainId;
public:
	CCmsDomainRelationDelNtfReq()
	{
		Clear();
		SetEvent(CMS_3AS_DOMAIN_RELATION_DEL_NTF_REQ);
	}

	~CCmsDomainRelationDelNtfReq()
	{
		Clear();
	}

	void Clear()
	{
		domainId = STRING_NULL;
	}

	const string &GetDomainId() const
	{
		return domainId;
	}

	string &GetDomainId()
	{
		return domainId;
	}

	void SetDomainId(const string &val)
	{
		domainId = val;
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsDomainRelationDelNtfRsp : public CCmsEventRsp
{
public:
	CCmsDomainRelationDelNtfRsp()
	{
		Clear();
		SetEvent(CMS_3AS_DOMAIN_RELATION_DEL_NTF_RSP);
	}

	~CCmsDomainRelationDelNtfRsp()
	{
		Clear();
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

class  CCmsDomainRelationModNtfReq : public CCmsEventReq
{
private:
	CCmsDomainRelationInfo domainRelationInfo;
public:
	CCmsDomainRelationModNtfReq()
	{
		Clear();
		SetEvent(CMS_3AS_DOMAIN_RELATION_MOD_NTF_REQ);
	}

	~CCmsDomainRelationModNtfReq()
	{
		Clear();
	}

	void Clear()
	{
		domainRelationInfo.Clear();
	}

	const CCmsDomainRelationInfo &GetDomainRelationInfo() const
	{
		return domainRelationInfo;
	}

	CCmsDomainRelationInfo &GetDomainRelationInfo()
	{
		return domainRelationInfo;
	}

	void SetDomainRelationInfo(const CCmsDomainRelationInfo &val)
	{
		domainRelationInfo = val;
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsDomainRelationModNtfRsp : public CCmsEventRsp
{
public:
	CCmsDomainRelationModNtfRsp()
	{
		Clear();
		SetEvent(CMS_3AS_DOMAIN_RELATION_MOD_NTF_RSP);
	}

	~CCmsDomainRelationModNtfRsp()
	{
		Clear();
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

class  CCmsTvwallAddReq: public CCmsEventReq
{
public:
	CCmsTvwallAddReq()
	{
		Clear();
		SetEvent(CMS_3AS_TVWALL_ADD_REQ);
		
	}

	~CCmsTvwallAddReq()
	{
		Clear();
	}

	void Clear()
	{
		tvwallInfo.Clear();
	}

private:
	CCmsTvwallInfo tvwallInfo;              

public:
	void SetTvwallInfo(const CCmsTvwallInfo& cTvwallInfo)
	{
		tvwallInfo = cTvwallInfo;
	}
	CCmsTvwallInfo& GetTvwallInfo()
	{
		return tvwallInfo;
	}
	const CCmsTvwallInfo& GetTvwallInfo() const
	{
		return tvwallInfo;
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsTvwallAddRsp: public CCmsEventRsp
{
public:
	CCmsTvwallAddRsp()
	{
		Clear();
		SetEvent(CMS_3AS_TVWALL_ADD_RSP);
	}

	~CCmsTvwallAddRsp()
	{
		Clear();
	}

	void Clear()
	{
		tvwallUri = STRING_NULL;
	}

private:
	string tvwallUri;              

public:
	void SetTvwallUri(const string& strTvwallUri)
	{
		tvwallUri = strTvwallUri;
	}
	string& GetTvwallUri()
	{
		return tvwallUri;
	}
	const string& GetTvwallUri() const
	{
		return tvwallUri;
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsTvwallAddNtfReq: public CCmsEventReq
{
public:
	CCmsTvwallAddNtfReq()
	{
		Clear();
		SetEvent(CMS_3AS_TVWALL_ADD_NTF_REQ);
	}

	~CCmsTvwallAddNtfReq()
	{
		Clear();
	}

	void Clear()
	{
		tvwallInfo.Clear();
	}

private:
	CCmsTvwallInfo tvwallInfo;

public:
	void SetTvwallInfo(const CCmsTvwallInfo& cTvwallInfo)
	{
		tvwallInfo = cTvwallInfo;
	}
	CCmsTvwallInfo& GetTvwallInfo()
	{
		return tvwallInfo;
	}
	const CCmsTvwallInfo& GetTvwallInfo() const
	{
		return tvwallInfo;
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsTvwallAddNtfRsp : public CCmsEventRsp
{
public:
	CCmsTvwallAddNtfRsp()
	{
		Clear();
		SetEvent(CMS_3AS_TVWALL_ADD_NTF_RSP);
	}

	~CCmsTvwallAddNtfRsp()
	{
		Clear();
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

class  CCmsTvwallDelReq: public CCmsEventReq
{
public:
	CCmsTvwallDelReq()
	{
		Clear();
		SetEvent(CMS_3AS_TVWALL_DEL_REQ);
	}

	~CCmsTvwallDelReq()
	{
		Clear();
	}

	void Clear()
	{
		tvwallUri = STRING_NULL;
	}
private:
	string tvwallUri;

public:
	void SetTvwallUri(const string& strTvwallUri)
	{
		tvwallUri = strTvwallUri;
	}
	string& GetTvwallUri()
	{
		return tvwallUri;
	}
	const string& GetTvwallUri() const
	{
		return tvwallUri;
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsTvwallDelRsp : public CCmsEventRsp
{
public:
	CCmsTvwallDelRsp()
	{
		Clear();
		SetEvent(CMS_3AS_TVWALL_DEL_RSP);
	}

	~CCmsTvwallDelRsp()
	{
		Clear();
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

class  CCmsTvwallDelNtfReq: public CCmsEventReq
{
public:
	CCmsTvwallDelNtfReq()
	{
		Clear();
		SetEvent(CMS_3AS_TVWALL_DEL_NTF_REQ);
	}

	~CCmsTvwallDelNtfReq()
	{
		Clear();
	}

	void Clear()
	{
		tvwallUri = STRING_NULL;
	}
private:
	string tvwallUri;

public:
	void SetTvwallUri(const string& strTvwallUri)
	{
		tvwallUri = strTvwallUri;
	}
	string& GetTvwallUri()
	{
		return tvwallUri;
	}
	const string& GetTvwallUri() const
	{
		return tvwallUri;
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsTvwallDelNtfRsp : public CCmsEventRsp
{
public:
	CCmsTvwallDelNtfRsp()
	{
		Clear();
		SetEvent(CMS_3AS_TVWALL_DEL_NTF_RSP);
	}

	~CCmsTvwallDelNtfRsp()
	{
		Clear();
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

class  CCmsTvwallModReq : public CCmsEventReq
{
public:
	CCmsTvwallModReq()
	{
		Clear();
		SetEvent(CMS_3AS_TVWALL_MOD_REQ);
	}

	~CCmsTvwallModReq()
	{
		Clear();
	}

	void Clear()
	{
		uri = STRING_NULL;
		optionField.Clear();
		name = STRING_NULL;
		tvNum = 0;
		style = 0;
		customStyleData = STRING_NULL;
		desc = STRING_NULL;
		tvDecoderBinders.clear();
	}

	const string &GetUri() const
	{
		return uri;
	}

	void SetUri(const string &val)
	{
		uri = val;
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
	bool GetTvDecoderBinders(vector<TCmsTvDecoderBinder> &val) const
	{
		if (!optionField.HasOption("tvDecoderBinders"))
			return false;
		val = tvDecoderBinders;
		return true;
	}
	void SetTvDecoderBinders(const vector<TCmsTvDecoderBinder> &val)
	{
		tvDecoderBinders = val;
		optionField.SetOption("tvDecoderBinders");
	}
private:
	string			uri;							// 电视墙uri
	CCmsOptionField optionField;
	string			name;							// 电视墙名称
	int				tvNum;							// 电视机数目
	int				style;							// 电视墙风格
	string			customStyleData;				// 电视墙自定义风格数据
	string			desc;							// 电视墙描述
	vector<TCmsTvDecoderBinder> tvDecoderBinders;	// 电视与解码器绑定关系
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsTvwallModRsp: public CCmsEventRsp
{
public:
	CCmsTvwallModRsp()
	{
		Clear();
		SetEvent(CMS_3AS_TVWALL_MOD_RSP);
	}

	~CCmsTvwallModRsp()
	{
		Clear();
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

class  CCmsTvwallModNtfReq : public CCmsEventReq
{
public:
	CCmsTvwallModNtfReq()
	{
		Clear();
		SetEvent(CMS_3AS_TVWALL_MOD_NTF_REQ);
	}

	~CCmsTvwallModNtfReq()
	{
		Clear();
	}

	void Clear()
	{
		tvwallInfo.Clear();
	}

private:
	CCmsTvwallInfo tvwallInfo;

public:
	void SetTvwallInfo(const CCmsTvwallInfo& cTvwallInfo)
	{
		tvwallInfo = cTvwallInfo;
	}
	CCmsTvwallInfo& GetTvwallInfo()
	{
		return tvwallInfo;
	}
	const CCmsTvwallInfo& GetTvwallInfo() const
	{
		return tvwallInfo;
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsTvwallModNtfRsp : public CCmsEventRsp
{
public:
	CCmsTvwallModNtfRsp()
	{
		Clear();
		SetEvent(CMS_3AS_TVWALL_MOD_NTF_RSP);
	}

	~CCmsTvwallModNtfRsp()
	{
		Clear();
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

class  CCmsTvwallQryReq : public CCmsEventReq
{
public:
	CCmsTvwallQryReq()
	{
		Clear();
		SetEvent(CMS_3AS_TVWALL_QRY_REQ);
	}

	~CCmsTvwallQryReq()
	{
		Clear();
	}

	void Clear()
	{
		startEntryIndex = 0;
		expectEntryNum  = 0;
		tvwallUri   = STRING_NULL;
		cuserUri    = STRING_NULL;
		cuserGrpUri = STRING_NULL;
	}
private:

	int startEntryIndex;
	int expectEntryNum;

	CCmsOptionField optionFields;
	string tvwallUri;
	string cuserUri;
	string cuserGrpUri;

public:
	void SetStartEntryIndex(int val)
	{
		startEntryIndex = val;
	}

	int GetStartEntryIndex() const
	{
		return startEntryIndex;
	}

	void SetExpectEntryNum(int val)
	{
		expectEntryNum = val;
	}

	int GetExpectEntryNum() const
	{
		return expectEntryNum;
	}

	bool GetTvwallUri(string &val) const
	{
		if (!optionFields.HasOption("tvwallUri"))
			return false;
		val = tvwallUri;
		return true;
	}

	void SetTvwallUri(const string &val)
	{
		optionFields.SetOption("tvwallUri");
		tvwallUri = val;
	}

	bool GetCuserUri(string &val) const
	{
		if (!optionFields.HasOption("cuserUri"))
			return false;
		val = cuserUri;
		return true;
	}

	void SetCuserUri(const string &val)
	{
		optionFields.SetOption("cuserUri");
		cuserUri = val;
	}

	bool GetCuserGrpUri(string &val) const
	{
		if (!optionFields.HasOption("cuserGrpUri"))
			return false;
		val = cuserGrpUri;
		return true;
	}

	void SetCuserGrpUri(const string &val)
	{
		optionFields.SetOption("cuserGrpUri");
		cuserGrpUri = val;
	}

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsTvwallQryRsp : public CCmsEventRsp
{
public:
	CCmsTvwallQryRsp()
	{
		Clear();
		SetEvent(CMS_3AS_TVWALL_QRY_RSP);
	}

	~CCmsTvwallQryRsp()
	{
		Clear();
	}

	void Clear()
	{
		totalEntryNum = 0;
		entryNum = 0;
	}

private:
	int totalEntryNum;			// 查询结果总记录数
	int entryNum;				// 查询通知中会返回的记录总数

public:
	int GetTotalEntryNum() const
	{
		return totalEntryNum;
	}
	void SetTotalEntryNum(int val)
	{
		totalEntryNum = val;
	}

	int GetEntryNum() const
	{
		return entryNum;
	}

	void SetEntryNum(int val)
	{
		entryNum = val;
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsTvwallQryNtfReq: public CCmsEventReq
{
public:
	CCmsTvwallQryNtfReq()
	{
		Clear();
		SetEvent(CMS_3AS_TVWALL_QRY_NTF_REQ);
	}

	~CCmsTvwallQryNtfReq()
	{
		Clear();
	}

	void Clear()
	{
		entryNum = 0;
		lastNtf  = false;
		infos.clear();
	}
private:
	int  entryNum;								//本次通知返回的结果数量
	bool lastNtf;								//标志着是否最后一条通知
	vector<CCmsTvwallInfo> infos;

public:
	int GetEntryNum() const
	{
		return entryNum;
	}

	void SetEntryNum(int nEntryNum)
	{
		entryNum = nEntryNum;
	}

	bool IsLastNtf() const
	{
		return lastNtf;
	}

	void SetLastNtf(bool bLastNtf)
	{
		lastNtf = bLastNtf;
	}

	void SetInfos(const vector<CCmsTvwallInfo>& cInfos)
	{
		infos = cInfos;
	}
	vector<CCmsTvwallInfo>& GetInfos()
	{
		return infos;
	}
	const vector<CCmsTvwallInfo>& GetInfos() const
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

class  CCmsTvwallQryNtfRsp : public CCmsEventRsp
{
public:
	CCmsTvwallQryNtfRsp()
	{
		Clear();
		SetEvent(CMS_3AS_TVWALL_QRY_NTF_RSP);
	}

	~CCmsTvwallQryNtfRsp()
	{
		Clear();
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

class  CCmsCuOnlineReq : public CCmsEventReq
{
public:
	CCmsCuOnlineReq()
	{
		Clear();
		SetEvent(CMS_3AS_CU_ONLINE_REQ);
	}

	~CCmsCuOnlineReq()
	{
		Clear();
	}

	void Clear()
	{
		userName = STRING_NULL;
		password = STRING_NULL;
	}

	const string& GetUserName() const
	{
		return userName;
	}
	string& GetUserName()
	{
		return userName;
	}
	void SetUserName(const string &strUserName)
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
	void SetPassword(const string &strPassword)
	{
		password = strPassword;
	}
private:
	string userName;     // cu用户名
	string password;     // cu密码
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsCuOnlineRsp : public CCmsEventRsp
{
public:
	CCmsCuOnlineRsp()
	{
		Clear();
		SetEvent(CMS_3AS_CU_ONLINE_RSP);
	}

	~CCmsCuOnlineRsp()
	{
		Clear();
	}

	void Clear()
	{
		cuSessionId = STRING_NULL;
		deviceTreeVer = STRING_NULL;
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
private:
	string cuSessionId;   // tas为cu生成的会话号
	string deviceTreeVer; // 设备树版本号
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsCuOfflineReq : public CCmsEventReq
{
public:
	CCmsCuOfflineReq()
	{
		Clear();
		SetEvent(CMS_3AS_CU_OFFLINE_REQ);
	}

	~CCmsCuOfflineReq()
	{
		Clear();
	}

	void Clear()
	{
		cuSessionId = STRING_NULL;
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
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsCuOfflineRsp : public CCmsEventRsp
{
public:
	CCmsCuOfflineRsp()
	{
		Clear();
		SetEvent(CMS_3AS_CU_OFFLINE_RSP); 
	}

	~CCmsCuOfflineRsp()
	{
		Clear();
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

class  CCmsUserQryReq : public CCmsEventReq
{
public:
    CCmsUserQryReq()
    {
        SetEvent(CMS_3AS_CUSER_QRY_REQ);
    }
private:
    CCmsUserInfoQueryCondition condition;
    int  startEntryIndex;                
    int  expectEntryNum;                 
    vector<TCmsSortItem> orderFields;    
public:
    void SetCondition(const CCmsUserInfoQueryCondition& cCondition)
    {
        condition = cCondition;
    }
    CCmsUserInfoQueryCondition& GetCondition()
    {
        return condition;
    }
    const CCmsUserInfoQueryCondition& GetCondition() const
    {
        return condition;
    }

    void SetStartEntryIndex(int nStartEntryIndex)
    {
        startEntryIndex = nStartEntryIndex;
    }
    int GetStartEntryIndex() const
    {
        return startEntryIndex;
    }

    void SetExpectEntryNum(int nExpectEntryNum)
    {
        expectEntryNum = nExpectEntryNum;
    }
    int GetExpectEntryNum() const
    {
        return expectEntryNum;
    }

    void SetOrderFields(const vector<TCmsSortItem>& cOrderFields)
    {
        orderFields = cOrderFields;
    }
    vector<TCmsSortItem>& GetOrderFields()
    {
        return orderFields;
    }
    const vector<TCmsSortItem>& GetOrderFields() const
    {
        return orderFields;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsUserQryRsp : public CCmsEventRsp
{
public:
    CCmsUserQryRsp()
    {
        SetEvent(CMS_3AS_CUSER_QRY_RSP);
    }
private:
    int totalEntryNum;
    int entryNum;     
public:
    void SetTotalEntryNum(int nTotalEntryNum)
    {
        totalEntryNum = nTotalEntryNum;
    }
    int GetTotalEntryNum() const
    {
        return totalEntryNum;
    }

    void SetEntryNum(int nEntryNum)
    {
        entryNum = nEntryNum;
    }
    int GetEntryNum() const
    {
        return entryNum;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsUserQryNtfReq :public CCmsEventReq
{
public:
    CCmsUserQryNtfReq()
    {
        SetEvent(CMS_3AS_CUSER_QRY_NTF_REQ);
    }

    vector<CCmsUserInfo>& GetUserInfo() 
    {
        return infos;
    }
    const vector<CCmsUserInfo>& GetUserInfo() const
    {
        return infos;
    }
    void SetUserInfo(const vector<CCmsUserInfo> &val)
    {
        infos = val;
    }

private:
    int entryNum;
    bool lastNtf;
    vector<CCmsUserInfo> infos;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetEntryNum(int nEntryNum)
    {
        entryNum = nEntryNum;
    }
    int GetEntryNum() const
    {
        return entryNum;
    }

    bool IsLastNtf() const
    {
        return lastNtf;
    }

    void SetLastNtf(bool bLastNtf)
    {
        lastNtf = bLastNtf;
    }
};

class  CCmsUserQryNtfRsp : public CCmsEventRsp
{
public:
    CCmsUserQryNtfRsp()
    {
        SetEvent(CMS_3AS_CUSER_QRY_NTF_RSP);
    }
    ~CCmsUserQryNtfRsp()
    {
        Clear();
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

class  CCmsUserAddNtfReq: public CCmsEventReq
{
public:
    CCmsUserAddNtfReq()
    {
        SetEvent(CMS_3AS_CUSER_ADD_NTF_REQ);
    }
    void GetUserInfo(CCmsUserInfo &val) const
    {
        val = info;
    }
    CCmsUserInfo& GetUserInfo()
    {
        return info;
    }
    const CCmsUserInfo& GetUserInfo() const
    {
        return info;
    }
    void SetUserInfo(const CCmsUserInfo &val)
    {
        info = val;
    }
private:
    CCmsUserInfo info;

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsUserAddNtfRsp : public CCmsEventRsp
{
public:
    CCmsUserAddNtfRsp()
    {
        SetEvent(CMS_3AS_CUSER_ADD_NTF_RSP);
    }
    ~CCmsUserAddNtfRsp()
    {
        Clear();
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

class  CCmsUserDelNtfReq: public CCmsEventReq
{
public:
    CCmsUserDelNtfReq()
    {
        SetEvent(CMS_3AS_CUSER_DEL_NTF_REQ);
    }

    string& GetUserUri() 
    {
        return userUri;
    }
    const string& GetUserUri() const
    {
        return userUri;
    }
    void SetUserUri(const string &val)
    {
        userUri = val;
    }
private:
    string userUri;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsUserDelNtfRsp : public CCmsEventRsp
{
public:
    CCmsUserDelNtfRsp()
    {
        SetEvent(CMS_3AS_CUSER_DEL_NTF_RSP);
    }
    ~CCmsUserDelNtfRsp()
    {
        Clear();
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

class  CCmsUserModNtfReq: public CCmsEventReq
{
public:
    CCmsUserModNtfReq()
    {
        SetEvent(CMS_3AS_CUSER_MOD_NTF_REQ);
    }

    CCmsUserInfo& GetUserInfo() 
    {
        return info;
    }
    const CCmsUserInfo& GetUserInfo() const
    {
        return info;
    }
    void SetUserInfo(const CCmsUserInfo &val)
    {
        info = val;
    }

private:
    CCmsUserInfo info;

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsUserModNtfRsp : public CCmsEventRsp
{
public:
    CCmsUserModNtfRsp()
    {
        SetEvent(CMS_3AS_CUSER_MOD_NTF_RSP);
    }
    ~CCmsUserModNtfRsp()
    {
        Clear();
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

class  CCmsUserDeviceQryReq: public CCmsEventReq
{
public:
    CCmsUserDeviceQryReq()
    {
        SetEvent(CMS_3AS_CUSER_DEVICE_QRY_REQ);
    }

    CCmsUserDeviceInfoQueryCondition& GetUserDeviceInfoQueryCondition()
    {
        return condition;
    }
    const CCmsUserDeviceInfoQueryCondition& GetUserDeviceInfoQueryCondition() const
    {
        return condition;
    }
    void SetUserDeviceInfoQueryCondition(const CCmsUserDeviceInfoQueryCondition &val)
    {
        condition = val;
    }

private:
    CCmsUserDeviceInfoQueryCondition condition;
    int  startEntryIndex;            
    int  expectEntryNum;             
    vector<TCmsSortItem> orderFields;

public:
    void SetStartEntryIndex(int nStartEntryIndex)
    {
        startEntryIndex = nStartEntryIndex;
    }
    int GetStartEntryIndex() const
    {
        return startEntryIndex;
    }

    void SetExpectEntryNum(int nExpectEntryNum)
    {
        expectEntryNum = nExpectEntryNum;
    }
    int GetExpectEntryNum() const
    {
        return expectEntryNum;
    }

    void SetOrderFields(const vector<TCmsSortItem>& cOrderFields)
    {
        orderFields = cOrderFields;
    }
    vector<TCmsSortItem>& GetOrderFields()
    {
        return orderFields;
    }
    const vector<TCmsSortItem>& GetOrderFields() const
    {
        return orderFields;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsUserDeviceQryRsp: public CCmsEventRsp
{
public:
    CCmsUserDeviceQryRsp()
    {
        SetEvent(CMS_3AS_CUSER_DEVICE_QRY_RSP);
    }
private:
    int totalEntryNum;
    int entryNum;     
public:
public:
    void SetTotalEntryNum(int nTotalEntryNum)
    {
        totalEntryNum = nTotalEntryNum;
    }
    int GetTotalEntryNum() const
    {
        return totalEntryNum;
    }

    void SetEntryNum(int nEntryNum)
    {
        entryNum = nEntryNum;
    }
    int GetEntryNum() const
    {
        return entryNum;
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsUserDeviceQryNtfReq: public CCmsEventReq
{
public:
    CCmsUserDeviceQryNtfReq()
    {
        SetEvent(CMS_3AS_CUSER_DEVICE_QRY_NTF_REQ);
    }

    vector<CCmsUserDeviceInfo>& GetCuserDevInfo() 
    {
        return infos;
    }
    const vector<CCmsUserDeviceInfo>& GetCuserDevInfo() const
    {
        return infos;
    }
    void SetCuserDevInfo(const vector<CCmsUserDeviceInfo> &val)
    {
        infos = val;
    }

    void SetEntryNum(int nEntryNum)
    {
        entryNum = nEntryNum;
    }
    int GetEntryNum() const
    {
        return entryNum;
    }
    bool IsLastNtf() const
    {
        return lastNtf;
    }

    void SetLastNtf(bool bLastNtf)
    {
        lastNtf = bLastNtf;
    }

private:
    vector<CCmsUserDeviceInfo> infos;
    int entryNum;
    bool lastNtf;

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsUserDeviceQryNtfRsp : public CCmsEventRsp
{
public:
    CCmsUserDeviceQryNtfRsp()
    {
        SetEvent(CMS_3AS_CUSER_DEVICE_QRY_NTF_RSP);
    }
    ~CCmsUserDeviceQryNtfRsp()
    {
        Clear();
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

//删除G300设备接口
class  CCmsGatewayDelReq : public CCmsEventReq
{
public:
    CCmsGatewayDelReq()
    {
        SetEvent(CMS_3AS_GATEWAY_DEL_REQ);
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
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsGatewayDelRsp : public CCmsEventRsp
{
public:
    CCmsGatewayDelRsp()
    {
        SetEvent(CMS_3AS_GATEWAY_DEL_RSP);
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
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

//---------------------------------------------------------------------------------
//                                rcs功能相关部分
//---------------------------------------------------------------------------------
class  CCmsRcsSubscribePuStatusReq : public CCmsEventReq
{
public:
	CCmsRcsSubscribePuStatusReq()
	{
		Clear();
		SetEvent(CMS_RCS_SUBSCRIBE_PU_STATUS_REQ);
	}

	~CCmsRcsSubscribePuStatusReq()
	{
		Clear();
	}

	void Clear()
	{
		userUri = STRING_NULL;
		devUri = STRING_NULL;
		ssTypes.clear();
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

	void SetSsTypes(const TCmsSsTypes& cSsTypes)
	{
		ssTypes = cSsTypes;
	}
	TCmsSsTypes& GetSsTypes()
	{
		return ssTypes;
	}
	const TCmsSsTypes& GetSsTypes() const
	{
		return ssTypes;
	}

private:
	string          userUri;                  //订阅者
	string          devUri;                   //被订阅者
	TCmsSsTypes     ssTypes;                  //订阅的状态种类
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
	
class  CCmsRcsSubscribePuStatusRsp : public CCmsEventRsp
{
public:
	CCmsRcsSubscribePuStatusRsp()
	{
		Clear();
		SetEvent(CMS_RCS_SUBSCRIBE_PU_STATUS_RSP);
	}

	~CCmsRcsSubscribePuStatusRsp()
	{
		Clear();
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

class  CCmsRcsUnsubscribePuStatusReq : public CCmsEventReq
{
public:
	CCmsRcsUnsubscribePuStatusReq()
	{
		Clear();
		SetEvent(CMS_RCS_UNSUBSCRIBE_PU_STATUS_REQ);
	}

	~CCmsRcsUnsubscribePuStatusReq()
	{
		Clear();
	}

	void Clear()
	{
		userUri = STRING_NULL;
		devUri = STRING_NULL;
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

private:
	string          userUri;     //订阅者
	string          devUri;      //被订阅者
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsRcsUnsubscribePuStatusRsp : public CCmsEventRsp
{
public:
	CCmsRcsUnsubscribePuStatusRsp()
	{
		Clear();
		SetEvent(CMS_RCS_UNSUBSCRIBE_PU_STATUS_RSP);
	}

	~CCmsRcsUnsubscribePuStatusRsp()
	{
		Clear();
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

class  CCmsRcsCatalogSubscribePuStatusReq : public CCmsEventReq
{
public:
	CCmsRcsCatalogSubscribePuStatusReq()
	{
		Clear();
		SetEvent(CMS_RCS_CATALOG_SUBSCRIBE_PU_STATUS_REQ);
	}

	~CCmsRcsCatalogSubscribePuStatusReq()
	{
		Clear();
	}

	void Clear()
	{
		userUri = STRING_NULL;
		domain = STRING_NULL;
		ssTypes.clear();
		batchNotify = false;
	}

private:
	string          userUri;                  //订阅者
	string          domain;                   //需要订阅的域名，用于域名方式的整体订阅
	TCmsSsTypes     ssTypes;                  //订阅的状态种类
	bool            batchNotify;			  //是否接收批量通知
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

	void SetDomain(const string& strDomain)
	{
		domain = strDomain;
	}
	string& GetDomain()
	{
		return domain;
	}
	const string& GetDomain() const
	{
		return domain;
	}

	void SetSsTypes(const TCmsSsTypes& cSsTypes)
	{
		ssTypes = cSsTypes;
	}
	TCmsSsTypes& GetSsTypes()
	{
		return ssTypes;
	}
	const TCmsSsTypes& GetSsTypes() const
	{
		return ssTypes;
	}

	bool GetBatchNotify() const
	{
		return batchNotify;
	}
	void SetBatchNotify(bool bFlag)
	{
		batchNotify = bFlag;
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsRcsCatalogSubscribePuStatusRsp : public CCmsEventRsp
{
public:
	CCmsRcsCatalogSubscribePuStatusRsp()
	{
		Clear();
		SetEvent(CMS_RCS_CATALOG_SUBSCRIBE_PU_STATUS_RSP);
	}

	~CCmsRcsCatalogSubscribePuStatusRsp()
	{
		Clear();
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

class  CCmsRcsCatalogRefreshSsPuStatusReq : public CCmsEventReq
{
public:
	CCmsRcsCatalogRefreshSsPuStatusReq()
	{
		Clear();
		SetEvent(CMS_RCS_CATALOG_REFRESH_SS_PU_STATUS_REQ);
	}

	~CCmsRcsCatalogRefreshSsPuStatusReq()
	{
		Clear();
	}

	void Clear()
	{
		ssOpType = STRING_NULL;
		ssTypes.clear();
	}

private:
	//刷新的是状态类型
	TCmsSsOpType       ssOpType;                      //刷新操作类型：添加或删除，SS_OPE_ADD，SS_OPE_ERASE
	TCmsSsTypes        ssTypes;                       //订阅刷新的状态种类
public:
	void SetSsOpType(const TCmsSsOpType& strSsOpType)
	{
		ssOpType = strSsOpType;
	}
	TCmsSsOpType& GetSsOpType()
	{
		return ssOpType;
	}
	const TCmsSsOpType& GetSsOpType() const
	{
		return ssOpType;
	}

	void SetSsTypes(const TCmsSsTypes& cSsTypes)
	{
		ssTypes = cSsTypes;
	}
	TCmsSsTypes& GetSsTypes()
	{
		return ssTypes;
	}
	const TCmsSsTypes& GetSsTypes() const
	{
		return ssTypes;
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsRcsCatalogRefreshSsPuStatusRsp : public CCmsEventRsp
{
public:
	CCmsRcsCatalogRefreshSsPuStatusRsp()
	{
		Clear();
		SetEvent(CMS_RCS_CATALOG_REFRESH_SS_PU_STATUS_RSP);
	}

	~CCmsRcsCatalogRefreshSsPuStatusRsp()
	{
		Clear();
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

class  CCmsRcsCatalogUnsubscribePuStatusReq : public CCmsEventReq
{
public:
	CCmsRcsCatalogUnsubscribePuStatusReq()
	{
		Clear();
		SetEvent(CMS_RCS_CATALOG_UNSUBSCRIBE_PU_STATUS_REQ);
	}

	~CCmsRcsCatalogUnsubscribePuStatusReq()
	{
		Clear();
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

class  CCmsRcsCatalogUnsubscribePuStatusRsp : public CCmsEventRsp
{
public:
	CCmsRcsCatalogUnsubscribePuStatusRsp()
	{
		Clear();
		SetEvent(CMS_RCS_CATALOG_UNSUBSCRIBE_PU_STATUS_RSP);
	}

	~CCmsRcsCatalogUnsubscribePuStatusRsp()
	{
		Clear();
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

class  CCmsRcsPuRecordStatusNtyReq : public CCmsEventReq
{
public:
	CCmsRcsPuRecordStatusNtyReq()
	{
		Clear();
		SetEvent(CMS_RCS_PU_RECORD_STATUS_NTF_REQ);
	}

	~CCmsRcsPuRecordStatusNtyReq()
	{
		Clear();
	}

	void Clear()
	{
		devURI = STRING_NULL;
		recordStatus.Clear();
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
	const TCmsRcsPuRecordStatus& GetRecordStatus() const
	{
		return recordStatus;
	}
	TCmsRcsPuRecordStatus& GetRecordStatus()
	{
		return recordStatus;
	}
	void SetRecordStatus(const TCmsRcsPuRecordStatus& tStatus)
	{
		recordStatus = tStatus;
	}

private:
	string                 devURI;
	TCmsRcsPuRecordStatus  recordStatus;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsRcsPuRecordStatusNtyRsp : public CCmsEventRsp
{
public:
	CCmsRcsPuRecordStatusNtyRsp()
	{
		Clear();
		SetEvent(CMS_RCS_PU_RECORD_STATUS_NTF_RSP);
	}

	~CCmsRcsPuRecordStatusNtyRsp()
	{
		Clear();
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

class  CCmsRcsGetSysParamReq : public CCmsEventReq
{
public:
	CCmsRcsGetSysParamReq()
	{
		Clear();
		SetEvent(CMS_RCS_GET_SYSPARAM_REQ);
	}

	~CCmsRcsGetSysParamReq()
	{
		Clear();
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

class  CCmsRcsGetSysParamRsp : public CCmsEventRsp
{
public:
	CCmsRcsGetSysParamRsp()
	{
		Clear();
		SetEvent(CMS_RCS_GET_SYSPARAM_RSP);
	}

	~CCmsRcsGetSysParamRsp()
	{
		Clear();
	}

	void Clear()
	{
		coverPolicy = STRING_NULL;
	}

public:
	const TCmsCoverPolicy& GetCoverPolicy() const
	{
		return coverPolicy;
	}
	TCmsCoverPolicy& GetCoverPolicy()
	{
		return coverPolicy;
	}
	void SetCoverPolicy(const TCmsCoverPolicy& var)
	{
		coverPolicy = var;
	}

private:
	TCmsCoverPolicy coverPolicy;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsRcsSetRecordTaskReq : public CCmsEventReq
{
public:
	CCmsRcsSetRecordTaskReq()
	{
		Clear();
		SetEvent(CMS_RCS_SET_RECORD_TASK_REQ);
	}

	~CCmsRcsSetRecordTaskReq()
	{
		Clear();
	}

	void Clear()
	{
		puChn.Clear();
		recType = STRING_NULL;
		isStart = true;
	}

public:
	//puChn
	const TCmsChannel& GetPuChn() const
	{
		return puChn;
	}
	TCmsChannel& GetPuChn()
	{
		return puChn;
	}
	void SetPuChn(const TCmsChannel& var)
	{
		puChn = var;
	}

	//recType
	const TCmsPlatRecType& GetRecType() const
	{
		return recType;
	}
	TCmsPlatRecType& GetRecType()
	{
		return recType;
	}
	void SetRecType(const TCmsPlatRecType& var)
	{
		recType = var;
	}

	//isStart
	bool GetIsStart() const
	{
		return isStart;
	}
	void SetIsStart(bool var)
	{
		isStart = var;
	}

private:
	TCmsChannel			puChn;
	TCmsPlatRecType		recType;        //录像类型：告警录像、手动录像
	bool				isStart;		//是否开启
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsRcsSetRecordTaskRsp : public CCmsEventRsp
{
public:
	CCmsRcsSetRecordTaskRsp()
	{
		Clear();
		SetEvent(CMS_RCS_SET_RECORD_TASK_RSP);
	}

	~CCmsRcsSetRecordTaskRsp()
	{
		Clear();
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

class  CCmsRcsDeleteRecordReq : public CCmsEventReq
{
public:
	CCmsRcsDeleteRecordReq()
	{
		Clear();
		SetEvent(CMS_RCS_DELETE_RECORD_REQ);
	}

	~CCmsRcsDeleteRecordReq()
	{
		Clear();
	}

	void Clear()
	{
		recToken = STRING_NULL;
	}

	void SetRecToken(const TCmsRecToken& strRecToken)
	{
		recToken = strRecToken;
	}
	TCmsRecToken& GetRecToken()
	{
		return recToken;
	}
	const TCmsRecToken& GetRecToken() const
	{
		return recToken;
	}
    void SetDomain(const string& strDomain)
    {
        domain = strDomain;
        optionFields.SetOption("domain");
    }
    bool GetDomain(string& strDomain) const
    {
        if (!optionFields.HasOption("domain"))
            return false;
        strDomain = domain;
        return true;
    }

private:
	TCmsRecToken recToken;

    CCmsOptionField       optionFields;
    string                domain;     //域名，如果设置了该值且不是本域，则会登录异域rcs查询平台录像，对前端录像查询无意义
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

};

class  CCmsRcsDeleteRecordRsp : public CCmsEventRsp
{
public:
	CCmsRcsDeleteRecordRsp()
	{
		Clear();
		SetEvent(CMS_RCS_DELETE_RECORD_RSP);
	}

	~CCmsRcsDeleteRecordRsp()
	{
		Clear();
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

class  CCmsRcsSetRecordAliasReq : public CCmsEventReq
{
public:
	CCmsRcsSetRecordAliasReq()
	{
		Clear();
		SetEvent(CMS_RCS_SET_RECORD_ALIAS_REQ);
	}

	~CCmsRcsSetRecordAliasReq()
	{
		Clear();
	}

	void Clear()
	{
		recDurationToken = STRING_NULL;
		recAlias = STRING_NULL;
	}

public:
	const TCmsRecToken &GetRecDurationToken() const
	{
		return recDurationToken;
	}
	TCmsRecToken &GetRecDurationToken()
	{
		return recDurationToken;
	}
	void SetRecDurationToken(const TCmsRecToken &tRecToken)
	{
		recDurationToken = tRecToken;
	}

	const string &GetRecAlias() const
	{
		return recAlias;
	}
	string &GetRecAlias()
	{
		return recAlias;
	}
	void SetRecAlias(const string &strRecAlias)
	{
		recAlias = strRecAlias;
	}
    void SetDomain(const string& strDomain)
    {
        domain = strDomain;
        optionFields.SetOption("domain");
    }
    bool GetDomain(string& strDomain) const
    {
        if (!optionFields.HasOption("domain"))
            return false;
        strDomain = domain;
        return true;
    }

private:
	TCmsRecToken recDurationToken;
	string       recAlias;

    CCmsOptionField       optionFields;
    string                domain;      //域名，如果设置了该值且不是本域，则会登录异域rcs查询平台录像，对前端录像查询无意义
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

};

class  CCmsRcsSetRecordAliasRsp : public CCmsEventRsp
{
public:
	CCmsRcsSetRecordAliasRsp()
	{
		Clear();
		SetEvent(CMS_RCS_SET_RECORD_ALIAS_RSP);
	}

	~CCmsRcsSetRecordAliasRsp()
	{
		Clear();
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

class  CCmsRcsRecordLockOprReq : public CCmsEventReq
{
public:
	CCmsRcsRecordLockOprReq()
	{
		Clear();
		SetEvent(CMS_RCS_RECORD_LOCK_OPR_REQ);
	}

	~CCmsRcsRecordLockOprReq()
	{
		Clear();
	}

	void Clear()
	{
		recToken = STRING_NULL;
		oprType = CMS_RCS_REC_LOCK;
		deadLine = 0;
	}

	const TCmsRecToken &GetRecToken() const
	{
		return recToken;
	}
	TCmsRecToken &GetRecToken()
	{
		return recToken;
	}
	void SetRecToken(const TCmsRecToken &tRecToken)
	{
		recToken = tRecToken;
	}

	const TCmsRecLockOprType &GetOprType() const
	{
		return oprType;
	}
	TCmsRecLockOprType &GetOprType()
	{
		return oprType;
	}
	void SetOprType(const TCmsRecLockOprType &tOprType)
	{
		oprType = tOprType;
	}

	int GetDeadLine() const
	{
		return deadLine;
	}
	void SetDeadLine(int nDeadLine)
	{
		deadLine = nDeadLine;
	}

    void SetDomain(const string& strDomain)
    {
        domain = strDomain;
        optionFields.SetOption("domain");
    }
    bool GetDomain(string& strDomain) const
    {
        if (!optionFields.HasOption("domain"))
            return false;
        strDomain = domain;
        return true;
    }

private:
	TCmsRecToken recToken;	
	TCmsRecLockOprType oprType;				// 操作类型（锁定/解锁/获取是否解锁）
	int deadLine;							// 解锁时间（绝对时间，为0表示一直锁定到解锁为止）

    CCmsOptionField       optionFields;/*               */
    string                domain;   /*               */
    //域名，如果设置了该值且不是本域，则会登录异域rcs查询平台录像，对前端录像查询无意义
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

};

class  CCmsRcsRecordLockOprRsp : public CCmsEventRsp
{
public:
	CCmsRcsRecordLockOprRsp()
	{
		Clear();
		SetEvent(CMS_RCS_RECORD_LOCK_OPR_RSP);
	}

	~CCmsRcsRecordLockOprRsp()
	{
		Clear();
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


//查询NRU设备
class  CCmsRcsQueryNruReq : public CCmsEventReq
{
public:
    CCmsRcsQueryNruReq()
    {
        Clear();
        SetEvent(CMS_RCS_QUERY_NRU_REQ);
    }

    ~CCmsRcsQueryNruReq()
    {
        Clear();
    }

    void Clear()
    {
        startEntryIndex = 0;
        expectEntryNum  = 0;
        orderFields.clear();
    }
private:
    int  startEntryIndex;
    int  expectEntryNum; 
    vector<TCmsSortItem> orderFields;
public:
    void SetStartEntryIndex(int val)
    {
        startEntryIndex = val;
    }

    int GetStartEntryIndex() const
    {
        return startEntryIndex;
    }

    void SetExpectEntryNum(int val)
    {
        expectEntryNum = val;
    }

    int GetExpectEntryNum() const
    {
        return expectEntryNum;
    }

    vector<TCmsSortItem>& GetOrderField()
    {
        return orderFields;
    }
    const vector<TCmsSortItem>& GetOrderField() const
    {
        return orderFields;
    }
    void SetOrderField(const vector<TCmsSortItem> &orderFields_)
    {
        orderFields = orderFields_;
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsRcsQueryNruRsp : public CCmsEventRsp
{
public:
    CCmsRcsQueryNruRsp()
    {
        Clear();
        SetEvent(CMS_RCS_QUERY_NRU_RSP);
    }

    void Clear()
    {
        totalEntryNum = 0;
        entryNum = 0;
    }
private:
    int totalEntryNum;			// 查询结果总记录数
    int entryNum;				// 查询通知中会返回的记录总数

public:
    int GetTotalEntryNum() const
    {
        return totalEntryNum;
    }
    void SetTotalEntryNum(int val)
    {
        totalEntryNum = val;
    }

    int GetEntryNum() const
    {
        return entryNum;
    }

    void SetEntryNum(int val)
    {
        entryNum = val;
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsRcsQueryNruNtfReq : public CCmsEventReq
{
public:
    CCmsRcsQueryNruNtfReq()
    {
        Clear();
        SetEvent(CMS_RCS_QUERY_NRU_NTF_REQ);
    }

    void Clear()
    {
        nruInfoList.clear();
    }

public:
    int GetEntryNum() const
    {
        return entryNum;
    }

    void SetEntryNum(int nEntryNum)
    {
        entryNum = nEntryNum;
    }

    bool IsLastNtf() const
    {
        return lastNtf;
    }

    void SetLastNtf(bool bLastNtf)
    {
        lastNtf = bLastNtf;
    }
    //nruInfo
    const vector<TCmsNruInfo>& GetNruInfoList() const
    {
        return nruInfoList;
    }
    vector<TCmsNruInfo>& GetNruInfoList()
    {
        return nruInfoList;
    }
    void SetNruInfoList(const vector<TCmsNruInfo>& var)
    {
        nruInfoList = var;
    }

private:
    int  entryNum;								//本次通知返回的结果数量
    bool lastNtf;								//标志着是否最后一条通知
    vector<TCmsNruInfo> nruInfoList;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsRcsQueryNruNtfRsp : public CCmsEventRsp
{
public:
    CCmsRcsQueryNruNtfRsp()
    {
        SetEvent(CMS_RCS_QUERY_NRU_NTF_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

//查询NRU设备
class  CCmsRcsQueryOnlineNruDetailInfoReq : public CCmsEventReq
{
public:
	CCmsRcsQueryOnlineNruDetailInfoReq()
	{
		Clear();
		SetEvent(CMS_RCS_QUERY_ONLINE_NRU_DETAIL_INFO_REQ);
	}

	~CCmsRcsQueryOnlineNruDetailInfoReq()
	{
		Clear();
	}

	void Clear()
	{
		startEntryIndex = 0;
		expectEntryNum  = 0;
		orderFields.clear();
	}
private:
	int  startEntryIndex;
	int  expectEntryNum; 
	vector<TCmsSortItem> orderFields;
public:
	void SetStartEntryIndex(int val)
	{
		startEntryIndex = val;
	}

	int GetStartEntryIndex() const
	{
		return startEntryIndex;
	}

	void SetExpectEntryNum(int val)
	{
		expectEntryNum = val;
	}

	int GetExpectEntryNum() const
	{
		return expectEntryNum;
	}

	vector<TCmsSortItem>& GetOrderField()
	{
		return orderFields;
	}
	const vector<TCmsSortItem>& GetOrderField() const
	{
		return orderFields;
	}
	void SetOrderField(const vector<TCmsSortItem> &orderFields_)
	{
		orderFields = orderFields_;
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsRcsQueryOnlineNruDetailInfoRsp : public CCmsEventRsp
{
public:
	CCmsRcsQueryOnlineNruDetailInfoRsp()
	{
		Clear();
		SetEvent(CMS_RCS_QUERY_ONLINE_NRU_DETAIL_INFO_RSP);
	}

	~CCmsRcsQueryOnlineNruDetailInfoRsp()
	{
		Clear();
	}

	void Clear()
	{
		totalEntryNum = 0;
		entryNum = 0;
	}
private:
	int totalEntryNum;			// 查询结果总记录数
	int entryNum;				// 查询通知中会返回的记录总数

public:
	int GetTotalEntryNum() const
	{
		return totalEntryNum;
	}
	void SetTotalEntryNum(int val)
	{
		totalEntryNum = val;
	}

	int GetEntryNum() const
	{
		return entryNum;
	}

	void SetEntryNum(int val)
	{
		entryNum = val;
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsRcsQueryOnlineNruDetailInfoNtfReq : public CCmsEventReq
{
public:
	CCmsRcsQueryOnlineNruDetailInfoNtfReq()
	{
		Clear();
		SetEvent(CMS_RCS_QUERY_ONLINE_NRU_DETAIL_INFO_NTF_REQ);
	}

	~CCmsRcsQueryOnlineNruDetailInfoNtfReq()
	{
		Clear();
	}

	void Clear()
	{
		nruDetailInfoList.clear();
	}

public:
	int GetEntryNum() const
	{
		return entryNum;
	}

	void SetEntryNum(int nEntryNum)
	{
		entryNum = nEntryNum;
	}

	bool IsLastNtf() const
	{
		return lastNtf;
	}

	void SetLastNtf(bool bLastNtf)
	{
		lastNtf = bLastNtf;
	}

	const TCmsNruDetailInfoList& GetNruDetailInfoList() const
	{
		return nruDetailInfoList;
	}
	TCmsNruDetailInfoList& GetNruDetailInfoList()
	{
		return nruDetailInfoList;
	}
	void SetNruDetailInfoList(const TCmsNruDetailInfoList& var)
	{
		nruDetailInfoList = var;
	}
private:
	int  entryNum;								//本次通知返回的结果数量
	bool lastNtf;								//标志着是否最后一条通知
	TCmsNruDetailInfoList nruDetailInfoList;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsRcsQueryOnlineNruDetailInfoNtfRsp : public CCmsEventRsp
{
public:
	CCmsRcsQueryOnlineNruDetailInfoNtfRsp()
	{
		SetEvent(CMS_RCS_QUERY_ONLINE_NRU_DETAIL_INFO_NTF_RSP);
	}

	~CCmsRcsQueryOnlineNruDetailInfoNtfRsp()
	{
		Clear();
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

//查询磁盘信息
class  CCmsRcsQueryDiskInfoReq : public CCmsEventReq
{
public:
    CCmsRcsQueryDiskInfoReq()
    {
        Clear();
        SetEvent(CMS_RCS_QUERY_DISK_INFO_REQ);
    }
    void Clear()
    {
        nruID = STRING_NULL;
    }

public:
    //nruID
    const string& GetNruID() const
    {
        return nruID;
    }
    string& GetNruID()
    {
        return nruID;
    }
    void SetNruID(const string& var)
    {
        nruID = var;
    }
    void SetStartEntryIndex(int val)
    {
        startEntryIndex = val;
    }

    int GetStartEntryIndex() const
    {
        return startEntryIndex;
    }

    void SetExpectEntryNum(int val)
    {
        expectEntryNum = val;
    }

    int GetExpectEntryNum() const
    {
        return expectEntryNum;
    }

    vector<TCmsSortItem>& GetOrderField()
    {
        return orderFields;
    }
    const vector<TCmsSortItem>& GetOrderField() const
    {
        return orderFields;
    }
    void SetOrderField(const vector<TCmsSortItem> &orderFields_)
    {
        orderFields = orderFields_;
    }
private:
    string nruID;
    int  startEntryIndex;
    int  expectEntryNum; 
    vector<TCmsSortItem> orderFields;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsRcsQueryDiskInfoRsp : public CCmsEventRsp
{
public:
    CCmsRcsQueryDiskInfoRsp()
    {
        Clear();
        SetEvent(CMS_RCS_QUERY_DISK_INFO_RSP);
    }
    void Clear()
    {
        totalEntryNum = 0;
        entryNum = 0;
    }
private:
    int totalEntryNum;			// 查询结果总记录数
    int entryNum;				// 查询通知中会返回的记录总数

public:
    int GetTotalEntryNum() const
    {
        return totalEntryNum;
    }
    void SetTotalEntryNum(int val)
    {
        totalEntryNum = val;
    }

    int GetEntryNum() const
    {
        return entryNum;
    }

    void SetEntryNum(int val)
    {
        entryNum = val;
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsRcsQueryDiskInfoNtfReq : public CCmsEventReq
{
public:
    CCmsRcsQueryDiskInfoNtfReq()
    {
        Clear();
        SetEvent(CMS_RCS_QUERY_DISK_INFO_NTF_REQ);
    }

    void Clear()
    {
        nruID = STRING_NULL;
        diskInfoList.clear();
    }

public:
    int GetEntryNum() const
    {
        return entryNum;
    }

    void SetEntryNum(int nEntryNum)
    {
        entryNum = nEntryNum;
    }

    bool IsLastNtf() const
    {
        return lastNtf;
    }

    void SetLastNtf(bool bLastNtf)
    {
        lastNtf = bLastNtf;
    }
    //nruID
    const string& GetNruID() const
    {
        return nruID;
    }
    string& GetNruID()
    {
        return nruID;
    }
    void SetNruID(const string& var)
    {
        nruID = var;
    }

    //diskInfo
    const vector<TCmsDiskInfo>& GetDiskInfoList() const
    {
        return diskInfoList;
    }
    vector<TCmsDiskInfo>& GetDiskInfoList()
    {
        return diskInfoList;
    }
    void SetDiskInfoList(const vector<TCmsDiskInfo>& var)
    {
        diskInfoList = var;
    }

private:
    int  entryNum;								//本次通知返回的结果数量
    bool lastNtf;								//标志着是否最后一条通知
    string      nruID;
    vector<TCmsDiskInfo>	diskInfoList;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsRcsQueryDiskInfoNtfRsp : public CCmsEventRsp
{
public:
    CCmsRcsQueryDiskInfoNtfRsp()
    {
        SetEvent(CMS_RCS_QUERY_DISK_INFO_NTF_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsRcsSubscribeDiskInfoReq : public CCmsEventReq
{
public:
	CCmsRcsSubscribeDiskInfoReq()
	{
		Clear();
		SetEvent(CMS_RCS_SUBSCRIBE_DISKINFO_REQ);
	}

	~CCmsRcsSubscribeDiskInfoReq()
	{
		Clear();
	}

	void Clear()
	{
		userUri = STRING_NULL;
		devUri = STRING_NULL;
		ssTypes.clear();
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

	//ssTypes
	const TCmsSsTypes& GetSsTypes() const
	{
		return ssTypes;
	}
	TCmsSsTypes& GetSsTypes()
	{
		return ssTypes;
	}
	void SetSsTypes(const TCmsSsTypes& var)
	{
		ssTypes = var;
	}

private:
	string          userUri;    //订阅者
	string          devUri;     //被订阅者
	TCmsSsTypes     ssTypes;    //订阅的状态种类
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsRcsSubscribeDiskInfoRsp : public CCmsEventRsp
{
public:
	CCmsRcsSubscribeDiskInfoRsp()
	{
		Clear();
		SetEvent(CMS_RCS_SUBSCRIBE_DISKINFO_RSP);
	}

	~CCmsRcsSubscribeDiskInfoRsp()
	{
		Clear();
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

class  CCmsRcsDiskPartAlarmNtfReq : public CCmsEventReq
{
public:
	CCmsRcsDiskPartAlarmNtfReq()
	{
		Clear();
		SetEvent(CMS_RCS_DISK_PART_ALARM_NTF_REQ);
	}

	~CCmsRcsDiskPartAlarmNtfReq()
	{
		Clear();
	}

	void Clear()
	{
		partInfo.Clear();
	}

public:
	const TCmsNruInfo& GetNruInfo() const
	{
		return nruInfo;
	}
	TCmsNruInfo& GetNruInfo()
	{
		return nruInfo;
	}
	void SetNruInfo(const TCmsNruInfo& var)
	{
		nruInfo = var;
	}

	const TCmsPartInfo& GetPartInfo() const
	{
		return partInfo;
	}
	TCmsPartInfo& GetPartInfo()
	{
		return partInfo;
	}
	void SetPartInfo(const TCmsPartInfo& var)
	{
		partInfo = var;
	}

private:
	TCmsNruInfo  nruInfo;
	TCmsPartInfo partInfo;
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsRcsDiskPartAlarmNtfRsp : public CCmsEventRsp
{
public:
	CCmsRcsDiskPartAlarmNtfRsp()
	{
		Clear();
		SetEvent(CMS_RCS_DISK_PART_ALARM_NTF_RSP);
	}

	~CCmsRcsDiskPartAlarmNtfRsp()
	{
		Clear();
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

class  CCmsRcsUnsubscribeDiskInfoReq : public CCmsEventReq
{
public:
	CCmsRcsUnsubscribeDiskInfoReq()
	{
		Clear();
		SetEvent(CMS_RCS_UNSUBSCRIBE_DISKINFO_REQ);
	}

	~CCmsRcsUnsubscribeDiskInfoReq()
	{
		Clear();
	}

	void Clear()
	{
		userUri = STRING_NULL;
		devUri = STRING_NULL;
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

private:
	string          userUri;    //订阅者
	string          devUri;     //被订阅者
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsRcsUnsubscribeDiskInfoRsp : public CCmsEventRsp
{
public:
	CCmsRcsUnsubscribeDiskInfoRsp()
	{
		Clear();
		SetEvent(CMS_RCS_UNSUBSCRIBE_DISKINFO_RSP);
	}

	~CCmsRcsUnsubscribeDiskInfoRsp()
	{
		Clear();
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

//周期录像接口
class  CCmsRcsSetRecordParamReq : public CCmsEventReq
{
public:
    CCmsRcsSetRecordParamReq()
    {
        Clear();
        SetEvent(CMS_RCS_SET_RECORD_PARAM_REQ);
    }

    void Clear()
    {
        recordParam.Clear();
    }

public:
    //recordTask
    const TCmsRecordParam& GetRecordParam() const
    {
        return recordParam;
    }
    TCmsRecordParam& GetRecordParam()
    {
        return recordParam;
    }
    void SetRecordParam(const TCmsRecordParam& var)
    {
        recordParam = var;
    }

private:
    TCmsRecordParam recordParam;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsRcsSetRecordParamRsp : public CCmsEventRsp
{
public:
    CCmsRcsSetRecordParamRsp()
    {
        SetEvent(CMS_RCS_SET_RECORD_PARAM_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsRcsQueryRecordParamReq : public CCmsEventReq
{
public:
    CCmsRcsQueryRecordParamReq()
    {
        Clear();
        SetEvent(CMS_RCS_QUERY_RECORD_PARAM_REQ);
    }

    void Clear()
    {
        fieldValueMap.clear();
    }

public:
    //puChn
    bool GetPuChn(TCmsChannel &var) const
    {
        return GetCmsFieldObjVar(fieldValueMap, "puChn", var);
    }
    void SetPuChn(const TCmsChannel &var)
    {
        SetCmsFieldObjVar(fieldValueMap, "puChn", var);
    }

    //fieldValueMap
    const TCmsFieldValueMap& GetFieldValueMap() const
    {
        return fieldValueMap;
    }
    TCmsFieldValueMap& GetFieldValueMap()
    {
        return fieldValueMap;
    }
    void SetFieldValueMap(const TCmsFieldValueMap& var)
    {
        fieldValueMap = var;
    }
    void SetStartEntryIndex(int val)
    {
        startEntryIndex = val;
    }

    int GetStartEntryIndex() const
    {
        return startEntryIndex;
    }

    void SetExpectEntryNum(int val)
    {
        expectEntryNum = val;
    }

    int GetExpectEntryNum() const
    {
        return expectEntryNum;
    }

    vector<TCmsSortItem>& GetOrderField()
    {
        return orderFields;
    }
    const vector<TCmsSortItem>& GetOrderField() const
    {
        return orderFields;
    }
    void SetOrderField(const vector<TCmsSortItem> &orderFields_)
    {
        orderFields = orderFields_;
    }

private:
    TCmsFieldValueMap    fieldValueMap;
    int  startEntryIndex;
    int  expectEntryNum; 
    vector<TCmsSortItem> orderFields;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsRcsQueryRecordParamRsp : public CCmsEventRsp
{
public:
    CCmsRcsQueryRecordParamRsp()
    {
        Clear();
        SetEvent(CMS_RCS_QUERY_RECORD_PARAM_RSP);
    }
    void Clear()
    {
        totalEntryNum = 0;
        entryNum = 0;
    }
private:
    int totalEntryNum;			// 查询结果总记录数
    int entryNum;				// 查询通知中会返回的记录总数

public:
    int GetTotalEntryNum() const
    {
        return totalEntryNum;
    }
    void SetTotalEntryNum(int val)
    {
        totalEntryNum = val;
    }

    int GetEntryNum() const
    {
        return entryNum;
    }

    void SetEntryNum(int val)
    {
        entryNum = val;
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsRcsQueryRecordParamNtfReq : public CCmsEventReq
{
public:
    CCmsRcsQueryRecordParamNtfReq()
    {
        Clear();
        SetEvent(CMS_RCS_QUERY_RECORD_PARAM_NTF_REQ);
    }

    void Clear()
    {
        recordParamList.clear();
    }

public:
    //recordParamList
    const TCmsRecordParamList& GetRecordParamList() const
    {
        return recordParamList;
    }
    TCmsRecordParamList& GetRecordParamList()
    {
        return recordParamList;
    }
    void SetRecordParamList(const TCmsRecordParamList& var)
    {
        recordParamList = var;
    }

    int GetEntryNum() const
    {
        return entryNum;
    }

    void SetEntryNum(int nEntryNum)
    {
        entryNum = nEntryNum;
    }

    bool IsLastNtf() const
    {
        return lastNtf;
    }

    void SetLastNtf(bool bLastNtf)
    {
        lastNtf = bLastNtf;
    }

private:
    TCmsRecordParamList  recordParamList;
    int  entryNum;								//本次通知返回的结果数量
    bool lastNtf;								//标志着是否最后一条通知
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsRcsQueryRecordParamNtfRsp : public CCmsEventRsp
{
public:
    CCmsRcsQueryRecordParamNtfRsp()
    {
        SetEvent(CMS_RCS_QUERY_RECORD_PARAM_NTF_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsRcsQueryRecordDaysReq : public CCmsEventReq
{
public:
	CCmsRcsQueryRecordDaysReq()
	{
		SetEvent(CMS_RCS_QUERY_RECORD_DAYS_REQ);
	}

	void Clear()
	{
		domain = STRING_NULL;
		puChnList.clear();
		timeRange.Clear();
	}

public:
	//domain
	const string& GetDomain() const
	{
		return domain;
	}
	string& GetDomain()
	{
		return domain;
	}
	void SetDomain(const string& var)
	{
		domain = var;
	}

	//puChnList
	const TCmsChannelList& GetPuChnList() const
	{
		return puChnList;
	}
	TCmsChannelList& GetPuChnList()
	{
		return puChnList;
	}
	void SetPuChnList(const TCmsChannelList& var)
	{
		puChnList = var;
	}

	//timeRange
	const TCmsTimeRange& GetTimeRange() const
	{
		return timeRange;
	}
	TCmsTimeRange& GetTimeRange()
	{
		return timeRange;
	}
	void SetTimeRange(const TCmsTimeRange& var)
	{
		timeRange = var;
	}

private:
	string        domain;				// 不填时亦视为本域
	TCmsChannelList puChnList;
	TCmsTimeRange timeRange;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsRcsQueryRecordDaysRsp : public CCmsEventRsp
{
public:
	CCmsRcsQueryRecordDaysRsp()
	{
		SetEvent(CMS_RCS_QUERY_RECORD_DAYS_RSP);
	}
	void Clear()
	{
		puChnRecDaysList.clear();
	}

public:
	//puChnRecDaysList
	const TCmsPuChnRecDaysList& GetPuChnRecDaysList() const
	{
		return puChnRecDaysList;
	}
	TCmsPuChnRecDaysList& GetPuChnRecDaysList()
	{
		return puChnRecDaysList;
	}
	void SetPuChnRecDaysList(const TCmsPuChnRecDaysList& var)
	{
		puChnRecDaysList = var;
	}

private:
	TCmsPuChnRecDaysList puChnRecDaysList;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

//实时码流上墙
class  CCmsRealMediaOnTvReq : public CCmsEventReq
{
public:
	CCmsRealMediaOnTvReq()
	{
		Clear();
		SetEvent(CMS_TVS_REAL_MEDIA_ON_TV_REQ);
	}

	~CCmsRealMediaOnTvReq()
	{
		Clear();
	}

	void Clear()
	{
		encChn.Clear();
		tvChn.Clear();
		tvOccurType = STRING_NULL;
	}

private:
	TCmsChannel         encChn;                              //编码器通道，码流的源
	TCmsTvChannel       tvChn;                               //电视通道
	TCmsTvOccurType		tvOccurType;
public:
	void SetEncChn(const TCmsChannel& cEncChn)
	{
		encChn = cEncChn;
	}
	TCmsChannel& GetEncChn()
	{
		return encChn;
	}
	const TCmsChannel& GetEncChn() const
	{
		return encChn;
	}

	void SetTvChn(const TCmsTvChannel& cTvChn)
	{
		tvChn = cTvChn;
	}
	TCmsTvChannel& GetTvChn()
	{
		return tvChn;
	}
	const TCmsTvChannel& GetTvChn() const
	{
		return tvChn;
	}

	void SetTvOccurType(const TCmsTvOccurType& strTvOccurType)
	{
		tvOccurType = strTvOccurType;
	}
	TCmsTvOccurType& GetTvOccurType()
	{
		return tvOccurType;
	}
	const TCmsTvOccurType& GetTvOccurType() const
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

class  CCmsRealMediaOnTvRsp : public CCmsEventRsp
{
public:
	CCmsRealMediaOnTvRsp()
	{
		Clear();
		SetEvent(CMS_TVS_REAL_MEDIA_ON_TV_RSP);
	}

	~CCmsRealMediaOnTvRsp()
	{
		Clear();
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

//实时码流下墙
class  CCmsRealMediaOffTvReq : public CCmsEventReq
{
public:
	CCmsRealMediaOffTvReq()
	{
		Clear();
		SetEvent(CMS_TVS_REAL_MEDIA_OFF_TV_REQ);
	}

	~CCmsRealMediaOffTvReq()
	{
		Clear();
	}

	void Clear()
	{
		encChn.Clear();
		tvChn.Clear();
		tvOccurType = STRING_NULL;
	}

private:
	TCmsChannel         encChn;                              //编码器通道，码流的源
	TCmsTvChannel       tvChn;                               //电视通道
	TCmsTvOccurType     tvOccurType;

public:
	void SetEncChn(const TCmsChannel& cEncChn)
	{
		encChn = cEncChn;
	}
	TCmsChannel& GetEncChn()
	{
		return encChn;
	}
	const TCmsChannel& GetEncChn() const
	{
		return encChn;
	}

	void SetTvChn(const TCmsTvChannel& cTvChn)
	{
		tvChn = cTvChn;
	}
	TCmsTvChannel& GetTvChn()
	{
		return tvChn;
	}
	const TCmsTvChannel& GetTvChn() const
	{
		return tvChn;
	}

	void SetTvOccurType(const TCmsTvOccurType& strTvOccurType)
	{
		tvOccurType = strTvOccurType;
	}
	TCmsTvOccurType& GetTvOccurType()
	{
		return tvOccurType;
	}
	const TCmsTvOccurType& GetTvOccurType() const
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

class  CCmsRealMediaOffTvRsp : public CCmsEventRsp
{
public:
	CCmsRealMediaOffTvRsp()
	{
		Clear();
		SetEvent(CMS_TVS_REAL_MEDIA_OFF_TV_RSP);
	}

	~CCmsRealMediaOffTvRsp()
	{
		Clear();
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

class CCmsRecOnTvReq : public CCmsEventReq
{
public:
    CCmsRecOnTvReq()
    {
        Clear();
        SetEvent(CMS_TVS_REC_ON_TV_REQ);
    }

    void Clear()
    {
        recUri.Clear();
        timeRange.Clear();
        tvChn.Clear();
        tvOccurType = STRING_NULL;
    }

private:

    TCmsSdpUri          recUri;                              //放像文件
    TCmsTimeRange       timeRange;                           //放像时间段
    TCmsTvChannel       tvChn;                               //电视通道
    string              tvOccurType;                         //电视操作类型: CMS_TV_OCCUR_TYPE_ALARM, CMS_TV_OCCUR_TYPE_HAND

public:

    void SetRecUri(const TCmsSdpUri& tUri)
    {
        recUri = tUri;
    }
    TCmsSdpUri& GetRecUri()
    {
        return recUri;
    }
    const TCmsSdpUri& GetRecUri() const
    {
        return recUri;
    }

    void SetTimeRange(const TCmsTimeRange& cTimeRange)
    {
        timeRange = cTimeRange;
    }
    TCmsTimeRange& GetTimeRange()
    {
        return timeRange;
    }
    const TCmsTimeRange& GetTimeRange() const
    {
        return timeRange;
    }

    void SetTvChn(const TCmsTvChannel& cTvChn)
    {
        tvChn = cTvChn;
    }
    TCmsTvChannel& GetTvChn()
    {
        return tvChn;
    }
    const TCmsTvChannel& GetTvChn() const
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

class CCmsRecOnTvRsp : public CCmsEventRsp
{
public:
    CCmsRecOnTvRsp()
    {
        Clear();
        SetEvent(CMS_TVS_REC_ON_TV_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CCmsRecOnTvAck : public CCmsEventRsp
{
public:
    CCmsRecOnTvAck()
    {
        Clear();
        SetEvent(CMS_TVS_REC_ON_TV_ACK);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsRecOnTvByeReq: public CCmsEventReq
{
public:
    CCmsRecOnTvByeReq()
    {
        Clear();
        SetEvent(CMS_TVS_REC_ON_TV_BYE_REQ);
    }

    void Clear()
    {
        reason = 0;  //默认是0，对应CMS_SUCCESS，表示正常bye
    }

    void SetReason(int nReason)
    {
        reason = nReason;
    }
    int GetReason() const
    {
        return reason;
    }

private:
    int reason;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsRecOnTvByeRsp: public CCmsEventRsp
{
public:
    CCmsRecOnTvByeRsp()
    {
        Clear();
        SetEvent(CMS_TVS_REC_ON_TV_BYE_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsTvWallSchemeGroupSetReq : public CCmsEventReq
{
public:
	CCmsTvWallSchemeGroupSetReq()
	{
		Clear();
		SetEvent(CMS_TVS_TVWALL_SCHEME_GROUP_SET_REQ);
	}

	~CCmsTvWallSchemeGroupSetReq()
	{
		Clear();
	}

	void Clear()
	{
		schemeGroup.Clear();
	}

private:
	TCmsTvWallSchemeGroup         schemeGroup;                 //预案组
public:
	void SetSchemeGroup(const TCmsTvWallSchemeGroup& cSchemeGroup)
	{
		schemeGroup = cSchemeGroup;
	}
	TCmsTvWallSchemeGroup& GetSchemeGroup()
	{
		return schemeGroup;
	}
	const TCmsTvWallSchemeGroup& GetSchemeGroup() const
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

class  CCmsTvWallSchemeGroupSetRsp : public CCmsEventRsp
{
public:
	CCmsTvWallSchemeGroupSetRsp()
	{
		Clear();
		SetEvent(CMS_TVS_TVWALL_SCHEME_GROUP_SET_RSP);
	}

	~CCmsTvWallSchemeGroupSetRsp()
	{
		Clear();
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

class  CCmsTvWallSchemeGroupGetReq : public CCmsEventReq
{
public:
	CCmsTvWallSchemeGroupGetReq()
	{
		Clear();
		SetEvent(CMS_TVS_TVWALL_SCHEME_GROUP_GET_REQ);
	}

	~CCmsTvWallSchemeGroupGetReq()
	{
		Clear();
	}

	void Clear()
	{
		tvWallUri = STRING_NULL;
	}

private:
	string      tvWallUri;                               //电视墙Uri
public:
	void SetTvWallUri(const string& strTvWallUri)
	{
		tvWallUri = strTvWallUri;
	}
	string& GetTvWallUri()
	{
		return tvWallUri;
	}
	const string& GetTvWallUri() const
	{
		return tvWallUri;
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsTvWallSchemeGroupGetRsp : public CCmsEventRsp
{
public:
	CCmsTvWallSchemeGroupGetRsp()
	{
		Clear();
		SetEvent(CMS_TVS_TVWALL_SCHEME_GROUP_GET_RSP);
	}

	~CCmsTvWallSchemeGroupGetRsp()
	{
		Clear();
	}

	void Clear()
	{
		schemeGroup.Clear();
	}

private:
	TCmsTvWallSchemeGroup         schemeGroup;                 //预案组
public:
	void SetSchemeGroup(const TCmsTvWallSchemeGroup& cSchemeGroup)
	{
		schemeGroup = cSchemeGroup;
	}
	TCmsTvWallSchemeGroup& GetSchemeGroup()
	{
		return schemeGroup;
	}
	const TCmsTvWallSchemeGroup& GetSchemeGroup() const
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

class  CCmsTvPollOperateReq : public CCmsEventReq
{
public:
	CCmsTvPollOperateReq()
	{
		Clear();
		SetEvent(CMS_TVS_TV_POLL_OPERATE_REQ);
	}

	~CCmsTvPollOperateReq()
	{
		Clear();
	}

	void Clear()
	{
		tvChn.Clear();
		oprType = STRING_NULL;
		pollList.clear();
	}

private:
	TCmsTvChannel    tvChn;                                             //电视通道
	TCmsOprType      oprType;
	CCmsTvPollList   pollList;                                          //轮询信息列表: 开启操作时需要该项
public:

	void SetTvChn(const TCmsTvChannel& cTvChn)
	{
		tvChn = cTvChn;
	}
	TCmsTvChannel& GetTvChn()
	{
		return tvChn;
	}
	const TCmsTvChannel& GetTvChn() const
	{
		return tvChn;
	}

	void SetOprType(const TCmsOprType& strOprType)
	{
		oprType = strOprType;
	}
	TCmsOprType& GetOprType()
	{
		return oprType;
	}
	const TCmsOprType& GetOprType() const
	{
		return oprType;
	}

	void SetPollList(const CCmsTvPollList& cPollList)
	{
		pollList = cPollList;
	}
	CCmsTvPollList& GetPollList()
	{
		return pollList;
	}
	const CCmsTvPollList& GetPollList() const
	{
		return pollList;
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsTvPollOperateRsp : public CCmsEventRsp
{
public:
	CCmsTvPollOperateRsp()
	{
		Clear();
		SetEvent(CMS_TVS_TV_POLL_OPERATE_RSP);
	}

	~CCmsTvPollOperateRsp()
	{
		Clear();
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

class  CCmsTvWallStatusSsReq : public CCmsEventReq
{
public:
	CCmsTvWallStatusSsReq()
	{
		Clear();
		SetEvent(CMS_TVS_TVWALL_STATUS_SS_REQ);
	}

	~CCmsTvWallStatusSsReq()
	{
		Clear();
	}

	void Clear()
	{
		userUri = STRING_NULL;
		devUri = STRING_NULL;
		ssTypes.clear();
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

	void SetSsTypes(const TCmsSsTypes& cSsTypes)
	{
		ssTypes = cSsTypes;
	}
	TCmsSsTypes& GetSsTypes()
	{
		return ssTypes;
	}
	const TCmsSsTypes& GetSsTypes() const
	{
		return ssTypes;
	}
private:
	string          userUri;                  //订阅者
	string          devUri;                   //被订阅者
	TCmsSsTypes     ssTypes;                  //订阅的状态种类
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsTvWallStatusSsRsp : public CCmsEventRsp
{
public:
	CCmsTvWallStatusSsRsp()
	{
		Clear();
		SetEvent(CMS_TVS_TVWALL_STATUS_SS_RSP);
	}

	~CCmsTvWallStatusSsRsp()
	{
		Clear();
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

class  CCmsTvWallStatusUnSsReq : public CCmsEventReq
{
public:
	CCmsTvWallStatusUnSsReq()
	{
		Clear();
		SetEvent(CMS_TVS_TVWALL_STATUS_UNSS_REQ);
	}

	~CCmsTvWallStatusUnSsReq()
	{
		Clear();
	}

	void Clear()
	{
		userUri = STRING_NULL;
		devUri = STRING_NULL;
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

private:
	string          userUri;     //订阅者
	string          devUri;      //被订阅者
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsTvWallStatusUnSsRsp : public CCmsEventRsp
{
public:
	CCmsTvWallStatusUnSsRsp()
	{
		Clear();
		SetEvent(CMS_TVS_TVWALL_STATUS_UNSS_RSP);
	}

	~CCmsTvWallStatusUnSsRsp()
	{
		Clear();
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

class  CCmsTvWallStatusNtyReq : public CCmsEventReq
{
public:
	CCmsTvWallStatusNtyReq()
	{
		Clear();
		SetEvent(CMS_TVS_TVWALL_STATUS_NTF_REQ);
	}

	~CCmsTvWallStatusNtyReq()
	{
		Clear();
	}

	void Clear()
	{
		devUri = STRING_NULL;
		state.Clear();
	}

private:
	string              devUri;                  //tvwall: uuid@domain
	TCmsTvWallState     state;                   //tvwall状态
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

	void SetState(const TCmsTvWallState& cState)
	{
		state = cState;
	}
	TCmsTvWallState& GetState()
	{
		return state;
	}
	const TCmsTvWallState& GetState() const
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

class  CCmsTvWallStatusNtyRsp : public CCmsEventRsp
{
public:
	CCmsTvWallStatusNtyRsp()
	{
		Clear();
		SetEvent(CMS_TVS_TVWALL_STATUS_NTF_RSP);
	}

	~CCmsTvWallStatusNtyRsp()
	{
		Clear();
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

class  CCmsTvWallSchemeSaveReq : public CCmsEventReq
{
public:
	CCmsTvWallSchemeSaveReq()
	{
		Clear();
		SetEvent(CMS_TVS_TVWALL_SCHEME_SAVE_REQ);
	}

	~CCmsTvWallSchemeSaveReq()
	{
		Clear();
	}

	void Clear()
	{
		scheme.Clear();
	}

private:
	TCmsTvWallScheme         scheme;                 //预案详细信息
public:
	void SetScheme(const TCmsTvWallScheme& cScheme)
	{
		scheme = cScheme;
	}
	TCmsTvWallScheme& GetScheme()
	{
		return scheme;
	}
	const TCmsTvWallScheme& GetScheme() const
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

class  CCmsTvWallSchemeSaveRsp : public CCmsEventRsp
{
public:
	CCmsTvWallSchemeSaveRsp()
	{
		Clear();
		SetEvent(CMS_TVS_TVWALL_SCHEME_SAVE_RSP);
	}

	~CCmsTvWallSchemeSaveRsp()
	{
		Clear();
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

typedef     vector<string>     CCmsTvWallSchemeNameList;

class  CCmsTvWallSchemeDelReq : public CCmsEventReq
{
public:
	CCmsTvWallSchemeDelReq()
	{
		Clear();
		SetEvent(CMS_TVS_TVWALL_SCHEME_DEL_REQ);
	}

	~CCmsTvWallSchemeDelReq()
	{
		Clear();
	}

	void Clear()
	{
		tvWallUri = STRING_NULL;
		schemeNameList.clear();
	}

private:
	string  tvWallUri;
	CCmsTvWallSchemeNameList schemeNameList;                    //预案列表
public:
	
	void SetTvWallUri(const string& strTvWallUri)
	{
		tvWallUri = strTvWallUri;
	}
	string& GetTvWallUri()
	{
		return tvWallUri;
	}
	const string& GetTvWallUri() const
	{
		return tvWallUri;
	}

	void SetSchemeNameList(const CCmsTvWallSchemeNameList& cSchemeNameList)
	{
		schemeNameList = cSchemeNameList;
	}
	CCmsTvWallSchemeNameList& GetSchemeNameList()
	{
		return schemeNameList;
	}
	const CCmsTvWallSchemeNameList& GetSchemeNameList() const
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

class  CCmsTvWallSchemeDelRsp : public CCmsEventRsp
{
public:
	CCmsTvWallSchemeDelRsp()
	{
		Clear();
		SetEvent(CMS_TVS_TVWALL_SCHEME_DEL_RSP);
	}

	~CCmsTvWallSchemeDelRsp()
	{
		Clear();
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

class  CCmsTvWallSchemeQryReq : public CCmsEventReq
{
public:
	CCmsTvWallSchemeQryReq()
	{
		Clear();
		SetEvent(CMS_TVS_TVWALL_SCHEME_QRY_REQ);
	}

	~CCmsTvWallSchemeQryReq()
	{
		Clear();
	}

	void Clear()
	{
		startEntryIndex = 0;
		expectEntryNum  = 0;
		tvWallUri = STRING_NULL;
	}

private:
	int startEntryIndex;
	int expectEntryNum;

	string  tvWallUri;

public:
	void SetStartEntryIndex(int val)
	{
		startEntryIndex = val;
	}

	int GetStartEntryIndex() const
	{
		return startEntryIndex;
	}

	void SetExpectEntryNum(int val)
	{
		expectEntryNum = val;
	}

	int GetExpectEntryNum() const
	{
		return expectEntryNum;
	}

	void SetTvWallUri(const string& strTvWallUri)
	{
		tvWallUri = strTvWallUri;
	}

	const string &GetTvWallUri() const
	{
		return tvWallUri;
	}

	string &GetTvWallUri()
	{
		return tvWallUri;
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsTvWallSchemeQryRsp : public CCmsEventRsp
{
public:
	CCmsTvWallSchemeQryRsp()
	{
		Clear();
		SetEvent(CMS_TVS_TVWALL_SCHEME_QRY_RSP);
	}

	~CCmsTvWallSchemeQryRsp()
	{
		Clear();
	}

	void Clear()
	{
		totalEntryNum = 0;
		entryNum = 0;
	}

private:
	int totalEntryNum;			// 查询结果总记录数
	int entryNum;				// 查询通知中会返回的记录总数

public:
	int GetTotalEntryNum() const
	{
		return totalEntryNum;
	}
	void SetTotalEntryNum(int val)
	{
		totalEntryNum = val;
	}

	int GetEntryNum() const
	{
		return entryNum;
	}

	void SetEntryNum(int val)
	{
		entryNum = val;
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsTvWallSchemeQryNtfReq : public CCmsEventReq
{
public:
	CCmsTvWallSchemeQryNtfReq()
	{
		Clear();
		SetEvent(CMS_TVS_TVWALL_SCHEME_QRY_NTF_REQ);
	}

	~CCmsTvWallSchemeQryNtfReq()
	{
		Clear();
	}

	void Clear()
	{
		entryNum = 0;
		lastNtf  = false;
		schemeList.clear();
	}

private:
	int  entryNum;								//本次通知返回的结果数量
	bool lastNtf;								//标志着是否最后一条通知
	CCmsTvWallSchemeList  schemeList;			//预案列表
	
public:
	int GetEntryNum() const
	{
		return entryNum;
	}

	void SetEntryNum(int nEntryNum)
	{
		entryNum = nEntryNum;
	}

	bool IsLastNtf() const
	{
		return lastNtf;
	}

	void SetLastNtf(bool bLastNtf)
	{
		lastNtf = bLastNtf;
	}

	void SetSchemeList(const CCmsTvWallSchemeList& cSchemeList)
	{
		schemeList = cSchemeList;
	}
	CCmsTvWallSchemeList& GetSchemeList()
	{
		return schemeList;
	}
	const CCmsTvWallSchemeList& GetSchemeList() const
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

class  CCmsTvWallSchemeQryNtfRsp : public CCmsEventRsp
{
public:
	CCmsTvWallSchemeQryNtfRsp()
	{
		Clear();
		SetEvent(CMS_TVS_TVWALL_SCHEME_QRY_NTF_RSP);
	}

	~CCmsTvWallSchemeQryNtfRsp()
	{
		Clear();
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

class  CCmsTvWallSchemeLoadReq : public CCmsEventReq
{
public:
	CCmsTvWallSchemeLoadReq()
	{
		Clear();
		SetEvent(CMS_TVS_TVWALL_SCHEME_LOAD_REQ);
	}

	~CCmsTvWallSchemeLoadReq()
	{
		Clear();
	}

	void Clear()
	{
		tvWallUri = STRING_NULL;
		schemeName = STRING_NULL;
	}

private:
	string  tvWallUri;
	string  schemeName;                        //预案名称
public:

	void SetTvWallUri(const string& strTvWallUri)
	{
		tvWallUri = strTvWallUri;
	}
	string& GetTvWallUri()
	{
		return tvWallUri;
	}
	const string& GetTvWallUri() const
	{
		return tvWallUri;
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

class  CCmsTvWallSchemeLoadRsp : public CCmsEventRsp
{
public:
	CCmsTvWallSchemeLoadRsp()
	{
		Clear();
		SetEvent(CMS_TVS_TVWALL_SCHEME_LOAD_RSP);
	}

	~CCmsTvWallSchemeLoadRsp()
	{
		Clear();
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

//电视窗口画面风格设置
class  CCmsTvWinStyleSetReq : public CCmsEventReq
{
public:
	CCmsTvWinStyleSetReq()
	{
		Clear();
		SetEvent(CMS_TVS_TV_WIN_STYLE_SET_REQ);
	}

	~CCmsTvWinStyleSetReq()
	{
		Clear();
	}

	void Clear()
	{
		tvWallUri = STRING_NULL;
		tvId = 0;
		style = CMS_TV_WIN_STYLE_1;
	}

private:
	string			tvWallUri;						//电视墙ID
	int				tvId;							//电视机ID
	TCmsTvWinStyle	style;							//画面风格
public:
	void SetTvWallUri(const string& strTvWallUri)
	{
		tvWallUri = strTvWallUri;
	}
	string& GetTvWallUri()
	{
		return tvWallUri;
	}
	const string& GetTvWallUri() const
	{
		return tvWallUri;
	}

	void SetTvId(int nTvId)
	{
		tvId = nTvId;
	}
	int GetTvId() const
	{
		return tvId;
	}

	void SetStyle(TCmsTvWinStyle eStyle)
	{
		style = eStyle;
	}
	TCmsTvWinStyle GetStyle() const
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

class  CCmsTvWinStyleSetRsp : public CCmsEventRsp
{
public:
	CCmsTvWinStyleSetRsp()
	{
		Clear();
		SetEvent(CMS_TVS_TV_WIN_STYLE_SET_RSP);
	}

	~CCmsTvWinStyleSetRsp()
	{
		Clear();
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

class  CCmsTvWinStyleGetReq : public CCmsEventReq
{
public:
	CCmsTvWinStyleGetReq()
	{
		Clear();
		SetEvent(CMS_TVS_TV_WIN_STYLE_GET_REQ);
	}

	~CCmsTvWinStyleGetReq()
	{
		Clear();
	}

	void Clear()
	{
		tvWallUri = STRING_NULL;
		tvId = 0;
	}

private:
	string  tvWallUri;                                 //电视墙ID
	int     tvId;                                     //电视机ID
public:
	void SetTvWallUri(const string& strTvWallUri)
	{
		tvWallUri = strTvWallUri;
	}
	string& GetTvWallUri()
	{
		return tvWallUri;
	}
	const string& GetTvWallUri() const
	{
		return tvWallUri;
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

class  CCmsTvWinStyleGetRsp : public CCmsEventRsp
{
public:
	CCmsTvWinStyleGetRsp()
	{
		Clear();
		SetEvent(CMS_TVS_TV_WIN_STYLE_GET_RSP);
	}

	~CCmsTvWinStyleGetRsp()
	{
		Clear();
	}

	void Clear()
	{
		tvWallUri = STRING_NULL;
		tvId = 0;
		style = CMS_TV_WIN_STYLE_1;
	}

private:
	string  tvWallUri;									 //电视墙ID
	int     tvId;										//电视机ID
	TCmsTvWinStyle style;                                //画面风格
public:
	void SetTvWallUri(const string& strTvWallUri)
	{
		tvWallUri = strTvWallUri;
	}
	string& GetTvWallUri()
	{
		return tvWallUri;
	}
	const string& GetTvWallUri() const
	{
		return tvWallUri;
	}

	void SetTvId(int nTvId)
	{
		tvId = nTvId;
	}
	int GetTvId() const
	{
		return tvId;
	}

	void SetStyle(TCmsTvWinStyle eStyle)
	{
		style = eStyle;
	}
	TCmsTvWinStyle GetStyle() const
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


//电视墙通道锁定、备份
class  CCmsTvChannelLockReq : public CCmsEventReq
{
public:
    CCmsTvChannelLockReq()
    {
        Clear();
        SetEvent(CMS_TV_CHANNEL_LOCK_REQ);
    }

    void Clear()
    {
        tvChn.Clear();
        lockTime.Clear();
        password = STRING_NULL;
    }

private:
    TCmsTvChannel    tvChn;                       //被锁定的电视通道
    TCmsTimeRange    lockTime;                    //被锁定时间
    string        password;                    //锁定密码
public:
    void SetTvChn(const TCmsTvChannel& cTvChn)
    {
        tvChn = cTvChn;
    }
    TCmsTvChannel& GetTvChn()
    {
        return tvChn;
    }
    const TCmsTvChannel& GetTvChn() const
    {
        return tvChn;
    }

    void SetLockTime(const TCmsTimeRange& cLockTime)
    {
        lockTime = cLockTime;
    }
    TCmsTimeRange& GetLockTime()
    {
        return lockTime;
    }
    const TCmsTimeRange& GetLockTime() const
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

class  CCmsTvChannelLockRsp : public CCmsEventRsp
{
public:
    CCmsTvChannelLockRsp()
    {
        SetEvent(CMS_TV_CHANNEL_LOCK_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsTvChannelUnlockReq : public CCmsEventReq
{
public:
    CCmsTvChannelUnlockReq()
    {
        Clear();
        SetEvent(CMS_TV_CHANNEL_UNLOCK_REQ);
    }

    void Clear()
    {
        tvChn.Clear();
        password = STRING_NULL;
    }

private:
    TCmsTvChannel    tvChn;                       //被锁定的电视通道
    string        password;                    //锁定密码
public:
    void SetTvChn(const TCmsTvChannel& cTvChn)
    {
        tvChn = cTvChn;
    }
    TCmsTvChannel& GetTvChn()
    {
        return tvChn;
    }
    const TCmsTvChannel& GetTvChn() const
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

class  CCmsTvChannelUnlockRsp : public CCmsEventRsp
{
public:
    CCmsTvChannelUnlockRsp()
    {
        SetEvent(CMS_TV_CHANNEL_UNLOCK_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsTvChannelWatchPointSaveReq : public CCmsEventReq
{
public:
    CCmsTvChannelWatchPointSaveReq()
    {
        Clear();
        SetEvent(CMS_TV_CHANNEL_WATCH_POINT_SAVE_REQ);
    }

    void Clear()
    {
        tvChn.Clear();
        saveTime.Clear();
    }

private:
    TCmsTvChannel    tvChn;                       //备份电视通道
    TCmsTimeRange    saveTime;                    //备份时间
public:
    void SetTvChn(const TCmsTvChannel& cTvChn)
    {
        tvChn = cTvChn;
    }
    TCmsTvChannel& GetTvChn()
    {
        return tvChn;
    }
    const TCmsTvChannel& GetTvChn() const
    {
        return tvChn;
    }

    void SetSaveTime(const TCmsTimeRange& cSaveTime)
    {
        saveTime = cSaveTime;
    }
    TCmsTimeRange& GetSaveTime()
    {
        return saveTime;
    }
    const TCmsTimeRange& GetSaveTime() const
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

class  CCmsTvChannelWatchPointSaveRsp : public CCmsEventRsp
{
public:
    CCmsTvChannelWatchPointSaveRsp()
    {
        SetEvent(CMS_TV_CHANNEL_WATCH_POINT_SAVE_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsTvChannelWatchPointLoadReq : public CCmsEventReq
{
public:
    CCmsTvChannelWatchPointLoadReq()
    {
        Clear();
        SetEvent(CMS_TV_CHANNEL_WATCH_POINT_LOAD_REQ);
    }

    void Clear()
    {
        tvChn.Clear();
    }

private:
    TCmsTvChannel    tvChn;                       //加载电视通道
public:
    void SetTvChn(const TCmsTvChannel& cTvChn)
    {
        tvChn = cTvChn;
    }
    TCmsTvChannel& GetTvChn()
    {
        return tvChn;
    }
    const TCmsTvChannel& GetTvChn() const
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

class  CCmsTvChannelWatchPointLoadRsp : public CCmsEventRsp
{
public:
    CCmsTvChannelWatchPointLoadRsp()
    {
        SetEvent(CMS_TV_CHANNEL_WATCH_POINT_LOAD_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsTvChannelWatchInfoGetReq : public CCmsEventReq
{
public:
    CCmsTvChannelWatchInfoGetReq()
    {
        Clear();
        SetEvent(CMS_TV_CHANNEL_WATCH_INFO_GET_REQ);
    }

    void Clear()
    {
        tvChn.Clear();
    }

private:
    TCmsTvChannel    tvChn;                       //电视通道    
public:
    void SetTvChn(const TCmsTvChannel& cTvChn)
    {
        tvChn = cTvChn;
    }
    TCmsTvChannel& GetTvChn()
    {
        return tvChn;
    }
    const TCmsTvChannel& GetTvChn() const
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

class  CCmsTvChannelWatchInfoGetRsp : public CCmsEventRsp
{
public:
    CCmsTvChannelWatchInfoGetRsp()
    {
        Clear();
        SetEvent(CMS_TV_CHANNEL_WATCH_INFO_GET_RSP);
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
    TCmsTvChannel    tvChn;                       //电视通道
    TCmsChannel      encChn;                      //编码通道
    TCmsTimeRange    saveTime;                    //备份时间
    bool          isLock;                      //是否锁定
    TCmsTimeRange    lockTime;                    //锁定时间
public:
    void SetTvChn(const TCmsTvChannel& cTvChn)
    {
        tvChn = cTvChn;
    }
    TCmsTvChannel& GetTvChn()
    {
        return tvChn;
    }
    const TCmsTvChannel& GetTvChn() const
    {
        return tvChn;
    }

    void SetEncChn(const TCmsChannel& cEncChn)
    {
        encChn = cEncChn;
    }
    TCmsChannel& GetEncChn()
    {
        return encChn;
    }
    const TCmsChannel& GetEncChn() const
    {
        return encChn;
    }

    void SetSaveTime(const TCmsTimeRange& cSaveTime)
    {
        saveTime = cSaveTime;
    }
    TCmsTimeRange& GetSaveTime()
    {
        return saveTime;
    }
    const TCmsTimeRange& GetSaveTime() const
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

    void SetLockTime(const TCmsTimeRange& cLockTime)
    {
        lockTime = cLockTime;
    }
    TCmsTimeRange& GetLockTime()
    {
        return lockTime;
    }
    const TCmsTimeRange& GetLockTime() const
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
class  CCmsTvWallSchemeLockReq : public CCmsEventReq
{
public:
    CCmsTvWallSchemeLockReq()
    {
        Clear();
        SetEvent(CMS_TVWALL_SCHEME_LOCK_REQ);
    }

    void Clear()
    {
        tvWallUri = STRING_NULL;
        lockTime.Clear();
        password = STRING_NULL;
    }

private:
    string        tvWallUri;                    //被锁定的电视墙
    TCmsTimeRange    lockTime;                    //被锁定时间
    string        password;                    //锁定密码
public:
    void SetTvWallUri(const string& strTvWallUri)
    {
        tvWallUri = strTvWallUri;
    }
    string& GetTvWallUri()
    {
        return tvWallUri;
    }
    const string& GetTvWallUri() const
    {
        return tvWallUri;
    }

    void SetLockTime(const TCmsTimeRange& cLockTime)
    {
        lockTime = cLockTime;
    }
    TCmsTimeRange& GetLockTime()
    {
        return lockTime;
    }
    const TCmsTimeRange& GetLockTime() const
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

class  CCmsTvWallSchemeLockRsp : public CCmsEventRsp
{
public:
    CCmsTvWallSchemeLockRsp()
    {
        SetEvent(CMS_TVWALL_SCHEME_LOCK_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsTvWallSchemeUnlockReq : public CCmsEventReq
{
public:
    CCmsTvWallSchemeUnlockReq()
    {
        Clear();
        SetEvent(CMS_TVWALL_SCHEME_UNLOCK_REQ);
    }

    void Clear()
    {
        tvWallUri = STRING_NULL;
        password = STRING_NULL;
    }

private:
    string        tvWallUri;                    //被锁定的电视墙
    string        password;                    //锁定密码
public:
    void SetTvWallUri(const string& strTvWallUri)
    {
        tvWallUri = strTvWallUri;
    }
    string& GetTvWallUri()
    {
        return tvWallUri;
    }
    const string& GetTvWallUri() const
    {
        return tvWallUri;
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

class  CCmsTvWallSchemeUnlockRsp : public CCmsEventRsp
{
public:
    CCmsTvWallSchemeUnlockRsp()
    {
        SetEvent(CMS_TVWALL_SCHEME_UNLOCK_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CCmsRealMediaSwitchOccurTypeReq : public CCmsEventReq
{
public:
    CCmsRealMediaSwitchOccurTypeReq()
    {
        Clear();
        SetEvent(CMS_REAL_MEDIA_SWITCH_OCCUR_TYPE_REQ);
    }

public:
    void Clear()
    {
        CCmsEventReq::Clear();

        tvChn.Clear();
        dstOccurType.clear();
    }

public:
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

    void SetTvChn(const TCmsTvChannel& cTvChn)
    {
        tvChn = cTvChn;
    }
    TCmsTvChannel& GetTvChn()
    {
        return tvChn;
    }
    const TCmsTvChannel& GetTvChn() const
    {
        return tvChn;
    }

private:
    TCmsTvChannel   tvChn;              //电视通道
    TCmsTvOccurType dstOccurType;       //需要切换到的电视操作类型: CMS_TV_OCCUR_TYPE_ALARM, CMS_TV_OCCUR_TYPE_HAND...
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CCmsRealMediaSwitchOccurTypeRsp : public CCmsEventRsp
{
public:
    CCmsRealMediaSwitchOccurTypeRsp()
    {
        SetEvent(CMS_REAL_MEDIA_SWITCH_OCCUR_TYPE_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


//---------------------------------------------------------------------------------
//                                uas功能相关部分
//---------------------------------------------------------------------------------

class  CCmsVideosourcePresetcfgAddReq: public CCmsEventReq
{
public:
	CCmsVideosourcePresetcfgAddReq()
	{
		Clear();
		SetEvent(CMS_UAS_VIDEOSOURCE_PRESETCFG_ADD_REQ);
	}

	~CCmsVideosourcePresetcfgAddReq()
	{
		Clear();
	}

	void Clear()
	{
		info.Clear();
	}

	void SetInfo(const TCmsVideosourcePresetCfgInfo& cInfo)
	{
		info = cInfo;
	}
	TCmsVideosourcePresetCfgInfo& GetInfo()
	{
		return info;
	}
	const TCmsVideosourcePresetCfgInfo& GetInfo() const
	{
		return info;
	}

private:
	TCmsVideosourcePresetCfgInfo info;			// 注意：预置位别名，一次只能添加一个             
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsVideosourcePresetcfgAddRsp: public CCmsEventRsp
{
public:
	CCmsVideosourcePresetcfgAddRsp()
	{
		Clear();
		SetEvent(CMS_UAS_VIDEOSOURCE_PRESETCFG_ADD_RSP);
	}

	~CCmsVideosourcePresetcfgAddRsp()
	{
		Clear();
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

class  CCmsVideosourcePresetcfgAddNtfReq: public CCmsEventReq
{
public:
	CCmsVideosourcePresetcfgAddNtfReq()
	{
		Clear();
		SetEvent(CMS_UAS_VIDEOSOURCE_PRESETCFG_ADD_NTF_REQ);
	}

	~CCmsVideosourcePresetcfgAddNtfReq()
	{
		Clear();
	}

	void Clear()
	{
		info.Clear();
	}

	void SetInfo(const TCmsVideosourcePresetCfgInfo& cInfo)
	{
		info = cInfo;
	}
	TCmsVideosourcePresetCfgInfo& GetInfo()
	{
		return info;
	}
	const TCmsVideosourcePresetCfgInfo& GetInfo() const
	{
		return info;
	}
private:
	TCmsVideosourcePresetCfgInfo info;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsVideosourcePresetcfgAddNtfRsp: public CCmsEventRsp
{
public:
	CCmsVideosourcePresetcfgAddNtfRsp()
	{
		Clear();
		SetEvent(CMS_UAS_VIDEOSOURCE_PRESETCFG_ADD_NTF_RSP);
	}

	~CCmsVideosourcePresetcfgAddNtfRsp()
	{
		Clear();
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

class  CCmsVideosourcePresetcfgDelReq: public CCmsEventReq
{
public:
	CCmsVideosourcePresetcfgDelReq()
	{
		Clear();
		SetEvent(CMS_UAS_VIDEOSOURCE_PRESETCFG_DEL_REQ);
	}

	~CCmsVideosourcePresetcfgDelReq()
	{
		Clear();
	}

	void Clear()
	{
		key.Clear();
	}

	void SetKey(const CCmsVideosourcePresetCfgInfoKey& cKey)
	{
		key = cKey;
	}
	CCmsVideosourcePresetCfgInfoKey& GetKey()
	{
		return key;
	}
	const CCmsVideosourcePresetCfgInfoKey& GetKey() const
	{
		return key;
	}

private:
	CCmsVideosourcePresetCfgInfoKey key;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsVideosourcePresetcfgDelRsp: public CCmsEventRsp
{
public:
	CCmsVideosourcePresetcfgDelRsp()
	{
		Clear();
		SetEvent(CMS_UAS_VIDEOSOURCE_PRESETCFG_DEL_RSP);
	}

	~CCmsVideosourcePresetcfgDelRsp()
	{
		Clear();
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

class  CCmsVideosourcePresetcfgDelNtfReq: public CCmsEventReq
{
public:
	CCmsVideosourcePresetcfgDelNtfReq()
	{
		Clear();
		SetEvent(CMS_UAS_VIDEOSOURCE_PRESETCFG_DEL_NTF_REQ);
	}

	~CCmsVideosourcePresetcfgDelNtfReq()
	{
		Clear();
	}

	void Clear()
	{
		key.Clear();
	}

	void SetKey(const CCmsVideosourcePresetCfgInfoKey& cKey)
	{
		key = cKey;
	}
	CCmsVideosourcePresetCfgInfoKey& GetKey()
	{
		return key;
	}
	const CCmsVideosourcePresetCfgInfoKey& GetKey() const
	{
		return key;
	}
private:
	CCmsVideosourcePresetCfgInfoKey key;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsVideosourcePresetcfgDelNtfRsp: public CCmsEventRsp
{
public:
	CCmsVideosourcePresetcfgDelNtfRsp()
	{
		Clear();
		SetEvent(CMS_UAS_VIDEOSOURCE_PRESETCFG_DEL_NTF_RSP);
	}

	~CCmsVideosourcePresetcfgDelNtfRsp()
	{
		Clear();
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

class  CCmsVideosourcePresetcfgQryReq : public CCmsEventReq
{
public:
	CCmsVideosourcePresetcfgQryReq()
	{
		Clear();
		SetEvent(CMS_UAS_VIDEOSOURCE_PRESETCFG_QRY_REQ);
	}

	~CCmsVideosourcePresetcfgQryReq()
	{
		Clear();
	}

	void Clear()
	{
		optionField.Clear();
		devUri = STRING_NULL;
		videosourceId = 0;
		positionId = 0;
	}

	bool GetDevUri(string &val) const
	{
		if (!optionField.HasOption("devUri"))
			return false;
		val = devUri;
		return true;
	}
	void SetDevUri(const string &val)
	{
		devUri = val;
		optionField.SetOption("devUri");
	}

	bool GetVideosourceId(int &val) const
	{
		if (!optionField.HasOption("videosourceId"))
			return false;
		val = videosourceId;
		return true;
	}
	void SetVideosourceId(int val)
	{
		videosourceId = val;
		optionField.SetOption("videosourceId");
	}

	bool GetPositionId(int &val) const
	{
		if (!optionField.HasOption("positionId"))
			return false;
		val = positionId;
		return true;
	}
	void SetPositionId(int val)
	{
		positionId = val;
		optionField.SetOption("positionId");
	}
private:
	CCmsOptionField optionField;
	string devUri;      
	int videosourceId;
	int positionId;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsVideosourcePresetcfgQryRsp: public CCmsEventRsp
{
public:
	CCmsVideosourcePresetcfgQryRsp()
	{
		Clear();
		SetEvent(CMS_UAS_VIDEOSOURCE_PRESETCFG_QRY_RSP);
	}

	~CCmsVideosourcePresetcfgQryRsp()
	{
		Clear();
	}

	void Clear()
	{
		totalEntryNum = 0;
		entryNum = 0;
	}

	int GetTotalEntryNum() const
	{
		return totalEntryNum;
	}

	void SetTotalEntryNum(int val)
	{
		totalEntryNum = val;
	}

	int GetEntryNum() const
	{
		return entryNum;
	}

	void SetEntryNum(int val)
	{
		entryNum = val;
	}

private:
	int totalEntryNum;
	int entryNum;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsVideosourcePresetcfgQryNtfReq: public CCmsEventReq
{
public:
	CCmsVideosourcePresetcfgQryNtfReq()
	{
		Clear();
		SetEvent(CMS_UAS_VIDEOSOURCE_PRESETCFG_QRY_NTF_REQ);
	}

	~CCmsVideosourcePresetcfgQryNtfReq()
	{
		Clear();
	}

	void Clear()
	{
		entryNum = 0;
		lastNtf  = false;
		infos.clear();
	}

	int GetEntryNum() const
	{
		return entryNum;
	}

	void SetEntryNum(int val)
	{
		entryNum = val;
	}

	bool IsLastNtf() const
	{
		return lastNtf;
	}
	void SetLastNtf(bool val)
	{
		lastNtf = val;
	}

	void SetInfos(const vector<TCmsVideosourcePresetCfgInfo>& cInfos)
	{
		infos = cInfos;
	}
	vector<TCmsVideosourcePresetCfgInfo>& GetInfos()
	{
		return infos;
	}
	const vector<TCmsVideosourcePresetCfgInfo>& GetInfos() const
	{
		return infos;
	}
private:
	int entryNum;
	bool lastNtf;
	vector<TCmsVideosourcePresetCfgInfo> infos;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsVideosourcePresetcfgQryNtfRsp: public CCmsEventRsp
{
public:
	CCmsVideosourcePresetcfgQryNtfRsp()
	{
		Clear();
		SetEvent(CMS_UAS_VIDEOSOURCE_PRESETCFG_QRY_NTF_RSP);
	}

	~CCmsVideosourcePresetcfgQryNtfRsp()
	{
		Clear();
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

//用户操作日志
class  CCmsOperateLogAddReq: public CCmsEventReq
{
public:
    CCmsOperateLogAddReq()
    {
        Clear();
        SetEvent(CMS_UAS_OPERATE_LOG_ADD_REQ);
    }

    void Clear()
    {

    }
private:
    TCmsOperateLogInfo info;              

public:
    void SetInfo(const TCmsOperateLogInfo& cInfo)
    {
        info = cInfo;
    }
    TCmsOperateLogInfo& GetInfo()
    {
        return info;
    }
    const TCmsOperateLogInfo& GetInfo() const
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

class  CCmsOperateLogAddRsp: public CCmsEventRsp
{
public:
    CCmsOperateLogAddRsp()
    {
        Clear();
        SetEvent(CMS_UAS_OPERATE_LOG_ADD_RSP);
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

class  CCmsOperateLogQryReq: public CCmsEventReq
{
public:
    CCmsOperateLogQryReq()
    {
        Clear();
        SetEvent(CMS_UAS_OPERATE_LOG_QRY_REQ);
    }

    void Clear()
    {

    }
private:
    CCmsOperateLogInfoQueryCondition condition;             
    int  startEntryIndex;
    int  expectEntryNum; 
    vector<TCmsSortItem> orderFields;

public:
    void SetCondition(const CCmsOperateLogInfoQueryCondition& cCondition)
    {
        condition = cCondition;
    }
    CCmsOperateLogInfoQueryCondition& GetCondition()
    {
        return condition;
    }
    const CCmsOperateLogInfoQueryCondition& GetCondition() const
    {
        return condition;
    }

    void SetStartEntryIndex(int val)
    {
        startEntryIndex = val;
    }

    int GetStartEntryIndex() const
    {
        return startEntryIndex;
    }

    void SetExpectEntryNum(int val)
    {
        expectEntryNum = val;
    }

    int GetExpectEntryNum() const
    {
        return expectEntryNum;
    }

    vector<TCmsSortItem>& GetOrderField()
    {
        return orderFields;
    }
    const vector<TCmsSortItem>& GetOrderField() const
    {
        return orderFields;
    }
    void SetOrderField(const vector<TCmsSortItem> &orderFields_)
    {
        orderFields = orderFields_;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsOperateLogQryRsp: public CCmsEventRsp
{
public:
    CCmsOperateLogQryRsp()
    {
        Clear();
        SetEvent(CMS_UAS_OPERATE_LOG_QRY_RSP);
    }

    void Clear()
    {
        totalEntryNum = 0;
        entryNum = 0;
    }

    int GetTotalEntryNum() const
    {
        return totalEntryNum;
    }

    void SetTotalEntryNum(int val)
    {
        totalEntryNum = val;
    }

    int GetEntryNum() const
    {
        return entryNum;
    }

    void SetEntryNum(int val)
    {
        entryNum = val;
    }

private:
    int totalEntryNum;
    int entryNum;

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsOperateLogQryNtfReq: public CCmsEventReq
{
public:
    CCmsOperateLogQryNtfReq()
    {
        Clear();
        SetEvent(CMS_UAS_OPERATE_LOG_QRY_NTF_REQ);
    }
private:
    vector<TCmsOperateLogInfo> infos;             
    int entryNum;
    bool lastNtf;

public:
    void SetInfos(const vector<TCmsOperateLogInfo>& cInfos)
    {
        infos = cInfos;
    }
    vector<TCmsOperateLogInfo>& GetInfos()
    {
        return infos;
    }
    const vector<TCmsOperateLogInfo>& GetInfos() const
    {
        return infos;
    }

    void Clear()
    {
        entryNum = 0;
        lastNtf  = false;
        infos.clear();
    }

    int GetEntryNum() const
    {
        return entryNum;
    }

    void SetEntryNum(int val)
    {
        entryNum = val;
    }

    bool IsLastNtf() const
    {
        return lastNtf;
    }
    void SetLastNtf(bool val)
    {
        lastNtf = val;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class  CCmsOperateLogQryNtfRsp: public CCmsEventRsp
{
public:
    CCmsOperateLogQryNtfRsp()
    {
        Clear();
        SetEvent(CMS_UAS_OPERATE_LOG_QRY_NTF_RSP);
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

typedef std::vector<TCmsSecurityModuleVkek> CMS_SECURITY_VKEK_LIST;

//用于设置pu用于加密码流的vkek( 可解密vek )
//以便解码时使用
class CCmsUpdatePuVkeksNtfReq : public CCmsEventReq
{
public:
	CCmsUpdatePuVkeksNtfReq()
	{
		Clear();
		SetEvent(CMS_UPDATE_PU_VKEKS_NTF_REQ);
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
		CCmsEventReq::Clear();
		vkeks.clear();
		tm_from.clear();
		tPuChannel.Clear();
	}

public:
	void SetVkeks( const CMS_SECURITY_VKEK_LIST &vkekList )
	{
		vkeks = vkekList;
	}
	const CMS_SECURITY_VKEK_LIST& GetVkeks() const
	{
		return vkeks;
	}
	CMS_SECURITY_VKEK_LIST& GetVkeks()
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


	void SetPuChannel(const TCmsChannel &tPuChl)
	{
		tPuChannel = tPuChl;
	}
	TCmsChannel& GetPuChannel()
	{
		return tPuChannel;
	}
	const TCmsChannel& GetPuChannel() const
	{
		return tPuChannel;
	}   

private:
	CMS_SECURITY_VKEK_LIST vkeks;
	std::string tm_from;                //该组vkek的启用时间, 注意 结束 永远是未知的
	//std::string tm_to;                //该组vkek的结束时间
	//时间格式参见 CSecurityModuleVkekQueryReq  的开始结束时间
	TCmsChannel tPuChannel;
};
class CCmsUpdatePuVkeksNtfRsp : public CCmsEventRsp
{
public:
	CCmsUpdatePuVkeksNtfRsp()
	{
		Clear();
		SetEvent(CMS_UPDATE_PU_VKEKS_NTF_RSP);
	}

	void Clear()
	{
		CCmsEventRsp::Clear();
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class CCmsPuSignatureCtrlReq: public CCmsEventReq
{
public:
	CCmsPuSignatureCtrlReq()
	{
		Clear();
		SetEvent( CMS_PU_SIGNATURE_CTRL_REQ );
	}

public:
	void Clear()
	{
		CCmsEventReq::Clear();
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

	bool GetEnabled() const
	{
		return enabled;
	}
	void SetEnabled( bool enabled )
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
class CCmsPuSignatureCtrlRsp: public CCmsEventRsp
{
public:
	CCmsPuSignatureCtrlRsp()
	{
		Clear();
		SetEvent( CMS_PU_SIGNATURE_CTRL_RSP );
	}

public:
	void Clear()
	{
		CCmsEventRsp::Clear();
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class CCmsPuEncryptionCtrlReq: public CCmsEventReq
{
public:
	CCmsPuEncryptionCtrlReq()
	{
		SetEvent( CMS_PU_ENCRYPTION_CTRL_REQ );
	}

public:
	void Clear()
	{
		CCmsEventReq::Clear();
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

	bool GetEnabled() const
	{
		return enabled;
	}
	void SetEnabled( bool enabled )
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
class CCmsPuEncryptionCtrlRsp: public CCmsEventRsp
{
public:
	CCmsPuEncryptionCtrlRsp()
	{
		Clear();
		SetEvent( CMS_PU_ENCRYPTION_CTRL_RSP );
	}
public:
	void Clear()
	{
		CCmsEventRsp::Clear();
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

#endif // __CMS_SDK_PROTO_H__








