#ifndef _KDM_TIMER_H_
#define _KDM_TIMER_H_

#include "osp.h"
#include "kdmfileautolock.h"


typedef void (*KdmTimerHandler)(void* param, u32 dwUserData);

typedef void* HTimer;
#define MAX_ASF_READER					400		//���ɲ���·��

extern HTimer KdmAddTimer(u32 dwInterval, KdmTimerHandler pCallBack, void* context, BOOL32 bTakeSem = TRUE);
//extern BOOL32 AsfDelTimer(HTimer hTimer);
extern BOOL32 KdmDelTimerFromThis( u32 dwParam );//06.01.04

#define			KDMFILE_WATCH_PORT_START		20900
#define			KDMFILE_WATCH_PORT_END			20949

typedef struct tagKdmTimer
{
    void* m_pParam;                 //�ص������Ĳ���
    u32 m_dwInterval;               //��ʱ���ļ��������
	KdmTimerHandler m_pCallBack;	//�ص�����
    u32 m_dwAbsTime;                //��ʱ���ľ��Դ���ʱ��    
    struct tagKdmTimer* m_ptNext;      //�û������ָ��

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

//��ʱ��������,�ڶ�ʱ���߳���ʹ��
class CKdmTimerMgr
{
public:
    CKdmTimerMgr();
    ~CKdmTimerMgr();
    
public:
    //public������Ҫ���л��⣬����selectֻ�ڶ�ʱ�߳����У����Բ���Ҫ����
    BOOL32 Create(s32 nTotalNum);
    void Close();
    void Select(); 
	void Preempt(TKdmMsgTimer tMsgTimer);//����������ָ��,ʹ�ò�������
private:
    TKdmTimer* AddTimer(u32 dwInterval, KdmTimerHandler pCallBack, void* context);
    BOOL32 DeleteTimer(TKdmTimer* ptTimer);
	BOOL32 DeleteTimer(u32 dwParam );
    BOOL32 Check();      
    BOOL32 DeleteFirst();    
    TKdmTimer* m_ptUsedList;
    TKdmTimer* m_ptFreeList;
    TKdmTimer* m_patTimer;    
    s32 m_nControlSocket;//����select����
    s32 m_nClientSocket;//ͨ����m_nControlSocket����Ϣ������ʱ��
    s32 m_nControlPort;
    u32 m_dwRunStartTime;
	s32 m_nTimerCount;//���涨ʱ���ܸ�����ĿǰΪMAX_ASF_READER�� 05.8.24    
   
	u32 m_dwMillSecPerTick;
    CKdmFileSem m_cTimerSem;
private:
    TKdmTimer* NewTimer();
    BOOL32 AddToUseList(TKdmTimer* ptTimer);
    u32 GetRunTimeInMilliseconds(void);    
};


extern CKdmTimerMgr g_tTimerMgr_asf;


#endif  // _KDVTIMER_H_
