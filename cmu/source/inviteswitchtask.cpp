/*****************************************************************************
模块名      : cmu 
文件名      : switchtask.cpp
相关文件    : 
文件实现功能: CSwitchTask实现
作者        : 黄至春
版本        : vs_phoenix  Copyright(C) 2009-2015 KDDACOM, All rights reserved.
-----------------------------------------------------------------------------
修改记录:
日  期         版本        修改人      修改内容
2011/11/04     1.0        黄至春        创建
2012/03/30     1.0        黄至春       重构交换逻辑：把之前的一个交换任务分成Src和Dst两个交换任务
******************************************************************************/

#include "inviteswitchtask.h"
#include "invitesenderswitchtask.h"
#include "invitereceiverswitchtask.h"
#include "3pctask.h"

#include "vtduregtask.h"
#include "cms/cmu/cmu_struct.h"


CSwitchList g_cSwitchList;
//CSwitchTaskMgr g_cCSwitchTaskMgr;
extern TDomainTable g_tDomainTable;
extern map<string, string> g_mapInterfaceIpList;
//////////////////////////////////////////////////////////////////////////////
//CSwitchTask
CSwitchTask::CSwitchTask(CInstExt *pcInst) : CCmuTask(pcInst)
{
    m_tSwitchMode = SWITCH_MODE_SNG;
    m_pcCurInviting = NULL;

    m_pcInput = NULL;
    m_pcCreatorTask = NULL;

    this->SetModuleID(CMU_MOD_SWITCH);
}

CSwitchTask::~CSwitchTask()
{
    ReleaseResource();

    //if (m_tDstDlgId != INVALID_DIALOG_ID)
    //{
    //    OspSipReleaseDlg(m_tDstDlgId);
    //    m_tDstDlgId = INVALID_DIALOG_ID;
    //}

    //if (m_tSrcDlgId != INVALID_DIALOG_ID)
    //{
    //    OspSipReleaseDlg(m_tSrcDlgId);
    //    m_tSrcDlgId = INVALID_DIALOG_ID;
    //}
}

void CSwitchTask::NextState(const u32 dwState)
{
    CSipTask::NextState(dwState);

    //交换完成，回调给目的输出结点
    if(SWITCH_STATE_SWITCHED == GetSwitchState())
    {
		// 交换完成后，如果源交换设置了延迟发送ack的标志，则此时发送源交换的ack
		CInviteSenderSwitchTask *pcSrcTask = dynamic_cast<CInviteSenderSwitchTask *>(m_pcInput);
		if (NULL != pcSrcTask && pcSrcTask->IsDelayPuAck())
		{
			TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "目的交换建立完成，向延迟发送Ack的源交换发送Ack！\n");

			pcSrcTask->SendDelayPuAck();
		}

        UpdateDevServiceState(true);

        //通知所有输出（目的）结点
        CSwitchTaskList::iterator itrOutput = this->GetOutputList().begin();
        for(; itrOutput != this->GetOutputList().end(); itrOutput++)
        {
            CSwitchTask* pcOutputTask = *itrOutput;

            pcOutputTask->CBInputSwitchFinish(this);
        }

        //通知3pc交换创建者
        u32 dwTaskNo = this->GetTaskNO();
        CallbackToTask(CMU_SWITCH_FINISH_CB, &dwTaskNo, sizeof(dwTaskNo), m_pcCreatorTask);
    }

}

//////////////////////////////////////////////////////////////////////////////
//操作
//////////////////////////////////////////////////////////////////////////////
bool CSwitchTask::IsInviteReqFrom(const CSwitchPin& cSwitchPin)
{
    if (cSwitchPin.GetDlgID() != INVALID_DIALOG_ID && cSwitchPin.GetDlgID() == this->GetSipDlgID())
    {
        return true;
    }

    return false;
}

CSwitchPin& CSwitchTask::GetSwitchPin(TKDSIP_DIALOG_ID tDlgId, bool bMatching)
{
    if( tDlgId == m_cSender.GetDlgID())
    {
        if(bMatching)
        {
            return m_cSender;
        }
        else
        {
            return m_cReceiver;
        }
    }
    else if( tDlgId == m_cReceiver.GetDlgID())
    {
        if(bMatching)
        {
            return m_cReceiver;
        }
        else
        {
            return m_cSender;
        }
    }
    else
    {
        TASKLOG(CMU_MOD_SWITCH, ERROR_LEV, "未找到相应的SwitchPin，DlgId[%lu]！\n", tDlgId);

        static CSwitchPin cInvalidSwitchPin;
        return cInvalidSwitchPin;
    }
}

CMS_RESULT CSwitchTask::InitSwitchPin(CSwitchPin& cSwitchPin, const TChannel& tDevChn, CLoginSession* pSessTask, 
    TKDSIP_DIALOG_ID tDlgID, const TSdpSessionName &tSessionName)
{
    TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "InitSwitchPin(tChannel[%s], pSessTask=%p, tDlgID=%lu)\n", 
        tDevChn.ToString().c_str(), pSessTask, tDlgID);

    cSwitchPin.SetChannel(tDevChn);
    cSwitchPin.SetDlgID(tDlgID);
    ///////////////////////////////////////////////////////////////
    //判断设备是否存在
    TSipURI tRouteUri;
    if(NULL == pSessTask)
    {
        bool bRouteFollowTopo =  IsInviteReqFrom(cSwitchPin) ? false : g_cCmuConf.m_bSwitchFollowTopo;

        CheckDevRouter(tDevChn.GetDevUri(), bRouteFollowTopo, tRouteUri, &pSessTask);
        if (this->GetErrorCode() !=  CMS_SUCCESS)
        {
            return this->GetErrorCode();
        }
    }
    else
    {
        //跨域时Session会跟DevUri不一样，这里不再需要诊断了
        //CMS_ASSERT(pSessTask->GetSession() == tDevChn.GetDevUri());

        tRouteUri = pSessTask->GetSession();
    }

    CMS_ASSERT(NULL != pSessTask && "pSessTask = NULL!!!");
    if (pSessTask->GetDevType() == DEV_TYPE_PU)
    {
        CPuData *pcPuData = GetPuDataByUri(tDevChn.GetDevUri());
        if (NULL != pcPuData)
        {
            if (SDP_SESSION_PLAY == tSessionName
                && DEVICE_TYPE_ENCODER == pcPuData->GetDevType() 
                && CHNSTATUS_ONLINE != pcPuData->GetChnStateConfig(tDevChn.GetChnNO()).GetChnState())
            {
                this->SetErrorCode(ERR_CMU_PU_NOT_ONLINE);

                return this->GetErrorCode();
            }
        }
        else
        {
            this->SetErrorCode(ERR_CMU_PU_NOT_IN_TOP);

            return this->GetErrorCode();
        }
    }

    //把设备的注册任务与当前交换任务进行关联
    cSwitchPin.SetDevSsnTask(pSessTask);
    pSessTask->AddTask(this);	         //把交换任务添加到设备会话任务中，便于该设备断链时能正确清空所有子任务

    cSwitchPin.SetToUri(tRouteUri);

    return this->GetErrorCode();
}

//******************************************************************************
// 函数名:    SetSdp
// 功能:      根据Sdp的Owner设置SwitchPin，否则拷贝SdpBasicInfo到另一侧SwitchPin
// 算法实现:  【NOTE】因为里面可能会生成Key去改写该tSdp参数,所以参数采用传对象而而非引用的方式。
// 访问权限:  public 
// 参数说明:  TSdp tSdp
// 返回值:    bool
//-----------------------------------------------------------
// 修改记录:
// 日      期     修改人        走读人          修改记录
// 2013/08/02     huangzhichun huangzhichun    创建
//******************************************************************************
bool CSwitchTask::SetSdp(TSdp tSdp)
{
    bool bFoundOwner = false;
    bool bIsNeedPunch = GetIsNeedPunch();

	//如果SDP是接收端RCS的（即当前任务为平台录像），则不生成PunchKey。
	//todo NRU使用注册IP，vtdu应该使用对应网段，目前好像没有？？？？
	if (m_cReceiver.GetChannel() == tSdp.GetOwner() && DEV_TYPE_RCS == m_cReceiver.GetDevType())
	{
		bIsNeedPunch = false;
	}

	//如果SDP是接收端CUI的，且是NAT探测包模式，则不Punch
	if (m_cReceiver.GetChannel() == tSdp.GetOwner() && tSdp.GetIsNatPacketMode())
	{
		bIsNeedPunch = false;
	}

	//来自CUI的音频呼叫，设置VTDU的NAT探测包模式
	if (DEV_TYPE_CU == m_cSender.GetDevType() && SDP_SESSION_AUDIOCALL == tSdp.GetSessionName())
	{
		m_cReceiver.GetSdp().SetIsNatPacketMode(tSdp.GetIsNatPacketMode());
	}

	//来自GBS和CUI1的音频呼叫，设置VTDU的InterfaceIp
	if ((DEV_TYPE_GBS == m_cSender.GetDevType() || DEV_TYPE_CUI1 == m_cSender.GetDevType()) && SDP_SESSION_AUDIOCALL == tSdp.GetSessionName())
	{
		m_cReceiver.GetSdp().SetInterfaceIp(tSdp.GetInterfaceIp());
	}

    //为各个通道，生成MediaChannelKey
    for(TMediaDescList::iterator itrMediaDesc = tSdp.GetMediaDescList().begin(); itrMediaDesc != tSdp.GetMediaDescList().end(); itrMediaDesc++)
    {
        //遍历tMediaTransChnList
        TMediaTransChannelList& tMediaTransChnList = itrMediaDesc->GetMediaTransChnList();

        for(TMediaTransChannelList::iterator itrTransChn = tMediaTransChnList.begin(); itrTransChn!=tMediaTransChnList.end(); itrTransChn++)
        {
            if (bIsNeedPunch)
            {
                itrTransChn->SetMediaChannelKey(CUUID());
            }
            else
            {
                itrTransChn->SetMediaChannelKey(TMediaChannelKey());
            }
        }
    }
    //tSdp.SetIsNeedPunch(true);

    //初始化Sender端的Sdp
    if(!m_cSender.GetChannel().IsNull() && m_cSender.GetChannel() == tSdp.GetOwner())
    {
        m_cSender.SetSdp(tSdp);
        bFoundOwner = true;
    }
    else
    {
        m_cSender.GetSdp().CopyBasicInfoFrom(tSdp);

        if (tSdp.GetExtInfo().end() != tSdp.GetExtInfo().find(SDP_EXT_INFO_TRANSFER_RATE))
        {
            m_cSender.GetSdp().SetTransferRate(tSdp.GetTransferRate());
        }
    }

    //初始化Receiver端的Sdp
    if(!m_cReceiver.GetChannel().IsNull() && m_cReceiver.GetChannel() == tSdp.GetOwner())
    {
        m_cReceiver.SetSdp(tSdp);
        bFoundOwner = true;
    }
    else
    {
        m_cReceiver.GetSdp().CopyBasicInfoFrom(tSdp);
    }

    //3PC交换一定会进这个逻辑，所以屏蔽打印
    //if (!bFoundOwner)
    //{
    //    TASKLOG(CMU_MOD_SWITCH, ERROR_LEV, "Unknown Sdp.Owner[%s]\n", tSdp.GetOwner().ToString().c_str());
    //}

    return true;
}

