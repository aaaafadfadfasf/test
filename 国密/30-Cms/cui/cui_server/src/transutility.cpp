/*****************************************************************************
   ģ����      : cui_fcgi 
   �ļ���      : cusdkserviceimp.cpp
   ����ļ�    : 
   �ļ�ʵ�ֹ���: cui_fcgi����
   ����        : ��  ��
   �汾        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
   2011/09/23  1.0         ��  ��        ����
******************************************************************************/
#include <time.h>
#include "../../common/cuiproto.h"
#include "transutility.h"
//#include "cms/cmsassert.h"
#ifdef WITH_FASTCGI
#include "system/libfcgi/include/fcgi_stdio.h"
#endif

CXSemLock       g_cTickGet64SemLock;
extern string g_strLogFile;

/*====================================================================
��������TaskDelay
���ܣ�������ʱ
�㷨ʵ�֣�����ѡ�
����ȫ�ֱ�����
�������˵����uMs: ʱ����(ms)

����ֵ˵����
====================================================================*/
// inline void TaskDelay(int dwMs)
// {
// #ifdef WIN32
//     Sleep(dwMs);
// #endif
// 
// #ifdef _LINUX_
//     usleep(dwMs*1000);
// #endif
// }

/*====================================================================
��������OspTickGet
���ܣ�ȡ�õ�ǰ��ϵͳtick��
�㷨ʵ�֣�����ѡ�
����ȫ�ֱ�����
�������˵����

  ����ֵ˵������ǰ��ϵͳtick��
====================================================================*/
API u64 _TickGet64()
{
#ifdef _MSC_VER
	static int dwIntialized = 0;		/* �Ƿ��ʼ�� */
	static u64 dwCurrentTick = 0;		/* ��ǰ��u64��tick*/
	static u32 dwLastTick = 0;		/* �ϴλ�ȡ��ϵͳtick*/			
	u32 dwNowTick;			/* ��ǰϵͳtick*/

	dwNowTick = GetTickCount();	
	
	if (dwIntialized == 0)
	{
		dwCurrentTick = (u32)dwNowTick;
		dwLastTick = dwNowTick;
		dwIntialized = 1;
		return dwCurrentTick;
	}
	
	if (dwNowTick >= dwLastTick)
	{
		dwCurrentTick += (dwNowTick - dwLastTick);
	}
	else
	{
		dwCurrentTick += (dwNowTick + 0xffffffff - dwLastTick);
	}
	
	dwLastTick = dwNowTick;
	return dwCurrentTick;
#endif


#ifdef _VXWORKS_
	return 0;
#endif

#ifdef _LINUX_
	static int initialized = 0; 	/* �Ƿ��ʼ�� */
	static u64 current_tick = 0;		/* ��ǰ��u64��tick*/
	static clock_t last_tick = 0;		/* �ϴλ�ȡ��ϵͳtick*/ 		
	clock_t now_tick;			/* ��ǰϵͳtick*/

	now_tick = times(NULL); 
	if (now_tick == -1)
	{
		now_tick = 0xffffffff - errno;
	}

	if (initialized == 0)
	{
		current_tick = (u32)now_tick;
		last_tick = now_tick;
		initialized = 1;
		return current_tick;
	}

	if (now_tick >= last_tick)
	{
		current_tick += (now_tick - last_tick);
	}
	else
	{
		current_tick += (now_tick + 0xffffffff - last_tick);
	}

	last_tick = now_tick;
	return current_tick;
#endif
}

API u64 TickGet64()
{
	u64 dwRet = 0;
    g_cTickGet64SemLock.Lock();

	dwRet = _TickGet64();

    g_cTickGet64SemLock.UnLock();
	return dwRet;
}

/*====================================================================
  ��������OspClkRateGet
  ���ܣ��õ�tick����
  �㷨ʵ�֣�����ѡ�
  ����ȫ�ֱ�����
  �������˵����

  ����ֵ˵����tick����
  ====================================================================*/
API u32 ClkRateGet()
{
#ifdef _MSC_VER
    return 1000;
#endif

#ifdef _VXWORKS_
    return sysClkRateGet();
#endif

#ifdef _LINUX_
    long ret = sysconf(_SC_CLK_TCK);
    //assert(ret > 0);

    return (u32)ret;
#endif
}

