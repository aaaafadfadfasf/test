/*****************************************************************************
   ģ����      : cui_fcgi 
   �ļ���      : transutility.h
   ����ļ�    : 
   �ļ�ʵ�ֹ���: cui_fcgi����
   ����        : ��  ��
   �汾        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
   2011/09/23  1.0         ��  ��        ����
******************************************************************************/
#ifndef _TRANS_UTILITY_H_
#define _TRANS_UTILITY_H_

#include <string>
#include "cms/cmsassert.h"
#include "cbb/osp/osp.h"

///////////////////////////////////////////////////////////////////
//	Linux ����ϵͳͷ�ļ�
///////////////////////////////////////////////////////////////////
#ifdef _LINUX_

#ifdef PWLIB_SUPPORT
#include <ptlib.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>
#include <malloc.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/types.h>
#include "sys/wait.h"
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/times.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <termios.h>
#include <signal.h>	
#include "dirent.h"	
#include "syslog.h"
//#include <stropts.h>

#ifndef _EQUATOR_
#include <mqueue.h>
#include <sys/epoll.h>
#include <sys/resource.h>
#endif

#endif //_LINUX_

///////////////////////////////////////////////////////////////////
//	Win32 ����ϵͳͷ�ļ�
///////////////////////////////////////////////////////////////////
#ifdef _MSC_VER
#include <malloc.h>

#ifndef _WINDOWS_
	#include <afx.h>
	#include <afxwin.h>
	#include <winsock2.h>
#else
    #include <winsock2.h>
	#include <windows.h>
#endif

#include <assert.h>
#include <time.h>
#pragma comment(lib,"Ws2_32.lib")
#endif	// _MSC_VER

#ifdef _CUISERVER_
#include "cuidata.h"
#endif


///////////////////////////////////////////////////////////////////
//	����ϵͳ����������Ͷ���
///////////////////////////////////////////////////////////////////
#ifdef _LINUX_

	typedef int 				 BOOL;
	#define TRUE				 1
	#define FALSE				 0

	typedef void *(*LINUXFUNC)(void *);

	#define SEMHANDLE			 pthread_cond_t*

	#define TASKHANDLE			 pthread_t

#endif	// _LINUX_
#ifndef _MSC_VER  //linux
#define SOCKHANDLE			    int
#ifndef SOCKADDR
#define SOCKADDR		        sockaddr
#endif
#ifndef SOCKADDR_IN
#define SOCKADDR_IN 		    sockaddr_in
#endif

#define SOCKET_ERROR		    (-1)
#define INVALID_SOCKET		    (-1)

#define SOCKLEN                 socklen_t
#endif

#ifdef _MSC_VER  //win
#define SOCKHANDLE			    SOCKET
#define SOCKLEN                 int
#endif	// _MSC_VER

//����ϵͳticks����ȡ�뼶ʱ�䣬��������ʱ���Ĳ�����Ϊ�޸���ϵͳʱ��������쳣
u64 GetSecByTicks();

int GetSockError(void);

bool IsErrorcodeWouldBlock(const int nErrNO);

int SetSendTimeOut(SOCKHANDLE skt, int waitms);
int SetRecvTimeOut(SOCKHANDLE skt, int waitms);

inline bool SockCloseAndClear(SOCKHANDLE &hSock)
{
    bool bRet = (TRUE == SockClose(hSock));
    if ( bRet )
    {
        hSock = INVALID_SOCKET;
    }
    return bRet;
}

#define MAX_RECV_MSG_LEN    (4 * 1024 * 1024)
#define MAX_EPOLL_SIZE      (5000)
#define MAX_SELECT_SIZE     1024 




struct TRecvNode 
{
public:
    enum emSockStatus
    {
        CLOSE = 0,
        RECVING,       //cui.fcgi��cuiserver��Ϊ�����ӣ�RECVING��ʾ���ӿ��ô���IDLE�������ڽ���
        WAITAPP,
        SENDING,
    };

    SOCKHANDLE tSock;
    
    int nRecvedLen;             //�Ѿ����յ��ܳ���, ����CMsgHead
    int nMsgTotalLen;           //������Ϣ�ܳ���, ����CMsgHead
                                //��APPʹ�ô�node��Ϊ������Ϣʱ����ʾҵ��Ӧ����Ϣ���ܳ���
    
    //int nDeadLine;            //�������(ϵͳʱ�䳬����ֵ���������)
    int m_nMsgType;             //����ҵ���ϵ���Ϣ(��Ȼ����д����Ϣ�ţ�������ô��Ϊ�˽����ҵ���������ϵ)
    u32 dwSeqNum;               //linux��sock���ظ�����ֵ��֤sock��u64key���ظ�
    TMsTime m_tTime;
    emSockStatus emCurStatus;   //��ǰRecvNode״̬��

    char szRecvBuf[MAX_RECV_MSG_LEN];
public:
    TRecvNode()
    {
        tSock = INVALID_SOCKET;
        //m_tTime = time(NULL);
        Clear();
    }
    TRecvNode(const TRecvNode& cOther)
    {        
        Copy(cOther);
    }
    ~TRecvNode()
    {
        #ifdef _CUISERVER_
        string strEvent = GetXmlKeyText(string(szRecvBuf), EVENT_BEGIN, EVENT_END).c_str();
        if(!strEvent.empty())
        {
            CElapsedTime cElapsedTime(strEvent, m_tTime);

            g_cElapsedTimeList.InsertElapsedTime(cElapsedTime);
        }
        
        #endif

        Clear();
    }
    void Clear()
    {
//         if ( INVALID_SOCKET != tSock )
//         {
//             SockCloseAndClear(tSock);
//             tSock = INVALID_SOCKET;
//         }
        //���ﲻ�ܹر��׽��֣���Ϊpnode���ڸ�����APP�߳�ֻ��ɾ���������ر�sock��ᵼ��recv����
        tSock = INVALID_SOCKET;
        nRecvedLen = 0;
        nMsgTotalLen = 0;
        m_nMsgType = 0;
        dwSeqNum = 0;
        emCurStatus = CLOSE;

        szRecvBuf[0] = '\0';
        //memset(szRecvBuf, 0, sizeof(szRecvBuf));  //��ʡ����
    }
    bool Copy(const TRecvNode& cOther)
    {
        tSock = cOther.tSock;
        nRecvedLen = cOther.nRecvedLen;
        nMsgTotalLen = cOther.nMsgTotalLen;
        m_nMsgType = cOther.m_nMsgType;
        dwSeqNum = cOther.dwSeqNum;
        emCurStatus = cOther.emCurStatus;
        memcpy(szRecvBuf, cOther.szRecvBuf, cOther.nMsgTotalLen);
        szRecvBuf[cOther.nMsgTotalLen] = '\0';
        return true;
    }
    TRecvNode& operator=(const TRecvNode& cOther)
    {
        if (this != &cOther)
        {
            Clear();
            Copy(cOther);
        }

        return *this;
    }
    bool SetRspMsg(int nMsgType, LPCSTR pchMsgBodyBuf, unsigned int dwMsgBodyLen);

    void GetCurTime()
    {
        m_tTime = OspGetMsTime();
    }
};

class CAutoDelete
{
public:
    CAutoDelete(TRecvNode *ptNode)
    {
        CMS_ASSERT(NULL != ptNode);
        m_ptNode=ptNode;
    };
    ~CAutoDelete()
    {
        if ( NULL != m_ptNode )
        {
            m_ptNode->Clear();
            delete m_ptNode;
            m_ptNode = NULL;
        }
    }
private:
    CAutoDelete(){m_ptNode=NULL;};
    TRecvNode *m_ptNode;
};

/*====================================================================
��������ConnectToCuiServer
���ܣ�����
�㷨ʵ�֣�����ѡ�
����ȫ�ֱ�����
�������˵����std::string &strCuiIp
            unsigned short wCuiPort
            int nTimeOut        ��λ����

����ֵ˵��������SOCKHANDLE
====================================================================*/
SOCKHANDLE ConnectToCuiServer(const std::string &strCuiIp, unsigned short wCuiPort, int nTimeOut = 5*1000);

/*====================================================================
��������SockSend
���ܣ�  ��CU������Ϣ
�㷨ʵ�֣��û�ֻ��Ҫ���ķ��͵���Ϣ���ݣ�MsgHead�ɱ�������װ
����ȫ�ֱ�����
�������˵����

����ֵ˵��������0��ʾ�ɹ�, ����ʧ��
2012.5.26 modify by liangli
��ΪAPP�߳��ڵ��ñ�����ʱ��Ҫʹ��sock���Ӷ�Υ�����߳�һ����ԭ��sockӦ��ֻ��recv�߳���ʹ��
���Ա�������2012.5.26����cuiserver���ٵ��ã�������������
====================================================================*/
int SockSend(SOCKHANDLE tSock, s32 nMsgType, LPCSTR pchMsgBodyBuf, unsigned int dwMsgBodyLen);

/*====================================================================
��������DoSend
���ܣ���CUI������Ϣ
�㷨ʵ�֣������ķ��͵����ݣ�ֻ����һ���԰�����Buf���ͳ�ȥ
����ȫ�ֱ�����
�������˵����

����ֵ˵��������0��ʾ�ɹ�, ����ʧ��
====================================================================*/
int DoSend(SOCKHANDLE tSock, LPCSTR pchBuf, unsigned int dwLen);

/*====================================================================
��������SockRecv
���ܣ���CUI������Ϣ
�㷨ʵ�֣�����ѡ�
����ȫ�ֱ�����
�������˵����

����ֵ˵��������0��ʾ�ɹ�, ����ʧ��
====================================================================*/
int SockRecv(TRecvNode *ptRecvNode);

/*====================================================================
��������DoRecv
���ܣ���CUI������Ϣ
�㷨ʵ�֣�����ѡ�
����ȫ�ֱ�����
�������˵����

����ֵ˵��������0��ʾ��Ϣ�������, 0��ʾ��Ҫ��������, -1��ʾʧ��
====================================================================*/
int DoRecv(TRecvNode *ptRecvNode);

/*====================================================================
��������CreateTcpListenSock
���ܣ���CUI������Ϣ
�㷨ʵ�֣�����ѡ�
����ȫ�ֱ�����
�������˵����

����ֵ˵�����ɹ������Ѿ�bind��socket��ʧ�ܷ���INVALID_SOCKET
====================================================================*/
SOCKHANDLE CreateTcpListenSock(unsigned int dwLocalIPAddr, unsigned short wListenPort);

int SetSockNoBlock(SOCKHANDLE tSock, int bNoBlock);

#endif