CSwitchTask* CSwitchTask::GetTopSrc() const
{
	CSwitchTask* pcInput = m_pcInput;

	for(; NULL!=pcInput; pcInput=pcInput->GetInput())
	{
		if(NULL == pcInput->GetInput())
		{
			return pcInput;
		}
	}

	return pcInput;
}

u32 CSwitchTask::GetTransChnNum() const
{
    //这里可以是m_cReceiver.Sdp也可以是m_cSender.Sdp
    return (u32)m_cReceiver.GetSdp().GetTransChnNum();
}

s32 CSwitchTask::GetFreeDistrSwitchNum() const
{
    if(m_cReceiver.GetDevType() == DEV_TYPE_VTDU)
    {
        CVtduRegTask* pcVtduRegTask = (CVtduRegTask*)m_cReceiver.GetDevSsnTask();
        u32 dwTransChnNum = GetTransChnNum();

        CMS_ASSERT(pcVtduRegTask && dwTransChnNum);

        s32 nFreeSndMmbTotal = pcVtduRegTask->GetFreeSwitchNum()/(s32)GetTransChnNum();
        s32 nFreeSndMmbPerRcvGrp = pcVtduRegTask->GetMaxSndMmbPerRcvGrp()- (s32)m_tOutputList.size();

        TASKLOG(CMU_MOD_SWITCH, EVENT_LEV, "Node[%s], VtduFreeSwitchNum[%d]/NodeTransChnNum[%d]=[%d], MaxSndMmbPerRcvGrp[%d] - OutputDst[%d] = [%d].\n", 
            m_cSender.GetChannel().ToString().c_str(), 
            pcVtduRegTask->GetFreeSwitchNum(), GetTransChnNum(), nFreeSndMmbTotal,
            pcVtduRegTask->GetMaxSndMmbPerRcvGrp(), m_tOutputList.size(), nFreeSndMmbPerRcvGrp);

        if(nFreeSndMmbPerRcvGrp < 0 || nFreeSndMmbTotal < 0)
        {
            return 0;
        }

        return CMS_MIN(nFreeSndMmbTotal, nFreeSndMmbPerRcvGrp);
    }

    return 0;
}

bool CSwitchTask::HasDistrCapFor(u32 dwSwitchNumToAdd) const
{
    return GetFreeDistrSwitchNum() >= (s32)dwSwitchNumToAdd;
}

void CSwitchTask::AddOutput(CSwitchTask* ptValue)
{
    //如果当前结点已经是该结点的输出，则不再添加
    if(ptValue->m_pcInput == this)
    {
        return;
    }

    //如果需要添加的输出结点已经跟其他结点链在一起，则打断他们的连接
    if(ptValue->m_pcInput != NULL)
    {
        ptValue->m_pcInput->RemoveOutput(ptValue);
    }

	m_tOutputList.push_back(ptValue);
	ptValue->m_pcInput = this;

    //如果RecviverPin = VTDU，则可以认为该SwitchTask是一个Src
    //Vtdu的信息都保存在Src上，并也在Src上修改
    if(m_cReceiver.GetDevType() == DEV_TYPE_VTDU)
    {
        CVtduRegTask* pcVtduRegTask = (CVtduRegTask*)m_cReceiver.GetDevSsnTask();
        pcVtduRegTask->AddSwitchNum(ptValue->GetTransChnNum());
    }
}

void CSwitchTask::RemoveOutput(CSwitchTask* ptValue)
{
	m_tOutputList.remove(ptValue);
	ptValue->m_pcInput = NULL;

    if(m_cReceiver.GetDevType() == DEV_TYPE_VTDU)
    {
        CVtduRegTask* pcVtduRegTask = (CVtduRegTask*)m_cReceiver.GetDevSsnTask();
        pcVtduRegTask->SubSwitchNum(ptValue->GetTransChnNum());
    }
}

void CSwitchTask::BreakLink(CSwitchTask* ptSrc, CSwitchTask* ptDst)
{
    if(ptSrc && ptDst)
    {
        ptSrc->RemoveOutput(ptDst);
    }
}

//找出相应类型的具有最大分发能力的结点（如备份源VTDU、MTU）
CSwitchTask* CSwitchTask::FindBakSrcByType(const string& strDevType)
{
    s32 nMaxFreeSwitchNumFound = 0;
    CSwitchTask* pcSwitchNodeSelected = NULL;

    CSwitchTaskList::iterator itrOutputNode = this->GetOutputList().begin();
    for(; itrOutputNode != this->GetOutputList().end(); itrOutputNode++)
    {
        CSwitchTask* pcOutputNode = *itrOutputNode;

        //TASKLOG(CMU_MOD_SWITCH, ALL_LEV, "遍历输出结点[%s].\n", pcOutputNode->GetChannel().ToString().c_str());

        if(pcOutputNode->GetReceiver().GetDevType() == strDevType) 
        { //Src
            s32 nFreeSwitchNum = pcOutputNode->GetFreeDistrSwitchNum();

            TASKLOG(CMU_MOD_SWITCH, EVENT_LEV, "匹配到类型为[%s]的输出结点[%s], FreeSwitchNum=%d, MaxFreeSwitchNumFound=%d.\n", 
                strDevType.c_str(), pcOutputNode->GetReceiver().GetChannel().ToString().c_str(), nFreeSwitchNum, nMaxFreeSwitchNumFound);

            if(nFreeSwitchNum > nMaxFreeSwitchNumFound)
            {
                nMaxFreeSwitchNumFound = nFreeSwitchNum;
                pcSwitchNodeSelected = pcOutputNode;
            }
        }
    }

    return pcSwitchNodeSelected;
}


////////////////////////////////////////////////////////////////////////////////////////////////
//开始交换业务逻辑
////////////////////////////////////////////////////////////////////////////////////////////////

