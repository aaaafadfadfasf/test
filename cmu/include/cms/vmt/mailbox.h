/*===========================================================================
  模 块 名：mainbox
  文 件 名：mailbox.h
  实现功能：定义 CMailBox 类的接口
  相关文件：osp.h, xqueue.h
  作    者：梁黎
  版本信息：
  ===========================================================================
  修改记录：
    日期              版本               修改人              修改内容
  2009/10/05          1.0                 梁黎                CREATE
  ==========================================================================*/
#ifndef __MAILBOX_H__
#define __MAILBOX_H__

#include "osp.h"
#include "xqueue.h"

#define MAX_MAIL_BOX_NAME_LEN 256       //邮箱名长度
#define DEFAULT_MAIL_BOX_SIZE 100       //默认邮箱大小

#ifdef _LINUX_
#ifndef INFINITE
#define INFINITE            0xFFFFFFFF  // Infinite timeout
#endif
#endif

class CVmtAutoLock
{
private:
	SEMHANDLE& m_Lock;

public:
	CVmtAutoLock(SEMHANDLE& rLock, u32 dwTimeout = INFINITE) : m_Lock(rLock)
	{
		if ( NULL != m_Lock )
		{
			OspSemTakeByTime(m_Lock, dwTimeout);
		}
	}

	~CVmtAutoLock()
	{
		if ( NULL != m_Lock )
		{
			OspSemGive(m_Lock);
		}
	}
};

struct TMailBoxInfo 
{
public:
	TMailBoxInfo()
	{
        memset(this, 0, sizeof(TMailBoxInfo));
    }
    ~TMailBoxInfo()
    {
        Uninit();
    }
    BOOL32  Init()
    {
        if ( NULL == m_hLock )
        {
            return OspSemBCreate(&m_hLock);
        }
        return FALSE;
    }
    BOOL32 IsInit()
    {
        return (NULL != m_hLock);
    }
    void    Uninit()
    {
        if ( NULL != m_hLock )
        {
            OspSemDelete(m_hLock);
        }
        m_hLock = NULL;
    }
    void    RecvMsgInc()
    {
        CVmtAutoLock cLock(m_hLock);
        m_dwRecvdMsgNum++;
    }
    void    UpdateMsgWaitingTop(u32 dwTopNum)
    {
        CVmtAutoLock cLock(m_hLock);
        m_dwMsgWaitingTop = dwTopNum;
    }
    u32     GetMsgWaitingTop()
    {
        CVmtAutoLock cLock(m_hLock);
        return m_dwMsgWaitingTop;
    }
    void    MsgQueErrInc()
    {
        CVmtAutoLock cLock(m_hLock);
        m_dwMsgQueErr++;
    }
    void    SetMaxMsgWaiting(u32 dwNum)
    {
        CVmtAutoLock cLock(m_hLock);
        m_dwMaxMsgWaiting = dwNum;
    }
    void    DropMsgInc()
    {
        CVmtAutoLock cLock(m_hLock);
        m_dwDropMsgNum++;
    }
    void    ShowMailBoxInfo()
    {
        CVmtAutoLock cLock(m_hLock);
        OspPrintf(TRUE, FALSE, "income msg num  = %u\n", m_dwRecvdMsgNum);
        OspPrintf(TRUE, FALSE, "processd msg num = %u\n", m_dwProcMsgNum);
        OspPrintf(TRUE, FALSE, "top waiting num = %u\n", m_dwMsgWaitingTop);
        OspPrintf(TRUE, FALSE, "drop msg num    = %u\n", m_dwDropMsgNum);
        OspPrintf(TRUE, FALSE, "msg queue error = %u\n", m_dwMsgQueErr);
        OspPrintf(TRUE, FALSE, "mail max msg    = %u\n", m_dwMaxMsgWaiting);
    }
    void    ProcessedMsgInc()
    {
        CVmtAutoLock cLock(m_hLock);
        m_dwProcMsgNum++;
    }
protected:
    u32 m_dwRecvdMsgNum;        //已经接收的消息总数
    u32 m_dwProcMsgNum;         //已经处理的消息,即已经从邮箱中读取的消息
    u32 m_dwMsgWaitingTop;      //等待峰值
    u32 m_dwDropMsgNum;         //丢弃消息总数
    u32 m_dwMsgQueErr;          //消息队列异常次数
    u32 m_dwMaxMsgWaiting;      //邮箱大小

    SEMHANDLE m_hLock;          //邮箱统计信息的互斥锁
};

/*
===========================================
Semaphore InitCount     0           n
-------------------------------------------
Give Op                OK           NO
-------------------------------------------
Take Op                NO           OK
===========================================
*/

