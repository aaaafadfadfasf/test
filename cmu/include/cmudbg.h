/*========================================================================================
模块名    ：cmu
文件名    ：cmudbg.h
相关文件  ：ospdbg.h
实现功能  ：cmu调试打印
作者      ：fanxg
版权      ：<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
修改记录：
日期               版本              修改人             走读人              修改记录
2011/11/15         V1.0              fanxg                                  新建文件
=========================================================================================*/

#ifndef _CMU_DBG_H
#define _CMU_DBG_H

#include "cms/ospext/ospdbg.h"
#include "cms/cmu/cmu_struct.h"
#include "cmuconf.h"

#define   MAX_CMS_URI_LEN    256

struct TPrintDevParam
{
    TPrintDevParam()
    {
        memset(this, 0, sizeof(TPrintDevParam));
    }

    s8 m_achPuId[MAX_CMS_URI_LEN + 1];
    s8 m_achDomainName[MAX_CMS_URI_LEN+1];
    u8 m_byType;
    u8 m_byState;
};

struct TPrintSwitchParam
{
    TPrintSwitchParam()
    {
        memset(this, 0, sizeof(TPrintSwitchParam));
    }

    s8 m_achDevUri[MAX_CMS_URI_LEN+1];
    u8 m_byChn;
};

struct TPrintSsParam
{
    TPrintSsParam()
    {
        memset(this, 0, sizeof(TPrintSsParam));
    }

    s8 m_szLoginSess[MAX_CMS_URI_LEN+1];
    s8 m_szDevUri[MAX_CMS_URI_LEN+1];
};

struct TPrintDomainPuList
{
	TPrintDomainPuList()
	{
		byFlag = 0;
		memset(szDomainName, 0, sizeof(szDomainName));
	}

	u8 byFlag;
	s8 szDomainName[MAX_CMS_URI_LEN+1];
};

struct TDomainSyncParam
{
    TDomainSyncParam()
    {
        memset(szDomainName, 0, sizeof(szDomainName));
        dwSyncNumOnce = DEFAULT_REDIS_DOMAIN_SYNC_NUM_ONCE;
    }

    s8 szDomainName[MAX_CMS_URI_LEN+1];
    u32 dwSyncNumOnce;
};


//cmu帮助命令
API void cmuhelp();

//cmu版本信息
API void cmuver();

//cmu配置信息
API void cmucfg();

//redissdk信息
API void redissdk();

//cmu状态信息
API void cmustat();

//显示log参数信息
API void logpara();

//设置log级别
API void loglev(u8 byLev);

//设置corelog级别
API void corelog(u8 byLev);

//设置puilog级别
API void puilog(u8 byLev);

//设置cuilog级别
API void cuilog(u8 byLev);

//设置pulog级别
API void pulog(u8 byLev);

//设置culog级别
API void culog(u8 byLev);

//设置3aslog级别
API void taslog(u8 byLev);

//设置uaslog级别
API void uaslog(u8 byLev);

//设置nmslog级别
API void nmslog(u8 byLev);

//设置异域cmulog级别
API void domainlog(u8 byLev);

//设置订阅log级别
API void sslog(u8 byLev);

//设置vtdulog级别
API void vtdulog(u8 byLev);

//设置交换log级别
API void switchlog(u8 byLev);

//设置redis日志级别
API void redislog(u8 byLev);

//同时设置所有模块log级别
API void logall(u8 byLev);

//关闭所有模块log级别
API void logoff();

//打开调试模式
API void debug(bool bVal);

//打印所有事务
API void tasklist(LPCSTR szTaskType = NULL);

//打印指定事务信息
API void task(u32 dwTaskNO);

//根据事务号打印指定事务的子事务，等同于pnst
API void taskson(u32 dwTaskNO);

//杀死指定僵尸事务，用于故障恢复或者测试，故意使用恶心的名称避免用户随意调用
API void IWillKillZombieTask(u32 dwTaskNO);

//根据事务号打印指定事务的子事务
API void pnst(u32 dwTaskNO);

//根据sesskon号打印指定事务的子事务
API void psst(LPCSTR szSession);

//trace 指定事务
API void tracet(u32 dwTaskNO);

//trace 下一个新建的事务
API void tracent();

//在线的pui列表
API void puilist(LPCSTR szPuiType);

//pu信息统计
API void sumpu(LPCSTR szDomainName);

//pu列表
API void pulist(LPCSTR szDomainName, u8 byState);

//打印指定pu信息
API void pu(LPCSTR szDevUri);

//domain信息列表
API void domainlist();

//调整域同步参数
API void syncparam(LPCSTR szDomainName, u32 dwSyncNumOnce);

//{{{{{{{{{{{{{{{ add by Royan 20140513 智能告警主机
//1.0puid和2.0puUri映射表
API void map12list();
//}}}}}}}}}}}}}}} add by Royan 20140513 智能告警主机

//domain数据
API void domain(LPCSTR szDomainName);

//连接的cmu列表
API void cmulist();

//连接的vtdu列表
API void vtdulist();

//在线的cui列表
API void cuilist();

//在线的cu列表
API void culist();

//在线的外设列表
API void pelist();

//本域设备订阅事务列表
API void sslist();

//异域设备订阅事务列表
API void dsslist();

//redis订阅事务列表
API void rsslist();

//查找设备的订阅者列表
API void sserlist(LPCSTR szDevUri);

//SwitchNode列表,DevChn格式："7d53960277f341858f00b043fb8636a3@china_0"
API void switchlist(const char* szDevChn=NULL);

API void pusl(const char* szDevChn);

API void cusl(const char* szSession);

//add domain table
API void adt(LPCSTR szPeerDomainName, LPCSTR szPeerIP, LPCSTR szLocalIP, LPCSTR szOpType);
//print domain table
API void pdt();
//set display area info
API void area(u8 byCfg);

//nms相关
//打印断链告警列表
API void nmsdalist();

//打印过载告警列表
API void nmsoalist();

//Natagent帮助
API void nahelp();

//Natagent调试命令入口
API void nacmd(u16 wCmdNum);

//退出cmu
API void cmuquit();

API void SetMemoryReleaseRate(int nRange);
API void GetMemoryReleaseRate();
API void ReleaseFreeMemory();
API void SetMemroyReleaseTimer(int nSeconds);
//初始化cmu的消息描述
void InitCmuMsgDesc();

//初始化cmu内各模块日志级别
void InitCmuModuleLog();

#ifdef _LINUX_
void RegCmuCmd();
#endif


#endif  //#ifndef _CMU_DBG_H