/*****************************************************************************
模块名      : cmu 
文件名      : inviteswitchtask.h
相关文件    : 
文件实现功能: 交换任务类
作者        : 黄至春
版本        : vs_phoenix  Copyright(C) 2009-2015 KDDACOM, All rights reserved.
-----------------------------------------------------------------------------
修改记录:
日  期         版本        修改人      修改内容
2011/11/04     1.0        黄至春        创建
******************************************************************************/

#ifndef _INVITE_SWITCH_TASK_H
#define _INVITE_SWITCH_TASK_H

#include "cms/ospsip/ospsip.h"
#include "cms/container/xmap.h"
#include "cms/cms_errorcode.h"

#include "cms/cmu/cmu_struct.h"
#include "cms/cmu/cmu_proto.h"
#include "cmudata.h"
#include "loginsession.h"
#include "cmutask.h"
#include "puregtask.h"


class CSwitchPin
{
protected:
    TChannel            m_tChannel;
    TSdp                m_tSdp;
    CLoginSession*		m_pDevSsnTask;
    TKDSIP_DIALOG_ID    m_tDlgID;               //invite的会话ID
    bool				m_bIsTravGAP;           //是否穿越网闸
    TSipURI             m_tToUri;

public:
    CSwitchPin()
    {
        Clear();
    }

    void Clear()
    {
        m_tChannel.Clear();
        m_tSdp.Clear();
        m_pDevSsnTask = NULL;
        m_tDlgID = INVALID_DIALOG_ID;
        m_bIsTravGAP = false;
    }

    void PrintData() const
    {
        //OspPrintf(TRUE, FALSE, "%s", ToXml().c_str());

        std::ostringstream oss;

        OspPrintf(TRUE, FALSE, "DevType: %s\n", GetDevType().c_str());
        OspPrintf(TRUE, FALSE, "m_tChannel: %s\n", m_tChannel.ToString().c_str());
		OspPrintf(TRUE, FALSE, "m_pDevSsnTask[0x%X]\n", m_pDevSsnTask);
		if (m_pDevSsnTask)
		{
			OspPrintf(TRUE, FALSE, "m_pDevSsnTask, TaskNO[%d]\n", m_pDevSsnTask->GetTaskNO());
		}
        OspPrintf(TRUE, FALSE, "m_tDlgID: %lu\n", m_tDlgID);
        OspPrintf(TRUE, FALSE, "m_bIsTravGAP: %d\n", m_bIsTravGAP);
        OspPrintf(TRUE, FALSE, "m_tToUri: %s\n", m_tToUri.GetURIString().c_str());
        //OspPrintf(TRUE, FALSE, "m_tSwtchType: %s\n", m_tSwtchType.c_str());
        OspPrintf(TRUE, FALSE, "\n");
        
        g_bDebugOn ? m_tSdp.PrintData(true) : m_tSdp.PrintData(false);
    }

    string GetDevType() const
    {
        if (m_pDevSsnTask)
        {
            return m_pDevSsnTask->GetDevType();
        }

        return DEV_TYPE_UNKNOWN;
    }

    size_t GetDevNetSegNO()
    {
        if (m_pDevSsnTask)
        {
            return m_pDevSsnTask->GetDevNetSegNO();
        }

        return 0;
    }

	string GetJoinInterfaceIp()
	{
		if (m_pDevSsnTask)
		{
            TSipURI tDevUri(m_tChannel.GetDevUri());
			GLOBELOG(EVENT_LEV, "DevUri[%s], DevSsnTask[%d]!\n",
				tDevUri.GetURIString().c_str(), m_pDevSsnTask->GetTaskNO());
			return m_pDevSsnTask->GetJoinInterfaceIp(tDevUri.GetUser());
		}
		else
		{
			GLOBELOG(WARNING_LEV, "DevSsnTask is NULL!\n");
		}

		return "";
	}

