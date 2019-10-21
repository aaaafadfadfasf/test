/*****************************************************************************
模块名      : cui_server 
文件名      : peinvitetasknatswitch.h
相关文件    : 
文件实现功能: peinvitetasknatswitch.h 定义所有CU常规请求事务
作者        : sunqiang
版本        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2015/09/17   1.0         sunqiang        创建
******************************************************************************/
#ifndef _CUI_PE_INVITE_TASK_NAT_SWITCH_H_
#define _CUI_PE_INVITE_TASK_NAT_SWITCH_H_

#include "cms/ospext/ospext.h"
#include "culogintask.h"
#include "normalcutask.h"
#include "cms/cmu/vtdulib.h"


class CPeInviteTaskNatSwitch : public CCallBackTask
{
public:
	enum
	{
		NOT_REQUEST_STATE = UNKNOWN_STATE + 1,      //空闲状态，未发送请求
		WAIT_SETUP_RSP_STATE,
		WAIT_SETUP_ACK_STATE,   //此状态没有应答
		INVITE_HOLD_STATE,
		WAIT_BYE_RSP_STATE,
	};
public:
	CPeInviteTaskNatSwitch(CInstExt *pcInst, const string &strCuSession, CSipTask *pcInviteInterActTask, const string &strPeUri);
	~CPeInviteTaskNatSwitch();
public:
	//子类需要重写的方法
	virtual const char* GetObjName() const //必须重载的方法
	{
		return "CPeInviteTaskNatSwitch";
	}
	virtual const char* GetStateName(u32 dwState) const //必须重载的方法
	{
		switch (dwState)
		{
		case NOT_REQUEST_STATE:
			{
				return "Not Request State";
			}
			break;
		case WAIT_SETUP_RSP_STATE:
			{
				return "Wait Setup Rsp State";
			}
			break;
		case WAIT_SETUP_ACK_STATE:
			{
				return "Wait Setup Ack State";
			}
			break;
		case INVITE_HOLD_STATE:
			{
				return "Invite Hold State";
			}
			break;
		case WAIT_BYE_RSP_STATE:
			{
				return "Wait Bye Rsp State";
			}
			break;
		default:
			break;
		}
		return "Unknown State";
	}
	void TerminateTask();
	virtual void InitStateMachine();
	virtual bool IsNotNeedPollState();
public:
	//状态函数
	u32 NotReqProcMsg(CMessage *const pcMsg);
	u32 WaitSetupRspProcMsg(CMessage *const pcMsg);
	u32 WaitSetupAckProcMsg(CMessage *const pcMsg);
	u32 WaitByeRspProcMsg(CMessage *const pcMsg);
	u32 InviteHoldProcMsg(CMessage *const pcMsg);
	u32 WaitSetupReqTimerPoll();
	u32 WaitSetupRspTimerPoll();
	u32 WaitAckTimerPoll();
	u32 WaitByeRspTimerPoll();
public:
	void NotifyStop2Cu(s32 dwErrorCode);
public:
	//对外接口
	u32 StartSendReq();
	u32 StartSendInviteReq(CInviteReq &cReq);
	u32 StartSendInviteAck();
	u32 StartSendInviteBye();
	bool TryBye(CSipTask *pcActTask);
	s32 StartVcrCtrl(const CCuVcrCtrlReq&cCuReq);
	u32 StartTask();
	CPlaybackProgressNtfReq &GetPlayBackProgressNtf();
	const CPlaybackProgressNtfReq &GetPlayBackProgressNtf() const;

	bool GetCuiNatAddrForPlat(TNetAddr &tCuiNatAddr);
	bool GetCuiNatAddrForCu(TNetAddr &tCuiNatAddr);

	void SwitchToNatAddressByNatPacket(TSdp &tSdp);
	bool SwitchToCuiNatSwitchAddress(TSdp &tSdp, const TNetAddr &tCuiNatAddr);

	void ProcNatCBMsg(CMessage *const pcMsg);

	bool CuiAddNatSwitch();

