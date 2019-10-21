/*****************************************************************************
   模块名      : cui_fcgi 
   文件名      : transutility.h
   相关文件    : 
   文件实现功能: cui_fcgi功能
   作者        : 梁  黎
   版本        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人      修改内容
   2011/09/23  1.0         梁  黎        创建
******************************************************************************/
#ifndef _TRANS_UTILITY_H_
#define _TRANS_UTILITY_H_

#include <string>
#include "cms/cmsassert.h"
#include "cbb/osp/osp.h"

///////////////////////////////////////////////////////////////////
//	Linux 操作系统头文件
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
//	Win32 操作系统头文件
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
//	操作系统相关数据类型定义
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

//根据系统ticks来获取秒级时间，避免依赖时间差的操作因为修改了系统时间而出现异常
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
        RECVING,       //cui.fcgi与cuiserver改为长连接，RECVING表示连接可用处于IDLE或者正在接收
        WAITAPP,
        SENDING,
    };

    SOCKHANDLE tSock;
    
    int nRecvedLen;             //已经接收的总长度, 包括CMsgHead
    int nMsgTotalLen;           //整个消息总长度, 包括CMsgHead
                                //在APP使用此node作为邮箱消息时，表示业务应答消息的总长度
    
    //int nDeadLine;            //最后期限(系统时间超过此值则操作作废)
    int m_nMsgType;             //保存业务上的消息(虽然可以写死消息号，但是这么做为了解除对业务的依赖关系)
    u32 dwSeqNum;               //linux下sock会重复，该值保证sock的u64key不重复
    TMsTime m_tTime;
    emSockStatus emCurStatus;   //当前RecvNode状态机

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
        //这里不能关闭套接字，因为pnode存在副本，APP线程只能删除副本，关闭sock后会导致recv出错
        tSock = INVALID_SOCKET;
        nRecvedLen = 0;
        nMsgTotalLen = 0;
        m_nMsgType = 0;
        dwSeqNum = 0;
        emCurStatus = CLOSE;

        szRecvBuf[0] = '\0';
        //memset(szRecvBuf, 0, sizeof(szRecvBuf));  //节省性能
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
函数名：ConnectToCuiServer
功能：连接
算法实现：（可选项）
引用全局变量：
输入参数说明：std::string &strCuiIp
            unsigned short wCuiPort
            int nTimeOut        单位毫秒

返回值说明：返回SOCKHANDLE
====================================================================*/
SOCKHANDLE ConnectToCuiServer(const std::string &strCuiIp, unsigned short wCuiPort, int nTimeOut = 5*1000);

/*====================================================================
函数名：SockSend
功能：  向CU发送消息
算法实现：用户只需要关心发送的消息内容，MsgHead由本函数封装
引用全局变量：
输入参数说明：

返回值说明：大于0表示成功, 否则失败
2012.5.26 modify by liangli
因为APP线程在调用本函数时需要使用sock，从而违反了线程一致性原则，sock应该只在recv线程内使用
所以本函数自2012.5.26日起，cuiserver不再调用，保留以作纪念
====================================================================*/
int SockSend(SOCKHANDLE tSock, s32 nMsgType, LPCSTR pchMsgBodyBuf, unsigned int dwMsgBodyLen);

/*====================================================================
函数名：DoSend
功能：向CUI发送消息
算法实现：不关心发送的内容，只负责一次性把整个Buf发送出去
引用全局变量：
输入参数说明：

返回值说明：大于0表示成功, 否则失败
====================================================================*/
int DoSend(SOCKHANDLE tSock, LPCSTR pchBuf, unsigned int dwLen);

/*====================================================================
函数名：SockRecv
功能：从CUI接收消息
算法实现：（可选项）
引用全局变量：
输入参数说明：

返回值说明：大于0表示成功, 否则失败
====================================================================*/
int SockRecv(TRecvNode *ptRecvNode);

/*====================================================================
函数名：DoRecv
功能：从CUI接收消息
算法实现：（可选项）
引用全局变量：
输入参数说明：

返回值说明：大于0表示消息接收完毕, 0表示需要继续接收, -1表示失败
====================================================================*/
int DoRecv(TRecvNode *ptRecvNode);

/*====================================================================
函数名：CreateTcpListenSock
功能：从CUI接收消息
算法实现：（可选项）
引用全局变量：
输入参数说明：

返回值说明：成功返回已经bind的socket，失败返回INVALID_SOCKET
====================================================================*/
SOCKHANDLE CreateTcpListenSock(unsigned int dwLocalIPAddr, unsigned short wListenPort);

int SetSockNoBlock(SOCKHANDLE tSock, int bNoBlock);

#endif