// const u32 GenUdpSequenceNum()
// {
//     u32 dwRet = g_dwSeqGenerator;
//     if ( g_dwSeqGenerator >= MAX_MSG_SEQ_NO )
//     {
//         g_dwSeqGenerator = 1;
//     }
//     else
//     {
//         ++g_dwSeqGenerator;
//     }
//     return dwRet;
// }

//����ϵͳticks����ȡ�뼶ʱ�䣬��������ʱ���Ĳ�����Ϊ�޸���ϵͳʱ��������쳣
u64 GetSecByTicks()
{
    //��ȡticks���ýӿڲ������������
    u64 lwTicks = TickGet64();

    //ticks/sec
    u32 dwClkRate = ClkRateGet();

    //���������
    return (lwTicks / dwClkRate);
}

int GetSockError(void)
{
#ifdef WIN32
    return WSAGetLastError();
#else
    return errno;
#endif
}

int SetSockNoBlock(SOCKHANDLE tSock, int bNoBlock)
{
    int nRet = 0;
    //�����׽���Ϊ������ģʽ
#ifdef WIN32	
    unsigned long arg = (u32)bNoBlock;
    nRet = ioctlsocket( tSock, FIONBIO, &arg );
#else
    int arg = bNoBlock;
    nRet = ioctl( tSock, FIONBIO, (void*)&arg );
#endif //WIN32
    if ( nRet < 0 )
    {
        int nErr = GetSockError();

        CuiWriteLog(g_strLogFile.c_str(), "Set socket[%d] to no block[%d] fail, Errcode[%d]\n", 
            tSock, bNoBlock, nErr);
    }

    return nRet;
}

bool IsErrorcodeWouldBlock(const int nErrNO)
{
#ifdef WIN32
    if ( nErrNO == WSAEWOULDBLOCK || nErrNO == WSAEINPROGRESS )
#else
	if ( nErrNO == EWOULDBLOCK || nErrNO == EAGAIN || nErrNO == EINPROGRESS || nErrNO == EINTR )
#endif
    {
        return true;
    }
    CuiWriteLog(g_strLogFile.c_str(), "IsErrorcodeWouldBlock, Errcode[%d]\n", 
        nErrNO);
    return false;
}

int SetSockReuse(SOCKHANDLE tSock, int bReuse)
{
    return setsockopt(tSock, SOL_SOCKET,SO_REUSEADDR, (char*)&bReuse, sizeof(int));
}

int SetSendTimeOut(SOCKHANDLE skt, int waitms)
{
	struct timeval tm;
	tm.tv_sec = waitms / 1000;
	tm.tv_usec = (waitms % 1000) * 1000;
	return setsockopt(skt, SOL_SOCKET, SO_SNDTIMEO, (const char *)&tm, sizeof(tm));
}
int SetRecvTimeOut(SOCKHANDLE skt, int waitms)
{
	struct timeval tm;
	tm.tv_sec = waitms / 1000;
	tm.tv_usec = (waitms % 1000) * 1000;
	return setsockopt(skt, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tm, sizeof(tm));
}

