
#include "cms/cms_const.h"
#include "cms/cms_errorcode.h"
#include "cms/cmu/cmu_event.h"
#include "cms/cmu/cmu_proto.h"

#include "twschemesavetasktest.h"
#include "cmustubtestconst.h"
#include "cmustubtestdata.h"
#include "cmustubtestcoreapp.h"

/*
//tvs相关测试命令

savescm "tvwallid_123456789@jiangsu" "s1" 0 0 0
savescm "tvwallid_123456789@jiangsu" "s1" 1 0 0
savescm "tvwallid_123456789@jiangsu" "s1" 1 1 0
savescm "tvwallid_123456789@jiangsu" "s1" 1 1 1

savescm "tvwallid_123456789@jiangsu" "s2" 0 0 0
savescm "tvwallid_123456789@jiangsu" "s2" 1 0 0
savescm "tvwallid_123456789@jiangsu" "s2" 1 1 0
savescm "tvwallid_123456789@jiangsu" "s2" 1 1 1

savescm "tvwallid_123456789@jiangsu" "s3" 0 0 0
savescm "tvwallid_123456789@jiangsu" "s3" 1 0 0
savescm "tvwallid_123456789@jiangsu" "s3" 1 1 0
savescm "tvwallid_123456789@jiangsu" "s3" 1 1 1

savescm "tvwallid_123456789@jiangsu" "scm_tv_poll" 1 1 2

savescm "bff1bace1caa4523b4a3b601ddd7e28a@jiangsu" "null" 0 0 0
savescm "bff1bace1caa4523b4a3b601ddd7e28a@jiangsu" "scm_tv_poll" 2 1 2

delscm "tvwallid_123456789@jiangsu" "s1"
delscm "tvwallid_123456789@jiangsu" "s2"
delscm "tvwallid_123456789@jiangsu" "scm_tv_poll"
delscm "tvwallid_123456789@jiangsu" "TvsSysTempScheme"

delscm "3be656fe01d948c2a6dd789c4e0bf08e@jiangsu" "TvsSysTempScheme"

loadscm "tvwallid_123456789@jiangsu" "s1"
loadscm "tvwallid_123456789@jiangsu" "s2"
loadscm "tvwallid_123456789@jiangsu" "scm_tv_poll"

loadscm "bff1bace1caa4523b4a3b601ddd7e28a@jiangsu" "null"
loadscm "bff1bace1caa4523b4a3b601ddd7e28a@jiangsu" "scm_tv_poll"

ontv "tvwallid_123456789@jiangsu" 0 0
ontv "b3af42ca76b145d68fa68a8a3ca92444@jiangsu" 0 0

offtv "tvwallid_123456789@jiangsu" 0 0
offtv "3be656fe01d948c2a6dd789c4e0bf08e@jiangsu" 0 0
offtv "bff1bace1caa4523b4a3b601ddd7e28a@jiangsu" 1 0

tvpoll "tvwallid_123456789@jiangsu" 1 0 "start"
tvpoll "tvwallid_123456789@jiangsu" 1 0 "pause"
tvpoll "tvwallid_123456789@jiangsu" 1 0 "resume"
tvpoll "tvwallid_123456789@jiangsu" 1 0 "stop"

tvpoll "bff1bace1caa4523b4a3b601ddd7e28a@jiangsu" 1 0 "start"
tvpoll "bff1bace1caa4523b4a3b601ddd7e28a@jiangsu" 1 0 "pause"
tvpoll "bff1bace1caa4523b4a3b601ddd7e28a@jiangsu" 1 0 "resume"
tvpoll "bff1bace1caa4523b4a3b601ddd7e28a@jiangsu" 1 0 "stop"

setscmgrp "tvwallid_123456789@jiangsu"
getscmgrp "tvwallid_123456789@jiangsu"


scmpoll "bff1bace1caa4523b4a3b601ddd7e28a@jiangsu" "start"
scmpoll "bff1bace1caa4523b4a3b601ddd7e28a@jiangsu" "stop"
scmpoll "tvwallid_123456789@jiangsu" "start"
scmpoll "tvwallid_123456789@jiangsu" "pause"
scmpoll "tvwallid_123456789@jiangsu" "resume"
scmpoll "tvwallid_123456789@jiangsu" "stop"

twss "tvwallid_123456789@jiangsu"

twss "bff1bace1caa4523b4a3b601ddd7e28a@jiangsu"

*/

