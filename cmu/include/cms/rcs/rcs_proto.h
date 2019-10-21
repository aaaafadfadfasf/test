/*****************************************************************************
   模块名      : RCS
   文件名      : rcs_proto.h
   相关文件    : 
   文件实现功能: rcs_proto.h 定义所有与RCS打交道的模块所有消息的消息体
   作者        : huangzhichun
   版本        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人				修改内容
   2012/06/11   1.0        huangzhichun			创建
******************************************************************************/

#ifndef __RCS_PROTO_H__
#define __RCS_PROTO_H__

#include "rcs_event.h"
#include "rcs_struct.h"


//class CRcsNruReq : public CEventReq
//{
//public:
//    CRcsNruReq()
//    {
//        Clear();
//    }
//
//    void Clear()
//    {
//        nruID.clear();
//    }
//
//public:
//    //nruID
//    const UUIDString& GetNruID() const
//    {
//        return nruID;
//    }
//    UUIDString& GetNruID()
//    {
//        return nruID;
//    }
//    void SetNruID(const UUIDString& var)
//    {
//        nruID = var;
//    }
//
//public:
//    const std::string ToXml()const;
//    void FromXml(const std::string& xml_string);
//    void ToXml(TiXmlDocument& xmlDoc)const;
//    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
//    void FromXml(const TiXmlNode& xmlNode);
//
//private:
//    UUIDString nruID;
//};

//
//class CRcsRegReq : public CEventReq
//{
//protected:
//    string      devUri;         //设备URI
//    string      devType;
//    CIpList     ipAddrList;
//
//public:
//
//    CRcsRegReq(void) 
//    {
//        event = OspExtEventDesc(RCS_REG_REQ);
//    }
//
//    void Clear()
//    {
//        devUri.clear();
//        devType.clear();
//        ipAddrList.clear();
//    }
//
//public:
//    //DevUri
//	const string& GetDevUri() const
//	{
//		return devUri;
//	}
//	string& GetDevUri()
//	{
//		return devUri;
//	}
//	void SetDevUri(const string& var)
//	{
//		devUri = var;
//	}
//
//    //devType
//    void SetDevType(const string& strDevType)
//    {
//        devType = strDevType;
//    }
//    string& GetDevType()
//    {
//        return devType;
//    }
//    const string& GetDevType() const
//    {
//        return devType;
//    }
//
//    //ipAddrList
//	const CIpList& GetIpAddrList() const
//	{
//		return ipAddrList;
//	}
//	CIpList& GetIpAddrList()
//	{
//		return ipAddrList;
//	}
//	void SetIpAddrList(const CIpList& var)
//	{
//		ipAddrList = var;
//	}
//
//public:
//    const std::string ToXml()const;
//    void FromXml(const std::string& xml_string);
//    void ToXml(TiXmlDocument& xmlDoc)const;
//    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
//    void FromXml(const TiXmlNode& xmlNode);
//};
//
//class CRcsRegRsp : public CEventRsp
//{
//public:
//    CRcsRegRsp()
//    {
//        event = OspExtEventDesc(RCS_REG_RSP);
//    }
//public:
//    const std::string ToXml()const;
//    void FromXml(const std::string& xml_string);
//    void ToXml(TiXmlDocument& xmlDoc)const;
//    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
//    void FromXml(const TiXmlNode& xmlNode);
//};


class CRcsSetSysParamReq : public CEventReq
{
public:
    CRcsSetSysParamReq()
    {
        event = OspExtEventDesc(RCS_SET_SYSPARAM_REQ);
    }