SOCKHANDLE ConnectToCuiServer(const std::string &strCuiIp, unsigned short wCuiPort, int nTimeOut)
{    
    /* ׼��SERVER��ַ */
    SOCKADDR_IN tSvrINAddr;
    memset(&tSvrINAddr, 0, sizeof(tSvrINAddr));
    tSvrINAddr.sin_family = AF_INET;
    tSvrINAddr.sin_port = htons(wCuiPort);
    tSvrINAddr.sin_addr.s_addr = inet_addr(strCuiIp.c_str());

    /* ����һ�� socket */
    SOCKHANDLE tSock = INVALID_SOCKET;
    tSock = socket(AF_INET, SOCK_STREAM, 0);
    if(tSock == INVALID_SOCKET)
    {
        CuiWriteLog(g_strLogFile.c_str(), "Osp: OspConnectTcpNode() create socket failed!\n");
        return INVALID_SOCKET;
    }

    //����Ϊ������ģʽ, ��ֹconnect����
    int nRet = SetSockNoBlock(tSock, 1);
    if( nRet == SOCKET_ERROR )
    {
        CuiWriteLog(g_strLogFile.c_str(), "[VsipConnect]: VsipConnect set socket in nonblocking mode error!\n");
        SockCloseAndClear(tSock);
        return INVALID_SOCKET;
    }

    //������������CUI������
    nRet = connect(tSock, (SOCKADDR*)&tSvrINAddr, sizeof(tSvrINAddr));

    if ( nRet == SOCKET_ERROR ) //һ�����������connect�϶�ʧ��
    {
        //������ǳ�ʱ, ��ʾ����ʧ��
        if ( ! IsErrorcodeWouldBlock(GetSockError()) )
        {
            CuiWriteLog(g_strLogFile.c_str(), "connect socket[%d] to Server[%s--%hu] fail, Errcode[%d]\n", 
                tSock, strCuiIp.c_str(), wCuiPort, GetSockError());

            //log
            SockCloseAndClear(tSock);
			return INVALID_SOCKET;
        }
        else
        {
            SOCKADDR_IN tPeerINAddr;
            memset(&tPeerINAddr, 0, sizeof(tPeerINAddr));

#ifndef USE_EPOLL
            fd_set rset, wset, eset;
            FD_ZERO( &rset );
            struct timeval va;
            va.tv_sec = nTimeOut/1000;
            va.tv_usec = (nTimeOut % 1000) * 1000;  //us = ms * 1000
            FD_SET( tSock, &rset );
            wset = rset;
			eset = rset;

			nRet = select( FD_SETSIZE, &rset, &wset, &eset, &va);
            if ( nRet > 0 && FD_ISSET( tSock, &wset ) )
            {
                //��Ҫ�����ж��Ƿ�connect �ɹ�
                int dwAddrLen = sizeof(tPeerINAddr);
                if(0 != getpeername(tSock,(SOCKADDR*)&tPeerINAddr,(SOCKLEN *)&dwAddrLen))
                {
                    CuiWriteLog(g_strLogFile.c_str(), "\nOsp: OspConnectTcpNode()  Error :%s\n", strerror(errno));
                    SockCloseAndClear(tSock);
                    return INVALID_SOCKET;
			    }
            }
            else if ( nRet == 0 )
            {
                //���ӳ�ʱ
                CuiWriteLog(g_strLogFile.c_str(), "socket connect time out!\n");
                SockCloseAndClear(tSock);
                return INVALID_SOCKET;
            }
            else
            {
                //����ʧ��
                //log
                CuiWriteLog(g_strLogFile.c_str(), "socket connect fail!\n");
                SockCloseAndClear(tSock);
                return INVALID_SOCKET;
            }
#else
            int nfds = 0;
            int epfd = epoll_create(2);
            struct epoll_event ev,events[2];
            memset(&ev, 0, sizeof(ev));
            memset(events, 0, sizeof(events));
            ev.events = EPOLLOUT;
            epoll_ctl(epfd, EPOLL_CTL_ADD, tSock, &ev);

            nfds = epoll_wait(epfd, events, 2, nTimeOut);
            if ( nfds <= 0 )
            {
                CuiWriteLog(g_strLogFile.c_str(), "\nOsp: OspConnectTcpNode() Connect TimeOut!\n");
                SockCloseAndClear(tSock);
                SockCloseAndClear(epfd);
				return INVALID_SOCKET;
            }

            //��Ҫ�����ж��Ƿ�connect �ɹ�
            int dwAddrLen = sizeof(tPeerINAddr);
            if(0 != getpeername(tSock,(SOCKADDR*)&tPeerINAddr,(SOCKLEN *)&dwAddrLen))
            {
                CuiWriteLog(g_strLogFile.c_str(), "\nOsp: OspConnectTcpNode()  Error :%s\n", strerror(errno));
                SockCloseAndClear(tSock);
                SockCloseAndClear(epfd);
                return INVALID_SOCKET;
            }

            //���ӳɹ�
            SockCloseAndClear(epfd);
#endif
        }
    }

    int optVal = 1;
    int set_result = 0;
    /* �����׽���ѡ��: ��������(�������κα��⾺�����㷨) */
    optVal = 1;
    set_result = setsockopt(tSock, IPPROTO_TCP, TCP_NODELAY, (char*)&optVal, sizeof(optVal));
    if(set_result == SOCKET_ERROR)
    {
        CuiWriteLog(g_strLogFile.c_str(), "\nOsp: OspConnectTcpNode() set sock option fail\n");
    }
    
    /* �����׽���ѡ��: �����׽ӿڷ��ͻ���Ĵ�С */
    optVal = 1024*1024*20;
    set_result = setsockopt(tSock, SOL_SOCKET, SO_SNDBUF, (char *)&optVal, sizeof(optVal));
    if(set_result == SOCKET_ERROR)
    {
        CuiWriteLog(g_strLogFile.c_str(),  "\nOsp: OspConnectTcpNode() set sock option fail\n");
    }
    
    /* �����׽���ѡ��: ����TCP���Ӽ�⹦�� */
    optVal = 1;
    set_result = setsockopt(tSock, SOL_SOCKET, SO_KEEPALIVE, (char *)&optVal, sizeof(optVal));
    if(set_result == SOCKET_ERROR )
    {
        CuiWriteLog(g_strLogFile.c_str(),  "\nOsp: OspConnectTcpNode() set sock option fail\n");
    }

    return tSock;
}

