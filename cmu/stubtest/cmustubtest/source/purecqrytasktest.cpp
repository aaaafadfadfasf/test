
#include "cms/cms_const.h"
#include "cms/cms_errorcode.h"
#include "cms/cmu/cmu_event.h"
#include "cms/cmu/cmu_proto.h"
#include "cms/natagent/natagent.h"

#include "purecqrytasktest.h"
#include "cmustubtestconst.h"
#include "cmustubtestdata.h"
#include "cmustubtestcoreapp.h"
#include "culogintasktest.h"
#include "cmustubtestconf.h"
/*
qrypurec "3d31caec96534a5498cb1aca2de064df@jiangsu"
*/

CPuRecQryTask::CPuRecQryTask(CInstExt *pcInst) : CSipTask(pcInst)
{
    m_tSipTransId = INVALID_TRANS_ID;
    m_tSipDlgId = INVALID_DIALOG_ID;
    m_bIsQryProgress = false;
}

CPuRecQryTask::~CPuRecQryTask()
{
    if (!m_strQryTag.empty())
    {
        g_cStubQryTaskList.Erase(m_strQryTag);
    }  
}

void CPuRecQryTask::InitStateMachine()
{
    CStateProc cWaitProc;
    cWaitProc.ProcMsg = (CTask::PFProcMsg)&CPuRecQryTask::OnWait;
    cWaitProc.ProcErrMsg = (CTask::PFProcMsg)&CPuRecQryTask::OnWait;
    cWaitProc.TimerPoll = (CTask::PFTimerPoll)&CPuRecQryTask::OnWaitTimer;
    AddRuleProc(Wait, cWaitProc);

    CStateProc cRspProc;
    cRspProc.ProcMsg = (CTask::PFProcMsg)&CPuRecQryTask::OnRsp;
    cRspProc.ProcErrMsg = (CTask::PFProcMsg)&CPuRecQryTask::OnRsp;
    cRspProc.TimerPoll = (CTask::PFTimerPoll)&CPuRecQryTask::OnRspTimer;
    AddRuleProc(Rsp, cRspProc);

    CStateProc cNtfProc;
    cNtfProc.ProcMsg = (CTask::PFProcMsg)&CPuRecQryTask::OnNtf;
    cNtfProc.ProcErrMsg = (CTask::PFProcMsg)&CPuRecQryTask::OnNtf;
    cNtfProc.TimerPoll = (CTask::PFTimerPoll)&CPuRecQryTask::OnNtfTimer;
    AddRuleProc(Ntf, cNtfProc);

    CStateProc cWaitRspProc;
    cWaitRspProc.ProcMsg = (CTask::PFProcMsg)&CPuRecQryTask::OnWaitRsp;
    cWaitRspProc.ProcErrMsg = (CTask::PFProcMsg)&CPuRecQryTask::OnWaitRsp;
    cWaitRspProc.TimerPoll = (CTask::PFTimerPoll)&CPuRecQryTask::OnWaitRspTimer;
    AddRuleProc(WaitRsp, cWaitRspProc);

    CStateProc cHoldProc;
    cHoldProc.ProcMsg = (CTask::PFProcMsg)&CPuRecQryTask::OnAck;
    cHoldProc.ProcErrMsg = (CTask::PFProcMsg)&CPuRecQryTask::OnAck;
    cHoldProc.TimerPoll = (CTask::PFTimerPoll)&CPuRecQryTask::OnAckTimer;
    AddRuleProc(Ack, cHoldProc);

    NextState(Wait);
}

void CPuRecQryTask::PrintData()
{
    CTask::PrintData();
}

