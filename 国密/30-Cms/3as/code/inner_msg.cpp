#include "inner_msg.h"
#include "inner_common.h"
#include "config_util/tas_config.h"

CUserPwdLockMng g_cUserPwdLockMng;

bool TLicenseUsageInfo::IsNotLessLicense( const TLicenseUsageInfo &tLicense )
{
    if (m_dwPUNum < tLicense.m_dwPUNum)
    {
        return false;
    }

    if (m_atOtherPULicenseInfos.size() < tLicense.m_atOtherPULicenseInfos.size())
    {
        return false;
    }

    u32 i, j;
    for (i = 0; i < tLicense.m_atOtherPULicenseInfos.size(); ++i)
    {
        for (j = 0; j < m_atOtherPULicenseInfos.size(); ++j)
        {
            if (tLicense.m_atOtherPULicenseInfos[i].m_strManufacture == m_atOtherPULicenseInfos[j].m_strManufacture)
            {
                break;
            }
        }
        if (j == m_atOtherPULicenseInfos.size() || 
            m_atOtherPULicenseInfos[j].m_dwLicenseNum < tLicense.m_atOtherPULicenseInfos[i].m_dwLicenseNum)
        {
            return false;
        }
    }

    if (m_dwManagePUNum < tLicense.m_dwManagePUNum)
    {
        return false;
    }
    return true;
}

u32 TLicenseUsageInfo::OtherPUTotalLicense()
{
    u32 dwLicenseNum = 0;
    for (u32 i = 0; i < m_atOtherPULicenseInfos.size(); ++i)
    {
        dwLicenseNum += m_atOtherPULicenseInfos[i].m_dwLicenseNum;
    }
    return dwLicenseNum;
}

u32 TLicenseUsageInfo::OtherPULicense( const string &strManufacture, const vector<TOtherPULicenceInfo> &vecOtherPuLimit )
{
    /* 合并相同厂商 */
    vector<TOtherPULicenceInfo> vecOtherPuLicenseUsage;
    vecOtherPuLicenseUsage.reserve(m_atOtherPULicenseInfos.size());
    string strManuTmp;
    for (u32 i = 0; i < m_atOtherPULicenseInfos.size(); ++i)
    {
        strManuTmp = m_atOtherPULicenseInfos[i].m_strManufacture;
        AdjustManufactureName(strManuTmp);

        vector<TOtherPULicenceInfo>::iterator itTmp = vecOtherPuLicenseUsage.begin();
        for (;itTmp != vecOtherPuLicenseUsage.end(); ++itTmp)
        {
            if (itTmp->m_strManufacture == strManuTmp)
            {
                itTmp->m_dwLicenseNum += m_atOtherPULicenseInfos[i].m_dwLicenseNum;
                break;
            }
        }
        if (itTmp == vecOtherPuLicenseUsage.end())
        {
			TOtherPULicenceInfo tInfo;
			tInfo.m_strManufacture = strManuTmp;
			tInfo.m_dwLicenseNum   = m_atOtherPULicenseInfos[i].m_dwLicenseNum;
            vecOtherPuLicenseUsage.push_back(tInfo);
        }
    }

    /* 找厂商 */
    vector<TOtherPULicenceInfo>::const_iterator it = vecOtherPuLimit.begin();

    for (;it != vecOtherPuLimit.end(); ++it)
    {
        if (it->m_strManufacture == strManufacture)
        {
            break;
        }
    }
    if (strManufacture == "other") // 处理出现other名称的厂商名字
    {
        it = vecOtherPuLimit.end();
    }

    if (it != vecOtherPuLimit.end())
    {
        u32 dwNum = 0;
        for (u32 i = 0; i < vecOtherPuLicenseUsage.size(); ++i)
        {
            if (vecOtherPuLicenseUsage[i].m_strManufacture == strManufacture)
            {
                dwNum = vecOtherPuLicenseUsage[i].m_dwLicenseNum;
                break;
            }
        }

        return dwNum;
    }
    else
    { // other厂商算上所有其它的厂商
        u32 dwNum = 0;
        for (u32 i = 0; i < vecOtherPuLicenseUsage.size(); ++i)
        {
            for (it = vecOtherPuLimit.begin(); it != vecOtherPuLimit.end(); ++it)
            {
                if (vecOtherPuLicenseUsage[i].m_strManufacture == it->m_strManufacture 
                    && it->m_strManufacture != "other")
                {
                    break;
                }
            }

            if (it == vecOtherPuLimit.end())
            {
                dwNum += vecOtherPuLicenseUsage[i].m_dwLicenseNum;
            }
        }

        return dwNum;
    }
}

void TLicenseUsageInfo::AddOtherPULicense( const string &strManufacture, s32 nLicenseNum )
{
    u32 i;
    for (i = 0; i < m_atOtherPULicenseInfos.size(); ++i)
    {
        if (m_atOtherPULicenseInfos[i].m_strManufacture == strManufacture)
        {
            break;
        }
    }
    if (i == m_atOtherPULicenseInfos.size())
    {
        TOtherPULicenceInfo tInfo(strManufacture);
        m_atOtherPULicenseInfos.push_back(tInfo);
    }

    m_atOtherPULicenseInfos[i].m_dwLicenseNum += nLicenseNum;
}


