/*****************************************************************************
   模块名      : cui_fcgi 
   文件名      : servertask.h
   相关文件    : 
   文件实现功能: 实现ServerTask定义
   作者        : liangli
   版本        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人      修改内容
   2011/10/10   1.0         liangli        创建
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
    //子类需要重写的方法
    virtual const char* GetObjName() const
    {
        return "CRegTask";
    }
    virtual const char* GetStateName(u32 dwState);
    virtual void InitStateMachine();
    virtual void TerminateTask();
public:
    //状态函数
    //ON REG_STATE
    u32 RegStateProcMsg(CMessage *const pcMsg);

    //ON UNREG_STATE
    u32 UnregStateProcMsg(CMessage *const pcMsg);
public:
    //子类自己的方法
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
    //子类需要重写的方法
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
    //状态函数
    u32 UnSsStateProcMsg(CMessage *const pcMsg);
    u32 SubscribeStateProcMsg(CMessage *const pcMsg);

    //公共接口
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

