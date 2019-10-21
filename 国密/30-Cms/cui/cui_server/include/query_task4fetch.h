/*  
用于查询者（比如CU）来自己获取结果的任务
因为CU与CUI 比 CUI与TAS之间的一次请求携带的数据量大的多，
CUI一般需要缓存一定数据量之后再返回给CU的。
*/

#ifndef _QUERY_TASK4FETCH_H
#define _QUERY_TASK4FETCH_H

#include "cms/ospext/ospext.h"
#include "cms/ospsip/ospsip.h"
#include "cuidata.h"
#include "culogintask.h"

class CQueryTask4Fetch: public CSipTask
{
public:
    enum
    {
        IDLE_STATE = UNKNOWN_STATE + 1,  
        SERVER_STATE
    };

    CQueryTask4Fetch(CInstExt *pcInst, CCuAgent *pcCuAgent, PTRecvNode ptRecvNode);
    ~CQueryTask4Fetch();

    virtual const char* GetObjName() const 
    {
        return "CQueryTask4Fetch";
    }
    virtual const char* GetStateName(u32 dwState) const
    {
        switch (dwState)
        {
        case IDLE_STATE:
            {
                return "Idle State";
            }
            break;
        case SERVER_STATE:
            {
                return "Server State";
            }
            break;
        default:
            break;
        }
        return "Unknown State";
    }
    virtual void InitStateMachine();

    bool PostRsp2Cu(PTRecvNode ptRecvNode); // 需要删除时返回false,否则返回true

private:
    u32 IdleProcMsg(CMessage *const pcMsg);
    u32 IdleTimerPoll();

    u32 ServerProcMsg(CMessage *const pcMsg);
    u32 ServerTimerPoll();

    CCuAgent *m_pcCuAgent; 
    PTRecvNode m_ptRecvNode;
    string m_strQueryTag;
    s32 m_nErrorCode;
    string m_strReqMsg;
    u16 m_wReqMsg;

    std::list<string> m_vecNtfReqList;
    bool m_bNtfRecvEnded;
    const s32 m_nNtfNumPerCuRsp;
};

// 找不到task返回false
bool CuFetchQueryTask(const string &QueryTag, PTRecvNode ptRecvNode, const CInstExt *pcInst);

#endif // _QUERY_TASK4FETCH_H
