/*========================================================================================
模块名    ：cmu
文件名    ：cmupassiveconnecttask.h
相关文件  ：
实现功能  ：接收cmu发起的登录事务
作者      ：xiongxh
版权      ：<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
修改记录：
日期               版本              修改人             走读人              修改记录
2016/08/16         V1.0              xiongxh                                新建文件
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

    //通知子事务连接结果
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

    //状态处理
    u32 OnIdle(CMessage *const pcMsg);
    u32 OnIdleTimer();

    u32 OnService(CMessage *const pcMsg);
    u32 OnServiceTimer();

public:
    //业务处理
    u32 DisProc(CMessage *const pcMsg);

public:
    //数据操作
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
    TSipURI m_tDstURI;      //对端CMU的URI
	TIpAddr m_tPeerIp;      //对端CMU的IP
};


#endif  //#ifndef _CMU_PASSIVE_CONNECT_TASK_H