/*====================================================================
��������SockSend
���ܣ���CUI������Ϣ
�㷨ʵ�֣��û�ֻ��Ҫ���ķ��͵���Ϣ���ݣ�MsgHead�ɱ�������װ
����ȫ�ֱ�����
�������˵����

����ֵ˵��������0��ʾ�ɹ�, ����ʧ��
====================================================================*/
int SockSend(SOCKHANDLE tSock, s32 nMsgType, LPCSTR pchMsgBodyBuf, unsigned int dwMsgBodyLen)
{
    CMsgHead cHead;
    cHead.nMsgType = nMsgType;
    cHead.nMsgLen = (int)dwMsgBodyLen;

    char szHeadBuf[sizeof(CMsgHead)] = {0};
    s32 nRet = cHead.ToBuffer(szHeadBuf, sizeof(CMsgHead));
    CMS_ASSERT(nRet > 0);

    s32 nSendRet = DoSend(tSock, szHeadBuf, (u32)nRet);
    if (nSendRet <= 0)
    {
        CuiWriteLog(g_strLogFile.c_str(), "SockSend Do Send Msg Head fail: socket[%d]\n", 
            tSock);
        return -1;
    }
    s32 nSendSecondRet = DoSend(tSock, pchMsgBodyBuf, dwMsgBodyLen);
    if(nSendSecondRet <=0)
    {
        CuiWriteLog(g_strLogFile.c_str(), "SockSend Do Send Msg Body fail: socket[%d]\n", 
            tSock);
        return -1;
    }
    nSendRet +=nSendSecondRet;
    return nSendRet;
}

/*====================================================================
��������DoSend
���ܣ���CUI������Ϣ
�㷨ʵ�֣������ķ��͵����ݣ�ֻ����һ���԰�����Buf���ͳ�ȥ
����ȫ�ֱ�����
�������˵����

����ֵ˵��������0��ʾ�ɹ�, ����ʧ��
====================================================================*/
int DoSend(SOCKHANDLE tSock, LPCSTR pchBuf, unsigned int dwLen)
{
    int nRet = SOCKET_ERROR;

    if((tSock == INVALID_SOCKET) || (pchBuf == NULL) || 0 == dwLen)
    {
        CuiWriteLog(g_strLogFile.c_str(), "Parameter is not correct:tSock\n");
        return -1;
    }
    //��������Ϊ����ģʽ, �Ա㷢��ʱ����
    nRet = SetSockNoBlock(tSock, 0);
    if( nRet == SOCKET_ERROR )
    {
        CuiWriteLog(g_strLogFile.c_str(), "DoSend set socket[%d] to block fail\n", 
            tSock);
        return nRet;
    }

    unsigned int nTotalSendLen = 0;
    while (nTotalSendLen < dwLen)
    {
        //����ʧ��ԭ��Ϊ�ײ�û��Bufʱ, Ҫ���³��Է���3��, ����Winϵͳ
        for(int nTryNum = 0; nTryNum < 3; nTryNum++)
        {
            nRet = send(tSock, (char*)(pchBuf + nTotalSendLen), (int)(dwLen - nTotalSendLen), 0);
            if(nRet == SOCKET_ERROR)
            {
                int nErrorCode = GetSockError();
                if ( !IsErrorcodeWouldBlock(nErrorCode) )
                { //����ʧ��
                    CuiWriteLog(g_strLogFile.c_str(), "DoSend socket[%d] send fail, Errocde[%d]\n", 
                        tSock, nErrorCode);

                    return -1;
                }
                TaskDelay(50);
            }
            else
            {
                break;
            }
        }

        if(nRet >= 0)
        {
            nTotalSendLen += (u32)nRet;
        }
    }
    return (int)nTotalSendLen;
}


