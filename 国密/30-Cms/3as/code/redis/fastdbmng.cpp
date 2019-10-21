#include "fastdbmng.h"
#include "sdk/fastdbsdk/fastdb.h"
#include "cms/ospext/ospext.h"
#include "inner_common.h"
#include "cms/commonconfig/cmcfg.h"
#include "cms/gbu/gbu_redis.h"
#include <stdlib.h>
#include <math.h>
#include "cms/utility/digital_string.h"
#include "cms/gbs/gbs_redis.h"
#include "config_util/tas_config.h"
static CFastDBInterface *s_pcFastDb = NULL;

void FDBSubscribeCallback(CFastDBInterface */*pInter*/, const std::string &/*strTopic*/, const std::string &/*strMsgData*/, void */*userdata*/);
void FDBRedisStatusCallback(CFastDBInterface */*pInter*/, EFDB_StatusCode /*sc*/, void */*reserve*/, void */*userdata*/);

void FDBRedisLogCallback(int nLevel, const char *strLogMsg, void *uerdata)
{
	GLOBELOG(nLevel, "%s \n", strLogMsg);
}

bool FastDbInit()
{
    s_pcFastDb = new CFastDBInterface();

    FDB_ConnInfo fdbConn;
    fdbConn.pfStatusCallBack = FDBRedisStatusCallback;
    fdbConn.clientname = "tas";
    fdbConn.username = "user";
    fdbConn.password = GetRedisPassword();
	EFDB_ErrorCode errCode = s_pcFastDb->Connect(GetRedisIP(), GetRedisPort(), fdbConn);
    if (EC_OK != errCode)
    {
        GLOBELOG(ERROR_LEV, "FastDbInit failed to connect redis,errcode[%d] ! \n",errCode);
        printf("FastDbInit failed to connect redis,errcode[%d] \n",errCode);
        exit(0); // 先按照最简单的实现
        return false;
    }
    SetLogCallBack(FDBRedisLogCallback,NULL);

    if(!RedisSubscribeDeviceBatchReq())
    {
        GLOBELOG(ERROR_LEV, "RedisSubscribeDeviceBatchReq failed.\n");
        return false;
    }

    return RedisDomainRelationInit();
}

bool IsRedisConnected()
{
	if (!s_pcFastDb)
	{
		return false;
	}
	return s_pcFastDb->IsConnected();
}

void FastDbQuit()
{
    s_pcFastDb->Disconnect();
    delete s_pcFastDb;
}

void FDBSubscribeCallback(CFastDBInterface */*pInter*/, const std::string &strTopic, const std::string &strMsgData, void */*userdata*/)
{
    if (strTopic == FDB_CHN_GBU_DEVICE_CHANGE)
    {
        if(0 != ::OspPost(MAKEIID(AID_CCtrl, CInstance::DAEMON), TAS_DEVICE_BATCH_CHANGE_REQ, strMsgData.c_str(), strMsgData.size()+1))
		{
			GLOBELOG(ERROR_LEV, "FDBSubscribeCallback recv FDB_CHN_GBU_DEVICE_CHANGE topic, but OspPost fail,body[%s] \n",
				strMsgData.c_str()); 
		}
    }
    else
    {
        GLOBELOG(ERROR_LEV, "FDBSubscribeCallback recv unknown topic, topic=%s\n", strTopic.c_str()); 
    }
}

void FDBRedisStatusCallback(CFastDBInterface */*pInter*/, EFDB_StatusCode sc, void */*reserve*/, void */*userdata*/)
{
    if (sc != SC_DISCONNECTED)
    {
        return;
    } 

    GLOBELOG(ERROR_LEV, "FDBRedisStatusCallback detected disconnection ! \n");
    exit(0); // 先按照最简单的实现
}

