#include "redisssmng.h"
#include "sdk/redissdk/redissdk.h"
#include "cmudata.h"

CRedisSsMng *CRedisSsMng::m_pcInstance = NULL;

CRedisSsMng *CRedisSsMng::GetInstance()
{
    if (NULL == m_pcInstance)
    {
        CRedisSdk &cRedisSdk = GetLocalRedisPipe().GetRedisSdk();
        m_pcInstance = new CRedisSsMng(&cRedisSdk);
    }

    return m_pcInstance;
}

CRedisSsMng::CRedisSsMng(CRedisSdk *pcRedisSdk) : m_pcRedisSdk(pcRedisSdk)
{
    CMS_ASSERT(NULL != pcRedisSdk && "pcRedisSdk = NULL!!!");
}

void CRedisSsMng::AddSs(const CRedisSsChannel &cSsChn, u32 dwTaskNO)
{
    map<CRedisSsChannel, set<u32> >::iterator it = m_cSsList.find(cSsChn);
    if (m_cSsList.end() != it && !it->second.empty())
    {
        it->second.insert(dwTaskNO);
    }
    else
    {
        set<CRedisSsChannel> cSsList;
        cSsList.insert(cSsChn);
        if (CMS_SUCCESS == m_pcRedisSdk->UpdateSsChannel(cSsList, RedisSdkOperType_Add))
        {
            m_cSsList[cSsChn].insert(dwTaskNO);
        }
        else
        {
            GLOBELOG(ERROR_LEV, "AddSs failed, SsChn\n", cSsChn.ToString().c_str());
        }
    }
}

void CRedisSsMng::DelSs(const CRedisSsChannel &cSsChn, u32 dwTaskNO)
{
    map<CRedisSsChannel, set<u32> >::iterator it = m_cSsList.find(cSsChn);
    if (m_cSsList.end() != it && 1 == it->second.size() && it->second.end() != it->second.find(dwTaskNO)) // 只有唯一的一个Task在订阅
    {
        set<CRedisSsChannel> cSsList;
        cSsList.insert(cSsChn);
        if (CMS_SUCCESS == m_pcRedisSdk->UpdateSsChannel(cSsList, RedisSdkOperType_Del))
        {
            m_cSsList.erase(it);
        }
        else
        {
            GLOBELOG(ERROR_LEV, "DelSs failed, SsChn\n", cSsChn.ToString().c_str());
        }
    }
    else
    {
        if (m_cSsList.end() != it)
        {
            it->second.erase(dwTaskNO);
        }
    }
}

void CRedisSsMng::ClearSs(const CRedisSsChannel &cSsChn)
{
    map<CRedisSsChannel, set<u32> >::iterator it = m_cSsList.find(cSsChn);
    if (m_cSsList.end() != it)
    {
        set<CRedisSsChannel> cSsList;
        cSsList.insert(cSsChn);
        if (CMS_SUCCESS == m_pcRedisSdk->UpdateSsChannel(cSsList, RedisSdkOperType_Del))
        {
            m_cSsList.erase(it);
        }
        else
        {
            GLOBELOG(ERROR_LEV, "ClearSs failed, SsChn\n", cSsChn.ToString().c_str());
        }
    }
}

bool CRedisSsMng::IsSsChnExist(const CRedisSsChannel &cSsChn) const
{
    return (m_cSsList.end() != m_cSsList.find(cSsChn));
}

void CRedisSsMng::GetSsers(const CRedisSsChannel &cSsChn, set<u32> &cTasks)
{
    map<CRedisSsChannel, set<u32> >::iterator it = m_cSsList.find(cSsChn);
    if (m_cSsList.end() != it)
    {
        cTasks = it->second;
    }
}

void CRedisSsMng::PrintData(const CRedisSsChannel &cSsChn) const
{
    map<CRedisSsChannel, set<u32> >::const_iterator it4Ss = m_cSsList.find(cSsChn);
    if (m_cSsList.end() != it4Ss)
    {
        ostringstream oss;
        const set<u32> &cTasks = it4Ss->second;
        for (set<u32>::const_iterator it4Task = cTasks.begin(); cTasks.end() != it4Task; ++it4Task)
        {
            oss << *it4Task << " ";
        }

        string strTasks(oss.str());
        OspPrintf(TRUE, FALSE, "\tSsChn[%s], Sser[%s]\n", it4Ss->first.ToString().c_str(), strTasks.c_str());
    }
}

void CRedisSsMng::PrintData(bool bVerbose) const
{
    if (!m_cSsList.empty())
    {
        OspPrintf(TRUE, FALSE, "\n-------------------------------RedisSsMng----------------------------------------\n");
        u32 dwCnt = 0;
        for (map<CRedisSsChannel, set<u32> >::const_iterator it4Ss = m_cSsList.begin(); 
            m_cSsList.end() != it4Ss; ++it4Ss)
        {
            if (bVerbose)
            {
                PrintData(it4Ss->first);
            }
            else
            {
                OspPrintf(TRUE, FALSE, "\tSsChn[%s], SserNum[%u]\n", it4Ss->first.ToString().c_str(), it4Ss->second.size());
            }
        }
        OspPrintf(TRUE, FALSE, "\n----------------------------------------------------------------------------\n");
    }
}

