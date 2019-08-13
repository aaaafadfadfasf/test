/*========================================================================================
ģ����    ��cmu
�ļ���    ��cuiregtask.h
����ļ�  ��
ʵ�ֹ���  ��cui��¼����
����      ��fanxg
��Ȩ      ��<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
�޸ļ�¼��
����               �汾              �޸���             �߶���              �޸ļ�¼
2012/01/08         V1.0              fanxg                                  �½��ļ�
=========================================================================================*/

#ifndef _CUI_REG_CMU_TASK_H
#define _CUI_REG_CMU_TASK_H

#include <cms/cmu/cmu_proto.h>
#include "loginsession.h"

class CCuLoginTask;

class CCuiRegTask : public CLoginSession
{
public:
    enum
    {
        WaitReg = UNKNOWN_STATE+1,
        Service,
    };

public:
    CCuiRegTask(CInstExt *pcInst);
    virtual ~CCuiRegTask();

public:
    virtual const char* GetObjName() const
    {
        return "CCuiRegTask";
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

public:
    void ReleaseResource();

public:
    //ҵ����
    bool IsService() const
    {
        return (GetState() == Service);
    }

    bool DisProc(CMessage *const pcMsg);

	//֪ͨCUIӳ�����Ϣ
	void NotifyCuiMappingTable();

public:
    //���ݲ���
    const CIpAddrList& GetIpAddrList() const
    {
        return m_cIpAddrList;
    }
    CIpAddrList& GetIpAddrList()
    {
        return m_cIpAddrList;
    }
    void SetIpAddrList(const CIpAddrList& cIpAddrList)
    {
        m_cIpAddrList = cIpAddrList;
        SetDevAddrList(m_cIpAddrList);
    }

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
private:
    CIpAddrList m_cIpAddrList;
	TNetSegmentAllInfo m_tNetSegAllInfo;   //������Ϣ
};

typedef CStrMap<CLoginSession*> CCuiMap;

class CCuiList : public CCuiMap
{
public:
    void PrintData();
};
extern CCuiList g_cCuiList;




#endif  //#ifndef _CUI_REG_CMU_TASK_H