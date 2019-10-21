/*****************************************************************************
   �ļ�ʵ�ֹ���: ���ݿ�����Ķ��̴߳���
   ����        : huzhiyun
   �汾        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
   2011/11/11   1.0        huzhiyun        ����
******************************************************************************/
#ifndef _DB_TASK_POOL_H_
#define _DB_TASK_POOL_H_
#include "dbopr/dbopr.h"
#include "dbopr/dbcache.h"
#include <list>
using std::list;
struct TDbTaskItem;


class CDbTaskPool
{
public:
    CDbTaskPool()
        :m_dwDbTaskInfoMaxNum(0), m_hDbTaskInfoListLock(NULL), m_hDbTaskInfoSemCount(NULL),
        m_pcDbOpr(NULL), m_phTaskHandle(NULL), m_bQuit(false),m_cTransTimeoutTicks(0),m_hTransProgressLock(NULL)
    {}
    ~CDbTaskPool();
public:
    bool Init();
    void Quit();
    // ����δ����������Ŀ������ֵʱ����ʧ��
    bool PostTask(u16 wEventId, const COspObj *pcMsg);

    void Dump(u32 dwCode);

	void AddTransProgress(const string &strTransTag, int nTotalNum, int nFinishNum);		// �Ǽǳ�ʼ������Ϣ
	bool AddTransProgress(const string &strTransTag, int nFinishNum, ECMS_ERRORCODE errCode=CMS_SUCCESS);						// ���½�����Ϣ
	TTransProgress GetTransProgress(const string &strTransTag);						// ��ȡ������Ϣ
	ECMS_ERRORCODE GetTransProErr(const string &strTransTag);

private:
    static void* ProcessTaskEntry(void* parm);
    void ProcessTask(u32 dwDbOprIndex, const TDbTaskItem &tDbTaskItem);
    void CheckDbConnection(u32 dwDbOprIndex, bool bTryExecTestSql = false);
	void CheckFastdbConnection(u32 dwDbOprIndex);

    void OnTaskSipMsgTransfer(CDbOpr *pcDbOpr, const CTaskMsgTransferBase *pcMsg);
    void OnDbMsgTransfer(CDbOpr *pcDbOpr, const CDbMsgTransferBase *pcMsg);
private:
    u32 m_dwDbTaskInfoMaxNum; // �������洢��������
    list<TDbTaskItem> m_tDbTaskInfoList;
    SEMHANDLE m_hDbTaskInfoListLock;
    SEMHANDLE m_hDbTaskInfoSemCount; 

    CDbCache m_cDbCache;
    CDbOpr *m_pcDbOpr;          // DbOpr����
    TASKHANDLE *m_phTaskHandle; // DbOpr TaskHandle����
    volatile bool m_bQuit;

	// ��������Ĵ��������Ϣ
	map<string, TTransProgress> m_cTransProgress;
	map<string, ECMS_ERRORCODE> m_cTransProcErr;  //�н���ҵ���������
    map<string, u64> m_cTransAliveData; // �ϴλ�ȡ���ȵ�ticksֵ��������2���Ӳ���ȡ������Ϊ����ʱ
    u64 m_cTransTimeoutTicks;

	SEMHANDLE m_hTransProgressLock;

	

private: /* ������Ա */
    void Enqueue(const TDbTaskItem &tDbTaskItem);
    bool Dequeue(TDbTaskItem &tDbTaskItem); // �涨ʱ����δȡ�����ݷ���false
};

struct TDbTaskItem
{
    TDbTaskItem():m_wMsgId(0), m_pMsg(NULL){}
    TDbTaskItem(u16 wEventId, const COspObj *pcMsg);
    u16 m_wMsgId;           // ��ϢID
    const COspObj *m_pMsg;  // ��Ϣָ��
};

#endif // _DB_TASK_POOL_H_
