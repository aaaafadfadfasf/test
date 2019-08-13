/*========================================================================================
模块名    ：cmustubtest
文件名    ：cmustubtestdbg.h
相关文件  ：ospdbg.h
实现功能  ：cmustubtest调试打印
作者      ：fanxg
版权      ：<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
修改记录：
日期               版本              修改人             走读人              修改记录
2011/11/15         V1.0              fanxg                                  新建文件
=========================================================================================*/

#ifndef _CMU_STUB_TEST_DBG_H
#define _CMU_STUB_TEST_DBG_H

#include "cms/ospext/ospdbg.h"

//cmu帮助命令
API void cmuhelp();

//cmu版本信息
API void cmuver();

//cmu配置信息
API void cmucfg();

//cmu状态信息
API void cmustat();

//显示log参数信息
API void logpara();

//设置log级别
API void loglev(u8 byLev);

//同时设置所有模块log级别
API void logall(u8 byLev);

//关闭所有模块log级别
API void logoff();

//打印所有事务
API void tasklist(LPCSTR szTaskType = NULL);

//打印指定事务信息
API void task(u32 dwTaskNO);

//打印所有事务
API void pat();

//打印指定事务信息
API void pt(u16 wTaskNO);

//trace 指定事务
API void tracet(u16 wTaskNO);

//trace 下一个新建的事务
API void tracent();

//在线的pui列表
API void puilist();

//在线的pu列表
API void pulist(u8 byFlag);

//连接的cmu列表
API void cmulist();

//在线的cui列表
API void cuilist();

//在线的cu列表
API void culist();

//cui登录
API void cuion();

//cu登录
API void cuon(u32 dwCuNum = 0);

//cui退出
API void cuioff();

//cu退出
API void cuoff(u32 dwTask);

//订阅设备状态
API void ssall(u32 dwInterver = 0);
API void ss(const char* szPuUri = NULL);

//取消订阅设备状态
API void unss(u32 dwTask);

//PTZ控制
API void ptz(const char* szPuUri = NULL);

//前端图片抓拍、下载
API void snap(const char* szPuUri = NULL);

//获取视频编码参数
API void getvp(const char* szPuUri = NULL);

//设置视频编码参数
API void setvp(const char* szPuUri = NULL);

//获取前端系统参数
API void getsp(const char* szPuUri = NULL);

//设置前端系统参数
API void setsp(const char* szPuUri = NULL);

//视频浏览
API void invite(const char* szPuUri=NULL, s32 nPuChn=0, s32 nCuChn=0);
API void byeinvite(u32 dwTask = 0);

API void ivtnum(u32 dwIvtNum, u32 dwInterval = 0);
API void byenum(u32 dwbyeNum, u32 dwInterval = 0);

API void dumploglev(u32 dwLev);
API void dumptrace(u32 dwTaskNO, u32 dwMediaType = 0);
API void dumptraceport(u16 dwRecvPort);
API void dumpdelport(u16 dwRecvPort);
API void dumptraceclear();
API void dumpstat(u16 dwRecvPort);



//长期第三方交换
API void invite3pc(const char* szSrcUri, s32 nSrcChn, const char* szDstUri, s32 nDstChn);

//开始第三方交换
API void start3pc(const char* szSrcUri, s32 nSrcChn, const char* szDstUri, s32 nDstChn);

//停止第三方交换
API void stop3pc(const char* szSrcUri, s32 nSrcChn, const char* szDstUri, s32 nDstChn);

//登录als
API void onals();

//设置前端告警联动
API void setal(const char* szPuUri = NULL);

//获取前端告警联动
API void getal(const char* szPuUri = NULL);

//查询前端告警联动
API void qryal(const char* szPuUri = NULL);


//退出cmu
API void cmuquit();

//初始化cmu的消息描述
void InitCmuMsgDesc();

#ifdef _LINUX_
void RegCmuCmd();
#endif

//
API void naloglev(u32 dwLev);

void dumplog( u32 elevel, const char * fmt, ... );


#endif  //#ifndef _CMU_STUB_TEST_DBG_H