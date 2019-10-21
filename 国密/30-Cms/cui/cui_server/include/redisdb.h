#ifndef _REDIS_FUNCTION_H_
#define _REDIS_FUNCTION_H_

#include "sdk/fastdbsdk/fastdb.h"
#include "cuidata.h"
#include "cuistruct.h"
#include "cms/cmu/cmu_redis.h"
#include "cuidbg.h"

string GetSubTypeByRedisSubChnName(const string &chnName);
string GetDevIdByRedisSubChnName(const string &chnName);

inline void cbRedisLog(int nLogLevel, const char *szMsg, void *pvUserData) { GLOBELOG(REDIS_LOG, nLogLevel, szMsg); }

class CRedisDBInterface : public CFastDBInterface
{
public:
	CRedisDBInterface();
	~CRedisDBInterface();

	// 需要转换的接口

	// 获取视频源别名
	EFDB_ErrorCode GetDevNameAndVSAlaias(const UUIDString &devId, int vsChnNo, string &strDevName, string &strVidSrcAlaias, string &strErrMsg);

	// 将编码通道转换成视频源通道获取视频源权限(hzy:编码通道权限和视频源权限相同)
	// 如果是视频源ID也可以正常转换
	EFDB_ErrorCode GetUserVideoPriByEncChn(const UUIDString &userId, const UUIDString &devId, int chnNo, CVideosourcePrivilege &priv, string &strErrMsg);

	// 通过获取用户设备权限判断用户是否划归了这个设备,实际不去除权限值
	bool IsUserHasDev(const UUIDString &userId, const UUIDString &devId);

	// 通过获取用户视频源权限判断用户是否划归了这个视频源,实际不去除权限值
	bool IsUserHasVideoSource(const UUIDString &userId, const UUIDString &devId, int nChnNo, string &strErrMsg);

	// 给CU的设备信息，包含了已划归的设备、视频源权限等。
	// 如果是admin,redis是没有权限信息的，所以要自己生成。
	EFDB_ErrorCode GetUserFullDevicefullinfoAndPrivilege(const UUIDString userId, const UUIDString &devId, const bool &bIsAdmin, TDevicefullinfoAndPrivilege &tDevFullInfoAndPri, string &strErrMsg);

	EFDB_ErrorCode GetUserFullDevForCu(const UUIDString userId, const UUIDString &devId, const bool &bIsAdmin, TDeviceInfoAndStatus &tDeviceInfoAndStatus, string &strErrMsg);

	// 给设备下线状态，给CU的设备配置信息中，通道状态将online设置为offline，idle状态不变
	EFDB_ErrorCode GetFullPuConfigForCu(IN const UUIDString &devId, OUT TPuConfig &puConfig, string &strErrMsg);

    // 获取前端服务状态
    EFDB_ErrorCode GetPuServiceStatusForCu(IN const UUIDString &devId, OUT TPuServiceStatus &serviceStatus, string &strErrMsg);

    // 获取告警状态
    EFDB_ErrorCode GetPuAlarmStatusForCu(IN const UUIDString &devId, OUT TPuAlarmStatus &alarmStatus, string &strErrMsg);

	// 通过设备Id获取设备所在域的域名
	EFDB_ErrorCode GetDomainNameByDevId(const UUIDString &devId, string &strDomainName, string &strErrMsg);

	// 获取所有域信息
	EFDB_ErrorCode GetAllDomainRelationInfo(CDomainInfoMap &domainInfoMap, string &strErrMsg);
};

#endif // _REDIS_FUNCTION_H_