    string GetDevDesc() const
    {
        return "["+GetDevType()+"]"+ m_tChannel.ToString();
    }

public:
    //m_tChannel
    const TChannel& GetChannel() const
    {
        return m_tChannel;
    }
    TChannel& GetChannel()
    {
        return m_tChannel;
    }
    void SetChannel(const TChannel& tValue)
    {
        m_tChannel = tValue;
    }

    //m_tSdp
    const TSdp& GetSdp() const
    {
        return m_tSdp;
    }
    TSdp& GetSdp()
    {
        return m_tSdp;
    }
    void SetSdp(const TSdp& var)
    {
		string strGBRecLocation = m_tSdp.GetGBRecLocation();

        m_tSdp = var;

		if (!strGBRecLocation.empty())
		{
			m_tSdp.SetGBRecLocation(strGBRecLocation);
		}

        if (m_tSdp.GetExtInfo().find(SDP_EXT_INFO_OFFLINE_SWITCH) == m_tSdp.GetExtInfo().end())
        {
            CMS_ASSERT(m_pDevSsnTask && "The InitSwitchPin must be called firstly!");

			if (m_pDevSsnTask)
			{
				m_tSdp.SetOfflineSwitch(m_pDevSsnTask->IsEnableOfflineSwitch());
			}
        }

        if (m_tSdp.GetExtInfo().find(SDP_EXT_INFO_IS_NEED_PUNCH) == m_tSdp.GetExtInfo().end())
        {
            CMS_ASSERT(m_pDevSsnTask && "The InitSwitchPin must be called firstly!");

			if (m_pDevSsnTask)
			{
				m_tSdp.SetIsNeedPunch(m_pDevSsnTask->GetIsNeedPunch());
			}
        }

        if (m_tSdp.GetExtInfo().find(SDP_EXT_INFO_STREAM_TAG) == m_tSdp.GetExtInfo().end())
        {
            CMS_ASSERT(m_pDevSsnTask && "The InitSwitchPin must be called firstly!");

            string strManufacture = MEDIA_MANUFACTURE_KEDACOM;
            if (m_pDevSsnTask && m_pDevSsnTask->GetDevType() == DEV_TYPE_PU)
            {
                CPuData *pcPuData = CCmuData::GetInstance()->GetPuDataByUri(m_tChannel.GetDevUri());
                if (NULL != pcPuData)
                {
                    strManufacture = pcPuData->GetDevManufacturer();
                }
            }

            m_tSdp.SetStreamTag(TStreamTag(strManufacture, MEDIA_STREAM_TYPE_RTP_ES));
        }
    }

    //m_pDevSsnTask
    const CLoginSession* GetDevSsnTask() const
    {
        return m_pDevSsnTask;
    }
    CLoginSession* GetDevSsnTask()
    {
        return m_pDevSsnTask;
    }
    void SetDevSsnTask(CLoginSession* ptValue)
    {
        m_pDevSsnTask = ptValue;
    }

    //m_tDlgID
    const TKDSIP_DIALOG_ID& GetDlgID() const
    {
        return m_tDlgID;
    }
    TKDSIP_DIALOG_ID& GetDlgID()
    {
        return m_tDlgID;
    }
    void SetDlgID(const TKDSIP_DIALOG_ID& tValue)
    {
        m_tDlgID = tValue;
    }

    //m_bIsTravGAP
    void SetIsTravGAP(bool bValue)
    {
        m_bIsTravGAP = bValue;
    }
    bool GetIsTravGAP() const
    {
        return m_bIsTravGAP;
    }

    //m_tToUri
    const TSipURI& GetToUri() const
    {
        return m_tToUri;
    }
    TSipURI& GetToUri()
    {
        return m_tToUri;
    }
    void SetToUri(const TSipURI& tValue)
    {
        m_tToUri = tValue;
    }
};


