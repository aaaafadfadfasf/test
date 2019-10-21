#include "cms/ospsip/ospsip.h"
#include "fakesvrapp.h"
#include "stubconfig.h"
#include "cms/cmu/cmu_proto.h"
#include "cms/tas/tas_proto.h"
#include "fakecu.h"

string g_strCuiInTasSess = "Cui_In_Tas_Session";//45b6da3a7da04f2489b42394633fd1b7";
string g_strCuInTasSess = "Cu_In_Tas_Session";//"86241e7368734d17b4cc403ae65b3b73";
string g_strCuInCmuSess = "Cu_In_Cmu_Session";//"330b0ab45ed2490283f602713b141fbc";
bool g_bLogin = false;

string ReplaceSessid( const s8* cstrRawdata, const string &cstrNewSessid )
{
    string str(cstrRawdata);
    if ( cstrNewSessid.empty() )
    {
        return str;
    }

    string::size_type pos1, pos2;
    pos1 = str.find("<session>");
    if ( pos1 == string::npos )
    {
        return str;
    }
    pos1 += strlen("<session>");
    pos2 = str.find("</session>");
    str.replace(pos1, pos2-pos1, cstrNewSessid.c_str());
    return str;
}

string ReplaceSeqno( const s8* cstrRawdata, u32 dwNewSeqno )
{
    string str(cstrRawdata);
    string::size_type pos1, pos2;
    pos1 = str.find("<seqNum>");
    if ( pos1 == string::npos )
    {
        return str;
    }
    pos1 += strlen("<seqNum>");
    pos2 = str.find("</seqNum>");
    s8 szBuf[20] = {0};
    sprintf(szBuf, "%u", dwNewSeqno);
    str.replace(pos1, pos2-pos1, szBuf);
    return str;
}

string ReplaceStr(const string &strXml, const string &strReplace, const s8* lpKeyBegin, const s8* lpEnd)
{
    string strRet(strXml);
    if ( NULL == lpKeyBegin || NULL == lpEnd )
    {
        return strRet;
    }
    int nKeyLen = strlen(lpKeyBegin);
    if ( 0 == nKeyLen || 0 == strlen(lpEnd) )
    {
        return strRet;
    }

    string::size_type pos1 = strXml.find(lpKeyBegin);
    if ( pos1 == string::npos )
    {
        return strRet;
    }
    pos1 += nKeyLen;
    string::size_type pos2 = strXml.find(lpEnd);
    if ( pos2 == string::npos )
    {
        return strRet;
    }
    strRet.replace(pos1, pos2-pos1, strReplace);
    return strRet;
}

void * HbThread(void *pParam)
{
    while ( 1 )
    {
        if ( g_bLogin )
        {
            GetNotify();
        }
        OspDelay(5000);
    }
}


CSrvInst::CSrvInst()
{

}


LPCSTR CSrvInst::GetStrState() const
{
    return "Service";
}



BOOL32 CSrvInst::OnUserInnerCommonMsg( CMessage* const pcMsg )
{
    switch (pcMsg->event)
    {
    case OSP_PROXY_CONNECT:
    case OSP_PROXY_DISCONNECT:
        {
            OspPrintf(TRUE, FALSE, "收到SIP消息[ %s ]，不处理\n", 
                OspExtEventDesc(pcMsg->event).c_str());
            g_bLogin = false;
            return TRUE;
        }
    case OSP_SIP_DISCONNECT:
    case OSP_SIP_MSG_PROC_FAIL:
    case OSP_SIP_DIALOG_TERMINATE:
        {
            OspPrintf(TRUE, FALSE, "收到SIP消息[ %s ]，不处理\n", 
                OspExtEventDesc(pcMsg->event).c_str());
            return TRUE;
        }
        break;
    default:
        break;
    }
    return FALSE;
}

