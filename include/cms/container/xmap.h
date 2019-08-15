/*========================================================================================
模块名    ：map容器
文件名    ：xmap.h
相关文件  ：
实现功能  ：实现map的摸板
作者      ：fanxg
版权      ：<Copyright(C) 2003-2008 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
修改记录：
日期               版本              修改人             走读人              修改记录
2010/06/08         V1.0              fanxg                                  新建文件
=========================================================================================*/

#ifndef _XMAP_INCLUDED_
#define _XMAP_INCLUDED_

#include "packassert.h"

#ifndef assert
#define  assert
#endif

#ifndef LPCSTR
#define LPCSTR  const char *
#endif

//typedef unsigned int size_t;
typedef unsigned char      byte;

#ifndef XITERATOR
#define XITERATOR

typedef struct Pos_Pointer {} *XPos;
#define ITERATOR_BEGIN   ((XPos)-1L)
#define ITERATOR_END     ((XPos)0L)
class Iterator
{
public:
	
	explicit Iterator(XPos Pos = ITERATOR_BEGIN) :m_Pos(Pos){}
	~Iterator(){m_Pos = ITERATOR_END;}
	
	Iterator(void* p) {m_Pos = (XPos)p;}
	Iterator& operator= (void* p) {m_Pos = (XPos)p;return *this;}
	bool operator== (void* p) const { return (m_Pos == (XPos)p);}
	XPos operator& () const {return m_Pos;}	
	bool Begin() const { return (m_Pos == ITERATOR_BEGIN);}
	bool End() const { return (m_Pos == ITERATOR_END);}
	bool Valid() const { return (!Begin() && !End());}
    void SetToBegin() {m_Pos = ITERATOR_BEGIN;}
	
private:
	XPos m_Pos;
};
#endif // #ifndef XITERATOR

// hash functions
inline size_t hash_mysql(LPCSTR key,   size_t len)  
{ 
	if (key == 0)  return 0;
	
	size_t nr=1, nr2=4; 
	while (len--) 
	{ 
		nr^= (((nr & 63)+nr2)*((size_t) (byte) *key++))+ (nr << 8); 
		nr2+=3; 
	} 
	return((size_t) nr);   
}

inline size_t hash_ms(LPCSTR pStr, size_t len)
{
	if (pStr == 0)  return 0;
	size_t nHash = 0;
	while (*pStr)  nHash = (nHash<<5) + nHash + *pStr++;
	
	return nHash;
}

//hash template
//primitive type hash function: int, short, long..., or object but not string

template<class KEY>
struct CHashFunctor
{
	size_t operator()(const KEY& key) const
	{
		//convert object to string buffer forcelly
		//KEY需要注意字节对齐，要么是一字节对齐，要么和编译器设定的对齐方式一致，
        //必须保证KEY对象中没有padding字节，否则对同一个KEY会有不同的hansh值

		//编译期检测KEY是否packed，如果不是会编译错误
		pack_assert<KEY>();
		size_t nLen = sizeof(KEY);
		LPCSTR pKey = reinterpret_cast<LPCSTR>(&key);
		return hash_mysql(pKey, nLen);
	}	
};

//equal template
template<class KEY>
struct CEqualFunctor
{
	int operator()(const KEY& key1, const KEY& key2) const
	{
		return (key1 == key2);
	}	
};

#define  DEFAULT_HASH_RESIZE_CON    (double)0.87
#define  DEFAULT_HASH_GROW_RATIO    (double)1.22 //1/DEFAULT_HASH_RESIZE_CON + 0.07;

enum
{ 
    DEFAULT_HASH_BLOCK_SIZE = 64,
};

//map
template<class KEY, class VALUE, class HASH = CHashFunctor<KEY>, class EQUAL = CEqualFunctor<KEY> >
class CXMap  
{
//protected:

public:
	// entry
	struct CEntry
	{
		CEntry* pNext;
		size_t nHashValue;
		KEY key;
		VALUE value;

