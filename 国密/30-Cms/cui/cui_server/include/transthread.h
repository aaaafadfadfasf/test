/*****************************************************************************
   ģ����      : cui_fcgi 
   �ļ���      : transthread.h
   ����ļ�    : 
   �ļ�ʵ�ֹ���: transthread.cppʵ�ֽ���cui_fcgi��Ϣ
   ����        : liangli
   �汾        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
   2011/11/8   1.0         liangli        ����
******************************************************************************/
#ifndef _TRANS_THREAD_H_
#define _TRANS_THREAD_H_
#include <list>
using std::list;
#include "cms/container/xmap.h"
#include "cms/cmsassert.h"
#include "cms/ospext/ospmailbox.h"
#include "transutility.h"

typedef TRecvNode * PTRecvNode;
typedef CXMap<u64, PTRecvNode> TRecvingNodeList;
const u32 SELECT_WAIT_INTERWAL_SEC = 0;     //�����߳̿��еȴ�ʱ��(s)
const u32 SELECT_WAIT_INTERWAL_MSEC = 20 * 1000;

struct TRecvThreadInfo
{
    u32             dwListenIpAddr;
    unsigned short  wListenPort;
};

typedef CXMailbox<PTRecvNode> CRecvThreadMailBox;  //recv�̵߳�����

/*====================================================================
������      : PostMsgToSvrApp
����        : Ϊ�˲���׮����װPost��Ϣ��App����Ĳ����߼�
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/26/             liangli
====================================================================*/
void PostMsgToSvrApp(u16 wEvent, const void* pvContent = 0, u16 wLength = 0);

/*====================================================================
������      : PostMsgToRecvThread
����        : APP�߳̽�RspMsg��ֵ���ݸ�Recv�߳�
�㷨ʵ��    : Ͷ�ݵ�Recv�̵߳�������
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/5/26/             liangli
====================================================================*/
bool PostMsgToRecvThread(PTRecvNode &ptRecvNode, int nMsgType, const char *szMsg, u32 dwLen);

void CleanNodeList( TRecvingNodeList &tNodeList );

#ifdef _LINUX_
void * EpollReceivingThread(void *pParam);
#endif
void * SelectReceivingThread(void *pParam);

#endif

