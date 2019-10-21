/*****************************************************************************
模块名      : cui_fcgi 
文件名      : transthread.cpp
相关文件    : 
文件实现功能: transthread.cpp实现接收cui_fcgi消息
作者        : liangli
版本        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2011/11/8   1.0         liangli        创建
******************************************************************************/
#include "cms/ospext/ospext.h"
#include "transthread.h"
#include "cuiproto.h"
#include "cuidbg.h"

CRecvThreadMailBox g_cRecvThreadMailBox;

extern bool     g_bSockRecvThreadRun;
extern string g_strLogFile;

#define MAX_MSG_SEQ_NO ((u32)0x1FFFFFFF)
static u32 g_dwSeqGenerator = 1;

const u32 GenUdpSequenceNum()
{
    u32 dwRet = g_dwSeqGenerator;
    if ( g_dwSeqGenerator >= MAX_MSG_SEQ_NO )
    {
        g_dwSeqGenerator = 1;
    }
    else
    {
        ++g_dwSeqGenerator;
    }
    return dwRet;
}

bool TRecvNode::SetRspMsg(int nMsgType, LPCSTR pchMsgBodyBuf, unsigned int dwMsgBodyLen)
{
    CMS_ASSERT(NULL != pchMsgBodyBuf);
    CMS_ASSERT( 0 != dwMsgBodyLen );
    CMsgHead cHead;
    cHead.nMsgType = nMsgType;
    cHead.nMsgLen = (s32)dwMsgBodyLen;

    s32 nRet = cHead.ToBuffer(szRecvBuf, sizeof(CMsgHead));
    CMS_ASSERT(nRet > 0);

    memcpy(szRecvBuf+ nRet, pchMsgBodyBuf, dwMsgBodyLen);

    nRecvedLen = 0;     //此时表示已经发送的消息长度
    nMsgTotalLen = nRet + (s32)dwMsgBodyLen;     //此时表示需要发送的总长度
    emCurStatus = SENDING;  //切换成sending状态，全部发送完毕或者发送失败后切换为close状态
    return true;
}

//该函数只能在本cpp线程内调用，禁止在app线程调用本函数
static void EraseAndDeleteRecvNode(TRecvingNodeList &tNodeList, const u64 &key, PTRecvNode &ptRecvNode)
{
    ptRecvNode->emCurStatus = TRecvNode::CLOSE;
    tNodeList.Erase(key);
    delete ptRecvNode;
    ptRecvNode = NULL;
}

bool PostMsgToRecvThread(PTRecvNode &ptRecvNode, int nMsgType, const char *szMsg, u32 dwLen)
{
    CMS_ASSERT(szMsg != NULL);
    CMS_ASSERT(NULL != ptRecvNode);
	if (!szMsg || !ptRecvNode)
	{
		return false;
	}
    ptRecvNode->m_nMsgType = nMsgType;
    if (dwLen < MAX_RECV_MSG_LEN)
    {
        ptRecvNode->nMsgTotalLen = (s32)dwLen;
        memcpy(ptRecvNode->szRecvBuf, szMsg, dwLen);
    }
    else
    {
        CMS_ASSERT(dwLen < MAX_RECV_MSG_LEN);
        CEventRsp cRsp;
        cRsp.SetErrorCode(ERR_CUI_RSP_MESSAGE_TOO_LONG);
        string strRsp(cRsp.ToXml());
        ptRecvNode->nMsgTotalLen = (s32)strRsp.size();
        memcpy(ptRecvNode->szRecvBuf, strRsp.c_str(), (s32)strRsp.size());

        GLOBELOG(TCP_THREAD, ERROR_LEV, "PostMsgToRecvThread failed, dwLen=%u, szMsg=%s\n", dwLen, szMsg);
    }
    

    bool bRet = g_cRecvThreadMailBox.PostMsg(ptRecvNode);
    if ( ! bRet )
    {
        delete ptRecvNode;
    }
    ptRecvNode = NULL;

    return bRet;
}

