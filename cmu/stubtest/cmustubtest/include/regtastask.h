/*========================================================================================
ģ����    ��cmu
�ļ���    ��regtastask.h
����ļ�  ��
ʵ�ֹ���  ����¼TAS����
����      ��fanxg
��Ȩ      ��<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
�޸ļ�¼��
����               �汾              �޸���             �߶���              �޸ļ�¼
2011/12/18         V1.0              fanxg                                  �½��ļ�
=========================================================================================*/

#ifndef _CMU_REG_TAS_TASK_H
#define _CMU_REG_TAS_TASK_H

#include "cms/ospsip/ospsip.h"

class CRegTasTask : public CSipTask
{
public:
    enum
    {
        Idle = UNKNOWN_STATE + 1,
        WaitRegTas,
        WaitGetDev,
        Service,
    };

public:
    CRegTasTask(CInstExt *pcInst);
    virtual ~CRegTasTask();

public:
    virtual const char* GetObjName() const
    {
        return "CRegTasTask";
    }

    virtual const char* GetStateName(u32 dwState) const
    {
        switch (dwState)
        {
        case Idle:
            return "Idle";
        case WaitRegTas:
            return "WaitRegTas";
        case WaitGetDev:
            return "WaitGetDev";
        case Service:
            return "Service";
        default:
            break;
        }
        return "Unknown State";
    }

public:
    virtual void InitStateMachine();

public:
    //״̬����
    u32 OnWaitRegTas(CMessage *const pcMsg);
    u32 OnWaitRegTasTimer();

    u32 OnWaitGetDev(CMessage *const pcMsg);
    u32 OnWaitGetDevTimer();

    u32 OnService(CMessage *const pcMsg);
    u32 OnServiceTimer();

public:
    //ҵ����
    u32 StartRegTas();
    u32 GetDevInfo();
    void TasDisProc(CMessage *const pcMsg);

public:
    //���ݲ���
    const string& GetUserName() const
    {
        return m_strUserName;
    }
    void SetUserName(const string& strUserName)
    {
        m_strUserName = strUserName;
    }
    const string& GetPassWord() const
    {
        return m_strPassWord;
    }
    void SetPassWord(const string& strPassWord)
    {
        m_strPassWord = strPassWord;
    }
    const string& GetSessionID() const
    {
        return m_strSessId;
    }
    void SetSessionID(const string& strSessId)
    {
        m_strSessId = strSessId;
    }

    const TSipURI& GetTasUri() const
    {
        return m_tTasURI;
    }
    void SetTasUri(const TSipURI& tSipUri)
    {
        m_tTasURI = tSipUri;
    }

private:
    string m_strUserName;    //CMU��¼3AS���û���
    string m_strPassWord;    //CMU��¼3AS������
    string m_strSessId;      //CMU��¼3AS��SessId
    TSipURI m_tTasURI;       //3AS��URI

    s32 m_nPuDevTotalNum;     //����PU��
    s32 m_nPuDevGetNum;       //�Ѿ���ȡ��PU��
};


#endif  //#ifndef _CMU_REG_TAS_TASK_H