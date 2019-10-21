#ifndef _SSN_TASKBASE_H_
#define _SSN_TASKBASE_H_
#include "inner_common.h"
class CCtrlAppData;
class CQueryTask;
class CLongQueryTask;
class CUserDevGrpVidsrcQryTask;
class CDevGrpVidsrcTotalnumGetTask;

typedef string TChangeNtfItem; // string是作为消息序列化之后的结果容器

class CSsnTaskbase: public CSipTask
{
    friend class CQueryTask;
	friend class CLongQueryTask;
	friend class CUserDevGrpVidsrcQryTask;
	friend class CDevGrpVidsrcTotalnumGetTask;
public:
    CSsnTaskbase(CInstExt *pcInst);
    ~CSsnTaskbase();
    void PrintData() const;
    u32 PollChangeNotifyLst(); // 定时轮循处理，发送一条主动通知消息
    void SaveNewChangeNotify(u16 wMsgId,TChangeNtfItem &cItem);

    virtual const char* GetObjName() const{return "ssn_taskbase";}
    virtual const char* GetStateName(u32 dwState) const{return "NA";}

    CCtrlAppData& AppData();
    TSipURI GetClientSipURI() const;

    u32 DefaultProc(CMessage *const pcMsg);
protected:
    void SetClientSipURI(const TSipURI& tUri);
    const CUUID& GetSessionId() const{return m_tSessionId;}
    bool SetHb(); // 设置心跳

private:
    // 变更通知过滤器，返回true表示需要通知
    virtual bool ChangeNotifyFilter(u16 /*wMsgId*/, TChangeNtfItem &cItem);
    void ReverseDeleteQueryTaskRecord(CQueryTask* pcTask); // 查询任务销毁时反向调用此函数
	void ReverseDeleteQueryTaskRecord(CLongQueryTask* pcTask);
	void ReverseDeleteQueryTaskNoNumRecord(CUserDevGrpVidsrcQryTask* pcTask);
	void ReverseDelGetDevGrpVidSrcNumRecord(CDevGrpVidsrcTotalnumGetTask* pcTask);
    u32 DoPollChangeNotifyLst();
public:
	bool GetIsNeedDevGrpGbId(){  return m_bNeedDevGrpGbId; }
protected:
    set<ENTasResourceTypes> m_cNtfFilterTypes;  // 通知指定类型的变更
	bool	m_bNeedDevGrpGbId;
private:
    bool m_bDestroying; // 会话对象正在析构标志，析构时查询任务无需反向删除自身的管理数据

    TSipURI m_tClientUri; // 客户端模块的URI
    bool m_bHbSetResult;  // 心跳设置结果, 成功之后必须主动清除心跳
    CUUID m_tSessionId;   // 分配的会话号
    // 存在查询任务时动态通知暂缓发送，解决查询结果与动态通知的同步问题(乱序时可能得到脏数据)
    set<CQueryTask*> m_cQueryTaskSet;        // 查询任务集合
	set<CLongQueryTask*> m_cLongQueryTaskSet; //长查询任务集合
	set<CUserDevGrpVidsrcQryTask*> m_cUserDevGrpVidsrcQryTaskSet; //应答我记录集数目的查询任务集合
	set<CDevGrpVidsrcTotalnumGetTask*> m_cDevGrpVidsrcNumGetTaskSet; 
    list<TChangeNtfItem> m_cChangeNotifyLst; // 变更通知队列(包括增加，删除和修改的通知，查询的结果通知在查询任务队列)
    u32 m_dwPollCount; // poll计数器, 每收到一个变更通知应答计数器清零，每次poll到来时计数器单调加1，poll计数器到达一定数值之后就尝试进行一次发送
    u32 m_dwPollDetectCount; // poll检测定时器

private: /* 业务功能实现 */
    void OnQryReq(CMessage *const pcMsg);
    void OnChangeNtfReq(const CTaskMsgTransferBase* pcMsg); // 数据库过来的变更通知(包括增删改)
    void OnChangeNtfRsp(CMessage *const pcMsg); // 对端对变更通知的Rsp
    void HandleChangeNtfRsp(s32 nSeqNo);

    virtual u32 OnLogout(CMessage *const pcMsg);
    virtual u32 OnDisconnect(CMessage *const pcMsg);
    virtual u32 OnSipMsgProcFail(CMessage *const pcMsg); // 重载时请注意调用该实现，除非你明确自己所做的
};
#endif // _SSN_TASKBASE_H_
