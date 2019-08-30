/*========================================================================================
ģ����    ��ospsip
�ļ���    ��ospsiproutessertask.h
����ļ�  ��
ʵ�ֹ���  ����sipproxy����SIP��Ԫ��·�ɱ�
����      ��fanxg
��Ȩ      ��<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
�޸ļ�¼��
����               �汾              �޸���             �߶���              �޸ļ�¼
2013/08/12         V1.0              fanxg                                  �½��ļ�
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
	//״̬����
	u32 OnWaitSs(CMessage *const pcMsg);
	u32 OnWaitSsTimer();

	u32 OnService(CMessage *const pcMsg);
	u32 OnServiceTimer();

public:
    void ReleaseResource();

public:
	//ҵ����
    void SendSsReq();
    void StartSs();
    void SendRouteTableToConvertor(const string& strRouteTable);

public:
	//���ݲ���
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

    KDSIP_DIALOG_ID m_tSsDlgId;       //���ĶԻ�SIP ID����COspSipMsg�л�ȡ

    string          m_strdevUri;      //������
    set<string>     m_tUaList;        //����UA�б��ձ�ʾ��������UA
};

#endif  //#ifndef _OSPSIP_ROUTE_SSER_TASK_H