//******************************************************************************
// 函数名:    StartSwitch
// 功能:      开始码流交换的入口函数，负责判断交换类型vtdu调度，及源端与目的端的关联。
// 算法实现:  静态函数
// 访问权限:  public 
// 参数说明:  const TChannel & tDstChn
// 参数说明:  const TChannel & tSrcChn
// 参数说明:  TSdp & tSdp
// 参数说明:  CCmuTask * pcReqTask
// 返回值:    CMS_RESULT
//-----------------------------------------------------------
// 修改记录:
// 日      期     修改人        走读人          修改记录
// 2012/03/30     huangzhichun huangzhichun    创建
//******************************************************************************
CMS_RESULT CSwitchTask::StartSwitch(const TChannel& tDstChn, const TChannel& tSrcChn, TSdp& tSdp, CCmuTask* pcReqTask)
{
	CMS_RESULT nErrorCode = CMS_SUCCESS;
	TInviteSubject tSubject(tDstChn, tSrcChn);

    GLOBELOG(CMU_MOD_SWITCH, CRITICAL_LEV, "Dst[%s], Src[%s], tSdp.Owner[%s], SessionName[%s] tSdp.OfflineSwitch[%s], tSdp.DirectTransfer[%s], tSdp.KAInfo[%s], tSdp.SSRC[%d], tSdp.IsNatPacketMode[%d], tSdp.InterfaceIp[%s].\n", 
        tDstChn.ToString().c_str(), 
        tSrcChn.ToString().c_str(), 
        tSdp.GetOwner().ToString().c_str(),
        tSdp.GetSessionName().c_str(),
        BoolStr(tSdp.GetOfflineSwitch()),
        BoolStr(tSdp.GetDirectTransfer()),
		tSdp.GetGBStreamKAInfo().c_str(),
		tSdp.GetSSRC(),
		tSdp.GetIsNatPacketMode(),
		tSdp.GetInterfaceIp().c_str());

	///////////////////////////////////////////////////////////////////////
	//{{参数校验
	if (tSdp.GetSessionName().empty())
	{
		GLOBELOG(CMU_MOD_SWITCH, WARNING_LEV, "无效的Sdp.SessionName[%s]！\n", 
			tSdp.GetSessionName().c_str());

		//if (tSdp.GetUri().IsEmpty())
		//{
		//    tSdp.SetSessionName(SDP_SESSION_PLAY);
		//} 
		//else
		//{
		//    tSdp.SetSessionName(SDP_SESSION_PLAYBACK);
		//}

		nErrorCode = ERR_INVALIAD_SDP;
		return nErrorCode;
	}

	if (!tSdp.HasMediaDes())
	{
		tSdp.CreateEmptyMediaList();

		GLOBELOG(CMU_MOD_SWITCH, WARNING_LEV, "Sdp no MediaDes, CreateEmptyMediaList()！\n");
	}
	//}}参数校验


	////////////////////////////////////////////////////////////////////////
	//查找目的交换任务。存在，则删除并重建；否则，创建
	CSwitchTask* pcDstSwitchTask = g_cSwitchList.FindSwitchNode(tDstChn, CHANNEL_TYPE_RECVONLY, tSdp.GetSessionName());
	if(NULL != pcDstSwitchTask )
	{
		CSwitchTask* pcTopSrcSwitchTask = pcDstSwitchTask->GetTopSrc();
		TChannel tTmpSrcChn;
		if(pcTopSrcSwitchTask)
		{
			tTmpSrcChn = pcTopSrcSwitchTask->GetSender().GetChannel();
		}

		GLOBELOG(CMU_MOD_SWITCH, WARNING_LEV, "The dst has exist a media swith to src[%s]！\n", tTmpSrcChn.ToString().c_str());

		//nErrorCode = ERR_CMU_SWITCH_DST_EXIST_SWITCH;
		//return nErrorCode;
		delete pcDstSwitchTask;
		pcDstSwitchTask = NULL;
	}

	////////////////////////////////////////////////////////////////////////
	//根据交换地址查找目的交换任务。存在，则删除并创建；否则，创建
	pcDstSwitchTask = g_cSwitchList.FindDstSwitchNode(tSdp);
	if (NULL != pcDstSwitchTask)
	{
		delete pcDstSwitchTask;
		pcDstSwitchTask = NULL;
	}

	/////////////////////////////////////////////////////////////////////////////////
	//检查Sender的连接会话
	CLoginSession* pSrcSess = NULL;
	TSipURI tRouteUri;
	nErrorCode = pcReqTask->CheckDevRouter(tSrcChn.GetDevUri(), g_cCmuConf.m_bSwitchFollowTopo, tRouteUri, &pSrcSess);
	if (nErrorCode !=  CMS_SUCCESS)
	{
		return nErrorCode;
	}

	/////////////////////////////////////////////////////////////////////////////////
	//检查需要采用的交换模式
	TSwitchMode tSwitchMode = CheckSwitchMode(pSrcSess, tSrcChn, tSdp);
	GLOBELOG(CMU_MOD_SWITCH, CRITICAL_LEV, "CheckSwitchMode:[%s]！\n", tSwitchMode.c_str());

	/////////////////////////////////////////////////////////////////////////////////
	//查找源交换任务[针对非直传的实时码流业务（浏览、音频呼叫）]。存在，则直接分发；否则，创建
	CSwitchTask* pcSrcSwitchTask = NULL;

	if (SWITCH_MODE_DRT != tSwitchMode && IsRealTimeMediaSession(tSdp.GetSessionName()) )
	{
		pcSrcSwitchTask = g_cSwitchList.FindSwitchNode(tSrcChn, CHANNEL_TYPE_SENDONLY, tSdp.GetSessionName());
	}

	TKDSIP_DIALOG_RESOURCE tInfo;
	if (GetWorkInst()->GetKdsipResInfo(tInfo, KDSIP_DIALOG_TYPE_INVITE))
	{
		u32 dwIvtDlgUnUsed = tInfo.dwMax - tInfo.dwUsage + 1;//加1是因为cui/rcs请求时就已经消耗了cmu一个dlg资源
		if (((dwIvtDlgUnUsed < CMU_SWITCH_NEED_DLG_NUM) && (NULL == pcSrcSwitchTask))
			|| ((dwIvtDlgUnUsed < CMU_DST_SWITCH_NEED_DLG_NUM) && (NULL != pcSrcSwitchTask)))
		{
			GLOBELOG(CMU_MOD_SWITCH, ERROR_LEV, "Invite dialog resource is not enough, IvtDlgMax[%lu], IvtDlgUsed[%lu]！\n",
				tInfo.dwMax, tInfo.dwUsage);
			nErrorCode = ERR_CMU_SWITCH_DLG_RES_NOT_ENOUGH;
			return nErrorCode;
		}

		GLOBELOG(CMU_MOD_SWITCH, EVENT_LEV, "IvtDlgMax[%lu], IvtDlgUsed[%lu]！\n",
			tInfo.dwMax, tInfo.dwUsage);
	}

	/////////////////////////////////////////////////////////////////////////////////
	//判断InviteReq来源
	if(tSdp.GetOwner() != tSrcChn)
	{
        //创建或初始化DstSwitchTask
		if(tSdp.GetOwner() == tDstChn)
		{
			//InviteReq来自Receiver。
			GLOBELOG(CMU_MOD_SWITCH, CRITICAL_LEV, "InviteReq come from DstChn！\n");

			pcDstSwitchTask = (CSwitchTask *)pcReqTask;
			//初始化DstSwitchTask参数
			nErrorCode = pcDstSwitchTask ->InitReceiver(tDstChn, tSdp.GetSessionName(), NULL, pcReqTask->GetSipDlgID());
			if(CMS_SUCCESS != nErrorCode)
			{
				return nErrorCode;
			}
		}
		else
		{
			//第三方呼叫控制
			GLOBELOG(CMU_MOD_SWITCH, CRITICAL_LEV, "3PCC！\n");

			//创建目的交换任务
			GLOBELOG(CMU_MOD_SWITCH, CRITICAL_LEV, "DstChn SwitchNode isn't exist, create one！\n");

			pcDstSwitchTask =  g_cSwitchList.CreateInviteSenderSwitchTask(pcReqTask->GetInstance());
			//初始化DstSwitchTask参数
			nErrorCode = pcDstSwitchTask ->InitReceiver(tDstChn, tSdp.GetSessionName());
			if(CMS_SUCCESS != nErrorCode)
			{
				SAFE_DEL_PTR(pcDstSwitchTask);
				return nErrorCode;
			}

			//把目的交换任务 与 创建者任务关联起来
			//cmu中交换调度规则：源交换任务先调度，目的交换任务后调度。
			//创建者回调必须在调度任务都成功后才能执行，所以将创建者设置在目的交换任务中。
			pcDstSwitchTask->SetSwitchCreatorTask((C3PCTask*)pcReqTask);
			((C3PCTask*)pcReqTask)->SetDstSwitchTask(pcDstSwitchTask);
		}

		pcDstSwitchTask->SetSubject(tSubject);
		pcDstSwitchTask->SetSessionName(tSdp.GetSessionName());
		pcDstSwitchTask->SetSwitchMode(tSwitchMode);
		pcDstSwitchTask->SetSdp(tSdp);
		//{{{{{{{{{{{{{{{ modify by Royan 20140606 国标流保活
		pcDstSwitchTask->GetReceiver().GetSdp().SetGBStreamKAInfo(tSdp.GetGBStreamKAInfo());
		//}}}}}}}}}}}}}}} modify by Royan 20140606 国标流保活

		//直传
		if ( SWITCH_MODE_DRT == tSwitchMode )
		{
			nErrorCode = pcDstSwitchTask ->InitSender(tSrcChn, tSdp.GetSessionName());
			if(CMS_SUCCESS != nErrorCode)
			{
				return nErrorCode;
			}

			pcDstSwitchTask->GetSender().GetDevSsnTask()->AddCurDirectTransNum();

			nErrorCode = pcDstSwitchTask->Start();
			return nErrorCode;
		}
		else	//平台转发交换模式
		{
			if (NULL == pcSrcSwitchTask)
			{
				//创建源交换结点
				GLOBELOG(CMU_MOD_SWITCH, CRITICAL_LEV, "SrcChn SwitchNode isn't exist, create one！\n");

				pcSrcSwitchTask = g_cSwitchList.CreateInviteSenderSwitchTask(pcReqTask->GetInstance());
				nErrorCode = pcSrcSwitchTask ->InitSender(tSrcChn, tSdp.GetSessionName());
				if(CMS_SUCCESS != nErrorCode)
				{
					SAFE_DEL_PTR(pcSrcSwitchTask);
					return nErrorCode;
				}

				pcSrcSwitchTask->SetSubject(tSubject);
				pcSrcSwitchTask->SetSessionName(tSdp.GetSessionName());
				pcSrcSwitchTask->SetSwitchMode(tSwitchMode);
				pcSrcSwitchTask->SetSdp(tSdp);
				//{{{{{{{{{{{{{{{ modify by Royan 20140606 国标流保活
				pcSrcSwitchTask->GetReceiver().GetSdp().SetGBStreamKAInfo(tSdp.GetGBStreamKAInfo());
				//}}}}}}}}}}}}}}} modify by Royan 20140606 国标流保活
				pcSrcSwitchTask->GetReceiver().GetSdp().SetGBRecLocation(tSdp.GetGBRecLocation());
			}
		}
	}
	else
	{
		//InviteReq来自Sender。
		GLOBELOG(CMU_MOD_SWITCH, CRITICAL_LEV, "InviteReq come from SrcChn！\n");
		CMS_ASSERT(false && "UNIMPLEMENTED for InviteReq come from SrcChn!");

        return ERR_INVALIAD_REQ_PARAM;

		////不允许非会话内的多次同源InviteReq
		//if(SWITCH_MODE_SNG == tSwitchMode && NULL != pcSrcSwitchTask && pcSrcSwitchTask->GetSender().GetDlgID() != tDlgID)
		//{
		//	GLOBELOG(CMU_MOD_SWITCH, CRITICAL_LEV, "ReInvite from SrcChn but not in Dialog！\n");
		//	nErrorCode = ERR_CMU_SWITCH_SRC_EXIST_SWITCH;
		//	return nErrorCode;
		//}

		//pcSrcSwitchTask = this;
		//nErrorCode = pcSrcSwitchTask ->InitSender(tSrcChn, NULL, tDlgID);
		//if(CMS_SUCCESS != nErrorCode)
		//{
		//	return nErrorCode;
		//}

		//pcSrcSwitchTask->SetSubject(tSubject);
		//pcSrcSwitchTask->SetSessionName(tSdp.GetSessionName());
		//pcSrcSwitchTask->SetSwitchMode(tSwitchMode);
		//pcSrcSwitchTask->SetSdp(tSdp);

		////直传
		//if ( SWITCH_MODE_DRT == tSwitchMode)
		//{
		//	pcDstSwitchTask->GetSender().GetDevSsnTask()->AddCurDirectTransNum();

		//	nErrorCode = pcSrcSwitchTask ->InitReceiver(tDstChn);
		//	if(CMS_SUCCESS != nErrorCode)
		//	{
		//		return nErrorCode;
		//	}

		//	nErrorCode = pcSrcSwitchTask->Start();

		//	return nErrorCode;
		//}
	}

	/////////////////////////////////////////////////////////////////////////////////
	//从源交换中选择分发节点 或 调度vtdu
	CSwitchTask* pcRtnDistribNode = NULL;
	nErrorCode = pcSrcSwitchTask->ChooseDistribNode(pcDstSwitchTask, pcRtnDistribNode);

	if(NULL != pcRtnDistribNode)
	{
		//直接给pcSrcSwitchTask赋值，之前分配给他的SwitchTask如何处理??? fanxg
		pcSrcSwitchTask = (CSwitchTask*)pcRtnDistribNode;
	}
	pcSrcSwitchTask->AddOutput(pcDstSwitchTask);                   //先把交换结点关联，否则失败时ptSrcSwitchNode不能被正确删除

	if(CMS_SUCCESS != nErrorCode)
	{
		//SAFE_DEL_PTR(pcDstSwitchTask);    //失败后外层会删除本Task
		return nErrorCode;
	}

	nErrorCode = MakeLink(pcSrcSwitchTask, pcDstSwitchTask);
	return nErrorCode;
}

TSwitchMode CSwitchTask::CheckSwitchMode(CLoginSession* pSrcSess, const TChannel& tSrcChn, const TSdp& tSdp)
{
    //对于实时码流，需要判断设备是否有直传能力
	if (IsRealTimeMediaSession(tSdp.GetSessionName()))
	{
        //if (pSrcSess->HasDirectTransferCap())
        //{
        //    GLOBELOG(CMU_MOD_SWITCH, CRITICAL_LEV, "Dev has DirectTransferCap!\n");

        //    if ((g_cCmuConf.m_nDirectTransMode == DIRECTTRANS_MODE_ONDEMAND && tSdp.GetDirectTransfer())
        //        || g_cCmuConf.m_nDirectTransMode == DIRECTTRANS_MODE_ALWAYS )
        //    {
        //        return SWITCH_MODE_DRT;
        //    }
        //}
		if (tSdp.GetPlatDirectTransfer() || (g_cCmuConf.m_nDirectTransMode == DIRECTTRANS_MODE_ONDEMAND && tSdp.GetDirectTransfer())
			|| g_cCmuConf.m_nDirectTransMode == DIRECTTRANS_MODE_ALWAYS )
		{
			return SWITCH_MODE_DRT;
		}
	}
    //对于历史码流，设备本身就支持一定能力的多路发送，不需判断直传能力
    else
    {
        if (tSdp.GetPlatDirectTransfer() || tSdp.GetDirectTransfer() )
        {
            return SWITCH_MODE_DRT;
        }
    }

	//HZCTODO:是否组播

	//默认 单播交换
	return SWITCH_MODE_SNG;
}

