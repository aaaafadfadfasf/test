/*========================================================================================
ģ����    ��pui
�ļ���    ��videncparamgettask.h
����ļ�  ��
ʵ�ֹ���  ����Ƶ���������ȡ����
����      ��fanxg
��Ȩ      ��<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
�޸ļ�¼��
����               �汾              �޸���             �߶���              �޸ļ�¼
2012/02/22         V1.0              fanxg                                  �½��ļ�
=========================================================================================*/

#ifndef _PU_VID_ENC_PARAM_GET_TASK_H
#define _PU_VID_ENC_PARAM_GET_TASK_H

#include "cms/ospsip/ospsip.h"
#include "cms/cmu/cmu_proto.h"

struct TMTIVideoEncParam;

class CVidEncParamGetTask : public CSipTask
{
public:
    enum
    {
        Wait = UNKNOWN_STATE+1,
        WaitRsp,
    };

public:
    CVidEncParamGetTask(CInstExt *pcInst);
    virtual ~CVidEncParamGetTask();

public:
    virtual const char* GetObjName() const
    {
        return "CVidEncParamGetTask";
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
    //ҵ���� 
    
public:

    //���ݻ�ȡ
    const CPuVidEncParamGetReq& GetReq() const
    {
        return m_tReq;
    }
    CPuVidEncParamGetReq& GetReq()
    {
        return m_tReq;
    }
    void SetReq(const CPuVidEncParamGetReq& tReq)
    {
        m_tReq = tReq;
    }

private:
    CPuVidEncParamGetReq m_tReq;
};

#endif  //#ifndef _PU_VID_ENC_PARAM_GET_TASK_H