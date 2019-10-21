/*****************************************************************************
   功能        : cmu redis头文件定义
   作者        : xiongxh
   版本        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人      修改内容
   2016/03/30  1.0         xiongxh       创建
******************************************************************************/
#ifndef _CMU_REDIS_H_
#define _CMU_REDIS_H_

#include <string>
using std::string;

#include "cms/cms_proto.h"
#include "cms/rcs/rcs_proto.h"

//===============================================================================================================================
// 单点订阅（根据设备Id和订阅类型来构建订阅通道，具体的规则位：DevId + ":" + SsType）
//===============================================================================================================================

// 根据DevId和订阅类型获取对应的Redis订阅通道
// Redis的订阅通道规则为：设备Id + ":" + 订阅类型，
// 比如DevId为"172e5444150a4b8ea17e241e2ded97e5"，其上下线对应的订阅通道为："172e5444150a4b8ea17e241e2ded97e5:PU_ON_OFF"
inline string RedisGetSubChn(const string &strDevId, const TSsType &tSsType)
{
	return strDevId + string(":") + tSsType;
}

inline vector<string> RedisGetSubChn(const string &strDevId, const TSsTypes &tSsTypes)
{
    vector<string> cSubChn;
    for (TSsTypes::const_iterator it = tSsTypes.begin(); tSsTypes.end() != it; ++it)
    {
        cSubChn.push_back(RedisGetSubChn(strDevId, *it));
    }

    return cSubChn;
}

inline vector<string> RedisGetSubChn(const TSsDevList &tSsDevList)
{
    vector<string> cSubChn;
    for (TSsDevList::const_iterator it4Dev = tSsDevList.begin(); tSsDevList.end() != it4Dev; ++it4Dev)
    {
        const TSsTypes &tSsTypes = it4Dev->second;
        for (TSsTypes::const_iterator it4Type = tSsTypes.begin(); tSsTypes.end() != it4Type; ++it4Type)
        {
            cSubChn.push_back(RedisGetSubChn(it4Dev->first, *it4Type));
        }
    }

    return cSubChn;
}

// 根据Redis订阅通道解析出对应的DevId和订阅类型，可以视为RedisGetSubChn的反函数
// 比如订阅通道为："172e5444150a4b8ea17e241e2ded97e5:PU_ON_OFF"，则可以解析出对应的DevId为"172e5444150a4b8ea17e241e2ded97e5"，订阅类型为PU_ON_OFF
inline bool RedisGetSubTypeByChn(const string &strChn, string &strDevId, TSsType &tSsType)
{
    string::size_type nPos = strChn.find(":");
    if (string::npos != nPos)
    {
        strDevId = strChn.substr(0, nPos);
        tSsType  = strChn.substr(nPos + 1);
        return true;
    }
    return false;
}

// 获取设备上下线状态的Redis订阅通道
// 通道规则：设备Id + ":" + 订阅类型
// 通道举例：DevId为"172e5444150a4b8ea17e241e2ded97e5"，其上下线的订阅通道为："172e5444150a4b8ea17e241e2ded97e5:PU_ON_OFF"
// 发布内容：TPuStatus调用ToXml序列化后的内容
inline string RedisGetOnOffSubChn(const string &strDevId)
{
	return RedisGetSubChn(strDevId, PU_ON_OFF_STATUS);
}

// 获取设备配置信息的Redis订阅通道
// 通道规则：设备Id + ":" + 订阅类型
// 通道举例：DevId为"172e5444150a4b8ea17e241e2ded97e5"，其配置信息的订阅通道为："172e5444150a4b8ea17e241e2ded97e5:PU_CONFIG"
// 发布内容：TPuDevConfig调用ToXml序列化后的内容
inline string RedisGetConfigSubChn(const string &strDevId)
{
	return RedisGetSubChn(strDevId, PU_CONFIG_STATUS);
}

// 获取设备服务状态的Redis订阅通道
// 通道规则：设备Id + ":" + 订阅类型
// 通道举例：DevId为"172e5444150a4b8ea17e241e2ded97e5"，其服务状态的订阅通道为："172e5444150a4b8ea17e241e2ded97e5:PU_SERVICE"
// 发布内容：TPuServiceStatus调用ToXml序列化后的内容
inline string RedisGetServiceSubChn(const string &strDevId)
{
	return RedisGetSubChn(strDevId, PU_SERVICE_STATUS);
}

