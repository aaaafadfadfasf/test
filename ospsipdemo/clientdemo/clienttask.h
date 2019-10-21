/*****************************************************************************
   ģ����      : cui_fcgi 
   �ļ���      : clienttask.h
   ����ļ�    : 
   �ļ�ʵ�ֹ���: client task����
   ����        : liangli
   �汾        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
   2011/10/10   1.0         liangli        ����
******************************************************************************/
#ifndef _CLIENT_TASK_H_
#define _CLIENT_TASK_H_
#include "cms/ospext/ospext.h"
#include "cms/ospsip/ospsip.h"

class CClientTask : public CSipTask
{
public:
    enum
    {
        IDLE_STATE = UNKNOWN_STATE + 1,  //��ʼ״̬, ���Է���ע������
        WAIT_REG_RSP_STATE,                  //�ȴ�����RegRsp
        SERVICE_STATE,                  //����̬, ���Է�����Ϣ�����ģ��Լ����չ㲥
    };
public:
    CClientTask(CInstExt *pcInst);
    ~CClientTask();
public:
    //������Ҫ��д�ķ���
    virtual const char* GetObjName() const
    {
        return "CClientTask";
    }
    virtual const char* GetStateName(u32 dwState);
    virtual void InitStateMachine();
    virtual void TerminateTask();
public:
    u32 WaitRegRspProcMsg(CMessage *const pcMsg);
    u32 WaitRegRspTimerPoll();
    u32 ServiceProcMsg(CMessage *const pcMsg);
public:
    //�����Լ��ķ���
    u32 StartReg();
    u32 StartUnReg();
    u32 SendMsg(const string strMsg);
    u32 StartSubscribe();
    u32 StopSubscribe();


	u32 StartInvite();
	u32 StopInvite();
    u32 SendInfoMsg();
    u32 SendDlgMsg();
public:
    //�����Լ������ݳ�Ա
    string m_strUserName;
    string m_strSession;
    TSipURI m_tServerUri;
    TKDSIP_DIALOG_ID m_tSubscribeID;
	TKDSIP_DIALOG_ID m_tInviteID;
};

#endif