//用于通过OSPSIP进行UA和UA之间直接通信测试
//#define  TVS_DIRECT_ADDR   "172.16.64.86:5072"

#ifdef TVS_DIRECT_ADDR
LPCSTR g_szTvsDirectAddr = TVS_DIRECT_ADDR;
#else
LPCSTR g_szTvsDirectAddr = NULL;
#endif


CTwSchemeSaveTask::CTwSchemeSaveTask(CInstExt *pcInst) : CSipTask(pcInst)
{
}

CTwSchemeSaveTask::~CTwSchemeSaveTask()
{
}

void CTwSchemeSaveTask::InitStateMachine()
{
    CStateProc cWaitProc;
    cWaitProc.ProcMsg = (CTask::PFProcMsg)&CTwSchemeSaveTask::OnWait;
    cWaitProc.ProcErrMsg = (CTask::PFProcMsg)&CTwSchemeSaveTask::OnWait;
    cWaitProc.TimerPoll = (CTask::PFTimerPoll)&CTwSchemeSaveTask::OnWaitTimer;
    AddRuleProc(Wait, cWaitProc);

    CStateProc cRspProc;
    cRspProc.ProcMsg = (CTask::PFProcMsg)&CTwSchemeSaveTask::OnRsp;
    cRspProc.ProcErrMsg = (CTask::PFProcMsg)&CTwSchemeSaveTask::OnRsp;
    cRspProc.TimerPoll = (CTask::PFTimerPoll)&CTwSchemeSaveTask::OnRspTimer;
    AddRuleProc(Rsp, cRspProc);

    NextState(Wait);
}

void CTwSchemeSaveTask::PrintData()
{
    CTask::PrintData();
}

