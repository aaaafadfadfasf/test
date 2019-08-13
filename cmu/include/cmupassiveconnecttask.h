/*========================================================================================
ģ����    ��cmu
�ļ���    ��cmupassiveconnecttask.h
����ļ�  ��
ʵ�ֹ���  ������cmu����ĵ�¼����
����      ��xiongxh
��Ȩ      ��<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
�޸ļ�¼��
����               �汾              �޸���             �߶���              �޸ļ�¼
2016/08/16         V1.0              xiongxh                                �½��ļ�
=========================================================================================*/

#ifndef _CMU_PASSIVE_CONNECT_TASK_H
#define _CMU_PASSIVE_CONNECT_TASK_H

#include "loginsession.h"

class CCmuPassiveConnecterTask : public CLoginSession
{
public:
    enum
    {
        Idle = UNKNOWN_STATE+1,
        Service,
    };

public:
    CCmuPassiveConnecterTask(CInstExt *pcInst);
    virtual ~CCmuPassiveConnecterTask();

    //֪ͨ���������ӽ��
    void NotifyCmuState(u32 dwConnResult);

public:
    virtual const char* GetObjName() const
    {
        return "CCmuPassiveConnecterTask";
    }

    virtual const char* GetStateName(u32 dwState) const
    {
        switch (dwState)
        {
        case Idle:
            return "Idle";
        case Service:
            return "Service";
        default:
            break;
        }
        return "Unknown State";
    }

    virtual bool IsService() const
    {
        return (Service == GetState());
    }

public:
    virtual void InitStateMachine();

    virtual bool IsNotNeedPollState()
    {
        return true;
    }

public:

    //״̬����
    u32 OnIdle(CMessage *const pcMsg);
    u32 OnIdleTimer();

    u32 OnService(CMessage *const pcMsg);
    u32 OnServiceTimer();

public:
    //ҵ����
    u32 DisProc(CMessage *const pcMsg);

public:
    //���ݲ���
    const TSipURI& GetDstUri() const
    {
        return m_tDstURI;
    }
    TSipURI& GetDstUri()
    {
        return m_tDstURI;
    }
    void SetDstUri(const TSipURI& tSipUri)
    {
        m_tDstURI = tSipUri;
    }

	const TIpAddr& GetPeerIp() const
	{
		return m_tPeerIp;
	}

private:
    TSipURI m_tDstURI;      //�Զ�CMU��URI
	TIpAddr m_tPeerIp;      //�Զ�CMU��IP
};


#endif  //#ifndef _CMU_PASSIVE_CONNECT_TASK_H