void CleanNodeList( TRecvingNodeList &tNodeList ) 
{
    Iterator pPos;
    pPos.SetToBegin();
    while ( ! pPos.End() )
    {
        u64 key = 0;
        SOCKHANDLE tSockHandle = INVALID_SOCKET;
        PTRecvNode ptRecvNode = NULL;

        if ( tNodeList.Iterate(pPos, key, ptRecvNode) )
        {
            tSockHandle = (SOCKHANDLE)key;
            CMS_ASSERT(INVALID_SOCKET != tSockHandle);         
            CMS_ASSERT(NULL != ptRecvNode);
            SockCloseAndClear(ptRecvNode->tSock);
            EraseAndDeleteRecvNode(tNodeList, key, ptRecvNode);
        }
    }
    tNodeList.Empty();
}

#ifdef USE_EPOLL
/*====================================================================
函数名：EpollAdd
功能：将一个tSock加入到epoll列表中
算法实现：（可选项）
引用全局变量：
输入参数说明：

返回值说明：0表示成功，-1表示失败
====================================================================*/
int EpollAdd(SOCKHANDLE tSock, int epollfd, unsigned int nContext = 0)
{
    epoll_event tEvent;
    memset(&tEvent, 0, sizeof(tEvent));
    tEvent.events = EPOLLIN | EPOLLPRI | EPOLLERR;  //EPOLLOUT事件不能添加, 否则长连接模式下epoll_wait陷入死循环

    unsigned long long ddwValue = nContext;
    tEvent.data.u64 = (ddwValue << 32) + tSock;

    int nRet = epoll_ctl(epollfd, EPOLL_CTL_ADD, tSock, &tEvent);
    if ( nRet < 0 )
    {
        return -1;
    }
    return 0;
}


/*====================================================================
函数名：EpollDel
功能：将一个tSock从epoll列表中删除
算法实现：（可选项）
引用全局变量：
输入参数说明：

返回值说明：0表示成功，-1表示失败
====================================================================*/
int EpollDelete(SOCKHANDLE tSock, int epollfd)
{
    epoll_event tEvent;
    memset(&tEvent, 0, sizeof(tEvent));

    int nRet = epoll_ctl (epollfd, EPOLL_CTL_DEL, tSock, &tEvent);
    if ( nRet < 0 )
    {
        return -1;
    }
    return 0;
}

#endif


#ifdef USE_EPOLL

