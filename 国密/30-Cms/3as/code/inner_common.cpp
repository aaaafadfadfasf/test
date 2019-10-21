#include "inner_common.h"
#include "db_task_pool.h"
#include "cms/commonconfig/cmcfg.h"

bool PostTask2DbPool( u16 wEventId, const COspObj* cOspObj )
{
    extern CDbTaskPool g_cDbTaskPool;
    return g_cDbTaskPool.PostTask(wEventId, cOspObj);
}

string CurrentDomainId()
{
    // 此配置要求重启生效
    static string strDomainId = GetDomainUUID();
    return strDomainId;
}

CXSemLock s_cLock4DomainName;
static string s_strDomainName;
string CurrentDomainName()
{
    static bool s_bstrDomainNameInited = false;
    if (!s_bstrDomainNameInited)
    {
        s_strDomainName = GetDomainName();
        s_bstrDomainNameInited = true;
    }

    s_cLock4DomainName.Lock();
    string strDomainName = s_strDomainName;
    s_cLock4DomainName.UnLock();
    return strDomainName;
}

void UpdateCurrentDomainName( const string &strNewDomainName )
{
    s_cLock4DomainName.Lock();
    s_strDomainName = strNewDomainName;
    s_cLock4DomainName.UnLock();
}

bool ValidateDomainRelation( const vector<CDomainRelationInfo> &acCurrentDomainList, const CDomainRelationInfo& cDomain, bool bAdd )
{
    vector<CDomainRelationInfo> acDomainList(acCurrentDomainList);

    if (bAdd)
    {
        for (vector<CDomainRelationInfo>::iterator it = acDomainList.begin(); it != acDomainList.end(); ++it)
        {
            if (it->GetDomainID() == cDomain.GetDomainID())
            {
                acDomainList.erase(it);
                break;
            }
        }

        acDomainList.push_back(cDomain);
    }
    else
    {
        for (vector<CDomainRelationInfo>::iterator it = acDomainList.begin(); it != acDomainList.end(); ++it)
        {
            if (it->GetDomainID() == cDomain.GetDomainID())
            {
                *it = cDomain;
                break;
            }
        }
    }

    /* 判断里面是否存在同名的 */
    bool bExistSameName = false;
    vector<CDomainRelationInfo>::iterator itTmp;
    for (vector<CDomainRelationInfo>::iterator it = acDomainList.begin(); it != acDomainList.end(); ++it)
    {
        itTmp = it;
        while(++itTmp != acDomainList.end())
        {
            if (it->GetDomainName() == itTmp->GetDomainName())
            {
                bExistSameName = true;
                break;
            }
        }

        if (bExistSameName)
        {
            break;
        }
    }

    if (bExistSameName) return false;

    /* 判断里面是否存在环 */
    bool bExistCircle = false;
    set<vector<CDomainRelationInfo>::iterator> pathElementset;
    UUIDString tParentDomainId;
    for (vector<CDomainRelationInfo>::iterator it = acDomainList.begin(); it != acDomainList.end(); ++it)
    {
        pathElementset.clear();
        pathElementset.insert(it);
        tParentDomainId = it->GetParentDomainID();
        
        while (!tParentDomainId.empty())
        {
            for (itTmp = acDomainList.begin(); itTmp != acDomainList.end(); ++itTmp)
            {
                if (tParentDomainId == itTmp->GetDomainID())
                {
                    if (pathElementset.find(itTmp) == pathElementset.end())
                    {
                        pathElementset.insert(itTmp);
                        tParentDomainId = itTmp->GetParentDomainID();
                    }
                    else
                    {
                        bExistCircle = true;
                    }
                    break;
                }
            }

            if (bExistCircle || acDomainList.end() == itTmp)
            {
                break;
            }
        }

        if (bExistCircle)
        {
            break;
        }
    }

    return !bExistCircle;
}

void AdjustManufactureName( string &strManufacture, bool *pbIsOtherPuDirectAccess )
{
    // "g100_xxx" 与 "xxx"共享"xxx"的license
    if (0 == strManufacture.find("g100_"))
    {
        strManufacture.erase(0, strlen("g100_"));
        if (NULL != pbIsOtherPuDirectAccess)
        {
            *pbIsOtherPuDirectAccess = false;
        }
    }
    else
    {
        if (NULL != pbIsOtherPuDirectAccess)
        {
            *pbIsOtherPuDirectAccess = (strManufacture != MANUFACTURE_KEDACOM);
        }
    }
}