bool RedisSubscribeDeviceBatchReq()
{
	vector<string> subName;
	subName.push_back(FDB_CHN_GBU_DEVICE_CHANGE);
	if(EC_OK != s_pcFastDb->FDBSubscribe(subName, FDBSubscribeCallback, NULL))
	{
		GLOBELOG(ERROR_LEV, "TAS SubRedisData fail ! \n");
		return false;
	}
	else
	{
		GLOBELOG(CRITICAL_LEV, "TAS SubRedisData success ! \n");
		return true;
	}

}

bool RedisPublishDeviceBatchRsp( const string &strGbuId, const CTasDeviceBatchChangeRsp &cRsp )
{
	string strRedisChnName = RedisGetDeviceBatchRspSubChn(strGbuId);
	if( EC_OK != s_pcFastDb->FDBPublish(strRedisChnName, cRsp.ToXml()))
	{
		GLOBELOG(ERROR_LEV, "RedisPublishDeviceBatchRsp pub CTasDeviceBatchChangeRsp[%s-%d] fail!\n ",
			strRedisChnName.c_str(), cRsp.taskno);
		return false;
	}
	return true;
}

void GetDevVideoGpsNtf(const TDeviceCapName &tDevCapName,vector<int> &index,CTasDevVideoGpsNtf &cDevVideoGpsNtf)
{
	TTasDevVideoGpsItem tDevVideoGrpItem;
	tDevVideoGrpItem.devId = tDevCapName.GetDeviceId();
	tDevVideoGrpItem.domainId = tDevCapName.GetDevDomainId();
	for (int i=0;i<index.size();i++)
	{
		tDevVideoGrpItem.idx = index[i];
		for (int j = 0; j<tDevCapName.deviceCapIndexNames.size(); ++j)
		{
			if (tDevCapName.deviceCapIndexNames[j].index == index[i])
			{
				tDevCapName.deviceCapIndexNames[j].extData.GetLat(tDevVideoGrpItem.latitude);
				tDevCapName.deviceCapIndexNames[j].extData.GetLon(tDevVideoGrpItem.longitude);

				cDevVideoGpsNtf.Items.push_back(tDevVideoGrpItem);
			}
		}
	}
}

void GetDevChangeNtf(const int nType,const CDeviceInfo &cDev,CTasDevChangeNtf &cDevChangeNtf)
{
	TTasDevChangeItem tDevChangeItem;
	tDevChangeItem.devId = cDev.GetDeviceUUID();
	tDevChangeItem.domainId = cDev.GetDomainUUID();
	tDevChangeItem.oprType = nType;
	tDevChangeItem.property.insert(pair<string,string>("creator",digital_string(cDev.GetCreater())));
	tDevChangeItem.property.insert(pair<string,string>("owner",cDev.GetOwner()));
	tDevChangeItem.property.insert(pair<string,string>("devtype",digital_string(cDev.GetDeviceType())));
	tDevChangeItem.property.insert(pair<string,string>("domainid",cDev.GetDomainUUID()));
	cDevChangeNtf.Items.push_back(tDevChangeItem);
}

void GetDevVideoNameNtf(const TDeviceCapName &tDevCapName,CTasDevVideoNameNtf &tDevVideoNameNtf)
{
	TTasDevVideoNameItem tDevVideoNameItem;
	CTasDevVideoNameNtf  cDevVideoNameNtf;
	tDevVideoNameItem.devId = tDevCapName.GetDeviceId();
	tDevVideoNameItem.domainId = tDevCapName.GetDevDomainId();
}
void GetDomainChangeNtf(const int nType,const CDomainRelationInfo &cDomainRelationInfo,CTasDomainChangeNtf &cDomainChangeNtf)
{
	cDomainChangeNtf.domainId = cDomainRelationInfo.GetDomainID();
	cDomainChangeNtf.oprType = nType;
}



