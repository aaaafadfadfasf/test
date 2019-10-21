/*****************************************************************************
   模块名      : cui_server
   文件名      : cuidbg.h
   相关文件    : 
   文件实现功能: cuidbg.h 实现CUI调试命令
   作者        : liangli
   版本        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人      修改内容
   2011/11/7   1.0         liangli        创建
******************************************************************************/

#ifndef _CUI_DEBUG_H_
#define _CUI_DEBUG_H_

#include "cms/ospext/ospext.h"
#include "cms/cms_proto.h"

#define CUI_VER FORMAT_VER("cui")

enum ECuiMod
{
    CUI_APP = USER_MODULE_START,
    CUI_DATA,
    LOGIN_TASK,
    NORMAL_TASK,
    TCP_THREAD,
	REDIS_LOG
};

enum ECuiCmd
{
    CUI_INNER_COMMAND_BEGIN = USER_INNER_MSG_BEGIN + 1000,
    CUI_QUIT,
    CUI_REDISSDK_DISC_CB,
    CUI_REDISSDK_SUB_CB,
    CUI_REDISSDK_MODULE_START,
    CUI_REDISSDK_MODULE_STOP,
    CUI_REDISSDK_MODULE_RESTART,
    CUI_REDISSDK_MODULE_UNKNOWN,

    CUI_PRINT_CU_LIST,
    CUI_PRINT_TAS_LIST,
    CUI_PRINT_DOMAIN_LIST,
    CUI_PRINT_CONFIG,
    CUI_PRINT_STATUS,
    CUI_PRINT_TASK_MAP,
    CUI_PRINT_MAIL_SIZE,
    CUI_PRINT_PE_AGENT,
    CUI_PRINT_USER_DEVINFO,
    CUI_PRINT_PUDATA,
    CUI_PRINT_CUREQ,         // 打印CU的请求消息，消息体为会话字符串。 内容非空时表示打印并且只打印指定会话的，否则表示不打印
    CUI_EMULATE_SIPMSG,      // 模拟SIP消息， 消息体为TCuiEmulateSipMsg
	CUI_INNER_COMMAND_END,
	CUI_PRINT_VS_STATUS_LIST, // 打印视频源状态列表
	CUI_SET_CU_NTF_MAX_SIZE,  // CU通知消息队列最大限制
	CUI_PRINT_PTZ_LOCK_DEV_VS_LIST,	// 打印锁定ptz的视频源
	CUI_PRINT_CU_BAT_OPT_LIST, // 打印缓冲的批量操作列表
};

struct TCuiEmulateSipMsg
{
    TCuiEmulateSipMsg()
    {
        szMsgBody[0] = '\0';
    }
    u32 dwDstTaskno;
    s32 nSipErrorno;
    s32 nEvent;
    s8 szMsgBody[10240];
};

struct TUserDevData
{
	TUserDevData()
	{
		memset(szUserId, 0, sizeof(s8) * 33);
		memset(szUserId, 0, sizeof(s8) * 33);
	}
	s8 szUserId[33];
	s8 szDevUUid[33];
};

void CuiRegHelpCmd();

API void cuiquit(int nSignal = 0);

API void cuihelp();

API void cuiver();

API void logpara();

API void logall(u8 bLogAll = 0);
void liblog(u8 byLog);

API void logoff();

API void cuilogvc(u32 bLogErr = 0);

API void cuilogerr();

API void cuilogtask(u32 bLogErr = 0);

//打印所有事务
API void tasklist(LPCSTR szTaskType = NULL);

//打印指定事务信息
API void task(u32 dwTaskNO);

//根据事务号打印指定事务的子事务，等同于pnst
API void taskson(u32 dwTaskNO);

//杀死指定僵尸事务，用于故障恢复或者测试，故意使用恶心的名称避免用户随意调用
API void IWillKillZombieTask(u32 dwTaskNO);

//trace 指定事务
API void tracet(u32 dwTaskNO);

//trace 下一个新建的事务
API void tracent();

API void culist(u32 bPrintSubTask = 0);

API void cuitaskshow();
API void ts();

API void taslist(u32 bPrintUserDevList = 0);

API void domain();

API void ds();

API void hb();

API void cuicfg();

API void logparam();

API void pt(u32 dwTaskNO);

API void tt(u32 dwTaskNO);

API void cuistat();

API void taskmap();

API void mailsize();

API void pe();

API void userdevinfo(LPCSTR szUserSess, LPCSTR szDevUri = NULL);

API void pudata(LPCSTR szDevUrl);

API void toptime();

API void settoptime(u32 dwNum);

API void pcq(LPCSTR szCuSession); // 打印cu的请求消息, 会话号为空时表示不打印

API void emulate_sipmsg(u32 dwTaskNo, s32 nSipError, s32 nEvent, LPCSTR szEventContent = NULL);

API void vtdunatclear(LPCSTR szUserSess); //清空用户NAT探测包

API void getvslist();

API void setcuntfsize(s32 nCuNtfMaxSize = 0);

API void getptzlock(LPCSTR szDevId); // 打印设备下被锁定的视频源

API void getbatopt();

API void GetRedis();
#endif