//参数为什么使用指针的引用，没看到指针值被修改啊???-fanxg
CMS_RESULT CSwitchTask::MakeLink(CSwitchTask* ptSrcSwitch, CSwitchTask* ptDstSwitch)
{
    CMS_RESULT nErrorCode = CMS_SUCCESS;

    GLOBELOG(CMU_MOD_SWITCH, CRITICAL_LEV, "Src[%s].SwitchState[%s], Dst[%s].SwitchState[%s]\n",
        ptSrcSwitch->GetSender().GetChannel().ToString().c_str(), GetSwitchStateStr(ptSrcSwitch->GetSwitchState()), 
        ptDstSwitch->GetReceiver().GetChannel().ToString().c_str(), GetSwitchStateStr(ptDstSwitch->GetSwitchState()));

    //外层已经添加过一次???  fanxg
    ptSrcSwitch->AddOutput(ptDstSwitch);

    //根据交换时序图，先完成Src部分的交换，如果Src已经交换完毕（浏览分发）则直接处理Dst部分
    if(ptSrcSwitch->GetSwitchState() == SWITCH_STATE_INIT )
    {
        nErrorCode = ptSrcSwitch->Start();
    }
    else if(ptSrcSwitch->GetSwitchState() == SWITCH_STATE_SWITCHED )
    {
        nErrorCode = ptDstSwitch->Start();
    }
    else
    {
        //如果ptSrcSwitch处于中间状态则忽略，不去触发任何动作；相应的动作完成，有需要触发的那时再去触发。
        GLOBELOG(CMU_MOD_SWITCH, WARNING_LEV, "SrcNode in a middle state: [%s], IGNORE!\n", GetSwitchStateStr(ptSrcSwitch->GetSwitchState()));
    }

    return nErrorCode;
}

///////////////////////////////////////////////////////////////////////////
//成员函数
CMS_RESULT CSwitchTask::CBInputSwitchFinish(const CSwitchTask* pInput)
{
    TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "[%s].CBInputSwitchFinish().\n", this->GetChannel().ToString().c_str());

    return this->Start();
}

bool CSwitchTask::IsLocalDomain(const string& strDomainName)
{
	return (g_cCmuConf.m_cCmCfg.GetDomainName() == strDomainName);
}

CMS_RESULT CSwitchTask::InviteTo(const CSwitchPin& cSwitchPinTo, /*const TInviteSubject tSubject,*/ const TSdp& tSdp)
{
    const TChannel& tDevChn = cSwitchPinTo.GetChannel();

    TASKLOG(CMU_MOD_SWITCH, EVENT_LEV, "Invite to [%s]!\n", tDevChn.ToString().c_str());

	CInviteReq cInviteReq;
	cInviteReq.SetSession(cSwitchPinTo.GetDevSsnTask()->GetSession());
	cInviteReq.SetDstChn(this->GetSubject().GetDstChn());
	cInviteReq.SetSrcChn(this->GetSubject().GetSrcChn());
	cInviteReq.SetSdp(tSdp);
    if (this->GetSwitchMode() == SWITCH_MODE_DRT)
    {
        cInviteReq.GetSdp().SetDirectTransfer(true);
    }

    bool bIsNeedPunch = this->GetIsNeedPunch();
    //int nNetSegNo = INVALID_NETSEG_NO;
    //if (!bIsNeedPunch)
    //{
    //    nNetSegNo = GetOtherSwitchPin(cSwitchPinTo.GetChannel()).GetDevNetSegNO();
    //}
	string strNetSegIp;
	CSwitchPin &cOtherSwitchPin = GetOtherSwitchPin(cSwitchPinTo.GetChannel());
	strNetSegIp = cOtherSwitchPin.GetJoinInterfaceIp();

	TASKLOG(CMU_MOD_SWITCH, EVENT_LEV, "cSwitchPinTo Chn[%s] ToUri[%s]\n", 
		cSwitchPinTo.GetChannel().ToXml().c_str(),
		cSwitchPinTo.GetToUri().GetURIString().c_str());
	TASKLOG(CMU_MOD_SWITCH, EVENT_LEV, "cOtherSwitchPin Chn[%s] ToUri[%s]\n", 
		cOtherSwitchPin.GetChannel().ToXml().c_str(),
		cOtherSwitchPin.GetToUri().GetURIString().c_str());
	TASKLOG(CMU_MOD_SWITCH, EVENT_LEV, "tSdp.InterfaceIp[%s], strNetSegIp[%s], OtherSwitchPinSdp.InterfaceIp[%s], OtherDevType[%s]!\n",
		tSdp.GetInterfaceIp().c_str(),
		strNetSegIp.c_str(),
		cOtherSwitchPin.GetSdp().GetInterfaceIp().c_str(),
		cOtherSwitchPin.GetDevType().c_str());

	string strDomainName = cOtherSwitchPin.GetToUri().GetDomain();
	TASKLOG(CMU_MOD_SWITCH, EVENT_LEV, "DomainName[%s]\n", strDomainName.c_str());

	if (!IsLocalDomain(strDomainName) || DEV_TYPE_CMU == cOtherSwitchPin.GetDevType())
	{
		TDomainTable::const_iterator it = g_tDomainTable.find(cOtherSwitchPin.GetToUri().GetDomain());
		TDomainTable::const_iterator itParent = g_tDomainTable.find(DOMAIN_NAME_OF_PARENT);
		if (g_tDomainTable.end() != it)
		{
			strNetSegIp = it->second.GetLocalIP();
			TASKLOG(CMU_MOD_SWITCH, EVENT_LEV, "strNetSegIp[%s]\n", strNetSegIp.c_str());
		} 
		else if (g_tDomainTable.end() != itParent)
		{
			strNetSegIp = itParent->second.GetLocalIP();
			TASKLOG(CMU_MOD_SWITCH, WARNING_LEV, "Not Find Domain[%s] Info, using Parent@Domain Info, strNetSegIp[%s]\n", 
				cOtherSwitchPin.GetToUri().GetDomain().c_str(),
				strNetSegIp.c_str());
		}
		else
		{
			TASKLOG(CMU_MOD_SWITCH, ERROR_LEV, "Not Find Domain[%s] Info\n",
				cOtherSwitchPin.GetToUri().GetDomain().c_str());
		}
	}

	if (strNetSegIp.empty())
	{
		strNetSegIp = cOtherSwitchPin.GetSdp().GetInterfaceIp();
	}

	if (strNetSegIp.empty() && cInviteReq.GetSdp().GetInterfaceIp().empty() && UNIQUE_SIP_UA_CMU == cSwitchPinTo.GetToUri().GetUser())
	{
		TDomainTable::const_iterator it = g_tDomainTable.find(cSwitchPinTo.GetToUri().GetDomain());
		if (g_tDomainTable.end() != it)
		{
			strNetSegIp = it->second.GetPeerIP();
			TASKLOG(CMU_MOD_SWITCH, EVENT_LEV, "Peer strNetSegIp[%s]\n", strNetSegIp.c_str());
		}
	}

	TASKLOG(CMU_MOD_SWITCH, EVENT_LEV, "SrcChn[%s]\n", cInviteReq.GetSrcChn().ToString().c_str());
	if (strNetSegIp.empty() && g_mapInterfaceIpList.end() != g_mapInterfaceIpList.find(cInviteReq.GetSrcChn().ToString()))
	{
		strNetSegIp = g_mapInterfaceIpList.find(cInviteReq.GetSrcChn().ToString())->second;
		TASKLOG(CMU_MOD_SWITCH, EVENT_LEV, "SrcChn[%s], InterfaceIp[%s]\n",
			cInviteReq.GetSrcChn().ToString().c_str(), strNetSegIp.c_str());
	}

    cInviteReq.GetSdp().SetIsNeedPunch(bIsNeedPunch);
	TASKLOG(CMU_MOD_SWITCH, EVENT_LEV, "NetSegIp[%s] InterfaceIp[%s]\n",
		strNetSegIp.c_str(),
		cInviteReq.GetSdp().GetInterfaceIp().c_str());
    //cInviteReq.GetSdp().SetNetSegNo(nNetSegNo);
	if (!strNetSegIp.empty())
	{
		cInviteReq.GetSdp().SetInterfaceIp(strNetSegIp);
	}

    if (cSwitchPinTo.GetSdp().GetExtInfo().end() != cSwitchPinTo.GetSdp().GetExtInfo().find(SDP_EXT_INFO_TRANSFER_RATE))
    {
        cInviteReq.GetSdp().SetTransferRate(cSwitchPinTo.GetSdp().GetTransferRate());
    }

    //tSdp.PrintData(false);
	this->SetSendToSipUri(cSwitchPinTo.GetToUri());

    if(PROCMSG_OK != PostReq(KDSIP_EVENT_INVITE_REQ, cInviteReq, cSwitchPinTo.GetToUri()))
    {
        TASKLOG(CMU_MOD_SWITCH, ERROR_LEV, "发送KDSIP_EVENT_INVITE_REQ到%s失败！\n", cSwitchPinTo.GetToUri().GetNOString());

        this->SetErrorCode(ERR_POST_MSG_FAILED);
        return this->GetErrorCode();
    }

    m_pcCurInviting = (CSwitchPin*)&cSwitchPinTo;

    return this->GetErrorCode();
}

CMS_RESULT CSwitchTask::PostInviteAck(const CSwitchPin& cSwitchPinTo, const TSdp& tSdp)
{
	CInviteACK cInviteACK;
	cInviteACK.SetSession(cSwitchPinTo.GetDevSsnTask()->GetSession());
	cInviteACK.SetSdp(tSdp);

	if(PROCMSG_OK!= PostInDlgReq(KDSIP_EVENT_INVITE_ACK, cInviteACK, cSwitchPinTo.GetDlgID()))
	{
		TASKLOG(CMU_MOD_SWITCH, ERROR_LEV, "发送KDSIP_EVENT_INVITE_ACK失败！\n");

        this->SetErrorCode(ERR_POST_MSG_FAILED);
        return this->GetErrorCode();
	}

	return this->GetErrorCode();
}

bool CSwitchTask::IsNeedTrans(const CSwitchTask* ptSrcSwitchNode, const CSwitchTask* ptDstSwitchNode)
{
    return false;
}

CSwitchTask* CSwitchTask::ChooseMTU(const CSwitchTask* ptSrcSwitchNode, const CSwitchTask* ptDstSwitchNode)
{
    /*
    //TODO判断该源节点是否已经存在转码模块的节点
    if(! 存在转码节点)
    {
    //TODO从全局转码模块中选择一个转码通道
    //TODO创建一个MTU转码节点,并链接到交换列表中
    CSwitchTask* ptMtuSwitchNode = CreateSwitchNode();
    this->AddOutputSwitchNode(ptMtuSwitchNode);
    }
    */
    return NULL;
}

/*====================================================================
函数名      : IsCreditDomain
功能        ：指定域是否是信任域
算法实现    ：
引用全局变量：
输入参数说明：const TKDMNO& tDomainNO 制定域
返回值说明  ：true, false
----------------------------------------------------------------------
修改记录    ：
日  期			版本        修改人        修改内容
2011/11/04		1.0        黄至春        创建
====================================================================*/
bool CSwitchTask::IsCreditDomain(const TSipURI&  tDevUri)
{
    Iterator pPos;
    string strUri;
    CLoginSession* pTask = NULL;

    while (!pPos.End())
    {
        if (g_cVtduList.Iterate(pPos, strUri, pTask))
        {
            CVtduRegTask* pcVtdu = (CVtduRegTask*)pTask;
            if(pcVtdu->GetIsTravGAP() && tDevUri.GetDomain() == pcVtdu->GetCreditDomainUri())
            {
                return true;
            }
        }
    }

    return false;
}


bool CSwitchTask::IsCreditDomain(const string& strDevUri)
{
    TSipURI  tDevUri;
    if(tDevUri.SetURIString(strDevUri))
    {
        return IsCreditDomain(tDevUri);
    }
    else
    {
        return false;
    }
}

