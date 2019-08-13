/*========================================================================================
ģ����    ��cmu
�ļ���    ��cmucoreapp.h
����ļ�  ��ospext.h cmuconst.h
ʵ�ֹ���  ��cmu���Ĵ���APP
����      ��fanxg
��Ȩ      ��<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
�޸ļ�¼��
����               �汾              �޸���             �߶���              �޸ļ�¼
2011/11/15         V1.0              fanxg                                  �½��ļ�
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
    //ʵ��״̬
    enum{Idle=0, RegProxy, Reg3as, Service};
public:
    CCmuCoreInst();
    virtual ~CCmuCoreInst();

    void ClearInst();
    void DisProcess();

    //��Ϣ���
    virtual void DaemonEntry(CMessage* const pcMsg);

    //�ϲ����״̬��Ϣ���������ӡ��Ϣ
    virtual BOOL32 OnUserInnerCommonMsg(CMessage* const pcMsg);
    
    //Idle״̬��ʵ��������
    void OnIdle(CMessage* const pcMsg);

    //RegProxy״̬��ʵ��������
    void OnRegProxy(CMessage* const pcMsg);

    //Reg3as״̬��ʵ��������
    void OnReg3as(CMessage* const pcMsg);

    //Service״̬��ʵ��������
    void OnService(CMessage* const pcMsg);

    //cmuע��Proxy
    void RegSipProxy(s32 nExpires = 3600);

    //cmuע��Tas
    void CmuRegTas();

    //cmu��¼���������
    void CmuLoginServer(const string& strServerUri);

    //Tas��������
    void TasDisProc(CMessage* const pcMsg);

    //proxy��������
    void ProxyDisProc();

    //proxy���ߴ���
    void ProxyConnectProc();

    //���񴴽�
    virtual CTask* CreateTask(u16 wEvent);

    //������Ϣ����ͳһ��������ķ��ؽ��
    void ProcTaskMsg(CTask* pTask, CMessage* const pcMsg);

    //��UASд��־
 //   void WriteLogToUas(const TDeviceLogInfo& tLogInfo);
 //   void WriteLogToUas(const TAlarmLogInfo& tLogInfo);
	//void WriteLogToUas(const TOperateLogInfo& tLogInfo);

    //��ȡsipproxy��־��Ϣ
    TDeviceLogInfo GetSipproxyLogInfo(bool bOnOffState);

    //�����豸stun��ַ
    void UpdateDevStunAddr(const TSipURI& tDevUri);
    void UpdateDevListStunAddr(const CStrMap<CLoginSession*>& tDevList);

	//cu���úͻ�ȡ�Ƿ�ͬ��ǰ�˵���ƵԴ���Ƶ�ƽ̨
	void ProcSwitchConfigSetReq(CMessage* const pcMsg);
	void ProcSwitchConfigGetReq(CMessage* const pcMsg);
	void SyncVidSrcAlias2Plat();

    //��nms���͸澯
    void SendAlarmToNms(const TNmsAlarm& tAlarm);

    //�����֪ͨ
    void BroadcastCmuDisNtf(const string& strDiscCmuUri);
    void SendCmuDisNtfToDev(const string& strDiscCmuUri, const TSipURI& tDevUri);
    void SendCmuDisNtfToDevList(const string& strDiscCmuUri, const CStrMap<CLoginSession*>& tDevList);

	//{{{{{{{{{{{{{{{ add by Royan 20140507 ���ܸ澯����
	BOOL32 IsSipUri(const string& strDevUri);
	BOOL32 GetLocalDomainSession(OUT string& strSess, IN const COspSipMsg *pSipMsg);
	void   ProcDomainAlarm(IN const string& strDomain, IN const COspSipMsg *pSipMsg);
	//}}}}}}}}}}}}}}} add by Royan 20140507 ���ܸ澯����

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
    void ProcDeviceNotify(const CTasDevChangeNtf &cNtf);                  // �豸�������
    void ProcDomainNotify(const CTasDomainChangeNtf &cNtf);               // ����Ϣ�������
    
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