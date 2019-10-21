/*****************************************************************************
ģ����      : cui_server 
�ļ���      : invitetasknatswitch.h
����ļ�    : 
�ļ�ʵ�ֹ���: invitetasknatswitch.h ��������CU������������
����        : sunqiang
�汾        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2015/09/17   1.0         sunqiang        ����
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
        NOT_REQUEST_STATE = UNKNOWN_STATE + 1,      //����״̬��δ��������
        WAIT_SETUP_RSP_STATE,
        /*
        invite��������
        1��UAC����invite_req
        2��UAS����invite_rsp
        3����ʱ�Ự��δ��ȫ��������Ҫ��UAC�ٷ���invite_ack
        ���һ��ʱ���ڻ�û��ack����ײ�Э��ջ�����ٻỰ����֪ͨ�ϲ�OSP_SIP_DIALOG_TERMINATE
        �ϲ��յ�OSP_SIP_DIALOG_TERMINATE�󣬵���OspSipReleaseDlg
        4��UAC����invite_ack��UAS���Դ������Ϣ��Ҳ���Բ�����
        ����������ֽ�������Ӧ���ڴ˴���ʼ��������
        invite_ack��һ�������֪ͨ��Ϣ��Э��ջ�����UAC�κλ�Ӧ
        5����UAS�յ�invite_ack����ʾ�Ự�Ѿ�������UAS���Դ������Ϣ��Ҳ���Բ�����
        6���ڻỰ��ʼ�������κν׶Σ�UAC�����Է���Bye����ֹ�Ự
        UAS�������InviteRsp֮ǰ����ByeReq����ʵ�ǷǷ�������Э��ջ��תΪ4.3������Ӧ��
        UASֻ���ں��гɹ���InviteRsp������ܡ��Ҷϡ���Bye��������ֻ�ܾܾ���403�������InviteRsp��
        7��BYE��SIP��Ϣ��û�ж�����Ӧ��ҵ�����Ϣ��BYE��Ӧ����Э��ջ�Լ���ɣ�����Ҳû����Ϣ��
        Ϊ��ɾ��OspSip��Bye����󶨣��ϲ㻹��Ҫ��������ByeRsp

        ��CUI�յ�CMU��ByeReqʱ���ײ�Э��ջ���Լ�����ByeRsp��
        ��������OspSip�㻹����Invite�Ự�󶨹�ϵ�������ϲ㻹��Ҫ��һ��ByeRsp����OspSipɾ���󶨹�ϵ
        CUI�˵�OspSip�յ�ByeRspʱ��������kdsip�ײ�ӿڣ�ֻɾ���󶨹�ϵ
        CMU�˵�OspSip�յ�kdsip�Զ��ظ���ByeRsp��Ҳɾ���󶨹�ϵ�����ϲ�ص�OSP_SIP_BYE_RSP
        */
        WAIT_SETUP_ACK_STATE,   //��״̬û��Ӧ��
        INVITE_HOLD_STATE,
        WAIT_BYE_RSP_STATE,
    };
public:
	CInviteTaskNatSwitch(CInstExt *pcInst, CCuAgent *pcCuAgent, CSipTask *pcInviteInterActTask,s32 nPlayId);
    ~CInviteTaskNatSwitch();
public:
    //������Ҫ��д�ķ���
    virtual const char* GetObjName() const //�������صķ���
    {
        return "CInviteTaskNatSwitch";
    }
    virtual const char* GetStateName(u32 dwState) const //�������صķ���
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
    //״̬����
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
	
	// ����ͻ���û��������Ƶ����ôCUI��Ҫ������Ƶ����������޷�ת����Ƶ����
	bool HasAudioNatSwitchAddress(TSdp &tSdp);
	void AddAudioNatSwitchAddressForPlat(TSdp &tSdp);
	void DelAudioNatSwitchAddress(TSdp &tSdp);
	void ProcNatCBMsg(CMessage *const pcMsg);

	bool CuiAddNatSwitch();

	bool GetCuiNatAddrForPlat(TNetAddr &tCuiNatAddr);
	bool GetCuiNatAddrForCu(TNetAddr &tCuiNatAddr);

	bool GenSwitchChn(); //����ý���������ɽ���ͨ��
	bool IsSendVideo();
	bool IsSendAudio();
	bool IsSendData();
	bool AddCuiSwitch();         //����cu��vtdu�Ľ���
	void RemoveCuiSwitch();      //ɾ��cu��vtdu�Ľ���

	TIpAddr GetVtduIp();
	s32 GetVtduVideoRtpChn();
	s32 GetVtduVideoRtcpChn();
	s32 GetVtduAudioRtpChn();
	s32 GetVtduAudioRtcpChn();
	s32 GetVtduDataRtpChn();
	s32 GetVtduDataRtcpChn();