bool RedisDeviceAdd(const CDeviceInfo &cDev)
{
	EFDB_ErrorCode errCode = EC_OK;
	CTasDevChangeNtf tDevChangeNtf;
	errCode = s_pcFastDb->AddDevice(cDev);
	if (errCode != EC_OK)
	{
		GLOBELOG(ERROR_LEV,"RedisDeviceAdd fail,errcode[%d]--content[%s] \n",errCode,cDev.ToXml().c_str());
		return false;
	}
	GetDevChangeNtf(0,cDev,tDevChangeNtf);
	errCode = s_pcFastDb->FDBPublish(FDB_CHN_TAS_DEV_CHANGE,tDevChangeNtf.ToXml());
	if (errCode != EC_OK)
	{
		GLOBELOG(ERROR_LEV,"RedisDeviceAdd Success,But Publish Fail,errcode[%d]--content[%s] \n",errCode,
			tDevChangeNtf.ToXml().c_str());
		return false;
	}

	GLOBELOG(EVENT_LEV,"Tas Publish DevChangeNtf Success,ChangeNtfInfo[%s] \n",tDevChangeNtf.ToXml().c_str());
	return true;
}

bool RedisDeviceDel(const CDeviceInfo &cDev)
{
	EFDB_ErrorCode errCode = EC_OK;
	CTasDevChangeNtf tDevChangeNtf;
	errCode = s_pcFastDb->DelDevice(cDev.GetDeviceUUID());
	if (errCode != EC_OK)
	{
		GLOBELOG(ERROR_LEV,"RedisDeviceDel fail,errcode[%d]--content[%s] \n",errCode,cDev.ToXml().c_str());
		return false;
	}
	GetDevChangeNtf(1,cDev,tDevChangeNtf);
	errCode = s_pcFastDb->FDBPublish(FDB_CHN_TAS_DEV_CHANGE,tDevChangeNtf.ToXml());
	if (errCode != EC_OK)
	{
		GLOBELOG(ERROR_LEV,"RedisDeviceDel Success,But Publish Fail,errcode[%d]--content[%s] \n",errCode,
			tDevChangeNtf.ToXml().c_str());
		return false;
	}

	GLOBELOG(EVENT_LEV,"Tas Publish DevChangeNtf Success,ChangeNtfInfo[%s] \n",tDevChangeNtf.ToXml().c_str());
	return true;
}

bool RedisDeviceMod(const CDeviceInfo &cDev, const set<TChangedField>& changeFields)
{
	EFDB_ErrorCode errCode = EC_OK;
	CTasDevChangeNtf tDevChangeNtf;
	errCode = s_pcFastDb->SetDeviceInfo(cDev.GetDeviceUUID(),cDev,ALL_CDeviceInfo);
	if (errCode != EC_OK)
	{
		GLOBELOG(ERROR_LEV,"RedisDeviceMod fail,errcode[%d]--content[%s] \n",errCode,cDev.ToXml().c_str());
		return false;
	}
	GetDevChangeNtf(2,cDev,tDevChangeNtf);
	if (!tDevChangeNtf.Items.empty())
	{
		tDevChangeNtf.Items.back().changedFields = changeFields;
	}
	errCode = s_pcFastDb->FDBPublish(FDB_CHN_TAS_DEV_CHANGE,tDevChangeNtf.ToXml());
	if (errCode != EC_OK)
	{
		GLOBELOG(ERROR_LEV,"RedisDeviceMod Success,But Publish Fail,errcode[%d]--content[%s] \n",errCode,
			tDevChangeNtf.ToXml().c_str());
		return false;
	}

	GLOBELOG(EVENT_LEV,"Tas Publish DevChangeNtf Success,ChangeNtfInfo[%s] \n",tDevChangeNtf.ToXml().c_str());
	return true;
}