// 获取设备告警信息的Redis订阅通道
// 通道规则：设备Id + ":" + 订阅类型
// 通道举例：DevId为"172e5444150a4b8ea17e241e2ded97e5"，其告警信息的订阅通道为："172e5444150a4b8ea17e241e2ded97e5:PU_ALARM"
// 发布内容：TPuAlarmStatus调用ToXml序列化后的内容
inline string RedisGetAlarmSubChn(const string &strDevId)
{
	return RedisGetSubChn(strDevId, PU_ALARM_STATUS);
}

// 获取设备GPS数据的Redis订阅通道
// 通道规则：设备Id + ":" + 订阅类型
// 通道举例：DevId为"172e5444150a4b8ea17e241e2ded97e5"，其GPS数据的订阅通道为："172e5444150a4b8ea17e241e2ded97e5:PU_GPS"
// 发布内容：TGpsData调用ToXml序列化后的内容
inline string RedisGetGpsSubChn(const string &strDevId)
{
	return RedisGetSubChn(strDevId, PU_GPS_STATUS);
}

// 获取设备透明数据的Redis订阅通道
// 通道规则：设备Id + ":" + 订阅类型
// 通道举例：DevId为"172e5444150a4b8ea17e241e2ded97e5"，其透明数据的订阅通道为："172e5444150a4b8ea17e241e2ded97e5:PU_TRANSDATA"
// 发布内容：TTransparentData调用ToXml序列化后的内容
inline string RedisGetTransDataSubChn(const string &strDevId)
{
	return RedisGetSubChn(strDevId, PU_TRANSDATA_STATUS);
}

// 获取设备平台录像的Redis订阅通道
// 通道规则：设备Id + ":" + 订阅类型
// 通道举例：DevId为"172e5444150a4b8ea17e241e2ded97e5"，其平台录像的订阅通道为："172e5444150a4b8ea17e241e2ded97e5:PU_PLATFORM_RECORD"
// 发布内容：TRcsPuRecordStatus调用ToXml序列化后的内容
inline string RedisGetPlatRecSubChn(const string &strDevId)
{
    return RedisGetSubChn(strDevId, SS_TYPE_PU_PLATFORM_RECORD_STATUS);
}

//===============================================================================================================================
// 域订阅（根据域Id和订阅类型来构建订阅通道，具体的规则位：DomainId + "#" + SsType）
//===============================================================================================================================
inline string RedisGetCatalogSubChn(const string &strDomainId, const TSsType &tSsType)
{
    return strDomainId + string("#") + tSsType;
}

inline vector<string> RedisGetCatalogSubChn(const string &strDomainId, const TSsTypes &tSsTypes)
{
    vector<string> cChn;
    for (TSsTypes::const_iterator it = tSsTypes.begin(); tSsTypes.end() != it; ++it)
    {
        cChn.push_back(RedisGetCatalogSubChn(strDomainId, *it));
    }

    return cChn;
}

// 根据Redis订阅通道解析出对应的域Id和订阅类型，可以视为RedisGetCatalogSubChn的反函数
// 比如订阅通道为："172e5444150a4b8ea17e241e2ded97e5#PU_ON_OFF"，则可以解析出对应的DomainId为"172e5444150a4b8ea17e241e2ded97e5"，订阅类型为PU_ON_OFF
inline bool RedisGetCatalogSubTypeByChn(const string &strChn, string &strDomainId, TSsType &tSsType)
{
    string::size_type nPos = strChn.find("#");
    if (string::npos != nPos)
    {
        strDomainId = strChn.substr(0, nPos);
        tSsType = strChn.substr(nPos + 1);
        return true;
    }
    return false;
}

// 获取设备上下线的Redis订阅通道
// 通道规则：域Id + "#" + 订阅类型
// 通道举例：DomainId为"172e5444150a4b8ea17e241e2ded97e5"，其上下线对应的订阅通道为："172e5444150a4b8ea17e241e2ded97e5#PU_ON_OFF"
// 发布内容：CPuStatusNtyReq调用ToXml序列化后的内容
inline string RedisGetOnOffCatalogSubChn(const string &strDomainId)
{
    return RedisGetCatalogSubChn(strDomainId, PU_ON_OFF_STATUS);
}