/*====================================================================
��������DoRecv
���ܣ���CUI������Ϣ
�㷨ʵ�֣�����ѡ�
����ȫ�ֱ�����
�������˵����

����ֵ˵��������0��ʾ��Ϣ�������, 0��ʾ��Ҫ��������, -1��ʾʧ��
====================================================================*/
int DoRecv(TRecvNode *ptRecvNode)
{
    CMS_ASSERT(NULL != ptRecvNode);
	if (!ptRecvNode)
	{
		return -1;
	}
    int nRet = 0;

    do 
    {
        //���Ƚ��ձ�ͷ
        if ( (u32)(ptRecvNode->nRecvedLen) < sizeof(CMsgHead) )
        {
            int nExpectLen = sizeof(CMsgHead) - (int)ptRecvNode->nRecvedLen;
            nRet = recv(ptRecvNode->tSock, ptRecvNode->szRecvBuf + ptRecvNode->nRecvedLen, nExpectLen, 0);
            if ( 0 == nRet )
            { //�Զ����ӹر�
                CuiWriteLog(g_strLogFile.c_str(), "DoRecv socket[%d] recv fail, remote close the connect, Errocde[%d]\n", 
                    ptRecvNode->tSock, GetSockError());

                return -1;
            }
            if ( nRet == SOCKET_ERROR )
            {
                int nErrorCode = GetSockError();
                if ( !IsErrorcodeWouldBlock(nErrorCode) )
                { //����ʧ��
                    CuiWriteLog(g_strLogFile.c_str(), "DoRecv MsgHead socket[%d] recv fail, Errocde[%d]\n", 
                        ptRecvNode->tSock, nErrorCode);

                    return -1;
                }
                else
                { //��������
					CuiWriteLog(g_strLogFile.c_str(), "DoRecv need retry nErrorCode=%d\n", nErrorCode);
                    break;      //֮ǰû����һ��ᵼ����Ϣ��ȱ������� nRecvedLen-1
                }
            }

            //���ν��ճɹ�
            ptRecvNode->nRecvedLen += nRet;
            if ( (u32)(ptRecvNode->nRecvedLen) < sizeof(CMsgHead) )
            { //��Ȼû�н��յ���������ͷ
                CuiWriteLog(g_strLogFile.c_str(), "DoRecv MsgHead is broken!\n"); 
                break;
            }

            //��֤���ĺϷ���
            CMsgHead cHead;
            nRet = cHead.FromBuffer(ptRecvNode->szRecvBuf, ptRecvNode->nRecvedLen);
            if ( nRet < 0 )
            {
                CuiWriteLog(g_strLogFile.c_str(), "DoRecv MsgHead Buffer is error\n"); 
                return -1;
            }
            ptRecvNode->nMsgTotalLen = (int)cHead.nMsgLen + sizeof(CMsgHead);
            CMS_ASSERT(ptRecvNode->nMsgTotalLen < MAX_RECV_MSG_LEN);

            //��Ϣֻ�а�ͷû�а��壬�Ѿ��������
            if ( ptRecvNode->nRecvedLen == ptRecvNode->nMsgTotalLen )
            {
                return ptRecvNode->nRecvedLen;
            }
        }
        else
        {
            int nExpectLen = ptRecvNode->nMsgTotalLen - ptRecvNode->nRecvedLen;
            CMS_ASSERT( 0 != nExpectLen );
            nRet = recv(ptRecvNode->tSock, ptRecvNode->szRecvBuf + ptRecvNode->nRecvedLen, nExpectLen, 0);
            if ( 0 == nRet )
            { //�Զ����ӹر�

                CuiWriteLog(g_strLogFile.c_str(), "DoRecv MsgBody socket[%d] recv fail, remote close the connect, Errocde[%d]\n", 
                    ptRecvNode->tSock, GetSockError());

                return -1;
            }
            if ( nRet == SOCKET_ERROR )
            {
                int nErrorCode = GetSockError();
				CuiWriteLog(g_strLogFile.c_str(), "DoRecv MsgBody socket[%d] recv fail, Errocde[%d]\n",ptRecvNode->tSock, nErrorCode);
				return -1;
               /*
			   if ( !IsErrorcodeWouldBlock(nErrorCode) )
                { //����ʧ��
                    CuiWriteLog(g_strLogFile.c_str(), "DoRecv MsgBody socket[%d] recv fail, Errocde[%d]\n", 
                        ptRecvNode->tSock, nErrorCode);

                    return -1;
                }
                else
                {
                    break;
                }
				*/
            }

            //���ν��ճɹ�
            ptRecvNode->nRecvedLen += nRet;
            break;
        }
    } while (0);

    CMS_ASSERT(0 != ptRecvNode->nMsgTotalLen);
    if ( ptRecvNode->nRecvedLen < ptRecvNode->nMsgTotalLen )
    {
        return 0;
    }
    return ptRecvNode->nRecvedLen;
}