		CEntry() : pNext(0), nHashValue(0), key(KEY()), value(VALUE()){}
		~CEntry()
		{
			pNext = 0;
			Clear();
		}
        void Clear()
        {
            //这里只清空数据, 不改变Next指针
            nHashValue=0;
            key = KEY();
            value = VALUE();
        }
	};

public:

    explicit CXMap(size_t nHashCapacity = DEFAULT_HASH_BLOCK_SIZE)
	{
		assert(nHashCapacity > 0);

		m_pHashTable = 0;
		m_nHashTableSize = size_t(nHashCapacity * DEFAULT_HASH_GROW_RATIO);
		m_nSize = 0;
		m_pFreeList = 0;
		m_nFreeSize = 0;
		m_nCapacity = 0;
	}

	virtual ~CXMap()
	{       
		Empty();
	}

public:	
	CXMap(const CXMap& cOther)
	{        
		Copy(cOther);
	}
	
	CXMap& operator=(const CXMap& cOther)
	{
		if (this != &cOther)
		{
			Empty();
			Copy(cOther);
		}
		
		return *this;
	}

public:

	bool Find(const KEY& key, VALUE& rValue) const
	{
		size_t nHash = 0;
		CEntry* pEntry = GetEntryAt(key, nHash);
		if (pEntry == 0) return false;
		
		rValue = pEntry->value;
		return true;
	}

	VALUE* Find(const KEY& key) const
	{
		size_t nHash = 0;
		CEntry* pEntry = GetEntryAt(key, nHash);
		if (pEntry == 0) return 0;
		
		return (&pEntry->value);
	}

	bool Exist(const KEY& key) const
	{
		VALUE Value;
		return Find(key, Value);
	}
	
	void Insert(const KEY& key, const VALUE& newValue)
	{
        //这里一步拆为两步.
        //当VALUE为空类时一步的话会被编译错误地优化掉
		VALUE& oldValue = (*this)[key];
        oldValue = newValue; 
	}
	
	VALUE& operator[](const KEY& key)
	{
		//resize hash table size if needed
		if (m_pHashTable == 0)
		{
			InitHashTable(m_nHashTableSize);
		}
		if ((double)(m_nCapacity)/m_nHashTableSize > DEFAULT_HASH_RESIZE_CON)
		{
			Resize(size_t(m_nCapacity * DEFAULT_HASH_GROW_RATIO));		
		}

		size_t nHash = 0;
		CEntry* pEntry = 0;
		if ((pEntry = GetEntryAt(key, nHash)) == 0)
		{			
			// it doesn't exist, add a new entry
			pEntry = GetFreeEntry();
			assert(pEntry);
			pEntry->nHashValue = nHash;
			pEntry->key = key;
			
			// put into hash table
			pEntry->pNext = m_pHashTable[nHash];
			m_pHashTable[nHash] = pEntry;
		}

		return pEntry->value;
	}

	void Resize(size_t nNewHashSize)
	{
		if (nNewHashSize <= m_nHashTableSize)
			return;

		//CEntry** pHashSave = m_pHashTable;

		// create new hash table
		CEntry** pNewHashTable = new CEntry* [nNewHashSize];
		memset(pNewHashTable, 0, sizeof(CEntry*) * nNewHashSize);

		// copy old table to new table one by one
		Iterator pPos;
		while (!pPos.End())
		{
			Iterator prePos = Iterate(pPos);

			if (!prePos.Begin())
			{
				//free entry to free list
				FreeEntry((CEntry*)&prePos);
			}

			if (!pPos.End())
			{
				CEntry* pEntry = (CEntry*)&pPos;

				// get free entry and copy old entry to it
				CEntry* pNewEntry = GetFreeEntry();
				assert(pNewEntry);
			    pNewEntry->key = pEntry->key;
				pNewEntry->value = pEntry->value;
                pNewEntry->nHashValue = HashCode(pEntry->key, nNewHashSize);

                // free old entry from HashTable
                m_pHashTable[pEntry->nHashValue] = pEntry->pNext;
                //因为遍历下一个节点的时候还需要pEntry->pNext, 这里不能FreeEntry
                //FreeEntry(pEntry);

				// put into new hash table
				pNewEntry->pNext = pNewHashTable[pNewEntry->nHashValue];
			    pNewHashTable[pNewEntry->nHashValue] = pNewEntry;
			}
		}

		//delete old table and set new table
		delete[] m_pHashTable;
		m_pHashTable = pNewHashTable;
		m_nHashTableSize = nNewHashSize;
	}
	
