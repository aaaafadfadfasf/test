#ifndef _SOCKET_EVENT_MANAGER_H_
#define _SOCKET_EVENT_MANAGER_H_

#ifdef _WIN32
#pragma warning (disable : 4786) 
#endif

#include <list>
#include <map>
#include "system/config/osconfig.h"
#include "system/libeventex/event.h"

namespace LibEvent{namespace Network{

class CLibEventCookie;

class CSocketEventManager
{
public:
    CSocketEventManager();
    
    void Add(event_base& evBase,Socket hSocket,
        void (*pFunCallback)(int, short, void *),
        void* pThisOfCallbackObj,
        const timeval& timeoutVal);
    void ReAddWhenTimeout(event_base& evBase,Socket hSocket, void (*pFunCallback)(int, short, void *), void* pThisOfCallbackObj, const timeval& timeoutVal);
    void Remove(Socket hSocket);
    ~CSocketEventManager();
    CLibEventCookie* Search(Socket hSocket);
protected:    
    typedef std::list<CLibEventCookie*> CEventPool;
    CEventPool m_eventPool;
    
    typedef std::map<Socket,CLibEventCookie*> CSocketEventMap;
    CSocketEventMap m_socketEventMap;
    
private:
    CSocketEventManager& operator=(const CSocketEventManager&);
    CSocketEventManager(const CSocketEventManager&);
};

}}

#endif 

//end of file