/*====================================================================
��������SockRecv
���ܣ���CUI������Ϣ
�㷨ʵ�֣�����ѡ�
����ȫ�ֱ�����
�������˵����

����ֵ˵��������0��ʾ�ɹ�, ����ʧ��
====================================================================*/
int SockRecv(TRecvNode *ptRecvNode)
{
    CMS_ASSERT(ptRecvNode != NULL);
    if(ptRecvNode == NULL)
    {
        CuiWriteLog(g_strLogFile.c_str(), "ptRecvNode is null\n");
        return -1;
    }
    CMS_ASSERT(ptRecvNode->tSock != INVALID_SOCKET);
    if(ptRecvNode->tSock == INVALID_SOCKET)
    {
        CuiWriteLog(g_strLogFile.c_str(), "ptRecvNode's Sock is invalid\n");
        return -1;
    }

    int nRet = 0;
    while ( 1 )
    {
        //��ʼ������Ϣ
        nRet = DoRecv(ptRecvNode);
        if ( nRet > 0 )
        {
            break;
        }
        else if ( nRet == 0 )
        {
            OspTaskDelay(50);   //���˽��յĿ죬�Զ˷��͵�������һ��
            continue;
        }
        else
        {
            //��DoRecv���Ѿ��ر���socket
            CuiWriteLog(g_strLogFile.c_str(), "SockRecv recv sock[%d] fail\n", ptRecvNode->tSock);
            return -1;
        }
    }

    return ptRecvNode->nRecvedLen;
}


/*====================================================================
��������CreateTcpListenSock
���ܣ���CUI������Ϣ
�㷨ʵ�֣�����ѡ�
����ȫ�ֱ�����
�������˵����

����ֵ˵�����ɹ������Ѿ�bind��socket��ʧ�ܷ���INVALID_SOCKET
====================================================================*/
SOCKHANDLE CreateTcpListenSock(unsigned int dwLocalIPAddr, unsigned short wListenPort)
{
    SOCKHANDLE tSock = socket(AF_INET, SOCK_STREAM, 0);
    if (INVALID_SOCKET == tSock)
    {
        return INVALID_SOCKET;
    }

    int nRet = SetSockReuse(tSock, 1);
    if ( SOCKET_ERROR == nRet )
    {
        SockCloseAndClear(tSock);
        return INVALID_SOCKET;
    }

    //�󶨵�ָ����ַ
    if ( dwLocalIPAddr == 0 )
    {
        dwLocalIPAddr = INADDR_ANY;
    }
    SOCKADDR_IN tSockAddr;
    memset(&tSockAddr, 0, sizeof(tSockAddr));
    tSockAddr.sin_family		= AF_INET;
    tSockAddr.sin_addr.s_addr	= dwLocalIPAddr;
    tSockAddr.sin_port			= htons( wListenPort );

    nRet = bind(tSock, (sockaddr *)&tSockAddr, sizeof(tSockAddr));
    if ( SOCKET_ERROR == nRet )
    {
        SockCloseAndClear(tSock);
        return INVALID_SOCKET;
    }

    nRet = listen( tSock, 128);
    if ( SOCKET_ERROR == nRet )
    {
        SockCloseAndClear(tSock);
        return INVALID_SOCKET;
    }

    return tSock;
}