	bool GenSwitchChn(); //根据媒体描述生成交换通道
	bool IsSendVideo();
	bool IsSendAudio();
	bool IsSendData();
	bool AddCuiSwitch();         //建立cu和vtdu的交换
	void RemoveCuiSwitch();      //删除cu和vtdu的交换
	
	TIpAddr GetPeIp();
	s32 GetPeVideoRtpChn();
	s32 GetPeVideoRtcpChn();
	s32 GetPeAudioRtpChn();
	s32 GetPeAudioRtcpChn();
	s32 GetPeDataChn();
	s32 GetPeDataRtpChn();
	s32 GetPeDataRtcpChn();
public:

	void AddChildTask(int nSeqNo, int nTaskNo);
	void DelChildTask(int nSeqNo);
	CTask* FindChildTask(int nSeqNo);
    u32 GetErrorCode() const { return m_dwErrorCode; }
public:
	//TChannel& GetDevChn();

	TChannel m_peChn;
	TChannel m_cuChn;

	TSwitchSsnID m_tSwitchSessID;
	TSwitchChannel m_tVideoRtpChn;
	TSwitchChannel m_tVideoRtcpChn;
	TSwitchChannel m_tAudioRtpChn;
	TSwitchChannel m_tAudioRtcpChn;
	TSwitchChannel m_tDataRtpChn;
	TSwitchChannel m_tDataRtcpChn;
public:
	string m_strPeSession;
public:
	CSipTask *m_pcInviteInterActTask;
	TKDSIP_DIALOG_ID m_tInviteID;
	bool m_bByeWhileWaitInviteRsp;

	CInviteReq m_tReq;
	CInviteRsp m_tRsp;
	CPlaybackProgressNtfReq m_tNtfReq;
	CXMap<int, int> m_cChildTaskList; //对话内事务列表，比如vcr控制事务

private:
    CUserStreamNumRestorer m_cStrmNumRestorer;
    u32 m_dwErrorCode;
};

class CPeInviteInterActionNatSwitch : public CSipTask
{
public:
	enum
	{
		NOT_REQUEST_STATE = UNKNOWN_STATE + 1,      //空闲状态，未发送请求
		WAIT_RESPONSE_STATE,                        //已经发送请求，等待单次应答
	};
public:
	CPeInviteInterActionNatSwitch(CInstExt *pcInst, const string &strCuSession, PTRecvNode ptRecvNode);
	virtual ~CPeInviteInterActionNatSwitch();
public:
	//子类需要重写的方法
	virtual const char* GetObjName() const //必须重载的方法
	{
		return "CPeInviteInterActionNatSwitch";
	}
	virtual const char* GetStateName(u32 dwState) const //必须重载的方法
	{
		switch (dwState)
		{
		case NOT_REQUEST_STATE:
			{
				return "Not Request State";
			}
			break;
		case WAIT_RESPONSE_STATE:
			{
				return "Wait Response State";
			}
			break;
		default:
			break;
		}
		return "Unknown State";
	}
	virtual void TerminateTask();
	virtual void InitStateMachine();
public:
	//状态函数
	u32 NotReqProcMsg(CMessage *const pcMsg);
	u32 WaitRspProcMsg(CMessage *const pcMsg);
	u32 WaitReqTimerPoll();
	u32 WaitRspTimerPoll();

public:
	void WriteLog(u32 dwErrorCode, const u32 dwEvent);
public:
	template<typename CRspToCu>
	u32 SendErrorMsg2Cu(u32 dwErrorCode = ERR_CUI_OPR_TIME_OUT)
	{
		CRspToCu cRsp;
		cRsp.SetErrorCode(dwErrorCode);
		return SendRspToCu<CRspToCu>(cRsp, m_ptRecvNode);
	}
	void PostErrMsg2Cu(u32 dwErrorCode); 
public:
	CTask *GetInviteTask(s32 nPlayId);
public:
	PTRecvNode m_ptRecvNode;
	string m_strRecordPlaySession;
	s32 m_nplayId;
	string m_strCuSession;
	u32 m_dwEventID;
	TChannel m_tDevChn;
	int m_dwSeqNum;
	string m_strReq;
};


#endif //_CUI_PE_INVITE_TASK_NAT_SWITCH_H_