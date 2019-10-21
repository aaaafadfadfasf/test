/*========================================================================================
模块名    ：线程类封装
文件名    ：xthread.h 
相关文件  ：
实现功能  ：线程类封装
作者      ：fanxg
版权      ：<Copyright(C) 2003-2008 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
修改记录：
日期               版本              修改人             走读人              修改记录
2009/04/10         V1.0              范小钢                                 新建文件
=========================================================================================*/

#ifndef _XTHREAD_INCLUDED_
#define _XTHREAD_INCLUDED_

#if defined(WIN32)

#include <windows.h>

#define  DEFAULT_STACK_SIZE       0
#define  DEFAULT_THREAD_PRIORITY  0

#define MS_VC_EXCEPTION 0x406D1388

typedef struct tagTHREADNAME_INFO
{
	DWORD dwType; // Must be 0x1000.
	LPCSTR szName; // Pointer to name (in user addr space).
	DWORD dwThreadID; // Thread ID (-1=caller thread).
	DWORD dwFlags; // Reserved for future use, must be zero.
} THREADNAME_INFO;


class CThread  //线程类
{
private:
	HANDLE m_hThread;                             //线程句柄
	int    m_nThreadPriority;
	DWORD  m_dwThreadStackSize;
	
private:
	static DWORD WINAPI ThreadBody(LPVOID pParam)
	{
		if (!pParam)
			return DWORD(-1);
		
		CThread *pThis = (CThread *)pParam;
		pThis->SetThreadName(-1, "win-thread");
		return pThis ->Run();		
	}

	void SetThreadName( DWORD dwThreadID, LPCSTR szThreadName)
	{
		THREADNAME_INFO info;
		info.dwType = 0x1000;
		info.szName = szThreadName;
		info.dwThreadID = dwThreadID;
		info.dwFlags = 0;
		
		__try
		{
			RaiseException( MS_VC_EXCEPTION, 0, sizeof(info)/sizeof(DWORD), (DWORD*)&info );
		}
		__except(EXCEPTION_CONTINUE_EXECUTION)
		{
		}
	}
	
protected:	
	virtual DWORD Run() = 0;
	
public:		
	CThread(HANDLE hThread=NULL, 
		    int nThreadPriority=DEFAULT_THREAD_PRIORITY, 
			DWORD dwThreadStackSize=DEFAULT_STACK_SIZE) 
			:m_hThread(hThread), m_nThreadPriority(nThreadPriority), m_dwThreadStackSize(dwThreadStackSize){};
	
private:
    
    // 禁止拷贝构造函数。
    CThread(const CThread&);
	// 禁止赋值操作符。
    void operator=(const CThread&);
	
public:
	
	BOOL Start() //创建线程并启动
	{   
		if (m_hThread)
			return FALSE;
		
		m_hThread = ::CreateThread(NULL, m_dwThreadStackSize, (LPTHREAD_START_ROUTINE)CThread::ThreadBody, (LPVOID)this, 0, NULL);

		if (NULL != m_hThread)
			return FALSE;

		if (abs(m_nThreadPriority) > THREAD_BASE_PRIORITY_MAX && m_nThreadPriority != THREAD_PRIORITY_TIME_CRITICAL)
		{
			m_nThreadPriority = 0;
		}

		::SetThreadPriority(m_hThread, m_nThreadPriority);
		
		return TRUE;		
	}
	
	void Join() //等待线程退出
	{ 
		::WaitForSingleObject(m_hThread,INFINITE);
	}

	DWORD SuspendThread()
	{
		return ::SuspendThread(m_hThread);
	}

	DWORD ResumeThread()
	{
		return ::ResumeThread(m_hThread);
	}
	
	void KillThread()
	{
		::TerminateThread(m_hThread, -1);
		m_hThread = NULL;
	}
	
	BOOL IsAlive() 
	{
		
        if(NULL == m_hThread) 
		{
            return FALSE;
        }
		
        DWORD dwRes = ::WaitForSingleObject(m_hThread, 0);
        
        if(WAIT_TIMEOUT == dwRes) 
		{
            return TRUE;    
        } 
		else if(WAIT_OBJECT_0 == dwRes || WAIT_FAILED == dwRes) 
		{
            return FALSE;
        } 
		else 
		{
            return FALSE;
        }
    }
	
	void Sleep(DWORD dwMilliseconds) 
	{
        ::Sleep(dwMilliseconds);
    }
};

#elif defined(_LINUX_)

#include <unistd.h>
#include <limits.h>
#include <pthread.h>
#include <sys/prctl.h>

#ifndef PR_SET_NAME
#define PR_SET_NAME 15
#endif

typedef int                              BOOL;
typedef unsigned long                    DWORD;
typedef unsigned char                    byte;

#define TRUE                             1
#define FALSE                            0

#define DEFAULT_STACK_SIZE       (200<<10)
#define DEFAULT_THREAD_PRIORITY  100

class CPthreadCtrl
{
	int             m_Flag;
	pthread_mutex_t m_Mutex;
	pthread_cond_t  m_Cond;

public:
	CPthreadCtrl()
	{
		m_Flag = 1;
		pthread_mutex_init(&m_Mutex, NULL);
		pthread_cond_init(&m_Cond, NULL);
	};

	~CPthreadCtrl()
	{
		m_Flag = 0;
		pthread_mutex_destroy(&m_Mutex);
		pthread_cond_destroy(&m_Cond);
	};

public:
	void Suspend()
	{
		pthread_mutex_lock(&m_Mutex);
		m_Flag--;
		pthread_mutex_unlock(&m_Mutex);
	}