/*====================================================================
函数名      : ChooseDistribNode
功能        ：在当前源节点链路上为目的选择一个分发节点
算法实现    ：
引用全局变量：
输入参数说明：
            [IN]CSwitchTask* ptDstSwitchNode--码流目的节点
            [OUT]CSwitchTask* ptDistribNode--找到的码流分发节点
返回值说明  ：CMS_RESULT
----------------------------------------------------------------------
修改记录    ：
日  期			版本        修改人        修改内容
2011/11/04		1.0        黄至春        创建
====================================================================*/
CMS_RESULT CSwitchTask::ChooseDistribNode(CSwitchTask* ptDstSwitchNode, CSwitchTask*& ptRtnDistribNode)
{
    TRedisModuleResourceId tResourceId;

    //判断该源是否有VTDU
    if( this->GetReceiver().GetDevSsnTask() != NULL )//源存在VTDU
    {
        CMS_RESULT nErrorCode = ChooseUseableSrcNode(ptDstSwitchNode->GetSessionName(), ptDstSwitchNode->GetReceiver(), ptRtnDistribNode, tResourceId);
        ptDstSwitchNode->SetVtduResourceId(tResourceId);

        return nErrorCode;
    }
    else
    {
        //判断源是否需要穿越网闸
        if( IsCreditDomain(this->GetSender().GetChannel().GetDevUri()) )
        {
            TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "源[%s]需要穿网闸.\n", this->GetSender().GetChannel().ToString().c_str());

            this->GetSender().SetIsTravGAP(true);
        }

        this->ChooseVtduAsReceiver(this->GetSender().GetIsTravGAP(), ptDstSwitchNode->GetTransChnNum(), 
            ptDstSwitchNode->GetSessionName(), ptDstSwitchNode->GetReceiver(), tResourceId);

        ptRtnDistribNode = this;

        ptDstSwitchNode->SetVtduResourceId(tResourceId);

        return this->GetErrorCode();
    }    
}

/*====================================================================
函数名      ：ChooseUseableSrcNode
功能        ：从存在交换的源节点上选择一条可用的分发路径
算法实现    ：
引用全局变量：
输入参数说明：
返回值说明  ：
----------------------------------------------------------------------
修改记录    ：
日  期			版本        修改人        修改内容
2011/11/04		1.0        黄至春        创建
====================================================================*/
CMS_RESULT CSwitchTask::ChooseUseableSrcNode(const TSdpSessionName &tSessionName, CSwitchPin& cDstSwitchPin, CSwitchTask*& ptRtnDistribNode, TRedisModuleResourceId &tVtduResourceId)
{
    //查找有分发能力的交换源
    //注：链表的源头有可能是个穿网闸的VTDU（跨域源）;但在此链表中后续的VTDU均是不穿网闸的VTDU
    CSwitchTask *ptLastSrcBakSwitchData = NULL;
    CSwitchTask *ptSrcSwitchNode = this;

    TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "源[%s]存在码流转发，寻找可用的分发VTDU.\n", ptSrcSwitchNode->GetChannel().ToString().c_str());

    CRedisSdk &cRedisSdk = GetLocalRedisPipe().GetRedisSdk();
    const TChannel &tSrcChn = m_cSender.GetChannel();
    const TChannel &tDstChn = cDstSwitchPin.GetChannel();
    TRedisModuleResourceId tResourceId;
    for(; ptSrcSwitchNode != NULL; ptSrcSwitchNode = ptSrcSwitchNode->FindBakSrcByType(DEV_TYPE_VTDU))
    {
        ptLastSrcBakSwitchData = ptSrcSwitchNode;

        if (CMS_SUCCESS == cRedisSdk.AllocVtduResource(ptSrcSwitchNode->GetVtduResourceId(), tResourceId))
        {
            TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "找到结点(%s备份源)[%s]进行码流转发.\n", (this != ptSrcSwitchNode)? "是":"非", ptSrcSwitchNode->GetChannel().ToString().c_str());

            ptRtnDistribNode = ptSrcSwitchNode;
            tVtduResourceId  = tResourceId;

            return CMS_SUCCESS;
        }
    }

    if (ptLastSrcBakSwitchData == NULL)
    {
        return ERR_CMU_SWITCH_EXCEPTION;
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    //找不到可用源，则调度VTDU构建新备份源
    TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "该源已存在的VTDU转发能力已满，需要寻找一个新的VTDU进行发码流分发，构建新备份源.\n");

    //构建新备份源
    CSwitchTask* ptSrcNextBakSwitchData = g_cSwitchList.CreateInviteSenderSwitchTask(GetInstance());
    if(NULL == ptSrcNextBakSwitchData)
    {
        return ERR_CMU_SWITCH_EXCEPTION;
    }

    ptSrcNextBakSwitchData->InitSender(ptLastSrcBakSwitchData->GetSender().GetChannel(), ptLastSrcBakSwitchData->GetSessionName());
    TInviteSubject tInviteSubject(ptSrcNextBakSwitchData->GetReceiver().GetChannel(), this->GetSubject().GetSrcChn());
    ptSrcNextBakSwitchData->SetSubject(tInviteSubject);
    ptSrcNextBakSwitchData->SetSessionName(this->GetSessionName());
    ptSrcNextBakSwitchData->SetSwitchMode(this->GetSwitchMode());
    //拷贝Input的Sdp基本信息，因为MakeLink交换当前备份源到上一个源节点时需要计算通道数
    ptSrcNextBakSwitchData->SetSdp(ptLastSrcBakSwitchData->GetSender().GetSdp());

    CMS_RESULT nErrorCode = ptSrcNextBakSwitchData->ChooseVtduAsReceiver(false, ptLastSrcBakSwitchData->GetTransChnNum()*2, 
        tSessionName, cDstSwitchPin, tResourceId);
    if(CMS_SUCCESS != nErrorCode)
    {
        TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "为源寻找新的备份源VTDU时，找不到可用的VTDU.\n");

        SAFE_DEL_PTR(ptSrcNextBakSwitchData);

        return nErrorCode;
    }

    TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "为源寻找到新的备份源VtduChn[%s].\n", ptSrcNextBakSwitchData->GetReceiver().GetChannel().ToString().c_str());

    /////////////////////////////////////////////////////////////////////////////////////////
    //停止到原目的的交换,寻找其中一路目的
    CSwitchTask *ptLastDstSwitchData = (CSwitchTask*)ptLastSrcBakSwitchData->GetOutputList().back();
    if(NULL == ptLastDstSwitchData)
    {
        CMS_ASSERT( ptLastDstSwitchData);

        return ERR_CMU_SWITCH_EXCEPTION;
    }

    TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "在源[%s]上寻找到需要停止的最后一路交换输出[%s].\n", 
        ptLastSrcBakSwitchData->GetChannel().ToString().c_str(),
        ptLastDstSwitchData->GetChannel().ToString().c_str());

    if(!ptLastDstSwitchData->StopSwitchToSrc(true))
    {
        TASKLOG(CMU_MOD_SWITCH, ERROR_LEV, "交换时为构建新的备份源停止原有交换Src[%s]--Dst[%s]失败.\n", 
            ptLastSrcBakSwitchData->GetChannel().ToString().c_str(),
            ptLastDstSwitchData->GetChannel().ToString().c_str());

        return ERR_CMU_SWITCH_STOP_SWITCH_FOR_NEW_BAK_SRC_FAILED;
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    //交换到新调度的备份源
    if(CMS_SUCCESS != MakeLink(ptLastSrcBakSwitchData, ptSrcNextBakSwitchData))
    {
        TASKLOG(CMU_MOD_SWITCH, ERROR_LEV, "从原源交换结点交换到新备份源Src[%s]--Dst[%s]失败，删除需要链在其之后的交换任务.\n", 
            ptLastSrcBakSwitchData->GetChannel().ToString().c_str(),
            ptSrcNextBakSwitchData->GetChannel().ToString().c_str());

        //备份源交换失败，删除需要链在其之后的交换任务。
        //TODO后续最好改成能传入错误码的接口（如果bye(nErrCode)）。
        SAFE_DEL_PTR(ptSrcNextBakSwitchData);
        SAFE_DEL_PTR(ptLastDstSwitchData);

        return ERR_CMU_SWITCH_ADD_BAK_SRC_FAILED;
    }

    TASKLOG(CMU_MOD_SWITCH, ERROR_LEV, "从原源交换结点交换到新备份源Src[%s]--Dst[%s]成功.\n", 
        ptLastSrcBakSwitchData->GetChannel().ToString().c_str(),
        ptSrcNextBakSwitchData->GetReceiver().GetChannel().ToString().c_str());

    /////////////////////////////////////////////////////////////////////////////////////////
    //从新备份源交换到被拆除的目的
    if (g_cCmuConf.m_bEnablePunch)
    {
        //如果是需要punch协商地址的，直接把最后一路拆除，有目的端(CU)重新发起交换
        ptLastDstSwitchData->SetErrorCode(ERR_CMU_SWITCH_AGAIN);
        SAFE_DEL_PTR(ptLastDstSwitchData);
    }
    else
    {
        //如果是不需要punch协商地址的，直接把最后一路链接到备份原源结点上
        if(CMS_SUCCESS == MakeLink(ptSrcNextBakSwitchData, ptLastDstSwitchData))
        {
            TASKLOG(CMU_MOD_SWITCH, ERROR_LEV, "从新的备份源[%s]上交换一路到原拆除的目的输出[%s]成功.\n", 
                ptSrcNextBakSwitchData->GetChannel().ToString().c_str(),
                ptLastDstSwitchData->GetChannel().ToString().c_str());
        }
        else
        {
            TASKLOG(CMU_MOD_SWITCH, ERROR_LEV, "从新的备份源[%s]上交换一路到原拆除的目的输出[%s]失败，删除该交换任务.\n", 
                ptSrcNextBakSwitchData->GetChannel().ToString().c_str(),
                ptLastDstSwitchData->GetChannel().ToString().c_str());

            //删除该交换任务
            SAFE_DEL_PTR(ptLastDstSwitchData);

            return ERR_CMU_SWITCH_ADD_BAK_SRC_FAILED;
        }
    }

    //返回新调度的备份源
    ptRtnDistribNode = ptSrcNextBakSwitchData;
    tVtduResourceId  = tResourceId;

    return CMS_SUCCESS;
}

