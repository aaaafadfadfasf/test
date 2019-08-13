/*****************************************************************************
ģ����      : cmu 
�ļ���      : switchtask.cpp
����ļ�    : 
�ļ�ʵ�ֹ���: CSwitchTaskʵ��
����        : ������
�汾        : vs_phoenix  Copyright(C) 2009-2015 KDDACOM, All rights reserved.
-----------------------------------------------------------------------------
�޸ļ�¼:
��  ��         �汾        �޸���      �޸�����
2011/11/04     1.0        ������        ����
2012/03/30     1.0        ������       �ع������߼�����֮ǰ��һ����������ֳ�Src��Dst������������
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

    //������ɣ��ص���Ŀ��������
    if(SWITCH_STATE_SWITCHED == GetSwitchState())
    {
		// ������ɺ����Դ�����������ӳٷ���ack�ı�־�����ʱ����Դ������ack
		CInviteSenderSwitchTask *pcSrcTask = dynamic_cast<CInviteSenderSwitchTask *>(m_pcInput);
		if (NULL != pcSrcTask && pcSrcTask->IsDelayPuAck())
		{
			TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "Ŀ�Ľ���������ɣ����ӳٷ���Ack��Դ��������Ack��\n");

			pcSrcTask->SendDelayPuAck();
		}

        UpdateDevServiceState(true);

        //֪ͨ���������Ŀ�ģ����
        CSwitchTaskList::iterator itrOutput = this->GetOutputList().begin();
        for(; itrOutput != this->GetOutputList().end(); itrOutput++)
        {
            CSwitchTask* pcOutputTask = *itrOutput;

            pcOutputTask->CBInputSwitchFinish(this);
        }

        //֪ͨ3pc����������
        u32 dwTaskNo = this->GetTaskNO();
        CallbackToTask(CMU_SWITCH_FINISH_CB, &dwTaskNo, sizeof(dwTaskNo), m_pcCreatorTask);
    }

}

//////////////////////////////////////////////////////////////////////////////
//����
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
        TASKLOG(CMU_MOD_SWITCH, ERROR_LEV, "δ�ҵ���Ӧ��SwitchPin��DlgId[%lu]��\n", tDlgId);

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
    //�ж��豸�Ƿ����
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
        //����ʱSession���DevUri��һ�������ﲻ����Ҫ�����
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

    //���豸��ע�������뵱ǰ����������й���
    cSwitchPin.SetDevSsnTask(pSessTask);
    pSessTask->AddTask(this);	         //�ѽ���������ӵ��豸�Ự�����У����ڸ��豸����ʱ����ȷ�������������

    cSwitchPin.SetToUri(tRouteUri);

    return this->GetErrorCode();
}

//******************************************************************************
// ������:    SetSdp
// ����:      ����Sdp��Owner����SwitchPin�����򿽱�SdpBasicInfo����һ��SwitchPin
// �㷨ʵ��:  ��NOTE����Ϊ������ܻ�����Keyȥ��д��tSdp����,���Բ������ô�������������õķ�ʽ��
// ����Ȩ��:  public 
// ����˵��:  TSdp tSdp
// ����ֵ:    bool
//-----------------------------------------------------------
// �޸ļ�¼:
// ��      ��     �޸���        �߶���          �޸ļ�¼
// 2013/08/02     huangzhichun huangzhichun    ����
//******************************************************************************
bool CSwitchTask::SetSdp(TSdp tSdp)
{
    bool bFoundOwner = false;
    bool bIsNeedPunch = GetIsNeedPunch();

	//���SDP�ǽ��ն�RCS�ģ�����ǰ����Ϊƽ̨¼�񣩣�������PunchKey��
	//todo NRUʹ��ע��IP��vtduӦ��ʹ�ö�Ӧ���Σ�Ŀǰ����û�У�������
	if (m_cReceiver.GetChannel() == tSdp.GetOwner() && DEV_TYPE_RCS == m_cReceiver.GetDevType())
	{
		bIsNeedPunch = false;
	}

	//���SDP�ǽ��ն�CUI�ģ�����NAT̽���ģʽ����Punch
	if (m_cReceiver.GetChannel() == tSdp.GetOwner() && tSdp.GetIsNatPacketMode())
	{
		bIsNeedPunch = false;
	}

	//����CUI����Ƶ���У�����VTDU��NAT̽���ģʽ
	if (DEV_TYPE_CU == m_cSender.GetDevType() && SDP_SESSION_AUDIOCALL == tSdp.GetSessionName())
	{
		m_cReceiver.GetSdp().SetIsNatPacketMode(tSdp.GetIsNatPacketMode());
	}

	//����GBS��CUI1����Ƶ���У�����VTDU��InterfaceIp
	if ((DEV_TYPE_GBS == m_cSender.GetDevType() || DEV_TYPE_CUI1 == m_cSender.GetDevType()) && SDP_SESSION_AUDIOCALL == tSdp.GetSessionName())
	{
		m_cReceiver.GetSdp().SetInterfaceIp(tSdp.GetInterfaceIp());
	}

    //Ϊ����ͨ��������MediaChannelKey
    for(TMediaDescList::iterator itrMediaDesc = tSdp.GetMediaDescList().begin(); itrMediaDesc != tSdp.GetMediaDescList().end(); itrMediaDesc++)
    {
        //����tMediaTransChnList
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

    //��ʼ��Sender�˵�Sdp
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

    //��ʼ��Receiver�˵�Sdp
    if(!m_cReceiver.GetChannel().IsNull() && m_cReceiver.GetChannel() == tSdp.GetOwner())
    {
        m_cReceiver.SetSdp(tSdp);
        bFoundOwner = true;
    }
    else
    {
        m_cReceiver.GetSdp().CopyBasicInfoFrom(tSdp);
    }

    //3PC����һ���������߼����������δ�ӡ
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
    //���������m_cReceiver.SdpҲ������m_cSender.Sdp
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
    //�����ǰ����Ѿ��Ǹý���������������
    if(ptValue->m_pcInput == this)
    {
        return;
    }

    //�����Ҫ��ӵ��������Ѿ��������������һ���������ǵ�����
    if(ptValue->m_pcInput != NULL)
    {
        ptValue->m_pcInput->RemoveOutput(ptValue);
    }

	m_tOutputList.push_back(ptValue);
	ptValue->m_pcInput = this;

    //���RecviverPin = VTDU���������Ϊ��SwitchTask��һ��Src
    //Vtdu����Ϣ��������Src�ϣ���Ҳ��Src���޸�
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

//�ҳ���Ӧ���͵ľ������ַ������Ľ�㣨�籸��ԴVTDU��MTU��
CSwitchTask* CSwitchTask::FindBakSrcByType(const string& strDevType)
{
    s32 nMaxFreeSwitchNumFound = 0;
    CSwitchTask* pcSwitchNodeSelected = NULL;

    CSwitchTaskList::iterator itrOutputNode = this->GetOutputList().begin();
    for(; itrOutputNode != this->GetOutputList().end(); itrOutputNode++)
    {
        CSwitchTask* pcOutputNode = *itrOutputNode;

        //TASKLOG(CMU_MOD_SWITCH, ALL_LEV, "����������[%s].\n", pcOutputNode->GetChannel().ToString().c_str());

        if(pcOutputNode->GetReceiver().GetDevType() == strDevType) 
        { //Src
            s32 nFreeSwitchNum = pcOutputNode->GetFreeDistrSwitchNum();

            TASKLOG(CMU_MOD_SWITCH, EVENT_LEV, "ƥ�䵽����Ϊ[%s]��������[%s], FreeSwitchNum=%d, MaxFreeSwitchNumFound=%d.\n", 
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
//��ʼ����ҵ���߼�
////////////////////////////////////////////////////////////////////////////////////////////////

//******************************************************************************
// ������:    StartSwitch
// ����:      ��ʼ������������ں����������жϽ�������vtdu���ȣ���Դ����Ŀ�Ķ˵Ĺ�����
// �㷨ʵ��:  ��̬����
// ����Ȩ��:  public 
// ����˵��:  const TChannel & tDstChn
// ����˵��:  const TChannel & tSrcChn
// ����˵��:  TSdp & tSdp
// ����˵��:  CCmuTask * pcReqTask
// ����ֵ:    CMS_RESULT
//-----------------------------------------------------------
// �޸ļ�¼:
// ��      ��     �޸���        �߶���          �޸ļ�¼
// 2012/03/30     huangzhichun huangzhichun    ����
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
	//{{����У��
	if (tSdp.GetSessionName().empty())
	{
		GLOBELOG(CMU_MOD_SWITCH, WARNING_LEV, "��Ч��Sdp.SessionName[%s]��\n", 
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

		GLOBELOG(CMU_MOD_SWITCH, WARNING_LEV, "Sdp no MediaDes, CreateEmptyMediaList()��\n");
	}
	//}}����У��


	////////////////////////////////////////////////////////////////////////
	//����Ŀ�Ľ������񡣴��ڣ���ɾ�����ؽ������򣬴���
	CSwitchTask* pcDstSwitchTask = g_cSwitchList.FindSwitchNode(tDstChn, CHANNEL_TYPE_RECVONLY, tSdp.GetSessionName());
	if(NULL != pcDstSwitchTask )
	{
		CSwitchTask* pcTopSrcSwitchTask = pcDstSwitchTask->GetTopSrc();
		TChannel tTmpSrcChn;
		if(pcTopSrcSwitchTask)
		{
			tTmpSrcChn = pcTopSrcSwitchTask->GetSender().GetChannel();
		}

		GLOBELOG(CMU_MOD_SWITCH, WARNING_LEV, "The dst has exist a media swith to src[%s]��\n", tTmpSrcChn.ToString().c_str());

		//nErrorCode = ERR_CMU_SWITCH_DST_EXIST_SWITCH;
		//return nErrorCode;
		delete pcDstSwitchTask;
		pcDstSwitchTask = NULL;
	}

	////////////////////////////////////////////////////////////////////////
	//���ݽ�����ַ����Ŀ�Ľ������񡣴��ڣ���ɾ�������������򣬴���
	pcDstSwitchTask = g_cSwitchList.FindDstSwitchNode(tSdp);
	if (NULL != pcDstSwitchTask)
	{
		delete pcDstSwitchTask;
		pcDstSwitchTask = NULL;
	}

	/////////////////////////////////////////////////////////////////////////////////
	//���Sender�����ӻỰ
	CLoginSession* pSrcSess = NULL;
	TSipURI tRouteUri;
	nErrorCode = pcReqTask->CheckDevRouter(tSrcChn.GetDevUri(), g_cCmuConf.m_bSwitchFollowTopo, tRouteUri, &pSrcSess);
	if (nErrorCode !=  CMS_SUCCESS)
	{
		return nErrorCode;
	}

	/////////////////////////////////////////////////////////////////////////////////
	//�����Ҫ���õĽ���ģʽ
	TSwitchMode tSwitchMode = CheckSwitchMode(pSrcSess, tSrcChn, tSdp);
	GLOBELOG(CMU_MOD_SWITCH, CRITICAL_LEV, "CheckSwitchMode:[%s]��\n", tSwitchMode.c_str());

	/////////////////////////////////////////////////////////////////////////////////
	//����Դ��������[��Է�ֱ����ʵʱ����ҵ���������Ƶ���У�]�����ڣ���ֱ�ӷַ������򣬴���
	CSwitchTask* pcSrcSwitchTask = NULL;

	if (SWITCH_MODE_DRT != tSwitchMode && IsRealTimeMediaSession(tSdp.GetSessionName()) )
	{
		pcSrcSwitchTask = g_cSwitchList.FindSwitchNode(tSrcChn, CHANNEL_TYPE_SENDONLY, tSdp.GetSessionName());
	}

	TKDSIP_DIALOG_RESOURCE tInfo;
	if (GetWorkInst()->GetKdsipResInfo(tInfo, KDSIP_DIALOG_TYPE_INVITE))
	{
		u32 dwIvtDlgUnUsed = tInfo.dwMax - tInfo.dwUsage + 1;//��1����Ϊcui/rcs����ʱ���Ѿ�������cmuһ��dlg��Դ
		if (((dwIvtDlgUnUsed < CMU_SWITCH_NEED_DLG_NUM) && (NULL == pcSrcSwitchTask))
			|| ((dwIvtDlgUnUsed < CMU_DST_SWITCH_NEED_DLG_NUM) && (NULL != pcSrcSwitchTask)))
		{
			GLOBELOG(CMU_MOD_SWITCH, ERROR_LEV, "Invite dialog resource is not enough, IvtDlgMax[%lu], IvtDlgUsed[%lu]��\n",
				tInfo.dwMax, tInfo.dwUsage);
			nErrorCode = ERR_CMU_SWITCH_DLG_RES_NOT_ENOUGH;
			return nErrorCode;
		}

		GLOBELOG(CMU_MOD_SWITCH, EVENT_LEV, "IvtDlgMax[%lu], IvtDlgUsed[%lu]��\n",
			tInfo.dwMax, tInfo.dwUsage);
	}

	/////////////////////////////////////////////////////////////////////////////////
	//�ж�InviteReq��Դ
	if(tSdp.GetOwner() != tSrcChn)
	{
        //�������ʼ��DstSwitchTask
		if(tSdp.GetOwner() == tDstChn)
		{
			//InviteReq����Receiver��
			GLOBELOG(CMU_MOD_SWITCH, CRITICAL_LEV, "InviteReq come from DstChn��\n");

			pcDstSwitchTask = (CSwitchTask *)pcReqTask;
			//��ʼ��DstSwitchTask����
			nErrorCode = pcDstSwitchTask ->InitReceiver(tDstChn, tSdp.GetSessionName(), NULL, pcReqTask->GetSipDlgID());
			if(CMS_SUCCESS != nErrorCode)
			{
				return nErrorCode;
			}
		}
		else
		{
			//���������п���
			GLOBELOG(CMU_MOD_SWITCH, CRITICAL_LEV, "3PCC��\n");

			//����Ŀ�Ľ�������
			GLOBELOG(CMU_MOD_SWITCH, CRITICAL_LEV, "DstChn SwitchNode isn't exist, create one��\n");

			pcDstSwitchTask =  g_cSwitchList.CreateInviteSenderSwitchTask(pcReqTask->GetInstance());
			//��ʼ��DstSwitchTask����
			nErrorCode = pcDstSwitchTask ->InitReceiver(tDstChn, tSdp.GetSessionName());
			if(CMS_SUCCESS != nErrorCode)
			{
				SAFE_DEL_PTR(pcDstSwitchTask);
				return nErrorCode;
			}

			//��Ŀ�Ľ������� �� �����������������
			//cmu�н������ȹ���Դ���������ȵ��ȣ�Ŀ�Ľ����������ȡ�
			//�����߻ص������ڵ������񶼳ɹ������ִ�У����Խ�������������Ŀ�Ľ��������С�
			pcDstSwitchTask->SetSwitchCreatorTask((C3PCTask*)pcReqTask);
			((C3PCTask*)pcReqTask)->SetDstSwitchTask(pcDstSwitchTask);
		}

		pcDstSwitchTask->SetSubject(tSubject);
		pcDstSwitchTask->SetSessionName(tSdp.GetSessionName());
		pcDstSwitchTask->SetSwitchMode(tSwitchMode);
		pcDstSwitchTask->SetSdp(tSdp);
		//{{{{{{{{{{{{{{{ modify by Royan 20140606 ����������
		pcDstSwitchTask->GetReceiver().GetSdp().SetGBStreamKAInfo(tSdp.GetGBStreamKAInfo());
		//}}}}}}}}}}}}}}} modify by Royan 20140606 ����������

		//ֱ��
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
		else	//ƽ̨ת������ģʽ
		{
			if (NULL == pcSrcSwitchTask)
			{
				//����Դ�������
				GLOBELOG(CMU_MOD_SWITCH, CRITICAL_LEV, "SrcChn SwitchNode isn't exist, create one��\n");

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
				//{{{{{{{{{{{{{{{ modify by Royan 20140606 ����������
				pcSrcSwitchTask->GetReceiver().GetSdp().SetGBStreamKAInfo(tSdp.GetGBStreamKAInfo());
				//}}}}}}}}}}}}}}} modify by Royan 20140606 ����������
				pcSrcSwitchTask->GetReceiver().GetSdp().SetGBRecLocation(tSdp.GetGBRecLocation());
			}
		}
	}
	else
	{
		//InviteReq����Sender��
		GLOBELOG(CMU_MOD_SWITCH, CRITICAL_LEV, "InviteReq come from SrcChn��\n");
		CMS_ASSERT(false && "UNIMPLEMENTED for InviteReq come from SrcChn!");

        return ERR_INVALIAD_REQ_PARAM;

		////������ǻỰ�ڵĶ��ͬԴInviteReq
		//if(SWITCH_MODE_SNG == tSwitchMode && NULL != pcSrcSwitchTask && pcSrcSwitchTask->GetSender().GetDlgID() != tDlgID)
		//{
		//	GLOBELOG(CMU_MOD_SWITCH, CRITICAL_LEV, "ReInvite from SrcChn but not in Dialog��\n");
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

		////ֱ��
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
	//��Դ������ѡ��ַ��ڵ� �� ����vtdu
	CSwitchTask* pcRtnDistribNode = NULL;
	nErrorCode = pcSrcSwitchTask->ChooseDistribNode(pcDstSwitchTask, pcRtnDistribNode);

	if(NULL != pcRtnDistribNode)
	{
		//ֱ�Ӹ�pcSrcSwitchTask��ֵ��֮ǰ���������SwitchTask��δ���??? fanxg
		pcSrcSwitchTask = (CSwitchTask*)pcRtnDistribNode;
	}
	pcSrcSwitchTask->AddOutput(pcDstSwitchTask);                   //�Ȱѽ���������������ʧ��ʱptSrcSwitchNode���ܱ���ȷɾ��

	if(CMS_SUCCESS != nErrorCode)
	{
		//SAFE_DEL_PTR(pcDstSwitchTask);    //ʧ�ܺ�����ɾ����Task
		return nErrorCode;
	}

	nErrorCode = MakeLink(pcSrcSwitchTask, pcDstSwitchTask);
	return nErrorCode;
}

TSwitchMode CSwitchTask::CheckSwitchMode(CLoginSession* pSrcSess, const TChannel& tSrcChn, const TSdp& tSdp)
{
    //����ʵʱ��������Ҫ�ж��豸�Ƿ���ֱ������
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
    //������ʷ�������豸�����֧��һ�������Ķ�·���ͣ������ж�ֱ������
    else
    {
        if (tSdp.GetPlatDirectTransfer() || tSdp.GetDirectTransfer() )
        {
            return SWITCH_MODE_DRT;
        }
    }

	//HZCTODO:�Ƿ��鲥

	//Ĭ�� ��������
	return SWITCH_MODE_SNG;
}

//����Ϊʲôʹ��ָ������ã�û����ָ��ֵ���޸İ�???-fanxg
CMS_RESULT CSwitchTask::MakeLink(CSwitchTask* ptSrcSwitch, CSwitchTask* ptDstSwitch)
{
    CMS_RESULT nErrorCode = CMS_SUCCESS;

    GLOBELOG(CMU_MOD_SWITCH, CRITICAL_LEV, "Src[%s].SwitchState[%s], Dst[%s].SwitchState[%s]\n",
        ptSrcSwitch->GetSender().GetChannel().ToString().c_str(), GetSwitchStateStr(ptSrcSwitch->GetSwitchState()), 
        ptDstSwitch->GetReceiver().GetChannel().ToString().c_str(), GetSwitchStateStr(ptDstSwitch->GetSwitchState()));

    //����Ѿ���ӹ�һ��???  fanxg
    ptSrcSwitch->AddOutput(ptDstSwitch);

    //���ݽ���ʱ��ͼ�������Src���ֵĽ��������Src�Ѿ�������ϣ�����ַ�����ֱ�Ӵ���Dst����
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
        //���ptSrcSwitch�����м�״̬����ԣ���ȥ�����κζ�������Ӧ�Ķ�����ɣ�����Ҫ��������ʱ��ȥ������
        GLOBELOG(CMU_MOD_SWITCH, WARNING_LEV, "SrcNode in a middle state: [%s], IGNORE!\n", GetSwitchStateStr(ptSrcSwitch->GetSwitchState()));
    }

    return nErrorCode;
}

///////////////////////////////////////////////////////////////////////////
//��Ա����
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
        TASKLOG(CMU_MOD_SWITCH, ERROR_LEV, "����KDSIP_EVENT_INVITE_REQ��%sʧ�ܣ�\n", cSwitchPinTo.GetToUri().GetNOString());

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
		TASKLOG(CMU_MOD_SWITCH, ERROR_LEV, "����KDSIP_EVENT_INVITE_ACKʧ�ܣ�\n");

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
    //TODO�жϸ�Դ�ڵ��Ƿ��Ѿ�����ת��ģ��Ľڵ�
    if(! ����ת��ڵ�)
    {
    //TODO��ȫ��ת��ģ����ѡ��һ��ת��ͨ��
    //TODO����һ��MTUת��ڵ�,�����ӵ������б���
    CSwitchTask* ptMtuSwitchNode = CreateSwitchNode();
    this->AddOutputSwitchNode(ptMtuSwitchNode);
    }
    */
    return NULL;
}

