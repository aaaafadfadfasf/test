/*****************************************************************************
ģ����      : cmu 
�ļ���      : 3pctask.cpp
����ļ�    : 
�ļ�ʵ�ֹ���: CSwitchCreatorʵ��
����        : ������
�汾        : vs_phoenix  Copyright(C) 2009-2015 KDDACOM, All rights reserved.
-----------------------------------------------------------------------------
�޸ļ�¼:
��  ��         �汾        �޸���      �޸�����
2012/08/02     1.0        ������        ����
******************************************************************************/

#include "3pctask.h"
#include "inviteswitchtask.h"

//
//CMS_RESULT C3PCTask::StartSwitch(const TChannel& tDstChn, const TChannel& tSrcChn, TSdp& tSdp)
//{
//    CMS_RESULT nErrorCode = CMS_SUCCESS;
//    TInviteSubject tSubject(tDstChn, tSrcChn);
//
//	TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "Dst[%s], Src[%s], tSdp.Owner[%s].\n", 
//		tDstChn.ToString().c_str(), tSrcChn.ToString().c_str(), tSdp.GetOwner().ToString().c_str());
//
//    ///////////////////////////////////////////////////////////////////////
//    //{{����У��
//    if (tSdp.GetSessionName().empty())
//    {
//        TASKLOG(CMU_MOD_SWITCH, WARNING_LEV, "��Ч��Sdp.SessionName[%s]��\n", 
//            tSdp.GetSessionName().c_str());
//
//        //if (tSdp.GetUri().IsEmpty())
//        //{
//        //    tSdp.SetSessionName(SDP_SESSION_PLAY);
//        //} 
//        //else
//        //{
//        //    tSdp.SetSessionName(SDP_SESSION_PLAYBACK);
//        //}
//
//        nErrorCode = ERR_INVALIAD_SDP;
//        return nErrorCode;
//    }
//    //}}����У��
//
//    if (!tSdp.HasMediaDes())
//    {
//        tSdp.CreateEmptyMediaList();
//
//        TASKLOG(CMU_MOD_SWITCH, WARNING_LEV, "Sdp no MediaDes, CreateEmptyMediaList()��\n");
//    }
//	////////////////////////////////////////////////////////////////////////
//	//����Ŀ�Ľ������񡣴��ڣ���ʧ�ܣ����򣬴���
//	CSwitchTask* pcDstSwitchTask = g_cSwitchList.FindSwitchNode(tDstChn, CHANNEL_TYPE_RECVONLY, tSdp.GetSessionName());
//	if(NULL != pcDstSwitchTask )
//	{
//		CSwitchTask* pcTopSrcSwitchTask = pcDstSwitchTask->GetTopSrc();
//		TChannel tSrcChn;
//		if(pcTopSrcSwitchTask)
//		{
//			tSrcChn = pcTopSrcSwitchTask->GetSender().GetChannel();
//		}
//
//		TASKLOG(CMU_MOD_SWITCH, WARNING_LEV, "The dst has exist a media swith to src[%s]��\n", tSrcChn.ToString().c_str());
//
//		nErrorCode = ERR_CMU_SWITCH_DST_EXIST_SWITCH;
//		return nErrorCode;
//	}
//
//
//	//����Ŀ�Ľ�������
//	TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "DstChn SwitchNode isn't exist, create one��\n");
//
//	pcDstSwitchTask =  g_cSwitchList.CreateInviteSenderSwitchTask(GetInstance());
//	pcDstSwitchTask->SetSubject(tSubject);
//    pcDstSwitchTask->SetSessionName(tSdp.GetSessionName());
//	nErrorCode = pcDstSwitchTask ->InitReceiver(tDstChn);
//	if(CMS_SUCCESS != nErrorCode)
//	{
//		SAFE_DEL_PTR(pcDstSwitchTask);
//		return nErrorCode;
//	}
//
//    //�����������е�sdpû��ʵ�ʵ�ý���ַ������ֻ��ע��uri�Ȼ�����Ϣ��Ҳ���迼��owner�ȡ�
//	pcDstSwitchTask->SetSdp(tSdp);
//
//	//ֱ��
//	if (tSdp.GetDirectTransfer() && pcDstSwitchTask->HasDirectTransferCap(tSrcChn))
//	{
//		nErrorCode = pcDstSwitchTask ->InitSender(tSrcChn);
//		this->SetErrorCode(nErrorCode);
//		if(CMS_SUCCESS != this->GetErrorCode())
//		{
//            SAFE_DEL_PTR(pcDstSwitchTask);
//			return this->GetErrorCode();
//		}
//
//		pcDstSwitchTask->SetSwitchMode(SWITCH_MODE_DRT);
//		pcDstSwitchTask->GetSender().GetDevSsnTask()->AddCurDirectTransNum();
//
//		nErrorCode = pcDstSwitchTask->Start();
//		this->SetErrorCode(nErrorCode);
//
//		return this->GetErrorCode();
//	}
//
//	/////////////////////////////////////////////////////////////////////////////////
//    //����ʵʱ����ҵ���������Ƶ���У�ʱ����Ҫ����Դ�������񡣴��ڣ���ֱ�ӷַ������򣬴���
//    CSwitchTask* pcSrcSwitchTask = NULL;
//    if (tSdp.GetSessionName() == SDP_SESSION_PLAY || tSdp.GetSessionName() == SDP_SESSION_AUDIOCALL)
//    {
//        pcSrcSwitchTask = g_cSwitchList.FindSwitchNode(tSrcChn, CHANNEL_TYPE_SENDONLY, tSdp.GetSessionName());
//    }
//
//	if(NULL == pcSrcSwitchTask)
//	{
//		//����Դ�������
//		TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "SrcChn SwitchNode isn't exist, create one��\n");
//
//		pcSrcSwitchTask = g_cSwitchList.CreateInviteSenderSwitchTask(GetInstance());
//		pcSrcSwitchTask->SetSubject(tSubject);
//        pcSrcSwitchTask->SetSessionName(tSdp.GetSessionName());
//		nErrorCode = pcSrcSwitchTask ->InitSender(tSrcChn);
//		if(CMS_SUCCESS != nErrorCode)
//		{
//			SAFE_DEL_PTR(pcDstSwitchTask);
//
//            if (ERR_CMU_CMU_NOT_CONN != nErrorCode)
//            {
//                SAFE_DEL_PTR(pcSrcSwitchTask);
//            }
//			
//			return nErrorCode;
//		}
//
//        //�����������е�sdpû��ʵ�ʵ�ý���ַ������ֻ��ע��uri�Ȼ�����Ϣ��Ҳ���迼��owner�ȡ�
//		pcSrcSwitchTask->SetSdp(tSdp);
//	}
//
//	/////////////////////////////////////////////////////////////////////////////////
//	//��Դ������ѡ��ַ��ڵ� �� ����vtdu
//	CSwitchTask* pcRtnDistribNode = NULL;
//	nErrorCode = pcSrcSwitchTask->ChooseDistribNode(pcDstSwitchTask, pcRtnDistribNode);
//
//	if(NULL != pcRtnDistribNode)
//	{
//		pcSrcSwitchTask = (CSwitchTask*)pcRtnDistribNode;
//	}
//	pcSrcSwitchTask->AddOutput(pcDstSwitchTask);                   //�Ȱѽ���������������ʧ��ʱptSrcSwitchNode���ܱ���ȷɾ��
//
//	if(CMS_SUCCESS != nErrorCode)
//	{
//		SAFE_DEL_PTR(pcDstSwitchTask);
//		return nErrorCode;
//	}
//
//    nErrorCode = CSwitchTask::MakeLink(pcSrcSwitchTask, pcDstSwitchTask);
//    if(CMS_SUCCESS != nErrorCode)
//    {
//        SAFE_DEL_PTR(pcDstSwitchTask);
//        return nErrorCode;
//    }
//
//    //if (bAddSwitchToCreator)
//    {
//        //��Ŀ�Ľ������� �� �����������������
//        pcDstSwitchTask->SetSwitchCreatorTask(this);
//        this->SetDstSwitchTask(pcDstSwitchTask);
//
//        /* fanxg:
//           cmu�н������ȹ���Դ���������ȵ��ȣ�Ŀ�Ľ����������ȡ�
//           �����߻ص������ڵ������񶼳ɹ������ִ�У����Խ�������������Ŀ�Ľ��������С�
//        */
//    }
//
//	return nErrorCode;
//}

C3PCSwitchEntryTask::C3PCSwitchEntryTask(CInstExt *pcInst) : CSipTask(pcInst), m_nErrorCode(CMS_SUCCESS), m_pcDstSwitch(NULL)
{

}

C3PCSwitchEntryTask::~C3PCSwitchEntryTask()
{
    if (NULL != m_pcDstSwitch)
    {
        m_pcDstSwitch->ProcCreatorExit(this);
    }
}
