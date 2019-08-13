/*========================================================================================
模块名    ：cmu
文件名    ：cuiregtask.h
相关文件  ：
实现功能  ：cui登录事务
作者      ：fanxg
版权      ：<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
修改记录：
日期               版本              修改人             走读人              修改记录
2012/01/08         V1.0              fanxg                                  新建文件
=========================================================================================*/

#ifndef _CUI_REG_CMU_TASK_H
#define _CUI_REG_CMU_TASK_H

#include "loginsession.h"

class CCuLoginTask;

class CCuiRegTask : public CLoginSession
{
public:
    enum
    {
        WaitReg = UNKNOWN_STATE+1,
        Service,
    };

public:
    CCuiRegTask(CInstExt *pcInst);
    virtual ~CCuiRegTask();

public:
    virtual const char* GetObjName() const
    {
        return "CCuiRegTask";
    }

    virtual const char* GetStateName(u32 dwState) const
    {
        switch (dwState)
        {
        case WaitReg:
            return "WaitReg";
        case Service:
            return "Service";
        default:
            break;
        }
        return "Unknown State";
    }

public:
    virtual void InitStateMachine();

public:
    //状态处理
    u32 OnWaitReg(CMessage *const pcMsg);
    u32 OnWaitRegTimer();

    u32 OnService(CMessage *const pcMsg);
    u32 OnServiceTimer();

public:
    //业务处理
    bool DisProc(CMessage *const pcMsg);

public:
    //数据操作
    const TSipURI GetDevUri() const
    {
        return m_tDevURI;
    }
    void SetDevUri(const TSipURI& tSipUri)
    {
        m_tDevURI = tSipUri;
    }

private:
    TSipURI m_tDevURI;
};

extern CCuiRegTask* g_pcCuiRegCmuTask;

typedef CStrMap<CLoginSession*> CCuiMap;

class CCuiList : public CCuiMap
{
public:
    void PrintData();
};
extern CCuiList g_cCuiList;




#endif  //#ifndef _CUI_REG_CMU_TASK_H