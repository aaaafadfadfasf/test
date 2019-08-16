/*****************************************************************************
   模块名      : KDM
   文件名      : flock.h
   相关文件    : 
   文件实现功能: 文件锁封装
   作者        : 胡志云
   版本        : V1.0  Copyright(C) 2010 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人        修改内容
   2010/03/09  1.0         胡志云        创建
******************************************************************************/

#ifndef _FLOCK_H
#define _FLOCK_H

#include "kdvtype.h"

#ifdef _LINUX_

#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

/*
目前该类在进程内不是线程安全的，禁止进程内多个线程同时访问同一个锁对象。
函数返回的错误码：1-129是操作系统自定义的错误码，1000以上的为类自定义错误码。
1000:该对象已经获取了一个锁
1001:该对象未拥有锁
 */
class CFileLock
{
    enum
    {
        MAX_FILENAME_LENGTH=1024
    };
public:
    // 文件名长度超过MAX_FILENAME_LENGTH时无效，文件不存在自动创建
    CFileLock(LPCSTR lpcstrFileName)
    {
        if ((NULL == lpcstrFileName) || (strlen(lpcstrFileName) >= MAX_FILENAME_LENGTH))
        {
            m_szFileName[0] = '\0';
        }
        else
        {
            strcpy(m_szFileName, lpcstrFileName);
        }

        m_nfd = -1;
        m_nLockType = F_UNLCK;
    }
    ~CFileLock(void)
    {
        if (F_UNLCK != m_nLockType)
        {
            Unlock();
        }
    }
private:
    CFileLock& operator=(const CFileLock&);
    CFileLock(const CFileLock&);

public:
    // 阻塞方式获取写锁，成功返回0，失败返回错误码，描述见本类描述
    s32 WrLock(void)
    {
        if (F_UNLCK != m_nLockType)
        {
            return 1000;
        }

        return LockFile(F_SETLKW, F_WRLCK);        
    }
    // 获取写锁，成功返回0，失败返回错误码，描述见本类描述
    s32 TryWrLock(void)
    {
        if (F_UNLCK != m_nLockType)
        {
            return 1000;
        }
        
        return LockFile(F_SETLK, F_WRLCK); 
    }
    // 阻塞方式获取读锁，成功返回0，失败返回错误码，描述见本类描述
    s32 RdLock(void)
    {
        if (F_UNLCK != m_nLockType)
        {
            return 1000;
        }
        
        return LockFile(F_SETLKW, F_RDLCK); 
    }
    // 获取读锁，成功返回0，失败返回错误码，描述见本类描述
    s32 TryRdLock(void)
    {
        if (F_UNLCK != m_nLockType)
        {
            return 1000;
        }
        
        return LockFile(F_SETLK, F_RDLCK);
    }
    // 解锁，成功返回0，失败返回错误码，描述见本类描述
    s32 Unlock(void)
    {
        if (F_UNLCK != m_nLockType)
        {
            return UnlockFile();
        }

        return 1001;
    }

private:
    // cmd: F_SETLK, F_SETLKW; locktype: F_RDLCK, F_WRLCK; 成功返回0，失败返回errno
    s32 LockFile(s32 cmd, s32 locktype)
    {
        s32 nRet;

        s32 nOpenFlag = O_CREAT;
        if (F_RDLCK == locktype)
        {
            nOpenFlag |= O_RDONLY;
        }
        else
        {
            nOpenFlag |= O_WRONLY; 
        }
        m_nfd = open(m_szFileName, nOpenFlag, 0666);
        if (-1 == m_nfd)
        {
            return errno;
        }

        s32 nFdFlag = fcntl(m_nfd, F_GETFD, 0);
        if( nFdFlag <0 )
        {
            close(m_nfd);
            m_nfd = -1;
            return errno;
        }
        nFdFlag |= FD_CLOEXEC;
        if( fcntl(m_nfd, F_SETFD, nFdFlag) <0 )
        {
            close(m_nfd);
            m_nfd = -1;
            return errno;
        }

        struct flock   flk;
        flk.l_type   = locktype; /* F_RDLCK, F_WRLCK */
        flk.l_whence = SEEK_SET; /* SEEK_SET, SEEK_CUR, SEEK_END */
        flk.l_start  = 0;        /* starting offset relative to l_whence */
        flk.l_len    = 0;        /* len == 0 means "until end of file" */
        flk.l_pid    = getpid(); /* Process ID of the process holding the
                                    lock, returned with F_GETLK */
        // F_SETLK, F_SETLKW
        if (F_SETLK == cmd)
        {
            nRet = fcntl(m_nfd, F_SETLK, &flk);
        }
        else
        {
            do 
            {
                nRet = fcntl(m_nfd, F_SETLKW, &flk);
                if (-1 == nRet)
                {
                    nRet = errno;
                }

            } while (EINTR == nRet);
        }

        if (0 == nRet)
        {
            m_nLockType = locktype;
        }
        else
        {
            close(m_nfd);
            m_nfd = -1;
        }

        return nRet;
    }
    // 成功返回0，失败返回errno
    s32 UnlockFile(void)
    {
        s32 nRet;

        struct flock   flk;
        flk.l_type   = F_UNLCK;  /* F_RDLCK, F_WRLCK, or F_UNLCK */
        flk.l_whence = SEEK_SET; /* SEEK_SET, SEEK_CUR, SEEK_END */
        flk.l_start  = 0;        /* starting offset relative to l_whence */
        flk.l_len    = 0;        /* len == 0 means "until end of file" */
        flk.l_pid    = getpid(); /* Process ID of the process holding the
                                    lock, returned with F_GETLK */                     
        nRet = fcntl(m_nfd, F_SETLK, &flk);
        if (-1 == nRet)
        {
            nRet = errno;
        }
        m_nLockType = F_UNLCK;
        if (0 == nRet)
        {
            nRet = close(m_nfd);
            if (-1 == nRet)
            {
                nRet = errno;
            }
        }
        else
        {
            close(m_nfd);
        }
        m_nfd = -1;

        return nRet;
    }
private:
    s8 m_szFileName[MAX_FILENAME_LENGTH+1]; // 文件名称
    s32 m_nfd; // 文件描述符，非法值为-1
    s32 m_nLockType; // 锁类型: F_RDLCK，F_WRLCK，非法值F_UNLCK
};

#else // WIN32

class CFileLock
{
    enum
    {
        MAX_FILENAME_LENGTH=1024
    };
public:
    // 文件名长度超过MAX_FILENAME_LENGTH时无效，文件不存在自动创建
    CFileLock(LPCSTR lpcstrFileName)
    {
    }
    ~CFileLock(void)
    {
    }
private:
    CFileLock& operator=(const CFileLock&);
    CFileLock(const CFileLock&);

public:
    // 阻塞方式获取写锁，成功返回0，失败返回错误码，描述见本类描述
    s32 WrLock(void)
    {
        return 0;        
    }
    // 获取写锁，成功返回0，失败返回错误码，描述见本类描述
    s32 TryWrLock(void)
    {
        return 0;
    }
    // 阻塞方式获取读锁，成功返回0，失败返回错误码，描述见本类描述
    s32 RdLock(void)
    {
        return 0;
    }
    // 获取读锁，成功返回0，失败返回错误码，描述见本类描述
    s32 TryRdLock(void)
    {
        return 0;
    }
    // 解锁，成功返回0，失败返回错误码，描述见本类描述
    s32 Unlock(void)
    {
        return 0;
    }
};

#endif // _LINUX_

#endif // _FLOCK_H