bool RedisVidSrcAliasOrGpsSet(const TDeviceCapName &tCap)
{
	EFDB_ErrorCode errCode = EC_OK;
	vector<int> vecChangedGps;//用于标记gps变更的通道index
	double tmpLan,tmpLon;
	CTasDevVideoNameNtf cDevVideoNameNtf;
	CTasDevVideoGpsNtf  cDevVideoGpsNtf;
	if (tCap.capId != DEVICE_CAP_VIDEOSOURCE)//不是视频源能力
	{
		return false;
	}


	for (u32 i= 0;i < tCap.deviceCapIndexNames.size();i++)
	{
		errCode = s_pcFastDb->SetDevInfo(tCap.GetDeviceId(),TPuConfig_vidSrcAlias(tCap.deviceCapIndexNames[i].index),tCap.deviceCapIndexNames[i].name);
		if (EC_OK != errCode)
		{
			GLOBELOG(ERROR_LEV,"RedisVidSrcAlias fail,errcode[%d]--content[%s] \n",errCode,tCap.ToXml().c_str());
			return false;
			break;
		}
		//gps有变更，且是有效的变更
		if ((tCap.deviceCapIndexNames[i].extData.GetLon(tmpLon) &&  fabs(tmpLon - INVALID_LONGITUDE) >= 0.001) 
			|| (tCap.deviceCapIndexNames[i].extData.GetLat(tmpLan) && fabs(tmpLan - INVALID_LATITUDE) >= 0.001))
		{
			vecChangedGps.push_back(tCap.deviceCapIndexNames[i].index);//记住有gps变更的通道index
			errCode = s_pcFastDb->SetDevInfo(tCap.GetDeviceId(),Redis_vidgps(tCap.deviceCapIndexNames[i].index),tCap.deviceCapIndexNames[i].extData.ToXml());
			if (EC_OK != errCode)
			{
				GLOBELOG(ERROR_LEV,"RedisVidSrcGps fail,errcode[%d]--content[%s] \n",errCode,tCap.ToXml().c_str());
				return false;
				break;
			}
		}
	}

	//视频源别名变更发布
	GetDevVideoNameNtf(tCap,cDevVideoNameNtf);
	errCode = s_pcFastDb->FDBPublish(FDB_CHN_TAS_DEV_VID_NAME,cDevVideoNameNtf.ToXml());
	if (EC_OK != errCode)
	{
		GLOBELOG(ERROR_LEV,"RedisVidSrcAliasOrGpsSet Success,But Publish VideoName Fail,errcode[%d]--content[%s] \n",
			errCode,cDevVideoNameNtf.ToXml().c_str());
		return false;
	}
	//有经纬度变更，则发布经纬度变更
	if (!vecChangedGps.empty())
	{
		GetDevVideoGpsNtf(tCap,vecChangedGps,cDevVideoGpsNtf);
		errCode = s_pcFastDb->FDBPublish(FDB_CHN_TAS_DEV_VID_GPS,cDevVideoGpsNtf.ToXml());
		if (EC_OK != errCode)
		{
			GLOBELOG(ERROR_LEV,"RedisVidSrcAliasOrGpsSet Success,But Publish VideoGps Fail,errcode[%d]--content[%s] \n",
				errCode,cDevVideoGpsNtf.ToXml().c_str());
			return false;
		}
	}

	GLOBELOG(EVENT_LEV,"Tas Publish DevVideoNameNtf Or DevVideoGpsNtf Success,ViedeoNameNtf[%s]--VideoGpsNtf[%s] \n",
		cDevVideoNameNtf.ToXml().c_str(),cDevVideoGpsNtf.ToXml().c_str());
	return true;
}

