/*****************************************************************************
   模块名      : cui_fcgi 
   文件名      : serverinst.h
   相关文件    : 
   文件实现功能: 实现ServerInst类的定义
   作者        : liangli
   版本        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人      修改内容
   2011/10/10   1.0         liangli        创建
******************************************************************************/
#ifndef _SERVER_INST_H_
#define _SERVER_INST_H_
#include "cms/ospext/ospext.h"
#include "../common/commonmsg.h"
#include "cms/container/xmap.h"
#include "servertask.h"

class CServerInst : public CInstExt
{
public:
    //实例状态
    enum{Idle=0, Service};

    CServerInst();
    virtual ~CServerInst();

public:
    virtual LPCSTR GetStrState();
private:
    //消息入口
    virtual void DaemonEntry(CMessage* const pcMsg);
    virtual void NormalEntry(CMessage* const pcMsg){};

//     //Idle状态下实例处理函数
//     void OnIdle(CMessage* const pcMsg);
//     //Service状态下实例处理函数
//     void OnService(CMessage* const pcMsg);
protected:
private:
};


class CServerData
{
public:
    CServerData()
    {
        
    }
    ~CServerData()
    {
        m_mapSessIdOnline.Empty();
        m_mapUserNameOnline.Empty();
    }
    bool AddUser(PCUserData pcUserData)
    {
        if ( NULL == pcUserData )
        {
            return false;
        }

        if ( m_mapSessIdOnline.Find(pcUserData->GetSessionID()) )
        {
            return false;
        }

        m_mapUserNameOnline.Insert(pcUserData->GetUserName(), pcUserData);
        m_mapSessIdOnline.Insert(pcUserData->GetSessionID(), pcUserData);
        return true;
    }
    PCUserData FindUserByUserName(const string strUserName)
    {
        PCUserData *ppcUserData = m_mapUserNameOnline.Find(strUserName);
        if ( ppcUserData )
        {
            return *ppcUserData;
        }
        return NULL;
    }
    PCUserData FindUserBySessId(const string strSessionId)
    {
        PCUserData *ppcUserData = m_mapSessIdOnline.Find(strSessionId);
        if ( ppcUserData )
        {
            return *ppcUserData;
        }
        return NULL;
    }
    void DelUser(PCUserData pcUserData)
    {
        if ( NULL == pcUserData )
        {
            return ;
        }
        m_mapSessIdOnline.Erase(pcUserData->GetSessionID());
        m_mapUserNameOnline.Erase(pcUserData->GetUserName());
    }

    void PrintUser()
    {
        u32 dwIndex = 1;
        Iterator pPos;
        while (!pPos.End())
        {
            string strUserName;
            CUserData *pcUserData = NULL;

            if (m_mapUserNameOnline.Iterate(pPos, strUserName, pcUserData))
            {
                if ( strUserName.size() != 0 )
                {
                    OspPrintf(TRUE, FALSE, "[%d] User: %s, Password: %s, SessId: %s\n", dwIndex, pcUserData->GetUserName().c_str(), pcUserData->GetPassWord().c_str(), pcUserData->GetSessionID().c_str());
                    dwIndex++;
                }
            }
        }
    }
    CSubject *GetSubject()
    {
        return &m_tSubject;
    }
protected:
    CStrMap<PCUserData> m_mapUserNameOnline;
    CStrMap<PCUserData> m_mapSessIdOnline;
    CSubject            m_tSubject;
private:
};

typedef COspApp<CServerInst, 64, CServerData> CServerApp;
extern CServerApp g_cServerApp;

#endif

