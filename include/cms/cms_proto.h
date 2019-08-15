/*****************************************************************************
   模块名      : cms_proto
   文件名      : cms_proto.h
   相关文件    : 
   文件实现功能: cms_proto.h 定义CMS系统所有消息体的基类
   作者        : liangli
   版本        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人      修改内容
   2011/10/22   1.0         liangli         创建
   2012/05/25   1.0         huzhiyun        提炼公共的修改字段标识类和查询业务基类
   2012/06/20   1.2         huangzhichun    增加订阅公共协议类
******************************************************************************/
#ifndef _CMS_PROTO_H_
#define _CMS_PROTO_H_
#include <string>
#include <vector>
#include <set>
#include <algorithm>
using std::string;
using std::vector;
using std::set;
/*****************************************************************************/
// 注释掉对ospsip.h引用，以便某些不使用OSP和SIP的模块不受影响 
/*****************************************************************************/
#include "cms/ospext/ospext.h"
#include "system/tinyxml/xml.h"
#include "cms/utility/cmstime.h"
#include "cms/utility/cmsver.h"
#include "cms/utility/uuid.h"
#include "cms/cms_struct.h"


class CSipEvent
{
public:
    virtual ~CSipEvent(){}     
    virtual const std::string ToXml()const = 0;
    virtual void FromXml(const std::string& xml_string) = 0;
protected:
    virtual void ToXml(TiXmlDocument& xmlDoc)const = 0;
public:
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const = 0;
    virtual void FromXml(const TiXmlNode& xmlNode) = 0;
};

class CEventReq: public CSipEvent
{
public:
    CEventReq()
    {
        seqNum = GenSequenceNum();
    }
    const int GetSeqNum() const
    {
        return seqNum;
    }
    void SetSeqNum(const int dwSeqNum)
    {
        seqNum = dwSeqNum;
    }
    const std::string GetEventStr() const 
    {
        return event;
    }
    const int GetEventId() const
    {
        return OspExtEventID(event.c_str());
    }
    void SetEvent(const std::string strEvent)
    {
        event = strEvent;
    }
    void SetEvent(const u16 wEvent)
    {
        event = OspExtEventDesc(wEvent);
    }
    const std::string GetSession() const
    {
        return session;
    }
    void SetSession(const std::string strSession)
    {
        session = strSession;
    }
    void Clear()
    {
        //event和结构体是绑定死的，故不用重新分配
        seqNum = GenSequenceNum();  //重新分配流水号
        session.clear();
    }

    //从req获取消息头
    void SetHeadFromReq(const CEventReq& cEvent)
    {
        session = cEvent.GetSession();
        seqNum = cEvent.GetSeqNum();
    }

    //HZC:从req获取消息头直接替换xml中相关字段，放在这里便于维护
    static void SetHeadFromReq(string& strXmlVar, const CEventReq& cEvent)
    {
        strXmlVar = ReplaceXmlKeyText(strXmlVar, cEvent.GetSession(), "<session>", "</session>");
        strXmlVar = ReplaceXmlKeyText(strXmlVar, cEvent.GetSession(), "<seqNum>", "</seqNum>");
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
protected:
    std::string event;
    int seqNum;
    std::string session;
};

class CEventRsp: public CSipEvent
{
public:
    CEventRsp()
    {
        seqNum = 0;
        errorCode = 0;
    }
    const int GetSeqNum() const
    {
        return seqNum;
    }
    void SetSeqNum(const int dwSeqNum)
    {
        seqNum = dwSeqNum;
    }
    const std::string GetEventStr() const 
    {
        return event;
    }
    const int GetEventId() const
    {
        return OspExtEventID(event.c_str());
    }
    void SetEvent(const std::string strEvent)
    {
        event = strEvent;
    }
    void SetEvent(const u16 wEvent)
    {
        event = OspExtEventDesc(wEvent);
    }
    const std::string GetSession() const
    {
        return session;
    }
    void SetSession(const std::string strSession)
    {
        session = strSession;
    }
    const int GetErrorCode() const
    {
        return errorCode;
    }
    void SetErrorCode(const int dwErrorCode) 
    {
        errorCode = dwErrorCode;
    }
    void Clear()
    {
        //event和结构体是绑定死的，故不用重新分配
        seqNum = 0;
        session.clear();
        errorCode = 0;
    }

