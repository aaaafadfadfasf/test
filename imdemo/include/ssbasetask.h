/*========================================================================================
模块名    ：cmu
文件名    ：ssbasetask.h
相关文件  ：
实现功能  ：单点订阅、列表订阅、目录订阅的基类，提供排队式的通知机制
作者      ：xiongxh
版权      ：<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
修改记录：
日期               版本              修改人             走读人              修改记录
2014/05/20         V1.0              xiongxh                                新建文件
=========================================================================================*/

#ifndef _SS_BASE_TASK_H
#define _SS_BASE_TASK_H

#include <list>
#include <queue>
using std::list;
using std::queue;
using std::pair;

#include "cms/ospsip/ospsip.h"

class CEventReq;
class CLoginSession;

// 订阅方式
enum TSsMode
{
	SINGLE_SS,					// 单点订阅
	LIST_SS,					// 列表订阅
	CATALOG_SS,					// 目录订阅
	ALL_SS
};

struct TNtfElement
{
public:
	TNtfElement()
	{
		tSsType     = PU_ON_OFF_STATUS;
        bDirectSend = true;
	}

	TSsType      tSsType;		// 通知类型
    bool         bDirectSend;   // 是否直接发送通知
    list<string> cContent;      // 通知内容保存在cContent中，当bDirectSend为true时，直接发送cContent中的内容，否则先生成，再发送
    string       strExtra1;     // 额外的参数1，当bDirectSend为false时需要生成通知内容，strExtra就是对应的DevUri
    string       strExtra2;     // 额外的参数2，当bDirectSend为false时需要生成通知内容，strExtra就是对应的Session
};

class CSsBaseTask : public CSipTask
{
public:
	enum
	{
		WaitSs = UNKNOWN_STATE+1,
		Notify,
		Service,
	};

public:
	CSsBaseTask(CInstExt *pcInst);
	virtual ~CSsBaseTask();

public:
	virtual const char* GetObjName() const
	{
		return "CSsBaseTask";
	}

	virtual const char* GetStateName(u32 dwState) const
	{
		switch (dwState)
		{
		case WaitSs:
			return "WaitSs";
		case Notify:
			return "Notify";
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
	void ReleaseResource();

public:
	//状态处理
	u32 OnWaitSs(CMessage *const pcMsg);
	u32 OnWaitSsTimer();

	u32 OnNotify(CMessage *const pcMsg);
	u32 OnNotifyTimer();

	u32 OnService(CMessage *const pcMsg);
	u32 OnServiceTimer();

	virtual bool IsNotNeedPollState()
	{
		return (GetState() == WaitSs || GetState() == Service);
	}

public:
	
	// 数据源（CPuData）通过该方法来发布通知
	void PostNotify(const string &strDevId, const TSsType &tSsType, const CEventReq &cNtfReq);

	// 向订阅者发送通知
	void PostNtf();

    // 生成通知内容
    void GenNtfContent(TNtfElement &tElement);

public:

	// 子类中实现以下纯虚函数来完成具体的业务逻辑
	virtual TSsMode GetSsMode() const = 0;								    // 获取订阅模式
	virtual u32     ProcSsReq(CMessage * const pcMsg) = 0;					// 处理订阅请求
	virtual void    ProcNtfRsp(CMessage * const pcMsg);					    // 处理通知应答
	virtual u32     ProcRefreshReq(CMessage * const pcMsg) = 0;             // 处理刷新订阅请求
	virtual void    ProcUnSsReq(CMessage * const pcMsg) = 0;                // 处理取消订阅请求
    virtual void    ProcRemovePu(const string &strDevUri) = 0;              // 处理设备退网通知
    virtual void    ProcRemoveDomain(const string &strDomainName) = 0;      // 处理域退网通知

public:

	const string& GetSession() const
	{
		return m_strSession;
	}
	string& GetSession()
	{
		return m_strSession;
	}
	void SetSession(const string& strSession)
	{
		m_strSession = strSession;
	}

	const string& GetUserUri() const
	{
		return m_strUserUri;
	}
	string& GetUserUri()
	{
		return m_strUserUri;
	}
	void SetUserUri(const string& tSipUri)
	{
		m_strUserUri = tSipUri;
	}

	bool GetRcvBatchNtf() const
	{
		return m_bRcvBatchNtf;
	}
	void SetRcvBatchNtf(bool bBatchNtf)
	{
		m_bRcvBatchNtf = bBatchNtf;
	}

private:

	KDSIP_DIALOG_ID    m_tSsDlgId;										// 订阅对话SIP ID，从COspSipMsg中获取

	queue<TNtfElement> m_tNtfQueue;										// 通知队列
	u32                m_dwTopNtf;										// 通知队列中的通知数的峰值
	u32                m_dwTotalNtf;									// 收到的通知总数
    u32                m_dwSkipNtf;									    // 跳过的通知总数

	bool               m_bRcvBatchNtf;									// 是否接收批量通知
	string             m_strCurNtf;										// 当前正在发送的通知

	string			   m_strSession;									// 用户登录session
	string			   m_strUserUri;									// 订阅者

    set<string>        m_cGpsDataList;                                  // 通知队列中存在未发送的gpsdata的设备列表
};

#endif  //#ifndef _SS_BASE_TASK_H