void CUserPwdLockMng::UnlockAllUserPwd()
{
	map<string, vector<TUserLoginInfo> >::iterator mapIter = m_mapUserLoginInfo.begin();
	for(; mapIter != m_mapUserLoginInfo.end(); mapIter++)
	{
		vector<TUserLoginInfo> &vectUserLoginInfo = mapIter->second;
		vector<TUserLoginInfo>::iterator vecIter = vectUserLoginInfo.begin();
		while (vecIter != vectUserLoginInfo.end())
		{
			vectUserLoginInfo.erase(vecIter);
			vecIter = vectUserLoginInfo.begin();
		}
	}
}


u32 CUserPwdLockMng::IsPwdLocked(const string &strUserName, const string &strLoginIp)
{
	map<string, vector<TUserLoginInfo> >::iterator mapIter = m_mapUserLoginInfo.find(strUserName);
	if(mapIter != m_mapUserLoginInfo.end())
	{
		vector<TUserLoginInfo> &vectUserLoginInfo = mapIter->second;
		vector<TUserLoginInfo>::iterator vecIter = vectUserLoginInfo.begin();
		for (; vecIter != vectUserLoginInfo.end(); vecIter++)
		{
			if (vecIter->loginIp == strLoginIp)
			{
				
				if (CTasConfig::GetPwdInputTimes() == vecIter->nInputPwdErrorTimes)
				{
					u32 dwPwdLockedTime = CCMSTime() - vecIter->cLockCMSTime;
					if (dwPwdLockedTime <= CTasConfig::GetPwdLockedTime() * 60)
					{
						return CTasConfig::GetPwdLockedTime() * 60 - dwPwdLockedTime;
					}
					else
					{
						vectUserLoginInfo.erase(vecIter);
					}
				}
				return 0;
			}
		}
	}
	return 0;
}



void CUserPwdLockMng::ProcPwdError(string strUserName, string strLoginIp)
{
	CCMSTime cNowTime = CCMSTime();
	map<string, vector<TUserLoginInfo> >::iterator mapIter = m_mapUserLoginInfo.find(strUserName);
	if(mapIter != m_mapUserLoginInfo.end())
	{
		vector<TUserLoginInfo> &vectUserLoginInfo = mapIter->second;
		vector<TUserLoginInfo>::iterator vecIter = vectUserLoginInfo.begin();
		for (; vecIter != vectUserLoginInfo.end(); vecIter++)
		{
			if (vecIter->loginIp == strLoginIp)
			{

				/*前面刚收到登陆请求时,已对输入密码错误达到上限做了操作，此处不需处理*/
				if ((cNowTime - vecIter->cLoginCMSTime) <= CTasConfig::GetPwdLimitTime() * 60)
				{
					if (CTasConfig::GetPwdInputTimes() != vecIter->nInputPwdErrorTimes)
					{
						vecIter->nInputPwdErrorTimes++;
						vecIter->cLockCMSTime = cNowTime;
					}
				}
				else
				{
					vecIter->cLoginCMSTime = cNowTime;
					vecIter->nInputPwdErrorTimes = 1;
				}
				return;
			}
		}
		TUserLoginInfo tUserLoginInfo;
		tUserLoginInfo.cLoginCMSTime = cNowTime;
		tUserLoginInfo.loginIp = strLoginIp;
		tUserLoginInfo.nInputPwdErrorTimes = 1;
		vectUserLoginInfo.push_back(tUserLoginInfo);
	}
	else
	{
		TUserLoginInfo tUserLoginInfo;
		tUserLoginInfo.cLoginCMSTime = cNowTime;
		tUserLoginInfo.loginIp = strLoginIp;
		tUserLoginInfo.nInputPwdErrorTimes = 1;
		vector<TUserLoginInfo> vectUserLoginInfo;
		vectUserLoginInfo.push_back(tUserLoginInfo);
		m_mapUserLoginInfo.insert(pair<string, vector<TUserLoginInfo> >(strUserName, vectUserLoginInfo));
	}
}


void CUserPwdLockMng::EraseFromLoginInfo(string strUserName, string strLoginIp)
{
	map<string, vector<TUserLoginInfo> >::iterator mapIter = m_mapUserLoginInfo.find(strUserName);
	if(mapIter != m_mapUserLoginInfo.end())
	{
		vector<TUserLoginInfo> &vectPmcLoginInfo = mapIter->second;
		vector<TUserLoginInfo>::iterator vecIter = vectPmcLoginInfo.begin();
		for (; vecIter != vectPmcLoginInfo.end(); vecIter++)
		{
			if (vecIter->loginIp == strLoginIp)
			{
				vectPmcLoginInfo.erase(vecIter);
				return;
			}
		}
	}
}