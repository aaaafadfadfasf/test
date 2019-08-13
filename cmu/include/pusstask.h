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
2014/05/27         V1.1              xiongxh                                ����ģ���ع�
=========================================================================================*/

#ifndef _PU_SUBSCIRBE_TASK_H
#define _PU_SUBSCIRBE_TASK_H

#include "cms/ospsip/ospsip.h"
#include "ssbasetask.h"

class CPuSubscribeTask : public CSsBaseTask
{
public:
	CPuSubscribeTask(CInstExt *pcInst);
	virtual ~CPuSubscribeTask();

public:
	virtual const char* GetObjName() const
	{
		return "CPuSubscribeTask";
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
    const string& GetDevUri() const          { return m_strDevUri; }
    const set<string>& GetStatusType() const { return m_tSsTypes;  }

private:
    
    string   m_strDevUri;       //��������
    TSsTypes m_tSsTypes;		//���ĵ�״̬����
};

#endif  //#ifndef _PU_SUBSCIRBE_TASK_H