	void Resume()
	{
		pthread_mutex_lock(&m_Mutex);
		m_Flag++;
		pthread_cond_signal(&m_Cond);
		pthread_mutex_unlock(&m_Mutex);
	}

	void Wait()
	{
		pthread_mutex_lock(&m_Mutex);
		while(m_Flag <= 0)
		{
			pthread_cond_wait(&m_Cond, &m_Mutex);
		}
		pthread_mutex_unlock(&m_Mutex); 
	}

};

class CThread  //线程类
{
        pthread_t     m_tThread;                            //线程句柄
		byte          m_byThreadPriority;
	    DWORD         m_dwThreadStackSize;
		char*         m_szThreadName;

		CPthreadCtrl m_ThreadCtrl;

        static void* ThreadBody(void* pParam)
        {
			if (!pParam)
				return NULL;
					
			CThread *pThis = (CThread *)pParam ;
			pThis->m_ThreadCtrl.Wait();
			
			if (pThis->m_szThreadName != NULL)
			{
				prctl(PR_SET_NAME, (DWORD)pThis->m_szThreadName);
			}

			pThis ->Run();

			return NULL;
        }

protected:
        virtual DWORD Run() = 0;

        typedef void* (*POSIX_THREAD_ROUTINE) (void* pParam);

public:
        CThread(pthread_t tThread = 0,
			    byte byThreadPriority = DEFAULT_THREAD_PRIORITY,
				DWORD dwThreadStackSize = DEFAULT_STACK_SIZE) 
				:m_tThread(tThread), m_byThreadPriority(byThreadPriority), m_dwThreadStackSize(dwThreadStackSize),m_szThreadName(NULL){};

private:

    // 禁止拷贝构造函数。
    CThread(const CThread&);
    // 禁止赋值操作符。
    void operator=(const CThread&);

public:
        BOOL Start() //创建线程并启动
        {
			if (m_tThread)
				return FALSE;
			
			pthread_attr_t attr;
			if(0 != pthread_attr_init(&attr))
			{
				return FALSE;
			}

			//设定优先级
			sched_param tSchParam;			
			pthread_attr_getschedparam(&attr, &tSchParam);
			tSchParam.sched_priority = m_byThreadPriority;
			pthread_attr_setschedparam(&attr, &tSchParam);

			//设置栈大小
			pthread_attr_setstacksize(&attr, m_dwThreadStackSize);

			//创建线程
			int nRes = pthread_create(&m_tThread,
				                      &attr,
				                      (POSIX_THREAD_ROUTINE)ThreadBody,
				                      (void*)this);
			
			pthread_attr_destroy(&attr);
		
			if(0 != nRes)
			{
				return FALSE;
			}
			
			pthread_detach(m_tThread);
			
			return TRUE;
        }

        void Join() //等待线程退出
        {
            for(;;)
            {
				if(!IsAlive())                      
				{
					return;
				}
				else
				{
					Sleep(10);
				}
			}
        }

		void SuspendThread()
		{
			m_ThreadCtrl.Suspend();
		}
		
		void ResumeThread()
		{
			m_ThreadCtrl.Resume();
		}

        void KillThread()
        {
			if (m_tThread != 0)
			{
				::pthread_cancel(m_tThread);
				m_tThread = 0;
			}
        }

        BOOL IsAlive()
        {
			if (m_tThread == 0) return FALSE;

			int nPolicy;
			struct sched_param tSp;
			int nRes = pthread_getschedparam(m_tThread, &nPolicy, &tSp);
			return (0 == nRes);
		}

        void Sleep(DWORD dwMilliseconds)
        {
			int nS  = dwMilliseconds / 1000;
			int nUs = (dwMilliseconds % 1000) * 1000;
			
			if(nS > 0) 
			{
				sleep(nS);
			}
			
			if(nUs > 0) 
			{
				usleep(nUs);
			}
		}
};

#else

    #error "Not supported OS type"

#endif // #if defined(_LINUX_)



template <class T>
class CXThread :public CThread
{
public:
	CXThread() :CThread(), m_pTheadFuncObj(NULL), m_pmfThreadBody(NULL){};
	virtual ~CXThread(){};

private:  
    // 禁止拷贝构造函数。
    CXThread(const CXThread&);
	// 禁止赋值操作符。
    void operator=(const CXThread&);

private:
	typedef DWORD (T::*ThreadFunc)();
	ThreadFunc m_pmfThreadBody;
	T* m_pTheadFuncObj; 

protected:			
	virtual DWORD Run() //重载线程方法
	{
		if (m_pTheadFuncObj != NULL && m_pmfThreadBody != NULL)
			return (m_pTheadFuncObj->*m_pmfThreadBody)();
		else
			return DWORD(-1);
	} 	
	
public:

    CXThread (T& obj, ThreadFunc threadfunc, int byThreadPriority = DEFAULT_THREAD_PRIORITY, DWORD dwThreadStackSize = DEFAULT_STACK_SIZE)
		     :CThread(0, byThreadPriority, dwThreadStackSize), m_pTheadFuncObj(&obj), m_pmfThreadBody(threadfunc) {}

public:
	BOOL StartThread(T& obj, ThreadFunc pmfThreadfunc)
	{
		m_pTheadFuncObj = &obj;
		m_pmfThreadBody = pmfThreadfunc;

		return CThread::Start();
	}
};

//sample
/*
class CObj
{
public:
    DWORD Do()
    {
        CXThread<CObj> thread_do;
        thread_do.StartThread(*this, &CObj::Upload);

        return 0;
    }

    DWORD Upload()
    {
        //镜像操作代码
        //...
        return 0;
    }

    DWORD Download()
    {
        //镜像操作代码
        //...
        return 0;
    }
};
*/


#endif // #ifndef _XTHREAD_INCLUDED_
