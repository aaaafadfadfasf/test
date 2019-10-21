/*****************************************************************************
   ģ����      : cui_fcgi 
   �ļ���      : observermode.cpp
   ����ļ�    : 
   �ļ�ʵ�ֹ���: �۲���ģʽʵ��
   ����        : liangli
   �汾        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
   2011/10/27   1.0         liangli        ����
******************************************************************************/
#include "observermode.h"

CSubject::CSubject()
{
    
}

CSubject::~CSubject()
{
    //����ڲ�۲��߲�Ϊ�գ����ͷ����й۲���
    //����CSubject����������CObserver����
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
            CObserver *pcObs = (CObserver *)(*pItem);  //һ��Ҫ��ת����Ȼ����eraseɾ��pItem������pItemָ����Ч
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
    //�յ���CSubject���Ѿ�ע������Ϣ���������ٴ˶�����
    if ( m_bSubscribed )
    {
        m_bSubscribed = false;
        m_pSubject = NULL;
        delete this;
    }
}

