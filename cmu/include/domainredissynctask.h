/*========================================================================================
模块名    ：cmu
文件名    ：domainredissynctask.h
相关文件  ：
实现功能  ：负责在不同的redis实例之间同步数据
作者      ：xiongxh
版权      ：<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
修改记录：
日期               版本              修改人             走读人              修改记录
2016/07/04         V1.0              xiongxh                                新建文件
=========================================================================================*/
#ifndef _DOMAIN_REDIS_SYNC_TASK_H
#define _DOMAIN_REDIS_SYNC_TASK_H

#include <iostream>
#include <deque>
using namespace std;

#include "cms/ospext/osptask.h"
#include "cms/cmu/cmu_proto.h"
#include "cmudata.h"

// redis同步单元
struct TRedisSyncUnit
{
    string   strDevId;                  // 设备Id
    TSsTypes tSsTypes;                  // 需要同步的信息类型
};

class CDomainRedisSyncTask : public CTask
{
public:
    enum
    {
        Idle = UNKNOWN_STATE + 1,
        Sync,
        Service
    };

public:
    CDomainRedisSyncTask(CInstExt* pcInst, CDomainData *pcSrcDomainData, CCmuConnecterTask *pcLoginTask);
    virtual ~CDomainRedisSyncTask();

public:
    virtual const char* GetObjName() const
    {
        return "CRedisSyncTask";
    }

    virtual const char* GetStateName(u32 dwState) const
    {
        switch (dwState)
        {
        case Idle:
            return "Idle";
        case Sync:
            return "Sync";
        case Service:
            return "Service";
        default:
            break;
        }
        return "Unknown State";
    }

public:
    virtual void InitStateMachine();
    virtual bool IsNotNeedPollState();
    virtual void PrintData() const;

public:
    //状态处理
    u32 OnIdle(CMessage *const pcMsg);
    u32 OnIdleTimer();

    u32 OnSync(CMessage *const pcMsg);
    u32 OnSyncTimer();

    u32 OnService(CMessage *const pcMsg);
    u32 OnServiceTimer();

public:

    //对外的接口
    void Reset();

    void AddSync();
    void AddSync(const string &strDevId);
    void AddSync(const vector<string> &cDevId);
    
    u32  GetSyncNumOnce() const
    {
        return m_dwSyncNumOnce;
    }
    void SetSyncNumOnce(u32 dwVal)
    {
        m_dwSyncNumOnce = dwVal > 0 ? dwVal : m_dwSyncNumOnce;
    }

    const string &GetSrcDomainId() const
    {
        return m_pcSrcDomainData->GetDomainId();
    }

    u32 GetRestSyncTimes() const
    {
        if (!m_cSyncList.empty())
        {
            return (m_cSyncList.size() / m_dwSyncNumOnce);
        }

        return 0;
    }

protected:
    void Post(const string &strDevId, const TSsTypes &tSsTypes);
    void StartSync();

private:

    u32                   m_dwSyncNumOnce;                   // 每次同步的数量

    CDomainData          *m_pcSrcDomainData;                 // 同步源端
    CCmuConnecterTask    *m_pcConnectTask;                   // 异域cmu连接task

    deque<TRedisSyncUnit> m_cSyncList;                       // 待同步设备列表
    u32                   m_dwTop;                           // 待同步设备列表的峰值
    u32                   m_dwTotal;                         // 已处理的同步设备总数
};

// key为源端DomainId
typedef map<string, CDomainRedisSyncTask *> CRedisSyncMap;


#endif  //#ifndef _REDIS_SYNC_TASK_H