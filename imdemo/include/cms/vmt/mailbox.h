/*===========================================================================
  ģ �� ����mainbox
  �� �� ����mailbox.h
  ʵ�ֹ��ܣ����� CMailBox ��Ľӿ�
  ����ļ���osp.h, xqueue.h
  ��    �ߣ�����
  �汾��Ϣ��
  ===========================================================================
  �޸ļ�¼��
    ����              �汾               �޸���              �޸�����
  2009/10/05          1.0                 ����                CREATE
  ==========================================================================*/
#ifndef __MAILBOX_H__
#define __MAILBOX_H__

#include "osp.h"
#include "xqueue.h"

#define MAX_MAIL_BOX_NAME_LEN 256       //����������
#define DEFAULT_MAIL_BOX_SIZE 100       //Ĭ�������С

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
    u32 m_dwRecvdMsgNum;        //�Ѿ����յ���Ϣ����
    u32 m_dwProcMsgNum;         //�Ѿ��������Ϣ,���Ѿ��������ж�ȡ����Ϣ
    u32 m_dwMsgWaitingTop;      //�ȴ���ֵ
    u32 m_dwDropMsgNum;         //������Ϣ����
    u32 m_dwMsgQueErr;          //��Ϣ�����쳣����
    u32 m_dwMaxMsgWaiting;      //�����С

    SEMHANDLE m_hLock;          //����ͳ����Ϣ�Ļ�����
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
    BOOL32 PushMsg(T tMsg)                  //Ͷ���ʼ���������
    {
        if ( !m_bInit )
        {
            return FALSE;
        }

        //�����ж��ܷ�д��
        if ( !TryBeginProduct() )
        {
            //��������Ϣ
            m_cMailBoxInfo.DropMsgInc();
            return FALSE;
        }

        {
            //�����ٽ�����ʼд��
            CVmtAutoLock cLock(m_hSemLock);

            if ( m_cMsgQueue.Push(tMsg) )
            {
                //����ɹ�, ����ͳ����Ϣ
                m_cMailBoxInfo.RecvMsgInc();
                if ( m_cMailBoxInfo.GetMsgWaitingTop() < m_cMsgQueue.GetSize() )
                {
                    m_cMailBoxInfo.UpdateMsgWaitingTop(m_cMsgQueue.GetSize());
                }
            }
            else
            {
                //�쳣����, ������Ϣ������ʱʧ��
                OspPrintf(TRUE, FALSE, "[CMailBox::PutMsg]�쳣����, ������Ϣ������ʱʧ��\n");

                //��Ҫ��pv�����ع�
                RollBackProduct();
                m_cMailBoxInfo.MsgQueErrInc();
                return FALSE;
            }
        }

        return TryEndProduct();
    }
    BOOL32 PopMsg(T &tMsg)                 //��������ȡ���ʼ�
    {
        if ( !m_bInit )
        {
            return FALSE;
        }

        //�����ж��ܷ��ȡ
        if ( !TryBeginConsume() )
        {
            //��������Ϣ���Զ�ȡ
            return FALSE;
        }

        //�����ٽ�����ʼ��ȡ
        CVmtAutoLock cLock(m_hSemLock);

        if ( m_cMsgQueue.Pop(tMsg) )
        {
            //��ȡ�ɹ�, ����ͳ����Ϣ
            m_cMailBoxInfo.ProcessedMsgInc();
        }
        else
        {
            //�쳣����, ��ȡ��Ϣ����ʧ��
            OspPrintf(TRUE, FALSE, "[CMailBox::GetMsg]�쳣����, ����Ϣ�����ж�ȡ��Ϣʱʧ��\n");
            
            //��Ҫ��pv�����ع�
            RollBackConsume();
            m_cMailBoxInfo.MsgQueErrInc();
            return FALSE;
        }

        return TryEndConsume();
    }

    /*====================================================================
    ������      : CreateMailBox
    ����        : ��������
    �㷨ʵ��    : 
    ����ȫ�ֱ���: 
    �������˵��: u32 dwMailBoxSize         �����Դ�Ŷ��ٷ��ʼ�
                  u32 dwAccessTimeOut       ���ʳ�ʱʱ��, ������ڲ�����ʱ��, 0��ʾ����Ͷ���ʼ�����ȡ�ʼ������ӳ�
                  BOOL32 bDelayForGetMsg    Ͷ���ʼ������ȴ�����ȡ���ʼ������ȴ�, FALSE��ʾ��Ͷ���ʼ�ʱ�ӳ�
                  ˵��: ����Ӧ�ó��ϵĲ�ͬ, �����ȴ���Ҫ��Ҳ��ͬ
                  1, �е�Ϊ�˽�ʡCPU����Ҫ��������Ӧ�ʼ���Ϣ,��Ҫ����ȡ�ʼ���ʱ������ȴ��ź���
                  ����Ӧ�ó�������Ϣ����App, ����Cmu��VcSsn�߳�, ����Ϣ����ʱ���̴���, û��ʱ˯�ߵȴ�
                  2, ���е�Ӧ��Ҫ������޶ȵ�ʹ��CPU���ұ�֤��Ϣ����ʧ, �����ڱ��߳�ҵ�������֮��
                  ���ڿ���ʱ�䴦���ʼ�,ͬʱҪ���ʼ�Ͷ�����ܹ������ȴ�, ȷ������ɹ�Ͷ�ݵ����߳�
                  ����Ӧ�ó�����ý�崦��App, ����RpCtrl��Rec�߳�, ������������, ֻ��ż�������û�����
    ����ֵ˵��  : �ɹ�����TRUE,ʧ�ܷ���FALSE
    ----------------------------------------------------------------------
    �޸ļ�¼    :
    ��  ��      �汾        �޸���          �޸�����
    20100216                liangli         ����
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
        
        //��������
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
    u32    GetMailCount()                           //��ǰ�ж��ٷ��ʼ�
    {
        //�����ٽ�����ʼ��ȡ
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
    //��������������ģ�͵���Ϊ�ӿ�
    BOOL32 TryBeginProduct()                             //����Ͷ���ʼ�, ������
    {
        return OspSemGive(m_hProductSemaphore);
    }
    BOOL32 RollBackProduct()                        //�������Ȩ��������ʧ��, �ع�������Ϊ
    { //��TryProduct�����෴
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

    BOOL32 TryBeginConsume()                             //����ȡ���ʼ�, ������
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
    BOOL32 RollBackConsume()                        //�������Ȩ��������ʧ��, �ع�������Ϊ
    { //��TryConsume�����෴
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
    BOOL32      m_bInit;                //�����ʼ���ڱ�
    CMsgQueue   m_cMsgQueue;            //������Ϣ����
    SEMHANDLE   m_hConsumeSemaphore;    //����ʵ��������������ʵ�ͬ���ź�
    SEMHANDLE   m_hProductSemaphore;    //����ʵ��������ͬ���ź�
    SEMHANDLE   m_hSemLock;             //����ʵ��������ʵĻ����ź�
    u32         m_dwTimeout;            //������ʳ�ʱʱ��
    BOOL32      m_bDelayForGetMsg;      //Ͷ���ʼ������ȴ�����ȡ���ʼ������ȴ�

//ͳ�Ƴ�Ա����
    TMailBoxInfo m_cMailBoxInfo;
};

#endif //__MAILBOX_H__