///////////////////////////////////////////////////////////////////////////////////////
//CSwitchTask
///////////////////////////////////////////////////////////////////////////////////////
//这里的交换状态 区别于 Task的事物状态，使其与具体业务流程独立
enum EmSwitchState
{
    SWITCH_STATE_INIT,
    SWITCH_STATE_SWITCHING,
	SWITCH_STATE_SWITCHED
};

static LPCSTR GetSwitchStateStr(EmSwitchState emValue)
{
    switch(emValue)
    {
    case SWITCH_STATE_INIT:         return "SWITCH_STATE_INIT";
    case SWITCH_STATE_SWITCHING:    return "SWITCH_STATE_SWITCHING";
    case SWITCH_STATE_SWITCHED:     return "SWITCH_STATE_SWITCHED";
    default:                        return "SWITCH_STATE_IVALID";
    }
}

class C3PCTask;
class CSwitchTask;
typedef std::list<CSwitchTask*> CSwitchTaskList;

class CSwitchTask : public CCmuTask
{
public:
    enum ESwitchTrace
    {
        TRACE_NONE,
        TRACE_UPWARDS = 1<<0,
        TRACE_DOWNWARDS = 1<<1,
        TRACE_ALL=TRACE_UPWARDS|TRACE_DOWNWARDS,
    };

    CSwitchTask(CInstExt *pcInst);
    virtual ~CSwitchTask();

    //////////////////////////////////////////////////////////////////////////////
    //重载/覆盖函数
public:
    virtual const char* GetObjName() const
    {
        return "CSwitchTask";
    }
    virtual const char* GetStateName(u32 dwState) const
    {
        return "";
    }

    void NextState(const u32 dwState);

    void PrintData(ESwitchTrace eTrace=TRACE_NONE) const;

    virtual void PrintData() const;

    //////////////////////////////////////////////////////////////////////////////
    //数据操作
public:
    virtual EmSwitchState GetSwitchState() = 0;

	//std::string ToString() const
	//{
	//	std::ostringstream oss;
	//	oss << *this;
	//	return oss.str();
	//}

    //返回Src或Dst的Chan，不包括VTDU
    const TChannel& GetChannel() const
    {
        if(m_cReceiver.GetDevType() != DEV_TYPE_VTDU)
        { //Dst
            return m_cReceiver.GetChannel();
        }
        else
        { //Src
            return m_cSender.GetChannel();
        }
    }

    //仅从SrcSwitchTask返回VtduPin
    const CSwitchPin* GetVtduPin() const
    {
        if(m_cReceiver.GetDevType() == DEV_TYPE_VTDU)
        { //Src
            return &m_cReceiver;
        }
        else
        {
            return NULL;
        }
    }

    //仅从SrcSwitchTask返回VtduRegTask
    const CVtduRegTask* GetVtduRegTask() const
    {
        if(m_cReceiver.GetDevType() == DEV_TYPE_VTDU)
        {
            return (CVtduRegTask*)m_cReceiver.GetDevSsnTask();
        }
        else
        {
            return NULL;
        }
    }

    CVtduRegTask* GetVtduRegTask()
    {
        if(m_cReceiver.GetDevType() == DEV_TYPE_VTDU)
        {
            return (CVtduRegTask*)m_cReceiver.GetDevSsnTask();
        }
        else
        {
            return NULL;
        }
    }

    bool IsInviteReqFrom(const CSwitchPin& cSwitchPin);

    //获取指定DlgId对应的SwitchPin或者另一侧的SwitchPin，用于选择当前交换pin或者反侧交换Pin
	CSwitchPin& GetSwitchPin(TKDSIP_DIALOG_ID tDlgId, bool bMatching = true);
    CSwitchPin& GetOtherSwitchPin(const TChannel& tChannel)
    {
        if (tChannel == m_cSender.GetChannel())
        {
            return m_cReceiver;
        }
        else
        {
            return m_cSender;
        }
    }

