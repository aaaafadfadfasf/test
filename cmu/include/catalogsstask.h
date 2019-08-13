/*========================================================================================
模块名    ：cmu
文件名    ：catalogsstask.h
相关文件  ：
实现功能  ：目录订阅事务，订阅者可以是本域用户，也可以是异域cmu
作者      ：fanxg
版权      ：<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
修改记录：
日期               版本              修改人             走读人              修改记录
2013/12/09         V1.0              fanxg                                  新建文件
=========================================================================================*/

#ifndef _CATALOG_SUBSCIRBE_TASK_H
#define _CATALOG_SUBSCIRBE_TASK_H

#include "cms/ospsip/ospsip.h"
#include "ssbasetask.h"

class CCatalogSubscribeTask : public CSsBaseTask
{
public:
	CCatalogSubscribeTask(CInstExt *pcInst);
	virtual ~CCatalogSubscribeTask();

public:
	virtual const char* GetObjName() const
	{
		return "CCatalogSubscribeTask";
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
    const string& GetDomain() const       { return m_strDomain; }
    const set<string>& GetSsTypes() const { return m_tSsTypes;  }

private:

    string   m_strDomain;      //被订阅的域名
    TSsTypes m_tSsTypes;	   //订阅的状态种类
};

#endif  //#ifndef _CATALOG_SUBSCIRBE_TASK_H