/*====================================================================
������      : IsCreditDomain
����        ��ָ�����Ƿ���������
�㷨ʵ��    ��
����ȫ�ֱ�����
�������˵����const TKDMNO& tDomainNO �ƶ���
����ֵ˵��  ��true, false
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��			�汾        �޸���        �޸�����
2011/11/04		1.0        ������        ����
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
������      : ChooseDistribNode
����        ���ڵ�ǰԴ�ڵ���·��ΪĿ��ѡ��һ���ַ��ڵ�
�㷨ʵ��    ��
����ȫ�ֱ�����
�������˵����
            [IN]CSwitchTask* ptDstSwitchNode--����Ŀ�Ľڵ�
            [OUT]CSwitchTask* ptDistribNode--�ҵ��������ַ��ڵ�
����ֵ˵��  ��CMS_RESULT
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��			�汾        �޸���        �޸�����
2011/11/04		1.0        ������        ����
====================================================================*/
CMS_RESULT CSwitchTask::ChooseDistribNode(CSwitchTask* ptDstSwitchNode, CSwitchTask*& ptRtnDistribNode)
{
    TRedisModuleResourceId tResourceId;

    //�жϸ�Դ�Ƿ���VTDU
    if( this->GetReceiver().GetDevSsnTask() != NULL )//Դ����VTDU
    {
        CMS_RESULT nErrorCode = ChooseUseableSrcNode(ptDstSwitchNode->GetSessionName(), ptDstSwitchNode->GetReceiver(), ptRtnDistribNode, tResourceId);
        ptDstSwitchNode->SetVtduResourceId(tResourceId);

        return nErrorCode;
    }
    else
    {
        //�ж�Դ�Ƿ���Ҫ��Խ��բ
        if( IsCreditDomain(this->GetSender().GetChannel().GetDevUri()) )
        {
            TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "Դ[%s]��Ҫ����բ.\n", this->GetSender().GetChannel().ToString().c_str());

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
������      ��ChooseUseableSrcNode
����        ���Ӵ��ڽ�����Դ�ڵ���ѡ��һ�����õķַ�·��
�㷨ʵ��    ��
����ȫ�ֱ�����
�������˵����
����ֵ˵��  ��
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��			�汾        �޸���        �޸�����
2011/11/04		1.0        ������        ����
====================================================================*/
CMS_RESULT CSwitchTask::ChooseUseableSrcNode(const TSdpSessionName &tSessionName, CSwitchPin& cDstSwitchPin, CSwitchTask*& ptRtnDistribNode, TRedisModuleResourceId &tVtduResourceId)
{
    //�����зַ������Ľ���Դ
    //ע�������Դͷ�п����Ǹ�����բ��VTDU������Դ��;���ڴ������к�����VTDU���ǲ�����բ��VTDU
    CSwitchTask *ptLastSrcBakSwitchData = NULL;
    CSwitchTask *ptSrcSwitchNode = this;

    TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "Դ[%s]��������ת����Ѱ�ҿ��õķַ�VTDU.\n", ptSrcSwitchNode->GetChannel().ToString().c_str());

    CRedisSdk &cRedisSdk = GetLocalRedisPipe().GetRedisSdk();
    const TChannel &tSrcChn = m_cSender.GetChannel();
    const TChannel &tDstChn = cDstSwitchPin.GetChannel();
    TRedisModuleResourceId tResourceId;
    for(; ptSrcSwitchNode != NULL; ptSrcSwitchNode = ptSrcSwitchNode->FindBakSrcByType(DEV_TYPE_VTDU))
    {
        ptLastSrcBakSwitchData = ptSrcSwitchNode;

        if (CMS_SUCCESS == cRedisSdk.AllocVtduResource(ptSrcSwitchNode->GetVtduResourceId(), tResourceId))
        {
            TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "�ҵ����(%s����Դ)[%s]��������ת��.\n", (this != ptSrcSwitchNode)? "��":"��", ptSrcSwitchNode->GetChannel().ToString().c_str());

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
    //�Ҳ�������Դ�������VTDU�����±���Դ
    TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "��Դ�Ѵ��ڵ�VTDUת��������������ҪѰ��һ���µ�VTDU���з������ַ��������±���Դ.\n");

    //�����±���Դ
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
    //����Input��Sdp������Ϣ����ΪMakeLink������ǰ����Դ����һ��Դ�ڵ�ʱ��Ҫ����ͨ����
    ptSrcNextBakSwitchData->SetSdp(ptLastSrcBakSwitchData->GetSender().GetSdp());

    CMS_RESULT nErrorCode = ptSrcNextBakSwitchData->ChooseVtduAsReceiver(false, ptLastSrcBakSwitchData->GetTransChnNum()*2, 
        tSessionName, cDstSwitchPin, tResourceId);
    if(CMS_SUCCESS != nErrorCode)
    {
        TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "ΪԴѰ���µı���ԴVTDUʱ���Ҳ������õ�VTDU.\n");

        SAFE_DEL_PTR(ptSrcNextBakSwitchData);

        return nErrorCode;
    }

    TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "ΪԴѰ�ҵ��µı���ԴVtduChn[%s].\n", ptSrcNextBakSwitchData->GetReceiver().GetChannel().ToString().c_str());

    /////////////////////////////////////////////////////////////////////////////////////////
    //ֹͣ��ԭĿ�ĵĽ���,Ѱ������һ·Ŀ��
    CSwitchTask *ptLastDstSwitchData = (CSwitchTask*)ptLastSrcBakSwitchData->GetOutputList().back();
    if(NULL == ptLastDstSwitchData)
    {
        CMS_ASSERT( ptLastDstSwitchData);

        return ERR_CMU_SWITCH_EXCEPTION;
    }

    TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "��Դ[%s]��Ѱ�ҵ���Ҫֹͣ�����һ·�������[%s].\n", 
        ptLastSrcBakSwitchData->GetChannel().ToString().c_str(),
        ptLastDstSwitchData->GetChannel().ToString().c_str());

    if(!ptLastDstSwitchData->StopSwitchToSrc(true))
    {
        TASKLOG(CMU_MOD_SWITCH, ERROR_LEV, "����ʱΪ�����µı���Դֹͣԭ�н���Src[%s]--Dst[%s]ʧ��.\n", 
            ptLastSrcBakSwitchData->GetChannel().ToString().c_str(),
            ptLastDstSwitchData->GetChannel().ToString().c_str());

        return ERR_CMU_SWITCH_STOP_SWITCH_FOR_NEW_BAK_SRC_FAILED;
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    //�������µ��ȵı���Դ
    if(CMS_SUCCESS != MakeLink(ptLastSrcBakSwitchData, ptSrcNextBakSwitchData))
    {
        TASKLOG(CMU_MOD_SWITCH, ERROR_LEV, "��ԭԴ������㽻�����±���ԴSrc[%s]--Dst[%s]ʧ�ܣ�ɾ����Ҫ������֮��Ľ�������.\n", 
            ptLastSrcBakSwitchData->GetChannel().ToString().c_str(),
            ptSrcNextBakSwitchData->GetChannel().ToString().c_str());

        //����Դ����ʧ�ܣ�ɾ����Ҫ������֮��Ľ�������
        //TODO������øĳ��ܴ��������Ľӿڣ����bye(nErrCode)����
        SAFE_DEL_PTR(ptSrcNextBakSwitchData);
        SAFE_DEL_PTR(ptLastDstSwitchData);

        return ERR_CMU_SWITCH_ADD_BAK_SRC_FAILED;
    }

    TASKLOG(CMU_MOD_SWITCH, ERROR_LEV, "��ԭԴ������㽻�����±���ԴSrc[%s]--Dst[%s]�ɹ�.\n", 
        ptLastSrcBakSwitchData->GetChannel().ToString().c_str(),
        ptSrcNextBakSwitchData->GetReceiver().GetChannel().ToString().c_str());

    /////////////////////////////////////////////////////////////////////////////////////////
    //���±���Դ�������������Ŀ��
    if (g_cCmuConf.m_bEnablePunch)
    {
        //�������ҪpunchЭ�̵�ַ�ģ�ֱ�Ӱ����һ·�������Ŀ�Ķ�(CU)���·��𽻻�
        ptLastDstSwitchData->SetErrorCode(ERR_CMU_SWITCH_AGAIN);
        SAFE_DEL_PTR(ptLastDstSwitchData);
    }
    else
    {
        //����ǲ���ҪpunchЭ�̵�ַ�ģ�ֱ�Ӱ����һ·���ӵ�����ԭԴ�����
        if(CMS_SUCCESS == MakeLink(ptSrcNextBakSwitchData, ptLastDstSwitchData))
        {
            TASKLOG(CMU_MOD_SWITCH, ERROR_LEV, "���µı���Դ[%s]�Ͻ���һ·��ԭ�����Ŀ�����[%s]�ɹ�.\n", 
                ptSrcNextBakSwitchData->GetChannel().ToString().c_str(),
                ptLastDstSwitchData->GetChannel().ToString().c_str());
        }
        else
        {
            TASKLOG(CMU_MOD_SWITCH, ERROR_LEV, "���µı���Դ[%s]�Ͻ���һ·��ԭ�����Ŀ�����[%s]ʧ�ܣ�ɾ���ý�������.\n", 
                ptSrcNextBakSwitchData->GetChannel().ToString().c_str(),
                ptLastDstSwitchData->GetChannel().ToString().c_str());

            //ɾ���ý�������
            SAFE_DEL_PTR(ptLastDstSwitchData);

            return ERR_CMU_SWITCH_ADD_BAK_SRC_FAILED;
        }
    }

    //�����µ��ȵı���Դ
    ptRtnDistribNode = ptSrcNextBakSwitchData;
    tVtduResourceId  = tResourceId;

    return CMS_SUCCESS;
}

