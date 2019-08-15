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
    UasResourceCuserCustomData = 1,         // Cu用户自定义数据
    UasResourceVideosourceBasiccfg  = 2,    // 视频源基本配置
    UasResourceVideosourcePresetcfg   = 3,  // 视频源预置位配置
};
// UAC登录的扩展信息
struct TLoginExternInfo4Uas
{
public:
    set<int> resourceTypes; // 关注的资源类型，UAS内部的一些变更只有外设关注时才通知。数值类型为ENUasResourceTypes
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
    enUserOpTypePlatformRecord = 3, // 废弃
    enUserOpTypePuRecord = 4,       // 废弃
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
    enUserOpTypeVideoPlay = 20, // 实时浏览

    enUserOpTypeOther = 14
};

//设备日志类型
enum EDeviceLogType
{
    enDeviceLogTypeInvalid = 0,	             // 无效值.
    enDeviceLogTypeOnline,		             // 注册上线.
    enDeviceLogTypeOffline,	                 // 下线.
    enDeviceLogTypePuException,		         // 设备异常. 1.0和需求中区分平台设备异常和前端设备异常. 2.0里没有平台设备了..
    enDeviceLogTypePlatformRecordException,	 // 平台录像异常.
    enDeviceLogTypePuRecordToPlatform,	     // 平台录像转存.
    enDeviceLogTypeOther,                    // 其他
};


struct TOperateLogInfo
{
    UUIDString logId;     // 日志编号
    UUIDString logTag;    // 日志标签(可以满足同一用户的不同次登录日志区别开来的应用需求)
    string userUri;       // 用户Uri
    string userName;      // 用户名
    string devUri;        // 设备Uri(关联的设备devUri，不关联时填写空)
    string happenedTime;  // 发生时间
    int operateType;      // 操作类型
    int operateResult;    // 操作结果
    string netAddr;       // 网络地址(用户登录的IP地址信息)
    string operateDesc;   // 操作描述

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
    UUIDString logId;       // 日志编号
    string devUri;          // 设备Uri
    string devName;         // 设备名称
    string devAlias;        // 设备别名
    string happenedTime;    // 发生时间
    int logType;            // 日志类型
    string netAddr;         // 网络地址(设备的IP地址信息)
    string logDesc;         // 日志描述

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
    UUIDString logId;       // 日志编号
    string devUri;          // 设备Uri
    string devName;         // 设备名称
    string devAlias;        // 设备别名
    int channId;            // 告警通道
    string type;            // 告警类型 参照cms_const.h中的"告警相关常量"
    string flag;            // 告警标志 参照cms_const.h中的"告警相关常量"
    string occurTime;       // 告警时间
    string resumeTime;      // 恢复时间
    string alias;           // 告警别名
    string desc;            // 告警描述

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
    string happenedTimeBegin; // happenedTimeBegin与happenedTimeEnd这两个字段需要同时指定或者同时不指定
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
    UUIDString userId;     // 用户编号
    int dataType;          // 数据类型
    string dataId;         // 数据标识
    int  currentFragNo;    // 分片序号
    string  fragData;      // 分片数据

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

    UUIDString userId;     // 用户编号
    int dataType;          // 数据类型
    string dataId;         // 数据标识
    int  fragNo;           // 分片序号 -1表示所有

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
    UUIDString userId;                  // 用户编号
    int dataType;                       // 数据类型
    string dataId;                      // 数据标识
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
    string devUri;          // 设备Uri
    int videosourceId;      // 视频源号
    int directionFlag;      // 方向配置

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
    int posId;      // 预置位号
    string posName; // 预置位别名

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
    string devUri;          // 设备Uri
    int videosourceId;      // 视频源号
    vector<TPresetPositionName> posNames; // 预置位别名

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