public:
    //����ӿ�
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

    CInviteReq m_tReq;//����CU�Ķ˿�
    CInviteRsp m_tRsp;//����VTDU�Ķ˿�
    CPlaybackProgressNtfReq m_tNtfReq;
    CXMap<int, int> m_cChildTaskList; //�Ի��������б�����vcr��������
	s32 m_nPlayId;                    //��ǰ�Ự��Ӧ��cu playid

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
        invite��������
        1��UAC����invite_req
        2��UAS����invite_rsp
        3����ʱ�Ự��δ��ȫ��������Ҫ��UAC�ٷ���invite_ack
        ���һ��ʱ���ڻ�û��ack����ײ�Э��ջ�����ٻỰ����֪ͨ�ϲ�OSP_SIP_DIALOG_TERMINATE
        �ϲ��յ�OSP_SIP_DIALOG_TERMINATE�󣬵���OspSipReleaseDlg
        4��UAC����invite_ack��UAS���Դ������Ϣ��Ҳ���Բ�����
        ����������ֽ�������Ӧ���ڴ˴���ʼ��������
        invite_ack��һ�������֪ͨ��Ϣ��Э��ջ�����UAC�κλ�Ӧ
        5����UAS�յ�invite_ack����ʾ�Ự�Ѿ�������UAS���Դ������Ϣ��Ҳ���Բ�����
        6���ڻỰ��ʼ�������κν׶Σ�UAC�����Է���Bye����ֹ�Ự
        UAS�������InviteRsp֮ǰ����ByeReq����ʵ�ǷǷ�������Э��ջ��תΪ4.3������Ӧ��
        UASֻ���ں��гɹ���InviteRsp������ܡ��Ҷϡ���Bye��������ֻ�ܾܾ���403�������InviteRsp��
        7��BYE��SIP��Ϣ��û�ж�����Ӧ��ҵ�����Ϣ��BYE��Ӧ����Э��ջ�Լ���ɣ�����Ҳû����Ϣ��
        Ϊ��ɾ��OspSip��Bye����󶨣��ϲ㻹��Ҫ��������ByeRsp

        ��CUI�յ�CMU��ByeReqʱ���ײ�Э��ջ���Լ�����ByeRsp��
        ��������OspSip�㻹����Invite�Ự�󶨹�ϵ�������ϲ㻹��Ҫ��һ��ByeRsp����OspSipɾ���󶨹�ϵ
        CUI�˵�OspSip�յ�ByeRspʱ��������kdsip�ײ�ӿڣ�ֻɾ���󶨹�ϵ
        CMU�˵�OspSip�յ�kdsip�Զ��ظ���ByeRsp��Ҳɾ���󶨹�ϵ�����ϲ�ص�OSP_SIP_BYE_RSP
        */
    enum
    {
        //NOT_3PC_REQ_STATE ,
        WAIT_SETUP_REQ_STATE = UNKNOWN_STATE + 1,      //����״̬��δ��������
        WAIT_ACK_STATE,
        INVITE_HOLD_STATE,
    };
public:
    CCallPuTaskNatSwitch(CInstExt *pcInst, const string &strCuSession,string strCallPuReq, CSipTask *pcTask);
    ~CCallPuTaskNatSwitch();
public:
    //������Ҫ��д�ķ���
    virtual const char* GetObjName() const //�������صķ���
    {
        return "CCallPuTaskNatSwitch";
    }
    virtual const char* GetStateName(u32 dwState) const //�������صķ���
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
    //״̬����
    u32 WaitSetUpReqProcMsg(CMessage *const pcMsg);
	u32 WaitSetUpReqTimerPoll();
	
    u32 WaitAckProcMsg(CMessage *const pcMsg);
    u32 WaitAckTimerPoll();

    u32 InviteHoldProcMsg(CMessage *const pcMsg);

	void ProcNatCBMsg(CMessage *const pcMsg);//NAT̽����ص���ַ�������ַ�б仯��Ҫ֪ͨVTDU ���Ľ�����ַ

	TIpAddr GetVtduIp();

	s32 GetVtduAudioRtpChn();
	s32 GetVtduAudioRtcpChn();

	//TMediaChannelKey GetVtduAudioRtpChnKey();
	//TMediaChannelKey GetVtduAudioRtcpChnKey();

	bool GetCuiNatAddr(TNetAddr &tCuiNatAddr);
	bool GetCuiNatAddrForCu(TNetAddr &tCuiNatAddr);
	bool GetCuiNatAddrForPlat(TNetAddr &tCuiNatAddr);
	bool GenSwitchChn(); //����ý���������ɽ���ͨ��
	bool AddCuiSwitch();         //����cu��vtdu�Ľ���
	void RemoveCuiSwitch();      //ɾ��cu��vtdu�Ľ���

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
    //������������DevChn��Ψһ��ʶ
    TChannel m_tCuChn;
    string m_strModuleId;
 private:
    //CUserStreamNumRestorer m_cStrmNumRestorer;
    u32 m_dwErrorCode;
    //TPuChnList m_tPuChnIDList;
};
#endif //_CUI_INVITE_TASK_NAT_SWITCH_H_
