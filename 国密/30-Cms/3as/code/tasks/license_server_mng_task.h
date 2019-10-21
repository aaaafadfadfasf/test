/*****************************************************************************
�ļ�ʵ�ֹ���: ʵ��license key�Ļ�ȡ��ʱ����
����        : zhouxuan
�汾        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2015/11/02  1.0         zhouxuan    ����
******************************************************************************/


#ifndef _LICENSE_SERVER_MNG_TASK_H_
#define _LICENSE_SERVER_MNG_TASK_H_
#include "inner_common.h"
#include "cbb/license/licensesdk.h"
#include "license_mng.h"

class CLicenseAppData;

class CLicenseServerMngTask : public CTask
{
public:
    //licenseKey���ͣ�0Ϊ���أ�USB���ļ�����1Ϊ����
    enum
    {
        LICENSE_KEY_CHECK_TIME = CLicenseInst::LICENSE_KEY_CHECH_INTERVAL / 1000, //��ѯʱ�� 5s
        LICENSE_KEY_INITSDK_CHECK_TIME = CLicenseInst::LICENSE_KEY_CHECH_INTERVAL/1000 *3 //SDK_Init����Ϊ15s����Ϊ��ʱΪ10s
    };
    enum
    {
        enLsKeyTypeLocal = 0,
        enLsKeyTypeNet
    };

    enum
    {
        enStateIdle = UNKNOWN_STATE+1,
        enStateApply,
        enStateServer
    };

    CLicenseServerMngTask(CInstExt *pcInst);
    ~CLicenseServerMngTask();

    virtual const char* GetObjName() const
    {
        return "CLicenseServerMngTask";
    }
    virtual const char* GetStateName(u32 dwState) const
    {
        switch (dwState)
        {
        case enStateIdle:
        return "Idle";
        case enStateApply:
        return "Apply";
        case enStateServer:
        return "Server";
        default:
        break;
        }
        return "Unknown State";
    }
    virtual void InitStateMachine();

public:
    u32 InitSDK();
    //license SDK�ص�����
    static void PFSDKEventCB(s32 dwMsg, s8* pchParam, void *pUser);
private:
    u32 OnIdle(CMessage *const pcMsg);
    u32 OnIdleTimerPoll();
    u32 OnApply(CMessage *const pcMsg);
    u32 OnApplyTimerPoll();
    u32 OnServer(CMessage *const pcMsg);
    u32 OnServerTimerPoll();
private:
    //��ȡlicense��Ȩ,��ȡ�ɹ����л�״̬
    void ApplyLicenseKey();
    //����licenseֵ
    void UpdateLicenseKey();
    bool GetLicenseValue(TLicenceInfo &tLicense, CLicenseAppData::TKeyType &tKeyType);
    //��ȡappdata
    CLicenseAppData &AppData();
    //�ص�����������
    void ProcCBMsg(CMessage *const pcMsg);
    //����License���õĺ��������޸������ļ���
    void ProcLicenseCfgSet(CMessage *const pcMsg);
    void UpdateLicenseServerConnectState(s32 nConnectStatus);

private:
    LICENSE_SERVER_INFO m_tLicenseServerInfo;
    SEMHANDLE m_hLicSDKInfoLock ;
    REQ_PARAM m_tLicenseReqParam;
    u32 m_dwLicenseSDKInitHandle;
};




#endif