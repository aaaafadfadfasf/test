/*========================================================================================
ģ����    ��cmu
�ļ���    ��cmuconnecttask.h
����ļ�  ��
ʵ�ֹ���  ��cmu֮��ĵ�¼����
����      ��fanxg
��Ȩ      ��<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
�޸ļ�¼��
����               �汾              �޸���             �߶���              �޸ļ�¼
2012/02/02         V1.0              fanxg                                  �½��ļ�
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
        TryConn = UNKNOWN_STATE+1,  //��������
        Service,                    //���ӳɹ�
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
    //��������
    u32 Connect();
    u32 StartConnect();

    //״̬����
    u32 OnTryConn(CMessage *const pcMsg);
    u32 OnTryConnTimer();

    u32 OnService(CMessage *const pcMsg);
    u32 OnServiceTimer();

public:
    //ҵ����
    bool IsService() const
    {
        return (GetState() == Service);
    }
    u32 DisProc(CMessage *const pcMsg);

    //֪ͨ���������ӽ��
    void NotifyCmuState(u32 dwConnResult);

	virtual TDeviceLogInfo GetDevLogInfo(bool bOnOffState);

    void PrintData() const;

    void SetSyncParam(u32 dwSyncNumOnce);
    void AddSync(const string &strDevId);

    void ExitDomainRedisSyncTask(const CDomainRedisSyncTask *pcTask);
    void ExitDomainPuListSserTask(const CDomainPuListSserTask *pcTask);
    void ExitDomainCatalogSserTask(const CDomainCatalogSserTask *pcTask);

    void StartSync() { m_bRedisSync = true; }

    // �������ӹ����У������������ʧ�ܣ���ҪΪ�����豸��������֪ͨ
    bool DomainPuOfflineProc(const vector<ElementIdType> &cIds);

	void GetRedisRealIp(string& strRedisRealIp);

public:
    //���ݲ���
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
    TSipURI m_tDstURI;      //�Զ�CMU��URI
	TIpAddr m_tPeerIp;      //�Զ�CMU��IP
    bool    m_bSupportRedis;   //�Զ�CMU�Ƿ�֧��redis

    bool    m_bRedisSync;      //redis����ͬ����־

    /* CDomainRedisSyncTask��CDomainPuListSserTask��CDomainCatalogSserTask������Task��CCmuConnecterTask�����˳� */
    CDomainData            *m_pcDomainData;
    CDomainRedisSyncTask   *m_pcDomainRedisSyncTask;
    CDomainPuListSserTask  *m_pcDomainPuListSserTask;
    CDomainCatalogSserTask *m_pcDomainCatalogSserTask;

    CSlidingWindowTask<CCmuConnecterTask> *m_pcNotifyTask;
    u16 m_wConnectTimes;            // ��������ʧ�ܵĴ�������
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