bool RedisDomainRelationAdd(const CDomainRelationInfo &cDom)
{
	EFDB_ErrorCode errCode = EC_OK;
	CTasDomainChangeNtf cDomainChangeNtf;
	errCode = s_pcFastDb->AddDomainRelationInfo(cDom.GetDomainID(), cDom);
	if (errCode != EC_OK)
	{
		GLOBELOG(ERROR_LEV,"RedisDomainRelationAdd fail,errcode[%d]--content[%s] \n",errCode,cDom.ToXml().c_str());
		return false;
	}

	GetDomainChangeNtf(0,cDom,cDomainChangeNtf);
	errCode = s_pcFastDb->FDBPublish(FDB_CHN_TAS_DOMAIN_CHANGE,cDomainChangeNtf.ToXml());
	if (errCode != EC_OK)
	{
		GLOBELOG(ERROR_LEV,"RedisDomainRelationAdd Success,But Publish Fail,errcode[%d]--content[%s] \n",errCode,
			cDomainChangeNtf.ToXml().c_str());
		return false;
	}

	GLOBELOG(EVENT_LEV,"Tas Publish DoaminChangeNtf Success,DomainChangeNtf[%s] \n",cDomainChangeNtf.ToXml().c_str());
	return true;
}

bool RedisDomainRelationDel(const CDomainRelationInfo &cDom)
{
	EFDB_ErrorCode errCode = EC_OK;
	CTasDomainChangeNtf cDomainChangeNtf;
	errCode = s_pcFastDb->DelDomainRelationInfo(cDom.GetDomainID());
	if (errCode != EC_OK)
	{
		GLOBELOG(ERROR_LEV,"RedisDomainRelationDel fail,errcode[%d]--content[%s] \n",errCode,cDom.ToXml().c_str());
		return false;
	}
	GetDomainChangeNtf(1,cDom,cDomainChangeNtf);
	errCode = s_pcFastDb->FDBPublish(FDB_CHN_TAS_DOMAIN_CHANGE,cDomainChangeNtf.ToXml());
	if (errCode != EC_OK)
	{
		GLOBELOG(ERROR_LEV,"RedisDomainRelationDel Success,But Publish Fail,errcode[%d]--content[%s] \n",
			errCode,cDomainChangeNtf.ToXml().c_str());
		return false;
	}

	//开始删除该域下面所有的设备
	vector<string> vecDevUriList;
	if (EC_OK != s_pcFastDb->GetDeviceIDList(vecDevUriList))
	{
		GLOBELOG(ERROR_LEV,"Redis GetDeviceIDList fail \n");
		return false;
	}

	CDeviceInfo cDevInfo;
	for (vector<string>::const_iterator it = vecDevUriList.begin();it != vecDevUriList.end();it++)
	{
		s_pcFastDb->GetDeviceInfo(*it,cDevInfo,CDeviceInfo_domainId);
		if (cDevInfo.GetDomainUUID() == cDom.GetDomainID())//是删除域的设备
			s_pcFastDb->DelDevice(*it);
		else
			continue;
	}


	GLOBELOG(EVENT_LEV,"Tas Publish DoaminChangeNtf Success,DomainChangeNtf[%s] \n",cDomainChangeNtf.ToXml().c_str());
	return true;
}

bool RedisDomainRelationMod(const CDomainRelationInfo &cDom)
{
	EFDB_ErrorCode errCode = EC_OK;
	CTasDomainChangeNtf cDomainChangeNtf;
	errCode = s_pcFastDb->SetDomainRelationInfo(cDom.GetDomainID(),cDom,ALL_CDomainRelationInfo);
	if (errCode != EC_OK)
	{
		GLOBELOG(ERROR_LEV,"RedisDomainRelationMod fail,errcode[%d]--content[%s] \n",errCode,cDom.ToXml().c_str());
		return false;
	}
	GetDomainChangeNtf(2,cDom,cDomainChangeNtf);
	errCode = s_pcFastDb->FDBPublish(FDB_CHN_TAS_DOMAIN_CHANGE,cDomainChangeNtf.ToXml());
	if (errCode != EC_OK)
	{
		GLOBELOG(ERROR_LEV,"RedisDomainRelationMod Success,But Publish Fail,errcode[%d]--content[%s] \n",errCode,
			cDomainChangeNtf.ToXml().c_str());
		return false;
	}

	GLOBELOG(EVENT_LEV,"Tas Publish DoaminChangeNtf Success,DomainChangeNtf[%s] \n",cDomainChangeNtf.ToXml().c_str());
	return true;
}

