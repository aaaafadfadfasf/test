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
		strErrMsg = "�������� �豸ID[" + devId + "]" + " ��ƵԴID[" + ss.str() + "]";
		return EC_INVALID_PARAM;
	}

	CDeviceInfo cDeviceInfo;
	EFDB_ErrorCode er = GetDeviceInfo(devId, cDeviceInfo, CDeviceInfo_name);
	if ( !IsFdbResultPositive(er) )
	{
		strErrMsg = "�޷���ȡ�豸����?����������";
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
		strErrMsg += "�޷���ȡ�豸����ƵԴ���� HMGet key[" + MAKE_DEV_KEY(devId) +"]field[" + vsfield + "] er[" + ss.str() + "]";
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
			strErrMsg += "�豸�����ǿյģ�";
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
		strErrMsg = "�������� :�û�id[" + userId + "] �豸id[" + devId + "] ��ƵԴ/ͨ��id[" + ss.str() + " < 0]";
		return EC_INVALID_PARAM;
	}

	TChannel tChannel(devId, chnNo);

	// ��¼һЩ�������̷��ص�ʧ�ܻ����
	string moreErrorMsg;

	// ��ȡ������Ϣ, ͨ����ƵԴӳ���ϵ
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
				// һ��ͨ��ֻ�ܶ�Ӧһ����ƵԴ
				vsIndex = *iter->second.begin();
			}
		}
		else
		{
			strErrMsg = "��ȡ�����豸������Ϣ�����޷�ȡ��ͨ����Ӧ����ƵԴid!";
			return EC_NOT_EXISTED;
		}

		if (vsIndex > INVALID_VIDEOSOURCE_ID)
		{
			// ȡ������ƵԴID,ֱ�ӻ�ȡ��ƵԴȨ��
			er = GetUserVideoPrivilege(userId, devId, vsIndex, priv);
			//ע�⣺֮ǰ ��3a��ȡ δ��ȡ����Ϊ true��
            //     ���ڴ�reids ��ȡ�����迼��δ��ȡ������
            if ( !IsFdbResultPositive(er) )
			{
				stringstream ss;
				ss << vsIndex;
				strErrMsg = "��ȡ�����豸������Ϣ�����޷�ȡ����ƵԴ["+ ss.str() +"]��Ӧ��Ȩ��!";
			}
			return er;
		}
		else
		{
			strErrMsg = "��ȡ�����豸������Ϣ����ͨ����Ӧ����ƵԴID����Ч��!";
			return EC_NOT_EXISTED;
		}
	}

	// û��ȡ���豸������Ϣ�е� ͨ����ƵԴ��Ӧ��ϵ
	// ��������Ϣ�жϻ�ȡ��Ӧ����ƵԴ
	fields.clear();

	TPuBaseCapSet puBaseCapSet;
	fields.push_back(TPuBaseCapSet_videoInPortNum);
	fields.push_back(TPuBaseCapSet_videoEncNum);

	er = GetPuBaseCapSet(devId, puBaseCapSet, fields);
	if ( !IsFdbResultPositive(er) )
	{
		strErrMsg = "�޷���ȡ�豸������Ϣ ��ƵԴ����������ͨ������!";
		return er;
	}

	int totalEncChnNum = puBaseCapSet.GetVidEncNum();
	int totalVsNum = puBaseCapSet.GetVideoInPortNum();

	if (totalEncChnNum <= 0
		|| totalVsNum <= 0)
	{
		stringstream sslog;
		sslog << "ȡ�����豸��Ϣ����ȷ," << "��ƵԴ����[" << totalVsNum << "] ����ͨ������[" << totalEncChnNum << "]";
		strErrMsg = sslog.str();
		return EC_NOT_EXISTED;
	}

	if (chnNo >= totalEncChnNum)
	{
		stringstream sslog;
		sslog << "��ѯ�ı���ͨ��id[" << chnNo << "] �����豸����ͨ������[" << totalEncChnNum << "]";
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
			// ȡ����Ϊ0������������1:1/1:2/1:3 ...
			vsIndex = chnNo%totalVsNum;
		}
		else
		{
			remainder = (totalEncChnNum + 1) % totalVsNum;
			if (remainder == 0)
			{
				// SVR�ϳ�ͨ��,��һ����ƵԴû�и���ͨ��(ֻ����ֻ��1�����������)
				vsIndex = (chnNo / totalVsNum) + (chnNo % totalVsNum);
			}
			else
			{
				// ��������Ͳ������ˣ�ֱ��ȡ�࣬���˾ʹ��ˡ�
				vsIndex == INVALID_VIDEOSOURCE_ID;
				strErrMsg = "��ѯ����ͨ�������޷�ת������ƵԴID!";
				return EC_INVALID_PARAM;
			}
		}
	}

	if (vsIndex <= INVALID_VIDEOSOURCE_ID)
	{
		CMS_ASSERT(false && "Get VsIndex Error!");
		strErrMsg = "��ѯ����ͨ�������޷�ת������ƵԴID!";
		return EC_INVALID_PARAM;
	}

	er = GetUserVideoPrivilege(userId, devId, vsIndex, priv);
	if ( !IsFdbResultPositive(er) )
	{
		strErrMsg = "��ѯ��ƵԴȨ��ʧ�ܣ�����û�л������ƵԴ!";
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
		strErrMsg = "�޷���ȡ�豸��������Ϣ";
		return er;
	}

	// �豸Ȩ��
	if (bIsAdmin)
	{
		// admin�˻���redis��û���豸��ص�Ȩ����Ϣk
		tDevFullInfoAndPri.devicePrivilege.ConvertFromInt(-1);

		TPuBaseCapSet tPuBaseCapSet;
		er = GetPuBaseCapSet(devId, tPuBaseCapSet, TPuBaseCapSet_videoInPortNum);
		if ( !IsFdbResultPositive(er) )
		{
			strErrMsg = "admin�˻� �޷���ȡ�豸����ƵԴ���� GetFullPuBaseCapSet";
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
				strErrMsg += "�޷���ȡ�豸����ƵԴ���� HMGet key[" + MAKE_DEV_KEY(devId) + "]field[" + vsfield + "] er[" + ss.str() + "]";
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
			strErrMsg = "�޷���ȡ�û����豸����ƵԴȨ��";
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
				strErrMsg += "�޷���ȡ�豸����ƵԴ���� HMGet key[" + MAKE_DEV_KEY(devId) + "]field[" + vsfield + "] er[" + ss.str() + "]";
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
		strErrMsg = "��ȡRedisPuDataʧ��";
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
			strErrMsg = "��ȡ�豸Ȩ��ʧ�ܣ�������ȡ���˻���";
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

		// cusdk.h�Ѿ�����������ֶ�,���ù���
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
		strErrMsg = "��ȡ�豸������Ϣʧ��";
		return er;
	}

	if ( !puConfig.IsCompleted() )
	{
		puConfig.Clear();
		strErrMsg = "��ȡ�豸������Ϣ�ɹ��������ò���ȷ��puConfig.IsCompleted()==false��";
		return EC_FAILED;
	}

	TPuStatus tPuStatus;
	er = GetPuStatus(devId, tPuStatus, TPuStatus_isOnline);
	if ( !IsFdbResultPositive(er) )
	{
		strErrMsg = "�޷���ȡ�豸������״̬��Ϣ����ô�����豸����Ϊ׼!���޸�����ͨ��״̬";
		return EC_OK;
	}

	if (tPuStatus.GetIsOnline())
	{
		return EC_OK;
	}

	strErrMsg = "�豸����״̬����CU��ͨ��״̬Ҫ������״̬�������״̬";

	// �豸�����ߣ�ҲҪ��CU�豸������Ϣ
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
        if (tPuStatus.GetIsOnline())            // ����ʱ��ȡ����״̬��Ϣ������û������
        {
            return GetFullPuServiceStatus(devId, serviceStatus);
        }

        return EC_OK;
    }

    strErrMsg = "��ȡ�豸������״̬��Ϣʧ�ܣ�";

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
        if (tPuStatus.GetIsOnline())            // ����ʱ��ȡ����״̬��Ϣ������û������
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

    strErrMsg = "��ȡ�豸������״̬��Ϣʧ�ܣ�";

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
		strErrMsg = "�޷���ȡ�豸����ID";
		return er;
	}

	string strDomainId = tDeviceInfo.GetDomainUUID();
	if (strDomainId.empty())
	{
		strErrMsg = "�豸����ID�ǿյ�";
		return EC_NOT_EXISTED;
	}

	CDomainRelationInfo tDomainInfo;
	er = GetDomainRelationInfo(strDomainId, tDomainInfo, CDomainRelationInfo_domainName);
	if ( !IsFdbResultPositive(er) )
	{
		strErrMsg = "�޷���ȡ�豸����ID[" + strDomainId + "]��Ӧ������";
		return er;
	}

	strDomainName = tDomainInfo.GetDomainName();
	if ( strDomainName.empty() )
	{
		strErrMsg = "�豸����ID[" + strDomainId + "]��Ӧ�������Ƕ�յ�";
		return EC_NOT_EXISTED;
	}

	return EC_OK;
}

// ��ȡ��������Ϣ
EFDB_ErrorCode CRedisDBInterface::GetAllDomainRelationInfo(CDomainInfoMap &domainInfoMap, string &strErrMsg)
{
	std::vector<UUIDString> vctDomainIdList;
	EFDB_ErrorCode er = GetDomainIDList(vctDomainIdList);
	if ( !IsFdbResultPositive(er) )
	{
		strErrMsg = "��ȡ��ID�б�ʧ��";
		return er;
	}

	std::vector<UUIDString>::const_iterator it = vctDomainIdList.begin();
	for (; it != vctDomainIdList.end(); ++it)
	{
		CDomainRelationInfo cDomainInfo;
		er = GetFullDomainRelationInfo(*it, cDomainInfo);
		if ( !IsFdbResultPositive(er) )
		{
			strErrMsg = "��ȡ��[" + *it + "]������Ϣʧ��";
			return er;
		}

		domainInfoMap.SetDomainInfo(cDomainInfo);
	}

	return EC_OK;
}

