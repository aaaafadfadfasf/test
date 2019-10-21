#include "redisdb.h"
#include "cuidata.h"

string GetSubTypeByRedisSubChnName(const string &chnName)
{
	string typeName;

	string::size_type strPos = chnName.find(REDIS_SEPARATOR);
	if (strPos != string::npos)
	{
		typeName.assign(chnName, strPos + 1, string::npos);
	}
	return typeName;
}

string GetDevIdByRedisSubChnName(const string &chnName)
{
	string typeName;

	string::size_type strPos = chnName.find(REDIS_SEPARATOR);
	if (strPos != string::npos)
	{
		typeName.assign(chnName, 0, strPos);
	}
	return typeName;
}

CRedisDBInterface::CRedisDBInterface()
{

}

CRedisDBInterface::~CRedisDBInterface()
{

}

EFDB_ErrorCode CRedisDBInterface::GetDevNameAndVSAlaias(const UUIDString &devId, int vsChnNo, string &strDevName, string &strVidSrcAlaias, string &strErrMsg)
{
	if (devId.empty() || vsChnNo <= INVALID_VIDEOSOURCE_ID)
	{
		stringstream ss;
		ss << vsChnNo;
		strErrMsg = "参数错误 设备ID[" + devId + "]" + " 视频源ID[" + ss.str() + "]";
		return EC_INVALID_PARAM;
	}

	CDeviceInfo cDeviceInfo;
	EFDB_ErrorCode er = GetDeviceInfo(devId, cDeviceInfo, CDeviceInfo_name);
	if ( !IsFdbResultPositive(er) )
	{
		strErrMsg = "无法获取设备名称?已退网了吗？";
	}
	strDevName = cDeviceInfo.GetDeviceName();
	
	std::map<std::string, std::string> mapVsName;
	string vsfield = TPuConfig_vidSrcAlias(vsChnNo);
	std::vector<std::string> vsfieldList;
	vsfieldList.push_back(vsfield);

	er =  HMGet(MAKE_DEV_KEY(devId), vsfieldList, mapVsName);
	if ( !IsFdbResultPositive(er) )
	{
		stringstream ss;
		ss << er;
		strErrMsg += "无法获取设备的视频源别名 HMGet key[" + MAKE_DEV_KEY(devId) +"]field[" + vsfield + "] er[" + ss.str() + "]";
	}
	else
	{
		std::map<std::string, std::string>::const_iterator iter = mapVsName.find(vsfield);
		if (iter != mapVsName.end())
		{
			strVidSrcAlaias = iter->second;
		}
	}

	if (strVidSrcAlaias.empty())
	{
		if (strDevName.empty())
		{
			strErrMsg += "设备名称是空的？";
			return EC_NOT_EXISTED;
		}

		stringstream ss;
		ss << vsChnNo;
		strVidSrcAlaias = strDevName + "_" + ss.str();
	}

	return EC_OK;
}