void * EpollReceivingThread(void *pParam)
{
    CMS_ASSERT(NULL != pParam);
    if ( NULL == pParam )
    {
        return NULL;
    }

    TRecvThreadInfo *ptInfo = (TRecvThreadInfo *)pParam;
    SOCKHANDLE hListenSock = INVALID_SOCKET;
    hListenSock = CreateTcpListenSock(ptInfo->dwListenIpAddr, ptInfo->wListenPort);
    CMS_ASSERT(INVALID_SOCKET != hListenSock);
    if ( INVALID_SOCKET == hListenSock )
    {
        return NULL;
    }

    TRecvingNodeList tNodeList;
    PTRecvNode ptTmpNode = new TRecvNode;

    int g_nEpollFd = epoll_create(MAX_EPOLL_SIZE+1);
    if ( g_nEpollFd == -1 )
    {
        return NULL;
    }

    if ( EpollAdd(hListenSock, g_nEpollFd, 0) < 0 )
    {
        return NULL;
    }

    int sEventcout = -1;		// epoll事件数
    epoll_event atEpollEvent[MAX_EPOLL_SIZE+4];
    memset( atEpollEvent , 0 , sizeof(atEpollEvent) );
    epoll_event* ptEpollEvent = NULL;

    while (g_bSockRecvThreadRun && true)
    {
        //首先检查邮箱
        bool bHasMail = true;
        while(bHasMail)
        {
            PTRecvNode ptTcpMailMsgToCu = NULL;
            bHasMail = g_cRecvThreadMailBox.GetMsg(ptTcpMailMsgToCu, 0);    //这个地方可以后续改为0，立刻返回
            if ( bHasMail )
            {
                CMS_ASSERT(NULL != ptTcpMailMsgToCu);

                u64 Value = ptTcpMailMsgToCu->dwSeqNum;
                u64 key = (Value << 32) + ptTcpMailMsgToCu->tSock;

                PTRecvNode ptNode = NULL;
                bool bFind = tNodeList.Find(key, ptNode);
                if ( bFind )
                {
                    if ( NULL != ptNode && ptNode->emCurStatus == TRecvNode::WAITAPP )
                    {
                        //这里的nMsgTotalLen含义变为应答的，没有经过CMsgHead头封装的业务消息
                        ptNode->SetRspMsg(ptTcpMailMsgToCu->m_nMsgType, ptTcpMailMsgToCu->szRecvBuf, ptTcpMailMsgToCu->nMsgTotalLen);
                    }
                    //否则该node指针为空 或 状态不正常，直接忽略该应答消息
                    else
                    { 
                        if(ptNode == NULL)
                        {
                            CuiWriteLog(g_strLogFile.c_str(), "ERROR:ptNode(seqnum %d, sock %d) is null\n",
                                ptTcpMailMsgToCu->dwSeqNum, ptTcpMailMsgToCu->tSock);
                        }
                        else
                        {
                            CuiWriteLog(g_strLogFile.c_str(), "ERROR:ptNode(seqnum %d, sock %d) status[%d] is not WAITAPP\n",
                                ptTcpMailMsgToCu->dwSeqNum, ptTcpMailMsgToCu->tSock, ptNode->emCurStatus);
                        }
                        //log 可考虑两种异常细分打印
                    }
                }
                else
                {
                    CuiWriteLog(g_strLogFile.c_str(), "ERROR:ptNode(seqnum %d, sock %d) has been deleted\n",
                        ptTcpMailMsgToCu->dwSeqNum, ptTcpMailMsgToCu->tSock);
                    //log 该node已经被删除
                }
                delete ptTcpMailMsgToCu;
                ptTcpMailMsgToCu = NULL;
            }
        }

        //依次发送node中处于sending状态的消息
        Iterator pPos;
        pPos.SetToBegin();
        while ( ! pPos.End() )
        {
            u64 key = 0;
            SOCKHANDLE tSockHandle = INVALID_SOCKET;
            PTRecvNode ptRecvNode = NULL;

            if ( tNodeList.Iterate(pPos, key, ptRecvNode) )
            {
                tSockHandle = (SOCKHANDLE)key;
                CMS_ASSERT(INVALID_SOCKET != tSockHandle);
                CMS_ASSERT(NULL != ptRecvNode);
                if ( ptRecvNode->emCurStatus == TRecvNode::SENDING )
                {
                    int nRet = send(tSockHandle, 
                        ptRecvNode->szRecvBuf + ptRecvNode->nRecvedLen, 
                        ptRecvNode->nMsgTotalLen - ptRecvNode->nRecvedLen, 0);
                    if ( nRet == SOCKET_ERROR )
                    {
                        if ( !IsErrorcodeWouldBlock(GetSockError()) )
                        { //接收失败，删除该node
                            //log
                            CuiWriteLog(g_strLogFile.c_str(),"发送sock[%d]失败\n", ptRecvNode->tSock);
                            SockCloseAndClear(ptRecvNode->tSock);
                            EraseAndDeleteRecvNode(tNodeList, key, ptRecvNode);
                        }
                        else
                        {
                            //发送阻塞，下次再发送
                        }
                    }
                    else
                    { //本次发送成功，但是未必发送完毕
                        ptRecvNode->nRecvedLen += nRet;
                        if ( ptRecvNode->nRecvedLen == ptRecvNode->nMsgTotalLen )
                        { //发送完毕 2012.12.10 cui.fcgi和cuiserver改为长连接，一次消息交互完毕后不再关闭连接
                            //SockCloseAndClear(ptRecvNode->tSock);
                            //EraseAndDeleteRecvNode(tNodeList, key, ptRecvNode);
                            //保存seqnum以及状态
                            u32 seqNum = ptRecvNode->dwSeqNum;
                            SOCKHANDLE tSock = ptRecvNode->tSock;
                            ptRecvNode->Clear();
                            ptRecvNode->dwSeqNum = seqNum;
                            ptRecvNode->tSock = tSock;
                            ptRecvNode->emCurStatus = TRecvNode::RECVING;
                        }
                    }
                }
            }
        }

        // 等待EPOLL事件，最多允许MAX_EPOLL_SIZE并发
        sEventcout = epoll_wait(g_nEpollFd, atEpollEvent, MAX_EPOLL_SIZE+1, (SELECT_WAIT_INTERWAL_MSEC)/1000);
        if ( 0 > sEventcout )
        {
            int nErrorCode = GetSockError();
            CuiWriteLog(g_strLogFile.c_str(), "epoll_wait error!!, errorNO: %d\n", nErrorCode);
            OspTaskDelay(10);
            continue;
        }
        else if ( 0 == sEventcout )
        {
            continue;
        }

        //处理epoll事件
        for (int nIndex = 0; nIndex<sEventcout; nIndex++)
        {
            ptEpollEvent = &(atEpollEvent[nIndex]);
            if( 0 == ( ptEpollEvent->events & ( EPOLLIN | EPOLLPRI | EPOLLERR ) ) )
            {
                //OspTaskDelay(10);
                continue;
            }

            //优先处理Listen事件, 新收到的连接
            if ( hListenSock == (u32)(ptEpollEvent->data.u64) )
            {
                SOCKADDR_IN tClientAddr;
                SOCKLEN tAddrLen = sizeof(sockaddr_in);
                SOCKHANDLE tNewClientSock = accept(hListenSock, (sockaddr *)&tClientAddr, &tAddrLen);
                if(tNewClientSock != INVALID_SOCKET)
                {
                    int nRet = SetSockNoBlock(tNewClientSock, 1);
                    if( nRet == SOCKET_ERROR )
                    {
                        CuiWriteLog(g_strLogFile.c_str(), "[VsipConnect]: VsipConnect set socket in nonblocking mode error!\n");
                        SockCloseAndClear(tNewClientSock);
                    }
                    else
                    {
                        u32 seqNum = GenUdpSequenceNum();
                        int nRet = EpollAdd(tNewClientSock, g_nEpollFd, seqNum);
                        if ( nRet < 0 )
                        {
                            CuiWriteLog(g_strLogFile.c_str(), "epoll_add fail\n");
                            SockCloseAndClear(tNewClientSock);   
                        }
                        else
                        {
                            s64 Value = seqNum;
                            u64 key = (Value << 32) + tNewClientSock;
                            PTRecvNode ptFreeRecvNode = new TRecvNode;
                            //判断是否new成功
                            CMS_ASSERT(ptFreeRecvNode != NULL);
                            ptFreeRecvNode->tSock = tNewClientSock;
                            ptFreeRecvNode->dwSeqNum = seqNum;
                            ptFreeRecvNode->emCurStatus = TRecvNode::RECVING;

                            tNodeList.Insert(key, ptFreeRecvNode);
                        }
                    }                  
                }
                else
                {
                    CuiWriteLog(g_strLogFile.c_str(), "accept fail\n");
                    SockCloseAndClear(tNewClientSock);
                }
            }
            else //其他TCP节点
            {
                u64 key = ptEpollEvent->data.u64;
                SOCKHANDLE tSock = (SOCKHANDLE)key;
                //查找tSock对应的RecvNode
                PTRecvNode ptRecvNode = NULL;
                bool bFind = tNodeList.Find(key, ptRecvNode);
                if ( bFind )
                {
                    CMS_ASSERT(INVALID_SOCKET != tSock);
                    CMS_ASSERT(NULL != ptRecvNode);
                    //首先要区分状态，判断处于Recving状态才能接收，否则直接关闭套接字，并释放Node
                    if(ptRecvNode->emCurStatus == TRecvNode::RECVING)
                    {
                        int nRet = DoRecv(ptRecvNode);
                        if ( nRet > 0 )
                        {
                            //接收完毕后，将消息按值传递交给app处理，App不能free该node
                            ptRecvNode->emCurStatus = TRecvNode::WAITAPP;

                            //复制一份数据副本，该副本不在nodelist中，由app线程删除
                            PTRecvNode ptNewMsgNode = new TRecvNode(*ptRecvNode);

                            CMsgHead cMsgHead;
                            cMsgHead.FromBuffer(ptRecvNode->szRecvBuf, ptRecvNode->nMsgTotalLen);

                            PostMsgToSvrApp((u16)(cMsgHead.nMsgType), &ptNewMsgNode, sizeof(ptNewMsgNode));
                        }
                        else if ( nRet == 0 )
                        { //未完毕
                            continue;
                        }
                        else
                        { 
                            //出错时，发现接收失败或者对端关闭连接，则抛弃此消息，删除node
                            CuiWriteLog(g_strLogFile.c_str(), "DoRecv Error on RECVING state: socket[%d]\n", ptRecvNode->tSock);
                            SockCloseAndClear(ptRecvNode->tSock);
                            EraseAndDeleteRecvNode(tNodeList, key, ptRecvNode);
                        }
                    }
                    else
                    {
                        //*ptTmpNode = *ptRecvNode;
                        ptTmpNode->Clear();
                        ptTmpNode->dwSeqNum = ptRecvNode->dwSeqNum;
                        ptTmpNode->tSock = ptRecvNode->tSock;
                        int nRet = DoRecv(ptTmpNode);
                        if ( nRet < 0 )
                        { //接收失败，表示连接断开
                            //log
                            CuiWriteLog(g_strLogFile.c_str(),"Node(sock %d, state %d) is not on RECVING state\n", ptRecvNode->tSock, ptRecvNode->emCurStatus);
                            SockCloseAndClear(ptRecvNode->tSock);
                            EraseAndDeleteRecvNode(tNodeList, key, ptRecvNode);
                        }
                    }
                }
                else
                {
                    CuiWriteLog(g_strLogFile.c_str(), "ERROR:ptNode has been deleted\n");
                    //log 该node已经被删除
                }
            }
        }
    }

    //删除临时node
    if(ptTmpNode != NULL)
    {
        delete ptTmpNode;
        ptTmpNode = NULL;
    }
    //关闭NodeList中的所有节点
    CleanNodeList(tNodeList);
    SockCloseAndClear(hListenSock);
    SockCloseAndClear(g_nEpollFd);
    return NULL;
}
#endif