bool RedisDomainRelationInit( void )
{
    EFDB_ErrorCode errCode = EC_OK;

    vector<UUIDString> vecDomain;
    errCode = s_pcFastDb->GetDomainIDList(vecDomain);
    if (errCode != EC_OK)
    {
        GLOBELOG(ERROR_LEV,"RedisDomainRelationInit GetDomainIDList failed,errcode[%d]\n",errCode);
        return false;
    }
    for (vector<UUIDString>::const_iterator it = vecDomain.begin(); it != vecDomain.end(); ++it)
    {
        if (*it == CurrentDomainId())
        {
            return true;
        }
    }

    CDomainRelationInfo cDom;
    cDom.SetDomainID(CurrentDomainId());
    cDom.SetDomainName(CurrentDomainName());

    errCode = s_pcFastDb->AddDomainRelationInfo(cDom.GetDomainID(), cDom);
    if (errCode != EC_OK)
    {
        GLOBELOG(ERROR_LEV,"RedisDomainRelationInit AddDomainRelationInfo fail,errcode[%d]\n",errCode);
        return false;
    }

    return true;
}

bool RedisAddUserDevPrivilege( IN const CUserDeviceInfo &devPriv )
{
	EFDB_ErrorCode errCode = EC_OK;
	errCode = s_pcFastDb->AddUserDevPrivilege(devPriv);
	if (EC_OK != errCode)
	{
		GLOBELOG(ERROR_LEV,"RedisAddUserDevPrivilege fail,errcode[%d]\n",errCode);
		return false;
	}
	CTasUserDevPrivilegeChangeNtf cNtf;
	cNtf.SetOprType(0);
	cNtf.SetContent(devPriv.ToXml());
	errCode = s_pcFastDb->FDBPublish(FDB_CHN_TAS_USER_DEVPRIVILEGE_CHANGE, cNtf.ToXml());
	if (EC_OK != errCode)
	{
		GLOBELOG(ERROR_LEV,"RedisAddUserDevPrivilege FDBPublish fail,errcode[%d]\n",errCode);
		return false;
	}
	return true;
}

bool RedisModUserDevPrivilege( IN const CUserDeviceModReq &devPriv )
{
	EFDB_ErrorCode errCode = EC_OK;
	errCode = s_pcFastDb->ModUserDevPrivilege(devPriv);
	if (EC_OK != errCode)
	{
		GLOBELOG(ERROR_LEV,"RedisModUserDevPrivilege fail,errcode[%d]\n",errCode);
		return false;
	}
	CTasUserDevPrivilegeChangeNtf cNtf;
	cNtf.SetOprType(2);
	cNtf.SetContent(devPriv.ToXml());
	errCode = s_pcFastDb->FDBPublish(FDB_CHN_TAS_USER_DEVPRIVILEGE_CHANGE, cNtf.ToXml());
	if (EC_OK != errCode)
	{
		GLOBELOG(ERROR_LEV,"RedisModUserDevPrivilege FDBPublish fail,errcode[%d]\n",errCode);
		return false;
	}
	return true;
}

bool RedisDelUserDevPrivilege( IN const CUserDeviceInfoKey &devPriv )
{
	EFDB_ErrorCode errCode = EC_OK;
	errCode = s_pcFastDb->DelUserDevPrivilege(devPriv);
	if (EC_OK != errCode)
	{
		GLOBELOG(ERROR_LEV,"RedisDelUserDevPrivilege fail,errcode[%d]\n",errCode);
		return false;
	}
	CTasUserDevPrivilegeChangeNtf cNtf;
	cNtf.SetOprType(1);
	cNtf.SetContent(devPriv.ToXml());
	errCode = s_pcFastDb->FDBPublish(FDB_CHN_TAS_USER_DEVPRIVILEGE_CHANGE, cNtf.ToXml());
	if (EC_OK != errCode)
	{
		GLOBELOG(ERROR_LEV,"RedisDelUserDevPrivilege FDBPublish fail,errcode[%d]\n",errCode);
		return false;
	}
	return true;
}

