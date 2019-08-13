/*========================================================================================
模块名    ：cmu
文件名    ：pubatchregtask.h
相关文件  ：
实现功能  ：pu批量注册事务
作者      ：xiongxh
版权      ：<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
修改记录：
日期               版本              修改人             走读人              修改记录
2016/05/27         V1.0              xiongxh                                新建文件
=========================================================================================*/

#ifndef _PU_BATCH_REG_TASK_H
#define _PU_BATCH_REG_TASK_H

#include "cms/cmu/cmu_struct.h"
#include "cms/uas/uas_proto.h"
#include "loginsession.h"
#include "cmudata.h"
#include "puinotifytask.h"

class CPuData;
class CPuiRegTask;

class CLoginSessionTask : public CLoginSession
{
public:
    enum
    {
        Service = UNKNOWN_STATE + 1
    };
public:
    CLoginSessionTask(CInstExt *pcInst) : CLoginSession(pcInst) {}
    ~CLoginSessionTask() {}

    virtual const char *GetObjName() const { return "CLoginSessionTask"; }

    virtual void InitStateMachine();

    u32 OnService(CMessage *const pcMsg);

	void SetPuiIp(const string& var)
	{
		m_strPuiIp = var;
	}
	virtual string GetJoinInterfaceIp(const string &strSession)
	{
		return m_strPuiIp;
	}

    virtual const char* GetStateName(u32 dwState) const
    {
        switch (dwState)
        {
        case Service:
            return "Service";
        default:
            break;
        }
        return "Unknown State";
    }
    
    virtual bool IsService(void) const { return true; }
    virtual bool IsNotNeedPollState() { return true; }

private:
	string m_strPuiIp;
};

struct TRegExtInfo
{
    TRegExtInfo()
    {
        pcLoginTask = NULL;
    }

    CLoginSessionTask *pcLoginTask;
    string strPuiIp;
};

class CPuBatchRegTask : public CPuRegBaseTask
{
public:
	enum
	{
		Service = UNKNOWN_STATE + 1
	};

public:
	CPuBatchRegTask(CInstExt *pcInst, CPuiRegTask *pcTask);
	virtual ~CPuBatchRegTask();
    virtual bool IsService() const 
    {
        return true;
    }

public:
	virtual const char* GetObjName() const
	{
		return "CPuBatchRegTask";
	}

	virtual const char* GetStateName(u32 dwState) const
	{
		switch (dwState)
		{
		case Service:
			return "Service";
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
    void ReleaseResource();
	virtual void PrintData() const;

	//状态处理
	u32 OnService(CMessage *const pcMsg);
	u32 OnServiceTimer();

    virtual CLoginSessionTask *GetLoginSession(const string &strDevId);
    virtual CPuiRegTask       *GetPuiRegTask();
    virtual void               ForceOffline(const string &strDevId);

    virtual string GetJoinInterfaceIp(const string &strSession);

	map<string, TRegExtInfo>& GetPuSet()
	{
		return m_cPuSet;
	}

protected:
    u32  UnRegProc(const string &strDevId);
    u32  OfflineProc(const string &strKey);

protected:
    // 批量注册处理接口
    void BatchRegProc(const vector<TGeneralData> &cData, u32 dwStartIndex, u32 dwEndIndex, vector<int> &cResult);

    // 注册业务（PreRegProc是无需访问redis的，可以先过滤一下；RegProc是需要先访问redis来获取数据的）
    u32  PreRegProc(const string &strDevId);
    u32  RegProc(const string &strDevId, const TPuRegInfo &tInfo, bool &bOnline);

    u32  NonBatchProc(const string &strType, const string &strId);

public:
	TDeviceLogInfo GetDevLog(CPuData *pcPuData);

private:

	CPuiRegTask *m_pcPuiRegTask;

    // m_cPuSet的key为DevId，value为CLoginSessionTask的指针（用来管理和该设备相关的Task）
    map<string, TRegExtInfo> m_cPuSet;
};

#endif  //#ifndef _PU_BATCH_REG_TASK_H
