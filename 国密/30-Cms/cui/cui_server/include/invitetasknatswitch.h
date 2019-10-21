/*****************************************************************************
模块名      : cui_server 
文件名      : invitetasknatswitch.h
相关文件    : 
文件实现功能: invitetasknatswitch.h 定义所有CU常规请求事务
作者        : sunqiang
版本        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2015/09/17   1.0         sunqiang        创建
******************************************************************************/
#ifndef _CUI_INVITE_TASK_NAT_SWITCH_H_
#define _CUI_INVITE_TASK_NAT_SWITCH_H_

#include "cms/ospext/ospext.h"
#include "culogintask.h"
#include "normalcutask.h"
#include "cms/cmu/vtdulib.h"
#include "UserStreamNumRestorer.h"

class CInviteTaskNatSwitch :
	public CSipTask
{
public:
    enum
    {
        NOT_REQUEST_STATE = UNKNOWN_STATE + 1,      //空闲状态，未发送请求
        WAIT_SETUP_RSP_STATE,
        /*
        invite的流程是
        1，UAC发送invite_req
        2，UAS发送invite_rsp
        3，此时会话还未完全建立，需要等UAC再发送invite_ack
        如果一段时间内还没有ack，则底层协议栈会销毁会话，并通知上层OSP_SIP_DIALOG_TERMINATE
        上层收到OSP_SIP_DIALOG_TERMINATE后，调用OspSipReleaseDlg
        4，UAC发送invite_ack，UAS可以处理此消息，也可以不处理
        如果是手拉手交换，则应该在此处开始发送码流
        invite_ack是一条单向的通知消息，协议栈不会给UAC任何回应
        5，当UAS收到invite_ack，表示会话已经建立，UAS可以处理该消息，也可以不处理
        6，在会话开始建立的任何阶段，UAC都可以发送Bye来终止会话
        UAS如果是在InviteRsp之前发送ByeReq，其实是非法操作，协议栈会转为4.3错误做应答
        UAS只有在呼叫成功（InviteRsp）后才能“挂断”（Bye），否则只能拒绝（403错误码的InviteRsp）
        7，BYE是SIP消息，没有定义相应的业务层消息，BYE的应答由协议栈自己完成，并且也没有消息体
        为了删除OspSip的Bye事务绑定，上层还是要主动发送ByeRsp

        当CUI收到CMU的ByeReq时，底层协议栈就自己发送ByeRsp了
        但是由于OspSip层还存在Invite会话绑定关系，所以上层还是要调一次ByeRsp，让OspSip删除绑定关系
        CUI端的OspSip收到ByeRsp时，不调用kdsip底层接口，只删除绑定关系
        CMU端的OspSip收到kdsip自动回复的ByeRsp，也删除绑定关系并向上层回调OSP_SIP_BYE_RSP
        */
        WAIT_SETUP_ACK_STATE,   //此状态没有应答
        INVITE_HOLD_STATE,
        WAIT_BYE_RSP_STATE,
    };
public:
	CInviteTaskNatSwitch(CInstExt *pcInst, CCuAgent *pcCuAgent, CSipTask *pcInviteInterActTask,s32 nPlayId);
    ~CInviteTaskNatSwitch();
public:
    //子类需要重写的方法
    virtual const char* GetObjName() const //必须重载的方法
    {
        return "CInviteTaskNatSwitch";
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
    u32 WaitSetupRspTimerPoll();
	
    u32 WaitSetupAckProcMsg(CMessage *const pcMsg);
    u32 WaitAckTimerPoll();

    u32 WaitByeRspProcMsg(CMessage *const pcMsg);
    u32 WaitByeRspTimerPoll();

    u32 InviteHoldProcMsg(CMessage *const pcMsg);

public:
    void NotifyStop2Cu(s32 dwErrorCode);
	void SwitchToNatAddressByNatPacket(TSdp &tSdp);
	bool SwitchToCuiNatSwitchAddress(TSdp &tSdp, const TNetAddr &tCuiNatAddr);
	bool IsPlayStreamInvite(TSdp &tSdp);
	
	// 如果客户端没有请求音频，那么CUI需要请求音频，否则后续无法转发音频码流
	bool HasAudioNatSwitchAddress(TSdp &tSdp);
	void AddAudioNatSwitchAddressForPlat(TSdp &tSdp);
	void DelAudioNatSwitchAddress(TSdp &tSdp);
	void ProcNatCBMsg(CMessage *const pcMsg);

	bool CuiAddNatSwitch();

	bool GetCuiNatAddrForPlat(TNetAddr &tCuiNatAddr);
	bool GetCuiNatAddrForCu(TNetAddr &tCuiNatAddr);

	bool GenSwitchChn(); //根据媒体描述生成交换通道
	bool IsSendVideo();
	bool IsSendAudio();
	bool IsSendData();
	bool AddCuiSwitch();         //建立cu和vtdu的交换
	void RemoveCuiSwitch();      //删除cu和vtdu的交换

	TIpAddr GetVtduIp();
	s32 GetVtduVideoRtpChn();
	s32 GetVtduVideoRtcpChn();
	s32 GetVtduAudioRtpChn();
	s32 GetVtduAudioRtcpChn();
	s32 GetVtduDataRtpChn();
	s32 GetVtduDataRtcpChn();
public:
    //对外接口
	s32 StartSendInviteReqNatSwitch(CInviteReq &cReq);
    s32 StartSendInviteAckNatSwitch();
    s32 StartSendInviteByeNatSwitch();
    bool TryBye(CSipTask *pcActTask);
    s32 StartVcrCtrl(const CCuVcrCtrlReq&cCuReq);
    CPlaybackProgressNtfReq &GetPlayBackProgressNtf();
    const CPlaybackProgressNtfReq &GetPlayBackProgressNtf() const;
public:
    TChannel& GetDevChn();
public:
    void AddChildTask(int nSeqNo, int TaskNo);
    void DelChildTask(int nSeqNo);
    CTask* FindChildTask(int nSeqNo);
    u32  GetErrorCode() const { return m_dwErrorCode; }
public:
    CCuAgent *m_pcCuAgent;
    CSipTask *m_pcInviteInterActTask;
    TKDSIP_DIALOG_ID m_tInviteID;
    bool m_bByeWhileWaitInviteRsp;

    CInviteReq m_tReq;//保存CU的端口
    CInviteRsp m_tRsp;//保存VTDU的端口
    CPlaybackProgressNtfReq m_tNtfReq;
    CXMap<int, int> m_cChildTaskList; //对话内事务列表，比如vcr控制事务
	s32 m_nPlayId;                    //当前会话对应的cu playid

	//COspSipMsg m_setupRspMsg;

	TChannel m_vtduChn;
	TChannel m_cuChn;

	TSwitchSsnID m_tSwitchSessID;
	TSwitchChannel m_tVideoRtpChn;
	TSwitchChannel m_tVideoRtcpChn;
	TSwitchChannel m_tAudioRtpChn;
	TSwitchChannel m_tAudioRtcpChn;
	TSwitchChannel m_tDataRtpChn;
	TSwitchChannel m_tDataRtcpChn;

    CUserStreamNumRestorer m_cStrmNumRestorer;
    u32 m_dwErrorCode;
};

class CCallPuTaskNatSwitch : public CSipTask
{
public:
        /*
        invite的流程是
        1，UAC发送invite_req
        2，UAS发送invite_rsp
        3，此时会话还未完全建立，需要等UAC再发送invite_ack
        如果一段时间内还没有ack，则底层协议栈会销毁会话，并通知上层OSP_SIP_DIALOG_TERMINATE
        上层收到OSP_SIP_DIALOG_TERMINATE后，调用OspSipReleaseDlg
        4，UAC发送invite_ack，UAS可以处理此消息，也可以不处理
        如果是手拉手交换，则应该在此处开始发送码流
        invite_ack是一条单向的通知消息，协议栈不会给UAC任何回应
        5，当UAS收到invite_ack，表示会话已经建立，UAS可以处理该消息，也可以不处理
        6，在会话开始建立的任何阶段，UAC都可以发送Bye来终止会话
        UAS如果是在InviteRsp之前发送ByeReq，其实是非法操作，协议栈会转为4.3错误做应答
        UAS只有在呼叫成功（InviteRsp）后才能“挂断”（Bye），否则只能拒绝（403错误码的InviteRsp）
        7，BYE是SIP消息，没有定义相应的业务层消息，BYE的应答由协议栈自己完成，并且也没有消息体
        为了删除OspSip的Bye事务绑定，上层还是要主动发送ByeRsp

        当CUI收到CMU的ByeReq时，底层协议栈就自己发送ByeRsp了
        但是由于OspSip层还存在Invite会话绑定关系，所以上层还是要调一次ByeRsp，让OspSip删除绑定关系
        CUI端的OspSip收到ByeRsp时，不调用kdsip底层接口，只删除绑定关系
        CMU端的OspSip收到kdsip自动回复的ByeRsp，也删除绑定关系并向上层回调OSP_SIP_BYE_RSP
        */
    enum
    {
        //NOT_3PC_REQ_STATE ,
        WAIT_SETUP_REQ_STATE = UNKNOWN_STATE + 1,      //空闲状态，未发送请求
        WAIT_ACK_STATE,
        INVITE_HOLD_STATE,
    };
public:
    CCallPuTaskNatSwitch(CInstExt *pcInst, const string &strCuSession,string strCallPuReq, CSipTask *pcTask);
    ~CCallPuTaskNatSwitch();
public:
    //子类需要重写的方法
    virtual const char* GetObjName() const //必须重载的方法
    {
        return "CCallPuTaskNatSwitch";
    }
    virtual const char* GetStateName(u32 dwState) const //必须重载的方法
    {
        switch (dwState)
        {
        case WAIT_SETUP_REQ_STATE:
            {
                return "Wait Set Up Req State";
            }
			break;
        case WAIT_ACK_STATE:
            {
                return "Wait Ack State";
            }
            break;
        case INVITE_HOLD_STATE:
            {
                return "Invite Hold State";
            }
            break;
        default:
            break;
        }
        return "Unknown State";
    }
    virtual void TerminateTask();
    virtual void InitStateMachine();
    virtual bool IsNotNeedPollState();
public:
    //状态函数
    u32 WaitSetUpReqProcMsg(CMessage *const pcMsg);
	u32 WaitSetUpReqTimerPoll();
	
    u32 WaitAckProcMsg(CMessage *const pcMsg);
    u32 WaitAckTimerPoll();

    u32 InviteHoldProcMsg(CMessage *const pcMsg);

	void ProcNatCBMsg(CMessage *const pcMsg);//NAT探测包回调地址，如果地址有变化需要通知VTDU 更改交换地址

	TIpAddr GetVtduIp();

	s32 GetVtduAudioRtpChn();
	s32 GetVtduAudioRtcpChn();

	//TMediaChannelKey GetVtduAudioRtpChnKey();
	//TMediaChannelKey GetVtduAudioRtcpChnKey();

	bool GetCuiNatAddr(TNetAddr &tCuiNatAddr);
	bool GetCuiNatAddrForCu(TNetAddr &tCuiNatAddr);
	bool GetCuiNatAddrForPlat(TNetAddr &tCuiNatAddr);
	bool GenSwitchChn(); //根据媒体描述生成交换通道
	bool AddCuiSwitch();         //建立cu和vtdu的交换
	void RemoveCuiSwitch();      //删除cu和vtdu的交换

    u32 GetErrorCode() const { return m_dwErrorCode; }
public:
    string m_strCuSession;
	string m_strInviteReq;
	string m_strCuCallPuReq;
    CSipTask *m_pcCallPuInterActTask;

	TChannel m_puChn;
	TChannel m_vtduChn;
	TChannel m_cuChn;

	TSwitchSsnID m_tSwitchSessID;
	TSwitchChannel m_tAudioRtpChn;
	TSwitchChannel m_tAudioRtcpChn;

public:
    //CSipTask *m_pcInviteInterActTask;
	TKDSIP_TRANS_ID m_tTransId;

    TKDSIP_DIALOG_ID m_tDialogID;
    //第三方呼叫中DevChn是唯一标识
    TChannel m_tCuChn;
    string m_strModuleId;
 private:
    //CUserStreamNumRestorer m_cStrmNumRestorer;
    u32 m_dwErrorCode;
    //TPuChnList m_tPuChnIDList;
};
#endif //_CUI_INVITE_TASK_NAT_SWITCH_H_
