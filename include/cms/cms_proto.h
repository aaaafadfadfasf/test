/*****************************************************************************
   ģ����      : cms_proto
   �ļ���      : cms_proto.h
   ����ļ�    : 
   �ļ�ʵ�ֹ���: cms_proto.h ����CMSϵͳ������Ϣ��Ļ���
   ����        : liangli
   �汾        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
   2011/10/22   1.0         liangli         ����
   2012/05/25   1.0         huzhiyun        �����������޸��ֶα�ʶ��Ͳ�ѯҵ�����
   2012/06/20   1.2         huangzhichun    ���Ӷ��Ĺ���Э����
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
// ע�͵���ospsip.h���ã��Ա�ĳЩ��ʹ��OSP��SIP��ģ�鲻��Ӱ�� 
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
        //event�ͽṹ���ǰ����ģ��ʲ������·���
        seqNum = GenSequenceNum();  //���·�����ˮ��
        session.clear();
    }

    //��req��ȡ��Ϣͷ
    void SetHeadFromReq(const CEventReq& cEvent)
    {
        session = cEvent.GetSession();
        seqNum = cEvent.GetSeqNum();
    }

    //HZC:��req��ȡ��Ϣͷֱ���滻xml������ֶΣ������������ά��
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
        //event�ͽṹ���ǰ����ģ��ʲ������·���
        seqNum = 0;
        session.clear();
        errorCode = 0;
    }

    //��req��ȡ��Ϣͷ
    void SetHeadFromReq(const CEventReq& cEvent)
    {
        session = cEvent.GetSession();
        seqNum = cEvent.GetSeqNum();
    }

    //HZC:��req��ȡ��Ϣͷֱ���滻xml������ֶΣ������������ά��
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

//ע�⣡CNtfRsp��Ϊ���ಢû�г�ʼ����Ϣ��, ��������CNtfRsp�ظ�NtfReqʱ�������ֶ�����RSP��Ϣ��
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
//��ѯ��ػ���
///////////////////////////////////////////////////////////////////////////////////////////////////////

// �޸���Ч�ֶα�ʶ��
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

// ��ѯ��Ч�ֶα�ʶ��
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

// ��ѯ�����ֶα�ʶ��
typedef std::string TQueryOrderFieldId;
struct TQueryOrderField
{
	TQueryOrderField(){asc = true;}
	bool asc;                     // ��0��ʾ����
	TQueryOrderFieldId  columnId; // �����б�ʶ, ����ҵ���岻ͬ�ı�ʶ�ֶ�

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
protected:
	void ToXml(TiXmlDocument& xmlDoc)const;
public:
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

// ��ѯ�������
// hzytodo2 ��ʱ��100000��Ϊ300000.�����ֵ����̫��,����������ˣ�
// ���β�ѯ��ʱ����ܻ�ܳ�����Դ����Ҳ��ܴ�ͬʱ���ܵ����ڲ�osp������������
const unsigned int ENTRY_MAX_NUM_ONE_PAGE = 300000; // ��������һҳ��������¼�������� 20160510 ��ʱ��Ϊ30��
const unsigned int ENTRY_MAX_NUM_ONE_PAGE_SPECIAL = 2000000; //������ķ�������¼������Ϊ200W
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
	int  startEntryIndex; // ��ʾ�ӵ�startEntryIndex������ʼ��ȡ��startEntryIndex��0��ʼ������Ϊ0ʱCQueryRspBase��ͬʱ���ط��������ļ�¼����
	int  expectEntryNum;  // ��ʾ�����õ��Ĳ�ѯ��¼������0��ʾ��ѯ�߲����Ʒ��صļ�¼�����������һҳ�����ѯ����������¼��ΪENTRY_MAX_NUM_ONE_PAGE
	UUIDString queryTag;  // ��ѯ��Ϊ��ǩ����ѯ������������ʶһ����ѯ�����׵�Rsp��NtfReq��������ͬ�ı�ʶ
	vector<TQueryOrderField> orderFields;// ��������ֶ�(�����еĵ�0��Ԫ����Ϊ��һ�����ֶΣ���1��Ԫ����Ϊ�ڶ������ֶΣ���������)
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
protected:
	void ToXml(TiXmlDocument& xmlDoc)const;
public:
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

// ��ѯӦ�����
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
	int totalEntryNum;    // ���ϲ�ѯ�����ļ�¼������ ֻ��CQueryReqBase�е�startEntryIndexΪ0��Ҳ����һҳ��ʱ����Ч, -1��ʾ��Ч
	int entryNum;         // ��ҳ���ݵļ�¼��Ŀ��Ҳ�����׵�NtfReq�еļ�¼��֮��
	// ��ʱ��ʵ�� string datasourceVer; // ����Դ�汾������ѯ���ݱ仯ʱ���ֶ�Ҳ��֮�仯�������ѯ������Ҫ���������еķ�ҳ��ѯ�и��ֶ�ֵ��ͬ��������Ҫ���²�ѯ
	UUIDString queryTag;  // ��ѯ��Ϊ��ǩ������CQueryReqBase�е�queryTag
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
protected:
	void ToXml(TiXmlDocument& xmlDoc)const;
public:
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

// ��ѯNtfReq����
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
	int entryNum;       // ��Ntf�м�¼����Ŀ
	bool lastNtf;       // ��Ntf�Ƿ�Ϊ��ҳ���ݵ����һ��Ntf
	UUIDString queryTag; // ��ѯ��Ϊ��ǩ������CQueryReqBase�е�queryTag
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
//������ع���Э����
//
//˵����
//      1��ˢ�¶����У����TSsOpTypeΪSS_OP_TYPE_DEL��������Ҫˢ�µ��豸�����Ͳ�û�ж��ģ���Ϊ�ɹ�
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


// �����б�keyΪ�豸uri��valueΪ��������
typedef map<string, TSsTypes> TSsDevList;

// ����ʧ���б�keyΪ�豸uri��valueΪ������
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
	string          userUri;	//������
    string          devUri;     //��������
    TSsTypes        ssTypes;    //���ĵ�״̬����
	bool            batchNotify;//��������֪ͨ
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
    TSsOpType ssOpType;   //ˢ�²�������
    TSsTypes  ssTypes;    //���ĵ�״̬����
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
    string userUri;     //������
    string devUri;      //��������
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

typedef CEventRsp CUnSsRsp;

// cmu���嶩���ع�ʱ�޸���CSubscribeReq�ȵ����֣�ͨ��typedef��������ǰ�Ĵ���
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
	TSsOpType  ssOpType;   // ˢ�²������ͣ�Լ�������������豸��TSsTypes����պ�Ӷ����豸�б���������豸��
	TSsDevList ssDevList; // ��Ҫˢ�µ��豸�б�
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
	string   userUri;				   //������
	string   domain;                   //��Ҫ���ĵ�����������������ʽ�����嶩��
	TSsTypes ssTypes;                  //���ĵ�״̬����
	bool     batchNotify;			   //��������֪ͨ�ı�־
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
	TSsOpType       ssOpType;                      //ˢ�²������ͣ���ӻ�ɾ����SS_OPE_ADD��SS_OPE_ERASE
	TSsTypes        ssTypes;                       //����ˢ�µ�״̬����
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