    //初始化指定的SwitchPin，不包括Sdp
    CMS_RESULT InitSwitchPin(CSwitchPin& cSwitchPin, const TChannel& tDevChn, CLoginSession* pSessTask, 
        TKDSIP_DIALOG_ID tDlgID, const TSdpSessionName &tSessionName);
    inline CMS_RESULT InitSender(const TChannel& tDevChn, const TSdpSessionName &tSessionName, CLoginSession* pSessTask=NULL, TKDSIP_DIALOG_ID tDlgID = INVALID_DIALOG_ID)
    {
        return InitSwitchPin(m_cSender, tDevChn, pSessTask, tDlgID, tSessionName);
    }
    inline CMS_RESULT InitReceiver(const TChannel& tDevChn, const TSdpSessionName &tSessionName, CLoginSession* pSessTask=NULL, TKDSIP_DIALOG_ID tDlgID = INVALID_DIALOG_ID)
    {
        return InitSwitchPin(m_cReceiver, tDevChn, pSessTask, tDlgID, tSessionName);
    }

    bool GetIsNeedPunch() const
    {
        TASKLOG(CMU_MOD_SWITCH, EVENT_LEV, "Sender.IsNeedPunch[%s] Receiver.IsNeedPunch[%s] g_cCmuConf.m_bEnablePunch[%s].\n", 
            m_cSender.GetDevSsnTask() ? BoolStr(m_cSender.GetDevSsnTask()->GetIsNeedPunch()) : "NULL",
            m_cReceiver.GetDevSsnTask() ? BoolStr(m_cReceiver.GetDevSsnTask()->GetIsNeedPunch()) : "NULL",
            BoolStr(g_cCmuConf.m_bEnablePunch)
            );

        if (m_cSender.GetDevSsnTask() && !m_cSender.GetDevSsnTask()->GetIsNeedPunch())
        {
            return false;
        }
        else if (m_cReceiver.GetDevSsnTask() && !m_cReceiver.GetDevSsnTask()->GetIsNeedPunch())
        {
            return false;
        }

        return g_cCmuConf.m_bEnablePunch;
    }

    bool SetSdp(TSdp tSdp); 

    inline u32 GetTransChnNum() const;
    
    //从SrcSwitchTask的ReceiverSwitchPin/VTDU上获取交换信息
    s32 GetFreeDistrSwitchNum() const;
    bool HasDistrCapFor(u32 dwSwitchNumToAdd) const;

    /*
    都是对Input和OutputList操作
    */
    void AddOutput(CSwitchTask* ptValue);
    void RemoveOutput(CSwitchTask* ptValue);
    static void BreakLink(CSwitchTask* ptSrc, CSwitchTask* ptDst);
    CSwitchTask* GetTopSrc() const;

    //从当前SwitchTask的输出列表中，找到是备份源，并且找出相应类型的具有最大分发能力的结点（如备份源VTDU、MTU）
    CSwitchTask* FindBakSrcByType(const string& strDevType);

    //////////////////////////////////////////////////////////////////////////////
    //业务逻辑
public:
    //CMS_RESULT ProcInviteRsp(COspSipMsg*const pcOspSipMsg);
    bool ProcDisconnect(const TDevOnOffCBMsg* ptDevOnOffCBMsg);
    bool ProcOspSipException(CMessage *const pcMsg);
    bool ProcByeReq(CMessage *const pcMsg);
    CMS_RESULT ProcByeRsp(const COspSipMsg*const pcOspSipMsg);
    CMS_RESULT ProcEndOfFileReq(const COspSipMsg*const pcOspSipMsg);
    CMS_RESULT ProcEndOfFileRsp(const COspSipMsg*const pcOspSipMsg);
    //CMS_RESULT ProcKeyFrameReq(CMessage *const pcMsg);
    //CMS_RESULT ProcKeyFrameRsp(CMessage *const pcMsg);

