/*========================================================================================
ģ����    ��cmu
�ļ���    ��puregtask.h
����ļ�  ��
ʵ�ֹ���  ��pu��¼����
����      ��fanxg
��Ȩ      ��<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
�޸ļ�¼��
����               �汾              �޸���             �߶���              �޸ļ�¼
2012/01/08         V1.0              fanxg                                  �½��ļ�
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

    // �����ṩ��ǿ���豸���߽ӿ�
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
    string  m_strPuiIp;           //pu��¼ʹ�õ�pui��IP������vtdu��������ַѡ��

    //ptz���������б�keyΪ��ƵԴͨ���ţ�valueΪptz��������
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
	//״̬����
	u32 OnWaitReg(CMessage *const pcMsg);
	u32 OnWaitRegTimer();

    u32 OnWaitCfgNtf(CMessage *const pcMsg);
    u32 OnWaitCfgNtfTimer();

	u32 OnService(CMessage *const pcMsg);
	u32 OnServiceTimer();

public:
    void ReleaseResource(bool bStay = false);

public:
	//ҵ����	
	void UnregProc(CMessage *const pcMsg);    //pu��������Ķ�������
    void DisconnectPu();

	void FilterBaseConfig(TPuBaseConfig &tPuBaseConfig);			//����pui�ϱ������ݣ��������ݱ���ʹ��3as��ȡ���ģ�
	void FilterPuDevConfig(TPuDevConfig &tPuDevConfig);				//����pui�ϱ������ݣ��������ݱ���ʹ��3as��ȡ���ģ�

	void UpdateVidSrcName2Tas(const CPuVidSrcCfgList& cPuCfg);
	void UpdateVidSrcName2Tas();                                   //����pui�ϱ�����ƵԴ���Ƶ�tas���ݿ�

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

    virtual CPuiRegTask   *GetPuiRegTask();      //��ȡpu;����pui��ע������
    virtual CLoginSession *GetLoginSession(const string &strDevId);
    virtual void           ForceOffline(const string &strDevId);

    //modify by ljn 20170720
    bool IsUTF8(IN unsigned char *pBuf, IN int strLen);
    string GetUTF8String(const string& strOri);

public:
	//���ݲ���
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
	string  m_strDevId;			 //�豸Id

    CPuData         *m_pcPuData;
    TPuConfig        m_tPuConfig;       // ��֧��redis��pui��ע�����������������ʱ�ϱ���PuConfig����ע��ɹ���һ��д��redis
	map<s32, string> m_cVidSrcNameBak;	// ����ǰ���ϱ�����ƵԴ������Ϣ����������ǰ����ƵԴ����ͬ���󣬿��Խ�����Ϣͬ����3as
};

typedef CStrMap<CTask*> CPuMap;

#endif  //#ifndef _PU_REG_CMU_TASK_H