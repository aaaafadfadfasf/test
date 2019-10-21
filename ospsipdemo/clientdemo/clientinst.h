/*****************************************************************************
   模块名      : cui_fcgi 
   文件名      : clientinst.h
   相关文件    : 
   文件实现功能: 实现ClientInst定义
   作者        : liangli
   版本        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人      修改内容
   2011/10/10   1.0         liangli        创建
******************************************************************************/
#ifndef _CLIENT_INST_H_
#define _CLIENT_INST_H_
#include "cms/ospext/ospext.h"
#include "cms/ospsip/ospsip.h"
#include "../common/commonmsg.h"

class CClientTask;

enum INNERMSG
{
    EV_START_REG = USER_INNER_MSG_BEGIN,
    EV_STOP_REG,
    EV_SEND_MSG,
    EV_START_SS,
    EV_STOP_SS,
	EV_START_IN,
	EV_STOP_IN,
    EV_SEND_INFO,
    EN_SEND_DLG_MSG,
    EV_MSG_NUM
};

class CClientInst : public CInstExt
{
public:
    //实例状态
    enum{Idle=0, Service};

    CClientInst();
    virtual ~CClientInst();

public:
    virtual LPCSTR GetStrState();
private:
    //消息入口
    virtual void DaemonEntry(CMessage* const pcMsg);
    virtual void NormalEntry(CMessage* const pcMsg){};
    virtual BOOL32 OnUserInnerCommonMsg(CMessage* const pcMsg);

    //Idle状态下实例处理函数
    void OnIdle(CMessage* const pcMsg);
    //Service状态下实例处理函数
    void OnService(CMessage* const pcMsg);

    CClientTask * FindClient(const char *pClientName);
    CClientTask * FindClientBySession(const char *pClientSession);
protected:
private:
};

#endif