CMS_RESULT CSwitchTask::ChooseVtduAsReceiver(bool bIsTravGAP, u32 dwSwitchNumToAdd, const TSdpSessionName &tSessionName, 
    CSwitchPin& cDstSwitchPin, TRedisModuleResourceId &tVtduResourceId)
{
    ECMS_ERRORCODE emRet = CMS_SUCCESS;

    //ѡ����Ӧ���͵�VTDU
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

        TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "�ҵ����õ�Vtdu[%s]��Resource[%s - %s].\n", 
            tVtduUri.GetURIString().c_str(), tResourceId.c_str(), tResource.ToJson().c_str());

        //��VTDU��Ϊ�������
        InitReceiver(tVtduChn, tSessionName, pcRegTask);
        GetSubject().SetDstChn(tVtduChn);
        SetVtduResourceId(tResourceId);
        tVtduResourceId = tResourceId;
    }
    else
    {
        TASKLOG(CMU_MOD_SWITCH, ERROR_LEV, "�Ҳ������õ�VTDU!\n");

        if (!tResourceId.empty())               // �ͷ���Դ
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
������      ��StartSwitchToSrc
����        ���ѵ�ǰ�����ָ��Դ������Ӳ�����ӵ�ַ��������vtdu�С�
�㷨ʵ��    ��
����ȫ�ֱ�����
�������˵����CSwitchNode* ptSrcSwitchNode--����Դ�ڵ�
����ֵ˵��  ��true, false
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��			�汾        �޸���        �޸�����
2011/11/04		1.0        ������        ����
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

		// vtdu��nru������ͬһ̨�����ϵ�ʱ�򣬽�����socketЭ��
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
						TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "Ŀ�Ľ����У�Դ��Ŀ�Ķ���ͬһ�����ϣ�Э��ʹ����socket!\n");

						tSdp.SetDomainSocket(true);
						break;
					}
				}
			}
		}

        // ������Դid�ͽ�������
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
//ֹͣ����ҵ����
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

        TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "Sender[%s] DevType[%s] �豸����\n!",
            lpszDevUri, strDevType.c_str());

        //�����豸����Ҫ����Bye���������Dialog��Դ�ͷŲ����ã����ٷ���ByeReq.
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

        TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "Receiver[%s] DevType[%s] �豸����\n!",
            lpszDevUri, strDevType.c_str());


        //�����豸����Ҫ����Bye���������Dialog��Դ�ͷŲ����ã����ٷ���ByeReq.
        if (INVALID_DIALOG_ID != this->GetReceiver().GetDlgID())
        {
            OspSipReleaseDlg(this->GetReceiver().GetDlgID());
            this->GetReceiver().SetDlgID(INVALID_DIALOG_ID);
        }

        bProcess = true;
    }
    else
    {
        TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "Unknown Dev[%s]����\n!", lpszDevUri);
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
        //Invite�Ự��ֹ
    case OSP_SIP_MSG_PROC_FAIL:
        {
            //����������յ�SIP��Ĵ���Ӧ��
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
            this->SetErrorCode(ERR_CMU_DEV_NO_ROUTER);

            TASKLOG(this->GetModuleID(), ERROR_LEV, "�յ�SipMethod[%s]SIP�����Ӧ��, SipErrorCode[%d]\n",
                pcOspSipMsg->GetSipEventStr().c_str(), pcOspSipMsg->GetSipErrorCode());

            //this->PrintData();
        }
        break;
    case OSP_SIP_DIALOG_TERMINATE:
        {
            //SIP��Dialog�쳣��ֹ֪ͨ
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
            TKDSIP_DIALOG_ID tDlgId = pcOspSipMsg->GetSipDlgID();

            CSwitchPin& cSwitchPin = GetSwitchPin(tDlgId);
            cSwitchPin.SetDlgID(INVALID_DIALOG_ID);
            this->SetErrorCode(ERR_CMU_DIALOG_TERMINATE);

			this->SetDialogTerminateErrocodeBySender(this->GetSendToSipUri());
			this->SendOptFailedToUas(this->GetSessionName());

            TASKLOG(this->GetModuleID(), ERROR_LEV, "�յ��豸[%s]DlgID[%lu]SIP��Dialog�쳣��ֹ, SipFrom[%s], SipErrorCode[%d], StatName[%s], ErrorCode[%d]\n",
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

	TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "�յ��豸[%s]DlgID[%lu]��ByeReq, Reason[%d]��\n", 
        cSwitchPin.GetDevDesc().c_str(), tReqDlgId, nByeReason);

	cSwitchPin.SetDlgID(INVALID_DIALOG_ID);

    ////////////////////////////////////////////////////////////
    //�����󷽻�ӦӦ��
    CByeRsp cRsp;

    u32 dwResult = PostRsp(KDSIP_EVENT_INVITE_BYE_RSP, pcOspSipMsg->GetSipTransID(), cRsp);
    TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "����KDSIP_EVENT_INVITE_BYE_RSP, ResultCode[%lu].\n", dwResult);

    return true;
}

