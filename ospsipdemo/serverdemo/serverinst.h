/*****************************************************************************
   ģ����      : cui_fcgi 
   �ļ���      : serverinst.h
   ����ļ�    : 
   �ļ�ʵ�ֹ���: ʵ��ServerInst��Ķ���
   ����        : liangli
   �汾        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
   2011/10/10   1.0         liangli        ����
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
    //ʵ��״̬
    enum{Idle=0, Service};

    CServerInst();
    virtual ~CServerInst();

public:
    virtual LPCSTR GetStrState();
private:
    //��Ϣ���
    virtual void DaemonEntry(CMessage* const pcMsg);
    virtual void NormalEntry(CMessage* const pcMsg){};

//     //Idle״̬��ʵ��������
//     void OnIdle(CMessage* const pcMsg);
//     //Service״̬��ʵ��������
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

