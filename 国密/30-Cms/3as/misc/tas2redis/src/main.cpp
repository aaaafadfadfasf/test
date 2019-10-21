/*****************************************************************************
   模块名      : tas2redis
   文件名      : tas2redis
   相关文件    : 
   文件实现功能: tas与redis中数据同步
   作者        : 汪辉
   版本        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人      修改内容
   2016/03/11   1.0         汪辉        创建
   2016/7/28                汪辉        加入用户和用户组对设备的权限写入
******************************************************************************/
#include <iostream>
#include "toolsdata.h"
#include "dboperate.h"
#include "redis.h"
#include <stdio.h>  
#include <stdlib.h> 
#include<sys/types.h>
#include "cms/commonconfig/cmcfg.h"
#include "cms/utility/cmstime.h"


int main(int argc,char *argv[])
{
	CDbOperate *pcOper = new CDbOperate();
	if (!pcOper->InitDB())
	{
		printf("数据库初始化失败,程序退出! \n");
		return 1;
	}

	if (!ReadCommonConfig())
	{
		printf("读取公共配置文件失败! \n");
		return 1;
	}

	if (!FastDbInit())
	{
		printf("redis初始换失败，程序退出! \n");
		return 1;
	}
		printf("Begin****************,  %s\n", CCMSTime().ToString().c_str());
	vector<CDomainRelationInfo> vec3asDomains;
	pcOper->QryAllDomainRelation(vec3asDomains);

	vector<TExtDeviceInfo> tDev;
	pcOper->QryAllDevs(tDev);
	printf("QUERY DEVICE INFO  END *************,  %s\n", CCMSTime().ToString().c_str());
	for (vector<TExtDeviceInfo>::iterator it = tDev.begin();it != tDev.end();it++)
	{
		pcOper->FullDeviceInfo(*it);
		g_cDeviceTable.AddDevice(*it);
	}
	printf("QUERY DeviceModel And TblDeviceCapIndexDate by devId END********,  %s\n", CCMSTime().ToString().c_str());
	//获取用户对设备权限
	vector<CUserDeviceInfo> vec3asUserDevInfo;
	pcOper->QryAllUserDevInfo(vec3asUserDevInfo);

	printf("Get User Device Privilege End**********,  %s\n", CCMSTime().ToString().c_str());
	//获取用户分组对设备的权限
	vector<CUserGroupDeviceInfo> vec3asUserGroupDevInfo;
	pcOper->QryAllUserGrpDevInfo(vec3asUserGroupDevInfo);
	printf("Get User Group  Device Privilege End**********,  %s\n", CCMSTime().ToString().c_str());
	//获取用户属于用户组的关系
	vector<CUserInfo> vec3asUser2UserGrp;
	pcOper->QryAllUsers(vec3asUser2UserGrp);

	vector<string> vecRedisDomains;
	GetAllDoaminList(vecRedisDomains);
	vector<TExtDeviceInfo> vec3asDevs;
	set<string> set3asDevs;
	vector<string> vecRedisDevs;
	set<string> setRedisDes;
	g_cDeviceTable.GetAllDevice(vec3asDevs);
	GetAllDevList(vecRedisDevs);
	
	for (vector<string>::iterator it = vecRedisDevs.begin();it != vecRedisDevs.end();it++)
	{
		setRedisDes.insert(*it);
	}
		
	for (vector<TExtDeviceInfo>::iterator itDev = vec3asDevs.begin();itDev != vec3asDevs.end();itDev++)
	{
		set3asDevs.insert(itDev->deviceInfo.GetDeviceUUID());
	}
	printf("获取用户属于用户组的关系结束**********,  %s\n", CCMSTime().ToString().c_str());	
	//3as数据库存在，redis中不存在的domain，redis中需要添加	
	for (vector<CDomainRelationInfo>::iterator it = vec3asDomains.begin();
		it != vec3asDomains.end();it++)
	{
		if (vecRedisDomains.end() == find(vecRedisDomains.begin(),vecRedisDomains.end(),it->GetDomainID()))
		{
			printf("domain[%s] exist in 3as,but not found in redis,need add!!! \n",it->GetDomainID().c_str());
			if (!RedisDomainAdd(*it))
			{
				printf("add domain[%s]##content[%s] domainInfo to redis fail!!! \n",
					it->GetDomainID().c_str(),it->ToXml().c_str());
			}
		}
	}
	printf("3as数据库存在，redis中不存在的domain，redis中需要添加，结束**********,  %s\n", CCMSTime().ToString().c_str());	
	//redis中存在，3as不存在的domain，redis中需要删除
	for (vector<string>::iterator it = vecRedisDomains.begin();it != vecRedisDomains.end();it++)
	{
		vector<CDomainRelationInfo>::iterator it2 = vec3asDomains.begin();
		for (;it2 != vec3asDomains.end();it2++)
		{
			if (*it == it2->GetDomainID())
				break;
		}
		if (it2 == vec3asDomains.end())
		{
			printf("domain[%s] exist in redis,but not found in 3as,need del!!! \n",it->c_str());
			if (!RedisDomainDel(*it))
			{
				printf("del domain[%s] to redis fail!!! \n",it->c_str());
			}
		}
	}
	printf("redis中存在，3as不存在的domain，redis中需要删除，结束**********,  %s\n", CCMSTime().ToString().c_str());	
	//3as数据库存在，redis中不存在的设备，redis中需要添加
	for (vector<TExtDeviceInfo>::iterator it = vec3asDevs.begin();it != vec3asDevs.end();it++)
	{
		if (setRedisDes.end() == setRedisDes.find(it->deviceInfo.GetDeviceUUID()))
		{
			printf("dev[%s] exist in 3as,but not found in redis,need add!!! \n",it->deviceInfo.GetDeviceUUID().c_str());
			//添加设备，修改视频源别名，gps信息
			if (!RedisDeviceAdd(it->deviceInfo))
			{
				printf("add dev[%s]##content[%s] deviceinfo to redis fail!!! \n",
					it->deviceInfo.GetDeviceUUID().c_str(),it->deviceInfo.ToXml().c_str());
			}
			if (!RedisVidSrcAliasOrGpsSet(it->deviceCapName))
			{
				printf("add dev[%s]##content[%s] VidSrcAliasOrGpsSet to redis fail!!! \n",
					it->deviceInfo.GetDeviceUUID().c_str(),it->deviceCapName.ToXml().c_str());
			}
		}
	}
	printf("3as数据库存在，redis中不存在的设备，redis中需要添加，结束**********,  %s\n", CCMSTime().ToString().c_str());	
	//redis中存在，3as不存在的设备，redis中需要删除
	for (vector<string>::iterator it = vecRedisDevs.begin();it != vecRedisDevs.end();it++)
	{
		if (set3asDevs.end() == set3asDevs.find(*it))
		{
			printf("dev[%s] exist in redis,but not found in 3as,need del!!! \n",it->c_str());
			if (!RedisDeviceDel(*it))
			{
				printf("del dev[%s] to redis fail!!! \n",it->c_str());
			}
		}
	}
	printf("redis中存在，3as不存在的设备，redis中需要删除，结束**********,  %s\n", CCMSTime().ToString().c_str());	
	//添加用户对设备的权限
	for (vector<CUserDeviceInfo>::iterator it_c = vec3asUserDevInfo.begin();it_c != vec3asUserDevInfo.end();it_c++)
	{
		if (!RedisUser2DevPriAdd(*it_c))
		{		
			printf("add user[%s]##content[%s] CUserDeviceInfo to redis fail!!! \n",
				it_c->GetUserID().c_str(),it_c->ToXml().c_str());
		}
	}
	printf("redis中添加用户对设备的权限，结束**********,  %s\n", CCMSTime().ToString().c_str());
	//添加用户组对设备的权限
	for (vector<CUserGroupDeviceInfo>::iterator it_c = vec3asUserGroupDevInfo.begin();it_c != vec3asUserGroupDevInfo.end();it_c++)
	{
		if (!RedisUserGrp2DevPriAdd(*it_c))
		{		
			printf("add grpid[%s]##content[%s] CUserGroupDeviceInfo to redis fail!!! \n",
				it_c->GetGroupID().c_str(),it_c->ToXml().c_str());
		}
	}
	printf("添加用户组对设备的权限，结束**********,  %s\n", CCMSTime().ToString().c_str());
	//设备用户属于的用户组关系
	for (vector<CUserInfo>::iterator it_c = vec3asUser2UserGrp.begin();it_c != vec3asUser2UserGrp.end();it_c++)
	{
		if (!RedisUser2UserGrpAdd(it_c->GetUserID(),it_c->GetUsergrpID()))
		{
			printf("add grpid[%s]##content[%s] User2UserGrp to redis fail!!! \n",
				it_c->GetUserID().c_str(),it_c->ToXml().c_str());
		}
	}
	printf("添加设备用户属于的用户组关系，结束**********,  %s\n", CCMSTime().ToString().c_str());

    FastDbQuit();
	printf("tas 与 redis 数据同步结束 \n");
	
	return 0;
}