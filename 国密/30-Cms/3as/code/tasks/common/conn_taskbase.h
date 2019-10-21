#ifndef _CONN_TASKBASE_H_
#define _CONN_TASKBASE_H_
#include "inner_common.h"

class CCtrlAppData;

class CConnTaskbase: public CSipTask
{
public:
    CConnTaskbase(CInstExt *pcInst, ENSyncType enSyncType);
    virtual ~CConnTaskbase();

    CCtrlAppData& AppData();
    virtual const char* GetObjName() const{return "conn_taskbase";}
    virtual const char* GetStateName(u32 dwState) const{return "unknown";}

    const TSipURI& GetPeerUri()const {return m_tPeerUri;}
    const string& GetPeerDomainId() const{return m_strPeerDomainId;}
    void SetPeerDomainname(const string &strDomainname);

    ENSyncType SyncType() const
    {
        return m_enSyncType;
    }

    virtual u32 ProcMsg(CMessage *const pcMsg);

private:
    virtual u32 OnDisconnect() = 0;
protected:
    TSipURI m_tPeerUri;       // ����˵�URI
    string m_strPeerDomainId; // ����˵����
    ENSyncType m_enSyncType; // ͬ������
    CUUID m_strSessionId;    // ��ȡ�ĻỰ��
};

#endif // _CONN_TASKBASE_H_
