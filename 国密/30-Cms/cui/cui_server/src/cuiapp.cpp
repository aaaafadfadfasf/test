/*****************************************************************************
ģ����      : cui_fcgi 
�ļ���      : cuiapp.cpp
����ļ�    : 
�ļ�ʵ�ֹ���: cuiapp.cpp ʵ��CUIAPP����
����        : liangli
�汾        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2011/11/7   1.0         liangli        ����
******************************************************************************/
#include "cuiapp.h"
#include "cuiproto.h"
#include "cms/commonconfig/cmcfg.h"
#include "cms/utility/md5.h"
#include "cms/utility/networkhelper.h"
#include "long_query_task.h"
#include "subscribe_task.h"
#include "culogintask.h"
#include "export_task.h"
#include "import_task.h"
#include "recordontvtask.h"
#include "cuidata.h"
#include "offlinevs_query_task.h"
#include "invitetasknatswitch.h"
#include "peinvitetasknatswitch.h"
#include "cms/camera/newcamera.h"

#include "cms/commonconfig/cmcfg.h"
#include "cms/commonconfig/xmlconfig.h"

#include "sdk/fastdbsdk/fastdb.h"
#include "sdk/fastdbsdk/keydef.h"
#include "getptzlockuserinfotask.h"
#include "dpssearchtask.h"
#include "querydevgrouptask.h"
#include "query_task4fetch.h"
#include "batoptchecktask.h"
#include "redisdb.h"
#include "sdk/redissdk/redissdk.h"
#include "redisssmng.h"
#include "StreamDecentrationAdapter.h"
#include "FastDbServiceExt.h"
#include "alarmtypecommon.h"

CRedisDBInterface g_cRedisDb;
CRedisSdk g_cCuiRedisSdk;

extern TMappingTable g_tMappingTable;
extern CXSemLock g_MainSemLock;
extern CRecvThreadMailBox g_cRecvThreadMailBox;
TCuiStat g_tCuiStat;
// CU�˳�����Ҫ��ʱ���cusession��Ӧ��nat̽���
vector<std::string> g_cuSessionForNatPacketClear;

class CCuiAlarmTypeWriter: public IAlarmDataWriter
{
public:
	CCuiAlarmTypeWriter(vector<TAlarmType> *pcTypeLst = NULL):m_pAlmTypeLst(pcTypeLst)
	{
		
	}
	virtual ~CCuiAlarmTypeWriter(){}
public:
	virtual void Write(const CMS_ALARM_TYPE &cmsType, const TAlarmData &tData)
	{
		if( m_pAlmTypeLst == NULL )
		{
			return;
		}
		
		TAlarmType tAlarmType;

		tAlarmType.SetAlarmType(cmsType);
		tAlarmType.SetAlarmTypeName( GetLanDesc(tData.strDescKey) );
		m_pAlmTypeLst->push_back(tAlarmType);
	}
private:
	vector<TAlarmType> *m_pAlmTypeLst;
};

//////////////////////////////////////////////////////////////////////////
//CuiApp
//vtdulib�յ�NAT̽���֮��Ļص�
void NatDetect_CB(TNatData tNatData)
{
	TNatPacketInfo tNatPacketInfo;

	tNatPacketInfo.m_dwNatIp = tNatData.GetNatIp();
	tNatPacketInfo.m_wNatPort = tNatData.GetNatPort();
	tNatPacketInfo.m_dwSrcIp = tNatData.GetSrcIp();
	tNatPacketInfo.m_wSrcPort = tNatData.GetSrcPort();
	tNatPacketInfo.m_dwPalyID = tNatData.GetSessID();
	strcpy(tNatPacketInfo.m_szCuid,tNatData.GetID().c_str());
	tNatPacketInfo.m_byReserve = tNatData.GetReserve();

	PostMsgToSvrApp(CUI_NATDETECT_CALLBACK,&tNatPacketInfo,sizeof(TNatPacketInfo));
	return;
}

void PostMsgToSvrApp(u16 wEvent, const void* pvContent, u16 wLength)
{
    OspPost(CUIAPP_DAEMON, wEvent, pvContent, wLength);
}

void RedisSubscribeCallback(CFastDBInterface *pInter, const std::string &chnName, const std::string &strMsgData, void *userdata)
{
	if (chnName.empty() || strMsgData.empty())
	{
		OspPrintf(TRUE, FALSE, "RedisSubscribeCallback chnName[%s], strMsgData[%s]\n", chnName.c_str(), strMsgData.c_str());
		return;
	}

	CFastDBInterface *pDB = &g_cRedisDb;
	TRedisNtfMsgInfo *ptMsg = new TRedisNtfMsgInfo;
	if (ptMsg != NULL)
	{
		if (strMsgData.length() < REDIS_NTF_MSG_SIZE)
		{
			ptMsg->m_szUserData = (void*)userdata;
			strncpy(ptMsg->m_szchnName, chnName.c_str(), sizeof(ptMsg->m_szchnName));
			strncpy(ptMsg->m_szMsgData, strMsgData.c_str(), sizeof(ptMsg->m_szMsgData));

			PostMsgToSvrApp(CUI_REDIS_NTF_CALLBACK, (const void*)&ptMsg, (u16)sizeof(ptMsg));
		}
		else
		{
			OspPrintf(TRUE, FALSE, "RedisSubscribeCallback ntfMsg ������̫�󣬴ﵽ10Mb�������!\n");
		}
	}
	else
	{
		OspPrintf(TRUE, FALSE, "RedisSubscribeCallback chnName[%s] new error!!!\n", chnName.c_str());
	}

}


LPCSTR CCuiInst::GetStrState() const
{
    switch (CurState())
    {
    case Idle:
        return "Idle";
        break;
    case Service:
        return "Service";
        break;
    case ReLogin:
        return "ReLogin";
        break;
    default:
        break;
    }
    return "Unknown State";
}

void CCuiInst::DaemonEntry(CMessage* const pcMsg)
{
    if (pcMsg->event != CU_CUI_WEBSERVICE_REQ) // ���������Ϣ����������Ϣ������Ϣ���ﲻ��ӡ��,������ӡ��
    {
        INSTLOG(CUI_APP, EVENT_LEV, "CCuiInst::DaemonEntry�յ���Ϣ[%d-%s][״̬:%s]\n",
            pcMsg->event, OspExtEventDesc(pcMsg->event).c_str(),
            GetStrState());
    }

    switch (CurState())
    {
    case Idle:
        {
            OnIdle(pcMsg);
        }
        break;
    case Service:
        {
            OnService(pcMsg);
        }
        break;
    case ReLogin:
        {
            OnReLogin(pcMsg);
        }
        break;
    default:
        {
            CMS_ASSERT(0&&"CCuiInst::DaemonEntry����δ֪״̬");
            INSTLOG(CUI_APP, ERROR_LEV, "CCuiInst::DaemonEntry����δ֪״̬[%s]\n", 
                GetStrState());
        }
        break;
    }
}

void CCuiInst::OnIdle(CMessage* const pcMsg)
{
    switch (pcMsg->event)
    {
    case OSP_PROXY_CONNECT:
        {
            KillTimer(CUI_REG_CMU_TIMER);
            g_cCuiDataMgr.m_cCuiState.m_bConnectProxy = true;
            g_cCuiDataMgr.SetCuiStateHappenTime();
            INSTLOG(CUI_APP, EVENT_LEV, "CUIע��Proxy�ɹ�, ��ʼ��CMUע��\n");
			
			//CUIע��proxy�ɹ���updateCuiInfo���ƶ��ͻ�������д��redis
			TRedisModuleCui tRedisModuleCui;
			tRedisModuleCui.dwMaxMobileNum = g_cCuiConfig.GetMobileClientLimit();
			ECMS_ERRORCODE erCode = g_cCuiRedisSdk.UpdateCuiInfo(g_cCuiConfig.GetLocalURI().GetUser(), tRedisModuleCui);
			if (erCode != CMS_SUCCESS)
			{
				INSTLOG(CUI_APP, ERROR_LEV, "UpdateCuiInfo���ƶ��ͻ�������д��redisʧ�ܣ�������[%d]\n",erCode);
			}

            SetTimer(CUI_REG_CMU_TIMER, CUI_REG_CMU_INTERVAL);
        }
        break;
    case OSP_PROXY_DISCONNECT:
        {
            INSTLOG(CUI_APP, EVENT_LEV, "CUI��Proxy����\n");
            g_cCuiDataMgr.m_cCuiState.m_bConnectProxy = false;
        }
        break;
    case OSP_SIP_DISCONNECT:
        {
            COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
            INSTLOG(CUI_APP, EVENT_LEV, "CUI�������[%s]����\n",
                pcOspSipMsg->GetMsgBody());
        }
        break;
    case CU_CUI_WEBSERVICE_REQ:
        {
            //�ܾ�������Ϣ
            OnIdleCuRequestMsg(pcMsg);
        }
        break;
    case CUI_REG_CMU_TIMER:
        {
			SetTimer(CUI_WAIT_CMU_REG_RSP_TIMER, CUI_REG_CMU_INTERVAL*5);

			CCuiRegReq cCuiRegReq;
            cCuiRegReq.SetDevUri(g_cCuiConfig.GetLocalURI().GetURIString());

            CIpAddrList cIpAddrList;
            list<string> lstIp;
            if (!GetOnlineIpList(lstIp) || lstIp.empty())
            {
                INSTLOG(ERROR_LEV, "Failed to GetOnlineIpList!\n");
                break;
            }
            vector<CCommonNetNatInfo> vecNat = ::GetCommonNetNatInfo();

            TNetAddr tNetAddr;
            for (list<string>::const_iterator it = lstIp.begin(); it != lstIp.end(); ++it)
            {
                tNetAddr.SetNetIp(*it);
                tNetAddr.SetNetPort(::GetHttpPort());
                cIpAddrList.push_back(tNetAddr);

                for (vector<CCommonNetNatInfo>::const_iterator it2 = vecNat.begin(); it2 != vecNat.end(); ++it2)
                {
                    if (it2->platformIP == tNetAddr.GetNetIp() 
                        && it2->platformPort == tNetAddr.GetNetPort())
                    {
                        tNetAddr.SetNetIp(it2->platformNatIP);
                        tNetAddr.SetNetPort(it2->platformNatPort);
                        cIpAddrList.push_back(tNetAddr);
                        break;
                    }
                }
            }
			m_tCuiNetSegInfo = GetNetSegmentInfo();
			cCuiRegReq.SetNetSegmentInfo(m_tCuiNetSegInfo);
            cCuiRegReq.SetIpAddrList(cIpAddrList);
            cCuiRegReq.SetIsSupportNA(true);

            TOspTransation tOspTrans;
            tOspTrans.dwIID = MAKEIID(GetAppID(), GetInsID());
            tOspTrans.dwTaskID = INVALID_TASKNO;
            tOspTrans.dwSeqNO = (u32)cCuiRegReq.GetSeqNum();
            OspSipPostReq(KDSIP_EVENT_MESSAGE_REQ, cCuiRegReq, g_cCuiConfig.GetCmuURI(), tOspTrans);
        }
        break;
    case CUI_WAIT_CMU_REG_RSP_TIMER:
        {
            INSTLOG(CUI_APP, ERROR_LEV, "CUIע��CMU��ʱ, %d�������\n",
                CUI_REG_CMU_INTERVAL/1000);
            SetTimer(CUI_REG_CMU_TIMER, CUI_REG_CMU_INTERVAL);
        }
        break;
    case OSP_SIP_MSG_PROC_FAIL:
        {
            //�Զ˲����߻��ߴ���SIP��Ϣʧ�ܣ��ظ�����Ϣ
            //Idle״̬�²�����
        }
        break;
    case CUI_REG_RSP:
        {
            COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
            CCuiRegRsp cCuiRegRsp;
            pcOspSipMsg->GetMsgBody(cCuiRegRsp);

            if ( cCuiRegRsp.GetErrorCode() != CMS_SUCCESS )
            {
                INSTLOG(CUI_APP, ERROR_LEV, "CUIע��CMUʧ��, ������ %d\n", 
                    cCuiRegRsp.GetErrorCode());
                SetTimer(CUI_REG_CMU_TIMER, CUI_REG_CMU_INTERVAL);
                return;
            }

            KillTimer(CUI_REG_CMU_TIMER);
            KillTimer(CUI_WAIT_CMU_REG_RSP_TIMER);
            INSTLOG(CUI_APP, EVENT_LEV, "CUIע��CMU�ɹ�, CUI�������̬\n");

             //ͨ��redissdk����ģ��״̬
            SubscribeModuleStatus();

            //ע��ɹ�
            TOspTransID tOspTransId;
            tOspTransId.dwIID = MAKEIID(GetAppID(), GetInsID());
            tOspTransId.dwTaskID = INVALID_TASKNO;
            tOspTransId.dwSeqNO = INVALID_SEQ_NO;
            OspSipSetHBParam(g_cCuiConfig.GetCmuURI(), tOspTransId);
			
            CreateCmuListSubscribeTask(cCuiRegRsp.GetSession());

			UpdateRegNetSeg();
            NextState(Service);
            g_cCuiDataMgr.SetCuiStateHappenTime();
            g_cCuiDataMgr.m_cCuiState.m_strSession = cCuiRegRsp.GetSession();

            //����tas
            RegAllLocalServer();
        }
		break;
    case TAS_LOGOUT_RSP:
    case CU_LOGOUT_RSP:
    case TAS_CU_OFFLINE_RSP:
    case CU_DISCONNECT_NTF_RSP:
        {
            //���ô���
            //CMS_ASSERT(0 && "[CCuiInst::OnIdle]CUI�ڿ���̬�յ�δ֪����");

            //�ܾ�������Ϣ



            //�Զ˲����߻��ߴ���SIP��Ϣʧ�ܣ��ظ�����Ϣ
            //Idle״̬�²�����
            //���ô���



            //ע��ɹ�

        }
        break;
    default:
        {
            INSTLOG(CUI_APP, WARNING_LEV, "CUI��δ����Proxy�ɹ�, �޷��ṩ����, �յ�δ֪��Ϣ[%s][%hu]\n", 
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            //CMS_ASSERT(0 && "[CCuiInst::OnIdle]CUI�ڿ���̬�յ�δ֪����");
        }
        break;
    }
}

//�ڴ˺�����һ�ɾܾ�����CU����, �����붼��"ƽ̨δ�������̬ ERR_CUI_NOT_IN_SERVICE"
void CCuiInst::OnIdleCuRequestMsg(CMessage* const pcMsg)
{
    PTRecvNode ptRecvNode = *(PTRecvNode*)(pcMsg->content);
    ptRecvNode->GetCurTime();

    CMsgHead cHead;
    s32 nRet = cHead.FromBuffer(ptRecvNode->szRecvBuf, ptRecvNode->nMsgTotalLen);
    if ( ! (nRet > 0) )
    {
        INSTLOG(CUI_APP, ERROR_LEV, "�յ�CU ������Ϣͷ����ʧ��\n" );
        CMS_ASSERT(nRet > 0);
        //������Ҫֱ��ɾ������Ϊ�����͸�������
        delete ptRecvNode;
        ptRecvNode = NULL;
        return;
    }
    char *pXmlMsgContent = ptRecvNode->szRecvBuf + nRet;

    string strEvent = GetSipXmlMsgType(pXmlMsgContent);
    if ( strEvent.empty() )
    {
        INSTLOG(CUI_APP, ERROR_LEV, "�յ�CU����[%s]����Ϣ�Ž���ʧ��\n",
            pXmlMsgContent);
        delete ptRecvNode;
        ptRecvNode = NULL;
        CMS_ASSERT( ! strEvent.empty());
        return;
    }

    u16 wEvent = OspExtEventID( strEvent.c_str() );
    if ( INVALID_OSP_EVENT == wEvent )
    {
        INSTLOG(CUI_APP, ERROR_LEV, "�յ�CU������Ϣ��[%s]����Ϣ�ŷǷ�\n",
            strEvent.c_str());
        printf( "�յ�CU������Ϣ��[%s]����Ϣ�ŷǷ�\n",
            strEvent.c_str());
        delete ptRecvNode;
        ptRecvNode = NULL;
        CMS_ASSERT(INVALID_OSP_EVENT != wEvent);
        return;
    }

    INSTLOG(CUI_APP, EVENT_LEV, "�յ�CU��������Ϣ[%s][%hu]������CUIδ�������̬��ֱ�Ӿܾ�CU����\n",
        strEvent.c_str(), wEvent);

    CEventRsp cRsp;
    cRsp.SetEvent(wEvent+1);
    string strSeqNO = GetSipSeqNO(pXmlMsgContent);
    cRsp.SetSeqNum((const int)atol(strSeqNO.c_str()));
    cRsp.SetSession(GetSipSessionID(pXmlMsgContent));
    cRsp.SetErrorCode(ERR_CUI_NOT_IN_SERVICE);
    string strRspXml = cRsp.ToXml();
    PostMsgToRecvThread(ptRecvNode, CUI_CU_WEBSERVICE_RSP, strRspXml.c_str(), strRspXml.length());
    //ֵ���ݱ�Ϊָ�봫�ݺ����ﲻdelete���ȵ������յ�����ɾ��
    //     delete ptRecvNode;
    //     ptRecvNode = NULL;
}

u32 CCuiInst::OnServiceCuRequestMsg(CMessage* const pcMsg)
{
    PTRecvNode ptRecvNode = *(PTRecvNode*)(pcMsg->content);
    ptRecvNode->GetCurTime();
    u32 dwRet = PROCMSG_OK;

    CMsgHead cHead;
    s32 nRet = cHead.FromBuffer(ptRecvNode->szRecvBuf, ptRecvNode->nMsgTotalLen);
    if ( ! (nRet > 0) )
    {
        INSTLOG(CUI_APP, ERROR_LEV, "�յ�CU ������Ϣͷ����ʧ��\n" );
        CMS_ASSERT(nRet > 0);
        delete ptRecvNode;
        ptRecvNode = NULL;
        return dwRet;
    }
    char *pXmlMsgContent = ptRecvNode->szRecvBuf + nRet;

    string strEvent = GetSipXmlMsgType(pXmlMsgContent);
    if ( strEvent.empty() )
    {
        INSTLOG(CUI_APP, ERROR_LEV, "�յ�CU����[%s]����Ϣ�Ž���ʧ��\n",
            pXmlMsgContent);
        delete ptRecvNode;
        ptRecvNode = NULL;
        CMS_ASSERT(strEvent.length() > 0);
        return dwRet;
    }

    u16 wEvent = OspExtEventID( strEvent.c_str() );
    if ( CU_CUI_GET_NOTIFY_REQ != wEvent )
    {
        INSTLOG(CUI_APP, EVENT_LEV, "�յ�CU������Ϣ[%s--%hu], msgContent:%s\n",
            strEvent.c_str(), wEvent, pXmlMsgContent);
    }
    else
    {
        INSTLOG(CUI_APP, TIMER_LEV, "�յ�CU����������Ϣ[%s--%hu]\n",
            strEvent.c_str(), wEvent);
    }

    if ( INVALID_OSP_EVENT == wEvent )
    {
        INSTLOG(CUI_APP, ERROR_LEV, "�յ�CU������Ϣ��[%s]����Ϣ�ŷǷ�\n",
            strEvent.c_str());
        delete ptRecvNode;
        ptRecvNode = NULL;
        CMS_ASSERT(INVALID_OSP_EVENT != wEvent);
        return dwRet;
    }

    if ( CU_LOGIN_REQ == wEvent )
    {
        CCuiCuLoginReq cReq;
        cReq.FromXml(pXmlMsgContent);

		if (! CheckCuVersionSupportive(cReq.GetVersion()))
		{
			INSTLOG(CUI_APP, ERROR_LEV, "Cu�û��汾[%s]����֧��\n",
				cReq.GetVersion().c_str());
			CCuiCuLoginRsp cRsp;
			cRsp.SetErrorCode(ERR_CUI_CU_VERSION_UNSUPPORTIVE);
			cRsp.SetSeqNum(cReq.GetSeqNum());
			SendRspToCu<CCuiCuLoginRsp>(cRsp, ptRecvNode);
			WriteLogWithoutLoginSession(cRsp.GetErrorCode(), cReq.GetUserName(), cReq.GetClientIp());
			return dwRet;
		}

        vector<CTask*> vctCuAgents = GetTasksArray("CCuAgent");
        if ( vctCuAgents.size() >= g_cCuiConfig.GetCuMaxNum())
        {
            INSTLOG(CUI_APP, ERROR_LEV, "Cu�û��Ѵﵽ���ޣ�ֱ�Ӿܾ��û�[%s]��¼\n",
                cReq.GetUserName().c_str());
            CCuiCuLoginRsp cRsp;
            cRsp.SetErrorCode(ERR_CUI_EXCEED_CU_MAX_NUM);
            cRsp.SetSeqNum(cReq.GetSeqNum());
            SendRspToCu<CCuiCuLoginRsp>(cRsp, ptRecvNode);
			WriteLogWithoutLoginSession(cRsp.GetErrorCode(), cReq.GetUserName(), cReq.GetClientIp());
            return dwRet;
        }

        //��ʱ���ο����¼
        if(GetDomainNameWithoutPreNameFromURI(cReq.GetUserName()) != GetDomainName())
        {
            INSTLOG(CUI_APP, ERROR_LEV, "Cu�û�[%s]�����뱾��������һ��[%s]\n",
                cReq.GetUserName().c_str(), GetDomainName().c_str());
            CCuiCuLoginRsp cRsp;
            cRsp.SetErrorCode(ERR_CUI_CROSS_DOMAIN_LOGIN);
            cRsp.SetSeqNum(cReq.GetSeqNum());
            SendRspToCu<CCuiCuLoginRsp>(cRsp, ptRecvNode);
			WriteLogWithoutLoginSession(cRsp.GetErrorCode(), cReq.GetUserName(), cReq.GetClientIp());
            return dwRet;
        }
        if (IsMobileClientType(cReq.GetClientType()) && GetMobileClientNumber()+1 > g_cCuiConfig.GetMobileClientLimit())
        {
            INSTLOG(CUI_APP, CRITICAL_LEV, "�ƶ��ͻ��˵�¼�����࣬��ǰ��¼��Ϊ%d, �Ѿ��ﵽ����\n",
                GetMobileClientNumber());
            CCuiCuLoginRsp cRsp;
            cRsp.SetErrorCode(ERR_CUI_EXCEED_MOBILE_CU_MAX_NUM);
            cRsp.SetSeqNum(cReq.GetSeqNum());
            SendRspToCu<CCuiCuLoginRsp>(cRsp, ptRecvNode);
			WriteLogWithoutLoginSession(cRsp.GetErrorCode(), cReq.GetUserName(), cReq.GetClientIp());
            return dwRet;
        }

        CCuAgent *pcTask = new CCuAgent(this, cReq.GetClientType());
        pcTask->InitStateMachine();
        pcTask->m_ptCuLoginNode = ptRecvNode;
        pcTask->m_strClientIp = cReq.GetClientIp();
		pcTask->m_strWebCuClientIp = cReq.GetWebCuIp();
		pcTask->m_strHostIp = pcTask->GetHostIp(cReq.GetHostIp());
		pcTask->m_bIsSupportNA = cReq.GetIsSupportNA();
		pcTask->m_strClientType = cReq.GetClientType();
		pcTask->m_tNatInfo.SetIpAddr(cReq.GetServiceIp());
		pcTask->m_tNatInfo.SetEthNo(pcTask->CalcEthNo(cReq.GetServiceIp()));
		pcTask->m_tNatInfo.SetIsHasNat(pcTask->IsHasNat(cReq.GetServiceIp(), pcTask->m_strHostIp));
		pcTask->SetCMUURI(g_cCuiConfig.GetCmuURI());
		pcTask->m_strClientType = cReq.GetClientType();
		INSTLOG(CUI_APP, ERROR_LEV, "�յ��û�[%s] HostIp is [%s], ServiceIp is [%s] IsHasNat is[%d] ethNo is [%d]\n",
			cReq.GetUserName().c_str(),cReq.GetHostIp().c_str(), cReq.GetServiceIp().c_str(), pcTask->m_tNatInfo.GetIsHasNat(),pcTask->m_tNatInfo.GetEthNo());

        if (string::npos == cReq.GetUserName().find('@'))
        {
            INSTLOG(CUI_APP, ERROR_LEV, "�յ��û�[%s]��¼�����û����Ƿ���ֱ�Ӿܾ���¼\n",
                cReq.GetUserName().c_str());
            CCuiCuLoginRsp cRsp;
            cRsp.SetErrorCode(ERR_CUI_INVALID_PARAM);
            cRsp.SetSeqNum(cReq.GetSeqNum());
            SendRspToCu<CCuiCuLoginRsp>(cRsp, ptRecvNode);
			WriteLogWithoutLoginSession(cRsp.GetErrorCode(), cReq.GetUserName(), cReq.GetClientIp());
            return dwRet;
        }
        pcTask->StartLogin(cReq.GetUserName(), cReq.GetPassword(), cReq.GetVersion());
        return dwRet;
    }
    else if ( CU_LOGOUT_REQ == wEvent )
    {
        CCuLogoutReq cReq;
        cReq.FromXml(pXmlMsgContent);
        CCuAgent *pcCuAgent = (CCuAgent *)g_cCuiDataMgr.GetTaskPtrBySession(cReq.GetSession(), this);
        if (NULL == pcCuAgent)
        {
            INSTLOG(CUI_APP, ERROR_LEV, "�û�[Session: %s]����logout��û�ж�Ӧ��CuAgent. reply ERR_CUI_CU_NOT_ONLINE\n", 
                cReq.GetSession().c_str());
            CEventRsp cRsp;
            cRsp.SetSession(cReq.GetSession());
            cRsp.SetErrorCode(ERR_CUI_CU_NOT_ONLINE);
            cRsp.SetEvent(wEvent+1);
            cRsp.SetSeqNum(cReq.GetSeqNum());
            SendRspToCu<CEventRsp>(cRsp, ptRecvNode);
			//�����������cuagent��û�У��û���Ҳû�У��û�IPҲû�У�ֻ��һ��Req���session����session�϶��Ѿ�ʧЧ���߷Ƿ�
			//���Բ�֪����μ�¼��־�ˡ�2918/8/14 zsy			
            return dwRet;
        }
        CMS_ASSERT( 0 == strcmp(pcCuAgent->GetObjName(), "CCuAgent") );

        INSTLOG(CUI_APP, EVENT_LEV, "CU�û�[%s][Session: %s]���������˳���¼\n", 
            pcCuAgent->m_strUserName.c_str(), cReq.GetSession().c_str());
        pcCuAgent->m_ptCuLogoutNode = ptRecvNode;
        pcCuAgent->WriteLog(CMS_SUCCESS, false);
        delete pcCuAgent;
        return dwRet;
    }
    else if (CU_CUI_GET_NOTIFY_REQ == wEvent)
    {
        CGetNotifyReq cReq;
        cReq.FromXml(pXmlMsgContent);
        CCuAgent *pcCuAgent = (CCuAgent *)g_cCuiDataMgr.GetTaskPtrBySession(cReq.GetSession(), this);
        if (NULL == pcCuAgent)
        {
            INSTLOG(CUI_APP, ERROR_LEV, "�û�[Session: %s]��ȡ֪ͨ��û�ж�Ӧ��CuAgent. reply ERR_CUI_CU_NOT_ONLINE\n", 
                cReq.GetSession().c_str());
            CEventRsp cRsp;
            cRsp.SetSession(cReq.GetSession());
            cRsp.SetErrorCode(ERR_CUI_CU_NOT_ONLINE);
            cRsp.SetEvent(wEvent+1);
            cRsp.SetSeqNum(cReq.GetSeqNum());
            SendRspToCu<CEventRsp>(cRsp, ptRecvNode);
            return dwRet;
        }
        CMS_ASSERT( 0 == strcmp(pcCuAgent->GetObjName(), "CCuAgent") );

        pcCuAgent->CuHBAndGetNotify(ptRecvNode, cReq.GetTimeoutSeconds());
        return dwRet;
    }

    //���������ȼ��CU����״̬
    string strSession = GetSipSessionID(pXmlMsgContent);
    if (!m_strSession4PrintCuReq.empty() && strSession == m_strSession4PrintCuReq)
    {
        OspPrintf(TRUE, FALSE, "CuReq event:%s, content:%s\n", strEvent.c_str(), pXmlMsgContent);
    }

    CCuAgent *pcCuAgent = (CCuAgent *)g_cCuiDataMgr.GetTaskPtrBySession(strSession, this);

    if ( NULL == pcCuAgent || pcCuAgent->GetState() != CCuAgent::ONLINE_STATE )
    {
        CEventRsp cRsp;
        cRsp.SetEvent(wEvent+1);
        string strSeqNO = GetSipSeqNO(pXmlMsgContent);
        cRsp.SetSeqNum((const int)atol(strSeqNO.c_str()));
        cRsp.SetSession(strSession);
        cRsp.SetErrorCode(ERR_CUI_CU_NOT_ONLINE);

        INSTLOG(CUI_APP, ERROR_LEV, "�û�[Session: %s]��������[%s][%hu]��û�ж�Ӧ��CuAgent����CuAgentδ�������̬��reply ERR_CUI_CU_NOT_ONLINE\n", 
            strSession.c_str(), strEvent.c_str(), wEvent);

        SendRspToCu<CEventRsp>(cRsp, ptRecvNode);
        return dwRet;
    }

    CMS_ASSERT(NULL != pcCuAgent->m_pcUserAccount);
    CMS_ASSERT( 0 == strcmp(pcCuAgent->GetObjName(), "CCuAgent") );
    INSTLOG(CUI_APP, EVENT_LEV, "�û�[%s Session: %s]��������[%s][%hu]����Ӧ��CuAgent��TaskNOΪ[%u]\n", 
        pcCuAgent->m_strUserName.c_str(), strSession.c_str(), strEvent.c_str(), wEvent, pcCuAgent->GetTaskNO());

    //ֻҪCU��������������Ϊ������
    pcCuAgent->ClearCuHBCount();

    switch (wEvent)
    {
   case CU_CUI_CU_PROPERTY_REQ:
	   {
			// ʹ���Ƿ��յ�������������ͬ�汾�Ŀͻ��˵�½ [1/22/2015 pangubing]
		   CCuUpdateCUPropertyReq cReq;
		   cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);

		   pcCuAgent->m_ptCuLoginNode = ptRecvNode;
		   pcCuAgent->m_strClientLocalIp = cReq.GetClientLocalIp();
		   pcCuAgent->m_ePlatNat = (ENATMode)cReq.GetPlatNatMode();
		   pcCuAgent->m_bClientSupCuiTransPlatRec = cReq.GetClientSupCuiTransPlatRec();
		   pcCuAgent->OnUpdateCuProperty();
	   }
		break;
    case CU_CUI_PU_REC_QRY_REQ:
        {
            CCuPuRecQryReq cReq;
            cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);

            if (cReq.GetQueryHandle().empty())
            {
                CLongQueryTask *pcCuQryReqTask = new CLongQueryTask(this, pcCuAgent, ptRecvNode);
                pcCuQryReqTask->InitStateMachine();
                SetCuReqMsg(cReq, pcCuQryReqTask);
            }
            else
            {
                digital_string strTaskNo(cReq.GetQueryHandle());
                CLongQueryTask *pcTask = dynamic_cast<CLongQueryTask*>(FindTask(strTaskNo.GetU32()));
                if (NULL != pcTask)
                {
                    pcTask->Fetch(ptRecvNode);
                }
                else
                {
                    CEventRsp cRsp;
                    cRsp.SetEvent(wEvent+1);
                    string strSeqNO = GetSipSeqNO(pXmlMsgContent);
                    cRsp.SetSeqNum((const int)atol(strSeqNO.c_str()));
                    cRsp.SetSession(strSession);
                    cRsp.SetErrorCode(ERR_CUI_TASK_DEL);

                    INSTLOG(CUI_APP, ERROR_LEV, "�û�[Session: %s]��������[%s][%hu]��û�ж�Ӧ��LongQueryTask��queryHandle=%s\n", 
                        strSession.c_str(), strEvent.c_str(), wEvent, cReq.GetQueryHandle().c_str());

                    SendRspToCu<CEventRsp>(cRsp, ptRecvNode);
                }
            }
        }
        break;
	case CU_CUI_OFFLINEDEV_QRY_REQ:  
		{
			CCuOffLineDevQryReq cReq;
			cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);

			string cuSession = cReq.GetSession();
			string domainName = cReq.GetDomanName();

			string newCuQueryTag = cReq.GetCuQueryTag();
			string oldCuQueryTag = g_mapDevVideoSrcStatList[cuSession][domainName].GetCuQueryTag();

			if ( newCuQueryTag.compare(oldCuQueryTag) != 0 //һ���µĲ�ѯ����Ĳ�ѯ��ʼλ�ò���Ϊ0 
				&& cReq.GetStartEntryIndex() != 0 )
			{
				CEventRsp cRsp;
				cRsp.SetEvent(wEvent+1);
				string strSeqNO = GetSipSeqNO(pXmlMsgContent);
				cRsp.SetSeqNum((const int)atol(strSeqNO.c_str()));
				cRsp.SetSession(strSession);
				cRsp.SetErrorCode(ERR_CUI_PARAMS_INVALID);

				INSTLOG(CUI_APP, ERROR_LEV, "�û�[Session: %s]��������[%s][%hu]��һ���µĲ�ѯ����Ĳ�ѯ��ʼλ�ò���Ϊ0 CuQueryTag=%s\n", 
					strSession.c_str(), strEvent.c_str(), wEvent, cReq.GetCuQueryTag().c_str());

				SendRspToCu<CEventRsp>(cRsp, ptRecvNode);
				break;
			}

			string taskQueryTag = g_mapDevVideoSrcStatList[cuSession][domainName].GetTaskQueryTag();
			string taskNo = g_mapDevVideoSrcStatList[cuSession][domainName].GetTaskNo();

			time_t curTime;
			time(&curTime);
			time_t cuUsingLastTime = g_mapDevVideoSrcStatList[cuSession][domainName].GetUsingLastTime();

			if ( oldCuQueryTag.empty() // oldCuQueryTag ��Ч
				|| taskQueryTag.empty() // taskQueryTag��Ч
				|| taskNo.empty() // taskNo��Ч
				|| (newCuQueryTag.compare(oldCuQueryTag) !=0) //����ͬһ�β�ѯ����
				)
			{
				// ɾ��֮ǰ�Ĳ�ѯ
				if ( !taskNo.empty() )
				{
					digital_string strTaskNo(taskNo);
					COfflineVsQueryTask *pcTask = dynamic_cast<COfflineVsQueryTask*>(FindTask(strTaskNo.GetU32()));
					if (NULL != pcTask)
					{
						INSTLOG(CUI_APP, EVENT_LEV, "�û�[Session: %s]��������[%s][%hu],�򲿷ֲ���ʧЧ��ɾ��ǰһ�β�ѯ taskNo=%s\n", 
							strSession.c_str(), strEvent.c_str(), wEvent, taskNo.c_str());
						delete pcTask;
						pcTask = NULL;
					}
					else
					{
						INSTLOG(CUI_APP, EVENT_LEV, "�û�[Session: %s]��������[%s][%hu],��û���ҵ���Task[%s]�޷�ɾ��\n", 
							strSession.c_str(), strEvent.c_str(), wEvent, taskNo.c_str());
					}
				}
			}
			else
			{
				if ( !taskNo.empty() )
				{
					digital_string strTaskNo(taskNo);
					COfflineVsQueryTask *pcTask = dynamic_cast<COfflineVsQueryTask*>(FindTask(strTaskNo.GetU32()));
					if (NULL != pcTask)
					{
						INSTLOG(CUI_APP, EVENT_LEV, "�û�[Session: %s]��������[%s][%hu],�ҵ�����Task[%s],ֱ����������\n", 
							strSession.c_str(), strEvent.c_str(), wEvent, taskNo.c_str());

						pcTask->m_ptRecvNode = ptRecvNode;
						SetCuReqMsg(cReq, pcTask);
						break;
					}
					else
					{
						INSTLOG(CUI_APP, EVENT_LEV, "�û�[Session: %s]��������[%s][%hu],δ�ҵ�����Task[%s],������������\n", 
							strSession.c_str(), strEvent.c_str(), wEvent, taskNo.c_str());
					}
				}
			}

			// �����������
			g_mapDevVideoSrcStatList[cuSession][cReq.GetDomanName()].Clear();

			// �½�һ�β�ѯ����
			COfflineVsQueryTask *pcCuQryReqTask = new COfflineVsQueryTask(this, pcCuAgent, ptRecvNode);

			g_mapDevVideoSrcStatList[cuSession][domainName].SetCuQueryTag(cReq.GetCuQueryTag());
			g_mapDevVideoSrcStatList[cuSession][domainName].SetTaskQueryTag(cReq.GetQueryTag());

			pcCuQryReqTask->InitStateMachine();
			SetCuReqMsg(cReq, pcCuQryReqTask);
		}
		break;
	case PU_LONG_OSD_GET_REQ:
		{
			CCuPuLongOsdGetReq cReq;
			cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);

			CSingleInterAction *pcReqTask = new CSingleInterAction(this, pcCuAgent, ptRecvNode);
			pcReqTask->InitStateMachine();

			SetCuReqMsg(cReq, pcReqTask);
		}
		break;
	case PU_LONG_OSD_SET_REQ:
		{
			CCuPuLongOsdSetReq cReq;
			cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);

			CSingleInterAction *pcReqTask = new CSingleInterAction(this, pcCuAgent, ptRecvNode);
			pcReqTask->InitStateMachine();

			SetCuReqMsg(cReq, pcReqTask);
		}
		break;
    case CU_GETCUILIST_REQ:
        {
            CCuGetCuiListReq cReq;
            cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);

            CSingleInterAction *pcReqTask = new CSingleInterAction(this, pcCuAgent, ptRecvNode);
            pcReqTask->InitStateMachine();
            //3���������ⲿ�������󣬽���������
            SetCuReqMsg(cReq, pcReqTask);
        }
        break;
    case PU_PTZ_CTRL_REQ:
        {
            CPuPtzCtrlReq cReq;
            cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);
            string strUserInfo;
            strUserInfo = pcCuAgent->m_strUserName;
            strUserInfo += "(From IP:";
            strUserInfo += pcCuAgent->m_strClientIp;
            strUserInfo += ")";
            cReq.SetUserName(strUserInfo);

            //2��PTZȨ������Լ�У��
            bool bTmpRet = false;
			int dwPtzPriLevel = 0;
            do 
            {
				C3acAgent *pc3acAgent = (C3acAgent *)g_cCuiDataMgr.Get3asAgentTaskPtrBy3asName(pcCuAgent->m_str3asName, this);
				if (NULL == pc3acAgent)
				{
					INSTLOG(CUI_APP, ERROR_LEV, "�յ�����CU����Ϣ[%s--%hu]������û�ж�Ӧ��3acAgent\n",
						OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

					break;
				}
				
				string strDevUUid = GetNameWithoutDomainFromURI(cReq.GetDevChn().GetDevUri());
				TChannel tVsChn(strDevUUid, cReq.GetDevChn().GetChnNO());

				CCuPtzLockInfo ptzLockInfo;
				if ( pc3acAgent->m_cPtzlockInfoList.GetCuPtzLockInfoByVs(cReq.GetDevChn(), ptzLockInfo) )
				{

					// �����Ч��Ҫɾ��
					if ( !ptzLockInfo.IsValid() )
					{
						pc3acAgent->Del3APtzLockInfoByVs(tVsChn);

						INSTLOG(CUI_APP, CRITICAL_LEV, "ɾ������PTZ��\n");
						ptzLockInfo.PrintData();
					}
					else
					{
						INSTLOG(CUI_APP, CRITICAL_LEV, "�û�[%s Session: %s]��ǰ��[%s]ͨ��[%d]PTZ����ʧ�ܣ�PTZ������\n",
							pcCuAgent->m_strUserName.c_str(), strSession.c_str(),
							cReq.GetDevChn().GetDevUri().c_str(), cReq.GetDevChn().GetChnNO());

						CPuPtzCtrlRsp cPuPtzCtrlRsp;
						cPuPtzCtrlRsp.Clear();

						cPuPtzCtrlRsp.SetSession(cReq.GetSession());
						cPuPtzCtrlRsp.SetUserId(ptzLockInfo.GetUserId());
						cPuPtzCtrlRsp.SetUsername(ptzLockInfo.GetUserName());
						cPuPtzCtrlRsp.SetPriPtzLevel(ptzLockInfo.GetUserPtzLevel());
						cPuPtzCtrlRsp.SetBeginTime(ptzLockInfo.GetBeginTime());
						cPuPtzCtrlRsp.SetEndTime(ptzLockInfo.GetEndTime());
						cPuPtzCtrlRsp.SetErrorCode(ERR_CUI_PU_PTZ_LOCKED);

						CGetPuPtzLockUserInfoTask *pcGetLockInfoTask = new CGetPuPtzLockUserInfoTask(this, pcCuAgent, ptRecvNode);
						if ( pcGetLockInfoTask  == NULL || ptzLockInfo.GetUserId().empty())
						{
							SendRspToCu<CPuPtzCtrlRsp>(cPuPtzCtrlRsp, ptRecvNode);
						}
						else
						{
							pcGetLockInfoTask->SetPuPtzCtrlRsp(cPuPtzCtrlRsp);
							pcGetLockInfoTask->InitStateMachine();
							if (pcGetLockInfoTask->GetUserNameInfo(ptzLockInfo.GetUserId(), tVsChn) == false)
							{
								SendRspToCu<CPuPtzCtrlRsp>(cPuPtzCtrlRsp, ptRecvNode);
							}
						}

						return dwRet;
					}
				}

				if (pcCuAgent->m_pcUserAccount->IsSuperAdmin())
				{
					dwPtzPriLevel = CMS_PTZ_MAX_PRI_LEV;
					bTmpRet = true;
					break;
				}

				CVideosourcePrivilege cVideoPri;
				string errMsg;
				EFDB_ErrorCode er = g_cRedisDb.GetUserVideoPriByEncChn(pcCuAgent->m_pcUserAccount->m_tUserID, strDevUUid, cReq.GetDevChn().GetChnNO(), cVideoPri, errMsg);
				if ( !IsFdbResultPositive(er) )
				{
					INSTLOG(CUI_APP, CRITICAL_LEV, "�û�name[%s] id[%s]û����ƵԴ[%s]Ȩ�� errmsg[%s] r[%d]\n",
						pcCuAgent->m_strUserName.c_str(), pcCuAgent->m_pcUserAccount->m_tUserID.c_str(), tVsChn.ToString().c_str(), errMsg.c_str(), er);
					break;
				}

				dwPtzPriLevel = cVideoPri.GetPTZCtrl();
				if ( dwPtzPriLevel <= 0 )
				{
					INSTLOG(CUI_APP, CRITICAL_LEV, "�û�name[%s] id[%s]û����ƵԴ[%s]PTZȨ��\n",
						pcCuAgent->m_strUserName.c_str(), pcCuAgent->m_pcUserAccount->m_tUserID.c_str(), tVsChn.ToString().c_str());
					break;
				}

				// ��ƵԴԤ��λ������ҪУ����Ȩ��
				if ( cReq.GetCamCtrl().GetCommandId() == CAM_COMMAND_PRESETSET 
					&& !cVideoPri.HasPrivilege(CVideosourcePrivilege::en_PrePos_Set) ) 
				{
					INSTLOG(CUI_APP, CRITICAL_LEV, "�û��޷�����Ԥ��λ��û�л���Ȩ��\n");
					break;
				}

                bTmpRet = true;
            } while (0);

            if (!bTmpRet)
            {
                INSTLOG(CUI_APP, EVENT_LEV, "�û�[%s Session: %s]��ǰ��[%s]ͨ��[%d]PTZ����ʧ�ܣ���Ȩ��\n", 
                    pcCuAgent->m_strUserName.c_str(), strSession.c_str(), 
                    cReq.GetDevChn().GetDevUri().c_str(), cReq.GetDevChn().GetChnNO());
                CPuPtzCtrlRsp cRsp;
                    cRsp.SetErrorCode(ERR_CUI_CU_NO_PERMISSION);
                SendRspToCu<CPuPtzCtrlRsp>(cRsp, ptRecvNode);
                return dwRet;
            }

			cReq.GetCamCtrl().SetPriLevel(dwPtzPriLevel);
			cReq.GetCamCtrl().SetHoldTime(CMS_PTZ_DEFAULT_HOLD_TIME);

            //3����������
            CSingleInterAction *pcPtzCtrlTask = new CSingleInterAction(this, pcCuAgent, ptRecvNode);
            pcPtzCtrlTask->InitStateMachine();
            SetCuReqMsg(cReq, pcPtzCtrlTask);
        }
        break;
    case PU_PTZ_LOCK_REQ:
        {
			// ��ǰptz�����ߵ���3A��¼���ݿ⣬����cmu����ʱ����ʹ�ø�����
            CPuPtzLockReq cReq;
            cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);
            cReq.SetLockUserName(pcCuAgent->m_strUserName);

            //2��PTZȨ������Լ�У��
            bool bAuthRet = true; //��Ȩ���
            do 
            {
                string strDomainName = GetDomainNameWithoutPreNameFromURI(cReq.GetPuChn().GetDevUri());
                if ( ! pcCuAgent->IsExistDomainInfo(strDomainName))
                {
                    bAuthRet = false;
                    break;
                }

				if (pcCuAgent->m_pcUserAccount->IsSuperAdmin())
                {
                    bAuthRet = true;
                    break;
                }

                string uuid = GetNameWithoutDomainFromURI(cReq.GetPuChn().GetDevUri());

				CVideosourcePrivilege cPri;
				string errMsg;
				EFDB_ErrorCode er = g_cRedisDb.GetUserVideoPriByEncChn(pcCuAgent->m_pcUserAccount->m_tUserID, uuid, cReq.GetPuChn().GetChnNO(), cPri, errMsg);
				if ( !IsFdbResultPositive(er) )
				{
					INSTLOG(CUI_APP, CRITICAL_LEV, "�û�name[%s] id[%s]û����ƵԴ[%s]Ȩ�� errmsg[%s] er[%d]\n",
						pcCuAgent->m_strUserName.c_str(), pcCuAgent->m_pcUserAccount->m_tUserID.c_str(), cReq.GetPuChn().ToString().c_str(), errMsg.c_str(), er);
					bAuthRet = false;
					break;
				}


            } while (0);

            if (!bAuthRet)
            {
                INSTLOG(CUI_APP, ERROR_LEV, "�û�[%s Session: %s]��ǰ��[%s]ͨ��[%d]PTZ����ʧ�ܣ���Ȩ��\n", 
                    pcCuAgent->m_strUserName.c_str(), strSession.c_str(), 
                    cReq.GetPuChn().GetDevUri().c_str(), cReq.GetPuChn().GetChnNO());
                CPuPtzLockRsp cRsp;
                cRsp.SetErrorCode(ERR_CUI_CU_NO_PERMISSION);
                SendRspToCu<CPuPtzLockRsp>(cRsp, ptRecvNode);
                return dwRet;
            }

            //3����������
            CSingleInterAction *pcPtzLockTask = new CSingleInterAction(this, pcCuAgent, ptRecvNode);
            pcPtzLockTask->InitStateMachine();
            SetCuReqMsg(cReq, pcPtzLockTask);
        }
        break;
    case PU_PTZ_UNLOCK_REQ:
		{
			// ��ǰptz�����ߵ���3A��¼���ݿ⣬����cmu����ʱ����ʹ�ø�����
            CPuPtzUnlockReq cReq;
            cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);
            cReq.SetUnlockUserName(pcCuAgent->m_strUserName);

            //������У��Ȩ��

            CSingleInterAction *pcPtzUnlockTask = new CSingleInterAction(this, pcCuAgent, ptRecvNode);
            pcPtzUnlockTask->InitStateMachine();
            SetCuReqMsg(cReq, pcPtzUnlockTask);
        }
        break;
	case TAS_PTZLOCK_ADD_REQ:
		{
			// redis�汾���ݽ���redis�ж�ȡ��������Ҫ�޸�

			CCuPtzLockAddReq cReq;
			cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);
			cReq.SetSession(pcCuAgent->m_strCuSessionIn3AS);

			CPtzLockInfo ptzLockInfo = cReq.GetCPtzLockInfo();

			cReq.GetCPtzLockInfo().SetUserId(pcCuAgent->m_pcUserAccount->m_tUserID);
			
			//2��PTZȨ��У��
			bool bAuthRet = true; //��Ȩ���
			do
			{
				TChannel vsChn;
				vsChn.SetDevUri(GetNameWithoutDomainFromURI(cReq.GetCPtzLockInfo().GetDeviceId()));
				vsChn.SetChnNO(cReq.GetCPtzLockInfo().GetVideoSrcId());

				C3acAgent *pc3acAgent = (C3acAgent *)g_cCuiDataMgr.Get3asAgentTaskPtrBy3asName(pcCuAgent->m_str3asName, this);
				if (NULL == pc3acAgent)
				{
					INSTLOG(CUI_APP, ERROR_LEV, "�û�[%s Session: %s]��ǰ��[%s]ͨ��[%d]PTZ����ʧ�ܣ���Ȩ��\n",
						pcCuAgent->m_strUserName.c_str(), strSession.c_str(),
						ptzLockInfo.GetDeviceId().c_str(), ptzLockInfo.GetVideoSrcId());
					CCuPtzLockAddRsp cRsp;
					cRsp.SetErrorCode(ERR_CUI_TAS_OFFLINE);
					SendRspToCu<CCuPtzLockAddRsp>(cRsp, ptRecvNode);
					return dwRet;
					break;
				}

				CCuPtzLockInfo oldPtzLockInfo;
				if (pc3acAgent->m_cPtzlockInfoList.GetCuPtzLockInfoByVs(vsChn, oldPtzLockInfo))
				{
					// �����Ч��Ҫɾ��
					if (!oldPtzLockInfo.IsValid())
					{
						pc3acAgent->Del3APtzLockInfoByVs(vsChn);

						INSTLOG(CUI_APP, CRITICAL_LEV, "ɾ������PTZ��\n");
						oldPtzLockInfo.PrintData();
					}
				}

				string strDomainName = GetDomainNameWithoutPreNameFromURI(ptzLockInfo.GetDeviceId());
				if (strDomainName != GetDomainName())
				{
					INSTLOG(CUI_APP, ERROR_LEV, "�����豸��֧��PTZ���� devId[%s]\n", ptzLockInfo.GetDeviceId().c_str());

					CCuPtzLockAddRsp cRsp;
					cRsp.SetErrorCode(ERR_CUI_CU_NO_PERMISSION);
					SendRspToCu<CCuPtzLockAddRsp>(cRsp, ptRecvNode);
					return dwRet;
				}

				if (!pcCuAgent->IsExistDomainInfo(strDomainName))
				{
					bAuthRet = false;
					break;
				}

				if (pcCuAgent->m_pcUserAccount->IsSuperAdmin())
				{
					bAuthRet = true;
					break;
				}

				string uuid = GetNameWithoutDomainFromURI(ptzLockInfo.GetDeviceId());

				CVideosourcePrivilege cPri;
				string errMsg;
				EFDB_ErrorCode er = g_cRedisDb.GetUserVideoPriByEncChn(pcCuAgent->m_pcUserAccount->m_tUserID, uuid, ptzLockInfo.GetVideoSrcId(), cPri, errMsg);
				if ( !IsFdbResultPositive(er) )
				{
					INSTLOG(CUI_APP, CRITICAL_LEV, "�û�name[%s] id[%s]û����ƵԴ[%s]Ȩ�� errmsg[%s] er[%d]\n",
						pcCuAgent->m_strUserName.c_str(), pcCuAgent->m_pcUserAccount->m_tUserID.c_str(), vsChn.ToString().c_str(), errMsg.c_str(), er);
					bAuthRet = false;
					break;
				}

				// �ж��Ƿ���PTZ����Ȩ�ޣ�ͬʱӵ�м���Ȩ��
				bool bHasPtzLockPri = cPri.GetPTZCtrl() != 0 && cPri.HasPrivilege(CVideosourcePrivilege::en_PTZ_Lock);
				if (!bHasPtzLockPri)
				{
					bAuthRet = false;
					break;
				}

			} while (0);

			if (!bAuthRet)
			{
				INSTLOG(CUI_APP, ERROR_LEV, "�û�[%s Session: %s]��ǰ��[%s]ͨ��[%d]PTZ����ʧ�ܣ���Ȩ��\n",
					pcCuAgent->m_strUserName.c_str(), strSession.c_str(),
					ptzLockInfo.GetDeviceId().c_str(), ptzLockInfo.GetVideoSrcId());
				CCuPtzLockAddRsp cRsp;
				cRsp.SetErrorCode(ERR_CUI_CU_NO_PERMISSION);
				SendRspToCu<CCuPtzLockAddRsp>(cRsp, ptRecvNode);
				return dwRet;
			}
		
			//3����������
			CSingleInterAction *pcPtzLockTask = new CSingleInterAction(this, pcCuAgent, ptRecvNode);
			pcPtzLockTask->InitStateMachine();
			SetCuReqMsg(cReq, pcPtzLockTask);
		}
		break;
	case TAS_PTZLOCK_DEL_REQ:
		{
			CCuPtzLockDelReq cReq;
			cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);

			cReq.SetSession(pcCuAgent->m_strCuSessionIn3AS);
			cReq.SetUserId(pcCuAgent->m_pcUserAccount->m_tUserID);

			if (cReq.GetOperateType() != PTZLOCK_UNLOCK
				&& cReq.GetOperateType() != PTZLOCK_UNLOCKALL)
			{
				// δ֪������
				INSTLOG(CUI_APP, ERROR_LEV, "�û�[%s Session: %s]��ǰ��[%s]ͨ��[%d]PTZ����ʧ�ܣ���Ȩ��\n",
					pcCuAgent->m_strUserName.c_str(), strSession.c_str(),
					cReq.GetDeviceId().c_str(), cReq.GetVideoSrcId());
				CCuPtzLockDelRsp cRsp;
				cRsp.SetErrorCode(ERR_CUI_PARAMS_INVALID);
				SendRspToCu<CCuPtzLockDelRsp>(cRsp, ptRecvNode);
				return dwRet;
			}

			//2��PTZȨ��У��
			bool bAuthRet = true; //��Ȩ���
			do
			{
				if ( cReq.GetOperateType() == PTZLOCK_UNLOCKALL )
				{
					if ( pcCuAgent->m_pcUserAccount->IsSuperAdmin() )
					{
						//ȫ����������ֻ�г�������Ա���ܵ���
						bAuthRet = true;
						break;
					}
					else
					{
						bAuthRet = false;
						break;
					}
				}
				else if ( cReq.GetOperateType() == PTZLOCK_UNLOCK )
				{

					C3acAgent *pc3acAgent = (C3acAgent *)g_cCuiDataMgr.Get3asAgentTaskPtrBy3asName(pcCuAgent->m_str3asName, this);
					if (NULL == pc3acAgent)
					{
						// δ֪������
						INSTLOG(CUI_APP, ERROR_LEV, "�û�[%s Session: %s]��ǰ��[%s]ͨ��[%d]PTZ����ʧ�ܣ���Ȩ��\n",
							pcCuAgent->m_strUserName.c_str(), strSession.c_str(),
							cReq.GetDeviceId().c_str(), cReq.GetVideoSrcId());
						CCuPtzLockDelRsp cRsp;
						cRsp.SetErrorCode(ERR_CUI_TAS_OFFLINE);
						SendRspToCu<CCuPtzLockDelRsp>(cRsp, ptRecvNode);
						return dwRet;
						break;
					}
					
					TChannel vsChn;
					vsChn.SetDevUri(GetNameWithoutDomainFromURI(cReq.GetDeviceId()));
					vsChn.SetChnNO(cReq.GetVideoSrcId());

					CCuPtzLockInfo ptzLockInfo;
					if (pc3acAgent->m_cPtzlockInfoList.GetCuPtzLockInfoByVs(vsChn, ptzLockInfo))
					{
						// �����Ч��Ҫɾ��
						if (!ptzLockInfo.IsValid())
						{
							pc3acAgent->Del3APtzLockInfoByVs(vsChn);

							INSTLOG(CUI_APP, CRITICAL_LEV, "ɾ������PTZ��\n");
							ptzLockInfo.PrintData();
						}
					}
				}

				string strDomainName = GetDomainNameWithoutPreNameFromURI(cReq.GetDeviceId());

				if (strDomainName != GetDomainName())
				{
					INSTLOG(CUI_APP, ERROR_LEV, "�����豸��֧��PTZ���� devId[%s]\n", cReq.GetDeviceId().c_str());

					CCuPtzLockDelRsp cRsp;
					cRsp.SetErrorCode(ERR_CUI_CU_NO_PERMISSION);
					SendRspToCu<CCuPtzLockDelRsp>(cRsp, ptRecvNode);
					return dwRet;
				}

				if (!pcCuAgent->IsExistDomainInfo(strDomainName))
				{
					bAuthRet = false;
					break;
				}

				if (pcCuAgent->m_pcUserAccount->IsSuperAdmin())
				{
					bAuthRet = true;
					break;
				}

				string uuid = GetNameWithoutDomainFromURI(cReq.GetDeviceId());
				CVideosourcePrivilege cPri;
				string errMsg;
				EFDB_ErrorCode er = g_cRedisDb.GetUserVideoPriByEncChn(pcCuAgent->m_pcUserAccount->m_tUserID, uuid, cReq.GetVideoSrcId(), cPri, errMsg);
				if ( !IsFdbResultPositive(er) )
				{
					TChannel tChannel(cReq.GetDeviceId(), cReq.GetVideoSrcId());
					INSTLOG(CUI_APP, CRITICAL_LEV, "�û�name[%s] id[%s]û����ƵԴ[%s]Ȩ�� errmsg[%s] er[%d]\n",
						pcCuAgent->m_strUserName.c_str(), pcCuAgent->m_pcUserAccount->m_tUserID.c_str(), tChannel.ToXml().c_str(), errMsg.c_str(), er);
					bAuthRet = false;
					break;
				}

				// �ж��Ƿ���PTZ����Ȩ��
				bool bHasPtzLockPri = cPri.GetPTZCtrl() != 0 && cPri.HasPrivilege(CVideosourcePrivilege::en_PTZ_Lock);
				if (!bHasPtzLockPri)
				{
					bAuthRet = false;
					break;
				}

			} while (0);

			if (!bAuthRet)
			{
				INSTLOG(CUI_APP, ERROR_LEV, "�û�[%s Session: %s]��ǰ��[%s]ͨ��[%d]PTZ����ʧ�ܣ���Ȩ��\n",
					pcCuAgent->m_strUserName.c_str(), strSession.c_str(),
					cReq.GetDeviceId().c_str(), cReq.GetVideoSrcId());
				CCuPtzLockDelRsp cRsp;
				cRsp.SetErrorCode(ERR_CUI_CU_NO_PERMISSION);
				SendRspToCu<CCuPtzLockDelRsp>(cRsp, ptRecvNode);
				return dwRet;
			}
			
			cReq.SetDeviceId(GetNameWithoutDomainFromURI(cReq.GetDeviceId()));

			//3����������
			CSingleInterAction *pcPtzLockTask = new CSingleInterAction(this, pcCuAgent, ptRecvNode);
			pcPtzLockTask->InitStateMachine();
			SetCuReqMsg(cReq, pcPtzLockTask);
		}
		break;
	case PU_RECORD_START_REQ:
		{
			//1 ��Ȩ
			CCuPuRecordStartReq cReq;
			bool bAuthRet = false; //��Ȩ���
			do 
			{                
				cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);

				if (pcCuAgent->m_pcUserAccount->IsSuperAdmin())
				{
					bAuthRet = true;
					break;
				}

				string uuid = GetNameWithoutDomainFromURI(cReq.GetDevUri());

				//�ж��Ƿ��з����Ȩ��
				CEncoderPrivilege cPri;
				EFDB_ErrorCode er = g_cRedisDb.GetUserEncoderPrivilege(pcCuAgent->m_pcUserAccount->m_tUserID, uuid, cPri);
				if ( !IsFdbResultPositive(er) )
				{
					INSTLOG(CUI_APP, CRITICAL_LEV, "�û�name[%s] id[%s]û���豸[%s]Ȩ�� er[%d]\n",
						pcCuAgent->m_strUserName.c_str(), pcCuAgent->m_pcUserAccount->m_tUserID.c_str(), cReq.GetDevUri().c_str(), er);
					break;
				}

				if (cPri.HasPrivilege(CEncoderPrivilege::en_Vod_Rec))
				{
					bAuthRet = true;
					break;
				}

				break;
			}while(0);

			if (!bAuthRet)
			{
				INSTLOG(CUI_APP, ERROR_LEV, "[ǰ��:%s�� Session: %s]��Ȩ�޿���¼������\n", 
					cReq.GetDevUri().c_str(),strSession.c_str());
				CCuPuRecordStartRsp cRsp;
				cRsp.SetErrorCode(ERR_CUI_CU_NO_PERMISSION);
				SendRspToCu<CCuPuRecordStartRsp>(cRsp, ptRecvNode);
				return dwRet;
			}
			cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);

			CSingleInterAction* pcTask = new CSingleInterAction(this, pcCuAgent, ptRecvNode);
			pcTask->InitStateMachine();
			SetCuReqMsg(cReq, pcTask);
		}
		break;
	case PU_RECORD_STOP_REQ:
		{
			CCuPuRecordStopReq cReq;
			cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);

			//1 ��Ȩ
			bool bAuthRet = false; //��Ȩ���
			do
			{
				if (pcCuAgent->m_pcUserAccount->IsSuperAdmin())
				{
					bAuthRet = true;
					break;
				}

				string uuid = GetNameWithoutDomainFromURI(cReq.GetDevUri());

				//�ж��Ƿ��з����Ȩ��
				CEncoderPrivilege cPri;
				EFDB_ErrorCode er = g_cRedisDb.GetUserEncoderPrivilege(pcCuAgent->m_pcUserAccount->m_tUserID, uuid, cPri);
				if ( !IsFdbResultPositive(er) )
				{
					INSTLOG(CUI_APP, CRITICAL_LEV, "�û�name[%s] id[%s]û���豸[%s]Ȩ�� er[%d]\n",
						pcCuAgent->m_strUserName.c_str(), pcCuAgent->m_pcUserAccount->m_tUserID.c_str(), cReq.GetDevUri().c_str(), er);
					break;
				}

				if (cPri.HasPrivilege(CEncoderPrivilege::en_Vod_Rec))
				{
					bAuthRet = true;
					break;
				}

				break;
			} while (0);

			if (!bAuthRet)
			{
				INSTLOG(CUI_APP, ERROR_LEV, "[ǰ��:%s�� Session: %s]��Ȩ��ֹͣ¼������\n",
					cReq.GetDevUri().c_str(), strSession.c_str());
				CCuPuRecordStopRsp cRsp;
				cRsp.SetErrorCode(ERR_CUI_CU_NO_PERMISSION);
				SendRspToCu<CCuPuRecordStopRsp>(cRsp, ptRecvNode);
				return dwRet;
			}

			CSingleInterAction* pcTask = new CSingleInterAction(this, pcCuAgent, ptRecvNode);
			pcTask->InitStateMachine();
			SetCuReqMsg(cReq, pcTask);
		}
		break;
	case PU_SERIALPARAM_CFG_GET_REQ:
		{
			CCuPuSerialParamCfgGetReq cReq;
			cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);
	
			CSingleInterAction* pcTask = new CSingleInterAction(this, pcCuAgent, ptRecvNode);
			pcTask->InitStateMachine();
			SetCuReqMsg(cReq, pcTask);
		}
		break;
	case PU_SERIALPARAM_CFG_SET_REQ:
		{
			CCuPuSerialParamCfgSetReq cReq;
			cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);

			CSingleInterAction* pcTask = new CSingleInterAction(this, pcCuAgent, ptRecvNode);
			pcTask->InitStateMachine();
			SetCuReqMsg(cReq, pcTask);
		}
		break;
    case PU_MOTION_DETECT_GET_REQ:
        {
            CCuPuMotionDetectGetReq cReq;
            cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);
            cReq.SetUserUri(pcCuAgent->m_pcUserAccount->m_tUserID);

            CSingleInterAction* pcTask = new CSingleInterAction(this, pcCuAgent, ptRecvNode);
            pcTask->InitStateMachine();
            SetCuReqMsg(cReq, pcTask);
        }
        break;
    case PU_VSIP_OPERATE_REQ:
        {
            CCuPuVsipOperateReq cReq;
            cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);
            cReq.SetUserUri(pcCuAgent->m_pcUserAccount->m_tUserID);

            CSingleInterAction* pcTask = new CSingleInterAction(this, pcCuAgent, ptRecvNode);
            pcTask->InitStateMachine();
            SetCuReqMsg(cReq, pcTask);
        }
        break;
    case PU_AREA_SHADE_GET_REQ:
        {
            CCuPuAreaShadeGetReq cReq;
            cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);
            cReq.SetUserUri(pcCuAgent->m_pcUserAccount->m_tUserID);

            CSingleInterAction* pcTask = new CSingleInterAction(this, pcCuAgent, ptRecvNode);
            pcTask->InitStateMachine();
            SetCuReqMsg(cReq, pcTask);
        }
        break;
    case PU_AREA_SHADE_SET_REQ:
        {
            CCuPuAreaShadeSetReq cReq;
            cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);
            cReq.SetUserUri(pcCuAgent->m_pcUserAccount->m_tUserID);

            CSingleInterAction* pcTask = new CSingleInterAction(this, pcCuAgent, ptRecvNode);
            pcTask->InitStateMachine();
            SetCuReqMsg(cReq, pcTask);
        }
        break;
    case PU_MOTION_DETECT_SET_REQ:
        {
            CCuPuMotionDetectSetReq cReq;
            cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);
            cReq.SetUserUri(pcCuAgent->m_pcUserAccount->m_tUserID);

            CSingleInterAction* pcTask = new CSingleInterAction(this, pcCuAgent, ptRecvNode);
            pcTask->InitStateMachine();
            SetCuReqMsg(cReq, pcTask);
        }
        break;
    case PU_TRANSPARENT_OPERATE_REQ:
        {
            CCuPuTransparentOperateReq cReq;
            cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);
            cReq.SetUserUri(pcCuAgent->m_strUserName);

            bool bAuthRet = true; //��Ȩ���
            do 
            {
				//�����PTZ�������ж��Ƿ�������
				INSTLOG(CUI_APP, EVENT_LEV, "SrvName[%s], MsgType[%d], DevChn[%s]\n",
					cReq.GetSrvName().c_str(),
					cReq.GetMsgType(),
					cReq.GetDevChn().ToString().c_str());
#define MSG_TYPE_PU_TRANS_OPR_EXT_PTZ_CTRL_QRY 0x13F0 //Ѳ��������ѯ
#define MSG_TYPE_PU_TRANS_OPR_EXT_PTZ_CTRL_SET 0x13B1 //Ѳ����������

				if ((PU_TRANS_OPR_EXT_PTZ_CTRL == cReq.GetSrvName()) 
					&& (MSG_TYPE_PU_TRANS_OPR_EXT_PTZ_CTRL_QRY == cReq.GetMsgType() 
					|| MSG_TYPE_PU_TRANS_OPR_EXT_PTZ_CTRL_SET == cReq.GetMsgType()))
				{
					C3acAgent *pc3acAgent = (C3acAgent *)g_cCuiDataMgr.Get3asAgentTaskPtrBy3asName(pcCuAgent->m_str3asName, this);
					if (NULL == pc3acAgent)
					{
						INSTLOG(CUI_APP, ERROR_LEV, "�յ�����CU����Ϣ[%s--%hu]������û�ж�Ӧ��3acAgent\n",
							OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

						break;
					}

					CCuPtzLockInfo ptzLockInfo;
					if ( pc3acAgent->m_cPtzlockInfoList.GetCuPtzLockInfoByVs(cReq.GetDevChn(), ptzLockInfo) )
					{
						TChannel vsChn;
						vsChn.SetDevUri(GetNameWithoutDomainFromURI(cReq.GetDevChn().GetDevUri()));
						vsChn.SetChnNO(cReq.GetDevChn().GetChnNO());

						// �����Ч��Ҫɾ��
						if ( !ptzLockInfo.IsValid() )
						{
							pc3acAgent->Del3APtzLockInfoByVs(vsChn);

							INSTLOG(CUI_APP, CRITICAL_LEV, "ɾ������PTZ��\n");
							ptzLockInfo.PrintData();
						}
						else
						{
							INSTLOG(CUI_APP, CRITICAL_LEV, "�û�[%s Session: %s]��ǰ��[%s]ͨ��[%d]PTZ����ʧ�ܣ�PTZ������\n",
								pcCuAgent->m_strUserName.c_str(), strSession.c_str(),
								cReq.GetDevChn().GetDevUri().c_str(), cReq.GetDevChn().GetChnNO());

							CCuPuTransparentOperateRsp cCuPuTransparentOperateRsp;
							cCuPuTransparentOperateRsp.Clear();

							cCuPuTransparentOperateRsp.SetSession(cReq.GetSession());
							cCuPuTransparentOperateRsp.SetUserId(ptzLockInfo.GetUserId());
							cCuPuTransparentOperateRsp.SetUsername(ptzLockInfo.GetUserName());
							cCuPuTransparentOperateRsp.SetPriPtzLevel(ptzLockInfo.GetUserPtzLevel());
							cCuPuTransparentOperateRsp.SetBeginTime(ptzLockInfo.GetBeginTime());
							cCuPuTransparentOperateRsp.SetEndTime(ptzLockInfo.GetEndTime());
							cCuPuTransparentOperateRsp.SetErrorCode(ERR_CUI_PU_PTZ_LOCKED);

							CGetPuPtzLockUserInfoTask *pcGetLockInfoTask = new CGetPuPtzLockUserInfoTask(this, pcCuAgent, ptRecvNode);
							if ( pcGetLockInfoTask  == NULL || ptzLockInfo.GetUserId().empty())
							{
								SendRspToCu<CCuPuTransparentOperateRsp>(cCuPuTransparentOperateRsp, ptRecvNode);
							}
							else
							{
								pcGetLockInfoTask->SetPuTransOperateRsp(cCuPuTransparentOperateRsp);
								pcGetLockInfoTask->InitStateMachine();
								if (pcGetLockInfoTask->GetUserNameInfo(ptzLockInfo.GetUserId(), vsChn) == false)
								{
									SendRspToCu<CCuPuTransparentOperateRsp>(cCuPuTransparentOperateRsp, ptRecvNode);
								}
							}
							
							return dwRet;
						}
					}
				}

                string strDomainName = GetDomainNameWithoutPreNameFromURI(cReq.GetDevChn().GetDevUri());
                if ( ! pcCuAgent->IsExistDomainInfo(strDomainName))
                {
                    bAuthRet = false;
                    break;
                }
				
				if (pcCuAgent->m_pcUserAccount->IsSuperAdmin())
                {
                    bAuthRet = true;
                    break;
                }

				if (cReq.GetDevChn().IsValid())
				{
					string uuid = GetNameWithoutDomainFromURI(cReq.GetDevChn().GetDevUri());
					CVideosourcePrivilege cPri;
					string errMsg;
					EFDB_ErrorCode er = g_cRedisDb.GetUserVideoPriByEncChn(pcCuAgent->m_pcUserAccount->m_tUserID, uuid, cReq.GetDevChn().GetChnNO(), cPri, errMsg);
					if ( !IsFdbResultPositive(er) )
					{
						INSTLOG(CUI_APP, CRITICAL_LEV, "�û�name[%s] id[%s]û����ƵԴ[%s]Ȩ�� errmsg[%s] er[%d]\n",
							pcCuAgent->m_strUserName.c_str(), pcCuAgent->m_pcUserAccount->m_tUserID.c_str(), cReq.GetDevChn().ToXml().c_str(), errMsg.c_str(), er);
						bAuthRet = false;
						break;
					}
				}
            } while (0);

            if (!bAuthRet)
            {
                INSTLOG(CUI_APP, ERROR_LEV, "�û�[%s Session: %s]��ǰ��[%s]ͨ��[%d]����͸������ʧ�ܣ���Ȩ��\n", 
                    pcCuAgent->m_strUserName.c_str(), strSession.c_str(), 
                    cReq.GetDevChn().GetDevUri().c_str(), cReq.GetDevChn().GetChnNO());

                CPuSendTransparentDataRsp cRsp;
                cRsp.SetErrorCode(ERR_CUI_CU_NO_PERMISSION);
                SendRspToCu<CPuSendTransparentDataRsp>(cRsp, ptRecvNode);
                return dwRet;
            }

            CSingleInterAction *pcSendTransDataTask = new CSingleInterAction(this, pcCuAgent, ptRecvNode);
            pcSendTransDataTask->InitStateMachine();
            SetCuReqMsg(cReq, pcSendTransDataTask);
        }
        break;
    case KEY_FRAME_REQ:
        {
            CCuKeyFrameReq cReq;
            cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);

            CSingleInterAction* pcTask = new CSingleInterAction(this, pcCuAgent, ptRecvNode);
            pcTask->InitStateMachine();
            SetCuReqMsg(cReq, pcTask);
        }
        break;
	case CU_CUI_BATCH_OPT_REQ:
		{
			CCuBatOptReq cReq;
			cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);

			string strTmp = pcCuAgent->m_pcUserAccount->m_tUserID +"@"+ pcCuAgent->m_tCmuURI.GetDomain();
			cReq.SetUserId(strTmp);
			cReq.SetUserName(pcCuAgent->m_strUserName);

			if ( cReq.GetGroupId().empty() )
			{
				INSTLOG(CUI_APP, ERROR_LEV, "�û�[%s Session: %s]�������� ���������IDΪ��!\n",
					pcCuAgent->m_strUserName.c_str(), strSession.c_str());

				CCuBatOptRsp cRsp;
				cRsp.SetErrorCode(ERR_CUI_PARAMS_INVALID);
				SendRspToCu<CCuBatOptRsp>(cRsp, ptRecvNode);
				return dwRet;
			}

			vector<string> &paramList = cReq.GetParamList();

			switch (cReq.GetOptType())
			{
			case enBatOptStartPlatRecord:
			case enBatOptStopPlatRecord:
				{
					// �û�Ȩ�ޣ���У��һ��
					bool bAuthRet = true;

					if ( !pcCuAgent->m_pcUserAccount->IsSuperAdmin())
					{
						//�ж��û��Ƿ���ƽ̨�����Ȩ��
						CPrivilege cPri = pcCuAgent->m_pcUserAccount->GetUserPri();
						if (!cPri.IsEmpty())
						{
							if ( !cPri.HasPri(CUserPrivilege::en_Vod_Rec) )
							{
								bAuthRet = false;
							}
						}
						else
						{
							bAuthRet = false;
						}
					}

					if ( !bAuthRet )
					{
						INSTLOG(CUI_APP, ERROR_LEV, "�û�[%s Session: %s]��������ƽ̨¼��û��Ȩ��!\n",
							pcCuAgent->m_strUserName.c_str(), strSession.c_str());

						CCuBatOptRsp cRsp;
						cRsp.SetErrorCode(ERR_CUI_CU_NO_PERMISSION);
						SendRspToCu<CCuBatOptRsp>(cRsp, ptRecvNode);
						return dwRet;
					}

					g_cCuiDataMgr.m_cPlatBatOptRequestList.PushBackBatOptStrReq(cReq.ToXml());
				}
				break;
			case enBatOptStartPuRecord:
			case enBatOptStopPuRecrod:
			case enBatOptStopWiper:
				{
					// ����Ҫ��������У��
					g_cCuiDataMgr.m_cPuBatOptRequestList.PushBackBatOptStrReq(cReq.ToXml());
				}
				break;
			case enBatOptStartPresetCall:
				{
					bool bParamFailed = false;
					
					if (paramList.empty()
						|| paramList.front().empty())
					{
						bParamFailed = true;
					}
					else
					{
						stringstream sstream1(paramList.front());
						int nPresetNum1 = -1;
						sstream1 >> nPresetNum1;

						stringstream sstream2(paramList.front());
						int nPresetNum2 = 1;
						sstream2 >> nPresetNum2;

						// ����ֵ�����˵��û�гɹ�ȡ��Ԥ��λ��
						if (nPresetNum1 != nPresetNum2)
						{
							bParamFailed = true;
						}
					}

					if ( bParamFailed )
					{
						INSTLOG(CUI_APP, ERROR_LEV, "�û�[%s Session: %s]��������Ԥ��λ�����ò���ȷ!\n",
							pcCuAgent->m_strUserName.c_str(), strSession.c_str());

						CCuBatOptRsp cRsp;
						cRsp.SetErrorCode(ERR_CUI_PARAMS_INVALID);
						SendRspToCu<CCuBatOptRsp>(cRsp, ptRecvNode);
						return dwRet;
					}
					g_cCuiDataMgr.m_cPuBatOptRequestList.PushBackBatOptStrReq(cReq.ToXml());
				}
				break;
			case enBatOptStartWiper:
				{
					bool bParamFailed = false;
					// ������ˢ��Ҫ�����ٶ�
					if (paramList.empty()
						|| paramList.front().empty())
					{
						bParamFailed = true;
					}
					else
					{
						stringstream sstream1(paramList.front());
						int nSpeed1 = -1;
						sstream1 >> nSpeed1;

						stringstream sstream2(paramList.front());
						int nSpeed2 = 1;
						sstream2 >> nSpeed2;

						// ����ֵ�����˵��û�гɹ�ȡ�� Speed
						if (nSpeed1 != nSpeed2)
						{
							bParamFailed = true;
						}
					}

					if ( bParamFailed )
					{
						INSTLOG(CUI_APP, ERROR_LEV, "�û�[%s Session: %s]����������ˢ�ٶ����ò���ȷ!\n",
							pcCuAgent->m_strUserName.c_str(), strSession.c_str());

						CCuBatOptRsp cRsp;
						cRsp.SetErrorCode(ERR_CUI_PARAMS_INVALID);
						SendRspToCu<CCuBatOptRsp>(cRsp, ptRecvNode);
						return dwRet;
					}
					g_cCuiDataMgr.m_cPuBatOptRequestList.PushBackBatOptStrReq(cReq.ToXml());
				}
				break;
			case enBatOptStopPuCall:
				{
					bool bParamFailed = false;
					if (paramList.empty()
						|| paramList.front().empty())
					{
						bParamFailed = true;
					}
					else
					{
						stringstream sstream1(paramList.front());
						int nPlayId1 = -1;
						sstream1 >> nPlayId1;

						stringstream sstream2(paramList.front());
						int nPlayId2 = 1;
						sstream2 >> nPlayId2;

						// ����ֵ�����˵��û�гɹ�ȡ��playId
						if (nPlayId1 != nPlayId2)
						{
							bParamFailed = true;
						}
					}

					if (bParamFailed)
					{
						INSTLOG(CUI_APP, ERROR_LEV, "�û�[%s Session: %s]����ֹͣ��Ƶ����playid���ò���ȷ!\n",
							pcCuAgent->m_strUserName.c_str(), strSession.c_str());

						CCuBatOptRsp cRsp;
						cRsp.SetErrorCode(ERR_CUI_PARAMS_INVALID);
						SendRspToCu<CCuBatOptRsp>(cRsp, ptRecvNode);
						return dwRet;
					}

					g_cCuiDataMgr.m_cPuBatOptRequestList.PushBackBatOptStrReq(cReq.ToXml());
				}
				break;
			case enBatOptTypeInvalid:
			default:
				{
					INSTLOG(CUI_APP, ERROR_LEV, "�û�[%s Session: %s]������������δ֪!\n",
						pcCuAgent->m_strUserName.c_str(), strSession.c_str());

					CCuBatOptRsp cRsp;
					cRsp.SetErrorCode(ERR_CUI_PARAMS_INVALID);
					SendRspToCu<CCuBatOptRsp>(cRsp, ptRecvNode);
					return dwRet;
				}
				break;
			}

			if (g_cCuiDataMgr.m_dwBatOptCheckTaskNo == INVALID_TASKNO)
			{
				CCuBatOptCheckTask *pTask = new CCuBatOptCheckTask(this);
				if ( pTask == NULL )
				{
					INSTLOG(CUI_APP, ERROR_LEV, "�û�[%s Session: %s]��������ʱ������TASKʧ��!\n",
						pcCuAgent->m_strUserName.c_str(), strSession.c_str());
				}
				else
				{
					INSTLOG(CUI_APP, EVENT_LEV, "����TASK[%u]����������������!\n",pTask->GetTaskNO());
					g_cCuiDataMgr.m_dwBatOptCheckTaskNo = pTask->GetTaskNO();
					pTask->InitStateMachine();
				}
			}

			CCuBatOptRsp cRsp;
			cRsp.SetErrorCode(CMS_SUCCESS);
			SendRspToCu<CCuBatOptRsp>(cRsp, ptRecvNode);
			return dwRet;
		}
		break;
    case TAS_CUSER_QRY_REQ:
        {
            CuQryReqToTAS<CCuQueryUserReq>(pXmlMsgContent, pcCuAgent, ptRecvNode);
        }
        break;
    case TAS_CUSER_GROUP_QRY_REQ:
        {
            CuQryReqToTAS<CCuUserGroupQryReq>(pXmlMsgContent, pcCuAgent, ptRecvNode);
        }
        break;
    case TAS_CUSER_DEVICEFULLINFO_AND_PRIVILEGE_QRY_REQ:
        {
            CuQryReqToTAS<CCuUserDevFullInfoQryReq>(pXmlMsgContent, pcCuAgent, ptRecvNode);
        }
        break;
    case TAS_DEVTREE_QRY_REQ:
        {
            CuQryReqToTAS<CCuDevTreeQryReq>(pXmlMsgContent, pcCuAgent, ptRecvNode);
        }
		break;
	case TAS_DEVICE_GROUPPATH_QRY_REQ:
		{
			CuQryReqToTAS<CCuDeviceGrpPathQryReq>(pXmlMsgContent, pcCuAgent, ptRecvNode);
		}
		break;
    case CU_CUI_GET_USER_VIDEOSRC_LIST_REQ:
        {
            CCuUserDevGrpVidsrcQryReq cReq;
            cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);
            if (pcCuAgent->m_strCuSessionIn3AS.empty())
            {
                ReplyBackToCu(cReq, ptRecvNode, ERR_CUI_TAS_OFFLINE);
                return dwRet;
            }

            cReq.SetSession(pcCuAgent->m_strCuSessionIn3AS);

            if (!cReq.GetQueryTag2().empty())
            {
                if (!CuFetchQueryTask(cReq.GetQueryTag2(), ptRecvNode, this))
                {
                    ReplyBackToCu(cReq, ptRecvNode, ERR_CUI_TASK_DEL);
                }
            }
            else
            {
                CQueryTask4Fetch *pcCuQryReqTask = new CQueryTask4Fetch(this, pcCuAgent, ptRecvNode);
                pcCuQryReqTask->InitStateMachine();
                if (PROCMSG_DEL == SetCuReqMsg(cReq, pcCuQryReqTask))
                {
                    delete pcCuQryReqTask;
                }
            }
        }
        break;
	case CU_CUI_SECURE_CERTIFICATE_QRY_REQ:
		{
			//TODO:
			CCuCertificateQryReq cReq;
			cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);
			if (pcCuAgent->m_strCuSessionIn3AS.empty())
			{
				ReplyBackToCu(cReq, ptRecvNode, ERR_CUI_TAS_OFFLINE);
				return PROCMSG_DEL;
			}
			CSingleInterAction *pcSingleInterAction = new CSingleInterAction(this, pcCuAgent, ptRecvNode);
			pcSingleInterAction->InitStateMachine();
			if (PROCMSG_DEL == SetCuReqMsg(cReq, pcSingleInterAction))
			{
				delete pcSingleInterAction;
			}

		}
		break;
	case PU_SIGNATURE_CTRL_REQ:
		{
			//TODO:
			CCuDeviceSignatrueCtrlReq cReq;
			cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);
			if (pcCuAgent->m_strCuSessionIn3AS.empty())
			{
				ReplyBackToCu(cReq, ptRecvNode, ERR_CUI_TAS_OFFLINE);
				return PROCMSG_DEL;
			}
			CSingleInterAction *pcSingleInterAction = new CSingleInterAction(this, pcCuAgent, ptRecvNode);
			pcSingleInterAction->InitStateMachine();
			if (PROCMSG_DEL == SetCuReqMsg(cReq, pcSingleInterAction))
			{
				delete pcSingleInterAction;
			}
		}
		break;
	case PU_ENCRYPTION_CTRL_REQ:
		{
			CCuDeviceEncryptionCtrlReq cReq;
			cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);
			if (pcCuAgent->m_strCuSessionIn3AS.empty())
			{
				ReplyBackToCu(cReq, ptRecvNode, ERR_CUI_TAS_OFFLINE);
				return PROCMSG_DEL;
			}
			CSingleInterAction *pcSingleInterAction = new CSingleInterAction(this, pcCuAgent, ptRecvNode);
			pcSingleInterAction->InitStateMachine();
			if (PROCMSG_DEL == SetCuReqMsg(cReq, pcSingleInterAction))
			{
				delete pcSingleInterAction;
			}
		}
		break;
    case TAS_DEVICE_GROUP_QRY_REQ:
        {
			CCuUserDevGroupQryReq cReq;
			cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);
			if (pcCuAgent->m_strCuSessionIn3AS.empty())
			{
				ReplyBackToCu(cReq, ptRecvNode, ERR_CUI_TAS_OFFLINE);
				return dwRet;
			}

			cReq.SetSession(pcCuAgent->m_strCuSessionIn3AS);

			CDevGroupQueryTask *pcCuQryReqTask = new CDevGroupQueryTask(this, pcCuAgent, ptRecvNode);
			pcCuQryReqTask->InitStateMachine();
			SetCuReqMsg(cReq, pcCuQryReqTask);
        }
        break;
    case TAS_TVWALL_ADD_REQ:
        {
            if(pcCuAgent->m_pcUserAccount->IsSuperAdmin())
            {
                CCuTvwallAddReq cReq;
                cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);
                cReq.SetSession(pcCuAgent->m_strCuSessionIn3AS);

                CSingleInterAction *pcSendTransDataTask = new CSingleInterAction(this, pcCuAgent, ptRecvNode);
                pcSendTransDataTask->InitStateMachine();
                SetCuReqMsg(cReq, pcSendTransDataTask);
            }
            else
            {
                CCuTvwallAddRsp cRsp;
                cRsp.SetErrorCode(ERR_CUI_CU_NO_PERMISSION);
                SendRspToCu<CCuTvwallAddRsp>(cRsp, ptRecvNode);
                return dwRet;
            }
        }
        break;
    case TAS_DEVICE_CAP_INDEX_NAME_GET_REQ:
        {
            CMS_ASSERT(NULL != pXmlMsgContent);

            CCuDeviceCapIndexNameGetReq cReq;
            cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);
            cReq.SetDeviceId(GetNameWithoutDomainFromURI(cReq.GetDeviceId()));

            if ( pcCuAgent->m_strCuSessionIn3AS.empty() )
            {
                dwRet = PROCMSG_FAIL;
                ReplyBackToCu(cReq, ptRecvNode);
                return dwRet;
            }
            //��Ϊ�Ƿ���3AS�ģ�sessionҪ��дCU��3AS�ĻỰID
            cReq.SetSession(pcCuAgent->m_strCuSessionIn3AS);

            CSingleInterAction *pcTask = new CSingleInterAction(this, pcCuAgent, ptRecvNode);
            pcTask->InitStateMachine();
            SetCuReqMsg(cReq, pcTask);
        }
        break;
    case TAS_DEVICE_CAP_INDEX_NAME_SET_REQ:
        {
            CMS_ASSERT(NULL != pXmlMsgContent);

            CCuDeviceCapIndexNameSetReq cReq;
            cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);

			string devUUid = GetNameWithoutDomainFromURI(cReq.GetDeviceCapName().deviceId);
			cReq.GetDeviceCapName().deviceId = devUUid;

            if ( pcCuAgent->m_strCuSessionIn3AS.empty() )
            {
                dwRet = PROCMSG_FAIL;
                ReplyBackToCu(cReq, ptRecvNode);
                return dwRet;
            }

			if (!pcCuAgent->m_pcUserAccount->IsSuperAdmin()
				&& cReq.GetDeviceCapName().capId == DEVICE_CAP_VIDEOSOURCE)
			{
				if (!cReq.GetDeviceCapName().deviceCapIndexNames.empty())
				{
					TDeviceCapIndexName &tDeviceCapIndexName = cReq.GetDeviceCapName().deviceCapIndexNames.front();
					string strErrMsg;
					if ( !g_cRedisDb.IsUserHasVideoSource(pcCuAgent->m_pcUserAccount->m_tUserID, devUUid, tDeviceCapIndexName.index, strErrMsg) )
					{
						INSTLOG(CUI_APP, EVENT_LEV, "�û�[%s]û�л�����ƵԴ[%s:%d]���޸���ƵԴ����ʧ�ܣ�������Ȩ�� errMsg[%s]\n",
							pcCuAgent->m_strUserName.c_str(), cReq.GetDeviceCapName().deviceId.c_str(), tDeviceCapIndexName.index, strErrMsg.c_str());

						CCuDeviceCapIndexNameSetRsp cRsp;
						cRsp.SetErrorCode(ERR_CUI_CU_NO_PERMISSION);
						SendRspToCu<CCuDeviceCapIndexNameSetRsp>(cRsp, ptRecvNode);
						return PROCMSG_OK;
					}
				}
			}

            //��Ϊ�Ƿ���3AS�ģ�sessionҪ��дCU��3AS�ĻỰID
            cReq.SetSession(pcCuAgent->m_strCuSessionIn3AS);

            CSingleInterAction *pcTask = new CSingleInterAction(this, pcCuAgent, ptRecvNode);
            pcTask->InitStateMachine();
            SetCuReqMsg(cReq, pcTask);
        }
        break;
	case TAS_DEVGRP_VIDSRC_TOTALNUM_GET_REQ:
		{
			CMS_ASSERT(NULL != pXmlMsgContent);

			CCuDevGrpVidsrcTotalnumGetReq cReq;
			cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);
			
			if (cReq.GetDevGrpIds().empty())
			{
				dwRet = PROCMSG_FAIL;
				ReplyBackToCu(cReq, ptRecvNode, ERR_CUI_PARAMS_INVALID);
				return dwRet;
			}

			vector<UUIDString> &devGroupIdList = cReq.GetDevGrpIds();
			for (size_t nIndex = 0; nIndex < devGroupIdList.size(); nIndex++)
			{
				devGroupIdList[nIndex] = GetNameWithoutDomainFromURI(devGroupIdList[nIndex]);
			}
			
			if (pcCuAgent->m_strCuSessionIn3AS.empty())
			{
				dwRet = PROCMSG_FAIL;
				ReplyBackToCu(cReq, ptRecvNode);
				return dwRet;
			}

			cReq.SetUserId(pcCuAgent->m_pcUserAccount->m_tUserID);
			//��Ϊ�Ƿ���3AS�ģ�sessionҪ��дCU��3AS�ĻỰID
			cReq.SetSession(pcCuAgent->m_strCuSessionIn3AS);

			CSingleInterAction *pcTask = new CSingleInterAction(this, pcCuAgent, ptRecvNode);
			pcTask->InitStateMachine();
			SetCuReqMsg(cReq, pcTask);
		}
		break;
	case TAS_VIDSRC_INFO_QRY_REQ:
		{
			CMS_ASSERT(NULL != pXmlMsgContent);

			CuQryReqToTAS<CCuVidsrcInfoQryReq>(pXmlMsgContent, pcCuAgent, ptRecvNode);
		}
		break;
	case TAS_DEVICE_GBINFO_QRY_REQ:
		{
			CMS_ASSERT(NULL != pXmlMsgContent);

			CuQryReqToTAS<CCuDeviceGbinfoQryReq>(pXmlMsgContent, pcCuAgent, ptRecvNode);
		}
		break;
	case TAS_DEVGRP_GBINFO_QRY_REQ:
		{
			CMS_ASSERT(NULL != pXmlMsgContent);

			CuQryReqToTAS<CCuDevgrpGbinfoQryReq>(pXmlMsgContent, pcCuAgent, ptRecvNode);
		}
		break;
	case CUI_GET_AZIMUTH_REQ:
		{
			CCuGetAzimuthReq cReq;
			cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);

			CCuGetAzimuthRsp cRsp;

			// ��Ȩ��У���û��Ƿ񻮹�������豸 ������ͳһ��Ȩ����ʱ����Ȩ

			// redis �汾ֱ��ȡ����
			TPuConfig tPuConfig;
			string devUUid = GetNameWithoutDomainFromURI(cReq.GetVidSrcChn().GetDevUri());

			EFDB_ErrorCode er = g_cRedisDb.GetPuConfig(devUUid, tPuConfig, TPuConfig_azimuth(cReq.GetVidSrcChn().GetChnNO()));
			if ( !IsFdbResultPositive(er) )
			{
				cRsp.SetErrorCode(ERR_CUI_GET_REDIS_DATA_FAILED);
				SendRspToCu<CCuGetAzimuthRsp>(cRsp, ptRecvNode);
				return PROCMSG_FAIL;
			}

			map<int,string>::iterator iter = tPuConfig.GetAzimuth().find(cReq.GetVidSrcChn().GetChnNO());
			if (iter != tPuConfig.GetAzimuth().end())
			{
				cRsp.SetAzimuthInfo(iter->second);
			}

			SendRspToCu<CCuGetAzimuthRsp>(cRsp, ptRecvNode);
			return PROCMSG_OK;
		}
		break;
	case CONFIG_SYNC_SWITCH_GET_REQ:
		{
			CCuGetSyncVideoSrcNameEnableReq cReq;
			cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);
						
			CSingleInterAction* pcTask = new CSingleInterAction(this, pcCuAgent, ptRecvNode);
			pcTask->InitStateMachine();
			SetCuReqMsg(cReq, pcTask);
		}
		break;
	case CONFIG_SYNC_SWITCH_SET_REQ:
		{
			CCuSetSyncVideoSrcNameEnableReq cReq;
			cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);

			CSingleInterAction* pcTask = new CSingleInterAction(this, pcCuAgent, ptRecvNode);
			pcTask->InitStateMachine();
			SetCuReqMsg(cReq, pcTask);
		}
		break;
    case TAS_TVWALL_DEL_REQ:
        {

            if(pcCuAgent->m_pcUserAccount->IsSuperAdmin())
            {
                CCuTvwallDelReq cReq;
                cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);
                cReq.SetSession(pcCuAgent->m_strCuSessionIn3AS);

                CSingleInterAction *pcSendTransDataTask = new CSingleInterAction(this, pcCuAgent, ptRecvNode);
                pcSendTransDataTask->InitStateMachine();
                SetCuReqMsg(cReq, pcSendTransDataTask);
            }
            else
            {
                CCuTvwallDelRsp cRsp;
                cRsp.SetErrorCode(ERR_CUI_CU_NO_PERMISSION);
                SendRspToCu<CCuTvwallDelRsp>(cRsp, ptRecvNode);
                return dwRet;
            }
        }
        break;
    case TAS_TVWALL_MOD_REQ:
        {            
            if(pcCuAgent->m_pcUserAccount->IsSuperAdmin())
            {
                CCuTvwallModReq cReq;
                cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);
                cReq.SetSession(pcCuAgent->m_strCuSessionIn3AS);

                CSingleInterAction *pcSendTransDataTask = new CSingleInterAction(this, pcCuAgent, ptRecvNode);
                pcSendTransDataTask->InitStateMachine();
                SetCuReqMsg(cReq, pcSendTransDataTask);
            }
            else
            {
                CCuTvwallModRsp cRsp;
                cRsp.SetErrorCode(ERR_CUI_CU_NO_PERMISSION);
                SendRspToCu<CCuTvwallModRsp>(cRsp, ptRecvNode);
                return dwRet;
            }   
        }
        break;
    case PU_VID_ENC_PARAM_GET_REQ:
        {
            CCuPuVidEncParamGetReq cReq;
            cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);

            string strUsrUri = pcCuAgent->m_pcUserAccount->m_tUserID +"@"+ pcCuAgent->m_tCmuURI.GetDomain();
            cReq.SetUserUri(strUsrUri);

            CSingleInterAction *pcTask = new CSingleInterAction(this, pcCuAgent, ptRecvNode);
            pcTask->InitStateMachine();
            SetCuReqMsg(cReq, pcTask);
        }
        break;
    case PU_VID_ENC_PARAM_SET_REQ:
        {
            CCuPuVidEncParamSetReq cReq;
            cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);

            string strUsrUri = pcCuAgent->m_pcUserAccount->m_tUserID +"@"+ pcCuAgent->m_tCmuURI.GetDomain();
            cReq.SetUserUri(strUsrUri);

            CSingleInterAction *pcTask = new CSingleInterAction(this, pcCuAgent, ptRecvNode);
            pcTask->InitStateMachine();
            SetCuReqMsg(cReq, pcTask);
        }
		break;
	case PU_AUD_ENC_PARAM_GET_REQ:
		{
			CCuPuAudEncParamGetReq cReq;
			cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);

			string strUsrUri = pcCuAgent->m_pcUserAccount->m_tUserID +"@"+ pcCuAgent->m_tCmuURI.GetDomain();
			cReq.SetUserUri(strUsrUri);

			CSingleInterAction *pcTask = new CSingleInterAction(this, pcCuAgent, ptRecvNode);
			pcTask->InitStateMachine();
			SetCuReqMsg(cReq, pcTask);
		}
		break;
	case PU_VIDSRC_OSD_SET_REQ:
		{
			CCuPuVidSrcOsdSetReq cReq;
			cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);

			CSingleInterAction *pcTask = new CSingleInterAction(this, pcCuAgent, ptRecvNode);
			pcTask->InitStateMachine();
			SetCuReqMsg(cReq, pcTask);
		}
		break;
	case PU_VIDSRC_OSD_GET_REQ:
		{
			CCuPuVidSrcOsdGetReq cReq;
			cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);

			CSingleInterAction *pcTask = new CSingleInterAction(this, pcCuAgent, ptRecvNode);
			pcTask->InitStateMachine();
			SetCuReqMsg(cReq, pcTask);
		}
		break;
    case PU_CAMERA_TYPE_SET_REQ:
        {
            CCuPuCameraTypeSetReq cReq;
            cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);

            string strUsrUri = pcCuAgent->m_pcUserAccount->m_tUserID +"@"+ pcCuAgent->m_tCmuURI.GetDomain();
            cReq.SetUserUri(strUsrUri);

            CSingleInterAction *pcTask = new CSingleInterAction(this, pcCuAgent, ptRecvNode);
            pcTask->InitStateMachine();
            SetCuReqMsg(cReq, pcTask);
        }
        break;
	case PU_ALARM_RESET_REQ:
		{
			CMS_ASSERT(NULL != pXmlMsgContent);
			CCuPuAlarmResetReq cReq;
			cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);

			CSingleInterAction *pcTask = new CSingleInterAction(this, pcCuAgent, ptRecvNode);
			pcTask->InitStateMachine();
			SetCuReqMsg(cReq, pcTask);
		}
		break;
    case TAS_TVWALL_QRY_REQ:
        {
            CMS_ASSERT(NULL != pXmlMsgContent);

            CCuTvwallQryReq cReq;
            cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);

            string strGrpID;
            cReq.GetCondition().GetCuserGrpId(strGrpID);
            if(strGrpID.empty())
            {
                string strUserID;
                cReq.GetCondition().GetCuserId(strUserID);

                if(strUserID.empty())
                {
                    cReq.GetCondition().SetCuserId(pcCuAgent->m_pcUserAccount->m_tUserID);
                }
            }  

            if ( pcCuAgent->m_strCuSessionIn3AS.empty() )
            {
                ReplyBackToCu(cReq, ptRecvNode);
                return dwRet;
            }
            cReq.SetSession(pcCuAgent->m_strCuSessionIn3AS);

            CQueryTask *pcCuQryReqTask = new CQueryTask(this, pcCuAgent, ptRecvNode);
            pcCuQryReqTask->InitStateMachine();
            SetCuReqMsg(cReq, pcCuQryReqTask);
        }
        break;
    case TAS_DOMAIN_RELATION_QRY_REQ:
        {
            CuQryReqToTAS<CCuDomainRelationQryReq>(pXmlMsgContent, pcCuAgent, ptRecvNode);
        }
        break;
    case CU_CUI_SUBSCRIBE_PUSTATUS_REQ:
        {
            //����PU״̬
            CCuSubscribeNotifyReq cReq;
            cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);

            //����һ�ο��Զ��Ķ��PuStatus�����Ҫ�������Task
            CSubBatchTask *pcSubBatchTask = new CSubBatchTask(this, pcCuAgent, ptRecvNode);
            pcSubBatchTask->InitStateMachine();
            SetCuReqMsg(cReq, pcSubBatchTask);
        }
        break;
    case CU_CUI_UNSUBSCRIBE_PUSTATUS_REQ:
        {
            CCuUnSubscribeNotifyReq cReq;
            cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);

            CSubBatchTask *pcSubBatchTask = new CSubBatchTask(this, pcCuAgent, ptRecvNode);
            pcSubBatchTask->InitStateMachine();
            SetCuReqMsg(cReq, pcSubBatchTask);
        }
        break;
    case TAS_DEVICE_GROUP_DEVICE_QRY_REQ:
        {
            CuQryReqToTAS<CCuQryDevGroupOfDevInfoReq>(pXmlMsgContent, pcCuAgent, ptRecvNode);
        }
        break;
    case TAS_DEVICE_GROUP_ADD_REQ:
        {
            CMS_ASSERT(NULL != pXmlMsgContent);
            CMS_ASSERT(NULL != pcCuAgent->m_pcUserAccount);

            CCuCreateDeviceGroupReq cReq;
            cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);
            if ( ! pcCuAgent->IsTasOnline() )
            {
                ReplyBackToCu(cReq, ptRecvNode);
                return dwRet;
            }
            //��Ϊ�Ƿ���3AS�ģ�sessionҪ��дCU��3AS�ĻỰID
            cReq.SetSession(pcCuAgent->m_strCuSessionIn3AS);

            if(!cReq.GetDeviceGroupInfo().GetDeviceParentGroupID().empty())
            {
                cReq.GetDeviceGroupInfo().SetDeviceParentGroupID(GetNameWithoutDomainFromURI(cReq.GetDeviceGroupInfo().GetDeviceParentGroupID()));
            }

            C3acAgent *pc3acAgent = (C3acAgent *)g_cCuiDataMgr.Get3asAgentTaskPtrBy3asName(pcCuAgent->m_str3asName, this);
            if ( NULL != pc3acAgent )
            {
                cReq.GetDeviceGroupInfo().SetDomainUUID(pc3acAgent->m_t3ASDomainID);
            }

            CSingleInterAction *pcCuSingleOperTask = new CSingleInterAction(this, pcCuAgent, ptRecvNode);
            pcCuSingleOperTask->InitStateMachine();
            SetCuReqMsg(cReq, pcCuSingleOperTask);
        }
        break;
    case TAS_DEVICE_GROUP_DEL_REQ:
        {
            //CuSingleOperReqToTAS<CCuDeleteDeviceGroupReq>(pXmlMsgContent, pcCuAgent, ptRecvNode);
            CMS_ASSERT(NULL != pXmlMsgContent);
            CMS_ASSERT(NULL != pcCuAgent->m_pcUserAccount);

            CCuDeleteDeviceGroupReq cReq;
            cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);
            if ( ! pcCuAgent->IsTasOnline() )
            {
                ReplyBackToCu(cReq, ptRecvNode);
                return dwRet;
            }
            //��Ϊ�Ƿ���3AS�ģ�sessionҪ��дCU��3AS�ĻỰID
            cReq.SetSession(pcCuAgent->m_strCuSessionIn3AS);

            cReq.GetDeviceGroupInfoKey().SetDeviceGroupID(GetNameWithoutDomainFromURI(cReq.GetDeviceGroupInfoKey().GetDeviceGroupID()));

            CSingleInterAction *pcCuSingleOperTask = new CSingleInterAction(this, pcCuAgent, ptRecvNode);
            pcCuSingleOperTask->InitStateMachine();
            SetCuReqMsg(cReq, pcCuSingleOperTask);
        }
        break;
    case TAS_DEVICE_GROUP_MOD_REQ:
        {
            //CuSingleOperReqToTAS<CCuModifyDeviceGroupReq>(pXmlMsgContent, pcCuAgent, ptRecvNode);
            CMS_ASSERT(NULL != pXmlMsgContent);
            CMS_ASSERT(NULL != pcCuAgent->m_pcUserAccount);

            CCuModifyDeviceGroupReq cReq;
            cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);
            if ( ! pcCuAgent->IsTasOnline() )
            {
                ReplyBackToCu(cReq, ptRecvNode);
                return dwRet;
            }
            //��Ϊ�Ƿ���3AS�ģ�sessionҪ��дCU��3AS�ĻỰID
            cReq.SetSession(pcCuAgent->m_strCuSessionIn3AS);

            string strDevParentGroupID;
            cReq.GetDeviceParentGroupID(strDevParentGroupID);
            if(!strDevParentGroupID.empty())
            {
                cReq.SetDeviceParentGroupID(GetNameWithoutDomainFromURI(strDevParentGroupID));
            }
            string strDevGroupID;
            cReq.GetDeviceGroupID(strDevGroupID);
            if(!strDevGroupID.empty())
            {
                cReq.SetDeviceGroupID(GetNameWithoutDomainFromURI(strDevGroupID));
            }

            CSingleInterAction *pcCuSingleOperTask = new CSingleInterAction(this, pcCuAgent, ptRecvNode);
            pcCuSingleOperTask->InitStateMachine();
            SetCuReqMsg(cReq, pcCuSingleOperTask);
        }
		break;
	case TAS_DEVICE_GROUP_MOD_4GB_REQ:
		{
			CMS_ASSERT(NULL != pXmlMsgContent);
			CMS_ASSERT(NULL != pcCuAgent->m_pcUserAccount);

			CCuModifyGBDeviceGroupReq cReq;
			cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);
			if (!pcCuAgent->IsTasOnline())
			{
				ReplyBackToCu(cReq, ptRecvNode);
				return dwRet;
			}
			//��Ϊ�Ƿ���3AS�ģ�sessionҪ��дCU��3AS�ĻỰID
			cReq.SetSession(pcCuAgent->m_strCuSessionIn3AS);

			string strDevParentGroupID = cReq.GetDevGrpId();
			if (!strDevParentGroupID.empty())
			{
				cReq.SetDevGrpId(GetNameWithoutDomainFromURI(strDevParentGroupID));
			}

			CSingleInterAction *pcCuSingleOperTask = new CSingleInterAction(this, pcCuAgent, ptRecvNode);
			pcCuSingleOperTask->InitStateMachine();
			SetCuReqMsg(cReq, pcCuSingleOperTask);
		}
		break;
    case TAS_DEVICE_GROUP_DEVICE_ORDER_REQ:
        {

			CMS_ASSERT(NULL != pXmlMsgContent);
			CMS_ASSERT(NULL != pcCuAgent->m_pcUserAccount);
            

            CDeviceGroupDeviceOrderReq cReq; 
            cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);
			cReq.SetSession(pcCuAgent->m_strCuSessionIn3AS);    //��Ϊ�Ƿ���3AS�ģ�sessionҪ��дCU��3AS�ĻỰID

            if (!pcCuAgent->IsTasOnline())
			{
				ReplyBackToCu(cReq, ptRecvNode);
				return dwRet;
			}

            CSingleInterAction *pcCuSingleOperTask = new CSingleInterAction(this, pcCuAgent, ptRecvNode);
			pcCuSingleOperTask->InitStateMachine();
			SetCuReqMsg(cReq, pcCuSingleOperTask);

        }
        break;

    case TAS_DEVICE_GROUP_DEVICE_ADD_REQ:
        {
            CuBatchOperReqToTAS<CCuDeviceGroupAddDeviceReq>(pXmlMsgContent, pcCuAgent, ptRecvNode);
        }
        break;
    case TAS_DEVICE_GROUP_DEVICE_DEL_REQ:
        {
            CuBatchOperReqToTAS<CCuDeviceGroupDelDeviceReq>(pXmlMsgContent, pcCuAgent, ptRecvNode);
        }
        break;
    case TAS_CUSER_GROUP_ADD_REQ:
        {
            CMS_ASSERT(NULL != pXmlMsgContent);
            CMS_ASSERT(NULL != pcCuAgent->m_pcUserAccount);

            CCuCreateUserGroupReq cReq;
            cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);
            if ( pcCuAgent->m_strCuSessionIn3AS.empty() )
            {
                ReplyBackToCu(cReq, ptRecvNode);
                return dwRet;
            }
            //��Ϊ�Ƿ���3AS�ģ�sessionҪ��дCU��3AS�ĻỰID
            cReq.SetSession(pcCuAgent->m_strCuSessionIn3AS);
            cReq.GetUserGroupInfo().SetCreateUser(pcCuAgent->m_pcUserAccount->m_tUserID);

            CSingleInterAction *pcCuSingleOperTask = new CSingleInterAction(this, pcCuAgent, ptRecvNode);
            pcCuSingleOperTask->InitStateMachine();
            SetCuReqMsg(cReq, pcCuSingleOperTask);
        }
        break;
    case TAS_CUSER_GROUP_DEL_REQ:
        {
            CuSingleOperReqToTAS<CCuDeleteUserGroupReq>(pXmlMsgContent, pcCuAgent, ptRecvNode);
        }
        break;
    case TAS_CUSER_GROUP_MOD_REQ:
        {
            CuSingleOperReqToTAS<CCuModifyUserGroupReq>(pXmlMsgContent, pcCuAgent, ptRecvNode);
        }
        break;
    case TAS_CUSER_ADD_REQ:
        {
            CMS_ASSERT(NULL != pXmlMsgContent);
            CMS_ASSERT(NULL != pcCuAgent->m_pcUserAccount);

            CCuCreateUserReq cReq;
            cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);
            if ( pcCuAgent->m_strCuSessionIn3AS.empty() )
            {
                ReplyBackToCu(cReq, ptRecvNode);
                return dwRet;
            }
            //��Ϊ�Ƿ���3AS�ģ�sessionҪ��дCU��3AS�ĻỰID
            cReq.SetSession(pcCuAgent->m_strCuSessionIn3AS);
            cReq.GetUserInfo().SetCreatorID(pcCuAgent->m_pcUserAccount->m_tUserID);
            string &strUserName = cReq.GetUserInfo().GetUserName();
            if ( string::npos != strUserName.find("@") )
            { //�û����к���@domain����Ҫȥ��
                cReq.GetUserInfo().SetUserName(GetNameWithoutDomainFromURI(strUserName));
            }

            CSingleInterAction *pcCuSingleOperTask = new CSingleInterAction(this, pcCuAgent, ptRecvNode);
            pcCuSingleOperTask->InitStateMachine();
            SetCuReqMsg(cReq, pcCuSingleOperTask);
            //CuSingleOperReqToTAS<CCuCreateUserReq>(pXmlMsgContent, pcCuAgent, ptRecvNode);
        }
        break;
    case TAS_CUSER_DEL_REQ:
        {
            CuSingleOperReqToTAS<CCuDeleteUserReq>(pXmlMsgContent, pcCuAgent, ptRecvNode);
        }
        break;
    case TAS_CUSER_MOD_REQ:
        {
            CMS_ASSERT(NULL != pXmlMsgContent);
            CMS_ASSERT(NULL != pcCuAgent->m_pcUserAccount);

            CCuModifyUserReq cReq;
            cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);
            if ( pcCuAgent->m_strCuSessionIn3AS.empty() )
            {
                ReplyBackToCu(cReq, ptRecvNode);
                return dwRet;
            }
            //��Ϊ�Ƿ���3AS�ģ�sessionҪ��дCU��3AS�ĻỰID
            cReq.SetSession(pcCuAgent->m_strCuSessionIn3AS);
            string strUserName;
            if ( cReq.GetUserName(strUserName) )
            {
                if ( string::npos != strUserName.find("@") )
                {
                    cReq.SetUserName(GetNameWithoutDomainFromURI(strUserName));
                }
            }

            CSingleInterAction *pcCuSingleOperTask = new CSingleInterAction(this, pcCuAgent, ptRecvNode);
            pcCuSingleOperTask->InitStateMachine();
            SetCuReqMsg(cReq, pcCuSingleOperTask);

            //CuSingleOperReqToTAS<CCuModifyUserReq>(pXmlMsgContent, pcCuAgent, ptRecvNode);
        }
		break;
	case TAS_DEVTREE_ADD_REQ:
		{
			if( !pcCuAgent->m_pcUserAccount->IsSuperAdmin() )
			{
				CCuDevTreeAddRsp cRsp;
				cRsp.SetErrorCode(ERR_CUI_CU_NO_PERMISSION);
				SendRspToCu<CCuDevTreeAddRsp>(cRsp, ptRecvNode);
				return dwRet;
			}

			CCuDevTreeAddReq cReq;
			cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);

			if ( pcCuAgent->m_strCuSessionIn3AS.empty() )
			{
				dwRet = PROCMSG_FAIL;
				ReplyBackToCu(cReq, ptRecvNode);
				return dwRet;
			}
			//��Ϊ�Ƿ���3AS�ģ�sessionҪ��дCU��3AS�ĻỰID
			cReq.SetSession(pcCuAgent->m_strCuSessionIn3AS);

			CSingleInterAction *pcTask = new CSingleInterAction(this, pcCuAgent, ptRecvNode);
			pcTask->InitStateMachine();
			SetCuReqMsg(cReq, pcTask);
		}
		break;
	case TAS_DEVTREE_MOD_REQ:
		{
			if ( !pcCuAgent->m_pcUserAccount->IsSuperAdmin() )
			{
				CCuDevTreeModRsp cRsp;
				cRsp.SetErrorCode(ERR_CUI_CU_NO_PERMISSION);
				SendRspToCu<CCuDevTreeModRsp>(cRsp, ptRecvNode);
				return dwRet;
			}

			CCuDevTreeModReq cReq;
			cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);

			if ( pcCuAgent->m_strCuSessionIn3AS.empty() )
			{
				dwRet = PROCMSG_FAIL;
				ReplyBackToCu(cReq, ptRecvNode);
				return dwRet;
			}
			//��Ϊ�Ƿ���3AS�ģ�sessionҪ��дCU��3AS�ĻỰID
			cReq.SetSession(pcCuAgent->m_strCuSessionIn3AS);

			CSingleInterAction *pcTask = new CSingleInterAction(this, pcCuAgent, ptRecvNode);
			pcTask->InitStateMachine();
			SetCuReqMsg(cReq, pcTask);
		}
		break;
	case TAS_DEVTREE_DEL_REQ:
		{
			if ( !pcCuAgent->m_pcUserAccount->IsSuperAdmin() )
			{
				CCuDevTreeDelRsp cRsp;
				cRsp.SetErrorCode(ERR_CUI_CU_NO_PERMISSION);
				SendRspToCu<CCuDevTreeDelRsp>(cRsp, ptRecvNode);
				return dwRet;
			}

			CCuDevTreeDelReq cReq;
			cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);

			if ( pcCuAgent->m_strCuSessionIn3AS.empty() )
			{
				dwRet = PROCMSG_FAIL;
				ReplyBackToCu(cReq, ptRecvNode);
				return dwRet;
			}
			//��Ϊ�Ƿ���3AS�ģ�sessionҪ��дCU��3AS�ĻỰID
			cReq.SetSession(pcCuAgent->m_strCuSessionIn3AS);

			CSingleInterAction *pcTask = new CSingleInterAction(this, pcCuAgent, ptRecvNode);
			pcTask->InitStateMachine();
			SetCuReqMsg(cReq, pcTask);
		}
		break;
    case CU_CUI_CHANGE_PASSWORD_REQ:
        {
            CMS_ASSERT(NULL != pXmlMsgContent);
            CMS_ASSERT(NULL != pcCuAgent->m_pcUserAccount);

            CCuChangePasswordReq cCuReq;
            cCuReq.FromXml(pXmlMsgContent);

            CCuModifyUserReq req;
            if ( pcCuAgent->m_strCuSessionIn3AS.empty() )
            {
                ReplyBackToCu(req, ptRecvNode);
                return dwRet;
            }

            //��Ϊ�Ƿ���3AS�ģ�sessionҪ��дCU��3AS�ĻỰID  
            req.SetSession(pcCuAgent->m_strCuSessionIn3AS);
            req.SetUserID(pcCuAgent->m_pcUserAccount->m_tUserID);
            req.SetUserOldPwd(cCuReq.GetOldPassword());
            req.SetUserPwd(cCuReq.GetNewPassword());
            req.SetPwdExpireDate(cCuReq.GetPasswordOverdueTime());

            CSingleInterAction *pcCuSingleOperTask = new CSingleInterAction(this, pcCuAgent, ptRecvNode);
            pcCuSingleOperTask->InitStateMachine();
            SetCuReqMsg(req, pcCuSingleOperTask);
        }
        break;
    case TAS_CUSER_DEVICE_QRY_REQ:
        {
            CuQryReqToTAS<CCuGetUserDeviceReq>(pXmlMsgContent, pcCuAgent, ptRecvNode);
        }
        break;
    case TAS_CUSER_GROUP_DEVICE_QRY_REQ:
        {
            CuQryReqToTAS<CCuGetUserGroupDeviceReq>(pXmlMsgContent, pcCuAgent, ptRecvNode);
        }
		break;
	case TAS_DEVICE_GROUP_DEVICE_QRY_4GB_REQ:
		{
			CuQryReqToTAS<CCuDeviceGroupDeviceQry4GbReq>(pXmlMsgContent, pcCuAgent, ptRecvNode);
		}
		break;
    case TAS_CUSER_DEVICE_ADD_REQ:
        {
            CuBatchOperReqToTAS<CCuUserAddDeviceReq>(pXmlMsgContent, pcCuAgent, ptRecvNode);
        }
        break;
    case TAS_CUSER_GROUP_DEVICE_ADD_REQ:
        {
            CuBatchOperReqToTAS<CCuUserGroupAddDeviceReq>(pXmlMsgContent, pcCuAgent, ptRecvNode);
        }
        break;
    case TAS_CUSER_DEVICE_DEL_REQ:
        {
            CuBatchOperReqToTAS<CCuUserDelDeviceReq>(pXmlMsgContent, pcCuAgent, ptRecvNode);
        }
        break;
    case TAS_CUSER_GROUP_DEVICE_DEL_REQ:
        {
            CuBatchOperReqToTAS<CCuUserGroupDelDeviceReq>(pXmlMsgContent, pcCuAgent, ptRecvNode);
        }
        break;
    case TAS_CUSER_DEVICE_MOD_REQ:
        {
            CuBatchOperReqToTAS<CCuUserModDeviceReq>(pXmlMsgContent, pcCuAgent, ptRecvNode);
        }
        break;
    case TAS_CUSER_GROUP_DEVICE_MOD_REQ:
        {
            CuBatchOperReqToTAS<CCuUserGroupModDeviceReq>(pXmlMsgContent, pcCuAgent, ptRecvNode);
        }
        break;
	case TAS_CUSER_DEVICE_GROUP_ADD_REQ:
		{
			CuSingleOperReqToTAS<CCuUserDeviceGroupAddReq>(pXmlMsgContent, pcCuAgent, ptRecvNode);
		}
		break;
	case TAS_CUSER_DEVICE_GROUP_DEL_REQ:
		{
			CuSingleOperReqToTAS<CCuUserDeviceGroupDelReq>(pXmlMsgContent, pcCuAgent, ptRecvNode);
		}
		break;
	case TAS_DEVTREE_DEVGROUP_ASSIGN_REQ:
		{
			CuSingleOperReqToTAS<CCuDevTreeDevGrpAssignReq>(pXmlMsgContent, pcCuAgent, ptRecvNode);
		}
		break;
	case TAS_CUSER_GROUP_DEVICE_GROUP_ADD_REQ:
		{
			CuSingleOperReqToTAS<CCuUserGroupDeviceGroupAddReq>(pXmlMsgContent, pcCuAgent, ptRecvNode);
		}
		break;
	case TAS_CUSER_GROUP_DEVICE_GROUP_DEL_REQ:
		{
			CuSingleOperReqToTAS<CCuUserGroupDeviceGroupDelReq>(pXmlMsgContent, pcCuAgent, ptRecvNode);
		}
		break;
	case TAS_DEVICE_AUTO_ASSIGN_TO_USER_SET_REQ:
		{
			CuSingleOperReqToTAS<CCuDeviceAutoAssignToUserSetReq>(pXmlMsgContent, pcCuAgent, ptRecvNode);
		}
		break;
	case TAS_CUSER_DATA_VERSION_GET_REQ:
		{
			CuSingleOperReqToTAS<CCuUserDataVersionGetReq>(pXmlMsgContent, pcCuAgent, ptRecvNode);
		}
		break;
	case TAS_DEVICE_AUTO_ASSIGN_TO_USER_GROUP_SET_REQ:
		{
			CuSingleOperReqToTAS<CCuDeviceAutoAssignToUserGroupSetReq>(pXmlMsgContent, pcCuAgent, ptRecvNode);
		}
		break;
	case TAS_DEVICE_AUTO_ASSIGN_TO_USER_QRY_REQ:
		{
			CuQryReqToTAS<CCuDeviceAutoAssignToUserQryReq>(pXmlMsgContent, pcCuAgent, ptRecvNode);
		}
		break;
	case TAS_DEVICE_AUTO_ASSIGN_TO_USER_GROUP_QRY_REQ:
		{
			CuQryReqToTAS<CCuDeviceAutoAssignToUserGroupQryReq>(pXmlMsgContent, pcCuAgent, ptRecvNode);
		}
		break;
	case TAS_TRANSACTION_PROGRESS_GET_REQ:
		{
			CuSingleOperReqToTAS<CCuTransactionProgressGetReq>(pXmlMsgContent, pcCuAgent, ptRecvNode);
		}
		break;
    case TAS_TEMP_AUTHORIZATION_QRY_REQ:
        {
            /*��ʱ��Ȩ�豸ֻ���ɳ�������Աʹ�ã���������Ȩ*/
            CMS_ASSERT(NULL != pcCuAgent->m_pcUserAccount);
            if ( !pcCuAgent->m_pcUserAccount->IsSuperAdmin() )
            {
                INSTLOG(CUI_APP, ERROR_LEV, "�û�[%s Session: %s]��ѯǰ����ʱ��Ȩ����Ȩ�ޣ�ֻ�г�������Ա����\n", 
                    pcCuAgent->m_strUserName.c_str(), strSession.c_str());

                CCuAddAuthorizationDeviceRsp cRsp;
                cRsp.SetErrorCode(ERR_CUI_CU_NO_PERMISSION);
                SendRspToCu<CCuAddAuthorizationDeviceRsp>(cRsp, ptRecvNode);
                return dwRet;
            }
            CuQryReqToTAS<CCuGetAuthorizationDeviceReq>(pXmlMsgContent, pcCuAgent, ptRecvNode);
        }
        break;
    case TAS_TEMP_AUTHORIZATION_ADD_REQ:
        {
            /*��ʱ��Ȩ�豸ֻ���ɳ�������Աʹ�ã���������Ȩ*/
            CMS_ASSERT(NULL != pcCuAgent->m_pcUserAccount);
            if ( !pcCuAgent->m_pcUserAccount->IsSuperAdmin() )
            {
                INSTLOG(CUI_APP, ERROR_LEV, "�û�[%s Session: %s]���ǰ����ʱ��Ȩ����Ȩ�ޣ�ֻ�г�������Ա����\n", 
                    pcCuAgent->m_strUserName.c_str(), strSession.c_str());
                CCuAddAuthorizationDeviceRsp cRsp;
                cRsp.SetErrorCode(ERR_CUI_CU_NO_PERMISSION);
                SendRspToCu<CCuAddAuthorizationDeviceRsp>(cRsp, ptRecvNode);
                return dwRet;
            }

            CuSingleOperReqToTAS<CCuAddAuthorizationDeviceReq>(pXmlMsgContent, pcCuAgent, ptRecvNode);
        }
        break;
    case TAS_TEMP_AUTHORIZATION_DEL_REQ:
        {
            /*��ʱ��Ȩ�豸ֻ���ɳ�������Աʹ�ã���������Ȩ*/
			CMS_ASSERT(NULL != pcCuAgent->m_pcUserAccount);
			if ( !pcCuAgent->m_pcUserAccount->IsSuperAdmin() )
            {
                INSTLOG(CUI_APP, ERROR_LEV, "�û�[%s Session: %s]ɾ��ǰ����ʱ��Ȩ����Ȩ�ޣ�ֻ�г�������Ա����\n", 
                    pcCuAgent->m_strUserName.c_str(), strSession.c_str());
                CCuAddAuthorizationDeviceRsp cRsp;
                cRsp.SetErrorCode(ERR_CUI_CU_NO_PERMISSION);
                SendRspToCu<CCuAddAuthorizationDeviceRsp>(cRsp, ptRecvNode);
                return dwRet;
            }
            CuSingleOperReqToTAS<CCuDelAuthorizationDeviceReq>(pXmlMsgContent, pcCuAgent, ptRecvNode);
        }
        break;
    case TAS_TEMP_AUTHORIZATION_MOD_REQ:
        {
            /*��ʱ��Ȩ�豸ֻ���ɳ�������Աʹ�ã���������Ȩ*/
			CMS_ASSERT(NULL != pcCuAgent->m_pcUserAccount);
			if ( !pcCuAgent->m_pcUserAccount->IsSuperAdmin() )
            {
                INSTLOG(CUI_APP, ERROR_LEV, "�û�[%s Session: %s]�޸�ǰ����ʱ��Ȩ����Ȩ�ޣ�ֻ�г�������Ա����\n", 
                    pcCuAgent->m_strUserName.c_str(), strSession.c_str());
                CCuAddAuthorizationDeviceRsp cRsp;
                cRsp.SetErrorCode(ERR_CUI_CU_NO_PERMISSION);
                SendRspToCu<CCuAddAuthorizationDeviceRsp>(cRsp, ptRecvNode);
                return dwRet;
            }
            CuSingleOperReqToTAS<CCuModifyAuthorizationDeviceReq>(pXmlMsgContent, pcCuAgent, ptRecvNode);
        }
        break;
    case TAS_DEVICE_MOD_REQ:
        {
            CuSingleOperReqToTAS<CCuDeviceNameModReq>(pXmlMsgContent, pcCuAgent, ptRecvNode);
        }
        break;
        //     case TAS_DEVICE_CAP_INDEX_NAME_SET_REQ:
        //         {
        //             CuSingleOperReqToTAS<CCuDeviceCapIndexNameSetReq>(pXmlMsgContent, pcCuAgent, ptRecvNode);
        //         }
        //         break;
    case TAS_DEVICE_QRY_REQ:
        {
            CuQryReqToTAS<CCuGetDeviceBaseInfoReq>(pXmlMsgContent, pcCuAgent, ptRecvNode);
        }
        break;
	case TAS_DEVICE_MATCH_GBID_REQ:
		{
			CuSingleOperReqToTAS<CCuDeviceMatchGbidReq>(pXmlMsgContent, pcCuAgent, ptRecvNode);
		}
		break;
	case TAS_DEVICE_GROUP_DEVICE_SUMNUM_REQ:
		{
			CuSingleOperReqToTAS<CCuDeviceGrpDevSumNumReq>(pXmlMsgContent, pcCuAgent, ptRecvNode);
		}
		break;
	case TAS_DEVICE_GROUP_DEVICE_MOD_4GB_REQ:
		{
			CuSingleOperReqToTAS<CCuDeviceGroupDeviceMod4GbReq>(pXmlMsgContent, pcCuAgent, ptRecvNode);
		}
		break;
	case TAS_DEVICE_GBID_ERROR_GET_REQ:
		{
			CuSingleOperReqToTAS<CCuDeviceGbidErrGetReq>(pXmlMsgContent, pcCuAgent, ptRecvNode);
		}
		break;
    case CU_CUI_INVITE_PLAY_DOWNLOADPURECORD_REQ:
    case CU_CUI_INVITE_PLAY_PURECORD_REQ:
    case INVITE_REQ:
        {       
            CCuSetupStreamReq cReq;
            cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);

			//����cuiip������vtdu������ѡ��
			cReq.GetSdp().SetInterfaceIp(pcCuAgent->m_tNatInfo.GetIpAddr());

            CInviteInterAction *pcCuInviteTask = new CInviteInterAction(this, pcCuAgent, ptRecvNode);
            pcCuInviteTask->InitStateMachine();
            SetCuReqMsg(cReq, pcCuInviteTask);

        }
        break;
    case CU_CUI_INVITE_ACK_REQ:
        {
            CCuPlayStreamReq cReq;
            cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);

            u32 dwTaskId = INVALID_TASKNO;
            bool bIsError = false;
            do 
            {
				if ( pcCuAgent == NULL )
				{
					bIsError = true;
					break;
				}

                if ( ! pcCuAgent->m_cInviteTaskSet.Find(cReq.GetPlayId(), dwTaskId) )
                {
                    INSTLOG(CUI_APP, ERROR_LEV, "�û�[%s Session: %s](1)ͨ��playID[%d]�Ҳ�����Ӧ��task\n", 
                        pcCuAgent->m_strUserName.c_str(), strSession.c_str(), cReq.GetPlayId());

                    bIsError = true;
                    break;
                }

                if ( INVALID_TASKNO == dwTaskId )
                {
                    INSTLOG(CUI_APP, ERROR_LEV, "�û�[%s Session: %s](2)ͨ��playID[%d]�Ҳ�����Ӧ��task\n", 
                        pcCuAgent->m_strUserName.c_str(), strSession.c_str(), cReq.GetPlayId());

                    bIsError = true;
                    break;
                }

				if (pcCuAgent->CuiTransStream4Pu())
				{
					CInviteTaskNatSwitch* pcInviteTask = (CInviteTaskNatSwitch *)(this->FindTask(dwTaskId));

					if(pcInviteTask == NULL || pcInviteTask->GetState() != CInviteTaskNatSwitch::INVITE_HOLD_STATE)
					{
						INSTLOG(CUI_APP, ERROR_LEV, "�û�[%s Session: %s](2)ͨ��playID[%d]��Ӧ��taskָ��Ϊ[%p]\n", 
							pcCuAgent->m_strUserName.c_str(), strSession.c_str(), cReq.GetPlayId(), pcInviteTask);
						bIsError = true;
						break;
					}

					CCuKeyFrameReq cCuKeyFrameReq;
					cCuKeyFrameReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);
					cCuKeyFrameReq.SetSession(cReq.GetSession());
					cCuKeyFrameReq.SetDevChn(pcInviteTask->GetDevChn());

					CSingleInterAction* pcTask = new CSingleInterAction(this, pcCuAgent, ptRecvNode);
					pcTask->InitStateMachine();
					SetCuReqMsg(cCuKeyFrameReq, pcTask);
					break;
				}
				else
				{
					CInviteTaskEx* pcInviteTask = (CInviteTaskEx *)(this->FindTask(dwTaskId));
					if(pcInviteTask!= NULL && pcInviteTask->GetState() == CInviteTaskEx::INVITE_HOLD_STATE)
					{
						CCuKeyFrameReq cCuKeyFrameReq;
						cCuKeyFrameReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);
						cCuKeyFrameReq.SetSession(cReq.GetSession());
						cCuKeyFrameReq.SetDevChn(pcInviteTask->GetDevChn());

						CSingleInterAction* pcTask = new CSingleInterAction(this, pcCuAgent, ptRecvNode);
						pcTask->InitStateMachine();
						SetCuReqMsg(cCuKeyFrameReq, pcTask);
					}
					else
					{
						bIsError = true;
						break;
					}
				}
            } while (0);

            if(bIsError)
            {
                CCuPlayStreamRsp cRsp;
                cRsp.SetErrorCode(ERR_CUI_TASK_DEL);
                SendRspToCu<CCuPlayStreamRsp>(cRsp, ptRecvNode);
            }
        }
        break;
    case CU_CUI_INVITE_BYE_REQ:
        {
            CCuStopStreamReq cReq;
            cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);

            CInviteInterAction *pcCuInviteTask = new CInviteInterAction(this, pcCuAgent, ptRecvNode);
            pcCuInviteTask->InitStateMachine();
            SetCuReqMsg(cReq, pcCuInviteTask);
        }
        break;
	case PU_TRANSPARENT_QRY_REQ:
		{
			CPuTransparentQryReq cReq;
			cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);

			CNotifyTransferTask *pcNtfTransTask = new CNotifyTransferTask(this,pcCuAgent,ptRecvNode);
			pcNtfTransTask->InitStateMachine();
			SetCuReqMsg(cReq,pcNtfTransTask);
		}
		break;
        /////////////////////////////////////////////////////////////////////////////////////////////
        ///////////////////////////���轻����ص�task/////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////////////////////
    case CU_CUI_INVITE_PLAY_PLATFORMRECORD_REQ:
        {
            CCuSetupPlayPlatformRecordReq cReq;
            cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);

			//����cuiip������vtdu������ѡ��
			cReq.GetReq().GetSdp().SetInterfaceIp(pcCuAgent->m_tNatInfo.GetIpAddr());

			if (pcCuAgent->CuiTransStream4PlatRec())
			{
				CPeInviteInterActionNatSwitch *pcCuInviteTask = new CPeInviteInterActionNatSwitch(this, pcCuAgent->m_strSession, ptRecvNode);

				pcCuInviteTask->InitStateMachine();
				dwRet = SetCuReqMsg(cReq, pcCuInviteTask);
			} 
			else
			{
				CPeInviteInterAction *pcCuInviteTask = new CPeInviteInterAction(this, pcCuAgent->m_strSession, ptRecvNode);

				pcCuInviteTask->InitStateMachine();
				dwRet = SetCuReqMsg(cReq, pcCuInviteTask);
			}
        }
        break;
    case CU_CUI_INVITE_ACK_PLAY_PLATFORMRECORD_REQ:
        {
            CCuPlayPlatformRecordReq cReq;
            cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);

			/*
			����CU��ֻЯ����PlayId������cui.fcgiͳһʹ����PLATFORM���
			ʵ�ʸ���Ϣ����ָǰ��¼��Ҳ����ֻƽ̨¼��
			*/
			CSipTask *pcIvtTask = pcCuAgent->GetInviteTaskPtrByPlayId(cReq.GetPlayId());
			CSipTask *pcPeInviteTask = pcCuAgent->GetPeInviteTaskPtrByPlayId(cReq.GetPlayId());

			if(pcIvtTask != NULL)
			{
				CInviteTaskEx* pcInviteTask = (CInviteTaskEx*)pcIvtTask;
				if(pcInviteTask->GetState() == CInviteTaskEx::INVITE_HOLD_STATE 
					&& pcInviteTask->m_tReq.GetSdp().GetSessionName() == SDP_SESSION_PLAY)
				{
					CCuKeyFrameReq cCuKeyFrameReq;
					cCuKeyFrameReq.SetSession(cReq.GetSession());
					cCuKeyFrameReq.SetDevChn(pcInviteTask->GetDevChn());

					CSingleInterAction* pcTask = new CSingleInterAction(this, pcCuAgent, ptRecvNode);
					pcTask->InitStateMachine();
					SetCuReqMsg(cCuKeyFrameReq, pcTask);
				}
				else
				{
					CCuPlayPlatformRecordRsp cRsp;
					cRsp.SetErrorCode(CMS_SUCCESS);
					SendRspToCu<CCuPlayPlatformRecordRsp>(cRsp, ptRecvNode);
				}
			}
			else if(pcPeInviteTask != NULL)
			{
				CCuTaskStartReq cCuReq;
				cCuReq.SetHeadFromReq(cReq);
				cCuReq.SetSession(cReq.GetSession());
				cCuReq.SetPlayId(cReq.GetPlayId());

				if (pcCuAgent->CuiTransStream4PlatRec() )
				{
					CPeInviteInterActionNatSwitch *pcCuInviteTask = new CPeInviteInterActionNatSwitch(this, pcCuAgent->m_strSession, ptRecvNode);
					pcCuInviteTask->InitStateMachine();
					dwRet = SetCuReqMsg(cCuReq, pcCuInviteTask);
				}
				else
				{
					CPeInviteInterAction *pcCuInviteTask = new CPeInviteInterAction(this, pcCuAgent->m_strSession, ptRecvNode);
					pcCuInviteTask->InitStateMachine();
					dwRet = SetCuReqMsg(cCuReq, pcCuInviteTask);
				}

				break;
			}
			else
			{
				CCuPlayPlatformRecordRsp cRsp;
				cRsp.SetErrorCode(ERR_CUI_TASK_DEL);
				SendRspToCu<CCuPlayPlatformRecordRsp>(cRsp, ptRecvNode);
			}
        }
        break;
    case CU_CUI_INVITE_BYE_PLAY_PLATFORMRECORD_REQ:
        {
            CCuStopPlatformRecordReq cReq;
            cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);

			/*
			����CU��ֻЯ����PlayId������cui.fcgiͳһʹ����PLATFORM���
			ʵ�ʸ���Ϣ����ָǰ��¼��Ҳ����ֻƽ̨¼��
			*/
			CSipTask *pcInviteTask = pcCuAgent->GetInviteTaskPtrByPlayId(cReq.GetPlayId());
			CSipTask *pcPeInviteTask = (CSipTask *)pcCuAgent->GetPeInviteTaskPtrByPlayId(cReq.GetPlayId());

			if(pcInviteTask != NULL)
			{
				CCuStopStreamReq cStopReq;
				cStopReq.SetEvent(CU_CUI_INVITE_STOP_PURECORD_REQ);
				cStopReq.SetPlayId(cReq.GetPlayId());

				CInviteInterAction *pcCuInviteTask = new CInviteInterAction(this, pcCuAgent, ptRecvNode);
				pcCuInviteTask->InitStateMachine();
				SetCuReqMsg(cStopReq, pcCuInviteTask);
			}
			else if(pcPeInviteTask != NULL)
			{
				if (pcCuAgent->CuiTransStream4PlatRec() )
				{
					CPeInviteInterActionNatSwitch *pcCuInviteTask = new CPeInviteInterActionNatSwitch(this, pcCuAgent->m_strSession, ptRecvNode);
					pcCuInviteTask->InitStateMachine();
					dwRet = SetCuReqMsg(cReq, pcCuInviteTask);
				}
				else
				{
					CPeInviteInterAction *pcCuInviteTask = new CPeInviteInterAction(this, pcCuAgent->m_strSession, ptRecvNode);
					pcCuInviteTask->InitStateMachine();
					dwRet = SetCuReqMsg(cReq, pcCuInviteTask);
				}
			}
			else
			{
				CCuStopPlatformRecordRsp cRsp;
				cRsp.SetErrorCode(ERR_CUI_TASK_DEL);
				SendRspToCu<CCuStopPlatformRecordRsp>(cRsp, ptRecvNode);
			}
        }
        break;
    case CU_CUI_INVITE_PLAY_DOWNLOADPLATFORMRECORD_REQ:
        {
            CCuDownloadPlatformRecordReq cReq;
            cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);

			//����cuiip������vtdu������ѡ��
			cReq.GetReq().GetSdp().SetInterfaceIp(pcCuAgent->m_tNatInfo.GetIpAddr());

			if (pcCuAgent->CuiTransStream4PlatRec() )
			{
				CPeInviteInterActionNatSwitch *pcCuInviteTask = new CPeInviteInterActionNatSwitch(this, pcCuAgent->m_strSession, ptRecvNode);
				pcCuInviteTask->InitStateMachine();
				dwRet = SetCuReqMsg(cReq, pcCuInviteTask);
			}
			else
			{
				CPeInviteInterAction *pcCuInviteTask = new CPeInviteInterAction(this, pcCuAgent->m_strSession, ptRecvNode);
				pcCuInviteTask->InitStateMachine();
				dwRet = SetCuReqMsg(cReq, pcCuInviteTask);
			}
        }
        break;
    case CU_CUI_INVITE_ACK_DOWNLOADPLATFORMRECORD_REQ:
        {
            CCuStartDownloadPlatformRecordReq cReq;
            cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);

			/*
			����CU��ֻЯ����PlayId������cui.fcgiͳһʹ����PLATFORM���
			ʵ�ʸ���Ϣ����ָǰ��¼��Ҳ����ֻƽ̨¼��
			*/
			CSipTask *pcInviteTask = pcCuAgent->GetInviteTaskPtrByPlayId(cReq.GetPlayId());
			CSipTask *pcPeInviteTask = pcCuAgent->GetPeInviteTaskPtrByPlayId(cReq.GetPlayId());

			if(pcInviteTask != NULL)
			{
				CCuStartDownloadPlatformRecordRsp cRsp;
				cRsp.SetErrorCode(CMS_SUCCESS);
				SendRspToCu<CCuStartDownloadPlatformRecordRsp>(cRsp, ptRecvNode);
			}
            else if(pcPeInviteTask != NULL)
			{
				CCuTaskStartReq cCuReq;
				cCuReq.SetHeadFromReq(cReq);
				cCuReq.SetPlayId(cReq.GetPlayId());

				if (pcCuAgent->CuiTransStream4PlatRec() )
				{
					CPeInviteInterActionNatSwitch *pcCuInviteTask = new CPeInviteInterActionNatSwitch(this, pcCuAgent->m_strSession, ptRecvNode);
					pcCuInviteTask->InitStateMachine();
					dwRet = SetCuReqMsg(cCuReq, pcCuInviteTask);
				}
				else
				{
					CPeInviteInterAction *pcCuInviteTask = new CPeInviteInterAction(this, pcCuAgent->m_strSession, ptRecvNode);
					pcCuInviteTask->InitStateMachine();
					dwRet = SetCuReqMsg(cCuReq, pcCuInviteTask);
				}
            }
            else
            {
                CCuStartDownloadPlatformRecordRsp cRsp;
                cRsp.SetErrorCode(ERR_CUI_TASK_DEL);
                SendRspToCu<CCuStartDownloadPlatformRecordRsp>(cRsp, ptRecvNode);
            }
        }
        break;
    case CU_CUI_INVITE_BYE_DOWNLOADPLATFORMRECORD_REQ:
        {
            CCuStopDownloadPlatformRecordReq cReq;
            cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);

			/*
			����CU��ֻЯ����PlayId������cui.fcgiͳһʹ����PLATFORM���
			ʵ�ʸ���Ϣ����ָǰ��¼��Ҳ����ֻƽ̨¼��
			*/
			CSipTask *pcInviteTask = pcCuAgent->GetInviteTaskPtrByPlayId(cReq.GetPlayId());
			CSipTask *pcPeInviteTask = pcCuAgent->GetPeInviteTaskPtrByPlayId(cReq.GetPlayId());

			if(pcInviteTask != NULL)
			{
				CCuStopStreamReq cStopReq;
				cStopReq.SetEvent(CU_CUI_INVITE_STOP_DOWNLOADPURECORD_REQ);
				cStopReq.SetPlayId(cReq.GetPlayId());

				CInviteInterAction *pcCuInviteTask = new CInviteInterAction(this, pcCuAgent, ptRecvNode);
				pcCuInviteTask->InitStateMachine();
				SetCuReqMsg(cStopReq, pcCuInviteTask);
			}
			else if(pcPeInviteTask != NULL)
			{
				if (pcCuAgent->CuiTransStream4PlatRec() )
				{
					CPeInviteInterActionNatSwitch *pcCuInviteTask = new CPeInviteInterActionNatSwitch(this, pcCuAgent->m_strSession, ptRecvNode);
					pcCuInviteTask->InitStateMachine();
					dwRet = SetCuReqMsg(cReq, pcCuInviteTask);
				}
				else
				{
					CPeInviteInterAction *pcCuInviteTask = new CPeInviteInterAction(this, pcCuAgent->m_strSession, ptRecvNode);
					pcCuInviteTask->InitStateMachine();
					dwRet = SetCuReqMsg(cReq, pcCuInviteTask);
				}
			}
			else
			{
				CCuPlayStreamRsp cRsp;
				cRsp.SetErrorCode(ERR_CUI_TASK_DEL);
				SendRspToCu<CCuPlayStreamRsp>(cRsp, ptRecvNode);
			}
        }
        break;
    case FILE_EXPORT_REQ:
        {
            CCuFileExportReq cReq;
            cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);
            if (cReq.GetSrvName() != FILE_EXPORT_CUSER_DEVICEDATA)
            {
                CExportTaskInterAction *pcTask = new CExportTaskInterAction(this, pcCuAgent->m_strSession, ptRecvNode);
                pcTask->InitStateMachine();
                dwRet = SetCuReqMsg(cReq, pcTask);
            }
            else
            {
                CExportTask4TasInterAction *pcTask = new CExportTask4TasInterAction(this, pcCuAgent->m_strSession, ptRecvNode);
                pcTask->InitStateMachine();
                dwRet = SetCuReqMsg(cReq, pcTask);
            }
        }
        break;
    case CU_CUI_STOP_EXPORT_REQ:
        {
            CCuStopExportReq cReq;
            cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);

            if (!cReq.GetExportFileUrl().empty())
            {
                CExportTaskInterAction *pcTask = new CExportTaskInterAction(this, pcCuAgent->m_strSession, ptRecvNode);
                pcTask->InitStateMachine();
                dwRet = SetCuReqMsg(cReq, pcTask);
            }
            else
            {
                CExportTask4TasInterAction *pcTask = new CExportTask4TasInterAction(this, pcCuAgent->m_strSession, ptRecvNode);
                pcTask->InitStateMachine();
                dwRet = SetCuReqMsg(cReq, pcTask);
            }
        }
		break;
	case CU_CUI_FILE_EXPORT_START_EX_REQ:
		{
			CCuFileExportStartExReq cReq;
			cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);
			CExportTaskExInterAction *pcTaskEx = new CExportTaskExInterAction(this, pcCuAgent->m_strSession, ptRecvNode);
			pcTaskEx->InitStateMachine();
			dwRet = SetCuReqMsg(cReq, pcTaskEx);
		}
		break;
	case CU_CUI_FILE_EXPORT_GET_PROCESS_EX_REQ:
		{
			CCuFileExportGetProcessExReq cReq;
			cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);

			u32 taskNo = cReq.GetTaskNo();
			CTask *pTask = this->FindTask(taskNo);
			if ( pTask != NULL )
			{
				// Ϊ�˲�Ӱ������ʵ�֣���У��һ��
				if ( cReq.GetTaskType() == DEF_TASK_TYPE_FILE_EXPORT_TASK_EX )
				{
					CExportTaskExInterAction *pcTask = new CExportTaskExInterAction(this, pcCuAgent->m_strSession, ptRecvNode);
					pcTask->InitStateMachine();
					dwRet = SetCuReqMsg(cReq, pcTask);
				}
				else if ( cReq.GetTaskType() == DEF_TASK_TYPE_DPS_SEARCH_TASK )
				{
					// �������Ȳ�ѯ�ͷ���task��;
					CDpsSearchInterAction *pcTask = new CDpsSearchInterAction(this, pcCuAgent->m_strSession, ptRecvNode);
					pcTask->InitStateMachine();
					dwRet = SetCuReqMsg(cReq, pcTask);
				}
				else
				{
					CCuFileExportGetProcessExRsp cRsp;
					cRsp.SetHeadFromReq(cReq);
					cRsp.SetErrorCode(ERR_CUI_UNKNOW_MSG);
					SendRspToCu<CCuFileExportGetProcessExRsp>(cRsp, ptRecvNode);
				}
			}
			else
			{
				CCuFileExportGetProcessExRsp cRsp;
				cRsp.SetErrorCode(ERR_CUI_TASK_DEL);
				SendRspToCu<CCuFileExportGetProcessExRsp>(cRsp, ptRecvNode);
			}

		}
		break;
	case CU_CUI_FILE_EXPORT_STOP_EX_REQ:
		{
			CCuFileExportStopExReq cReq;
			cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);

			u32 taskNo = cReq.GetTaskNo();
			CTask *pTask = this->FindTask(taskNo);
			if ( pTask != NULL )
			{
				CExportTaskExInterAction *pcTask = new CExportTaskExInterAction(this, pcCuAgent->m_strSession, ptRecvNode);
				pcTask->InitStateMachine();
				dwRet = SetCuReqMsg(cReq, pcTask);
			}
			else
			{
				CCuFileExportStopExRsp cRsp;
				cRsp.SetErrorCode(ERR_CUI_TASK_DEL);
				SendRspToCu<CCuFileExportStopExRsp>(cRsp, ptRecvNode);
			}

		}
		break;
	case CU_CUI_FILE_IMPORT_START_EX_REQ:
		{
			CCuFileImportStartExReq cReq;
			cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);

			CImportTaskExInterAction *pcTask = new CImportTaskExInterAction(this, pcCuAgent->m_strSession, ptRecvNode);
			pcTask->InitStateMachine();

			dwRet = SetCuReqMsg(cReq, pcTask);
		}
		break;
	case CU_CUI_FILE_IMPORT_SET_PROCESS_EX_REQ:
		{
			CCuFileImportSetProcessExReq cReq;
			cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);

			u32 nTaskNo = cReq.GetTaskNo();
			CTask *pcTask = this->FindTask(nTaskNo);
			if (pcTask != NULL)
			{
				CImportTaskExInterAction *pcTaskAct = new CImportTaskExInterAction(this, pcCuAgent->m_strSession, ptRecvNode);
				pcTaskAct->InitStateMachine();
				dwRet = SetCuReqMsg(cReq, pcTaskAct);
			}
			else
			{
				CCuFileImportSetProcessExRsp cRsp;
				cRsp.SetErrorCode(ERR_CUI_TASK_DEL);
				SendRspToCu(cRsp, ptRecvNode);
			}
		}
		break;
	case CU_CUI_FILE_IMPORT_STOP_EX_REQ:
		{
			CCuFileImportStopExReq cReq;
			cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);

			u32 nTaskNo = cReq.GetTaskNo();
			CTask *pcTask = this->FindTask(nTaskNo);
			if (pcTask != NULL)
			{
				CImportTaskExInterAction *pcTaskAct = new CImportTaskExInterAction(this, pcCuAgent->m_strSession, ptRecvNode);
				pcTaskAct->InitStateMachine();
				dwRet = SetCuReqMsg(cReq, pcTaskAct);
			}
			else
			{
				CCuFileImportStopExRsp cRsp;
				cRsp.SetErrorCode(ERR_CUI_TASK_DEL);
				SendRspToCu(cRsp, ptRecvNode);
			}
		}
		break;
	case CU_CUI_DPS_SEARCH_REQ:
		{
			CCuDPSSearchReq cReq;
			cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);
			
			CDpsSearchInterAction *pcTask = new CDpsSearchInterAction(this, pcCuAgent->m_strSession, ptRecvNode);
			if ( pcTask == NULL)
			{
				CCuDPSSearchRsp cRsp;
				cRsp.SetErrorCode(ERR_CUI_TASK_DEL);
				SendRspToCu(cRsp, ptRecvNode);
			}
			else
			{
				pcTask->InitStateMachine();
				dwRet = SetCuReqMsg(cReq, pcTask);
			}
		}
		break;
	case CU_CUI_DPS_GET_SEARCH_RESULT_REQ:
		{
			CCuDPSGetResultReq cReq;
			cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);

			u32 nTaskNo = cReq.GetTaskNo();
			CTask *pcTask = this->FindTask(nTaskNo);
			if (pcTask != NULL)
			{
				CDpsSearchInterAction *pcTaskAct = new CDpsSearchInterAction(this, pcCuAgent->m_strSession, ptRecvNode);
				pcTaskAct->InitStateMachine();
				dwRet = SetCuReqMsg(cReq, pcTaskAct);
			}
			else
			{
				CCuDPSGetResultRsp cRsp;
				cRsp.SetErrorCode(ERR_CUI_TASK_DEL);
				SendRspToCu(cRsp, ptRecvNode);
			}
		}
		break;
	case CU_CUI_DPS_GET_SEARCH_RESULT_BY_GROUP_REQ:
		{
			CCuDPSGetResultByGroupReq cReq;
			cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);

			u32 nTaskNo = cReq.GetTaskNo();
			CTask *pcTask = this->FindTask(nTaskNo);
			if (pcTask != NULL)
			{
				CDpsSearchInterAction *pcTaskAct = new CDpsSearchInterAction(this, pcCuAgent->m_strSession, ptRecvNode);
				pcTaskAct->InitStateMachine();
				dwRet = SetCuReqMsg(cReq, pcTaskAct);
			}
			else
			{
				CCuDPSGetResultByGroupRsp cRsp;
				cRsp.SetErrorCode(ERR_CUI_TASK_DEL);
				SendRspToCu(cRsp, ptRecvNode);
			}
		}
		break;
	case CU_CUI_DPS_SEARCH_STOP_REQ:
		{
			CCuDPSSearchStopReq cReq;
			cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);

			u32 nTaskNo = cReq.GetTaskNo();
			CTask *pcTask = this->FindTask(nTaskNo);
			if (pcTask != NULL)
			{
				CDpsSearchInterAction *pcTaskAct = new CDpsSearchInterAction(this, pcCuAgent->m_strSession, ptRecvNode);
				pcTaskAct->InitStateMachine();
				dwRet = SetCuReqMsg(cReq, pcTaskAct);
			}
			else
			{
				CCuDPSSearchStopRsp cRsp;
				cRsp.SetErrorCode(ERR_CUI_TASK_DEL);
				SendRspToCu(cRsp, ptRecvNode);
			}
		}
		break;
        //һ���Խ�����task///////////////��ʱ������cmu��ʽ������/////////////////////////////////////////////////////////////////////////////////
	case DPS_GET_DEVICE_STATUS_REQ:
		{
			string strPeUri = string(DEV_TYPE_DPS) + "@" + GetDomainName();
			CuOperReqToPe<CDPSGetDeviceStatusReq, CPeSingleInterAction>(pXmlMsgContent, pcCuAgent->m_strSession, strPeUri, ptRecvNode);
		}
		break;
    case RCS_GET_SYSPARAM_REQ:
        {  
            TSipURI tRcsUri;
            CStreamDecentrationAdapter::FindRcsUri( tRcsUri, GetDomainName() );
            CuOperReqToPe<CCuRcsGetSysParamReq, CPeSingleInterAction>(pXmlMsgContent, pcCuAgent->m_strSession, tRcsUri.GetURIString(), ptRecvNode);
        }
        break;
    case RCS_SET_SYSPARAM_REQ:
        {
            TSipURI tRcsUri;
            CStreamDecentrationAdapter::FindRcsUri( tRcsUri, GetDomainName() );
            CuOperReqToPe<CCuRcsSetSysParamReq, CPeSingleInterAction>(pXmlMsgContent, pcCuAgent->m_strSession, tRcsUri.GetURIString() , ptRecvNode);
        }
        break;
    case RCS_BINDING_PUCHN_REQ:
        {
            TSipURI tRcsUri;
            CStreamDecentrationAdapter::FindRcsUri( tRcsUri, GetDomainName() );
            CuOperReqToPe<CCuRcsBindingPuChnReq, CPeSingleInterAction>(pXmlMsgContent, pcCuAgent->m_strSession, tRcsUri.GetURIString(), ptRecvNode);
        }
        break;
    case RCS_UNBINDING_PUCHN_REQ:
        {
            TSipURI tRcsUri;
            CStreamDecentrationAdapter::FindRcsUri( tRcsUri, GetDomainName() );
            CuOperReqToPe<CCuRcsUnbindingPuChnReq, CPeSingleInterAction>(pXmlMsgContent, pcCuAgent->m_strSession, tRcsUri.GetURIString(), ptRecvNode);
        }
        break;
    case PU_REC_DAYS_QRY_REQ:
        {
            CCuPuRecDaysQryReq cReq;
            cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);

            CSingleInterAction* pcTask = new CSingleInterAction(this, pcCuAgent, ptRecvNode);
            pcTask->InitStateMachine();
            SetCuReqMsg(cReq, pcTask);
        }
        break;
    case RCS_QUERY_RECORD_DAYS_REQ:
        {
            CCuRcsQueryRecordDaysReq cCuRcsQueryRecordDaysReq;
            cCuRcsQueryRecordDaysReq.FromXml(pXmlMsgContent);

            TSipURI tPeUri;
            CStreamDecentrationAdapter::FindRcsUri( tPeUri, cCuRcsQueryRecordDaysReq.GetDomainName() );
            //CRcsQueryRecordDaysReq cReq = cCuRcsQueryRecordDaysReq.GetReq();
            string strReq = cCuRcsQueryRecordDaysReq.GetReq();
            CRcsQueryRecordDaysReq cReq;
            cReq.FromXml(strReq);

            CPeSingleInterAction* pcTask = new CPeSingleInterAction(this, pcCuAgent->m_strSession, tPeUri.GetURIString(), ptRecvNode);
            pcTask->InitStateMachine();
            dwRet = SetCuReqMsg(cReq, pcTask);
            if (dwRet == PROCMSG_DEL)
            {
                INSTLOG(CUI_APP, ALL_LEV, "����[%s-%u]�������Ҫɾ��---msg[%s-%u]\n", 
                    pcTask->GetObjName(), pcTask->GetTaskNO(),
                    cReq.GetEventStr().c_str(), cReq.GetEventId());

                delete pcTask;
                pcTask = NULL;
            }
            else if (dwRet == PROCMSG_FAIL)
            {
                INSTLOG(CUI_APP, ERROR_LEV, "����[%s-%u]����ʧ��---msg[%s-%u]\n", 
                    pcTask->GetObjName(), pcTask->GetTaskNO(),
                    cReq.GetEventStr().c_str(), cReq.GetEventId());
            }
            else if (dwRet == PROCMSG_OK)
            {
                INSTLOG(CUI_APP, ALL_LEV, "����[%s-%u]����ɹ�---msg[%s-%u]\n", 
                    pcTask->GetObjName(), pcTask->GetTaskNO(),
                    cReq.GetEventStr().c_str(), cReq.GetEventId());
            }
            else
            {
                INSTLOG(CUI_APP, ERROR_LEV, "����[%s-%u]������δ֪---msg[%s-%u]\n", 
                    pcTask->GetObjName(), pcTask->GetTaskNO(),
                    cReq.GetEventStr().c_str(), cReq.GetEventId());
            }
        }
        break;
    case RCS_QUERY_RECORD_REQ:
        {
            CCuRcsQueryRecordReq cCuRcsQueryRecordReq;
            cCuRcsQueryRecordReq.FromXml(pXmlMsgContent);

            CRcsQueryRecordReq cReq;
            cReq.FromXml(cCuRcsQueryRecordReq.GetReq());


            TSipURI tPeUri;
            CStreamDecentrationAdapter::FindRcsUri( tPeUri, cCuRcsQueryRecordReq.GetDomainName(), false );

            CPeQueryTask* pcTask = new CPeQueryTask(this, pcCuAgent->m_strSession, tPeUri.GetURIString(), ptRecvNode);
            pcTask->InitStateMachine();
            dwRet = SetCuReqMsg(cReq, pcTask);
            if (dwRet == PROCMSG_DEL)
            {
                INSTLOG(CUI_APP, ALL_LEV, "����[%s-%u]�������Ҫɾ��---msg[%s-%u]\n", 
                    pcTask->GetObjName(), pcTask->GetTaskNO(),
                    cReq.GetEventStr().c_str(), cReq.GetEventId());

                delete pcTask;
                pcTask = NULL;
            }
            else if (dwRet == PROCMSG_FAIL)
            {
                INSTLOG(CUI_APP, ERROR_LEV, "����[%s-%u]����ʧ��---msg[%s-%u]\n", 
                    pcTask->GetObjName(), pcTask->GetTaskNO(),
                    cReq.GetEventStr().c_str(), cReq.GetEventId());
            }
            else if (dwRet == PROCMSG_OK)
            {
                INSTLOG(CUI_APP, ALL_LEV, "����[%s-%u]����ɹ�---msg[%s-%u]\n", 
                    pcTask->GetObjName(), pcTask->GetTaskNO(),
                    cReq.GetEventStr().c_str(), cReq.GetEventId());
            }
            else
            {
                INSTLOG(CUI_APP, ERROR_LEV, "����[%s-%u]������δ֪---msg[%s-%u]\n", 
                    pcTask->GetObjName(), pcTask->GetTaskNO(),
                    cReq.GetEventStr().c_str(), cReq.GetEventId());
            }
        }
        break;
    case RCS_SET_RECORD_PARAM_REQ:
        {
            TSipURI tPeUri;
            CStreamDecentrationAdapter::FindRcsUri( tPeUri, GetDomainName() );
            CuOperReqToPe<CCuRcsSetRecordParamReq, CPeSingleInterAction>(pXmlMsgContent, pcCuAgent->m_strSession, tPeUri.GetURIString(), ptRecvNode);
        }
        break;
    case TV_POLL_OPERATE_REQ:
        {
            string strPeUri = string( DEV_TYPE_TVS ) + "@" + GetDomainName();
            CuOperReqToPe<CCuTvPollOperateReq, CPeSingleInterAction>(pXmlMsgContent, pcCuAgent->m_strSession, strPeUri, ptRecvNode);
        }
        break;
    case TVWALL_SCHEME_SAVE_REQ:
        {
            string strPeUri = string( DEV_TYPE_TVS ) + "@" + GetDomainName();
            CuOperReqToPe<CCuTvWallSchemeSaveReq, CPeSingleInterAction>(pXmlMsgContent, pcCuAgent->m_strSession, strPeUri, ptRecvNode);
        }
        break;
    case TVWALL_SCHEME_DEL_REQ:
        {
            string strPeUri = string( DEV_TYPE_TVS ) + "@" + GetDomainName();
            CuOperReqToPe<CCuTvWallSchemeDelReq, CPeSingleInterAction>(pXmlMsgContent, pcCuAgent->m_strSession, strPeUri, ptRecvNode);
        }
        break;
    case TVWALL_SCHEME_LOAD_REQ:
        {
            string strPeUri = string( DEV_TYPE_TVS ) + "@" + GetDomainName();
            CuOperReqToPe<CCuTvWallSchemeLoadReq, CPeSingleInterAction>(pXmlMsgContent, pcCuAgent->m_strSession, strPeUri, ptRecvNode);
        }
        break;
    case TVWALL_SCHEME_GROUP_SET_REQ:
        {
            string strPeUri = string( DEV_TYPE_TVS ) + "@" + GetDomainName();
            CuOperReqToPe<CCuTvWallSchemeGroupSetReq, CPeSingleInterAction>(pXmlMsgContent, pcCuAgent->m_strSession, strPeUri, ptRecvNode);         
        }
        break;
    case TVWALL_SCHEME_GROUP_GET_REQ:
        {
            string strPeUri = string( DEV_TYPE_TVS ) + "@" + GetDomainName();
            CuOperReqToPe<CCuTvWallSchemeGroupGetReq, CPeSingleInterAction>(pXmlMsgContent, pcCuAgent->m_strSession, strPeUri, ptRecvNode); 
        }
        break;
    case TVWALL_SCHEME_POLL_OPERATE_REQ:
        {
            string strPeUri = string( DEV_TYPE_TVS ) + "@" + GetDomainName();
            CuOperReqToPe<CCuTvWallSchemePollOperateReq, CPeSingleInterAction>(pXmlMsgContent, pcCuAgent->m_strSession, strPeUri, ptRecvNode); 
        }
        break;
    case MPS_MAP_CREATE_REQ:
        {
            string strPeUri = string( DEV_TYPE_MPS ) + "@" + GetDomainName();
            CuOperReqToPe<CCuMapCreateReq, CPeSingleInterAction>(pXmlMsgContent, pcCuAgent->m_strSession, strPeUri, ptRecvNode);
        }
        break;
    case MPS_MAP_MOD_REQ:
        {
            string strPeUri = string( DEV_TYPE_MPS ) + "@" + GetDomainName();
            CuOperReqToPe<CCuMapModReq, CPeSingleInterAction>(pXmlMsgContent, pcCuAgent->m_strSession, strPeUri, ptRecvNode);
        }
        break;
    case MPS_MAP_DEL_REQ:
        {
            string strPeUri = string( DEV_TYPE_MPS ) + "@" + GetDomainName();
            CuOperReqToPe<CCuMapDelReq, CPeSingleInterAction>(pXmlMsgContent, pcCuAgent->m_strSession, strPeUri, ptRecvNode);
        }
        break;
    case MPS_MAP_ELEMENT_CREATE_REQ:
        {
            string strPeUri = string( DEV_TYPE_MPS ) + "@" + GetDomainName();
            CuOperReqToPe<CCuMapElementCreateReq, CPeSingleInterAction>(pXmlMsgContent, pcCuAgent->m_strSession, strPeUri, ptRecvNode);
        }
        break;
    case MPS_MAP_ELEMENT_MOD_REQ:
        {
            string strPeUri = string( DEV_TYPE_MPS ) + "@" + GetDomainName();
            CuOperReqToPe<CCuMapElementModReq, CPeSingleInterAction>(pXmlMsgContent, pcCuAgent->m_strSession, strPeUri, ptRecvNode);
        }
        break;
    case MPS_MAP_ELEMENT_DEL_REQ:
        {
            string strPeUri = string( DEV_TYPE_MPS ) + "@" + GetDomainName();
            CuOperReqToPe<CCuMapElementDelReq, CPeSingleInterAction>(pXmlMsgContent, pcCuAgent->m_strSession, strPeUri, ptRecvNode);
        }
        break;
    case MPS_POS_CORRECT_MARS_TO_EARTH_REQ:
        {
            string strPeUri = string( DEV_TYPE_MPS ) + "@" + GetDomainName();
            CuOperReqToPe<CCuPosCorrectMarsToEarthReq, CPeSingleInterAction>(pXmlMsgContent, pcCuAgent->m_strSession, strPeUri, ptRecvNode);
        }
        break;
    case MPS_POS_CORRECT_EARTH_TO_MARS_REQ:
        {
            string strPeUri = string( DEV_TYPE_MPS ) + "@" + GetDomainName();
            CuOperReqToPe<CCuPosCorrectEarthToMarsReq, CPeSingleInterAction>(pXmlMsgContent, pcCuAgent->m_strSession, strPeUri, ptRecvNode);
        }
        break;
    case MPS_DATA_VERSION_GET_REQ:
        {
            string strPeUri = string( DEV_TYPE_MPS ) + "@" + GetDomainName();
            CuOperReqToPe<CCuMpsDataVersionGetReq, CPeSingleInterAction>(pXmlMsgContent, pcCuAgent->m_strSession, strPeUri, ptRecvNode);
        }
        break;
	case MPS_GPS_DATA_BAKEUP_REQ:
		{
			string strPeUri = string( DEV_TYPE_MPS ) + "@" + GetDomainName();
			CuOperReqToPe<CCuGpsDataBakeUpReq, CPeSingleInterAction>(pXmlMsgContent, pcCuAgent->m_strSession, strPeUri, ptRecvNode);
		}
		break;
	case MPS_GPS_STORE_CONFIG_GET_REQ:
		{
			string strPeUri = string( DEV_TYPE_MPS ) + "@" + GetDomainName();
			CuOperReqToPe<CCuMpsGpsStoreCfgGetReq,CPeSingleInterAction>(pXmlMsgContent, pcCuAgent->m_strSession, strPeUri, ptRecvNode);
		}
		break;
    case MPS_MAP_UPLOAD_REQ:
        {
            string strPeUri = string( DEV_TYPE_MPS ) + "@" + GetDomainName();
            u32 dwRet = PROCMSG_OK;
            CMS_ASSERT(NULL != pXmlMsgContent);
            CCuMapUploadReq cReq;
            cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);
            cReq.SetClientNatInfo(pcCuAgent->m_tNatInfo);
            CPeSingleInterAction* pcTask = new CPeSingleInterAction(this, strSession, strPeUri, ptRecvNode);
            pcTask->InitStateMachine();
            dwRet = SetCuReqMsg(cReq, pcTask);
            if (dwRet == PROCMSG_DEL)
            {
                INSTLOG(CUI_APP, ALL_LEV, "����[%s-%u]�������Ҫɾ��---msg[%s-%u]\n", 
                    pcTask->GetObjName(), pcTask->GetTaskNO(),
                    cReq.GetEventStr().c_str(), cReq.GetEventId());
                delete pcTask;
                pcTask = NULL;
            }
            else if (dwRet == PROCMSG_FAIL)
            {
                INSTLOG(CUI_APP, ERROR_LEV, "����[%s-%u]����ʧ��---msg[%s-%u]\n", 
                    pcTask->GetObjName(), pcTask->GetTaskNO(),
                    cReq.GetEventStr().c_str(), cReq.GetEventId());
            }
            else if (dwRet == PROCMSG_OK)
            {
                INSTLOG(CUI_APP, ALL_LEV, "����[%s-%u]����ɹ�---msg[%s-%u]\n", 
                    pcTask->GetObjName(), pcTask->GetTaskNO(),
                    cReq.GetEventStr().c_str(), cReq.GetEventId());
            }
            else
            {
                INSTLOG(CUI_APP, ERROR_LEV, "����[%s-%u]������δ֪---msg[%s-%u]\n", 
                    pcTask->GetObjName(), pcTask->GetTaskNO(),
                    cReq.GetEventStr().c_str(), cReq.GetEventId());
            }
        }
        break;
    case MPS_ELEMENT_ICON_UPLOAD_REQ:
        {
            string strPeUri = string( DEV_TYPE_MPS ) + "@" + GetDomainName();
            u32 dwRet = PROCMSG_OK;
            CMS_ASSERT(NULL != pXmlMsgContent);

            CCuElementIconUploadReq cReq;
            cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);
            cReq.SetClientNatInfo(pcCuAgent->m_tNatInfo);

            CPeSingleInterAction* pcTask = new CPeSingleInterAction(this, strSession, strPeUri, ptRecvNode);
            pcTask->InitStateMachine();
            dwRet = SetCuReqMsg(cReq, pcTask);
            if (dwRet == PROCMSG_DEL)
            {
                INSTLOG(CUI_APP, ALL_LEV, "����[%s-%u]�������Ҫɾ��---msg[%s-%u]\n", 
                    pcTask->GetObjName(), pcTask->GetTaskNO(),
                    cReq.GetEventStr().c_str(), cReq.GetEventId());

                delete pcTask;
                pcTask = NULL;
            }
            else if (dwRet == PROCMSG_FAIL)
            {
                INSTLOG(CUI_APP, ERROR_LEV, "����[%s-%u]����ʧ��---msg[%s-%u]\n", 
                    pcTask->GetObjName(), pcTask->GetTaskNO(),
                    cReq.GetEventStr().c_str(), cReq.GetEventId());
            }
            else if (dwRet == PROCMSG_OK)
            {
                INSTLOG(CUI_APP, ALL_LEV, "����[%s-%u]����ɹ�---msg[%s-%u]\n", 
                    pcTask->GetObjName(), pcTask->GetTaskNO(),
                    cReq.GetEventStr().c_str(), cReq.GetEventId());
            }
            else
            {
                INSTLOG(CUI_APP, ERROR_LEV, "����[%s-%u]������δ֪---msg[%s-%u]\n", 
                    pcTask->GetObjName(), pcTask->GetTaskNO(),
                    cReq.GetEventStr().c_str(), cReq.GetEventId());
            }

            //string strPeUri = string( DEV_TYPE_MPS ) + "@" + GetDomainName();
            //CuOperReqToPe<CCuElementIconUploadReq, CPeSingleInterAction>(pXmlMsgContent, pcCuAgent->m_strSession, strPeUri, ptRecvNode);
        }
        break;
    case ALARMLINK_CFG_GET_REQ:
        {
            string strPeUri = string( DEV_TYPE_ALS ) + "@" + GetDomainName();
            CuOperReqToPe<CCuAlarmLinkCfgGetReq, CPeSingleInterAction>(pXmlMsgContent, pcCuAgent->m_strSession, strPeUri, ptRecvNode);
        }
        break;
    case ALARMLINK_CFG_SET_REQ:
        {
            string strPeUri = string( DEV_TYPE_ALS ) + "@" + GetDomainName();
            CuOperReqToPe<CCuAlarmLinkCfgSetReq, CPeSingleInterAction>(pXmlMsgContent, pcCuAgent->m_strSession, strPeUri, ptRecvNode);
        }
        break;
    case UAS_CUSER_CUSTOMDATA_ADD_REQ:
        {
            string strPeUri = string( DEV_TYPE_UAS ) + "@" + GetDomainName();
            CuOperReqToPe<CCuUserCustomDataAddReq, CPeSingleInterAction>(pXmlMsgContent, pcCuAgent->m_strSession, strPeUri, ptRecvNode);
        }
        break;
    case UAS_CUSER_CUSTOMDATA_DEL_REQ:
        {
            string strPeUri = string( DEV_TYPE_UAS ) + "@" + GetDomainName();
            CuOperReqToPe<CCuUserCustomDataDelReq, CPeSingleInterAction>(pXmlMsgContent, pcCuAgent->m_strSession, strPeUri, ptRecvNode);
        }
        break;
    case UAS_CUSER_CUSTOMDATA_QRY_REQ:
        {
            string strPeUri = string( DEV_TYPE_UAS ) + "@" + GetDomainName();
            CuOperReqToPe<CCuUserCustomDataQryReq, CPeQueryTask>(pXmlMsgContent, pcCuAgent->m_strSession, strPeUri, ptRecvNode);
        }
        break;
    case UAS_CONFIG_UAS_GET_REQ:
        {
            string strPeUri = string( DEV_TYPE_UAS ) + "@" + GetDomainName();
            CuOperReqToPe<CCuConfigUasGetReq, CPeSingleInterAction>(pXmlMsgContent, pcCuAgent->m_strSession, strPeUri, ptRecvNode);
        }
        break;
    case UAS_CONFIG_UAS_SET_REQ:
        {
            string strPeUri = string( DEV_TYPE_UAS ) + "@" + GetDomainName();
            CuOperReqToPe<CCuConfigUasSetReq, CPeSingleInterAction>(pXmlMsgContent, pcCuAgent->m_strSession, strPeUri, ptRecvNode);
        }
        break;
	case GBS_GBDOMAIN_PARENT_PRIVILEGE_GET_REQ: 
		{
			string strPeUri = string(DEV_TYPE_GBS) + "@" + GetDomainName();
			CuOperReqToPe<CCuGbsGbdomainParentPrivilegeGetReq,CPeSingleInterAction>(pXmlMsgContent,pcCuAgent->m_strSession,strPeUri,ptRecvNode);
		}
		break;
	case GBS_GBDOMAIN_PARENT_PRIVILEGE_SET_REQ: 
		{
			string strPeUri = string(DEV_TYPE_GBS) + "@" + GetDomainName();
			CuOperReqToPe<CCuGbsGbdomainParentPrivilegeSetReq,CPeSingleInterAction>(pXmlMsgContent,pcCuAgent->m_strSession,strPeUri,ptRecvNode);
		}
		break;
	case GBS_OPTION_CONF_GET_REQ:
		{
			string strPeUri = string(DEV_TYPE_GBS) + "@" + GetDomainName();
			CuOperReqToPe<CCuGbsOptionConfGetReq, CPeSingleInterAction>(pXmlMsgContent, pcCuAgent->m_strSession, strPeUri, ptRecvNode);
		}
		break;
	case GBS_OPTION_CONF_SET_REQ:
		{
			string strPeUri = string(DEV_TYPE_GBS) + "@" + GetDomainName();
			CuOperReqToPe<CCuGbsOptionConfSetReq, CPeSingleInterAction>(pXmlMsgContent, pcCuAgent->m_strSession, strPeUri, ptRecvNode);
		}
		break;
	case GBS_CERTIFICATE_UPLOAD_REQ:
		{
			string strPeUri = string(DEV_TYPE_GBS) + "@" + GetDomainName();
			CuOperReqToPe<CCuGbsCertificateUploadReq, CPeSingleInterAction>(pXmlMsgContent, pcCuAgent->m_strSession, strPeUri, ptRecvNode);
		}
		break;
	case GBS_CERTIFICATE_GET_REQ:
		{
			string strPeUri = string(DEV_TYPE_GBS) + "@" + GetDomainName();
			CuOperReqToPe<CCuGbsCertificateGetReq, CPeSingleInterAction>(pXmlMsgContent, pcCuAgent->m_strSession, strPeUri, ptRecvNode);
		}
		break;
	case GBS_DEV_GBID_GET_REQ:
		{
			string strPeUri = string(DEV_TYPE_GBS) + "@" + GetDomainName();
			CuOperReqToPe<CCuGbsDevGbidGetReq,CPeSingleInterAction>(pXmlMsgContent,pcCuAgent->m_strSession,strPeUri,ptRecvNode);
		}
		break;
	case GBS_GBPARENTDOMAIN_GET_REQ: 
		{
			string strPeUri = string(DEV_TYPE_GBS) + "@" + GetDomainName();
			CuOperReqToPe<CCuGbsGbParentDomainGetReq,CPeSingleInterAction>(pXmlMsgContent,pcCuAgent->m_strSession,strPeUri,ptRecvNode);
		}
		break;
	case GBS_GBPARENTDOMAIN_CATALOG_LOAD_REQ: 
		{
			string strPeUri = string(DEV_TYPE_GBS) + "@" + GetDomainName();
			CuOperReqToPe<CCuGbsGbPDCatalogLoadReq,CPeSingleInterAction>(pXmlMsgContent,pcCuAgent->m_strSession,strPeUri,ptRecvNode);
		}
		break;
	case GBS_GBDEV_TELEBOOT_REQ:
		{
			string strPeUri = string( DEV_TYPE_GBS ) + "@" + GetDomainName();
			CuOperReqToPe<CCuGbsGbDevTeleBootReq, CPeSingleInterAction>(pXmlMsgContent, pcCuAgent->m_strSession, strPeUri, ptRecvNode);
		}
		break;
	case GBS_DEVGRP_CIVILCODE_GET_REQ:
		{
			string strPeUri = string( DEV_TYPE_GBS ) + "@" + GetDomainName();
			CuOperReqToPe<CCuGbsDevGrpCivilCodeGetReq, CPeSingleInterAction>(pXmlMsgContent, pcCuAgent->m_strSession, strPeUri, ptRecvNode);
		}
		break;
	case GBS_DEV_KDID_GET_REQ:
		{
			string strPeUri = string( DEV_TYPE_GBS ) + "@" + GetDomainName();
			CuOperReqToPe<CCuGbsDevKdidGetReq, CPeSingleInterAction>(pXmlMsgContent, pcCuAgent->m_strSession, strPeUri, ptRecvNode);
		}
		break;
	case GBS_CATALOG_SELECT_REPORT_START_REQ:
		{
			string strPeUri = string(DEV_TYPE_GBS) + "@" + GetDomainName();
			CuOperReqToPe<CCuGbsCatalogSelectReportStartReq, CPeSingleInterAction>(pXmlMsgContent, pcCuAgent->m_strSession, strPeUri, ptRecvNode);
		}
		break;
	case CU_CUI_SECURITY_AUTHORIZATION_FIRST_REQ:
		{
			//ת���� GBS �ӿڷ���gbs
			string strPeUri = string(DEV_TYPE_GBS) + "@" + GetDomainName();
			CuOperReqToPe<CCuSecurityAuthorizationFirstReq, CPeSingleInterAction>(pXmlMsgContent, pcCuAgent->m_strSession, strPeUri, ptRecvNode);
		}
		break;
	case CU_CUI_SECURITY_AUTHORIZATION_SECOND_REQ:
		{
			//ת���� GBS �ӿڷ���gbs
			string strPeUri = string(DEV_TYPE_GBS) + "@" + GetDomainName();
			CuOperReqToPe<CCuSecurityAuthorizationSecondReq, CPeSingleInterAction>(pXmlMsgContent, pcCuAgent->m_strSession, strPeUri, ptRecvNode);
		}
		break;
    case VCR_CTRL_REQ:
        {
            CCuVcrCtrlReq cReq;
            cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);
            CSipTask *pcInviteTask = pcCuAgent->GetInviteTaskPtrByPlayId(cReq.GetPlayId());
            if(NULL != pcInviteTask)
            {      
                CInviteInterAction *pcCuInviteTask = new CInviteInterAction(this, pcCuAgent, ptRecvNode);
                pcCuInviteTask->InitStateMachine();
                SetCuReqMsg(cReq, pcCuInviteTask);
            }
            else
            {
                CSipTask *pcPeInviteTask = pcCuAgent->GetPeInviteTaskPtrByPlayId(cReq.GetPlayId());
                if(NULL == pcPeInviteTask)
                {
                    INSTLOG(CUI_APP, ERROR_LEV, "�û�[%s Session: %s]ͨ��PlayId[%d]�Ҳ���InviteTask\n", 
                        pcCuAgent->m_strUserName.c_str(), strSession.c_str(), cReq.GetPlayId());
                    CCuVcrCtrlRsp cRsp;
                    cRsp.SetErrorCode(ERR_CUI_TASK_DEL);
                    SendRspToCu<CCuVcrCtrlRsp>(cRsp, ptRecvNode);
                }
                else
                {
					if (pcCuAgent->CuiTransStream4PlatRec())
					{
						CPeInviteInterActionNatSwitch *pcPeInviteTask = new CPeInviteInterActionNatSwitch(this, pcCuAgent->m_strSession, ptRecvNode);
						pcPeInviteTask->InitStateMachine();
						SetCuReqMsg(cReq, pcPeInviteTask);
					}
					else
					{
						CPeInviteInterAction *pcPeInviteTask = new CPeInviteInterAction(this, pcCuAgent->m_strSession, ptRecvNode);
						pcPeInviteTask->InitStateMachine();
						SetCuReqMsg(cReq, pcPeInviteTask);
					}
                }
            }
        }
        break;
    case CU_CUI_GET_EXPORT_PROCESS_REQ:
        {
            CCuGetExportProcessReq cCuReq;
            cCuReq.FromXml(pXmlMsgContent);
            CCuGetExportProcessRsp cCuRsp;
            CExportTask *pcTask = pcCuAgent->GetExportTaskPtrByFileUrl(cCuReq.GetExportFileUrl());
            if( NULL != pcTask)
            {
                cCuRsp.SetPercent(pcTask->GetProcessNtfReq().GetPercent());
                SendRspToCu<CCuGetExportProcessRsp>(cCuRsp, ptRecvNode);
            }
            else
            {
                INSTLOG(CUI_APP, ERROR_LEV, "�û�[%s Session: %s]ͨ��FileUrl[%s]�Ҳ���CExportTask\n", 
                    pcCuAgent->m_strUserName.c_str(), strSession.c_str(), cCuReq.GetExportFileUrl().c_str());
                cCuRsp.SetErrorCode(ERR_CUI_TASK_DEL);
                SendRspToCu<CCuGetExportProcessRsp>(cCuRsp, ptRecvNode);
            }
        }
        break;
    case CU_CUI_PLAYBACK_PROGRESS_QUERY_REQ:
        {
            CCuPlaybackProgressQueryReq cCuReq;
            cCuReq.FromXml(pXmlMsgContent);

            CCuPlaybackProgressQueryRsp cCuRsp;

            CSipTask *pcTask = pcCuAgent->GetInviteTaskPtrByPlayId(cCuReq.GetPlayId());
            if(NULL != pcTask)
            {
				if (pcCuAgent->CuiTransStream4Pu())
				{
					CInviteTaskNatSwitch* pcInviteTask = (CInviteTaskNatSwitch*)pcTask;
					cCuRsp.SetPlayBackProgressNtf(pcInviteTask->GetPlayBackProgressNtf());

					SendRspToCu<CCuPlaybackProgressQueryRsp>(cCuRsp, ptRecvNode);
				}
				else
				{
					CInviteTaskEx* pcInviteTask = (CInviteTaskEx*)pcTask;
					cCuRsp.SetPlayBackProgressNtf(pcInviteTask->GetPlayBackProgressNtf());

					SendRspToCu<CCuPlaybackProgressQueryRsp>(cCuRsp, ptRecvNode);
				}

            }
            else
            {
                CSipTask *pcTask = pcCuAgent->GetPeInviteTaskPtrByPlayId(cCuReq.GetPlayId());
                if(NULL == pcTask)
                {
                    INSTLOG(CUI_APP, ERROR_LEV, "�û�[%s Session: %s]ͨ��PlayId[%d]�Ҳ���InviteTask\n", 
                        pcCuAgent->m_strUserName.c_str(), strSession.c_str(), cCuReq.GetPlayId());
                    cCuRsp.SetErrorCode(ERR_CUI_TASK_DEL);
                    SendRspToCu<CCuPlaybackProgressQueryRsp>(cCuRsp, ptRecvNode);
                }
                else
                {
					if ( pcCuAgent->CuiTransStream4PlatRec() )
					{
						CPeInviteTaskNatSwitch *pcPeInviteTask = (CPeInviteTaskNatSwitch *)pcTask;
						cCuRsp.SetPlayBackProgressNtf(pcPeInviteTask->GetPlayBackProgressNtf());
					}
					else
					{
						CPeInviteTask *pcPeInviteTask = (CPeInviteTask *)pcTask;
						cCuRsp.SetPlayBackProgressNtf(pcPeInviteTask->GetPlayBackProgressNtf());
					}

					SendRspToCu<CCuPlaybackProgressQueryRsp>(cCuRsp, ptRecvNode);
				}
            }
        }
        break;
    case PLAT_REC_ON_TV_REQ:
        {
            string strPeUri = string( DEV_TYPE_TVS ) + "@" + GetDomainName();
            CuOperReqToPe<CCuPlatRecOnTvReq, CPeSingleInterAction>(pXmlMsgContent, pcCuAgent->m_strSession, strPeUri, ptRecvNode);
        }
        break;
	case RECORD_ON_TV_REQ:
		{
			/*string strPeUri = string( DEV_TYPE_TVS ) + "@" + GetDomainName();
			CuOperReqToPe<CCuRecordOnTvReq, CPeSingleInterAction>(pXmlMsgContent, pcCuAgent->m_strSession, strPeUri, ptRecvNode);*/
			CCuRecordOnTvReq cReq;
			cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);

			if (IsTvChnTaskExit(cReq.GetTvChn()))
			{
				CCuRecordOnTvRsp cRsp;
				cRsp.SetHeadFromReq(cReq);
				cRsp.SetErrorCode(ERR_TVS_TVWALL_TVCHN_TASK_EXIST);
				SendRspToCu<CCuRecordOnTvRsp>(cRsp,ptRecvNode);
			}
			else
			{
				CTask* pTask = CreateTask(wEvent,pcCuAgent,ptRecvNode);
				if (NULL != pTask)
				{
					//CCuRecordOnTvReq cReq;
					//cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);

					CMessage cCuReqMsg;
					cCuReqMsg.event = wEvent;
					cCuReqMsg.content = (u8*)&cReq;

					ProcTaskMsg(pTask,&cCuReqMsg);
				}
			}
		}
		break;
	case RECORD_OFF_TV_REQ:
		{
			/*string strPeUri = string( DEV_TYPE_TVS ) + "@" + GetDomainName();
			CuOperReqToPe<CCuRecordOffTvReq, CPeSingleInterAction>(pXmlMsgContent, pcCuAgent->m_strSession, strPeUri, ptRecvNode);*/
			CRecordOffTvReq cReq;
			cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);

			CTask* pTask = pcCuAgent->GetRecTvTask(cReq.GetTvChn());
			if (pTask != NULL)
			{
				CRecordOnTvTask* pRecTask = (CRecordOnTvTask*)pTask;
				pRecTask->SetRecvNode(ptRecvNode);

				CMessage cCuReqMsg;
				cCuReqMsg.event = wEvent;
				cCuReqMsg.content = (u8*)&cReq;

				ProcTaskMsg(pTask,&cCuReqMsg);
			}
			else
			{
				CRecordOffTvRsp cRsp;
				cRsp.SetHeadFromReq(cReq);
				cRsp.SetErrorCode(ERR_TVS_TV_PLAY_NOT_EXIST);
				SendRspToCu<CRecordOffTvRsp>(cRsp,ptRecvNode);
			}
		}
		break;
    case REAL_MEDIA_ON_TV_REQ:
        {
            string strPeUri = string( DEV_TYPE_TVS ) + "@" + GetDomainName();
            CuOperReqToPe<CCuRealMediaOnTvReq, CPeSingleInterAction>(pXmlMsgContent, pcCuAgent->m_strSession, strPeUri, ptRecvNode);
        }
        break;
	case REAL_MEDIA_SWITCH_OCCUR_TYPE_REQ:
		{
			string strPeUri = string( DEV_TYPE_TVS ) + "@" + GetDomainName();
			CuOperReqToPe<CCuRealMediaSwitchOccurTypeReq, CPeSingleInterAction>(pXmlMsgContent, pcCuAgent->m_strSession, strPeUri, ptRecvNode);
		}
		break;
    case REAL_MEDIA_OFF_TV_REQ:
        {
            string strPeUri = string( DEV_TYPE_TVS ) + "@" + GetDomainName();
            CuOperReqToPe<CCuRealMediaOffTvReq, CPeSingleInterAction>(pXmlMsgContent, pcCuAgent->m_strSession, strPeUri, ptRecvNode);
        }
        break;

    case MPS_GET_SUPPORT_MAP_TYPES_REQ:
        {
            string strPeUri = string( DEV_TYPE_MPS ) + "@" + GetDomainName();
            u32 dwRet = PROCMSG_OK;
            CMS_ASSERT(NULL != pXmlMsgContent);

            CCuGetSupportMapTypesReq cReq;
            cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);
            cReq.SetClientNatInfo(pcCuAgent->m_tNatInfo);

            CPeSingleInterAction* pcTask = new CPeSingleInterAction(this, strSession, strPeUri, ptRecvNode);
            pcTask->InitStateMachine();
            dwRet = SetCuReqMsg(cReq, pcTask);
            if (dwRet == PROCMSG_DEL)
            {
                INSTLOG(CUI_APP, ALL_LEV, "����[%s-%u]�������Ҫɾ��---msg[%s-%u]\n", 
                    pcTask->GetObjName(), pcTask->GetTaskNO(),
                    cReq.GetEventStr().c_str(), cReq.GetEventId());

                delete pcTask;
                pcTask = NULL;
            }
            else if (dwRet == PROCMSG_FAIL)
            {
                INSTLOG(CUI_APP, ERROR_LEV, "����[%s-%u]����ʧ��---msg[%s-%u]\n", 
                    pcTask->GetObjName(), pcTask->GetTaskNO(),
                    cReq.GetEventStr().c_str(), cReq.GetEventId());
            }
            else if (dwRet == PROCMSG_OK)
            {
                INSTLOG(CUI_APP, ALL_LEV, "����[%s-%u]����ɹ�---msg[%s-%u]\n", 
                    pcTask->GetObjName(), pcTask->GetTaskNO(),
                    cReq.GetEventStr().c_str(), cReq.GetEventId());
            }
            else
            {
                INSTLOG(CUI_APP, ERROR_LEV, "����[%s-%u]������δ֪---msg[%s-%u]\n", 
                    pcTask->GetObjName(), pcTask->GetTaskNO(),
                    cReq.GetEventStr().c_str(), cReq.GetEventId());
            }
        }
        break;
    case MPS_MAP_AREA_CREATE_REQ:
        {
            string strPeUri = string( DEV_TYPE_MPS ) + "@" + GetDomainName();
            CuOperReqToPe<CCuMapAreaCreateReq, CPeSingleInterAction>(pXmlMsgContent, pcCuAgent->m_strSession, strPeUri, ptRecvNode);
        }
        break;
    case MPS_MAP_AREA_MOD_REQ:
        {
            string strPeUri = string( DEV_TYPE_MPS ) + "@" + GetDomainName();
            CuOperReqToPe<CCuMapAreaModReq, CPeSingleInterAction>(pXmlMsgContent, pcCuAgent->m_strSession, strPeUri, ptRecvNode);
        }
        break;
	case RCS_RECORD_LOCK_OPR_REQ:
		{
            TSipURI tPeUri;
            CStreamDecentrationAdapter::FindRcsUri( tPeUri, GetDomainName() );
			CuOperReqToPe<CCuRcsRecordLockOprReq, CPeSingleInterAction>(pXmlMsgContent, pcCuAgent->m_strSession, tPeUri.GetURIString(), ptRecvNode);
		}
		break;
    case RCS_SET_RECORD_TASK_REQ:
        {
            //1 ��Ȩ
            bool bAuthRet = true; //��Ȩ���
            do 
            {                
                CCuRcsSetRecordTaskReq cReq;
                cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);
                string strDomainName = GetDomainNameWithoutPreNameFromURI(cReq.GetPuChn().GetDevUri());
                if ( ! pcCuAgent->IsExistDomainInfo(strDomainName))
                {
                    bAuthRet = false;
                    break;
                }
				
				if (pcCuAgent->m_pcUserAccount->IsSuperAdmin())
                {
                    bAuthRet = true;
                    break;
                }

                //�ж��û��Ƿ���ƽ̨�����Ȩ��
                CPrivilege cPri = pcCuAgent->m_pcUserAccount->GetUserPri();
                if(!cPri.IsEmpty())
                {
                    if(cPri.HasPri(CUserPrivilege::en_Vod_Rec))
                    {
                        bAuthRet = true;
                        break;
                    }
                    else
                    {
                        bAuthRet = false;
                    }
                }
                else
                {
                    bAuthRet = false;
                    break;
                }

            }while(0);

            if (!bAuthRet)
            {
                INSTLOG(CUI_APP, ERROR_LEV, "�û�[%s Session: %s]��Ȩ�޿���ƽ̨¼������\n", 
                    pcCuAgent->m_strUserName.c_str(), strSession.c_str());
                CCuRcsSetRecordTaskRsp cRsp;
                cRsp.SetErrorCode(ERR_CUI_CU_NO_PERMISSION);
                SendRspToCu<CCuRcsSetRecordTaskRsp>(cRsp, ptRecvNode);
                return dwRet;
            }
            
            TSipURI tPeUri;
            CStreamDecentrationAdapter::FindRcsUri( tPeUri, GetDomainName() );
            //ȥ�������Ļ�
            CuOperReqToPe<CCuRcsSetRecordTaskReq, CPeSingleInterAction>(pXmlMsgContent, pcCuAgent->m_strSession, tPeUri.GetURIString(), ptRecvNode);
        }
        break;
    case TV_WIN_STYLE_SET_REQ:
        {
            string strPeUri = string( DEV_TYPE_TVS ) + "@" + GetDomainName();
            CuOperReqToPe<CCuTvWinStyleSetReq, CPeSingleInterAction>(pXmlMsgContent, pcCuAgent->m_strSession, strPeUri, ptRecvNode);
        }
        break;
    case UAS_VIDEOSOURCE_PRESETCFG_ADD_REQ:
        {
            string strPeUri = string( DEV_TYPE_UAS ) + "@" + GetDomainName();
            CuOperReqToPe<CCuVideosourcePresetcfgAddReq, CPeSingleInterAction>(pXmlMsgContent, pcCuAgent->m_strSession, strPeUri, ptRecvNode);
        }
        break;
    case UAS_VIDEOSOURCE_PRESETCFG_DEL_REQ:
        {
            string strPeUri = string( DEV_TYPE_UAS ) + "@" + GetDomainName();
            CuOperReqToPe<CCuVideosourcePresetcfgDelReq, CPeSingleInterAction>(pXmlMsgContent, pcCuAgent->m_strSession, strPeUri, ptRecvNode);
        }
        break;
	/************************����CAPS������****************************/
	case CAPS_GET_CONFIG_REQ:
		{
			string strPeUri = string( DEV_TYPE_CAPS ) + "@" + GetDomainName();
			CuOperReqToPe<CCuGetCAPSConfigReq, CPeSingleInterAction>(pXmlMsgContent, pcCuAgent->m_strSession, strPeUri, ptRecvNode);
		}
		break;
	case CAPS_SET_CONFIG_REQ:
		{
			string strPeUri = string( DEV_TYPE_CAPS ) + "@" + GetDomainName();
			CuOperReqToPe<CCuSetCAPSConfigReq, CPeSingleInterAction>(pXmlMsgContent, pcCuAgent->m_strSession, strPeUri, ptRecvNode);
		}
		break;
	case CAPS_QUERY_DEVICE_LIST_REQ:  
		{
			string strPeUri = string( DEV_TYPE_CAPS ) + "@" + GetDomainName();
			CuOperReqToPe<CCuQueryCAPSDeviceListReq, CPeQueryTask>(pXmlMsgContent, pcCuAgent->m_strSession, strPeUri, ptRecvNode);
		}
		break;
	case CAPS_ADD_DEVICE_REQ:
		{
			string strPeUri = string( DEV_TYPE_CAPS ) + "@" + GetDomainName();
			CuOperReqToPe<CCuCAPSAddDeviceReq, CPeSingleInterAction>(pXmlMsgContent, pcCuAgent->m_strSession, strPeUri, ptRecvNode);
		}
		break;
	case CAPS_MODIFY_DEVICE_REQ:
		{
			string strPeUri = string( DEV_TYPE_CAPS ) + "@" + GetDomainName();
			CuOperReqToPe<CCuCAPSModifyDeviceReq, CPeSingleInterAction>(pXmlMsgContent, pcCuAgent->m_strSession, strPeUri, ptRecvNode);
		}
		break;
	case CAPS_DEL_DEVICE_REQ:
		{
			string strPeUri = string( DEV_TYPE_CAPS ) + "@" + GetDomainName();
			CuOperReqToPe<CCuCAPSDelDeviceReq, CPeSingleInterAction>(pXmlMsgContent, pcCuAgent->m_strSession, strPeUri, ptRecvNode);
		}
		break;
   /************************����CAPS������ END****************************/

        /***********************************����ģ���ѯtask***************************************/
    case RCS_QUERY_DISK_INFO_REQ:
        {
            TSipURI tRcsUri;
            CStreamDecentrationAdapter::FindRcsUri( tRcsUri, GetDomainName(), false );//ע�����ﲻ���þ��⻯
            CuOperReqToPe<CCuRcsQueryDiskInfoReq, CPeQueryTask>(pXmlMsgContent, pcCuAgent->m_strSession, tRcsUri.GetURIString(), ptRecvNode);
        }
        break;
    case RCS_QUERY_NRU_REQ:
        {
            TSipURI tRcsUri;
            CStreamDecentrationAdapter::FindRcsUri( tRcsUri, GetDomainName() );
            CuOperReqToPe<CCuRcsQueryNruReq, CPeQueryTask>(pXmlMsgContent, pcCuAgent->m_strSession, tRcsUri.GetURIString(), ptRecvNode);
        }
        break;
    case RCS_QUERY_PUCHN_BINDING_REQ:
        {
            TSipURI tRcsUri;
            CStreamDecentrationAdapter::FindRcsUri( tRcsUri, GetDomainName() );
            CuOperReqToPe<CCuRcsQueryPuChnBindingReq, CPeQueryTask>(pXmlMsgContent, pcCuAgent->m_strSession, tRcsUri.GetURIString(), ptRecvNode);
        }
        break;
    case RCS_QUERY_RECORD_PARAM_REQ:
        {
            TSipURI tRcsUri;
            CStreamDecentrationAdapter::FindRcsUri( tRcsUri, GetDomainName() );
            CuOperReqToPe<CCuRcsQueryRecordParamReq, CPeQueryTask>(pXmlMsgContent, pcCuAgent->m_strSession, tRcsUri.GetURIString(), ptRecvNode);
        }
        break;
    case TVWALL_SCHEME_QUERY_REQ:
        {
            string strPeUri = string( DEV_TYPE_TVS ) + "@" + GetDomainName();
            CuOperReqToPe<CCuTvWallSchemeQueryReq, CPeQueryTask>(pXmlMsgContent, pcCuAgent->m_strSession, strPeUri, ptRecvNode);
        }
        break;
    case MPS_MAP_QRY_REQ:
        {
            string strPeUri = string( DEV_TYPE_MPS ) + "@" + GetDomainName();
            CuOperReqToPe<CCuMapQryReq, CPeQueryTask>(pXmlMsgContent, pcCuAgent->m_strSession, strPeUri, ptRecvNode);
        }
        break;
    case MPS_MAP_ELEMENT_QRY_REQ:
        {
			CCuMapElementQryReq cReq;
			cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);

			if (pcCuAgent->m_pcUserAccount == NULL)
			{
				INSTLOG(CUI_APP, ERROR_LEV, "pcCuAgent->m_pcUserAccount == NULL!!!\n");
				CCuMapElementQryRsp cRsp;
				cRsp.SetErrorCode(ERR_CUI_TAS_OFFLINE);
				SendRspToCu<CCuMapElementQryRsp>(cRsp, ptRecvNode);
				return dwRet;
			}

			cReq.GetCondition().SetUserId(pcCuAgent->m_pcUserAccount->m_tUserID);
			cReq.GetCondition().SetIsSuperAdmin(pcCuAgent->m_pcUserAccount->IsSuperAdmin());

			string strPeUri = string(DEV_TYPE_MPS) + "@" + GetDomainName();

			CPeQueryTask* pcTask = new CPeQueryTask(this, pcCuAgent->m_strSession, strPeUri, ptRecvNode);
			pcTask->InitStateMachine();

			dwRet = SetCuReqMsg(cReq, pcTask);
			if (dwRet == PROCMSG_DEL)
			{
				INSTLOG(CUI_APP, ALL_LEV, "����[%s-%u]�������Ҫɾ��---msg[%s-%u]\n",
					pcTask->GetObjName(), pcTask->GetTaskNO(),
					cReq.GetEventStr().c_str(), cReq.GetEventId());

				pcTask = NULL;
			}
			else if (dwRet == PROCMSG_FAIL)
			{
				INSTLOG(CUI_APP, ERROR_LEV, "����[%s-%u]����ʧ��---msg[%s-%u]\n",
					pcTask->GetObjName(), pcTask->GetTaskNO(),
					cReq.GetEventStr().c_str(), cReq.GetEventId());
			}
			else if (dwRet == PROCMSG_OK)
			{
				INSTLOG(CUI_APP, ALL_LEV, "����[%s-%u]����ɹ�---msg[%s-%u]\n",
					pcTask->GetObjName(), pcTask->GetTaskNO(),
					cReq.GetEventStr().c_str(), cReq.GetEventId());
			}
			else
			{
				INSTLOG(CUI_APP, ERROR_LEV, "����[%s-%u]������δ֪---msg[%s-%u]\n",
					pcTask->GetObjName(), pcTask->GetTaskNO(),
					cReq.GetEventStr().c_str(), cReq.GetEventId());
			}
        }
		break;
	case MPS_MAP_ELEMENT_SEARCH_REQ:
		{
			CCuMapElementSearchReq cReq;
			cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);

			if (pcCuAgent->m_pcUserAccount == NULL)
			{
				INSTLOG(CUI_APP, ERROR_LEV, "pcCuAgent->m_pcUserAccount == NULL!!!\n");
				CCuMapElementSearchRsp cRsp;
				cRsp.SetErrorCode(ERR_CUI_TAS_OFFLINE);
				SendRspToCu<CCuMapElementSearchRsp>(cRsp, ptRecvNode);
				return dwRet;
			}

			cReq.SetUserId(pcCuAgent->m_pcUserAccount->m_tUserID);
			cReq.SetIsSuperAdmin(pcCuAgent->m_pcUserAccount->IsSuperAdmin());

			string strPeUri = string(DEV_TYPE_MPS) + "@" + GetDomainName();

			CPeQueryTask* pcTask = new CPeQueryTask(this, pcCuAgent->m_strSession, strPeUri, ptRecvNode);
			pcTask->InitStateMachine();

			dwRet = SetCuReqMsg(cReq, pcTask);
			if (dwRet == PROCMSG_DEL)
			{
				INSTLOG(CUI_APP, ALL_LEV, "����[%s-%u]�������Ҫɾ��---msg[%s-%u]\n",
					pcTask->GetObjName(), pcTask->GetTaskNO(),
					cReq.GetEventStr().c_str(), cReq.GetEventId());

				pcTask = NULL;
			}
			else if (dwRet == PROCMSG_FAIL)
			{
				INSTLOG(CUI_APP, ERROR_LEV, "����[%s-%u]����ʧ��---msg[%s-%u]\n",
					pcTask->GetObjName(), pcTask->GetTaskNO(),
					cReq.GetEventStr().c_str(), cReq.GetEventId());
			}
			else if (dwRet == PROCMSG_OK)
			{
				INSTLOG(CUI_APP, ALL_LEV, "����[%s-%u]����ɹ�---msg[%s-%u]\n",
					pcTask->GetObjName(), pcTask->GetTaskNO(),
					cReq.GetEventStr().c_str(), cReq.GetEventId());
			}
			else
			{
				INSTLOG(CUI_APP, ERROR_LEV, "����[%s-%u]������δ֪---msg[%s-%u]\n",
					pcTask->GetObjName(), pcTask->GetTaskNO(),
					cReq.GetEventStr().c_str(), cReq.GetEventId());
			}
		}
		break;
    case MPS_MAP_AREA_QRY_REQ:
        {
            string strPeUri = string( DEV_TYPE_MPS ) + "@" + GetDomainName();
            CuOperReqToPe<CCuMapAreaQryReq, CPeQueryTask>(pXmlMsgContent, pcCuAgent->m_strSession, strPeUri, ptRecvNode);
        }
        break;
    case MPS_ELEMENT_IMAGE_QRY_REQ:
        {
            string strPeUri = string( DEV_TYPE_MPS ) + "@" + GetDomainName();
            CuOperReqToPe<CElementImageQryReq, CPeQueryTask>(pXmlMsgContent, pcCuAgent->m_strSession, strPeUri, ptRecvNode);
        }
        break;
	case MPS_GPS_TRACE_QRY_REQ:
		{
			string strPeUri = string( DEV_TYPE_MPS ) + "@" + GetDomainName();
			CuOperReqToPe<CCuGpsTraceQryReq,CPeQueryTask>(pXmlMsgContent,pcCuAgent->m_strSession,strPeUri,ptRecvNode);
		}
		break;
    case ALARMLINK_VERSION_GET_REQ:
        {
            string strPeUri = string( DEV_TYPE_ALS ) + "@" + GetDomainName();
            CuOperReqToPe<CCuAlarmLinkVersionGetReq, CPeSingleInterAction>(pXmlMsgContent, pcCuAgent->m_strSession, strPeUri, ptRecvNode);
        }
        break;
    case ALARMLINK_QUERY_REQ:
        {
            string strPeUri = string( DEV_TYPE_ALS ) + "@" + GetDomainName();
            CuOperReqToPe<CCuAlarmLinkQueryReq, CPeQueryTask>(pXmlMsgContent, pcCuAgent->m_strSession, strPeUri, ptRecvNode);
        }
        break;
    case UAS_OPERATE_LOG_QRY_REQ:
        {
            string strPeUri = string( DEV_TYPE_UAS ) + "@" + GetDomainName();
            CuOperReqToPe<CCuOperateLogQryReq, CPeQueryTask>(pXmlMsgContent, pcCuAgent->m_strSession, strPeUri, ptRecvNode);
        }
        break;
   // case UAS_OPERATE_LOG_ADD_REQ:
   //     {
			//// �������ò��û��ʵ�֣�û�нӿڵ���
   //         CCuOperateLogAddReq cReq;
   //         cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);

   //         CSingleInterAction *pcReqTask = new CSingleInterAction(this, pcCuAgent, ptRecvNode);
   //         pcReqTask->InitStateMachine();
   //         //3���������ⲿ�������󣬽���������
   //         SetCuReqMsg(cReq, pcReqTask);
   //     }
   //     break;
    case UAS_DEVICE_LOG_QRY_REQ:
        {
            string strPeUri = string( DEV_TYPE_UAS ) + "@" + GetDomainName();
            CuOperReqToPe<CCuDeviceLogQryReq, CPeQueryTask>(pXmlMsgContent, pcCuAgent->m_strSession, strPeUri, ptRecvNode);
        }
        break;
    case UAS_VIDEOSOURCE_PRESETCFG_QRY_REQ:
        {
            string strPeUri = string( DEV_TYPE_UAS ) + "@" + GetDomainName();
            CuOperReqToPe<CVideosourcePresetcfgQryReq, CPeQueryTask>(pXmlMsgContent, pcCuAgent->m_strSession, strPeUri, ptRecvNode);
        }
        break;
    case UAS_ALARM_LOG_QRY_REQ:
        {
            CMS_ASSERT(NULL != pXmlMsgContent);

            CCuAlarmLogQryReq cReq;
            cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);

            string DomainName;
            cReq.GetCondition().GetDevDomainName(DomainName);
            string strPeUri;
            if(DomainName.empty())
            {
                strPeUri = string( DEV_TYPE_UAS ) + "@" + GetDomainName();
            }
            else
            {
                strPeUri = string( DEV_TYPE_UAS ) + "@" + DomainName;
            }

            CPeQueryTask* pcTask = new CPeQueryTask(this, pcCuAgent->m_strSession, strPeUri, ptRecvNode);
            pcTask->InitStateMachine();
            dwRet = SetCuReqMsg(cReq, pcTask);
            if (dwRet == PROCMSG_DEL)
            {
                INSTLOG(CUI_APP, ALL_LEV, "����[%s-%u]�������Ҫɾ��---msg[%s-%u]\n", 
                    pcTask->GetObjName(), pcTask->GetTaskNO(),
                    cReq.GetEventStr().c_str(), cReq.GetEventId());

                delete pcTask;
                pcTask = NULL;
            }
            else if (dwRet == PROCMSG_FAIL)
            {
                INSTLOG(CUI_APP, ERROR_LEV, "����[%s-%u]����ʧ��---msg[%s-%u]\n", 
                    pcTask->GetObjName(), pcTask->GetTaskNO(),
                    cReq.GetEventStr().c_str(), cReq.GetEventId());
            }
            else if (dwRet == PROCMSG_OK)
            {
                INSTLOG(CUI_APP, ALL_LEV, "����[%s-%u]����ɹ�---msg[%s-%u]\n", 
                    pcTask->GetObjName(), pcTask->GetTaskNO(),
                    cReq.GetEventStr().c_str(), cReq.GetEventId());
            }
            else
            {
                INSTLOG(CUI_APP, ERROR_LEV, "����[%s-%u]������δ֪---msg[%s-%u]\n", 
                    pcTask->GetObjName(), pcTask->GetTaskNO(),
                    cReq.GetEventStr().c_str(), cReq.GetEventId());
            }
            //             string strPeUri = string( DEV_TYPE_UAS ) + "@" + GetDomainName();
            //             CuOperReqToPe<CCuAlarmLogQryReq, CPeQueryTask>(pXmlMsgContent, pcCuAgent->m_strSession, strPeUri, ptRecvNode);
        }
        break;
    case START_THIRD_PARTY_CALL_REQ:
        {
            CCuStartThirdPartyCallReq cReq;
            cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);
            CCallPuInterAction* pcTask = new CCallPuInterAction(this, pcCuAgent->m_strSession, ptRecvNode);
            pcTask->InitStateMachine();
            dwRet = SetCuReqMsg(cReq, pcTask);
            //todo �����´������ɺ������ο�cmu
            if (dwRet == PROCMSG_DEL)
            {
                INSTLOG(CUI_APP, ALL_LEV, "����[%s-%u]�������Ҫɾ��---msg[%s-%u]\n", 
                    pcTask->GetObjName(), pcTask->GetTaskNO(),
                    cReq.GetEventStr().c_str(), cReq.GetEventId());

                delete pcTask;
                pcTask = NULL;
            }
            else if (dwRet == PROCMSG_FAIL)
            {
                INSTLOG(CUI_APP, ERROR_LEV, "����[%s-%u]����ʧ��---msg[%s-%u]\n", 
                    pcTask->GetObjName(), pcTask->GetTaskNO(),
                    cReq.GetEventStr().c_str(), cReq.GetEventId());
            }
            else if (dwRet == PROCMSG_OK)
            {
                INSTLOG(CUI_APP, ALL_LEV, "����[%s-%u]����ɹ�---msg[%s-%u]\n", 
                    pcTask->GetObjName(), pcTask->GetTaskNO(),
                    cReq.GetEventStr().c_str(), cReq.GetEventId());
            }
            else
            {
                INSTLOG(CUI_APP, ERROR_LEV, "����[%s-%u]������δ֪---msg[%s-%u]\n", 
                    pcTask->GetObjName(), pcTask->GetTaskNO(),
                    cReq.GetEventStr().c_str(), cReq.GetEventId());
            }
        }
        break;
    case STOP_THIRD_PARTY_CALL_REQ:
        {
            CCuStopThirdPartyCallReq cReq;
            cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);

            CCallPuInterAction* pcTask = new CCallPuInterAction(this, pcCuAgent->m_strSession, ptRecvNode);
            pcTask->InitStateMachine();
            dwRet = SetCuReqMsg(cReq, pcTask);
            if (dwRet == PROCMSG_DEL)
            {
                INSTLOG(CUI_APP, ALL_LEV, "����[%s-%u]�������Ҫɾ��---msg[%s-%u]\n", 
                    pcTask->GetObjName(), pcTask->GetTaskNO(),
                    cReq.GetEventStr().c_str(), cReq.GetEventId());

                delete pcTask;
                pcTask = NULL;
            }
            else if (dwRet == PROCMSG_FAIL)
            {
                INSTLOG(CUI_APP, ERROR_LEV, "����[%s-%u]����ʧ��---msg[%s-%u]\n", 
                    pcTask->GetObjName(), pcTask->GetTaskNO(),
                    cReq.GetEventStr().c_str(), cReq.GetEventId());
            }
            else if (dwRet == PROCMSG_OK)
            {
                INSTLOG(CUI_APP, ALL_LEV, "����[%s-%u]����ɹ�---msg[%s-%u]\n", 
                    pcTask->GetObjName(), pcTask->GetTaskNO(),
                    cReq.GetEventStr().c_str(), cReq.GetEventId());
            }
            else
            {
                INSTLOG(CUI_APP, ERROR_LEV, "����[%s-%u]������δ֪---msg[%s-%u]\n", 
                    pcTask->GetObjName(), pcTask->GetTaskNO(),
                    cReq.GetEventStr().c_str(), cReq.GetEventId());
            }
        }
        break;

		/* ֱ��ȡredis���ݣ������κ�����Ľӿ� */
	case CU_CUI_GET_DEV_INFO_AND_STATUS_REQ:
		{
			CCuGetDevInfoAndStatusReq cReq;
			cReq.FromXml(pXmlMsgContent);cReq.SetUserMonitorId(pcCuAgent->m_pcUserAccount->m_tUserID);

			std::vector<string> &devIdList = cReq.GetDevIdList();

			CCuGetDevInfoAndStatusRsp cRsp;
			vector<TDeviceInfoAndStatus> &devInfoList = cRsp.GetDevInfoList();

			size_t maxSize = devIdList.size();
			// ���ǵ���������������ܵ��³�ʱ����ʱ������൥��֧��100���豸������ȡ��
			if (devIdList.size() > REDIS_MAX_QUERY_DEVICE_INFO_ENTRY_NUM)
			{
				maxSize = REDIS_MAX_QUERY_DEVICE_INFO_ENTRY_NUM;
				INSTLOG(CUI_APP, ERROR_LEV, "���β�ѯ�豸��Ϣ�������[%d]���˴β�ѯ������[%d]�Ѿ��������ƣ�������ѯ��������ѯ��������Ƶ�����\n",
					REDIS_MAX_QUERY_DEVICE_INFO_ENTRY_NUM, devIdList.size());
			}

			for (size_t nIndex = 0; nIndex < maxSize; nIndex++)
			{
				string strDevUri = devIdList[nIndex];
				TSipURI tDevUri;
				if ( !tDevUri.SetURIString(strDevUri) )
				{
					tDevUri.SetUser(strDevUri);
				}

				TDeviceInfoAndStatus tDeviceInfoAndStatus;

				string uuid = tDevUri.GetUser();
				if ( uuid.empty() )
				{
					tDeviceInfoAndStatus.devInfo.deviceInfo.SetDeviceUUID(strDevUri);
					tDeviceInfoAndStatus.devConfig.SetDevUri(strDevUri);
					tDeviceInfoAndStatus.isDevInfoValid = false;

					devInfoList.push_back(tDeviceInfoAndStatus);
					continue;
				}

				if (pcCuAgent->m_pcUserAccount == NULL)
				{
					INSTLOG(CUI_APP, ERROR_LEV, "pcCuAgent->m_pcUserAccount == NULL!");
					cRsp.SetErrorCode(ERR_CUI_TAS_OFFLINE);
					SendRspToCu<CCuGetDevInfoAndStatusRsp>(cRsp, ptRecvNode);
				}

				//string errMsg;
				//EFDB_ErrorCode er = g_cRedisDb.GetUserFullDevForCu(pcCuAgent->m_pcUserAccount->m_tUserID, uuid, pcCuAgent->m_pcUserAccount->IsSuperAdmin(), tDeviceInfoAndStatus, errMsg);
				//if ( !IsFdbResultPositive(er) )
				//{
				//	tDeviceInfoAndStatus.devInfo.deviceInfo.Clear();
				//	tDeviceInfoAndStatus.devInfo.deviceInfo.SetDeviceUUID(strDevUri);
				//	tDeviceInfoAndStatus.isDevInfoValid = false;

				//	INSTLOG(CUI_APP, ERROR_LEV, "��redis��ȡ�豸��ϢGetFullDeviceInfoʧ�� uuid[%s] errMsg[%s] er[%d]\n",uuid.c_str(), errMsg.c_str(), er);
				//}
				//else
				//{
				//	tDeviceInfoAndStatus.isDevInfoValid = true;
				//}

				string errMsg;
				EFDB_ErrorCode er = g_cRedisDb.GetUserFullDevicefullinfoAndPrivilege(pcCuAgent->m_pcUserAccount->m_tUserID, uuid, pcCuAgent->m_pcUserAccount->IsSuperAdmin(), tDeviceInfoAndStatus.devInfo, errMsg);
				if ( !IsFdbResultPositive(er) )
				{
					tDeviceInfoAndStatus.devInfo.deviceInfo.Clear();
					tDeviceInfoAndStatus.devInfo.deviceInfo.SetDeviceUUID(strDevUri);
					tDeviceInfoAndStatus.isDevInfoValid = false;
					
					INSTLOG(CUI_APP, ERROR_LEV, "��redis��ȡ�豸��ϢGetUserFullDevicefullinfoAndPrivilegeʧ�� uuid[%s] errMsg[%s] er[%d]\n", uuid.c_str(), errMsg.c_str(), er);
					continue;
				}

				tDeviceInfoAndStatus.devInfo.deviceInfo.SetDeviceUUID(strDevUri);
				tDeviceInfoAndStatus.isDevInfoValid = true;

				TPuStatus tPuStat;
				er = g_cRedisDb.GetPuStatus(uuid, tPuStat, TPuStatus_isOnline);
				if ( !IsFdbResultPositive(er) )
				{
					tDeviceInfoAndStatus.isOnline = false;
					INSTLOG(CUI_APP, ERROR_LEV, "��redis��ȡ�豸��ϢGetPuStatusʧ�� uuid[%s] er[%d]\n", uuid.c_str(), er);
				}
				else
				{
					tDeviceInfoAndStatus.isOnline = tPuStat.GetIsOnline();
				}

				er = g_cRedisDb.GetFullPuConfigForCu(uuid, tDeviceInfoAndStatus.devConfig, errMsg);
				if ( !IsFdbResultPositive(er) )
				{
					INSTLOG(CUI_APP, ERROR_LEV, "��ȡ�豸������ϢGetFullPuConfigForCuʧ�� uuid[%s] errMsg[%s] er[%d]\n", uuid.c_str(), errMsg.c_str(), er);
				}

				tDeviceInfoAndStatus.devConfig.SetDevUri(strDevUri);

                er = g_cRedisDb.GetPuServiceStatusForCu(uuid, tDeviceInfoAndStatus.serviceStatus, errMsg);
                if ( !IsFdbResultPositive(er) )
                {
                    INSTLOG(CUI_APP, ERROR_LEV, "��ȡ�豸����״̬GetPuServiceStatusForCuʧ�� uuid[%s] errMsg[%s] er[%d]\n", uuid.c_str(), errMsg.c_str(), er);
                }

                er = g_cRedisDb.GetPuAlarmStatusForCu(uuid, tDeviceInfoAndStatus.alarmStatus, errMsg);
                if ( !IsFdbResultPositive(er) )
                {
                    INSTLOG(CUI_APP, ERROR_LEV, "��ȡ�豸�澯��ϢGetPuAlarmStatusForCuʧ�� uuid[%s] errMsg[%s] er[%d]\n", uuid.c_str(), errMsg.c_str(), er);
                }

				devInfoList.push_back(tDeviceInfoAndStatus);
			}

			cRsp.SetErrorCode(CMS_SUCCESS);
			SendRspToCu<CCuGetDevInfoAndStatusRsp>(cRsp, ptRecvNode);
		}
		break;

    case CU_CUI_DEV_RECORD_SVR_QRY_REQ:
        {
            CCuDeviceRecordSvrQryRsp cRsp;
            //TODO: һ��һ��
            CFastDbServiceExt cRedisExt;
            cRedisExt.AttachFastDb( &g_cCuiRedisSdk.GetFastDBInstance() );
            THostDocNatInfo tHostDocInfo;
            bool bSuccessful = cRedisExt.GetRecordServerUri( tHostDocInfo );
            if( bSuccessful && tHostDocInfo.bEnabled )
            {
                if( !tHostDocInfo.GetInUriStr().empty() )
                {
                    cRsp.GetUriList().push_back( tHostDocInfo.GetInUriStr() );
                }
                if( !tHostDocInfo.GetOutUriStr().empty() )
                {
                    cRsp.GetUriList().push_back( tHostDocInfo.GetOutUriStr() );
                }
            }

            cRsp.SetErrorCode( bSuccessful ? CMS_SUCCESS : ERR_CUI_GET_REDIS_DATA_FAILED );
            SendRspToCu<CCuDeviceRecordSvrQryRsp>(cRsp, ptRecvNode);
        }
        break;

	case ALARM_TYPE_GET_REQ:
		{
			CCuGetAlarmTypesRsp cRsp;
			vector<TAlarmType> &tAlarmTypeList = cRsp.GetAlarmTypeArray();

			const CAlarmTypeCommon* pcAlmTypeUtil = GetCommonType();
			if( pcAlmTypeUtil != NULL )
			{
				CCuiAlarmTypeWriter cWriter( &tAlarmTypeList );
				pcAlmTypeUtil->WriteAllAlarmDesc( &cWriter );
			}
			//TAlarmType tAlarmType;
			//tAlarmType.SetAlarmType(ALARM_TYPE_PIN);
			//tAlarmType.SetAlarmTypeName(GetLanDesc(LAN_ALARM_TYPE_PIN));
			//tAlarmTypeList.push_back(tAlarmType);
			//tAlarmType.Clear();
			//...


			cRsp.SetErrorCode(CMS_SUCCESS);
			SendRspToCu<CCuGetAlarmTypesRsp>(cRsp, ptRecvNode);
		}
		break;
    default:
        {
            INSTLOG(CUI_APP, ERROR_LEV, "[OnServiceCuRequestMsg]CUI����δ֪����[%s--%hu]\n", 
                strEvent.c_str(), wEvent);
            CMS_ASSERT(0 && "[OnServiceCuRequestMsg]CUI����δ֪����");

            CEventRsp cRsp;
            cRsp.SetErrorCode(ERR_CUI_UNKNOW_MSG);
            cRsp.SetEvent(wEvent+1);
            SendRspToCu<CEventRsp>(cRsp, ptRecvNode);
        }
        break;
    }

    return dwRet;
}

void CCuiInst::OnReLogin(CMessage* const pcMsg)
{
    //��״̬������cmu����������״̬�����˴�����ע�ᣬ����Ҫ����ȡ��������Ϣ
    switch (pcMsg->event)
    {
    case OSP_PROXY_CONNECT:
        {
            g_cCuiDataMgr.m_cCuiState.m_bConnectProxy = true;
            g_cCuiDataMgr.SetCuiStateHappenTime();
            INSTLOG(CUI_APP, EVENT_LEV, "CUIע��Proxy�ɹ���\n");
        }
        break;
    case OSP_PROXY_DISCONNECT:
        {
            INSTLOG(CUI_APP, EVENT_LEV, "CUI��Proxy����\n");
            g_cCuiDataMgr.m_cCuiState.m_bConnectProxy = false;
        }
        break;
    case OSP_SIP_DISCONNECT:
        {
            COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
            INSTLOG(CUI_APP, EVENT_LEV, "CUI�������[%s]����\n",
                pcOspSipMsg->GetMsgBody());
        }
        break;
    case CU_CUI_WEBSERVICE_REQ:
        {
            //�ܾ�������Ϣ
            OnIdleCuRequestMsg(pcMsg);
        }
        break;
    case CUI_REG_CMU_TIMER:
        {
            SetTimer(CUI_WAIT_CMU_REG_RSP_TIMER, CUI_REG_CMU_INTERVAL*5);

            CCuiRegReq cCuiRegReq;
            cCuiRegReq.SetDevUri(g_cCuiConfig.GetLocalURI().GetURIString());
            CIpAddrList cIpAddrList;
            list<string> lstIp;
            if (!GetOnlineIpList(lstIp) ||lstIp.empty())
            {
                INSTLOG(ERROR_LEV, "Failed to GetOnlineIpList!\n");
                break;
            }
            
            vector<CCommonNetNatInfo> vecNat = ::GetCommonNetNatInfo();

            TNetAddr tNetAddr;
            for (list<string>::const_iterator it = lstIp.begin(); it != lstIp.end(); ++it)
            {
                tNetAddr.SetNetIp(*it);
                tNetAddr.SetNetPort(::GetHttpPort());
                cIpAddrList.push_back(tNetAddr);

                for (vector<CCommonNetNatInfo>::const_iterator it2 = vecNat.begin(); it2 != vecNat.end(); ++it2)
                {
                    if (it2->platformIP == tNetAddr.GetNetIp() 
                        && it2->platformPort == tNetAddr.GetNetPort())
                    {
                        tNetAddr.SetNetIp(it2->platformNatIP);
                        tNetAddr.SetNetPort(it2->platformNatPort);
                        cIpAddrList.push_back(tNetAddr);
                        break;
                    }
                }
            }
			m_tCuiNetSegInfo = GetNetSegmentInfo();

            cCuiRegReq.SetIpAddrList(cIpAddrList);
            cCuiRegReq.SetIsSupportNA(true);
			cCuiRegReq.SetNetSegmentInfo(m_tCuiNetSegInfo);

            TOspTransation tOspTrans;
            tOspTrans.dwIID = MAKEIID(GetAppID(), GetInsID());
            tOspTrans.dwTaskID = INVALID_TASKNO;
            tOspTrans.dwSeqNO = (u32)cCuiRegReq.GetSeqNum();
            OspSipPostReq(KDSIP_EVENT_MESSAGE_REQ, cCuiRegReq, g_cCuiConfig.GetCmuURI(), tOspTrans);

            INSTLOG(CUI_APP, EVENT_LEV, "CUI��CMUע��, ������ַ[%s: %s ]��������ַ[%s]\n", 
                cCuiRegReq.GetDevUri().c_str(), tNetAddr.ToString().c_str(),
                g_cCuiConfig.GetCmuURI().GetURIString().c_str());
        }
        break;
    case CUI_WAIT_CMU_REG_RSP_TIMER:
        {
            INSTLOG(CUI_APP, ERROR_LEV, "CUIע��CMU��ʱ, %d�������\n",
                CUI_REG_CMU_INTERVAL/1000);
            SetTimer(CUI_REG_CMU_TIMER, CUI_REG_CMU_INTERVAL);
        }
        break;
    case OSP_SIP_MSG_PROC_FAIL:
        {
            //�Զ˲����߻��ߴ���SIP��Ϣʧ�ܣ��ظ�����Ϣ
            //Idle״̬�²�����
        }
        break;
    case CUI_REG_RSP:
        {
            COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
            CCuiRegRsp cCuiRegRsp;
            pcOspSipMsg->GetMsgBody(cCuiRegRsp);

            if ( cCuiRegRsp.GetErrorCode() != CMS_SUCCESS )
            {
                INSTLOG(CUI_APP, ERROR_LEV, "CUIע��CMUʧ��, ������ %d\n", 
                    cCuiRegRsp.GetErrorCode());
                SetTimer(CUI_REG_CMU_TIMER, CUI_REG_CMU_INTERVAL);
                return;
            }

            KillTimer(CUI_REG_CMU_TIMER);
            KillTimer(CUI_WAIT_CMU_REG_RSP_TIMER);
            INSTLOG(CUI_APP, EVENT_LEV, "CUIע��CMU�ɹ�, CUI�������̬\n");

            //ע��ɹ�
            TOspTransID tOspTransId;
            tOspTransId.dwIID = MAKEIID(GetAppID(), GetInsID());
            tOspTransId.dwTaskID = INVALID_TASKNO;
            tOspTransId.dwSeqNO = INVALID_SEQ_NO;
            OspSipSetHBParam(g_cCuiConfig.GetCmuURI(), tOspTransId);

            CreateCmuListSubscribeTask(cCuiRegRsp.GetSession());
            NextState(Service);
            RegAllLocalServer();
            g_cCuiDataMgr.SetCuiStateHappenTime();
            g_cCuiDataMgr.m_cCuiState.m_strSession = cCuiRegRsp.GetSession();
			UpdateRegNetSeg();
        }
        break;
    case TAS_LOGOUT_RSP:
    case CU_LOGOUT_RSP:
    case TAS_CU_OFFLINE_RSP:
    case CU_DISCONNECT_NTF_RSP:
        {
            //���ô���
        }
        break;
    default:
        {
            INSTLOG(CUI_APP, WARNING_LEV, "CUI��δ����Proxy�ɹ�, �޷��ṩ����, �յ�δ֪��Ϣ[%s][%hu]\n", 
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            //CMS_ASSERT(0 && "[CCuiInst::OnIdle]CUI�ڿ���̬�յ�δ֪����");
        }
        break;
    }
}

void CCuiInst::OnService(CMessage* const pcMsg)
{
    COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;

    switch (pcMsg->event)
    {
	case CUI_BAT_OPT_TASK_NTF:
	case CUI_BAT_OPT_TASK_TIMER:
		{
			//����ʱ������
			u32 dwTaskNO = *(u32*)pcMsg->content;
			CTask* pcTask = FindTask(dwTaskNO);
			if (pcTask != NULL)
			{
				ProcTaskMsg(pcTask, pcMsg);
			}
			else
			{
				INSTLOG(ERROR_LEV, "taskNO[%u]û�ж�Ӧ����---msg[%s-%u]\n", 
					dwTaskNO, OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
			}
		}
		break;
    case OSP_PROXY_DISCONNECT:
        {
            INSTLOG(CUI_APP, EVENT_LEV, "CUI��Proxy�����������κδ����ȴ�CMU��ҵ��ģ������ٴ���\n");
            g_cCuiDataMgr.m_cCuiState.m_bConnectProxy = false;
            //             ClearAllTask();
            //             NextState(Idle);
        }
        break;
    case OSP_PROXY_CONNECT:
        {
            INSTLOG(CUI_APP, EVENT_LEV, "CUI��Proxy�ָ��������ڼ�ҵ��ģ�鲻��֪\n");
            g_cCuiDataMgr.m_cCuiState.m_bConnectProxy = true;
        }
        break;
    case OSP_SIP_DISCONNECT:
        {
            OnSipUADisconnect(pcMsg);
        }
        break;
    case INVITE_REQ:
        {
            INSTLOG(CUI_APP, EVENT_LEV, "�յ��������е�Invite_Req����\n");

            CInviteReq cReq;
            pcOspSipMsg->GetMsgBody(cReq);

            CCuAgent *pcCuAgent = (CCuAgent *)g_cCuiDataMgr.GetTaskPtrBySession(cReq.GetSrcChn().GetDevUri(), this);
            if(NULL != pcCuAgent)
            {
                u32 dwTaskNo = INVALID_TASKNO;
                pcCuAgent->m_cCallPuTaskSet.Find(cReq.GetSrcChn().GetChnNO(), dwTaskNo);
                if( INVALID_TASKNO != dwTaskNo )
                {
                    CSipTask * pcCallPuTask = (CSipTask *)FindTask(dwTaskNo);
                    if ( NULL != pcCallPuTask )
                    {
                        u32 dwRet = pcCallPuTask->ProcMsg(pcMsg);
                        if(PROCMSG_DEL == dwRet)
                        {
                            delete pcCallPuTask;
                            pcCallPuTask = NULL;
                        }
                        return;
                    }
                }               
            }

            CInviteRsp cRsp;
            cRsp.SetErrorCode(ERR_CUI_TASK_DEL);
            cRsp.SetSeqNum(cReq.GetSeqNum());
            cRsp.SetSession(cReq.GetSession());

            TOspTransation tOspTrans;
            tOspTrans.dwIID = MAKEIID(GetAppID(), GetInsID());
            tOspTrans.dwTaskID = INVALID_TASKNO;

            OspSipPostRsp(KDSIP_EVENT_INVITE_RSP, pcOspSipMsg->GetSipTransID(), cRsp, tOspTrans);
            //����ʧ�ܺ�cmu���漴�ٷ�һ��ByeReq�����Լ���
        }
        break;
    case OSP_SIP_BYE_REQ:
        {
            INSTLOG(CUI_APP, EVENT_LEV, "�յ�InviteByeReq���󣬽���Dialog[%u]\n", 
                pcOspSipMsg->GetSipDlgID());

            CSipTask * pcInviteTask = (CSipTask *)FindTask(pcOspSipMsg->GetOspTaskNO());
            if ( NULL != pcInviteTask )
            {
                pcInviteTask->ProcMsg(pcMsg);
            }
            else
            {
                INSTLOG(CUI_APP, EVENT_LEV, "InviteByeReq�Ҳ���Dialog[%u]��Ӧ��TaskNO[%u]����Task�����ڣ�ֱ�ӻظ�Ӧ��\n",
                    pcOspSipMsg->GetSipDlgID(), pcOspSipMsg->GetOspTaskNO());
                TOspTransation tOspTrans;
                tOspTrans.dwIID = MAKEIID(GetAppID(), GetInsID());
                tOspTrans.dwTaskID = INVALID_TASKNO;

                string strNullMsg;
                OspSipPostRsp(KDSIP_EVENT_INVITE_BYE_RSP, pcOspSipMsg->GetSipTransID(), strNullMsg, tOspTrans);
            }
        }
        break;
    case OSP_SIP_BYE_RSP:
        {
            INSTLOG(CUI_APP, EVENT_LEV, "�յ�BYE_RSP���󣬽���TaskNO[%u]��Dialog[%u]\n", 
                pcOspSipMsg->GetOspTaskNO(), pcOspSipMsg->GetSipDlgID());
            CSipTask * pcInviteTask = (CSipTask *)FindTask(pcOspSipMsg->GetOspTaskNO());
            if ( NULL != pcInviteTask )
            {
                pcInviteTask->ProcMsg(pcMsg);
            }
            else
            {
                INSTLOG(CUI_APP, ERROR_LEV, "BYE_RSP�Ҳ���Dialog[%u]��Ӧ��TaskNO[%u]����Task������\n",
                    pcOspSipMsg->GetSipDlgID(), pcOspSipMsg->GetOspTaskNO());
            }
        }
        break;
    case OSP_SIP_MSG_PROC_FAIL:
    case OSP_SIP_DIALOG_TERMINATE:
        {
            //�Զ˲����߻��ߴ���SIP��Ϣʧ�ܣ��ظ�����Ϣ
            CSipTask *pcTask = (CSipTask *)FindTask(pcOspSipMsg->GetOspTaskNO());
            if ( NULL != pcTask )
            {
                //todo ����ط��Ĵ�ӡ������
                string strEventReq = GetXmlKeyText(pcOspSipMsg->GetMsgBody(), EVENT_BEGIN, EVENT_END);
                INSTLOG(CUI_APP, ERROR_LEV, "SIP��Ϣ[%s]����ʧ�ܣ��յ������[%d]Ӧ��, ����TaskNO[%u]����\n", 
                    strEventReq.c_str(), pcOspSipMsg->GetSipErrorCode(), pcOspSipMsg->GetOspTaskNO());
                pcTask->ProcMsg(pcMsg);
            }
            else
            {
                INSTLOG(CUI_APP, ERROR_LEV, "�յ� OSP_SIP_MSG_PROC_FAIL ��Ϣ�������Ҳ���TaskNO[%u]��Ӧ�����񣬿���������Ϣ������Task���� ���� Task�Ѿ�������\n",
                    pcOspSipMsg->GetOspTaskNO());
            }
        }
        break;
    case CU_CUI_WEBSERVICE_REQ:
        {
            //����CU������
            OnServiceCuRequestMsg(pcMsg);
        }
        break;
        //////////////////////////////////////////////////////////////////////////
    case TAS_LOGIN_RSP:
    case TAS_CU_ONLINE_RSP:
    case TAS_DOMAIN_NAME_GET_RSP:
    case TAS_DOMAIN_RELATION_QRY_RSP:
    case TAS_CUSER_QRY_RSP:
    case TAS_CUSER_GROUP_QRY_RSP:
    case CU_LOGIN_RSP:
    case CU_GETCUILIST_RSP:
	case PU_LONG_OSD_SET_RSP:
	case PU_LONG_OSD_GET_RSP:
    case PU_PTZ_CTRL_RSP:
    case PU_PTZ_LOCK_RSP:
	case PU_PTZ_UNLOCK_RSP:
	case TAS_PTZLOCK_ADD_RSP:
	case TAS_PTZLOCK_DEL_RSP:
	case TAS_PTZLOCK_QRY_RSP:
    case PU_TRANSPARENT_OPERATE_RSP:
    case KEY_FRAME_RSP:
    case PU_VSIP_OPERATE_RSP:
	case TAS_DEVICE_GROUP_QRY_RSP:
	case TAS_DEVTREE_QRY_RSP:
	case TAS_DEVICE_GROUPPATH_QRY_RSP:
    case TAS_CUSER_DEVICEFULLINFO_AND_PRIVILEGE_QRY_RSP:
    case RCS_LIST_SUBSCRIBE_PU_STATUS_RSP:
    case RCS_LIST_REFRESH_SS_PU_STATUS_RSP:
    case PULIST_STATUS_SS_RSP:
    case PULIST_STATUS_REFRESH_SS_RSP:
    case MPS_PU_GPS_SS_RSP:
    case MPS_PU_GPS_REFRESH_SS_RSP:
    case TAS_DEVICE_GROUP_DEVICE_QRY_RSP:
    case TAS_DEVICE_GROUP_ADD_RSP:
    case TAS_DEVICE_GROUP_DEL_RSP:
    case TAS_DEVICE_GROUP_MOD_RSP:
	case TAS_DEVICE_GROUP_MOD_4GB_RSP:
    case TAS_DEVICE_GROUP_DEVICE_ORDER_RSP:
    case TAS_DEVICE_GROUP_DEVICE_ADD_RSP:
	case TAS_DEVICE_GROUP_DEVICE_DEL_RSP:
	case TAS_DEVTREE_ADD_RSP:
	case TAS_DEVTREE_MOD_RSP:
	case TAS_DEVTREE_DEL_RSP:
    case TAS_CUSER_GROUP_ADD_RSP:
    case TAS_CUSER_GROUP_DEL_RSP:
    case TAS_CUSER_GROUP_MOD_RSP:
    case TAS_CUSER_ADD_RSP:
    case TAS_CUSER_DEL_RSP:
    case TAS_CUSER_MOD_RSP:
    case TAS_CUSER_DEVICE_QRY_RSP:
    case TAS_CUSER_GROUP_DEVICE_QRY_RSP:
	case TAS_DEVICE_GROUP_DEVICE_QRY_4GB_RSP:
    case TAS_CUSER_DEVICE_ADD_RSP:
    case TAS_CUSER_GROUP_DEVICE_ADD_RSP:
    case TAS_CUSER_DEVICE_DEL_RSP:
    case TAS_CUSER_GROUP_DEVICE_DEL_RSP:
    case TAS_CUSER_DEVICE_MOD_RSP:
    case TAS_CUSER_GROUP_DEVICE_MOD_RSP:
    case TAS_TEMP_AUTHORIZATION_QRY_RSP:
    case TAS_TEMP_AUTHORIZATION_ADD_RSP:
    case TAS_TEMP_AUTHORIZATION_DEL_RSP:
    case TAS_TEMP_AUTHORIZATION_MOD_RSP:
    case TAS_DEVICE_QRY_RSP:
    case TAS_DEVICE_MOD_RSP:
    case TAS_CUSER_DEVGRP_VIDSRC_QRY_RSP:
    case TAS_DEVICE_CAP_INDEX_NAME_SET_RSP:
    case TAS_DEVICE_CAP_INDEX_NAME_GET_RSP:
	case TAS_CUSER_DEVICE_GROUP_ADD_RSP:
	case TAS_CUSER_DEVICE_GROUP_DEL_RSP:
	case TAS_DEVTREE_DEVGROUP_ASSIGN_RSP:
	case TAS_CUSER_GROUP_DEVICE_GROUP_ADD_RSP:
	case TAS_CUSER_GROUP_DEVICE_GROUP_DEL_RSP:
	case TAS_DEVICE_AUTO_ASSIGN_TO_USER_SET_RSP:
	case TAS_CUSER_DATA_VERSION_GET_RSP:
	case TAS_DEVICE_AUTO_ASSIGN_TO_USER_GROUP_SET_RSP:
	case TAS_DEVICE_AUTO_ASSIGN_TO_USER_QRY_RSP:
	case TAS_DEVICE_AUTO_ASSIGN_TO_USER_GROUP_QRY_RSP:
	case TAS_TRANSACTION_PROGRESS_GET_RSP:
	case TAS_DEVGRP_VIDSRC_TOTALNUM_GET_RSP:
	case TAS_VIDSRC_INFO_QRY_RSP:
	case TAS_DEVICE_MATCH_GBID_RSP:
	case TAS_DEVICE_GROUP_DEVICE_SUMNUM_RSP:
	case TAS_DEVICE_GROUP_DEVICE_MOD_4GB_RSP:
	case TAS_DEVICE_GBID_ERROR_GET_RSP:
	case TAS_DEVICE_GBINFO_QRY_RSP:
	case TAS_DEVGRP_GBINFO_QRY_RSP:
    case INVITE_RSP:
    case INVITE_ACK:
	case FILE_EXPORT_RSP:
	case FILE_IMPORT_RSP:
	case FILE_COLLECT_PROGRESS_NTF_RSP:
	case FILE_TRANS_END_RSP:
        /*********************************************����ģ��һ����task********************************************************/
    case TV_POLL_OPERATE_RSP:
    case TVWALL_SCHEME_SAVE_RSP:
    case TVWALL_SCHEME_DEL_RSP:
    case TVWALL_SCHEME_LOAD_RSP:
    case TVWALL_SCHEME_GROUP_SET_RSP:
    case TVWALL_SCHEME_GROUP_GET_RSP:
    case TVWALL_SCHEME_POLL_OPERATE_RSP:
    case MPS_MAP_CREATE_RSP:
    case MPS_MAP_MOD_RSP:
    case MPS_MAP_DEL_RSP:
    case MPS_MAP_ELEMENT_CREATE_RSP:
    case MPS_MAP_ELEMENT_MOD_RSP:
    case MPS_MAP_ELEMENT_DEL_RSP:
    case MPS_POS_CORRECT_MARS_TO_EARTH_RSP:
    case MPS_POS_CORRECT_EARTH_TO_MARS_RSP:
    case MPS_DATA_VERSION_GET_RSP:
	case MPS_GPS_DATA_BAKEUP_RSP:
    case MPS_MAP_UPLOAD_RSP:
    case MPS_ELEMENT_ICON_UPLOAD_RSP:
    case ALARMLINK_VERSION_GET_RSP:
    case ALARMLINK_CFG_GET_RSP:
    case ALARMLINK_CFG_SET_RSP:
    case UAS_CUSER_CUSTOMDATA_ADD_RSP:
    case UAS_CUSER_CUSTOMDATA_DEL_RSP:
    case UAS_CONFIG_UAS_GET_RSP:
    case UAS_CONFIG_UAS_SET_RSP:
	case GBS_GBDOMAIN_PARENT_PRIVILEGE_GET_RSP:
	case GBS_GBDOMAIN_PARENT_PRIVILEGE_SET_RSP:
	case GBS_OPTION_CONF_GET_RSP:
	case GBS_OPTION_CONF_SET_RSP:
	case GBS_CERTIFICATE_UPLOAD_RSP:
	case GBS_CERTIFICATE_GET_RSP:
	case GBS_DEV_GBID_GET_RSP:
	case GBS_GBDEV_TELEBOOT_RSP:
	case GBS_GBPARENTDOMAIN_GET_RSP:
	case GBS_GBPARENTDOMAIN_CATALOG_LOAD_RSP:
	case GBS_DEVGRP_CIVILCODE_GET_RSP:
	case GBS_DEV_KDID_GET_RSP:
	case GBS_CATALOG_SELECT_REPORT_START_RSP:
	case GBS_SAFE_AUTHINFO_CHECK_FIRST_RSP:
	case GBS_SAFE_AUTHINFO_CHECK_SECOND_RSP:
	case RECORD_ON_TV_RSP:
	case RECORD_OFF_TV_RSP:
    case REAL_MEDIA_ON_TV_RSP:
	case REAL_MEDIA_SWITCH_OCCUR_TYPE_RSP:
    case REAL_MEDIA_OFF_TV_RSP:
    case RCS_QUERY_RECORD_RSP:
    case RCS_QUERY_NRU_RSP:
    case RCS_QUERY_PUCHN_BINDING_RSP:
    case RCS_QUERY_RECORD_PARAM_RSP:
    case TVWALL_SCHEME_QUERY_RSP:
    case MPS_MAP_QRY_RSP:
    case MPS_MAP_ELEMENT_QRY_RSP:
	case MPS_MAP_ELEMENT_SEARCH_RSP:
    case MPS_MAP_AREA_QRY_RSP:
    case MPS_ELEMENT_IMAGE_QRY_RSP:
	case MPS_GPS_TRACE_QRY_RSP:
    case ALARMLINK_QUERY_RSP:
    case UAS_OPERATE_LOG_QRY_RSP:
    case UAS_DEVICE_LOG_QRY_RSP:
    case UAS_VIDEOSOURCE_PRESETCFG_QRY_RSP:
    case UAS_CUSER_CUSTOMDATA_QRY_RSP:
    case UAS_ALARM_LOG_QRY_RSP:
    case RCS_QUERY_DISK_INFO_RSP:
    case RCS_SET_RECORD_TASK_RSP:
	case RCS_RECORD_LOCK_OPR_RSP:
    case RCS_GET_SYSPARAM_RSP:
    case RCS_SET_SYSPARAM_RSP:
    case RCS_BINDING_PUCHN_RSP:
    case RCS_UNBINDING_PUCHN_RSP:
	case PU_RECORD_START_RSP:
	case PU_RECORD_STOP_RSP:
    case PU_SERIALPARAM_CFG_GET_RSP:
    case PU_SERIALPARAM_CFG_SET_RSP:
    case START_THIRD_PARTY_CALL_RSP:
    case STOP_THIRD_PARTY_CALL_RSP:
    case VCR_CTRL_RSP:
	case UPDATE_SWITCH_ADDRESS_RSP:
    case RCS_TASK_START_RSP:
    case PLAYBACK_PROGRESS_QUERY_RSP:
    case DEV_LOGIN_RSP:
    case TVWALL_STATUS_SS_RSP:
    case TVWALL_STATUS_UNSS_RSP:
    case TAS_TVWALL_QRY_RSP:
    case TAS_TVWALL_ADD_RSP:
    case TAS_TVWALL_MOD_RSP:
    case TAS_TVWALL_DEL_RSP:
	case CONFIG_SYNC_SWITCH_SET_RSP:
	case CONFIG_SYNC_SWITCH_GET_RSP:
    case PU_VID_ENC_PARAM_GET_RSP:
    case PU_VID_ENC_PARAM_SET_RSP:
	case PU_AUD_ENC_PARAM_GET_RSP:
	case PU_CAMERA_TYPE_SET_RSP:
	case PU_ALARM_RESET_RSP:
	case PU_VIDSRC_OSD_SET_RSP:
	case PU_VIDSRC_OSD_GET_RSP:
    case RCS_QUERY_RECORD_DAYS_RSP:
    case RCS_SET_RECORD_PARAM_RSP:
    case PU_MOTION_DETECT_GET_RSP:
    case PU_MOTION_DETECT_SET_RSP:
    case PU_AREA_SHADE_GET_RSP:
    case PU_AREA_SHADE_SET_RSP:
    case PU_REC_DAYS_QRY_RSP:
	case SECURITY_MODULE_CERTIFICATE_QUERY_RSP:
    case TV_WIN_STYLE_SET_RSP:
    case UAS_OPERATE_LOG_ADD_RSP:
    case MPS_GET_SUPPORT_MAP_TYPES_RSP:
    case MPS_MAP_AREA_CREATE_RSP:
    case MPS_MAP_AREA_MOD_RSP:
    case UAS_VIDEOSOURCE_PRESETCFG_ADD_RSP:
    case UAS_VIDEOSOURCE_PRESETCFG_DEL_RSP:
    case PU_REC_QRY_RSP:
	case PU_TRANSPARENT_QRY_RSP:
	case CAPS_GET_CONFIG_RSP:
	case CAPS_SET_CONFIG_RSP:
	case CAPS_QUERY_DEVICE_LIST_RSP:
	case CAPS_ADD_DEVICE_RSP:
	case CAPS_MODIFY_DEVICE_RSP:
	case CAPS_DEL_DEVICE_RSP:
	case CMU_PU_OFFLINEDEV_QRY_RSP: // cache_query����
	case UPDATE_CULOGIN_INFO_RSP:
	case DPS_SEARCH_GET_RESULT_RSP:
	case DPS_SEARCH_GET_RESULT_BY_GROUP_RSP:
	case DPS_GET_DEVICE_STATUS_RSP:
	case DPS_SEARCH_RSP:
	case DPS_FILTER_EMPTY_DEVICE_GROUP_RSP:
        {
            //�ҵ�CUI��Task��ע�⣬���ﲻ����SessionID������Task����Ϊ��CuAgent�з���3acAgent����Ϣ, Session��¼����3ACAgent��
            //CUI��CU����SIP�����Բ����ߵ�����

            const u32 dwTaskNo = pcOspSipMsg->GetOspTaskNO();

            INSTLOG(CUI_APP, EVENT_LEV, "�յ�Ӧ����Ϣ[%s][%hu]����Ӧ��TaskNOΪ[%u]\n", 
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event, dwTaskNo );
            CSipTask *pcDiscTask = (CSipTask *)FindTask( dwTaskNo );
            if ( NULL != pcDiscTask )
            {
                u32 dwResult = pcDiscTask->ProcMsg(pcMsg);
                if( dwResult == PROCMSG_DEL )
                {
                    //������ʵ�֣� ��task �Լ�����ɾ��������ok
                    //���ڵ�ʵ�֣� ��task ����del cuiapp ����ɾ��
                    //Ϊ�˷�ֹ ��task �Ѿ�ɾ�����ٷ��� del �ظ�ɾ���������ɾ��֮ǰ�ٲ���һ������
                    CSipTask *pcTaskChk = (CSipTask *)FindTask( dwTaskNo );
                    if( pcTaskChk == pcDiscTask )
                    {
                        delete pcTaskChk;
                        pcTaskChk = NULL;//����ָ�����ֵ��ͬ��ɾ��һ������
                        pcDiscTask= NULL;
                    }
                }
            }
            else
            {
                INSTLOG(CUI_APP, ERROR_LEV, "TaskNO[%u]�����ڣ����������Ѿ���ʱ������\n", 
                    pcOspSipMsg->GetOspTaskNO());
            }
        }
        break;
        //////////////////////////////////////////////////////////////////////////
        //CMU, 3AS��ѯ����֪ͨ������Session�ҵ���Ӧ��Task����
    case TAS_DOMAIN_RELATION_QRY_NTF_REQ:
    case TAS_CUSER_QRY_NTF_REQ:
    case TAS_CUSER_GROUP_QRY_NTF_REQ:
    case TAS_DEVICE_GROUP_QRY_NTF_REQ:
	case TAS_DEVTREE_QRY_NTF_REQ:
	case TAS_DEVICE_GROUPPATH_QRY_NTF_REQ:
    case TAS_CUSER_DEVICEFULLINFO_AND_PRIVILEGE_QRY_NTF_REQ:
	case TAS_DEVICE_AUTO_ASSIGN_TO_USER_QRY_NTF_REQ:
	case TAS_DEVICE_AUTO_ASSIGN_TO_USER_GROUP_QRY_NTF_REQ:
	case TAS_PTZLOCK_QRY_NTF_REQ:
    case PU_STATUS_NTF_REQ:
    case PU_CONFIG_NTF_REQ:
    case PLAYBACK_PROGRESS_NTF_REQ:
    case PU_SERVICE_STATUS_NTF_REQ:
    case PU_ALARM_STATUS_NTF_REQ:
    case PU_TRANSDATA_STATUS_NTF_REQ:
    case TVWALL_STATUS_NTF_REQ:
    case RCS_PU_RECORD_STATUS_NTF_REQ:
    case MPS_PU_GPS_NTF_REQ:
    case TAS_DEVICE_GROUP_DEVICE_QRY_NTF_REQ:
    case TAS_CUSER_DEVICE_QRY_NTF_REQ:
    case TAS_CUSER_GROUP_DEVICE_QRY_NTF_REQ:
	case TAS_DEVICE_GROUP_DEVICE_QRY_4GB_NTF_REQ:
    case TAS_TEMP_AUTHORIZATION_QRY_NTF_REQ:
    case TAS_DEVICE_QRY_NTF_REQ:
    case TAS_TVWALL_QRY_NTF_REQ:
    case PU_REC_QRY_NTF_REQ:
    case FILE_COLLECT_PROGRESS_NTF_REQ:
    case STUN_UPDATE_REQ:
	case PU_TRANSPARENT_QRY_NTF_REQ:
	case GBS_GBPARENTDOMAIN_CL_PROGRESS_NTF_REQ:
	case ALARM_EVENT_NTF_REQ:
	case CMU_PU_OFFLINEDEV_QRY_NTF_REQ:
	case TAS_CUSER_DEVGRP_VIDSRC_QRY_NTF_REQ:
	case TAS_VIDSRC_INFO_QRY_NTF_REQ:
	case TAS_DEVICE_GBINFO_QRY_NTF_REQ:
	case TAS_DEVGRP_GBINFO_QRY_NTF_REQ:

        {
			OnServicePlatformNtfMsg(pcMsg);
        }
        break;
    case TAS_DEVICE_ADD_NTF_REQ:
    case TAS_DEVICE_DEL_NTF_REQ:
    case TAS_DEVICE_MOD_NTF_REQ:
    case TAS_DEVICE_CAP_INDEX_NAME_MOD_NTF_REQ:
    case TAS_CUSER_DEVICE_ADD_NTF_REQ:
    case TAS_CUSER_DEVICE_DEL_NTF_REQ:
    case TAS_CUSER_DEVICE_MOD_NTF_REQ:
    case TAS_DEVICE_GROUP_ADD_NTF_REQ:
    case TAS_DEVICE_GROUP_DEL_NTF_REQ:
    case TAS_DEVICE_GROUP_MOD_NTF_REQ:
    case TAS_DEVICE_GROUP_DEVICE_ADD_NTF_REQ:
    case TAS_DEVICE_GROUP_DEVICE_DEL_NTF_REQ:
    case TAS_CUSER_GROUP_DEVICE_ADD_NTF_REQ:
    case TAS_CUSER_GROUP_DEVICE_DEL_NTF_REQ:
    case TAS_CUSER_GROUP_DEVICE_MOD_NTF_REQ:
    case TAS_TEMP_AUTHORIZATION_ADD_NTF_REQ:
    case TAS_TEMP_AUTHORIZATION_DEL_NTF_REQ:
    case TAS_TEMP_AUTHORIZATION_MOD_NTF_REQ:
    case TAS_DOMAIN_RELATION_ADD_NTF_REQ:
    case TAS_DOMAIN_RELATION_DEL_NTF_REQ:
    case TAS_DOMAIN_RELATION_MOD_NTF_REQ:
    case TAS_DEVICE_MODEL_ADD_NTF_REQ:
    case TAS_DEVICE_MODEL_DEL_NTF_REQ:
    case TAS_CUSER_ADD_NTF_REQ:
    case TAS_CUSER_DEL_NTF_REQ:
    case TAS_CUSER_MOD_NTF_REQ:
    case TAS_CUSER_GROUP_ADD_NTF_REQ:
    case TAS_CUSER_GROUP_DEL_NTF_REQ:
    case TAS_CUSER_GROUP_MOD_NTF_REQ:
    case TAS_TACUSER_ADD_NTF_REQ:
    case TAS_TACUSER_DEL_NTF_REQ:
    case TAS_TACUSER_MOD_NTF_REQ:
    case RCS_QUERY_RECORD_NTF_REQ:
    case RCS_QUERY_NRU_NTF_REQ:
    case RCS_QUERY_PUCHN_BINDING_NTF_REQ:
    case RCS_QUERY_RECORD_PARAM_NTF_REQ:
    case TVWALL_SCHEME_NTF_REQ:
    case MPS_MAP_QRY_NTF_REQ:
    case MPS_MAP_ELEMENT_QRY_NTF_REQ:
	case MPS_MAP_ELEMENT_SEARCH_NTF_REQ:
    case MPS_MAP_AREA_QRY_NTF_REQ:
    case MPS_ELEMENT_IMAGE_QRY_NTF_REQ:
	case MPS_MAP_ADD_NTF_REQ:
	case MPS_MAP_MOD_NTF_REQ:
	case MPS_MAP_DEL_NTF_REQ:
	case MPS_MAP_LAYER_ADD_NTF_REQ:
	case MPS_MAP_LAYER_MOD_NTF_REQ:
	case MPS_MAP_LAYER_DEL_NTF_REQ:
	case MPS_MAP_AREA_ADD_NTF_REQ:
	case MPS_MAP_AREA_MOD_NTF_REQ:
	case MPS_MAP_AREA_DEL_NTF_REQ:
	case MPS_MAP_ELEMENT_ADD_NTF_REQ:
	case MPS_MAP_ELEMENT_MOD_NTF_REQ:
	case MPS_MAP_ELEMENT_DEL_NTF_REQ:
	case MPS_GPS_TRACE_QRY_NTF_REQ:
    case ALARMLINK_NTF_REQ:
    case UAS_OPERATE_LOG_QRY_NTF_REQ:
    case UAS_DEVICE_LOG_QRY_NTF_REQ:
    case UAS_VIDEOSOURCE_PRESETCFG_QRY_NTF_REQ:
    case UAS_ALARM_LOG_QRY_NTF_REQ:
    case UAS_CUSER_CUSTOMDATA_QRY_NTF_REQ:
    case RCS_QUERY_DISK_INFO_NTF_REQ:
    case TAS_TVWALL_ADD_NTF_REQ:
    case TAS_TVWALL_DEL_NTF_REQ:
    case TAS_TVWALL_MOD_NTF_REQ:
    case END_OF_FILE_NTF_REQ:
    case ALARMLINK_MOD_NTF_REQ:
	case CAPS_QUERY_DEVICE_LIST_NTF_REQ:
	case MAPPING_TABLE_NTF_REQ:
	case TAS_PTZLOCK_ADD_NTF_REQ:
	case TAS_PTZLOCK_DEL_NTF_REQ:
	case DPS_SEARCH_GET_RESULT_NTF_REQ:
	case DPS_SEARCH_PROGRESS_NTF_REQ:
        {
            OnServicePlatformNtfMsg(pcMsg);
        }
        break;
        //////////////////////////////////////////////////////////////////////////
    case TAS_LOGOUT_RSP:
    case CU_LOGOUT_RSP:
    case TAS_CU_OFFLINE_RSP:
    case CU_DISCONNECT_NTF_RSP:
        {
            //���ô���
        }
        break;
    case CUI_REG_CMU_TIMER:
    case CUI_REG_RSP:
        {
            //��IDLE״̬��������Ϣ��������
        }
        break;
	case CUI_UPDATE_NETSEG_TIMER:
		{
			UpdateRegNetSeg();
		}
		break;
	case UPDATE_REG_NETSEG_RSP:
		{
			INSTLOG(CUI_APP, ERROR_LEV, "cui�������ε�ַ�б�ɹ�.\n");
		}
		break;
	case CUI_CLEAR_VTDUNATPACKET_TIMER:
		{
			if ( !g_cuSessionForNatPacketClear.empty() )
			{
				// ֻȡ��һ��cusession
				std::vector<std::string>::iterator iter = g_cuSessionForNatPacketClear.begin();
				std::string cuSession = *iter;

				g_cuSessionForNatPacketClear.erase(iter);

				u32 dwTaskID = INVALID_TASKNO;
				g_cCuiDataMgr.m_mapSessToTaskMap.Find(cuSession, dwTaskID);
				if ( dwTaskID == INVALID_TASKNO )
				{
					//INSTLOG(CUI_APP, EVENT_LEV, "�ӳ����CU:%s��NAT̽���\n",cuSession.c_str());
					RemoveNatData(cuSession, 0);
				}
				else
				{
					INSTLOG(CUI_APP, EVENT_LEV, "���������ʹ�õ�CU:%sNAT̽���\n",cuSession.c_str());
				}
			}
			break;
		}
    default:
        {
            INSTLOG(CUI_APP, ERROR_LEV, "CUI�ڷ���̬�յ�δ֪��Ϣ[%s--%hu]\n", 
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            //CMS_ASSERT(0 && "CUI�ڷ���̬�յ�δ֪����");
            //�����cu��Ϣ�ǲ��ܻظ�403��
            COspSipMsg* pSipMsg = (COspSipMsg*)pcMsg->content;

            TOspTransID tOspTrasnID;
            tOspTrasnID.dwIID = GetOspIID();
            string strEmpty;
            OspSipPostRsp(pSipMsg->GetSipRsp(), pSipMsg->GetSipTransID(), strEmpty, 
                tOspTrasnID, KDSIP_RESPONSE_403_FORBIDDEN);
        }
        break;
    }
}

void CCuiInst::OnServicePlatformNtfMsg(CMessage* const pcMsg)
{
    COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
    string strSessionID = GetSipSessionID(pcOspSipMsg->GetMsgBody());

    INSTLOG(CUI_APP, EVENT_LEV, "�յ�������[%s]��֪ͨ��Ϣ[%s--%hu], Ŀ��CU Session[%s]��SIP DialogID[%d], TaskID[%d]\n", 
        pcOspSipMsg->GetSipFromUri().GetURIString().c_str(),
        OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event,
        strSessionID.c_str(), pcOspSipMsg->GetSipDlgID(), pcOspSipMsg->GetOspTaskNO());

    switch (pcMsg->event)
    {
        //////////////////////////////////////////////////////////////////////////
        //CMU��֪ͨ��Ϣ
	case MAPPING_TABLE_NTF_REQ:		
		{
			ProcMappingTableNtfReq(pcMsg);
		}
		break;
    case PU_STATUS_NTF_REQ:
    case PU_CONFIG_NTF_REQ:
    case PU_SERVICE_STATUS_NTF_REQ:
    case PU_ALARM_STATUS_NTF_REQ:
    case PU_TRANSDATA_STATUS_NTF_REQ:
    case TVWALL_STATUS_NTF_REQ:
    case RCS_PU_RECORD_STATUS_NTF_REQ:
    case MPS_PU_GPS_NTF_REQ:
        {
            CSipTask *pcTask = (CSipTask *)FindTask(pcOspSipMsg->GetOspTaskNO());
            if ( NULL != pcTask )
            {
                pcTask->ProcMsg(pcMsg);
                return;
            }

            INSTLOG(CUI_APP, ERROR_LEV, "���棡�Ҳ���������[%s]�ĻỰ����Ϣ[%s--%hu]�Ľ���Task, SIP DialogID[%d], TaskID[%d]������\n",
                pcOspSipMsg->GetSipFromUri().GetURIString().c_str(),
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event,
                pcOspSipMsg->GetSipDlgID(), pcOspSipMsg->GetOspTaskNO());

            //�Ҳ�����Ӧ��task, �ܾ���ntf
            CNtfRsp cRsp;
            cRsp.SetEvent(pcMsg->event + 1);
            cRsp.SetSession(strSessionID);
            string strSeqNum = GetSipSeqNO(pcOspSipMsg->GetMsgBody());
            int nSeqNum = static_cast<int>(atol(strSeqNum.c_str()));
            cRsp.SetSeqNum(nSeqNum);
            TOspTransID tSrcTransId;
            tSrcTransId.dwIID = MAKEIID(GetAppID(), GetInsID());
            tSrcTransId.dwSeqNO = (u32)nSeqNum;
            tSrcTransId.dwTaskID = INVALID_TASKNO;
            OspSipPostRsp(KDSIP_EVENT_NOTIFY_RSP, pcOspSipMsg->GetSipTransID(), cRsp, tSrcTransId);
            return;
        }
        break;
	case DPS_SEARCH_GET_RESULT_NTF_REQ:
	case DPS_SEARCH_PROGRESS_NTF_REQ:
	case END_OF_FILE_NTF_REQ:
	case PLAYBACK_PROGRESS_NTF_REQ:
	case FILE_COLLECT_PROGRESS_NTF_REQ:
        {
            CSipTask *pcTask = (CSipTask *)FindTask(pcOspSipMsg->GetOspTaskNO());
            if ( NULL != pcTask )
            {
                pcTask->ProcMsg(pcMsg);
                return;
            }

            INSTLOG(CUI_APP, ERROR_LEV, "���棡�Ҳ���������[%s]�ĻỰ����Ϣ[%s--%hu]�Ľ���Task, SIP DialogID[%d], TaskID[%d]������\n",
                pcOspSipMsg->GetSipFromUri().GetURIString().c_str(),
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event,
                pcOspSipMsg->GetSipDlgID(), pcOspSipMsg->GetOspTaskNO());

            //�Ҳ�����Ӧ��task, �ܾ���ntf
            CNtfRsp cRsp;
            cRsp.SetEvent(pcMsg->event + 1);
            cRsp.SetSession(strSessionID);
            string strSeqNum = GetSipSeqNO(pcOspSipMsg->GetMsgBody());
            int nSeqNum = static_cast<int>(atol(strSeqNum.c_str()));
            cRsp.SetSeqNum(nSeqNum);
            TOspTransID tSrcTransId;
            tSrcTransId.dwIID = MAKEIID(GetAppID(), GetInsID());
            tSrcTransId.dwSeqNO = (u32)nSeqNum;
            tSrcTransId.dwTaskID = INVALID_TASKNO;
            OspSipPostRsp(KDSIP_EVENT_MESSAGE_DIALOG_RSP, pcOspSipMsg->GetSipTransID(), cRsp, tSrcTransId);
            return;
        }
        break;
    
	//////////////////////////////////////////////////////////////////////////
        //cmu
	case PU_TRANSPARENT_QRY_NTF_REQ:
		//TAS�Ĳ�ѯ֪ͨ
    case TAS_DOMAIN_RELATION_QRY_NTF_REQ:
    case TAS_CUSER_QRY_NTF_REQ:
    case TAS_CUSER_GROUP_QRY_NTF_REQ:
	case TAS_DEVICE_GROUP_QRY_NTF_REQ:
	case TAS_DEVTREE_QRY_NTF_REQ:
	case TAS_DEVICE_GROUPPATH_QRY_NTF_REQ:
    case TAS_CUSER_DEVICEFULLINFO_AND_PRIVILEGE_QRY_NTF_REQ:
    case TAS_DEVICE_GROUP_DEVICE_QRY_NTF_REQ:
    case TAS_CUSER_DEVICE_QRY_NTF_REQ:
    case TAS_CUSER_GROUP_DEVICE_QRY_NTF_REQ:
	case TAS_DEVICE_GROUP_DEVICE_QRY_4GB_NTF_REQ:
	case TAS_TEMP_AUTHORIZATION_QRY_NTF_REQ:
	case TAS_DEVICE_AUTO_ASSIGN_TO_USER_QRY_NTF_REQ:
	case TAS_DEVICE_AUTO_ASSIGN_TO_USER_GROUP_QRY_NTF_REQ:
    case TAS_DEVICE_QRY_NTF_REQ:
	case TAS_PTZLOCK_QRY_NTF_REQ:
    case TAS_CUSER_DEVGRP_VIDSRC_QRY_NTF_REQ:
    case RCS_QUERY_RECORD_NTF_REQ:
    case RCS_QUERY_NRU_NTF_REQ:
    case RCS_QUERY_PUCHN_BINDING_NTF_REQ:
    case RCS_QUERY_RECORD_PARAM_NTF_REQ:
    case TVWALL_SCHEME_NTF_REQ:
    case MPS_MAP_QRY_NTF_REQ:
    case MPS_MAP_ELEMENT_QRY_NTF_REQ:
	case MPS_MAP_ELEMENT_SEARCH_NTF_REQ:
    case MPS_MAP_AREA_QRY_NTF_REQ:
    case MPS_ELEMENT_IMAGE_QRY_NTF_REQ:
	case MPS_GPS_TRACE_QRY_NTF_REQ:
    case ALARMLINK_NTF_REQ:
    case UAS_OPERATE_LOG_QRY_NTF_REQ:
    case UAS_DEVICE_LOG_QRY_NTF_REQ:
    case UAS_VIDEOSOURCE_PRESETCFG_QRY_NTF_REQ:
    case UAS_ALARM_LOG_QRY_NTF_REQ:
    case UAS_CUSER_CUSTOMDATA_QRY_NTF_REQ:
    case RCS_QUERY_DISK_INFO_NTF_REQ:
    case TAS_TVWALL_QRY_NTF_REQ:
    case PU_REC_QRY_NTF_REQ:
	case CAPS_QUERY_DEVICE_LIST_NTF_REQ:
	case CMU_PU_OFFLINEDEV_QRY_NTF_REQ:
	case TAS_VIDSRC_INFO_QRY_NTF_REQ:
	case TAS_DEVICE_GBINFO_QRY_NTF_REQ:
	case TAS_DEVGRP_GBINFO_QRY_NTF_REQ:
        {
            string strQueryTag = GetQueryTag(pcOspSipMsg->GetMsgBody());

            //ע��, ������CQueryTask
            CSipTask *pcTask = g_cCuiDataMgr.GetTaskPtrBySession(strQueryTag, this);
            if ( NULL != pcTask )
            {
                pcTask->ProcMsg(pcMsg);
                return;
            }

            INSTLOG(CUI_APP, ERROR_LEV, "�Ҳ���QueryTag[%s]��Ӧ��QueryTask, ֱ�ӻظ�TASӦ����Ϣ\n", 
                strQueryTag.c_str());

            SendNtfRspToServer(pcMsg);
        }
        break;
        //////////////////////////////////////////////////////////////////////////
        //TAS����Ϣ�޸�֪ͨ
    case TAS_DEVICE_CAP_INDEX_NAME_MOD_NTF_REQ:
        {
            CDeviceCapIndexNameModNtfReq cNtfReq;
            pcOspSipMsg->GetMsgBody(cNtfReq);

            SendNtfRspToServer(pcMsg);

            //������������
            //             if ( cNtfReq.GetDeviceCapName().capId != DEVICE_CAP_VIDEOSOURCE )
            //             { //��ʱ�Ȳ�������ƵԴ�������������
            //                 return;
            //             }

            C3acAgent *pc3acAgent = (C3acAgent *)g_cCuiDataMgr.Get3asAgentTaskPtrBy3asName(pcOspSipMsg->GetSipFromUri().GetURIString(), this);
            if ( NULL != pc3acAgent )
            {
                //UUID��Ϊkey
                string tDevUUID = cNtfReq.GetDeviceCapName().deviceId; /* + "@" + pc3acAgent->m_str3ASDomainName;  //??����δ����TAS�����豸������TASδ�ṩ����Ϣ�������Ƚ���*/

                Iterator pUserAccountItem;
                while ( ! pUserAccountItem.End() )
                {
                    string *pUserName = NULL;
                    CUserAccount **ppcUserAccount = NULL;
                    CUserAccount *pcUserAccount = NULL;
                    if ( pc3acAgent->m_cUserMgr.m_mapUserName2PCuserAccount.Iterate(pUserAccountItem, pUserName, ppcUserAccount) )
                    {
                        CMS_ASSERT(NULL != pUserName);
                        CMS_ASSERT(NULL != ppcUserAccount);
                        pcUserAccount = *ppcUserAccount;
                        /*
                        1����λ���ĸ����3AS
                        2����ΪCDeviceCapIndexNameModNtfReq ��ֻ��DeviceId�����ֻ��ͨ�����������Ǹ�UserAccountӵ�и��豸Ȩ��
                        3���ж��û��Ƿ�ӵ�и��豸
                        4�������û�ӵ�еľ�����ƵԴ�������µ�֪ͨ��Ϣ
                        */

						if ( !pcUserAccount->IsSuperAdmin()
							&& !g_cRedisDb.IsUserHasDev(pcUserAccount->m_tUserID, tDevUUID) )
						{
							// �û�û�л�������豸;
							continue;
						}

						//EFDB_ErrorCode er = g_cRedisDb.GetDevInfo(tDevUUID, CDeviceInfo_domainId, strDomainId);
						//if ( !IsFdbResultPositive(er) )
						//{
						//	INSTLOG(CUI_APP, ERROR_LEV, "�յ�����[%s]��֪ͨ��Ϣ[%s--%hu]������û�в�ѯ���豸[%s]������ er[%d]\n",
						//		pcOspSipMsg->GetSipFromUri().GetURIString().c_str(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event, tDevUUID.c_str(), er);
						//	continue;
						//}

                        //CUserAccountDeviceInfo *pcUserDev = pcUserAccount->FindUserDevInfo(tDevUUID);
                        //if ( NULL != pcUserDev )
                        //{

						UUIDString strDomainId = cNtfReq.GetDeviceCapName().GetDevDomainId();
						if ( strDomainId.empty() )
						{
							INSTLOG(CUI_APP, ERROR_LEV, "��ƵԴ�����޸�֪ͨ TDeviceCapName.domainIdΪ�� [%s] \n",cNtfReq.ToXml().c_str());
							break;
						}

                        CDeviceCapIndexNameModNtfReq cNtf;  //�û����ܲ�ӵ��ȫ��ͨ��������һ����Ntf
                        string tDevURI = tDevUUID + "@"+  pc3acAgent->GetDomainNameByDomainID(strDomainId);

                        TDeviceCapName tDeciceCapName;
                        tDeciceCapName.deviceId = tDevURI;
                        tDeciceCapName.capId = cNtfReq.GetDeviceCapName().capId;
                        vector<TDeviceCapIndexName>::iterator pDevCapIndex = cNtfReq.GetDeviceCapName().deviceCapIndexNames.begin();
                        while ( pDevCapIndex != cNtfReq.GetDeviceCapName().deviceCapIndexNames.end() )
						{
							tDeciceCapName.deviceCapIndexNames.push_back(*pDevCapIndex);
							//if(pcUserDev->HasPri(tDeciceCapName.capId, pDevCapIndex->index))
							//{
							//}
                            pDevCapIndex++;
                        }
                        cNtf.SetDeviceCapName(tDeciceCapName);

                        list<PCuAgent>::iterator pItem = pcUserAccount->GetCuAgentList().begin();
                        while ( pItem != pcUserAccount->GetCuAgentList().end() )
                        {
                            PCuAgent pcCuAgent = (PCuAgent)(*pItem);
                            CMS_ASSERT(0 == strcmp(pcCuAgent->GetObjName(), "CCuAgent"));

                            pcCuAgent->m_cNotifyPool.InsertCapChangeNotify(cNtf);
                            pItem++;
                        }

                        //}
                    }
                }
            }
            else
            {
                INSTLOG(CUI_APP, ERROR_LEV, "�յ�����[%s]��֪ͨ��Ϣ[%s--%hu]������û�ж�Ӧ��3acAgent\n",
                    pcOspSipMsg->GetSipFromUri().GetURIString().c_str(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            }
        }
        break;
    case TAS_DEVICE_DEL_NTF_REQ:
        {
            CDeviceDelNtfReq cDelNtfReq;
            pcOspSipMsg->GetMsgBody(cDelNtfReq);

            SendNtfRspToServer(pcMsg);

            C3acAgent *pc3acAgent = (C3acAgent *)g_cCuiDataMgr.Get3asAgentTaskPtrBy3asName(pcOspSipMsg->GetSipFromUri().GetURIString(), this);
            if ( NULL != pc3acAgent )
            {
                string tDevUUID = cDelNtfReq.GetDeviceUUID(); 
                TSipURI tDevUri;
                tDevUri.SetUser(tDevUUID);
                tDevUri.SetDomain(pc3acAgent->GetDomainNameByDomainID(cDelNtfReq.GetDomainId()));

				//�豸��������ȡ������֮ǰģ���豸������Ϣ֪ͨcu
				NotifyCuPuOffline(tDevUri.GetURIString());

                EraseSubscribeKeyByDevUri(tDevUri.GetURIString());

                Iterator pUserAccountItem;
                while ( ! pUserAccountItem.End() )
                {
                    string *pUserName = NULL;
                    CUserAccount **ppcUserAccount = NULL;
                    CUserAccount *pcUserAccount = NULL;
                    if ( pc3acAgent->m_cUserMgr.m_mapUserName2PCuserAccount.Iterate(pUserAccountItem, pUserName, ppcUserAccount) )
                    {
                        CMS_ASSERT(NULL != pUserName);
                        CMS_ASSERT(NULL != ppcUserAccount);
                        pcUserAccount = *ppcUserAccount;
                        pcUserAccount->DelUserDevInfo(tDevUri.GetURIString(), false);
                    }
                }
            }
            else
            {
                INSTLOG(CUI_APP, ERROR_LEV, "�յ�����[%s]��֪ͨ��Ϣ[%s--%hu]������û�ж�Ӧ��3acAgent\n",
                    pcOspSipMsg->GetSipFromUri().GetURIString().c_str(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            }
        }
        break;
    case TAS_CUSER_DEVICE_ADD_NTF_REQ:
        {
            CUserDeviceAddNtfReq cNtfReq;
            pcOspSipMsg->GetMsgBody(cNtfReq);
            SendNtfRspToServer(pcMsg);

            //C3acAgent *pc3acAgent = (C3acAgent *)g_cCuiDataMgr.Get3asAgentTaskPtrBy3asName(pcOspSipMsg->GetSipFromUri().GetURIString(), this);
            //if ( NULL != pc3acAgent )
            //{
            //    CUserDeviceInfo& cUserDevInfo = cNtfReq.GetUserDeviceInfo();

            //    CUserAccount *pcUserAccount = pc3acAgent->m_cUserMgr.FindUserAccountByUserID(cUserDevInfo.GetUserID());
            //    if ( NULL != pcUserAccount )
            //    {
            //        pcUserAccount->UpdateUserDevInfo(cUserDevInfo);
            //    }
            //}
            //else
            //{
            //    INSTLOG(CUI_APP, ERROR_LEV, "�յ�����[%s]��֪ͨ��Ϣ[%s--%hu]������û�ж�Ӧ��3acAgent\n",
            //        pcOspSipMsg->GetSipFromUri().GetURIString().c_str(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            //}
        }
        break;
    case TAS_CUSER_DEVICE_MOD_NTF_REQ:
        {
            CUserDeviceModNtfReq cNtfReq;
            pcOspSipMsg->GetMsgBody(cNtfReq);
            SendNtfRspToServer(pcMsg);

            //C3acAgent *pc3acAgent = (C3acAgent *)g_cCuiDataMgr.Get3asAgentTaskPtrBy3asName(pcOspSipMsg->GetSipFromUri().GetURIString(), this);
            //if ( NULL != pc3acAgent )
            //{
            //    CUserDeviceInfo& cUserDevInfo = cNtfReq.GetUserDeviceInfo();

            //    CUserAccount *pcUserAccount = pc3acAgent->m_cUserMgr.FindUserAccountByUserID(cUserDevInfo.GetUserID());
            //    if ( NULL != pcUserAccount )
            //    {
            //        pcUserAccount->UpdateUserDevInfo(cUserDevInfo);
            //    }
            //    else
            //    {
            //        INSTLOG(CUI_APP, ERROR_LEV, "�յ��û��豸�޸�֪ͨ�������Ҳ���UserID[%s]��Ӧ��UserAccount\n",
            //            cUserDevInfo.GetUserID().c_str());
            //    }
            //}
            //else
            //{
            //    INSTLOG(CUI_APP, ERROR_LEV, "�յ�����[%s]��֪ͨ��Ϣ[%s--%hu]������û�ж�Ӧ��3acAgent\n",
            //        pcOspSipMsg->GetSipFromUri().GetURIString().c_str(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            //}
        }
        break;
    case TAS_CUSER_DEVICE_DEL_NTF_REQ:
        {
            CUserDeviceDelNtfReq cUserDevDelNtf;
            pcOspSipMsg->GetMsgBody(cUserDevDelNtf);

            SendNtfRspToServer(pcMsg);

            //1�����û��豸ɾ����
            C3acAgent *pc3acAgent = (C3acAgent *)g_cCuiDataMgr.Get3asAgentTaskPtrBy3asName(pcOspSipMsg->GetSipFromUri().GetURIString(), this);
            if ( NULL != pc3acAgent )
            {
                CUserDeviceInfoKey& cKey = cUserDevDelNtf.GetUserDeviceInfoKey();     

                string strUserID = cKey.GetUserID() + "@" + pc3acAgent->m_str3ASDomainName;
                CUserAccount *pcUserAccount = pc3acAgent->m_cUserMgr.FindUserAccountByUserID(cKey.GetUserID());
                if ( NULL != pcUserAccount )
                {
                    vector<TDeviceCapIndexs>& vctDevCap = cKey.GetDeviceCapIndexs();
                    if ( vctDevCap.empty() )
                    {
						// ����������ϢΪ��ʱ��ʾɾ�������豸
						string strDomainName = pc3acAgent->GetDomainNameByDomainID(cUserDevDelNtf.GetDevDomainId());
						if ( !strDomainName.empty() )
						{
							pcUserAccount->DelUserDevInfo(cKey.GetDeviceID()+"@"+ strDomainName, false);
						}
                    }
                    //else
                    //{
                    //    pcUserAccount->DelUserDevCap(cKey.GetDeviceID(), cKey.GetDeviceCapIndexs());
                    //}
                }
                else
                {
                    INSTLOG(CUI_APP, ERROR_LEV, "�յ��û��豸ɾ��֪ͨ�������Ҳ���UserID[%s]��Ӧ��UserAccount\n",
                        cKey.GetUserID().c_str());
                }

				for (vector<TDeviceCapIndexs>::iterator itor = cKey.GetDeviceCapIndexs().begin();
					itor != cKey.GetDeviceCapIndexs().end(); itor++)
				{
					TChannel vsChn;
					vsChn.SetDevUri(GetNameWithoutDomainFromURI(cKey.GetDeviceID()));
					vsChn.SetChnNO(itor->id);

					pc3acAgent->m_cPtzlockInfoList.DelCuPtzLockInfoByUserWithVsChn(GetNameWithoutDomainFromURI(cKey.GetUserID()), vsChn);
				}
            }
            else
            {
                INSTLOG(CUI_APP, ERROR_LEV, "�յ�����[%s]��֪ͨ��Ϣ[%s--%hu]������û�ж�Ӧ��3acAgent\n",
                    pcOspSipMsg->GetSipFromUri().GetURIString().c_str(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            }

            //2��֪ͨUserAccount�ĸ���CuAgent������Ŀǰû��ϸ�����豸��ƵԴɾ��֪ͨ�������Ȳ�ʵ��֪ͨ��CU hzytodo
        }
        break;
    case TAS_CUSER_GROUP_DEVICE_ADD_NTF_REQ:
        {
            CUserGroupDeviceAddNtfReq cNtfReq;
            pcOspSipMsg->GetMsgBody(cNtfReq);
            SendNtfRspToServer(pcMsg);

            //C3acAgent *pc3acAgent = (C3acAgent *)g_cCuiDataMgr.Get3asAgentTaskPtrBy3asName(pcOspSipMsg->GetSipFromUri().GetURIString(), this);
            //if ( NULL != pc3acAgent )
            //{
            //    CUserGroupDeviceInfo &cUserGrpDevInfo = cNtfReq.GetUserGroupDeviceInfo();

            //    CUserGroup *pcUserGroup = pc3acAgent->m_cUserMgr.FindUserGroupByGroupID(cUserGrpDevInfo.GetGroupID());
            //    if ( NULL != pcUserGroup )
            //    {
            //        pcUserGroup->UpdateUserDevInfo(cUserGrpDevInfo);
            //    }
            //    else
            //    {
            //        INSTLOG(CUI_APP, ERROR_LEV, "�յ��û����豸���֪ͨ�������Ҳ���UserGroupID[%s]��Ӧ��CUserGroup\n",
            //            cUserGrpDevInfo.GetGroupID().c_str());
            //    }
            //}
            //else
            //{
            //    INSTLOG(CUI_APP, ERROR_LEV, "�յ�����[%s]��֪ͨ��Ϣ[%s--%hu]������û�ж�Ӧ��3acAgent\n",
            //        pcOspSipMsg->GetSipFromUri().GetURIString().c_str(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            //}
        }
        break;
    case TAS_CUSER_GROUP_DEVICE_DEL_NTF_REQ:
        {
            CUserGroupDeviceDelNtfReq cNtfReq;
            pcOspSipMsg->GetMsgBody(cNtfReq);
            SendNtfRspToServer(pcMsg);

            C3acAgent *pc3acAgent = (C3acAgent *)g_cCuiDataMgr.Get3asAgentTaskPtrBy3asName(pcOspSipMsg->GetSipFromUri().GetURIString(), this);
            if ( NULL != pc3acAgent )
            {
                CUserGroupDeviceInfoKey &cUserGroupDevInfo = cNtfReq.GetUserGroupDeviceInfoKey();
                string tDevUUID = cUserGroupDevInfo.GetDeviceID(); // + "@" + pc3acAgent->m_str3ASDomainName;    //??����δ����TAS�����豸���Ƚ���

                CUserGroup *pcUserGroup = pc3acAgent->m_cUserMgr.FindUserGroupByGroupID(cUserGroupDevInfo.GetGroupID());
                if ( NULL != pcUserGroup )
				{
					// ����������ϢΪ��ʱ��ʾɾ�������豸
					string strDomainName = pc3acAgent->GetDomainNameByDomainID(tDevUUID);
					//if (!strDomainName.empty())
					//{
					//	pcUserAccount->DelUserDevInfo(cKey.GetDeviceID() + "@" + strDomainName, false);
					//}
                    string tDevURI = tDevUUID + "@" + strDomainName;  //xuyao
                    cUserGroupDevInfo.SetDeviceID(tDevURI);
                    vector<TDeviceCapIndexs>& vctDevCap = cUserGroupDevInfo.GetDeviceCapIndexs();
                    if ( vctDevCap.empty() )
                    { // ����������ϢΪ��ʱ��ʾɾ�������豸
                        pcUserGroup->DelUserDevInfo(tDevUUID);
                    }
                //    else
                //    {
                //        pcUserGroup->DelUserDevCap(tDevUUID, cUserGroupDevInfo.GetDeviceCapIndexs());
                //    }
                //}
                //else
                //{
                //    INSTLOG(CUI_APP, ERROR_LEV, "�յ��û����豸ɾ��֪ͨ�������Ҳ���UserGroupID[%s]��Ӧ��CUserGroup\n",
                //        cUserGroupDevInfo.GetGroupID().c_str());
                }
            }
            else
            {
                INSTLOG(CUI_APP, ERROR_LEV, "�յ�����[%s]��֪ͨ��Ϣ[%s--%hu]������û�ж�Ӧ��3acAgent\n",
                    pcOspSipMsg->GetSipFromUri().GetURIString().c_str(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            }
        }
        break;
    case TAS_CUSER_GROUP_DEVICE_MOD_NTF_REQ:
        {
            CUserGroupDeviceModNtfReq cNtfReq;
            pcOspSipMsg->GetMsgBody(cNtfReq);
            SendNtfRspToServer(pcMsg);

            //C3acAgent *pc3acAgent = (C3acAgent *)g_cCuiDataMgr.Get3asAgentTaskPtrBy3asName(pcOspSipMsg->GetSipFromUri().GetURIString(), this);
            //if ( NULL != pc3acAgent )
            //{
            //    CUserGroupDeviceInfo &cUserGroupDevInfo = cNtfReq.GetUserGroupDeviceInfo();
            //    //string strDomainName = g_cCuiDataMgr.GetDomainNameByDomainID(cUserGroupDevInfo.GetDevDomainID());
            //    //string tDevURI = cUserGroupDevInfo.GetDeviceID() + "@" + strDomainName;
            //    //cUserGroupDevInfo.SetDeviceID(tDevURI);
            //    CUserGroup *pcUserGroup = pc3acAgent->m_cUserMgr.FindUserGroupByGroupID(cUserGroupDevInfo.GetGroupID());
            //    if ( NULL != pcUserGroup )
            //    {
            //        pcUserGroup->UpdateUserDevInfo(cUserGroupDevInfo);
            //    }
            //    else
            //    {
            //        INSTLOG(CUI_APP, ERROR_LEV, "�յ��û����豸�޸�֪ͨ�������Ҳ���UserGroupID[%s]��Ӧ��CUserGroup\n",
            //            cUserGroupDevInfo.GetGroupID().c_str());
            //    }
            //}
            //else
            //{
            //    INSTLOG(CUI_APP, ERROR_LEV, "�յ�����[%s]��֪ͨ��Ϣ[%s--%hu]������û�ж�Ӧ��3acAgent\n",
            //        pcOspSipMsg->GetSipFromUri().GetURIString().c_str(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            //}
        }
        break;
    case TAS_DEVICE_MOD_NTF_REQ:
        {
            CDeviceModNtfReq cNtfReq;
            pcOspSipMsg->GetMsgBody(cNtfReq);
            SendNtfRspToServer(pcMsg);

            //C3acAgent *pc3acAgent = (C3acAgent *)g_cCuiDataMgr.Get3asAgentTaskPtrBy3asName(pcOspSipMsg->GetSipFromUri().GetURIString(), this);
            //if ( NULL != pc3acAgent )
            //{
            //    string strDevUri = cNtfReq.GetDeviceInfo().GetDeviceName() + "@" 
            //        +  pc3acAgent->GetDomainNameByDomainID(cNtfReq.GetDeviceInfo().GetDomainUUID());
            //    g_cPuList.ModPuData(strDevUri, cNtfReq.GetDeviceInfo().GetDeviceName());
            //    CDeviceInfo& cDevInfo = cNtfReq.GetDeviceInfo();

            //    Iterator pPos;
            //    while ( ! pPos.End() )
            //    {
            //        string *pcUserName = NULL;
            //        CUserAccount **ppcUserAccount = NULL;
            //        CUserAccount *pcUserAccount = NULL;
            //        if ( pc3acAgent->m_cUserMgr.m_mapUserName2PCuserAccount.Iterate(pPos, pcUserName, ppcUserAccount) )
            //        {
            //            CMS_ASSERT(NULL != pcUserName);
            //            CMS_ASSERT(NULL != ppcUserAccount);
            //            pcUserAccount = *ppcUserAccount;
            //            
            //            if (!pcUserAccount->InGroup())
            //            {
            //                pcUserAccount->SyncUserDevInfo(cDevInfo.GetDeviceUUID(), cDevInfo.GetDeviceModelCapDetail());
            //            }
            //        }
            //    }

            //    pPos.SetToBegin();
            //    while ( ! pPos.End() )
            //    {
            //        string *pcUserGroupId = NULL;
            //        CUserGroup *pcUserGroup = NULL;
            //        if ( pc3acAgent->m_cUserMgr.m_mapGroupID2UserGroup.Iterate(pPos, pcUserGroupId, pcUserGroup) )
            //        {
            //            CMS_ASSERT(NULL != pcUserGroupId);
            //            CMS_ASSERT(NULL != pcUserGroup);

            //            pcUserGroup->DelUserDevCap(cDevInfo.GetDeviceUUID(), cDevInfo.GetDeviceModelCapDetail());
            //        }
            //    }
            //}
            //else
            //{
            //    INSTLOG(CUI_APP, ERROR_LEV, "�յ�����[%s]��֪ͨ��Ϣ[%s--%hu]������û�ж�Ӧ��3acAgent\n",
            //        pcOspSipMsg->GetSipFromUri().GetURIString().c_str(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            //}
        }
        break;
    case ALARMLINK_MOD_NTF_REQ:
        {
            CAlarmLinkModNtfReq cNtfReq;
            pcOspSipMsg->GetMsgBody(cNtfReq);

            //ͳһ�ȸ����ظ���Ϣ
            SendNtfRspToServer(pcMsg);

            //֪ͨ����cu�û�
            vector<CTask*> vctCuAgents = GetTasksArray("CCuAgent");
            if ( ! vctCuAgents.empty() )
            {
                vector<CTask*>::iterator pCuAgentItem = vctCuAgents.begin();
                while ( pCuAgentItem != vctCuAgents.end() )
                {
                    CCuAgent *pcCuAgent = (CCuAgent *)(*pCuAgentItem);
                    CMS_ASSERT( 0 == strcmp(pcCuAgent->GetObjName(), "CCuAgent") );
                    if ( CCuAgent::ONLINE_STATE == pcCuAgent->GetState() )
                    {
                        pcCuAgent->m_cNotifyPool.InsertAlarmLinkCfgModNotify(cNtfReq);
                    }
                    pCuAgentItem++;
                }
            }
        }
        break;
    case TAS_DOMAIN_RELATION_ADD_NTF_REQ:
        {
            CDomainRelationAddNtfReq cNtf;
            pcOspSipMsg->GetMsgBody(cNtf);

            SendNtfRspToServer(pcMsg);
            C3acAgent* pt3acAgent= (C3acAgent* )g_cCuiDataMgr.Get3asAgentTaskPtrBy3asName(pcOspSipMsg->GetSipFromUri().GetURIString(), this);
            if(pt3acAgent != NULL)
            {
                const CDomainRelationInfo& tInfo = cNtf.GetDomainRelation();
                pt3acAgent->m_mapDomainInfo.SetDomainInfo(tInfo); 
            }
            else
            {
                INSTLOG(CUI_APP, ERROR_LEV, "�յ�����[%s]��֪ͨ��Ϣ[%s--%hu]������û�ж�Ӧ��3acAgent\n",
                    pcOspSipMsg->GetSipFromUri().GetURIString().c_str(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            }
        }
        break;
    case TAS_DOMAIN_RELATION_DEL_NTF_REQ:
        {
            CDomainRelationDelNtfReq cNtf;
            pcOspSipMsg->GetMsgBody(cNtf);

            SendNtfRspToServer(pcMsg);
            C3acAgent* pt3acAgent= (C3acAgent* )g_cCuiDataMgr.Get3asAgentTaskPtrBy3asName(pcOspSipMsg->GetSipFromUri().GetURIString(), this);
            if(pt3acAgent != NULL)
            {
                const UUIDString& tDomainId = cNtf.GetDomainId(); 
                CDomainRelationInfo* pcDeledDomain = pt3acAgent->m_mapDomainInfo.FindDomainInfoById(tDomainId);
                if (pcDeledDomain != NULL)
                {
                    string strDomainName = pcDeledDomain->GetDomainName();
                    INSTLOG(CUI_APP, CRITICAL_LEV, "��[%s]-[%s]��������������͸�����ص���������\n", 
                        strDomainName.c_str(), tDomainId.c_str());
                    pt3acAgent->ClearDomainResource(strDomainName); 
                    pt3acAgent->m_mapDomainInfo.DelDomainInfo(tDomainId); 
                }
                else
                {
                    INSTLOG(CUI_APP, CRITICAL_LEV, "��UUID[%s]���������ϵ������ɾ��\n", 
                        tDomainId.c_str());
                }
            }
            else
            {
                INSTLOG(CUI_APP, ERROR_LEV, "�յ�����[%s]��֪ͨ��Ϣ[%s--%hu]������û�ж�Ӧ��3acAgent\n",
                    pcOspSipMsg->GetSipFromUri().GetURIString().c_str(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            }
        }
        break;
    case TAS_DOMAIN_RELATION_MOD_NTF_REQ:
        {
            CDomainRelationModNtfReq cNtf;
            pcOspSipMsg->GetMsgBody(cNtf);

            SendNtfRspToServer(pcMsg);
            C3acAgent* pt3acAgent= (C3acAgent* )g_cCuiDataMgr.Get3asAgentTaskPtrBy3asName(pcOspSipMsg->GetSipFromUri().GetURIString(), this);
            if(pt3acAgent != NULL)
            {
                const CDomainRelationInfo& tInfo = cNtf.GetDomainRelation();
                if (tInfo.GetDomainID() == ::GetDomainUUID())
                {
                    if (tInfo.GetDomainName() != ::GetDomainName())
                    {
                        INSTLOG(CUI_APP,  CRITICAL_LEV, "����[%s]�������޸�Ϊ[%s]���ȴ�pms����\n", 
                            ::GetDomainName().c_str(), tInfo.GetDomainName().c_str());
                    }
                    else
                    {
                        if (!(pt3acAgent->m_mapDomainInfo.GetParentDomainId().empty()) && tInfo.GetParentDomainID().empty())
                        {
                            INSTLOG(CUI_APP,  CRITICAL_LEV, "����[%s]���ϼ���[%s]�������ȴ����ϼ��������������\n", 
                                ::GetDomainName().c_str(), ::GetParentDomainName().c_str());
                            pt3acAgent->m_mapDomainInfo.SetParentDomainId(tInfo.GetParentDomainID());
                        }
                        else if (pt3acAgent->m_mapDomainInfo.GetParentDomainId().empty() && !(tInfo.GetParentDomainID().empty()))
                        {
                            pt3acAgent->m_mapDomainInfo.SetParentDomainId(tInfo.GetParentDomainID());                  
                            INSTLOG(CUI_APP,  CRITICAL_LEV, "����[%s]���������ϼ���[%s]-[%s]\n", 
                                ::GetDomainName().c_str(), ::GetParentDomainName().c_str(), tInfo.GetParentDomainID().c_str());
                        }
                        else
                        {
                            INSTLOG(CUI_APP,  CRITICAL_LEV, "�ϼ���������������ȴ����ϼ��������������\n");                       
                        }
                    }  
                }
                else
                {
                    const UUIDString& tDomainId = tInfo.GetDomainID();
                    CDomainRelationInfo* pcModedDomain = pt3acAgent->m_mapDomainInfo.FindDomainInfoById(tDomainId);
                    if (pcModedDomain != NULL)
                    {
                        string strDomainName = pcModedDomain->GetDomainName();
                        if (strDomainName != tInfo.GetDomainName())
                        {
                            INSTLOG(CUI_APP,  CRITICAL_LEV, "����[%s]�������޸�Ϊ[%s]������͸���[%s]��ص���������\n", 
                                strDomainName.c_str(), tInfo.GetDomainName().c_str(), strDomainName.c_str());
                            pt3acAgent->ClearDomainResource(strDomainName); 
                        }
                        else
                        {
                            INSTLOG(CUI_APP,  CRITICAL_LEV, "����[%s]δ��������յ����֪ͨ\n", 
                                strDomainName.c_str());
                        }
                    }
                    else
                    {
                        INSTLOG(CUI_APP,  CRITICAL_LEV, "��UUID[%s]���������ϵ�����账��\n", 
                            tDomainId.c_str());
                    }
                }
                pt3acAgent->m_mapDomainInfo.ModDomainInfo(tInfo);
            }
        }
        break;
    case TAS_CUSER_DEL_NTF_REQ:
        {
            CUserDelNtfReq cNtfReq;
            pcOspSipMsg->GetMsgBody(cNtfReq);
            SendNtfRspToServer(pcMsg);

            C3acAgent *pc3acAgent = (C3acAgent *)g_cCuiDataMgr.Get3asAgentTaskPtrBy3asName(pcOspSipMsg->GetSipFromUri().GetURIString(), this);
            if ( NULL != pc3acAgent )
            {
                //string strUserID = cNtfReq.GetUserID() + "@" + pc3acAgent->m_str3ASDomainName;
                CUserAccount *pcUserAccount = pc3acAgent->m_cUserMgr.FindUserAccountByUserID(cNtfReq.GetUserID());
                if ( NULL != pcUserAccount )
                {
                    list<PCuAgent> lstTmpCuAgent = pcUserAccount->GetCuAgentList();     //Ϊ�˱���ɾ��CuAgentʱӰ�쵽pcUserAccount->m_lstCuAgentList�ı������ظ���һ��
                    list<PCuAgent>::iterator pCuItem = lstTmpCuAgent.begin();
                    while ( pCuItem != lstTmpCuAgent.end() )
                    {
                        PCuAgent pcCuAgent = (PCuAgent)(*pCuItem);
                        CMS_ASSERT( 0 == strcmp(pcCuAgent->GetObjName(), "CCuAgent") );
                        if ( CCuAgent::ONLINE_STATE == pcCuAgent->GetState())
                        {
                            INSTLOG(CUI_APP, EVENT_LEV, "�յ��û��˻�[%s]������֪ͨ��ǿ�ƽ�CU[�Ự: %s]����\n",
                                pcUserAccount->m_strUserName.c_str(), pcCuAgent->m_strSession.c_str());
                            
                            delete pcCuAgent;
                            //pCuItem = lstTmpCuAgent.erase(pCuItem);
                        }
                        //hzytodo2 ���ｫUserAccount��CuAgent�б�����һ����ʱ����Ȼ��ɾ���������ߣ����û����������û����ܲ��ᱻ����
                        //else
                        //{
                        pCuItem++;
                        //}
                    }
                    //��ɾ��UserAccount���ȴ�3acAgent�Լ�����
                }
                else
                {
                    INSTLOG(CUI_APP, EVENT_LEV, "�յ��û�ɾ��֪ͨ�������Ҳ���UserID[%s]��Ӧ��CUserAccount\n",
                        cNtfReq.GetUserID().c_str());
                }

				// ɾ���û�PTZ��
				pc3acAgent->m_cPtzlockInfoList.DelCuPtzLockInfoByUserId(cNtfReq.GetUserID());

                //�����ò������ӹ�ϵ�����Թ㲥������cu�û�
                //֪ͨ����cu�û�

                string strUserID = cNtfReq.GetUserID() + "@" + pc3acAgent->m_str3ASDomainName;

                vector<CTask*> vctCuAgents = GetTasksArray("CCuAgent");
                if ( ! vctCuAgents.empty() )
                {
                    vector<CTask*>::iterator pCuAgentItem = vctCuAgents.begin();
                    while ( pCuAgentItem != vctCuAgents.end() )
                    {
                        CCuAgent *pcCuAgent = (CCuAgent *)(*pCuAgentItem);
                        CMS_ASSERT( 0 == strcmp(pcCuAgent->GetObjName(), "CCuAgent") );
                        if ( CCuAgent::ONLINE_STATE == pcCuAgent->GetState() )
                        {
                            //���ƽ̨֪ͨ��Ϣ
                            if( pcCuAgent->m_str3asName == pc3acAgent->m_t3asURI.GetURIString())
                            {
                                pcCuAgent->m_cNotifyPool.InsertUserDelNotify(strUserID);
                            }
                        }

                        pCuAgentItem++;
                    }
                }
            }
            else
            {
                INSTLOG(CUI_APP, ERROR_LEV, "�յ�����[%s]��֪ͨ��Ϣ[%s--%hu]������û�ж�Ӧ��3acAgent\n",
                    pcOspSipMsg->GetSipFromUri().GetURIString().c_str(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            }
        }
        break;
    case TAS_CUSER_GROUP_DEL_NTF_REQ:
        {
            CUserGroupDelNtfReq cNtfReq;
            pcOspSipMsg->GetMsgBody(cNtfReq);
            SendNtfRspToServer(pcMsg);

            C3acAgent *pc3acAgent = (C3acAgent *)g_cCuiDataMgr.Get3asAgentTaskPtrBy3asName(pcOspSipMsg->GetSipFromUri().GetURIString(), this);
            if ( NULL != pc3acAgent )
            {
                CUserGroup *pcUserGroup = pc3acAgent->m_cUserMgr.FindUserGroupByGroupID(cNtfReq.GetUserGroupInfoKey().GetGroupNO());
                if ( NULL != pcUserGroup )
                {
                    Iterator pPos;
                    while ( ! pPos.End() )
                    {
                        string *pcUserName = NULL;
                        CUserAccount **ppcUserAccount = NULL;
                        CUserAccount *pcUserAccount = NULL;
                        if ( pc3acAgent->m_cUserMgr.m_mapUserName2PCuserAccount.Iterate(pPos, pcUserName, ppcUserAccount) )
                        {
                            CMS_ASSERT(NULL != pcUserName);
                            CMS_ASSERT(NULL != ppcUserAccount);
                            pcUserAccount = *ppcUserAccount;
                            if ( pcUserAccount->m_tUserGroupID == pcUserGroup->m_tGroupID )
                            {
                                list<PCuAgent> lstTmpCuAgent = pcUserAccount->GetCuAgentList();     //Ϊ�˱���ɾ��CuAgentʱӰ�쵽pcUserAccount->m_lstCuAgentList�ı������ظ���һ��
                                list<PCuAgent>::iterator pCuItem = lstTmpCuAgent.begin();
                                while ( pCuItem != lstTmpCuAgent.end() )
                                {
                                    PCuAgent pcCuAgent = (PCuAgent)(*pCuItem);
                                    CMS_ASSERT( 0 == strcmp(pcCuAgent->GetObjName(), "CCuAgent") );
                                    if ( CCuAgent::ONLINE_STATE == pcCuAgent->GetState() )
                                    {
                                        INSTLOG(CUI_APP, EVENT_LEV, "�յ��û���[%s]������֪ͨ���û��˻�[%s]���ڸ��飬ǿ�ƽ�CU[�Ự: %s]����\n",
                                            pcUserGroup->m_strGroupName.c_str(), pcUserAccount->m_strUserName.c_str(), pcCuAgent->m_strSession.c_str());

                                        delete pcCuAgent;
                                        //pCuItem = lstTmpCuAgent.erase(pCuItem);  //hzytodo2 ͬline 3588
                                    }

                                    pCuItem++;
                                }
                                //��ɾ��UserAccount���ȴ�3acAgent�Լ�����
                            }
                        }
                    }

                }
                else
                {
                    INSTLOG(CUI_APP, EVENT_LEV, "�յ��û���ɾ��֪ͨ�������Ҳ���UserGroupID[%s]��Ӧ��CUserGroup\n",
                        cNtfReq.GetUserGroupInfoKey().GetGroupNO().c_str());
                }

                string strUserGroupURI = cNtfReq.GetUserGroupInfoKey().GetGroupNO() + "@" + pc3acAgent->m_str3ASDomainName;
                vector<CTask*> vctCuAgents = GetTasksArray("CCuAgent");
                if ( ! vctCuAgents.empty() )
                {
                    vector<CTask*>::iterator pCuAgentItem = vctCuAgents.begin();
                    while ( pCuAgentItem != vctCuAgents.end() )
                    {
                        CCuAgent *pcCuAgent = (CCuAgent *)(*pCuAgentItem);
                        CMS_ASSERT( 0 == strcmp(pcCuAgent->GetObjName(), "CCuAgent") );
                        if ( CCuAgent::ONLINE_STATE == pcCuAgent->GetState() )
                        {
                            //���ƽ̨֪ͨ��Ϣ
                            if( pcCuAgent->m_str3asName == pc3acAgent->m_t3asURI.GetURIString())
                            {
                                pcCuAgent->m_cNotifyPool.InsertUserGroupDelNotify(strUserGroupURI);
                            }
                        }

                        pCuAgentItem++;
                    }
                }
            }
            else
            {
                INSTLOG(CUI_APP, ERROR_LEV, "�յ�����[%s]��֪ͨ��Ϣ[%s--%hu]������û�ж�Ӧ��3acAgent\n",
                    pcOspSipMsg->GetSipFromUri().GetURIString().c_str(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            }
        }
        break;
    case TAS_CUSER_MOD_NTF_REQ:
        {
            CUserModNtfReq cNtfReq;
            pcOspSipMsg->GetMsgBody(cNtfReq);
            SendNtfRspToServer(pcMsg);

            C3acAgent *pc3acAgent = (C3acAgent *)g_cCuiDataMgr.Get3asAgentTaskPtrBy3asName(pcOspSipMsg->GetSipFromUri().GetURIString(), this);
            if ( NULL != pc3acAgent )
            {
                //string strUserID = cNtfReq.GetUserInfo().GetUserID() + "@" + pc3acAgent->m_str3ASDomainName;
                CUserAccount *pcUserAccount = pc3acAgent->m_cUserMgr.FindUserAccountByUserID(cNtfReq.GetUserInfo().GetUserID());
                if ( NULL != pcUserAccount )
                {
                    //�����û���Ϣ
                    pcUserAccount->m_strPasswd = cNtfReq.GetUserInfo().GetUserPwd();
                    CPrivilege cUserPri;
                    cUserPri.SetPri(cNtfReq.GetUserInfo().GetUserPrivilege());
                    pcUserAccount->m_tUserPri = cUserPri;
                    pcUserAccount->m_nUserType = cNtfReq.GetUserInfo().GetUserType();
                    string strUserGroupID = pcUserAccount->m_tUserGroupID;
                    pcUserAccount->m_tUserGroupID = cNtfReq.GetUserInfo().GetUsergrpID();
                    pcUserAccount->m_cStrmRestrict.SetInAll( cNtfReq.GetUserInfo().GetSwitchNum() );

                    if(strUserGroupID != cNtfReq.GetUserInfo().GetUsergrpID())
                    {
                        //�Ƿ�ת��������
                        if(cNtfReq.GetUserInfo().GetUsergrpID().empty())
                        { //���飬�ж�UserAccount����
                            if(pcUserAccount->m_bDevPriState != DEVPRI_NOT_START)
                            {
                                INSTLOG(CUI_APP, EVENT_LEV, "��ǰUserAccount[%s]�ѻ�ȡ������Ϣ��\n",
                                    pcUserAccount->m_strUserName.c_str());
                            }
                        }
                        else
                        {
                            //�����У�������3acAgent���Ƿ��и�����Ϣ
                            CUserGroup *pcUserGroup = pc3acAgent->m_cUserMgr.FindUserGroupByGroupID(cNtfReq.GetUserInfo().GetUsergrpID());
                            if(pcUserGroup != NULL)
                            {
                                CMS_ASSERT(pcUserGroup->m_bDevPriState != DEVPRI_NOT_START && "�Ѵ��ڵ���ض��Ѿ���ȡ���豸Ȩ����Ϣ");
                                INSTLOG(CUI_APP, EVENT_LEV, "��ǰ����Ϣ[%s-%s]���ڣ�˵���Ѿ���ȡ������Ϣ�������ȡ��\n",
                                    pcUserGroup->m_strGroupName.c_str(), pcUserGroup->m_tGroupID.c_str());
                            }
                            else
                            {
                                //�Ҳ���������Ϣ����Ҫ���»�ȡ
                                CUserGroup cUserGroup(&pc3acAgent->m_cUserMgr);
                                cUserGroup.m_tGroupID = cNtfReq.GetUserInfo().GetUsergrpID();
                                cUserGroup.m_bDevPriState = DEVPRI_NOT_START;

                                //��ȡ����Ϣ
                                pc3acAgent->m_cUserMgr.m_mapGroupID2UserGroup.Insert(cUserGroup.m_tGroupID, cUserGroup);
                                string str3asName = pc3acAgent->m_t3asURI.GetURIString();
                                CGetGroupInfo *pcTask = new CGetGroupInfo(this, str3asName, cNtfReq.GetUserInfo().GetUsergrpID());
                                pcTask->InitStateMachine();
                                pcTask->StartQryGroupInfo(); 
                            }
                        }
                    }
                }

                CCuUserInfo cUserInfo;
                cUserInfo = cNtfReq.GetUserInfo();
                cUserInfo.SetDomainID(pc3acAgent->m_t3ASDomainID);
                if(!cUserInfo.GetUserID().empty())
                {
                    cUserInfo.SetUserID(cUserInfo.GetUserID() + "@" + pc3acAgent->GetDomainNameByDomainID(cUserInfo.GetDomainID()));
                }

                if(!cUserInfo.GetUserName().empty())
                {
                    cUserInfo.SetUserName(cUserInfo.GetUserName() + "@" + pc3acAgent->GetDomainNameByDomainID(cUserInfo.GetDomainID()));
                }

                if(!cUserInfo.GetCreatorID().empty())
                {
                    cUserInfo.SetCreatorID(cUserInfo.GetCreatorID() + "@" + pc3acAgent->GetDomainNameByDomainID(cUserInfo.GetDomainID()));
                }

                if(!cUserInfo.GetUsergrpID().empty())
                {
                    cUserInfo.SetUsergrpID(cUserInfo.GetUsergrpID() + "@" + pc3acAgent->GetDomainNameByDomainID(cUserInfo.GetDomainID()));
                }   

                vector<CTask*> vctCuAgents = GetTasksArray("CCuAgent");
                if ( ! vctCuAgents.empty() )
                {
                    vector<CTask*>::iterator pCuAgentItem = vctCuAgents.begin();
                    while ( pCuAgentItem != vctCuAgents.end() )
                    {
                        PCuAgent pcCuAgent = (PCuAgent)(*pCuAgentItem);
                        CMS_ASSERT( 0 == strcmp(pcCuAgent->GetObjName(), "CCuAgent") );
                        if ( CCuAgent::ONLINE_STATE == pcCuAgent->GetState() && 
                            pcCuAgent->m_str3asName == pc3acAgent->m_t3asURI.GetURIString())
                        {
                            //���ƽ̨֪ͨ��Ϣ
                            pcCuAgent->m_cNotifyPool.InsertUserModNotify(cUserInfo);
                        }
                        pCuAgentItem++;
                    }
                }                 
            }
            else
            {
                INSTLOG(CUI_APP, ERROR_LEV, "�յ�����[%s]��֪ͨ��Ϣ[%s--%hu]������û�ж�Ӧ��3acAgent\n",
                    pcOspSipMsg->GetSipFromUri().GetURIString().c_str(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            }
        }
        break;
    case TAS_CUSER_GROUP_MOD_NTF_REQ:
        {
            CUserGroupModNtfReq cNtfReq;
            pcOspSipMsg->GetMsgBody(cNtfReq);
            SendNtfRspToServer(pcMsg);

            C3acAgent *pc3acAgent = (C3acAgent *)g_cCuiDataMgr.Get3asAgentTaskPtrBy3asName(pcOspSipMsg->GetSipFromUri().GetURIString(), this);
            if ( NULL != pc3acAgent )
            {
                CUserGroup *pcUserGroup = pc3acAgent->m_cUserMgr.FindUserGroupByGroupID(cNtfReq.GetUserGroupInfo().GetGroupNO());
                if ( NULL != pcUserGroup )
                {
                    pcUserGroup->m_strGroupName = cNtfReq.GetUserGroupInfo().GetGroupName();
                    CPrivilege cUserGroupPri;
                    cUserGroupPri.SetPri(cNtfReq.GetUserGroupInfo().GetUserPrivilege());
                    pcUserGroup->m_tUserGroupPri = cUserGroupPri;
                    pcUserGroup->m_nGroupType = cNtfReq.GetUserGroupInfo().GetUserType();
                    pcUserGroup->m_cStrmRestrict.SetInAll( cNtfReq.GetUserGroupInfo().GetSwitchNum() );
                }

                cNtfReq.GetUserGroupInfo().SetCreateUser(cNtfReq.GetUserGroupInfo().GetCreateUser() + "@" + pc3acAgent->m_str3ASDomainName);
                cNtfReq.GetUserGroupInfo().SetGroupNO(cNtfReq.GetUserGroupInfo().GetGroupNO() + "@" + pc3acAgent->m_str3ASDomainName);
                vector<CTask*> vctCuAgents = GetTasksArray("CCuAgent");
                if ( ! vctCuAgents.empty() )
                {
                    vector<CTask*>::iterator pCuAgentItem = vctCuAgents.begin();
                    while ( pCuAgentItem != vctCuAgents.end() )
                    {
                        CCuAgent *pcCuAgent = (CCuAgent *)(*pCuAgentItem);
                        CMS_ASSERT( 0 == strcmp(pcCuAgent->GetObjName(), "CCuAgent") );
                        if ( CCuAgent::ONLINE_STATE == pcCuAgent->GetState() && 
                            pcCuAgent->m_str3asName == pc3acAgent->m_t3asURI.GetURIString())
                        {
                            pcCuAgent->m_cNotifyPool.InsertUserGroupModNotify(cNtfReq);
                        }
                        pCuAgentItem++;
                    }
                }
            }
            else
            {
                INSTLOG(CUI_APP, ERROR_LEV, "�յ�����[%s]��֪ͨ��Ϣ[%s--%hu]������û�ж�Ӧ��3acAgent\n",
                    pcOspSipMsg->GetSipFromUri().GetURIString().c_str(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            }
        }
        break;
        //////////////////////////////////////////////////////////////////////////
    case TAS_TVWALL_ADD_NTF_REQ:
        {
            CTvwallAddNtfReq cNtfReq;
            pcOspSipMsg->GetMsgBody(cNtfReq);

            //ͳһ�ȸ����ظ���Ϣ
            SendNtfRspToServer(pcMsg);

            C3acAgent *pc3acAgent = (C3acAgent *)g_cCuiDataMgr.Get3asAgentTaskPtrBy3asName(pcOspSipMsg->GetSipFromUri().GetURIString(), this);            
            //if(NULL != pc3acAgent)
            //{
            //    CStrMap<CUserAccount*> & cUserAccountList = pc3acAgent->m_cUserMgr.m_mapUserName2PCuserAccount;
            //    if(!cUserAccountList.IsEmpty())
            //    {
            //        Iterator pPos;
            //        while( ! pPos.End() )
            //        {
            //            string strUserName;
            //            CUserAccount *pcUserAccount;
            //            if( cUserAccountList.Iterate(pPos, strUserName, pcUserAccount) )
            //            {
            //                if( pcUserAccount->IsSuperAdmin())
            //                {
            //                    CBitsContainer tDevPri;
            //                    tDevPri.ConvertFromInt(32);
            //                    vector<TDeviceCapPrivilege> tDeviceCapPrivileges;
            //                    vector<TDeviceCapInfo>::const_iterator item = cNtfReq.GetTvwallInfo().GetDeviceModelCapDetail().begin();
            //                    while(item != cNtfReq.GetTvwallInfo().GetDeviceModelCapDetail().end())
            //                    {
            //                        TDeviceCapPrivilege cDevCapPri;
            //                        cDevCapPri.id = item->type;
            //                        map<int, CBitsContainer> privileges;
            //                        for(int tmp = 0;tmp < item->num; tmp++)
            //                        {
            //                            privileges[tmp] = tDevPri;
            //                        }
            //                        cDevCapPri.privileges = privileges;

            //                        tDeviceCapPrivileges.push_back(cDevCapPri);

            //                        item++;
            //                    }

            //                    //CUserAccountDeviceInfo cUserDevInfo;
            //                    //cUserDevInfo.SetDeviceFullInfo(cNtfReq.GetTvwallInfo().GetDeviceUUID(), cNtfReq.GetTvwallInfo().GetDomainUUID()
            //                    //    , tDevPri, tDeviceCapPrivileges);

            //                    //pcUserAccount->AddUserDevInfo(cUserDevInfo);
            //                }
            //            }
            //        }
            //    }
            //}

			if (pc3acAgent == NULL)
			{
				INSTLOG(CUI_APP, ERROR_LEV, "�յ�����[%s]��֪ͨ��Ϣ[%s--%hu]������û�ж�Ӧ��3acAgent\n",
					pcOspSipMsg->GetSipFromUri().GetURIString().c_str(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
				break;
			}

            vector<TvDecoderBinder>& tvDecoderBinder = cNtfReq.GetTvwallInfo().GetTvDecoderBinders();
            for(u32 tvBinderNum = 0; tvBinderNum < tvDecoderBinder.size(); tvBinderNum++)
            {
                if(!tvDecoderBinder[tvBinderNum].decoderId.empty())
                {
                    string strEncoder = tvDecoderBinder[tvBinderNum].decoderId + "@" + pc3acAgent->GetDomainNameByDomainID(cNtfReq.GetTvwallInfo().GetDomainUUID());
                    tvDecoderBinder[tvBinderNum].decoderId = strEncoder;
                }            
            }
            cNtfReq.GetTvwallInfo().SetDeviceUUID(cNtfReq.GetTvwallInfo().GetDeviceUUID()
                + "@" + pc3acAgent->GetDomainNameByDomainID(cNtfReq.GetTvwallInfo().GetDomainUUID()));

            vector<CTask*> vctCuAgents = GetTasksArray("CCuAgent");
            if ( ! vctCuAgents.empty() )
            {
                vector<CTask*>::iterator pCuAgentItem = vctCuAgents.begin();
                while ( pCuAgentItem != vctCuAgents.end() )
                {
                    CCuAgent *pcCuAgent = (CCuAgent *)(*pCuAgentItem);
                    CMS_ASSERT( 0 == strcmp(pcCuAgent->GetObjName(), "CCuAgent") );

                    if ( CCuAgent::ONLINE_STATE == pcCuAgent->GetState()
                        && pcCuAgent->m_pcUserAccount->IsSuperAdmin())
                    {
                        pcCuAgent->m_cNotifyPool.InsertTvWallAddNotify(cNtfReq);
                    }

                    pCuAgentItem++;
                }
            }
        }
        break;
    case TAS_TVWALL_MOD_NTF_REQ:
        {
            CTvwallModNtfReq cNtfReq;
            pcOspSipMsg->GetMsgBody(cNtfReq);


            //ͳһ�ȸ����ظ���Ϣ
            SendNtfRspToServer(pcMsg);

            C3acAgent *pc3acAgent = (C3acAgent *)g_cCuiDataMgr.Get3asAgentTaskPtrBy3asName(pcOspSipMsg->GetSipFromUri().GetURIString(), this);            
            if(NULL != pc3acAgent)
            {

                vector<TvDecoderBinder>& tvDecoderBinder = cNtfReq.GetTvwallInfo().GetTvDecoderBinders();
                for(u32 tvBinderNum = 0; tvBinderNum < tvDecoderBinder.size(); tvBinderNum++)
                {
                    if(!tvDecoderBinder[tvBinderNum].decoderId.empty())
                    {
                        string strEncoder = tvDecoderBinder[tvBinderNum].decoderId + "@" + pc3acAgent->GetDomainNameByDomainID(cNtfReq.GetTvwallInfo().GetDomainUUID());
                        tvDecoderBinder[tvBinderNum].decoderId = strEncoder;
                    }            
                }


                //CStrMap<CUserAccount*> & cUserAccountList = pc3acAgent->m_cUserMgr.m_mapUserName2PCuserAccount;
                //if(!cUserAccountList.IsEmpty())
                //{
                //    hzytodo2 ÿ��ˢ��һ�£�����̫�����˱�domian nameŪ��

                //    Iterator pPos;
                //    while( ! pPos.End() )
                //    {
                //        string strUserName;
                //        CUserAccount *pcUserAccount;
                //        if( cUserAccountList.Iterate(pPos, strUserName, pcUserAccount) )
                //        {
                //            if( pcUserAccount->IsSuperAdmin())
                //            {
                //                CBitsContainer tDevPri;
                //                tDevPri.ConvertFromInt(32);
                //                vector<TDeviceCapPrivilege> tDeviceCapPrivileges;
                //                vector<TDeviceCapInfo>::const_iterator item = cNtfReq.GetTvwallInfo().GetDeviceModelCapDetail().begin();
                //                while(item != cNtfReq.GetTvwallInfo().GetDeviceModelCapDetail().end())
                //                {
                //                    TDeviceCapPrivilege cDevCapPri;
                //                    cDevCapPri.id = item->type;
                //                    map<int, CBitsContainer> privileges;
                //                    for(int tmp = 0;tmp < item->num; tmp++)
                //                    {
                //                        privileges[tmp] = tDevPri;
                //                    }
                //                    cDevCapPri.privileges = privileges;

                //                    tDeviceCapPrivileges.push_back(cDevCapPri);

                //                    item++;
                //                }

                //                CUserAccountDeviceInfo cUserDevInfo;
                //                cUserDevInfo.SetDeviceFullInfo(cNtfReq.GetTvwallInfo().GetDeviceUUID(), cNtfReq.GetTvwallInfo().GetDomainUUID()
                //                    , tDevPri, tDeviceCapPrivileges);

                //                pcUserAccount->AddUserDevInfo(cUserDevInfo);
                //            }

                //        }
                //    }
                //}
            }

			if (pc3acAgent == NULL)
			{
				INSTLOG(CUI_APP, ERROR_LEV, "�յ�����[%s]��֪ͨ��Ϣ[%s--%hu]������û�ж�Ӧ��3acAgent\n",
					pcOspSipMsg->GetSipFromUri().GetURIString().c_str(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
				break;
			}

            cNtfReq.GetTvwallInfo().SetDeviceUUID(cNtfReq.GetTvwallInfo().GetDeviceUUID() + "@" +
                pc3acAgent->GetDomainNameByDomainID(cNtfReq.GetTvwallInfo().GetDomainUUID()));

            vector<CTask*> vctCuAgents = GetTasksArray("CCuAgent");
            if ( ! vctCuAgents.empty() )
            {
                vector<CTask*>::iterator pCuAgentItem = vctCuAgents.begin();
                while ( pCuAgentItem != vctCuAgents.end() )
                {
                    CCuAgent *pcCuAgent = (CCuAgent *)(*pCuAgentItem);
                    CMS_ASSERT( 0 == strcmp(pcCuAgent->GetObjName(), "CCuAgent") );
                    if ( CCuAgent::ONLINE_STATE == pcCuAgent->GetState()
                        && pcCuAgent->m_str3asName == pcOspSipMsg->GetSipFromUri().GetURIString())
                    {   
                        //���ƽ̨֪ͨ��Ϣ
                        pcCuAgent->m_cNotifyPool.InsertTvWallModNotify(cNtfReq);
                    }
                    pCuAgentItem++;
                } 
            }

        }
        break;
    case TAS_TVWALL_DEL_NTF_REQ:
        {
            CTvwallDelNtfReq cNtfReq;
            pcOspSipMsg->GetMsgBody(cNtfReq);

            TSipURI tSipUri;

            //ͳһ�ȸ����ظ���Ϣ
            SendNtfRspToServer(pcMsg);
            C3acAgent *pc3acAgent = (C3acAgent *)g_cCuiDataMgr.Get3asAgentTaskPtrBy3asName(pcOspSipMsg->GetSipFromUri().GetURIString(), this);
            if ( NULL != pc3acAgent )
            {
                Iterator pUserAccountItem;
                while ( ! pUserAccountItem.End() )
                {
                    string *pUserName = NULL;
                    CUserAccount **ppcUserAccount = NULL;
                    CUserAccount *pcUserAccount = NULL;
                    tSipUri.SetUser(cNtfReq.GetTvwallId());
                    tSipUri.SetDomain(::GetDomainName());

                    if ( pc3acAgent->m_cUserMgr.m_mapUserName2PCuserAccount.Iterate(pUserAccountItem, pUserName, ppcUserAccount) )
                    {
                        CMS_ASSERT(NULL != pUserName);
                        CMS_ASSERT(NULL != ppcUserAccount);
                        pcUserAccount = *ppcUserAccount;

                        list<PCuAgent>::iterator item = pcUserAccount->GetCuAgentList().begin();
                        while( item !=  pcUserAccount->GetCuAgentList().end())
                        {
                            CCuAgent *pcCuAgent = (CCuAgent *)(*item);
                            CMS_ASSERT( 0 == strcmp(pcCuAgent->GetObjName(), "CCuAgent") );

                            if ( CCuAgent::ONLINE_STATE == pcCuAgent->GetState())
                            {
                                pcCuAgent->m_cNotifyPool.InsertTvWallDelNotify(tSipUri.GetURIString());
                            }
                            item++;
                        }
                    }
                }
            }
            else
            {
                INSTLOG(CUI_APP, ERROR_LEV, "�յ�����[%s]��֪ͨ��Ϣ[%s--%hu]������û�ж�Ӧ��3acAgent\n",
                    pcOspSipMsg->GetSipFromUri().GetURIString().c_str(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            }

            
            tSipUri.SetDomain(::GetDomainName());
            CPeAgent *pcPeAgent = (CPeAgent *)g_cCuiDataMgr.GetPeAgentPtrByPeName(tSipUri.GetTvsURI().GetURIString(), this);
            if (NULL != pcPeAgent)
            {
                tSipUri.SetUser(cNtfReq.GetTvwallId());
                string strTvwallSubKey = tSipUri.GetURIString() + "+" + TVWALL_ALL_STATUS;
                u32 dwTaskNum = INVALID_TASKNO;
                pcPeAgent->m_cSubscribeTaskSet.Find(strTvwallSubKey, dwTaskNum);

                if(dwTaskNum != INVALID_TASKNO)
                {
                    CSubBaseTask *pcSubscribeTask = dynamic_cast<CSubBaseTask*>(FindTask(dwTaskNum));
                    if (NULL != pcSubscribeTask)
                    {
                        delete pcSubscribeTask;
                    }
                    else
                    {
                        CMS_ASSERT(false);
                        INSTLOG(CUI_APP, ERROR_LEV, "Task Id is not CSubBaseTask, taskNum=%d\n", dwTaskNum);
                    }
                } 
            }
        }
        break;
    case TAS_DEVICE_GROUP_MOD_NTF_REQ:
        {
            CDeviceGroupModNtfReq cNtfReq;
            pcOspSipMsg->GetMsgBody(cNtfReq);

            //ͳһ�ȸ����ظ���Ϣ
            SendNtfRspToServer(pcMsg);

            C3acAgent *pc3acAgent = (C3acAgent *)g_cCuiDataMgr.Get3asAgentTaskPtrBy3asName(pcOspSipMsg->GetSipFromUri().GetURIString(), this);            
            if(NULL != pc3acAgent)
            {
                string strParentDomainName = pc3acAgent->GetDomainNameByDomainID(cNtfReq.GetDeviceGroupInfo().GetDeviceParentGroupID());
                if(!strParentDomainName.empty())
                {
                    cNtfReq.GetDeviceGroupInfo().SetDeviceParentGroupID(cNtfReq.GetDeviceGroupInfo().GetDeviceParentGroupID() + "@" + strParentDomainName);
                }
            }
            string strDomainName = pcOspSipMsg->GetSipFromUri().GetDomain();
            cNtfReq.GetDeviceGroupInfo().SetDeviceGroupID(cNtfReq.GetDeviceGroupInfo().GetDeviceGroupID() + "@" + strDomainName);

            //֪ͨ����cu�û�
            vector<CTask*> vctCuAgents = GetTasksArray("CCuAgent");
            if ( ! vctCuAgents.empty() )
            {
                vector<CTask*>::iterator pCuAgentItem = vctCuAgents.begin();
                while ( pCuAgentItem != vctCuAgents.end() )
                {
                    CCuAgent *pcCuAgent = (CCuAgent *)(*pCuAgentItem);
                    CMS_ASSERT( 0 == strcmp(pcCuAgent->GetObjName(), "CCuAgent") );
                    if ( CCuAgent::ONLINE_STATE == pcCuAgent->GetState() )
                    {
                        pcCuAgent->m_cNotifyPool.InsertDeviceGroupModNotify(cNtfReq);
                    }
                    pCuAgentItem++;
                }
            }
        }
        break;
    case TAS_CUSER_ADD_NTF_REQ:
        {
            CUserAddNtfReq cNtfReq;
            pcOspSipMsg->GetMsgBody(cNtfReq);

            string strDomainName = GetDomainNameWithoutPreNameFromURI(pcOspSipMsg->GetSipFromUri().GetURIString());
            C3acAgent * pcAgent = (C3acAgent *)g_cCuiDataMgr.Get3asAgentTaskPtrBy3asName(pcOspSipMsg->GetSipFromUri().GetURIString(), this);

            CMS_ASSERT(pcAgent != NULL);

            //ͳһ�ȸ����ظ���Ϣ
            SendNtfRspToServer(pcMsg);

            CCuUserInfo cUserInfo;
            cUserInfo = cNtfReq.GetUserInfo();
            string strUserID = cUserInfo.GetUserID();
            cUserInfo.SetUserID(strUserID + "@" + strDomainName);
            cUserInfo.SetDomainID(pcAgent->m_t3ASDomainID);
            cUserInfo.SetUserName(cUserInfo.GetUserName() + "@" + strDomainName);
            if(!cUserInfo.GetUsergrpID().empty())
            {
                cUserInfo.SetUsergrpID(cUserInfo.GetUsergrpID() + "@" + strDomainName);
            }

            string strCreatorID;
            if(!cUserInfo.GetCreatorID().empty())
            {
                strCreatorID = cUserInfo.GetCreatorID();
                cUserInfo.SetCreatorID(strCreatorID  + "@" + strDomainName);
            }

            vector<CTask*> vctCuAgents = GetTasksArray("CCuAgent");
            if ( ! vctCuAgents.empty() )
            {
                vector<CTask*>::iterator pCuAgentItem = vctCuAgents.begin();
                while ( pCuAgentItem != vctCuAgents.end() )
                {
                    CCuAgent *pcCuAgent = (CCuAgent *)(*pCuAgentItem);
                    CMS_ASSERT( 0 == strcmp(pcCuAgent->GetObjName(), "CCuAgent") );
                    if(GetDomainNameWithoutPreNameFromURI(pcCuAgent->m_str3asName) == strDomainName)
                    {
                        if ( CCuAgent::ONLINE_STATE == pcCuAgent->GetState() && 
                            (pcCuAgent->m_pcUserAccount->IsSuperAdmin()
                            || pcCuAgent->m_pcUserAccount->m_tUserID == strCreatorID 
                            || pcCuAgent->m_pcUserAccount->m_tUserID == strUserID))
                        {
                            pcCuAgent->m_cNotifyPool.InsertUserAddNotify(cUserInfo);
                        }
                    }

                    pCuAgentItem++;
                }
            }
        }
        break;
    case TAS_DEVICE_ADD_NTF_REQ:
        {
            CDeviceAddNtfReq cNtfReq;
            pcOspSipMsg->GetMsgBody(cNtfReq);

            //ͳһ�ȸ����ظ���Ϣ
            SendNtfRspToServer(pcMsg);

            C3acAgent *pc3acAgent = (C3acAgent *)g_cCuiDataMgr.Get3asAgentTaskPtrBy3asName(pcOspSipMsg->GetSipFromUri().GetURIString(), this);            
            if(NULL != pc3acAgent)
            {
				TSipURI tDevUri;
				tDevUri.SetUser(cNtfReq.GetDeviceInfo().GetDeviceUUID());
				tDevUri.SetDomain(pc3acAgent->GetDomainNameByDomainID(cNtfReq.GetDeviceInfo().GetDomainUUID()));

                CStrMap<CUserAccount*> & cUserAccountList = pc3acAgent->m_cUserMgr.m_mapUserName2PCuserAccount;
                if(!cUserAccountList.IsEmpty())
                {
                    Iterator pPos;
                    while( ! pPos.End() )
                    {
                        string strUserName;
                        CUserAccount *pcUserAccount;
                        if( cUserAccountList.Iterate(pPos, strUserName, pcUserAccount) )
                        {
                            if( pcUserAccount->IsSuperAdmin() )
                            {
								list<PCuAgent> &pCuAgentList = pcUserAccount->GetCuAgentList();
								for (list<PCuAgent>::iterator cuIter = pCuAgentList.begin();
										cuIter != pCuAgentList.end(); cuIter++)
								{
									cNtfReq.GetDeviceInfo().SetDeviceUUID(tDevUri.GetURIString());
									(*cuIter)->m_cNotifyPool.InsertDeviceAddNotify(cNtfReq.GetDeviceInfo());
								}
                            }
                        }
                    }
                }
            }
        }
        break;
    case TAS_CUSER_GROUP_ADD_NTF_REQ:
        {
            CUserGroupAddNtfReq cNtfReq;
            pcOspSipMsg->GetMsgBody(cNtfReq);

            //ͳһ�ȸ����ظ���Ϣ
            SendNtfRspToServer(pcMsg);

            cNtfReq.GetUserGroupInfo().SetGroupNO(cNtfReq.GetUserGroupInfo().GetGroupNO() + "@" + pcOspSipMsg->GetSipFromUri().GetDomain());
            string strCreatorID = cNtfReq.GetUserGroupInfo().GetCreateUser();
            cNtfReq.GetUserGroupInfo().SetCreateUser( strCreatorID + "@" + pcOspSipMsg->GetSipFromUri().GetDomain());
            //֪ͨ����cu�û�
            vector<CTask*> vctCuAgents = GetTasksArray("CCuAgent");
            if ( ! vctCuAgents.empty() )
            {
                vector<CTask*>::iterator pCuAgentItem = vctCuAgents.begin();
                while ( pCuAgentItem != vctCuAgents.end() )
                {
                    CCuAgent *pcCuAgent = (CCuAgent *)(*pCuAgentItem);
                    CMS_ASSERT( 0 == strcmp(pcCuAgent->GetObjName(), "CCuAgent") );
                    if ( CCuAgent::ONLINE_STATE == pcCuAgent->GetState() 
                        && pcCuAgent->m_pcUserAccount->m_tUserID == strCreatorID)
                    {
                        pcCuAgent->m_cNotifyPool.InsertUserGroupAddNotify(cNtfReq);
                    }
                    pCuAgentItem++;
                }
            }
        }
        break;
    case TAS_DEVICE_GROUP_DEVICE_ADD_NTF_REQ:
        {
            CDeviceGroupDeviceAddNtfReq cNtfReq;
            pcOspSipMsg->GetMsgBody(cNtfReq);

            C3acAgent *pc3acAgent = (C3acAgent *)g_cCuiDataMgr.Get3asAgentTaskPtrBy3asName(pcOspSipMsg->GetSipFromUri().GetURIString(), this);            
            if(pc3acAgent != NULL)
            {
                if(!cNtfReq.GetDeviceGroupDeviceInfo().GetDeviceGroupID().empty())
                {
                    cNtfReq.GetDeviceGroupDeviceInfo().SetDeviceGroupID(cNtfReq.GetDeviceGroupDeviceInfo().GetDeviceGroupID() +
                        "@" + pc3acAgent->GetDomainNameByDomainID(cNtfReq.GetDeviceGroupDeviceInfo().GetDomainId()));
                }

                if(!cNtfReq.GetDeviceGroupDeviceInfo().GetDeviceID().empty())
                {
                    cNtfReq.GetDeviceGroupDeviceInfo().SetDeviceID(cNtfReq.GetDeviceGroupDeviceInfo().GetDeviceID() + 
                        "@" + pc3acAgent->GetDomainNameByDomainID(cNtfReq.GetDeviceGroupDeviceInfo().GetDomainId()));
                }   

                //ͳһ�ȸ����ظ���Ϣ
                SendNtfRspToServer(pcMsg);

                //֪ͨ����cu�û�
                vector<CTask*> vctCuAgents = GetTasksArray("CCuAgent");
                if ( ! vctCuAgents.empty() )
                {
                    vector<CTask*>::iterator pCuAgentItem = vctCuAgents.begin();
                    while ( pCuAgentItem != vctCuAgents.end() )
                    {
                        CCuAgent *pcCuAgent = (CCuAgent *)(*pCuAgentItem);
                        CMS_ASSERT( 0 == strcmp(pcCuAgent->GetObjName(), "CCuAgent") );
                        if ( CCuAgent::ONLINE_STATE == pcCuAgent->GetState() 
                            && GetDomainNameWithoutPreNameFromURI(pcCuAgent->m_str3asName) 
                            ==  pc3acAgent->GetDomainNameByDomainID(cNtfReq.GetDeviceGroupDeviceInfo().GetDomainId()))
                        {
                            pcCuAgent->m_cNotifyPool.InsertDevGroupDevAddNotify(cNtfReq);
                        }
                        pCuAgentItem++;
                    }
                }
            }
        }
        break;
    case TAS_DEVICE_GROUP_DEVICE_DEL_NTF_REQ:
        {
            CDeviceGroupDeviceDelNtfReq cNtfReq;
            pcOspSipMsg->GetMsgBody(cNtfReq);
			C3acAgent *pc3acAgent = (C3acAgent *)g_cCuiDataMgr.Get3asAgentTaskPtrBy3asName(pcOspSipMsg->GetSipFromUri().GetURIString(), this);            
			if(pc3acAgent != NULL)
			{
				string strDomainName = pc3acAgent->GetDomainNameByDomainID(cNtfReq.GetDomainId());
				if(!cNtfReq.GetDeviceGroupDeviceInfoKey().GetDeviceID().empty())
				{
					cNtfReq.GetDeviceGroupDeviceInfoKey().SetDeviceID(cNtfReq.GetDeviceGroupDeviceInfoKey().GetDeviceID() + "@" +
						strDomainName);
				}

				if(!cNtfReq.GetDeviceGroupDeviceInfoKey().GetDeviceGroupID().empty())
				{
					cNtfReq.GetDeviceGroupDeviceInfoKey().SetDeviceGroupID(cNtfReq.GetDeviceGroupDeviceInfoKey().GetDeviceGroupID() + "@" +
						strDomainName);
				}
			}
            //ͳһ�ȸ����ظ���Ϣ
            SendNtfRspToServer(pcMsg);

            //֪ͨ����cu�û�
            vector<CTask*> vctCuAgents = GetTasksArray("CCuAgent");
            if ( ! vctCuAgents.empty() )
            {
                vector<CTask*>::iterator pCuAgentItem = vctCuAgents.begin();
                while ( pCuAgentItem != vctCuAgents.end() )
                {
                    CCuAgent *pcCuAgent = (CCuAgent *)(*pCuAgentItem);
                    CMS_ASSERT( 0 == strcmp(pcCuAgent->GetObjName(), "CCuAgent") );
                    if ( CCuAgent::ONLINE_STATE == pcCuAgent->GetState())
                    {
                        pcCuAgent->m_cNotifyPool.InsertDevGroupDevDelNotify(cNtfReq);
                    }
                    pCuAgentItem++;
                }
            }
        }
        break;
    case TAS_DEVICE_GROUP_ADD_NTF_REQ: 
        {
            CDeviceGroupAddNtfReq cNtfReq;
            pcOspSipMsg->GetMsgBody(cNtfReq);

            string strDomainName;
            C3acAgent *pc3acAgent = (C3acAgent *)g_cCuiDataMgr.Get3asAgentTaskPtrBy3asName(pcOspSipMsg->GetSipFromUri().GetURIString(), this);            
            if(pc3acAgent != NULL)
            {
                string strDomainName = pc3acAgent->GetDomainNameByDomainID(cNtfReq.GetDeviceGroupInfo().GetDomainUUID());
                string strGroupID = cNtfReq.GetDeviceGroupInfo().GetDeviceGroupID();
                string strParentID = cNtfReq.GetDeviceGroupInfo().GetDeviceParentGroupID();
                //string strParentDomainName = g_cCuiDataMgr.GetDomainNameByDomainID(cNtfReq.GetDeviceGroupInfo().GetDeviceParentGroupID());

                if(!strDomainName.empty())
                {
                    if(!strGroupID.empty())
                    {
                        cNtfReq.GetDeviceGroupInfo().SetDeviceGroupID(strGroupID + "@" + strDomainName);
                    }

                    if(!strParentID.empty())
                    {
                        cNtfReq.GetDeviceGroupInfo().SetDeviceParentGroupID(strParentID + "@" + strDomainName);
                    }
                }

                //ͳһ�ȸ����ظ���Ϣ
                SendNtfRspToServer(pcMsg);

                //֪ͨ����cu�û�
                vector<CTask*> vctCuAgents = GetTasksArray("CCuAgent");
                if ( ! vctCuAgents.empty() )
                {
                    vector<CTask*>::iterator pCuAgentItem = vctCuAgents.begin();
                    while ( pCuAgentItem != vctCuAgents.end() )
                    {
                        CCuAgent *pcCuAgent = (CCuAgent *)(*pCuAgentItem);
                        CMS_ASSERT( 0 == strcmp(pcCuAgent->GetObjName(), "CCuAgent") );
                        if ( CCuAgent::ONLINE_STATE == pcCuAgent->GetState())
                        {
                            pcCuAgent->m_cNotifyPool.InsertDeviceGroupAddNotify(cNtfReq);
                        }
                        pCuAgentItem++;
                    }
                }
            }
        }
        break;
	case TAS_DEVICE_GROUP_DEL_NTF_REQ:
		{
			CDeviceGroupDelNtfReq cNtfReq;
			pcOspSipMsg->GetMsgBody(cNtfReq);

			string strNotify ;
			C3acAgent *pc3acAgent = (C3acAgent *)g_cCuiDataMgr.Get3asAgentTaskPtrBy3asName(pcOspSipMsg->GetSipFromUri().GetURIString(), this);            
			if(pc3acAgent != NULL)
			{
				strNotify = cNtfReq.GetDeviceGroupInfoKey().GetDeviceGroupID() + "@" + pc3acAgent->GetDomainNameByDomainID(cNtfReq.GetDomainId());
			}
			else
			{
				strNotify = cNtfReq.GetDeviceGroupInfoKey().GetDeviceGroupID() + "@" + pcOspSipMsg->GetSipFromUri().GetDomain();
			}

			//ͳһ�ȸ����ظ���Ϣ
			SendNtfRspToServer(pcMsg);

			//֪ͨ����cu�û�
			vector<CTask*> vctCuAgents = GetTasksArray("CCuAgent");
			if ( ! vctCuAgents.empty() )
			{
				vector<CTask*>::iterator pCuAgentItem = vctCuAgents.begin();
				while ( pCuAgentItem != vctCuAgents.end() )
				{
					CCuAgent *pcCuAgent = (CCuAgent *)(*pCuAgentItem);
					CMS_ASSERT( 0 == strcmp(pcCuAgent->GetObjName(), "CCuAgent") );
					if ( CCuAgent::ONLINE_STATE == pcCuAgent->GetState() )
					{
						pcCuAgent->m_cNotifyPool.InsertDeviceGroupDelNotify(strNotify);
					}
					pCuAgentItem++;
				}
			}
		}
		break;
    case STUN_UPDATE_REQ:
        {
            CStunUpdateReq cReq;
            pcOspSipMsg->GetMsgBody(cReq);

            string strSessionID = GetSipSessionID(pcOspSipMsg->GetMsgBody());

            //�ԻỰ����ͨ��Ϣ��ͳһ�ظ�Ntf
            INSTLOG(CUI_APP, EVENT_LEV, "ͳһ�ظ�Rsp[%s--%hu]��Ϣ\n",
                OspExtEventDesc(pcMsg->event + 1).c_str(), pcMsg->event + 1);
            CStunUpdateRsp cRsp;
            cRsp.SetSession(strSessionID);
            string strSeqNum = GetSipSeqNO(pcOspSipMsg->GetMsgBody());
            int nSeqNum = static_cast<int>(atol(strSeqNum.c_str()));
            cRsp.SetSeqNum(nSeqNum);
            TOspTransID tSrcTransId;
            tSrcTransId.dwIID = MAKEIID(GetAppID(), GetInsID());
            tSrcTransId.dwSeqNO = (u32)nSeqNum;
            tSrcTransId.dwTaskID = INVALID_TASKNO;
            OspSipPostRsp(KDSIP_EVENT_MESSAGE_RSP, pcOspSipMsg->GetSipTransID(), cRsp, tSrcTransId);

            this->SetStunAddr(cReq.GetStunAddr());

            //֪ͨ����cu�û�
            vector<CTask*> vctCuAgents = GetTasksArray("CCuAgent");
            if ( ! vctCuAgents.empty() )
            {
                vector<CTask*>::iterator pCuAgentItem = vctCuAgents.begin();
                while ( pCuAgentItem != vctCuAgents.end() )
                {
                    CCuAgent *pcCuAgent = (CCuAgent *)(*pCuAgentItem);
                    CMS_ASSERT( 0 == strcmp(pcCuAgent->GetObjName(), "CCuAgent") );
                    if ( CCuAgent::ONLINE_STATE == pcCuAgent->GetState())
                    {
                        pcCuAgent->m_cNotifyPool.InsertStunUpdateNotify(cReq);
                    }
                    pCuAgentItem++;
                }
            }
        }
        break;
    case TAS_TEMP_AUTHORIZATION_ADD_NTF_REQ:
    case TAS_TEMP_AUTHORIZATION_DEL_NTF_REQ:
    case TAS_TEMP_AUTHORIZATION_MOD_NTF_REQ:
    case TAS_DEVICE_MODEL_ADD_NTF_REQ:
    case TAS_DEVICE_MODEL_DEL_NTF_REQ:
    case TAS_TACUSER_ADD_NTF_REQ:
    case TAS_TACUSER_DEL_NTF_REQ:
    case TAS_TACUSER_MOD_NTF_REQ:
        {
            //����Ҫ����
            SendNtfRspToServer(pcMsg);
        }
        break;
	case TAS_PTZLOCK_ADD_NTF_REQ:
		{
			CPtzLockAddNtfReq cNtfReq;
			pcOspSipMsg->GetMsgBody(cNtfReq);

			C3acAgent *pc3acAgent = (C3acAgent *)g_cCuiDataMgr.Get3asAgentTaskPtrBy3asName(pcOspSipMsg->GetSipFromUri().GetURIString(), this);
			if (pc3acAgent != NULL)
			{
				CPtzLockInfo &tasPtzLockInfo = cNtfReq.GetInfo();

				CCuPtzLockInfo cuPtzLockInfo;
				cuPtzLockInfo.SetDeviceId(tasPtzLockInfo.GetDeviceId());
				cuPtzLockInfo.SetVideoSrcId(tasPtzLockInfo.GetVideoSrcId());
				cuPtzLockInfo.SetUserId(tasPtzLockInfo.GetUserId());
				cuPtzLockInfo.SetBeginTime(tasPtzLockInfo.GetBeginTime());
				cuPtzLockInfo.SetEndTime(tasPtzLockInfo.GetEndTime());
				cuPtzLockInfo.SetPassword(tasPtzLockInfo.GetPassword());

				pc3acAgent->m_cPtzlockInfoList.AddCuPtzLockInfo(cuPtzLockInfo);
			}
			else
			{
				INSTLOG(CUI_APP, ERROR_LEV, "�Ҳ���3A���󣿼���ʧ��!![%s]\n", cNtfReq.ToXml().c_str());
			}
			SendNtfRspToServer(pcMsg);
		}
		break;
	case TAS_PTZLOCK_DEL_NTF_REQ:
		{
			CPtzLockDelNtfReq cNtfReq;
			pcOspSipMsg->GetMsgBody(cNtfReq);

			C3acAgent *pc3acAgent = (C3acAgent *)g_cCuiDataMgr.Get3asAgentTaskPtrBy3asName(pcOspSipMsg->GetSipFromUri().GetURIString(), this);
			if (pc3acAgent != NULL)
			{
				TChannel vsChn;
				vsChn.SetDevUri(cNtfReq.GetDeviceId());
				vsChn.SetChnNO(cNtfReq.GetVideoSrcId());
				pc3acAgent->m_cPtzlockInfoList.DelCuPtzLockInfoByVs(vsChn);
			}
			else
			{
				INSTLOG(CUI_APP, ERROR_LEV, "�Ҳ���3A����ɾ��ʧ��!![%s]\n", cNtfReq.ToXml().c_str());
			}

			SendNtfRspToServer(pcMsg);
		}
		break;
	case MPS_MAP_ADD_NTF_REQ:
		{
			CMapAddNtfReq cReq;
			pcOspSipMsg->GetMsgBody(cReq);
                 
			SendNtfRspToServer(pcMsg);

// 			cReq.GetMapInfo().SetMapId(cReq.GetMapInfo().GetMapId()+pcOspSipMsg->GetSipFromUri().GetDomain());
// 			cReq.GetMapInfo().SetParentMapId(cReq.GetMapInfo().GetParentMapId()+pcOspSipMsg->GetSipFromUri().GetDomain());
			//֪ͨ����cu�û�
			vector<CTask*> vctCuAgents = GetTasksArray("CCuAgent");
			if ( ! vctCuAgents.empty() )
			{
				vector<CTask*>::iterator pCuAgentItem = vctCuAgents.begin();
				while ( pCuAgentItem != vctCuAgents.end() )
				{
					CCuAgent *pcCuAgent = (CCuAgent *)(*pCuAgentItem);
					CMS_ASSERT( 0 == strcmp(pcCuAgent->GetObjName(), "CCuAgent") );
					if ( CCuAgent::ONLINE_STATE == pcCuAgent->GetState())
					{
						pcCuAgent->m_cNotifyPool.InsertMapAddNotify(cReq);
					}
					pCuAgentItem++;
				}
			}
		}
		break;
	case MPS_MAP_MOD_NTF_REQ:
		{
			CMapModNtfReq cReq;
			pcOspSipMsg->GetMsgBody(cReq);

			SendNtfRspToServer(pcMsg);

			// 			cReq.GetMapInfo().SetMapId(cReq.GetMapInfo().GetMapId()+pcOspSipMsg->GetSipFromUri().GetDomain());
			// 			cReq.GetMapInfo().SetParentMapId(cReq.GetMapInfo().GetParentMapId()+pcOspSipMsg->GetSipFromUri().GetDomain());
			//֪ͨ����cu�û�
			vector<CTask*> vctCuAgents = GetTasksArray("CCuAgent");
			if ( ! vctCuAgents.empty() )
			{
				vector<CTask*>::iterator pCuAgentItem = vctCuAgents.begin();
				while ( pCuAgentItem != vctCuAgents.end() )
				{
					CCuAgent *pcCuAgent = (CCuAgent *)(*pCuAgentItem);
					CMS_ASSERT( 0 == strcmp(pcCuAgent->GetObjName(), "CCuAgent") );
					if ( CCuAgent::ONLINE_STATE == pcCuAgent->GetState())
					{
						pcCuAgent->m_cNotifyPool.InsertMapModNotify(cReq);
					}
					pCuAgentItem++;
				}
			}
		}
		break;
	case MPS_MAP_DEL_NTF_REQ:
		{
			CMapDelNtfReq cReq;
			pcOspSipMsg->GetMsgBody(cReq);

			SendNtfRspToServer(pcMsg);

			// 			cReq.GetMapInfo().SetMapId(cReq.GetMapInfo().GetMapId()+pcOspSipMsg->GetSipFromUri().GetDomain());
			// 			cReq.GetMapInfo().SetParentMapId(cReq.GetMapInfo().GetParentMapId()+pcOspSipMsg->GetSipFromUri().GetDomain());
			//֪ͨ����cu�û�
			vector<CTask*> vctCuAgents = GetTasksArray("CCuAgent");
			if ( ! vctCuAgents.empty() )
			{
				vector<CTask*>::iterator pCuAgentItem = vctCuAgents.begin();
				while ( pCuAgentItem != vctCuAgents.end() )
				{
					CCuAgent *pcCuAgent = (CCuAgent *)(*pCuAgentItem);
					CMS_ASSERT( 0 == strcmp(pcCuAgent->GetObjName(), "CCuAgent") );
					if ( CCuAgent::ONLINE_STATE == pcCuAgent->GetState())
					{
						pcCuAgent->m_cNotifyPool.InsertMapDelNotify(cReq);
					}
					pCuAgentItem++;
				}
			}
		}
		break;
	case MPS_MAP_LAYER_ADD_NTF_REQ:
		{
			CMapLayerAddNtfReq cReq;
			pcOspSipMsg->GetMsgBody(cReq);

			SendNtfRspToServer(pcMsg);

			// 			cReq.GetMapInfo().SetMapId(cReq.GetMapInfo().GetMapId()+pcOspSipMsg->GetSipFromUri().GetDomain());
			// 			cReq.GetMapInfo().SetParentMapId(cReq.GetMapInfo().GetParentMapId()+pcOspSipMsg->GetSipFromUri().GetDomain());
			//֪ͨ����cu�û�
			vector<CTask*> vctCuAgents = GetTasksArray("CCuAgent");
			if ( ! vctCuAgents.empty() )
			{
				vector<CTask*>::iterator pCuAgentItem = vctCuAgents.begin();
				while ( pCuAgentItem != vctCuAgents.end() )
				{
					CCuAgent *pcCuAgent = (CCuAgent *)(*pCuAgentItem);
					CMS_ASSERT( 0 == strcmp(pcCuAgent->GetObjName(), "CCuAgent") );
					if ( CCuAgent::ONLINE_STATE == pcCuAgent->GetState())
					{
						pcCuAgent->m_cNotifyPool.InsertMapLayerAddNotify(cReq);
					}
					pCuAgentItem++;
				}
			}
		}
		break;
	case MPS_MAP_LAYER_MOD_NTF_REQ:
		{
			CMapLayerModNtfReq cReq;
			pcOspSipMsg->GetMsgBody(cReq);

			SendNtfRspToServer(pcMsg);

			// 			cReq.GetMapInfo().SetMapId(cReq.GetMapInfo().GetMapId()+pcOspSipMsg->GetSipFromUri().GetDomain());
			// 			cReq.GetMapInfo().SetParentMapId(cReq.GetMapInfo().GetParentMapId()+pcOspSipMsg->GetSipFromUri().GetDomain());
			//֪ͨ����cu�û�
			vector<CTask*> vctCuAgents = GetTasksArray("CCuAgent");
			if ( ! vctCuAgents.empty() )
			{
				vector<CTask*>::iterator pCuAgentItem = vctCuAgents.begin();
				while ( pCuAgentItem != vctCuAgents.end() )
				{
					CCuAgent *pcCuAgent = (CCuAgent *)(*pCuAgentItem);
					CMS_ASSERT( 0 == strcmp(pcCuAgent->GetObjName(), "CCuAgent") );
					if ( CCuAgent::ONLINE_STATE == pcCuAgent->GetState())
					{
						pcCuAgent->m_cNotifyPool.InsertMapLayerModNotify(cReq);
					}
					pCuAgentItem++;
				}
			}
		}
		break;
	case MPS_MAP_LAYER_DEL_NTF_REQ:
		{
			CMapLayerDelNtfReq cReq;
			pcOspSipMsg->GetMsgBody(cReq);

			SendNtfRspToServer(pcMsg);

			// 			cReq.GetMapInfo().SetMapId(cReq.GetMapInfo().GetMapId()+pcOspSipMsg->GetSipFromUri().GetDomain());
			// 			cReq.GetMapInfo().SetParentMapId(cReq.GetMapInfo().GetParentMapId()+pcOspSipMsg->GetSipFromUri().GetDomain());
			//֪ͨ����cu�û�
			vector<CTask*> vctCuAgents = GetTasksArray("CCuAgent");
			if ( ! vctCuAgents.empty() )
			{
				vector<CTask*>::iterator pCuAgentItem = vctCuAgents.begin();
				while ( pCuAgentItem != vctCuAgents.end() )
				{
					CCuAgent *pcCuAgent = (CCuAgent *)(*pCuAgentItem);
					CMS_ASSERT( 0 == strcmp(pcCuAgent->GetObjName(), "CCuAgent") );
					if ( CCuAgent::ONLINE_STATE == pcCuAgent->GetState())
					{
						pcCuAgent->m_cNotifyPool.InsertMapLayerDelNotify(cReq);
					}
					pCuAgentItem++;
				}
			}
		}
		break;
	case MPS_MAP_AREA_ADD_NTF_REQ:
		{
			CMapAreaAddNtfReq cReq;
			pcOspSipMsg->GetMsgBody(cReq);

			SendNtfRspToServer(pcMsg);

			// 			cReq.GetMapInfo().SetMapId(cReq.GetMapInfo().GetMapId()+pcOspSipMsg->GetSipFromUri().GetDomain());
			// 			cReq.GetMapInfo().SetParentMapId(cReq.GetMapInfo().GetParentMapId()+pcOspSipMsg->GetSipFromUri().GetDomain());
			//֪ͨ����cu�û�
			vector<CTask*> vctCuAgents = GetTasksArray("CCuAgent");
			if ( ! vctCuAgents.empty() )
			{
				vector<CTask*>::iterator pCuAgentItem = vctCuAgents.begin();
				while ( pCuAgentItem != vctCuAgents.end() )
				{
					CCuAgent *pcCuAgent = (CCuAgent *)(*pCuAgentItem);
					CMS_ASSERT( 0 == strcmp(pcCuAgent->GetObjName(), "CCuAgent") );
					if ( CCuAgent::ONLINE_STATE == pcCuAgent->GetState())
					{
						pcCuAgent->m_cNotifyPool.InsertMapAreaAddNotify(cReq);
					}
					pCuAgentItem++;
				}
			}
		}
		break;
	case MPS_MAP_AREA_MOD_NTF_REQ:
		{
			CMapAreaModNtfReq cReq;
			pcOspSipMsg->GetMsgBody(cReq);

			SendNtfRspToServer(pcMsg);

			// 			cReq.GetMapInfo().SetMapId(cReq.GetMapInfo().GetMapId()+pcOspSipMsg->GetSipFromUri().GetDomain());
			// 			cReq.GetMapInfo().SetParentMapId(cReq.GetMapInfo().GetParentMapId()+pcOspSipMsg->GetSipFromUri().GetDomain());
			//֪ͨ����cu�û�
			vector<CTask*> vctCuAgents = GetTasksArray("CCuAgent");
			if ( ! vctCuAgents.empty() )
			{
				vector<CTask*>::iterator pCuAgentItem = vctCuAgents.begin();
				while ( pCuAgentItem != vctCuAgents.end() )
				{
					CCuAgent *pcCuAgent = (CCuAgent *)(*pCuAgentItem);
					CMS_ASSERT( 0 == strcmp(pcCuAgent->GetObjName(), "CCuAgent") );
					if ( CCuAgent::ONLINE_STATE == pcCuAgent->GetState())
					{
						pcCuAgent->m_cNotifyPool.InsertMapAreaModNotify(cReq);
					}
					pCuAgentItem++;
				}
			}
		}
		break;
	case MPS_MAP_AREA_DEL_NTF_REQ:
		{
			CMapAreaDelNtfReq cReq;
			pcOspSipMsg->GetMsgBody(cReq);

			SendNtfRspToServer(pcMsg);

			// 			cReq.GetMapInfo().SetMapId(cReq.GetMapInfo().GetMapId()+pcOspSipMsg->GetSipFromUri().GetDomain());
			// 			cReq.GetMapInfo().SetParentMapId(cReq.GetMapInfo().GetParentMapId()+pcOspSipMsg->GetSipFromUri().GetDomain());
			//֪ͨ����cu�û�
			vector<CTask*> vctCuAgents = GetTasksArray("CCuAgent");
			if ( ! vctCuAgents.empty() )
			{
				vector<CTask*>::iterator pCuAgentItem = vctCuAgents.begin();
				while ( pCuAgentItem != vctCuAgents.end() )
				{
					CCuAgent *pcCuAgent = (CCuAgent *)(*pCuAgentItem);
					CMS_ASSERT( 0 == strcmp(pcCuAgent->GetObjName(), "CCuAgent") );
					if ( CCuAgent::ONLINE_STATE == pcCuAgent->GetState())
					{
						pcCuAgent->m_cNotifyPool.InsertMapAreaDelNotify(cReq);
					}
					pCuAgentItem++;
				}
			}
		}
		break;
	case MPS_MAP_ELEMENT_ADD_NTF_REQ:
		{
			CMapElementAddNtfReq cReq;
			pcOspSipMsg->GetMsgBody(cReq);

			SendNtfRspToServer(pcMsg);

			// 			cReq.GetMapInfo().SetMapId(cReq.GetMapInfo().GetMapId()+pcOspSipMsg->GetSipFromUri().GetDomain());
			// 			cReq.GetMapInfo().SetParentMapId(cReq.GetMapInfo().GetParentMapId()+pcOspSipMsg->GetSipFromUri().GetDomain());
			//֪ͨ����cu�û�
			vector<CTask*> vctCuAgents = GetTasksArray("CCuAgent");
			if ( ! vctCuAgents.empty() )
			{
				vector<CTask*>::iterator pCuAgentItem = vctCuAgents.begin();
				while ( pCuAgentItem != vctCuAgents.end() )
				{
					CCuAgent *pcCuAgent = (CCuAgent *)(*pCuAgentItem);
					CMS_ASSERT( 0 == strcmp(pcCuAgent->GetObjName(), "CCuAgent") );
					if ( CCuAgent::ONLINE_STATE == pcCuAgent->GetState())
					{
						pcCuAgent->m_cNotifyPool.InsertMapElementAddNotify(cReq);
					}
					pCuAgentItem++;
				}
			}
		}
		break;
	case MPS_MAP_ELEMENT_MOD_NTF_REQ:
		{
			CMapElementModNtfReq cReq;
			pcOspSipMsg->GetMsgBody(cReq);

			SendNtfRspToServer(pcMsg);

			// 			cReq.GetMapInfo().SetMapId(cReq.GetMapInfo().GetMapId()+pcOspSipMsg->GetSipFromUri().GetDomain());
			// 			cReq.GetMapInfo().SetParentMapId(cReq.GetMapInfo().GetParentMapId()+pcOspSipMsg->GetSipFromUri().GetDomain());
			//֪ͨ����cu�û�
			vector<CTask*> vctCuAgents = GetTasksArray("CCuAgent");
			if ( ! vctCuAgents.empty() )
			{
				vector<CTask*>::iterator pCuAgentItem = vctCuAgents.begin();
				while ( pCuAgentItem != vctCuAgents.end() )
				{
					CCuAgent *pcCuAgent = (CCuAgent *)(*pCuAgentItem);
					CMS_ASSERT( 0 == strcmp(pcCuAgent->GetObjName(), "CCuAgent") );
					if ( CCuAgent::ONLINE_STATE == pcCuAgent->GetState())
					{
						pcCuAgent->m_cNotifyPool.InsertMapElementModNotify(cReq);
					}
					pCuAgentItem++;
				}
			}
		}
		break;
	case MPS_MAP_ELEMENT_DEL_NTF_REQ:
		{
			CMapElementDelNtfReq cReq;
			pcOspSipMsg->GetMsgBody(cReq);

			SendNtfRspToServer(pcMsg);

			// 			cReq.GetMapInfo().SetMapId(cReq.GetMapInfo().GetMapId()+pcOspSipMsg->GetSipFromUri().GetDomain());
			// 			cReq.GetMapInfo().SetParentMapId(cReq.GetMapInfo().GetParentMapId()+pcOspSipMsg->GetSipFromUri().GetDomain());
			//֪ͨ����cu�û�
			vector<CTask*> vctCuAgents = GetTasksArray("CCuAgent");
			if ( ! vctCuAgents.empty() )
			{
				vector<CTask*>::iterator pCuAgentItem = vctCuAgents.begin();
				while ( pCuAgentItem != vctCuAgents.end() )
				{
					CCuAgent *pcCuAgent = (CCuAgent *)(*pCuAgentItem);
					CMS_ASSERT( 0 == strcmp(pcCuAgent->GetObjName(), "CCuAgent") );
					if ( CCuAgent::ONLINE_STATE == pcCuAgent->GetState())
					{
						pcCuAgent->m_cNotifyPool.InsertMapElementDelNotify(cReq);
					}
					pCuAgentItem++;
				}
			}
		}
		break;
	case GBS_GBPARENTDOMAIN_CL_PROGRESS_NTF_REQ:
		{
			CGbsGbPDCLProgressNtfReq cReq;
			pcOspSipMsg->GetMsgBody(cReq);

			//ͳһ�ȸ����ظ���Ϣ
			SendNtfRspToServer(pcMsg);

			vector<CTask*> vctCuAgents = GetTasksArray("CCuAgent");
			if ( ! vctCuAgents.empty() )
			{
				vector<CTask*>::iterator pCuAgentItem = vctCuAgents.begin();
				while ( pCuAgentItem != vctCuAgents.end() )
				{
					CCuAgent *pcCuAgent = (CCuAgent *)(*pCuAgentItem);
					CMS_ASSERT( 0 == strcmp(pcCuAgent->GetObjName(), "CCuAgent") );

					string cuSession = cReq.GetCuSession();

					if ( CCuAgent::ONLINE_STATE == pcCuAgent->GetState()
						&& pcCuAgent->m_strSession == cuSession
						)
					{
						pcCuAgent->m_cNotifyPool.InsertGbsGbPDCLProgressNotify(cReq);
					}
					pCuAgentItem++;
				}
			}
		}
		break;
	case ALARM_EVENT_NTF_REQ:
		{
			CAlarmEventNtfReq cReq;
			pcOspSipMsg->GetMsgBody(cReq);

			//ͳһ�ȸ����ظ���Ϣ
			SendNtfRspToServer(pcMsg);

			//֪ͨ����cu�û�
			vector<CTask*> vctCuAgents = GetTasksArray("CCuAgent");
			if ( vctCuAgents.empty() )
			{
				break; // switch
			}

			vector<CTask*>::iterator pCuAgentItem = vctCuAgents.begin();
			while ( pCuAgentItem != vctCuAgents.end() )
			{
				CCuAgent *pcCuAgent = (CCuAgent *)(*pCuAgentItem);
				CMS_ASSERT( 0 == strcmp(pcCuAgent->GetObjName(), "CCuAgent") );
				if ( CCuAgent::ONLINE_STATE == pcCuAgent->GetState())
				{
					// ���ݸ澯�����ж��Ƿ�����CU����Ҫ֪ͨ
					string eventType = cReq.GetEventType();
					
					// ¼��澯��Ҫ�ж��ѵ�¼CU�Ƿ񻮹��˸��豸
					// �豸Ƶ��������Ҫ�ж��ѵ�¼CU�Ƿ񻮹��˸��豸
					if ( ( eventType == GetLanDesc(LAN_ALARM_EVENT_TYPE_RECORD) ||  eventType == GetLanDesc(LAN_DESC_PU_OFFLINE_TOO_BRIEF))
						&& cReq.GetDevType() == PU_ENCODER )
					{
						CUserAccount *paccount = pcCuAgent->m_pcUserAccount;
						if (paccount != NULL && !paccount->IsSuperAdmin() )
						{
							string cuSession = pcCuAgent->m_strSession;
							string devUUid = GetNameWithoutDomainFromURI(cReq.GetDevUri());
							
							if (!g_cRedisDb.IsUserHasDev(pcCuAgent->m_pcUserAccount->m_tUserID, devUUid) )
							{
								INSTLOG(CUI_APP, EVENT_LEV, "CUû�л�����豸����֪ͨCU[Session: %s], CAlarmEventNtfReq[%s]\n", pcCuAgent->m_strSession.c_str(), cReq.ToXml().c_str());
								pCuAgentItem++;
								continue;
							}

							// ¼��澯֪ͨ����Ҫ�ж�CU�Ƿ񻮹������ͨ��
							if ( eventType == GetLanDesc(LAN_ALARM_EVENT_TYPE_RECORD) )
							{
								CVideosourcePrivilege cPri;
								string errMsg;
								EFDB_ErrorCode er = g_cRedisDb.GetUserVideoPriByEncChn(pcCuAgent->m_pcUserAccount->m_tUserID, devUUid, cReq.GetChnNO(), cPri, errMsg);
								if ( !IsFdbResultPositive(er) )
								{
									INSTLOG(CUI_APP, EVENT_LEV, "CUû�л����ͨ������֪ͨCU[Session: %s], CAlarmEventNtfReq[%s] errMsg[%s]\n", pcCuAgent->m_strSession.c_str(), cReq.ToXml().c_str(), errMsg.c_str());
									pCuAgentItem++;
									continue;
								}
							}
						}
						else
						{
							if (paccount == NULL)
							{
								INSTLOG(CUI_APP, EVENT_LEV, "CuAgentû���˻���Ϣ��m_pcUserAccount=NULL,[Session: %s]\n", pcCuAgent->m_strSession.c_str());
							}
						}
					}
					pcCuAgent->m_cNotifyPool.InsertAlarmEventNotify(cReq);
				}
				pCuAgentItem++;
			} // while
		}
		break;
    default:
        {
            INSTLOG(CUI_APP, ERROR_LEV, "[OnServicePlatformNtfMsg]CUI����δ֪����[%s--%hu]\n", 
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            CMS_ASSERT(0 && "[OnServicePlatformNtfMsg]CUI����δ֪����");
        }
        break;
    }
}

BOOL32 CCuiInst::OnUserInnerCommonMsg( CMessage* const pcMsg )
{
    BOOL32 bIsInnerMsg = TRUE;
    switch (pcMsg->event)
    {
	case CUI_REDIS_DISC:
		{
			OspPrintf(TRUE, FALSE, "Redis disconnect, cui quit!\n");
			OnRedisDisconnect();
		}
		break;
    case CUI_PRINT_CU_LIST:
        {
            u32 bPrintSubTask = *(u32 *)pcMsg->content;
            OspPrintf(TRUE, FALSE, "\n====================== Begin =====================\n");
            vector<CTask*> vctCuAgents = GetTasksArray("CCuAgent");
            if ( vctCuAgents.empty() )
            {
                OspPrintf(TRUE, FALSE, "û��CU����\n");
            }
            else
            {
                int nIndex = 1;
                vector<CTask*>::iterator pcCuAgentItem = vctCuAgents.begin();
                while ( pcCuAgentItem != vctCuAgents.end() )
                {
                    CMS_ASSERT( 0 == strcmp((*pcCuAgentItem)->GetObjName(), "CCuAgent" ) );
                    CCuAgent *pcCuAgentTask = (CCuAgent *)(*pcCuAgentItem);

                    OspPrintf(TRUE, FALSE, "[%02d]�û�\n",nIndex++);
                    //OspPrintf(TRUE, FALSE, "\t�û�IP[%s]",
                    //    pcCuAgentTask->m_strClientIp.c_str());
                    //OspPrintf(TRUE, FALSE, "\t�ͻ�������[%s]",
                    //    pcCuAgentTask->m_strClientType.c_str());
                    //OspPrintf(TRUE, FALSE, "\tCMU�ĻỰID[%s]��TAS�ĻỰ[%s]\n", 
                    //    pcCuAgentTask->m_strSession.c_str(), pcCuAgentTask->m_strCuSessionIn3AS.c_str());
					// ֱ��ʹ��task��printdata��������ֻһ����ӡ [2/11/2015 pangubing]
					pcCuAgentTask->PrintData();
                    OspPrintf(TRUE, FALSE, "\tTaskNo[%u] ����[%s]״̬[ %u ]��(�������[%u]����Ϊ������ʱ)\n",
                        pcCuAgentTask->GetTaskNO(), pcCuAgentTask->GetStateName(pcCuAgentTask->GetState()), 
                        pcCuAgentTask->GetCurStateHoldTime(), g_cCuiConfig.GetCuHbTimeout());

                    ///////////////////////////////////////////////////////////////////////////////////
                    OspPrintf(TRUE, FALSE, "\t�����������: %d\n",
                        pcCuAgentTask->m_cDevSubTaskSet.GetSize());
                    int nSunIndex = 1;
                    if( 1 == bPrintSubTask && 0 != pcCuAgentTask->m_cDevSubTaskSet.GetSize())
                    {
                        Iterator pPos;
                        while( ! pPos.End())
                        {
                            string strDevType;
                            u32 dwTaskNO = INVALID_TASKNO;
                            if ( pcCuAgentTask->m_cDevSubTaskSet.Iterate(pPos, strDevType, dwTaskNO))
                            {
                                if( INVALID_TASKNO == dwTaskNO)
                                {
                                    OspPrintf(TRUE, FALSE, "CuAgent�Ķ���������TaskNOΪ0\n");
                                    continue;
                                }

                                CSubBaseTask *pcSubBaseTask = (CSubBaseTask *)FindTask(dwTaskNO);
                                if ( NULL == pcSubBaseTask)
                                {
                                    OspPrintf(TRUE, FALSE, "CuAgent�Ķ���������TaskNOΪ %u, ��Ӧ�Ķ��������Ѿ�����\n",
                                        dwTaskNO);
                                    continue;
                                }

                                OspPrintf(TRUE, FALSE, "\t\t[%02d] TaskNO[%u} ���ĻỰDialogID[%u]�� �����豸Uri[%s] ����״̬[%s]\n",
                                    nSunIndex++, 
                                    pcSubBaseTask->GetTaskNO(),
                                    pcSubBaseTask->m_tSubscribeID, 
                                    pcSubBaseTask->m_strDevUri.c_str(),
                                    pcSubBaseTask->m_strSubType.c_str());
                            }
                        }
                    }

                    OspPrintf(TRUE, FALSE, "\t�����������������: %d\n",
                        pcCuAgentTask->m_cCallPuTaskSet.GetSize());
                    int nCallIndex = 1;
                    if( 1== bPrintSubTask && 0 != pcCuAgentTask->m_cCallPuTaskSet.GetSize())
                    {
                        Iterator pPos;
                        while( ! pPos.End())
                        {
                            s32 dwPlayId;
                            u32 dwTaskNO = INVALID_TASKNO;
                            if (pcCuAgentTask->m_cCallPuTaskSet.Iterate(pPos, dwPlayId, dwTaskNO))
                            {
                                if( INVALID_TASKNO == dwTaskNO)
                                {
                                    OspPrintf(TRUE, FALSE, "CuAgent����������������TaskNOΪ0\n");
                                    continue;
                                }

                                CCallPuTask *pcCallPuTask = (CCallPuTask*)FindTask(dwTaskNO);
                                if ( NULL == pcCallPuTask )
                                {
                                    OspPrintf(TRUE, FALSE, "CuAgent����������������TaskNOΪ %u, ��Ӧ��CCallPuTask�����Ѿ�����\n",
                                        dwTaskNO);
                                    continue;
                                }

                                CMS_ASSERT( 0 == strcmp(pcCallPuTask->GetObjName(), "CCallPuTask") );
                                OspPrintf(TRUE, FALSE, "\t\t[%02d] TaskNO[%u] cu session[%s] �������лỰDialogID[%u]�� ����Cu[%s-%d]\n",
                                    nCallIndex++, 
                                    pcCallPuTask->GetTaskNO(),
                                    pcCallPuTask->m_strCuSession.c_str(),
                                    pcCallPuTask->m_tDialogID, 
                                    pcCallPuTask->m_tCuChn.GetDevUri().c_str(),pcCallPuTask->m_tCuChn.GetChnNO());
                            }
                        }
                    }

                    OspPrintf(TRUE, FALSE, "\t����invite���������: %d\n",
                        pcCuAgentTask->m_cPeInviteTaskSet.GetSize());
                    int nPeInviteIndex = 1;
                    if( 1 == bPrintSubTask && 0 != pcCuAgentTask->m_cPeInviteTaskSet.GetSize())
                    {
                        Iterator pPos;
                        while( ! pPos.End() )
                        {
                            s32 dwPlayId;
                            u32 dwTaskNO = INVALID_TASKNO;
                            if (pcCuAgentTask->m_cPeInviteTaskSet.Iterate(pPos, dwPlayId, dwTaskNO))
                            {
                                if( INVALID_TASKNO == dwTaskNO)
                                {
                                    OspPrintf(TRUE, FALSE, "CuAgent�Ķ���������TaskNOΪ0\n");
                                    continue;
                                }

                                CPeInviteTask *pcPeInviteTask = (CPeInviteTask*)FindTask(dwTaskNO);
                                if ( NULL == pcPeInviteTask )
                                {
                                    OspPrintf(TRUE, FALSE, "CuAgent������Invite������TaskNOΪ %u, ��Ӧ��CPeInviteTask�����Ѿ�����\n",
                                        dwTaskNO);
                                    continue;
                                }
                                else
                                {
                                    //CMS_ASSERT( 0 == strcmp(pcPeInviteTask->GetObjName(), "CPeInviteTask") );
                                    OspPrintf(TRUE, FALSE, "\t\t[%02d] TaskNO[%u] ����[%s]session[%s] invite�ỰDialogID[%u]�� invite�豸[%s-%d] invite Cu[%s-%d]\n",
                                        nPeInviteIndex++, 
                                        pcPeInviteTask->GetTaskNO(),
                                        pcPeInviteTask->m_tPeUri.GetURIString().c_str(),
                                        pcPeInviteTask->m_strPeSession.c_str(),
                                        pcPeInviteTask->m_tInviteID, 
                                        pcPeInviteTask->m_tReq.GetSrcChn().GetDevUri().c_str(),pcPeInviteTask->m_tReq.GetSrcChn().GetChnNO(),
                                        pcPeInviteTask->m_tReq.GetDstChn().GetDevUri().c_str(),pcPeInviteTask->m_tReq.GetDstChn().GetChnNO());
                                }
                            }
                        }
                    }

                    OspPrintf(TRUE, FALSE, "\tInvite���������: %d\n",
                        pcCuAgentTask->m_cInviteTaskSet.GetSize());
                    int nInviteIndex = 1;
                    if ( 1 == bPrintSubTask && pcCuAgentTask->m_cInviteTaskSet.GetSize() != 0 )
                    {
                        Iterator pPos;
                        while ( ! pPos.End() )
                        {
                            s32 nPlayId = INVALID_PLAY_ID;
                            u32 dwTaskNO = INVALID_TASKNO;
                            if ( pcCuAgentTask->m_cInviteTaskSet.Iterate(pPos, nPlayId, dwTaskNO) )
                            {
                                if ( INVALID_PLAY_ID == nPlayId )
                                {
                                    OspPrintf(TRUE, FALSE, "CuAgent��Invite�����У�PlayIDΪ-1�Ƿ�ֵ\n");
                                    continue;
                                }
                                if ( INVALID_TASKNO == dwTaskNO )
                                {
                                    OspPrintf(TRUE, FALSE, "CuAgent��Invite������TaskNOΪ0\n");
                                    continue;
                                }
                                CInviteTask *pcInviteTask = (CInviteTask *)FindTask(dwTaskNO);
                                if ( NULL == pcInviteTask )
                                {
                                    OspPrintf(TRUE, FALSE, "CuAgent��Invite������TaskNOΪ %u, ��Ӧ��CInviteTask�����Ѿ�����\n",
                                        dwTaskNO);
                                    continue;
                                }
                                CMS_ASSERT( 0 == strcmp(pcInviteTask->GetObjName(), "CInviteTask") );
                                OspPrintf(TRUE, FALSE, "\t\t[%02d] TaskNO[%u] InviteDialog[%u] Cu���Ŵ���[%d]����豸ͨ��[%s--%d]\n",
                                    nInviteIndex++,
                                    pcInviteTask->GetTaskNO(),
                                    pcInviteTask->m_tInviteID,
                                    pcInviteTask->m_tReq.GetDstChn().GetChnNO(),
                                    pcInviteTask->m_tReq.GetSrcChn().GetDevUri().c_str(), pcInviteTask->m_tReq.GetSrcChn().GetChnNO());
                            }
                        }
                    }

                    pcCuAgentItem++;
                }
            }
            OspPrintf(TRUE, FALSE, "======================= End ======================\n");
        }
        break;
    case CUI_QUIT:
        {
            ClearAllTask();
            g_cCuiDataMgr.ClearAllData();
            NextState(Idle);
            g_cCuiDataMgr.SetCuiStateHappenTime();
            g_MainSemLock.UnLock();
			g_cCuiDataMgr.m_cPuBatOptRequestList.Clear();
			g_cCuiDataMgr.m_cPlatBatOptRequestList.Clear();
        }
        break;
    case CUI_PRINT_TAS_LIST:
        {
            u32 bPrintUserDevList = *(u32*)pcMsg->content;
            u32 dwMaxDev = (bPrintUserDevList == 1) ? (~(u32)0) : 0;
            OspPrintf(TRUE, FALSE, "\n====================== Begin =====================\n");
            vector<CTask*> vctTasAgents = GetTasksArray("C3acAgent");
            if ( vctTasAgents.empty() )
            {
                OspPrintf(TRUE, FALSE, "û���������κ�TAS\n");
            }
            else
            {
                int nIndex = 1;
                vector<CTask*>::iterator pcTasAgentItem = vctTasAgents.begin();
                while ( pcTasAgentItem != vctTasAgents.end() )
                {
                    CMS_ASSERT( 0 == strcmp((*pcTasAgentItem)->GetObjName(), "C3acAgent" ) );
                    C3acAgent *pcTasAgentTask = (C3acAgent *)(*pcTasAgentItem);

                    OspPrintf(TRUE, FALSE, "[%02d] TAS����[%s]\n", 
                        nIndex++, pcTasAgentTask->m_t3asURI.GetURIString().c_str());
                    OspPrintf(TRUE, FALSE, "\t����������[%s]����ID[%s]\n",
                        pcTasAgentTask->m_str3ASDomainName.c_str(), pcTasAgentTask->m_t3ASDomainID.c_str());
                    OspPrintf(TRUE, FALSE, "\t��ǰ�Ĳ�ѯ������ %d ��\n",
                        pcTasAgentTask->m_vctTmpQueryTag.size());
                    if ( ! pcTasAgentTask->m_cUserMgr.m_mapUserName2PCuserAccount.IsEmpty()  )
                    {
                        OspPrintf(TRUE, FALSE, "\t��ǰ��¼��Cu�˻�:\n");
                        Iterator pPos;
                        while ( ! pPos.End() )
                        {
                            string *pstrUserName = NULL;
                            CUserAccount** ppcUserAccount = NULL;
                            if ( pcTasAgentTask->m_cUserMgr.m_mapUserName2PCuserAccount.Iterate(pPos, pstrUserName, ppcUserAccount) )
                            {
                                CMS_ASSERT(NULL != ppcUserAccount);
                                (*ppcUserAccount)->PrintData(2, dwMaxDev);
                            }
                        }
                    }
                    if ( ! pcTasAgentTask->m_cUserMgr.m_mapGroupID2UserGroup.IsEmpty() )
                    {
                        OspPrintf(TRUE, FALSE, "\t��ǰ��¼��Cu�û���:\n");
                        Iterator pPos;
                        while ( ! pPos.End() )
                        {
                            string* ptGroupId = NULL;
                            CUserGroup* pcUserGroup = NULL;
                            if ( pcTasAgentTask->m_cUserMgr.m_mapGroupID2UserGroup.Iterate(pPos, ptGroupId, pcUserGroup) )
                            {
                                CMS_ASSERT(NULL != pcUserGroup);
                                pcUserGroup->PrintData(2, dwMaxDev);
                            }
                        }
                    }

                    pcTasAgentItem++;
                }
            }
            OspPrintf(TRUE, FALSE, "======================= End ======================\n");
        }
        break;
    case CUI_PRINT_DOMAIN_LIST:
        {
            OspPrintf(TRUE, FALSE, "\n====================== Begin =====================\n");

			string strTAS = string(DEV_TYPE_3AS) + "@" + GetDomainName();
			C3acAgent *pc3acAgent = (C3acAgent *)g_cCuiDataMgr.Get3asAgentTaskPtrBy3asName(strTAS, this);
			if ( pc3acAgent != NULL && (C3acAgent::ONLINE_STATE == pc3acAgent->GetState()) )
			{
				pc3acAgent->m_mapDomainInfo.PrintData();
			}
//             Iterator pPos;
//             int nIndex = 1;
//             while ( ! pPos.End() )
//             {
//                 //                string strDomainId;
//                 //               string strDomainName;
//                 //                 if ( g_cCuiDataMgr.m_mapDomainIdToDomainNameMap.Iterate(pPos, strDomainId, strDomainName) )
//                 //                 {
//                 //                     OspPrintf(TRUE, FALSE, "%02d ����: %s��ID: %s\n",
//                 //                         nIndex++, strDomainName.c_str(), strDomainId.c_str());
//                 //                 }
//             }
            OspPrintf(TRUE, FALSE, "======================= End ======================\n");
        }
        break;
    case CUI_PRINT_CONFIG:
        {
            OspPrintf(TRUE, FALSE, "��������\n");
            OspPrintf(TRUE, FALSE, "\t��������: %s; UUID: %s\n",
                GetDomainName().c_str(), GetDomainUUID().c_str());
            OspPrintf(TRUE, FALSE, "\tProxy��ַ: %s:%hu\n", 
                GetProxyIPAddress().c_str(), GetProxyPort());
            OspPrintf(TRUE, FALSE, "\t�ϼ�URL: %s\n",
                GetParentPlatformProxyUrl().c_str());
            OspPrintf(TRUE, FALSE, "\tSTUN��������ַ: %s:%hu\n",
                this->GetStunAddr().GetNetIp().c_str(), this->GetStunAddr().GetNetPort());
            OspPrintf(TRUE, FALSE, "CUI��������\n");
            g_cCuiConfig.PrintData(1);
			OspPrintf(TRUE, FALSE, "UDP�˿�ӳ����������%d\n", g_tMappingTable.size());
			int nCount = 0;
			for (TMappingTable::const_iterator it=g_tMappingTable.begin(); it!=g_tMappingTable.end();it++)
			{
				nCount++;
				OspPrintf(TRUE, FALSE, "   [%d]��[%s][Local-%s:%d -> Mapping-%s:%d]\n", 
					nCount, it->second.GetProtocol().c_str(),
					it->second.GetLocalIP().c_str(), it->second.GetLocalPort(),
					it->second.GetMappingIP().c_str(), it->second.GetMappingPort());
			}
        }
        break;
    case CUI_PRINT_STATUS:
        {
            OspPrintf(TRUE, FALSE, "\nCuiServer״̬��Ϣ��\n");

            OspPrintf(TRUE, FALSE, "------------------------------------------------------------------------------\n");
            OspPrintf(TRUE, FALSE, "  CuiServer����ʱ��  :[%s]\n", g_cCuiDataMgr.m_cCuiState.m_tCuiBeginRun.GetXSDDateTime().c_str());
            OspPrintf(TRUE, FALSE, "  �Ƿ�ע��PROXY :[%d]\n", g_cCuiDataMgr.m_cCuiState.m_bConnectProxy);
            OspPrintf(TRUE, FALSE, "  ע��CMU״̬   :[%s]\n",GetStrState());

			g_tCuiStat.strRedisStat = g_cRedisDb.IsConnected()? "Service": "not login";

			string strDstUri;
			strDstUri = string(DEV_TYPE_3AS) + "@" + GetDomainName();
            g_tCuiStat.strTasState = Get3asLoginStateStr(strDstUri);
			strDstUri = string(DEV_TYPE_TVS) + "@" + GetDomainName();
			g_tCuiStat.strTvsState = GetPeLoginStateStr(strDstUri);
			strDstUri = string(DEV_TYPE_ALS) + "@" + GetDomainName();
			g_tCuiStat.strAlsState = GetPeLoginStateStr(strDstUri);
			strDstUri = string(DEV_TYPE_RCS) + "@" + GetDomainName();
			g_tCuiStat.strRcsState = GetPeLoginStateStr(strDstUri);
			strDstUri = string(DEV_TYPE_MPS) + "@" + GetDomainName();
			g_tCuiStat.strMpsState = GetPeLoginStateStr(strDstUri);
			strDstUri = string(DEV_TYPE_UAS) + "@" + GetDomainName();
			g_tCuiStat.strUasState = GetPeLoginStateStr(strDstUri);
			strDstUri = string(DEV_TYPE_GBS) + "@" + GetDomainName();
			g_tCuiStat.strGbsState = GetPeLoginStateStr(strDstUri);
			strDstUri = string(DEV_TYPE_DPS) + "@" + GetDomainName();
			g_tCuiStat.strDpsState = GetPeLoginStateStr(strDstUri);

			g_tCuiStat.PrintData();

            OspPrintf(TRUE, FALSE, "  �����ƶ��ͻ���������%d\n", GetMobileClientNumber());
        }
        break;
    case CUI_PRINT_TASK_MAP:
        {
            g_cCuiDataMgr.PrintData(this);
        }
        break;
    case CUI_PRINT_MAIL_SIZE:
        {
            g_cRecvThreadMailBox.Print();
        }
        break;
    case CUI_PRINT_PE_AGENT:
        {
            int nPeAgent = 1;
            vector<CTask*> vctPeAgentTask = GetTasksArray("CPeAgent");
            if( ! vctPeAgentTask.empty())
            {
                vector<CTask*>::iterator pPuAgentItem = vctPeAgentTask.begin();
                while( pPuAgentItem != vctPeAgentTask.end() )
                {
                    CPeAgent *pcPeAgent = (CPeAgent *)(*pPuAgentItem);
                    CMS_ASSERT( 0 == strcmp(pcPeAgent->GetObjName(), "CPeAgent") );
                    //pcPuStatus->StopSubscribePuStatus();
                    OspPrintf(TRUE, FALSE, "\t\t[%02d] TaskNO[%u] ����ģ��Uri[%s] CuiSession[%s]\n",
                        nPeAgent++,
                        pcPeAgent->GetTaskNO(),
                        pcPeAgent->m_tPeURI.GetURIString().c_str(),
                        pcPeAgent->m_strSessionID.c_str());

                    pPuAgentItem++;
                }
            }
        }
        break;
    case CUI_PRINT_USER_DEVINFO:
        {
            //const char* szUserSess = (const char*) (pcMsg->content);
			const TUserDevData *tUserDevData = (const TUserDevData*)(pcMsg->content);

			CCuAgent *pCuAgent = NULL;
			string strUserUri;
            do
            {
                if(strlen(tUserDevData->szUserId) == 0)
                {
                    INSTLOG(CUI_APP, WARNING_LEV, "szUserSess == NULL\n");
                    break;
                }

				strUserUri = tUserDevData->szDevUUid;
				strUserUri += "@";
				strUserUri += GetDomainName();

                pCuAgent = (CCuAgent *)g_cCuiDataMgr.GetTaskPtrBySession(strUserUri, this);
                if(NULL == pCuAgent)
                {
                    INSTLOG(CUI_APP, WARNING_LEV, "session[%s]�����ڵ�¼����[%s-%d]\n", 
                        strUserUri.c_str(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
                    break;
                }
                else
                {
                    DumpModuleListSubscribeByUser(pCuAgent);
                }
            }
            while (0);

			if (pCuAgent == NULL)
			{
				break;
			}

			OspPrintf(TRUE, FALSE, "\nCCuAgent TaskNo:%u\n", pCuAgent->GetTaskNO());
			pCuAgent->m_pcUserAccount->PrintData();

			if ( pCuAgent->m_pcUserAccount->IsSuperAdmin() )
			{
				OspPrintf(TRUE, FALSE, "\nAdmin�˻���ӵ�������豸Ȩ�޺���ƵԴȨ��!\n");
				break;
			}

			if (strlen(tUserDevData->szUserId) != 0
				&& strlen(tUserDevData->szDevUUid) != 0)
			{
				// �豸Ȩ��
				CEncoderPrivilege encPri;
				EFDB_ErrorCode er = g_cRedisDb.GetUserEncoderPrivilege(tUserDevData->szUserId, tUserDevData->szDevUUid, encPri);
				if ( !IsFdbResultPositive(er) )
				{
					break;
				}
				OspPrintf(TRUE, FALSE, "\n �豸[%s] Ȩ��:%s\n", tUserDevData->szDevUUid, GetPriString(encPri).c_str());

				// ��ƵԴȨ��
				TPuBaseCapSet tPuBaseCapSet;
				string errMsg;
				int vsNum = INVALID_VIDEOSOURCE_ID;
				er = g_cRedisDb.GetPuBaseCapSet(tUserDevData->szDevUUid, tPuBaseCapSet, TPuBaseCapSet_videoInPortNum);
				if (( !IsFdbResultPositive(er) )
					|| tPuBaseCapSet.GetVideoInPortNum() <= INVALID_VIDEOSOURCE_ID)
				{
					INSTLOG(CUI_APP, WARNING_LEV, "��ȡ�豸[%s] ��ƵԴ����[%d]ʧ�� ������Ϣ[%s]\n",	tUserDevData->szDevUUid, tPuBaseCapSet.GetVideoInPortNum(), errMsg.c_str());
				}

				vsNum = tPuBaseCapSet.GetVideoInPortNum();

				CVideosourcePrivilege vsPri;
				for (int nIndex = 0; nIndex < vsNum; nIndex++)
				{
					er = g_cRedisDb.GetUserVideoPrivilege(tUserDevData->szUserId, tUserDevData->szDevUUid, nIndex, vsPri);
					if ( !IsFdbResultPositive(er) )
					{
						continue;
					}
					OspPrintf(TRUE, FALSE, "\n��ƵԴ[%d] Ȩ��:%s\n", tUserDevData->szDevUUid, nIndex, GetPriString(vsPri).c_str());
				}
			}
        }
        break;
    case CUI_PRINT_PUDATA:
        {
			const char* szDevUrl = (const char*)(pcMsg->content);

			string devUUid = szDevUrl;
			TSipURI tSipUri;
			if (tSipUri.SetURIString(devUUid))
			{
				devUUid = tSipUri.GetUser();
			}

			if ( !devUUid.empty() )
			{
				TPuBaseCapSet tPuBaseCapSet;
				EFDB_ErrorCode er = g_cRedisDb.GetFullPuBaseCapSet(devUUid, tPuBaseCapSet);
				if ( IsFdbResultPositive(er) )
				{
					OspPrintf(TRUE, FALSE, "\nGetFullPuBaseCapSet[%s] \n", tPuBaseCapSet.ToXml().c_str());
				}
				else
				{
					OspPrintf(TRUE, FALSE, "\nGetFullPuBaseCapSet From Redis Return [%d] \n", er);
				}

				CDeviceInfo cDevInfo;
				er = g_cRedisDb.GetFullDeviceInfo(devUUid, cDevInfo);
				if ( IsFdbResultPositive(er) )
				{
					OspPrintf(TRUE, FALSE, "\nDevInfo[%s] \n", cDevInfo.ToXml().c_str());
				}
				else
				{
					OspPrintf(TRUE, FALSE, "\nDevInfo From Redis Return [%d] \n", er);
				}

				TPuStatus tPuStatus;
				er = g_cRedisDb.GetFullPuStatus(devUUid, tPuStatus);
				if ( IsFdbResultPositive(er) )
				{
					OspPrintf(TRUE, FALSE, "\nGetFullPuStatus[%s] \n", tPuStatus.ToXml().c_str());
				}
				else
				{
					OspPrintf(TRUE, FALSE, "\nGetFullPuStatus From Redis Return [%d] \n", er);
				}


				TPuConfig tPuConfig;
				er = g_cRedisDb.GetFullPuConfig(devUUid, tPuConfig);
				if ( IsFdbResultPositive(er) )
				{
					// ����ṹ��̫��ֱ�Ӵ�ӡ�ᳬ��OspPrintf����
					OspPrintf(TRUE, FALSE, "\nGetFullPuConfig\n");
					tPuConfig.PrintData();
				}
				else
				{
					OspPrintf(TRUE, FALSE, "\nGetFullPuConfig From Redis Return [%d] \n", er);
				}

				//CRedisPuData tRedisPuData;
				//er = g_cRedisDb.GetFullPuData(devUUid, tRedisPuData);
				//if ( IsFdbResultPositive(er) )
				//{
				//	OspPrintf(TRUE, FALSE, "\nGetFullPuConfig[%s] \n", tRedisPuData.).c_str());
				//}
				//else
				//{
				//	OspPrintf(TRUE, FALSE, "\nGetFullPuConfig From Redis Return [%d] \n", er);
				//}
			}

			string domainName;
			string strErrMsg;
			EFDB_ErrorCode er = g_cRedisDb.GetDomainNameByDevId(devUUid, domainName, strErrMsg);
			if ( IsFdbResultPositive(er) )
			{
				DumpModuleListSubscribeByDevUri(devUUid+"@"+domainName);
			}
			else
			{
				OspPrintf(TRUE, FALSE, "�޷���ȡ�豸[%s]������, errMsg[%s], er[%d]\n",
					devUUid.c_str(), strErrMsg.c_str(), er);
			}

        }
        break;
    case CUI_PRINT_CUREQ:
        {
            if (NULL == pcMsg->content)
            {
                m_strSession4PrintCuReq.clear();
            }
            else
            {
                m_strSession4PrintCuReq = (const char*) (pcMsg->content);
            }
        }
        break;
    case CUI_EMULATE_SIPMSG:
        {
            TCuiEmulateSipMsg *ptMsg = reinterpret_cast<TCuiEmulateSipMsg*>(pcMsg->content);

            CMessage cEmulateMsg;
            COspSipMsg cEmulateOspSipMsg;
            cEmulateOspSipMsg.SetSipErrorCode((EKDSIP_RESPONSE_CODE)ptMsg->nSipErrorno);
            cEmulateOspSipMsg.SetMsgBody(ptMsg->szMsgBody, sizeof(ptMsg->szMsgBody));
            cEmulateMsg.event = (u16)ptMsg->nEvent;
            cEmulateMsg.content = (u8*)&cEmulateOspSipMsg;
            cEmulateMsg.length = sizeof(cEmulateOspSipMsg);
            
            CTask *pcTask = FindTask(ptMsg->dwDstTaskno);
            if (NULL != pcTask)
            {
                if (PROCMSG_DEL == pcTask->ProcMsg(&cEmulateMsg))
                {
                    // ���ںܶ�TASK����delete this�����ﲻdelete task
                    OspPrintf(TRUE, FALSE, "emulate sip msg proc ret PROCMSG_DEL, but will not delete task here\n");
                }
            }
        }
        break;
	case CUI_NATDETECT_CALLBACK:
		{
			TNatPacketInfo* ptNatMsg = reinterpret_cast<TNatPacketInfo*>(pcMsg->content);
			string strCuid(ptNatMsg->m_szCuid);

			INSTLOG(CUI_APP, EVENT_LEV, "�յ�NAT̽���srcAddress[%s-%d],natAddress[%s-%d],Cuid[%s],Playid[%d]\n",
				IpToStr(ptNatMsg->m_dwSrcIp).c_str(),ptNatMsg->m_wSrcPort,IpToStr(ptNatMsg->m_dwNatIp).c_str(),ptNatMsg->m_wNatPort,
				ptNatMsg->m_szCuid,ptNatMsg->m_dwPalyID);

			CTask *pcTask = GetMediaTask(strCuid,ptNatMsg->m_dwPalyID);
			if (NULL != pcTask)
			{
				pcTask->ProcMsg(pcMsg);
			}
			else
			{
				INSTLOG(CUI_APP, CRITICAL_LEV, "�յ�NAT̽��� ����û���ҵ���Ӧ��task!\n");
			}
		}
		break;
	case CUI_CLEAR_VTDUNATPACKET:
		{
			const char* szUserSess = (const char*) (pcMsg->content);
			if(szUserSess == NULL)
			{
				RemoveAllNatData();
			}
			else
			{
				string strSession = szUserSess;
				RemoveNatData(strSession,0);
			}
		}
		break;
	case CUI_PRINT_VS_STATUS_LIST:
		{
			OspPrintf(TRUE, FALSE, "\n====================== Begin =====================\n");
			CMapCuVideoSrcStatList::iterator cuIter = g_mapDevVideoSrcStatList.begin();
			while ( cuIter!= g_mapDevVideoSrcStatList.end())
			{
				OspPrintf(TRUE, FALSE, "CuSession��%s\n", cuIter->first.c_str());
				
				CMapVideoSrcStatList::iterator domainIter = cuIter->second.begin();
				while ( domainIter != cuIter->second.end() )
				{
					OspPrintf(TRUE, FALSE, "  DomainName=%s\n", domainIter->first.c_str());
					domainIter->second.PrintData();
					domainIter++;
				}

				cuIter++;
			}
			OspPrintf(TRUE, FALSE, "======================= End ======================\n");
		}
		break;
	case CUI_PRINT_CU_BAT_OPT_LIST:
		{
			OspPrintf(TRUE, FALSE, "\n====================== Begin =====================\n");
			g_cCuiDataMgr.m_cPuBatOptRequestList.PrintData();
			g_cCuiDataMgr.m_cPlatBatOptRequestList.PrintData();
			OspPrintf(TRUE, FALSE, "======================= End ======================\n");
		}
		break;
	case CUI_SET_CU_NTF_MAX_SIZE:
		{
			OspPrintf(TRUE, FALSE, "\n====================== Begin =====================\n");
			s32 nNtfSize = *(s32 *)pcMsg->content;

			g_cCuiConfig.SetMaxCuNtfSize(nNtfSize);
			nNtfSize = g_cCuiConfig.GetMaxCuNtfSize();

			vector<CTask*> vctCuAgents = GetTasksArray("CCuAgent");
			if ( !vctCuAgents.empty() )
			{
				int nIndex = 1;
				vector<CTask*>::iterator pcCuAgentItem = vctCuAgents.begin();
				while ( pcCuAgentItem != vctCuAgents.end() )
				{
					CMS_ASSERT( 0 == strcmp((*pcCuAgentItem)->GetObjName(), "CCuAgent" ) );
					if ( 0 != strcmp((*pcCuAgentItem)->GetObjName(), "CCuAgent") )
					{
						continue;
					}

					CCuAgent *pcCuAgentTask = (CCuAgent *)(*pcCuAgentItem);
					pcCuAgentTask->m_cNotifyPool.SetMaxCuNtfSize(nNtfSize);

					pcCuAgentItem++;
				}
			}
			OspPrintf(TRUE, FALSE, "����CU֪ͨ��Ϣ�����������Ϊ%d\n", nNtfSize);
			OspPrintf(TRUE, FALSE, "======================= End ======================\n");
		}
		break;
	case CUI_REDIS_NTF_CALLBACK:
		{
			TRedisNtfMsgInfo *ptMsg = *(TRedisNtfMsgInfo**)pcMsg->content;
			if ( ptMsg == NULL )
			{
				INSTLOG(CUI_APP, WARNING_LEV, "ptMsg == NULL\n");
				break;
			}

			string strChnName(ptMsg->m_szchnName);
			string strMsgData(ptMsg->m_szMsgData);
			if ( !strChnName.empty() && !strMsgData.empty() )
			{
				INSTLOG(CUI_APP, EVENT_LEV, "CUI_REDIS_NTF_CALLBACK strChnName[%s]\n", strChnName.c_str());
				OnRedisNtfCallBack(strChnName, strMsgData);
			}
			else
			{
				INSTLOG(CUI_APP, WARNING_LEV, "CUI_REDIS_NTF_CALLBACK strChnName[%s] strMsgData[%s]\n", strChnName.c_str(), strMsgData.c_str());
			}

			delete ptMsg;
			ptMsg = NULL;

		}
		break;
	case CUI_PRINT_PTZ_LOCK_DEV_VS_LIST:
		{
			string strDevId;
			const char* szDevId = (const char*)(pcMsg->content);
			if ( szDevId != NULL )
			{
				strDevId = szDevId;
			}

			string strTAS = string(DEV_TYPE_3AS) + "@" + GetDomainName();
			C3acAgent *pc3acAgent = (C3acAgent *)g_cCuiDataMgr.Get3asAgentTaskPtrBy3asName(strTAS, this);
			if (pc3acAgent != NULL && (C3acAgent::ONLINE_STATE == pc3acAgent->GetState()))
			{
				if ( strDevId.empty() )
				{
					pc3acAgent->m_cPtzlockInfoList.PrintData();
				}
				else
				{
					pc3acAgent->m_cPtzlockInfoList.PrintDataByDevId(strDevId);
				}
			}
			else
			{
				INSTLOG(CUI_APP, ERROR_LEV, "û������3A��û�в�ѯPTZ��������ƵԴ�б�\n");
			}
		}
		break;
    case CUI_REDISSDK_DISC_CB:
        {
            TRedisSdkDiscCB *ptRedisSDKCB = *(TRedisSdkDiscCB **)(pcMsg->content);
            if (NULL != ptRedisSDKCB)
            {
                ProcRedisSdkDiscCB(ptRedisSDKCB);
                delete ptRedisSDKCB;
                ptRedisSDKCB = NULL;
            }
        }
        break;
    case CUI_REDISSDK_SUB_CB:
        {
            TRedisSdkSubCB *ptRedisSDKCB = *(TRedisSdkSubCB **)(pcMsg->content);
            if (NULL != ptRedisSDKCB)
            {
                ProcRedisSdkCB_SubsNtf(ptRedisSDKCB);
                delete ptRedisSDKCB;
                ptRedisSDKCB = NULL;
            }
        }
        break;
    default:
        {
            bIsInnerMsg = FALSE;
        }
        break;
    }
    return bIsInnerMsg;
}

void CCuiInst::ProcCmuDisconnect()
{
    if (m_pcCmuListSubscribeTask != NULL)
    {
        delete m_pcCmuListSubscribeTask;
        m_pcCmuListSubscribeTask = NULL;
    }

    ClearAllTask();
    g_cCuiDataMgr.ClearAllData();
    g_tMappingTable.clear();//��cmu���������ӳ���

    g_cCuiDataMgr.m_cPlatBatOptRequestList.Clear();
    g_cCuiDataMgr.m_cPuBatOptRequestList.Clear();

    NextState(ReLogin);
    g_cCuiDataMgr.SetCuiStateHappenTime();
    SetTimer(CUI_REG_CMU_TIMER, CUI_REG_CMU_INTERVAL);
}

void CCuiInst::OnSipUADisconnect( CMessage* const pcMsg )
{
    COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;

    //����DSTURI�ҵ���������
    TSipURI tHbDst;
    tHbDst.SetURIString(pcOspSipMsg->GetMsgBody());
    INSTLOG(CUI_APP, EVENT_LEV, "�յ�[OSP_SIP_DISCONNECT]��ģ��[%s]����ʧ�ܣ�SIP�����\n", 
        tHbDst.GetURIString().c_str());

    if ( tHbDst.GetUser() == UNIQUE_SIP_UA_CMU )
    {
        ProcCmuDisconnect();
    }
    else if (tHbDst.GetUser() == UNIQUE_SIP_UA_3AS)
    {
        INSTLOG(CUI_APP, EVENT_LEV, "CUI��3AS[%s]����, ����3AS���������, ���ѵ�¼CU����Ӱ��\n", 
            tHbDst.GetURIString().c_str());
        C3acAgent *pc3acAgentTask = (C3acAgent *)g_cCuiDataMgr.Get3asAgentTaskPtrBy3asName(tHbDst.GetURIString(), this);
        if (NULL != pc3acAgentTask)
        {
            CMS_ASSERT( 0 == strcmp(pc3acAgentTask->GetObjName(), "C3acAgent") );

            /*
            �ж��Ƿ�������CU������У�˵��TAS�쳣���ߣ���Ҫ����
            ͬʱ3acAgent�б����CUserAccount��Ȼ��������Ӱ��CuAgent�е�m_pcUserAccount
            */
            if ( pc3acAgentTask->CheckUserOnline() != 0 )
            {
                CReLoginTasTask *pcReloginTask = new CReLoginTasTask(this);
                pcReloginTask->InitStateMachine();
                pcReloginTask->m_t3asURI = pc3acAgentTask->m_t3asURI;
                pcReloginTask->m_tCuAgentIds = pc3acAgentTask->GetCuAgentList();    //���صļ�����pc3acAgentTask->CheckUserOnline()���Ӽ�
                pcReloginTask->m_dw3acAgentTask = pc3acAgentTask->GetTaskNO();

                if ( pcReloginTask->m_tCuAgentIds.empty() )
                {
                    ts();
                    taslist();
                    OspDelay(1000);
                }
                CMS_ASSERT( !pcReloginTask->m_tCuAgentIds.empty()); //����϶�������CU
                /*
                ���������û�����ʱ3acAgent�е�UserAccount�������٣�ֻ������һЩ״̬
                pcCuAgentTask->m_pcUserAccount�����
                */
                pc3acAgentTask->ClearDataWhenTASOfflineOrDisconnect();
                pc3acAgentTask->EnterWaitReloginState();

                //pcReloginTask->LoginTasReq(); // ���˳�TAS�ٵȶ�ʱ���������ٵ�¼TAS�������ʱ��δ����3acAgent
            }
            else
            {
                //ֻ����3acAgent��û������Cu�˲�ɾ��������
                delete pc3acAgentTask; //��TerminateTask��ע���Ựӳ��

				g_cCuiDataMgr.m_cPlatBatOptRequestList.Clear();
				g_cCuiDataMgr.m_cPuBatOptRequestList.Clear();
            }
        }
        else
        {
            INSTLOG(CUI_APP, EVENT_LEV, "�Ҳ���TAS[%s]��Ӧ��C3acAgentTask\n", 
                tHbDst.GetURIString().c_str());
        }
    }
    //���CUI����ģ�齨���ˣ���Ҫ���������
    else
    {
        INSTLOG(CUI_APP, ERROR_LEV, "��δ֪UA[%s]����\n", 
            tHbDst.GetURIString().c_str());

        CSipTask *pcTask = (CSipTask *)FindTask(pcOspSipMsg->GetOspTaskNO());
        if ( NULL != pcTask )
        {
            pcTask->ProcMsg(pcMsg);
        }
        else
        {
            INSTLOG(CUI_APP, ERROR_LEV, "Inst[ReLogin] �յ� PU_STATUS_REFRESH_SS_RSP ��Ϣ�������Ҳ���TaskNO[%u]��Ӧ�����񣬿���������Ϣ������Task���� ���� Task�Ѿ�������\n",
                pcOspSipMsg->GetOspTaskNO());
        }
        //CMS_ASSERT( 0 && "��δ֪UA����" );
    }
}

void CCuiInst::OnRedisDisconnect()
{
	// ��redis����
	OspSipStopHB(g_cCuiConfig.GetCmuURI());

	//��redis������ֱ���˳�����
	cuiquit();
}

void CCuiInst::OnRedisNtfCallBack(const string &strChnName, const string &strMsgData)
{
	if (strChnName.empty()||strMsgData.empty())
	{
		return;
	}

	string strChnType;
	string devId;

	// cmu������ͨ������
	if (!RedisGetSubTypeByChn(strChnName, devId, strChnType))
	{
		INSTLOG(CUI_APP, ERROR_LEV, "RedisGetSubTypeByChn(%s) return false!\n", strChnName.c_str());
		return;
	}

	string field = CDeviceInfo_domainId;
	string domainId;
	g_cRedisDb.GetDevInfo(devId, field, domainId);
	if (domainId.empty())
	{
		INSTLOG(CUI_APP, ERROR_LEV, "domainId is empty\n");
		return;
	}

	string domainName;
	if (!GetDomainNameByDomainId(domainId, domainName))
	{
		INSTLOG(CUI_APP, ERROR_LEV, "domainName is empty\n");
		// �Ҳ�������
		return;
	}

	string devUri = devId + "@" + domainName;

	if (m_pcCmuListSubscribeTask == NULL)
	{
		INSTLOG(CUI_APP, ERROR_LEV, "m_pcCmuListSubscribeTask is empty\n");
		return;
	}

	CMessage cNtfReqMsg;
	COspSipMsg cNtfOspSipMsg;
	cNtfOspSipMsg.SetSipErrorCode(KDSIP_RESPONSE_NONE);
	string strNtfReq;

	if (strChnType == PU_ON_OFF_STATUS)
	{
		TPuStatus tPuStatus;
		tPuStatus.FromXml(strMsgData);

		CPuStatusNtyReq cNtfReq;
		cNtfReq.SetDevURI(devUri);
		cNtfReq.SetOnOffStatus(tPuStatus);

		cNtfReqMsg.event = cNtfReq.GetEventId();

		strNtfReq = cNtfReq.ToXml();
		cNtfOspSipMsg.SetMsgBody(strNtfReq.c_str(), strNtfReq.size());
		cNtfReqMsg.content = (u8*)&cNtfOspSipMsg;
		cNtfReqMsg.length = sizeof(cNtfOspSipMsg);

		m_pcCmuListSubscribeTask->ProcMsg(&cNtfReqMsg);
	}
	else if (strChnType == PU_CONFIG_STATUS)
	{
		TPuDevConfig tPuDevConfig;
		tPuDevConfig.FromXml(strMsgData);
		
		CPuConfigNtyReq cNtfReq;
		cNtfReq.SetDevURI(devUri);

		cNtfReqMsg.event = cNtfReq.GetEventId();

		strNtfReq = cNtfReq.ToXml();
		cNtfOspSipMsg.SetMsgBody(strNtfReq.c_str(), strNtfReq.size());
		cNtfReqMsg.content = (u8*)&cNtfOspSipMsg;
		cNtfReqMsg.length = sizeof(cNtfOspSipMsg);

		//��ֹ��Ϣ�������Ϣ���ضϣ���Ϣ�����ݲ�����Ϣ����
		cNtfReq.SetConfig(tPuDevConfig);
		m_pcCmuListSubscribeTask->SetNtfString(cNtfReq.ToXml());

		m_pcCmuListSubscribeTask->ProcMsg(&cNtfReqMsg);
	}
	else if (strChnType == PU_SERVICE_STATUS)
	{
		TPuServiceStatus tPuServiceStatus;
		tPuServiceStatus.FromXml(strMsgData);

		CPuServiceStatusNtyReq cNtfReq;
		cNtfReq.SetDevURI(devUri);
		cNtfReq.SetServiceStatus(tPuServiceStatus);

		cNtfReqMsg.event = cNtfReq.GetEventId();

		strNtfReq = cNtfReq.ToXml();
		cNtfOspSipMsg.SetMsgBody(strNtfReq.c_str(), strNtfReq.size());
		cNtfReqMsg.content = (u8*)&cNtfOspSipMsg;
		cNtfReqMsg.length = sizeof(cNtfOspSipMsg);

		m_pcCmuListSubscribeTask->ProcMsg(&cNtfReqMsg);
	}
	else if (strChnType == PU_ALARM_STATUS)
	{
		TPuAlarmStatus tPuAlarmStatus;
		tPuAlarmStatus.FromXml(strMsgData);

		CPuAlarmStatusNtyReq cNtfReq;
		cNtfReq.SetDevURI(devUri);
		cNtfReq.SetAlarmStatus(tPuAlarmStatus);
		
		cNtfReqMsg.event = cNtfReq.GetEventId();

		strNtfReq = cNtfReq.ToXml();

		cNtfOspSipMsg.SetMsgBody(strNtfReq.c_str(), strNtfReq.size());
		cNtfReqMsg.content = (u8*)&cNtfOspSipMsg;
		cNtfReqMsg.length = sizeof(cNtfOspSipMsg);

		m_pcCmuListSubscribeTask->ProcMsg(&cNtfReqMsg);
	}
	else if (strChnType == PU_TRANSDATA_STATUS)
	{
		TTransparentData tTransparentData;
		tTransparentData.FromXml(strMsgData);

		CPuTransDataStatusNtyReq cNtfReq;
		cNtfReq.SetDevURI(devUri);
		cNtfReq.SetData(tTransparentData);

		cNtfReqMsg.event = cNtfReq.GetEventId();

		strNtfReq = cNtfReq.ToXml();
		cNtfOspSipMsg.SetMsgBody(strNtfReq.c_str(), strNtfReq.size());
		cNtfReqMsg.content = (u8*)&cNtfOspSipMsg;
		cNtfReqMsg.length = sizeof(cNtfOspSipMsg);

		m_pcCmuListSubscribeTask->ProcMsg(&cNtfReqMsg);
	}

	return;
}

void CCuiInst::SendNtfRspToServer(CMessage* const pcMsg)
{
    COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
    string strSessionID = GetSipSessionID(pcOspSipMsg->GetMsgBody());

    //�ԻỰ����ͨ��Ϣ��ͳһ�ظ�Ntf
    INSTLOG(CUI_APP, EVENT_LEV, "ͳһ�ظ�NtfRsp[%s--%hu]��Ϣ\n",
        OspExtEventDesc(pcMsg->event + 1).c_str(), pcMsg->event + 1);
    CNtfRsp cRsp;
    cRsp.SetEvent(pcMsg->event + 1);
    cRsp.SetSession(strSessionID);
    string strSeqNum = GetSipSeqNO(pcOspSipMsg->GetMsgBody());
    int nSeqNum = static_cast<int>(atol(strSeqNum.c_str()));
    cRsp.SetSeqNum(nSeqNum);
    TOspTransID tSrcTransId;
    tSrcTransId.dwIID = MAKEIID(GetAppID(), GetInsID());
    tSrcTransId.dwSeqNO = (u32)nSeqNum;
    tSrcTransId.dwTaskID = INVALID_TASKNO;
    OspSipPostRsp(KDSIP_EVENT_MESSAGE_RSP, pcOspSipMsg->GetSipTransID(), cRsp, tSrcTransId);
}

/*====================================================================
������      : ClearAllTask
����        : �����������
�㷨ʵ��    : ����CU������֮���������������CuAgent���кܶ�CSubscribeTask������
ֱ�ӵ���EmptyTask()���ܵ��³������
Ӧ�ñ���ɾ��������
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/4/11/             liangli
====================================================================*/
void CCuiInst::ClearAllTask()
{
    vector<CTask*> vctSubTask = GetTasksArray("CSubTask");
    if( ! vctSubTask.empty())
    {
        vector<CTask*>::iterator pSubItem = vctSubTask.begin();
        while( pSubItem != vctSubTask.end() )
        {
            CSubBaseTask *pSubBaseTask = (CSubBaseTask *)(*pSubItem);
            delete pSubBaseTask;
            pSubBaseTask = NULL;
            pSubItem++;
        }
    }

    vector<CTask*> vctCallPuTask = GetTasksArray("CCallPuTask");
    if( ! vctCallPuTask.empty())
    {
        vector<CTask*>::iterator pCallPuItem = vctCallPuTask.begin();
        while( pCallPuItem != vctCallPuTask.end() )
        {
            CCallPuTask *pcCallPu = (CCallPuTask *)(*pCallPuItem);
            CMS_ASSERT( 0 == strcmp(pcCallPu->GetObjName(), "CCallPuTask") );
            //pcCallPu->StopSubscribePuStatus();
            delete pcCallPu;
            pcCallPu = NULL;
            pCallPuItem++;
        }
    }

    vector<CTask*> vctPeAgentTask = GetTasksArray("CPeAgent");
    if( ! vctPeAgentTask.empty())
    {
        vector<CTask*>::iterator pPeAgentItem = vctPeAgentTask.begin();
        while( pPeAgentItem != vctPeAgentTask.end() )
        {
            CPeAgent *pcPeAgent = (CPeAgent *)(*pPeAgentItem);
            CMS_ASSERT( 0 == strcmp(pcPeAgent->GetObjName(), "CPeAgent") );
            //pcCallPu->StopSubscribePuStatus();
            pcPeAgent->StartLogOut();
            delete pcPeAgent;
            pcPeAgent = NULL;
            pPeAgentItem++;
        }
    }

    vector<CTask*> vctCuAgents = GetTasksArray("CCuAgent");
    if ( ! vctCuAgents.empty() )
    {
        vector<CTask*>::iterator pCuAgentItem = vctCuAgents.begin();
        while ( pCuAgentItem != vctCuAgents.end() )
        {
            CCuAgent *pcCuAgent = (CCuAgent *)(*pCuAgentItem);
            CMS_ASSERT( 0 == strcmp(pcCuAgent->GetObjName(), "CCuAgent") );
            delete pcCuAgent;
            pCuAgentItem++;
        }
    }

    vector<CTask*> vct3acAgents = GetTasksArray("C3acAgent");
    if ( ! vct3acAgents.empty() )
    {
        vector<CTask*>::iterator p3acAgentItem = vct3acAgents.begin();
        while ( p3acAgentItem != vct3acAgents.end() )
        {
            C3acAgent *pc3acAgent = (C3acAgent *)(*p3acAgentItem);
            CMS_ASSERT( 0 == strcmp(pc3acAgent->GetObjName(), "C3acAgent") );
            delete pc3acAgent;
            p3acAgentItem++;
        }
    }

    //ɾ����3acAgents����ɾ��CReLoginTasTask
    vector<CTask*> vctReLoginTas = GetTasksArray("CReLoginTasTask");
    if ( ! vctReLoginTas.empty() )
    {
        vector<CTask*>::iterator pReLoginTasItem = vctReLoginTas.begin();
        while ( pReLoginTasItem != vctReLoginTas.end() )
        {
            CReLoginTasTask *pcReLoginTasItem = (CReLoginTasTask *)(*pReLoginTasItem);
            CMS_ASSERT( 0 == strcmp(pcReLoginTasItem->GetObjName(), "CReLoginTasTask") );
            delete pcReLoginTasItem;
            pReLoginTasItem++;
        }
    }

    vector<CTask*> vctOtherTasks = GetTasksArray();
    if ( ! vctOtherTasks.empty() )
    {
        INSTLOG(CUI_APP, ERROR_LEV, "��ɾ��������CuAgent��3acAgent֮���������������!\n������:\n");
        vector<CTask*>::iterator pTaskItem = vctOtherTasks.begin();
        while ( pTaskItem != vctOtherTasks.end() )
        {
            INSTLOG(CUI_APP, ERROR_LEV, "TaskNO: %u, TaskName: %s, State: %s\n",
                (*pTaskItem)->GetTaskNO(), (*pTaskItem)->GetObjName(), (*pTaskItem)->GetStateName((*pTaskItem)->GetState()));
            pTaskItem++;
        }
        //CMS_ASSERT(0&&"��ɾ��������CuAgent��3acAgent֮���������������");
    }
}

//�Ƿ��ṩ����
string CCuiInst::IsProvideServer(const TSipURI &tSvrURI, u32 dwCallbackTaskNO)
{
    string strRcsSession;

    CPeAgent *pcPeAgent = (CPeAgent *)g_cCuiDataMgr.GetPeAgentPtrByPeName(tSvrURI.GetURIString(), this);
    if ( !pcPeAgent )
    {
        pcPeAgent = new CPeAgent(this);
        pcPeAgent->InitStateMachine();
        pcPeAgent->m_tPeURI = tSvrURI;

        CDevLoginReq cDevLoginReq;
        cDevLoginReq.SetDevUri(g_cCuiConfig.GetLocalURI().GetURIString());
        cDevLoginReq.SetDevType(DEV_TYPE_CUI);
        TNetAddrList tNetAddrList;
        TNetAddr tNetAddr;
        tNetAddr.SetNetIp(GetLocalIPAddress());
        tNetAddr.SetNetPort(g_cCuiConfig.GetLocalPort());
        cDevLoginReq.GetDevAddrList().push_back(tNetAddr);
        if (tSvrURI.GetUser() == UNIQUE_SIP_UA_RCS)
        {
            cDevLoginReq.SetIsSupportNA(true);
        }
        pcPeAgent->m_strReqMsg = cDevLoginReq.ToXml();

        g_cCuiDataMgr.m_mapPeNameToPeTaskMap.Insert(tSvrURI.GetURIString(),  pcPeAgent->GetTaskNO()); 
        pcPeAgent->StartLogin();
    }

    if ( pcPeAgent->GetState() != CPeAgent::Service )
    {
        pcPeAgent->m_tCallBackTaskList.push_back(dwCallbackTaskNO);
    }
    else
    {
        strRcsSession = pcPeAgent->m_strSessionID;
    }
    return strRcsSession;
}

void CCuiInst::RegServer(const string& strUri, bool bSupportNa)
{
    TSipURI tSipUri;
    tSipUri.SetURIString(strUri);
    CPeAgent *cPeAgent = new CPeAgent(this);
    cPeAgent->InitStateMachine();
    cPeAgent->m_tPeURI = tSipUri;

    CDevLoginReq cDevLoginReq;
    cDevLoginReq.SetDevUri(g_cCuiConfig.GetLocalURI().GetURIString());
    cDevLoginReq.SetDevType(DEV_TYPE_CUI);
    cDevLoginReq.SetIsSupportNA(bSupportNa);

    TNetAddr tNetAddr;
    tNetAddr.SetNetIp(GetLocalIPAddress());
    tNetAddr.SetNetPort(g_cCuiConfig.GetLocalPort());
    cDevLoginReq.GetDevAddrList().push_back(tNetAddr);
    cPeAgent->m_strReqMsg = cDevLoginReq.ToXml();

    g_cCuiDataMgr.m_mapPeNameToPeTaskMap.Insert(tSipUri.GetURIString(),  cPeAgent->GetTaskNO());            

    cPeAgent->StartLogin();
}

void CCuiInst::RegAllLocalServer()
{
    /////////////////////////////////////////////////////////////////////
    //ע�᱾��Tas��ȡ��ID��������Ӧ��ϵ��


    //ע�᱾��RCS
    string strRcsUri = string(DEV_TYPE_RCS) + "@" + GetDomainName();
    RegServer(strRcsUri, true);

    //ע�᱾��tvs
    string strTvsUri = string(DEV_TYPE_TVS) + "@" + GetDomainName();
    RegServer(strTvsUri);

    RegServer(string(DEV_TYPE_UAS)+"@"+GetDomainName());
    RegServer(string(DEV_TYPE_MPS)+"@"+GetDomainName());
    RegServer(string(DEV_TYPE_ALS)+"@"+GetDomainName());
	RegServer(string(DEV_TYPE_GBS)+"@"+GetDomainName());
	RegServer(string(DEV_TYPE_CAPS) + "@" + GetDomainName());
	RegServer(string(DEV_TYPE_DPS) + "@" + GetDomainName());
}

void CCuiInst::CreateCmuListSubscribeTask( const string &strCmuSession )
{
    CMS_ASSERT(m_pcCmuListSubscribeTask == NULL);
    m_pcCmuListSubscribeTask = new CCmuListSubscribeTask(this, 
        g_cCuiConfig.GetCmuURI().GetURIString(), strCmuSession);
    m_pcCmuListSubscribeTask->InitStateMachine();
	m_pcCmuListSubscribeTask->SetStateRefreshSub();
}


string CCuiInst::Get3asLoginStateStr(const string& strSession)
{
	string strState;
	C3acAgent *pc3acAgent = (C3acAgent *)g_cCuiDataMgr.Get3asAgentTaskPtrBy3asName(strSession, this);
	if (pc3acAgent != NULL)
	{
		strState = pc3acAgent->GetStateName(pc3acAgent->GetState());
	}
	else
	{
		strState = "not login";
	}
	return strState;
}

string CCuiInst::GetPeLoginStateStr(const string& strSession)
{
	string strState;
	CPeAgent *pcPeAgent = (CPeAgent *)g_cCuiDataMgr.GetPeAgentPtrByPeName(strSession,this);
	if (pcPeAgent != NULL)
	{
		strState = pcPeAgent->GetStateName(pcPeAgent->GetState());
	}
	else
	{
		strState = "not login";
	}
	return strState;
}

bool CCuiInst::GetDomainNameByDomainId(const string &strDomainId, string &strDomainName)
{
	string str3asName = string(DEV_TYPE_3AS) + "@" + GetDomainName();

	C3acAgent *pc3acAgent = (C3acAgent *)g_cCuiDataMgr.Get3asAgentTaskPtrBy3asName(str3asName, this);
	if (pc3acAgent == NULL)
	{
		return false;
	}

	if (pc3acAgent->GetState() != C3acAgent::ONLINE_STATE)
	{
		INSTLOG(CUIAPP_AID, ERROR_LEV, "3acAgent not login\n");
		return false;
	}

	strDomainName = pc3acAgent->GetDomainNameByDomainID(strDomainId);

	return true;
}

bool CCuiInst::GetDefaultIface(string& strIface) const
{
	bool bRet = false;

	s32 nRouteTableIndex = 0;
	u32 dwAddrTableIndex = 0;
	FILE * pfRoute = fopen("/proc/net/route", "r");
	s8 achLine[128];
	s8 achTempIface[128];
	u32 dwTempDst, dwTempMask, dwTemp;
	if (NULL == pfRoute)
	{
		return false; 
	}

	// ������һ��
	fgets(achLine, sizeof(achLine), pfRoute);

	while (NULL != fgets(achLine, sizeof(achLine), pfRoute))
	{
		dwTempMask = 0;
		dwTempDst = 0;
		if (8 == sscanf(achLine, "%s %X %X %X %X %X %X %X", achTempIface, (unsigned int*)&dwTempDst,
			(unsigned int*)&dwTemp, (unsigned int*)&dwTemp, (unsigned int*)&dwTemp, (unsigned int*)&dwTemp,
			(unsigned int*)&dwTemp, (unsigned int*)&dwTempMask) && 0 == dwTempMask)
		{
			strIface = achTempIface;
			bRet = true;
		}
	}
	fclose(pfRoute);

	return bRet;
}

TNetSegmentAllInfo CCuiInst::GetNetSegmentInfo()
{
	TNetSegmentAllInfo tNetSegmentInfo;

	//ѭ����ȡ����IP
	bool bGetLocalIps = false;
	u16  wReadCount = 0;
	vector<TNicInfo> vetNic;
	do 
	{
		GetOnlineNicList(vetNic);

		if (vetNic.empty())
		{
			OspDelay(1000);
		}
		else
		{
			bGetLocalIps = true;
		}
	} while (!bGetLocalIps && ++wReadCount<3);
	INSTLOG(CUI_APP, EVENT_LEV, "[GetOnlineNicList] bGetLocalIps(%d), wReadCount(%d)\n",
		bGetLocalIps, wReadCount);

	if (bGetLocalIps)
	{
		string strDefaultNic="";
		GetDefaultIface(strDefaultNic);
		INSTLOG(CUI_APP, EVENT_LEV,  "[GetNetSegmentInfo] DefaultNic(%s)\n",
			strDefaultNic.c_str());

		TNetSegmentList tNetSegmentList;
		for (vector<TNicInfo>::const_iterator it = vetNic.begin(); it!=vetNic.end(); ++it)
		{
			TNetSegment tNetSegment;
			tNetSegment.SetNetIp(it->strNicIp);
			tNetSegment.SetNicName(it->strNicName);
			tNetSegment.SetNetMask(it->strNicMask);
			tNetSegmentList.push_back(tNetSegment);

			if (strDefaultNic == it->strNicName)
			{
				tNetSegmentInfo.SetDefaultRoute(it->strNicIp);
			}
			INSTLOG(CUI_APP, EVENT_LEV, "[GetNetSegmentInfo] Nic(%s), Ip(%s), Mask(%s)\n",
				it->strNicName.c_str(), it->strNicIp.c_str(), it->strNicMask.c_str());
		}

		tNetSegmentInfo.SetNetSegmentList(tNetSegmentList);
	}
	return tNetSegmentInfo;
}

void CCuiInst::UpdateRegNetSeg()
{
	KillTimer(CUI_UPDATE_NETSEG_TIMER);
	SetTimer(CUI_UPDATE_NETSEG_TIMER,g_cCuiConfig.GetUpdateNetInterval());

	const TNetSegmentAllInfo& tNetSegmentAllInfo = GetNetSegmentInfo();
	if (tNetSegmentAllInfo == m_tCuiNetSegInfo)
	{
		INSTLOG(CUI_APP, EVENT_LEV, "[UpdateRegNetSeg] ������Ϣû�б仯.\n");
		return;
	}
	m_tCuiNetSegInfo.SetDefaultRoute(tNetSegmentAllInfo.GetDafaultRoute());
	m_tCuiNetSegInfo.SetNetSegmentList(tNetSegmentAllInfo.GetNetSegmentList());
	INSTLOG(CUI_APP, EVENT_LEV, "UpdateRegNetSeg[%s]\n", m_tCuiNetSegInfo.ToXml().c_str());

	CUpdateRegNetSegReq cReq;
	cReq.SetNetSegmentAllInfo(tNetSegmentAllInfo);
	cReq.SetSession(g_cCuiConfig.GetLocalURI().GetURIString());

	TOspTransation tOspTrans;
	tOspTrans.dwIID = MAKEIID(GetAppID(), GetInsID());
	tOspTrans.dwTaskID = INVALID_TASKNO;
	tOspTrans.dwSeqNO = (u32)cReq.GetSeqNum();
	OspSipPostReq(KDSIP_EVENT_MESSAGE_REQ, cReq, g_cCuiConfig.GetCmuURI(), tOspTrans);
}

//¼����ǽʱ��������ǰ��ǽ�ĵ���ͨ���Ƿ��Ѿ���������½�û�ռ��
bool CCuiInst::IsTvChnTaskExit(const TTvChannel& tTvChn)
{
	vector<CTask*> vctCuAgents = GetTasksArray("CCuAgent");
	if ( !vctCuAgents.empty() )
	{
		int nIndex = 1;
		vector<CTask*>::iterator pcCuAgentItem = vctCuAgents.begin();
		for( ;pcCuAgentItem != vctCuAgents.end();pcCuAgentItem++)
		{
			CMS_ASSERT( 0 == strcmp((*pcCuAgentItem)->GetObjName(), "CCuAgent" ) );
			CCuAgent *pcCuAgentTask = (CCuAgent *)(*pcCuAgentItem);
			if (NULL !=  pcCuAgentTask->GetRecTvTask(tTvChn))
			{
				return true;
			}
		}
	}
	return false;
}

void CCuiInst::ProcMappingTableNtfReq(CMessage* const pcMsg)
{
	COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
	if (pcOspSipMsg == NULL)
	{
		INSTLOG(CUI_APP, ERROR_LEV, "ospsip ��ϢΪ��\n");
		return;
	}

	CMappingTableNtfReq cReq;
	pcOspSipMsg->GetMsgBody(cReq);
	for (TMappingTable::const_iterator it=cReq.GetMappingTable().begin(); it!=cReq.GetMappingTable().end(); it++)
	{
		if (MAPPING_OP_TYPE_DEL == it->second.GetOperateType())
		{
			g_tMappingTable.erase(it->first);
		}
		else
		{
			g_tMappingTable[it->first] = it->second;
		}
	}

	CMappingTableNtfRsp cRsp;
	cRsp.SetHeadFromReq(cReq);
	TOspTransID tOspTrasnID;
	tOspTrasnID.dwIID = GetOspIID();
	OspSipPostRsp(pcOspSipMsg->GetSipRsp(), pcOspSipMsg->GetSipTransID(), cRsp, tOspTrasnID);

	g_cCuiConfig.UpdateMapedNatAddr();
}

//���񴴽�
CTask* CCuiInst::CreateTask(u16 wEvent,CCuAgent *pcCuAgent, TRecvNode * ptRecvNode)
{
	CTask* pTask = NULL;

	switch(wEvent)
	{
	case RECORD_ON_TV_REQ:
		{
			pTask = new CRecordOnTvTask(this,pcCuAgent,ptRecvNode);
		}
		break;
	default:
		{
			INSTLOG(CUI_APP, ERROR_LEV, "Msg[%s-%d]��������ʧ��\n", 
				OspExtEventDesc(wEvent).c_str(), wEvent);
		}
		break;
	}

	if (pTask != NULL)
	{
		pTask->InitStateMachine();
	}

	return pTask;
}

//������Ϣ����ͳһ��������ķ��ؽ��
void CCuiInst::ProcTaskMsg(CTask* pTask, CMessage* const pcMsg)
{
	if (pTask == NULL)
	{
		INSTLOG(CUI_APP, ERROR_LEV, "pTask == NULL---msg[%s-%u]\n", 
			OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
		return;
	}

	u32 dwRet = pTask->ProcMsg(pcMsg);
	if (dwRet == PROCMSG_DEL)
	{
		INSTLOG(CUI_APP, ALL_LEV, "����[%s-%u]�������Ҫɾ��---msg[%s-%u]\n", 
			pTask->GetObjName(), pTask->GetTaskNO(),
			OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

		delete pTask;
		pTask = NULL;
	}
	else if (dwRet == PROCMSG_FAIL)
	{
		INSTLOG(CUI_APP, ERROR_LEV, "����[%s-%u]����ʧ��---msg[%s-%u]\n", 
			pTask->GetObjName(), pTask->GetTaskNO(),
			OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
	}
	else if (dwRet == PROCMSG_OK)
	{
		INSTLOG(CUI_APP, ALL_LEV, "����[%s-%u]����ɹ�---msg[%s-%u]\n", 
			pTask->GetObjName(), pTask->GetTaskNO(),
			OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
	}
	else
	{
		INSTLOG(CUI_APP, ERROR_LEV, "����[%s-%u]������δ֪---msg[%s-%u]\n", 
			pTask->GetObjName(), pTask->GetTaskNO(),
			OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
	}
}

void CCuiInst::WriteLogWithoutLoginSession(u32 dwErrorCode, const string &userName, const string &strIp)
{

	string  strLog = GetLanDesc(LAN_DESC_LOGIN) + GetLanDesc(LAN_DESC_FAIL);

	CUsrOprLogTask* pcTask = new CUsrOprLogTask(this, "",//��ʱ��û���û���¼session												
												string(DEV_TYPE_UAS) + "@" + GetDomainName(),
												enUserOpTypeLogin,
												(u32)dwErrorCode,
												strLog,
												string()
												);

	pcTask->m_tLogInfo.userName = userName;
	pcTask->m_tLogInfo.netAddr = strIp;
	pcTask->InitStateMachine();
	pcTask->StartSendLog();

	INSTLOG(CUI_APP, ERROR_LEV, "Cu�û�[%s][%s] ��¼ʧ��, ������[%d] => UAS��־\n", userName.c_str(), strIp.c_str(), dwErrorCode);
}
void CCuiInst::ProcRedisSdkDiscCB(const TRedisSdkDiscCB* const ptCB)
{
    INSTLOG(CUI_APP, EVENT_LEV, "cui disconnected from redis errno[%d]\n", ptCB->emCode);

    OnRedisDisconnect();
}

void CCuiInst::ProcRedisSdkCB_SubsNtf(const TRedisSdkSubCB* const ptCB)
{
    INSTLOG(CUI_APP, EVENT_LEV,"ProcRedisSdkCB_SubsNtf cChannel=[%s] data=[%s]\n",ptCB->cSsChn.GetType().c_str(), ptCB->strContent.c_str());

    const CRedisSsChannel& cChannel = ptCB->cSsChn;

    const string& strModuleStatus = ptCB->strContent;

    if(cChannel.GetType() == RedisSsChannelType_Module_Status || cChannel.GetType() == RedisSsChannelType_Module_Type_Status)
    {
        TRedisModuleStatus tRedisModuleStatus;
        tRedisModuleStatus.FromJson(strModuleStatus);
        if(tRedisModuleStatus.tState == RedisModuleState_Restart || tRedisModuleStatus.tState == RedisModuleState_Stop)
        {
            //�˴���ʱֱ��delete task,����Ͷ����Ϣÿ��task��Ҫ������״̬����
            //CMessage cMsg;
            //cMsg.event = Redis2CUI_ModuleState(tRedisModuleStatus.tState);

            set<u32> cTasks;
            CRedisSsMng::GetInstance()->GetSsers(ptCB->cSsChn, cTasks);
            for (set<u32>::const_iterator it = cTasks.begin(); cTasks.end() != it; ++it)
            {
                CTask *pcTask = FindTask(*it);
                if (NULL != pcTask)
                {
                    delete pcTask;
                    pcTask = NULL;
                    //ProcTaskMsg(pcTask, &cMsg);
                }
            }
        }
    }
    else
    {
        INSTLOG(CUI_APP, WARNING_LEV, "Unknown Notify type[%s]\n", cChannel.GetType().c_str());
    }
}

u16 CCuiInst::Redis2CUI_ModuleState(const TRedisModuleState &tRedisModuleState)
{
    if(tRedisModuleState == RedisModuleState_Start)
    {
        return CUI_REDISSDK_MODULE_START;
    }
    else if(tRedisModuleState == RedisModuleState_Restart)
    {
        return CUI_REDISSDK_MODULE_RESTART;
    }
    else if(tRedisModuleState == RedisModuleState_Stop)
    {
        return CUI_REDISSDK_MODULE_STOP;
    }
    else
    {
        return CUI_REDISSDK_MODULE_UNKNOWN;
    }
}

//ֻҪredissdk Initialize �ɹ���Ϳ���ͨ��������ģ��״̬
void CCuiInst::SubscribeModuleStatus()
{
    //set<string> cModuleIds;

    ////��ȡcmu����ͨ�� 
    //string strCmuModuleId = g_cCuiConfig.GetCmuURI().GetUser();

    //cModuleIds.insert(strCmuModuleId);

    //

    //set<CRedisSsChannel> cChannels= g_cCuiRedisSdk.GetModuleSsChannelById(cModuleIds);

    //ECMS_ERRORCODE emRet = g_cCuiRedisSdk.UpdateSsChannel(cChannels);

    //if(emRet != CMS_SUCCESS)
    //{
    //    INSTLOG(CUI_APP, ERROR_LEV, "redissdk subscribe module status error[%d]\n",emRet);
    //}
}