CMS_RESULT CSwitchTask::ProcByeRsp(const COspSipMsg*const pcOspSipMsg)
{
    TKDSIP_DIALOG_ID tReqDlgId = pcOspSipMsg->GetSipDlgID();

	//CSwitchPin& cSwitchPin = GetSwitchPin(tReqDlgId); //��Ϊ����ʱ�Ѿ���SwitchPin����INVALID_DIALOG_ID�������Ҳ�����
	TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "�յ�DlgID[%lu]��ByeRsp��\n", tReqDlgId);

    return CMS_SUCCESS;
}

void CSwitchTask::ReleaseResource()
{
    CCmuTask::ReleaseResource();

    //���ٶԻ���������
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

    //��ʼ�������
    StopAllSwitch();

    //����Ի���Դ������Ƿ�Ӧ���
    if(INVALID_TRANS_ID != this->GetSipTransID())
    {
        //����δ��ɣ�δӦ�𣩾����٣��϶���ʧ���ˣ������벻Ӧ�û���CMS_SUCCESS
        if (this->GetErrorCode() == CMS_SUCCESS)
        {
            //CMS_ASSERT(false && "����δ��ɣ�δӦ�𣩾����٣��϶���ʧ���ˣ������벻Ӧ�û���CMS_SUCCESS");
            this->SetErrorCode(ERR_CMU_SWITCH_EXCEPTION);
        }

        //����Ӧ��
        CInviteRsp cRsp;
        DoPostRsp(KDSIP_EVENT_INVITE_RSP, cRsp);
    }

    /////////////////////////////////////////////////////////////////////////////////
    //����Receiver����Դ
    CLoginSession* pDevSsnTask = m_cReceiver.GetDevSsnTask();
    if(pDevSsnTask)
    {
        pDevSsnTask->DelTask(this);   //ɾ�������������豸��¼�Ự��������ϵ

		//��Receiver�˷���Bye����
		if(INVALID_DIALOG_ID != m_cReceiver.GetDlgID())
		{
			CByeReq cReq;
			cReq.SetSession(pDevSsnTask->GetSession());
			cReq.SetReason(this->GetErrorCode());

			u32 dwResult = PostInDlgReq(KDSIP_EVENT_INVITE_BYE_REQ, cReq, m_cReceiver.GetDlgID());

			TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "����KDSIP_EVENT_INVITE_BYE_REQ���豸(DlgID[%ld]--DevChn[%s]) Reason[%d], ResultCode[%lu].\n", 
				m_cReceiver.GetDlgID(), m_cReceiver.GetChannel().ToString().c_str(), this->GetErrorCode(), dwResult);

			m_cReceiver.SetDlgID(INVALID_DIALOG_ID);
		}
    }

    //////////////////////////////////////////////////////////////////////////////////////
    //����Sender����Դ
    pDevSsnTask = m_cSender.GetDevSsnTask();
    if(pDevSsnTask)
    {
        if (SWITCH_MODE_DRT == this->GetSwitchMode())
        {
            pDevSsnTask->SubCurDirectTransNum();
        }
        pDevSsnTask->DelTask(this);   //ɾ�������������豸��¼�Ự��������ϵ

		//��Sender�˷���Bye����
		if(INVALID_DIALOG_ID != m_cSender.GetDlgID())
		{
			CByeReq cReq;
			cReq.SetSession(pDevSsnTask->GetSession());
			cReq.SetReason(this->GetErrorCode());

			u32 dwResult = PostInDlgReq(KDSIP_EVENT_INVITE_BYE_REQ, cReq, m_cSender.GetDlgID());

			TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "����KDSIP_EVENT_INVITE_BYE_REQ���豸(DlgID[%ld]--DevChn[%s]) Reason[%d], ResultCode[%lu].\n", 
				m_cSender.GetDlgID(), m_cSender.GetChannel().ToString().c_str(), this->GetErrorCode(), dwResult);

			m_cSender.SetDlgID(INVALID_DIALOG_ID);
		}
    }

    // �ͷ�redis�е�vtduת����Դ
    if (!m_tVtduResourceId.empty())
    {
        CRedisSdk &cRedisSdk = GetLocalRedisPipe().GetRedisSdk();
        cRedisSdk.ReleaseVtduResource(m_tVtduResourceId);
    }

    //�ӽ����б���ɾ���ý��
    g_cSwitchList.EraseSwitchNode(this);
}

