/*========================================================================================
模块名    ：ospsip
文件名    ：ospsiproutessertask.h
相关文件  ：
实现功能  ：向sipproxy订阅SIP网元的路由表
作者      ：fanxg
版权      ：<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
修改记录：
日期               版本              修改人             走读人              修改记录
2013/08/12         V1.0              fanxg                                  新建文件
=========================================================================================*/

#ifndef _OSPSIP_ROUTE_SSER_TASK_H
#define _OSPSIP_ROUTE_SSER_TASK_H

#include "cms/ospsip/ospsip.h"
#include "cms/cmu/cmu_struct.h"

class COspsipRouteSserTask : public CSipTask
{
public:
	enum
	{
		WaitSs = UNKNOWN_STATE+1,
		Service,
	};

public:
	COspsipRouteSserTask(CInstExt *pcInst);
	virtual ~COspsipRouteSserTask();

public:
	virtual const char* GetObjName() const
	{
        #define   TASK_TYPE_OSPSIP_ROUTE_SSER    "COspsipRouteSserTask"

		return "COspsipRouteSserTask";
	}

	virtual const char* GetStateName(u32 dwState) const
	{
		switch (dwState)
		{
		case WaitSs:
			return "WaitSs";
		case Service:
			return "Service";
		default:
			break;
		}
		return "Unknown State";
	}

public:
	virtual void InitStateMachine();
    virtual void PrintData() const;

public:
	//状态处理
	u32 OnWaitSs(CMessage *const pcMsg);
	u32 OnWaitSsTimer();

	u32 OnService(CMessage *const pcMsg);
	u32 OnServiceTimer();

public:
    void ReleaseResource();

public:
	//业务处理
    void SendSsReq();
    void StartSs();
    void SendRouteTableToConvertor(const string& strRouteTable);

public:
	//数据操作
    const string& GetDevUri() const
    {
        return m_strdevUri;
    }

    string& GetDevUri()
    {
        return m_strdevUri;
    }

    void SetDevUri(const string& tSipUri)
    {
        m_strdevUri = tSipUri;
    }

private:

    KDSIP_DIALOG_ID m_tSsDlgId;       //订阅对话SIP ID，从COspSipMsg中获取

    string          m_strdevUri;      //订阅者
    set<string>     m_tUaList;        //订阅UA列表，空表示订阅所有UA
};

#endif  //#ifndef _OSPSIP_ROUTE_SSER_TASK_H