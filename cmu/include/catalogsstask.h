/*========================================================================================
ģ����    ��cmu
�ļ���    ��catalogsstask.h
����ļ�  ��
ʵ�ֹ���  ��Ŀ¼�������񣬶����߿����Ǳ����û���Ҳ����������cmu
����      ��fanxg
��Ȩ      ��<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
�޸ļ�¼��
����               �汾              �޸���             �߶���              �޸ļ�¼
2013/12/09         V1.0              fanxg                                  �½��ļ�
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
	void PublishInitialNotify(const TSsTypes &tSsTypes);			// ������ʼ֪ͨ

public:
	// ʵ�ָ����ж���Ĵ��麯��
	virtual TSsMode GetSsMode() const;								// ��ȡ����ģʽ
	virtual u32  ProcSsReq(CMessage * const pcMsg);					// ����������
	virtual u32  ProcRefreshReq(CMessage * const pcMsg);            // ����ˢ�¶�������
	virtual void ProcUnSsReq(CMessage * const pcMsg);               // ����ȡ����������
    virtual void ProcRemovePu(const string &strDevUri);
    virtual void ProcRemoveDomain(const string &strDomainName);

public:
	//���ݲ���
    const string& GetDomain() const       { return m_strDomain; }
    const set<string>& GetSsTypes() const { return m_tSsTypes;  }

private:

    string   m_strDomain;      //�����ĵ�����
    TSsTypes m_tSsTypes;	   //���ĵ�״̬����
};

#endif  //#ifndef _CATALOG_SUBSCIRBE_TASK_H