bool CSwitchTask::StopAllSwitch()
{
    //�����CreatorTask��������ϵ
    if(m_pcCreatorTask)
    {
        m_pcCreatorTask->SetDstSwitchTask(NULL);

        m_pcCreatorTask->SetErrorCode(this->GetErrorCode());
        SAFE_DEL_PTR(m_pcCreatorTask);
    }

    //�����ýڵ������Ŀ�Ľ����ڵ㣬���ݹ�ֹͣ����
    CSwitchTaskList::iterator itrOutputNode = this->GetOutputList().begin();
    while(itrOutputNode != this->GetOutputList().end())
    {
        CSwitchTask* pcOutputNode = *itrOutputNode;
        itrOutputNode++;    //����StopSwitchToSrc�����п��ܻ��cOutputNodeList��ɾ����Ӧ��Ԫ�أ�����Ҫ��++

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
������      ��StopSwitchToSrc
����        �������ǰ���������Դ���֮������н�����
�㷨ʵ��    ��
����ȫ�ֱ�����
�������˵������
����ֵ˵��  ��true, false
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��			�汾        �޸���        �޸�����
2011/11/04		1.0        ������        ����
====================================================================*/
bool CSwitchTask::StopSwitchToSrc(bool bByeToSender)
{
    CSwitchTask* ptDstSwitchNode = this;

    CSwitchTask* ptSrcSwitchNode = this->GetInput();
    if(NULL == ptSrcSwitchNode)
    {
        TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "�ý������û�н���Դ������ֹͣ��Դ�Ľ���\n");
        return true;
    }

    //ɾ���ڵ���������ϵ
    //BreakLink(ptSrcSwitchNode, ptDstSwitchNode);
    ptSrcSwitchNode->RemoveOutput(ptDstSwitchNode);

    if (bByeToSender)
    {
        CLoginSession* pDevSsnTask = m_cSender.GetDevSsnTask();
        if(pDevSsnTask)
        {
            pDevSsnTask->DelTask(this);   //ɾ������������vtdu�����������ϵ
        }

        //��Sender�˷���Bye����
        if(INVALID_DIALOG_ID != m_cSender.GetDlgID())   //TODO���ﻹ���ԼӸ�״̬�жϣ�����Ự��δ��ɣ�InviteReq״̬����ɾ��������û��Ҫ����ByeReq����ʹ����Ҳ��ʧ��
        {
            CByeReq cReq;

            u32 dwResult = PostInDlgReq(KDSIP_EVENT_INVITE_BYE_REQ, cReq, m_cSender.GetDlgID());

            TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "����KDSIP_EVENT_INVITE_BYE_REQ���豸(DlgID[%ld]--DevChn[%s]), ResultCode[%lu].\n", 
                m_cSender.GetDlgID(), m_cSender.GetChannel().ToString().c_str(), dwResult);

            m_cSender.SetDlgID(INVALID_DIALOG_ID);
            m_cSender.Clear();
        }
    }

    return true;
}

