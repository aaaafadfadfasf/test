#ifndef _OSP_SIP_APP_H
#define _OSP_SIP_APP_H

#include "cms/ospext/ospext.h"
#include "cms/ospsip/ospsip.h"
#include "ospsipconst.h"

class COspSipInst : public CInstExt
{
public:
    //daemon实例状态
    enum{DaemonIdle=0, DaemonWaitReg, DaemonService};

    //普通实例状态
    enum{Idle=0, Waiting, Service};

    virtual LPCSTR GetStrState() const;

public:
    COspSipInst();
    ~COspSipInst();

    //清空实例数据
    void ClearInst();

    //断链处理
    void DisProcess();

    //心跳处理
    void HeartBeat();

    //注册proxy
    void RegProxy();

    //刷新注册
    void RegProxyUpdate();

    //发送注册proxy请求
    void SendRegProxyReq();

    //发送路由订阅请求
    void SendRouteSsReq();

    //proxy断链，重置路由订阅事务
    void ResetRouteSsTask();

    //事务绑定关系的超时检测
    void CheckSipTrans();

    //消息入口
    virtual void DaemonEntry(CMessage* const pcMsg);

    //DaemonIdle状态下实例处理函数
    void OnDaemonIdle(CMessage* const pcMsg);

    //DaemonWaitReg状态下实例处理函数
    void OnDaemonWaitReg(CMessage* const pcMsg);

    //DaemonService状态下实例处理函数
    void OnDaemonService(CMessage* const pcMsg);


    //消息入口
    virtual void NormalEntry(CMessage* const pcMsg);

    //Idle状态下实例处理函数
    void OnIdle(CMessage* const pcMsg);

    //Service状态下实例处理函数
    void OnService(CMessage* const pcMsg);

    void ProcTaskMsg(CTask* pTask, CMessage* const pcMsg);

public:
    u16 m_wCurrHbCnt;

    TSipURI m_tHbDst;
    TOspTransID m_tHbSrc;
    u16 m_wHbTime;  //单位：秒
    u16 m_wHbCnt;
    string m_strDirectUAAddr;
};

typedef COspApp<COspSipInst, SIP_HB_NODE_MAX> COspSipApp;

extern COspSipApp g_cOspSipApp;

void InitOspSipMsgDesc();
#ifdef _LINUX_
void RegOspSipCmd();
#endif

#endif //#ifndef _OSP_SIP_APP_H