/*
1���洢���е�CU������������
2����CMUУ���Ƿ���Խ�������������CMU������������˳���������
3��������������������
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
1��CMU�Ǽ���������--��������
*/

class CCuBatOptCheckTask : public CSipTask
{
public:
    enum
    {
        SERVICE_STATE = UNKNOWN_STATE + 1,	// ����״̬����CMUУ�����������Ƿ��ִ��
    };

	enum EBAT_OPT_LIST
	{
		EBAT_OPT_PLAT_LIST = 0, // ��Ӧ g_cCuiDataMgr.m_cPlatBatOptRequestList
		EBAT_OPT_PU_LIST = 1 // ��Ӧ g_cCuiDataMgr.m_cPuBatOptRequestList
	};
public:
    CCuBatOptCheckTask(CInstExt *pcInst/*, PTRecvNode ptRecvNode*/);
    ~CCuBatOptCheckTask();

public:
    //������Ҫ��д�ķ���
    virtual const char* GetObjName() const //�������صķ���
    {
        return "CBatOptCheckTask";
    }
    virtual const char* GetStateName(u32 dwState) const //�������صķ���
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
        TASKLOG(NORMAL_TASK, EVENT_LEV, "CBatOptCheckTask[TaskNO %u]������Ϣ[%s--%d]��������[%s]\n", 
            GetTaskNO(), cReq.GetEventStr().c_str(), cReq.GetEventId(), tDstURI.GetURIString().c_str());
        return PostMsgReq<CReq>(cReq, tDstURI);
    }

private:
	EBAT_OPT_LIST m_enNextCheckList; // ��һ��У������������б�

public:
    //״̬����
    u32 ServiceProcMsg(CMessage *const pcMsg);
	u32 ServiceTimerPoll();
	CuBatOptList &GetCheckList();
	u32 CheckBatOpt();

};

#endif // _BAT_OPT_CHECK_TASK_H_
