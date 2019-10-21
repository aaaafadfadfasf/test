/*****************************************************************************
   模块名      : RCS
   文件名      : rcs_struct.h
   相关文件    : 
   文件实现功能: rcs_struct.h 定义所有与RCS交互的公共结构体
   作者        : huangzhichun
   版本        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人				修改内容
   2012/06/11   1.0        huangzhichun			创建
******************************************************************************/

#ifndef __RCS_STRUCT_H__
#define __RCS_STRUCT_H__

#include "../cms_struct.h"


#define CMS_ENUM_STR(x)		static const char* x = #x

typedef map<string,string> TFieldValueMap;        //查询的的条件字段，或 需要修改的字段

typedef string TRecLockState;
#define RCS_REC_LOCKED                "locked"
#define RCS_REC_UNLOCKED              "unlocked"
#define RCS_REC_UNKNOWN               "unknown"

//获取Field的值
inline bool GetFieldIntVar(const TFieldValueMap& fieldValueMap, const string key, int& var)
{
        TFieldValueMap::const_iterator itr = fieldValueMap.find(key);
        if ( itr!= fieldValueMap.end())
        {
            if ( sscanf( itr->second.c_str(), "%d", &var ) == 1 )
            {
                return true;
            }
        }

        return false;
}

inline bool GetFieldDoubleVar(const TFieldValueMap& fieldValueMap, const string key, double& var)
{
        TFieldValueMap::const_iterator itr = fieldValueMap.find(key);
        if ( itr!= fieldValueMap.end())
        {
            if ( sscanf( itr->second.c_str(), "%lf", &var ) == 1 )
            {
                return true;
            }
        }

        return false;
}

inline bool GetFieldStringVar(const TFieldValueMap& fieldValueMap, const string key, string& var)
{
        TFieldValueMap::const_iterator itr = fieldValueMap.find(key);
        if ( itr!= fieldValueMap.end())
        {
            var = itr->second;

            return true;
        }

        return false;
}

template<class T>
bool GetFieldObjVar(const TFieldValueMap& fieldValueMap, const string key, T& var)
{
        TFieldValueMap::const_iterator itr = fieldValueMap.find(key);
        if ( itr!= fieldValueMap.end())
        {
            var.FromXml(itr->second);

            return true;
        }

        return false;
}


//设置Field的值
inline void SetFieldIntVar(TFieldValueMap& fieldValueMap, const string key, int var)
{
        std::ostringstream oss;
        oss << var;

        fieldValueMap[key] = oss.str();
}

inline void SetFieldDoubleVar(TFieldValueMap& fieldValueMap, const string key, double var)
{
        std::ostringstream oss;
        oss << var;

        fieldValueMap[key] = oss.str();
}

inline void SetFieldStringVar(TFieldValueMap& fieldValueMap, const string key, const string& var)
{
        fieldValueMap[key] = var;
}

template<class T>
void SetFieldObjVar(TFieldValueMap& fieldValueMap, const string key, const T& var)
{
        fieldValueMap[key] = var.ToXml();
}

/////////////////////////////////////////////////////////////////////
//{{RMS录像机管理子系统相关接口
/////////////////////////////////////////////////////////////////////
typedef string TRecType;
typedef vector<TRecType> TRecTypeList;
CMS_ENUM_STR(RCS_RECTYPE_ALARM);                //告警录像
CMS_ENUM_STR(RCS_RECTYPE_HANDLE);               //手动录像
CMS_ENUM_STR(RCS_RECTYPE_PLAN);                 //计划录像，包括定时录像和周期录像
CMS_ENUM_STR(RCS_RECTYPE_UNKNOWN);

inline int GetRecTypePri(TRecType tRecType)
{
    if (tRecType == RCS_RECTYPE_ALARM)
    {
        return 1;
    }
    else if (tRecType == RCS_RECTYPE_HANDLE)
    {
        return 2;
    }
    else if (tRecType == RCS_RECTYPE_PLAN)
    {
        return 3;
    }
    else
    {
        return 255;
    }
}

typedef string TRecMode;
CMS_ENUM_STR(RCS_RECMODE_ALL);                   //既录视频又录音频
CMS_ENUM_STR(RCS_RECMODE_ONLYVIDEO);             //只录视频
CMS_ENUM_STR(RCS_RECMODE_ONLYKEYFRAME);          //只录关键帧
CMS_ENUM_STR(RCS_RECMODE_UNKNOWN);               //未知模式

typedef string TRecState;
CMS_ENUM_STR(RCS_RECSTATE_RECORD);               //录像
CMS_ENUM_STR(RCS_RECSTATE_PRERECORD);            //预录
CMS_ENUM_STR(RCS_RECSTATE_UNKNOWN);              //未知状态

typedef string TCoverPolicy;
CMS_ENUM_STR(RCS_COVERPOLICY_STOPRECORD);       //通道空间满停录像
CMS_ENUM_STR(RCS_COVERPOLICY_COVERALL);         //通道空间满覆盖所有录像
CMS_ENUM_STR(RCS_COVERPOLICY_COVERNORMAL);      //通道空间满覆盖普通录像
CMS_ENUM_STR(RCS_COVERPOLICY_UNKNOWN);

typedef string TNruType; // <设备类型>@<部署方式> 比如, KDM2801@platform platform为NRU部署于平台，否则NRU部署于阵列
inline bool IsNruTypeOnPlatform(const TNruType &tNruType)
{
    char *pPos = strstr((char*)tNruType.c_str(), "@platform");
    if (NULL == pPos)
    {
        return false;
    }
    else
    {
        return 0 == strcmp(pPos, "@platform");
    }
}

struct TPuChnBinding
{
private:
    UUIDString nruID;
    string diskMountPath;
    TChannelList puChnList;

public:
    TPuChnBinding()
    {
    }

    void Clear()
    {
        nruID.clear();
        diskMountPath.clear();
        puChnList.clear();
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

    //diskMountPath
    const string& GetDiskMountPath() const
    {
        return diskMountPath;
    }
    string& GetDiskMountPath()
    {
        return diskMountPath;
    }
    void SetDiskMountPath(const string& var)
    {
        diskMountPath = var;
    }

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
};

enum emRecStreamType
{
    REC_STREAM_TYPE_MAIN = 0,       //主码流
    REC_STREAM_TYPE_SUB1 = 1,       //辅流1
    REC_STREAM_TYPE_SUB2 = 2,       //辅流2
    REC_STREAM_TYPE_SUB3 = 3       //辅流3
};

struct TRecordParam
{
private:
	TChannel        puChn;
	TRecMode        recMode;            //录像模式：既录视频又录音频,只录视频,只录关键帧
    int             preRecDuration;     //预录时长。0 表示不预录。单位:s
    int             alarmRecDelayTime;  //告警恢复后，延迟停止录像时间。单位:s
    bool            isManualRecOn;      //注意：外部设置录像参数时此参数无效
    TCronTimeList   planTimeList;
    bool            isSupportRecStorageDuration;           //是否支持录像保留时长设置
    bool            isRecStorageDurationEnabled;            //当前是否启用录像时长设置
    int             recStorageDuration;                    //录像存储时长
    bool            isSupportChangeStreamType;      //是否支持码流切换
    int             recStreamType;      // 录像码流类型。0-主流，1-辅流1，2-辅流2，3-辅流3

public:
    TRecordParam()
    {
        Clear();
        recMode = RCS_RECMODE_ALL;
        recStreamType = REC_STREAM_TYPE_MAIN;
        isSupportChangeStreamType = true;
    }

