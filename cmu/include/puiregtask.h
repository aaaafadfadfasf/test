/*========================================================================================
模块名    ：cmu
文件名    ：puiregtask.h
相关文件  ：
实现功能  ：pui登录事务
作者      ：fanxg
版权      ：<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
修改记录：
日期               版本              修改人             走读人              修改记录
2012/01/08         V1.0              fanxg                                  新建文件
=========================================================================================*/

#ifndef _PUI_REG_CMU_TASK_H
#define _PUI_REG_CMU_TASK_H

#include "loginsession.h"

class CPuRegTask;
class CPuBatchRegTask;

class CPuiRegTask : public CLoginSession
{
public:
    enum
    {
        WaitReg = UNKNOWN_STATE+1,
        Service,
    };

public:
    CPuiRegTask(CInstExt *pcInst);
    virtual ~CPuiRegTask();

public:
    virtual const char* GetObjName() const
    {
        return "CPuiRegTask";
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

    CPuBatchRegTask *GetBatchRegTask();

public:
    void ReleaseResource();
	virtual void PrintData() const;

public:
    //业务处理
    bool IsService() const
    {
        return (GetState() == Service);
    }

    bool DisProc(CMessage *const pcMsg);

	u32 GetVidInPortNum();

public:
    //数据操作

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

    bool GetIsSupportRedis() const
    {
        return m_bIsSupportRedis;
    }

private:
	TNetSegmentAllInfo m_tNetSegAllInfo;   //网段信息
    bool m_bIsSupportRedis;
    CPuBatchRegTask *m_pcBatchRegTask;
};

typedef CStrMap<CLoginSession*> CPuiMap;

class CPuiList : public CPuiMap
{
public:
    void PrintData(const string &strPuiType = string());
};
extern CPuiList g_cPuiList;




#endif  //#ifndef _PUI_REG_CMU_TASK_H