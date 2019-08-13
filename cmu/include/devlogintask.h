/*========================================================================================
ģ����    ��cms
�ļ���    ��devlogintask.h
����ļ�  ��
ʵ�ֹ���  ��ͨ���豸��¼���񣬷�Ϊ��������: ��¼�����ߺ͵�¼������
����      ��fanxg
��Ȩ      ��<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
�޸ļ�¼��
����               �汾              �޸���             �߶���              �޸ļ�¼
2012/07/06         V1.0              fanxg                                  �½��ļ�
=========================================================================================*/

#ifndef _DEV_LOGIN_TASK_H
#define _DEV_LOGIN_TASK_H

#include "loginsession.h"

#define    DEV_LOGIN_TIMEOUT     (10)  //s 

//��¼����������
class CLoginSenderTask : public CLoginSession
{
public:
    enum
    {
        WaitLogin = UNKNOWN_STATE+1,
        Service,
        WaitLogout,     
    };

public:
    CLoginSenderTask(CInstExt *pcInst);
    virtual ~CLoginSenderTask();

public:
    virtual const char* GetObjName() const
    {
        #define   TASK_TYPE_LOGIN_SENDER    "CLoginSenderTask"

        return "CLoginSenderTask";
    }

    virtual const char* GetStateName(u32 dwState) const
    {
        switch (dwState)
        {
        case WaitLogin:
            return "WaitLogin";
        case Service:
            return "Service";
        case WaitLogout:
            return "WaitLogout";
        default:
            break;
        }
        return "Unknown State";
    }

    virtual bool IsNotNeedPollState()
    {
        return (GetState() == Service);
    }

public:
    virtual void InitStateMachine();

public:
    //״̬����
    u32 OnWaitLogin(CMessage *const pcMsg);
    u32 OnWaitLoginTimer();

    u32 OnService(CMessage *const pcMsg);
    u32 OnServiceTimer();

    u32 OnWaitLogout(CMessage *const pcMsg);
    u32 OnWaitLogoutTimer();

    virtual TDeviceLogInfo GetDevLogInfo(bool bOnOffState);

public:
    void ReleaseResource();

public:
    //ҵ����

    //������¼���ǳ�
    void StartLogin();
    void StartLogout();

    bool IsService() const
    {
        return (GetState() == Service);
    }
};

//��¼����������
class CLoginRecverTask : public CLoginSession
{
public:
    enum
    {
        WaitLogin = UNKNOWN_STATE+1,
        Service, 
    };

public:
    CLoginRecverTask(CInstExt *pcInst);
    virtual ~CLoginRecverTask();

public:
    virtual const char* GetObjName() const
    {
        #define   TASK_TYPE_LOGIN_RECVER    "CLoginRecverTask"

        return "CLoginRecverTask";
    }

    virtual const char* GetStateName(u32 dwState) const
    {
        switch (dwState)
        {
        case WaitLogin:
            return "WaitLogin";
        case Service:
            return "Service";
        default:
            break;
        }
        return "Unknown State";
    }

    virtual bool IsNotNeedPollState()
    {
        return (GetState() == WaitLogin || GetState() == Service);
    }

public:
    virtual void InitStateMachine();

public:
    //״̬����
    u32 OnWaitLogin(CMessage *const pcMsg);
    u32 OnWaitLoginTimer();

    u32 OnService(CMessage *const pcMsg);
    u32 OnServiceTimer();

public:
    void ReleaseResource();

public:
    bool IsService() const
    {
        return (GetState() == Service);
    }
};

typedef CStrMap<CLoginSession*> CLoginTaskMap;

class CGeneralLoginList : public CLoginTaskMap
{
public:
    void PrintData();
};
extern CGeneralLoginList g_cGeneralLoginList;




#endif  //#ifndef _DEV_LOGIN_TASK_H