EFDB_ErrorCode CRedisDBInterface::GetUserVideoPriByEncChn(const UUIDString &userId, const UUIDString &devId, int chnNo, CVideosourcePrivilege &priv, string &strErrMsg)
{
	if (userId.empty()
		|| devId.empty()
		|| chnNo <= INVALID_VIDEOSOURCE_ID)
	{
		stringstream ss;
		ss << chnNo;
		strErrMsg = "参数错误 :用户id[" + userId + "] 设备id[" + devId + "] 视频源/通道id[" + ss.str() + " < 0]";
		return EC_INVALID_PARAM;
	}

	TChannel tChannel(devId, chnNo);

	// 记录一些不是立刻返回的失败或错误
	string moreErrorMsg;

	// 先取配置信息, 通道视频源映射关系
	vector<string> fields;
	fields.push_back(TPuConfig_devName);
	fields.push_back(TPuConfig_chnVidSrcSet(chnNo));

	int vsIndex = INVALID_VIDEOSOURCE_ID;
	TPuConfig tPuConfig;
	EFDB_ErrorCode er = GetPuConfig(devId, tPuConfig, fields);
	if (( IsFdbResultPositive(er) ) && tPuConfig.IsCompleted())
	{
		map<int, CVidSrcSet> &chnVidSrcSet = tPuConfig.GetChnVidSrcSet();
		map<int, CVidSrcSet>::const_iterator iter = chnVidSrcSet.find(chnNo);
		if (iter != chnVidSrcSet.end())
		{
			if (!iter->second.empty())
			{
				// 一个通道只能对应一个视频源
				vsIndex = *iter->second.begin();
			}
		}
		else
		{
			strErrMsg = "获取到了设备配置信息，但无法取得通道对应的视频源id!";
			return EC_NOT_EXISTED;
		}

		if (vsIndex > INVALID_VIDEOSOURCE_ID)
		{
			// 取到了视频源ID,直接获取视频源权限
			er = GetUserVideoPrivilege(userId, devId, vsIndex, priv);
			//注意：之前 从3a获取 未获取完视为 true，
            //     现在从reids 获取，无需考虑未获取完的情况
            if ( !IsFdbResultPositive(er) )
			{
				stringstream ss;
				ss << vsIndex;
				strErrMsg = "获取到了设备配置信息，但无法取得视频源["+ ss.str() +"]对应的权限!";
			}
			return er;
		}
		else
		{
			strErrMsg = "获取到了设备配置信息，但通道对应的视频源ID是无效的!";
			return EC_NOT_EXISTED;
		}
	}

	// 没有取到设备配置信息中的 通道视频源对应关系
	// 从入网信息判断获取对应的视频源
	fields.clear();

	TPuBaseCapSet puBaseCapSet;
	fields.push_back(TPuBaseCapSet_videoInPortNum);
	fields.push_back(TPuBaseCapSet_videoEncNum);

	er = GetPuBaseCapSet(devId, puBaseCapSet, fields);
	if ( !IsFdbResultPositive(er) )
	{
		strErrMsg = "无法获取设备基本信息 视频源数量、编码通道数量!";
		return er;
	}

	int totalEncChnNum = puBaseCapSet.GetVidEncNum();
	int totalVsNum = puBaseCapSet.GetVideoInPortNum();

	if (totalEncChnNum <= 0
		|| totalVsNum <= 0)
	{
		stringstream sslog;
		sslog << "取到的设备信息不正确," << "视频源总数[" << totalVsNum << "] 编码通道总数[" << totalEncChnNum << "]";
		strErrMsg = sslog.str();
		return EC_NOT_EXISTED;
	}

	if (chnNo >= totalEncChnNum)
	{
		stringstream sslog;
		sslog << "查询的编码通道id[" << chnNo << "] 大于设备编码通道总数[" << totalEncChnNum << "]";
		strErrMsg = sslog.str();
		return EC_NOT_EXISTED;
	}

	if (chnNo < totalVsNum)
	{
		vsIndex = chnNo;
	}
	else
	{
		int remainder = totalEncChnNum % totalVsNum;
		if (remainder == 0)
		{
			// 取余数为0，则主辅流是1:1/1:2/1:3 ...
			vsIndex = chnNo%totalVsNum;
		}
		else
		{
			remainder = (totalEncChnNum + 1) % totalVsNum;
			if (remainder == 0)
			{
				// SVR合成通道,第一个视频源没有辅流通道(只考虑只有1个辅流的情况)
				vsIndex = (chnNo / totalVsNum) + (chnNo % totalVsNum);
			}
			else
			{
				// 其他情况就不关心了，直接取余，错了就错了。
				vsIndex == INVALID_VIDEOSOURCE_ID;
				strErrMsg = "查询的是通道，但无法转换成视频源ID!";
				return EC_INVALID_PARAM;
			}
		}
	}

	if (vsIndex <= INVALID_VIDEOSOURCE_ID)
	{
		CMS_ASSERT(false && "Get VsIndex Error!");
		strErrMsg = "查询的是通道，但无法转换成视频源ID!";
		return EC_INVALID_PARAM;
	}

	er = GetUserVideoPrivilege(userId, devId, vsIndex, priv);
	if ( !IsFdbResultPositive(er) )
	{
		strErrMsg = "查询视频源权限失败，可能没有划归该视频源!";
	}
	return er;
}