bool RedisAddUsergrpDevPrivilege( IN const CUserGroupDeviceInfo &devPriv )
{
	EFDB_ErrorCode errCode = EC_OK;
	errCode = s_pcFastDb->AddUsergrpDevPrivilege(devPriv);
	if (EC_OK != errCode)
	{
		GLOBELOG(ERROR_LEV,"RedisAddUsergrpDevPrivilege fail,errcode[%d]\n",errCode);
		return false;
	}
	CTasUserGrpDevPrivilegeChangeNtf cNtf;
	cNtf.SetOprType(0);
	cNtf.SetContent(devPriv.ToXml());
	errCode = s_pcFastDb->FDBPublish(FDB_CHN_TAS_USERGRP_DEVPRIVILEGE_CHANGE, cNtf.ToXml());
	if (EC_OK != errCode)
	{
		GLOBELOG(ERROR_LEV,"RedisAddUsergrpDevPrivilege FDBPublish fail,errcode[%d]\n",errCode);
		return false;
	}
	return true;
}

bool RedisModUsergrpDevPrivilege( IN const CUserGroupDeviceModReq &devPriv )
{
	EFDB_ErrorCode errCode = EC_OK;
	errCode = s_pcFastDb->ModUsergrpDevPrivilege(devPriv);
	if (EC_OK != errCode)
	{
		GLOBELOG(ERROR_LEV,"RedisModUsergrpDevPrivilege fail,errcode[%d]\n",errCode);
		return false;
	}
	CTasUserGrpDevPrivilegeChangeNtf cNtf;
	cNtf.SetOprType(2);
	cNtf.SetContent(devPriv.ToXml());
	errCode = s_pcFastDb->FDBPublish(FDB_CHN_TAS_USERGRP_DEVPRIVILEGE_CHANGE, cNtf.ToXml());
	if (EC_OK != errCode)
	{
		GLOBELOG(ERROR_LEV,"RedisModUsergrpDevPrivilege FDBPublish fail,errcode[%d]\n",errCode);
		return false;
	}
	return true;
}

bool RedisDelUsergrpDevPrivilege( IN const CUserGroupDeviceInfoKey &devPriv )
{
	EFDB_ErrorCode errCode = EC_OK;
	errCode = s_pcFastDb->DelUsergrpDevPrivilege(devPriv);
	if (EC_OK != errCode)
	{
		GLOBELOG(ERROR_LEV,"RedisDelUsergrpDevPrivilege fail,errcode[%d]\n",errCode);
		return false;
	}
	CTasUserGrpDevPrivilegeChangeNtf cNtf;
	cNtf.SetOprType(1);
	cNtf.SetContent(devPriv.ToXml());
	errCode = s_pcFastDb->FDBPublish(FDB_CHN_TAS_USERGRP_DEVPRIVILEGE_CHANGE, cNtf.ToXml());
	if (EC_OK != errCode)
	{
		GLOBELOG(ERROR_LEV,"RedisDelUsergrpDevPrivilege FDBPublish fail,errcode[%d]\n",errCode);
		return false;
	}
	return true;
}

bool RedisSetUserUsergrpId( IN const UUIDString &userId, IN const UUIDString &usergrpId )
{
	EFDB_ErrorCode errCode = EC_OK;
	errCode = s_pcFastDb->SetUserUsergrpId(userId, usergrpId);
	if (EC_OK != errCode)
	{
		GLOBELOG(ERROR_LEV,"RedisSetUserUsergrpId fail,errcode[%d]\n",errCode);
		return false;
	}
	return true;
}

