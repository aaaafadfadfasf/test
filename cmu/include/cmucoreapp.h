/*========================================================================================
模块名    ：cmu
文件名    ：cmucoreapp.h
相关文件  ：ospext.h cmuconst.h
实现功能  ：cmu核心处理APP
作者      ：fanxg
版权      ：<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
修改记录：
日期               版本              修改人             走读人              修改记录
2011/11/15         V1.0              fanxg                                  新建文件
=========================================================================================*/

#ifndef _CMU_CORE_APP_H
#define _CMU_CORE_APP_H

#include "cms/ospext/ospext.h"
#include "cms/ospsip/ospsip.h"
#include "cmuconst.h"
#include "sdk/fastdbsdk/fastdb.h"
#include "uaslogtask.h"

class CLoginSession;

class CFlowCtrlMngTask;

class CUasLogTask;

struct TDeviceLogInfo;

struct TAlarmLogInfo;

struct TOperateLogInfo;

struct TNetAddr;

struct TNmsAlarm;

class CPuData;

class CRedisPipe;
class CRedisSsChannel;
class CTasDevChangeNtf;
class CTasDevVideoNameNtf;
struct CTasDomainChangeNtf;

static const u32 s_dwMaxPrintPuNum = 300000;


class CCmuCoreInst : public CInstExt
{
public:
    //实例状态
    enum{Idle=0, RegProxy, Reg3as, Service};
public:
    CCmuCoreInst();
    virtual ~CCmuCoreInst();

    void ClearInst();
    void DisProcess();

    //消息入口
    virtual void DaemonEntry(CMessage* const pcMsg);

    //上层的无状态消息处理，比如打印消息
    virtual BOOL32 OnUserInnerCommonMsg(CMessage* const pcMsg);
    
    //Idle状态下实例处理函数
    void OnIdle(CMessage* const pcMsg);

    //RegProxy状态下实例处理函数
    void OnRegProxy(CMessage* const pcMsg);

    //Reg3as状态下实例处理函数
    void OnReg3as(CMessage* const pcMsg);

    //Service状态下实例处理函数
    void OnService(CMessage* const pcMsg);

    //cmu注册Proxy
    void RegSipProxy(s32 nExpires = 3600);

    //cmu注册Tas
    void CmuRegTas();

    //cmu登录外设服务器
    void CmuLoginServer(const string& strServerUri);

    //Tas断链处理
    void TasDisProc(CMessage* const pcMsg);

    //proxy断链处理
    void ProxyDisProc();

    //proxy上线处理
    void ProxyConnectProc();

    //事务创建
    virtual CTask* CreateTask(u16 wEvent);

    //事务消息处理，统一处理事务的返回结果
    void ProcTaskMsg(CTask* pTask, CMessage* const pcMsg);

    //向UAS写日志
 //   void WriteLogToUas(const TDeviceLogInfo& tLogInfo);
 //   void WriteLogToUas(const TAlarmLogInfo& tLogInfo);
	//void WriteLogToUas(const TOperateLogInfo& tLogInfo);

    //获取sipproxy日志信息
    TDeviceLogInfo GetSipproxyLogInfo(bool bOnOffState);

    //更新设备stun地址
    void UpdateDevStunAddr(const TSipURI& tDevUri);
    void UpdateDevListStunAddr(const CStrMap<CLoginSession*>& tDevList);

	//cu设置和获取是否同步前端的视频源名称到平台
	void ProcSwitchConfigSetReq(CMessage* const pcMsg);
	void ProcSwitchConfigGetReq(CMessage* const pcMsg);
	void SyncVidSrcAlias2Plat();

    //向nms发送告警
    void SendAlarmToNms(const TNmsAlarm& tAlarm);

    //域断链通知
    void BroadcastCmuDisNtf(const string& strDiscCmuUri);
    void SendCmuDisNtfToDev(const string& strDiscCmuUri, const TSipURI& tDevUri);
    void SendCmuDisNtfToDevList(const string& strDiscCmuUri, const CStrMap<CLoginSession*>& tDevList);

	//{{{{{{{{{{{{{{{ add by Royan 20140507 智能告警主机
	BOOL32 IsSipUri(const string& strDevUri);
	BOOL32 GetLocalDomainSession(OUT string& strSess, IN const COspSipMsg *pSipMsg);
	void   ProcDomainAlarm(IN const string& strDomain, IN const COspSipMsg *pSipMsg);
	//}}}}}}}}}}}}}}} add by Royan 20140507 智能告警主机

	bool GetKdsipResInfo(OUT TKDSIP_DIALOG_RESOURCE& tInfo, IN EKDSIP_DIALOG_TYPE eType);

	void AddFlowCtrlJob(u32 dwTaskNO);
	void DelFlowCtrlJob(u32 dwTaskNO);

	void ProcDomainTableNtf(CMessage* const pcMsg);

    void ProcNotify(const CRedisPipe *pcRedisPipe, const string &strChn, const string &strData);
    void ProcRdisSdkDisc(const CRedisPipe *pcRedisPipe, EFDB_StatusCode emCode);
    void ProcRdisSdkNotify(const CRedisPipe *pcRedisPipe, const CRedisSsChannel &cSsChn, const string &strContent);

	CUasLogTask* GetUasLogTask();
	TDeviceLogInfo GetDevLog(CPuData *pcPuData);

protected:
    void ProcDeviceNotify(const CTasDevChangeNtf &cNtf);                  // 设备变更处理
    void ProcDomainNotify(const CTasDomainChangeNtf &cNtf);               // 域信息变更处理
    
public:
    virtual LPCSTR GetStrState() const;

private:
public:
	bool IsUTF8(IN unsigned char *pBuf, IN int strLen);
	string GetUTF8String(const string& strOri);
	CFlowCtrlMngTask *m_pcFlowCtrlMngTask;
};

typedef COspApp<CCmuCoreInst, 1> CCmuCoreApp;

extern CCmuCoreApp g_cCmuCoreApp;



#endif //#ifndef _CMU_CORE_APP_H