/*========================================================================================
ģ����    ��stack����
�ļ���    ��xstack.h
����ļ�  ��
ʵ�ֹ���  ��ʵ��stack������
����      ��fanxg
��Ȩ      ��<Copyright(C) 2003-2008 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
�޸ļ�¼��
����               �汾              �޸���             �߶���              �޸ļ�¼
2010/06/08         V1.0              fanxg                                  �½��ļ�
=========================================================================================*/

#ifndef _XSTACK_INCLUDED_
#define _XSTACK_INCLUDED_

#ifndef assert
#define  assert
#endif

//typedef unsigned int size_t;

template <class T>
class CXStack
{
public:
	CXStack(size_t nCapacity = 1)  :m_pData(0), m_nSize(0), m_nCapacity(nCapacity)
	{
		m_pData = new T[m_nCapacity];
		assert(m_pData);
	}

	~CXStack() 
	{
		Empty();
	}
	
public:	

	CXStack(const CXStack& cOther)
	{
		Copy(cOther);
	}
	
	CXStack& operator=(const CXStack& cOther)
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
		if (m_nCapacity == m_nSize)
		{
			if (!Resize(m_nCapacity * 2))
				return false;
		}
		
		m_pData[m_nSize++] = rNewElem;
		
		return true;
	}

	bool Pop(T& rElem)
	{
		if (m_nSize <= 0) return false;

		rElem = m_pData[--m_nSize];
		return true; 
	}
	
	size_t GetSize() const
	{
		return m_nSize;
	}
	
	void Empty()
	{
		delete[] m_pData;
		m_pData = 0;
		m_nSize = 0;
		m_nCapacity = 0;
	}
	
	bool IsEmpty() const { return (m_nSize == 0); }

private:
	bool Resize(size_t nNewCapacity)
	{
		if (nNewCapacity == 0) nNewCapacity = 1;
		
		if (nNewCapacity <= m_nSize) return false;
		
		T* pNewData = new T[nNewCapacity];
		if (pNewData == 0) return false;
		
		for(size_t i=0; i<m_nSize; i++)
		{
			pNewData[i] = m_pData[i];
		}
		m_nCapacity = nNewCapacity;
		delete[] m_pData;
		m_pData = pNewData;
		
		return true;
	}
	
	bool Copy(const CXStack& cOther)
	{
		m_nCapacity = cOther.m_nCapacity;
		if (m_nCapacity <= 0) return false;
		
		m_pData = new T[m_nCapacity];
		if (m_pData == 0) return false;
		
		m_nSize = cOther.m_nSize;
		for(size_t i=0; i<cOther.m_nSize; i++)
		{
			m_pData[i] = cOther.m_pData[i];
		}

		return true;
	}
	
private:
	T* m_pData;
	size_t m_nSize;
	size_t m_nCapacity;
};

#endif //#ifndef _XSTACK_INCLUDED_