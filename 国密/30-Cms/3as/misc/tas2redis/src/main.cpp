/*****************************************************************************
   ģ����      : tas2redis
   �ļ���      : tas2redis
   ����ļ�    : 
   �ļ�ʵ�ֹ���: tas��redis������ͬ��
   ����        : ����
   �汾        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
   2016/03/11   1.0         ����        ����
   2016/7/28                ����        �����û����û�����豸��Ȩ��д��
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
		printf("���ݿ��ʼ��ʧ��,�����˳�! \n");
		return 1;
	}

	if (!ReadCommonConfig())
	{
		printf("��ȡ���������ļ�ʧ��! \n");
		return 1;
	}

	if (!FastDbInit())
	{
		printf("redis��ʼ��ʧ�ܣ������˳�! \n");
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
	//��ȡ�û����豸Ȩ��
	vector<CUserDeviceInfo> vec3asUserDevInfo;
	pcOper->QryAllUserDevInfo(vec3asUserDevInfo);

	printf("Get User Device Privilege End**********,  %s\n", CCMSTime().ToString().c_str());
	//��ȡ�û�������豸��Ȩ��
	vector<CUserGroupDeviceInfo> vec3asUserGroupDevInfo;
	pcOper->QryAllUserGrpDevInfo(vec3asUserGroupDevInfo);
	printf("Get User Group  Device Privilege End**********,  %s\n", CCMSTime().ToString().c_str());
	//��ȡ�û������û���Ĺ�ϵ
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
	printf("��ȡ�û������û���Ĺ�ϵ����**********,  %s\n", CCMSTime().ToString().c_str());	
	//3as���ݿ���ڣ�redis�в����ڵ�domain��redis����Ҫ���	
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
	printf("3as���ݿ���ڣ�redis�в����ڵ�domain��redis����Ҫ��ӣ�����**********,  %s\n", CCMSTime().ToString().c_str());	
	//redis�д��ڣ�3as�����ڵ�domain��redis����Ҫɾ��
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
	printf("redis�д��ڣ�3as�����ڵ�domain��redis����Ҫɾ��������**********,  %s\n", CCMSTime().ToString().c_str());	
	//3as���ݿ���ڣ�redis�в����ڵ��豸��redis����Ҫ���
	for (vector<TExtDeviceInfo>::iterator it = vec3asDevs.begin();it != vec3asDevs.end();it++)
	{
		if (setRedisDes.end() == setRedisDes.find(it->deviceInfo.GetDeviceUUID()))
		{
			printf("dev[%s] exist in 3as,but not found in redis,need add!!! \n",it->deviceInfo.GetDeviceUUID().c_str());
			//����豸���޸���ƵԴ������gps��Ϣ
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
	printf("3as���ݿ���ڣ�redis�в����ڵ��豸��redis����Ҫ��ӣ�����**********,  %s\n", CCMSTime().ToString().c_str());	
	//redis�д��ڣ�3as�����ڵ��豸��redis����Ҫɾ��
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
	printf("redis�д��ڣ�3as�����ڵ��豸��redis����Ҫɾ��������**********,  %s\n", CCMSTime().ToString().c_str());	
	//����û����豸��Ȩ��
	for (vector<CUserDeviceInfo>::iterator it_c = vec3asUserDevInfo.begin();it_c != vec3asUserDevInfo.end();it_c++)
	{
		if (!RedisUser2DevPriAdd(*it_c))
		{		
			printf("add user[%s]##content[%s] CUserDeviceInfo to redis fail!!! \n",
				it_c->GetUserID().c_str(),it_c->ToXml().c_str());
		}
	}
	printf("redis������û����豸��Ȩ�ޣ�����**********,  %s\n", CCMSTime().ToString().c_str());
	//����û�����豸��Ȩ��
	for (vector<CUserGroupDeviceInfo>::iterator it_c = vec3asUserGroupDevInfo.begin();it_c != vec3asUserGroupDevInfo.end();it_c++)
	{
		if (!RedisUserGrp2DevPriAdd(*it_c))
		{		
			printf("add grpid[%s]##content[%s] CUserGroupDeviceInfo to redis fail!!! \n",
				it_c->GetGroupID().c_str(),it_c->ToXml().c_str());
		}
	}
	printf("����û�����豸��Ȩ�ޣ�����**********,  %s\n", CCMSTime().ToString().c_str());
	//�豸�û����ڵ��û����ϵ
	for (vector<CUserInfo>::iterator it_c = vec3asUser2UserGrp.begin();it_c != vec3asUser2UserGrp.end();it_c++)
	{
		if (!RedisUser2UserGrpAdd(it_c->GetUserID(),it_c->GetUsergrpID()))
		{
			printf("add grpid[%s]##content[%s] User2UserGrp to redis fail!!! \n",
				it_c->GetUserID().c_str(),it_c->ToXml().c_str());
		}
	}
	printf("����豸�û����ڵ��û����ϵ������**********,  %s\n", CCMSTime().ToString().c_str());

    FastDbQuit();
	printf("tas �� redis ����ͬ������ \n");
	
	return 0;
}