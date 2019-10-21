/*****************************************************************************
   文件实现功能: 数据库任务的多线程处理
   作者        : huzhiyun
   版本        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人      修改内容
   2011/11/11   1.0        huzhiyun        创建
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
    // 池中未处理任务数目超过阀值时返回失败
    bool PostTask(u16 wEventId, const COspObj *pcMsg);

    void Dump(u32 dwCode);

	void AddTransProgress(const string &strTransTag, int nTotalNum, int nFinishNum);		// 登记初始进度信息
	bool AddTransProgress(const string &strTransTag, int nFinishNum, ECMS_ERRORCODE errCode=CMS_SUCCESS);						// 更新进度信息
	TTransProgress GetTransProgress(const string &strTransTag);						// 获取进度信息
	ECMS_ERRORCODE GetTransProErr(const string &strTransTag);

private:
    static void* ProcessTaskEntry(void* parm);
    void ProcessTask(u32 dwDbOprIndex, const TDbTaskItem &tDbTaskItem);
    void CheckDbConnection(u32 dwDbOprIndex, bool bTryExecTestSql = false);
	void CheckFastdbConnection(u32 dwDbOprIndex);

    void OnTaskSipMsgTransfer(CDbOpr *pcDbOpr, const CTaskMsgTransferBase *pcMsg);
    void OnDbMsgTransfer(CDbOpr *pcDbOpr, const CDbMsgTransferBase *pcMsg);
private:
    u32 m_dwDbTaskInfoMaxNum; // 最大允许存储的任务数
    list<TDbTaskItem> m_tDbTaskInfoList;
    SEMHANDLE m_hDbTaskInfoListLock;
    SEMHANDLE m_hDbTaskInfoSemCount; 

    CDbCache m_cDbCache;
    CDbOpr *m_pcDbOpr;          // DbOpr数组
    TASKHANDLE *m_phTaskHandle; // DbOpr TaskHandle数组
    volatile bool m_bQuit;

	// 保存事务的处理进度信息
	map<string, TTransProgress> m_cTransProgress;
	map<string, ECMS_ERRORCODE> m_cTransProcErr;  //有进度业务处理错误码
    map<string, u64> m_cTransAliveData; // 上次获取进度的ticks值，请求者2分钟不获取进度认为事务超时
    u64 m_cTransTimeoutTicks;

	SEMHANDLE m_hTransProgressLock;

	

private: /* 辅助成员 */
    void Enqueue(const TDbTaskItem &tDbTaskItem);
    bool Dequeue(TDbTaskItem &tDbTaskItem); // 规定时间内未取到数据返回false
};

struct TDbTaskItem
{
    TDbTaskItem():m_wMsgId(0), m_pMsg(NULL){}
    TDbTaskItem(u16 wEventId, const COspObj *pcMsg);
    u16 m_wMsgId;           // 消息ID
    const COspObj *m_pMsg;  // 消息指针
};

#endif // _DB_TASK_POOL_H_