// 获取设备配置信息的Redis订阅通道
// 通道规则：域Id + "#" + 订阅类型
// 通道举例：DomainId为"172e5444150a4b8ea17e241e2ded97e5"，其配置信息的订阅通道为："172e5444150a4b8ea17e241e2ded97e5#PU_CONFIG"
// 发布内容：CPuDevConfigNtyReq调用ToXml序列化后的内容
inline string RedisGetConfigCatalogSubChn(const string &strDomainId)
{
    return RedisGetCatalogSubChn(strDomainId, PU_CONFIG_STATUS);
}

// 获取设备服务状态的Redis订阅通道
// 通道规则：域Id + "#" + 订阅类型
// 通道举例：DomainId为"172e5444150a4b8ea17e241e2ded97e5"，其服务状态的订阅通道为："172e5444150a4b8ea17e241e2ded97e5#PU_SERVICE"
// 发布内容：CPuServiceStatusNtyReq调用ToXml序列化后的内容
inline string RedisGetServiceCatalogSubChn(const string &strDomainId)
{
    return RedisGetCatalogSubChn(strDomainId, PU_SERVICE_STATUS);
}

// 获取设备告警信息的Redis订阅通道
// 通道规则：域Id + "#" + 订阅类型
// 通道举例：DomainId为"172e5444150a4b8ea17e241e2ded97e5"，其告警信息的订阅通道为："172e5444150a4b8ea17e241e2ded97e5#PU_ALARM"
// 发布内容：CPuAlarmStatusNtyReq调用ToXml序列化后的内容
inline string RedisGetAlarmCatalogSubChn(const string &strDomainId)
{
    return RedisGetCatalogSubChn(strDomainId, PU_ALARM_STATUS);
}

// 获取设备GPS数据的Redis订阅通道
// 通道规则：域Id + "#" + 订阅类型
// 通道举例：DomainId为"172e5444150a4b8ea17e241e2ded97e5"，其GPS数据的订阅通道为："172e5444150a4b8ea17e241e2ded97e5#PU_GPS"
// 发布内容：CPuGpsStatusNtyReq调用ToXml序列化后的内容
inline string RedisGetGpsCatalogSubChn(const string &strDomainId)
{
    return RedisGetCatalogSubChn(strDomainId, PU_GPS_STATUS);
}

// 获取设备透明数据的Redis订阅通道
// 通道规则：域Id + "#" + 订阅类型
// 通道举例：DomainId为"172e5444150a4b8ea17e241e2ded97e5"，其透明数据的订阅通道为："172e5444150a4b8ea17e241e2ded97e5#PU_TRANSDATA"
// 发布内容：CPuTransDataStatusNtyReq调用ToXml序列化后的内容
inline string RedisGetTransDataCatalogSubChn(const string &strDomainId)
{
    return RedisGetCatalogSubChn(strDomainId, PU_TRANSDATA_STATUS);
}

// 模拟注册过程中用于通知cmu设备配置变更的订阅通道
// 通道规则：域Id + "#" + 订阅类型
// 通道举例：DomainId为"172e5444150a4b8ea17e241e2ded97e5"，其透明数据的订阅通道为："172e5444150a4b8ea17e241e2ded97e5#PU_DEVCONFIG_NTF"
// 发布内容：CPuDevConfigNtyReq调用ToXml序列化后的内容
inline string RedisGetDevConfigNtfCatalogSubChn(const string &strDomainId)
{
    return RedisGetCatalogSubChn(strDomainId, PU_DEV_CONFIG_NTF);
}

// 获取设备平台录像的Redis订阅通道
// 通道规则：域Id + ":" + 订阅类型
// 通道举例：DomainId为"172e5444150a4b8ea17e241e2ded97e5"，其平台录像的订阅通道为："172e5444150a4b8ea17e241e2ded97e5#PU_PLATFORM_RECORD"
// 发布内容：CRcsPuRecordStatusNtyReq调用ToXml序列化后的内容
inline string RedisGetPlatRecCatalogSubChn(const string &strDomainId)
{
    return RedisGetCatalogSubChn(strDomainId, SS_TYPE_PU_PLATFORM_RECORD_STATUS);
}

//===============================================================================================================================
// 设备上报数据订阅（根据域Id和订阅类型来构建订阅通道，具体的规则位：DomainId + "#" + SsType）
//===============================================================================================================================
//inline RedisGetDevReportDataSubChn()

#endif // _CMU_REDIS_H_