	static CMS_RESULT StartSwitch(const TChannel& tDstChn, const TChannel& tSrcChn, TSdp& tSdp, CCmuTask* pcReqTask);
	static TSwitchMode CheckSwitchMode(CLoginSession* pSrcSess, const TChannel& tSrcChn, const TSdp& tSdp);
	static CMS_RESULT MakeLink(CSwitchTask* ptSrcSwitch, CSwitchTask* ptDstSwitch);

    virtual CMS_RESULT Start() = 0;

    CMS_RESULT CBInputSwitchFinish(const CSwitchTask* pInput);
    //CMS_RESULT CBOutputSwitchFinish(CSwitchTask* pOutput);

    //公共基类拥有发送InviteReq和InviteAck
    CMS_RESULT InviteTo(const CSwitchPin& cSwitchPinTo, /*const TInviteSubject tSubject,*/ const TSdp& tSdp);
	CMS_RESULT PostInviteAck(const CSwitchPin& cSwitchPinTo, const TSdp& tSdp);

    bool IsCreditDomain(const TSipURI&  tDevUri);
    bool IsCreditDomain(const string& strDevUri);

    bool IsNeedTrans(const CSwitchTask* ptSrcSwitchNode, const CSwitchTask* ptDstSwitchNode);
    CSwitchTask* ChooseMTU(const CSwitchTask* ptSrcSwitchNode, const CSwitchTask* ptDstSwitchNode);

    CMS_RESULT ChooseDistribNode(CSwitchTask* ptDstSwitchNode, CSwitchTask*& ptRtnDistribNode);
    CMS_RESULT ChooseUseableSrcNode(const TSdpSessionName &tSessionName, CSwitchPin& cDstSwitchPin, 
        CSwitchTask*& ptRtnDistribNode, TRedisModuleResourceId &tVtduResourceId);
    CMS_RESULT ChooseVtduAsReceiver(bool bIsTravGAP, u32 dwSwitchNumToAdd, const TSdpSessionName &tSessionName, 
        CSwitchPin& cDstSwitchPin, TRedisModuleResourceId &tVtduResourceId);

    CMS_RESULT StartSwitchToSrc(CSwitchTask* ptSrcSwitchNode);
    bool StopSwitchToSrc(bool bByeToSender);   

	bool IsLocalDomain(const string& strDomainName);

protected:
    bool StopAllSwitch();
    void ReleaseResource();
    void UpdateDevServiceState(bool bStart);

    //////////////////////////////////////////////////////////////////////////////
    //成员属性
public:
	//m_tSubject
	const TInviteSubject& GetSubject() const
	{
		return m_tSubject;
	}
	TInviteSubject& GetSubject()
	{
		return m_tSubject;
	}
	void SetSubject(const TInviteSubject& var)
	{
		m_tSubject = var;
	}

    //m_tSessionName
    const TSdpSessionName& GetSessionName() const
    {
        return m_tSessionName;
    }
    TSdpSessionName& GetSessionName()
    {
        return m_tSessionName;
    }
    void SetSessionName(const TSdpSessionName& var)
    {
        m_tSessionName = var;
    }

    //m_tSwitchMode
    const TSwitchMode& GetSwitchMode() const
    {
        return m_tSwitchMode;
    }
    TSwitchMode& GetSwitchMode()
    {
        return m_tSwitchMode;
    }
    void SetSwitchMode(const TSwitchMode& var)
    {
        m_tSwitchMode = var;
    }

    //m_cSender
    const CSwitchPin& GetSender() const
    {
        return m_cSender;
    }
    CSwitchPin& GetSender()
    {
        return m_cSender;
    }
    void SetSender(const CSwitchPin& var)
    {
        m_cSender = var;
    }

    //m_cReceiver
    const CSwitchPin& GetReceiver() const
    {
        return m_cReceiver;
    }
    CSwitchPin& GetReceiver()
    {
        return m_cReceiver;
    }
    void SetReceiver(const CSwitchPin& var)
    {
        m_cReceiver = var;
    }