	bool Erase(const KEY& key)
	{		
		if (m_pHashTable == 0)
			return false;
		
		CEntry** ppEntryPrev = 0;
        ppEntryPrev = &m_pHashTable[HashCode(key)];
		
		CEntry* pEntry = 0;
		for (pEntry = *ppEntryPrev; pEntry != 0; pEntry = pEntry->pNext)
		{
			if (EQUAL()(pEntry->key, key))
			{
				// free it to free list
				*ppEntryPrev = pEntry->pNext;
				FreeEntry(pEntry);
				return true;
			}
			ppEntryPrev = &pEntry->pNext;
		}
		
		return false;
	}

	bool Iterate(Iterator& rNextPosition, KEY& rKey, VALUE& rValue) const
	{
        KEY* pKey = 0;
        VALUE* pValue = 0;
        bool bFind = Iterate(rNextPosition,pKey,pValue);
        if (bFind)
        {
            rKey = *pKey;
            rValue = *pValue;
        }
        return bFind;
	}
    bool Iterate(Iterator& rNextPosition, KEY*& rKey, VALUE*& rValue) const
	{
		if (m_pHashTable == 0 || GetSize() == 0)
		{
			rNextPosition = ITERATOR_END;
			return false;
		}
		
		CEntry* pEntryRet = (CEntry*)&rNextPosition;
		if (pEntryRet == (CEntry*) ITERATOR_END)
			return false;
		
		if (pEntryRet == (CEntry*) ITERATOR_BEGIN)
		{
			// find the first entry
			for (size_t nBucket = 0; nBucket < m_nHashTableSize; nBucket++)
			{
				if ((pEntryRet = m_pHashTable[nBucket]) != 0)
					break;
			}

			if (pEntryRet == 0)
			{
				rNextPosition = ITERATOR_END;
			    return false;
			}
		}
		
		// find next entry
		CEntry* pEntryNext = 0;
		if ((pEntryNext = pEntryRet->pNext) == 0)
		{
			// go to next bucket
			for (size_t nBucket = pEntryRet->nHashValue + 1; nBucket < m_nHashTableSize; nBucket++)
			{
				if ((pEntryNext = m_pHashTable[nBucket]) != 0)
					break;
			}
		}
		
		rNextPosition = pEntryNext;
		
		// fill in return data
		rKey = &pEntryRet->key;
		rValue = &pEntryRet->value;

		return true;
	}

	size_t GetSize() const
	{
		return m_nSize;
	}

	size_t GetFreeSize() const
	{
		return m_nFreeSize;
	}

	size_t GetCapacity() const
	{
		return m_nCapacity;
	}

	size_t GetHashTableSize() const
	{
		return m_nHashTableSize;
	}

	bool IsEmpty() const
	{
		return (m_nSize == 0);
	}

	void Empty()
	{		
		if (m_pHashTable != 0)
		{
			// free Entry-nodes in hash table
			for (size_t nHash = 0; nHash < m_nHashTableSize; nHash++)
			{
				RemoveEntryList(m_pHashTable[nHash]);
				m_pHashTable[nHash] = 0;
			}
		}
		
		// free hash table
		delete[] m_pHashTable;
		m_pHashTable = 0;
		
		// free Entry-nodes in free list
		if (m_pFreeList != 0)
		{
			RemoveEntryList(m_pFreeList);
			m_pFreeList = 0;
		}
		
		m_nSize = 0;
		m_nFreeSize = 0;
		m_nCapacity = 0;
		m_nHashTableSize = 0;
	}

#ifdef _DEBUG