    void Clear()
    {
        coverPolicy.clear();
    }

public:
    const TCoverPolicy& GetCoverPolicy() const
    {
        return coverPolicy;
    }
    TCoverPolicy& GetCoverPolicy()
    {
        return coverPolicy;
    }
    void SetCoverPolicy(const TCoverPolicy& var)
    {
        coverPolicy = var;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
private:
    TCoverPolicy           coverPolicy;
};

class CRcsSetSysParamRsp : public CEventRsp
{
public:
    CRcsSetSysParamRsp()
    {
        event = OspExtEventDesc(RCS_SET_SYSPARAM_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CRcsGetSysParamReq : public CEventReq
{
public:
    CRcsGetSysParamReq()
    {
        event = OspExtEventDesc(RCS_GET_SYSPARAM_REQ);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CRcsGetSysParamRsp : public CEventRsp
{
public:
    CRcsGetSysParamRsp()
    {
        event = OspExtEventDesc(RCS_GET_SYSPARAM_RSP);
    }

    void Clear()
    {
        coverPolicy.clear();
    }

public:
    const TCoverPolicy& GetCoverPolicy() const
    {
        return coverPolicy;
    }
    TCoverPolicy& GetCoverPolicy()
    {
        return coverPolicy;
    }
    void SetCoverPolicy(const TCoverPolicy& var)
    {
        coverPolicy = var;
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

private:
    TCoverPolicy           coverPolicy;
};


class CRcsBindingPuChnReq : public CEventReq
{
public:
    CRcsBindingPuChnReq()
    {
        event = OspExtEventDesc(RCS_BINDING_PUCHN_REQ);
    }

    void Clear()
    {
        puChnBinding.Clear();
    }

public:
    //puChnBinding
    const TPuChnBinding& GetPuChnBinding() const
    {
        return puChnBinding;
    }
    TPuChnBinding& GetPuChnBinding()
    {
        return puChnBinding;
    }
    void SetPuChnBinding(const TPuChnBinding& var)
    {
        puChnBinding = var;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

private:
    TPuChnBinding puChnBinding;
};

class CRcsBindingPuChnRsp : public CEventRsp
{
public:
    CRcsBindingPuChnRsp()
    {
        event = OspExtEventDesc(RCS_BINDING_PUCHN_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CRcsUnbindingPuChnReq : public CEventReq
{
public:
    CRcsUnbindingPuChnReq()
    {
        event = OspExtEventDesc(RCS_UNBINDING_PUCHN_REQ);
    }

    void Clear()
    {
        puChnBinding.Clear();
    }

public:
    //puChnBinding
    const TPuChnBinding& GetPuChnBinding() const
    {
        return puChnBinding;
    }
    TPuChnBinding& GetPuChnBinding()
    {
        return puChnBinding;
    }
    void SetPuChnBinding(const TPuChnBinding& var)
    {
        puChnBinding = var;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

private:
    TPuChnBinding puChnBinding;
};

class CRcsUnbindingPuChnRsp : public CEventRsp
{
public:
    CRcsUnbindingPuChnRsp()
    {
        event = OspExtEventDesc(RCS_UNBINDING_PUCHN_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CRcsQueryPuChnBindingReq : public CQueryReqBase
{
public:
    CRcsQueryPuChnBindingReq()
    {
        event = OspExtEventDesc(RCS_QUERY_PUCHN_BINDING_REQ);
    }

    void Clear()
    {
        fieldValueMap.clear();
    }

public:
    //nruID
    bool GetNruID(string &var) const
    {
        return GetFieldStringVar(fieldValueMap, "nruID", var);
    }
    void SetNruID(const string &var)
    {
        SetFieldStringVar(fieldValueMap, "nruID", var);
    }

    //diskMountPath
    bool GetDiskMountPath(string &var) const
    {
        return GetFieldStringVar(fieldValueMap, "diskMountPath", var);
    }
    void SetDiskMountPath(const string &var)
    {
        SetFieldStringVar(fieldValueMap, "diskMountPath", var);
    }

    //fieldValueMap
    const TFieldValueMap& GetFieldValueMap() const
    {
        return fieldValueMap;
    }
    TFieldValueMap& GetFieldValueMap()
    {
        return fieldValueMap;
    }
    void SetFieldValueMap(const TFieldValueMap& var)
    {
        fieldValueMap = var;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

private:
    TFieldValueMap    fieldValueMap;
};

class CRcsQueryPuChnBindingRsp : public CQueryRspBase
{
public:
    CRcsQueryPuChnBindingRsp()
    {
        event = OspExtEventDesc(RCS_QUERY_PUCHN_BINDING_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CRcsQueryPuChnBindingNtfReq : public CQueryNtfReqBase
{
public:
    CRcsQueryPuChnBindingNtfReq()
    {
        event = OspExtEventDesc(RCS_QUERY_PUCHN_BINDING_NTF_REQ);
    }

    void Clear()
    {
        puChnList.clear();
    }

public:
    //puChnList
    const TChannelList& GetPuChnList() const
    {
        return puChnList;
    }
    TChannelList& GetPuChnList()
    {
        return puChnList;
    }
    void SetPuChnList(const TChannelList& var)
    {
        puChnList = var;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

private:
    TChannelList puChnList;
};

class CRcsQueryPuChnBindingNtfRsp : public CNtfRsp
{
public:
    CRcsQueryPuChnBindingNtfRsp()
    {
        event = OspExtEventDesc(RCS_QUERY_PUCHN_BINDING_NTF_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CRcsSetRecordParamReq : public CEventReq
{
public:
    CRcsSetRecordParamReq()
    {
        event = OspExtEventDesc(RCS_SET_RECORD_PARAM_REQ);
    }

    void Clear()
    {
        recordParam.Clear();
    }

public:
    //recordTask
    const TRecordParam& GetRecordParam() const
    {
        return recordParam;
    }
    TRecordParam& GetRecordParam()
    {
        return recordParam;
    }
    void SetRecordParam(const TRecordParam& var)
    {
        recordParam = var;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

private:
    TRecordParam recordParam;
};

class CRcsSetRecordParamRsp : public CEventRsp
{
public:
    CRcsSetRecordParamRsp()
    {
        event = OspExtEventDesc(RCS_SET_RECORD_PARAM_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CRcsQueryRecordParamReq : public CQueryReqBase
{
public:
    CRcsQueryRecordParamReq()
    {
        event = OspExtEventDesc(RCS_QUERY_RECORD_PARAM_REQ);
    }

    void Clear()
    {
        fieldValueMap.clear();
    }

public:
    //puChn
    bool GetPuChn(TChannel &var) const
    {
        return GetFieldObjVar(fieldValueMap, "puChn", var);
    }
    void SetPuChn(const TChannel &var)
    {
        SetFieldObjVar(fieldValueMap, "puChn", var);
    }

    //fieldValueMap
    const TFieldValueMap& GetFieldValueMap() const
    {
        return fieldValueMap;
    }
    TFieldValueMap& GetFieldValueMap()
    {
        return fieldValueMap;
    }
    void SetFieldValueMap(const TFieldValueMap& var)
    {
        fieldValueMap = var;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

private:
    TFieldValueMap    fieldValueMap;
};

class CRcsQueryRecordParamRsp : public CQueryRspBase
{
public:
    CRcsQueryRecordParamRsp()
    {
        event = OspExtEventDesc(RCS_QUERY_RECORD_PARAM_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CRcsQueryRecordParamNtfReq : public CQueryNtfReqBase
{
public:
    CRcsQueryRecordParamNtfReq()
    {
        event = OspExtEventDesc(RCS_QUERY_RECORD_PARAM_NTF_REQ);
    }

    void Clear()
    {
        recordParamList.clear();
    }

public:
    //recordParamList
    const TRecordParamList& GetRecordParamList() const
    {
        return recordParamList;
    }
    TRecordParamList& GetRecordParamList()
    {
        return recordParamList;
    }
    void SetRecordParamList(const TRecordParamList& var)
    {
        recordParamList = var;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

private:
    TRecordParamList  recordParamList;
};

class CRcsQueryRecordParamNtfRsp : public CNtfRsp
{
public:
    CRcsQueryRecordParamNtfRsp()
    {
        event = OspExtEventDesc(RCS_QUERY_RECORD_PARAM_NTF_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CRcsSetRecordTaskReq : public CEventReq
{
public:
    CRcsSetRecordTaskReq()
    {
        Clear();
        event = OspExtEventDesc(RCS_SET_RECORD_TASK_REQ);
    }

    void Clear()
    {
		puChn.Clear();
        recType.clear();
        isStart = true;
    }

public:
    //puChn
    const TChannel& GetPuChn() const
    {
        return puChn;
    }
    TChannel& GetPuChn()
    {
        return puChn;
    }
    void SetPuChn(const TChannel& var)
    {
        puChn = var;
    }

    //recType
    const TRecType& GetRecType() const
    {
        return recType;
    }
    TRecType& GetRecType()
    {
        return recType;
    }
    void SetRecType(const TRecType& var)
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


public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

private:
	TChannel        puChn;
	TRecType        recType;        //录像类型：告警录像、手动录像
    bool            isStart;           //是否开启
};

class CRcsSetRecordTaskRsp : public CEventRsp
{
public:
    CRcsSetRecordTaskRsp()
    {
        event = OspExtEventDesc(RCS_SET_RECORD_TASK_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

typedef vector<TChannel> CRcsChannelList;
typedef vector<int> CRcsErrCodeList;

class CRcsLockRecordReq : public CEventReq
{
public:
	CRcsLockRecordReq()
	{
		Clear();
		event = OspExtEventDesc(RCS_LOCK_RECORD_REQ);
	}

	void Clear()
	{
		recLockParamList.clear();
		CEventReq::Clear();
	}

public:
	//recLockParamList
	const TRcsLockRecParamList& GetPuChnList() const
	{
		return recLockParamList;
	}
	TRcsLockRecParamList& GetPuChnList()
	{
		return recLockParamList;
	}
	void SetPuChnList(const TRcsLockRecParamList& var)
	{
		recLockParamList = var;
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);

private:
	TRcsLockRecParamList recLockParamList;
};

class CRcsLockRecordRsp : public CEventRsp
{
public:
	CRcsLockRecordRsp()
	{
		Clear();
		event = OspExtEventDesc(RCS_LOCK_RECORD_RSP);
	}

	void Clear()
	{
		errCodeList.clear();
		CEventRsp::Clear();
	}

public:
	//errCodeList
	const CRcsErrCodeList& GetErrCodeList() const
	{
		return errCodeList;
	}
	CRcsErrCodeList& GetErrCodeList()
	{
		return errCodeList;
	}
	void SetErrCodeList(const CRcsErrCodeList& var)
	{
		errCodeList = var;
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);

private:
	CRcsErrCodeList errCodeList;
};

class CRcsQueryRecordReq : public CQueryReqBase
{
public:
    CRcsQueryRecordReq()
    {
        event = OspExtEventDesc(RCS_QUERY_RECORD_REQ);
		Clear();
    }

    void Clear()
    {
        fieldValueMap.clear();
    }

public:
    //puChn
    bool GetPuChn(TChannel &var) const
    {
        return GetFieldObjVar(fieldValueMap, "puChn", var);
    }
    void SetPuChn(const TChannel &var)
    {
        SetFieldObjVar(fieldValueMap, "puChn", var);
    }

    //recType
    bool GetRecType(TRecType &var) const
    {
        return GetFieldStringVar(fieldValueMap, "recType", var);
    }
    void SetRecType(const TRecType &var)
    {
        SetFieldStringVar(fieldValueMap, "recType", var);
    }

    //timeRange
    bool GetTimeRange(TTimeRange &var) const
    {
        return GetFieldObjVar(fieldValueMap, "timeRange", var);
    }
    void SetTimeRange(const TTimeRange &var)
    {
        SetFieldObjVar(fieldValueMap, "timeRange", var);
    }

	//lockState
	bool GetLockState(TRecLockState &var) const
	{
		return GetFieldStringVar(fieldValueMap, "lockState", var);
	}
	void SetLockState(const TRecLockState &var)
	{
		SetFieldStringVar(fieldValueMap, "lockState", var);
	}

    //fieldValueMap
    const TFieldValueMap& GetFieldValueMap() const
    {
        return fieldValueMap;
    }
    TFieldValueMap& GetFieldValueMap()
    {
        return fieldValueMap;
    }
    void SetFieldValueMap(const TFieldValueMap& var)
    {
        fieldValueMap = var;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

private:
    TFieldValueMap    fieldValueMap;
};

class CRcsQueryRecordRsp : public CQueryRspBase
{
public:
    CRcsQueryRecordRsp()
    {
        event = OspExtEventDesc(RCS_QUERY_RECORD_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CRcsQueryRecordNtfReq : public CQueryNtfReqBase
{
public:
    CRcsQueryRecordNtfReq()
    {
        event = OspExtEventDesc(RCS_QUERY_RECORD_NTF_REQ);
    }

    void Clear()
    {
        recDurationList.clear();
    }

public:
    //recDurationList
    const TRecDurationList& GetRecDurationList() const
    {
        return recDurationList;
    }
    TRecDurationList& GetRecDurationList()
    {
        return recDurationList;
    }
    void SetRecDurationList(const TRecDurationList& var)
    {
        recDurationList = var;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

private:
    TRecDurationList    recDurationList;
};

class CRcsQueryRecordNtfRsp : public CNtfRsp
{
public:
    CRcsQueryRecordNtfRsp()
    {
        event = OspExtEventDesc(RCS_QUERY_RECORD_NTF_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CRcsDeleteRecordReq : public CEventReq
{
public:
    CRcsDeleteRecordReq()
    {
        event = OspExtEventDesc(RCS_DELETE_RECORD_REQ);
    }

private:
    TRecToken recToken;                
public:
    void SetRecToken(const TRecToken& strRecToken)
    {
        recToken = strRecToken;
    }
    TRecToken& GetRecToken()
    {
        return recToken;
    }
    const TRecToken& GetRecToken() const
    {
        return recToken;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CRcsDeleteRecordRsp : public CEventRsp
{
public:
    CRcsDeleteRecordRsp()
    {
        event = OspExtEventDesc(RCS_DELETE_RECORD_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CRcsRecDelReq : public CEventReq
{
public:
	CRcsRecDelReq()
	{
		Clear();
		event = OspExtEventDesc(RCS_REC_DEL_REQ);
	}

	void Clear()
	{
		puChn.Clear();
		timeRange.Clear();
	}

	//puChn
	const TChannel& GetPuChn() const
	{
		return puChn;
	}
	TChannel& GetPuChn()
	{
		return puChn;
	}
	void SetPuChn(const TChannel& var)
	{
		puChn = var;
	}

	//timeRange
	const TTimeRange& GetTimeRange() const
	{
		return timeRange;
	}
	TTimeRange& GetTimeRange()
	{
		return timeRange;
	}
	void SetTimeRange(const TTimeRange& var)
	{
		timeRange = var;
	}

private:
	TChannel   puChn;
	TTimeRange timeRange;
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class CRcsRecDelRsp : public CEventRsp
{
public:
	CRcsRecDelRsp()
	{
		Clear();
		event = OspExtEventDesc(RCS_REC_DEL_RSP);
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

class CRcsQueryRecordDaysReq : public CEventReq
{
public:
    CRcsQueryRecordDaysReq()
    {
        event = OspExtEventDesc(RCS_QUERY_RECORD_DAYS_REQ);
    }

    void Clear()
    {
        puChnList.clear();
        timeRange.Clear();
    }

public:
    //puChnList
    const TChannelList& GetPuChnList() const
    {
        return puChnList;
    }
    TChannelList& GetPuChnList()
    {
        return puChnList;
    }
    void SetPuChnList(const TChannelList& var)
    {
        puChnList = var;
    }

    //timeRange
    const TTimeRange& GetTimeRange() const
    {
        return timeRange;
    }
    TTimeRange& GetTimeRange()
    {
        return timeRange;
    }
    void SetTimeRange(const TTimeRange& var)
    {
        timeRange = var;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

private:
    TChannelList puChnList;
    TTimeRange timeRange;
};

class CRcsQueryRecordDaysRsp : public CEventRsp
{
public:
    CRcsQueryRecordDaysRsp()
    {
        event = OspExtEventDesc(RCS_QUERY_RECORD_DAYS_RSP);
    }
    void Clear()
    {
        puChnRecDaysList.clear();
    }

public:
    //puChnRecDaysList
    const TPuChnRecDaysList& GetPuChnRecDaysList() const
    {
        return puChnRecDaysList;
    }
    TPuChnRecDaysList& GetPuChnRecDaysList()
    {
        return puChnRecDaysList;
    }
    void SetPuChnRecDaysList(const TPuChnRecDaysList& var)
    {
        puChnRecDaysList = var;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

private:
    TPuChnRecDaysList puChnRecDaysList;
};

class CRcsSetRecordAliasReq : public CEventReq
{
public:
	CRcsSetRecordAliasReq()
	{
		Clear();
		event = OspExtEventDesc(RCS_SET_RECORD_ALIAS_REQ);
	}

	void Clear()
	{
		recDurationToken.clear();
		recAlias.clear();
	}

public:
	const TRecToken &GetRecDurationToken() const
	{
		return recDurationToken;
	}
	TRecToken &GetRecDurationToken()
	{
		return recDurationToken;
	}
	void SetRecDurationToken(const TRecToken &tRecToken)
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

private:
	TRecToken recDurationToken;
	string    recAlias;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CRcsSetRecordAliasRsp : public CEventRsp
{
public:
	CRcsSetRecordAliasRsp()
	{
		Clear();
		event = OspExtEventDesc(RCS_SET_RECORD_ALIAS_RSP);
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

class CRcsAddRecTagReq : public CEventReq
{
public:
    CRcsAddRecTagReq()
    {
        event = OspExtEventDesc(RCS_ADD_RECTAG_REQ);
    }

    void Clear()
    {
        recTag.Clear();
    }

public:
    //recTag
    const TRecTag& GetRecTag() const
    {
        return recTag;
    }
    TRecTag& GetRecTag()
    {
        return recTag;
    }
    void SetRecTag(const TRecTag& var)
    {
        recTag = var;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

private:
    TRecTag  recTag;
};

class CRcsAddRecTagRsp : public CEventRsp
{
public:
    CRcsAddRecTagRsp()
    {
        event = OspExtEventDesc(RCS_ADD_RECTAG_RSP);
    }
    void Clear()
    {
        recTagID = 0;
    }

public:
    //recTagID
    int GetRecTagID() const
    {
        return recTagID;
    }
    void SetRecTagID(int var)
    {
        recTagID = var;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

private:
    int  recTagID;
};


class CRcsDelRecTagReq : public CEventReq
{
public:
    CRcsDelRecTagReq()
    {
        event = OspExtEventDesc(RCS_DEL_RECTAG_REQ);
    }

    void Clear()
    {
        recTagID = 0;
    }

public:
    //recTagID
    int GetRecTagID() const
    {
        return recTagID;
    }
    void SetRecTagID(int var)
    {
        recTagID = var;
    }


public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

private:
    int  recTagID;
};

class CRcsDelRecTagRsp : public CEventRsp
{
public:
    CRcsDelRecTagRsp()
    {
        event = OspExtEventDesc(RCS_DEL_RECTAG_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CRcsUpdateRecTagReq : public CEventReq
{
public:
    CRcsUpdateRecTagReq()
    {
        event = OspExtEventDesc(RCS_UPDATE_RECTAG_REQ);
    }

    void Clear()
    {
        recTagID = 0;
        tagDesc.clear();
    }

public:
    //recTagID
    int GetRecTagID() const
    {
        return recTagID;
    }
    void SetRecTagID(int var)
    {
        recTagID = var;
    }

    //tagDesc
    const string& GetTagDesc() const
    {
        return tagDesc;
    }
    string& GetTagDesc()
    {
        return tagDesc;
    }
    void SetTagDesc(const string& var)
    {
        tagDesc = var;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

private:
    int         recTagID;
    string      tagDesc;
};

class CRcsUpdateRecTagRsp : public CEventRsp
{
public:
    CRcsUpdateRecTagRsp()
    {
        event = OspExtEventDesc(RCS_UPDATE_RECTAG_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CRcsQueryRecTagReq : public CQueryReqBase
{
public:
    CRcsQueryRecTagReq()
    {
        event = OspExtEventDesc(RCS_QUERY_RECTAG_REQ);
    }

    void Clear()
    {
        fieldValueMap.clear();
    }

public:
    //puChn
    bool GetPuChn(TChannel &var) const
    {
        return GetFieldObjVar(fieldValueMap, "puChn", var);
    }
    void SetPuChn(const TChannel &var)
    {
        SetFieldObjVar(fieldValueMap, "puChn", var);
    }

    //timeRange
    bool GetTimeRange(string &var) const
    {
        return GetFieldStringVar(fieldValueMap, "timeRange", var);
    }
    void SetTimeRange(const string &var)
    {
        SetFieldStringVar(fieldValueMap, "timeRange", var);
    }

    //tagDesc，模糊匹配
    bool GetTagDesc(string &var) const
    {
        return GetFieldStringVar(fieldValueMap, "tagDesc", var);
    }
    void SetTagDesc(const string &var)
    {
        SetFieldStringVar(fieldValueMap, "tagDesc", var);
    }

    //fieldValueMap
    const TFieldValueMap& GetFieldValueMap() const
    {
        return fieldValueMap;
    }
    TFieldValueMap& GetFieldValueMap()
    {
        return fieldValueMap;
    }
    void SetFieldValueMap(const TFieldValueMap& var)
    {
        fieldValueMap = var;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

private:
    TFieldValueMap    fieldValueMap;
};

class CRcsQueryRecTagRsp : public CQueryRspBase
{
public:
    CRcsQueryRecTagRsp()
    {
        event = OspExtEventDesc(RCS_QUERY_RECTAG_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CRcsQueryRecTagNtfReq : public CQueryNtfReqBase
{
public:
    CRcsQueryRecTagNtfReq()
    {
        event = OspExtEventDesc(RCS_QUERY_RECTAG_NTF_REQ);
    }

    void Clear()
    {
        recTagList.clear();
    }

public:
    //recTagList
    const TRecTagList& GetRecTagList() const
    {
        return recTagList;
    }
    TRecTagList& GetRecTagList()
    {
        return recTagList;
    }
    void SetRecTagList(const TRecTagList& var)
    {
        recTagList = var;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

private:
    TRecTagList recTagList;
};

class CRcsQueryRecTagNtfRsp : public CNtfRsp
{
public:
    CRcsQueryRecTagNtfRsp()
    {
        event = OspExtEventDesc(RCS_QUERY_RECTAG_NTF_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

//PU录像状态订阅
#define  SS_TYPE_PU_PLATFORM_RECORD_STATUS    "PU_PLATFORM_RECORD"

//typedef CSsReq CRcsSubscribePuStatusReq;
//typedef CSsRsp CRcsSubscribePuStatusRsp;
//typedef CUnSsReq CRcsUnsubscribePuStatusReq;
//typedef CUnSsRsp CRcsUnsubscribePuStatusRsp;

class CRcsSubscribePuStatusReq : public CSsReq
{
public:
    CRcsSubscribePuStatusReq()
    {
        event = OspExtEventDesc(RCS_SUBSCRIBE_PU_STATUS_REQ);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CRcsSubscribePuStatusRsp : public CSsRsp
{
public:
    CRcsSubscribePuStatusRsp()
    {
        event = OspExtEventDesc(RCS_SUBSCRIBE_PU_STATUS_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CRcsUnsubscribePuStatusReq : public CUnSsReq
{
public:
    CRcsUnsubscribePuStatusReq()
    {
        event = OspExtEventDesc(RCS_UNSUBSCRIBE_PU_STATUS_REQ);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CRcsUnsubscribePuStatusRsp : public CUnSsRsp
{
public:
    CRcsUnsubscribePuStatusRsp()
    {
        event = OspExtEventDesc(RCS_UNSUBSCRIBE_PU_STATUS_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CRcsPuRecordStatusNtyReq : public CEventReq
{
public:
    CRcsPuRecordStatusNtyReq()
    {
        event = OspExtEventDesc(RCS_PU_RECORD_STATUS_NTF_REQ);
    };

    void Clear()
    {
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
    const TRcsPuRecordStatus& GetRecordStatus() const
    {
        return recordStatus;
    }
    TRcsPuRecordStatus& GetRecordStatus()
    {
        return recordStatus;
    }
    void SetRecordStatus(const TRcsPuRecordStatus& tStatus)
    {
        recordStatus = tStatus;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

private:
    string              devURI;         //HZCTODO: pu uuid @domain，可以删除？
    TRcsPuRecordStatus  recordStatus;   //业务状态
};

typedef CNtfRsp CRcsPuRecordStatusNtyRsp;

class CRcsListSubscribePuStatusReq : public CListSsReq
{
public:
	CRcsListSubscribePuStatusReq()
	{
		event = OspExtEventDesc(RCS_LIST_SUBSCRIBE_PU_STATUS_REQ);
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class CRcsListSubscribePuStatusRsp : public CListSsRsp
{
public:
	CRcsListSubscribePuStatusRsp()
	{
		event = OspExtEventDesc(RCS_LIST_SUBSCRIBE_PU_STATUS_RSP);
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class CRcsListRefreshSsPuStatusReq : public CListRefreshSsReq
{
public:
	CRcsListRefreshSsPuStatusReq()
	{
		event = OspExtEventDesc(RCS_LIST_REFRESH_SS_PU_STATUS_REQ);
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class CRcsListRefreshSsPuStatusRsp : public CListRefreshSsRsp
{
public:
	CRcsListRefreshSsPuStatusRsp()
	{
		event = OspExtEventDesc(RCS_LIST_REFRESH_SS_PU_STATUS_RSP);
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class CRcsListUnsubscribePuStatusReq : public CListUnSsReq
{
public:
	CRcsListUnsubscribePuStatusReq()
	{
		event = OspExtEventDesc(RCS_LIST_UNSUBSCRIBE_PU_STATUS_REQ);
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class CRcsListUnsubscribePuStatusRsp : public CListUnSsRsp
{
public:
	CRcsListUnsubscribePuStatusRsp()
	{
		event = OspExtEventDesc(RCS_LIST_UNSUBSCRIBE_PU_STATUS_RSP);
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class CRcsCatalogSubscribePuStatusReq : public CCatalogSsReq
{
public:
	CRcsCatalogSubscribePuStatusReq()
	{
		event = OspExtEventDesc(RCS_CATALOG_SUBSCRIBE_PU_STATUS_REQ);
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CRcsCatalogSubscribePuStatusRsp : public CCatalogSsRsp
{
public:
	CRcsCatalogSubscribePuStatusRsp()
	{
		event = OspExtEventDesc(RCS_CATALOG_SUBSCRIBE_PU_STATUS_RSP);
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CRcsCatalogRefreshSsPuStatusReq : public CCatalogRefreshSsReq
{
public:
	CRcsCatalogRefreshSsPuStatusReq()
	{
		event = OspExtEventDesc(RCS_CATALOG_REFRESH_SS_PU_STATUS_REQ);
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CRcsCatalogRefreshSsPuStatusRsp : public CCatalogRefreshSsRsp
{
public:
	CRcsCatalogRefreshSsPuStatusRsp()
	{
		event = OspExtEventDesc(RCS_CATALOG_REFRESH_SS_PU_STATUS_RSP);
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CRcsCatalogUnsubscribePuStatusReq : public CCatalogUnSsReq
{
public:
	CRcsCatalogUnsubscribePuStatusReq()
	{
		event = OspExtEventDesc(RCS_CATALOG_UNSUBSCRIBE_PU_STATUS_REQ);
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CRcsCatalogUnsubscribePuStatusRsp : public CCatalogUnSsRsp
{
public:
	CRcsCatalogUnsubscribePuStatusRsp()
	{
		event = OspExtEventDesc(RCS_CATALOG_UNSUBSCRIBE_PU_STATUS_RSP);
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CRcsRecordLockOprReq : public CEventReq
{
public:
	CRcsRecordLockOprReq()
	{
		Clear();
		event = OspExtEventDesc(RCS_RECORD_LOCK_OPR_REQ);
	}

	void Clear()
	{
		recToken.clear();
		oprType = RCS_REC_LOCK;
		deadLine = 0;
	}

	const TRecToken &GetRecToken() const
	{
		return recToken;
	}
	TRecToken &GetRecToken()
	{
		return recToken;
	}
	void SetRecToken(const TRecToken &tRecToken)
	{
		recToken = tRecToken;
	}

	const TRecLockOprType &GetOprType() const
	{
		return oprType;
	}
	TRecLockOprType &GetOprType()
	{
		return oprType;
	}
	void SetOprType(const TRecLockOprType &tOprType)
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

private:
	TRecToken recToken;	
	TRecLockOprType oprType;				// 操作类型（锁定/解锁/获取是否解锁）
	int deadLine;							// 解锁时间（绝对时间，为0表示一直锁定到解锁为止）
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CRcsRecordLockOprRsp : public CEventRsp
{
public:
	CRcsRecordLockOprRsp()
	{
		Clear();
		event = OspExtEventDesc(RCS_RECORD_LOCK_OPR_RSP);
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

//////////////////////////////////////////////////////////////////////
//NRU管理

//搜索NRU，可以不填ssTypes
typedef CSsReq CRcsStartDiscoveryNruReq;       //RCS_START_DISCOVERY_NRU_REQ
typedef CSsRsp CRcsStartDiscoveryNruRsp;       //RCS_START_DISCOVERY_NRU_RSP
typedef CUnSsReq CRcsStopDiscoveryNruReq;      //RCS_STOP_DISCOVERY_NRU_REQ
typedef CUnSsRsp CRcsStopDiscoveryNruRsp;      //RCS_STOP_DISCOVERY_NRU_RSP

class CRcsDiscoveryNruNtfReq : public CEventReq
{
public:
    CRcsDiscoveryNruNtfReq()
    {
        event = OspExtEventDesc(RCS_DISCOVERY_NRU_NTF_REQ);
    }

    void Clear()
    {
        nruInfoList.clear();
    }

public:
    //discoveryNruInfo
    const TDiscoveryNruInfoList& GetNruInfoList() const
    {
        return nruInfoList;
    }
    TDiscoveryNruInfoList& GetNruInfoList()
    {
        return nruInfoList;
    }
    void SetNruInfoList(const TDiscoveryNruInfoList& var)
    {
        nruInfoList = var;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

private:
    TDiscoveryNruInfoList nruInfoList;
};

class CRcsDiscoveryNruNtfRsp : public CNtfRsp
{
public:
    CRcsDiscoveryNruNtfRsp()
    {
        event = OspExtEventDesc(RCS_DISCOVERY_NRU_NTF_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CRcsAddNruReq : public CEventReq
{
public:
    CRcsAddNruReq()
    {
        event = OspExtEventDesc(RCS_ADD_NRU_REQ);
    }

    void Clear()
    {
        nruInfo.Clear();
    }

public:
    //nruInfo
    const TNruInfo& GetNruInfo() const
    {
        return nruInfo;
    }
    TNruInfo& GetNruInfo()
    {
        return nruInfo;
    }
    void SetNruInfo(const TNruInfo& var)
    {
        nruInfo = var;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

private:
    TNruInfo nruInfo;
};

class CRcsAddNruRsp : public CEventRsp
{
public:
    CRcsAddNruRsp()
    {
        event = OspExtEventDesc(RCS_ADD_NRU_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CRcsModNruReq : public CEventReq
{
public:
    CRcsModNruReq()
    {
		Clear();
        event = OspExtEventDesc(RCS_MOD_NRU_REQ);
    }

    void Clear()
    {
        nruBaseInfo.Clear();
        isUsed = true;
    }

public:

    //nruBaseInfo
    const TNruBaseInfo& GetNruBaseInfo() const
    {
        return nruBaseInfo;
    }
    TNruBaseInfo& GetNruBaseInfo()
    {
        return nruBaseInfo;
    }
    void SetNruBaseInfo(const TNruBaseInfo& var)
    {
        nruBaseInfo = var;
    }

    //isUsed
    bool GetIsUsed() const
    {
        return isUsed;
    }
    void SetIsUsed(bool var)
    {
        isUsed = var;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

private:
    TNruBaseInfo    nruBaseInfo;        //1. 根据设备编号修改设备信息，比如设备名、设备启停标识等; 2. 设备编号不可改变
    bool            isUsed;             //是否启用
};

class CRcsModNruRsp : public CEventRsp
{
public:
    CRcsModNruRsp()
    {
        event = OspExtEventDesc(RCS_MOD_NRU_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CRcsDelNruReq : public CEventReq
{
public:
    CRcsDelNruReq()
    {
        event = OspExtEventDesc(RCS_DEL_NRU_REQ);
    }

    void Clear()
    {
        nruID.clear();
    }

public:

    //nruID
    const UUIDString& GetNruID() const
    {
        return nruID;
    }
    UUIDString& GetNruID()
    {
        return nruID;
    }
    void SetNruID(const UUIDString& var)
    {
        nruID = var;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

private:
    UUIDString nruID;
};

class CRcsDelNruRsp : public CEventRsp
{
public:
    CRcsDelNruRsp()
    {
        event = OspExtEventDesc(RCS_DEL_NRU_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CRcsModNruNetworkReq : public CEventReq
{
public:
    CRcsModNruNetworkReq()
    {
        event = OspExtEventDesc(RCS_MOD_NRU_NETWORK_REQ);
    }

    void Clear()
    {
        nruID.clear();
        networkInfo.Clear();
    }

public:

    //nruID
    const UUIDString& GetNruID() const
    {
        return nruID;
    }
    UUIDString& GetNruID()
    {
        return nruID;
    }
    void SetNruID(const UUIDString& var)
    {
        nruID = var;
    }

    //networkInfo
    const TNetworkInfo& GetNetworkInfo() const
    {
        return networkInfo;
    }
    TNetworkInfo& GetNetworkInfo()
    {
        return networkInfo;
    }
    void SetNetworkInfo(const TNetworkInfo& var)
    {
        networkInfo = var;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

private:
    UUIDString nruID;
    TNetworkInfo networkInfo;
};

class CRcsModNruNetworkRsp : public CEventRsp
{
public:
    CRcsModNruNetworkRsp()
    {
        event = OspExtEventDesc(RCS_MOD_NRU_NETWORK_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CRcsQueryNruReq : public CQueryReqBase
{
public:
    CRcsQueryNruReq()
    {
        event = OspExtEventDesc(RCS_QUERY_NRU_REQ);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CRcsQueryNruRsp : public CQueryRspBase
{
public:
    CRcsQueryNruRsp()
    {
        event = OspExtEventDesc(RCS_QUERY_NRU_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CRcsQueryNruNtfReq : public CQueryNtfReqBase
{
public:
    CRcsQueryNruNtfReq()
    {
        event = OspExtEventDesc(RCS_QUERY_NRU_NTF_REQ);
    }

    void Clear()
    {
        nruInfoList.clear();
    }

public:
    //nruInfo
    const TNruInfoList& GetNruInfoList() const
    {
        return nruInfoList;
    }
    TNruInfoList& GetNruInfoList()
    {
        return nruInfoList;
    }
    void SetNruInfoList(const TNruInfoList& var)
    {
        nruInfoList = var;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

private:
    TNruInfoList nruInfoList;
};

class CRcsQueryNruNtfRsp : public CNtfRsp
{
public:
    CRcsQueryNruNtfRsp()
    {
        event = OspExtEventDesc(RCS_QUERY_NRU_NTF_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CRcsQueryOnlineNruDetailInfoReq : public CQueryReqBase
{
public:
	CRcsQueryOnlineNruDetailInfoReq()
	{
		Clear();
		event = OspExtEventDesc(RCS_QUERY_ONLINE_NRU_DETAIL_INFO_REQ);
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

class CRcsQueryOnlineNruDetailInfoRsp : public CQueryRspBase
{
public:
	CRcsQueryOnlineNruDetailInfoRsp()
	{
		Clear();
		event = OspExtEventDesc(RCS_QUERY_ONLINE_NRU_DETAIL_INFO_RSP);
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

class CRcsQueryOnlineNruDetailInfoNtfReq : public CQueryNtfReqBase
{
public:
	CRcsQueryOnlineNruDetailInfoNtfReq()
	{
		Clear();
		event = OspExtEventDesc(RCS_QUERY_ONLINE_NRU_DETAIL_INFO_NTF_REQ);
	}

	void Clear()
	{
		nruDetailInfoList.clear();
	}

public:

	const TNruDetailInfoList& GetNruDetailInfoList() const
	{
		return nruDetailInfoList;
	}
	TNruDetailInfoList& GetNruDetailInfoList()
	{
		return nruDetailInfoList;
	}
	void SetNruDetailInfoList(const TNruDetailInfoList& var)
	{
		nruDetailInfoList = var;
	}

private:
	TNruDetailInfoList nruDetailInfoList;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CRcsQueryOnlineNruDetailInfoNtfRsp : public CNtfRsp
{
public:
	CRcsQueryOnlineNruDetailInfoNtfRsp()
	{
		Clear();
		event = OspExtEventDesc(RCS_QUERY_ONLINE_NRU_DETAIL_INFO_NTF_RSP);
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

//订阅NRU设备变更和NRU状态，目前可以不填ssTypes
typedef CSsReq CRcsSubscribeNruReq;          //RCS_SUBSCRIBE_NRU_REQ
typedef CSsRsp CRcsSubscribeNruRsp;          //RCS_SUBSCRIBE_NRU_RSP
typedef CUnSsReq CRcsUnsubscribeNruReq;      //RCS_UNSUBSCRIBE_NRU_REQ
typedef CUnSsRsp CRcsUnsubscribeNruRsp;      //RCS_UNSUBSCRIBE_NRU_RSP


class CRcsNruNtfReq : public CEventReq
{
public:
    CRcsNruNtfReq()
    {
        event = OspExtEventDesc(RCS_NRU_NTF_REQ);
    }

    void Clear()
    {
		ntfOpType.clear();
        nruInfo.Clear();
    }

public:
    //ntfOpType
    const TNtfOpType& GetNtfOpType() const
    {
        return ntfOpType;
    }
    TNtfOpType& GetNtfOpType()
    {
        return ntfOpType;
    }
    void SetNtfOpType(const TNtfOpType& var)
    {
        ntfOpType = var;
    }

    //nruInfo
    const TNruInfo& GetNruInfo() const
    {
        return nruInfo;
    }
    TNruInfo& GetNruInfo()
    {
        return nruInfo;
    }
    void SetNruInfo(const TNruInfo& var)
    {
        nruInfo = var;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

private:
    TNtfOpType  ntfOpType;
    TNruInfo    nruInfo;
};

class CRcsNruNtfRsp : public CNtfRsp
{
public:
    CRcsNruNtfRsp()
    {
        event = OspExtEventDesc(RCS_NRU_NTF_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CRcsFdiskReq : public CEventReq
{
public:
    CRcsFdiskReq()
    {
		Clear();
        event = OspExtEventDesc(RCS_FDISK_REQ);
    }

    void Clear()
    {
        nruID.clear();
        diskDev.Clear();
		partCfgLineList.clear();
		isClearPartHead = true;
    }

public:
    //nruID
    const UUIDString& GetNruID() const
    {
        return nruID;
    }
    UUIDString& GetNruID()
    {
        return nruID;
    }
    void SetNruID(const UUIDString& var)
    {
        nruID = var;
    }

    //diskDev
    const TDiskDev& GetDiskDev() const
    {
        return diskDev;
    }
    TDiskDev& GetDiskDev()
    {
        return diskDev;
    }
    void SetDiskDev(const TDiskDev& var)
    {
        diskDev = var;
    }

    //partCfgLineList
    const TPartCfgLineList& GetPartCfgLineList() const
    {
        return partCfgLineList;
    }
    TPartCfgLineList& GetPartCfgLineList()
    {
        return partCfgLineList;
    }
    void SetPartCfgLineList(const TPartCfgLineList& var)
    {
        partCfgLineList = var;
    }

    //isClearPartHead
    bool GetIsClearPartHead() const
    {
        return isClearPartHead;
    }
    void SetIsClearPartHead(bool var)
    {
        isClearPartHead = var;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

private:
    UUIDString			nruID;
    TDiskDev			diskDev;
	TPartCfgLineList	partCfgLineList;
	bool				isClearPartHead;		//分区完是否清空分区头，有时只是进行恢复分区表操作时，不希望清空分区表。
};

class CRcsFdiskRsp : public CEventRsp
{
public:
    CRcsFdiskRsp()
    {
        event = OspExtEventDesc(RCS_FDISK_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CRcsFormatDiskReq : public CEventReq
{
public:
    CRcsFormatDiskReq()
    {
        event = OspExtEventDesc(RCS_FORMAT_DISK_REQ);
    }

    void Clear()
    {
        nruID.clear();
        diskDev.Clear();
		fsType.clear();
    }

public:
    //nruID
    const UUIDString& GetNruID() const
    {
        return nruID;
    }
    UUIDString& GetNruID()
    {
        return nruID;
    }
    void SetNruID(const UUIDString& var)
    {
        nruID = var;
    }

    //diskDev
    const TDiskDev& GetDiskDev() const
    {
        return diskDev;
    }
    TDiskDev& GetDiskDev()
    {
        return diskDev;
    }
    void SetDiskDev(const TDiskDev& var)
    {
        diskDev = var;
    }

    //fsType
    const TPartType& GetFsType() const
    {
        return fsType;
    }
    TPartType& GetFsType()
    {
        return fsType;
    }
    void SetFsType(const TPartType& var)
    {
        fsType = var;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

private:
    UUIDString  nruID;
    TDiskDev    diskDev;
	TPartType   fsType;
};

class CRcsFormatDiskRsp : public CEventRsp
{
public:
    CRcsFormatDiskRsp()
    {
        event = OspExtEventDesc(RCS_FORMAT_DISK_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CRcsCheckDiskReq : public CEventReq
{
public:
    CRcsCheckDiskReq()
    {
        event = OspExtEventDesc(RCS_CHECK_DISK_REQ);
    }

    void Clear()
    {
        nruID.clear();
        diskDev.Clear();
    }

public:
    //nruID
    const UUIDString& GetNruID() const
    {
        return nruID;
    }
    UUIDString& GetNruID()
    {
        return nruID;
    }
    void SetNruID(const UUIDString& var)
    {
        nruID = var;
    }

    //diskDev
    const TDiskDev& GetDiskDev() const
    {
        return diskDev;
    }
    TDiskDev& GetDiskDev()
    {
        return diskDev;
    }
    void SetDiskDev(const TDiskDev& var)
    {
        diskDev = var;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

private:
    UUIDString nruID;
    TDiskDev   diskDev;
};

class CRcsCheckDiskRsp : public CEventRsp
{
public:
    CRcsCheckDiskRsp()
    {
        event = OspExtEventDesc(RCS_CHECK_DISK_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CRcsMountDiskReq : public CEventReq
{
public:
    CRcsMountDiskReq()
    {
        event = OspExtEventDesc(RCS_MOUNT_DISK_REQ);
    }

    void Clear()
    {
        nruID.clear();
        diskDev.Clear();
        mountPath.clear();
    }

public:
    //nruID
    const UUIDString& GetNruID() const
    {
        return nruID;
    }
    UUIDString& GetNruID()
    {
        return nruID;
    }
    void SetNruID(const UUIDString& var)
    {
        nruID = var;
    }

    //diskDev
    const TDiskDev& GetDiskDev() const
    {
        return diskDev;
    }
    TDiskDev& GetDiskDev()
    {
        return diskDev;
    }
    void SetDiskDev(const TDiskDev& var)
    {
        diskDev = var;
    }

    //mountPath
    const string& GetMountPath() const
    {
        return mountPath;
    }
    string& GetMountPath()
    {
        return mountPath;
    }
    void SetMountPath(const string& var)
    {
        mountPath = var;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

private:
    UUIDString nruID;
    TDiskDev   diskDev;
    string     mountPath;           //默认为空，由服务端自动构造挂载路径
};

class CRcsMountDiskRsp : public CEventRsp
{
public:
    CRcsMountDiskRsp()
    {
        event = OspExtEventDesc(RCS_MOUNT_DISK_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CRcsUnmountDiskReq : public CEventReq
{
public:
    CRcsUnmountDiskReq()
    {
        event = OspExtEventDesc(RCS_UMOUNT_DISK_REQ);
    }

    void Clear()
    {
        nruID.clear();
        diskDev.Clear();
    }

public:
    //nruID
    const UUIDString& GetNruID() const
    {
        return nruID;
    }
    UUIDString& GetNruID()
    {
        return nruID;
    }
    void SetNruID(const UUIDString& var)
    {
        nruID = var;
    }

    //diskDev
    const TDiskDev& GetDiskDev() const
    {
        return diskDev;
    }
    TDiskDev& GetDiskDev()
    {
        return diskDev;
    }
    void SetDiskDev(const TDiskDev& var)
    {
        diskDev = var;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

private:
    UUIDString nruID;
    TDiskDev   diskDev;
};

class CRcsUnmountDiskRsp : public CEventRsp
{
public:
    CRcsUnmountDiskRsp()
    {
        event = OspExtEventDesc(RCS_UMOUNT_DISK_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};



class CRcsCancleDiskOpReq : public CEventReq
{
public:
    CRcsCancleDiskOpReq()
    {
        event = OspExtEventDesc(RCS_CANCEL_DISK_OP_REQ);
    }

    void Clear()
    {
        nruID.clear();
        diskDev.Clear();
    }

public:
    //nruID
    const UUIDString& GetNruID() const
    {
        return nruID;
    }
    UUIDString& GetNruID()
    {
        return nruID;
    }
    void SetNruID(const UUIDString& var)
    {
        nruID = var;
    }

    //diskDev
    const TDiskDev& GetDiskDev() const
    {
        return diskDev;
    }
    TDiskDev& GetDiskDev()
    {
        return diskDev;
    }
    void SetDiskDev(const TDiskDev& var)
    {
        diskDev = var;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

private:
    UUIDString nruID;
    TDiskDev   diskDev;
};

class CRcsCancleDiskOpRsp : public CEventRsp
{
public:
    CRcsCancleDiskOpRsp()
    {
        event = OspExtEventDesc(RCS_CANCEL_DISK_OP_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CRcsDelOfflineDiskReq : public CEventReq
{
public:
    CRcsDelOfflineDiskReq()
    {
        event = OspExtEventDesc(RCS_DEL_OFFLINE_DISK_REQ);
    }

    void Clear()
    {
        nruID.clear();
        diskDev.Clear();
    }

public:
    //nruID
    const UUIDString& GetNruID() const
    {
        return nruID;
    }
    UUIDString& GetNruID()
    {
        return nruID;
    }
    void SetNruID(const UUIDString& var)
    {
        nruID = var;
    }

    //diskDev
    const TDiskDev& GetDiskDev() const
    {
        return diskDev;
    }
    TDiskDev& GetDiskDev()
    {
        return diskDev;
    }
    void SetDiskDev(const TDiskDev& var)
    {
        diskDev = var;
    }

    //isDelVideoRecord
    bool GetIsDelVideoRecord() const
    {
        return isDelVideoRecord;
    }
    void SetIsDelVideoRecord(bool var)
    {
        isDelVideoRecord = var;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

private:
    UUIDString nruID;
    TDiskDev   diskDev;
    bool       isDelVideoRecord;
};

class CRcsDelOfflineDiskRsp : public CEventRsp
{
public:
    CRcsDelOfflineDiskRsp()
    {
        event = OspExtEventDesc(RCS_DEL_OFFLINE_DISK_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CRcsQueryDiskInfoReq : public CQueryReqBase
{
public:
    CRcsQueryDiskInfoReq()
    {
        event = OspExtEventDesc(RCS_QUERY_DISK_INFO_REQ);
    }
    void Clear()
    {
		nruID.clear();
    }

public:
    //nruID
    const UUIDString& GetNruID() const
    {
        return nruID;
    }
    UUIDString& GetNruID()
    {
        return nruID;
    }
    void SetNruID(const UUIDString& var)
    {
        nruID = var;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

private:
	UUIDString nruID;
};

class CRcsQueryDiskInfoRsp : public CQueryRspBase
{
public:
    CRcsQueryDiskInfoRsp()
    {
        event = OspExtEventDesc(RCS_QUERY_DISK_INFO_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CRcsQueryDiskInfoNtfReq : public CQueryNtfReqBase
{
public:
    CRcsQueryDiskInfoNtfReq()
    {
        event = OspExtEventDesc(RCS_QUERY_DISK_INFO_NTF_REQ);
    }

    void Clear()
    {
        nruID.clear();
        diskInfoList.clear();
    }

public:
    //nruID
    const UUIDString& GetNruID() const
    {
        return nruID;
    }
    UUIDString& GetNruID()
    {
        return nruID;
    }
    void SetNruID(const UUIDString& var)
    {
        nruID = var;
    }

	//diskInfo
    const TDiskInfoList& GetDiskInfoList() const
    {
        return diskInfoList;
    }
    TDiskInfoList& GetDiskInfoList()
    {
        return diskInfoList;
    }
    void SetDiskInfoList(const TDiskInfoList& var)
    {
        diskInfoList = var;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

private:
    UUIDString      nruID;
    TDiskInfoList	diskInfoList;
};

class CRcsQueryDiskInfoNtfRsp : public CNtfRsp
{
public:
    CRcsQueryDiskInfoNtfRsp()
    {
        event = OspExtEventDesc(RCS_QUERY_DISK_INFO_NTF_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


//订阅磁盘变更和磁盘状态
#define SS_TYPE_DISKDEV_INFO        "DISKDEV_INFO"
#define SS_TYPE_DISKDEV_STATE       "DISKDEV_STATE"
#define SS_TYPE_DISKDEV_OP_STATE    "DISKDEV_OP_STATE"
#define SS_TYPE_DISKDEV_IOSTAT      "DISKDEV_IOSTAT"
#define SS_TYPE_DISKDEV_ABN         "DISKDEV_ABN"
#define SS_TYPE_DISK_PART_ALARM     "DISK_PART_ALARM"

typedef CSsReq CRcsSubscribeDiskInfoReq;         //RCS_SUBSCRIBE_DISKINFO_REQ ,订阅的目的设备是精确到NRU
typedef CSsRsp CRcsSubscribeDiskInfoRsp;         //RCS_SUBSCRIBE_DISKINFO_RSP
typedef CUnSsReq CRcsUnsubscribeDiskInfoReq;     //RCS_UNSUBSCRIBE_DISKINFO_REQ
typedef CUnSsRsp CRcsUnsubscribeDiskInfoRsp;     //RCS_UNSUBSCRIBE_DISKINFO_RSP
typedef CRefreshSsReq CRcsRefreshSsNruDiskInfoReq;      //RCS_REFRESH_SS_DISKINFO_REQ
typedef CRefreshSsRsp CRcsRefreshSsNruDiskInfoRsp;      //RCS_REFRESH_SS_DISKINFO_RSP


class CRcsDiskInfoNtfReq : public CEventReq
{
public:
    CRcsDiskInfoNtfReq()
    {
        event = OspExtEventDesc(RCS_DISK_INFO_NTF_REQ);
    }

    void Clear()
    {
        //nruID.clear();
        diskInfo.Clear();
		ntfOpType.clear();
    }

public:
    //nruID
    //const UUIDString& GetNruID() const
    //{
    //    return nruID;
    //}
    //UUIDString& GetNruID()
    //{
    //    return nruID;
    //}
    //void SetNruID(const UUIDString& var)
    //{
    //    nruID = var;
    //}

	//diskInfo
    const TDiskInfo& GetDiskInfo() const
    {
        return diskInfo;
    }
    TDiskInfo& GetDiskInfo()
    {
        return diskInfo;
    }
    void SetDiskInfo(const TDiskInfo& var)
    {
        diskInfo = var;
    }

    //ntfOpType
    const TNtfOpType& GetNtfOpType() const
    {
        return ntfOpType;
    }
    TNtfOpType& GetNtfOpType()
    {
        return ntfOpType;
    }
    void SetNtfOpType(const TNtfOpType& var)
    {
        ntfOpType = var;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

private:
    //UUIDString      nruID;
    TDiskInfo		diskInfo;
	TNtfOpType		ntfOpType;
};

class CRcsDiskInfoNtfRsp : public CNtfRsp
{
public:
    CRcsDiskInfoNtfRsp()
    {
        event = OspExtEventDesc(RCS_DISK_INFO_NTF_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CRcsDiskPartInfoNtfReq : public CEventReq
{
public:
    CRcsDiskPartInfoNtfReq()
    {
        event = OspExtEventDesc(RCS_DISK_PART_INFO_NTF_REQ);
    }


public:
    //nruID
    //const UUIDString& GetNruID() const
    //{
    //    return nruID;
    //}
    //UUIDString& GetNruID()
    //{
    //    return nruID;
    //}
    //void SetNruID(const UUIDString& var)
    //{
    //    nruID = var;
    //}

	//partInfo
    const TPartInfo& GetPartInfo() const
    {
        return partInfo;
    }
    TPartInfo& GetPartInfo()
    {
        return partInfo;
    }
    void SetPartInfo(const TPartInfo& var)
    {
        partInfo = var;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

private:
    //UUIDString      nruID;
    TPartInfo		partInfo;
};

class CRcsDiskPartInfoNtfRsp : public CNtfRsp
{
public:
    CRcsDiskPartInfoNtfRsp()
    {
        event = OspExtEventDesc(RCS_DISK_PART_INFO_NTF_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};



class CRcsDiskStateNtfReq : public CEventReq
{
public:
    CRcsDiskStateNtfReq()
    {
        event = OspExtEventDesc(RCS_DISK_STATE_NTF_REQ);
    }

    void Clear()
    {
        diskDev.Clear();
		diskState.clear();
    }

public:
    //diskDev
    const TDiskDev& GetDiskDev() const
    {
        return diskDev;
    }
    TDiskDev& GetDiskDev()
    {
        return diskDev;
    }
    void SetDiskDev(const TDiskDev& var)
    {
        diskDev = var;
    }

    //diskState
    const TDiskState& GetDiskState() const
    {
        return diskState;
    }
    TDiskState& GetDiskState()
    {
        return diskState;
    }
    void SetDiskState(const TDiskState& var)
    {
        diskState = var;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

private:
    TDiskDev        diskDev;
    TDiskState		diskState;
};

class CRcsDiskStateNtfRsp : public CNtfRsp
{
public:
    CRcsDiskStateNtfRsp()
    {
        event = OspExtEventDesc(RCS_DISK_STATE_NTF_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CRcsDiskPartStateNtfReq : public CEventReq
{
public:
    CRcsDiskPartStateNtfReq()
    {
        event = OspExtEventDesc(RCS_DISK_PART_STATE_NTF_REQ);
    }


public:
    //diskDev
    const TDiskDev& GetDiskDev() const
    {
        return diskDev;
    }
    TDiskDev& GetDiskDev()
    {
        return diskDev;
    }
    void SetDiskDev(const TDiskDev& var)
    {
        diskDev = var;
    }

	//partState
    const TPartState& GetPartState() const
    {
        return partState;
    }
    TPartState& GetPartState()
    {
        return partState;
    }
    void SetPartState(const TPartState& var)
    {
        partState = var;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

private:
    TDiskDev        diskDev;
    TPartState		partState;
};

class CRcsDiskPartStateNtfRsp : public CNtfRsp
{
public:
    CRcsDiskPartStateNtfRsp()
    {
        event = OspExtEventDesc(RCS_DISK_PART_STATE_NTF_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CRcsDiskOpStateNtfReq : public CEventReq
{
public:
    CRcsDiskOpStateNtfReq()
    {
        event = OspExtEventDesc(RCS_DISK_OP_STATE_NTF_REQ);
    }


public:
    //diskDev
    const TDiskDev& GetDiskDev() const
    {
        return diskDev;
    }
    TDiskDev& GetDiskDev()
    {
        return diskDev;
    }
    void SetDiskDev(const TDiskDev& var)
    {
        diskDev = var;
    }

	//opState
    const TDiskDevOpState& GetOpState() const
    {
        return opState;
    }
    TDiskDevOpState& GetOpState()
    {
        return opState;
    }
    void SetOpState(const TDiskDevOpState& var)
    {
        opState = var;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

private:
    TDiskDev            diskDev;
    TDiskDevOpState     opState;
};

class CRcsDiskOpStateNtfRsp : public CNtfRsp
{
public:
    CRcsDiskOpStateNtfRsp()
    {
        event = OspExtEventDesc(RCS_DISK_OP_STATE_NTF_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CRcsDiskIoUtilNtfReq : public CEventReq
{
public:
    CRcsDiskIoUtilNtfReq()
    {
        event = OspExtEventDesc(RCS_DISK_IOUTIL_NTF_REQ);
    }

    void Clear()
    {
        diskDev.Clear();
        ioUtil = 0;
    }

public:
    //diskDev
    const TDiskDev& GetDiskDev() const
    {
        return diskDev;
    }
    TDiskDev& GetDiskDev()
    {
        return diskDev;
    }
    void SetDiskDev(const TDiskDev& var)
    {
        diskDev = var;
    }

    //ioUtil
    void SetIoUtil(int nIoUtil)
    {
        ioUtil = nIoUtil;
    }
    int GetIoUtil() const
    {
        return ioUtil;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

private:
    TDiskDev        diskDev;
    int    			ioUtil;       //IO使用率
};

class CRcsDiskIoUtilNtfRsp : public CNtfRsp
{
public:
    CRcsDiskIoUtilNtfRsp()
    {
        event = OspExtEventDesc(RCS_DISK_IOUTIL_NTF_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CRcsDiskAbnNtfReq : public CEventReq
{
public:
    CRcsDiskAbnNtfReq()
    {
        event = OspExtEventDesc(RCS_DISK_ABN_NTF_REQ);
    }
    void Clear()
    {
        diskDev.Clear();
        abnNum = 0;
        abnCode.clear();
    }

public:
    //diskDev
    const TDiskDev& GetDiskDev() const
    {
        return diskDev;
    }
    TDiskDev& GetDiskDev()
    {
        return diskDev;
    }
    void SetDiskDev(const TDiskDev& var)
    {
        diskDev = var;
    }

    //abnNum
    void SetAbnNum(int var)
    {
        abnNum = var;
    }
    int GetAbnNum() const
    {
        return abnNum;
    }

    //abnCode
    void SetAbnCode(const TDiskAbnCode& var)
    {
        abnCode = var;
    }
    TDiskAbnCode& GetAbnCode()
    {
        return abnCode;
    }
    const TDiskAbnCode& GetAbnCode() const
    {
        return abnCode;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

private:
    TDiskDev        diskDev;
    int             abnNum;          //异常次数
	TDiskAbnCode    abnCode;         //异常代码
};

class CRcsDiskAbnNtfRsp : public CNtfRsp
{
public:
    CRcsDiskAbnNtfRsp()
    {
        event = OspExtEventDesc(RCS_DISK_ABN_NTF_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CRcsDiskPartAlarmNtfReq : public CEventReq
{
public:
	CRcsDiskPartAlarmNtfReq()
	{
		Clear();
		event = OspExtEventDesc(RCS_DISK_PART_ALARM_NTF_REQ);
	}
	void Clear()
	{
		partInfo.Clear();
	}

public:
	const TNruInfo& GetNruInfo() const
	{
		return nruInfo;
	}
	TNruInfo& GetNruInfo()
	{
		return nruInfo;
	}
	void SetNruInfo(const TNruInfo& var)
	{
		nruInfo = var;
	}

	const TPartInfo& GetPartInfo() const
	{
		return partInfo;
	}
	TPartInfo& GetPartInfo()
	{
		return partInfo;
	}
	void SetPartInfo(const TPartInfo& var)
	{
		partInfo = var;
	}

private:
	TNruInfo  nruInfo;
	TPartInfo partInfo;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CRcsDiskPartAlarmNtfRsp : public CNtfRsp
{
public:
	CRcsDiskPartAlarmNtfRsp()
	{
		Clear();
		event = OspExtEventDesc(RCS_DISK_PART_ALARM_NTF_RSP);
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


class CRcsQueryDiskLogReq : public CQueryReqBase
{
public:
    CRcsQueryDiskLogReq()
    {
        event = OspExtEventDesc(RCS_QUERY_DISK_LOG_REQ);
    }
    void Clear()
    {
		nruID.clear();
        diskDev.Clear();
        timeRange.Clear();
    }

public:
    //nruID
    const UUIDString& GetNruID() const
    {
        return nruID;
    }
    UUIDString& GetNruID()
    {
        return nruID;
    }
    void SetNruID(const UUIDString& var)
    {
        nruID = var;
    }

    //diskDev
    const TDiskDev& GetDiskDev() const
    {
        return diskDev;
    }
    TDiskDev& GetDiskDev()
    {
        return diskDev;
    }
    void SetDiskDev(const TDiskDev& var)
    {
        diskDev = var;
    }

    //timeRange
    const TTimeRange& GetTimeRange() const
    {
        return timeRange;
    }
    TTimeRange& GetTimeRange()
    {
        return timeRange;
    }
    void SetTimeRange(const TTimeRange& var)
    {
        timeRange = var;
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

private:
	UUIDString nruID;
    TDiskDev   diskDev;     //必选
    TTimeRange timeRange;
};

class CRcsQueryDiskLogRsp : public CQueryRspBase
{
public:
    CRcsQueryDiskLogRsp()
    {
        event = OspExtEventDesc(RCS_QUERY_DISK_LOG_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CRcsQueryDiskLogNtfReq : public CQueryNtfReqBase
{
public:
    CRcsQueryDiskLogNtfReq()
    {
        event = OspExtEventDesc(RCS_QUERY_DISK_LOG_NTF_REQ);
    }

    void Clear()
    {
        diskLogList.clear();
    }

public:
    //nruInfo
    const TDiskLogList& GetDiskLogList() const
    {
        return diskLogList;
    }
    TDiskLogList& GetDiskLogList()
    {
        return diskLogList;
    }
    void SetDiskLogList(const TDiskLogList& var)
    {
        diskLogList = var;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

private:
    TDiskLogList    diskLogList;
};

class CRcsQueryDiskLogNtfRsp : public CNtfRsp
{
public:
    CRcsQueryDiskLogNtfRsp()
    {
        event = OspExtEventDesc(RCS_QUERY_DISK_LOG_NTF_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CRcsGetNetFSCfgReq : public CEventReq
{
private:
	UUIDString nruID;             

public:
	CRcsGetNetFSCfgReq()
	{
		event = OspExtEventDesc(RCS_GET_NETFS_CFG_REQ);
		Clear();
	}

	void Clear()
	{
		nruID.clear();
	}
public:
	void SetNruID(const UUIDString& strNruID)
	{
		nruID = strNruID;
	}
	UUIDString& GetNruID()
	{
		return nruID;
	}
	const UUIDString& GetNruID() const
	{
		return nruID;
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class CRcsGetNetFSCfgRsp : public CEventRsp
{
private:
	TNetFSCfgList netFSCfgList; 

public:
	CRcsGetNetFSCfgRsp()
	{
		event = OspExtEventDesc(RCS_GET_NETFS_CFG_RSP);
		Clear();
	}

	void Clear()
	{
		netFSCfgList.clear();
	}

public:
	void SetNetFSCfgList(const TNetFSCfgList& var)
	{
		netFSCfgList = var;
	}
	TNetFSCfgList& GetNetFSCfgList()
	{
		return netFSCfgList;
	}
	const TNetFSCfgList& GetNetFSCfgList() const
	{
		return netFSCfgList;
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};


class CRcsAddNetFSCfgReq : public CEventReq
{
private:
	UUIDString nruID;             
	TNetFSCfg  netFSCfg;           

public:
	CRcsAddNetFSCfgReq()
	{
		event = OspExtEventDesc(RCS_ADD_NETFS_CFG_REQ);
		Clear();
	}

	void Clear()
	{
		nruID.clear();
		netFSCfg.Clear();
	}

public:
	void SetNruID(const UUIDString& strNruID)
	{
		nruID = strNruID;
	}
	UUIDString& GetNruID()
	{
		return nruID;
	}
	const UUIDString& GetNruID() const
	{
		return nruID;
	}

	void SetNetFSCfg(const TNetFSCfg& var)
	{
		netFSCfg = var;
	}
	TNetFSCfg& GetNetFSCfg()
	{
		return netFSCfg;
	}
	const TNetFSCfg& GetNetFSCfg() const
	{
		return netFSCfg;
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class CRcsAddNetFSCfgRsp : public CEventRsp
{
public:
	CRcsAddNetFSCfgRsp()
	{
		event = OspExtEventDesc(RCS_ADD_NETFS_CFG_RSP);
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};


class CRcsDelNetFSCfgReq : public CEventReq
{
private:
	UUIDString nruID;             
	TNetFSCfg  netFSCfg;             

public:
	CRcsDelNetFSCfgReq()
	{
		event = OspExtEventDesc(RCS_DEL_NETFS_CFG_REQ);
		Clear();
	}

	void Clear()
	{
		nruID.clear();
		netFSCfg.Clear();
	}

public:
	void SetNruID(const UUIDString& strNruID)
	{
		nruID = strNruID;
	}
	UUIDString& GetNruID()
	{
		return nruID;
	}
	const UUIDString& GetNruID() const
	{
		return nruID;
	}

	void SetNetFSCfg(const TNetFSCfg& var)
	{
		netFSCfg = var;
	}
	TNetFSCfg& GetNetFSCfg()
	{
		return netFSCfg;
	}
	const TNetFSCfg& GetNetFSCfg() const
	{
		return netFSCfg;
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class CRcsDelNetFSCfgRsp : public CEventRsp
{
public:
	CRcsDelNetFSCfgRsp()
	{
		event = OspExtEventDesc(RCS_DEL_NETFS_CFG_RSP);
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};


class CRcsSetDiskUsageReq : public CEventReq
{
private:
	UUIDString nruID;
	TDiskDev   diskDev;
	TDiskUsage diskUsage;

public:
	CRcsSetDiskUsageReq()
	{
		event = OspExtEventDesc(RCS_SET_DISK_USAGE_REQ);
		Clear();
	}

	void Clear()
	{
		nruID.clear();
		diskDev.Clear();
		diskUsage.clear();
	}

public:
	//nruID
	const UUIDString& GetNruID() const
	{
		return nruID;
	}
	UUIDString& GetNruID()
	{
		return nruID;
	}
	void SetNruID(const UUIDString& var)
	{
		nruID = var;
	}

	//diskDev
	const TDiskDev& GetDiskDev() const
	{
		return diskDev;
	}
	TDiskDev& GetDiskDev()
	{
		return diskDev;
	}
	void SetDiskDev(const TDiskDev& var)
	{
		diskDev = var;
	}

	//diskUsage
	const TDiskUsage& GetDiskUsage() const
	{
		return diskUsage;
	}
	TDiskUsage& GetDiskUsage()
	{
		return diskUsage;
	}
	void SetDiskUsage(const TDiskUsage& var)
	{
		diskUsage = var;
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class CRcsSetDiskUsageRsp : public CEventRsp
{
public:
	CRcsSetDiskUsageRsp()
	{
		event = OspExtEventDesc(RCS_SET_DISK_USAGE_RSP);
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};


class CRcsGetNetFSExportListReq : public CEventReq
{
private:
	UUIDString nruID;
	string     serverAddr;
	int        pageNum;
	int        numPerPage;

public:
	CRcsGetNetFSExportListReq()
	{
		event = OspExtEventDesc(RCS_GET_NETFS_EXPORT_LIST_REQ);
		Clear();
	}

	void Clear()
	{
		nruID.clear();
		serverAddr.clear();
		pageNum = 0;
		numPerPage = 0;
	}
public:
	void SetNruID(const UUIDString& strNruID)
	{
		nruID = strNruID;
	}
	UUIDString& GetNruID()
	{
		return nruID;
	}
	const UUIDString& GetNruID() const
	{
		return nruID;
	}

	void SetServerAddr(const string& var)
	{
		serverAddr = var;
	}
	string& GetServerAddr()
	{
		return serverAddr;
	}
	const string& GetServerAddr() const
	{
		return serverAddr;
	}

	void SetPageNum(const int var)
	{
		pageNum = var;
	}
	int GetPageNum()
	{
		return pageNum;
	}
	const int GetPageNum() const
	{
		return pageNum;
	}

	void SetNumPerPage(const int var)
	{
		numPerPage = var;
	}
	int GetNumPerPage()
	{
		return numPerPage;
	}
	const int GetNumPerPage() const
	{
		return numPerPage;
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class CRcsGetNetFSExportListRsp : public CEventRsp
{
private:
	TNetFSExportList netFSExportList; 
	int              totalNum;

public:
	CRcsGetNetFSExportListRsp()
	{
		event = OspExtEventDesc(RCS_GET_NETFS_EXPORT_LIST_RSP);
		Clear();
	}

	void Clear()
	{
		netFSExportList.clear();
		totalNum = 0;
	}

public:
	void SetNetFSExportList(const TNetFSExportList& var)
	{
		netFSExportList = var;
	}
	TNetFSExportList& GetNetFSExportList()
	{
		return netFSExportList;
	}
	const TNetFSExportList& GetNetFSExportList() const
	{
		return netFSExportList;
	}

	void SetTotalNum(const int var)
	{
		totalNum = var;
	}
	int GetTotalNum()
	{
		return totalNum;
	}
	const int GetTotalNum() const
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


class CRcsGetIscsiConfReq : public CEventReq
{
private:
    UUIDString nruID;             

public:
    CRcsGetIscsiConfReq()
    {
        event = OspExtEventDesc(RCS_GET_ISCSI_CONF_REQ);
    }

    void Clear()
    {
        nruID.clear();
    }
public:
    void SetNruID(const UUIDString& strNruID)
    {
        nruID = strNruID;
    }
    UUIDString& GetNruID()
    {
        return nruID;
    }
    const UUIDString& GetNruID() const
    {
        return nruID;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CRcsGetIscsiConfRsp : public CEventRsp
{
private:
    TIscsiState tIscsiState;
    TIscsiConfList iscsiConfList;             
public:
    CRcsGetIscsiConfRsp()
    {
        event = OspExtEventDesc(RCS_GET_ISCSI_CONF_RSP);
    }
public:
    void SetIscsiConfList(const TIscsiConfList& cIscsiConfList)
    {
        iscsiConfList = cIscsiConfList;
    }
    TIscsiConfList& GetIscsiConfList()
    {
        return iscsiConfList;
    }
    const TIscsiConfList& GetIscsiConfList() const
    {
        return iscsiConfList;
    }

    void SetIscsiState(const TIscsiState& cIscsiState)
    {
        tIscsiState = cIscsiState;
    }

    TIscsiState& GetIscsiState()
    {
        return tIscsiState;
    }

    const TIscsiState& GetIscsiState() const
    {
        return tIscsiState;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CRcsAddIscsiConfReq : public CEventReq
{
private:
    UUIDString nruID;             
    TIscsiConf iscsiConf;             

public:
    CRcsAddIscsiConfReq()
    {
        event = OspExtEventDesc(RCS_ADD_ISCSI_CONF_REQ);
    }

    void Clear()
    {
        nruID.clear();
        iscsiConf.Clear();
    }
public:
    void SetNruID(const UUIDString& strNruID)
    {
        nruID = strNruID;
    }
    UUIDString& GetNruID()
    {
        return nruID;
    }
    const UUIDString& GetNruID() const
    {
        return nruID;
    }

    void SetIscsiConf(const TIscsiConf& cIscsiConf)
    {
        iscsiConf = cIscsiConf;
    }
    TIscsiConf& GetIscsiConf()
    {
        return iscsiConf;
    }
    const TIscsiConf& GetIscsiConf() const
    {
        return iscsiConf;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CRcsAddIscsiConfRsp : public CEventRsp
{
public:
    CRcsAddIscsiConfRsp()
    {
        event = OspExtEventDesc(RCS_ADD_ISCSI_CONF_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CRcsDelIscsiConfReq : public CEventReq
{
private:
    UUIDString nruID;             
    TNetAddr   discoveryAddr;             

public:
    CRcsDelIscsiConfReq()
    {
        event = OspExtEventDesc(RCS_DEL_ISCSI_CONF_REQ);
    }

    void Clear()
    {
        nruID.clear();
        discoveryAddr.Clear();
    }
public:
    void SetNruID(const UUIDString& strNruID)
    {
        nruID = strNruID;
    }
    UUIDString& GetNruID()
    {
        return nruID;
    }
    const UUIDString& GetNruID() const
    {
        return nruID;
    }

    void SetDiscoveryAddr(const TNetAddr& cDiscoveryAddr)
    {
        discoveryAddr = cDiscoveryAddr;
    }
    TNetAddr& GetDiscoveryAddr()
    {
        return discoveryAddr;
    }
    const TNetAddr& GetDiscoveryAddr() const
    {
        return discoveryAddr;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CRcsDelIscsiConfRsp : public CEventRsp
{
public:
    CRcsDelIscsiConfRsp()
    {
        event = OspExtEventDesc(RCS_DEL_ISCSI_CONF_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CRcsCtrlIscsiReq : public CEventReq
{
private:
    UUIDString nruID;             
    TIscsiCtrl iscsiCtrl;             

public:
    CRcsCtrlIscsiReq()
    {
        event = OspExtEventDesc(RCS_CTRL_ISCSI_REQ);
    }

    void Clear()
    {
        nruID.clear();
        iscsiCtrl.clear();
    }
public:
    void SetNruID(const UUIDString& strNruID)
    {
        nruID = strNruID;
    }
    UUIDString& GetNruID()
    {
        return nruID;
    }
    const UUIDString& GetNruID() const
    {
        return nruID;
    }

    void SetIscsiCtrl(const TIscsiCtrl& strIscsiCtrl)
    {
        iscsiCtrl = strIscsiCtrl;
    }
    TIscsiCtrl& GetIscsiCtrl()
    {
        return iscsiCtrl;
    }
    const TIscsiCtrl& GetIscsiCtrl() const
    {
        return iscsiCtrl;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CRcsCtrlIscsiRsp : public CEventRsp
{
public:
    CRcsCtrlIscsiRsp()
    {
        event = OspExtEventDesc(RCS_CTRL_ISCSI_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CRcsDisconnectIscsiReq : public CEventReq
{
private:
    UUIDString nruID;             
    TNetAddr   discoveryAddr;             

public:
    CRcsDisconnectIscsiReq()
    {
        event = OspExtEventDesc(RCS_DISCONNECT_ISCSI_REQ);
    }

    void Clear()
    {
        nruID.clear();
        discoveryAddr.Clear();
    }
public:
    void SetNruID(const UUIDString& strNruID)
    {
        nruID = strNruID;
    }
    UUIDString& GetNruID()
    {
        return nruID;
    }
    const UUIDString& GetNruID() const
    {
        return nruID;
    }

    void SetDiscoveryAddr(const TNetAddr& cDiscoveryAddr)
    {
        discoveryAddr = cDiscoveryAddr;
    }
    TNetAddr& GetDiscoveryAddr()
    {
        return discoveryAddr;
    }
    const TNetAddr& GetDiscoveryAddr() const
    {
        return discoveryAddr;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CRcsDisconnectIscsiRsp : public CEventRsp
{
public:
    CRcsDisconnectIscsiRsp()
    {
        event = OspExtEventDesc(RCS_DISCONNECT_ISCSI_RSP);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CRcsGetArrayInfoReq : public CEventReq
{
private:
    UUIDString nruID;             

public:
    CRcsGetArrayInfoReq()
    {
        event = OspExtEventDesc(RCS_GET_ARRAY_INFO_REQ);
    }

    void Clear()
    {
        nruID.clear();
    }
public:
    void SetNruID(const UUIDString& strNruID)
    {
        nruID = strNruID;
    }
    UUIDString& GetNruID()
    {
        return nruID;
    }
    const UUIDString& GetNruID() const
    {
        return nruID;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CRcsGetArrayInfoRsp : public CEventRsp
{
private:
    TArrayInfoList arrayInfoList;             

public:
    CRcsGetArrayInfoRsp()
    {
        event = OspExtEventDesc(RCS_GET_ARRAY_INFO_RSP);
    }
public:
    void SetArrayInfoList(const TArrayInfoList& cArrayInfoList)
    {
        arrayInfoList = cArrayInfoList;
    }
    TArrayInfoList& GetArrayInfoList()
    {
        return arrayInfoList;
    }
    const TArrayInfoList& GetArrayInfoList() const
    {
        return arrayInfoList;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CRcsGetDiskGroupInfoReq : public CEventReq
{
private:
    UUIDString nruID;             
	TIpAddr	   arrayIP;             

public:
    CRcsGetDiskGroupInfoReq()
    {
        event = OspExtEventDesc(RCS_GET_DISK_GROUP_INFO_REQ);
    }

    void Clear()
    {
        nruID.clear();
		arrayIP.clear();
    }
public:
    void SetNruID(const UUIDString& strNruID)
    {
        nruID = strNruID;
    }
    UUIDString& GetNruID()
    {
        return nruID;
    }
    const UUIDString& GetNruID() const
    {
        return nruID;
    }

    void SetArrayIP(const TIpAddr& cArrayIP)
    {
        arrayIP = cArrayIP;
    }
    TIpAddr& GetArrayIP()
    {
        return arrayIP;
    }
    const TIpAddr& GetArrayIP() const
    {
        return arrayIP;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CRcsGetDiskGroupInfoRsp : public CEventRsp
{
private:
    TDiskGroupInfoList diskGroupInfoList;             

public:
    CRcsGetDiskGroupInfoRsp()
    {
        event = OspExtEventDesc(RCS_GET_DISK_GROUP_INFO_RSP);
    }
public:
    void SetDiskGroupInfoList(const TDiskGroupInfoList& cDiskGroupInfoList)
    {
        diskGroupInfoList = cDiskGroupInfoList;
    }
    TDiskGroupInfoList& GetDiskGroupInfoList()
    {
        return diskGroupInfoList;
    }
    const TDiskGroupInfoList& GetDiskGroupInfoList() const
    {
        return diskGroupInfoList;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CRcsGetPhysicalDiskInfoReq : public CEventReq
{
private:
    UUIDString nruID;             
	TIpAddr	   arrayIP;             

public:
    CRcsGetPhysicalDiskInfoReq()
    {
        event = OspExtEventDesc(RCS_GET_PHYSICAL_DISK_INFO_REQ);
    }

    void Clear()
    {
        nruID.clear();
		arrayIP.clear();
    }
public:
    void SetNruID(const UUIDString& strNruID)
    {
        nruID = strNruID;
    }
    UUIDString& GetNruID()
    {
        return nruID;
    }
    const UUIDString& GetNruID() const
    {
        return nruID;
    }

    void SetArrayIP(const TIpAddr& cArrayIP)
    {
        arrayIP = cArrayIP;
    }
    TIpAddr& GetArrayIP()
    {
        return arrayIP;
    }
    const TIpAddr& GetArrayIP() const
    {
        return arrayIP;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CRcsGetPhysicalDiskInfoRsp : public CEventRsp
{
private:
    TPhysicalDiskInfoList physicalDiskInfoList;             

public:
    CRcsGetPhysicalDiskInfoRsp()
    {
        event = OspExtEventDesc(RCS_GET_PHYSICAL_DISK_INFO_RSP);
    }
public:
    void SetPhysicalDiskInfoList(const TPhysicalDiskInfoList& cPhysicalDiskInfoList)
    {
        physicalDiskInfoList = cPhysicalDiskInfoList;
    }
    TPhysicalDiskInfoList& GetPhysicalDiskInfoList()
    {
        return physicalDiskInfoList;
    }
    const TPhysicalDiskInfoList& GetPhysicalDiskInfoList() const
    {
        return physicalDiskInfoList;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CRcsGetVirtualDiskInfoReq : public CEventReq
{
private:
    UUIDString nruID;             
	TIpAddr	   arrayIP;             
	string	   diskGroupName;             

public:
    CRcsGetVirtualDiskInfoReq()
    {
        event = OspExtEventDesc(RCS_GET_VIRTUAL_DISK_INFO_REQ);
    }

    void Clear()
    {
        nruID.clear();
		arrayIP.clear();
		diskGroupName.clear();
    }
public:
    void SetNruID(const UUIDString& strNruID)
    {
        nruID = strNruID;
    }
    UUIDString& GetNruID()
    {
        return nruID;
    }
    const UUIDString& GetNruID() const
    {
        return nruID;
    }

    void SetArrayIP(const TIpAddr& cArrayIP)
    {
        arrayIP = cArrayIP;
    }
    TIpAddr& GetArrayIP()
    {
        return arrayIP;
    }
    const TIpAddr& GetArrayIP() const
    {
        return arrayIP;
    }

    void SetDiskGroupName(const string& strDiskGroupName)
    {
        diskGroupName = strDiskGroupName;
    }
    string& GetDiskGroupName()
    {
        return diskGroupName;
    }
    const string& GetDiskGroupName() const
    {
        return diskGroupName;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CRcsGetVirtualDiskInfoRsp : public CEventRsp
{
private:
    TVirtualDiskInfoList virtualDiskInfoList;             

public:
    CRcsGetVirtualDiskInfoRsp()
    {
        event = OspExtEventDesc(RCS_GET_VIRTUAL_DISK_INFO_RSP);
    }
public:
    void SetVirtualDiskInfoList(const TVirtualDiskInfoList& cVirtualDiskInfoList)
    {
        virtualDiskInfoList = cVirtualDiskInfoList;
    }
    TVirtualDiskInfoList& GetVirtualDiskInfoList()
    {
        return virtualDiskInfoList;
    }
    const TVirtualDiskInfoList& GetVirtualDiskInfoList() const
    {
        return virtualDiskInfoList;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CRcsGetIscsiInitatorNameReq : public CEventReq
{
private:
    UUIDString  nruID;        

public:
    CRcsGetIscsiInitatorNameReq()
    {
        event = OspExtEventDesc(RCS_GET_ISCSI_INITIATORNAME_REQ);
    }
public:
    void SetNruID(const UUIDString& strNruID)
    {
        nruID = strNruID;
    }
    UUIDString& GetNruID()
    {
        return nruID;
    }
    const UUIDString& GetNruID() const
    {
        return nruID;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CRcsGetIscsiInitatorNameRsp : public CEventRsp
{
private:
    string  InitatorName;        

public:
    CRcsGetIscsiInitatorNameRsp()
    {
        event = OspExtEventDesc(RCS_GET_ISCSI_INITIATORNAME_RSP);
    }
public:
    void SetInitatorName(const string& strInitatorName)
    {
        InitatorName = strInitatorName;
    }
    string& GetInitatorName()
    {
        return InitatorName;
    }
    const string& GetInitatorName() const
    {
        return InitatorName;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CRcsTaskStartReq : public CEventReq
{
public:
    CRcsTaskStartReq()
    {
        event = OspExtEventDesc(RCS_TASK_START_REQ);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
class CRcsTaskStartRsp : public CEventRsp
{
public:
    CRcsTaskStartRsp()
    {
        event = OspExtEventDesc(RCS_TASK_START_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CRcsGetUsedOtherStorageSizeReq : public CEventReq
{
public:
	CRcsGetUsedOtherStorageSizeReq()
	{
		event = OspExtEventDesc(RCS_GET_USED_OTHER_STORAGE_SIZE_REQ);
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};
class CRcsGetUsedOtherStorageSizeRsp : public CEventRsp
{
public:
	CRcsGetUsedOtherStorageSizeRsp()
	{
		event = OspExtEventDesc(RCS_GET_USED_OTHER_STORAGE_SIZE_RSP);
	}

public:
	void SetUsedSize(const unsigned int var)
	{
		usedSize = var;
	}
	unsigned int GetUsedSize()
	{
		return usedSize;
	}
	const unsigned int GetUsedSize() const
	{
		return usedSize;
	}

private:
	unsigned int usedSize; //单位：T

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

#endif //__RCS_PROTO_H__










