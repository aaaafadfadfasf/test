/*========================================================================================
ģ����    ��cmustubtest
�ļ���    ��cmustubtestcoreapp.h
����ļ�  ��ospext.h cmuconst.h
ʵ�ֹ���  ��cmustubtest���Ĵ���APP
����      ��fanxg
��Ȩ      ��<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
�޸ļ�¼��
����               �汾              �޸���             �߶���              �޸ļ�¼
2011/11/15         V1.0              fanxg                                  �½��ļ�
=========================================================================================*/

#ifndef _CMU_STUB_TEST_CORE_APP_H
#define _CMU_STUB_TEST_CORE_APP_H

#include "cms/ospext/ospext.h"
#include "cmustubtestconst.h"

class CLoginSession;

class CAppData
{
public:
    CXMap<u32, CTask*> m_tTestTask;
};

class CCmuStubTestCoreInst : public CInstExt
{
public:
    //ʵ��״̬
    enum{Idle=0, RegProxy, Service};
public:
    CCmuStubTestCoreInst();
    ~CCmuStubTestCoreInst();

    CAppData& GetAppData();

    void ClearInst();
    void DisProcess();

    //��Ϣ���
    virtual void DaemonEntry(CMessage* const pcMsg);
    virtual void NormalEntry(CMessage* const pcMsg);

    //�ϲ����״̬��Ϣ���������ӡ��Ϣ
    virtual BOOL32 OnUserInnerCommonMsg(CMessage* const pcMsg);
    
    //Idle״̬��ʵ��������
    void OnIdle(CMessage* const pcMsg);

    //RegProxy״̬��ʵ��������
    void OnRegProxy(CMessage* const pcMsg);

    //Service״̬��ʵ��������
    void OnService(CMessage* const pcMsg);

    //cmuע��Proxy
    void RegSipProxy(s32 nExpires = 3600);

    //proxy��������
    void ProxyDisProc();

    //���񴴽�
    virtual CTask* CreateTask(u16 wEvent);

    //��ȡ��¼session
    CLoginSession* GetLoginSession(const string& strSession);
    
public:
    virtual LPCSTR GetStrState() const;

};



typedef COspApp<CCmuStubTestCoreInst, 1, CAppData> CCmuStubTestCoreApp;

extern CCmuStubTestCoreApp g_cCmuStubTestCoreApp;



#endif //#ifndef _CMU_STUB_TEST_CORE_APP_H