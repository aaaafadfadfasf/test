// ***************************************************************
//  threadapp   version:  1.0   ? date: 05/07/2011
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2011 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************
#ifndef _THREAD_APP_H_
#define _THREAD_APP_H_

#include "osp.h"
#include "mailbox.h"

#define DEFAULT_STACK_SIZE       (1<<20)
#define DEFAULT_THREAD_PRIORITY  100
#define MAX_THREAD_NAME_LEN      128

class CThreadApp  
{
public:
    CThreadApp(u8 nThreadPriority=DEFAULT_THREAD_PRIORITY,  u32 dwThreadStackSize=DEFAULT_STACK_SIZE)
                :m_hThread(0), m_byThreadPriority(nThreadPriority), m_dwThreadStackSize(dwThreadStackSize), m_dwThreadId(0)
    {
        strcpy(m_szThreadName, "no name");
    }
    virtual ~CThreadApp(){};
    
    BOOL32 CreateThread(LPSTR pAppName, u8 byPri, u32 dwStackSize)
    {
        if (m_hThread)
            return FALSE;
        
        m_byThreadPriority = byPri;
        m_dwThreadStackSize = dwStackSize;
        
#ifdef _LINUX_
        m_hThread = OspTaskCreate((LINUXFUNC)CThreadApp::ThreadBody, pAppName, m_byThreadPriority, m_dwThreadStackSize, (u32)this, 0, &m_dwThreadId);
#else
        m_hThread = OspTaskCreate(CThreadApp::ThreadBody, pAppName, m_byThreadPriority, m_dwThreadStackSize, (u32)this, 0, &m_dwThreadId);
#endif
        
        if (0 == m_hThread || 0 == m_dwThreadId)
            return FALSE;
        
        if (pAppName != NULL)
        {
            strncpy(m_szThreadName, pAppName, sizeof(m_szThreadName)-1);
            m_szThreadName[sizeof(m_szThreadName)-1] = '\0';
        }

#ifdef _LINUX_
        OspRegTaskInfo(m_dwThreadId, m_szThreadName);
#endif
        
        return TRUE;
    }
    BOOL32 TerminateThread()
    {
        if (!m_hThread)
            return FALSE;
        
        BOOL32 bRet = OspTaskTerminate(m_hThread);
        
        m_hThread = 0;
        m_dwThreadId = 0;
        memset(m_szThreadName, 0, sizeof(m_szThreadName));
        return bRet;
    }
    
    TASKHANDLE GetThreadHandle() const
    {
        return m_hThread;
    }
    u32 GetThreadId() const
    {
        return m_dwThreadId;
    }
    const char* GetThreadName() const
    {
        return m_szThreadName;
    }
    virtual void PrintAppMsg() const{};
    
    int SetPriority(const u8 byPriority)
    {
#ifdef _MSC_VER	
        int nPri;   // 实际设置的优先级
        
        if(byPriority < 50)
        {
            nPri = THREAD_PRIORITY_TIME_CRITICAL;
        }
        else if(byPriority < 100)
        {
            nPri = THREAD_PRIORITY_HIGHEST;
        }
        else if(byPriority < 120)
        {
            nPri = THREAD_PRIORITY_ABOVE_NORMAL;
        }
        else if(byPriority < 150)
        {
            nPri = THREAD_PRIORITY_NORMAL;
        }
        else if(byPriority < 200)
        {
            nPri = THREAD_PRIORITY_BELOW_NORMAL;		
        }	
        else
        {
            nPri = THREAD_PRIORITY_LOWEST;
        }
        
        return SetThreadPriority(m_hThread, nPri);
#endif
    }
    //void SetStackSize(const u32 dwStackSize);
protected:
    TASKHANDLE m_hThread;       //线程句柄, 依据此来对线程进行控制
    u8   m_byThreadPriority;    //线程优先级
    u32  m_dwThreadStackSize;   //线程栈大小
    u32  m_dwThreadId;          //线程编号
    char m_szThreadName[MAX_THREAD_NAME_LEN+1];
    
    virtual u32 Run(void* pArg) = 0;    //线程类必须实现此方法
private:
    static u32 ThreadBody(void* pParam)    //线程运行地址
    {
        if (!pParam)
            return u32(-1);
        
        CThreadApp *pThis = (CThreadApp *)pParam;
        
        pThis->Run(pThis);
        return 0;
    }
};


template<class TMSG>
class CPostDealMsg
{
public:
    void operator()(TMSG &tMsg){}
protected:
private:
};

template<class TMSG, BOOL32 bDelayPop = TRUE, class POST_DEAL_MSG = CPostDealMsg<TMSG> >
class CNoOspApp : public CThreadApp
{
public:
    CNoOspApp(){}
    ~CNoOspApp(){};
public:
    //启动应用
    // APP最高优先级为80，如果小于80，OSP将强制为80。此后, 用户可以调用OspSetAppPriority()或CApp::SetPriority()更改优先级.
    // 默认消息队列100, 栈大小为200 KB 
	//成功返回OSP_OK      //操作成功    失败返回OSP_ERROR  //操作失败
    int CreateApp( char *name, u8 pri = 80, u16 queuesize = 100, u32 uStackSize = 200<<10 )
    {
        BOOL32 bRet = FALSE;
        if ( !m_cMailBox.CreateMailBox(queuesize, INFINITE, bDelayPop) )
        {
            return OSP_ERROR;
        }

        bRet = CreateThread(name, pri, uStackSize);
        return (bRet ? OSP_OK : OSP_ERROR);
    }
    BOOL32 PushMsg(TMSG &tMsg)
    {
        return m_cMailBox.PushMsg(tMsg);
    }
    //实例入口函数
	virtual void InstanceEntry( TMSG &tMsg ) = 0;
    virtual void RoutineEntry()
    {
        //here has done nothing; reload it and do your things if you want
    }
private:
    using CThreadApp::CreateThread;
    virtual u32 Run(void* pArg)
    {
        while (1)
        {
            TMSG tMsg;
            if (m_cMailBox.PopMsg(tMsg))
            {
                InstanceEntry(tMsg);
                POST_DEAL_MSG()(tMsg);
            }
            else
            {
                RoutineEntry();
                //OspPrintf(TRUE, FALSE, "%s Recv Msg Fail\n", m_szThreadName);
                OspDelay(100);
            }
        }
    }
protected:
    CMailBox<TMSG> m_cMailBox;
private:
};


#endif



