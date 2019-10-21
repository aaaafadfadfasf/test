#ifndef _THREAD_FACTORY_H_
#define _THREAD_FACTORY_H_

#include "kdvtype.h"
#include "system/libeventex/concurrency/threaddefine.h"

struct event_base;
struct event_msgqueue;

namespace LibEvent{namespace Concurrency{
    
    class IRunnable;
    class CQueueMsg;
    
    class CThreadRunner
    {
    public:
        CThreadRunner();
        ~CThreadRunner();
        
        void Stop();
        void SetWorker(IRunnable* pWorker);
        void Run();
        void PostMessage(const CQueueMsg& msg);
        event_base* GetEventBase();
    protected:
        static libevent_result_t LIBEVENT_CALL EventLoop( void * arg );
        static void OnResponse( const CQueueMsg& queueData, void * arg);
        
    private:
        IRunnable* m_pWorker;
        libevent_thread_t m_thread;
        event_base* m_pEventBase;
        event_msgqueue* m_pMsgqueue;
        //libevent_cond_t m_isRunningCond;
        libevent_mutex_t m_isRunningMutex;
        //libevent_cond_t m_isStoppedCond;
        libevent_mutex_t m_isStoppedMutex;
    };
    
}}

#endif

//end of file

