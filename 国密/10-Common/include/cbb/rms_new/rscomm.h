#ifndef _RS_COMM_H_
#define _RS_COMM_H_

#include "osp.h"
#include <time.h>

#ifndef rsmin
#define rsmin(a,b)    (((a) < (b)) ? (a) : (b))
#endif

#ifndef rsmax
#define rsmax(a,b)    (((a) > (b)) ? (a) : (b))
#endif

#define RsSemCreate     OspSemBCreate
#define RsSemTake(hSem) {if(hSem) OspSemTake(hSem);}
#define RsSemGive(hSem) {if(hSem) OspSemGive(hSem);}
#define RsSemDelete(hSem)     {if(hSem) OspSemDelete(hSem);}
#define RsDelay         OspDelay

#ifdef _LINUX_
#define RsSnprintf        snprintf
#define RsVsnprintf       vsnprintf
#define RsMkdir(szPath)    {if (NULL!=szPath) mkdir(szPath, 0x1FF);}
#else
#define RsSnprintf			_snprintf
#define RsVsnprintf        _vsnprintf
#define RsMkdir(szPath)    {if (NULL!=szPath) _mkdir(szPath);}
#endif

class CRsSelfLock
{
public:
	CRsSelfLock(SEMHANDLE hSem) 
	{
		m_hSem = NULL;
		if(hSem != NULL)
		{
			if(OspSemTake(hSem))
			{
				m_hSem=hSem;
			}
		}		
	}

    ~CRsSelfLock() 
	{		
		if(m_hSem != NULL)
		{
			if (!OspSemGive(m_hSem))
			{		
			}
			
			m_hSem=NULL;
		}	
	}

private:
	SEMHANDLE m_hSem;
};


#define RsScopedLock(theLock)    CRsSelfLock TmpLockName##__LINE__(theLock)

#define RsDeletePtr(ptr) \
	if(ptr)  \
	{ \
		delete ptr; \
		ptr = NULL;\
	}


#endif