CMS_RESULT CSwitchTask::ChooseVtduAsReceiver(bool bIsTravGAP, u32 dwSwitchNumToAdd, const TSdpSessionName &tSessionName, 
    CSwitchPin& cDstSwitchPin, TRedisModuleResourceId &tVtduResourceId)
{
    ECMS_ERRORCODE emRet = CMS_SUCCESS;

    //选择相应类型的VTDU
    CRedisSdk &cRedisSdk = GetLocalRedisPipe().GetRedisSdk();
    const TChannel &tSrcChn = m_cSender.GetChannel();
    const TChannel &tDstChn = cDstSwitchPin.GetChannel();
    TSipURI tVtduUri;
    TRedisModuleResourceId   tResourceId;
    CVtduRegTask *pcRegTask = NULL;
    TRedisModuleVtduResource tResource;
    string strId, strDomainName;

    string strDstAddr;
    if (cDstSwitchPin.GetDevType() == DEV_TYPE_RCS)
    {
        bool bFoundDstAddr = false;
        const TMediaDescList &tMediaList = cDstSwitchPin.GetSdp().GetMediaDescList();
        for (TMediaDescList::const_iterator it4Media = tMediaList.begin();
            !bFoundDstAddr && tMediaList.end() != it4Media; ++it4Media)
        {
            if (MEDIA_TYPE_VIDEO == it4Media->GetMediaType() || MEDIA_TYPE_VIDEO_LOW == it4Media->GetMediaType())
            {
                const TMediaTransChannelList &tTransList = it4Media->GetMediaTransChnList();
                for (TMediaTransChannelList::const_iterator it4Trans = tTransList.begin(); 
                    !bFoundDstAddr && tTransList.end() != it4Trans; ++it4Trans)
                {
                    if (MEDIA_TRANS_CHN_RTP == it4Trans->GetTransChnType())
                    {
                        const TNetAddrList &tAddrList = it4Trans->GetAddrList();
                        if (!tAddrList.empty())
                        {
                            strDstAddr = tAddrList.at(0).GetNetIp();
                        }
                        
                        bFoundDstAddr = true;
                    }                    
                }
            }
        }
    }
    
	bool bSameIpVtdu = false;
    if (!strDstAddr.empty())
    {
        emRet = cRedisSdk.SelectVtduEx(CRedisPipe::SdpSessionName2RedisSwitchType(tSessionName), 
            TRedisSwitchAgent(TSipURI(tSrcChn.GetDevUri()).GetUser(), tSrcChn.GetChnNO()), 
            TRedisSwitchAgent(TSipURI(tDstChn.GetDevUri()).GetUser(), tDstChn.GetChnNO()), 
            strDstAddr, bSameIpVtdu, strId, strDomainName, tResourceId, &tResource);
    }
    else
    {
        emRet = cRedisSdk.SelectVtdu(CRedisPipe::SdpSessionName2RedisSwitchType(tSessionName), 
            TRedisSwitchAgent(TSipURI(tSrcChn.GetDevUri()).GetUser(), tSrcChn.GetChnNO()), 
            TRedisSwitchAgent(TSipURI(tDstChn.GetDevUri()).GetUser(), tDstChn.GetChnNO()), 
            strId, strDomainName, tResourceId, &tResource);
    }
    
    if (CMS_SUCCESS == emRet)
    {
        tVtduUri.SetUser(strId);
        tVtduUri.SetDomain(strDomainName);
        pcRegTask = dynamic_cast<CVtduRegTask *>(GetLoginSession(tVtduUri.GetURIString()));
    }

    if (CMS_SUCCESS == emRet && NULL != pcRegTask)
    {
        TChannel tVtduChn;
        tVtduChn.SetDevUri(tVtduUri.GetURIString());
        tVtduChn.SetChnNO(tResource.dwPortChn);

        TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "找到可用的Vtdu[%s]，Resource[%s - %s].\n", 
            tVtduUri.GetURIString().c_str(), tResourceId.c_str(), tResource.ToJson().c_str());

        //把VTDU作为交换结点
        InitReceiver(tVtduChn, tSessionName, pcRegTask);
        GetSubject().SetDstChn(tVtduChn);
        SetVtduResourceId(tResourceId);
        tVtduResourceId = tResourceId;
    }
    else
    {
        TASKLOG(CMU_MOD_SWITCH, ERROR_LEV, "找不到可用的VTDU!\n");

        if (!tResourceId.empty())               // 释放资源
        {
            cRedisSdk.ReleaseVtduResource(tResourceId);
        }

        if(bIsTravGAP)
        {
            SetErrorCode(ERR_CMU_SWITCH_GAP_VTDU_NOT_AVAILABLE);
        }
        else
        {
            SetErrorCode(ERR_CMU_SWITCH_VTDU_NOT_AVAILABLE);
        }
    }

    return GetErrorCode();
}

/*====================================================================
函数名      ：StartSwitchToSrc
功能        ：把当前结点与指定源结点链接并且添加地址交换规则到vtdu中。
算法实现    ：
引用全局变量：
输入参数说明：CSwitchNode* ptSrcSwitchNode--码流源节点
返回值说明  ：true, false
----------------------------------------------------------------------
修改记录    ：
日  期			版本        修改人        修改内容
2011/11/04		1.0        黄至春        创建
====================================================================*/
CMS_RESULT CSwitchTask::StartSwitchToSrc(CSwitchTask* ptSrcSwitchNode)
{
    TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "StartSwitchToSrc([%s]).\n", ptSrcSwitchNode->GetChannel().ToString().c_str());

    if(SWITCH_STATE_SWITCHED == ptSrcSwitchNode->GetSwitchState())
    {
        //m_cSender.GetSdp().CopyBasicInfoFrom( ptSrcSwitchNode->GetReceiver().GetSdp() );
        this->InitSender(ptSrcSwitchNode->GetReceiver().GetChannel(), ptSrcSwitchNode->GetSessionName(), ptSrcSwitchNode->GetReceiver().GetDevSsnTask());

		TSdp tSdp = m_cReceiver.GetSdp();
		if ( !IsRealTimeMediaSession(this->GetSessionName()) )
		{
			tSdp.SetPlaybackSessUri(ptSrcSwitchNode->GetSender().GetSdp().GetPlaybackSessUri());
		}

		// vtdu和nru部署在同一台机器上的时候，进行域socket协商
		if (m_cReceiver.GetDevType() == DEV_TYPE_RCS)
		{
			CVtduRegTask *pcVtdu = dynamic_cast<CVtduRegTask *>(m_cSender.GetDevSsnTask());
			if (NULL != pcVtdu)
			{
				for (CIpList::const_iterator itr = pcVtdu->GetIpAddrList().begin();
					itr != pcVtdu->GetIpAddrList().end(); itr++)
				{
					if(m_cReceiver.GetSdp().IsExistIP(*itr))
					{
						TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "目的交换中，源和目的端在同一机器上，协商使用域socket!\n");

						tSdp.SetDomainSocket(true);
						break;
					}
				}
			}
		}

        // 设置资源id和交换流程
        tSdp.SetResourceId(m_tVtduResourceId);
        tSdp.SetSwitchFlow(SWITCH_FLOW_CMU);

        InviteTo(m_cSender, tSdp);
    }
    else
    {
        TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "SrcSwitch has not complete: [%s]!\n", GetSwitchStateStr(ptSrcSwitchNode->GetSwitchState()));
    }

    return this->GetErrorCode();
}


CMS_RESULT CSwitchTask::ProcEndOfFileReq(const COspSipMsg*const pcOspSipMsg)
{
    CMS_RESULT nErrorCode = CMS_SUCCESS;

    return nErrorCode;
}

CMS_RESULT CSwitchTask::ProcEndOfFileRsp(const COspSipMsg*const pcOspSipMsg)
{
    CMS_RESULT nErrorCode = CMS_SUCCESS;

    return nErrorCode;
}

//CMS_RESULT CSwitchTask::ProcKeyFrameReq(CMessage *const pcMsg)
//{
//    CMS_RESULT nErrorCode = CMS_SUCCESS;
//    COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
//
//    CKeyFrameReq cKeyFrameReq;
//    pcOspSipMsg->GetMsgBody(cKeyFrameReq);
//
//    if (this->GetInput() != NULL)
//    {
//        CallbackToTask(pcMsg, this->GetInput());
//    }
//    else
//    {
//        if (!DoPostInDlgReq(KDSIP_EVENT_MESSAGE_REQ, cKeyFrameReq, this->GetSender().GetDlgID()))
//        {
//            nErrorCode = ERR_POST_MSG_FAILED;
//        }
//    }
//
//    return nErrorCode;
//}
//
//CMS_RESULT CSwitchTask::ProcKeyFrameRsp(CMessage *const pcMsg)
//{
//    CMS_RESULT nErrorCode = CMS_SUCCESS;
//
//    return nErrorCode;
//}
////////////////////////////////////////////////////////////////////////////////////////////////
//停止交换业务处理
////////////////////////////////////////////////////////////////////////////////////////////////

bool CSwitchTask::ProcDisconnect(const TDevOnOffCBMsg* ptDevOnOffCBMsg)
{
    bool bProcess = false;
    LPCSTR lpszDevUri = ptDevOnOffCBMsg->GetDevUri().c_str();
    string strDevType = ptDevOnOffCBMsg->GetDevType();

    CLoginSession* pSenderSsnTask = this->GetSender().GetDevSsnTask();
    CLoginSession* pReceiverSsnTask = this->GetReceiver().GetDevSsnTask();
    

    if (NULL != pSenderSsnTask && pSenderSsnTask->GetSession() == lpszDevUri)
    {
        strDevType = pSenderSsnTask->GetDevType();

        TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "Sender[%s] DevType[%s] 设备下线\n!",
            lpszDevUri, strDevType.c_str());

        //下线设备不需要发送Bye请求，这里把Dialog资源释放并重置，不再发送ByeReq.
        if (INVALID_DIALOG_ID != this->GetSender().GetDlgID())
        {
            OspSipReleaseDlg(this->GetSender().GetDlgID());
            this->GetSender().SetDlgID(INVALID_DIALOG_ID);
        }

        bProcess = true;
    }
    else if (NULL != pReceiverSsnTask && pReceiverSsnTask->GetSession() == lpszDevUri)
    {
        strDevType = pReceiverSsnTask->GetDevType();

        TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "Receiver[%s] DevType[%s] 设备下线\n!",
            lpszDevUri, strDevType.c_str());


        //下线设备不需要发送Bye请求，这里把Dialog资源释放并重置，不再发送ByeReq.
        if (INVALID_DIALOG_ID != this->GetReceiver().GetDlgID())
        {
            OspSipReleaseDlg(this->GetReceiver().GetDlgID());
            this->GetReceiver().SetDlgID(INVALID_DIALOG_ID);
        }

        bProcess = true;
    }
    else
    {
        TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "Unknown Dev[%s]下线\n!", lpszDevUri);
    }

    if (DEV_TYPE_VTDU == strDevType)
    {
        this->SetErrorCode(ERR_CMU_SWITCH_AGAIN);
    }
    else
    {
        this->SetErrorCode(ERR_CMU_SWITCH_DEV_NOT_AVAILABLE);
    }

    return bProcess;
}

