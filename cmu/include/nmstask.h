/*****************************************************************************
模块名      : cmu 
文件名      : nmstask.h
相关文件    : 
文件实现功能: 网管任务类
作者        : fanxg
版本        : vs_phoenix  Copyright(C) 2009-2015 KDDACOM, All rights reserved.
-----------------------------------------------------------------------------
修改记录:
日  期         版本        修改人      修改内容
2013/04/01     1.0         fanxg        创建
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
  nms告警流水号使用规则：
  设备产生告警流水号，由设备动态生成。主要用于告警产生和恢复
  的匹配。同一类告警在没有恢复之前，其上报的流水号是同一个。
  流水号需要使用流水号池实现。
*/

typedef CXMap<TNmsAlarmSrc, TTnmDevAlarmBase>  CNmsOverLoadAlarmMap;

typedef CXMap<TNmsAlarmSrc, TnmAlarmCodeModuleDisconnect>  CNmsDisAlarmMap;

class CNmsTask : public CSipTask
{
public:
    enum
    {
        Service = UNKNOWN_STATE+1,   //该事务只有一个状态，定时获取cmu信息上报给网管
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

    //状态处理
    u32 OnService(CMessage *const pcMsg);
    u32 OnServiceTimer();

    //业务处理
    void SendAlarm(const TNmsAlarm& tAlarm);
    void CheckOverloadAlarm(u32 dwAlarmCode, u32 dwCurNum);

public:

    void PrintDisAlarm() const;
    void PrintOverloadAlarm() const;

private:
    CNmsOverLoadAlarmMap m_cNmsOverloadAlarmMap;
    CNmsDisAlarmMap m_cNmsDisAlarmMap;
    CIndexPool m_cAlarmSNPoll;//流水号需要使用流水号池实现
};

#endif  //#ifndef _NMS_TASK_H