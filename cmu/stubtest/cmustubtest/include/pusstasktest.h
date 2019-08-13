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
=========================================================================================*/

#ifndef _PU_SUBSCIRBE_TASK_H
#define _PU_SUBSCIRBE_TASK_H

#include "cms/ospsip/ospsip.h"

#include <string>
using std::string;
#include <set>
using std::set;

class CPuData;
struct TPuStatus;
struct TPuConfig;
struct TPuDevConfig;
struct TPuServiceStatus;
struct TPuAlarmStatus;
struct TGpsData;

class CPuSubscribeTask : public CSipTask
{
public:
	enum
	{
		WaitSs = UNKNOWN_STATE+1,
		Service,
	};

public:
	CPuSubscribeTask(CInstExt *pcInst);
	virtual ~CPuSubscribeTask();

public:
	virtual const char* GetObjName() const
	{
		return "CPuSubscribeTask";
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

public:
	//状态处理
	u32 OnWaitSs(CMessage *const pcMsg);
	u32 OnWaitSsTimer();

	u32 OnService(CMessage *const pcMsg);
	u32 OnServiceTimer();

public:
    virtual void TerminateTask();

public:
	//业务处理
    void OnOffNtf(const TPuStatus& tPuStatus);
    void ConfigNtf(const TPuDevConfig& tPuConfig);
    void ServiceNtf(const TPuServiceStatus& tPuService);
    void AlarmNtf(const TPuAlarmStatus& tPuAlarm);
    void GpsDataNtf(const TGpsData& tPuAlarm);

public:
	//数据操作
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

    const set<string>& GetStatusType() const
    {
        return m_tStatusTypes;
    }
    set<string>& GetStatusType()
    {
        return m_tStatusTypes;
    }
    void SetStatusType(const set<string>& tStatusTypes)
    {
        m_tStatusTypes = tStatusTypes;
    }

private:

    KDSIP_DIALOG_ID m_tSsDlgId;       //订阅对话SIP ID，从COspSipMsg中获取

    string          m_strUserUri;     //订阅者
    string          m_strSession;     //用户登录session
    string          m_strdevUri;      //被订阅者
    set<string>     m_tStatusTypes;   //订阅的状态种类
};

#endif  //#ifndef _PU_SUBSCIRBE_TASK_H