/*****************************************************************************
文件实现功能: 实现license key的获取定时更新
作者        : zhouxuan
版本        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2015/11/02  1.0         zhouxuan    创建
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
    //licenseKey类型，0为本地（USB和文件），1为网络
    enum
    {
        LICENSE_KEY_CHECK_TIME = CLicenseInst::LICENSE_KEY_CHECH_INTERVAL / 1000, //轮询时间 5s
        LICENSE_KEY_INITSDK_CHECK_TIME = CLicenseInst::LICENSE_KEY_CHECH_INTERVAL/1000 *3 //SDK_Init设置为15s，因为超时为10s
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
    //license SDK回调函数
    static void PFSDKEventCB(s32 dwMsg, s8* pchParam, void *pUser);
private:
    u32 OnIdle(CMessage *const pcMsg);
    u32 OnIdleTimerPoll();
    u32 OnApply(CMessage *const pcMsg);
    u32 OnApplyTimerPoll();
    u32 OnServer(CMessage *const pcMsg);
    u32 OnServerTimerPoll();
private:
    //获取license授权,获取成功后切换状态
    void ApplyLicenseKey();
    //更新license值
    void UpdateLicenseKey();
    bool GetLicenseValue(TLicenceInfo &tLicense, CLicenseAppData::TKeyType &tKeyType);
    //获取appdata
    CLicenseAppData &AppData();
    //回调函数处理函数
    void ProcCBMsg(CMessage *const pcMsg);
    //处理License配置的函数，会修改配置文件的
    void ProcLicenseCfgSet(CMessage *const pcMsg);
    void UpdateLicenseServerConnectState(s32 nConnectStatus);

private:
    LICENSE_SERVER_INFO m_tLicenseServerInfo;
    SEMHANDLE m_hLicSDKInfoLock ;
    REQ_PARAM m_tLicenseReqParam;
    u32 m_dwLicenseSDKInitHandle;
};




#endif