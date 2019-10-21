/*****************************************************************************
   ģ����      : cui_fcgi 
   �ļ���      : clientinst.h
   ����ļ�    : 
   �ļ�ʵ�ֹ���: ʵ��ClientInst����
   ����        : liangli
   �汾        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
   2011/10/10   1.0         liangli        ����
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
    //ʵ��״̬
    enum{Idle=0, Service};

    CClientInst();
    virtual ~CClientInst();

public:
    virtual LPCSTR GetStrState();
private:
    //��Ϣ���
    virtual void DaemonEntry(CMessage* const pcMsg);
    virtual void NormalEntry(CMessage* const pcMsg){};
    virtual BOOL32 OnUserInnerCommonMsg(CMessage* const pcMsg);

    //Idle״̬��ʵ��������
    void OnIdle(CMessage* const pcMsg);
    //Service״̬��ʵ��������
    void OnService(CMessage* const pcMsg);

    CClientTask * FindClient(const char *pClientName);
    CClientTask * FindClientBySession(const char *pClientSession);
protected:
private:
};

#endif

