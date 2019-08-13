/*========================================================================================
ģ����    ��tvs
�ļ���    ��twschemesavetasktest.h
����ļ�  ��
ʵ�ֹ���  ������ǽԤ����������
����      ��fanxg
��Ȩ      ��<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
�޸ļ�¼��
����               �汾              �޸���             �߶���              �޸ļ�¼
2012/08/01         V1.0              fanxg                                  �½��ļ�
=========================================================================================*/

#ifndef _TW_SCHEME_SAVE_TASK_H
#define _TW_SCHEME_SAVE_TASK_H

#include "cms/ospsip/ospsip.h"
#include "cms/cmu/cmu_proto.h"


class CTwSchemeSaveTask : public CSipTask
{
public:
    enum
    {
        Wait = UNKNOWN_STATE+1,
        Rsp,
    };

public:
    CTwSchemeSaveTask(CInstExt *pcInst);
    virtual ~CTwSchemeSaveTask();

public:
    virtual const char* GetObjName() const
    {
        return "CTwSchemeSaveTask";
    }

    virtual const char* GetStateName(u32 dwState) const
    {
        switch (dwState)
        {
        case Wait:
            return "Wait";
        case Rsp:
            return "Rsp";
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

    u32 OnRsp(CMessage *const pcMsg);
    u32 OnRspTimer();


public:

public:

    //���ݻ�ȡ

private:
};

#endif  //#ifndef _TW_SCHEME_SAVE_TASK_H