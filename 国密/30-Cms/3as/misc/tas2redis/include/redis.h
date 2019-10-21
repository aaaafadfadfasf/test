#ifndef _REDIS_H_
#define _REDIS_H_

#include <string>
#include <map>
#include <set>
#include <vector>
#include <math.h>
#include "sdk/fastdbsdk/fastdb.h"
#include "sdk/fastdbsdk/keydef.h"
#include "toolsdata.h"
#include "cms/tas/tas_struct.h"

using namespace std;

bool FastDbInit();
void FastDbQuit();

bool RedisDomainAdd(const CDomainRelationInfo &cDomain);
bool RedisDomainDel(const string &strDomainId);
bool RedisDeviceAdd(const CDeviceInfo &cDev);
bool RedisDeviceDel(const string &strDevId);

bool GetAllDoaminList(vector<string> &vecDoaminList);
bool GetAllDevList(vector<string> &vecDevList);
bool RedisVidSrcAliasOrGpsSet(const TDeviceCapName &tCap);

bool RedisUser2DevPriAdd(const CUserDeviceInfo &cUserDevInfo);
bool RedisUserGrp2DevPriAdd(const CUserGroupDeviceInfo &cUserGrpDevInfo);

bool RedisUser2UserGrpAdd(const string &strUserId,const string &strUserGrpId);


#endif //REDIS