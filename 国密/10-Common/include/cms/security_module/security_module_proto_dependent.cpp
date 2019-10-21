#include "security_module_proto_dependent.h"

#ifdef _NO_OSPEXT

#include <map>
using std::map;
#include <string>
using std::string;
#include "osp.h"

class CXSemLock4SecurityModule
{
public:
    CXSemLock4SecurityModule(){if (OspSemBCreate(&m_hSemaphore) == FALSE){OspPrintf(FALSE, TRUE, "OspSemBCreate() 调用失败\n");}}
    ~CXSemLock4SecurityModule(){if (m_hSemaphore != NULL){OspSemDelete(m_hSemaphore);m_hSemaphore = NULL;}}

private:
    CXSemLock4SecurityModule(const CXSemLock4SecurityModule&);
    void operator=(const CXSemLock4SecurityModule&);

public:
    //上锁
    BOOL32 Lock(){if (OspSemTake(m_hSemaphore) == FALSE){OspPrintf(FALSE, TRUE, "OspSemTake() 调用失败\n");return FALSE;} return TRUE;}

    //解锁
    BOOL32 UnLock(){if (OspSemGive(m_hSemaphore) == FALSE){OspPrintf(FALSE, TRUE, "OspSemGive() 调用失败\n");return FALSE;}return TRUE;}

private:
    SEMHANDLE m_hSemaphore;
};

static map<u16,string> s_event_id2desc;
static map<string,u16> s_event_desc2id;
static CXSemLock4SecurityModule s_event_lock;


u16 OspExtEventID(const char* szEventDesc)
{
    u16 ret;
    s_event_lock.Lock();
    map<string,u16>::const_iterator it=s_event_desc2id.find(szEventDesc);
    ret=(it!=s_event_desc2id.end())?it->second:0;
    s_event_lock.UnLock();
    return ret;
}

const string OspExtEventDesc( u16 wEventID )
{
    string ret;
    s_event_lock.Lock();
    map<u16,string>::const_iterator it=s_event_id2desc.find(wEventID);
    ret=(it!=s_event_id2desc.end())?it->second:std::string();
    s_event_lock.UnLock();
    return ret;
}

void OspExtAddEventDescAndID(const char * desc, u16 event)
{
    s_event_lock.Lock();
    s_event_id2desc[event]=desc;
    s_event_desc2id[desc]=event;
    s_event_lock.UnLock();
}

#endif // _NO_OSPEXT
