#include "conn_taskbase.h"
#include "cctrl.h"


CConnTaskbase::CConnTaskbase( CInstExt *pcInst, ENSyncType enSyncType )
    :CSipTask(pcInst), m_enSyncType(enSyncType)
{
    AppData().AddTasConnTask(this);
}

CConnTaskbase::~CConnTaskbase()
{
    AppData().DelTasConnTask(this);
}

CCtrlAppData& CConnTaskbase::AppData()
{
    return static_cast<CCtrlInst*>(GetInstance())->GetAppData();
}

u32 CConnTaskbase::ProcMsg( CMessage *const pcMsg )
{
    if (OSP_SIP_DISCONNECT == pcMsg->event)
    {
        CLASSLOG(WARNING_LEV, "recved disconnection msg, m_tPeerUri=%s\n", m_tPeerUri.GetURIString().c_str());
        return OnDisconnect();
    }

    return CSipTask::ProcMsg(pcMsg);
}

void CConnTaskbase::SetPeerDomainname( const string &strDomainname )
{
    m_tPeerUri.SetDomain(strDomainname);
}

