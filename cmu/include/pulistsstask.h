/*========================================================================================
ģ����    ��cmu
�ļ���    ��pulistsstask.h
����ļ�  ��
ʵ�ֹ���  ��PU�б�״̬�����������񣬶����߿����Ǳ����û���Ҳ����������cmu
����      ��fanxg
��Ȩ      ��<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
�޸ļ�¼��
����               �汾              �޸���             �߶���              �޸ļ�¼
2013/12/12         V1.0              fanxg                                  �½��ļ�
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
	void PublishInitialNotify(const TSsDevList &tSsDevList);				// ������ʼ֪ͨ

public:
	// ʵ�ָ����ж���Ĵ��麯��
	virtual TSsMode GetSsMode() const;													// ��ȡ����ģʽ
	virtual u32  ProcSsReq(CMessage * const pcMsg);										// ����������
	virtual u32  ProcRefreshReq(CMessage * const pcMsg);								// ����ˢ�¶�������
	virtual void ProcUnSsReq(CMessage * const pcMsg);									// ����ȡ����������
    virtual void ProcRemovePu(const string &strDevUri);                                 // �����豸����֪ͨ
    virtual void ProcRemoveDomain(const string &strDomainName);                         // ����������֪ͨ


private:

    TSsDevList m_tSsDevList;
};

#endif  //#ifndef _PULIST_SUBSCIRBE_TASK_H