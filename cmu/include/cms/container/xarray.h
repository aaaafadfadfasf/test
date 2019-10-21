/*========================================================================================
模块名    ：动态数组容器
文件名    ：xarray.h
相关文件  ：
实现功能  ：实现动态数组的摸板
作者      ：fanxg
版权      ：<Copyright(C) 2003-2008 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
修改记录：
日期               版本              修改人             走读人              修改记录
2010/06/08         V1.0              fanxg                                  新建文件
=========================================================================================*/

#ifndef _XARRAY_INCLUDED_
#define _XARRAY_INCLUDED_

#ifndef assert
#define  assert
#endif

//typedef unsigned int size_t;

template <class T>
class CXArray
{
public:
	CXArray(size_t nCapacity = 1) :m_pData(0), m_nSize(0), m_nCapacity(nCapacity)
	{
		m_pData = new T[m_nCapacity];
		assert(m_pData);
	}
	~CXArray()
	{
		Empty();
	}

public:	
	CXArray(const CXArray& cOther)
	{
		Copy(cOther);
	}
	
	CXArray& operator=(const CXArray& cOther)
	{
		if (this != &cOther)
		{
			Empty();
			Copy(cOther);
		}
		
		return *this;
	}

public:

	size_t GetSize() const 
	{ 
		return m_nSize; 
	}
    size_t GetCapacity()const
    {
        return m_nCapacity;
    }

    void SetCapacity(size_t nCapacity)
    {
        if (nCapacity <= m_nCapacity)
        {
            return;
        }
        else
        {
            Resize(nCapacity);
        }
    }

	bool IsEmpty() const 
	{ 
		return (m_nSize == 0); 
	}

	void SetSize(size_t nSize)
	{
		assert(nSize > m_nSize);
		if (nSize <= m_nSize)
			return;

		Resize(nSize);
	}

	const T& GetAt(size_t nIndex) const
	{
		assert(nIndex >= 0 && nIndex < m_nSize);
		return m_pData[nIndex];
	}

	bool GetAt(size_t nIndex, T& rElem) const
	{
		if (nIndex > m_nSize-1 || nIndex < 0) 
			return false;

		rElem = m_pData[nIndex];
		return true;

	}

	T& operator[](size_t nIndex)
	{
		assert(nIndex >= 0 && nIndex < m_nSize);
		return m_pData[nIndex];
	}

	const T& operator[](size_t nIndex) const
	{
		assert(nIndex >= 0 && nIndex < m_nSize);
		return m_pData[nIndex];
	}

	bool Add(const T& rNewElem)
	{
		return Insert(rNewElem, m_nSize);
	}
    void Add(const T* pArrayItems,size_t nItems)
    {
        Assign(pArrayItems,nItems,GetSize());
    }
    
    void Assign(const T* pArrayItems,size_t nItems,size_t nOffset = 0)
    {
        if (nItems == 0)
        {
            return;
        }
        SetCapacity(nItems + nOffset);
        for(size_t index = 0;index < nItems; ++index)
        {
            m_pData[index + nOffset] = pArrayItems[index];
        }
        m_nSize = nItems + nOffset;
    }

	bool Remove(size_t nIndex) 
	{
		assert(nIndex >= 0 && nIndex < m_nSize);
		if (nIndex+1 > m_nSize || nIndex < 0) return false;

		if (m_nSize <= 0) return false;

		for(size_t i=nIndex; i+1<m_nSize; i++)
		{
			m_pData[i] = m_pData[i+1];
		}
		m_nSize--;

        return true;		
	}

	size_t Erase(const T& cElem)
	{
		size_t nEraseNum = 0;
		for(size_t i=0; i<m_nSize; i++)
		{
			if (cElem == m_pData[i])
			{
				if (Remove(i))
				{
					nEraseNum++;
					i--;
				}
			}		
		}
		
		return nEraseNum;
	}

	void Empty()
	{
		delete[] m_pData;
		m_pData = 0;
		m_nSize = 0;
		m_nCapacity = 0;
	}

	bool Insert(const T& rNewElem, size_t nIndex)
	{
		assert(nIndex >= 0 && nIndex <= m_nSize);
		if (nIndex > m_nSize || nIndex < 0) return false;

		if (m_nCapacity == m_nSize)
		{
			if (!Resize(m_nCapacity * 2))
				return false;
		}

		for(size_t i=m_nSize; i>nIndex; i--)
		{
			m_pData[i] = m_pData[i-1];			
		}

		m_nSize++;
		m_pData[nIndex] = rNewElem; 
		
		return true;
	}

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

	bool Copy(const CXArray& cOther)
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

#endif //#ifndef _XARRAY_INCLUDED_


//end of file