bool CRedisDBInterface::IsUserHasDev(const UUIDString &userId, const UUIDString &devId)
{
	if (userId.empty()
		|| devId.empty())
	{
		return false;
	}

	//CEncoderPrivilege cPri;
	//EFDB_ErrorCode er = GetUserEncoderPrivilege(userId, devId, cPri);
	CBitsContainer cPri;
	EFDB_ErrorCode er = GetUserDevicePrivilege(userId, devId, cPri);
	return IsFdbResultPositive(er) ;
}

bool CRedisDBInterface::IsUserHasVideoSource(const UUIDString &userId, const UUIDString &devId, int nChnNo, string &strErrMsg)
{
	if (userId.empty()
		|| devId.empty()
		|| nChnNo <= INVALID_VIDEOSOURCE_ID)
	{
		return false;
	}

	CVideosourcePrivilege cPri;
	EFDB_ErrorCode er = GetUserVideoPriByEncChn(userId, devId, nChnNo, cPri, strErrMsg);
	if ( !IsFdbResultPositive(er) )
	{
		stringstream sslog;
		sslog << strErrMsg << " redis er:"<< er;
		strErrMsg = sslog.str();
	}
	
	return  IsFdbResultPositive(er) ;
}

EFDB_ErrorCode CRedisDBInterface::GetUserFullDevicefullinfoAndPrivilege(const UUIDString userId, const UUIDString &devId, const bool &bIsAdmin, TDevicefullinfoAndPrivilege &tDevFullInfoAndPri, string &strErrMsg)
{
	if (userId.empty() || devId.empty())
	{
		return EC_INVALID_PARAM;
	}

	EFDB_ErrorCode er = GetFullDeviceInfo(devId, tDevFullInfoAndPri.deviceInfo);
	if ( !IsFdbResultPositive(er) )
	{
		strErrMsg = "无法获取设备的完整信息";
		return er;
	}

	// 设备权限
	if (bIsAdmin)
	{
		// admin账户，redis中没有设备相关的权限信息k
		tDevFullInfoAndPri.devicePrivilege.ConvertFromInt(-1);

		TPuBaseCapSet tPuBaseCapSet;
		er = GetPuBaseCapSet(devId, tPuBaseCapSet, TPuBaseCapSet_videoInPortNum);
		if ( !IsFdbResultPositive(er) )
		{
			strErrMsg = "admin账户 无法获取设备的视频源个数 GetFullPuBaseCapSet";
			return er;
		}

		for (int nVsIndex = 0; nVsIndex < tPuBaseCapSet.GetVideoInPortNum(); nVsIndex++)
		{
			TUserVideoSourceCapInfo tUserVideoSourceCapInfo;

			CVideosourcePrivilege cVsPri;
			cVsPri.SetPTZCtrl(CMS_PTZ_MAX_PRI_LEV);
			cVsPri.ConvertFromInt(-1);

			tUserVideoSourceCapInfo.privilege = cVsPri;

			std::map<std::string, std::string> mapVsName;
			string vsfield = TPuConfig_vidSrcAlias(nVsIndex);
			std::vector<std::string> vsfieldList;
			vsfieldList.push_back(vsfield);
			er = HMGet(MAKE_DEV_KEY(devId), vsfieldList, mapVsName);
			if ( !IsFdbResultPositive(er) )
			{
				stringstream ss;
				ss << er;
				strErrMsg += "无法获取设备的视频源别名 HMGet key[" + MAKE_DEV_KEY(devId) + "]field[" + vsfield + "] er[" + ss.str() + "]";
			}
			else
			{
				std::map<std::string, std::string>::const_iterator iter = mapVsName.find(vsfield);
				if (iter != mapVsName.end())
				{
					tUserVideoSourceCapInfo.name = iter->second;
				}
			}

			string strVsGps;
			er = GetDevInfo(devId, Redis_vidgps(nVsIndex), strVsGps);
			if (( !IsFdbResultPositive(er) ) || strVsGps.empty())
			{
				TDeviceCapExtData capExtData;
				capExtData.SetLon(INVALID_LONGITUDE);
				capExtData.SetLat(INVALID_LATITUDE);
				tUserVideoSourceCapInfo.extData = capExtData.ToXml();
			}
			else
			{
				tUserVideoSourceCapInfo.extData = strVsGps;
			}
            tDevFullInfoAndPri.vidSrcIndex.push_back(nVsIndex);
			tDevFullInfoAndPri.videosourceinfos[nVsIndex] = tUserVideoSourceCapInfo;
		}
	}
	else
	{
		map<int, CVideosourcePrivilege> mapVsPri;
		CEncoderPrivilege devPri;
		er = GetUserEncoderFullPrivilege(userId, devId, devPri, mapVsPri);
		if ( !IsFdbResultPositive(er) )
		{
			strErrMsg = "无法获取用户的设备及视频源权限";
			return er;
		}

		tDevFullInfoAndPri.devicePrivilege.ConvertFromBitsContainer(devPri);

		for (map<int, CVideosourcePrivilege>::iterator vsIter = mapVsPri.begin();
				vsIter != mapVsPri.end(); vsIter++)
		{
			int nVsIndex = vsIter->first;
			TUserVideoSourceCapInfo tUserVideoSourceCapInfo;

			tUserVideoSourceCapInfo.privilege = vsIter->second;

			std::map<std::string, std::string> mapVsName;
			string vsfield = TPuConfig_vidSrcAlias(nVsIndex);
			std::vector<std::string> vsfieldList;
			vsfieldList.push_back(vsfield);
			er = HMGet(MAKE_DEV_KEY(devId), vsfieldList, mapVsName);
			if ( !IsFdbResultPositive(er) )
			{
				stringstream ss;
				ss << er;
				strErrMsg += "无法获取设备的视频源别名 HMGet key[" + MAKE_DEV_KEY(devId) + "]field[" + vsfield + "] er[" + ss.str() + "]";
			}
			else
			{
				std::map<std::string, std::string>::const_iterator iter = mapVsName.find(vsfield);
				if (iter != mapVsName.end())
				{
					tUserVideoSourceCapInfo.name = iter->second;
				}
			}

			string strVsGps;
			er = GetDevInfo(devId, Redis_vidgps(nVsIndex), strVsGps);
			if (( !IsFdbResultPositive(er) ) || strVsGps.empty())
			{
				TDeviceCapExtData capExtData;
				capExtData.SetLon(INVALID_LONGITUDE);
				capExtData.SetLat(INVALID_LATITUDE);
				tUserVideoSourceCapInfo.extData = capExtData.ToXml();
			}
			else
			{
				tUserVideoSourceCapInfo.extData = strVsGps;
			}

            tDevFullInfoAndPri.vidSrcIndex.push_back(nVsIndex);
			tDevFullInfoAndPri.videosourceinfos[nVsIndex] = tUserVideoSourceCapInfo;
		}
	}

	return EC_OK;
}

