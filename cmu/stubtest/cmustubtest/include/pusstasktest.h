/*========================================================================================
ģ����    ��cmu
�ļ���    ��pusstask.h
����ļ�  ��
ʵ�ֹ���  ��PU�豸״̬�����������񣬶����߿����Ǳ����û���Ҳ����������cmu
����      ��fanxg
��Ȩ      ��<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
�޸ļ�¼��
����               �汾              �޸���             �߶���              �޸ļ�¼
2012/01/12         V1.0              fanxg                                  �½��ļ�
2012/02/07         V1.0              fanxg                                  ֧������cmu��Ϊ������
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
	//״̬����
	u32 OnWaitSs(CMessage *const pcMsg);
	u32 OnWaitSsTimer();

	u32 OnService(CMessage *const pcMsg);
	u32 OnServiceTimer();

public:
    virtual void TerminateTask();

public:
	//ҵ����
    void OnOffNtf(const TPuStatus& tPuStatus);
    void ConfigNtf(const TPuDevConfig& tPuConfig);
    void ServiceNtf(const TPuServiceStatus& tPuService);
    void AlarmNtf(const TPuAlarmStatus& tPuAlarm);
    void GpsDataNtf(const TGpsData& tPuAlarm);

public:
	//���ݲ���
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

    KDSIP_DIALOG_ID m_tSsDlgId;       //���ĶԻ�SIP ID����COspSipMsg�л�ȡ

    string          m_strUserUri;     //������
    string          m_strSession;     //�û���¼session
    string          m_strdevUri;      //��������
    set<string>     m_tStatusTypes;   //���ĵ�״̬����
};

#endif  //#ifndef _PU_SUBSCIRBE_TASK_H