    void Clear()
    {
		puChn.Clear();
        recMode.clear();
        planTimeList.clear();
        preRecDuration = 0;
        alarmRecDelayTime = 60;
        isManualRecOn = false;
        recStreamType = REC_STREAM_TYPE_MAIN;
        recStorageDuration = 0;
		isSupportRecStorageDuration = true;
		isRecStorageDurationEnabled = false;
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

    //recMode
    const TRecMode& GetRecMode() const
    {
        return recMode;
    }
    TRecMode& GetRecMode()
    {
        return recMode;
    }
    void SetRecMode(const TRecMode& var)
    {
        recMode = var;
    }

    //preRecDuration
    int GetPreRecDuration() const
    {
        return preRecDuration;
    }
    void SetPreRecDuration(int var)
    {
        preRecDuration = var;
    }

    //alarmRecDelayTime
    int GetAlarmRecDelayStopTime() const
    {
        return alarmRecDelayTime;
    }
    void SetAlarmRecDelayStopTime(int var)
    {
        alarmRecDelayTime = var;
    }

    //isManualRecOn
    bool GetIsManualRecOn() const
    {
        return isManualRecOn;
    }
    void SetIsManualRecOn(bool var)
    {
        isManualRecOn = var;
    }

    //planTimeList
    const TCronTimeList& GetPlanTimeList() const
    {
        return planTimeList;
    }
    TCronTimeList& GetPlanTimeList()
    {
        return planTimeList;
    }
    void SetPlanTimeList(const TCronTimeList& var)
    {
        planTimeList = var;
    }

    //recStreamType
    int GetRecStreamType() const
    {
        return recStreamType;
    }
    void SetRecStreamType(int var)
    {
        recStreamType = var;
    }
    //isSupportChangeStreamType
    bool GetIsSupportChangeStreamType() const
    {
        return isSupportChangeStreamType;
    }
    void SetIsSupportChangeStreamType(bool var)
    {
        isSupportChangeStreamType = var;
    }

    //isSupportRecStorageDuration
    bool GetIsSupportRecStorageDuration() const
    {
        return isSupportRecStorageDuration;
    }
    void SetIsSupportRecStorageDuration(bool var)
    {
        isSupportRecStorageDuration = var;
    }

    //isRecStorageDurationEnabled
    bool GetIsRecStorageDurationEnabled() const
    {
        return isRecStorageDurationEnabled;
    }
    void SetIsRecStorageDurationEnabled(bool var)
    {
        isRecStorageDurationEnabled = var;
    }

    //recStorageDuration
    int GetRecStorageDuration() const
    {
        return recStorageDuration;
    }
    void SetRecStorageDuration(int var)
    {
        recStorageDuration = var;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
typedef vector<TRecordParam> TRecordParamList;

typedef string TPartMountName;
typedef string TResourceId;
typedef string TManufacturer;
struct TRcsRecordParam
{
private:
	TChannel        puChn;              //前端编码通道
	TPartMountName  partMountName;      //分区挂载名称
	bool            isBlindRecord;      //是否盲录   false
	bool            isSupportResend;    //是否支持重传 true
	TRecMode        recMode;            //录像模式：既录视频又录音频,只录视频,只录关键帧  RCS_RECMODE_ALL
	int             recExpire;          //录像有效期：0表示不锁定，其他表示锁定  0
	TResourceId     nruResourceId;      //NRU的资源ID  0
	TManufacturer   puManu;             //监控点厂商

public:
	TRcsRecordParam()
	{
		Clear();
		recMode = RCS_RECMODE_ALL;
	}

	void Clear()
	{
		puChn.Clear();
		partMountName.clear();
		isBlindRecord = false;
		isSupportResend = true;
		recMode.clear();
		recExpire = 0;
		nruResourceId.clear();
		puManu = MEDIA_MANUFACTURE_KEDACOM;
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

	//partMountName
	const TPartMountName& GetPartMountName() const
	{
		return partMountName;
	}
	TPartMountName& GetPartMountName()
	{
		return partMountName;
	}
	void SetPartMountName(const TPartMountName& var)
	{
		partMountName = var;
	}

	//isBlindRecord
	bool GetIsBlindRecord() const
	{
		return isBlindRecord;
	}
	void SetIsBlindRecord(bool var)
	{
		isBlindRecord = var;
	}

	//isSupportResend
	bool GetIsSupportResend() const
	{
		return isSupportResend;
	}
	void SetIsSupportResend(bool var)
	{
		isSupportResend = var;
	}

	//recMode
	const TRecMode& GetRecMode() const
	{
		return recMode;
	}
	TRecMode& GetRecMode()
	{
		return recMode;
	}
	void SetRecMode(const TRecMode& var)
	{
		recMode = var;
	}

	//recExpire
	int GetRecExpire() const
	{
		return recExpire;
	}
	void SetRecExpire(int var)
	{
		recExpire = var;
	}

	//nruResourceId
	const TResourceId& GetNruResourceId() const
	{
		return nruResourceId;
	}
	TResourceId& GetNruResourceId()
	{
		return nruResourceId;
	}
	void SetNruResourceId(const TResourceId& var)
	{
		nruResourceId = var;
	}

	//puManu
	const TManufacturer& GetPuManufacturer() const
	{
		return puManu;
	}
	TManufacturer& GetPuManufacturer()
	{
		return puManu;
	}
	void SetPuManufacturer(const TManufacturer& var)
	{
		puManu = var;
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};
typedef vector<TRcsRecordParam> TRcsRecordParamList;

typedef string TRecFileName;
struct TRcsLockRecParam
{
private:
	TChannel        puChn;              //前端编码通道
	int             recLockTime;        //录像锁定天数：0表示解锁

public:
	TRcsLockRecParam()
	{
		Clear();
	}

	void Clear()
	{
		puChn.Clear();
		recLockTime = 0;
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

	//recLockTime
	int GetRecLockTime() const
	{
		return recLockTime;
	}
	void SetRecLockTime(int var)
	{
		recLockTime = var;
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};
typedef vector<TRcsLockRecParam> TRcsLockRecParamList;

struct TNruBaseInfo
{
private:
    UUIDString nruID;
    string devName;     //设备名
    TNruType devType;   //<设备类型>@<部署方式>
    string manuFactory; //设备厂商

public:
    TNruBaseInfo()
    {
    }

    void Clear()
    {
        nruID.clear();
        devName.clear();
        devType.clear();
        manuFactory.clear();
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

    //devName
    const string& GetDevName() const
    {
        return devName;
    }
    string& GetDevName()
    {
        return devName;
    }
    void SetDevName(const string& var)
    {
        devName = var;
    }

    //devType
    const TNruType& GetDevType() const
    {
        return devType;
    }
    TNruType& GetDevType()
    {
        return devType;
    }
    void SetDevType(const TNruType& var)
    {
        devType = var;
    }

    //manuFactory
    const string& GetManuFactory() const
    {
        return manuFactory;
    }
    string& GetManuFactory()
    {
        return manuFactory;
    }
    void SetManuFactory(const string& var)
    {
        manuFactory = var;
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


struct TNetworkInfo
{
private:
    string ifName;      //网卡名
    TIpAddr ipAddr;      //ip地址
    TIpAddr netMask;      //ip地址,HZCTODO：可以考虑删除？
    TIpAddr gateWay;     //网关地址

public:
    TNetworkInfo()
    {
    }

    void Clear()
    {
        ifName.clear();
        ipAddr.clear();
        netMask.clear();
        gateWay.clear();
    }

public:

    //ifName
    const string& GetIfName() const
    {
        return ifName;
    }
    string& GetIfName()
    {
        return ifName;
    }
    void SetIfName(const string& var)
    {
        ifName = var;
    }

    //ipAddr
    const TIpAddr& GetIpAddr() const
    {
        return ipAddr;
    }
    TIpAddr& GetIpAddr()
    {
        return ipAddr;
    }
    void SetIpAddr(const TIpAddr& var)
    {
        ipAddr = var;
    }

    //netMask
    const TIpAddr& GetNetMask() const
    {
        return netMask;
    }
    TIpAddr& GetNetMask()
    {
        return netMask;
    }
    void SetNetMask(const TIpAddr& var)
    {
        netMask = var;
    }

    //gateWay
    const TIpAddr& GetGateWay() const
    {
        return gateWay;
    }
    TIpAddr& GetGateWay()
    {
        return gateWay;
    }
    void SetGateWay(const TIpAddr& var)
    {
        gateWay = var;
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


struct TNruInfo
{
private:
    TNruBaseInfo    nruBaseInfo;
    TNetworkInfo    networkInfo;
    TIpAddr         svrAddr;            //注册的RMS服务器地址
    s32             maxBandWidth;       //nru的最大数据带宽，单位：Kb/s
    bool            isOnline;
    bool            isScOnline;
    bool            isUsed;             //是否启用

public:
    TNruInfo()
    {
        Clear();
    }

    void Clear()
    {
        nruBaseInfo.Clear();
        networkInfo.Clear();
        svrAddr.clear();
        maxBandWidth = 0;
        isOnline = false;
        isScOnline = false;
        isUsed = false;
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

    //svrAddr
    const TIpAddr& GetSvrAddr() const
    {
        return svrAddr;
    }
    TIpAddr& GetSvrAddr()
    {
        return svrAddr;
    }
    void SetSvrAddr(const TIpAddr& var)
    {
        svrAddr = var;
    }

    //maxBandWidth
    s32 GetMaxBandWidth() const
    {
        return maxBandWidth;
    }
    void SetMaxBandWidth(s32 var)
    {
        maxBandWidth = var;
    }

    //isOnline
    bool GetIsOnline() const
    {
        return isOnline;
    }
    void SetIsOnline(bool var)
    {
        isOnline = var;
    }

    //isScOnline
    bool GetIsScOnlie() const
    {
        return isScOnline;
    }
    void SetIsScOnline(bool var)
    {
        isScOnline = var;
    }

    //nru是否提供完整服务（录像业务及配置业务）
    bool IsNruFullyAvailable() const
    {
        return isOnline && isScOnline;
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
};

typedef vector<TNruInfo> TNruInfoList;

typedef TNruInfo TDiscoveryNruInfo;
typedef vector<TDiscoveryNruInfo> TDiscoveryNruInfoList;

struct TRecTimeRange
{
private:
    TRecType        recType;
    TTimeRange      timeRange;

public:
    TRecTimeRange()
    {
        Clear();
    }

    void Clear()
    {
        recType.clear();
        timeRange.Clear();
    }

public:

    //recType
    const string& GetRecType() const
    {
        return recType;
    }
    string& GetRecType()
    {
        return recType;
    }
    void SetRecType(const string& var)
    {
        recType = var;
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
};
typedef vector<TRecTimeRange> TRecTimeRangeList;

struct TRecDuration
{
private:
    TChannel            puChn;
    TRecToken           recDurationToken;
	int                 recSize;              /*录像大小 单位KB*/          //add by Royan 20140312 添加录像大小
	string              recAlias;
	bool                recLocked;				/*录像是否锁定*/
    TTimeRange          timeRange;
    TRecTimeRangeList   recTimeRangeList;

public:
    TRecDuration()
    {
        Clear();
    }

    void Clear()
    {
        puChn.Clear();
        recDurationToken.clear();
		recSize = 0;
		recAlias.clear();
        timeRange.Clear();
        recTimeRangeList.clear();
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

    //recDurationToken
    const TRecToken& GetRecDurationToken() const
    {
        return recDurationToken;
    }
    TRecToken& GetRecDurationToken()
    {
        return recDurationToken;
    }
    void SetRecDurationToken(const TRecToken& var)
    {
        recDurationToken = var;
    }

	//recSize
	const int& GetRecSize() const
	{
		return recSize;
	}
	int& GetRecSize()
	{
		return recSize;
	}
	void SetRecSize(const int& var)
	{
		recSize = var;
	}

	//recAlias
	const string& GetRecAlias() const
	{
		return recAlias;
	}
	string& GetRecAlias()
	{
		return recAlias;
	}
	void SetRecAlias(const string& var)
	{
		recAlias = var;
	}

	bool GetRecLocked() const
	{
		return recLocked;
	}
	void SetRecLocked(bool bRecLocked)
	{
		recLocked = bRecLocked;
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

    //recTimeRangeList
    const TRecTimeRangeList& GetRecTimeRangeList() const
    {
        return recTimeRangeList;
    }
    TRecTimeRangeList& GetRecTimeRangeList()
    {
        return recTimeRangeList;
    }
    void SetRecTimeRangeList(const TRecTimeRangeList& var)
    {
        recTimeRangeList = var;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
typedef vector<TRecDuration> TRecDurationList;
//
//struct TRecTimeTable
//{
//private:
//    TChannel            puChn;
//    TRecDurationList    recDurationList;
//
//public:
//    TRecTimeTable()
//    {
//        Clear();
//    }
//
//    void Clear()
//    {
//        puChn.Clear();
//        recDurationList.clear();
//    }
//
//public:
//
//    //puChn
//    const TChannel& GetPuChn() const
//    {
//        return puChn;
//    }
//    TChannel& GetPuChn()
//    {
//        return puChn;
//    }
//    void SetPuChn(const TChannel& var)
//    {
//        puChn = var;
//    }
//
//    //recDurationList
//    const TRecDurationList& GetRecDurationList() const
//    {
//        return recDurationList;
//    }
//    TRecDurationList& GetRecDurationList()
//    {
//        return recDurationList;
//    }
//    void SetRecDurationList(const TRecDurationList& var)
//    {
//        recDurationList = var;
//    }
//
//public:
//    const std::string ToXml()const;
//    void FromXml(const std::string& xml_string);
//    void ToXml(TiXmlDocument& xmlDoc)const;
//    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
//    void FromXml(const TiXmlNode& xmlNode);
//};

struct TPuChnRecDays
{
private:
    TChannel    puChn;
    int         dayBitMask;

public:
    TPuChnRecDays()
    {
        Clear();
    }

    void Clear()
    {
        puChn.Clear();
        dayBitMask = 0;
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

    //dayBitMask
    int GetDayBitMask() const
    {
        return dayBitMask;
    }
    void SetDayBitMask(int var)
    {
        dayBitMask = var;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
typedef vector<TPuChnRecDays> TPuChnRecDaysList;

struct TRecTag
{
private:
    int         tagID;
    TChannel    puChn;
    string      tagTime;
    string      tagDesc;

public:
    TRecTag()
    {
        Clear();
    }

    void Clear()
    {
        tagID = 0;
        puChn.Clear();
        tagTime.clear();
        tagDesc.clear();
    }

public:
    //tagID
    int GetTagID() const
    {
        return tagID;
    }
    void SetTagID(int var)
    {
        tagID = var;
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

    //tagTime
    const string& GetTagTime() const
    {
        return tagTime;
    }
    string& GetTagTime()
    {
        return tagTime;
    }
    void SetTagTime(const string& var)
    {
        tagTime = var;
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
};
typedef vector<TRecTag> TRecTagList;


//平台录像状态定义
typedef string TPlatRecType;
#define   PLAT_REC_NO          "PlatRecNo"
#define   PLAT_REC_OK          "PlatRecOk"
#define   PLAT_REC_TRY         "PlatRecTry"
#define   PLAT_REC_STOPPING    "PlatRecStopping"

struct TRcsPuRecordStatus
{
private:
    map<int,TRecStatus>    platRec;    //通道的平台录像状态
public:
    TRcsPuRecordStatus()
    {
        Clear();
    }

    ~TRcsPuRecordStatus()
    {
        Clear();
    }

    void Clear()
    {
        platRec.clear();      
    }
public:
    void SetPlatRec(const map<int,TRecStatus>& cPlatRec)
    {
        platRec = cPlatRec;
    }
    map<int,TRecStatus>& GetPlatRec()
    {
        return platRec;
    }
    const map<int,TRecStatus>& GetPlatRec() const
    {
        return platRec;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

typedef string TRecLockOprType;
#define RCS_REC_LOCK               "lock"
#define RCS_REC_UNLOCK             "unlock"
#define RCS_REC_LOCK_GET           "get"

/////////////////////////////////////////////////////////////////////
//}}RMS录像机管理子系统相关接口
/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
//{{SCSDK录像机系统配置管理相关接口
/////////////////////////////////////////////////////////////////////
typedef string TDiskState;
#define RCS_DISKSTATE_ONLINE        "ONLINE"
#define RCS_DISKSTATE_OFFLINE       "OFFLINE"
#define RCS_DISKSTATE_INACTIVE      "INACTIVE"
#define RCS_DISKSTATE_UNKNOWN       "UNKNOWN"

typedef string TPartState;
#define	RCS_PARTSTATE_OFFLINE       "OFFLINE"           //下线
#define	RCS_PARTSTATE_UNFORMATTED   "UNFORMATTED"       //未格式化
#define	RCS_PARTSTATE_FORMATTED     "FORMATTED"         //已格式化
#define	RCS_PARTSTATE_MOUNTED       "MOUNTED"           //已挂载
#define	RCS_PARTSTATE_UMOUNTED      "UMOUNTED"          //已卸载
#define	RCS_PARTSTATE_UNKNOWN       "UNKNOWN"           //未知状态

typedef string TDiskDevOperation;
#define RCS_DISKDEV_OPERATION_UNKNOWN               "UNKNOWN"               //无操作
#define RCS_DISKDEV_OPERATION_STOPOPERATION         "STOPOPERATION"	        //停止操作  //目前未使用
#define RCS_DISKDEV_OPERATION_WAITFORMAT            "WAITFORMAT"			//等待格式化
#define RCS_DISKDEV_OPERATION_FORMATTING            "FORMATTING"			//正在格式化
#define RCS_DISKDEV_OPERATION_WAITFSCK              "WAITFSCK"				//等待修复
#define RCS_DISKDEV_OPERATION_FSCKING               "FSCKING"				//正在修复
#define RCS_DISKDEV_OPERATION_WAITMOUNT             "WAITMOUNT"				//等待挂载
#define RCS_DISKDEV_OPERATION_MOUNTING              "MOUNTING"				//正在挂载
#define RCS_DISKDEV_OPERATION_WAITUNMOUNT           "WAITUNMOUNT"			//等待卸载
#define RCS_DISKDEV_OPERATION_UNMOUNTING            "UNMOUNTING"			//正在卸载
#define RCS_DISKDEV_OPERATION_SYSINVALID            "SYSINVALID"			//已卸载   //文件系统无效
#define RCS_DISKDEV_OPERATION_ERROR                 "ERROR"					//格式化失败或修复失败
#define RCS_DISKDEV_OPERATION_REBOOT                "REBOOT"
#define RCS_DISKDEV_OPERATION_UNUSED                "UNUSED"				//未使用,如磁盘数过多

typedef string TPartType;
#define RCS_PARTTYPE_VBFS                       "vbfs"					//分区格式为vbfs
#define RCS_PARTTYPE_EXT3                       "ext3"					//分区格式为ext3
#define RCS_PARTTYPE_EXT4                       "ext4"                  //分区格式为ext4
#define RCS_PARTTYPE_CLOUD                      "cloud"                 //分区格式为cloud
#define RCS_PARTTYPE_UNKNOWN                    "unknow"				//无法识别

typedef string TDiskLogType;
#define RCS_DISK_LOGTYPE_EXCEPTION              "EXCEPTION"             //日志类型显示异常
#define RCS_DISK_LOGTYPE_ERROR                  "ERROR"                 //错误
#define RCS_DISK_LOGTYPE_WARNING                "WARNING"               //警告
#define RCS_DISK_LOGTYPE_INFO                   "INFO"                  //信息

//以后多个告警码是可以合成的
//enum EDiskAbnCode
//{
//    ABNCODE_DISK_NORMAL = 0,				//磁盘正常
//    ABNCODE_DISK_RO = 1<<0,				    //文件系统只读
//    ABNCODE_FS_ERROR = 1<<1,				//文件系统错误
//};

typedef string TDiskAbnCode;
#define RCS_ABNCODE_DISK_NORMAL             "NORMAL"               //磁盘正常
#define RCS_ABNCODE_DISK_RO                 "RO"                   //文件系统只读
#define RCS_ABNCODE_FS_ERROR                "FS_ERROR"             //文件系统错误
#define RCS_ABNCODE_UNKOWN                  "UNKOWN"               //文件系统错误

typedef string TDiskUsage;
#define RCS_DISK_USAGE_DATA                 "DATA"      //数据盘
#define RCS_DISK_USAGE_SYSTEM               "SYSTEM"    //系统盘
#define RCS_DISK_USAGE_BACKUP               "BACKUP"    //备份盘
#define RCS_DISK_USAGE_SATA                 "SATA"      //内置sata盘
#define RCS_DISK_USAGE_APPDATA              "APPDATA"   //应用数据盘
#define RCS_DISK_USAGE_GPSDATA              "GPSDATA"   //GPS数据盘
#define RCS_DISK_USAGE_MAP                  "MAP"       //离线地图盘
#define RCS_DISK_USAGE_UNKNOWN              "UNKNOWN"
typedef TDiskUsage TPartUsage;

enum EDiskClass
{
	ERCS_DC_unknown = 0,
	ERCS_DC_USB = 1,
	ERCS_DC_ISCSI_TRANSPORT = 2,
	ERCS_DC_INTERAL_SATA = 4,
	ERCS_DC_NETFS = 8,
	ERCS_DC_LVM = 16,
	ERCS_DC_EXTERNAL = ERCS_DC_USB | ERCS_DC_ISCSI_TRANSPORT | ERCS_DC_NETFS,
};

typedef string TIscsiState;
#define RCS_ISCSISTATE_RUNNING              "RUNNING"     //在线
#define RCS_ISCSISTATE_STOPED               "STOPED"    //离线
#define RCS_ISCSISTATE_UNSUPPORTED          "UNSUPPORT"  //不支持
#define RCS_ISCSISTATE_UNKNOWN              "UNKNOWN"     //不知道

typedef string TDiskMountType;
#define RCS_DISK_MOUNT_TYPE_NULL            "NULL"   //不指定
#define RCS_DISK_MOUNT_TYPE_NFS             "NFS"    //NFS
#define RCS_DISK_MOUNT_TYPE_CIFS            "CIFS"   //CIFS

////////////////////////////////////////////////////////////////////
//{{ISCSI及阵列相关
typedef string TIscsiCtrl;
#define RCS_ISCSI_CTRL_START                    "start"
#define RCS_ISCSI_CTRL_RESTART                  "restart"
#define RCS_ISCSI_CTRL_STOP                     "stop"
#define RCS_ISCSI_CTRL_RELOAD                   "reload"    //暂时不实现

typedef string TArrayType;
#define RCS_ARRAYTYPE_VS200                     "VS200"
#define RCS_ARRAYTYPE_VS400                     "VS400"
#define RCS_AARAYTYPE_UNKNOWN                   "UNKNOWN"

typedef string TDiskGroupState;
#define RCS_DISKGROUPSTATE_CLEAN                "CLEAN"
#define RCS_DISKGROUPSTATE_DEGRADED             "DEGRADED"
#define RCS_DISKGROUPSTATE_RECOVERING           "RECOVERING"
#define RCS_DISKGROUPSTATE_FAILED               "FAILED"
#define RCS_DISKGROUPSTATE_RESYING              "RESYING"
#define RCS_DISKGROUPSTATE_UNKNOWN              "UNKNOWN"

typedef string TDiskHealth;
#define RCS_DISKHEALTH_OK                       "OK"
#define RCS_DISKHEALTH_BAD                      "BAD"
#define RCS_DISKHEALTH_UNKNOWN                  "UNKNOWN"

typedef string TVirtualDiskState;
#define RCS_VIRTUALDISKSTATE_ESTABLISHED		"ESTABLISHED"
#define RCS_VIRTUALDISKSTATE_DROPPED			"DROPPED"
#define RCS_VIRTUALDISKSTATE_UNKNOWN			"UNKNOWN"

typedef string TVirtualDiskType;
#define RCS_VIRTUALDISKTYPE_ISCSI				"ISCSI"
#define RCS_VIRTUALDISKTYPE_UNKNOWN				"UNKNOWN"

typedef string TArrayState;
typedef TArrayState TConnectState;
#define RCS_ARRAYSTATE_ONLINE                   "ONLINE"
#define RCS_ARRAYSTATE_OFFLINE                  "OFFLINE"
#define RCS_ARRAYSTATE_UNKNOWN                  "UNKNOWN"
//}}CSI及阵列相关
////////////////////////////////////////////////////////////////////

struct TDiskDev
{
private:
    int     diskID;
	int		partID;
    string	devName;

public:
    TDiskDev()
    {
        Clear();
    }

    void Clear()
    {
        diskID = 0;
		partID = 0;
        devName.clear();
    }

public:
    //diskID
    int GetDiskID() const
    {
        return diskID;
    }
    void SetDiskID(int var)
    {
        diskID = var;
    }

	//partID
	void SetPartID(int nPartID)
	{
		partID = nPartID;
	}
	int GetPartID() const
	{
		return partID;
	}

    //devName
    const string& GetDevName() const
    {
        return devName;
    }
    string& GetDevName()
    {
        return devName;
    }
    void SetDevName(const string& var)
    {
        devName = var;
    }


public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


struct TDiskDevOpState
{
private:
    TDiskDevOperation   operation;                   //操作状态
    int                 percent;                     //进度百分比                    
    string              desc;                        //操作状态描述

public:
    TDiskDevOpState()
    {
        Clear();
    }

    void Clear()
    {
        operation.clear();
        percent = 0;
        desc.clear();
    }

    string ToString() const
    {
        std::ostringstream ossObj;
        return ossObj.str();
    }

    friend std::ostream& operator<<(std::ostream& output, const TDiskDevOpState& obj)
    {
        output<<obj.operation<<"("<<obj.percent<<")";

        return output;
    }

public:
    void SetOperation(const TDiskDevOperation& strOperation)
    {
        operation = strOperation;
    }
    TDiskDevOperation& GetOperation()
    {
        return operation;
    }
    const TDiskDevOperation& GetOperation() const
    {
        return operation;
    }

    void SetPercent(int nPercent)
    {
        percent = nPercent;
    }
    int GetPercent() const
    {
        return percent;
    }

    void SetDesc(const string& strDesc)
    {
        desc = strDesc;
    }
    string& GetDesc()
    {
        return desc;
    }
    const string& GetDesc() const
    {
        return desc;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

typedef int TPartAlarmLevel;
#define RCS_ALARM_LEVEL_NORMAL        0						//磁盘空间当前状况良好
#define RCS_ALARM_LEVEL_ONE           1						//磁盘空间一级告警
#define RCS_ALARM_LEVEL_TWO           2						//磁盘空间二级告警


struct TPartInfo
{
private:
	int			 			diskID;             			//磁盘ID
	int 					partID;             			//分区ID
	string		     		partName;             			//分区名
    string      			mountPath;             			//挂载路径,通常以/mnt、/vbfs开头
	int 		 			totalSize;             			//分区大小（单位：M）;
	int	    	 			freeSize;             			//可用空间大小 （单位：M）;  
	int                     preAllocSize;                   //预留空间大小 （单位：M）
	int                     lockSize;                       //锁定空间大小 （单位：M）
    TPartState				partState;             			//分区状态
    TPartType				partType;             			//分区格式（vbfs，ext3）
	int						start;             				//起始块位置
	int						end;             				//结束块位置
	TPartAlarmLevel         alarmLevel;                     //磁盘分区告警级别（RCS_ALARM_LEVEL_NORMAL时为告警产生，其他为告警取消）

    //动态信息
	TDiskDevOpState			opState;						//操作状态
	int    					ioUtil;							//IO使用率
    int						abnNum;             			//异常次数
	TDiskAbnCode			abnCode;						//异常代码
	int						recordNum;             			//录像数
	TPartUsage              partUsage;                      //分区用途
    bool                    bIsReportByNru;                 //是否由nru上报

public:
    TPartInfo()
    {
        Clear();
    }

    void Clear()
    {
        diskID = 0;
        partID = 0;
		partName.clear();
        mountPath.clear();
		totalSize = 0;
		freeSize = 0;
		preAllocSize = 0;
		lockSize = 0;
		partState.clear();
		partType.clear();
		start = 0;
		end = 0;
		alarmLevel = RCS_ALARM_LEVEL_NORMAL;

		opState.Clear();
		ioUtil = 0;
        abnNum = 0;
		abnCode.clear();
		recordNum = 0;
		partUsage = RCS_DISK_USAGE_UNKNOWN;
        bIsReportByNru = false;
    }

    void PrintData() const;

public:
    bool GetIsReportByNru()
    {
        return bIsReportByNru;
    }
    void SetIsReportByNru(bool var)
    {
        bIsReportByNru = var;
    }
    void SetDiskID(int nDiskID)
    {
        diskID = nDiskID;
    }
    int GetDiskID() const
    {
        return diskID;
    }

    void SetPartID(int nPartID)
    {
        partID = nPartID;
    }
    int GetPartID() const
    {
        return partID;
    }

    void SetPartName(const string& strPartName)
    {
        partName = strPartName;
    }
    string& GetPartName()
    {
        return partName;
    }
    const string& GetPartName() const
    {
        return partName;
    }

	void SetMountPath(const string& strMountPath)
	{
		mountPath = strMountPath;
	}
	string& GetMountPath()
	{
		return mountPath;
	}
	const string& GetMountPath() const
	{
		return mountPath;
	}

    void SetTotalSize(int var)
    {
        totalSize = var;
    }
    int GetTotalSize() const
    {
        return totalSize;
    }

    void SetFreeSize(int var)
    {
        freeSize = var;
    }
    int GetFreeSize() const
    {
        return freeSize;
    }

	void SetPreAllocSize(int var)
	{
		preAllocSize = var;
	}
	int GetPreAllocSize() const
	{
		return preAllocSize;
	}

	void SetLockSize(int var)
	{
		lockSize = var;
	}
	int GetLockSize() const
	{
		return lockSize;
	}

    void SetPartState(const TPartState& strPartState)
    {
        partState = strPartState;
    }
    TPartState& GetPartState()
    {
        return partState;
    }
    const TPartState& GetPartState() const
    {
        return partState;
    }

    void SetPartType(const TPartType& strPartType)
    {
        partType = strPartType;
    }
    TPartType& GetPartType()
    {
        return partType;
    }
    const TPartType& GetPartType() const
    {
        return partType;
    }

    void SetStart(int nStart)
    {
        start = nStart;
    }
    int GetStart() const
    {
        return start;
    }

    void SetEnd(int nEnd)
    {
        end = nEnd;
    }
    int GetEnd() const
    {
        return end;
    }

	TPartAlarmLevel GetPartAlarmLevel() const
	{
		return alarmLevel;
	}

	void SetPartAlarmLevel(TPartAlarmLevel tAlarmLevel)
	{
		alarmLevel = tAlarmLevel;
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

	//ioUtil
	void SetIoUtil(int nIoUtil)
	{
		ioUtil = nIoUtil;
	}
	int GetIoUtil() const
	{
		return ioUtil;
	}

	//abnNum
	void SetAbnNum(int nAbnNum)
	{
		abnNum = nAbnNum;
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

	//recordNum
    void SetRecordNum(int nRecordNum)
    {
        recordNum = nRecordNum;
    }
    int GetRecordNum() const
    {
        return recordNum;
    }

	//partUsage
	void SetPartUsage(const TPartUsage& var)
	{
		partUsage = var;
	}
	TPartUsage& GetPartUsage()
	{
		return partUsage;
	}
	const TPartUsage& GetPartUsage() const
	{
		return partUsage;
	}

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
typedef vector<TPartInfo> TPartInfoList;


struct TDiskInfo
{
private:
	int                 diskID;                                  //磁盘ID
	string              diskName;                                //磁盘名
	int                 diskSize;                                //总容量（单位：M）; 
	int                 diskAbnNum;                              //异常次数
	TDiskState          diskState;                               //磁盘状态
    string              diskSource;                              //磁盘来源
	int 			    cylinders;                               //磁盘柱面数
    int 			    heads;                                   //磁盘磁头数,HZCTODO废弃
    int                 sectors;                                 //扇区数,HZCTODO废弃
    TDiskUsage          diskUsage;                               //磁盘用途   
	TPartInfoList       partList;                                //分区信息
	string              diskVendor;                              //磁盘厂商
	EDiskClass          diskClass;                               //磁盘类型：NFS/ISCSI/本机

public:
    TDiskInfo()
    {
        Clear();
    }

    void Clear()
    {
		diskID = 0;
		diskName.clear();
		diskSize = 0;
		diskAbnNum = 0;
		diskState.clear();
		diskSource.clear();
		cylinders = 0;
		heads = 0;
		sectors = 0;
		partList.clear();
		diskVendor.clear();
		diskClass = ERCS_DC_unknown;
    }

    void PrintData() const;

public:
    void SetDiskID(int nDiskID)
    {
        diskID = nDiskID;
    }
    int GetDiskID() const
    {
        return diskID;
    }

    void SetDiskName(const string& strDiskName)
    {
        diskName = strDiskName;
    }
    string& GetDiskName()
    {
        return diskName;
    }
    const string& GetDiskName() const
    {
        return diskName;
    }

    void SetDiskSize(int var)
    {
        diskSize = var;
    }
    int GetDiskSize() const
    {
        return diskSize;
    }

    void SetDiskAbnNum(int nDiskAbnNum)
    {
        diskAbnNum = nDiskAbnNum;
    }
    int GetDiskAbnNum() const
    {
        return diskAbnNum;
    }

    void SetDiskState(const TDiskState& strDiskState)
    {
        diskState = strDiskState;
    }
    TDiskState& GetDiskState()
    {
        return diskState;
    }
    const TDiskState& GetDiskState() const
    {
        return diskState;
    }

    void SetDiskSource(const string& cDiskSource)
    {
        diskSource = cDiskSource;
    }
    string& GetDiskSource()
    {
        return diskSource;
    }
    const string& GetDiskSource() const
    {
        return diskSource;
    }

    void SetCylinders(int nCylinders)
    {
        cylinders = nCylinders;
    }
    int GetCylinders() const
    {
        return cylinders;
    }

    void SetHeads(int nHeads)
    {
        heads = nHeads;
    }
    int GetHeads() const
    {
        return heads;
    }

    void SetSectors(int nSectors)
    {
        sectors = nSectors;
    }
    int GetSectors() const
    {
        return sectors;
    }

    void SetDiskUsage(const TDiskUsage& cDiskUsage)
    {
        diskUsage = cDiskUsage;
    }
    TDiskUsage& GetDiskUsage()
    {
        return diskUsage;
    }
    const TDiskUsage& GetDiskUsage() const
    {
        return diskUsage;
    }

    void SetPartList(const TPartInfoList& cPartList)
    {
        partList = cPartList;
    }
    TPartInfoList& GetPartList()
    {
        return partList;
    }
    const TPartInfoList& GetPartList() const
    {
        return partList;
    }

	void SetDiskVendor(const string& cDiskVendor)
	{
		diskVendor = cDiskVendor;
	}
	string& GetDiskVendor()
	{
		return diskVendor;
	}
	const string& GetDiskVendor() const
	{
		return diskVendor;
	}

	void SetDiskClass(const EDiskClass& var)
	{
		diskClass = var;
	}
	EDiskClass& GetDiskClass()
	{
		return diskClass;
	}
	const EDiskClass& GetDiskClass() const
	{
		return diskClass;
	}

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
typedef vector<TDiskInfo> TDiskInfoList;

struct TNruDetailInfo
{
private:
	string nruID;
	TDiskInfoList diskInfoList;

public:
	TNruDetailInfo()
	{
		Clear();
	}

	void Clear()
	{
		nruID.clear();
		diskInfoList.clear();
	}

	const string &GetNruID() const
	{
		return nruID;
	}
	string &GetNruID()
	{
		return nruID;
	}
	void SetNruID(const string &var)
	{
		nruID = var;
	}

	const TDiskInfoList &GetNruInfoList() const
	{
		return diskInfoList;
	}
	TDiskInfoList &GetNruInfoList()
	{
		return diskInfoList;
	}
	void SetNruInfoList(const TDiskInfoList &var)
	{
		diskInfoList = var;
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
typedef vector<TNruDetailInfo> TNruDetailInfoList;

//各个分区的分界线
struct TPartCfgLine
{
private:
	int				cylinderStart;                  //起始柱面
    int				cylinders;                      //磁盘柱面数
    int				partID;                         //分区ID，从0开始

public:
    TPartCfgLine()
    {
        Clear();
    }

    void Clear()
    {
        cylinderStart = 0;
        cylinders = 0;
        partID = 0;
    }
public:
    void SetCylinderStart(int nCylinderStart)
    {
        cylinderStart = nCylinderStart;
    }
    int GetCylinderStart() const
    {
        return cylinderStart;
    }

    void SetCylinders(int nCylinders)
    {
        cylinders = nCylinders;
    }
    int GetCylinders() const
    {
        return cylinders;
    }

    void SetPartID(int nPartID)
    {
        partID = nPartID;
    }
    int GetPartID() const
    {
        return partID;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
typedef vector<TPartCfgLine> TPartCfgLineList;

typedef string TDiskLogType;
#define RCS_LOGTYPE_EXCEPTION ("EXCEPTION")  //告警录像
#define RCS_LOGTYPE_ERROR     ("ERROR")
#define RCS_LOGTYPE_WARNING   ("WARNING")
#define RCS_LOGTYPE_INFO      ("INFO")
#define RCS_LOGTYPE_UNKNOW    ("UNKNOW")
struct TDiskLog
{
private:
	string          logTime;             
    TDiskLogType    logType;             
    string          content;             

public:
    TDiskLog()
    {
        Clear();
    }

    void Clear()
    {
        logTime.clear();
        logType.clear();
        content.clear();
    }

public:
    //logTime
    const string& GetLogTime() const
    {
        return logTime;
    }
    string& GetLogTime()
    {
        return logTime;
    }
    void SetLogTime(const string& var)
    {
        logTime = var;
    }

    //logType
    const TDiskLogType& GetLogType() const
    {
        return logType;
    }
    TDiskLogType& GetLogType()
    {
        return logType;
    }
    void SetLogType(const TDiskLogType& var)
    {
        logType = var;
    }

    //content
    const string& GetContent() const
    {
        return content;
    }
    string& GetContent()
    {
        return content;
    }
    void SetContent(const string& var)
    {
        content = var;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
typedef vector<TDiskLog> TDiskLogList;

struct TNetFSCfg
{
private:
	string              serverPath;
	string              mountPath;
	string              userName;
	string              userPassword;
	string              mountOption;
	TDiskMountType      mounttype;
	bool                isConnected;

public:
	TNetFSCfg()
	{
		Clear();
	}

	void Clear()
	{
		serverPath.clear();
		mountPath.clear();
		userName.clear();
		userPassword.clear();
		mountOption.clear();
		mounttype.clear();
		isConnected = false;
	}	
public:
	void SetServerPath(const string& var)
	{
		serverPath = var;
	}
	string& GetServerPath()
	{
		return serverPath;
	}
	const string& GetServerPath() const
	{
		return serverPath;
	}

	void SetMountPath(const string& var)
	{
		mountPath = var;
	}
	string& GetMountPath()
	{
		return mountPath;
	}
	const string& GetMountPath() const
	{
		return mountPath;
	}

	void SetUserName(const string& var)
	{
		userName = var;
	}
	string& GetUserName()
	{
		return userName;
	}
	const string& GetUserName() const
	{
		return userName;
	}

	void SetUserPassword(const string& var)
	{
		userPassword = var;
	}
	string& GetUserPassword()
	{
		return userPassword;
	}
	const string& GetUserPassword() const
	{
		return userPassword;
	}

	void SetMountOption(const string& var)
	{
		mountOption = var;
	}
	string& GetMountOption()
	{
		return mountOption;
	}
	const string& GetMountOption() const
	{
		return mountOption;
	}

	void SetDiskMountType(const TDiskMountType& var)
	{
		mounttype = var;
	}
	TDiskMountType& GetDiskMountType()
	{
		return mounttype;
	}
	const TDiskMountType& GetDiskMountType() const
	{
		return mounttype;
	}

	bool GetIsConnected() const
	{
		return isConnected;
	}
	void SetIsConnected(bool var)
	{
		isConnected = var;
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};
typedef vector<TNetFSCfg> TNetFSCfgList;

struct TNetFSExport
{
private:
	string              sharePath;
	string              permitIpList;
	bool                isAdded;
	
public:
	TNetFSExport()
	{
		Clear();
	}

	void Clear()
	{
		sharePath.clear();
		permitIpList.clear();
		isAdded = false;
	}	
public:
	void SetSharePath(const string& var)
	{
		sharePath = var;
	}
	string& GetSharePath()
	{
		return sharePath;
	}
	const string& GetSharePath() const
	{
		return sharePath;
	}

	void SetPermitIpList(const string& var)
	{
		permitIpList = var;
	}
	string& GetPermitIpList()
	{
		return permitIpList;
	}
	const string& GetPermitIpList() const
	{
		return permitIpList;
	}

	bool GetIsAdded() const
	{
		return isAdded;
	}
	void SetIsAdded(bool var)
	{
		isAdded = var;
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};
typedef vector<TNetFSExport> TNetFSExportList;

struct TIscsiConf
{
private:
    TNetAddr            discoveryAddr;             
    string              chapName;             
    string              chapPasswd;                          //加密问题，怎么处理？？
    TConnectState		connectState;             			//阵列的状态

public:
    TIscsiConf()
    {
        Clear();
    }

    void Clear()
    {
        discoveryAddr.Clear();
        chapName.clear();
        chapName.clear();
        chapPasswd.clear();
        connectState.clear();
    }
public:
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

    void SetChapName(const string& strChapName)
    {
        chapName = strChapName;
    }
    string& GetChapName()
    {
        return chapName;
    }
    const string& GetChapName() const
    {
        return chapName;
    }

    void SetChapPasswd(const string& strChapPasswd)
    {
        chapPasswd = strChapPasswd;
    }
    string& GetChapPasswd()
    {
        return chapPasswd;
    }
    const string& GetChapPasswd() const
    {
        return chapPasswd;
    }

    void SetConnectState(const TConnectState& strConnectState)
    {
        connectState = strConnectState;
    }
    TConnectState& GetConnectState()
    {
        return connectState;
    }
    const TConnectState& GetConnectState() const
    {
        return connectState;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
typedef vector<TIscsiConf> TIscsiConfList;


struct TArrayInfo
{
private:
	TIpAddr				arrayIP;             											//阵列IP
	TArrayType			arrayType;             											//阵列类型
	int					sendRate;             											//网络发送速度(Mb/s)
	int					receiveRate;             										//网络接收速度(Mb/s)
	int					cpuUtil;             											//cpu占用率
    int					memUtil;             											//内存占用率
	TConnectState		connectState;             										//是否连接

public:
    TArrayInfo()
    {
        Clear();
    }

    void Clear()
    {
        arrayIP.clear();
        arrayType.clear();
        sendRate = 0;
        receiveRate = 0;
        cpuUtil = 0;
		memUtil = 0;
        connectState.clear();
    }
public:
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

    void SetArrayType(const TArrayType& strArrayType)
    {
        arrayType = strArrayType;
    }
    TArrayType& GetArrayType()
    {
        return arrayType;
    }
    const TArrayType& GetArrayType() const
    {
        return arrayType;
    }

    void SetSendRate(int nSendRate)
    {
        sendRate = nSendRate;
    }
    int GetSendRate() const
    {
        return sendRate;
    }

    void SetReceiveRate(int nReceiveRate)
    {
        receiveRate = nReceiveRate;
    }
    int GetReceiveRate() const
    {
        return receiveRate;
    }

    void SetCpuUtil(int nCpuUtil)
    {
        cpuUtil = nCpuUtil;
    }
    int GetCpuUtil() const
    {
        return cpuUtil;
    }

    void SetMemUtil(int nMemUtil)
    {
        memUtil = nMemUtil;
    }
    int GetMemUtil() const
    {
        return memUtil;
    }

    void SetConnectState(const TConnectState& strConnectState)
    {
        connectState = strConnectState;
    }
    TConnectState& GetConnectState()
    {
        return connectState;
    }
    const TConnectState& GetConnectState() const
    {
        return connectState;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
typedef vector<TArrayInfo> TArrayInfoList;

struct TDiskGroupInfo
{
private:
	TIpAddr					arrayIP;             										//阵列IP
	string					diskGroupName;             									//磁盘组名
	TDiskGroupState			diskGroupState;             							    //磁盘组状态
	int				        raidLevel;             										//RAID级别
    int						diskBitmap;
    int						spareBitmap;

public:
    TDiskGroupInfo()
    {
        Clear();
    }

    void Clear()
    {
        arrayIP.clear();
        diskGroupName.clear();
        diskGroupState.clear();
        raidLevel = -1;
        diskBitmap = 0;
        spareBitmap = 0;
    }
public:
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

    void SetDiskGroupState(const TDiskGroupState& strDiskGroupState)
    {
        diskGroupState = strDiskGroupState;
    }
    TDiskGroupState& GetDiskGroupState()
    {
        return diskGroupState;
    }
    const TDiskGroupState& GetDiskGroupState() const
    {
        return diskGroupState;
    }

    void SetRaidLevel(int nRaidLevel)
    {
        raidLevel = nRaidLevel;
    }
    int GetRaidLevel() const
    {
        return raidLevel;
    }

    void SetDiskBitmap(int nVar)
    {
        diskBitmap = nVar;
    }
    int GetDiskBitmap() const
    {
        return diskBitmap;
    }

    void SetSpareBitmap(int nVar)
    {
        spareBitmap = nVar;
    }
    int GetSpareBitmap() const
    {
        return spareBitmap;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
typedef vector<TDiskGroupInfo> TDiskGroupInfoList;


struct TPhysicalDiskInfo
{
private:
	TIpAddr				arrayIP;             											//阵列IP
	string				diskGroupName;             										//磁盘组名
	int					diskSlot;             											//磁盘槽位号
	string				vendor;             												//厂商名
	string				model;             												//型号
	int					diskSize;             											//磁盘容量（M）
	TDiskState			diskState;             											//磁盘状态
	TDiskHealth			diskHealth;             											//磁盘健康状态
	int					reallocatedSectors;             									//重新分配扇区数
	bool				isBackup;             											//是否热备份

public:
    TPhysicalDiskInfo()
    {
        Clear();
    }

    void Clear()
    {
        arrayIP.clear();
        diskGroupName.clear();
        diskSlot = 0;
        vendor.clear();
        model.clear();
        diskSize = 0;
        diskState.clear();
        diskHealth.clear();
        reallocatedSectors = 0;
        isBackup = false;
    }
public:
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

    void SetDiskSlot(int nDiskSlot)
    {
        diskSlot = nDiskSlot;
    }
    int GetDiskSlot() const
    {
        return diskSlot;
    }

    void SetVendor(const string& strVendor)
    {
        vendor = strVendor;
    }
    string& GetVendor()
    {
        return vendor;
    }
    const string& GetVendor() const
    {
        return vendor;
    }

    void SetModel(const string& strModel)
    {
        model = strModel;
    }
    string& GetModel()
    {
        return model;
    }
    const string& GetModel() const
    {
        return model;
    }

    void SetDiskSize(int nDiskSize)
    {
        diskSize = nDiskSize;
    }
    int GetDiskSize() const
    {
        return diskSize;
    }

    void SetDiskState(const TDiskState& strDiskState)
    {
        diskState = strDiskState;
    }
    TDiskState& GetDiskState()
    {
        return diskState;
    }
    const TDiskState& GetDiskState() const
    {
        return diskState;
    }

    void SetDiskHealth(const TDiskHealth& strDiskHealth)
    {
        diskHealth = strDiskHealth;
    }
    TDiskHealth& GetDiskHealth()
    {
        return diskHealth;
    }
    const TDiskHealth& GetDiskHealth() const
    {
        return diskHealth;
    }

    void SetReallocatedSectors(int nReallocatedSectors)
    {
        reallocatedSectors = nReallocatedSectors;
    }
    int GetReallocatedSectors() const
    {
        return reallocatedSectors;
    }

    void SetIsBackup(bool bIsBackup)
    {
        isBackup = bIsBackup;
    }
    bool GetIsBackup() const
    {
        return isBackup;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
typedef vector<TPhysicalDiskInfo> TPhysicalDiskInfoList;



struct TVirtualDiskInfo
{
private:
	TIpAddr				arrayIP;             											//阵列IP
	string				diskGroupName;             										//磁盘组名
	string				virtualDiskName;             									//虚拟磁盘名
	TVirtualDiskType	virtualDiskType;             									//虚拟磁盘类型
	u32					virtualDiskSize;             									//虚拟磁盘容量
	TVirtualDiskState	virtualDiskState;             									//虚拟磁盘状态
	u32					establishedTime;             									// 连接建立至今累计的时间 
	u32					droppedTime;             							    		// 连接断开至今累计的时间 
	u32					targetID;             											//TargetID
	u32					lunID;             												//LunID
	string				localDeviceName;             									//映射设备名

public:
    TVirtualDiskInfo()
    {
        Clear();
    }

    void Clear()
    {
        arrayIP.clear();
        diskGroupName.clear();
        virtualDiskName.clear();
        virtualDiskType.clear();
        virtualDiskSize = 0;
        virtualDiskState.clear();
        establishedTime = 0;
        droppedTime = 0;
        targetID = 0;
        lunID = 0;
        localDeviceName.clear();
    }
public:
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

    void SetVirtualDiskName(const string& strVirtualDiskName)
    {
        virtualDiskName = strVirtualDiskName;
    }
    string& GetVirtualDiskName()
    {
        return virtualDiskName;
    }
    const string& GetVirtualDiskName() const
    {
        return virtualDiskName;
    }

    void SetVirtualDiskType(const TVirtualDiskType& strVirtualDiskType)
    {
        virtualDiskType = strVirtualDiskType;
    }
    TVirtualDiskType& GetVirtualDiskType()
    {
        return virtualDiskType;
    }
    const TVirtualDiskType& GetVirtualDiskType() const
    {
        return virtualDiskType;
    }

    void SetVirtualDiskSize(u32 nVirtualDiskSize)
    {
        virtualDiskSize = nVirtualDiskSize;
    }
    u32 GetVirtualDiskSize() const
    {
        return virtualDiskSize;
    }

    void SetVirtualDiskState(const TVirtualDiskState& strVirtualDiskState)
    {
        virtualDiskState = strVirtualDiskState;
    }
    TVirtualDiskState& GetVirtualDiskState()
    {
        return virtualDiskState;
    }
    const TVirtualDiskState& GetVirtualDiskState() const
    {
        return virtualDiskState;
    }

    void SetEstablishedTime(u32 nEstablishedTime)
    {
        establishedTime = nEstablishedTime;
    }
    u32 GetEstablishedTime() const
    {
        return establishedTime;
    }

    void SetDroppedTime(u32 nDroppedTime)
    {
        droppedTime = nDroppedTime;
    }
    u32 GetDroppedTime() const
    {
        return droppedTime;
    }

    void SetTargetID(u32 nTargetID)
    {
        targetID = nTargetID;
    }
    u32 GetTargetID() const
    {
        return targetID;
    }

    void SetLunID(u32 nLunID)
    {
        lunID = nLunID;
    }
    u32 GetLunID() const
    {
        return lunID;
    }

    void SetLocalDeviceName(const string& strLocalDeviceName)
    {
        localDeviceName = strLocalDeviceName;
    }
    string& GetLocalDeviceName()
    {
        return localDeviceName;
    }
    const string& GetLocalDeviceName() const
    {
        return localDeviceName;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
typedef vector<TVirtualDiskInfo> TVirtualDiskInfoList;


/////////////////////////////////////////////////////////////////////
//}}SCSDK录像机系统配置管理相关接口
/////////////////////////////////////////////////////////////////////

#endif  //#ifndef __CMS_STRUCT_H__









