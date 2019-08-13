/*****************************************************************************
ģ����      : cmu 
�ļ���      : nmstask.h
����ļ�    : 
�ļ�ʵ�ֹ���: ����������
����        : fanxg
�汾        : vs_phoenix  Copyright(C) 2009-2015 KDDACOM, All rights reserved.
-----------------------------------------------------------------------------
�޸ļ�¼:
��  ��         �汾        �޸���      �޸�����
2013/04/01     1.0         fanxg        ����
******************************************************************************/

#ifndef _NMS_TASK_H
#define _NMS_TASK_H

#include "cms/container/xmap.h"
#include "cms/ospsip/ospsip.h"
#include "cbb/nms/nmclib.h"
#include "cmudata.h"

extern CNmcLib g_cNmsLib;

#define   CMU_UPDATE_NMS_INFO_INTERVAL   10      //10s

/*
  nms�澯��ˮ��ʹ�ù���
  �豸�����澯��ˮ�ţ����豸��̬���ɡ���Ҫ���ڸ澯�����ͻָ�
  ��ƥ�䡣ͬһ��澯��û�лָ�֮ǰ�����ϱ�����ˮ����ͬһ����
  ��ˮ����Ҫʹ����ˮ�ų�ʵ�֡�
*/

typedef CXMap<TNmsAlarmSrc, TTnmDevAlarmBase>  CNmsOverLoadAlarmMap;

typedef CXMap<TNmsAlarmSrc, TnmAlarmCodeModuleDisconnect>  CNmsDisAlarmMap;

class CNmsTask : public CSipTask
{
public:
    enum
    {
        Service = UNKNOWN_STATE+1,   //������ֻ��һ��״̬����ʱ��ȡcmu��Ϣ�ϱ�������
    };

public:
    CNmsTask(CInstExt *pcInst):CSipTask(pcInst)
    {
    }
    virtual ~CNmsTask()
    {
    }

public:
    virtual const char* GetObjName() const
    {
        return "CNmsTask";
    }

    virtual const char* GetStateName(u32 dwState) const
    {
        switch (dwState)
        {
        case Service:
            return "Service";
        default:
            break;
        }
        return "Unknown State";
    }

    virtual void InitStateMachine();

    //״̬����
    u32 OnService(CMessage *const pcMsg);
    u32 OnServiceTimer();

    //ҵ����
    void SendAlarm(const TNmsAlarm& tAlarm);
    void CheckOverloadAlarm(u32 dwAlarmCode, u32 dwCurNum);

public:

    void PrintDisAlarm() const;
    void PrintOverloadAlarm() const;

private:
    CNmsOverLoadAlarmMap m_cNmsOverloadAlarmMap;
    CNmsDisAlarmMap m_cNmsDisAlarmMap;
    CIndexPool m_cAlarmSNPoll;//��ˮ����Ҫʹ����ˮ�ų�ʵ��
};

#endif  //#ifndef _NMS_TASK_H