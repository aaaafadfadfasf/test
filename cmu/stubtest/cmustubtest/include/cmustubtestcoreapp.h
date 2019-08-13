/*========================================================================================
模块名    ：cmustubtest
文件名    ：cmustubtestcoreapp.h
相关文件  ：ospext.h cmuconst.h
实现功能  ：cmustubtest核心处理APP
作者      ：fanxg
版权      ：<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
修改记录：
日期               版本              修改人             走读人              修改记录
2011/11/15         V1.0              fanxg                                  新建文件
=========================================================================================*/

#ifndef _CMU_STUB_TEST_CORE_APP_H
#define _CMU_STUB_TEST_CORE_APP_H

#include "cms/ospext/ospext.h"
#include "cmustubtestconst.h"

class CLoginSession;

class CAppData
{
public:
    CXMap<u32, CTask*> m_tTestTask;
};

class CCmuStubTestCoreInst : public CInstExt
{
public:
    //实例状态
    enum{Idle=0, RegProxy, Service};
public:
    CCmuStubTestCoreInst();
    ~CCmuStubTestCoreInst();

    CAppData& GetAppData();

    void ClearInst();
    void DisProcess();

    //消息入口
    virtual void DaemonEntry(CMessage* const pcMsg);
    virtual void NormalEntry(CMessage* const pcMsg);

    //上层的无状态消息处理，比如打印消息
    virtual BOOL32 OnUserInnerCommonMsg(CMessage* const pcMsg);
    
    //Idle状态下实例处理函数
    void OnIdle(CMessage* const pcMsg);

    //RegProxy状态下实例处理函数
    void OnRegProxy(CMessage* const pcMsg);

    //Service状态下实例处理函数
    void OnService(CMessage* const pcMsg);

    //cmu注册Proxy
    void RegSipProxy(s32 nExpires = 3600);

    //proxy断链处理
    void ProxyDisProc();

    //事务创建
    virtual CTask* CreateTask(u16 wEvent);

    //获取登录session
    CLoginSession* GetLoginSession(const string& strSession);
    
public:
    virtual LPCSTR GetStrState() const;

};



typedef COspApp<CCmuStubTestCoreInst, 1, CAppData> CCmuStubTestCoreApp;

extern CCmuStubTestCoreApp g_cCmuStubTestCoreApp;



#endif //#ifndef _CMU_STUB_TEST_CORE_APP_H