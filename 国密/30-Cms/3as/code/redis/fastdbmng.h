/*****************************************************************************
   功能        : fastdb通信管理(当前使用的是redis)
   作者        : liuyongtao
   版本        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人      修改内容
   2016/03/24  1.0         liuyongtao     创建
   2016/04/19  1.1         huzhiyun       接口调整
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

/* tas自身数据向redis写入和发布 */
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

/* tas与gbu之间混排的设备批量的入网修改退网的功能 */
bool RedisSubscribeDeviceBatchReq();
bool RedisPublishDeviceBatchRsp(const string &strGbuId, const CTasDeviceBatchChangeRsp &cRsp);

// process为[0,100]
void RedisSetProcessValue4Tas(ENTasProcessType type, const string &id, int process);

bool RedisSetTasDbInfo(const string& strDbIp, const u16 wDbPort, const string& strDbPassword);

/*国标选择推送需要用到的一些接口*/
int GetMaxCivilcodeLev();
#endif // _FASTDBMNG_H
