#include "UserStreamNumRestorer.h"
#include "culogintask.h"
#include "cuidata.h"
extern CCuiDataMgr g_cCuiDataMgr;

const char LOGIN_TASK_NAME[] = "CCuAgent";

CUserStreamNumRestorer::CUserStreamNumRestorer( CInstExt *pcInst )
{
    m_pcInst     = pcInst;
    m_bStarted   = false;
}


CUserStreamNumRestorer::~CUserStreamNumRestorer()
{
    Stop();
}


CUserAccount* CUserStreamNumRestorer::FindUserAccount()
{
    if( m_pcInst    == NULL )
    {
        return NULL;
    }
    
    CTask *pcTask = g_cCuiDataMgr.GetTaskPtrBySession( m_strUserSession, m_pcInst );
    if( pcTask == NULL || strcmp( pcTask->GetObjName(), LOGIN_TASK_NAME ) != 0  )
    {
        return NULL;
    }
    CCuAgent *pcCuAgent = (CCuAgent *)pcTask;
    return pcCuAgent->m_pcUserAccount;
}

void CUserStreamNumRestorer::SetUserLoginSession( const string &strUserSession )
{
    m_strUserSession = strUserSession;
}


bool CUserStreamNumRestorer::Start()
{
    CUserAccount*pcUserAccountTask = FindUserAccount();
    if( pcUserAccountTask == NULL )
    {
        return false;
    }
    m_bStarted = pcUserAccountTask->IncreaseConcurrency();
    return m_bStarted;
}

void CUserStreamNumRestorer::Stop()
{
    if( m_bStarted )
    {
        CUserAccount*pcUserAccountTask = FindUserAccount();
        if( pcUserAccountTask == NULL )
        {
            return;
        }
        pcUserAccountTask->DecreaseConcurrency();
        m_bStarted = false;
    }
}
