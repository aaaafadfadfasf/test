
#include "redisloginsession.h"
#include "sdk/redissdk/redissdk.h"
#include "cmucoreapp.h"
#include "cmudata.h"

extern bool g_bDebugOn;

CRedisLoginSession::CRedisLoginSession(CInstExt *pcInst, CRedisLoginSessionMng *pcMng, const string &strSession ): CLoginSession(pcInst), m_pcMng(pcMng)
{
    CMS_ASSERT(NULL != m_pcMng && "m_pcMng == NULL!!!");

    // ���ĸ�ģ���״̬
    SetSession( strSession );

    CRedisSdk &cRedisSdk = GetLocalRedisPipe().GetRedisSdk();
    CRedisSsChannel cSsChn = cRedisSdk.GetModuleSsChannelById(GetSession());
    CRedisSsMng::GetInstance()->AddSs(cSsChn, GetTaskNO());
}

CRedisLoginSession::~CRedisLoginSession()
{
    DestroyChildTasks();

    // ���ĸ�ģ���״̬
    TSipURI tDevUri(GetSession());
    CRedisSdk &cRedisSdk = GetLocalRedisPipe().GetRedisSdk();
    CRedisSsChannel cSsChn = cRedisSdk.GetModuleSsChannelById(GetSession());
    CRedisSsMng::GetInstance()->DelSs(cSsChn, GetTaskNO());
    
    // ɾ��CRedisLoginSessionMng�е�ӳ���ϵ
    if (NULL != m_pcMng)
    {
        m_pcMng->m_cLoginSessions.erase(GetSession());
    }

}

CRedisLoginSessionMng *CRedisLoginSessionMng::m_pcInst = NULL;

CRedisLoginSessionMng::CRedisLoginSessionMng()
{

}

CRedisLoginSessionMng::~CRedisLoginSessionMng()
{
    map<string, CRedisLoginSession *>::iterator it = m_cLoginSessions.begin();
    while (m_cLoginSessions.end() != it && NULL != it->second)
    {
        CRedisLoginSession *pcTask = it->second;
        ++it;           // ������Ҫ���޸ĵ��������������delete��ʱ����ܵ�����ʧЧ
        delete pcTask;
    }
}

CRedisLoginSessionMng *CRedisLoginSessionMng::GetInstance()
{
    if (NULL == m_pcInst)
    {
        m_pcInst = new CRedisLoginSessionMng();
        CMS_ASSERT(NULL != m_pcInst && "m_pcInst = NULL!!!");
    }

    return m_pcInst;
}

CRedisLoginSession *CRedisLoginSessionMng::GetLoginSession(const string &strSession)
{
    TSipURI tDevUri(strSession);
    string strId = tDevUri.GetUser().empty() ? strSession : tDevUri.GetUser();
    map<string, CRedisLoginSession *>::iterator it = m_cLoginSessions.find(strId);
    if (m_cLoginSessions.end() == it || NULL == it->second)
    {
        // 1. ��redis�л�ȡģ����Ϣ
        TRedisModuleDetailInfo tModuleDetailInfo;
        CRedisSdk &cRedisSdk = GetLocalRedisPipe().GetRedisSdk();
        if (CMS_SUCCESS == cRedisSdk.GetModuleDetailInfo(strId, tModuleDetailInfo))
        {
            // 2. �����Ựtask
            CRedisLoginSession *pcTask = new CRedisLoginSession(GetWorkInst(), this, strId);
            pcTask->InitStateMachine();
            pcTask->SetDevType(RedisModuleType2DevType(tModuleDetailInfo.tModule.tId.tModuleType));

            // 3. ����Ựtask
            m_cLoginSessions[strId] = pcTask;

            return pcTask;
        }

        return NULL;
    }
    else
    {
        return it->second;
    }
}

void CRedisLoginSessionMng::PrintData() const
{
    if (!m_cLoginSessions.empty())
    {
        OspPrintf(TRUE, FALSE, "\n---------------------------------RedisLoginSession----------------------------\n");

        for (map<string, CRedisLoginSession *>::const_iterator it = m_cLoginSessions.begin(); m_cLoginSessions.end() != it; ++it)
        {
            if (NULL != it->second)
            {
                u32 dwTaskNO = (NULL == it->second) ? INVALID_TASKNO : it->second->GetTaskNO();
                OspPrintf(TRUE, FALSE, "Session[%s], Sser[%u]\n", it->first.c_str(), dwTaskNO);
            }
        }

        OspPrintf(TRUE, FALSE, "\n------------------------------------------------------------------------------\n");
    }
}
