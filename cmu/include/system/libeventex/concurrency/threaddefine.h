#ifndef __THREAD_DEFINES_H__
#define __THREAD_DEFINES_H__

#ifndef WIN32

/// pthread

#include <pthread.h>
#include <unistd.h>

typedef void * libevent_result_t;
typedef pthread_mutex_t libevent_mutex_t;
typedef pthread_cond_t  libevent_cond_t;
typedef pthread_t       libevent_thread_t;
typedef pthread_attr_t  libevent_attr_t;

#define libevent_mutex_init     pthread_mutex_init
#define libevent_mutex_destroy  pthread_mutex_destroy
#define libevent_mutex_lock     pthread_mutex_lock
#define libevent_mutex_unlock   pthread_mutex_unlock

#define libevent_attr_init           pthread_attr_init
#define libevent_attr_destroy        pthread_attr_destroy
#define libevent_attr_setdetachstate pthread_attr_setdetachstate
#define libevent_CREATE_DETACHED     PTHREAD_CREATE_DETACHED
#define libevent_attr_setstacksize   pthread_attr_setstacksize

#define libevent_thread_id    pthread_self
#define libevent_thread_create  pthread_create

#define LIBEVENT_CALL
typedef libevent_result_t ( * LIBEVENT_CALL libevent_func_t )( void * args );

#ifndef sp_sleep
#define sp_sleep(x) sleep(x)
#endif

#else ///////////////////////////////////////////////////////////////////////

// win32 thread

#include "system/config/osconfig.h"

#ifdef __cplusplus
extern "C" {
#endif
    
    typedef unsigned libevent_thread_t;
    
    typedef unsigned libevent_result_t;
#define LIBEVENT_CALL __stdcall
    typedef libevent_result_t ( LIBEVENT_CALL * libevent_func_t )( void * args );
    
    typedef HANDLE  libevent_mutex_t;
    typedef HANDLE  libevent_cond_t;
    
    //typedef DWORD   libevent_attr_t;
    typedef struct taglibevent_attr {
        int stacksize;
        int detachstate;
    } libevent_attr_t;
    
#define libevent_CREATE_DETACHED 1
    
#ifndef sp_sleep
#define sp_sleep(x) Sleep(1000*x)
#endif
    
    int libevent_mutex_init( libevent_mutex_t * mutex, void * attr );
    int libevent_mutex_destroy( libevent_mutex_t * mutex );
    int libevent_mutex_lock( libevent_mutex_t * mutex );
    int libevent_mutex_unlock( libevent_mutex_t * mutex );
    
    int libevent_attr_init( libevent_attr_t * attr );
    int libevent_attr_destroy( libevent_attr_t * attr );
    int libevent_attr_setdetachstate( libevent_attr_t * attr, int detachstate );
    int libevent_attr_setstacksize( libevent_attr_t * attr, size_t stacksize );
    
    int libevent_thread_create( libevent_thread_t * thread, libevent_attr_t * attr,
        libevent_func_t myfunc, void * args );
    libevent_thread_t libevent_thread_id();
    
#ifdef __cplusplus
}
#endif

#endif

#endif
//end of file