bool RedisDelUser( IN const UUIDString &userId )
{
	EFDB_ErrorCode errCode = EC_OK;
	do 
	{
		errCode = s_pcFastDb->DelUser(userId);
		if (EC_OK == errCode)
		{
			return true;
		}
		else if(EC_AGAIN != errCode)
		{
			GLOBELOG(ERROR_LEV,"RedisDelUser fail,errcode[%d]\n",errCode);
			return false;
		}

	} while (EC_AGAIN == errCode);

    GLOBELOG(ERROR_LEV,"RedisDelUser fail2,errcode[%d]\n",errCode);
    return false;
}

bool RedisDelUsergrp( IN const UUIDString &usergrpId )
{
	EFDB_ErrorCode errCode = EC_OK;
	do 
	{
		errCode = s_pcFastDb->DelUsergrp(usergrpId);
		if (EC_OK == errCode)
		{
			return true;
		}
		else if (EC_AGAIN != errCode)
		{
			GLOBELOG(ERROR_LEV,"RedisDelUsergrp fail,errcode[%d]\n",errCode);
			return false;
		}
	} while (EC_AGAIN == errCode);

	GLOBELOG(ERROR_LEV,"RedisDelUsergrp fail2,errcode[%d]\n",errCode);
    return false;
}

bool IsDevVidSrcOnline( const UUIDString& devId, const int vidIdx )
{
	EFDB_ErrorCode errCode = EC_OK;
	CRedisPuData cPuData;
	
	if (EC_OK != s_pcFastDb->GetFullPuData(devId,cPuData))
	{
		return false;
	}
	else
	{
		return cPuData.GetIsOnline() && CHNSTATUS_ONLINE == cPuData.GetPuConfig().GetChnStateConfig(vidIdx).GetChnState();
	}
}

bool IsDevEnable(const UUIDString &devId)
{
    EFDB_ErrorCode errCode = EC_OK;
    int dwUseFlag = 0;

    if (EC_OK != s_pcFastDb->GetDevInfo(devId, CDeviceInfo_usedFlag, dwUseFlag))
    {
	    GLOBELOG(ERROR_LEV,"IsDevEnable 从redis中获取设备[devId=%s]启用状态失败，视为enable!\n", devId.c_str());
        return true;
    }
    else
    {
        // 启停状态标志：0停用，非0启用
        return (dwUseFlag != 0);
    }
}

void RedisSetProcessValue4Tas( ENTasProcessType type, const string &id, int process )
{
    EFDB_ErrorCode errCode = EC_OK;
    
    vector<string> fileds;
    fileds.push_back(RedisProcessValueFiled(type, id));
    if(process!=100){
        vector<string> values;
        char szVal[100];
        snprintf(szVal, sizeof(szVal), "%d", process);
        values.push_back(szVal);
        s_pcFastDb->HMSet(FDB_TAS_PROCESS_VALUE_KEY, fileds, values);
    }else{
        s_pcFastDb->HDel(FDB_TAS_PROCESS_VALUE_KEY, fileds);
    }
}

//不需要publish，因为不会被修改
bool RedisSetTasDbInfo(const string& strDbIp, const u16 wDbPort, const string& strDbPassword)
{
    vector<string> fields, values;
    fields.push_back(REDIS_TASDBINFO_IP);
    values.push_back(strDbIp);

    fields.push_back(REDIS_TASDBINFO_PORT);
    values.push_back(digital_string(int(wDbPort)));

    fields.push_back(REDIS_TASDBINFO_PASSWORD);
    values.push_back(strDbPassword);


    if (EC_OK != s_pcFastDb->HMSet(REDIS_TASDBINFO_KEY,fields, values));
	{
		return false;
	}

    return true;
}

int GetMaxCivilcodeLev()
{
    if (CTasConfig::CanMaxCivilCodeLevUp2Five())
    {
        return 5;
    }
    else
    {
        return 4;   // 默认为4级
    }
}

