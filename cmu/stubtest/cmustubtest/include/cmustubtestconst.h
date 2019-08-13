/*========================================================================================
模块名    ：cmustubtest
文件名    ：cmustubtestcost.h
相关文件  ：ospext.h
实现功能  ：cmustubtest常量定义
作者      ：fanxg
版权      ：<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
修改记录：
日期               版本              修改人             走读人              修改记录
2011/11/15         V1.0              fanxg                                  新建文件
=========================================================================================*/

#ifndef _CMU_STUB_TEST_CONST_H
#define _CMU_STUB_TEST_CONST_H

#include "cms/ospext/ospext.h"

#define CMU_STUB_TEST_CORE_APP_ID   138
#define CMU_STUB_TEST_CORE_DAEMON   (MAKEIID(CMU_STUB_TEST_CORE_APP_ID, CInstance::DAEMON))

enum ECmuStubTestInnerEvent
{
    CMU_INNER_EVENT_BEGIN = USER_INNER_MSG_BEGIN,

    //打印消息, cmu所有的打印消息在CMU_PRINT_EVENT_BEGIN和CMU_PRINT_EVENT_END之间添加
    CMU_PRINT_EVENT_BEGIN,

    CMU_PRINT_CONF,
    CMU_PRINT_STAT,
    CMU_PRINT_PUI_LIST,
    CMU_PRINT_PU_LIST,
    CMU_PRINT_CUI_LIST,
    CMU_PRINT_CU_LIST,
    CMU_PRINT_VTDU_LIST,
    CMU_PRINT_CMU_LIST,

    CMU_PRINT_EVENT_END,

    //内部业务消息
    CMU_START,
    CMU_PUI_DISCONNECT,
    CMU_CUI_DISCONNECT,
    CMU_CMU_DISCONNECT,
    CMU_CMU_CONNECT_SUCCESS,
    CMU_CMU_CONNECT_FAIL,
    CMU_PU_STATUS_UPDATE,
    CMU_STUN_QRY_ADDR_NTF,
    CMU_PUNCH_RESULT_NTF,

    //定时器消息
    CMU_TASK_TIMER_TEST,
    CMU_TASK_TIMER_UPDATE,
    CMU_TASK_TIMER_KILL,

};

enum ECmuModule
{
    MOD_CUI = USER_MODULE_START,
    MOD_CU,
    MOD_SS,
    MOD_PTZ,
    MOD_ALS,
    MOD_TVS,
    MOD_SWITCH,

};

enum
{
    EDumpMediaBoth = 0,
    EDumpMediaVideo,
    EDumpMediaAudio,
};

//DUMP日志级别类型
enum 
{
    DumpLog_None                  = 0,        //关闭所有打印
    DumpLog_Error                 = 1,        //错误级别
    DumpLog_Warning               = 2,        //警告级别
    DumpLog_Info                  = 3,        //信息（普通）级别
    DumpLog_All                   = (u32)(~0),//放开所有打印
};

#define CMU_REG_TAS_INTERVAL     (60*1000)   //ms

#define CMU_REQ_RSP_TASK_TIMEOUT  (3)        //s

#define CMU_CONNECT_CMU_TIMEOUT   (3)        //s

#define CMU_SS_PU_STATUS_TIMEOUT  (3)        //s

#define LOCAL_INVITE_START_PORT 50128


#endif  //#ifndef _CMU_STUB_TEST_CONST_H