bool CSwitchTask::ProcOspSipException(CMessage *const pcMsg)
{
    switch(pcMsg->event)
    {
        //Invite会话终止
    case OSP_SIP_MSG_PROC_FAIL:
        {
            //发起的请求收到SIP层的错误应答
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
            this->SetErrorCode(ERR_CMU_DEV_NO_ROUTER);

            TASKLOG(this->GetModuleID(), ERROR_LEV, "收到SipMethod[%s]SIP层错误应答, SipErrorCode[%d]\n",
                pcOspSipMsg->GetSipEventStr().c_str(), pcOspSipMsg->GetSipErrorCode());

            //this->PrintData();
        }
        break;
    case OSP_SIP_DIALOG_TERMINATE:
        {
            //SIP层Dialog异常终止通知
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
            TKDSIP_DIALOG_ID tDlgId = pcOspSipMsg->GetSipDlgID();

            CSwitchPin& cSwitchPin = GetSwitchPin(tDlgId);
            cSwitchPin.SetDlgID(INVALID_DIALOG_ID);
            this->SetErrorCode(ERR_CMU_DIALOG_TERMINATE);

			this->SetDialogTerminateErrocodeBySender(this->GetSendToSipUri());
			this->SendOptFailedToUas(this->GetSessionName());

            TASKLOG(this->GetModuleID(), ERROR_LEV, "收到设备[%s]DlgID[%lu]SIP层Dialog异常终止, SipFrom[%s], SipErrorCode[%d], StatName[%s], ErrorCode[%d]\n",
                cSwitchPin.GetChannel().ToString().c_str(), tDlgId, this->GetSendToSipUri().GetURIString().c_str(),
				pcOspSipMsg->GetSipErrorCode(), this->GetStateName(this->GetState()), this->GetErrorCode());
        }
        break;

    default:
        {
            TASKLOG(this->GetModuleID(), WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

            return false;
        }
    }

    return true;
}

bool CSwitchTask::ProcByeReq(CMessage *const pcMsg)
{
    COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;

    s32 nByeReason = CMS_SUCCESS;
    if (pcOspSipMsg->GetMsgBodyLen() != 0)
    {
        CByeReq cByeReq;
        pcOspSipMsg->GetMsgBody(cByeReq);

        nByeReason = cByeReq.GetReason();
    }

    if (CMS_SUCCESS != nByeReason)
    {
        this->SetErrorCode(nByeReason);
    }
    else
    {
        this->SetErrorCode(ERR_CMU_SWITCH_STOP_BY_BYE);
    }

    TKDSIP_DIALOG_ID tReqDlgId = pcOspSipMsg->GetSipDlgID();

	CSwitchPin& cSwitchPin = GetSwitchPin(tReqDlgId);

	TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "收到设备[%s]DlgID[%lu]的ByeReq, Reason[%d]！\n", 
        cSwitchPin.GetDevDesc().c_str(), tReqDlgId, nByeReason);

	cSwitchPin.SetDlgID(INVALID_DIALOG_ID);

    ////////////////////////////////////////////////////////////
    //向请求方回应应答
    CByeRsp cRsp;

    u32 dwResult = PostRsp(KDSIP_EVENT_INVITE_BYE_RSP, pcOspSipMsg->GetSipTransID(), cRsp);
    TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "发送KDSIP_EVENT_INVITE_BYE_RSP, ResultCode[%lu].\n", dwResult);

    return true;
}

CMS_RESULT CSwitchTask::ProcByeRsp(const COspSipMsg*const pcOspSipMsg)
{
    TKDSIP_DIALOG_ID tReqDlgId = pcOspSipMsg->GetSipDlgID();

	//CSwitchPin& cSwitchPin = GetSwitchPin(tReqDlgId); //因为发送时已经给SwitchPin设置INVALID_DIALOG_ID，所以找不到。
	TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "收到DlgID[%lu]的ByeRsp！\n", tReqDlgId);

    return CMS_SUCCESS;
}

void CSwitchTask::ReleaseResource()
{
    CCmuTask::ReleaseResource();

    //销毁对话内子事务
    Iterator pPos;
    int nSeqNo = 0;
    CTask* pcTask = NULL;
    while(!pPos.End())
    {
        if (m_cChildTaskList.Iterate(pPos, nSeqNo, pcTask) && pcTask != NULL)
        {
            delete pcTask;
            pcTask = NULL;
        }
    }

    UpdateDevServiceState(false);

    //开始拆除交换
    StopAllSwitch();

    //处理对话资源：检查是否应答过
    if(INVALID_TRANS_ID != this->GetSipTransID())
    {
        //任务还未完成（未应答）就销毁，肯定是失败了，错误码不应该还是CMS_SUCCESS
        if (this->GetErrorCode() == CMS_SUCCESS)
        {
            //CMS_ASSERT(false && "任务还未完成（未应答）就销毁，肯定是失败了，错误码不应该还是CMS_SUCCESS");
            this->SetErrorCode(ERR_CMU_SWITCH_EXCEPTION);
        }

        //发送应答
        CInviteRsp cRsp;
        DoPostRsp(KDSIP_EVENT_INVITE_RSP, cRsp);
    }

    /////////////////////////////////////////////////////////////////////////////////
    //清理Receiver端资源
    CLoginSession* pDevSsnTask = m_cReceiver.GetDevSsnTask();
    if(pDevSsnTask)
    {
        pDevSsnTask->DelTask(this);   //删除交换任务与设备登录会话的依赖关系

		//向Receiver端发送Bye请求
		if(INVALID_DIALOG_ID != m_cReceiver.GetDlgID())
		{
			CByeReq cReq;
			cReq.SetSession(pDevSsnTask->GetSession());
			cReq.SetReason(this->GetErrorCode());

			u32 dwResult = PostInDlgReq(KDSIP_EVENT_INVITE_BYE_REQ, cReq, m_cReceiver.GetDlgID());

			TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "发送KDSIP_EVENT_INVITE_BYE_REQ到设备(DlgID[%ld]--DevChn[%s]) Reason[%d], ResultCode[%lu].\n", 
				m_cReceiver.GetDlgID(), m_cReceiver.GetChannel().ToString().c_str(), this->GetErrorCode(), dwResult);

			m_cReceiver.SetDlgID(INVALID_DIALOG_ID);
		}
    }

    //////////////////////////////////////////////////////////////////////////////////////
    //清理Sender端资源
    pDevSsnTask = m_cSender.GetDevSsnTask();
    if(pDevSsnTask)
    {
        if (SWITCH_MODE_DRT == this->GetSwitchMode())
        {
            pDevSsnTask->SubCurDirectTransNum();
        }
        pDevSsnTask->DelTask(this);   //删除交换任务与设备登录会话的依赖关系

		//向Sender端发送Bye请求
		if(INVALID_DIALOG_ID != m_cSender.GetDlgID())
		{
			CByeReq cReq;
			cReq.SetSession(pDevSsnTask->GetSession());
			cReq.SetReason(this->GetErrorCode());

			u32 dwResult = PostInDlgReq(KDSIP_EVENT_INVITE_BYE_REQ, cReq, m_cSender.GetDlgID());

			TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "发送KDSIP_EVENT_INVITE_BYE_REQ到设备(DlgID[%ld]--DevChn[%s]) Reason[%d], ResultCode[%lu].\n", 
				m_cSender.GetDlgID(), m_cSender.GetChannel().ToString().c_str(), this->GetErrorCode(), dwResult);

			m_cSender.SetDlgID(INVALID_DIALOG_ID);
		}
    }

    // 释放redis中的vtdu转发资源
    if (!m_tVtduResourceId.empty())
    {
        CRedisSdk &cRedisSdk = GetLocalRedisPipe().GetRedisSdk();
        cRedisSdk.ReleaseVtduResource(m_tVtduResourceId);
    }

    //从交换列表中删除该结点
    g_cSwitchList.EraseSwitchNode(this);
}

bool CSwitchTask::StopAllSwitch()
{
    //解除跟CreatorTask的依赖关系
    if(m_pcCreatorTask)
    {
        m_pcCreatorTask->SetDstSwitchTask(NULL);

        m_pcCreatorTask->SetErrorCode(this->GetErrorCode());
        SAFE_DEL_PTR(m_pcCreatorTask);
    }

    //遍历该节点的所有目的交换节点，并递归停止交换
    CSwitchTaskList::iterator itrOutputNode = this->GetOutputList().begin();
    while(itrOutputNode != this->GetOutputList().end())
    {
        CSwitchTask* pcOutputNode = *itrOutputNode;
        itrOutputNode++;    //由于StopSwitchToSrc函数中可能会从cOutputNodeList中删除相应的元素，所以要先++

        if(pcOutputNode)
        {
            ((CSwitchTask*)pcOutputNode)->StopSwitchToSrc(false);

            pcOutputNode->SetErrorCode(this->GetErrorCode());
            delete pcOutputNode;
        }
    }

    CSwitchTask* pcInput = this->GetInput();
    if(pcInput)
    {
        StopSwitchToSrc(false);
        if(pcInput->GetOutputList().size() == 0)
        {
            pcInput->SetErrorCode(this->GetErrorCode());
            delete pcInput;
        }
    }

    return true;
}

/*====================================================================
函数名      ：StopSwitchToSrc
功能        ：拆除当前交换结点与源结点之间的所有交换。
算法实现    ：
引用全局变量：
输入参数说明：无
返回值说明  ：true, false
----------------------------------------------------------------------
修改记录    ：
日  期			版本        修改人        修改内容
2011/11/04		1.0        黄至春        创建
====================================================================*/
bool CSwitchTask::StopSwitchToSrc(bool bByeToSender)
{
    CSwitchTask* ptDstSwitchNode = this;

    CSwitchTask* ptSrcSwitchNode = this->GetInput();
    if(NULL == ptSrcSwitchNode)
    {
        TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "该交换结点没有交换源，无需停止到源的交换\n");
        return true;
    }

    //删除节点间的依赖关系
    //BreakLink(ptSrcSwitchNode, ptDstSwitchNode);
    ptSrcSwitchNode->RemoveOutput(ptDstSwitchNode);

    if (bByeToSender)
    {
        CLoginSession* pDevSsnTask = m_cSender.GetDevSsnTask();
        if(pDevSsnTask)
        {
            pDevSsnTask->DelTask(this);   //删除交换任务与vtdu任务的依赖关系
        }

        //向Sender端发送Bye请求
        if(INVALID_DIALOG_ID != m_cSender.GetDlgID())   //TODO这里还可以加个状态判断，如果会话还未完成（InviteReq状态）就删除交换，没必要发送ByeReq，即使发送也会失败
        {
            CByeReq cReq;

            u32 dwResult = PostInDlgReq(KDSIP_EVENT_INVITE_BYE_REQ, cReq, m_cSender.GetDlgID());

            TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "发送KDSIP_EVENT_INVITE_BYE_REQ到设备(DlgID[%ld]--DevChn[%s]), ResultCode[%lu].\n", 
                m_cSender.GetDlgID(), m_cSender.GetChannel().ToString().c_str(), dwResult);

            m_cSender.SetDlgID(INVALID_DIALOG_ID);
            m_cSender.Clear();
        }
    }

    return true;
}

void CSwitchTask::UpdateDevServiceState(bool bStart)
{
    //没有输出结点，即为目的结点
    if (this->GetOutputList().size() == 0)
    {
        //更新 语音呼叫 目的结点的呼叫状态
        if (this->GetReceiver().GetSdp().GetSessionName() == SDP_SESSION_AUDIOCALL)
        {
            CLoginSession* pcPuRegTask = this->GetReceiver().GetDevSsnTask();

            if (NULL != pcPuRegTask && pcPuRegTask->GetDevType() == DEV_TYPE_PU)
            {
                CPuData *pcPuData = GetPuDataByUri(GetReceiver().GetChannel().GetDevUri());
                if (NULL != pcPuData)
                {
                    s32 nChnNO = GetReceiver().GetChannel().GetChnNO();
                    TChannel tChn;
                    if (bStart)
                    {
                        CSwitchTask *pcSwitchTask = GetTopSrc();
                        if (NULL != pcSwitchTask)
                        {
                            tChn = pcSwitchTask->GetChannel();
                        }
                        else
                        {
                            TASKLOG(CMU_MOD_SWITCH, ERROR_LEV, "this->GetTopSrc() == NULL\n");
                        }
                    }
                    
                    //更新状态
                    pcPuData->SetIsPuAudCall(GetReceiver().GetChannel().GetChnNO(), tChn);

                    TPuServiceStatus tPuServiceStatus;
                    tPuServiceStatus.GetAudioCallSrc().insert(map<int,TChannel>::value_type(nChnNO, tChn));
                    pcPuData->PublishRedisService(tPuServiceStatus);
                }
            }
        }
    }
}

