/*========================================================================================
模块名    ：cmu
文件名    ：pulistsstask.h
相关文件  ：
实现功能  ：PU列表状态被订阅者事务，订阅者可以是本域用户，也可以是异域cmu
作者      ：fanxg
版权      ：<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
修改记录：
日期               版本              修改人             走读人              修改记录
2013/12/12         V1.0              fanxg                                  新建文件
=========================================================================================*/

#ifndef _PULIST_SUBSCIRBE_TASK_H
#define _PULIST_SUBSCIRBE_TASK_H


#include "cms/ospsip/ospsip.h"
#include "ssbasetask.h"

class CPuListSubscribeTask : public CSsBaseTask
{
public:
	CPuListSubscribeTask(CInstExt *pcInst);
	virtual ~CPuListSubscribeTask();

public:
	virtual const char* GetObjName() const
	{
		return "CPuListSubscribeTask";
	}

public:
	virtual void PrintData() const;

public:
	void ReleaseResource();
	void PublishInitialNotify(const TSsDevList &tSsDevList);				// 发布初始通知

public:
	// 实现父类中定义的纯虚函数
	virtual TSsMode GetSsMode() const;													// 获取订阅模式
	virtual u32  ProcSsReq(CMessage * const pcMsg);										// 处理订阅请求
	virtual u32  ProcRefreshReq(CMessage * const pcMsg);								// 处理刷新订阅请求
	virtual void ProcUnSsReq(CMessage * const pcMsg);									// 处理取消订阅请求
    virtual void ProcRemovePu(const string &strDevUri);                                 // 处理设备退网通知
    virtual void ProcRemoveDomain(const string &strDomainName);                         // 处理域退网通知


private:

    TSsDevList m_tSsDevList;
};

#endif  //#ifndef _PULIST_SUBSCIRBE_TASK_H