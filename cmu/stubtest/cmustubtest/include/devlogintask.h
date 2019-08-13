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

#include "cms/cmu/cmu_struct.h"

#include "loginsession.h"

#define    DEV_LOGIN_TIMEOUT     (10)  //s 

class CLoginTask : public CLoginSession
{
public:
    CLoginTask(CInstExt *pcInst) : CLoginSession(pcInst)
    {

    }
    virtual ~CLoginTask()
    {

    }

public:
    //���ݲ���
    void SetDevUri(const string& strDevUri)
    {
        m_strDevUri = strDevUri;
    }
    string& GetDevUri()
    {
        return m_strDevUri;
    }
    const string& GetDevUri() const
    {
        return m_strDevUri;
    }

    void SetDevType(const string& strDevType)
    {
        m_strDevType = strDevType;
    }
    string& GetDevType()
    {
        return m_strDevType;
    }
    const string& GetDevType() const
    {
        return m_strDevType;
    }

    void SetDevAddrList(const TNetAddrList& cDevAddrList)
    {
        m_tDevAddrList = cDevAddrList;
    }
    TNetAddrList& GetDevAddrList()
    {
        return m_tDevAddrList;
    }
    const TNetAddrList& GetDevAddrList() const
    {
        return m_tDevAddrList;
    }


    void SetServerUri(const string& strServerUri)
    {
        m_strServerUri = strServerUri;
    }
    string& GetServerUri()
    {
        return m_strServerUri;
    }
    const string& GetServerUri() const
    {
        return m_strServerUri;
    }

    //
    TSipURI GetDevSipUri() const
    {
        TSipURI tUri;
        tUri.SetURIString(GetDevUri());
        return tUri;
    }

    TSipURI GetServerSipUri() const
    {
        TSipURI tUri;
        tUri.SetURIString(GetServerUri());
        return tUri;
    }

    virtual void PrintData() const
    {
        CTask::PrintData();

        OspPrintf(TRUE, FALSE, "\n  ��¼������[%s]\n", 
            m_strServerUri.c_str());
        OspPrintf(TRUE, FALSE, "  ��¼�豸[%s-%s]\n", 
           m_strDevType.c_str(), m_strDevUri.c_str());

        OspPrintf(TRUE, FALSE, "  ��¼�豸��ַ�б�\n");
  
        u32 i = 0;

        typedef TNetAddrList::const_iterator   VI;
        for (VI it=m_tDevAddrList.begin(); it!=m_tDevAddrList.end(); ++it)
        {
            i++;
            OspPrintf(TRUE, FALSE, "  %4u. [%s-%d]\n", i, it->GetNetIp().c_str(), it->GetNetPort());
        }    
    }

private:
    //��¼�豸��Ϣ
    string           m_strDevUri;                   //�豸URI
    string           m_strDevType;                  //�豸����
    TNetAddrList     m_tDevAddrList;                //�豸��ַ�б�

    //��¼��������Ϣ
    string           m_strServerUri;                //������URI
};

//��¼����������
class CLoginSenderTask : public CLoginTask
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
class CLoginRecverTask : public CLoginTask
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

typedef CStrMap<CLoginTask*> CLoginTaskMap;

class CGeneralLoginList : public CLoginTaskMap
{
public:
    void PrintData();
};
extern CGeneralLoginList g_cGeneralLoginList;




#endif  //#ifndef _DEV_LOGIN_TASK_H