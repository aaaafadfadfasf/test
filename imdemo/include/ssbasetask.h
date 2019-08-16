/*========================================================================================
ģ����    ��cmu
�ļ���    ��ssbasetask.h
����ļ�  ��
ʵ�ֹ���  �����㶩�ġ��б��ġ�Ŀ¼���ĵĻ��࣬�ṩ�Ŷ�ʽ��֪ͨ����
����      ��xiongxh
��Ȩ      ��<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
�޸ļ�¼��
����               �汾              �޸���             �߶���              �޸ļ�¼
2014/05/20         V1.0              xiongxh                                �½��ļ�
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

// ���ķ�ʽ
enum TSsMode
{
	SINGLE_SS,					// ���㶩��
	LIST_SS,					// �б���
	CATALOG_SS,					// Ŀ¼����
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

	TSsType      tSsType;		// ֪ͨ����
    bool         bDirectSend;   // �Ƿ�ֱ�ӷ���֪ͨ
    list<string> cContent;      // ֪ͨ���ݱ�����cContent�У���bDirectSendΪtrueʱ��ֱ�ӷ���cContent�е����ݣ����������ɣ��ٷ���
    string       strExtra1;     // ����Ĳ���1����bDirectSendΪfalseʱ��Ҫ����֪ͨ���ݣ�strExtra���Ƕ�Ӧ��DevUri
    string       strExtra2;     // ����Ĳ���2����bDirectSendΪfalseʱ��Ҫ����֪ͨ���ݣ�strExtra���Ƕ�Ӧ��Session
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
	//״̬����
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
	
	// ����Դ��CPuData��ͨ���÷���������֪ͨ
	void PostNotify(const string &strDevId, const TSsType &tSsType, const CEventReq &cNtfReq);

	// �����߷���֪ͨ
	void PostNtf();

    // ����֪ͨ����
    void GenNtfContent(TNtfElement &tElement);

public:

	// ������ʵ�����´��麯������ɾ����ҵ���߼�
	virtual TSsMode GetSsMode() const = 0;								    // ��ȡ����ģʽ
	virtual u32     ProcSsReq(CMessage * const pcMsg) = 0;					// ����������
	virtual void    ProcNtfRsp(CMessage * const pcMsg);					    // ����֪ͨӦ��
	virtual u32     ProcRefreshReq(CMessage * const pcMsg) = 0;             // ����ˢ�¶�������
	virtual void    ProcUnSsReq(CMessage * const pcMsg) = 0;                // ����ȡ����������
    virtual void    ProcRemovePu(const string &strDevUri) = 0;              // �����豸����֪ͨ
    virtual void    ProcRemoveDomain(const string &strDomainName) = 0;      // ����������֪ͨ

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

	KDSIP_DIALOG_ID    m_tSsDlgId;										// ���ĶԻ�SIP ID����COspSipMsg�л�ȡ

	queue<TNtfElement> m_tNtfQueue;										// ֪ͨ����
	u32                m_dwTopNtf;										// ֪ͨ�����е�֪ͨ���ķ�ֵ
	u32                m_dwTotalNtf;									// �յ���֪ͨ����
    u32                m_dwSkipNtf;									    // ������֪ͨ����

	bool               m_bRcvBatchNtf;									// �Ƿ��������֪ͨ
	string             m_strCurNtf;										// ��ǰ���ڷ��͵�֪ͨ

	string			   m_strSession;									// �û���¼session
	string			   m_strUserUri;									// ������

    set<string>        m_cGpsDataList;                                  // ֪ͨ�����д���δ���͵�gpsdata���豸�б�
};

#endif  //#ifndef _SS_BASE_TASK_H