#ifndef _KDM_FILE_AUTO_LOCK_H_
#define _KDM_FILE_AUTO_LOCK_H_

#include "osp.h"

class CKdmFileSem
{
    // make copy constructor and assignment operator inaccessible
    CKdmFileSem(const CKdmFileSem &refCritSec);
    CKdmFileSem &operator=(const CKdmFileSem &refCritSec);

    SEMHANDLE m_hSem;
#ifdef _DEBUG
public:
    u32   m_currentOwner;
    u32   m_lockCount;    
public:
    CKdmFileSem();
    ~CKdmFileSem();
    void Lock();
    void Unlock();

#else

public:
    
    CKdmFileSem() 
    {
        ::OspSemBCreate(&m_hSem);
    };

    ~CKdmFileSem() 
    {        
        ::OspSemDelete(m_hSem);
    };

    void Lock() 
    {        
        ::OspSemTake(m_hSem);
    };

    void Unlock() 
    {        
        ::OspSemGive(m_hSem);
    };
#endif
};

class CKdmFileAutoLock 
{
    // make copy constructor and assignment operator inaccessible
    CKdmFileAutoLock(const CKdmFileAutoLock &refAutoLock);
    CKdmFileAutoLock &operator=(const CKdmFileAutoLock &refAutoLock);

protected:
    CKdmFileSem * m_pLock;

public:
    CKdmFileAutoLock(CKdmFileSem* plock)
    {
        m_pLock = plock;
        m_pLock->Lock();
    };

    ~CKdmFileAutoLock()
    {
        m_pLock->Unlock();
    };
};

#endif
