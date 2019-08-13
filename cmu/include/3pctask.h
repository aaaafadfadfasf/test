/*****************************************************************************
模块名      : cmu 
文件名      : 3pctask.h
相关文件    : 
文件实现功能: 交换任务类
作者        : 黄至春
版本        : vs_phoenix  Copyright(C) 2009-2015 KDDACOM, All rights reserved.
-----------------------------------------------------------------------------
修改记录:
日  期         版本        修改人      修改内容
2012/08/02     1.0        黄至春        创建
******************************************************************************/

#ifndef _SWITCH_CREATOR_TASK_H
#define _SWITCH_CREATOR_TASK_H

#include "cms/ospsip/ospsip.h"
#include "cms/container/xmap.h"
#include "cms/cms_errorcode.h"

#include "cms/cmu/cmu_struct.h"
#include "cms/cmu/cmu_proto.h"
#include "cmudata.h"
#include "cmutask.h"
#include "redisswitchunittask.h"

class CSwitchTask;
class C3PCTask : public CCmuTask
{
public:
    C3PCTask(CInstExt *pcInst):CCmuTask(pcInst)
    {
        m_pcDstSwitchTask = NULL;

        this->SetModuleID(CMU_MOD_SWITCH);
    }
    virtual ~C3PCTask()
    {
    }

    /////////////////////////////////////////////////////////////
    //重载函数
public:
    virtual const char* GetObjName() const
    {
        return "C3PCTask";
    }

public:
	//CMS_RESULT StartSwitch(const TChannel& tDstChn, const TChannel& tSrcChn, TSdp& tSdp);

    /////////////////////////////////////////////////////////////
    //成员属性
public:
    void SetDstSwitchTask(CSwitchTask* pcSwitchTask)
    {
        m_pcDstSwitchTask = pcSwitchTask;
    }
    CSwitchTask* GetDstSwitchTask()
    {
        return m_pcDstSwitchTask;
    }

protected:
    CSwitchTask*                m_pcDstSwitchTask;
};

class CRedis3PCDstSwitchUnitTask;
class C3PCSwitchEntryTask : public CSipTask
{
public:
    C3PCSwitchEntryTask(CInstExt *pcInst);
    ~C3PCSwitchEntryTask();

    virtual const char* GetObjName() const
    {
        return "C3PCSwitchEntryTask";
    }

    virtual void ProcSwitchFinished() = 0;
    virtual void ProcSwitchExit(CRedis3PCDstSwitchUnitTask *pcDstSwitch) = 0;

    s32 GetErrorCode() const { return m_nErrorCode; }

protected:
    s32 m_nErrorCode;
    CRedis3PCDstSwitchUnitTask *m_pcDstSwitch;
};

#endif  //#ifndef _SWITCH_CREATOR_TASK_H