	void DumpPerform() const 
	{
		printf("hash size = %d, capacity = %d, node Size = %d, idle node = %d\n", 
			   m_nHashTableSize, m_nCapacity, m_nSize, m_nFreeSize);

		size_t nMaxBucketDepth = 0;
		size_t nTotalBucketDepth = 0;
		double fAvgBucketDepth = 0.0;
		double fBucketUseRatio = 0.0;
		size_t nBucketUseCount = 0;

		for(size_t i=0; i<m_nHashTableSize; i++)
		{
			CEntry* pEntry = m_pHashTable[i];
			if (pEntry != 0)
			{
				nBucketUseCount++;

				size_t nBucketDepth = 0;
				while(pEntry)
				{
					nTotalBucketDepth++;
					nBucketDepth++;
					pEntry = pEntry->pNext;
				}

				if (nBucketDepth > nMaxBucketDepth) nMaxBucketDepth = nBucketDepth;
			}
		}

        fAvgBucketDepth = (double)nTotalBucketDepth/nBucketUseCount;
        fBucketUseRatio = (double)nBucketUseCount/m_nHashTableSize;

		printf("nMaxBucketDepth = %d, AvgBucketDepth = %f, BucketUseRatio = %f, BucketUseCount = %d\n", 
			   nMaxBucketDepth, fAvgBucketDepth, fBucketUseRatio, nBucketUseCount);

	}

	void Print()
	{
		Iterator pPos;
		int i = 0;
		while(!pPos.End())
		{
			Iterate(pPos);
			if (pPos.Valid())
			{
				i++;
				CEntry* pEntry = (CEntry*)&pPos;

				printf("i = %d, hashval = %d, key = %s\n", i, pEntry->nHashValue, pEntry->key);
				printf("val:");
				pEntry->value.Print();
			}
		}

		printf("print one map element end!\n\n");
	}
#endif //#ifdef _DEBUG


protected:

    // 这个接口无法在外部使用, 因为无法访问内部成员, 只能作为Map内部遍历使用, 所以改为私有成员
    const Iterator Iterate(Iterator& rNextPosition) const
    {
        if (m_pHashTable == 0 || 0 == GetSize())
        {
            rNextPosition = ITERATOR_END;
            return rNextPosition;
        }
        
        if (rNextPosition == ITERATOR_END)
        {
            return rNextPosition;
        }
        
        Iterator prePos = rNextPosition;
        
        if (rNextPosition == ITERATOR_BEGIN)
        {
            // find the first entry
            CEntry* pEntryFirst = 0;
            for (size_t nBucket = 0; nBucket < m_nHashTableSize; nBucket++)
            {
                if ((pEntryFirst = m_pHashTable[nBucket]) != 0)
                    break;
            }
            
            rNextPosition = pEntryFirst;
        }
        else
        {
            // find next entry
            CEntry* pEntryPre = (CEntry*)&rNextPosition;
            CEntry* pEntryNext = 0;
            if ((pEntryNext = pEntryPre->pNext) == 0)
            {
                // go to next bucket
                for (size_t nBucket = pEntryPre->nHashValue + 1; nBucket < m_nHashTableSize; nBucket++)
                {
                    if ((pEntryNext = m_pHashTable[nBucket]) != 0)
                        break;
                }
            }
            
            rNextPosition = pEntryNext;
        }
        
        return prePos;
	}

    size_t HashCode(const KEY& key, size_t nHashSize = 0) const
    {
        if ( 0 == nHashSize )
        {
            nHashSize = m_nHashTableSize;
        }
        return HASH()(key) % nHashSize;
    }

	void InitHashTable(size_t nHashSize)
	{
		assert(m_nSize == 0);

		nHashSize = (nHashSize <= 0) ? (size_t(DEFAULT_HASH_BLOCK_SIZE * DEFAULT_HASH_GROW_RATIO)) : nHashSize;
		
		if (m_pHashTable != 0)
		{
			// free hash table
			delete[] m_pHashTable;
			m_pHashTable = 0;
		}
				
		m_pHashTable = new CEntry* [nHashSize];
		memset(m_pHashTable, 0, sizeof(CEntry*) * nHashSize);
		m_nHashTableSize = nHashSize;
	}

