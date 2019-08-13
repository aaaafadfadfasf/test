/*========================================================================================
ģ����    ��pui
�ļ���    ��punormalcfgtask.h
����ļ�  ��
ʵ�ֹ���  ��puͨ�õĲ����������񣬰������úͻ�ȡ
����      ��fanxg
��Ȩ      ��<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
�޸ļ�¼��
����               �汾              �޸���             �߶���              �޸ļ�¼
2012/08/16         V1.0              fanxg                                  �½��ļ�
=========================================================================================*/

#ifndef _PU_NORMAL_CFG_TASK_H
#define _PU_NORMAL_CFG_TASK_H

#include "cms/ospsip/ospsip.h"
#include "cms/cms_proto.h"

class CPuNormalCfgTask : public CSipTask
{
public:
    enum
    {
        Wait = UNKNOWN_STATE+1,
        Rsp,
    };

public:
    CPuNormalCfgTask(CInstExt *pcInst);
    virtual ~CPuNormalCfgTask();

public:
    virtual const char* GetObjName() const
    {
        return "CPuNormalCfgTask";
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


private:
};

#endif  //#ifndef _PU_NORMAL_CFG_TASK_H