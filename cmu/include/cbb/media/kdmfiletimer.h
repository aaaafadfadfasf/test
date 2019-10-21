#ifndef _KDM_TIMER_H_
#define _KDM_TIMER_H_

#include "osp.h"
#include "kdmfileautolock.h"


typedef void (*KdmTimerHandler)(void* param, u32 dwUserData);

typedef void* HTimer;
#define MAX_ASF_READER					400		//最多可播放路数

extern HTimer KdmAddTimer(u32 dwInterval, KdmTimerHandler pCallBack, void* context, BOOL32 bTakeSem = TRUE);
//extern BOOL32 AsfDelTimer(HTimer hTimer);
extern BOOL32 KdmDelTimerFromThis( u32 dwParam );//06.01.04

#define			KDMFILE_WATCH_PORT_START		20900
#define			KDMFILE_WATCH_PORT_END			20949

typedef struct tagKdmTimer
{
    void* m_pParam;                 //回调函数的参数
    u32 m_dwInterval;               //定时器的间隔毫秒数
	KdmTimerHandler m_pCallBack;	//回调函数
    u32 m_dwAbsTime;                //定时器的绝对触发时刻    
    struct tagKdmTimer* m_ptNext;      //用户链表的指针

	tagKdmTimer()
	{
		m_pCallBack = NULL;
		m_pParam = NULL;
		m_ptNext = NULL;
		m_dwInterval = 0;
		m_dwAbsTime = 0;
	};
	~tagKdmTimer()
	{
		m_pCallBack = NULL;
		m_pParam = NULL;
		m_ptNext = NULL;
		m_dwInterval = 0;
		m_dwAbsTime = 0;
	}

}TKdmTimer;

enum em_Timer_Msg
{
    em_Timer_Add = 1,
    em_Timer_Delete,
    em_Timer_Null,
};
typedef struct tagKdmMsgTimer
{
    u8 byTimerType;
    void* m_pParam;
    u32 m_dwInterval;
	KdmTimerHandler m_pCallBack;

    tagKdmMsgTimer()
    {
        byTimerType = em_Timer_Null;
        m_pCallBack = NULL;
		m_pParam = NULL;		
		m_dwInterval = 0;
    }
}TKdmMsgTimer;

//定时器管理类,在定时器线程中使用
class CKdmTimerMgr
{
public:
    CKdmTimerMgr();
    ~CKdmTimerMgr();
    
public:
    //public函数需要进行互斥，但是select只在定时线程运行，所以不需要互斥
    BOOL32 Create(s32 nTotalNum);
    void Close();
    void Select(); 
	void Preempt(TKdmMsgTimer tMsgTimer);//参数不能用指针,使用参数拷贝
private:
    TKdmTimer* AddTimer(u32 dwInterval, KdmTimerHandler pCallBack, void* context);
    BOOL32 DeleteTimer(TKdmTimer* ptTimer);
	BOOL32 DeleteTimer(u32 dwParam );
    BOOL32 Check();      
    BOOL32 DeleteFirst();    
    TKdmTimer* m_ptUsedList;
    TKdmTimer* m_ptFreeList;
    TKdmTimer* m_patTimer;    
    s32 m_nControlSocket;//控制select函数
    s32 m_nClientSocket;//通过向m_nControlSocket发消息启动定时器
    s32 m_nControlPort;
    u32 m_dwRunStartTime;
	s32 m_nTimerCount;//保存定时器总个数，目前为MAX_ASF_READER个 05.8.24    
   
	u32 m_dwMillSecPerTick;
    CKdmFileSem m_cTimerSem;
private:
    TKdmTimer* NewTimer();
    BOOL32 AddToUseList(TKdmTimer* ptTimer);
    u32 GetRunTimeInMilliseconds(void);    
};


extern CKdmTimerMgr g_tTimerMgr_asf;


#endif  // _KDVTIMER_H_
