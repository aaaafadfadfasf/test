/*****************************************************************************
   ����        : fastdbͨ�Ź���(��ǰʹ�õ���redis)
   ����        : liuyongtao
   �汾        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
   2016/03/24  1.0         liuyongtao     ����
   2016/04/19  1.1         huzhiyun       �ӿڵ���
******************************************************************************/
#ifndef _FASTDBMNG_H
#define _FASTDBMNG_H
#include "tas_redis.h"
#include "tas_struct.h"
#include "cms/tas/tas_proto.h"
#include "cms/cmu/cmu_redis.h"
#include "cms/cmu/cmu_struct.h"
#include <string>
using std::string;

bool FastDbInit();
bool IsRedisConnected();
void FastDbQuit();

/* tas����������redisд��ͷ��� */
bool RedisDeviceAdd(const CDeviceInfo &cDev);   
bool RedisDeviceDel(const CDeviceInfo &cDev);   
bool RedisDeviceMod(const CDeviceInfo &cDev, const set<TChangedField>& changeFields);
bool RedisVidSrcAliasOrGpsSet(const TDeviceCapName &tCap);
bool RedisDomainRelationAdd(const CDomainRelationInfo &cDom);
bool RedisDomainRelationDel(const CDomainRelationInfo &cDom);
bool RedisDomainRelationMod(const CDomainRelationInfo &cDom);
bool RedisDomainRelationInit(void);
bool RedisAddUserDevPrivilege(IN const CUserDeviceInfo &devPriv);
bool RedisModUserDevPrivilege(IN const CUserDeviceModReq &devPriv);
bool RedisDelUserDevPrivilege(IN const CUserDeviceInfoKey &devPriv);
bool RedisAddUsergrpDevPrivilege(IN const CUserGroupDeviceInfo &devPriv);
bool RedisModUsergrpDevPrivilege(IN const CUserGroupDeviceModReq &devPriv);
bool RedisDelUsergrpDevPrivilege(IN const CUserGroupDeviceInfoKey &devPriv);
bool RedisSetUserUsergrpId(IN const UUIDString &userId, IN const UUIDString &usergrpId);
bool RedisDelUser(IN const UUIDString &userId);
bool RedisDelUsergrp(IN const UUIDString &usergrpId);

bool IsDevVidSrcOnline(const UUIDString& devId, const int vidIdx);
bool IsDevEnable(const UUIDString &devId);

/* tas��gbu֮����ŵ��豸�����������޸������Ĺ��� */
bool RedisSubscribeDeviceBatchReq();
bool RedisPublishDeviceBatchRsp(const string &strGbuId, const CTasDeviceBatchChangeRsp &cRsp);

// processΪ[0,100]
void RedisSetProcessValue4Tas(ENTasProcessType type, const string &id, int process);

bool RedisSetTasDbInfo(const string& strDbIp, const u16 wDbPort, const string& strDbPassword);

/*����ѡ��������Ҫ�õ���һЩ�ӿ�*/
int GetMaxCivilcodeLev();
#endif // _FASTDBMNG_H
