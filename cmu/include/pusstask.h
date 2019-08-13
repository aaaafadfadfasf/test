/*========================================================================================
模块名    ：cmu
文件名    ：pusstask.h
相关文件  ：
实现功能  ：PU设备状态被订阅者事务，订阅者可以是本域用户，也可以是异域cmu
作者      ：fanxg
版权      ：<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
修改记录：
日期               版本              修改人             走读人              修改记录
2012/01/12         V1.0              fanxg                                  新建文件
2012/02/07         V1.0              fanxg                                  支持异域cmu作为订阅者
2014/05/27         V1.1              xiongxh                                订阅模型重构
=========================================================================================*/

#ifndef _PU_SUBSCIRBE_TASK_H
#define _PU_SUBSCIRBE_TASK_H

#include "cms/ospsip/ospsip.h"
#include "ssbasetask.h"

class CPuSubscribeTask : public CSsBaseTask
{
public:
	CPuSubscribeTask(CInstExt *pcInst);
	virtual ~CPuSubscribeTask();

public:
	virtual const char* GetObjName() const
	{
		return "CPuSubscribeTask";
	}

public:
    virtual void PrintData() const;

public:
    void ReleaseResource();
	void PublishInitialNotify(const TSsTypes &tSsTypes);			// 发布初始通知

public:
	// 实现父类中定义的纯虚函数
	virtual TSsMode GetSsMode() const;								// 获取订阅模式
	virtual u32  ProcSsReq(CMessage * const pcMsg);					// 处理订阅请求
	virtual u32  ProcRefreshReq(CMessage * const pcMsg);            // 处理刷新订阅请求
	virtual void ProcUnSsReq(CMessage * const pcMsg);               // 处理取消订阅请求
    virtual void ProcRemovePu(const string &strDevUri);
    virtual void ProcRemoveDomain(const string &strDomainName);

public:
	//数据操作
    const string& GetDevUri() const          { return m_strDevUri; }
    const set<string>& GetStatusType() const { return m_tSsTypes;  }

private:
    
    string   m_strDevUri;       //被订阅者
    TSsTypes m_tSsTypes;		//订阅的状态种类
};

#endif  //#ifndef _PU_SUBSCIRBE_TASK_H