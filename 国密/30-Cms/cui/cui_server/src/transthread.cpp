/*****************************************************************************
ģ����      : cui_fcgi 
�ļ���      : transthread.cpp
����ļ�    : 
�ļ�ʵ�ֹ���: transthread.cppʵ�ֽ���cui_fcgi��Ϣ
����        : liangli
�汾        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2011/11/8   1.0         liangli        ����
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

    nRecvedLen = 0;     //��ʱ��ʾ�Ѿ����͵���Ϣ����
    nMsgTotalLen = nRet + (s32)dwMsgBodyLen;     //��ʱ��ʾ��Ҫ���͵��ܳ���
    emCurStatus = SENDING;  //�л���sending״̬��ȫ��������ϻ��߷���ʧ�ܺ��л�Ϊclose״̬
    return true;
}

//�ú���ֻ���ڱ�cpp�߳��ڵ��ã���ֹ��app�̵߳��ñ�����
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
��������EpollAdd
���ܣ���һ��tSock���뵽epoll�б���
�㷨ʵ�֣�����ѡ�
����ȫ�ֱ�����
�������˵����

����ֵ˵����0��ʾ�ɹ���-1��ʾʧ��
====================================================================*/
int EpollAdd(SOCKHANDLE tSock, int epollfd, unsigned int nContext = 0)
{
    epoll_event tEvent;
    memset(&tEvent, 0, sizeof(tEvent));
    tEvent.events = EPOLLIN | EPOLLPRI | EPOLLERR;  //EPOLLOUT�¼��������, ��������ģʽ��epoll_wait������ѭ��

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
��������EpollDel
���ܣ���һ��tSock��epoll�б���ɾ��
�㷨ʵ�֣�����ѡ�
����ȫ�ֱ�����
�������˵����

����ֵ˵����0��ʾ�ɹ���-1��ʾʧ��
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

    int sEventcout = -1;		// epoll�¼���
    epoll_event atEpollEvent[MAX_EPOLL_SIZE+4];
    memset( atEpollEvent , 0 , sizeof(atEpollEvent) );
    epoll_event* ptEpollEvent = NULL;

    while (g_bSockRecvThreadRun && true)
    {
        //���ȼ������
        bool bHasMail = true;
        while(bHasMail)
        {
            PTRecvNode ptTcpMailMsgToCu = NULL;
            bHasMail = g_cRecvThreadMailBox.GetMsg(ptTcpMailMsgToCu, 0);    //����ط����Ժ�����Ϊ0�����̷���
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
                        //�����nMsgTotalLen�����ΪӦ��ģ�û�о���CMsgHeadͷ��װ��ҵ����Ϣ
                        ptNode->SetRspMsg(ptTcpMailMsgToCu->m_nMsgType, ptTcpMailMsgToCu->szRecvBuf, ptTcpMailMsgToCu->nMsgTotalLen);
                    }
                    //�����nodeָ��Ϊ�� �� ״̬��������ֱ�Ӻ��Ը�Ӧ����Ϣ
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
                        //log �ɿ��������쳣ϸ�ִ�ӡ
                    }
                }
                else
                {
                    CuiWriteLog(g_strLogFile.c_str(), "ERROR:ptNode(seqnum %d, sock %d) has been deleted\n",
                        ptTcpMailMsgToCu->dwSeqNum, ptTcpMailMsgToCu->tSock);
                    //log ��node�Ѿ���ɾ��
                }
                delete ptTcpMailMsgToCu;
                ptTcpMailMsgToCu = NULL;
            }
        }

        //���η���node�д���sending״̬����Ϣ
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
                        { //����ʧ�ܣ�ɾ����node
                            //log
                            CuiWriteLog(g_strLogFile.c_str(),"����sock[%d]ʧ��\n", ptRecvNode->tSock);
                            SockCloseAndClear(ptRecvNode->tSock);
                            EraseAndDeleteRecvNode(tNodeList, key, ptRecvNode);
                        }
                        else
                        {
                            //�����������´��ٷ���
                        }
                    }
                    else
                    { //���η��ͳɹ�������δ�ط������
                        ptRecvNode->nRecvedLen += nRet;
                        if ( ptRecvNode->nRecvedLen == ptRecvNode->nMsgTotalLen )
                        { //������� 2012.12.10 cui.fcgi��cuiserver��Ϊ�����ӣ�һ����Ϣ������Ϻ��ٹر�����
                            //SockCloseAndClear(ptRecvNode->tSock);
                            //EraseAndDeleteRecvNode(tNodeList, key, ptRecvNode);
                            //����seqnum�Լ�״̬
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

        // �ȴ�EPOLL�¼����������MAX_EPOLL_SIZE����
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

        //����epoll�¼�
        for (int nIndex = 0; nIndex<sEventcout; nIndex++)
        {
            ptEpollEvent = &(atEpollEvent[nIndex]);
            if( 0 == ( ptEpollEvent->events & ( EPOLLIN | EPOLLPRI | EPOLLERR ) ) )
            {
                //OspTaskDelay(10);
                continue;
            }

            //���ȴ���Listen�¼�, ���յ�������
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
                            //�ж��Ƿ�new�ɹ�
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
            else //����TCP�ڵ�
            {
                u64 key = ptEpollEvent->data.u64;
                SOCKHANDLE tSock = (SOCKHANDLE)key;
                //����tSock��Ӧ��RecvNode
                PTRecvNode ptRecvNode = NULL;
                bool bFind = tNodeList.Find(key, ptRecvNode);
                if ( bFind )
                {
                    CMS_ASSERT(INVALID_SOCKET != tSock);
                    CMS_ASSERT(NULL != ptRecvNode);
                    //����Ҫ����״̬���жϴ���Recving״̬���ܽ��գ�����ֱ�ӹر��׽��֣����ͷ�Node
                    if(ptRecvNode->emCurStatus == TRecvNode::RECVING)
                    {
                        int nRet = DoRecv(ptRecvNode);
                        if ( nRet > 0 )
                        {
                            //������Ϻ󣬽���Ϣ��ֵ���ݽ���app����App����free��node
                            ptRecvNode->emCurStatus = TRecvNode::WAITAPP;

                            //����һ�����ݸ������ø�������nodelist�У���app�߳�ɾ��
                            PTRecvNode ptNewMsgNode = new TRecvNode(*ptRecvNode);

                            CMsgHead cMsgHead;
                            cMsgHead.FromBuffer(ptRecvNode->szRecvBuf, ptRecvNode->nMsgTotalLen);

                            PostMsgToSvrApp((u16)(cMsgHead.nMsgType), &ptNewMsgNode, sizeof(ptNewMsgNode));
                        }
                        else if ( nRet == 0 )
                        { //δ���
                            continue;
                        }
                        else
                        { 
                            //����ʱ�����ֽ���ʧ�ܻ��߶Զ˹ر����ӣ�����������Ϣ��ɾ��node
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
                        { //����ʧ�ܣ���ʾ���ӶϿ�
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
                    //log ��node�Ѿ���ɾ��
                }
            }
        }
    }

    //ɾ����ʱnode
    if(ptTmpNode != NULL)
    {
        delete ptTmpNode;
        ptTmpNode = NULL;
    }
    //�ر�NodeList�е����нڵ�
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
        //���ȼ������
        bool bHasMail = true;
        while ( bHasMail )
        {
            PTRecvNode ptTcpMailMsgToCu = NULL;
            bHasMail = g_cRecvThreadMailBox.GetMsg(ptTcpMailMsgToCu, 0);    //����ط����Ժ�����Ϊ0�����̷���
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
                        //�����nMsgTotalLen�����ΪӦ��ģ�û�о���CMsgHeadͷ��װ��ҵ����Ϣ
                        ptNode->SetRspMsg(ptTcpMailMsgToCu->m_nMsgType, ptTcpMailMsgToCu->szRecvBuf, (u32)ptTcpMailMsgToCu->nMsgTotalLen);
                    }
                    //�����nodeָ��Ϊ�� �� ״̬��������ֱ�Ӻ��Ը�Ӧ����Ϣ
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
                        //log �ɿ��������쳣ϸ�ִ�ӡ
                    }
                }
                else
                {
                    CuiWriteLog(g_strLogFile.c_str(), "ERROR:ptNode(seqnum %d, sock %d) has been deleted\n",
                        ptTcpMailMsgToCu->dwSeqNum, ptTcpMailMsgToCu->tSock);
                    //log ��node�Ѿ���ɾ��
                }
                delete ptTcpMailMsgToCu;
                ptTcpMailMsgToCu = NULL;
            }
        }

        //���η���node�д���sending״̬����Ϣ
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
                        { //����ʧ�ܣ�ɾ����node
                            //log
                            CuiWriteLog(g_strLogFile.c_str(),"����sock[%d]ʧ��\n", ptRecvNode->tSock);
                            SockCloseAndClear(ptRecvNode->tSock);
                            EraseAndDeleteRecvNode(tNodeList, key, ptRecvNode);
                        }
                        else
                        {
                            //�����������´��ٷ���
                        }
                    }
                    else
                    { //���η��ͳɹ�������δ�ط������
                        ptRecvNode->nRecvedLen += nRet;
                        if ( ptRecvNode->nRecvedLen == ptRecvNode->nMsgTotalLen )
                        { //������� 2012.12.10 cui.fcgi��cuiserver��Ϊ�����ӣ�һ����Ϣ������Ϻ��ٹر�����
                            //SockCloseAndClear(ptRecvNode->tSock);
                            //EraseAndDeleteRecvNode(tNodeList, key, ptRecvNode);
                            //����seqnum�Լ�״̬
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
        //��ListenSock�������������
        if ( INVALID_SOCKET != hListenSock )
        {
            FD_SET( hListenSock, &SockSet );
        }

        //���ͻ�������Ҳ�������������
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

        //��ʼ�����׽���
        int nRet = select( FD_SETSIZE, &SockSet, NULL, NULL, &tTimeout );
        //û������
        if ( 0 == nRet )
        {
            tTimeout.tv_sec = SELECT_WAIT_INTERWAL_SEC; // linuxƽ̨��timeout��ʵʱ�޸ģ���ʾʣ��ʱ��
            tTimeout.tv_usec = SELECT_WAIT_INTERWAL_MSEC;
            continue;
        }
        //����
        else if ( SOCKET_ERROR == nRet )
        {
            int nErrorCode = GetSockError();
            OspPrintf(TRUE, FALSE, "select error!!, errorNO: %d\n", nErrorCode);
            CuiWriteLog(g_strLogFile.c_str(), "select error!!, errorNO: %d\n", nErrorCode);
            OspTaskDelay(10);
            continue;
        }

        //���ȴ��������¼�
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

        //����������ӵ�����
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
                            //������Ϻ󣬽���Ϣ��ֵ���ݽ���app����App����free��node
                            ptRecvNode->emCurStatus = TRecvNode::WAITAPP;

                            //����һ�����ݸ������ø�������nodelist�У���app�߳�ɾ��
                            PTRecvNode ptNewMsgNode = new TRecvNode(*ptRecvNode);

                            CMsgHead cMsgHead;
                            cMsgHead.FromBuffer(ptRecvNode->szRecvBuf, ptRecvNode->nMsgTotalLen);

                            PostMsgToSvrApp((u16)(cMsgHead.nMsgType), &ptNewMsgNode, sizeof(ptNewMsgNode));
                        }
                        else if ( nRecRet == 0 )
                        { //δ���
                            continue;
                        }
                        else
                        { 
                            //����ʱ�����ֽ���ʧ�ܻ��߶Զ˹ر����ӣ�����������Ϣ��ɾ��node
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
                        { //����ʧ�ܣ���ʾ���ӶϿ�
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

    //ɾ����ʱnode
    if(ptTmpNode != NULL)
    {
        delete ptTmpNode;
        ptTmpNode = NULL;
    }
    //�ر�NodeList�е����нڵ�
    CleanNodeList(tNodeList);
    SockCloseAndClear(hListenSock);
    return NULL;
}