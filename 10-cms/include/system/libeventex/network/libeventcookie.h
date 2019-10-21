#ifndef _LIB_EVENT_COOKIE_H_
#define _LIB_EVENT_COOKIE_H_

#include "system/config/osconfig.h"
#include "system/types/basictypes.h"
#include "libeventex/event.h"

namespace LibEvent{namespace Network{
    class CLibEventCookie
    {
    public:
        CLibEventCookie(Socket hSocket): m_hSocket(hSocket)
        {
            memset(&m_event,0,sizeof(m_event));
        }
    public:
        Socket m_hSocket;
        event m_event;
    protected:
        CLibEventCookie& operator=(const CLibEventCookie&);
        CLibEventCookie(const CLibEventCookie&);
    };
    
}}
#endif
//end of file