    //从req获取消息头
    void SetHeadFromReq(const CEventReq& cEvent)
    {
        session = cEvent.GetSession();
        seqNum = cEvent.GetSeqNum();
    }

    //HZC:从req获取消息头直接替换xml中相关字段，放在这里便于维护
    static string& SetHeadFromReq(string& strXmlVar, const CEventReq& cEvent)
    {
        //strXmlVar = ReplaceXmlKeyText(strXmlVar, cEvent.GetSession(), "<session>", "</session>");
        //strXmlVar = ReplaceXmlKeyText(strXmlVar, cEvent.GetSeqNum(), "<seqNum>", "</seqNum>");

        return strXmlVar;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
protected:
    std::string event;
    int seqNum;
    std::string session;
    int errorCode;
};

//注意！CNtfRsp作为基类并没有初始化消息号, 所以在用CNtfRsp回复NtfReq时，必须手动设置RSP消息号
class CNtfRsp: public CEventRsp
{
public:
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////
//查询相关基类
///////////////////////////////////////////////////////////////////////////////////////////////////////

// 修改有效字段标识类
class CModOptionField
{
public:
	CModOptionField(){}
	void SetOption(const string &opt)
	{
		if (fields.end() == find(fields.begin(), fields.end(), opt))
			fields.push_back(opt);
	}
	bool HasOption(const string &opt) const
	{
		return fields.end() != find(fields.begin(), fields.end(), opt);
	}
private:
	vector<string> fields;
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
protected:
	void ToXml(TiXmlDocument& xmlDoc)const;
public:
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

// 查询有效字段标识类
class CQryOptionField
{
public:
	CQryOptionField(){}
	void SetOption(const string &opt)
	{
		if (fields.end() == find(fields.begin(), fields.end(), opt))
			fields.push_back(opt);
	}
	bool HasOption(const string &opt) const
	{
		return fields.end() != find(fields.begin(), fields.end(), opt);
	}
private:
	vector<string> fields;
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
protected:
	void ToXml(TiXmlDocument& xmlDoc)const;
public:
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

// 查询排序字段标识类
typedef std::string TQueryOrderFieldId;
struct TQueryOrderField
{
	TQueryOrderField(){asc = true;}
	bool asc;                     // 非0表示升序
	TQueryOrderFieldId  columnId; // 排序列标识, 根据业务定义不同的标识字段

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
protected:
	void ToXml(TiXmlDocument& xmlDoc)const;
public:
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

// 查询请求基类
// hzytodo2 临时由100000改为300000.这个数值不能太大,否则服务器端，
// 单次查询的时间可能会很长和资源开销也会很大，同时可能导致内部osp队列满的问题
const unsigned int ENTRY_MAX_NUM_ONE_PAGE = 300000; // 服务器端一页数据最大记录条数限制 20160510 临时改为30万
const unsigned int ENTRY_MAX_NUM_ONE_PAGE_SPECIAL = 2000000; //对特殊的服务，最大记录数调整为200W
class CQueryReqBase: public CEventReq
{
public:
	CQueryReqBase()
	{
		startEntryIndex = 0;
		expectEntryNum = 0;
		CUUID cUUID;
		queryTag = cUUID;
	}
	int GetStartEntryIndex()
	{
		return startEntryIndex;
	}
	int GetStartEntryIndex() const
	{
		return startEntryIndex;
	}
	void SetStartEntryIndex(int val)
	{
		startEntryIndex = val;
	}
	int GetExpectEntryNum()
	{
		return expectEntryNum;
	}
	int GetExpectEntryNum() const
	{
		return expectEntryNum;
	}
	void SetExpectEntryNum(int val)
	{
		expectEntryNum = val;
	}
	const UUIDString& GetQueryTag() const
	{
		return queryTag;
	}
	UUIDString& GetQueryTag()
	{
		return queryTag;
	}
	void SetQueryTag(const UUIDString& val)
	{
		queryTag = val;
	}
	vector<TQueryOrderField>& GetOrderField()
	{
		return orderFields;
	}
	const vector<TQueryOrderField>& GetOrderField() const
	{
		return orderFields;
	}
	void SetOrderField(const vector<TQueryOrderField> &orderFields_)
	{
		orderFields = orderFields_;
	}
private:
	int  startEntryIndex; // 表示从第startEntryIndex条数开始获取，startEntryIndex从0开始，当它为0时CQueryRspBase中同时返回符合条件的记录总数
	int  expectEntryNum;  // 表示期望得到的查询记录总数，0表示查询者不限制返回的记录总数；服务端一页允许查询出来的最大记录数为ENTRY_MAX_NUM_ONE_PAGE
	UUIDString queryTag;  // 查询行为标签，查询者利用它来标识一个查询，配套的Rsp和NtfReq都带上相同的标识
	vector<TQueryOrderField> orderFields;// 结果排序字段(数组中的第0个元素作为第一排序字段，第1个元素作为第二排序字段，依此类推)
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
protected:
	void ToXml(TiXmlDocument& xmlDoc)const;
public:
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

// 查询应答基类
class CQueryRspBase: public CEventRsp
{
public:
	CQueryRspBase()
	{
		totalEntryNum = -1;
		entryNum = 0;
	}
	int GetTotalEntryNum() 
	{
		return totalEntryNum;
	}
	int GetTotalEntryNum() const
	{
		return totalEntryNum;
	}
	void SetTotalEntryNum(int nTotalEntry)
	{
		totalEntryNum = nTotalEntry;
	}
	int GetEntryNum() 
	{
		return entryNum;
	}
	int GetEntryNum() const
	{
		return entryNum;
	}
	void SetEntryNum(int nEntryNum)
	{
		entryNum = nEntryNum;
	}
	const UUIDString& GetQueryTag() const
	{
		return queryTag;
	}
	UUIDString& GetQueryTag()
	{
		return queryTag;
	}
	void SetQueryTag(const UUIDString& val)
	{
		queryTag = val;
	}
private:
	int totalEntryNum;    // 符合查询条件的记录总数， 只有CQueryReqBase中的startEntryIndex为0（也即第一页）时才有效, -1表示无效
	int entryNum;         // 本页内容的记录数目，也即配套的NtfReq中的记录数之和
	// 暂时不实现 string datasourceVer; // 数据源版本，被查询数据变化时此字段也随之变化；整体查询过程中要求里面所有的分页查询中该字段值相同，否则需要重新查询
	UUIDString queryTag;  // 查询行为标签，来自CQueryReqBase中的queryTag
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
protected:
	void ToXml(TiXmlDocument& xmlDoc)const;
public:
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

// 查询NtfReq基类
class CQueryNtfReqBase: public CEventReq
{
public:
	CQueryNtfReqBase()
	{
		entryNum = 0;
		lastNtf = false;
	}
	int GetEntryNum() 
	{
		return entryNum;
	}
	int GetEntryNum() const
	{
		return entryNum;
	}
	void SetEntryNum(int val)
	{
		entryNum = val;
	}
	bool IsLastNtf() 
	{
		return lastNtf;
	}
	const bool IsLastNtf() const
	{
		return lastNtf;
	}
	void SetLastNtf(bool val)
	{
		lastNtf = val;
	}
	const UUIDString& GetQueryTag() const
	{
		return queryTag;
	}
	UUIDString& GetQueryTag()
	{
		return queryTag;
	}
	void SetQueryTag(const UUIDString& val)
	{
		queryTag = val;
	}
private:
	int entryNum;       // 本Ntf中记录的数目
	bool lastNtf;       // 本Ntf是否为本页数据的最后一个Ntf
	UUIDString queryTag; // 查询行为标签，来自CQueryReqBase中的queryTag
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
protected:
	void ToXml(TiXmlDocument& xmlDoc)const;
public:
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////
//订阅相关公共协议类
//
//说明：
//      1、刷新订阅中，如果TSsOpType为SS_OP_TYPE_DEL，但是需要刷新的设备或类型并没有订阅，视为成功
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////
typedef std::string TSsType;
typedef set<TSsType> TSsTypes;
#define  PU_ON_OFF_STATUS    "PU_ON_OFF"
#define  PU_CONFIG_STATUS    "PU_CONFIG"
#define  PU_SERVICE_STATUS   "PU_SERVICE"
#define  PU_ALARM_STATUS     "PU_ALARM"
#define  PU_GPS_STATUS       "PU_GPS"
#define  PU_TRANSDATA_STATUS "PU_TRANSDATA"

#define  PU_DEV_CONFIG_NTF   "PU_DEVCONFIG_NTF"

typedef string TSsOpType;
#define SS_OP_TYPE_ADD   "ADD"
#define SS_OP_TYPE_DEL   "DEL"

#define SS_OPE_ADD     SS_OP_TYPE_ADD
#define SS_OPE_ERASE   SS_OP_TYPE_DEL


// 订阅列表，key为设备uri，value为订阅类型
typedef map<string, TSsTypes> TSsDevList;

// 订阅失败列表，key为设备uri，value为错误码
typedef map<string, int> TSsErrorList;


class CSsReq : public CEventReq
{
public:
    CSsReq()
    {
		Clear();
    }

