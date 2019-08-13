/*========================================================================================
ģ����    ��cmu
�ļ���    ��domainredissynctask.h
����ļ�  ��
ʵ�ֹ���  �������ڲ�ͬ��redisʵ��֮��ͬ������
����      ��xiongxh
��Ȩ      ��<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
�޸ļ�¼��
����               �汾              �޸���             �߶���              �޸ļ�¼
2016/07/04         V1.0              xiongxh                                �½��ļ�
=========================================================================================*/
#ifndef _DOMAIN_REDIS_SYNC_TASK_H
#define _DOMAIN_REDIS_SYNC_TASK_H

#include <iostream>
#include <deque>
using namespace std;

#include "cms/ospext/osptask.h"
#include "cms/cmu/cmu_proto.h"
#include "cmudata.h"

// redisͬ����Ԫ
struct TRedisSyncUnit
{
    string   strDevId;                  // �豸Id
    TSsTypes tSsTypes;                  // ��Ҫͬ������Ϣ����
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
    //״̬����
    u32 OnIdle(CMessage *const pcMsg);
    u32 OnIdleTimer();

    u32 OnSync(CMessage *const pcMsg);
    u32 OnSyncTimer();

    u32 OnService(CMessage *const pcMsg);
    u32 OnServiceTimer();

public:

    //����Ľӿ�
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

    u32                   m_dwSyncNumOnce;                   // ÿ��ͬ��������

    CDomainData          *m_pcSrcDomainData;                 // ͬ��Դ��
    CCmuConnecterTask    *m_pcConnectTask;                   // ����cmu����task

    deque<TRedisSyncUnit> m_cSyncList;                       // ��ͬ���豸�б�
    u32                   m_dwTop;                           // ��ͬ���豸�б�ķ�ֵ
    u32                   m_dwTotal;                         // �Ѵ����ͬ���豸����
};

// keyΪԴ��DomainId
typedef map<string, CDomainRedisSyncTask *> CRedisSyncMap;


#endif  //#ifndef _REDIS_SYNC_TASK_H