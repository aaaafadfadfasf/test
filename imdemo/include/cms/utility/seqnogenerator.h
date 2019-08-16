#ifndef SEQ_NO_GENERATOR
#define SEQ_NO_GENERATOR

/*****************************************************************************
   模块名      : utility
   文件名      : seqnogenerator.h 
   相关文件    : 
   文件实现功能: 基于CTaskNO的实现进行提取和优化
   作者        : huangzhichun
   版本        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人				修改内容
   2013/05/25   1.0        huangzhichun			创建
******************************************************************************/
#include "kdvtype.h"
#include "cms/container/xset.h"

class CU32SeqNoGenerator
{
#define MAX_U32NO                  ((u32)0x1FFFFFFF)
#define INVALID_SEQ_NO             (0)

public:
    CU32SeqNoGenerator()
    {
        m_dwSeedNO = 1;
    }

    u32 Alloc()
    {
        //CAutoLock cLock(m_cLock);

        u32 dwRetNO = INVALID_SEQ_NO;
        for (u32 dwIdx = 0; dwIdx<=MAX_U32NO; dwIdx++)
        {
            if ( !m_cUsedSeqNOSet.Exist(m_dwSeedNO) )
            {
                dwRetNO = m_dwSeedNO;
                m_cUsedSeqNOSet.Insert(m_dwSeedNO);

                NextSeqNO();
                return dwRetNO;
            }

            NextSeqNO();
        }

        return INVALID_SEQ_NO;
    }

    void Free(const u32 dwTaskNO)
    {
        //CAutoLock cLock(m_cLock);

        if ( dwTaskNO <= MAX_U32NO )
        {
            m_cUsedSeqNOSet.Erase(dwTaskNO);
        }
    }

    //偷看下一个可用的SeqNO(不分配)
    u32 Peek()
    {
        //CAutoLock cLock(m_cLock);

        u32 m_dwSeedNOSaved = m_dwSeedNO;

        u32 dwRetNO = INVALID_SEQ_NO;
        for (u32 dwIdx = 0; dwIdx<=MAX_U32NO; dwIdx++)
        {
            if ( !m_cUsedSeqNOSet.Exist(m_dwSeedNO) )
            {
                dwRetNO = m_dwSeedNO;
                break;
            }

            NextSeqNO();
        }

        m_dwSeedNO = m_dwSeedNOSaved;

        return dwRetNO;
    }

protected:
    u32 NextSeqNO()
    {
        m_dwSeedNO = ++m_dwSeedNO % MAX_U32NO;
        if ( INVALID_SEQ_NO == m_dwSeedNO )
        {
            ++m_dwSeedNO;
        }
        return m_dwSeedNO;
    }

protected:
    u32 m_dwSeedNO;
    CXSet<u32> m_cUsedSeqNOSet;

    //CXSemLock m_cLock;
};

#endif  //#ifndef SEQ_NO_GENERATOR