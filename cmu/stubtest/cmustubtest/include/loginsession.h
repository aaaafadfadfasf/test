/*========================================================================================
模块名    ：cmu
文件名    ：loginsession.h
相关文件  ：
实现功能  ：所有登录事务抽象基类，包括pu、cu、pui、cui、vtdu、cmu等各种登录事务需要
            从它继承，该基类实现登录事务的子事务的统一管理
作者      ：fanxg
版权      ：<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
修改记录：
日期               版本              修改人             走读人              修改记录
2012/02/06         V1.0              fanxg                                  新建文件
=========================================================================================*/

#ifndef _LOGIN_SESSION_H
#define _LOGIN_SESSION_H

#include "cms/ospsip/ospsip.h"
#include "cms/container/xset.h"

class CLoginSession : public CSipTask
{
public:
    CLoginSession(CInstExt *pcInst);
    virtual ~CLoginSession();


public:
    void AddTask(CTask* pTask);

    void DelTask(CTask* pTask);

    void DestroyChildTasks();

    bool IterateChildTasks(Iterator& rNextPosition, CTask*& pTask) const;

public:
    //业务处理    
public:
    //数据操作
    const string& GetSession() const
    {
        return m_strSession;
    }
    string& GetSession()
    {
        return m_strSession;
    }
    void SetSession(const string& strSession)
    {
        m_strSession = strSession;
    }

private:
    string m_strSession;        //登录session
    CXSet<CTask*> m_tTaskList;  //登录session当前存在的Task列表
};

#endif  //#ifndef _LOGIN_SESSION_H