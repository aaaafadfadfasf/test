/*
功能：多个任务关注同一个URI的心跳检测。
应用场景举例：
1. 作为客户端(下级)可能同时以上下级和跨域授权两种身份登录同一个TAS而对它进行心跳检测。
2. 作为服务端(上级)可能同时接收下级TAS的登录和向同一个下级TAS进行全联通数据同步...

建立起URI与TaskNo的映射，Task主动退出时主动释放心跳检测
*/

#ifndef _HB_DETECT_MULTI_TASK_H_
#define _HB_DETECT_MULTI_TASK_H_

#include "inner_common.h"

// 增加一个创建时间字段用于防止TASK号重复时导致的问题, 注意:只有在cctrl app的TASK中使用
bool SetMultiHb(const TSipURI &tDstURI, u32 dwTaskNo, time_t tTaskCreateTm);
bool StopMultiHb(const TSipURI &tDstURI, u32 dwTaskNo, time_t tTaskCreateTm);

class CHbDetectMultiTask: public CSipTask
{
public:
    enum
    {
        enStateServer = UNKNOWN_STATE + 1
    };

    CHbDetectMultiTask(CInstExt *pcInst);
    ~CHbDetectMultiTask();
    virtual void PrintData() const;

    virtual const char* GetObjName() const{return "CHbDetectMultiTask";}
    virtual const char* GetStateName(u32 dwState) const {return "ServerState";}
    virtual void InitStateMachine();

    bool SetMultiHb(const TSipURI &tDstURI, u32 dwTaskNo, time_t tTaskCreateTm);
    bool StopMultiHb(const TSipURI &tDstURI, u32 dwTaskNo, time_t tTaskCreateTm);

private:
    u32 OnServer(CMessage *const pcMsg);

private:
    map<string, list<u32> > m_cPeerUri2TasknoList;
};

#endif // _HB_DETECT_MULTI_TASK_H_