u32 CTwSchemeSaveTask::OnWait(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_FAIL;
    switch(pcMsg->event)
    {
    case TVWALL_SCHEME_SAVE_REQ:
        {         
            TTwSchemeId* ptSchemeId = (TTwSchemeId*)(pcMsg->content);

            static int s_MagicNum = 0;

            TTvWallScheme         tScheme;                 //预案详细信息
            tScheme.SetTvWallId(ptSchemeId->m_szTwId);
            tScheme.SetSchemeName(ptSchemeId->m_szSchemeName);

            CTvWinList& tTvWinList = tScheme.GetTvWinList();
            for (size_t iTv=0; iTv<ptSchemeId->m_nTvNum; iTv++)
            {
                TTvWin tTvWin;
                tTvWin.SetTvWinId(iTv);
                ETvWinStype eStyle = ETvWinStype(ptSchemeId->m_nTvDivNum);
                tTvWin.SetStyle(eStyle);

                //电视分画面表
                TTvWinDivList& tTvWinDivList = tTvWin.GetDivList();
                for (size_t iTvDiv=0; iTvDiv<(size_t)eStyle; iTvDiv++)
                {
                    TTvWinDiv tTvDivWin;
                    tTvDivWin.SetTvWinDivId(iTvDiv);

                    //画面轮询
                    CTvPollList& tPollList = tTvDivWin.GetPollList();
                    for (size_t iPollIndex=0; iPollIndex<ptSchemeId->m_nPollNum; iPollIndex++)
                    {
                        TTvPollUnit tPollUnit;
                        tPollUnit.SetPollInex(iPollIndex);
                        tPollUnit.GetEncChn().SetDevUri(GetTestPuUri(iPollIndex));
                        tPollUnit.GetEncChn().SetChnNO(0);
                        tPollUnit.SetPollTime(120);

                        tPollList.push_back(tPollUnit);
                    }

                    tTvWinDivList.push_back(tTvDivWin);
                }

                tTvWinList.push_back(tTvWin);
            }



            CTvWallSchemeSaveReq cReq;
            cReq.SetScheme(tScheme);

            PostMsgReq(cReq, GetTvsUri(), g_szTvsDirectAddr);

            //等待Rsp
            NextState(Rsp);

            dwRet = PROCMSG_OK;
        }
        break;

    case TVWALL_SCHEME_DEL_REQ:
        {         
            TTwSchemeId* ptSchemeId = (TTwSchemeId*)(pcMsg->content);

            static int s_MagicNum = 0;

            TTvWallScheme         tScheme;                 //预案详细信息
            tScheme.SetTvWallId(ptSchemeId->m_szTwId);
            tScheme.SetSchemeName(ptSchemeId->m_szSchemeName);

            CTvWallSchemeDelReq cReq;
            cReq.SetTvWallId(ptSchemeId->m_szTwId);
            CTvWallSchemeNameList tSchemeList;
            tSchemeList.push_back(ptSchemeId->m_szSchemeName);
            cReq.SetSchemeNameList(tSchemeList);

            PostMsgReq(cReq, GetTvsUri(), g_szTvsDirectAddr);

            //等待Rsp
            NextState(Rsp);

            dwRet = PROCMSG_OK;
        }
        break;

    case TVWALL_SCHEME_LOAD_REQ:
        {         
            TTwSchemeId* ptSchemeId = (TTwSchemeId*)(pcMsg->content);

            static int s_MagicNum = 0;

            TTvWallScheme         tScheme;                 //预案详细信息
            tScheme.SetTvWallId(ptSchemeId->m_szTwId);
            tScheme.SetSchemeName(ptSchemeId->m_szSchemeName);

            CTvWallSchemeLoadReq cReq;
            cReq.SetTvWallId(ptSchemeId->m_szTwId);
            cReq.SetSchemeName(ptSchemeId->m_szSchemeName);

            PostMsgReq(cReq, GetTvsUri());

            //等待Rsp
            NextState(Rsp);

            dwRet = PROCMSG_OK;
        }
        break;

    case REAL_MEDIA_ON_TV_REQ:
        {         
            string strTvChn = (const char*)(pcMsg->content);

            static int s_MagicNum = 0;

            TTvChannel tTvChn;
            tTvChn.FromXml(strTvChn);

            CRealMediaOnTvReq cReq;
            cReq.SetTvChn(tTvChn);
            cReq.GetEncChn().SetDevUri(TEST_PU_URI);
            cReq.GetEncChn().SetChnNO(0);
            cReq.SetTvOccurType(TV_OCCUR_TYPE_HAND);

            PostMsgReq(cReq, GetTvsUri());

            //等待Rsp
            NextState(Rsp);

            dwRet = PROCMSG_OK;
        }
        break;

    case REAL_MEDIA_OFF_TV_REQ:
        {         
            string strTvChn = (const char*)(pcMsg->content);

            static int s_MagicNum = 0;

            TTvChannel tTvChn;
            tTvChn.FromXml(strTvChn);

            CRealMediaOffTvReq cReq;
            cReq.SetTvChn(tTvChn);
            cReq.GetEncChn().SetDevUri(TEST_PU_URI);
            cReq.GetEncChn().SetChnNO(0);
            cReq.SetTvOccurType(TV_OCCUR_TYPE_HAND);

            PostMsgReq(cReq, GetTvsUri());

            //等待Rsp
            NextState(Rsp);

            dwRet = PROCMSG_OK;
        }
        break;

    case TV_POLL_OPERATE_REQ:
        {         
            string strReq = (const char*)(pcMsg->content);

            static int s_MagicNum = 0;

            CTvPollOperateReq cReq;
            cReq.FromXml(strReq);

            CTvPollList& tPollList = cReq.GetPollList();
            for (size_t i=0; i<2; i++)
            {
                TTvPollUnit tUnit;
                tUnit.GetEncChn().SetDevUri(GetTestPuUri(i));
                tUnit.GetEncChn().SetChnNO(0);
                tUnit.SetPollInex(i);
                tUnit.SetPollTime(30);

                tPollList.push_back(tUnit);
            }

            PostMsgReq(cReq, GetTvsUri());

            //等待Rsp
            NextState(Rsp);

            dwRet = PROCMSG_OK;
        }
        break;

    case TVWALL_SCHEME_GROUP_SET_REQ:
        {         
            string strReq = (const char*)(pcMsg->content);

            static int s_MagicNum = 0;

            CTvWallSchemeGroupSetReq cReq;
            cReq.FromXml(strReq);

            PostMsgReq(cReq, GetTvsUri());

            //等待Rsp
            NextState(Rsp);

            dwRet = PROCMSG_OK;
        }
        break;

    case TVWALL_SCHEME_GROUP_GET_REQ:
        {         
            string strReq = (const char*)(pcMsg->content);

            static int s_MagicNum = 0;

            CTvWallSchemeGroupGetReq cReq;
            cReq.FromXml(strReq);

            PostMsgReq(cReq, GetTvsUri());

            //等待Rsp
            NextState(Rsp);

            dwRet = PROCMSG_OK;
        }
        break;

    case TVWALL_SCHEME_POLL_OPERATE_REQ:
        {         
            string strReq = (const char*)(pcMsg->content);

            static int s_MagicNum = 0;

            CTvWallSchemePollOperateReq cReq;
            cReq.FromXml(strReq);

            PostMsgReq(cReq, GetTvsUri());

            //等待Rsp
            NextState(Rsp);

            dwRet = PROCMSG_OK;
        }
        break;

    case TVWALL_STATUS_SS_REQ:
        {         
            string strReq = (const char*)(pcMsg->content);

            static int s_MagicNum = 0;

            CTvWallStatusSsReq cReq;
            cReq.FromXml(strReq);

            PostReq(KDSIP_EVENT_SUBSCRIBE_REQ, cReq, GetTvsUri());

            //等待Rsp
            NextState(Rsp);

            dwRet = PROCMSG_OK;
        }
        break;

    default:
        {
            TASKLOG(MOD_TVS, WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }

    return dwRet;
}

u32 CTwSchemeSaveTask::OnWaitTimer()
{
    return TIMERPOLL_DONE;
}

u32 CTwSchemeSaveTask::OnRsp(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_FAIL;
    switch(pcMsg->event)
    {
    case TVWALL_SCHEME_SAVE_RSP:
        {
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
            if (pcOspSipMsg == NULL)
            {
                TASKLOG(ERROR_LEV, "ospsip 消息为空\n");
                return PROCMSG_DEL;
            }

            CTvWallSchemeSaveRsp cRsp;
            pcOspSipMsg->GetMsgBody(cRsp);

            if (cRsp.GetErrorCode() == CMS_SUCCESS)
            {
                //成功
                TASKLOG(MOD_TVS, EVENT_LEV, "保存预案成功\n");
                return PROCMSG_DEL;
            }
            else
            {
                //失败
                TASKLOG(MOD_TVS, ERROR_LEV, "保存预案失败\n");
                return PROCMSG_DEL;
            }
        }
        break;

    case TVWALL_SCHEME_DEL_RSP:
        {
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
            if (pcOspSipMsg == NULL)
            {
                TASKLOG(ERROR_LEV, "ospsip 消息为空\n");
                return PROCMSG_DEL;
            }

            CTvWallSchemeDelRsp cRsp;
            pcOspSipMsg->GetMsgBody(cRsp);

            if (cRsp.GetErrorCode() == CMS_SUCCESS)
            {
                //成功
                TASKLOG(MOD_TVS, EVENT_LEV, "删除预案成功\n");
                return PROCMSG_DEL;
            }
            else
            {
                //失败
                TASKLOG(MOD_TVS, ERROR_LEV, "删除预案失败\n");
                return PROCMSG_DEL;
            }
        }
        break;

    case TVWALL_SCHEME_LOAD_RSP:
        {
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
            if (pcOspSipMsg == NULL)
            {
                TASKLOG(ERROR_LEV, "ospsip 消息为空\n");
                return PROCMSG_DEL;
            }

            CTvWallSchemeLoadRsp cRsp;
            pcOspSipMsg->GetMsgBody(cRsp);

            if (cRsp.GetErrorCode() == CMS_SUCCESS)
            {
                //成功
                TASKLOG(MOD_TVS, EVENT_LEV, "加载预案成功\n");
                return PROCMSG_DEL;
            }
            else
            {
                //失败
                TASKLOG(MOD_TVS, ERROR_LEV, "加载预案失败\n");
                return PROCMSG_DEL;
            }
        }
        break;

    case REAL_MEDIA_ON_TV_RSP:
        {
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
            if (pcOspSipMsg == NULL)
            {
                TASKLOG(ERROR_LEV, "ospsip 消息为空\n");
                return PROCMSG_DEL;
            }

            CRealMediaOnTvRsp cRsp;
            pcOspSipMsg->GetMsgBody(cRsp);

            if (cRsp.GetErrorCode() == CMS_SUCCESS)
            {
                //成功
                TASKLOG(MOD_TVS, EVENT_LEV, "上墙成功\n");
                return PROCMSG_DEL;
            }
            else
            {
                //失败
                TASKLOG(MOD_TVS, ERROR_LEV, "上墙失败\n");
                return PROCMSG_DEL;
            }
        }
        break;

    case REAL_MEDIA_OFF_TV_RSP:
        {
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
            if (pcOspSipMsg == NULL)
            {
                TASKLOG(ERROR_LEV, "ospsip 消息为空\n");
                return PROCMSG_DEL;
            }

            CRealMediaOffTvRsp cRsp;
            pcOspSipMsg->GetMsgBody(cRsp);

            if (cRsp.GetErrorCode() == CMS_SUCCESS)
            {
                //成功
                TASKLOG(MOD_TVS, EVENT_LEV, "上墙成功\n");
                return PROCMSG_DEL;
            }
            else
            {
                //失败
                TASKLOG(MOD_TVS, ERROR_LEV, "上墙失败\n");
                return PROCMSG_DEL;
            }
        }
        break;

    case TV_POLL_OPERATE_RSP:
        {
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
            if (pcOspSipMsg == NULL)
            {
                TASKLOG(ERROR_LEV, "ospsip 消息为空\n");
                return PROCMSG_DEL;
            }

            CTvPollOperateRsp cRsp;
            pcOspSipMsg->GetMsgBody(cRsp);

            if (cRsp.GetErrorCode() == CMS_SUCCESS)
            {
                //成功
                TASKLOG(MOD_TVS, EVENT_LEV, "轮询操作成功\n");
                return PROCMSG_DEL;
            }
            else
            {
                //失败
                TASKLOG(MOD_TVS, ERROR_LEV, "轮询操作失败\n");
                return PROCMSG_DEL;
            }
        }
        break;

    case TVWALL_SCHEME_GROUP_SET_RSP:
        {
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
            if (pcOspSipMsg == NULL)
            {
                TASKLOG(ERROR_LEV, "ospsip 消息为空\n");
                return PROCMSG_DEL;
            }

            CTvWallSchemeGroupSetRsp cRsp;
            pcOspSipMsg->GetMsgBody(cRsp);

            if (cRsp.GetErrorCode() == CMS_SUCCESS)
            {
                //成功
                TASKLOG(MOD_TVS, EVENT_LEV, "预案组设置成功\n");
                return PROCMSG_DEL;
            }
            else
            {
                //失败
                TASKLOG(MOD_TVS, ERROR_LEV, "预案组设置失败\n");
                return PROCMSG_DEL;
            }
        }
        break;

    case TVWALL_SCHEME_GROUP_GET_RSP:
        {
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
            if (pcOspSipMsg == NULL)
            {
                TASKLOG(ERROR_LEV, "ospsip 消息为空\n");
                return PROCMSG_DEL;
            }

            CTvWallSchemeGroupGetRsp cRsp;
            pcOspSipMsg->GetMsgBody(cRsp);

            if (cRsp.GetErrorCode() == CMS_SUCCESS)
            {
                //成功
                TASKLOG(MOD_TVS, EVENT_LEV, "预案组获取成功\n");
                return PROCMSG_DEL;
            }
            else
            {
                //失败
                TASKLOG(MOD_TVS, ERROR_LEV, "预案组获取失败\n");
                return PROCMSG_DEL;
            }
        }
        break;

    case TVWALL_SCHEME_POLL_OPERATE_RSP:
        {
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
            if (pcOspSipMsg == NULL)
            {
                TASKLOG(ERROR_LEV, "ospsip 消息为空\n");
                return PROCMSG_DEL;
            }

            CTvWallSchemePollOperateRsp cRsp;
            pcOspSipMsg->GetMsgBody(cRsp);

            if (cRsp.GetErrorCode() == CMS_SUCCESS)
            {
                //成功
                TASKLOG(MOD_TVS, EVENT_LEV, "预案轮询成功\n");
                return PROCMSG_DEL;
            }
            else
            {
                //失败
                TASKLOG(MOD_TVS, ERROR_LEV, "预案轮询失败\n");
                return PROCMSG_DEL;
            }
        }
        break;

    case TVWALL_STATUS_SS_RSP:
        {
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
            if (pcOspSipMsg == NULL)
            {
                TASKLOG(ERROR_LEV, "ospsip 消息为空\n");
                return PROCMSG_DEL;
            }

            CTvWallStatusSsRsp cRsp;
            pcOspSipMsg->GetMsgBody(cRsp);

            if (cRsp.GetErrorCode() == CMS_SUCCESS)
            {
                //成功
                TASKLOG(MOD_TVS, EVENT_LEV, "电视墙状态订阅成功\n");
                return PROCMSG_OK;
            }
            else
            {
                //失败
                TASKLOG(MOD_TVS, ERROR_LEV, "电视墙状态订阅失败\n");
                return PROCMSG_DEL;
            }
        }
        break;

    case TVWALL_STATUS_NTF_REQ:
        {
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;

            CTvWallStatusNtyReq cReq;
            pcOspSipMsg->GetMsgBody(cReq);

            CTvWallStatusNtyRsp cRsp;
            cRsp.SetEvent(TVWALL_STATUS_NTF_RSP);
            cRsp.SetErrorCode(CMS_SUCCESS);
            cRsp.SetSeqNum(cReq.GetSeqNum());
            cRsp.SetSession(cReq.GetSession());
            PostRsp(KDSIP_EVENT_NOTIFY_RSP, pcOspSipMsg->GetSipTransID(), cRsp);

            dwRet = PROCMSG_OK;
        }
        break;

    default:
        {
            TASKLOG(MOD_TVS, WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }

    return dwRet;
}

u32 CTwSchemeSaveTask::OnRspTimer()
{
//     if (GetCurStateHoldTime() > 10)
//     {
//         TASKLOG(MOD_TVS, ERROR_LEV, "save scheme timeout!\n");
//         return TIMERPOLL_DEL;
//     }

    return TIMERPOLL_DONE;
}