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

	// ��Ҫת���Ľӿ�

	// ��ȡ��ƵԴ����
	EFDB_ErrorCode GetDevNameAndVSAlaias(const UUIDString &devId, int vsChnNo, string &strDevName, string &strVidSrcAlaias, string &strErrMsg);

	// ������ͨ��ת������ƵԴͨ����ȡ��ƵԴȨ��(hzy:����ͨ��Ȩ�޺���ƵԴȨ����ͬ)
	// �������ƵԴIDҲ��������ת��
	EFDB_ErrorCode GetUserVideoPriByEncChn(const UUIDString &userId, const UUIDString &devId, int chnNo, CVideosourcePrivilege &priv, string &strErrMsg);

	// ͨ����ȡ�û��豸Ȩ���ж��û��Ƿ񻮹�������豸,ʵ�ʲ�ȥ��Ȩ��ֵ
	bool IsUserHasDev(const UUIDString &userId, const UUIDString &devId);

	// ͨ����ȡ�û���ƵԴȨ���ж��û��Ƿ񻮹��������ƵԴ,ʵ�ʲ�ȥ��Ȩ��ֵ
	bool IsUserHasVideoSource(const UUIDString &userId, const UUIDString &devId, int nChnNo, string &strErrMsg);

	// ��CU���豸��Ϣ���������ѻ�����豸����ƵԴȨ�޵ȡ�
	// �����admin,redis��û��Ȩ����Ϣ�ģ�����Ҫ�Լ����ɡ�
	EFDB_ErrorCode GetUserFullDevicefullinfoAndPrivilege(const UUIDString userId, const UUIDString &devId, const bool &bIsAdmin, TDevicefullinfoAndPrivilege &tDevFullInfoAndPri, string &strErrMsg);

	EFDB_ErrorCode GetUserFullDevForCu(const UUIDString userId, const UUIDString &devId, const bool &bIsAdmin, TDeviceInfoAndStatus &tDeviceInfoAndStatus, string &strErrMsg);

	// ���豸����״̬����CU���豸������Ϣ�У�ͨ��״̬��online����Ϊoffline��idle״̬����
	EFDB_ErrorCode GetFullPuConfigForCu(IN const UUIDString &devId, OUT TPuConfig &puConfig, string &strErrMsg);

    // ��ȡǰ�˷���״̬
    EFDB_ErrorCode GetPuServiceStatusForCu(IN const UUIDString &devId, OUT TPuServiceStatus &serviceStatus, string &strErrMsg);

    // ��ȡ�澯״̬
    EFDB_ErrorCode GetPuAlarmStatusForCu(IN const UUIDString &devId, OUT TPuAlarmStatus &alarmStatus, string &strErrMsg);

	// ͨ���豸Id��ȡ�豸�����������
	EFDB_ErrorCode GetDomainNameByDevId(const UUIDString &devId, string &strDomainName, string &strErrMsg);

	// ��ȡ��������Ϣ
	EFDB_ErrorCode GetAllDomainRelationInfo(CDomainInfoMap &domainInfoMap, string &strErrMsg);
};

#endif // _REDIS_FUNCTION_H_
