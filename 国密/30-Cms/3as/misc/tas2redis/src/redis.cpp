#include "redis.h"
#include "cms/commonconfig/cmcfg.h"

static CFastDBInterface *s_pcFastDb = NULL;

bool FastDbInit()
{
	s_pcFastDb = new CFastDBInterface();

	FDB_ConnInfo fdbConn;
	fdbConn.clientname = "tas2redis";
	fdbConn.username = "NA";
	fdbConn.password = GetRedisPassword();
	printf("ipaddress[%s]--redisport[%d]--password[%s]\n",GetRedisIP().c_str(),GetRedisPort(),GetRedisPassword().c_str());
	if (EC_OK != s_pcFastDb->Connect(GetRedisIP(), GetRedisPort(), fdbConn))
	{
		printf("FastDbInit failed to connect redis ! \n");
		//exit(0); // 先按照最简单的实现
		return false;
	}
	return true;
}

void FastDbQuit()
{
	s_pcFastDb->Disconnect();
	delete s_pcFastDb;
	s_pcFastDb = NULL;
}

bool RedisDomainAdd(const CDomainRelationInfo &cDomain)
{
	EFDB_ErrorCode errCode = EC_OK;
	errCode = s_pcFastDb->AddDomainRelationInfo(cDomain.GetDomainID(),cDomain);
	if (errCode != EC_OK)
	{
		printf("AddDomainRelationInfo fail,errcode[%d]--domainid[%s] \n",errCode,cDomain.GetDomainID().c_str());
		return false;
	}
	return true;
}

bool RedisDomainDel(const string &strDomainId)
{
	EFDB_ErrorCode errCode = EC_OK;
	errCode = s_pcFastDb->DelDomainRelationInfo(strDomainId);
	if (errCode != EC_OK)
	{
		printf("DelDomainRelationInfo fail,errcode[%d]--domainid[%s] \n",errCode,strDomainId.c_str());
		return false;
	}
	return true;
}

bool RedisDeviceAdd(const CDeviceInfo &tDev)
{
	EFDB_ErrorCode errCode = EC_OK;
	errCode = s_pcFastDb->AddDevice(tDev);
	if (errCode != EC_OK)
	{
		printf("RedisDeviceAdd fail,errcode[%d]--kdid[%s] \n",errCode,tDev.GetDeviceUUID().c_str());
		return false;
	}
	return true;
}

bool RedisDeviceDel(const string &strDevId)
{
	EFDB_ErrorCode errCode = EC_OK;
	errCode = s_pcFastDb->DelDevice(strDevId);
	if (errCode != EC_OK)
	{
		printf("RedisDeviceDel fail,errcode[%d]--kdid[%s] \n",errCode,strDevId.c_str());
		return false;
	}
	return true;
}

bool GetAllDoaminList(vector<string> &vecDoaminList)
{
	vecDoaminList.clear();
	EFDB_ErrorCode errCode = EC_OK;
	errCode = s_pcFastDb->GetDomainIDList(vecDoaminList);
	if (errCode != EC_OK)
	{
		printf("GetDomainIDList fail,errcode[%d] \n",errCode);
		return false;
	}
	return true;
}

bool GetAllDevList(vector<string> &vecDevList)
{
	vecDevList.clear();
	EFDB_ErrorCode errCode = EC_OK;
	errCode = s_pcFastDb->GetDeviceIDList(vecDevList);
	if (errCode != EC_OK)
	{
		printf("GetDeviceIDList fail,errcode[%d] \n",errCode);
		return false;
	}
	return true;
}

bool RedisVidSrcAliasOrGpsSet(const TDeviceCapName &tCap)
{
	EFDB_ErrorCode errCode = EC_OK;
	double tmpLan,tmpLon;
	if (tCap.capId != DEVICE_CAP_VIDEOSOURCE)//不是视频源能力
	{
		return false;
	}

	for (u32 i= 0;i < tCap.deviceCapIndexNames.size();i++)
	{
		if (EC_OK != s_pcFastDb->SetDevInfo(tCap.GetDeviceId(),TPuConfig_vidSrcName(i),tCap.deviceCapIndexNames[i].name))
		{
			printf("RedisVidSrcAlias fail,errcode[%d]--kdid[%s] \n",errCode,tCap.deviceId.c_str());
			return false;
			break;
		}
		//gps有变更，且是有效的变更
		if ((tCap.deviceCapIndexNames[i].extData.GetLon(tmpLon) &&  fabs(tmpLon - INVALID_LONGITUDE) >= 0.001) 
			|| (tCap.deviceCapIndexNames[i].extData.GetLat(tmpLan) && fabs(tmpLan - INVALID_LATITUDE) >= 0.001))
		{
			if (EC_OK != s_pcFastDb->SetDevInfo(tCap.GetDeviceId(),Redis_vidgps(i),tCap.deviceCapIndexNames[i].extData.ToXml()))
			{
				printf("RedisVidSrcGps fail,errcode[%d]--kdid[%s] \n",errCode,tCap.deviceId.c_str());
				return false;
				break;
			}
		}
	}
	return true;
}


bool RedisUser2DevPriAdd(const CUserDeviceInfo &cUserDevInfo)
{
	EFDB_ErrorCode errCode = EC_OK;
	errCode = s_pcFastDb->AddUserDevPrivilege(cUserDevInfo);
	if (errCode != EC_OK)
	{
		printf("RedisUser2DevPriAdd fail,errcode[%d]--userid[%s]--kdid[%s] \n",
			errCode,cUserDevInfo.GetUserID().c_str(),cUserDevInfo.GetDeviceID().c_str());
		return false;
	}
	return true;
}

bool RedisUserGrp2DevPriAdd(const CUserGroupDeviceInfo &cUserGrpDevInfo)
{
	EFDB_ErrorCode errCode = EC_OK;
	errCode = s_pcFastDb->AddUsergrpDevPrivilege(cUserGrpDevInfo);
	if (errCode != EC_OK)
	{
		printf("RedisUserGrp2DevPriAdd fail,errcode[%d]--grpid[%s]--kdid[%s] \n",
			errCode,cUserGrpDevInfo.GetGroupID().c_str(),cUserGrpDevInfo.GetDeviceID().c_str());
		return false;
	}
	return true;
}

bool RedisUser2UserGrpAdd(const string &strUserId,const string &strUserGrpId)
{
	EFDB_ErrorCode errCode = EC_OK;
	errCode = s_pcFastDb->SetUserUsergrpId(strUserId,strUserGrpId);
	if (errCode != EC_OK)
	{
		printf("RedisUser2UserGrpAdd fail,errcode[%d]--userid[%s]--usergrpid[%s] \n",
			errCode,strUserId.c_str(),strUserGrpId.c_str());
		return false;
	}
	return true;
}