template <class T>
class CMailBox 
{
public:
typedef CXQueue<T> CMsgQueue;

	CMailBox()
    {
        m_bInit = FALSE;
        m_hConsumeSemaphore = NULL;
        m_hProductSemaphore = NULL;
        m_hSemLock = NULL;
        m_dwTimeout = INFINITE;
        m_bDelayForGetMsg = FALSE;
    }
	virtual ~CMailBox()
    {
        DestroyMailBox();
    }
    BOOL32 PushMsg(T tMsg)                  //投递邮件到邮箱中
    {
        if ( !m_bInit )
        {
            return FALSE;
        }

        //首先判断能否写入
        if ( !TryBeginProduct() )
        {
            //丢弃该消息
            m_cMailBoxInfo.DropMsgInc();
            return FALSE;
        }

        {
            //进入临界区开始写入
            CVmtAutoLock cLock(m_hSemLock);

            if ( m_cMsgQueue.Push(tMsg) )
            {
                //插入成功, 更新统计信息
                m_cMailBoxInfo.RecvMsgInc();
                if ( m_cMailBoxInfo.GetMsgWaitingTop() < m_cMsgQueue.GetSize() )
                {
                    m_cMailBoxInfo.UpdateMsgWaitingTop(m_cMsgQueue.GetSize());
                }
            }
            else
            {
                //异常流程, 插入消息到队列时失败
                OspPrintf(TRUE, FALSE, "[CMailBox::PutMsg]异常流程, 插入消息到队列时失败\n");

                //需要将pv操作回滚
                RollBackProduct();
                m_cMailBoxInfo.MsgQueErrInc();
                return FALSE;
            }
        }

        return TryEndProduct();
    }
    BOOL32 PopMsg(T &tMsg)                 //从邮箱中取出邮件
    {
        if ( !m_bInit )
        {
            return FALSE;
        }

        //首先判断能否获取
        if ( !TryBeginConsume() )
        {
            //本次无消息可以读取
            return FALSE;
        }

        //进入临界区开始获取
        CVmtAutoLock cLock(m_hSemLock);

        if ( m_cMsgQueue.Pop(tMsg) )
        {
            //获取成功, 更新统计信息
            m_cMailBoxInfo.ProcessedMsgInc();
        }
        else
        {
            //异常流程, 读取消息队列失败
            OspPrintf(TRUE, FALSE, "[CMailBox::GetMsg]异常流程, 从消息队列中读取消息时失败\n");
            
            //需要将pv操作回滚
            RollBackConsume();
            m_cMailBoxInfo.MsgQueErrInc();
            return FALSE;
        }

        return TryEndConsume();
    }

