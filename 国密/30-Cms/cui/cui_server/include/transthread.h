/*****************************************************************************
   模块名      : cui_fcgi 
   文件名      : transthread.h
   相关文件    : 
   文件实现功能: transthread.cpp实现接收cui_fcgi消息
   作者        : liangli
   版本        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人      修改内容
   2011/11/8   1.0         liangli        创建
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
const u32 SELECT_WAIT_INTERWAL_SEC = 0;     //接收线程空闲等待时间(s)
const u32 SELECT_WAIT_INTERWAL_MSEC = 20 * 1000;

struct TRecvThreadInfo
{
    u32             dwListenIpAddr;
    unsigned short  wListenPort;
};

typedef CXMailbox<PTRecvNode> CRecvThreadMailBox;  //recv线程的邮箱

/*====================================================================
函数名      : PostMsgToSvrApp
功能        : 为了测试桩，封装Post消息到App处理的部分逻辑
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/26/             liangli
====================================================================*/
void PostMsgToSvrApp(u16 wEvent, const void* pvContent = 0, u16 wLength = 0);

/*====================================================================
函数名      : PostMsgToRecvThread
功能        : APP线程将RspMsg按值传递给Recv线程
算法实现    : 投递到Recv线程的邮箱中
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/5/26/             liangli
====================================================================*/
bool PostMsgToRecvThread(PTRecvNode &ptRecvNode, int nMsgType, const char *szMsg, u32 dwLen);

void CleanNodeList( TRecvingNodeList &tNodeList );

#ifdef _LINUX_
void * EpollReceivingThread(void *pParam);
#endif
void * SelectReceivingThread(void *pParam);

#endif

