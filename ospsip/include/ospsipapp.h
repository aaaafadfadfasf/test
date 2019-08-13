#ifndef _OSP_SIP_APP_H
#define _OSP_SIP_APP_H

#include "cms/ospext/ospext.h"
#include "cms/ospsip/ospsip.h"
#include "ospsipconst.h"

class COspSipInst : public CInstExt
{
public:
    //daemonʵ��״̬
    enum{DaemonIdle=0, DaemonWaitReg, DaemonService};

    //��ͨʵ��״̬
    enum{Idle=0, Waiting, Service};

    virtual LPCSTR GetStrState() const;

public:
    COspSipInst();
    ~COspSipInst();

    //���ʵ������
    void ClearInst();

    //��������
    void DisProcess();

    //��������
    void HeartBeat();

    //ע��proxy
    void RegProxy();

    //ˢ��ע��
    void RegProxyUpdate();

    //����ע��proxy����
    void SendRegProxyReq();

    //����·�ɶ�������
    void SendRouteSsReq();

    //proxy����������·�ɶ�������
    void ResetRouteSsTask();

    //����󶨹�ϵ�ĳ�ʱ���
    void CheckSipTrans();

    //��Ϣ���
    virtual void DaemonEntry(CMessage* const pcMsg);

    //DaemonIdle״̬��ʵ��������
    void OnDaemonIdle(CMessage* const pcMsg);

    //DaemonWaitReg״̬��ʵ��������
    void OnDaemonWaitReg(CMessage* const pcMsg);

    //DaemonService״̬��ʵ��������
    void OnDaemonService(CMessage* const pcMsg);


    //��Ϣ���
    virtual void NormalEntry(CMessage* const pcMsg);

    //Idle״̬��ʵ��������
    void OnIdle(CMessage* const pcMsg);

    //Service״̬��ʵ��������
    void OnService(CMessage* const pcMsg);

    void ProcTaskMsg(CTask* pTask, CMessage* const pcMsg);

public:
    u16 m_wCurrHbCnt;

    TSipURI m_tHbDst;
    TOspTransID m_tHbSrc;
    u16 m_wHbTime;  //��λ����
    u16 m_wHbCnt;
    string m_strDirectUAAddr;
};

typedef COspApp<COspSipInst, SIP_HB_NODE_MAX> COspSipApp;

extern COspSipApp g_cOspSipApp;

void InitOspSipMsgDesc();
#ifdef _LINUX_
void RegOspSipCmd();
#endif

#endif //#ifndef _OSP_SIP_APP_H