EFDB_ErrorCode CRedisDBInterface::GetUserFullDevForCu(const UUIDString userId, const UUIDString &devId, const bool &bIsAdmin, TDeviceInfoAndStatus &tDeviceInfoAndStatus, string &strErrMsg)
{
	if (userId.empty() || devId.empty())
	{
		return EC_INVALID_PARAM;
	}

	CRedisPuData cPuData;

	std::vector<string> fields;
	fields.push_back(CRedisPuData_m_tDevInfo);
	fields.push_back(CRedisDevData_m_tPuOnOff);
	fields.push_back(CRedisDevData_m_tPuConfig);
	//fields.push_back(ALL_CRedisPuData_m_tVidSrcAliasMap);

	EFDB_ErrorCode er = GetPuData(devId, cPuData, fields);

	if ( !IsFdbResultPositive(er) )
	{
		strErrMsg = "获取RedisPuData失败";
		return er;
	}

	TDevicefullinfoAndPrivilege &tDevFullInfoAndPri = tDeviceInfoAndStatus.devInfo;
	tDevFullInfoAndPri.deviceInfo = cPuData.GetDevInfo();

	if (bIsAdmin)
	{
		tDevFullInfoAndPri.devicePrivilege.ConvertFromInt(-1);
	}
	else
	{
		CEncoderPrivilege cDevPri;
		er = GetUserEncoderPrivilege(userId, devId, cDevPri);
		if ( !IsFdbResultPositive(er) )
		{
			strErrMsg = "获取设备权限失败，可能是取消了划归";
			return er;
		}

		tDevFullInfoAndPri.devicePrivilege.ConvertFromBitsContainer(cDevPri);
	}

	for (int nVsIndex = 0; nVsIndex < tDeviceInfoAndStatus.devConfig.GetBaseCapSet().GetVideoInPortNum(); nVsIndex++)
	{
		TUserVideoSourceCapInfo tUserVideoSourceCapInfo;

		CVideosourcePrivilege cVsPri;
		int nPtzLevel;

		if (bIsAdmin)
		{
			cVsPri.SetPTZCtrl(CMS_PTZ_MAX_PRI_LEV);
			cVsPri.ConvertFromInt(-1);
		}
		else
		{
			er = GetUserVideoPrivilege(userId, devId, nVsIndex, cVsPri);
			if ( !IsFdbResultPositive(er) )
			{
				continue;
			}
		}

		tUserVideoSourceCapInfo.privilege = cVsPri;

		if ( IsFdbResultPositive(er) )
		{
			tUserVideoSourceCapInfo.name = cPuData.GetPuConfig().GetVidSrcNameByVidSrcChn(nVsIndex);
		}

		string strVsGps;
		er = GetDevInfo(devId, Redis_vidgps(nVsIndex), strVsGps);
		if ( IsFdbResultPositive(er) )
		{
			tUserVideoSourceCapInfo.extData = strVsGps;
		}
		else
		{
			TDeviceCapExtData capExtData;
			capExtData.SetLon(INVALID_LONGITUDE);
			capExtData.SetLat(INVALID_LATITUDE);
			tUserVideoSourceCapInfo.extData = capExtData.ToXml();
		}

		// cusdk.h已经废弃了这个字段,不用构建
		//tDevFullInfoAndPri.alarminputinfos;

		tDeviceInfoAndStatus.devInfo.videosourceinfos[nVsIndex] = tUserVideoSourceCapInfo;
	}


	tDeviceInfoAndStatus.isOnline = cPuData.GetPuStatus().GetIsOnline();
	tDeviceInfoAndStatus.devConfig = cPuData.GetPuConfig();

	return EC_OK;
}