    //m_pcInput
    const CSwitchTask* GetInput() const
    {
        return m_pcInput;
    }
    CSwitchTask* GetInput()
    {
        return m_pcInput;
    }

    //m_tOutputList
    const CSwitchTaskList& GetOutputList() const
    {
        return m_tOutputList;
    }
    CSwitchTaskList& GetOutputList()
    {
        return m_tOutputList;
    }

    //m_pcCreatorTask
    const C3PCTask* GetSwitchCreatorTask() const
    {
        return m_pcCreatorTask;
    }
    C3PCTask* GetSwitchCreatorTask()
    {
        return m_pcCreatorTask;
    }
    void SetSwitchCreatorTask(C3PCTask* pcCreatorTask)
    {
        m_pcCreatorTask = pcCreatorTask;
    }

    void AddChildTask(int nSeqNo, CTask* pTask)
    {
        if (pTask == NULL)
        {
            TASKLOG(ERROR_LEV, "pTask == NULL");
            return;
        }

        m_cChildTaskList.Insert(nSeqNo, pTask);
    }

    void DelChildTask(int nSeqNo)
    {
        m_cChildTaskList.Erase(nSeqNo);
    }

    CTask* FindChildTask(int nSeqNo)
    {
        CTask* pcTask = NULL;
        m_cChildTaskList.Find(nSeqNo, pcTask);
        return pcTask;
    }

	const TSipURI &GetSendToSipUri() const
	{
		return m_tSendToSipUri;
	}
	TSipURI &GetSendToSipUri()
	{
		return m_tSendToSipUri;
	}
	void SetSendToSipUri(const TSipURI &tSendToSipUri)
	{
		m_tSendToSipUri = tSendToSipUri;
	}

    virtual TRedisModuleResourceId GetVtduResourceId() const
    {
        return m_tVtduResourceId;
    }
    void SetVtduResourceId(const TRedisModuleResourceId &tResourceId)
    {
        m_tVtduResourceId = tResourceId;
    }

protected:
    TInviteSubject		m_tSubject;
    TSdpSessionName     m_tSessionName;
    TSwitchMode         m_tSwitchMode;

    CSwitchPin			m_cSender;
    CSwitchPin			m_cReceiver;
    CSwitchPin*			m_pcCurInviting;

    CSwitchTask*        m_pcInput;
    CSwitchTaskList     m_tOutputList;
	C3PCTask* m_pcCreatorTask;
	TSipURI m_tSendToSipUri;

    TRedisModuleResourceId m_tVtduResourceId;         // vtdu的资源id

    //key是消息序列号，value是事务指针
    CXMap<int, CTask*> m_cChildTaskList;   //对话内事务列表，比如vcr控制事务
};

//////////////////////////////////////////////////////////////////////////////////////////////////////
//typedef CStrMap<CSwitchTask*> CSwitchMap;

class CSwitchList : public CSwitchTaskList
{
public:
    CSwitchTask* CreateInviteReceiverSwitchTask(CInstExt *pcInst);
    CSwitchTask* CreateInviteSenderSwitchTask(CInstExt *pcInst);

    void EraseSwitchNode(CSwitchTask* pSwitchNode);

	CSwitchTask* FindSwitchNode(const TChannel& tDevChn, const TChannelType& tChannelType = "", const TSdpSessionName& tSessionName = "", const TSwitchMode& tSwitchMode = SWITCH_MODE_SNG ) const;
	CSwitchTask* FindDstSwitchNode(TSdp& tDstSdp) const;

    void PrintData() const;

    /*该函数返回值的是业务(pmc)上需要使用的码流路数，即目的交换数。
      理论上(不考虑备份源)各vtdu中所有SwitchSsnID个数相加应该等于该值。
    */
    u32 GetDstSwitchNum() const; 
};
extern CSwitchList g_cSwitchList;


