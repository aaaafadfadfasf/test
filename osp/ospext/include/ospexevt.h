/*========================================================================================
模块名    ：ospext.lib
文件名    ：ospexevt.h
相关文件  ：
实现功能  ：ospext内部消息定义，上层不会直接使用
作者      ：fanxg
版权      ：<Copyright(C) 2003-2008 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
修改记录：
日期               版本              修改人             走读人              修改记录
2011/04/19         V1.0              fanxg                                  新建文件
=========================================================================================*/

#ifndef _OSP_EX_INNER_EVENT_H
#define _OSP_EX_INNER_EVENT_H

#include "cms/cms_eventid.h"
#include "cms/ospext/ospext.h"

enum
{
    OSP_EXT_INNER_EVENT_BEGIN = OSP_EXT_INNER_MSG_BEGIN,

    //设置指定APP-INST的打印级别
    OSP_EXT_EVENT_SET_LOG_LEV,

    //设置指定APP的打印级别
    OSP_EXT_EVENT_SET_APP_LOG_LEV,

    //INST中轮询事务的定时器
    OSP_EXT_EVENT_POLL_TASK,

    //打印指定APP的事务信息
    OSP_EXT_EVENT_PRINT_APP_TASK,

    //打印指定APP-INST的事务信息
    OSP_EXT_EVENT_PRINT_INST_TASK,

    //打印指定APP的轮询事务信息
    OSP_EXT_EVENT_PRINT_APP_POLL_TASK,

    //打印指定APP-INST的轮询事务信息
    OSP_EXT_EVENT_PRINT_INST_POLL_TASK,

    //打印指定APP-INST-TaskNO的事务信息
    OSP_EXT_EVENT_PRINT_ONE_TASK,

    //终止指定APP-INST-TaskNO的事务
    OSP_EXT_EVENT_KILL_TASK,

    //TRACE指定INST
    OSP_EXT_EVENT_TRACE_INST,

    //打印指定AppId的所有Inst状态
    OSP_EXT_EVENT_PRINT_INST_STATE,

    //OSPEXT中task的定时器ID
    OSP_EXT_EVENT_TASK_TIMER_ID_BEGIN = OSP_EXT_INNER_EVENT_BEGIN + 500,  //50500
    OSP_EXT_EVENT_TASK_TIMER_ID_END = USER_INNER_MSG_BEGIN - 1,           //54999


    OSP_EXT_INNER_EVENT_END = USER_INNER_MSG_BEGIN - 1,

};

#endif //#ifndef _OSP_EX_INNER_EVENT_H