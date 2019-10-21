#ifndef RS_MSG_QUEUE_H_
#define RS_MSG_QUEUE_H_

#ifdef WIN32
#pragma warning (disable: 4514 4786)
#endif

#include <list>
#include "kdvtype.h"
#include "rstypes.h"

typedef struct TMsgItem
{
public:    
    static TMsgItem* CreateMsgItem(const u32 dwSrcIID, const u16 wEventId, const CRSComm_OspMsg* pcOspMsg)
	{
		if (NULL == pcOspMsg)
		{
			return NULL;
		}
		
		TMsgItem* pNewItem = NULL;
		pNewItem = (TMsgItem*)::OspAllocMem(sizeof(TMsgItem));
		if ( NULL == pNewItem )
		{
			return NULL;
		}
		
		time_t nCurTime;
		time(&nCurTime);
		
		pNewItem->m_dwSrcIID = dwSrcIID;
		pNewItem->m_wEventId = wEventId;
		pNewItem->m_dwRecvMsgTime = nCurTime;
		pNewItem->m_cMsg.SetMsg(pcOspMsg->GetMsg(), pcOspMsg->GetMsgLen());
		
		return pNewItem;
	}

    static void DestoryMsgItem(TMsgItem* & pMsgItem)
	{
		if (pMsgItem)
		{
			OspFreeMem(pMsgItem);
		}
		
		pMsgItem = NULL;
	}
	
    BOOL32 IsMsgTimeout(u32 dwTimeOutSecond)const
	{
		time_t nCurTime;
		time(&nCurTime);
		
		if ( (u32)nCurTime > m_dwRecvMsgTime )
		{
			if ( nCurTime - m_dwRecvMsgTime > dwTimeOutSecond )
			{
				return TRUE;
			}
		}
		
		return FALSE;
	}

    CRSComm_OspMsg* GetMsg()
	{
		return &m_cMsg;
	}

    const u16 GetEventId() const
	{
		return m_wEventId;
	}

	const u32 GetSrcIID() const
	{
		return m_dwSrcIID;
	}

protected:

    CRSComm_OspMsg     m_cMsg;
    u32                m_dwRecvMsgTime;
    u16                m_wEventId;
	u32                m_dwSrcIID;

protected:
    TMsgItem()
	{
		Reset();
	}

    ~TMsgItem();

    void Reset()
	{
		memset(this,0,sizeof(this));
	}

}TMsgItem;

class CMsgQueue
{
public:

	CMsgQueue()
	{
		m_dwQueueMaxSize = 0;
		m_dwCurQueueSize = 0;
		m_listWaitDealMsg.clear();
	}

	~CMsgQueue()
	{

	}

    void AddMsgItem(const u32 dwSrcIID, const u16 wEventId, const CRSComm_OspMsg* pcOspMsg)
	{
		if (NULL == pcOspMsg)
		{
			return;
		}
		
		TMsgItem* ptMsgItem = TMsgItem::CreateMsgItem(dwSrcIID, wEventId, pcOspMsg);
		if (NULL == ptMsgItem)
		{
			return;
		}
		
		m_listWaitDealMsg.insert(m_listWaitDealMsg.end(), ptMsgItem);

		m_dwCurQueueSize++;
		if (m_dwCurQueueSize > m_dwQueueMaxSize)
		{
			m_dwQueueMaxSize = m_dwCurQueueSize;
		}
	}
   
    TMsgItem* GetMsgItem()
	{
		if (m_listWaitDealMsg.empty())
		{
			return NULL;
		}
		else
		{
			return m_listWaitDealMsg.front();
		}
	}

    void RemoveFirstMsgItem()
	{
		if (m_listWaitDealMsg.empty())
		{
			m_dwCurQueueSize = 0;
			return;
		}
		else
		{
			TMsgItem* ptMsgItem = m_listWaitDealMsg.front();
			TMsgItem::DestoryMsgItem(ptMsgItem);
			m_listWaitDealMsg.pop_front();

			m_dwCurQueueSize--;
		}
	}

    BOOL32 IsMsgQueueEmpty() const
	{
		return m_listWaitDealMsg.empty();
	}

    u32 GetMsgItemCount()const
	{
		return m_listWaitDealMsg.size();
	}

	u32 GetMsgItemSize() const 
	{
		return sizeof(TMsgItem);
	}

	u32 GetMsgItemMaxCount() const
	{
		return m_dwQueueMaxSize;
	}
   
private:
    std::list<TMsgItem*> m_listWaitDealMsg;
	u32   m_dwQueueMaxSize;
	u32   m_dwCurQueueSize;
};

#endif 