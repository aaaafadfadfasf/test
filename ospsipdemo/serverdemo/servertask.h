/*****************************************************************************
   ģ����      : cui_fcgi 
   �ļ���      : servertask.h
   ����ļ�    : 
   �ļ�ʵ�ֹ���: ʵ��ServerTask����
   ����        : liangli
   �汾        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
   2011/10/10   1.0         liangli        ����
******************************************************************************/
#ifndef _SERVER_TASK_H_
#define _SERVER_TASK_H_
#include "cms/ospext/ospext.h"
#include "cms/ospsip/ospsip.h"
#include "../common/commonmsg.h"
#include "observermode.h"


class CRegTask : public CSipTask
{
public:
    enum
    {
        REG_STATE = UNKNOWN_STATE + 1,
        UNREG_STATE,
    };
public:
    CRegTask(CInstExt *pcInst);
    ~CRegTask();
public:
    //������Ҫ��д�ķ���
    virtual const char* GetObjName() const
    {
        return "CRegTask";
    }
    virtual const char* GetStateName(u32 dwState);
    virtual void InitStateMachine();
    virtual void TerminateTask();
public:
    //״̬����
    //ON REG_STATE
    u32 RegStateProcMsg(CMessage *const pcMsg);

    //ON UNREG_STATE
    u32 UnregStateProcMsg(CMessage *const pcMsg);
public:
    //�����Լ��ķ���
    const string GetUserName() const
    {
        return m_strUserName;
    }
    void SetUserName(const string strUserName)
    {
        m_strUserName = strUserName;
    }
    const string GetPassWord() const
    {
        return m_strPassWord;
    }
    void SetPassWord(const string strPassWord)
    {
        m_strPassWord = strPassWord;
    }
    const string GetSessionID() const
    {
        return m_strSessId;
    }
    void SetSessionID(const string strSessId)
    {
        m_strSessId = strSessId;
    }
    const TSipURI GetClientURI() const
    {
        return m_tClientURI;
    }
    void SetClientURI(const TSipURI &tSipURI)
    {
        m_tClientURI = tSipURI;
    }
    void AddTask(CTask *pcTask);
    void DelTask(CTask *pcTask);

	void AddInviteTask(CTask *pcTask);
	void DelInviteTask(CTask *pcTask);
protected:
    string m_strUserName;
    string m_strPassWord;
    string m_strSessId;
    TSipURI m_tClientURI;

    vector<CTask *> m_tTaskArray;
	vector<CTask *> m_InviteArray;
};
typedef CRegTask CUserData;
typedef CUserData * PCUserData;
class CClientObsvr;

class CClientSubscribeTask : public CSipTask
{
public:
    enum
    {
        UNSUBSCRIBE_STATE = UNKNOWN_STATE + 1,
        SUBSCRIBLE_STATE,
    };
public:
    CClientSubscribeTask(CInstExt *pcInst);
    ~CClientSubscribeTask();
public:
    //������Ҫ��д�ķ���
    virtual const char* GetObjName() const
    {
        return "CClientSubscribeTask";
    }
    virtual const char* GetStateName(u32 dwState)
    {
        switch (dwState)
        {
        case UNSUBSCRIBE_STATE:
            {
                return "Unsubscribe State";
            }
        	break;
        case SUBSCRIBLE_STATE:
            {
                return "Subscribed State";
            }
        default:
            break;
        }
        return "Unknown State";
    }
    virtual void InitStateMachine();
    virtual void TerminateTask();
public:
    //״̬����
    u32 UnSsStateProcMsg(CMessage *const pcMsg);
    u32 SubscribeStateProcMsg(CMessage *const pcMsg);

    //�����ӿ�
    const string GetSessionID() const
    {
        return m_strSessId;
    }
    void SetSessionID(const string strSessId)
    {
        m_strSessId = strSessId;
    }
    const TSipURI GetClientURI() const
    {
        return m_tClientURI;
    }
    void SetClientURI(const TSipURI &tSipURI)
    {
        m_tClientURI = tSipURI;
    }
    const TKDSIP_DIALOG_ID GetSubscribeID() const
    {
        return m_tSubscribeID;
    }
    void SetSubscribeID(const TKDSIP_DIALOG_ID tID)
    {
        m_tSubscribeID = tID;
    }

    void BroadcastMsg(const CObsMessage &cMsg);
protected:
    string m_strSessId;
    TSipURI m_tClientURI;
    CClientObsvr *m_pcObsvr;
    TKDSIP_DIALOG_ID m_tSubscribeID;
private:
};

class CClientObsvr : public CObserver
{
public:
    CClientObsvr(CClientSubscribeTask *pcTask, CSubject *pSub);
    virtual ~CClientObsvr(){};
public:
    virtual void Update(CObsMessage &cMsg);
protected:
    CClientSubscribeTask *m_pcTask;
};


class CClientInviteTask : public CSipTask
{
public:
	enum
	{
		UNINVITE_STATE=UNKNOWN_STATE+1,
		INVITE_STATE,
	};
public:
	CClientInviteTask(CInstExt *pcInst);
	~CClientInviteTask();
public:
	virtual const char* GetObjName() const;
	virtual const char* GetStateName(u32 dwState);
	virtual void InitStateMachine();
	virtual void TerminateTask();

public:
	u32 UnInviteProcMsg(CMessage *const pcMsg);
	u32 InviteProcMsg(CMessage *const pcMsg);

public:
	void SetSessId(const string strSessId);
	void SetClientURI(const TSipURI client);
	void SetDialogId(TKDSIP_DIALOG_ID id);
	string GetSessId();
	TSipURI GetClientURE();
	TKDSIP_DIALOG_ID GetDialogId();

protected:
	string m_strSessId;
	TSipURI m_tClientURI;
	TKDSIP_DIALOG_ID m_tInviteID;
};

#endif