template<class CReq, class CRsp>
class CInDlgReqRspTask : public CSipTask
{
public:
    enum
    {
        Req = UNKNOWN_STATE+1,
        WaitRsp,
    };

public:

    CInDlgReqRspTask(CInstExt* pcInst, CSwitchTask* pcSwitchTask) : CSipTask(pcInst)
    {
        CMS_ASSERT(m_pcSwitchTask != NULL&& "m_pcSwitchTask == NULL");
        m_pcSwitchTask = pcSwitchTask;       
        m_tSipTransId = INVALID_TRANS_ID;
        m_eSipRspEvent = KDSIP_EVENT_UNKNOWN;
    }

    ~CInDlgReqRspTask()
    {	
        ReleaseResource();
    }

    void ReleaseResource()
    {
        if (m_pcSwitchTask != NULL)
        {
            m_pcSwitchTask->DelChildTask(GetSeqNo());
        }
    }

public:
    virtual const char* GetObjName() const
    {
        return "CInDlgReqRspTask";
    }

    virtual const char* GetStateName(u32 dwState) const
    {
        switch (dwState)
        {
        case Req:
            return "Req";
        case WaitRsp:
            return "WaitRsp";
        default:
            break;
        }
        return "Unknown State";
    }


    virtual void InitStateMachine()
    {
        CStateProc cReqProc;
        cReqProc.ProcMsg = (CTask::PFProcMsg)&CInDlgReqRspTask::OnReq;
        cReqProc.ProcErrMsg = (CTask::PFProcMsg)&CInDlgReqRspTask::OnReq;
        cReqProc.TimerPoll = (CTask::PFTimerPoll)&CInDlgReqRspTask::OnReqTimer;
        AddRuleProc(Req, cReqProc);

        CStateProc cWaitRspProc;
        cWaitRspProc.ProcMsg = (CTask::PFProcMsg)&CInDlgReqRspTask::OnWaitRsp;
        cWaitRspProc.ProcErrMsg = (CTask::PFProcMsg)&CInDlgReqRspTask::OnWaitRsp;
        cWaitRspProc.TimerPoll = (CTask::PFTimerPoll)&CInDlgReqRspTask::OnWaitRspTimer;
        AddRuleProc(WaitRsp, cWaitRspProc);

        NextState(Req);
    }

    virtual void PrintData() const
    {
        CTask::PrintData();

        //直接打印xml数据
        OspPrintf(TRUE, FALSE, "  %s\n", m_tReq.ToXml().c_str());
    }

