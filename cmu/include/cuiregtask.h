/*========================================================================================
模块名    ：cmu
文件名    ：cuiregtask.h
相关文件  ：
实现功能  ：cui登录事务
作者      ：fanxg
版权      ：<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
修改记录：
日期               版本              修改人             走读人              修改记录
2012/01/08         V1.0              fanxg                                  新建文件
=========================================================================================*/

#ifndef _CUI_REG_CMU_TASK_H
#define _CUI_REG_CMU_TASK_H

#include <cms/cmu/cmu_proto.h>
#include "loginsession.h"

class CCuLoginTask;

class CCuiRegTask : public CLoginSession
{
public:
    enum
    {
        WaitReg = UNKNOWN_STATE+1,
        Service,
    };

public:
    CCuiRegTask(CInstExt *pcInst);
    virtual ~CCuiRegTask();

public:
    virtual const char* GetObjName() const
    {
        return "CCuiRegTask";
    }

    virtual const char* GetStateName(u32 dwState) const
    {
        switch (dwState)
        {
        case WaitReg:
            return "WaitReg";
        case Service:
            return "Service";
        default:
            break;
        }
        return "Unknown State";
    }

    virtual bool IsNotNeedPollState()
    {
        return (GetState() == WaitReg || GetState() == Service);
    }

public:
    virtual void InitStateMachine();

public:
    //状态处理
    u32 OnWaitReg(CMessage *const pcMsg);
    u32 OnWaitRegTimer();

    u32 OnService(CMessage *const pcMsg);
    u32 OnServiceTimer();

public:
    void ReleaseResource();

public:
    //业务处理
    bool IsService() const
    {
        return (GetState() == Service);
    }

    bool DisProc(CMessage *const pcMsg);

	//通知CUI映射表信息
	void NotifyCuiMappingTable();

public:
    //数据操作
    const CIpAddrList& GetIpAddrList() const
    {
        return m_cIpAddrList;
    }
    CIpAddrList& GetIpAddrList()
    {
        return m_cIpAddrList;
    }
    void SetIpAddrList(const CIpAddrList& cIpAddrList)
    {
        m_cIpAddrList = cIpAddrList;
        SetDevAddrList(m_cIpAddrList);
    }

	//网段信息
	const TNetSegmentAllInfo& GetNetSegAllInfo() const
	{
		return m_tNetSegAllInfo;
	}
	TNetSegmentAllInfo& GetNetSegAllInfo()
	{
		return m_tNetSegAllInfo;
	}
	void SetNetSegAllInfo(const TNetSegmentAllInfo& var)
	{
		m_tNetSegAllInfo = var;
	}
private:
    CIpAddrList m_cIpAddrList;
	TNetSegmentAllInfo m_tNetSegAllInfo;   //网段信息
};

typedef CStrMap<CLoginSession*> CCuiMap;

class CCuiList : public CCuiMap
{
public:
    void PrintData();
};
extern CCuiList g_cCuiList;




#endif  //#ifndef _CUI_REG_CMU_TASK_H