EFDB_ErrorCode CRedisDBInterface::GetFullPuConfigForCu(IN const UUIDString &devId, OUT TPuConfig &puConfig, string &strErrMsg)
{
	if (devId.empty())
	{
		return EC_INVALID_PARAM;
	}

	EFDB_ErrorCode er = GetFullPuConfig(devId, puConfig);
	if ( !IsFdbResultPositive(er) )
	{
		puConfig.Clear();
		strErrMsg = "获取设备配置信息失败";
		return er;
	}

	if ( !puConfig.IsCompleted() )
	{
		puConfig.Clear();
		strErrMsg = "获取设备配置信息成功，但配置不正确（puConfig.IsCompleted()==false）";
		return EC_FAILED;
	}

	TPuStatus tPuStatus;
	er = GetPuStatus(devId, tPuStatus, TPuStatus_isOnline);
	if ( !IsFdbResultPositive(er) )
	{
		strErrMsg = "无法获取设备的在线状态信息，那么就以设备配置为准!不修改在线通道状态";
		return EC_OK;
	}

	if (tPuStatus.GetIsOnline())
	{
		return EC_OK;
	}

	strErrMsg = "设备下线状态，给CU的通道状态要从在线状态变成下线状态";

	// 设备不在线，也要给CU设备配置信息
	map<TChannelID, TPuChnStateConfig> &chnState = puConfig.GetChnStateSet();
	for (map<TChannelID, TPuChnStateConfig>::iterator iter = chnState.begin();
		iter != chnState.end(); iter++)
	{
		if (iter->second.GetChnState() == CHNSTATUS_ONLINE)
		{
			iter->second.SetChnState(CHNSTATUS_OFFLINE);
		}
	}

	return EC_OK;
}

