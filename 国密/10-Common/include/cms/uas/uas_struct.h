#ifndef _UAS_STRUCT_H_
#define _UAS_STRUCT_H_

#include "kdvtype.h"
#include "cms/cmsassert.h"
#include "cms/utility/uuid.h"
#include "cms/cms_proto.h"
#include "cms/cms_const.h"
#include <string>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
using std::string;
using std::vector;
using std::set;
using std::map;

class TiXmlDocument;
class TiXmlNode;

enum ENUasResourceTypes
{
    UasResourceCuserCustomData = 1,         // Cu�û��Զ�������
    UasResourceVideosourceBasiccfg  = 2,    // ��ƵԴ��������
    UasResourceVideosourcePresetcfg   = 3,  // ��ƵԴԤ��λ����
};
// UAC��¼����չ��Ϣ
struct TLoginExternInfo4Uas
{
public:
    set<int> resourceTypes; // ��ע����Դ���ͣ�UAS�ڲ���һЩ���ֻ�������עʱ��֪ͨ����ֵ����ΪENUasResourceTypes
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

enum TOperateType
{
    enUserOpTypeInvalid = 0, 
    enUserOpTypeLogin = 1, 
    enUserOpTypeLiveStream = 2, 
    enUserOpTypePlatformRecord = 3, // ����
    enUserOpTypePuRecord = 4,       // ����
    enUserOpTypeTVWall = 5, 
    enUserOpTypeCall = 6, 
    enUserOpTypeUserManage = 7, 
    enUserOpTypeNRUManage =8, 
    enUserOpTypeMapConfig = 9, 
    enUserOpTypePuConfig = 10, 
    enUserOpTypePtzCtrl = 11,
    enUserOpTypeAlarmConfig = 12,
    enUserOpTypeSysParamConfig = 13,
    enUserOpTypeRecordStart = 15,
    enUserOpTypeRecordStop  = 16,
    enUserOpTypeRecordPlayback = 17,
    enUserOpTypeRecordDownload = 18,
    enUserOpTypeCuLocalRecord  = 19,
    enUserOpTypeVideoPlay = 20, // ʵʱ���

    enUserOpTypeOther = 14
};

//�豸��־����
enum EDeviceLogType
{
    enDeviceLogTypeInvalid = 0,	             // ��Чֵ.
    enDeviceLogTypeOnline,		             // ע������.
    enDeviceLogTypeOffline,	                 // ����.
    enDeviceLogTypePuException,		         // �豸�쳣. 1.0������������ƽ̨�豸�쳣��ǰ���豸�쳣. 2.0��û��ƽ̨�豸��..
    enDeviceLogTypePlatformRecordException,	 // ƽ̨¼���쳣.
    enDeviceLogTypePuRecordToPlatform,	     // ƽ̨¼��ת��.
    enDeviceLogTypeOther,                    // ����
};


struct TOperateLogInfo
{
    UUIDString logId;     // ��־���
    UUIDString logTag;    // ��־��ǩ(��������ͬһ�û��Ĳ�ͬ�ε�¼��־��������Ӧ������)
    string userUri;       // �û�Uri
    string userName;      // �û���
    string devUri;        // �豸Uri(�������豸devUri��������ʱ��д��)
    string happenedTime;  // ����ʱ��
    int operateType;      // ��������
    int operateResult;    // �������
    string netAddr;       // �����ַ(�û���¼��IP��ַ��Ϣ)
    string operateDesc;   // ��������

public:
	TOperateLogInfo()
	{
		operateType   = enUserOpTypeInvalid;
		operateResult = 0;
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class COperateLogInfoQueryCondition
{
public:
    bool GetLogTag(UUIDString &val) const
    {
        if (!optionField.HasOption("logTag"))
            return false;
        val = logTag;
        return true;
    }
    void SetLogTag(const UUIDString &val)
    {
        logTag = val;
        optionField.SetOption("logTag");
    }
    bool GetUserUri(string &val) const
    {
        if (!optionField.HasOption("userUri"))
            return false;
        val = userUri;
        return true;
    }
    void SetUserUri(const string &val)
    {
        userUri = val;
        optionField.SetOption("userUri");
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
    bool GetHappenedTimeBegin(string &val) const
    {
        if (!optionField.HasOption("happenedTimeBegin"))
            return false;
        val = happenedTimeBegin;
        return true;
    }
    void SetHappenedTimeBegin(const string &val)
    {
        happenedTimeBegin = val;
        optionField.SetOption("happenedTimeBegin");
    }
    bool GetHappenedTimeEnd(string &val) const
    {
        if (!optionField.HasOption("happenedTimeEnd"))
            return false;
        val = happenedTimeEnd;
        return true;
    }
    void SetHappenedTimeEnd(const string &val)
    {
        happenedTimeEnd = val;
        optionField.SetOption("happenedTimeEnd");
    }
    bool GetOperateType(int &val) const
    {
        if (!optionField.HasOption("operateType"))
            return false;
        val = operateType;
        return true;
    }
    void SetOperateType(int val)
    {
        operateType = val;
        optionField.SetOption("operateType");
    }
private:
    CQryOptionField optionField;

    UUIDString logTag;
    string userUri;
    string devUri;
    string domainName;
    string happenedTimeBegin;
    string happenedTimeEnd;
    int operateType;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

struct TDeviceLogInfo
{
    UUIDString logId;       // ��־���
    string devUri;          // �豸Uri
    string devName;         // �豸����
    string devAlias;        // �豸����
    string happenedTime;    // ����ʱ��
    int logType;            // ��־����
    string netAddr;         // �����ַ(�豸��IP��ַ��Ϣ)
    string logDesc;         // ��־����

public:
	TDeviceLogInfo()
	{
		logType = 0;
	}

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDeviceLogInfoQueryCondition
{
public:
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
    bool GetDevDomainName(string &val) const
    {
        if (!optionField.HasOption("devDomainName"))
            return false;
        val = devDomainName;
        return true;
    }
    void SetDevDomainName(const string &val)
    {
        devDomainName = val;
        optionField.SetOption("devDomainName");
    }
    bool GetDevName(string &val) const
    {
        if (!optionField.HasOption("devName"))
            return false;
        val = devName;
        return true;
    }
    void SetDevName(const string &val)
    {
        devName = val;
        optionField.SetOption("devName");
    }
    bool GetDevAlias(string &val) const
    {
        if (!optionField.HasOption("devAlias"))
            return false;
        val = devAlias;
        return true;
    }
    void SetDevAlias(const string &val)
    {
        devAlias = val;
        optionField.SetOption("devAlias");
    }
    bool GetHappenedTimeBegin(string &val) const
    {
        if (!optionField.HasOption("happenedTimeBegin"))
            return false;
        val = happenedTimeBegin;
        return true;
    }
    void SetHappenedTimeBegin(const string &val)
    {
        happenedTimeBegin = val;
        optionField.SetOption("happenedTimeBegin");
    }
    bool GetHappenedTimeEnd(string &val) const
    {
        if (!optionField.HasOption("happenedTimeEnd"))
            return false;
        val = happenedTimeEnd;
        return true;
    }
    void SetHappenedTimeEnd(const string &val)
    {
        happenedTimeEnd = val;
        optionField.SetOption("happenedTimeEnd");
    }
    bool GetLogType(int &val) const
    {
        if (!optionField.HasOption("logType"))
            return false;
        val = logType;
        return true;
    }
    void SetLogType(int val)
    {
        logType = val;
        optionField.SetOption("logType");
    }
private:
    CQryOptionField optionField;

    string devUri;
    string devDomainName;
    string devName;
    string devAlias;
    string happenedTimeBegin;
    string happenedTimeEnd;
    int logType;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

struct TAlarmLogInfo
{
    UUIDString logId;       // ��־���
    string devUri;          // �豸Uri
    string devName;         // �豸����
    string devAlias;        // �豸����
    int channId;            // �澯ͨ��
    string type;            // �澯���� ����cms_const.h�е�"�澯��س���"
    string flag;            // �澯��־ ����cms_const.h�е�"�澯��س���"
    string occurTime;       // �澯ʱ��
    string resumeTime;      // �ָ�ʱ��
    string alias;           // �澯����
    string desc;            // �澯����

public:
	TAlarmLogInfo()
	{
		channId = 0;
        resumeTime = ALARM_RESUME_TIME_INVALIDVAL;
	}

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CAlarmLogInfoQueryCondition
{
public:
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
    bool GetDevDomainName(string &val) const
    {
        if (!optionField.HasOption("devDomainName"))
            return false;
        val = devDomainName;
        return true;
    }
    void SetDevDomainName(const string &val)
    {
        devDomainName = val;
        optionField.SetOption("devDomainName");
    }
    bool GetChannelId(int &val) const
    {
        if (!optionField.HasOption("channelId"))
            return false;
        val = channelId;
        return true;
    }
    void SetChannelId(int val)
    {
        channelId = val;
        optionField.SetOption("channelId");
    }
    bool GetHappenedTimeBegin(string &val) const
    {
        if (!optionField.HasOption("happenedTimeBegin"))
            return false;
        val = happenedTimeBegin;
        return true;
    }
    void SetHappenedTimeBegin(const string &val)
    {
        happenedTimeBegin = val;
        optionField.SetOption("happenedTimeBegin");
    }
    bool GetHappenedTimeEnd(string &val) const
    {
        if (!optionField.HasOption("happenedTimeEnd"))
            return false;
        val = happenedTimeEnd;
        return true;
    }
    void SetHappenedTimeEnd(const string &val)
    {
        happenedTimeEnd = val;
        optionField.SetOption("happenedTimeEnd");
    }
    bool GetType(string &val) const
    {
        if (!optionField.HasOption("type"))
            return false;
        val = type;
        return true;
    }
    void SetType(string val)
    {
        type = val;
        optionField.SetOption("type");
    }
    bool GetFlag(string &val) const
    {
        if (!optionField.HasOption("flag"))
            return false;
        val = flag;
        return true;
    }
    void SetFlag(string val)
    {
        flag = val;
        optionField.SetOption("flag");
    }
    bool GetAlias(string &val) const
    {
        if (!optionField.HasOption("alias"))
            return false;
        val = alias;
        return true;
    }
    void SetAlias(const string &val)
    {
        alias = val;
        optionField.SetOption("alias");
    }
private:
    CQryOptionField optionField;

    UUIDString devUri;
    string devDomainName;
    int channelId;
    string happenedTimeBegin; // happenedTimeBegin��happenedTimeEnd�������ֶ���Ҫͬʱָ������ͬʱ��ָ��
    string happenedTimeEnd;
    string type;
    string flag;
    string alias;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

struct TUserCustomDataInfo
{
    UUIDString userId;     // �û����
    int dataType;          // ��������
    string dataId;         // ���ݱ�ʶ
    int  currentFragNo;    // ��Ƭ���
    string  fragData;      // ��Ƭ����

public:
	TUserCustomDataInfo()
	{
		dataType = 0;
		currentFragNo =0;
	}

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUserCustomDataInfoQueryCondition
{
public:
    bool GetUserId(UUIDString &val) const
    {
        if (!optionField.HasOption("userId"))
            return false;
        val = userId;
        return true;
    }
    void SetUserId(const UUIDString &val)
    {
        userId = val;
        optionField.SetOption("userId");
    }
    bool GetDataType(int &val) const
    {
        if (!optionField.HasOption("dataType"))
            return false;
        val = dataType;
        return true;
    }
    void SetDataType(int val)
    {
        dataType = val;
        optionField.SetOption("dataType");
    }
    bool GetDataId(string &val) const
    {
        if (!optionField.HasOption("dataId"))
            return false;
        val = dataId;
        return true;
    }
    void SetDataId(const string &val)
    {
        dataId = val;
        optionField.SetOption("dataId");
    }
    void SetFragNo(int nFragNo)
    {
        fragNo = nFragNo;
		optionField.SetOption("fragNo");
    }
    bool GetFragNo(int &val) const
    {
		if (!optionField.HasOption("fragNo"))
			return false;
		val = fragNo;
		return true;
    }

private:
    CQryOptionField optionField;

    UUIDString userId;     // �û����
    int dataType;          // ��������
    string dataId;         // ���ݱ�ʶ
    int  fragNo;           // ��Ƭ��� -1��ʾ����

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUserCustomDataInfoKey
{
private:
    UUIDString userId;                  // �û����
    int dataType;                       // ��������
    string dataId;                      // ���ݱ�ʶ
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

    void SetDataType(int nDataType)
    {
        dataType = nDataType;
    }
    int GetDataType() const
    {
        return dataType;
    }

    void SetDataId(const string& strDataId)
    {
        dataId = strDataId;
    }
    string& GetDataId()
    {
        return dataId;
    }
    const string& GetDataId() const
    {
        return dataId;
    }

};

struct TVideosourceBasicCfgInfo
{
    string devUri;          // �豸Uri
    int videosourceId;      // ��ƵԴ��
    int directionFlag;      // ��������

public:
	TVideosourceBasicCfgInfo()
	{
		videosourceId = 0;
		directionFlag = 0;
	}

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CVideosourceBasicCfgInfoQueryCondition
{
public:
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

private:
    CQryOptionField optionField;

    string devUri; 
    int videosourceId;

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CVideosourceBasicCfgInfoKey
{
private:
    string devUri;               
    int videosourceId;                 
public:
    void SetDevUri(const string& strDevId)
    {
        devUri = strDevId;
    }
    string& GetDevUri()
    {
        return devUri;
    }
    const string& GetDevUri() const
    {
        return devUri;
    }

    void SetVideosourceId(int nVideosourceId)
    {
        videosourceId = nVideosourceId;
    }
    int GetVideosourceId() const
    {
        return videosourceId;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

struct TPresetPositionName
{
    int posId;      // Ԥ��λ��
    string posName; // Ԥ��λ����

public:
	TPresetPositionName()
	{
		posId = 0;
	}

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
struct TVideosourcePresetCfgInfo
{
    string devUri;          // �豸Uri
    int videosourceId;      // ��ƵԴ��
    vector<TPresetPositionName> posNames; // Ԥ��λ����

public:
	TVideosourcePresetCfgInfo()
	{
		videosourceId = 0;
	}

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CVideosourcePresetCfgInfoQueryCondition
{
public:
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
    CQryOptionField optionField;

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

class CVideosourcePresetCfgInfoKey
{
private:
    string devUri;              
    int videosourceId;                 
    int positionId;                    
public:
    void SetDevUri(const string& strDevId)
    {
        devUri = strDevId;
    }
    string& GetDevUri()
    {
        return devUri;
    }
    const string& GetDevUri() const
    {
        return devUri;
    }

    void SetVideosourceId(int nVideosourceId)
    {
        videosourceId = nVideosourceId;
    }
    int GetVideosourceId() const
    {
        return videosourceId;
    }

    void SetPositionId(int nPositionId)
    {
        positionId = nPositionId;
    }
    int GetPositionId() const
    {
        return positionId;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

#endif // _UAS_STRUCT_H_