	bool CreateFreeList()
	{
		assert(m_pFreeList == 0);
		if (m_pFreeList == 0)
		{
			size_t nFreeNodeCnt = (m_nSize == 0) ? DEFAULT_HASH_BLOCK_SIZE : m_nSize;
			assert(nFreeNodeCnt > 0);
			
			for(size_t i=0; i<nFreeNodeCnt; i++)
			{
				CEntry* pFreeNode = new CEntry;
				if (pFreeNode == 0)
				{
					RemoveEntryList(m_pFreeList);
					m_pFreeList = 0;
					m_nFreeSize = 0;
					return false;
				}
				
				pFreeNode->pNext = m_pFreeList;
				m_pFreeList = pFreeNode;
				m_nFreeSize++;
			}
			
			m_nCapacity += nFreeNodeCnt;
			assert(m_nCapacity == size_t(m_nSize + m_nFreeSize));
		}	
		
		return true;
	}

    CEntry* GetFreeEntry()
	{
		if (m_pFreeList == 0 && !CreateFreeList())
		{
			return 0;
		}
		assert(m_pFreeList != 0);
		
		CEntry* pEntry = m_pFreeList;
		m_pFreeList = m_pFreeList->pNext;
		m_nSize++;
		m_nFreeSize--;

		return pEntry;
	}

	void FreeEntry(CEntry* pEntry)
	{
		if (pEntry == 0)
			return;

        pEntry->Clear();
		pEntry->pNext = m_pFreeList;
		m_pFreeList = pEntry;
		m_nSize--;
		m_nFreeSize++;
	}

	void RemoveEntryList(CEntry* pEntry)
	{
		if (pEntry == 0)
			return;

		CEntry* pCurr = pEntry;
		CEntry* pNext = pEntry->pNext;
		
		while(pCurr != 0)
		{
			pNext = pCurr->pNext;
			delete pCurr;
			pCurr = pNext;
		}
	}

	CEntry* GetEntryAt(const KEY& key, size_t& nHash) const
	{
		if (m_nHashTableSize == 0 || m_pHashTable == 0)
			return 0;

        nHash = HashCode(key);

		CEntry* pEntry;
		for (pEntry = m_pHashTable[nHash]; pEntry != 0; pEntry = pEntry->pNext)
		{
			if (EQUAL()(pEntry->key, key))
				return pEntry;
		}

		return 0;
	}

	bool Copy(const CXMap& cOther)
	{
		m_pHashTable = 0;
		m_nHashTableSize = cOther.m_nHashTableSize;
		m_nSize = 0;
		m_pFreeList = 0;
		m_nFreeSize = 0;
		m_nCapacity = 0;
		
		InitHashTable(m_nHashTableSize);
		if (m_pHashTable == 0) return false;

		Iterator pPos;
		while(!pPos.End())
		{
			KEY key;
			VALUE val;
			if (cOther.Iterate(pPos, key, val))
			{
				Insert(key, val);
			}
		}

		return true;
	}

protected:
	CEntry** m_pHashTable;
	size_t m_nHashTableSize;
	size_t m_nSize;
	CEntry* m_pFreeList;
	size_t m_nFreeSize;
	size_t m_nCapacity;
};


#include <string>
using std::string;

//string hash fun
template<> struct CHashFunctor<string>
{
	size_t operator()(const string& key) const
	{
		return hash_ms(key.c_str(), 0);
	}	
};
typedef CHashFunctor<string> CMsHashStr;

//sting map
template<class VALUE, class HASH = CMsHashStr> 
class CStrMap :public CXMap<string, VALUE, HASH>
{
	typedef CXMap<string, VALUE, HASH> BaseMap;
public:
	explicit CStrMap(size_t nHashCapacity = DEFAULT_HASH_BLOCK_SIZE) :BaseMap(nHashCapacity){};
};

#endif // #ifndef _XMAP_INCLUDED_