void CSwitchTask::PrintData(ESwitchTrace eTrace)const
{
    CCmuTask::PrintData();
    OspPrintf(TRUE, FALSE, "\n");

    OspPrintf(TRUE, FALSE, "m_tSubject:[%s]\n", m_tSubject.ToString().c_str());
    OspPrintf(TRUE, FALSE, "m_tSessionName:[%s]\n", m_tSessionName.c_str());
    OspPrintf(TRUE, FALSE, "m_tSwitchMode:[%s]\n", m_tSwitchMode.c_str());
    OspPrintf(TRUE, FALSE, "m_tVtduResourceId:[%s]\n", m_tVtduResourceId.c_str());
    OspPrintf(TRUE, FALSE, "\n");

    OspPrintf(TRUE, FALSE, "Receiver:\n");
    m_cReceiver.PrintData();
    OspPrintf(TRUE, FALSE, "Sender:\n");
    m_cSender.PrintData();

    OspPrintf(TRUE, FALSE, "m_tOutputList: [%d]\n", m_tOutputList.size());
    OspPrintf(TRUE, FALSE, "m_pcInput: 0x%X\n", m_pcInput);

    if(eTrace & TRACE_UPWARDS)
    {
        OspPrintf(TRUE, FALSE, ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>> \n");
        if(m_pcInput)
        {
            m_pcInput->PrintData(TRACE_UPWARDS);
        }
    }

    if(eTrace & TRACE_DOWNWARDS)
    {
        OspPrintf(TRUE, FALSE, "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< \n");
        for(CSwitchTaskList::const_iterator itr = m_tOutputList.begin(); itr!=m_tOutputList.end(); itr++)
        {
            (*itr)->PrintData(TRACE_DOWNWARDS);
        }
    }
}

void CSwitchTask::PrintData() const
{
    PrintData(TRACE_ALL);
}

CSwitchTask* CSwitchList::CreateInviteReceiverSwitchTask(CInstExt *pcInst)
{
    CInviteReceiverSwitchTask* pcSwitchTask = new CInviteReceiverSwitchTask(pcInst);
    CMS_ASSERT(pcSwitchTask && "new CInviteReceiverSwitchTask failed!");

    pcSwitchTask->InitStateMachine();

    push_back(pcSwitchTask);//Insert(pcSwitchNode);

    CLASSLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "创建CSwitchTask: TaskNO[%lu].\n", pcSwitchTask->GetTaskNO());

    return pcSwitchTask;
}

CSwitchTask* CSwitchList::CreateInviteSenderSwitchTask(CInstExt *pcInst)
{
    CInviteSenderSwitchTask* pcSwitchTask = new CInviteSenderSwitchTask(pcInst);
    CMS_ASSERT(pcSwitchTask && "new CInviteSenderSwitchTask failed!");

    pcSwitchTask->InitStateMachine();

    push_back(pcSwitchTask);//Insert(pcSwitchNode);

    CLASSLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "创建CSwitchTask: TaskNO[%lu].\n", pcSwitchTask->GetTaskNO());

    return pcSwitchTask;
}

void CSwitchList::EraseSwitchNode(CSwitchTask* pSwitchNode)
{
    this->remove(pSwitchNode);//this->Erase(pSwitchNode->GetChannel().ToString());
}


//******************************************************************************
// 函数名:    FindSwitchNode
// 功能:      
// 算法实现:  参数tChannelType、tSessionName为空，代表不需要匹配
// 访问权限:  public 
// 参数说明:  const TChannel & tDevChn
// 参数说明:  const TChannelType & tChannelType
// 参数说明:  const TSdpSessionName & tSessionName
// 参数说明:  const TSwitchMode & tSwitchMode
// 返回值:    CSwitchTask*
//-----------------------------------------------------------
// 修改记录:
// 日      期     修改人        走读人          修改记录
// 2012/03/30     huangzhichun huangzhichun    创建
//******************************************************************************
CSwitchTask* CSwitchList::FindSwitchNode(const TChannel& tDevChn, const TChannelType& tChannelType, const TSdpSessionName& tSessionName, const TSwitchMode& tSwitchMode) const
{
    for(CSwitchTaskList::const_iterator itr = this->begin(); itr != this->end(); itr++)
    {
        CSwitchTask* pcSwitchNode = *itr;

        if ( tChannelType.empty() || CHANNEL_TYPE_RECVONLY == tChannelType)
        {
            if (pcSwitchNode->GetReceiver().GetChannel() == tDevChn
                && (tSessionName.empty() || tSessionName == pcSwitchNode->GetSessionName()) 
                && tSwitchMode == pcSwitchNode->GetSwitchMode()
				)//pcSwitchNode->GetReceiver().GetSdp().GetSessionName()
            {
                return pcSwitchNode;
            }
        }

        if ( tChannelType.empty() || CHANNEL_TYPE_SENDONLY == tChannelType)
        {
            if (pcSwitchNode->GetSender().GetChannel() == tDevChn
                && (tSessionName.empty() || tSessionName == pcSwitchNode->GetSessionName())
				&& tSwitchMode == pcSwitchNode->GetSwitchMode()
                )
            {
                return pcSwitchNode;
            }
        }
    }

    return NULL;
}

/*******************************************************************************
*根据视频Rtp地址查找匹配的目的交换task。
*只处理实时浏览，其他类型（如音频呼叫）暂不处理。
*******************************************************************************/
CSwitchTask* CSwitchList::FindDstSwitchNode(TSdp& tDstSdp) const
{
	if (SDP_SESSION_PLAY != tDstSdp.GetSessionName())
	{//只判断实时浏览
		return NULL;
	}

	TNetAddr tVidRtpAddr;
	TMediaDescList& tMediaDescList = tDstSdp.GetMediaDescList();
	for(TMediaDescList::iterator itrMediaDesc = tMediaDescList.begin(); 
		itrMediaDesc!=tMediaDescList.end(); itrMediaDesc++)
	{
		if (itrMediaDesc->GetMediaType() == MEDIA_TYPE_VIDEO)
		{
			TMediaTransChannelList& tMediaTransChnList = itrMediaDesc->GetMediaTransChnList();
			for(TMediaTransChannelList::iterator itrTransChn = tMediaTransChnList.begin(); 
				itrTransChn!=tMediaTransChnList.end(); itrTransChn++)
			{
				if (itrTransChn->GetTransChnType() == MEDIA_TRANS_CHN_RTP)
				{
					if (itrTransChn->GetAddrList().empty())
					{
						CLASSLOG(CMU_MOD_SWITCH, ERROR_LEV, "AddrList empty\n");
						continue;
					}
					tVidRtpAddr = itrTransChn->GetAddrList().at(0);
				}
			}
		}
	}

	if (tVidRtpAddr.GetNetIp().empty() || 0 == tVidRtpAddr.GetNetPort())//地址无效，无需继续判断
	{
		return NULL;
	}

    for(CSwitchTaskList::const_iterator itr = this->begin(); itr != this->end(); itr++)
    {
        CSwitchTask* pcSwitchNode = *itr;

		TNetAddr tTmpRtpAddr;
		TNetAddr tTmpRtcpAddr;

		TMediaDescList& tTmpMediaDescList = pcSwitchNode->GetReceiver().GetSdp().GetMediaDescList();
		for(TMediaDescList::iterator itrTmpMediaDesc = tTmpMediaDescList.begin(); 
			itrTmpMediaDesc!=tTmpMediaDescList.end(); itrTmpMediaDesc++)
		{
			if (itrTmpMediaDesc->GetMediaType() == MEDIA_TYPE_VIDEO)
			{
				TMediaTransChannelList& tTmpMediaTransChnList = itrTmpMediaDesc->GetMediaTransChnList();
				for(TMediaTransChannelList::iterator itrTmpTransChn = tTmpMediaTransChnList.begin(); 
					itrTmpTransChn!=tTmpMediaTransChnList.end(); itrTmpTransChn++)
				{
					if (itrTmpTransChn->GetAddrList().empty())
					{
						CLASSLOG(CMU_MOD_SWITCH, ERROR_LEV, "AddrList empty, puChn[%s:%d]\n",
							pcSwitchNode->GetReceiver().GetChannel().GetDevUri().c_str(),
							pcSwitchNode->GetReceiver().GetChannel().GetChnNO());
						continue;
					}
					if (itrTmpTransChn->GetTransChnType() == MEDIA_TRANS_CHN_RTP)
					{
						tTmpRtpAddr = itrTmpTransChn->GetAddrList().at(0);
					}
					else if (itrTmpTransChn->GetTransChnType() == MEDIA_TRANS_CHN_RTCP)
					{
						tTmpRtcpAddr = itrTmpTransChn->GetAddrList().at(0);
					}
				}
			}
		}

		if (tTmpRtpAddr.GetNetIp().empty() || tTmpRtpAddr.GetNetPort() == 0 //地址无效，无需继续判断
			|| tTmpRtcpAddr.GetNetIp().empty() || tTmpRtcpAddr.GetNetPort() == 0 //地址无效，无需继续判断
			|| tTmpRtcpAddr == tTmpRtpAddr  //Rtcp和Rtp地址相同表示目的为按源交换，无法判断目的地址冲突
			)
		{
			continue;
		}

        if (tVidRtpAddr == tTmpRtpAddr)
        {
            return pcSwitchNode;
        }
    }

    return NULL;
}
 
void CSwitchList::PrintData() const
{
    Iterator pPos;
    string strUri;
    CSwitchTask* pTask = NULL;

    OspPrintf(TRUE, FALSE, "\n交换节点SwitchNode数：[%u]个, 业务转发路数[%u]\n", 
        this->size(), GetDstSwitchNum());	
    u32 i = 0;

    for(CSwitchTaskList::const_iterator itr = this->begin(); itr != this->end(); itr++)
    {
        pTask = *itr;
        i++;
        OspPrintf(TRUE, FALSE, "\nSwitchNode[%2u]=====================================================\n", i);
        pTask->PrintData(CSwitchTask::TRACE_NONE);
    }

    OspPrintf(TRUE, FALSE, "\n交换节点SwitchNode数：[%u]个, 业务转发路数[%u]\n", 
        this->size(), GetDstSwitchNum());
}

u32 CSwitchList::GetDstSwitchNum() const
{
    CSwitchTask* pTask = NULL;

    u32 dwDstSwitchNum = 0;

    for(CSwitchTaskList::const_iterator itr = this->begin(); itr != this->end(); itr++)
    {
        pTask = *itr;
        if (pTask != NULL && pTask->GetOutputList().empty())  //无输出节点的交换即为目的交换
        {
            dwDstSwitchNum++;
        }
    }

    return dwDstSwitchNum;
}
