/*========================================================================================
模块名    ：cmu
文件名    ：cmuconnecttask.h
相关文件  ：
实现功能  ：cmu之间的登录事务
作者      ：fanxg
版权      ：<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
修改记录：
日期               版本              修改人             走读人              修改记录
2012/02/02         V1.0              fanxg                                  新建文件
=========================================================================================*/

#ifndef _CMU_CONNECT_TASK_H
#define _CMU_CONNECT_TASK_H

#include "loginsession.h"
#include "slidingwindowtmpl.h"

class CDomainData;
class CDomainRedisSyncTask;
class CDomainCatalogSserTask;
class CDomainPuListSserTask;

class CCmuConnecterTask : public CLoginSession
{
public:
    enum
    {
        TryConn = UNKNOWN_STATE+1,  //尝试连接
        Service,                    //连接成功
    };

    typedef string ElementIdType;

public:
    CCmuConnecterTask(CInstExt *pcInst, CDomainData *pcDomainData);
    virtual ~CCmuConnecterTask();

public:
    virtual const char* GetObjName() const
    {
        return "CCmuConnecterTask";
    }

    virtual const char* GetStateName(u32 dwState) const
    {
        switch (dwState)
        {
        case TryConn:
            return "TryConn";
        case Service:
            return "Service";
        default:
            break;
        }
        return "Unknown State";
    }

public:
    virtual void InitStateMachine();

public:
    //开启连接
    u32 Connect();
    u32 StartConnect();

    //状态处理
    u32 OnTryConn(CMessage *const pcMsg);
    u32 OnTryConnTimer();

    u32 OnService(CMessage *const pcMsg);
    u32 OnServiceTimer();

public:
    //业务处理
    bool IsService() const
    {
        return (GetState() == Service);
    }
    u32 DisProc(CMessage *const pcMsg);

    //通知子事务连接结果
    void NotifyCmuState(u32 dwConnResult);

	virtual TDeviceLogInfo GetDevLogInfo(bool bOnOffState);

    void PrintData() const;

    void SetSyncParam(u32 dwSyncNumOnce);
    void AddSync(const string &strDevId);

    void ExitDomainRedisSyncTask(const CDomainRedisSyncTask *pcTask);
    void ExitDomainPuListSserTask(const CDomainPuListSserTask *pcTask);
    void ExitDomainCatalogSserTask(const CDomainCatalogSserTask *pcTask);

    void StartSync() { m_bRedisSync = true; }

    // 异域连接过程中，如果连续连接失败，需要为在线设备发送下线通知
    bool DomainPuOfflineProc(const vector<ElementIdType> &cIds);

	void GetRedisRealIp(string& strRedisRealIp);

public:
    //数据操作
    const TSipURI& GetDstUri() const
    {
        return m_tDstURI;
    }
    TSipURI& GetDstUri()
    {
        return m_tDstURI;
    }
    void SetDstUri(const TSipURI& tSipUri)
    {
        m_tDstURI = tSipUri;
    }

	const TIpAddr& GetPeerIp() const
	{
		return m_tPeerIp;
	}

    bool GetIsSupportRedis() const
    {
        return m_bSupportRedis;
    }

protected:

    void StartSyncData();

private:
    TSipURI m_tDstURI;      //对端CMU的URI
	TIpAddr m_tPeerIp;      //对端CMU的IP
    bool    m_bSupportRedis;   //对端CMU是否支持redis

    bool    m_bRedisSync;      //redis数据同步标志

    /* CDomainRedisSyncTask、CDomainPuListSserTask、CDomainCatalogSserTask这三个Task由CCmuConnecterTask控制退出 */
    CDomainData            *m_pcDomainData;
    CDomainRedisSyncTask   *m_pcDomainRedisSyncTask;
    CDomainPuListSserTask  *m_pcDomainPuListSserTask;
    CDomainCatalogSserTask *m_pcDomainCatalogSserTask;

    CSlidingWindowTask<CCmuConnecterTask> *m_pcNotifyTask;
    u16 m_wConnectTimes;            // 连续连接失败的次数计数
};

typedef map<string, CLoginSession*> CCmuMap;

class CCmuList
{
public:
    void PrintData();

    void Insert(const string &strSession, CLoginSession *pcTask, bool bInitiative = true);
    void Erase(const string &strSession, bool bInitiative = true);
    bool Find(const string &strSession, CLoginSession *&pcTask, bool bInitiative = true);

private:
    CCmuMap m_cCmuMap;
    CCmuMap m_cCmuPassiveMap;
};
extern CCmuList g_cCmuList;




#endif  //#ifndef _CMU_CONNECT_TASK_H