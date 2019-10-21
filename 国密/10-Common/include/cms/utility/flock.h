/*****************************************************************************
   ģ����      : KDM
   �ļ���      : flock.h
   ����ļ�    : 
   �ļ�ʵ�ֹ���: �ļ�����װ
   ����        : ��־��
   �汾        : V1.0  Copyright(C) 2010 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���        �޸�����
   2010/03/09  1.0         ��־��        ����
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
Ŀǰ�����ڽ����ڲ����̰߳�ȫ�ģ���ֹ�����ڶ���߳�ͬʱ����ͬһ��������
�������صĴ����룺1-129�ǲ���ϵͳ�Զ���Ĵ����룬1000���ϵ�Ϊ���Զ�������롣
1000:�ö����Ѿ���ȡ��һ����
1001:�ö���δӵ����
 */
class CFileLock
{
    enum
    {
        MAX_FILENAME_LENGTH=1024
    };
public:
    // �ļ������ȳ���MAX_FILENAME_LENGTHʱ��Ч���ļ��������Զ�����
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
    // ������ʽ��ȡд�����ɹ�����0��ʧ�ܷ��ش����룬��������������
    s32 WrLock(void)
    {
        if (F_UNLCK != m_nLockType)
        {
            return 1000;
        }

        return LockFile(F_SETLKW, F_WRLCK);        
    }
    // ��ȡд�����ɹ�����0��ʧ�ܷ��ش����룬��������������
    s32 TryWrLock(void)
    {
        if (F_UNLCK != m_nLockType)
        {
            return 1000;
        }
        
        return LockFile(F_SETLK, F_WRLCK); 
    }
    // ������ʽ��ȡ�������ɹ�����0��ʧ�ܷ��ش����룬��������������
    s32 RdLock(void)
    {
        if (F_UNLCK != m_nLockType)
        {
            return 1000;
        }
        
        return LockFile(F_SETLKW, F_RDLCK); 
    }
    // ��ȡ�������ɹ�����0��ʧ�ܷ��ش����룬��������������
    s32 TryRdLock(void)
    {
        if (F_UNLCK != m_nLockType)
        {
            return 1000;
        }
        
        return LockFile(F_SETLK, F_RDLCK);
    }
    // �������ɹ�����0��ʧ�ܷ��ش����룬��������������
    s32 Unlock(void)
    {
        if (F_UNLCK != m_nLockType)
        {
            return UnlockFile();
        }

        return 1001;
    }

private:
    // cmd: F_SETLK, F_SETLKW; locktype: F_RDLCK, F_WRLCK; �ɹ�����0��ʧ�ܷ���errno
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
    // �ɹ�����0��ʧ�ܷ���errno
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
    s8 m_szFileName[MAX_FILENAME_LENGTH+1]; // �ļ�����
    s32 m_nfd; // �ļ����������Ƿ�ֵΪ-1
    s32 m_nLockType; // ������: F_RDLCK��F_WRLCK���Ƿ�ֵF_UNLCK
};

#else // WIN32

class CFileLock
{
    enum
    {
        MAX_FILENAME_LENGTH=1024
    };
public:
    // �ļ������ȳ���MAX_FILENAME_LENGTHʱ��Ч���ļ��������Զ�����
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
    // ������ʽ��ȡд�����ɹ�����0��ʧ�ܷ��ش����룬��������������
    s32 WrLock(void)
    {
        return 0;        
    }
    // ��ȡд�����ɹ�����0��ʧ�ܷ��ش����룬��������������
    s32 TryWrLock(void)
    {
        return 0;
    }
    // ������ʽ��ȡ�������ɹ�����0��ʧ�ܷ��ش����룬��������������
    s32 RdLock(void)
    {
        return 0;
    }
    // ��ȡ�������ɹ�����0��ʧ�ܷ��ش����룬��������������
    s32 TryRdLock(void)
    {
        return 0;
    }
    // �������ɹ�����0��ʧ�ܷ��ش����룬��������������
    s32 Unlock(void)
    {
        return 0;
    }
};

#endif // _LINUX_

#endif // _FLOCK_H