    /*====================================================================
    函数名      : CreateMailBox
    功能        : 创建邮箱
    算法实现    : 
    引用全局变量: 
    输入参数说明: u32 dwMailBoxSize         最多可以存放多少封邮件
                  u32 dwAccessTimeOut       访问超时时间, 这个是内部加锁时间, 0表示无论投递邮件还是取邮件均不延迟
                  BOOL32 bDelayForGetMsg    投递邮件加锁等待还是取出邮件加锁等待, FALSE表示在投递邮件时延迟
                  说明: 由于应用场合的不同, 加锁等待的要求也不同
                  1, 有的为了节省CPU并且要求立刻响应邮件消息,就要求在取邮件的时候加锁等待信号量
                  此类应用常见于消息处理App, 例如Cmu的VcSsn线程, 有消息来到时立刻处理, 没有时睡眠等待
                  2, 而有的应用要求最大限度的使用CPU并且保证消息不丢失, 尽量在本线程业务处理完毕之后
                  才在空闲时间处理邮件,同时要求邮件投递者能够阻塞等待, 确保命令成功投递到本线程
                  此类应用常见于媒体处理App, 例如RpCtrl的Rec线程, 尽力处理码流, 只是偶尔处理用户命令
    返回值说明  : 成功返回TRUE,失败返回FALSE
    ----------------------------------------------------------------------
    修改记录    :
    日  期      版本        修改人          修改内容
    20100216                liangli         创建
    ====================================================================*/
    BOOL32 CreateMailBox(u32 dwMailBoxSize, u32 dwAccessTimeOut, BOOL32 bDelayForGetMsg)
    {
        if ( 0 == dwMailBoxSize )
        {
            return FALSE;
        }

        if ( m_bInit )
        {
            return FALSE;
        }
        
        if ( !m_cMailBoxInfo.Init() )
        {
            return FALSE;
        }
        
        //申请新锁
        if ( !OspSemBCreate(&m_hSemLock) )
        {
            m_hSemLock = NULL;
            return FALSE;
        }
        
        m_bDelayForGetMsg = bDelayForGetMsg ? TRUE : FALSE;
        u32 dwConsumeInitCount = 0;
        if ( m_bDelayForGetMsg )
        {
            dwConsumeInitCount = 0;
        }
        else
        {
            dwConsumeInitCount = dwMailBoxSize;
        }

        if ( !OspSemCCreate(&m_hConsumeSemaphore, dwConsumeInitCount, dwMailBoxSize) )
        {
            OspSemDelete(m_hSemLock);
            m_hSemLock = NULL;
            m_hConsumeSemaphore = NULL;
            return FALSE;
        }
        if ( !OspSemCCreate(&m_hProductSemaphore, 0, dwMailBoxSize) )
        {
            OspSemDelete(m_hSemLock);
            OspSemDelete(m_hConsumeSemaphore);
            m_hSemLock = NULL;
            m_hConsumeSemaphore = NULL;
            m_hProductSemaphore = NULL;
            return FALSE;
        }
        
        m_dwTimeout = dwAccessTimeOut;
        m_bInit = TRUE;
        m_cMailBoxInfo.SetMaxMsgWaiting(dwMailBoxSize);
        return m_bInit;
    }
    u32    GetMailCount()                           //当前有多少封邮件
    {
        //进入临界区开始获取
        CVmtAutoLock cLock(m_hSemLock);
        return m_cMsgQueue.GetSize();
    }
    BOOL32 DestroyMailBox()
    {
        if ( m_hSemLock )
        {
            OspSemDelete(m_hSemLock);
            m_hSemLock = NULL;
        }
        
        if ( m_hConsumeSemaphore )
        {
            OspSemDelete(m_hConsumeSemaphore);
            m_hConsumeSemaphore = NULL;
        }
        if ( m_hProductSemaphore )
        {
            OspSemDelete(m_hProductSemaphore);
            m_hProductSemaphore = NULL;
        }

        m_bInit = FALSE;
        m_dwTimeout = INFINITE;
        m_bDelayForGetMsg = FALSE;
        m_cMsgQueue.Empty();
        m_cMailBoxInfo.Uninit();
        return TRUE;
    }
    void   ShowMailBoxInfo()
    {
        CVmtAutoLock cLock(m_hSemLock);
        m_cMailBoxInfo.ShowMailBoxInfo();
    }
protected:
    //以下是生产消费模型的行为接口
    BOOL32 TryBeginProduct()                             //尝试投递邮件, 即生产
    {
        return OspSemGive(m_hProductSemaphore);
    }
    BOOL32 RollBackProduct()                        //获得生产权但又生产失败, 回滚生产行为
    { //与TryProduct动作相反
        return OspSemTakeByTime(m_hProductSemaphore, 0);
    }
    BOOL32 TryEndProduct()
    {
        if ( m_bDelayForGetMsg )
        {
            return OspSemGive(m_hConsumeSemaphore);
            
        }
        else
        {
            return OspSemTakeByTime(m_hConsumeSemaphore, 0);
        }
    }

    BOOL32 TryBeginConsume()                             //尝试取出邮件, 即消费
    {
        if ( m_bDelayForGetMsg )
        {
            return OspSemTakeByTime(m_hConsumeSemaphore, m_dwTimeout);
        }
        else
        {
            return OspSemGive(m_hConsumeSemaphore);
        }
    }
    BOOL32 RollBackConsume()                        //获得消费权但又消费失败, 回滚消费行为
    { //与TryConsume动作相反
        if ( m_bDelayForGetMsg )
        {
            return OspSemGive(m_hConsumeSemaphore);
        }
        else
        {
            return OspSemTakeByTime(m_hConsumeSemaphore, 0);
        }
    }
    BOOL32 TryEndConsume()
    {
        return OspSemTakeByTime(m_hProductSemaphore, 0);
    }
    
protected:
    BOOL32      m_bInit;                //邮箱初始化哨兵
    CMsgQueue   m_cMsgQueue;            //邮箱消息载体
    SEMHANDLE   m_hConsumeSemaphore;    //用于实现消费者邮箱访问的同步信号
    SEMHANDLE   m_hProductSemaphore;    //用于实现生产者同步信号
    SEMHANDLE   m_hSemLock;             //用于实现邮箱访问的互斥信号
    u32         m_dwTimeout;            //邮箱访问超时时间
    BOOL32      m_bDelayForGetMsg;      //投递邮件加锁等待还是取出邮件加锁等待

//统计成员变量
    TMailBoxInfo m_cMailBoxInfo;
};

#endif //__MAILBOX_H__


