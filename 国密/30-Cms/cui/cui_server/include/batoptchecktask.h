/*
1、存储所有的CU批量操作命令
2、向CMU校验是否可以进行批量操作（CMU控制批量操作顺序和数量）
3、触发、控制批量操作
*/
#ifndef _BAT_OPT_CHECK_TASK_H_
#define _BAT_OPT_CHECK_TASK_H_

#include "cms/ospext/ospext.h"
#include "cms/ospsip/ospsip.h"
#include "cuidata.h"
#include "cms/commonconfig/cmcfg.h"
#include "culogintask.h"
#include "cms/utility/digital_string.h"

/*
1、CMU登记批量操作--方案待定
*/

class CCuBatOptCheckTask : public CSipTask
{
public:
    enum
    {
        SERVICE_STATE = UNKNOWN_STATE + 1,	// 服务状态，向CMU校验批量操作是否可执行
    };

	enum EBAT_OPT_LIST
	{
		EBAT_OPT_PLAT_LIST = 0, // 对应 g_cCuiDataMgr.m_cPlatBatOptRequestList
		EBAT_OPT_PU_LIST = 1 // 对应 g_cCuiDataMgr.m_cPuBatOptRequestList
	};
public:
    CCuBatOptCheckTask(CInstExt *pcInst/*, PTRecvNode ptRecvNode*/);
    ~CCuBatOptCheckTask();

public:
    //子类需要重写的方法
    virtual const char* GetObjName() const //必须重载的方法
    {
        return "CBatOptCheckTask";
    }
    virtual const char* GetStateName(u32 dwState) const //必须重载的方法
    {
        switch (dwState)
        {
        case SERVICE_STATE:
            {
                return "Service State";
            }
            break;
        default:
            break;
        }
        return "Unknown State";
    }
	template<typename CReq>
	u32 SetCuReqMsg(CReq &cReq, CSipTask *pcTask)
	{
		CMessage cCuReqMsg;
		cCuReqMsg.event = cReq.GetEventId();
		cCuReqMsg.content = (u8*)&cReq;
		return pcTask->ProcMsg(&cCuReqMsg);
	}
    void TerminateTask();
    virtual void InitStateMachine();
public:

    template <typename CReq>
    u32 FakePostMsgReq(CReq &cReq, const TSipURI &tDstURI)
    {
        TASKLOG(NORMAL_TASK, EVENT_LEV, "CBatOptCheckTask[TaskNO %u]发送消息[%s--%d]给服务器[%s]\n", 
            GetTaskNO(), cReq.GetEventStr().c_str(), cReq.GetEventId(), tDstURI.GetURIString().c_str());
        return PostMsgReq<CReq>(cReq, tDstURI);
    }

private:
	EBAT_OPT_LIST m_enNextCheckList; // 上一次校验的批量任务列表

public:
    //状态函数
    u32 ServiceProcMsg(CMessage *const pcMsg);
	u32 ServiceTimerPoll();
	CuBatOptList &GetCheckList();
	u32 CheckBatOpt();

};

#endif // _BAT_OPT_CHECK_TASK_H_
