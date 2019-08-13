/*========================================================================================
模块名    ：cmu
文件名    ：puregtask.h
相关文件  ：
实现功能  ：pu登录事务
作者      ：fanxg
版权      ：<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
修改记录：
日期               版本              修改人             走读人              修改记录
2012/01/08         V1.0              fanxg                                  新建文件
=========================================================================================*/

#ifndef _PU_REG_CMU_TASK_H
#define _PU_REG_CMU_TASK_H

#include "cms/cmu/cmu_struct.h"
#include "cms/uas/uas_proto.h"
#include "loginsession.h"
#include "cmudata.h"
#include "flowctrltask.h"

class CPuData;
class CPuiRegTask;

class CPuRegBaseTask : public CLoginSession
{
public:
    CPuRegBaseTask(CInstExt *pcInst);
    virtual ~CPuRegBaseTask();

    CTask* GetPtzCtrlTask(const TChannel &nVidSrcChn) const;
    void SetPtzCtrlTask(const TChannel &nVidSrcChn, CTask* ptTask);
    void ErasePtzCtrlTask(const TChannel &nVidSrcChn);
    s32 GetPuLicenseNum(const vector<TDeviceCapInfo> &vctCapInfo);
    u32 ValidLicense(const string &strDevId, TCreater tCreater, s32 nLicenseNum, s32 nVidEncNum, s32 nFreeLicenseNum);
    u32 ValidLicense(const CDeviceInfo &cDevInfo, s32 nVidEncNum, s32 nFreeLicenseNum);

    virtual CLoginSession *GetLoginSession(const string &strDevId) = 0;
    virtual CPuiRegTask   *GetPuiRegTask() = 0;

    // 对外提供的强制设备下线接口
    virtual void ForceOffline(const string &strDevId) = 0;

public:
    const TSipURI& GetPuiUri() const
    {
        return m_tPuiUri;
    }
    TSipURI& GetPuiUri()
    {
        return m_tPuiUri;
    }
    void SetPuiUri(const TSipURI& tSipUri)
    {
        m_tPuiUri = tSipUri;
    }

    const string& GetPuiIp() const
    {
        return m_strPuiIp;
    }
    string& GetPuiIp()
    {
        return m_strPuiIp;
    }
    void SetPuiIp(const string& strPuiIp)
    {
        m_strPuiIp = strPuiIp;
    }

private:
    TSipURI m_tPuiUri;
    string  m_strPuiIp;           //pu登录使用的pui的IP，用于vtdu的码流地址选择

    //ptz控制事务列表，key为视频源通道号，value为ptz控制事务
    CXMap<TChannel, CTask*> m_tPuPtzCtrlTaskList;
};

class CPuRegTask : public CPuRegBaseTask, public CFlowCtrlTask
{
public:
	enum
	{
		WaitReg = UNKNOWN_STATE+1,
        WaitCfgNtf,
		Service,
	};

public:
	CPuRegTask(CInstExt *pcInst);
	virtual ~CPuRegTask();

public:
	virtual const char* GetObjName() const
	{
		return "CPuRegTask";
	}

	virtual const char* GetStateName(u32 dwState) const
	{
		switch (dwState)
		{
		case WaitReg:
			return "WaitReg";
        case WaitCfgNtf:
            return "WaitCfgNtf";
		case Service:
			return "Service";
		default:
			break;
		}
		return "Unknown State";
	}

    virtual bool IsNotNeedPollState()
    {
        return (GetState() == WaitReg || GetState() == Service);
    }

public:
	virtual void InitStateMachine();

	virtual void PrintData() const;

public:
	//状态处理
	u32 OnWaitReg(CMessage *const pcMsg);
	u32 OnWaitRegTimer();

    u32 OnWaitCfgNtf(CMessage *const pcMsg);
    u32 OnWaitCfgNtfTimer();

	u32 OnService(CMessage *const pcMsg);
	u32 OnServiceTimer();

public:
    void ReleaseResource(bool bStay = false);

public:
	//业务处理	
	void UnregProc(CMessage *const pcMsg);    //pu主动发起的断链处理
    void DisconnectPu();

	void FilterBaseConfig(TPuBaseConfig &tPuBaseConfig);			//过滤pui上报的数据（部分数据必须使用3as获取到的）
	void FilterPuDevConfig(TPuDevConfig &tPuDevConfig);				//过滤pui上报的数据（部分数据必须使用3as获取到的）

	void UpdateVidSrcName2Tas(const CPuVidSrcCfgList& cPuCfg);
	void UpdateVidSrcName2Tas();                                   //更新pui上报的视频源名称到tas数据库

    void SendPuOfflineTooBrief();
	TDeviceLogInfo GetDevLogInfoOfflineTooBrief();

    virtual TDeviceLogInfo GetDevLogInfo(bool bOnOffState);
    TAlarmLogInfo GetAlarmLogInfo(const TAlarmInput& tAlarmInput);
    void WriteAlarmLog2Uas(const TAlarmInput& tAlarmInput);

    bool IsService() const
    {
        return (GetState() == Service);
    }

    bool IsAllChnNotOnline() const;

	BOOL32 IsSipUri(const string& strDevUri);

	virtual bool StartWork();

	
	bool CheckBaseConfig(IN TPuBaseConfig& tBaseConfig, OUT vector<TDeviceCapInfo>& vctDevCapInfo);
	void UpdateAlarmAlias(TAlarmInput& tAlarmInput);
    u32  UpdateAlarm(TAlarmInput &tAlarmInput);

    virtual CPuiRegTask   *GetPuiRegTask();      //获取pu途经的pui的注册事务
    virtual CLoginSession *GetLoginSession(const string &strDevId);
    virtual void           ForceOffline(const string &strDevId);

    //modify by ljn 20170720
    bool IsUTF8(IN unsigned char *pBuf, IN int strLen);
    string GetUTF8String(const string& strOri);

public:
	//数据操作
    virtual s32 GetSupportDirectTransNum()
    {
		CMS_ASSERT(NULL != m_pcPuData);
        return m_pcPuData->GetDirectTransNum();
    }

    const string &GetDevId() const
    {
        return m_strDevId;
    }

	virtual size_t GetDevNetSegNO();
	virtual string GetJoinInterfaceIp(const string &strSession);

protected:
    void ProcOnline();
    void ProcOffline();

private:
	string  m_strDevId;			 //设备Id

    CPuData         *m_pcPuData;
    TPuConfig        m_tPuConfig;       // 不支持redis的pui在注册过程中用来保存临时上报的PuConfig，待注册成功后一次写入redis
	map<s32, string> m_cVidSrcNameBak;	// 保存前端上报的视频源别名信息，当开启了前端视频源别名同步后，可以将此信息同步到3as
};

typedef CStrMap<CTask*> CPuMap;

#endif  //#ifndef _PU_REG_CMU_TASK_H