void * SelectReceivingThread(void *pParam)
{
    CMS_ASSERT(NULL != pParam);
    if ( NULL == pParam )
    {
        return NULL;
    }

    TRecvThreadInfo *ptInfo = (TRecvThreadInfo *)pParam;
    SOCKHANDLE hListenSock = CreateTcpListenSock(ptInfo->dwListenIpAddr, ptInfo->wListenPort);
    CMS_ASSERT(INVALID_SOCKET != hListenSock);
    if ( INVALID_SOCKET == hListenSock )
    {
        return NULL;
    }

    TRecvingNodeList tNodeList;
    PTRecvNode ptTmpNode = new TRecvNode;

    fd_set SockSet;
    FD_ZERO(&SockSet);
    struct timeval tTimeout;
    memset(&tTimeout, 0, sizeof(tTimeout));
    tTimeout.tv_sec = SELECT_WAIT_INTERWAL_SEC;
    tTimeout.tv_usec = SELECT_WAIT_INTERWAL_MSEC;

    while ( g_bSockRecvThreadRun && true )
    {
        //首先检查邮箱
        bool bHasMail = true;
        while ( bHasMail )
        {
            PTRecvNode ptTcpMailMsgToCu = NULL;
            bHasMail = g_cRecvThreadMailBox.GetMsg(ptTcpMailMsgToCu, 0);    //这个地方可以后续改为0，立刻返回
            if ( bHasMail )
            {
                CMS_ASSERT(NULL != ptTcpMailMsgToCu);

                u64 Value = ptTcpMailMsgToCu->dwSeqNum;
                u64 key = (Value << 32) + ptTcpMailMsgToCu->tSock;

                PTRecvNode ptNode = NULL;
                bool bFind = tNodeList.Find(key, ptNode);
                if ( bFind )
                {
                    if ( NULL != ptNode && ptNode->emCurStatus == TRecvNode::WAITAPP )
                    {
                        //这里的nMsgTotalLen含义变为应答的，没有经过CMsgHead头封装的业务消息
                        ptNode->SetRspMsg(ptTcpMailMsgToCu->m_nMsgType, ptTcpMailMsgToCu->szRecvBuf, (u32)ptTcpMailMsgToCu->nMsgTotalLen);
                    }
                    //否则该node指针为空 或 状态不正常，直接忽略该应答消息
                    else
                    { 
                        if(ptNode == NULL)
                        {
                            CuiWriteLog(g_strLogFile.c_str(), "ERROR:ptNode(seqnum %d, sock %d) is null\n",
                                ptTcpMailMsgToCu->dwSeqNum, ptTcpMailMsgToCu->tSock);
                        }
                        else
                        {
                            CuiWriteLog(g_strLogFile.c_str(), "ERROR:ptNode(seqnum %d, sock %d) status[%d] is not WAITAPP\n",
                                ptTcpMailMsgToCu->dwSeqNum, ptTcpMailMsgToCu->tSock, ptNode->emCurStatus);
                        }
                        //log 可考虑两种异常细分打印
                    }
                }
                else
                {
                    CuiWriteLog(g_strLogFile.c_str(), "ERROR:ptNode(seqnum %d, sock %d) has been deleted\n",
                        ptTcpMailMsgToCu->dwSeqNum, ptTcpMailMsgToCu->tSock);
                    //log 该node已经被删除
                }
                delete ptTcpMailMsgToCu;
                ptTcpMailMsgToCu = NULL;
            }
        }

        //依次发送node中处于sending状态的消息
        Iterator pPos;
        pPos.SetToBegin();
        while ( ! pPos.End() )
        {
            u64 key = 0;
            SOCKHANDLE tSockHandle = INVALID_SOCKET;
            PTRecvNode ptRecvNode = NULL;

            if ( tNodeList.Iterate(pPos, key, ptRecvNode) )
            {
                tSockHandle = (SOCKHANDLE)key;
                CMS_ASSERT(INVALID_SOCKET != tSockHandle);
                CMS_ASSERT(NULL != ptRecvNode);
                if ( ptRecvNode->emCurStatus == TRecvNode::SENDING )
                {
                    int nRet = send(tSockHandle, 
                        ptRecvNode->szRecvBuf + ptRecvNode->nRecvedLen, 
                        ptRecvNode->nMsgTotalLen - ptRecvNode->nRecvedLen, 0);
                    if ( nRet == SOCKET_ERROR )
                    {
                        if ( !IsErrorcodeWouldBlock(GetSockError()) )
                        { //接收失败，删除该node
                            //log
                            CuiWriteLog(g_strLogFile.c_str(),"发送sock[%d]失败\n", ptRecvNode->tSock);
                            SockCloseAndClear(ptRecvNode->tSock);
                            EraseAndDeleteRecvNode(tNodeList, key, ptRecvNode);
                        }
                        else
                        {
                            //发送阻塞，下次再发送
                        }
                    }
                    else
                    { //本次发送成功，但是未必发送完毕
                        ptRecvNode->nRecvedLen += nRet;
                        if ( ptRecvNode->nRecvedLen == ptRecvNode->nMsgTotalLen )
                        { //发送完毕 2012.12.10 cui.fcgi和cuiserver改为长连接，一次消息交互完毕后不再关闭连接
                            //SockCloseAndClear(ptRecvNode->tSock);
                            //EraseAndDeleteRecvNode(tNodeList, key, ptRecvNode);
                            //保存seqnum以及状态
                            u32 seqNum = ptRecvNode->dwSeqNum;
                            SOCKHANDLE tSock = ptRecvNode->tSock;
                            ptRecvNode->Clear();
                            ptRecvNode->dwSeqNum = seqNum;
                            ptRecvNode->tSock = tSock;
                            ptRecvNode->emCurStatus = TRecvNode::RECVING;
                        }
                    }
                }
            }
        }

        FD_ZERO( &SockSet);
        //将ListenSock加入监听数组中
        if ( INVALID_SOCKET != hListenSock )
        {
            FD_SET( hListenSock, &SockSet );
        }

        //将客户端连接也加入监听数组中
        pPos.SetToBegin();
        while ( ! pPos.End() )
        {
            u64 key = 0;
            SOCKHANDLE tSockHandle = INVALID_SOCKET;
            PTRecvNode ptRecvNode = NULL;

            if ( tNodeList.Iterate(pPos, key, ptRecvNode) )
            {
                tSockHandle = (SOCKHANDLE)key;
                if ( INVALID_SOCKET != tSockHandle )
                {
                    FD_SET( tSockHandle, &SockSet );
                }
            }
        }

        //开始监听套接字
        int nRet = select( FD_SETSIZE, &SockSet, NULL, NULL, &tTimeout );
        //没有数据
        if ( 0 == nRet )
        {
            tTimeout.tv_sec = SELECT_WAIT_INTERWAL_SEC; // linux平台下timeout被实时修改，表示剩余时间
            tTimeout.tv_usec = SELECT_WAIT_INTERWAL_MSEC;
            continue;
        }
        //出错
        else if ( SOCKET_ERROR == nRet )
        {
            int nErrorCode = GetSockError();
            OspPrintf(TRUE, FALSE, "select error!!, errorNO: %d\n", nErrorCode);
            CuiWriteLog(g_strLogFile.c_str(), "select error!!, errorNO: %d\n", nErrorCode);
            OspTaskDelay(10);
            continue;
        }

        //优先处理连接事件
        if ( FD_ISSET( hListenSock, &SockSet ) )
        {
            SOCKADDR_IN tClientAddr;
            SOCKLEN tAddrLen = sizeof(sockaddr_in);
            SOCKHANDLE tNewClientSock = accept(hListenSock, (sockaddr *)&tClientAddr, &tAddrLen);

            if(tNewClientSock != INVALID_SOCKET)
            {
                int nSetNoBlockRet = SetSockNoBlock(tNewClientSock, 1);
                if( nSetNoBlockRet == SOCKET_ERROR )
                {
                    CuiWriteLog(g_strLogFile.c_str(), "[Connect set socket in nonblocking mode error!\n");
                    SockCloseAndClear(tNewClientSock);                    
                }
                else
                {
                    PTRecvNode ptFreeRecvNode = new TRecvNode;
                    CMS_ASSERT(NULL != ptFreeRecvNode);
                    ptFreeRecvNode->tSock = tNewClientSock;
                    ptFreeRecvNode->dwSeqNum = GenUdpSequenceNum();
                    ptFreeRecvNode->emCurStatus = TRecvNode::RECVING;

                    u64 Value = ptFreeRecvNode->dwSeqNum;
                    u64 key = (Value << 32) + tNewClientSock;
                    tNodeList.Insert(key, ptFreeRecvNode);
                }
            }          
        }

        //处理旧有连接的数据
        pPos.SetToBegin();
        while ( ! pPos.End() )
        {
            u64 key = 0;
            SOCKHANDLE tSock = INVALID_SOCKET;
            PTRecvNode ptRecvNode = NULL;
            if ( tNodeList.Iterate(pPos, key, ptRecvNode) )
            {
                tSock = (SOCKHANDLE)key;
                CMS_ASSERT(INVALID_SOCKET != tSock);
                CMS_ASSERT(NULL != ptRecvNode);
                if ( FD_ISSET( tSock, &SockSet ) )
                {
                    if(ptRecvNode->emCurStatus == TRecvNode::RECVING)
                    {
                        int nRecRet = DoRecv(ptRecvNode);
                        if ( nRecRet > 0 )
                        {
                            //接收完毕后，将消息按值传递交给app处理，App不能free该node
                            ptRecvNode->emCurStatus = TRecvNode::WAITAPP;

                            //复制一份数据副本，该副本不在nodelist中，由app线程删除
                            PTRecvNode ptNewMsgNode = new TRecvNode(*ptRecvNode);

                            CMsgHead cMsgHead;
                            cMsgHead.FromBuffer(ptRecvNode->szRecvBuf, ptRecvNode->nMsgTotalLen);

                            PostMsgToSvrApp((u16)(cMsgHead.nMsgType), &ptNewMsgNode, sizeof(ptNewMsgNode));
                        }
                        else if ( nRecRet == 0 )
                        { //未完毕
                            continue;
                        }
                        else
                        { 
                            //出错时，发现接收失败或者对端关闭连接，则抛弃此消息，删除node
                            CuiWriteLog(g_strLogFile.c_str(), "DoRecv Error on RECVING state: socket[%d]\n", ptRecvNode->tSock);
                            SockCloseAndClear(ptRecvNode->tSock);
                            EraseAndDeleteRecvNode(tNodeList, key, ptRecvNode);
                        }
                    }
                    else
                    {
                        ptTmpNode->Clear();
                        ptTmpNode->dwSeqNum = ptRecvNode->dwSeqNum;
                        ptTmpNode->tSock = ptRecvNode->tSock;
                        int nRecRet = DoRecv(ptTmpNode);
                        if ( nRecRet < 0 )
                        { //接收失败，表示连接断开
                            //log
                            CuiWriteLog(g_strLogFile.c_str(),"Node(sock %d, state %d) is not on RECVING state\n", ptRecvNode->tSock, ptRecvNode->emCurStatus);
                            SockCloseAndClear(ptRecvNode->tSock);
                            EraseAndDeleteRecvNode(tNodeList, key, ptRecvNode);
                        }
                    }
                }
            }
        }
    }

    //删除临时node
    if(ptTmpNode != NULL)
    {
        delete ptTmpNode;
        ptTmpNode = NULL;
    }
    //关闭NodeList中的所有节点
    CleanNodeList(tNodeList);
    SockCloseAndClear(hListenSock);
    return NULL;
}