    void Clear()
    {
		userUri.clear();
        devUri.clear();
        ssTypes.clear();
		batchNotify = false;
    }

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

    const TSsTypes& GetSsTypes() const
    {
        return ssTypes;
    }
    TSsTypes& GetSsTypes()
    {
        return ssTypes;
    }
    void SetSsTypes(const TSsTypes& var)
    {
        ssTypes = var;
    }

	bool GetBatchNotify() const
	{
		return batchNotify;
	}
	void SetBatchNotify(bool bFlag)
	{
		batchNotify = bFlag;
	}

private:
	string          userUri;	//订阅者
    string          devUri;     //被订阅者
    TSsTypes        ssTypes;    //订阅的状态种类
	bool            batchNotify;//接收批量通知
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

typedef CEventRsp CSsRsp;

class CRefreshSsReq : public CEventReq
{
public:
    CRefreshSsReq()
    {
    }

    void Clear()
    {
        ssOpType.clear();
        ssTypes.clear();
    }

public:
    const TSsOpType& GetSsOpType() const
    {
        return ssOpType;
    }
    TSsOpType& GetSsOpType()
    {
        return ssOpType;
    }
    void SetSsOpType(const TSsOpType& var)
    {
        ssOpType = var;
    }

    const TSsTypes& GetSsTypes() const
    {
        return ssTypes;
    }
    TSsTypes& GetSsTypes()
    {
        return ssTypes;
    }
    void SetSsTypes(const TSsTypes& var)
    {
        ssTypes = var;
    }

private:
    TSsOpType ssOpType;   //刷新操作类型
    TSsTypes  ssTypes;    //订阅的状态种类
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

typedef CEventRsp CRefreshSsRsp;

class CUnSsReq : public CEventReq
{
public:
    CUnSsReq()
    {
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

private:
    string userUri;     //订阅者
    string devUri;      //被订阅者
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

typedef CEventRsp CUnSsRsp;

// cmu整体订阅重构时修改了CSubscribeReq等的名字，通过typedef来兼容以前的代码
typedef CSsReq CSubscribeReq;
typedef CSsRsp CSubscribeRsp;
typedef CUnSsReq CUnsubscribeReq;
typedef CUnSsRsp CUnsubscribeRsp;

class CListSsReq : public CEventReq
{
public:
	CListSsReq()
	{
		Clear();
	}

	void Clear()
	{
		userUri.clear();
		ssDevList.clear();
		batchNotify = false;
	}

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

	const TSsDevList& GetSsDevList() const
	{
		return ssDevList;
	}
	TSsDevList& GetSsDevList()
	{
		return ssDevList;
	}
	void SetSsDevList(const TSsDevList &tSsUnitList)
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

private:
	string     userUri;
	TSsDevList ssDevList;
	bool       batchNotify;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CListSsRsp : public CEventRsp
{
public:
	CListSsRsp()
	{

	}

	void Clear()
	{
		ssErrorList.clear();
	}

public:
	const TSsErrorList& GetSsErrorList() const
	{
		return ssErrorList;
	}
	TSsErrorList& GetSsErrorList()
	{
		return ssErrorList;
	}
	void SetSsErrorList(const TSsErrorList &tSsErrorList)
	{
		ssErrorList = tSsErrorList;
	}
	void SetSsErrorItem(const string &strDevUri, int enErrorCode)
	{
		ssErrorList[strDevUri] = enErrorCode;
	}

private:
	TSsErrorList ssErrorList;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CListRefreshSsReq : public CEventReq
{
public:
	CListRefreshSsReq()
	{
	}

	void Clear()
	{
		ssOpType.clear();
		ssDevList.clear();
	}

public:
	const TSsOpType& GetSsOpType() const
	{
		return ssOpType;
	}
	TSsOpType& GetSsOpType()
	{
		return ssOpType;
	}
	void SetSsOpType(const TSsOpType& var)
	{
		ssOpType = var;
	}

	const TSsDevList& GetSsDevList() const
	{
		return ssDevList;
	}
	TSsDevList& GetSsDevList()
	{
		return ssDevList;
	}
	void SetSsDevList(const TSsDevList &tSsUnitList)
	{
		ssDevList = tSsUnitList;
	}

private:
	TSsOpType  ssOpType;   // 刷新操作类型（约定：当被订阅设备的TSsTypes被清空后从订阅设备列表中清除该设备）
	TSsDevList ssDevList; // 需要刷新的设备列表
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CListRefreshSsRsp : public CEventRsp
{
public:
	CListRefreshSsRsp()
	{

	}

	void Clear()
	{
		ssErrorList.clear();
	}

public:
	const TSsErrorList& GetSsErrorList() const
	{
		return ssErrorList;
	}
	TSsErrorList& GetSsErrorList()
	{
		return ssErrorList;
	}
	void SetSsErrorList(const TSsErrorList &tSsErrorList)
	{
		ssErrorList = tSsErrorList;
	}
	void SetSsErrorItem(const string &strDevUri, int enErrorCode)
	{
		ssErrorList[strDevUri] = enErrorCode;
	}

private:
	TSsErrorList ssErrorList;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

typedef CEventReq CListUnSsReq;

typedef CEventRsp CListUnSsRsp;

class CCatalogSsReq : public CEventReq
{
public:
	CCatalogSsReq()
	{
		Clear();
	}

	void Clear()
	{
		userUri.clear();
		domain.clear();
		ssTypes.clear();
		batchNotify = false;
	}

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

	bool GetBatchNotify() const
	{
		return batchNotify;
	}
	void SetBatchNotify(bool bFlag)
	{
		batchNotify = bFlag;
	}

private:
	string   userUri;				   //订阅者
	string   domain;                   //需要订阅的域名，用于域名方式的整体订阅
	TSsTypes ssTypes;                  //订阅的状态种类
	bool     batchNotify;			   //接收批量通知的标志
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

typedef CEventRsp CCatalogSsRsp;

class CCatalogRefreshSsReq : public CEventReq
{
public:
	CCatalogRefreshSsReq()
	{
		
	}

	void Clear()
	{
		ssOpType.clear();
		ssTypes.clear();
	}

public:
	void SetSsOpType(const TSsOpType& strSsOpType)
	{
		ssOpType = strSsOpType;
	}
	TSsOpType& GetSsOpType()
	{
		return ssOpType;
	}
	const TSsOpType& GetSsOpType() const
	{
		return ssOpType;
	}

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

private:
	TSsOpType       ssOpType;                      //刷新操作类型：添加或删除，SS_OPE_ADD，SS_OPE_ERASE
	TSsTypes        ssTypes;                       //订阅刷新的状态种类
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

typedef CEventRsp CCatalogRefreshSsRsp;

typedef CEventReq CCatalogUnSsReq;

typedef CEventRsp CCatalogUnSsRsp;


#endif // _CMS_PROTO_H_