    //状态处理
    u32 OnReq(CMessage *const pcMsg)
    {
        u32 dwRet = PROCMSG_OK;
        if ((u16)m_tReq.GetEventId() == pcMsg->event)
        {
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
            m_tSipTransId = pcOspSipMsg->GetSipTransID();
            m_eSipRspEvent = pcOspSipMsg->GetSipRsp();

            CReq cReq;
            pcOspSipMsg->GetMsgBody(cReq);
            m_tReq = cReq; 

            m_pcSwitchTask->AddChildTask(GetSeqNo(), this);

            CSwitchTask* pcTask = m_pcSwitchTask->GetTopSrc();
            if (pcTask != NULL)
            {
                pcTask->ProcMsg(pcMsg);
            }
            else
            {
				bool bDirectTransfer = m_pcSwitchTask->GetReceiver().GetSdp().GetPlatDirectTransfer()
					|| m_pcSwitchTask->GetReceiver().GetSdp().GetDirectTransfer();
				TASKLOG(CMU_MOD_SWITCH, EVENT_LEV, "DirectTransfer:%d\n",
					bDirectTransfer);
				if (bDirectTransfer && (NULL != m_pcSwitchTask))
				{
					CReq cReq;
					pcOspSipMsg->GetMsgBody(cReq);
					if(PROCMSG_OK!= PostInDlgReq(pcOspSipMsg->GetSipReq(), cReq, m_pcSwitchTask->GetSender().GetDlgID()))
					{
						dwRet = PROCMSG_FAIL;
						TASKLOG(CMU_MOD_SWITCH, WARNING_LEV, "消息投递失败 Msg[%s-%d]\n",
							OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
					}
				}
				else
				{
					TASKLOG(CMU_MOD_SWITCH, WARNING_LEV, "获取源交换事务失败 Msg[%s-%d]\n", 
						OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
					dwRet = PROCMSG_FAIL;
				}   
            }
        }
        else
        {
            TASKLOG(CMU_MOD_PU, WARNING_LEV, "Receive unkown Msg[%s-%d]\n", 
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            dwRet = PROCMSG_FAIL;
        }

        if (dwRet != PROCMSG_OK)
        {
            SendRsp(ERR_CMU_SWITCH_VCR_FAILED);
        }
        else
        {
            NextState(WaitRsp);
        }

        return dwRet;
    }

    u32 OnReqTimer()
    {
        return TIMERPOLL_DONE;
    }

    u32 OnWaitRsp(CMessage *const pcMsg)
    {
        u32 dwRet = PROCMSG_DEL;

        //SIP层错误处理
        if (pcMsg->event == OSP_SIP_MSG_PROC_FAIL)
        {
            //发起的请求收到SIP层的错误应答
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;

            TASKLOG(CMU_MOD_PU, ERROR_LEV, "发生SIP层错误，sip status code[%d]\n",
                pcOspSipMsg->GetSipErrorCode());

            SendRsp(ERR_CMU_SWITCH_VCR_FAILED);

            return dwRet;
        }

        CRsp cRsp;
        if ((u16)cRsp.GetEventId() == pcMsg->event)
        {
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;

            pcOspSipMsg->GetMsgBody(cRsp);
            cRsp.SetSession(m_tReq.GetSession());
            SendRsp(cRsp);
        }
        else
        {
            TASKLOG(CMU_MOD_PU, WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            dwRet = PROCMSG_FAIL;
        }

        //事务处理结束，删除之
        return dwRet;
    }

    u32 OnWaitRspTimer()
    {
        //应答超时
        if (GetCurStateHoldTime() > CMU_REQ_RSP_TASK_TIMEOUT)
        {
            TASKLOG(CMU_MOD_PU, WARNING_LEV, "对话内控制应答超时, Msg[%s-%d]请求失败\n", 
                m_tReq.GetEventStr().c_str(), m_tReq.GetEventId());
            SendRsp(ERR_CMU_DEV_RSP_TIMEOUT);
            return TIMERPOLL_DEL;
        }

        return TIMERPOLL_DONE;
    }

    bool SendRsp(u32 dwError)
    {
        CRsp cRsp;
        cRsp.SetSeqNum(m_tReq.GetSeqNum());
        cRsp.SetSession(m_tReq.GetSession());
        cRsp.SetErrorCode(dwError);

        return (PROCMSG_OK == PostRsp(m_eSipRspEvent, m_tSipTransId, cRsp));
    }

    bool SendRsp(const CRsp& cRsp)
    {
        return (PROCMSG_OK == PostRsp(m_eSipRspEvent, m_tSipTransId, cRsp));
    }

public: 
    const TKDSIP_TRANS_ID GetSipTransID() const
    {
        return m_tSipTransId;
    }
    void SetSipTransID(const TKDSIP_TRANS_ID tSipTransID)
    {
        m_tSipTransId = tSipTransID;
    }
    int GetSeqNo() const
    {
        return m_tReq.GetSeqNum();
    }

private:
    CSwitchTask* m_pcSwitchTask;
    TKDSIP_TRANS_ID m_tSipTransId;
    EKDSIP_EVENT m_eSipRspEvent;
    
    CReq m_tReq;
};

typedef CInDlgReqRspTask<CVcrCtrlReq, CVcrCtrlRsp> CPuVcrCtrlTask;



#endif  //#ifndef _INVITE_SWITCH_TASK_H