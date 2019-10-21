/*========================================================================================
模块名    ：ospext.lib
文件名    ：ospmailbox.h
相关文件  ：xdeque.h osptool.h
实现功能  ：消息邮箱
作者      ：fanxg
版权      ：<Copyright(C) 2003-2008 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
修改记录：
日期               版本              修改人             走读人              修改记录
2012/03/01         V1.0              fanxg                                  新建文件
=========================================================================================*/

#ifndef _XMAILBOX_INCLUDED_
#define _XMAILBOX_INCLUDED_

#include "cms/container/xdeque.h"
#include "cms/ospext/osptool.h"

#define  DEFAULT_MAX_MSG_WAITING 10000
#define  MAX_MAIL_BOX_NAME_LEN   1024

struct TMailboxInfo
{
	u32 m_dwRcvedMsgNum;
	u32 m_dwSndedMsgNum;
	u32 m_dwMsgWaitingTop;
	u32 m_dwRcvDropNum;
	u32 m_dwSndDropNum;
	u32 m_dwDropNum;

	u32 m_dwMaxMsgWaiting;
	char m_szMailBoxName[MAX_MAIL_BOX_NAME_LEN];

	TMailboxInfo()
	{
		memset(this, 0, sizeof(TMailboxInfo));
	}
};

template<class CMsg>
class CXMailbox
{
protected:
	//消息队列容器
	typedef CXDeque<CMsg> CMsgQueue;

private:
	CXSemaphore m_cSem;                   //mailbox 同步信号量
	CXSemLock   m_cMutex;                 //mailbox 互斥体
	CMsgQueue m_cMsgQue;                  //mailbox 容器		
	TMailboxInfo m_tMailboxInfo;          //mailbox 统计信息

private:	
	CXMailbox(const CXMailbox&);
	void operator=(const CXMailbox&);

public:
	CXMailbox(const char* szMailboxName = "no name", u32 dwMaxMsgWaiting = DEFAULT_MAX_MSG_WAITING) : m_cSem(0, dwMaxMsgWaiting)
	{
		if (szMailboxName != NULL)
		{
			strcpy(m_tMailboxInfo.m_szMailBoxName, szMailboxName);
		}

		m_tMailboxInfo.m_dwMaxMsgWaiting = dwMaxMsgWaiting;
	}
	~CXMailbox() {}

public:
	//将消息添加到邮箱中，并唤醒阻塞线程
	bool PostMsg(const CMsg& cMsg)
    {
        /*
        //下面这段代码可以提升邮箱已满情况下访问效率
        //但可能会出现邮箱未满就丢消息的情况
        //暂时先不使用这段代码
        if (m_cMsgQue.GetSize() >= m_tMailboxInfo.m_dwMaxMsgWaiting) //邮箱已满,丢弃该消息
        {
            m_tMailboxInfo.m_dwRcvDropNum++;
            return false;
        }
        */

        CAutoLock cLock(m_cMutex);

        if (m_cMsgQue.GetSize() >= m_tMailboxInfo.m_dwMaxMsgWaiting) //邮箱已满,丢弃该消息
        {
            m_tMailboxInfo.m_dwRcvDropNum++;
            return false;
        }

        if (m_cMsgQue.PushBack(cMsg))
        {
            if (m_cSem.Give())
            {
                m_tMailboxInfo.m_dwRcvedMsgNum++;
                if (m_cMsgQue.GetSize() > m_tMailboxInfo.m_dwMsgWaitingTop)
                {
                    m_tMailboxInfo.m_dwMsgWaitingTop = m_cMsgQue.GetSize();
                }
                return true;
            }
            else
            {
                CMsg tMsg;
                m_cMsgQue.PopBack(tMsg);
            }
        }

        m_tMailboxInfo.m_dwRcvDropNum++;

        return false;
    }

	//从邮箱中获取消息，如果邮箱为空则阻塞调用线程
	bool GetMsg(CMsg& rMsg, u32 dwTimeout = INFINITE)
	{
		if (m_cSem.Take(dwTimeout))  //若邮箱为空则阻塞
		{
			CAutoLock cLock(m_cMutex);

			if (m_cMsgQue.PopFront(rMsg))
			{
				m_tMailboxInfo.m_dwSndedMsgNum++;
				return true;
			}
			else
			{
				m_tMailboxInfo.m_dwSndDropNum++;
			}
		}

		return false;
	}
	
	u32 GetSize() const
	{
		CAutoLock cLock(m_cMutex);
		return m_cMsgQue.GetSize();
	}
	
	void Empty()
	{
		CAutoLock cLock(m_cMutex);
		m_cMsgQue.Empty();
	}
	
	bool IsEmpty() const
	{
		CAutoLock cLock(m_cMutex);
		return m_cMsgQue.IsEmpty();
	}

	const char* GetName() const
	{
		CAutoLock cLock(m_cMutex);
		return m_tMailboxInfo.m_szMailBoxName;
	}
	
	void Print()
	{
		CAutoLock cLock(m_cMutex);
		OspPrintf(TRUE, FALSE, "MBname [%s], " , m_tMailboxInfo.m_szMailBoxName );
		OspPrintf(TRUE, FALSE, "curr [%u], " , m_cMsgQue.GetSize() );
		OspPrintf(TRUE, FALSE, "recv [%u], " , m_tMailboxInfo.m_dwRcvedMsgNum );
        OspPrintf(TRUE, FALSE, "recv drop[%u], " , m_tMailboxInfo.m_dwRcvDropNum );
		OspPrintf(TRUE, FALSE, "send [%u], " , m_tMailboxInfo.m_dwSndedMsgNum );
		OspPrintf(TRUE, FALSE, "send drop [%u], " , m_tMailboxInfo.m_dwSndDropNum );
		OspPrintf(TRUE, FALSE, "wait top [%u], " , m_tMailboxInfo.m_dwMsgWaitingTop );
		OspPrintf(TRUE, FALSE, "max wait [%u]\n" , m_tMailboxInfo.m_dwMaxMsgWaiting );
	}
};

#endif //_XMAILBOX_INCLUDED_