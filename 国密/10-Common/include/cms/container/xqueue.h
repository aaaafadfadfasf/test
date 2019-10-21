/*========================================================================================
模块名    ：队列容器
文件名    ：xqueue.h
相关文件  ：
实现功能  ：实现队列的摸板
作者      ：fanxg
版权      ：<Copyright(C) 2003-2008 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
修改记录：
日期               版本              修改人             走读人              修改记录
2010/06/08         V1.0              fanxg                                  新建文件
=========================================================================================*/

#ifndef _XQUEQUE_INCLUDED_
#define _XQUEQUE_INCLUDED_

#ifndef assert
#define  assert
#endif

//typedef unsigned int size_t;

template<class T>
class CXQueue
{
private:		
	struct CLink
	{
		CLink* pNext;
		CLink* pPrev;
		T data;
		
		CLink() :pNext(0), pPrev(0) {}
	};

public:
	CXQueue(size_t nCapacity = 1)
	{
		m_nSize = 0;
		m_nCapacity = 0;
		m_pNodeHead = m_pNodeTail = m_pNodeFree = 0;
		CreateFreeList(nCapacity);
	}
	~CXQueue() 
	{
		Empty();
	}
	
public:	
	CXQueue(const CXQueue& cOther)
	{
		Copy(cOther);
	}

	CXQueue& operator=(const CXQueue& cOther)
	{
		if (this != &cOther)
		{
			Empty();
			Copy(cOther);
		}

		return *this;
	}
	
public:
	
	bool Push(const T& rNewElem) 
	{
		CLink* pNewNode = CreateNode(0, m_pNodeHead);
		if (pNewNode == 0) return false;
		
		pNewNode->data = rNewElem;
		if (m_pNodeHead != 0)
		{
			m_pNodeHead->pPrev = pNewNode;
		}
		else
		{
			m_pNodeTail = pNewNode;
			m_pNodeTail->pNext = 0;
		}
		
		m_pNodeHead = pNewNode;
		m_pNodeHead->pPrev = 0;
		return true;
	}
	
	bool Pop(T& rElem)
	{
		if (m_pNodeTail == 0) return false;
		
		rElem = m_pNodeTail->data;
		
		CLink* pOldNode = m_pNodeTail;		
		m_pNodeTail = (m_pNodeHead == m_pNodeTail) ? 0 : pOldNode->pPrev;
		
		if (m_pNodeTail != 0)
			m_pNodeTail->pNext = 0;
		else
			m_pNodeHead = 0;
		
		FreeNode(pOldNode);
		return true;
	}
	
	size_t GetSize() const
	{
		return m_nSize;
	}

	size_t GetCapacity() const
	{
		return m_nCapacity;
	}
	
	void Empty()
	{
		DestroyNodeList(m_pNodeHead);
		DestroyNodeList(m_pNodeFree);
		
		m_pNodeHead = m_pNodeTail = m_pNodeFree = 0;		
		m_nSize = 0;		
		m_nCapacity = 0;
	}
	
	bool IsEmpty() const 
	{ 
		return (m_nSize == 0); 
	}

private:
	bool CreateFreeList(size_t nSize)
	{
		if (m_pNodeFree == 0)
		{
			if (nSize == 0) nSize = 1;
			
			for(size_t i=0; i<nSize; i++)
			{
				CLink* pFreeNode = new CLink;
				if (pFreeNode == 0)
				{
					DestroyNodeList(m_pNodeFree);
					m_pNodeFree = 0;
					return false;
				}
				
				pFreeNode->pNext = m_pNodeFree;
				m_pNodeFree = pFreeNode;
				m_nCapacity++;
			}
		}
		
		return true;
	}
	
	void DestroyNodeList(CLink* pNode)
	{
		if (pNode == 0)
			return;
		
		CLink* pCurr = pNode;
		CLink* pNext = pNode->pNext;
		
		while(pCurr != 0)
		{
			pNext = pCurr->pNext;
			delete pCurr;
			pCurr = pNext;
			m_nCapacity--;
		}
	}
	
	CLink* CreateNode(CLink* pPrev, CLink* pNext)
	{
		if (m_pNodeFree == 0 && !CreateFreeList(m_nCapacity))
		{
			return 0;
		}
		
		CLink* pNode = m_pNodeFree;
		m_pNodeFree = m_pNodeFree->pNext;
		pNode->pPrev = pPrev;
		pNode->pNext = pNext;
		m_nSize++;
		
		return pNode;
	}
	
	void FreeNode(CLink* pNode)
	{
		if (pNode != 0)
		{
			pNode->data = T();
			pNode->pNext = m_pNodeFree;
			m_pNodeFree = pNode;
			m_nSize--;
		}
	}

	void Copy(const CXQueue& cOther)
	{
		size_t nCapacity = cOther.m_nCapacity;
		if (nCapacity <= 0) return;
		
		m_nSize = 0;
		m_nCapacity = 0;
		m_pNodeHead = m_pNodeTail = m_pNodeFree = 0;
		CreateFreeList(nCapacity);
		
		CLink* pTail = cOther.m_pNodeTail;
		if (pTail == 0) return;
		
		CLink* pCurr = pTail;
		
		while(pCurr != 0)
		{
			Push(pCurr->data);
			pCurr = pCurr->pPrev;
		}
	}
	
private:
	CLink* m_pNodeHead;
	CLink* m_pNodeTail;
	size_t m_nSize;
	CLink* m_pNodeFree;
	size_t m_nCapacity;
};

#endif  //#ifndef _XQUEQUE_INCLUDED_