void CSwitchTask::UpdateDevServiceState(bool bStart)
{
    //û�������㣬��ΪĿ�Ľ��
    if (this->GetOutputList().size() == 0)
    {
        //���� �������� Ŀ�Ľ��ĺ���״̬
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
                    
                    //����״̬
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

    CLASSLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "����CSwitchTask: TaskNO[%lu].\n", pcSwitchTask->GetTaskNO());

    return pcSwitchTask;
}

CSwitchTask* CSwitchList::CreateInviteSenderSwitchTask(CInstExt *pcInst)
{
    CInviteSenderSwitchTask* pcSwitchTask = new CInviteSenderSwitchTask(pcInst);
    CMS_ASSERT(pcSwitchTask && "new CInviteSenderSwitchTask failed!");

    pcSwitchTask->InitStateMachine();

    push_back(pcSwitchTask);//Insert(pcSwitchNode);

    CLASSLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "����CSwitchTask: TaskNO[%lu].\n", pcSwitchTask->GetTaskNO());

    return pcSwitchTask;
}

void CSwitchList::EraseSwitchNode(CSwitchTask* pSwitchNode)
{
    this->remove(pSwitchNode);//this->Erase(pSwitchNode->GetChannel().ToString());
}


//******************************************************************************
// ������:    FindSwitchNode
// ����:      
// �㷨ʵ��:  ����tChannelType��tSessionNameΪ�գ�������Ҫƥ��
// ����Ȩ��:  public 
// ����˵��:  const TChannel & tDevChn
// ����˵��:  const TChannelType & tChannelType
// ����˵��:  const TSdpSessionName & tSessionName
// ����˵��:  const TSwitchMode & tSwitchMode
// ����ֵ:    CSwitchTask*
//-----------------------------------------------------------
// �޸ļ�¼:
// ��      ��     �޸���        �߶���          �޸ļ�¼
// 2012/03/30     huangzhichun huangzhichun    ����
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
*������ƵRtp��ַ����ƥ���Ŀ�Ľ���task��
*ֻ����ʵʱ������������ͣ�����Ƶ���У��ݲ�����
*******************************************************************************/
CSwitchTask* CSwitchList::FindDstSwitchNode(TSdp& tDstSdp) const
{
	if (SDP_SESSION_PLAY != tDstSdp.GetSessionName())
	{//ֻ�ж�ʵʱ���
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

	if (tVidRtpAddr.GetNetIp().empty() || 0 == tVidRtpAddr.GetNetPort())//��ַ��Ч����������ж�
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

		if (tTmpRtpAddr.GetNetIp().empty() || tTmpRtpAddr.GetNetPort() == 0 //��ַ��Ч����������ж�
			|| tTmpRtcpAddr.GetNetIp().empty() || tTmpRtcpAddr.GetNetPort() == 0 //��ַ��Ч����������ж�
			|| tTmpRtcpAddr == tTmpRtpAddr  //Rtcp��Rtp��ַ��ͬ��ʾĿ��Ϊ��Դ�������޷��ж�Ŀ�ĵ�ַ��ͻ
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

    OspPrintf(TRUE, FALSE, "\n�����ڵ�SwitchNode����[%u]��, ҵ��ת��·��[%u]\n", 
        this->size(), GetDstSwitchNum());	
    u32 i = 0;

    for(CSwitchTaskList::const_iterator itr = this->begin(); itr != this->end(); itr++)
    {
        pTask = *itr;
        i++;
        OspPrintf(TRUE, FALSE, "\nSwitchNode[%2u]=====================================================\n", i);
        pTask->PrintData(CSwitchTask::TRACE_NONE);
    }

    OspPrintf(TRUE, FALSE, "\n�����ڵ�SwitchNode����[%u]��, ҵ��ת��·��[%u]\n", 
        this->size(), GetDstSwitchNum());
}

u32 CSwitchList::GetDstSwitchNum() const
{
    CSwitchTask* pTask = NULL;

    u32 dwDstSwitchNum = 0;

    for(CSwitchTaskList::const_iterator itr = this->begin(); itr != this->end(); itr++)
    {
        pTask = *itr;
        if (pTask != NULL && pTask->GetOutputList().empty())  //������ڵ�Ľ�����ΪĿ�Ľ���
        {
            dwDstSwitchNum++;
        }
    }

    return dwDstSwitchNum;
}