u32 CPuRecQryTask::OnWait(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_FAIL;
    switch(pcMsg->event)
    {
    case PU_REC_QRY_REQ:
        {         
            string strDevUri;
            if (pcMsg->content == NULL)
            {
                strDevUri = TEST_PU_URI;
            }
            else
            {
                const char* szPuUri = (const char*) (pcMsg->content);
                strDevUri = szPuUri;
            }

            CCuLoginTask* ptCuLoginTask = NULL;

            Iterator pPos;
            string strSession;
            CLoginSession* pTask = NULL;
            while (!pPos.End())
            {
                if (g_cCuList.Iterate(pPos, strSession, pTask) && pTask != NULL)
                {
                    ptCuLoginTask = (CCuLoginTask*)(pTask);
                    m_strCuSession = strSession;
                    break;
                }
            }

            if (ptCuLoginTask == NULL)
            {
                TASKLOG(ERROR_LEV, "没有cu用户登录\n");
                return PROCMSG_DEL;
            }

            CPuData* ptPuData = g_cPuList.FindPuData(strDevUri);
            if (ptPuData == NULL)
            {
                TASKLOG(ERROR_LEV, "设备[%s]不存在, 创建新设备\n",
                    strDevUri.c_str());

                CPuData* ptPuData = new CPuData;
                ptPuData->SetDevUri(strDevUri);
                g_cPuList.AddPuData(ptPuData);
            }

            CPuRecQryReq cReq;
            cReq.GetDevChn().SetDevUri(strDevUri);
            cReq.GetDevChn().SetChnNO(0);
            cReq.SetRecType(PU_EVENT_TYPE_FILE);
            CCMSTime cStartTime;
            cStartTime.SetLocalTime(2014, 03, 20, 6,50, 1);
            CCMSTime cEndTime;
            cEndTime.SetLocalTime(2014, 03, 20, 6, 59, 1);
            cReq.GetTimeRange().SetStartTime(cStartTime.ToString());
            cReq.GetTimeRange().SetEndTime(cEndTime.ToString());

            m_tReq = cReq;

            PostMsgReq(cReq, GetCmuUri());

            //等待Rsp
            NextState(Rsp);

            g_cStubQryTaskList[cReq.GetQueryTag()] = this;

            m_strQryTag = cReq.GetQueryTag();

            dwRet = PROCMSG_OK;
        }
        break;

    default:
        {
            TASKLOG(MOD_CU, WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }

    return dwRet;
}

u32 CPuRecQryTask::OnWaitTimer()
{
    return TIMERPOLL_DONE;
}

u32 CPuRecQryTask::OnRsp(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_FAIL;
    switch(pcMsg->event)
    {
    case PU_REC_QRY_RSP:
        {
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;

            CPuRecQryRsp cRsp;
            pcOspSipMsg->GetMsgBody(cRsp);

            if (cRsp.GetErrorCode() == CMS_SUCCESS)
            {
                //成功
                TASKLOG(MOD_CU, EVENT_LEV, "前端录像查询成功. 总数[%d]\n",
                    cRsp.GetTotalEntryNum());

                if (cRsp.GetTotalEntryNum() == 0)
                {
                    dwRet = PROCMSG_DEL;
                }
                else
                {
                    dwRet = PROCMSG_OK;
                    NextState(Ntf);
                }
            }
            else
            {
                //失败
                TASKLOG(MOD_CU, ERROR_LEV, "前端录像查询成功失败\n");
                return PROCMSG_DEL;
            }
        }
        break;

    default:
        {
            TASKLOG(MOD_CU, WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }

    return dwRet;
}

u32 CPuRecQryTask::OnRspTimer()
{
    if (GetCurStateHoldTime() > 30000)
    {
        TASKLOG(MOD_CU, ERROR_LEV, "query pu record timeout!\n");
        return TIMERPOLL_DEL;
    }

    return TIMERPOLL_DONE;
}

u32 CPuRecQryTask::OnNtf(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_FAIL;
    switch(pcMsg->event)
    {
    case PU_REC_QRY_NTF_REQ:
        {
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
            if (pcOspSipMsg == NULL)
            {
                TASKLOG(ERROR_LEV, "ospsip 消息为空\n");
                return PROCMSG_DEL;
            }

            CPuRecQryNtfReq cReq;
            pcOspSipMsg->GetMsgBody(cReq);

            for (u32 i=0; i<cReq.GetRecInfoList().size(); i++)
            {
                m_cRecInfoList.push_back(cReq.GetRecInfoList().at(i));
            }

            CPuRecQryNtfRsp cRsp;
            cRsp.SetHeadFromReq(cReq);

            TASKLOG(MOD_CU, EVENT_LEV, "前端录像通知:[%s]\n",
                cReq.ToXml().c_str());

            PostMsgRsp(pcOspSipMsg->GetSipTransID(), cRsp);

            if (cReq.IsLastNtf())
            {
                TASKLOG(MOD_CU, ERROR_LEV, "前端录像通知结束,共[%lu]个，开始删除\n",
                    m_cRecInfoList.size());

                CPuRecordDelReq cRecDelReq;
                cRecDelReq.SetDevChn(m_tReq.GetDevChn());
                cRecDelReq.SetRecInfo(m_cRecInfoList.at(0));

                //转发消息
                PostReq(KDSIP_EVENT_MESSAGE_REQ, cRecDelReq, GetCmuUri());

                //等待应答
                NextState(WaitRsp);


                

#if 0
                TASKLOG(MOD_CU, ERROR_LEV, "前端录像通知结束,共[%lu]个，开始放像\n",
                    m_cRecInfoList.size());

                //开始放像
                //构造请求
                m_tInviteReq.SetSession(m_strCuSession);

                TChannel tSrcChn = m_tReq.GetDevChn();    //源通道

                int nCuChn = 1;
                TChannel tDstChn;    //目的设备通道
                tDstChn.SetDevUri(m_strCuSession);
                tDstChn.SetChnNO(nCuChn);

                m_tInviteReq.SetSrcChn(tSrcChn);
                m_tInviteReq.SetDstChn(tDstChn);

                TSdp tSdp;
                tSdp.SetOwner(tDstChn);
                tSdp.SetSessionName(SDP_SESSION_PLAYBACK);

                TNetAddr tNetAddr(g_cCmuConf.m_tLocalAddr.tIpAddr, 60128+nCuChn*4);
                TNetAddrList tAddrList;
                tAddrList.push_back(tNetAddr);

                TMediaDesc tVideoMediaDesc;
                tVideoMediaDesc.SetMediaType(MEDIA_TYPE_VIDEO);
                tVideoMediaDesc.AddMediaTransChn(TMediaTransChannel(MEDIA_TRANS_CHN_RTP, tAddrList));

                tAddrList[0].SetNetPort(tAddrList[0].GetNetPort()+1);
                tVideoMediaDesc.AddMediaTransChn(TMediaTransChannel(MEDIA_TRANS_CHN_RTCP, tAddrList));
                tSdp.AddMediaDesc(tVideoMediaDesc);


                TMediaDesc tAudioMediaDesc;
                tAudioMediaDesc.SetMediaType(MEDIA_TYPE_AUDIO);
                tAddrList[0].SetNetPort(tAddrList[0].GetNetPort()+2);
                tAudioMediaDesc.AddMediaTransChn(TMediaTransChannel(MEDIA_TRANS_CHN_RTP, tAddrList));

                tAddrList[0].SetNetPort(tAddrList[0].GetNetPort()+3);
                tAudioMediaDesc.AddMediaTransChn(TMediaTransChannel(MEDIA_TRANS_CHN_RTCP, tAddrList));
                tSdp.AddMediaDesc(tAudioMediaDesc);

                TSdpUri tSdpUri; 
                tSdpUri.SetPuChn(m_tReq.GetDevChn());
                tSdpUri.SetPlaybackType(PLAYBACK_TYPE_PU);
                const TPuRecInfo& tRecInfo = m_cRecInfoList.at(0);
                tSdpUri.SetRecToken(tRecInfo.GetRecToken().ToString());
                tSdp.SetUri(tSdpUri);
                
                TTimeRange tTimeRange;
                CCMSTime tStartTime;
                tStartTime.SetTime(1362614400);
                tTimeRange.SetStartTime(tStartTime.ToString());
                CCMSTime tEndTime;
                tEndTime.SetTime(1362700800);
                tTimeRange.SetEndTime(tEndTime.ToString());            
                //tSdp.SetTimeRange(tTimeRange);
                tSdp.SetTimeRange(tRecInfo.GetTimeRange());

                //tSdp.SetDirectTransfer(true);
                /*
                TChannel            owner;              //sdp的创建者，即mediaDescList是属于谁的
                TSdpSessionName     sessionName;        //操作类型
                TSdpUri             uri;                //放像、下载INVITE_REQ时必须填；浏览时不填
                TTimeRange          timeRange;          //放像、下载INVITE_REQ时必须填；浏览时可以不填
                TMediaDescList      mediaDescList;
                TExtInfo            extInfo;
                */

                m_tInviteReq.SetSdp(tSdp);

                //转发消息
                PostReq(KDSIP_EVENT_INVITE_REQ, m_tInviteReq, GetCmuUri());

                //等待应答
                NextState(WaitRsp);
#endif
                
            }
        
            dwRet = PROCMSG_OK;
        }
        break;

    default:
        {
            TASKLOG(MOD_CU, WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }

    return dwRet;
}

u32 CPuRecQryTask::OnNtfTimer()
{
    return TIMERPOLL_DONE;
}

u32 CPuRecQryTask::OnWaitRsp(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_DEL;

    COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
    if (pcOspSipMsg == NULL)
    {
        TASKLOG(ERROR_LEV, "ospsip 消息为空\n");
        return dwRet;
    }

    switch(pcMsg->event)
    {
    case PU_RECORD_DEL_RSP:
        {
            CPuRecordDelRsp cRsp;
            pcOspSipMsg->GetMsgBody(cRsp);

            if (cRsp.GetErrorCode() == CMS_SUCCESS)
            {
                TASKLOG(CRITICAL_LEV, "删除录像成功\n");
            }
            else
            {
                TASKLOG(CRITICAL_LEV, "删除录像失败，errCode[%d]\n",
                    cRsp.GetErrorCode());
            }

            return PROCMSG_OK;
        }
        break;

    case INVITE_RSP:
        {
            CInviteRsp cRsp;
            pcOspSipMsg->GetMsgBody(cRsp);
            m_tSipDlgId = pcOspSipMsg->GetSipDlgID();

            if (cRsp.GetErrorCode() == CMS_SUCCESS)
            {
                TASKLOG(CRITICAL_LEV, "对设备[%s]Invite到[%s]成功\n", 
                    m_tInviteReq.GetSrcChn().ToString().c_str(), m_tInviteReq.GetDstChn().ToString().c_str());

                CInviteACK cInviteAck;
                cInviteAck.SetSession(m_tInviteReq.GetSession());
                if(PROCMSG_OK!= PostInDlgReq(KDSIP_EVENT_INVITE_ACK, cInviteAck, m_tSipDlgId))
                {
                    TASKLOG(ERROR_LEV, "发送KDSIP_EVENT_INVITE_ACK失败！\n");
                }

                NextState(Ack);

                //开始punch    
                TMediaDescList& tDstMediaTransDescList = cRsp.GetSdp().GetMediaDescList();

                int dwPunchNum = 0;
                TPunchInfo atPunchInfo[NATAGENT_MAX_PORT_NUM]={0};

                int nCuChn = 1;
                int nPuRecPortStart = 60128+nCuChn*4;

                for(TMediaDescList::iterator itrMediaDesc = tDstMediaTransDescList.begin(); 
                    itrMediaDesc!=tDstMediaTransDescList.end(); itrMediaDesc++)
                {
                    ///////////////////////////////////////////////////////////////
                    //遍历目的TMediaTransChannelList
                    TMediaTransChannelList& tMediaTransChnList = itrMediaDesc->GetMediaTransChnList();

                    for(TMediaTransChannelList::iterator itrTransChn = tMediaTransChnList.begin(); 
                        itrTransChn!=tMediaTransChnList.end(); itrTransChn++)
                    {
                        const TMediaTransChannel& tMediaTransChn = *itrTransChn;
                        if (!tMediaTransChn.GetMediaChannelKey().empty())
                        {
                            if (dwPunchNum >= NATAGENT_MAX_PORT_NUM)
                            {
                                CLASSLOG(ERROR_LEV, "TransChns to punch exceed NATAGENT_MAX_PORT_NUM, IGNORE remnant!\n");
                                break;
                            }

                            std::ostringstream oss;
                            oss<<tMediaTransChn;
                            CLASSLOG(CRITICAL_LEV, "MediaTransChn need to Punch:[%s]\n", oss.str().c_str());

                            //PunchKey
                            atPunchInfo[dwPunchNum].m_tPunchKey.m_wLen = tMediaTransChn.GetMediaChannelKey().length();
                            strncpy(atPunchInfo[dwPunchNum].m_tPunchKey.m_achKey, tMediaTransChn.GetMediaChannelKey().c_str(), sizeof(atPunchInfo[dwPunchNum].m_tPunchKey.m_achKey));

                            //LocalPort
                            if (itrMediaDesc->GetMediaType() == MEDIA_TYPE_VIDEO)
                            {
                                if (tMediaTransChn.GetTransChnType() == MEDIA_TRANS_CHN_RTP)
                                {
                                    atPunchInfo[dwPunchNum].m_wLocalPort = nPuRecPortStart+0;
                                }
                                else
                                {
                                    atPunchInfo[dwPunchNum].m_wLocalPort = nPuRecPortStart+1;
                                }
                            }
                            else
                            {
                                if (tMediaTransChn.GetTransChnType() == MEDIA_TRANS_CHN_RTP)
                                {
                                    atPunchInfo[dwPunchNum].m_wLocalPort = nPuRecPortStart+2;
                                }
                                else
                                {
                                    atPunchInfo[dwPunchNum].m_wLocalPort = nPuRecPortStart+3;
                                }
                            }                  

                            //RemoteAddrList
                            TNatAddr* ptRemoteAddrList = new TNatAddr[tMediaTransChn.GetAddrList().size()];
                            memset(ptRemoteAddrList, 0, sizeof(tMediaTransChn.GetAddrList().size()));

                            s32 nValidAddrsNum = 0;
                            for (TNetAddrList::const_iterator itrNetAddr = tMediaTransChn.GetAddrList().begin(); 
                                itrNetAddr < tMediaTransChn.GetAddrList().end(); itrNetAddr++)
                            {
                                if (Kdm_IsValidIPStr(itrNetAddr->GetNetIp().c_str()) && Kdm_IsValidPort(itrNetAddr->GetNetPort()))
                                {
                                    Cms2Na_NetAddr(*itrNetAddr, ptRemoteAddrList[nValidAddrsNum]);

                                    nValidAddrsNum++;
                                }
                                else
                                {
                                    CLASSLOG(WARNING_LEV, "IGNORE invalid address [%s:%d]!\n", itrNetAddr->GetNetIp().c_str(), itrNetAddr->GetNetPort());
                                }
                            }

                            atPunchInfo[dwPunchNum].m_wRemoteAddrsNum = nValidAddrsNum;
                            atPunchInfo[dwPunchNum].m_ptRemoteAddrList = ptRemoteAddrList;

                            dwPunchNum++;
                        }
                    }           
                }

                //开始Punch
                u32 dwSeqNum = 0;
                ENATAGENT_OPR_RESULT eNAResult = NATAGENT_OK;

                if (dwPunchNum != 0 )
                {
                    eNAResult = NAPunch(emSelfBind, dwPunchNum, atPunchInfo, NATAGENT_INVALID_IP_STR, &dwSeqNum, this);
                    if (NATAGENT_OK != eNAResult)
                    {
                        CLASSLOG(ERROR_LEV, "NAPunch is fail!\n");

                        return PROCMSG_DEL;
                    }
                    else
                    {
                        CLASSLOG(CRITICAL_LEV, "NAPunch is successful.\n");
                    }

                    //释放资源
                    for (s32 dwPunIdx = 0; dwPunIdx < dwPunchNum; dwPunIdx++)
                    {
                        SAFE_DEL_ARRAY_PTR(atPunchInfo[dwPunIdx].m_ptRemoteAddrList);
                    }
                }
                else
                {
                    CLASSLOG( WARNING_LEV, "There is no TransChn need to punch!\n");
                }

                return PROCMSG_OK;
            }
            else
            {
                TASKLOG(ERROR_LEV, "对设备[%s]Invite到[%s]失败，错误码[%d]\n", 
                    m_tInviteReq.GetSrcChn().ToString().c_str(), m_tInviteReq.GetDstChn().ToString().c_str(), 
                    cRsp.GetErrorCode());

                return PROCMSG_DEL;
            }
        }
        break;

    default:
        {
            TASKLOG(MOD_CU, WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }

    return dwRet;
}

u32 CPuRecQryTask::OnWaitRspTimer()
{
    //应答超时
    if (GetCurStateHoldTime() > CMU_REQ_RSP_TASK_TIMEOUT)
    {
        TASKLOG(ERROR_LEV, "Dev[%s]应答超时, Msg[%s-%d]请求失败\n", 
            m_tInviteReq.GetSrcChn().ToString().c_str(), m_tInviteReq.GetEventStr().c_str(), m_tInviteReq.GetEventId());

        return TIMERPOLL_DEL;
    }

    return TIMERPOLL_DONE;
}

u32 CPuRecQryTask::OnAck(CMessage *const pcMsg)
{
    switch(pcMsg->event)
    {
    case PLAYBACK_PROGRESS_NTF_REQ:
        {
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
            CPlaybackProgressNtfReq cReq;
            pcOspSipMsg->GetMsgBody(cReq);

            COspTime cCreateTime((time_t)cReq.GetStartPlayTime());
            COspTime cCurTime((time_t)cReq.GetCurPlayTime());

            TASKLOG(EVENT_LEV, "PuPlayRec progress ntf: IsEnd[%d], CreateTime[%s]-CurTime[%s]-TotalTime[%us]\n",
                cReq.GetIsPlayFinished(), cCreateTime.GetString().c_str(), 
                cCurTime.GetString().c_str(), cReq.GetTotalPlayTime());

            CPlaybackProgressNtfRsp cRsp;
            cRsp.SetHeadFromReq(cReq);
            cRsp.SetErrorCode(CMS_SUCCESS);

            u32 dwResult = PostRsp(KDSIP_EVENT_INFO_RSP, pcOspSipMsg->GetSipTransID(), cRsp);
            TASKLOG(MOD_SWITCH, EVENT_LEV, "发送KDSIP_EVENT_INFO_RSP, ResultCode[%lu].\n", dwResult);

            if (!cReq.GetIsPlayFinished())
            {
                //m_bIsQryProgress = true;              
            }
            else
            {
                TASKLOG(EVENT_LEV, "PuPlayRec progress ntf: 放像结束\n");
                return PROCMSG_OK;
            }         
        }
        break;

    case END_OF_FILE_NTF_REQ:
        {
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
            CEndOfFileNtfReq cReq;
            pcOspSipMsg->GetMsgBody(cReq);

            CEndOfFileNtfRsp cRsp;
            cRsp.SetHeadFromReq(cReq);

            PostRsp(KDSIP_EVENT_MESSAGE_DIALOG_RSP, pcOspSipMsg->GetSipTransID(), cRsp);

            TASKLOG(EVENT_LEV, "PuPlayRec End ntf: 放像结束\n");

            string strNull;
            PostInDlgReq(KDSIP_EVENT_INVITE_BYE_REQ, strNull, m_tSipDlgId);

            return PROCMSG_DEL;
        }
        break;

    case VCR_CTRL_REQ:
        {
            /*
            #define VCRCMD_START    "START"         //开始放像
            #define VCRCMD_STOP     "STOP"          //停止放像
            #define VCRCMD_PAUSE    "PAUSE"         //暂停放像
            #define VCRCMD_RESUME   "RESUME"        //恢复放像
            #define VCRCMD_SEEK     "SEEK"          //时间定位
            #define VCRCMD_FASTPLAY "FASTPLAY"      //快放
            #define VCRCMD_SLOWPLAY "SLOWPLAY"      //慢放
            #define VCRCMD_ONEFRAME "ONEFRAME"      //单帧播放
            #define VCRCMD_KEYFRAME "KEYFRAME"      //只放关键帧
            */
            TVcrCtrlParam tVcrParam = *(TVcrCtrlParam*)pcMsg->content;

            string strVcrCmd = tVcrParam.m_szVcrCmd;

            CVcrCtrlReq cReq;
            cReq.SetVcrCmd(strVcrCmd);

            /*
            int     playRate;       //放像速率，正数为正反，负数为倒放
            int     seekTime;       //相对时间
            */

            if (strVcrCmd == "START")
            {
            }
            else if (strVcrCmd == "STOP")
            {
            }
            else if (strVcrCmd == "PAUSE")
            {
            }
            else if (strVcrCmd == "RESUME")
            {
            }
            else if (strVcrCmd == "SEEK")
            {
                cReq.SetSeekTime(60);
            }
            else if (strVcrCmd == "FASTPLAY")
            {
                cReq.SetPlayRate(2);
            }
            else if (strVcrCmd == "SLOWPLAY")
            {
                cReq.SetPlayRate(2);
            }
            else if (strVcrCmd == "ONEFRAME")
            {
            }
            else if (strVcrCmd == "KEYFRAME")
            {
            }
            else
            {
                TASKLOG(EVENT_LEV, "未知VcrCmd[%s]\n", 
                    strVcrCmd.c_str());
                return PROCMSG_FAIL;
            }

            PostInDlgReq(KDSIP_EVENT_INFO_REQ, cReq, m_tSipDlgId);

            TASKLOG(CRITICAL_LEV, "发送vcr控制\n");

        }
        break;

    case VCR_CTRL_RSP:
        {
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
            CVcrCtrlRsp cRsp;
            pcOspSipMsg->GetMsgBody(cRsp);
            if (cRsp.GetErrorCode() == CMS_SUCCESS)
            {
                TASKLOG(CRITICAL_LEV, "vcr控制成功\n");
            }
            else
            {
                TASKLOG(CRITICAL_LEV, "vcr控制失败，错误码[%d]\n", 
                    cRsp.GetErrorCode());
            }
        }
        break;

        //cmustubtest客户端自己发送的内部主动bye请求
    case BYE_REQ:
        {
            TASKLOG(ERROR_LEV, "bye devDev[%s], dlg[%d]\n", 
                m_tInviteReq.GetSrcChn().ToString().c_str(), m_tSipDlgId);

            string strNull;
            PostInDlgReq(KDSIP_EVENT_INVITE_BYE_REQ, strNull, m_tSipDlgId);
            return PROCMSG_DEL;
        }
        break;

        //收到服务端的BYE请求
    case OSP_SIP_BYE_REQ:
        {
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;

            s32 nByeReason = CMS_SUCCESS;
            if (pcOspSipMsg->GetMsgBodyLen() != 0)
            {
                CByeReq cByeReq;
                pcOspSipMsg->GetMsgBody(cByeReq);

                nByeReason = cByeReq.GetReason();
            }

            TKDSIP_DIALOG_ID tReqDlgId = pcOspSipMsg->GetSipDlgID();

            TASKLOG(MOD_SWITCH, CRITICAL_LEV, "收到设备DlgID[%lu]的ByeReq, Reason[%d]！\n", 
                tReqDlgId, nByeReason);

            ////////////////////////////////////////////////////////////
            //向请求方回应应答
            CByeRsp cRsp;

            u32 dwResult = PostRsp(KDSIP_EVENT_INVITE_BYE_RSP, pcOspSipMsg->GetSipTransID(), cRsp);
            TASKLOG(MOD_SWITCH, CRITICAL_LEV, "发送KDSIP_EVENT_INVITE_BYE_RSP, ResultCode[%lu].\n", dwResult);

            return PROCMSG_DEL;
        }

    default:
        {
            TASKLOG(MOD_SWITCH, WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

            return PROCMSG_FAIL;
        }
        break;
    }

    return PROCMSG_OK;
}

u32 CPuRecQryTask::OnAckTimer()
{
    if (m_bIsQryProgress)
    {
        CPlaybackProgressQueryReq cReq;
        cReq.SetPlaybackSessUri("fxg_test");
        PostInDlgReq(KDSIP_EVENT_INFO_REQ, cReq, m_tSipDlgId);
    }
    
    return TIMERPOLL_DONE;
}