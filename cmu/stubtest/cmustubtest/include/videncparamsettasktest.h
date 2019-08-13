/*========================================================================================
ģ����    ��cmu
�ļ���    ��videncparamsettask.h
����ļ�  ��
ʵ�ֹ���  ����Ƶ���������������
����      ��fanxg
��Ȩ      ��<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
�޸ļ�¼��
����               �汾              �޸���             �߶���              �޸ļ�¼
2012/02/22         V1.0              fanxg                                  �½��ļ�
=========================================================================================*/

#ifndef _PU_VID_ENC_PARAM_SET_TASK_H
#define _PU_VID_ENC_PARAM_SET_TASK_H

#include "cms/ospsip/ospsip.h"
#include "cms/cmu/cmu_proto.h"

struct TMTIVideoEncParam;

class CVidEncParamSetTask : public CSipTask
{
public:
    enum
    {
        Wait = UNKNOWN_STATE+1,
        WaitRsp,
    };

public:
    CVidEncParamSetTask(CInstExt *pcInst);
    virtual ~CVidEncParamSetTask();

public:
    virtual const char* GetObjName() const
    {
        return "CVidEncParamSetTask";
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
    const CPuVidEncParamSetReq& GetReq() const
    {
        return m_tReq;
    }
    CPuVidEncParamSetReq& GetReq()
    {
        return m_tReq;
    }
    void SetReq(const CPuVidEncParamSetReq& tReq)
    {
        m_tReq = tReq;
    }

private:
    CPuVidEncParamSetReq m_tReq;    //������Ϣ��
};

#endif  //#ifndef _PU_VID_ENC_PARAM_SET_TASK_H