void CSrvInst::DaemonEntry( CMessage* const pcMsg )
{
    COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
    OspPrintf(TRUE, FALSE, "收到业务消息[ %s ], SIP消息[%s]\n", 
        OspExtEventDesc(pcMsg->event).c_str(), pcOspSipMsg->GetSipEventStr().c_str());

    if ( KDSIP_EVENT_INVITE_BYE_REQ == pcOspSipMsg->GetSipEventID() )
    {
        OspPrintf(TRUE, FALSE, "收到Invite Bye Req，发送应答\n");
        //OspSipReleaseDlg(pcOspSipMsg->GetSipDlgID());
        TOspTransation tOspTrans;
        tOspTrans.dwIID = MAKEIID(GetAppID(), GetInsID());
        tOspTrans.dwTaskID = INVALID_TASKNO;

        string strNullMsg;
        OspSipPostRsp(KDSIP_EVENT_INVITE_BYE_RSP, pcOspSipMsg->GetSipTransID(), strNullMsg, tOspTrans);
        return;
    }

    switch (pcMsg->event)
    {
    case TAS_LOGIN_REQ:
        {
            CLoginTasReq cReq;
            pcOspSipMsg->GetMsgBody(cReq);
            CLoginTasRsp cRsp;
            cRsp.SetSeqNum(cReq.GetSeqNum());
            cRsp.SetSession(g_strCuiInTasSess);

            TOspTransation tOspTrans;
            tOspTrans.dwIID = MAKEIID(GetAppID(), GetInsID());
            tOspTrans.dwTaskID = INVALID_TASKNO;
            //tOspTrans.dwSeqNO = cReq.GetSeqNum();
            //tOspTrans.dwSeqNO = atol(GetXmlVal(strMsgReq, XML_SEQ_NUM_NODE).c_str());

            OspSipPostRsp(KDSIP_EVENT_MESSAGE_RSP, pcOspSipMsg->GetSipTransID(), cRsp, tOspTrans);

            TOspTransID tOspTransId;
            tOspTransId.dwIID = MAKEIID(GetAppID(), GetInsID());
            tOspTransId.dwTaskID = INVALID_TASKNO;
            tOspTransId.dwSeqNO = INVALID_SEQ_NO;
            OspSipSetHBParam(pcOspSipMsg->GetSipFromUri(), tOspTransId);
            OspPrintf(TRUE, FALSE, "对TAS[%s]设置心跳\n", 
                pcOspSipMsg->GetSipFromUri().GetURIString().c_str());
        }
        break;
    case TAS_CU_ONLINE_REQ:
        {
            CCuOnlineReq cReq;
            pcOspSipMsg->GetMsgBody(cReq);

            CCuOnlineRsp cRsp;
            cRsp.SetSession(g_strCuiInTasSess);
            cRsp.SetCuSessionId(g_strCuInTasSess);

            TOspTransation tOspTrans;
            tOspTrans.dwIID = MAKEIID(GetAppID(), GetInsID());
            tOspTrans.dwTaskID = INVALID_TASKNO;
            //tOspTrans.dwSeqNO = cReq.GetSeqNum();
            //tOspTrans.dwSeqNO = atol(GetXmlVal(strMsgReq, XML_SEQ_NUM_NODE).c_str());

            OspSipPostRsp(KDSIP_EVENT_MESSAGE_RSP, pcOspSipMsg->GetSipTransID(), cRsp, tOspTrans);
        }
        break;
    case CUI_REG_REQ:
        {
            CCuiRegReq cReq;
            pcOspSipMsg->GetMsgBody(cReq);

            CCuiRegRsp cRsp;
            cRsp.SetSeqNum(cReq.GetSeqNum());
            cRsp.SetSession(g_strCuInCmuSess);

            TOspTransation tOspTrans;
            tOspTrans.dwIID = MAKEIID(GetAppID(), GetInsID());
            tOspTrans.dwTaskID = INVALID_TASKNO;
            //tOspTrans.dwSeqNO = cReq.GetSeqNum();
            //tOspTrans.dwSeqNO = atol(GetXmlVal(strMsgReq, XML_SEQ_NUM_NODE).c_str());

            OspSipPostRsp(KDSIP_EVENT_MESSAGE_RSP, pcOspSipMsg->GetSipTransID(), cRsp, tOspTrans);

            TOspTransID tOspTransId;
            tOspTransId.dwIID = MAKEIID(GetAppID(), GetInsID());
            tOspTransId.dwTaskID = INVALID_TASKNO;
            tOspTransId.dwSeqNO = INVALID_SEQ_NO;
            OspSipSetHBParam(pcOspSipMsg->GetSipFromUri(), tOspTransId);
            OspPrintf(TRUE, FALSE, "对CUI[%s]设置心跳\n", 
                pcOspSipMsg->GetSipFromUri().GetURIString().c_str());
        }
        break;
    case INVITE_REQ:
        {
//             CInviteReq cReq;
//             pcOspSipMsg->GetMsgBody(cReq);
// 
//             CInviteRsp cRsp;
//             cRsp.SetSession(cReq.GetSession());
//             cRsp.SetSeqNum(cReq.GetSeqNum());
//             cRsp.SetDstChn(cReq.GetDstChn());
//             cRsp.SetSrcChn(cReq.GetSrcChn());
// 
//             TOspTransation tOspTrans;
//             tOspTrans.dwIID = MAKEIID(GetAppID(), GetInsID());
//             tOspTrans.dwTaskID = INVALID_TASKNO;
//             //tOspTrans.dwSeqNO = cReq.GetSeqNum();
//             //tOspTrans.dwSeqNO = atol(GetXmlVal(strMsgReq, XML_SEQ_NUM_NODE).c_str());
//             OspPrintf(TRUE, FALSE, "回复业务消息内容[ %s ]\n\n\n", 
//                 cRsp.ToXml().c_str());
//              OspSipPostRsp(KDSIP_EVENT_INVITE_RSP, pcOspSipMsg->GetSipTransID(), cRsp.ToXml(), tOspTrans);
        }
        break;
    case INVITE_ACK:
        {
            OspPrintf(TRUE, FALSE, "收到InviteAck\n");
        }
        break;
    case PU_STATUS_SS_REQ:
        {
            CPuStatusSsReq cReq;
            pcOspSipMsg->GetMsgBody(cReq);

            CPuStatusSsRsp cRsp;
            cRsp.SetSession(cReq.GetSession());
            cRsp.SetSeqNum(cReq.GetSeqNum());

            TOspTransation tOspTrans;
            tOspTrans.dwIID = MAKEIID(GetAppID(), GetInsID());
            tOspTrans.dwTaskID = INVALID_TASKNO;
            //tOspTrans.dwSeqNO = cReq.GetSeqNum();
            //tOspTrans.dwSeqNO = atol(GetXmlVal(strMsgReq, XML_SEQ_NUM_NODE).c_str());
            OspPrintf(TRUE, FALSE, "回复业务消息内容[ %s ]\n\n\n", 
                cRsp.ToXml().c_str());
            OspSipPostRsp(KDSIP_EVENT_SUBSCRIBE_RSP, pcOspSipMsg->GetSipTransID(), cRsp.ToXml(), tOspTrans);
        }
        break;
    case PU_STATUS_REFRESH_SS_REQ:
        {
            CPuStatusRefreshSsReq cReq;
            pcOspSipMsg->GetMsgBody(cReq);

            CPuStatusRefreshSsRsp cRsp;
            cRsp.SetSession(cReq.GetSession());
            cRsp.SetSeqNum(cReq.GetSeqNum());

            TOspTransation tOspTrans;
            tOspTrans.dwIID = MAKEIID(GetAppID(), GetInsID());
            tOspTrans.dwTaskID = INVALID_TASKNO;
            //tOspTrans.dwSeqNO = cReq.GetSeqNum();
            //tOspTrans.dwSeqNO = atol(GetXmlVal(strMsgReq, XML_SEQ_NUM_NODE).c_str());
            OspPrintf(TRUE, FALSE, "回复业务消息内容[ %s ]\n\n\n", 
                cRsp.ToXml().c_str());
            OspSipPostRsp(KDSIP_EVENT_SUBSCRIBE_RSP, pcOspSipMsg->GetSipTransID(), cRsp.ToXml(), tOspTrans);
        }
        break;
    case PU_STATUS_UNSS_REQ:
        {
            CPuStatusUnSsReq cReq;
            pcOspSipMsg->GetMsgBody(cReq);

            CPuStatusUnSsRsp cRsp;
            cRsp.SetSession(cReq.GetSession());
            cRsp.SetSeqNum(cReq.GetSeqNum());

            TOspTransation tOspTrans;
            tOspTrans.dwIID = MAKEIID(GetAppID(), GetInsID());
            tOspTrans.dwTaskID = INVALID_TASKNO;
            //tOspTrans.dwSeqNO = cReq.GetSeqNum();
            //tOspTrans.dwSeqNO = atol(GetXmlVal(strMsgReq, XML_SEQ_NUM_NODE).c_str());
            OspPrintf(TRUE, FALSE, "回复业务消息内容[ %s ]\n\n\n", 
                cRsp.ToXml().c_str());
            OspSipPostRsp(KDSIP_EVENT_UNSUBSCRIBE_RSP, pcOspSipMsg->GetSipTransID(), cRsp.ToXml(), tOspTrans);
        }
        break;
    default:
        {
            if ( GetSipXmlMsgType(pcOspSipMsg->GetMsgBody()).find("QRY_NTF_RSP") != string::npos )
            {
                //NtfRsp不需要回复
                return;
            }

            int nRspEventId = pcOspSipMsg->GetOspEventID() + 1;
            string strRspXml;
            string strSess;
            if ( ! g_cEventToMsgMap.Find(nRspEventId, strRspXml) )
            {
                OspPrintf(TRUE, FALSE, "找不到消息[%s][%d]的映射\n", 
                    OspExtEventDesc(nRspEventId), nRspEventId);
                return;
            }
            else
            {
                string strSeqNo = GetSipSeqNO(pcOspSipMsg->GetMsgBody());
                u32 dwSeqNo = (u32)atol(strSeqNo.c_str());
                strSess = GetSipSessionID(pcOspSipMsg->GetMsgBody());
                string strNewRspXml = ReplaceSessid(strRspXml.c_str(), strSess);
                strNewRspXml = ReplaceSeqno(strNewRspXml.c_str(), dwSeqNo);

                PostRsp(pcOspSipMsg->GetSipTransID(), strNewRspXml);
            }

            switch ( pcMsg->event )
            {
            case TAS_DEVICE_QRY_REQ:
            case TAS_DEVICE_MODEL_QRY_REQ:
            case TAS_DEVICE_MANUFACTURER_QRY_REQ:
            case TAS_CUSER_QRY_REQ:
            case TAS_CUSER_DEVICE_QRY_REQ:
            case TAS_DEVICE_GROUP_QRY_REQ:
            case TAS_DEVICE_GROUP_DEVICE_QRY_REQ:
            case TAS_CUSER_GROUP_QRY_REQ:
            case TAS_CUSER_GROUP_DEVICE_QRY_REQ:
            case TAS_DOMAIN_RELATION_QRY_REQ:
            case TAS_TEMP_AUTHORIZATION_QRY_REQ:
            case TAS_CUSER_DEVICEFULLINFO_AND_PRIVILEGE_QRY_REQ:
                {
                    nRspEventId = pcMsg->event + 2;
                    if ( ! g_cEventToMsgMap.Find(nRspEventId, strRspXml) )
                    {
                        OspPrintf(TRUE, FALSE, "找不到消息[%s][%d]的映射\n", 
                            OspExtEventDesc(nRspEventId), nRspEventId);
                        return;
                    }
                    else
                    {
                        u32 dwSeqNo = (u32)GenSequenceNum();
                        string strNewRspXml = ReplaceSessid(strRspXml.c_str(), strSess);
                        strNewRspXml = ReplaceSeqno(strNewRspXml.c_str(), dwSeqNo);

                        TOspTransation tOspTrans;
                        tOspTrans.dwIID = MAKEIID(GetAppID(), GetInsID());
                        tOspTrans.dwTaskID = INVALID_TASKNO;
                        OspPrintf(TRUE, FALSE, "发送查询通知消息内容[ %s ]\n\n", 
                            strNewRspXml.c_str());
                        OspSipPostReq(KDSIP_EVENT_MESSAGE_REQ, strNewRspXml, pcOspSipMsg->GetSipFromUri(), tOspTrans);
                    }
                }
            default:
                break;
            }
        }
        break;
    }
}

void CSrvInst::PostRsp( TKDSIP_TRANS_ID tTransId, const string &strNewRspXml )
{
    TOspTransation tOspTrans;
    tOspTrans.dwIID = MAKEIID(GetAppID(), GetInsID());
    tOspTrans.dwTaskID = INVALID_TASKNO;
    //tOspTrans.dwSeqNO = cReq.GetSeqNum();
    //tOspTrans.dwSeqNO = atol(GetXmlVal(strMsgReq, XML_SEQ_NUM_NODE).c_str());
    OspPrintf(TRUE, FALSE, "回复业务消息内容[ %s ]\n\n\n", 
        strNewRspXml.c_str());
    OspSipPostRsp(KDSIP_EVENT_MESSAGE_RSP, tTransId, strNewRspXml, tOspTrans);
}