EFDB_ErrorCode CRedisDBInterface::GetPuServiceStatusForCu(IN const UUIDString &devId, OUT TPuServiceStatus &serviceStatus, string &strErrMsg)
{
    if (devId.empty())
	{
		return EC_INVALID_PARAM;
	}

	TPuStatus tPuStatus;
	EFDB_ErrorCode er = GetPuStatus(devId, tPuStatus, TPuStatus_isOnline);
    if ( IsFdbResultPositive(er) )
    {
        if (tPuStatus.GetIsOnline())            // 在线时才取服务状态信息，否则没有意义
        {
            return GetFullPuServiceStatus(devId, serviceStatus);
        }

        return EC_OK;
    }

    strErrMsg = "获取设备的在线状态信息失败！";

    return er;
}

EFDB_ErrorCode CRedisDBInterface::GetPuAlarmStatusForCu(IN const UUIDString &devId, OUT TPuAlarmStatus &alarmStatus, string &strErrMsg)
{
    if (devId.empty())
    {
        return EC_INVALID_PARAM;
    }

    TPuStatus tPuStatus;
    EFDB_ErrorCode er = GetPuStatus(devId, tPuStatus, TPuStatus_isOnline);
    if ( IsFdbResultPositive(er) )
    {
        if (tPuStatus.GetIsOnline())            // 在线时才取服务状态信息，否则没有意义
        {
            CRedisPuData cPuData;
            map<std::string, CAlarmInputList> &cAlarmInfo = cPuData.GetPuAlarmCollector();
            er = GetAlarmChnInfo(devId, cAlarmInfo);
            if ( IsFdbResultPositive(er) )
            {
                alarmStatus = cPuData.GetPuAlarmStatus();
            }

            return er;
        }

        return EC_OK;
    }

    strErrMsg = "获取设备的在线状态信息失败！";

    return er;
}

EFDB_ErrorCode CRedisDBInterface::GetDomainNameByDevId(const UUIDString &devId, string &strDomainName, string &strErrMsg)
{
	if (devId.empty())
	{
		return EC_INVALID_PARAM;
	}

	CDeviceInfo tDeviceInfo;
	EFDB_ErrorCode er = GetDeviceInfo(devId, tDeviceInfo, CDeviceInfo_domainId);
	if ( !IsFdbResultPositive(er) )
	{
		strErrMsg = "无法获取设备的域ID";
		return er;
	}

	string strDomainId = tDeviceInfo.GetDomainUUID();
	if (strDomainId.empty())
	{
		strErrMsg = "设备的域ID是空的";
		return EC_NOT_EXISTED;
	}

	CDomainRelationInfo tDomainInfo;
	er = GetDomainRelationInfo(strDomainId, tDomainInfo, CDomainRelationInfo_domainName);
	if ( !IsFdbResultPositive(er) )
	{
		strErrMsg = "无法获取设备的域ID[" + strDomainId + "]对应的域名";
		return er;
	}

	strDomainName = tDomainInfo.GetDomainName();
	if ( strDomainName.empty() )
	{
		strErrMsg = "设备的域ID[" + strDomainId + "]对应的域名是多空的";
		return EC_NOT_EXISTED;
	}

	return EC_OK;
}

// 获取所有域信息
EFDB_ErrorCode CRedisDBInterface::GetAllDomainRelationInfo(CDomainInfoMap &domainInfoMap, string &strErrMsg)
{
	std::vector<UUIDString> vctDomainIdList;
	EFDB_ErrorCode er = GetDomainIDList(vctDomainIdList);
	if ( !IsFdbResultPositive(er) )
	{
		strErrMsg = "获取域ID列表失败";
		return er;
	}

	std::vector<UUIDString>::const_iterator it = vctDomainIdList.begin();
	for (; it != vctDomainIdList.end(); ++it)
	{
		CDomainRelationInfo cDomainInfo;
		er = GetFullDomainRelationInfo(*it, cDomainInfo);
		if ( !IsFdbResultPositive(er) )
		{
			strErrMsg = "获取域[" + *it + "]完整信息失败";
			return er;
		}

		domainInfoMap.SetDomainInfo(cDomainInfo);
	}

	return EC_OK;
}

