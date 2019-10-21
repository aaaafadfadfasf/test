/*****************************************************************************
   模块名      : cui_fcgi 
   文件名      : observermode.cpp
   相关文件    : 
   文件实现功能: 观察者模式实现
   作者        : liangli
   版本        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人      修改内容
   2011/10/27   1.0         liangli        创建
******************************************************************************/
#include "observermode.h"

CSubject::CSubject()
{
    
}

CSubject::~CSubject()
{
    //如果在册观察者不为空，则释放所有观察者
    //但是CSubject不负责销毁CObserver对象
    if ( !m_tObsvrArray.empty() )
    {
        m_tObsvrArray.clear();
    }
}

void CSubject::RegisterObserver(CObserver *pObsvr)
{
    if ( NULL == pObsvr )
    {
        return;
    }

    CObsvrVector::iterator pItem = m_tObsvrArray.begin();
    while (pItem != m_tObsvrArray.end())
    {
        if ( (*pItem == pObsvr) )
        {
            return;
        }
        ++pItem;
    }

    m_tObsvrArray.push_back(pObsvr);
}

void CSubject::RemoveObserver(CObserver *pObsvr)
{
    if ( NULL == pObsvr )
    {
        return;
    }

    CObsvrVector::iterator pItem = m_tObsvrArray.begin();
    while (pItem != m_tObsvrArray.end())
    {
        if ( (*pItem == pObsvr) )
        {
            CObserver *pcObs = (CObserver *)(*pItem);  //一定要先转换，然后再erase删除pItem，否则pItem指针无效
            pcObs->UnsubscribeNtf();
            m_tObsvrArray.erase(pItem);
            return;
        }
        ++pItem;
    }
}

void CSubject::NotifyObserver(CObsMessage &cMsg)
{
    CObsvrVector::iterator pItem = m_tObsvrArray.begin();
    while (pItem != m_tObsvrArray.end())
    {
        (*pItem)->Update(cMsg);
        ++pItem;
    }
}

//////////////////////////////////////////////////////////////////////////
CObserver::CObserver(CSubject *pSub)
{
    m_pSubject = pSub;
    m_bSubscribed = false;
}

CObserver::~CObserver()
{
    UnsubscribeReq();
}

void CObserver::Subscribe()
{
    m_pSubject->RegisterObserver(this);
    m_bSubscribed = true;
}

void CObserver::UnsubscribeReq()
{
    if ( m_bSubscribed && m_pSubject )
    {
        m_pSubject->RemoveObserver(this);
    }
}

void CObserver::UnsubscribeNtf()
{
    //收到在CSubject中已经注销的消息，可以销毁此对象了
    if ( m_bSubscribed )
    {
        m_bSubscribed = false;
        m_pSubject = NULL;
        delete this;
    }
}

