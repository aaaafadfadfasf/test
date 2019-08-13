/*========================================================================================
ģ����    ��cmu
�ļ���    ��sysparamgettask.h
����ļ�  ��
ʵ�ֹ���  ��ǰ��ϵͳ������ȡ����
����      ��fanxg
��Ȩ      ��<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
�޸ļ�¼��
����               �汾              �޸���             �߶���              �޸ļ�¼
2012/02/22         V1.0              fanxg                                  �½��ļ�
=========================================================================================*/

#ifndef _PU_SYS_PARAM_GET_TASK_H
#define _PU_SYS_PARAM_GET_TASK_H

#include "cms/ospsip/ospsip.h"
#include "cms/cmu/cmu_proto.h"

class CPuSysParamGetTask : public CSipTask
{
public:
    enum
    {
        Wait = UNKNOWN_STATE+1,
        WaitRsp,
    };

public:
    CPuSysParamGetTask(CInstExt *pcInst);
    virtual ~CPuSysParamGetTask();

public:
    virtual const char* GetObjName() const
    {
        return "CPuSysParamGetTask";
    }

    virtual const char* GetStateName(u32 dwState) const
    {
        switch (dwState)
        {
        case Wait:
            return "Wait";
        case WaitRsp:
            return "WaitRsp";
        default:
            break;
        }
        return "Unknown State";
    }

public:
    virtual void InitStateMachine();
    virtual void PrintData();

public:
    //״̬����
    u32 OnWait(CMessage *const pcMsg);
    u32 OnWaitTimer();

    u32 OnWaitRsp(CMessage *const pcMsg);
    u32 OnWaitRspTimer();
    
public:

    //���ݻ�ȡ
    const CPuSysParamGetReq& GetReq() const
    {
        return m_tReq;
    }
    CPuSysParamGetReq& GetReq()
    {
        return m_tReq;
    }
    void SetReq(const CPuSysParamGetReq& tReq)
    {
        m_tReq = tReq;
    }

private:
    CPuSysParamGetReq m_tReq;
};

#endif  //#ifndef _PU_SYS_PARAM_GET_TASK_H