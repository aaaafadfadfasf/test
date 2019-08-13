/*****************************************************************************
模块名      : cmu 
文件名      : 3pctask.cpp
相关文件    : 
文件实现功能: CSwitchCreator实现
作者        : 黄至春
版本        : vs_phoenix  Copyright(C) 2009-2015 KDDACOM, All rights reserved.
-----------------------------------------------------------------------------
修改记录:
日  期         版本        修改人      修改内容
2012/08/02     1.0        黄至春        创建
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
//    //{{参数校验
//    if (tSdp.GetSessionName().empty())
//    {
//        TASKLOG(CMU_MOD_SWITCH, WARNING_LEV, "无效的Sdp.SessionName[%s]！\n", 
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
//    //}}参数校验
//
//    if (!tSdp.HasMediaDes())
//    {
//        tSdp.CreateEmptyMediaList();
//
//        TASKLOG(CMU_MOD_SWITCH, WARNING_LEV, "Sdp no MediaDes, CreateEmptyMediaList()！\n");
//    }
//	////////////////////////////////////////////////////////////////////////
//	//查找目的交换任务。存在，则失败；否则，创建
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
//		TASKLOG(CMU_MOD_SWITCH, WARNING_LEV, "The dst has exist a media swith to src[%s]！\n", tSrcChn.ToString().c_str());
//
//		nErrorCode = ERR_CMU_SWITCH_DST_EXIST_SWITCH;
//		return nErrorCode;
//	}
//
//
//	//创建目的交换任务
//	TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "DstChn SwitchNode isn't exist, create one！\n");
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
//    //第三方交换中的sdp没有实际的媒体地址描述，只关注其uri等基本信息，也无需考虑owner等。
//	pcDstSwitchTask->SetSdp(tSdp);
//
//	//直传
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
//    //开启实时码流业务（浏览、音频呼叫）时，需要查找源交换任务。存在，则直接分发；否则，创建
//    CSwitchTask* pcSrcSwitchTask = NULL;
//    if (tSdp.GetSessionName() == SDP_SESSION_PLAY || tSdp.GetSessionName() == SDP_SESSION_AUDIOCALL)
//    {
//        pcSrcSwitchTask = g_cSwitchList.FindSwitchNode(tSrcChn, CHANNEL_TYPE_SENDONLY, tSdp.GetSessionName());
//    }
//
//	if(NULL == pcSrcSwitchTask)
//	{
//		//创建源交换结点
//		TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "SrcChn SwitchNode isn't exist, create one！\n");
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
//        //第三方交换中的sdp没有实际的媒体地址描述，只关注其uri等基本信息，也无需考虑owner等。
//		pcSrcSwitchTask->SetSdp(tSdp);
//	}
//
//	/////////////////////////////////////////////////////////////////////////////////
//	//从源交换中选择分发节点 或 调度vtdu
//	CSwitchTask* pcRtnDistribNode = NULL;
//	nErrorCode = pcSrcSwitchTask->ChooseDistribNode(pcDstSwitchTask, pcRtnDistribNode);
//
//	if(NULL != pcRtnDistribNode)
//	{
//		pcSrcSwitchTask = (CSwitchTask*)pcRtnDistribNode;
//	}
//	pcSrcSwitchTask->AddOutput(pcDstSwitchTask);                   //先把交换结点关联，否则失败时ptSrcSwitchNode不能被正确删除
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
//        //把目的交换任务 与 创建者任务关联起来
//        pcDstSwitchTask->SetSwitchCreatorTask(this);
//        this->SetDstSwitchTask(pcDstSwitchTask);
//
//        /* fanxg:
//           cmu中交换调度规则：源交换任务先调度，目的交换任务后调度。
//           创建者回调必须在调度任务都成功后才能执行，所以将创建者设置在目的交换任务中。
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
