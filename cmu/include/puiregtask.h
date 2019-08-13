/*========================================================================================
ģ����    ��cmu
�ļ���    ��puiregtask.h
����ļ�  ��
ʵ�ֹ���  ��pui��¼����
����      ��fanxg
��Ȩ      ��<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
�޸ļ�¼��
����               �汾              �޸���             �߶���              �޸ļ�¼
2012/01/08         V1.0              fanxg                                  �½��ļ�
=========================================================================================*/

#ifndef _PUI_REG_CMU_TASK_H
#define _PUI_REG_CMU_TASK_H

#include "loginsession.h"

class CPuRegTask;
class CPuBatchRegTask;

class CPuiRegTask : public CLoginSession
{
public:
    enum
    {
        WaitReg = UNKNOWN_STATE+1,
        Service,
    };

public:
    CPuiRegTask(CInstExt *pcInst);
    virtual ~CPuiRegTask();

public:
    virtual const char* GetObjName() const
    {
        return "CPuiRegTask";
    }

    virtual const char* GetStateName(u32 dwState) const
    {
        switch (dwState)
        {
        case WaitReg:
            return "WaitReg";
        case Service:
            return "Service";
        default:
            break;
        }
        return "Unknown State";
    }

    virtual bool IsNotNeedPollState()
    {
        return (GetState() == WaitReg || GetState() == Service);
    }

public:
    virtual void InitStateMachine();

public:
    //״̬����
    u32 OnWaitReg(CMessage *const pcMsg);
    u32 OnWaitRegTimer();

    u32 OnService(CMessage *const pcMsg);
    u32 OnServiceTimer();

    CPuBatchRegTask *GetBatchRegTask();

public:
    void ReleaseResource();
	virtual void PrintData() const;

public:
    //ҵ����
    bool IsService() const
    {
        return (GetState() == Service);
    }

    bool DisProc(CMessage *const pcMsg);

	u32 GetVidInPortNum();

public:
    //���ݲ���

	//������Ϣ
	const TNetSegmentAllInfo& GetNetSegAllInfo() const
	{
		return m_tNetSegAllInfo;
	}
	TNetSegmentAllInfo& GetNetSegAllInfo()
	{
		return m_tNetSegAllInfo;
	}
	void SetNetSegAllInfo(const TNetSegmentAllInfo& var)
	{
		m_tNetSegAllInfo = var;
	}

    bool GetIsSupportRedis() const
    {
        return m_bIsSupportRedis;
    }

private:
	TNetSegmentAllInfo m_tNetSegAllInfo;   //������Ϣ
    bool m_bIsSupportRedis;
    CPuBatchRegTask *m_pcBatchRegTask;
};

typedef CStrMap<CLoginSession*> CPuiMap;

class CPuiList : public CPuiMap
{
public:
    void PrintData(const string &strPuiType = string());
};
extern CPuiList g_cPuiList;




#endif  //#ifndef _PUI_REG_CMU_TASK_H