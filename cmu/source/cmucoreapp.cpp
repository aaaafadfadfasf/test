
#include "cms/cms_const.h"
#include "cms/cms_errorcode.h"

#include "cms/cmu/cmu_event.h"
#include "cms/cmu/cmu_struct.h"
#include "cms/cmu/cmu_proto.h"
#include "cms/cmu/cmu_redis.h"

#include "cms/tas/tas_event.h"
#include "cms/tas/tas_struct.h"
#include "cms/tas/tas_proto.h"

#include "cms/natagent/natagent.h"

#include "cmudbg.h"

#include "loginsession.h"
#include "regtastask.h"
#include "puiregtask.h"
#include "cuiregtask.h"
#include "culogintask.h"
#include "puregtask.h"
#include "pubatchregtask.h"
#include "pusstask.h"
#include "pureqrsptask.h"
#include "puptzctrltask.h"
#include "pureqrspntftask.h"

#include "vtduregtask.h"
#include "inviteswitchtask.h"
#include "invitesenderswitchtask.h"
#include "invitereceiverswitchtask.h"
#include "3pcmsgtask.h"
#include "3pcinvitetask.h"
#include "devlogintask.h"
#include "uaswritelogtask.h"
#include "nmstask.h"
#include "cmureqrsptask.h"
#include "catalogsstask.h"
#include "pulistsstask.h"
#include "domaincatalogssertask.h"
#include "cmudata.h"
#include "cms/cmu/cmu_redis.h"
#include "cms/cmu/kdmstruct.h"
#include "getofflinedevtask.h"
#include "pudecinvitetask.h"
#include "flowctrltask.h"
#include "cmupassiveconnecttask.h"
#include "uaslogtask.h"
#include "redisloginsession.h"
#include "switchentrytask.h"
#include "3pcmsgentrytask.h"
#include "3pcinviteentrytask.h"
#include "cmemorymngtask.h"

extern CXSemLock g_cMainSemLock;

CCmuCoreApp g_cCmuCoreApp;

CRegTasTask* g_pcRegTasTask = NULL;

CNmsTask* g_pcNmsTask = NULL;

TCmuStat g_tCmuStat;

CPuQueryTaskList g_cPuQueryTaskList;

TMappingTable g_tMappingTable;
TDomainTable  g_tDomainTable;

CUasLogTask* g_pcUasLogProcTask;


CCmuCoreInst::CCmuCoreInst() : CInstExt()
{
    ClearInst();

	m_pcFlowCtrlMngTask = NULL;
}

CCmuCoreInst::~CCmuCoreInst()
{
    ClearInst();
}

void CCmuCoreInst::ClearInst()
{
    EmptyTask();
}

void CCmuCoreInst::DisProcess()
{
    //断链处理
    ClearInst();
    NextState(Idle);
}

LPCSTR CCmuCoreInst::GetStrState() const
{
    switch(CurState())
    {
    case Idle:
        return "Idle";
    case RegProxy:
        return "RegProxy";
    case Reg3as:
        return "Reg3as";
    case Service:
        return "Service";
    default:
        return "未知状态";
    }
}


void CCmuCoreInst::DaemonEntry(CMessage *const pcMsg)
{
    u8 byLogLev = EVENT_LEV;

    INSTLOG(CMU_MOD_INS, byLogLev, "Receive Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

    //对cmu不支持的消息进行统一的错误应答
    if (!IsCmuSurpportEvent(pcMsg->event))
    {
        INSTLOG(CMU_MOD_INS, ERROR_LEV, "Cmu recv Unsurpported Msg[%s-%d], Cmu can't process it!!!!!!!!\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

        COspSipMsg* pSipMsg = (COspSipMsg*)pcMsg->content;
        if (pSipMsg == NULL)
        {
            INSTLOG(CMU_MOD_INS, ERROR_LEV, "ospsip 消息为空\n");
            return;
        }

        if (pSipMsg->GetSipRsp() != KDSIP_EVENT_UNKNOWN)
        {
            TOspTransID tOspTrasnID;
            tOspTrasnID.dwIID = GetOspIID();
            string strEmpty;
            OspSipPostRsp(pSipMsg->GetSipRsp(), pSipMsg->GetSipTransID(), strEmpty, 
                tOspTrasnID, KDSIP_RESPONSE_403_FORBIDDEN);

            INSTLOG(CMU_MOD_INS, ERROR_LEV, "cmu response sip status code[%d] to peer!!!!!!\n", KDSIP_RESPONSE_403_FORBIDDEN);
        }
        else
        {
            INSTLOG(CMU_MOD_INS, ERROR_LEV, "Msg[%s-%d] is not Sip-req event, needn't response to peer!!!\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        
        return;
    }

    switch (pcMsg->event)               // 有些消息是在所有状态下都需要处理的
    {
    case CMU_FASTDB_SUB_CB:
        {
            TFastdbSubCB *ptCB = *(TFastdbSubCB **)(pcMsg->content);
            if (NULL != ptCB)
            {
                ProcNotify(ptCB->pcRedisPipe, ptCB->strChn, ptCB->strData);

                delete ptCB;
                ptCB = NULL;
            }
        }
        break;
    case CMU_REDISSDK_DISC_CB:
        {
            TRedisSdkDiscCB *ptCB = *(TRedisSdkDiscCB **)(pcMsg->content);
            if (NULL != ptCB)
            {
                ProcRdisSdkDisc(ptCB->pcRedisPipe, ptCB->emCode);

                delete ptCB;
                ptCB = NULL;
            }
        }
        break;
    case CMU_REDISSDK_SUB_CB:
        {
            TRedisSdkSubCB *ptCB = *(TRedisSdkSubCB **)(pcMsg->content);
            if (NULL != ptCB)
            {
                ProcRdisSdkNotify(ptCB->pcRedisPipe, ptCB->cSsChn, ptCB->strContent);

                delete ptCB;
                ptCB = NULL;
            }
        }
        break;
    default:
        {
            switch(CurState())
            {
            case Idle:
                {
                    OnIdle(pcMsg);
                }
                break;

            case RegProxy:
                {
                    OnRegProxy(pcMsg);        
                }
                break;

            case Reg3as:
                {
                    OnReg3as(pcMsg);        
                }
                break;

            case Service:
                {
                    OnService(pcMsg);
                }
                break;

            default:
                {            
                    INSTLOG(CMU_MOD_INS, WARNING_LEV, "unkown state recv Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
                }
                break;
            }  
        }
        break;
    }
}

BOOL32 CCmuCoreInst::OnUserInnerCommonMsg(CMessage* const pcMsg)
{
    BOOL32 bIsProcessMsg = TRUE;

    //内部消息处理

    switch(pcMsg->event)
    {
    case CMU_QUIT:
        {
            ClearInst();
            g_cMainSemLock.Give();
        }
        break;

    case CMU_PRINT_CONF:
        {
            g_cCmuConf.PrintData();
        }
        break;

    case CMU_PRINT_REDISSDK:
        {
            CRedisSdk &cRedisSdk = GetLocalRedisPipe().GetRedisSdk();
            cRedisSdk.PrintData();
        }
        break;

    case CMU_PRINT_STAT:
        {
            if (g_pcRegTasTask != NULL)
            {
                g_tCmuStat.strTasState = g_pcRegTasTask->GetStateName(g_pcRegTasTask->GetState());
            }
            else
            {
                g_tCmuStat.strTasState = "NULL";
            }
            
            g_tCmuStat.bIsConnectNms = g_cNmsLib.HasConnectedNms();
            g_tCmuStat.strCmuState = GetStrState();
            g_tCmuStat.PrintData();
            if (g_bDebugOn)
            {
                g_cPuiNotifyList.PrintData();
            }
        }
        break;

    case CMU_PRINT_PUI_LIST:
        {
            LPCSTR szPuiType = (LPCSTR)(pcMsg->content);
            if (NULL == szPuiType)
            {
                g_cPuiList.PrintData();
            }
            else
            {
                g_cPuiList.PrintData(szPuiType);
            }
        }
        break;

    case CMU_PRINT_CUI_LIST:
        {
            g_cCuiList.PrintData();
        }
        break;

	case CMU_PRINT_SUM_PU:
		{
            TPrintDevParam *ptParam = (TPrintDevParam *)(pcMsg->content);
            string strDomainName(ptParam->m_achDomainName);
            if (strDomainName.empty())
            {
                strDomainName = GetLocalUri().GetDomain();
            }

            CDomainData *pcDomainData = GetDomainDataByDomainName(strDomainName);
            if (NULL != pcDomainData)
            {
                pcDomainData->GetPuList().PrintData(true);
            }
            else
            {
                OspPrintf(TRUE, FALSE, "域名[%s]无效!!!\n", strDomainName.c_str());
            }
		}
		break;

    case CMU_PRINT_DEV_LIST:
        {
            TPrintDevParam *ptParam = (TPrintDevParam *)(pcMsg->content);
            string strDomainName(ptParam->m_achDomainName);
            if (strDomainName.empty())
            {
                strDomainName = GetLocalUri().GetDomain();
            }

            CDomainData *pcDomainData = GetDomainDataByDomainName(strDomainName);
            if (NULL != pcDomainData)
            {
                pcDomainData->GetPuList().PrintData(false, ptParam->m_byState);
            }
            else
            {
                OspPrintf(TRUE, FALSE, "域名[%s]无效!!!\n", strDomainName.c_str());
            }
        }
        break;

    case CMU_PRINT_PU_INFO:
        {
            LPCSTR szDevUri = (LPCSTR)(pcMsg->content);
            if (szDevUri == NULL)
            {
                OspPrintf(TRUE, FALSE, "puid不能为空\n");
            }
            else
            {
                TSipURI tDevUri;
                tDevUri.SetURIString(szDevUri);
                if (!tDevUri.IsValidURI())
                {
                    tDevUri.SetUser(szDevUri);

                    string strDomainId;
                    if (CCmuData::GetInstance()->GetLocalRedisPipe().GetDevDomainId(szDevUri, strDomainId) && !strDomainId.empty())
                    {
                        tDevUri.SetDomain(DomainId2DomainName(strDomainId));
                    }
                    else
                    {
                        tDevUri.SetDomain(GetLocalUri().GetDomain());
                    }
                }
                
                CPuData *pcPuData = GetPuDataByUri(tDevUri);
                if (NULL != pcPuData)
                {
                    pcPuData->PrintData();
                }
                else
                {
                    OspPrintf(TRUE, FALSE, "pu[%s]不存在\n", tDevUri.GetURIString().c_str());
                }
            }
        }
        break;

	case CMU_PRINT_DOMAIN_LIST:
		{
            CCmuData::GetInstance()->PrintDomainList();
		}
		break;

    case CMU_PRINT_DOMAIN_SYNC_PARAM:
        {
            TDomainSyncParam *ptParam = (TDomainSyncParam *)(pcMsg->content);
            if (NULL != ptParam)
            {
                CDomainData *pcDomainData = GetDomainDataByDomainName(ptParam->szDomainName);
                if (NULL != pcDomainData)
                {
                    CCmuConnecterTask *pcConnectTask = pcDomainData->GetConnectTask();
                    if (NULL != pcConnectTask)
                    {
                        pcConnectTask->SetSyncParam(ptParam->dwSyncNumOnce);
                    }
                    else
                    {
                        OspPrintf(TRUE, FALSE, "CmuConnectTask is NULL!\n");
                    }
                }
                else
                {
                    OspPrintf(TRUE, FALSE, "szDomainName无效!\n");
                }
            }
        }
        break;

    case CMU_PRINT_DOMAIN_DATA:
        {
            LPCSTR szDomainName = (LPCSTR)(pcMsg->content);
            if (szDomainName == NULL)
            {
                OspPrintf(TRUE, FALSE, "szDomainId不能为空\n");
            }
            else
            {
                CDomainData *pcDomainData = CCmuData::GetInstance()->GetDomainDataByDomainName(szDomainName);
                if (NULL != pcDomainData)
                {
                    pcDomainData->PrintData();
                }
                else
                {
                    OspPrintf(TRUE, FALSE, "domain[%s]不存在\n", szDomainName);
                }
            }
        }
        break;

	case CMU_PRINT_PUID_URI_MAP_LIST:
		{
			OspPrintf(TRUE, FALSE, "PuidUriMapList Size: %d\n",
				g_cPuIdUriMapList.size());
			u32 dwNum = 0;
			for (TPuIdUriMapList::const_iterator itor = g_cPuIdUriMapList.begin(); itor != g_cPuIdUriMapList.end(); itor++)
			{
				dwNum++;
				OspPrintf(TRUE, FALSE, "[%u], PUID[%s], URI[%s]\n",
					dwNum,
					itor->first.c_str(),
					itor->second.c_str());
			}
		}
		break;



    case CMU_PRINT_CU_LIST:
        {
            g_cCuList.PrintData();
        }
        break;      

    case CMU_PRINT_CMU_LIST:
        {
            g_cCmuList.PrintData();
        }
        break;

    case CMU_PRINT_VTDU_LIST:
        {
            g_cVtduList.PrintData();
        }
        break;

    case CMU_PRINT_PE_LIST:
        {
            if (g_pcRegTasTask != NULL)
            {
                CLoginSession* pTask = g_pcRegTasTask;
                OspPrintf(TRUE, FALSE, "tas登录信息：\n");
                OspPrintf(TRUE, FALSE, "  dev[%s-%s]-Server[%s]-session[%s]-TaskNO[%u]-State[%s]\n", 
                    pTask->GetDevType().c_str(), pTask->GetDevUri().c_str(), pTask->GetServerUri().c_str(),
                    pTask->GetSession().c_str(), pTask->GetTaskNO(), pTask->GetStateName(pTask->GetState()));
                OspPrintf(TRUE, FALSE, "\n");
            }
            
            g_cGeneralLoginList.PrintData();
        }
        break;

    case CMU_PRINT_SS_LIST:			// 打印本域所有订阅事务信息
        {
			OspPrintf(TRUE, FALSE, "\n-------------------------------pusstask---------------------------------------\n");

			PrintTask("CPuSubscribeTask");

			OspPrintf(TRUE, FALSE, "\n-------------------------------pulistsstask-----------------------------------\n");

			PrintTask("CPuListSubscribeTask");

			OspPrintf(TRUE, FALSE, "\n-------------------------------catalogsstask----------------------------------\n");

			PrintTask("CCatalogSubscribeTask");
        }
        break;

    case CMU_PRINT_DSS_LIST:
        {
			OspPrintf(TRUE, FALSE, "\n-------------------------------domainpulistssertask-----------------------------\n");

			PrintTask("CDomainPuListSserTask");

			OspPrintf(TRUE, FALSE, "\n-------------------------------domaincatalogssertask----------------------------\n");

			PrintTask("CDomainCatalogSserTask");
        }
        break;

    case CMU_PRINT_RSS_LIST:
        {
            CCmuData::GetInstance()->PrintRedisPipeList();

            OspPrintf(TRUE, FALSE, "\n============================================================================\n");

            CRedisSdk &cRedisSdk = GetLocalRedisPipe().GetRedisSdk();
            CRedisSsMng::GetInstance()->PrintData(g_bDebugOn);
        }
        break;

    case CMU_PRINT_SSER_LIST:
        {
            LPCSTR szDevUri = (LPCSTR)(pcMsg->content);
            string strDevUri(szDevUri);
            if (!strDevUri.empty())
            {
                CPuData *pcPuData = GetPuDataByUri(strDevUri);
                if (NULL != pcPuData)
                {
                    pcPuData->PrintSsList();
                }
            }
        }
        break;

    case CMU_PRINT_SWITCH_NODE_LIST:
        {
            TPrintSwitchParam* ptParam = (TPrintSwitchParam*)pcMsg->content;

            if (ptParam->m_achDevUri[0] == '\0')
            {
                OspPrintf(TRUE, FALSE, "Pint all SwitchNode.\n");
                g_cSwitchList.PrintData();
            }
            else
            {
                TChannel tDevChn(ptParam->m_achDevUri, ptParam->m_byChn);
                CSwitchTask* pcSwitchNode = g_cSwitchList.FindSwitchNode(tDevChn);
                if(pcSwitchNode)
                {
                    pcSwitchNode->PrintData(CSwitchTask::TRACE_ALL);
                }
                else
                {
                    OspPrintf(TRUE, FALSE, "CMU_PRINT_SWITCH_NODE_LIST: can't find DevChn[%s].\n", tDevChn.ToString().c_str());
                }
            }

        }
        break;

    case CMU_PRINT_NMS_DIS_ALARM_LIST:
        {
            if (g_pcNmsTask != NULL)
            {
                g_pcNmsTask->PrintDisAlarm();
            }
        }
        break;

    case CMU_PRINT_NMS_OVERLOAD_ALARM_LIST:
        {
            if (g_pcNmsTask != NULL)
            {
                g_pcNmsTask->PrintOverloadAlarm();
            }
        }
		break;

    case CMU_PRINT_SON_TASK_LIST_BY_NO:
        {
            u16 wTaskNO = *(u16*)(pcMsg->content);
            CTask* ptTask = FindTask(wTaskNO);
            CLoginSession* pSessTask = NULL;
            do 
            {
                if (ptTask == NULL)
                {
                    INSTLOG(CMU_MOD_INS, WARNING_LEV, "task no[%u]没有对应的事务，[%s-%d]\n", 
                        wTaskNO, OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
                    break;
                }

                pSessTask = dynamic_cast<CLoginSession*>(ptTask);
                if (pSessTask == NULL)
                {
                    INSTLOG(CMU_MOD_INS, WARNING_LEV, "task[%s-%u]不是登录事务，[%s-%d]\n", 
                        ptTask->GetObjName(), wTaskNO, 
                        OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
                    break;
                }
            } 
            while (0);
            
            if (pSessTask != NULL)
            {
                pSessTask->PrintChildTasks();
            }          
        }
        break;

    case CMU_PRINT_SON_TASK_LIST_BY_SESS:
        {
            const char* szPuUri = (const char*) (pcMsg->content);
            CLoginSession* pSessTask = NULL;

            do 
            {
                if (szPuUri == NULL)
                {
                    INSTLOG(CMU_MOD_INS, WARNING_LEV, "szPuUri == NULL\n");
                    break;
                }

                string strSession = szPuUri;
                pSessTask = GetLoginSession(strSession);
                if (pSessTask == NULL)
                {
                    INSTLOG(CMU_MOD_INS, WARNING_LEV, "session[%s]不存在登录事务，[%s-%d]\n", 
                        szPuUri, OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
                    break;
                }
            } 
            while (0);

            if (pSessTask != NULL)
            {
                pSessTask->PrintChildTasks();
            }
        }
        break;

	case CMU_PRINT_CERTAIN_PU_SWITCH_INFO:
		{
			TPrintSwitchParam* ptParam = (TPrintSwitchParam*)pcMsg->content;
			if (ptParam->m_achDevUri[0] != '\0')
			{
				TChannel tDevChn(ptParam->m_achDevUri, ptParam->m_byChn);
				const CSwitchTask* pcSndSwitchNode = g_cSwitchList.FindSwitchNode(tDevChn, CHANNEL_TYPE_SENDONLY);
				if (NULL != pcSndSwitchNode)
				{
					OspPrintf(TRUE, FALSE, "\n=========================以DevChn[%s]作为码流发送方的交换信息如下=========================\n", 
						tDevChn.ToString().c_str());

					const CSwitchTaskList &tRcvTaskList = pcSndSwitchNode->GetOutputList();
					for (CSwitchTaskList::const_iterator it = tRcvTaskList.begin(); tRcvTaskList.end() != it; ++it)
					{
						const CSwitchTask *pcRcvSwitchNode = *it;

						const CSwitchPin &cSender   = pcSndSwitchNode->GetSender();
						const CSwitchPin &cVtdu     = pcSndSwitchNode->GetReceiver();
						const CSwitchPin &cReceiver = pcRcvSwitchNode->GetReceiver();
						
						OspPrintf(TRUE, FALSE, "SessionName[%s]\nSender[%s] ---> Vtdu[%s] --> Receiver[%s]\n\n", 
							pcSndSwitchNode->GetSessionName().c_str(), 
							cSender.GetChannel().ToString().c_str(), 
							cVtdu.GetChannel().ToString().c_str(), 
							cReceiver.GetChannel().ToString().c_str());

						vector<CSwitchPin> cInfos;
						cInfos.push_back(cSender);
						cInfos.push_back(cVtdu);
						cInfos.push_back(cReceiver);
						for (u32 i = 0; i < cInfos.size(); ++i)
						{
							const CSwitchPin &cInfo = cInfos[i];
							OspPrintf(TRUE, FALSE, "DevType[%s] Uri[%s]\n", 
								cInfo.GetDevType().c_str(), cInfo.GetChannel().ToString().c_str());
							const TMediaDescList &tSndMediaDescList = cInfo.GetSdp().GetMediaDescList();
							for (TMediaDescList::const_iterator it = tSndMediaDescList.begin(); 
								tSndMediaDescList.end() != it; ++it)
							{
								const TMediaTransChannelList &tSndTransChnList = it->GetMediaTransChnList();
								for (TMediaTransChannelList::const_iterator it4Chn = tSndTransChnList.begin(); 
									tSndTransChnList.end() != it4Chn; ++it4Chn)
								{
									string strAddr;
									s8 achTmp[128] = {0};
									for (TNetAddrList::const_iterator it4Addr = it4Chn->GetAddrList().begin(); 
										it4Chn->GetAddrList().end() != it4Addr; ++it4Addr)
									{
										memset(achTmp, 0, sizeof(achTmp));
										sprintf(achTmp, "[%s:%d]", it4Addr->GetNetIp().c_str(), it4Addr->GetNetPort());
										strAddr += string(achTmp);
									}

									OspPrintf(TRUE, FALSE, "MediaType[%s] TransChnType[%s] NetAddr%s\n", 
										it->GetMediaType().c_str(), it4Chn->GetTransChnType().c_str(), strAddr.c_str());
								}
							}

							OspPrintf(TRUE, FALSE, "\n");
						}
					}
				}
				else
				{
					OspPrintf(TRUE, FALSE, "\n没有以DevChn[%s]作为码流发送方的交换存在!!!\n", 
						tDevChn.ToString().c_str());
				}

				const CSwitchTask* pcRcvSwitchNode = g_cSwitchList.FindSwitchNode(tDevChn, CHANNEL_TYPE_RECVONLY);
				if (NULL != pcRcvSwitchNode)
				{
					OspPrintf(TRUE, FALSE, "\n=========================以DevChn[%s]作为码流接收方的交换信息如下=========================\n", 
						tDevChn.ToString().c_str());

					const CSwitchTask *pcSndSwitchTask = pcRcvSwitchNode->GetInput();
					if (NULL != pcSndSwitchTask)
					{
						const CSwitchPin &cSender   = pcSndSwitchTask->GetSender();
						const CSwitchPin &cVtdu     = pcSndSwitchTask->GetReceiver();
						const CSwitchPin &cReceiver = pcRcvSwitchNode->GetReceiver();

						OspPrintf(TRUE, FALSE, "SessionName[%s]\nSender[%s] ---> Vtdu[%s] --> Receiver[%s]\n\n", 
							pcRcvSwitchNode->GetSessionName().c_str(), 
							cSender.GetChannel().ToString().c_str(), 
							cVtdu.GetChannel().ToString().c_str(), 
							cReceiver.GetChannel().ToString().c_str());

						vector<CSwitchPin> cInfos;
						cInfos.push_back(cSender);
						cInfos.push_back(cVtdu);
						cInfos.push_back(cReceiver);
						for (u32 i = 0; i < cInfos.size(); ++i)
						{
							const CSwitchPin &cInfo = cInfos[i];
							OspPrintf(TRUE, FALSE, "DevType[%s] Uri[%s]\n", 
								cInfo.GetDevType().c_str(), cInfo.GetChannel().ToString().c_str());
							const TMediaDescList &tSndMediaDescList = cInfo.GetSdp().GetMediaDescList();
							for (TMediaDescList::const_iterator it = tSndMediaDescList.begin(); 
								tSndMediaDescList.end() != it; ++it)
							{
								const TMediaTransChannelList &tSndTransChnList = it->GetMediaTransChnList();
								for (TMediaTransChannelList::const_iterator it4Chn = tSndTransChnList.begin(); 
									tSndTransChnList.end() != it4Chn; ++it4Chn)
								{
									string strAddr;
									s8 achTmp[128] = {0};
									for (TNetAddrList::const_iterator it4Addr = it4Chn->GetAddrList().begin(); 
										it4Chn->GetAddrList().end() != it4Addr; ++it4Addr)
									{
										memset(achTmp, 0, sizeof(achTmp));
										sprintf(achTmp, "[%s:%d]", it4Addr->GetNetIp().c_str(), it4Addr->GetNetPort());
										strAddr += string(achTmp);
									}

									OspPrintf(TRUE, FALSE, "MediaType[%s] TransChnType[%s] NetAddr%s\n", 
										it->GetMediaType().c_str(), it4Chn->GetTransChnType().c_str(), strAddr.c_str());
								}
							}

							OspPrintf(TRUE, FALSE, "\n");
						}
					}
				}
				else
				{
					OspPrintf(TRUE, FALSE, "\n没有以DevChn[%s]作为码流接收方的交换存在!!!\n", 
						tDevChn.ToString().c_str());
				}
			}
		}
		break;

    case CMU_PRINT_CERTAIN_CU_SWITCH_INFO:
        {
            TPrintSwitchParam* ptParam = (TPrintSwitchParam*)pcMsg->content;
            if (ptParam->m_achDevUri[0] != '\0')
            {
                // todo by xiongxh 根据cu的登录Session，打印出该cu相关的所有交换
				OspPrintf(TRUE, FALSE, "暂未实现的调试命令!!!\n");
            }
        }
        break;

    default:
        {
            bIsProcessMsg = FALSE;
        }
        break;
    }  

    return bIsProcessMsg;
}

void CCmuCoreInst::OnIdle(CMessage* const pcMsg)
{
    switch(pcMsg->event)
    {
    case CMU_START:
        {
            g_tCmuStat.strStartTime = COspTime::GetCurrStrTime();
            RegSipProxy();
            NextState(RegProxy);
        }
        break;

    default:
        {
            INSTLOG(CMU_MOD_INS, WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }    
}

void CCmuCoreInst::OnRegProxy(CMessage* const pcMsg)
{
    switch(pcMsg->event)
    {
	case DOMAIN_TABLE_NTF_REQ:
		{
			ProcDomainTableNtf(pcMsg);
		}
		break;

    case OSP_PROXY_CONNECT:
        {
            ProxyConnectProc();

			// 登录uas服务器，用于记录设备日志、告警日志
			CmuLoginServer(g_cCmuConf.m_tLocalUri.GetUasURI().GetURIString());

			CmuRegTas();
			NextState(Reg3as);
        }
        break;

    case OSP_PROXY_DISCONNECT:
        {
            ProxyDisProc();
        }
        break;
        
    default:
        {
            INSTLOG(CMU_MOD_INS, WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }
}

void CCmuCoreInst::OnReg3as(CMessage* const pcMsg)
{
    switch(pcMsg->event)
    {
	case DOMAIN_TABLE_NTF_REQ:
		{
			ProcDomainTableNtf(pcMsg);
		}
		break;

	case DEV_LOGIN_RSP:
	case TAS_LOGIN_RSP:
	case OSP_SIP_DISCONNECT:
    case NETADDR_QUERY_RSP:
        {
            COspSipMsg* pSipMsg = (COspSipMsg*)pcMsg->content;
            if (pSipMsg == NULL)
            {
                INSTLOG(CMU_MOD_INS, ERROR_LEV, "ospsip 消息为空\n");
                return;
            }

            CTask* pTask = FindTask(pSipMsg->GetOspTaskNO());
            if (pTask != NULL)
            {
                ProcTaskMsg(pTask, pcMsg);

				if (NULL != g_pcRegTasTask && CRegTasTask::Service == g_pcRegTasTask->GetState())
				{
					//启动网管事务
					g_pcNmsTask = new CNmsTask(this);
					g_pcNmsTask->InitStateMachine();

                    // 开启数据更新
                    CCmuData::GetInstance()->StartWork();

					//
					CTask *pTask = new CMemoryMngTask(this);
					pTask->InitStateMachine();

                    NextState(Service);
				}
            }
            else
            {
                INSTLOG(CMU_MOD_INS, ERROR_LEV, "taskNO[%u]没有对应事务---msg[%s-%u]\n", 
                    pSipMsg->GetOspTaskNO(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            }
        }
        break;

    case OSP_PROXY_DISCONNECT:
        {
            ProxyDisProc();
        }
        break;

    case OSP_PROXY_CONNECT:
        {
            ProxyConnectProc();
        }
        break;

    default:
        {
            INSTLOG(CMU_MOD_INS, WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }
}

void CCmuCoreInst::OnService(CMessage* const pcMsg)
{
    switch(pcMsg->event)
    {
	case DOMAIN_TABLE_NTF_REQ:
		{
			ProcDomainTableNtf(pcMsg);
		}
		break;

    case PUI_REG_REQ:
    case PU_REG_REQ:
    case CUI_REG_REQ:
    case CU_LOGIN_REQ:
	case VTDU_REG_REQ:
	case INVITE_REQ:
    case KEY_FRAME_REQ:
    case INVITE_THIRD_PARTY_CALL_REQ:
    case START_THIRD_PARTY_CALL_REQ:
    case STOP_THIRD_PARTY_CALL_REQ:
    case CATALOG_STATUS_SS_REQ:
    case PULIST_STATUS_SS_REQ:
    case PU_STATUS_SS_REQ:
    case PU_PTZ_CTRL_REQ:
    case PU_VID_ENC_PARAM_GET_REQ:
    case PU_VID_ENC_PARAM_SET_REQ:
    case PU_SYS_PARAM_GET_REQ:
    case PU_SYS_PARAM_SET_REQ:
    case DEV_LOGIN_REQ:
    case PU_IO_OUT_REQ:
    case PU_READ_SEC_REQ:
	case PU_ALARM_RESET_REQ:
	case PU_CAMERA_TYPE_SET_REQ:
	case PU_MOTION_DETECT_SET_REQ:
    case PU_MOTION_DETECT_GET_REQ:
    case PU_AREA_SHADE_SET_REQ:
    case PU_AREA_SHADE_GET_REQ:
    case PU_REC_QRY_REQ:
    case PU_PIC_QRY_REQ:
    case PU_TRANSPARENT_QRY_REQ:
	case PU_DEC_STYLE_SET_REQ:
	case PU_DEC_STYLE_GET_REQ:
	case PU_VIDSRC_OSD_SET_REQ:
	case PU_VIDSRC_OSD_GET_REQ:
    case PU_VSIP_OPERATE_REQ:
    case PU_TRANSPARENT_OPERATE_REQ:
    case PU_PIC_SNAPSHOT_REQ:
    case PU_PIC_DOWNLOAD_REQ:
    case PU_REC_DAYS_QRY_REQ:

    case PU_GPS_CFG_SET_REQ:
    case PU_GPS_CFG_GET_REQ:
    case PU_GPS_SS_SET_REQ:
    case PU_GPS_SS_GET_REQ:

    case PU_RECORD_START_REQ:
    case PU_RECORD_STOP_REQ:
    case PU_RECORD_DEL_REQ:
    case PU_TRANSDATA_CFG_SET_REQ:
    case PU_TRANSDATA_CFG_GET_REQ:
    case PU_LOGO_BMPOSD_TRANSMIT_REQ:
	case PU_LONG_OSD_SET_REQ:
	case PU_LONG_OSD_GET_REQ:
	case PU_MULTI_OSD_SET_REQ:
	case PU_MULTI_OSD_GET_REQ:
    case PU_AUD_ENC_PARAM_SET_REQ:
    case PU_AUD_ENC_PARAM_GET_REQ:

	case PU_SNAPSHOT_CFG_GET_REQ: //add by ywy 2014.5.14
	case PU_SNAPSHOT_CFG_SET_REQ:

	case PU_SERIALPARAM_CFG_GET_REQ:
	case PU_SERIALPARAM_CFG_SET_REQ:
	case CMU_PU_OFFLINEDEV_QRY_REQ:
	case INVITE_DECORDER_CALL_REQ:
        {
            CTask* pTask = CreateTask(pcMsg->event);
            if (pTask == NULL)
            {
                INSTLOG(CMU_MOD_INS, ERROR_LEV, "事务创建失败Msg[%s-%d]\n",
                    OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
                return;
            }

            ProcTaskMsg(pTask, pcMsg);
        }
        break;
	case MODULE_ON_OFF_LOG_NTF_REQ:			// 为其他模块写设备上下线日志
		{
			COspSipMsg* pSipMsg = (COspSipMsg*)pcMsg->content;
			CModuleOnOffLogNtfReq cReq;
			pSipMsg->GetMsgBody(cReq);

			CModuleOnOffLogNtfRsp cRsp;
			cRsp.SetHeadFromReq(cReq);
			cRsp.SetErrorCode(CMS_SUCCESS);
			TOspTransID tOspTrasnID;
			tOspTrasnID.dwIID = GetOspIID();
			OspSipPostRsp(pSipMsg->GetSipRsp(), pSipMsg->GetSipTransID(), cRsp, tOspTrasnID);

			TDeviceLogInfo tInfo(cReq.GetInfo());
			GetUasLogTask()->AddDeviceLog(tInfo);
		}
		break;

	case CONFIG_SYNC_SWITCH_SET_REQ:
		{
			ProcSwitchConfigSetReq(pcMsg);
		}
		break;
	case CONFIG_SYNC_SWITCH_GET_REQ:
		{
			ProcSwitchConfigGetReq(pcMsg);
		}
		break;
	case DOMAIN_ALARM_REQ:
		{
			COspSipMsg* pSipMsg = (COspSipMsg*)pcMsg->content;
			CDomainAlarmReq cReq;
			pSipMsg->GetMsgBody(cReq);
			TAlarmInput& tAlarm = cReq.GetAlarm();

			string strDevUri = tAlarm.GetDevUri();

			if (FALSE == IsSipUri(strDevUri))
			{
				if (g_cPuIdUriMapList.end() == g_cPuIdUriMapList.find(strDevUri))
				{
					CDomainAlarmRsp cRsp;
					cRsp.SetErrorCode(ERR_CMU_PU_NOT_IN_TOP);
					cRsp.SetSeqNum(cReq.GetSeqNum());
					cRsp.SetSession(cReq.GetSession());
					TOspTransID tOspTrasnID;
					tOspTrasnID.dwIID = GetOspIID();
					OspSipPostRsp(pSipMsg->GetSipRsp(), pSipMsg->GetSipTransID(), cRsp, tOspTrasnID);

					INSTLOG(CMU_MOD_INS, ERROR_LEV, "DOMAIN_ALARM_REQ 未找到PUID[%s]对应的URI\n",
						strDevUri.c_str());

					break;
				}

				strDevUri = g_cPuIdUriMapList.find(tAlarm.GetDevUri())->second;
			}

			CLoginSession* pTask = GetLoginSession(strDevUri);
			if ((NULL == pTask) || (!pTask->IsService()))
			{
				CDomainAlarmRsp cRsp;
				cRsp.SetErrorCode(ERR_CMU_PU_NOT_ONLINE);
				cRsp.SetSeqNum(cReq.GetSeqNum());
				cRsp.SetSession(cReq.GetSession());
				TOspTransID tOspTrasnID;
				tOspTrasnID.dwIID = GetOspIID();
				OspSipPostRsp(pSipMsg->GetSipRsp(), pSipMsg->GetSipTransID(), cRsp, tOspTrasnID);

				INSTLOG(CMU_MOD_INS, ERROR_LEV, "DOMAIN_ALARM_REQ 未找到PU[%s]对应的登录事务[%p]或事务未进入服务态\n",
					strDevUri.c_str(),
					pTask);
			}
			else
			{
				ProcTaskMsg(pTask, pcMsg);
			}
		}
		break;

    case CMU_CONNECT_REQ:
        {
            //跨域连接事务是长期事务

            //异域的cmu发起连接
            int dwErrorCode = CMS_SUCCESS;
            COspSipMsg* pSipMsg = (COspSipMsg*)pcMsg->content;

            CCmuConnectReq cReq;
            pSipMsg->GetMsgBody(cReq);

            TSipURI tPeerCmuUri;
            tPeerCmuUri.SetNOString(cReq.GetSrcUri());

            if (!tPeerCmuUri.IsValidURI()   //无效的SIP URI
                || tPeerCmuUri.GetUser() != UNIQUE_SIP_UA_CMU  //不是CMU URI
                || tPeerCmuUri.GetDomain() == GetLocalUri().GetDomain())  //不是异域CMU的URI
            {
                INSTLOG(CMU_MOD_CMU, ERROR_LEV, "无效cmu uri[%s]连接cmu\n",
                    tPeerCmuUri.GetURIString().c_str());

                dwErrorCode = ERR_CMU_CMU_INVALID_CMU_URI;

                CCmuConnectRsp cRsp;
                cRsp.SetErrorCode(dwErrorCode);
                cRsp.SetSeqNum(cReq.GetSeqNum());
                cRsp.SetSession(cReq.GetSession());
				cRsp.SetLocalIp(g_cCmuConf.m_tLocalAddr.GetIpStr());

                TOspTransID tOspTrasnID;
                tOspTrasnID.dwIID = GetOspIID();
                OspSipPostRsp(pSipMsg->GetSipRsp(), pSipMsg->GetSipTransID(), cRsp, tOspTrasnID);

                return;
            }

            CTask *pTask = CreateTask(pcMsg->event);
            if (pTask == NULL)
            {
                INSTLOG(CMU_MOD_INS, ERROR_LEV, "事务创建失败Msg[%s-%d]\n",
                    OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
                return;
            }

            ProcTaskMsg(pTask, pcMsg);
        }
        break;

	case PU_ALARM_INPUT_NTF_REQ:
		{
			COspSipMsg* pSipMsg = (COspSipMsg*)pcMsg->content;
			if (pSipMsg == NULL)
			{
				INSTLOG(CMU_MOD_INS, ERROR_LEV, "ospsip 消息为空\n");
				return;
			}

			string strSess;
			BOOL32 bLocalSess = GetLocalDomainSession(strSess, pSipMsg);
			if (FALSE == bLocalSess)
			{
				INSTLOG(CMU_MOD_INS, WARNING_LEV, "GetLocalDomainSession fail, maybe DomainPu Alarm\n");
				return;
			}

			CLoginSession* pTask = GetLoginSession(strSess);
			if (pTask != NULL)
			{
				ProcTaskMsg(pTask, pcMsg);
			}
			else
			{
				TOspTransID tOspTrasnID;
				tOspTrasnID.dwIID = GetOspIID();
				string strEmpty;
				OspSipPostRsp(pSipMsg->GetSipRsp(), pSipMsg->GetSipTransID(), strEmpty, 
					tOspTrasnID, KDSIP_RESPONSE_404_NOT_FOUND);

				INSTLOG(CMU_MOD_INS, ERROR_LEV, "Sess[%s]没有对应登录事务\n", strSess.c_str());
			}
		}
		break;

    case PU_UNREG_REQ:
    case CU_LOGOUT_REQ:
    case CU_GETCUILIST_REQ:
    case PU_GPS_DATA_NTF_REQ:
    case PU_DEV_CONFIG_NTF_REQ:
    case PU_TRANSPARENT_DATA_NTF_REQ:
    case DEV_LOGOUT_REQ:
	//{{{{{{{{{{{{{{{ add by Royan 20140527 前端录像状态通知
	case PU_RECORED_STATUS_NTF_REQ:
	//}}}}}}}}}}}}}}} add by Royan 20140527 前端录像状态通知
	case UPDATE_REG_NETSEG_REQ:
	case UPDATE_CULOGIN_INFO_REQ:
	case MAPPING_TABLE_NTF_REQ:
    case PU_ONLINE_NTF_REQ:
        {
            COspSipMsg* pSipMsg = (COspSipMsg*)pcMsg->content;
            if (pSipMsg == NULL)
            {
                INSTLOG(CMU_MOD_INS, ERROR_LEV, "ospsip 消息为空\n");
                return;
            }

			string strSess = GetSipSessionID(pSipMsg->GetMsgBody());
            CLoginSession* pTask = GetLoginSession(strSess);
            if (pTask != NULL)
            {
                ProcTaskMsg(pTask, pcMsg);
            }
            else
            {
                TOspTransID tOspTrasnID;
                tOspTrasnID.dwIID = GetOspIID();
                string strEmpty;
                OspSipPostRsp(pSipMsg->GetSipRsp(), pSipMsg->GetSipTransID(), strEmpty, 
                    tOspTrasnID, KDSIP_RESPONSE_404_NOT_FOUND);

                INSTLOG(CMU_MOD_INS, ERROR_LEV, "Sess[%s]没有对应登录事务\n", strSess.c_str());
            }
        }
        break;

    case PU_REC_QRY_NTF_REQ:
    case PU_PIC_QRY_NTF_REQ:
    case PU_TRANSPARENT_QRY_NTF_REQ:
	case CMU_PU_OFFLINEDEV_QRY_NTF_REQ:
	case TAS_VIDSRC_INFO_QRY_NTF_REQ:
        {
            COspSipMsg* pSipMsg = (COspSipMsg*)pcMsg->content;
            if (pSipMsg == NULL)
            {
                INSTLOG(CMU_MOD_INS, ERROR_LEV, "ospsip 消息为空\n");
                return;
            }

            string strQueryTag = GetQueryTag(pSipMsg->GetMsgBody());
            CTask* pTask = GetQueryTask(strQueryTag);
            if (pTask != NULL)
            {
                ProcTaskMsg(pTask, pcMsg);
            }
            else
            {
                TOspTransID tOspTrasnID;
                tOspTrasnID.dwIID = GetOspIID();
                string strEmpty;
                OspSipPostRsp(pSipMsg->GetSipRsp(), pSipMsg->GetSipTransID(), strEmpty, 
                    tOspTrasnID, KDSIP_RESPONSE_404_NOT_FOUND);

                INSTLOG(CMU_MOD_INS, ERROR_LEV, "QueryTag[%s]没有对应查询事务\n", strQueryTag.c_str());
            }
        }
        break;

    case CATALOG_STATUS_REFRESH_SS_REQ:
    case CATALOG_STATUS_UNSS_REQ:

    case PULIST_STATUS_REFRESH_SS_REQ:
    case PULIST_STATUS_UNSS_REQ:

    case PU_STATUS_REFRESH_SS_REQ:
    case PU_STATUS_UNSS_REQ:

    case PU_STATUS_NTF_RSP:
    case PU_CONFIG_NTF_RSP:
    case PU_SERVICE_STATUS_NTF_RSP:
    case PU_ALARM_STATUS_NTF_RSP:
    case PU_GPS_STATUS_NTF_RSP:
    case PU_TRANSDATA_STATUS_NTF_RSP:
	case PU_BATCH_NTF_RSP:

    case CATALOG_STATUS_SS_RSP:
    case CATALOG_STATUS_REFRESH_SS_RSP:
    case CATALOG_STATUS_UNSS_RSP:

    case PULIST_STATUS_SS_RSP:
    case PULIST_STATUS_REFRESH_SS_RSP:
    case PULIST_STATUS_UNSS_RSP:

    case PU_STATUS_SS_RSP:
    case PU_STATUS_REFRESH_SS_RSP:
    case PU_STATUS_UNSS_RSP:
    
    case PU_STATUS_NTF_REQ:
    case PU_CONFIG_NTF_REQ:
    case PU_SERVICE_STATUS_NTF_REQ:
    case PU_ALARM_STATUS_NTF_REQ:
	case PU_GPS_STATUS_NTF_REQ:
	case PU_TRANSDATA_STATUS_NTF_REQ:
	case PU_BATCH_NTF_REQ:

    case CMU_CONNECT_RSP:
    case PU_PTZ_CTRL_RSP:
    case PU_VID_ENC_PARAM_GET_RSP:
    case PU_VID_ENC_PARAM_SET_RSP:
    case PU_SYS_PARAM_GET_RSP:
    case PU_SYS_PARAM_SET_RSP:
    case PU_IO_OUT_RSP:
    case PU_READ_SEC_RSP:
	case PU_ALARM_RESET_RSP:
	case PU_CAMERA_TYPE_SET_RSP:
	case PU_MOTION_DETECT_SET_RSP:
    case PU_MOTION_DETECT_GET_RSP:
    case PU_AREA_SHADE_SET_RSP:
    case PU_AREA_SHADE_GET_RSP:
    case CMU_DISCONNECT_PU_RSP:
    case PU_REC_QRY_RSP:
    case PU_REC_QRY_NTF_RSP:
	case PU_DEC_STYLE_SET_RSP:
	case PU_DEC_STYLE_GET_RSP:
	case PU_VIDSRC_OSD_SET_RSP:
	case PU_VIDSRC_OSD_GET_RSP:
    case PU_VSIP_OPERATE_RSP:
    case PU_TRANSPARENT_OPERATE_RSP:

    case PU_GPS_CFG_SET_RSP:
    case PU_GPS_CFG_GET_RSP:
    case PU_GPS_SS_SET_RSP:
    case PU_GPS_SS_GET_RSP:

    case PU_RECORD_START_RSP:
    case PU_RECORD_STOP_RSP:
    case PU_RECORD_DEL_RSP:
    case PU_TRANSDATA_CFG_SET_RSP:
    case PU_TRANSDATA_CFG_GET_RSP:
    case PU_LOGO_BMPOSD_TRANSMIT_RSP:
	case PU_LONG_OSD_SET_RSP:
	case PU_LONG_OSD_GET_RSP:
	//{{{{{{{{{{{{{{{ add by Royan 20140418 多行字幕
	case PU_MULTI_OSD_SET_RSP:
	case PU_MULTI_OSD_GET_RSP:
	//}}}}}}}}}}}}}}} add by Royan 20140418 多行字幕
    case PU_AUD_ENC_PARAM_SET_RSP:
    case PU_AUD_ENC_PARAM_GET_RSP:
	case PU_SNAPSHOT_CFG_GET_RSP:
	case PU_SNAPSHOT_CFG_SET_RSP:

	case PU_SERIALPARAM_CFG_GET_RSP:			
	case PU_SERIALPARAM_CFG_SET_RSP:





        //{{交换相关
    case VTDU_ADDR_ASSIGN_RSP:
    case VTDU_ADDSWITCH_RSP:
    case VTDU_REMOVESWITCH_RSP:
    case VTDU_PUNCH_RESULT_NTF_RSP:

    case INVITE_RSP:
    case INVITE_ACK:
    case OSP_SIP_BYE_REQ:
    case BYE_REQ:
    case OSP_SIP_BYE_RSP:
    case BYE_RSP:
    case INVITE_THIRD_PARTY_CALL_RSP:
    case INVITE_THIRD_PARTY_CALL_ACK:
    case START_THIRD_PARTY_CALL_RSP:
    case STOP_THIRD_PARTY_CALL_RSP:
    case END_OF_FILE_NTF_REQ:
    case KEY_FRAME_RSP:
    //更新交换地址
	case UPDATE_SWITCH_ADDRESS_REQ:
	case UPDATE_SWITCH_ADDRESS_RSP:

    case VCR_CTRL_REQ:
    //case PLAYBACK_PROGRESS_QUERY_REQ:
    case VCR_CTRL_RSP:
    //case PLAYBACK_PROGRESS_QUERY_RSP:

    case PLAYBACK_PROGRESS_NTF_REQ:
    case PLAYBACK_PROGRESS_NTF_RSP:
        //}}交换相关

    //cmu登录其他服务器应答
    case DEV_LOGIN_RSP:

    //cmu写日志应答
    case UAS_DEVICE_LOG_ADD_RSP:
    case UAS_ALARM_LOG_ADD_RSP:
	case BATCH_OPERATE_RSP:

    //前端图片抓拍
    case PU_PIC_SNAPSHOT_RSP:
    case PU_PIC_DOWNLOAD_RSP:
    case PU_PIC_QRY_RSP:
    case PU_PIC_QRY_NTF_RSP:

    case PU_TRANSPARENT_QRY_RSP:
    case PU_TRANSPARENT_QRY_NTF_RSP:

    //前端录像日历查询
    case PU_REC_DAYS_QRY_RSP:

    //获取服务模块地址信息
    case NETADDR_QUERY_RSP:

    //STUN地址更新
    case STUN_UPDATE_RSP:

    //域断链通知
    case CMU_DISCONNECT_NTF_RSP:

	//{{{{{{{{{{{{{{{ add by Royan 20140514 智能告警主机
	case DOMAIN_ALARM_RSP:
	//}}}}}}}}}}}}}}} add by Royan 20140514 智能告警主机

    case OSP_SIP_DISCONNECT:
    case OSP_SIP_MSG_PROC_FAIL:
    case OSP_SIP_DIALOG_TERMINATE:
	case  CMU_PU_OFFLINEDEV_QRY_RSP:
	case  CMU_PU_OFFLINEDEV_QRY_NTF_RSP:
	case INVITE_DECORDER_CALL_ACK:
	case INVITE_DECORDER_CALL_RSP:

	case MAPPING_TABLE_NTF_RSP:
	case TAS_DEVICE_CAP_INDEX_NAME_SET_RSP:

	case TAS_VIDSRC_INFO_QRY_RSP:
        {
            COspSipMsg* pSipMsg = (COspSipMsg*)pcMsg->content;
            if (pSipMsg == NULL)
            {
                INSTLOG(CMU_MOD_INS, ERROR_LEV, "ospsip 消息为空\n");
                return;
            }

            CTask* pTask = FindTask(pSipMsg->GetOspTaskNO());
            if (pTask != NULL)
            {
                ProcTaskMsg(pTask, pcMsg);
            }
            else
            {
                INSTLOG(CMU_MOD_INS, ERROR_LEV, "taskNO[%u]没有对应事务---msg[%s-%u]\n", 
                    pSipMsg->GetOspTaskNO(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

				//INVITE的应答如果没有对应的task，提取出SipDLg手动释放资源，以防止资源泄漏。
				if ((INVITE_RSP == pcMsg->event) || (INVITE_ACK == pcMsg->event))
				{
					OspSipReleaseDlg(pSipMsg->GetSipDlgID());
				}
			}
        }
        break;

    //3as消息处理
    case TAS_DEVICE_ADD_NTF_REQ:
    case TAS_DEVICE_DEL_NTF_REQ:
    case TAS_DEVICE_MOD_NTF_REQ:
    case TAS_LOGIN_RSP:
    case TAS_DEVICE_QRY_RSP:
    case TAS_DEVICE_QRY_NTF_REQ:   
        {
            if (g_pcRegTasTask != NULL)
            {
                ProcTaskMsg(g_pcRegTasTask, pcMsg);
            }
            else
            {
                INSTLOG(CMU_MOD_INS, ERROR_LEV, "g_pcRegTasTask == NULL，event[%s-%d]\n",
                    OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            } 
        }
        break;

    case CMU_DOMAIN_REDIS_SYNC:
    case CMU_SLIDING_WINDOW_PROC:
        {
            u32 *pdwTaskNO = (u32 *)pcMsg->content;
            if (NULL != pdwTaskNO)
            {
                CTask *pcTask = FindTask(*pdwTaskNO);
                if (NULL != pcTask)
                {
                    ProcTaskMsg(pcTask, pcMsg);
                }
                else
                {
                    INSTLOG(CMU_MOD_INS, ERROR_LEV, "pcTask = NULL, event[%s-%d], TaskNO[%u]\n",
                        OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event, *pdwTaskNO);
                }
            }
        }
        break;

    case OSP_SIP_SIGNAL_RELOAD_CFG:
        {
            INSTLOG(CMU_MOD_INS, CRITICAL_LEV, "收到公共配置变更信号，重新加载\n");
            ReadCommonConfig();

            CCommonConfig cCmCfg = GetCommonConfig();

            bool bIsStunMod = false;
            if (cCmCfg.GetStunIP() != g_cCmuConf.m_cCmCfg.GetStunIP())
            {
                bIsStunMod = true;              
            }

            if (cCmCfg.GetStunPort() != g_cCmuConf.m_cCmCfg.GetStunPort())
            {
                bIsStunMod = true;
            }

            g_cCmuConf.m_cCmCfg = cCmCfg;

            //stun服务器的地址有变更，通知:vtdu、pui、cui、rcs
            if (bIsStunMod)
            {
                UpdateDevListStunAddr(g_cVtduList);
                UpdateDevListStunAddr(g_cPuiList);
                UpdateDevListStunAddr(g_cCuiList);
                UpdateDevStunAddr(g_cCmuConf.m_tLocalUri.GetRcsURI());

                //更新cmu自己
                TNatAddr tStunAddr;
                strncpy(tStunAddr.m_achIP, cCmCfg.GetStunIP().c_str(), sizeof(tStunAddr.m_achIP));
                tStunAddr.m_achIP[sizeof(tStunAddr.m_achIP) - 1] = '\0';
                tStunAddr.m_wPort = cCmCfg.GetStunPort();
                
                NAUpdateStunAddr(&tStunAddr);
            }
        }
        break;
    case BATCH_PU_REG_REQ:
        {
            COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
            CEventReq cReq;
            pcOspSipMsg->GetMsgBody(cReq);

            CPuiRegTask *pcPuiTask = dynamic_cast<CPuiRegTask *>(GetLoginSession(cReq.GetSession()));
            if (NULL != pcPuiTask)
            {
                ProcTaskMsg(pcPuiTask->GetBatchRegTask(), pcMsg);
            }
            else
            {
                CBatchPuRegRsp cRsp;
                cRsp.SetHeadFromReq(cReq);
                cRsp.SetErrorCode(ERR_INVALID_SESSIONID);
                TOspTransID tOspTrasnID;
                tOspTrasnID.dwIID = GetOspIID();
                OspSipPostRsp(pcOspSipMsg->GetSipRsp(), pcOspSipMsg->GetSipTransID(), cRsp, tOspTrasnID);
            }
        }
        break;
    //PROXY处理
    case OSP_PROXY_DISCONNECT:
        {
            ProxyDisProc();
        }
        break;

    case OSP_PROXY_CONNECT:
        {
            ProxyConnectProc();
        }
        break;
        
    default:
        {
            INSTLOG(CMU_MOD_INS, WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }
}

void CCmuCoreInst::ProcTaskMsg(CTask* pTask, CMessage* const pcMsg)
{
    if (pTask == NULL)
    {
        INSTLOG(CMU_MOD_INS, ERROR_LEV, "pTask == NULL---msg[%s-%u]\n", 
            OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        return;
    }

    u32 dwRet = pTask->ProcMsg(pcMsg);
    if (dwRet == PROCMSG_DEL)
    {
        INSTLOG(CMU_MOD_INS, ALL_LEV, "事务[%s-%u]处理后需要删除---msg[%s-%u]\n", 
            pTask->GetObjName(), pTask->GetTaskNO(),
            OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

        delete pTask;
        pTask = NULL;
    }
    else if (dwRet == PROCMSG_FAIL)
    {
        INSTLOG(CMU_MOD_INS, ERROR_LEV, "事务[%s-%u]处理失败---msg[%s-%u]\n", 
            pTask->GetObjName(), pTask->GetTaskNO(),
            OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
    }
    else if (dwRet == PROCMSG_OK)
    {
        INSTLOG(CMU_MOD_INS, ALL_LEV, "事务[%s-%u]处理成功---msg[%s-%u]\n", 
            pTask->GetObjName(), pTask->GetTaskNO(),
            OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
    }
    else
    {
        INSTLOG(CMU_MOD_INS, ERROR_LEV, "事务[%s-%u]处理结果未知---msg[%s-%u]\n", 
            pTask->GetObjName(), pTask->GetTaskNO(),
            OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
    }
}

void CCmuCoreInst::RegSipProxy(s32 nExpires)
{
    OspSipRegProxy(nExpires);
}

void CCmuCoreInst::CmuRegTas()
{
    if (g_pcRegTasTask == NULL)
    {
        CRegTasTask* pRegTasTask = new CRegTasTask(this);
        pRegTasTask->InitStateMachine();
        pRegTasTask->SetUserName(g_cCmuConf.m_tLocalUri.GetURIString());
        pRegTasTask->SetTasUri(g_cCmuConf.m_t3asUri);
        pRegTasTask->StartRegTas();

        g_pcRegTasTask = pRegTasTask;
    }
}

//Tas断链处理
void CCmuCoreInst::TasDisProc(CMessage* const pcMsg)
{

}

void CCmuCoreInst::CmuLoginServer(const string& strServerUri)
{
    //本地地址列表
    TNetAddrList tLocalAddrList;
    TNetAddr tLocalAddr;
    tLocalAddr.SetNetIp(g_cCmuConf.m_tLocalAddr.GetIpStr());
    tLocalAddr.SetNetPort(g_cCmuConf.m_tLocalAddr.wPort);
    tLocalAddrList.push_back(tLocalAddr);

    //登录
    CLoginSenderTask* pcCmuLoginServerTask = new CLoginSenderTask(this);
    pcCmuLoginServerTask->InitStateMachine();

    pcCmuLoginServerTask->SetServerUri(strServerUri);
    pcCmuLoginServerTask->SetDevType(UNIQUE_SIP_UA_CMU);
    pcCmuLoginServerTask->SetDevUri(g_cCmuConf.m_tLocalUri.GetURIString());
    pcCmuLoginServerTask->SetDevAddrList(tLocalAddrList);
    pcCmuLoginServerTask->StartLogin();

    g_cGeneralLoginList.Insert(strServerUri, pcCmuLoginServerTask);
}

//proxy断链处理
void CCmuCoreInst::ProxyDisProc()
{
    g_tCmuStat.bIsRegSipProxy = false;

    TNmsAlarm tAlarm;
    tAlarm.byAlarmStatus = TNM_STATUS_ALARM;
    tAlarm.dwAlarmCode = TNM_ALARM_CODE_MODULE_DISCONNECT;
    tAlarm.strModuleId = g_cCmuConf.m_tProxyUri.GetURIString();
    tAlarm.strModuleName = DEV_TYPE_PROXY;
    tAlarm.strModuleIp = g_cCmuConf.m_tProxyAddr.GetIpStr();

    SendAlarmToNms(tAlarm);
}

//proxy上线处理
void CCmuCoreInst::ProxyConnectProc()
{
    g_tCmuStat.bIsRegSipProxy = true;
   GetUasLogTask()->AddDeviceLog(GetSipproxyLogInfo(true));

    TNmsAlarm tAlarm;
    tAlarm.byAlarmStatus = TNM_STATUS_NORMAL;
    tAlarm.dwAlarmCode = TNM_ALARM_CODE_MODULE_DISCONNECT;
    tAlarm.strModuleId = g_cCmuConf.m_tProxyUri.GetURIString();
    tAlarm.strModuleName = DEV_TYPE_PROXY;
    tAlarm.strModuleIp = g_cCmuConf.m_tProxyAddr.GetIpStr();

    SendAlarmToNms(tAlarm);
}

//事务创建
CTask* CCmuCoreInst::CreateTask(u16 wEvent)
{
    CTask* pTask = NULL;

    switch(wEvent)
    {
    case PUI_REG_REQ:
        {
            pTask = new CPuiRegTask(this);
        }
        break;

    case CUI_REG_REQ:
        {
            pTask = new CCuiRegTask(this);
        }
        break;

    case PU_REG_REQ:
        {
            pTask = new CPuRegTask(this);
        }
        break;

    case CU_LOGIN_REQ:
        {
            pTask = new CCuLoginTask(this);
        }
        break;

    case CATALOG_STATUS_SS_REQ:
        {
            pTask = new CCatalogSubscribeTask(this);
        }
        break;

    case PULIST_STATUS_SS_REQ:
        {
            pTask = new CPuListSubscribeTask(this);
        }
        break;

    case PU_STATUS_SS_REQ:
        {
            pTask = new CPuSubscribeTask(this);
        }
        break;

    case PU_SYS_PARAM_GET_REQ:
        {
            pTask = new CGetPuSysParamTask(this);
        }
        break;

    case PU_SYS_PARAM_SET_REQ:
        {
            pTask = new CSetPuSysParamTask(this);
        }
        break;

    case PU_SAVE_CFG_REQ:
        {
            pTask = new CSavePuCfgTask(this);
        }
        break;

    case PU_VID_ENC_PARAM_GET_REQ:
        {
            pTask = new CGetPuVidEncParamTask(this);
        }
        break;

    case PU_VID_ENC_PARAM_SET_REQ:
        {
            pTask = new CSetPuVidEncParamTask(this);
        }
        break;

    case CMU_CONNECT_REQ:
        {
            pTask = new CCmuPassiveConnecterTask(this);
        }
        break;

    case PU_PTZ_CTRL_REQ:
        {
            pTask = new CPuPtzCtrlTask(this);
        }
        break;

    case VTDU_REG_REQ:
        {
            pTask = new CVtduRegTask(this);
        }
        break;

        //{{交换相关
    case INVITE_REQ:
        {
            pTask = new CSwitchEntryTask(this);
        }
        break;
    case INVITE_THIRD_PARTY_CALL_REQ:
        {
            pTask = new C3PCInviteEntryTask(this);
        }
        break;
    case START_THIRD_PARTY_CALL_REQ:
        {
            pTask = new C3PCMsgEntryTask(this);
        }
        break;
    case STOP_THIRD_PARTY_CALL_REQ:
        {
            pTask = new C3PCMsgEntryTask(this);
        }
        break;
        //}}交换相关

    case DEV_LOGIN_REQ:
        {
            pTask = new CLoginRecverTask(this);
        }
        break;

    case PU_IO_OUT_REQ:
        {
            pTask = new CPuIoOutTask(this);
        }
        break;

    case PU_READ_SEC_REQ:
        {
            pTask = new CPuReadSecTask(this);
        }
        break;

	case PU_ALARM_RESET_REQ:
		{
			pTask = new CPuAlarmResetTask(this);
		}
		break;

	case PU_CAMERA_TYPE_SET_REQ:
		{
			pTask = new CSetPuCameraTypeTask(this);
		}
		break;

    case PU_MOTION_DETECT_GET_REQ:
        {
            pTask = new CGetPuMotionDetectTask(this);
        }
        break;

    case PU_MOTION_DETECT_SET_REQ:
        {
            pTask = new CSetPuMotionDetectTask(this);
        }
        break;

    case PU_AREA_SHADE_GET_REQ:
        {
            pTask = new CGetPuAreaShadeTask(this);
        }
        break;

    case PU_AREA_SHADE_SET_REQ:
        {
            pTask = new CSetPuAreaShadeTask(this);
        }
        break;

    case PU_REC_QRY_REQ:
        {
            pTask = new CPuRecordQryTask(this);
        }
        break;

	case PU_DEC_STYLE_SET_REQ:
		{
			pTask = new CSetPuDecStyleTask(this);
		}
		break;

	case PU_DEC_STYLE_GET_REQ:
		{
			pTask = new CGetPuDecStyleTask(this);
		}
		break;

	case PU_VIDSRC_OSD_SET_REQ:
		{
			pTask = new CSetPuVidSrcOsdTask(this);
		}
		break;

	case PU_VIDSRC_OSD_GET_REQ:
		{
			pTask = new CGetPuVidSrcOsdTask(this);
		}
		break;

    case PU_VSIP_OPERATE_REQ:
        {
            pTask = new CPuVsipOperateTask(this);
        }
        break;

    case KEY_FRAME_REQ:
        {
            pTask = new CKeyFrameTask(this);
        }
        break;

    case PU_TRANSPARENT_OPERATE_REQ:
        {
            pTask = new CPuTransparentOperateTask(this);
        }
        break;   

    case PU_PIC_QRY_REQ:
        {
            pTask = new CPuPictureQryTask(this);
        }
        break;

    case PU_TRANSPARENT_QRY_REQ:
        {
            pTask = new CPuTransparentQryTask(this);
        }
        break;

    case PU_PIC_SNAPSHOT_REQ:
        {
            pTask = new CPuPicSnapshotTask(this);
        }
        break;  

    case PU_PIC_DOWNLOAD_REQ:
        {
            pTask = new CPuPicDownloadTask(this);
        }
        break;

    case PU_REC_DAYS_QRY_REQ:
        {
            pTask = new CPuRecDaysQryTask(this);
        }
        break;

    //GPS相关
    case PU_GPS_CFG_SET_REQ:
        {
            pTask = new CPuGpsCfgSetTask(this);
        }
        break;

    case PU_GPS_CFG_GET_REQ:
        {
            pTask = new CPuGpsCfgGetTask(this);
        }
        break;

    case PU_GPS_SS_SET_REQ:
        {
            pTask = new CPuGpsSsSetTask(this);
        }
        break;

    case PU_GPS_SS_GET_REQ:
        {
            pTask = new CPuGpsSsGetTask(this);
        }
        break;

    //前端录像相关
    case PU_RECORD_START_REQ:
        {
            pTask = new CPuRecordStartTask(this);
        }
        break;

    case PU_RECORD_STOP_REQ:
        {
            pTask = new CPuRecordStopTask(this);
        }
        break;

    case PU_RECORD_DEL_REQ:
        {
            pTask = new CPuRecordDelTask(this);
        }
        break;

    //透明数据配置的设置、获取
    case PU_TRANSDATA_CFG_SET_REQ:
        {
            pTask = new CPuTransdataCfgSetTask(this);
        }
        break;

    case PU_TRANSDATA_CFG_GET_REQ:
        {
            pTask = new CPuTransdataCfgGetTask(this);
        }
        break;

    //传输台标
    case PU_LOGO_BMPOSD_TRANSMIT_REQ:
        {
            pTask = new CPuLogoBmpOsdTransmitTask(this);
        }
        break;

    //长字幕配置
    case PU_LONG_OSD_SET_REQ:
        {
            pTask = new CPuLongOsdSetTask(this);
        }
        break;

    case PU_LONG_OSD_GET_REQ:
        {
            pTask = new CPuLongOsdGetTask(this);
        }
        break;

	//{{{{{{{{{{{{{{{ add by Royan 20140418 多行字幕
    //多行字幕配置
    case PU_MULTI_OSD_SET_REQ:
        {
            pTask = new CPuMultiOsdSetTask(this);
        }
        break;

    case PU_MULTI_OSD_GET_REQ:
        {
            pTask = new CPuMultiOsdGetTask(this);
        }
        break;
	//}}}}}}}}}}}}}}} add by Royan 20140418 多行字幕

    //音频参数配置
    case PU_AUD_ENC_PARAM_SET_REQ:
        {
            pTask = new CPuAudEncParamSetTask(this);
        }
        break;

    case PU_AUD_ENC_PARAM_GET_REQ:
        {
            pTask = new CPuAudEncParamGetTask(this);
        }
        break;
	case PU_SNAPSHOT_CFG_GET_REQ:
		{
			pTask = new CPuSnapShotCfgGetTask(this);
		}
		break;
	case PU_SNAPSHOT_CFG_SET_REQ:
		{
			pTask = new CPuSnapShotCfgSetTask(this);
		}
		break;
	case PU_SERIALPARAM_CFG_GET_REQ:
		{
			pTask = new CPuSerialParamCfgGetTask(this);
		}
		break;
	case PU_SERIALPARAM_CFG_SET_REQ:
		{
			pTask = new CPuSerialParamCfgSetTask(this);
		}
		break;
	case CMU_PU_OFFLINEDEV_QRY_REQ:
		{
			pTask = new CGetOfflineDevTask(this);
		}
		break;
	case INVITE_DECORDER_CALL_REQ:
		{
			pTask = new CPuDecInviteTask(this);
		}
		break;
    default:
        {
            INSTLOG(CMU_MOD_INS, ERROR_LEV, "Msg[%s-%d]创建事务失败\n", 
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

//void CCmuCoreInst::WriteLogToUas(const TDeviceLogInfo& tLogInfo)
//{
//    //首先判断是否连接上uas
//    string strUasUri = g_cCmuConf.m_tLocalUri.GetUasURI().GetURIString();
//    CLoginSession* pcUasLoginTask = NULL;
//    g_cGeneralLoginList.Find(strUasUri, pcUasLoginTask);
//    if (pcUasLoginTask == NULL)
//    {
//        INSTLOG(CMU_MOD_UAS, WARNING_LEV, "cmu not login uas, can't write log to uas\n");
//        return;
//    }
//
//    if (pcUasLoginTask->GetState() != CLoginSenderTask::Service)
//    {
//        INSTLOG(CMU_MOD_UAS, WARNING_LEV, "cmu login uas not in service, can't write log to uas\n");
//        return;
//    }
//
//    //向uas写日志
//    CUasWriteDevLogTask* pcTask = new CUasWriteDevLogTask(this);
//    pcTask->InitStateMachine();
//
//    CDeviceLogAddReq cReq;
//    cReq.SetSession(pcUasLoginTask->GetSession());
//    cReq.SetInfo(tLogInfo);
//    pcTask->SetReq(cReq);
//    pcTask->SetDstUri(pcUasLoginTask->GetServerSipUri());
//    pcTask->StartWrite();
//
//    INSTLOG(CMU_MOD_UAS, EVENT_LEV, "send device log to uas\n");
//
//    //向nms发送平台设备的上下线告警
//    //enDeviceLogTypeOnline : enDeviceLogTypeOffline
//	TSipURI tUri(tLogInfo.devName);				// cmu和cmu之间的上下线日志，devName为cmu@xxx
//    if (tLogInfo.devName == DEV_TYPE_PUI
//        ||tLogInfo.devName == DEV_TYPE_CUI
//        ||tLogInfo.devName == DEV_TYPE_VTDU
//        ||tLogInfo.devName == DEV_TYPE_RCS
//        ||tLogInfo.devName == DEV_TYPE_TVS
//        ||tLogInfo.devName == DEV_TYPE_ALS
//        ||tLogInfo.devName == DEV_TYPE_MPS
//        ||tLogInfo.devName == DEV_TYPE_UAS
//        ||tLogInfo.devName == DEV_TYPE_3AS
//        ||tLogInfo.devName == DEV_TYPE_GBS
//        ||tLogInfo.devName == DEV_TYPE_CUI1
//        ||tLogInfo.devName == DEV_TYPE_GXX
//		||tLogInfo.devName == DEV_TYPE_DPS
//		||(tUri.IsValidURI() && tUri.GetUser() == DEV_TYPE_CMU))
//    {
//        TNmsAlarm tAlarm;
//        tAlarm.byAlarmStatus = (tLogInfo.logType==enDeviceLogTypeOffline) ? TNM_STATUS_ALARM : TNM_STATUS_NORMAL;
//        tAlarm.dwAlarmCode = TNM_ALARM_CODE_MODULE_DISCONNECT;
//        tAlarm.strModuleId = tLogInfo.devUri;
//        tAlarm.strModuleName = tLogInfo.devName;
//        tAlarm.strModuleIp = tLogInfo.netAddr;
//
//        SendAlarmToNms(tAlarm);
//    }
//}
//
//void CCmuCoreInst::WriteLogToUas(const TAlarmLogInfo& tLogInfo)
//{
//    //首先判断是否连接上uas
//    string strUasUri = g_cCmuConf.m_tLocalUri.GetUasURI().GetURIString();
//    CLoginSession* pcUasLoginTask = NULL;
//    g_cGeneralLoginList.Find(strUasUri, pcUasLoginTask);
//    if (pcUasLoginTask == NULL)
//    {
//        INSTLOG(CMU_MOD_UAS, WARNING_LEV, "cmu not login uas, can't write log to uas\n");
//        return;
//    }
//
//    if (pcUasLoginTask->GetState() != CLoginSenderTask::Service)
//    {
//        INSTLOG(CMU_MOD_UAS, WARNING_LEV, "cmu login uas not in service, can't write log to uas\n");
//        return;
//    }
//
//    //向uas写日志
//    CUasWriteAlarmLogTask* pcTask = new CUasWriteAlarmLogTask(this);
//    pcTask->InitStateMachine();
//
//    CAlarmLogAddReq cReq;
//    cReq.SetSession(pcUasLoginTask->GetSession());
//    cReq.SetInfo(tLogInfo);
//    pcTask->SetReq(cReq);
//    pcTask->SetDstUri(pcUasLoginTask->GetServerSipUri());
//    pcTask->StartWrite();
//
//    INSTLOG(CMU_MOD_UAS, EVENT_LEV, "send alarm log to uas\n");
//}
//
//void CCmuCoreInst::WriteLogToUas(const TOperateLogInfo& tLogInfo)
//{
//	//首先判断是否连接上uas
//	string strUasUri = g_cCmuConf.m_tLocalUri.GetUasURI().GetURIString();
//	CLoginSession* pcUasLoginTask = NULL;
//	g_cGeneralLoginList.Find(strUasUri, pcUasLoginTask);
//	if (pcUasLoginTask == NULL)
//	{
//		INSTLOG(CMU_MOD_UAS, WARNING_LEV, "cmu not login uas, can't write log to uas\n");
//		return;
//	}
//
//	if (pcUasLoginTask->GetState() != CLoginSenderTask::Service)
//	{
//		INSTLOG(CMU_MOD_UAS, WARNING_LEV, "cmu login uas not in service, can't write log to uas\n");
//		return;
//	}
//
//	//向uas写日志
//	CUasWriteUserOptLogTask* pcTask = new CUasWriteUserOptLogTask(this);
//	pcTask->InitStateMachine();
//
//	COperateLogAddReq cReq;
//	cReq.SetSession(pcUasLoginTask->GetSession());
//	cReq.SetInfo(tLogInfo);
//	pcTask->SetReq(cReq);
//	pcTask->SetDstUri(pcUasLoginTask->GetServerSipUri());
//	pcTask->StartWrite();
//
//	INSTLOG(CMU_MOD_UAS, EVENT_LEV, "send operate log to uas\n");
//}

TDeviceLogInfo CCmuCoreInst::GetSipproxyLogInfo(bool bOnOffState)
{
    TDeviceLogInfo tLogInfo;

    tLogInfo.devUri = g_cCmuConf.m_tProxyUri.GetURIString();

    tLogInfo.devName = DEV_TYPE_PROXY;
    tLogInfo.devAlias = DEV_TYPE_PROXY;

    tLogInfo.happenedTime = CCMSTime().ToString();
    tLogInfo.logType = (bOnOffState ? enDeviceLogTypeOnline : enDeviceLogTypeOffline);

    tLogInfo.netAddr = g_cCmuConf.m_tProxyAddr.GetIpStr();

    if (bOnOffState)
    {
        tLogInfo.logDesc = GetLanDesc(LAN_DESC_ONLINE);
    }
    else
    {
        tLogInfo.logDesc = GetLanDesc(LAN_DESC_OFFLINE);
    }

    return tLogInfo;
}

void CCmuCoreInst::UpdateDevStunAddr(const TSipURI& tDevUri)
{
    CStunUpdateTask* pcTask = new CStunUpdateTask(this);
    pcTask->InitStateMachine();

    TNetAddr tStunAddr;
    tStunAddr.SetNetIp(g_cCmuConf.m_cCmCfg.GetStunIP());
    tStunAddr.SetNetPort(g_cCmuConf.m_cCmCfg.GetStunPort());

    CStunUpdateReq cReq;
    cReq.SetStunAddr(tStunAddr);
    pcTask->SetReq(cReq);
    pcTask->SetDstUri(tDevUri);

    pcTask->StartReq();
}

void CCmuCoreInst::UpdateDevListStunAddr(const CStrMap<CLoginSession*>& tDevList)
{
    Iterator pPos;
    string strUri;
    CLoginSession* pTask = NULL;

    while (!pPos.End())
    {
        if (tDevList.Iterate(pPos, strUri, pTask) && pTask != NULL)
        {
            UpdateDevStunAddr(pTask->GetDevSipUri());
        }
    }
}

void CCmuCoreInst::SendAlarmToNms(const TNmsAlarm& tAlarm)
{
    if (g_pcNmsTask != NULL)
    {
        g_pcNmsTask->SendAlarm(tAlarm);
    }      
}

void CCmuCoreInst::BroadcastCmuDisNtf(const string& strDiscCmuUri)
{
    //通知pui(cmssdk), cui, rcs, tvs, mps, als
    SendCmuDisNtfToDevList(strDiscCmuUri, g_cPuiList);
    SendCmuDisNtfToDevList(strDiscCmuUri, g_cCuiList);
    SendCmuDisNtfToDev(strDiscCmuUri, g_cCmuConf.m_tLocalUri.GetRcsURI());
    SendCmuDisNtfToDev(strDiscCmuUri, g_cCmuConf.m_tLocalUri.GetTvsURI());
    SendCmuDisNtfToDev(strDiscCmuUri, g_cCmuConf.m_tLocalUri.GetMpsURI());
    SendCmuDisNtfToDev(strDiscCmuUri, g_cCmuConf.m_tLocalUri.GetAlsURI());
}

void CCmuCoreInst::SendCmuDisNtfToDev(const string& strDiscCmuUri, const TSipURI& tDevUri)
{
    CCmuDisconnectNtfTask* pcTask = new CCmuDisconnectNtfTask(this);
    pcTask->InitStateMachine();

    CCmuDisconnectNtfReq cReq;
    cReq.SetCmuUri(strDiscCmuUri);
    pcTask->SetReq(cReq);
    pcTask->SetDstUri(tDevUri);

    pcTask->StartReq();
}

void CCmuCoreInst::SendCmuDisNtfToDevList(const string& strDiscCmuUri, const CStrMap<CLoginSession*>& tDevList)
{
    Iterator pPos;
    string strUri;
    CLoginSession* pTask = NULL;

    while (!pPos.End())
    {
        if (tDevList.Iterate(pPos, strUri, pTask) && pTask != NULL)
        {
            SendCmuDisNtfToDev(strDiscCmuUri, pTask->GetDevSipUri());
        }
    }
}

//{{{{{{{{{{{{{{{ add by Royan 20140507 智能告警主机
//判断strDevUri是否包含“@”，有是Uri，没有是Puid
BOOL32 CCmuCoreInst::IsSipUri(const string& strDevUri)
{
	return (string::npos != strDevUri.find("@"));
}

BOOL32 CCmuCoreInst::GetLocalDomainSession(OUT string& strSess, IN const COspSipMsg *pSipMsg)
{
	BOOL32 bLocalDomain = TRUE;

	CPuAlarmInputNtyReq cReq;
	pSipMsg->GetMsgBody(cReq);
	const TAlarmInput& tAlarmInput = cReq.GetAlarm();

	TSipURI tSessUri;
	tSessUri.SetURIString(tAlarmInput.GetDevUri());
	
	TKDMNO tKdmNO;
	tKdmNO.SetNOString(tAlarmInput.GetDevUri().c_str());
	TKDMNO tDomainNO = tKdmNO.GetCmuNO();
    string strDomainName = DomainNO2DomainName(tDomainNO);

	if ((ALARM_TYPE_VIDEOPOLL == tAlarmInput.GetAlarmType() || ALARM_TYPE_INTELLIGENT == tAlarmInput.GetAlarmType()) 
        && FALSE == IsSipUri(tAlarmInput.GetDevUri()))
	{
		if (strDomainName.empty())
		{
			CPuAlarmInputNtyRsp cRsp;
			cRsp.SetHeadFromReq(cReq);
			cRsp.SetErrorCode(ERR_CMU_CMU_NOT_CONN);
			TOspTransID tOspTrasnID;
			tOspTrasnID.dwIID = GetOspIID();
			OspSipPostRsp(pSipMsg->GetSipRsp(), pSipMsg->GetSipTransID(), cRsp, tOspTrasnID, KDSIP_RESPONSE_200_OK);

			bLocalDomain = FALSE;

			INSTLOG(CMU_MOD_INS, ERROR_LEV, "GetLocalDomainSession 未找到Pu域信息, Sess[%s], AlarmDevUri[%s], DomainNO[%s], LocalDomain[%d]\n",
				strSess.c_str(),
				tAlarmInput.GetDevUri().c_str(),
				tDomainNO.GetNOStr(),
				bLocalDomain);

			return bLocalDomain;
		}

		if (g_cCmuConf.m_tLocalUri.GetDomain() == strDomainName)//本域
		{
			if (g_cPuIdUriMapList.end() != g_cPuIdUriMapList.find(tAlarmInput.GetDevUri()))
			{
				strSess = g_cPuIdUriMapList.find(tAlarmInput.GetDevUri())->second;
			}

			bLocalDomain = TRUE;
		}
		else//异域
		{
			bLocalDomain = FALSE;
			ProcDomainAlarm(strDomainName, pSipMsg);
		}
	}
	else if ((ALARM_TYPE_VIDEOPOLL == tAlarmInput.GetAlarmType() || ALARM_TYPE_INTELLIGENT == tAlarmInput.GetAlarmType()) 
        && TRUE == IsSipUri(tAlarmInput.GetDevUri()))
	{
		if (g_cCmuConf.m_tLocalUri.GetDomain() == tSessUri.GetDomain())//本域
		{
			strSess = tAlarmInput.GetDevUri();
			bLocalDomain = TRUE;
		}
		else//异域
		{
			bLocalDomain = FALSE;
			ProcDomainAlarm(tSessUri.GetDomain(), pSipMsg);
		}
	}
	else
	{
		strSess = GetSipSessionID(pSipMsg->GetMsgBody());
	}

	INSTLOG(CMU_MOD_INS, EVENT_LEV, "GetLocalDomainSession Sess[%s], AlarmDevUri[%s], AlarmType[%s], IsLocalDomain[%d], AlarmDomain[%s], KDMNO[%s], DomainNO[%s]\n",
		strSess.c_str(),
		tAlarmInput.GetDevUri().c_str(),
		tAlarmInput.GetAlarmType().c_str(),
		bLocalDomain,
		strDomainName.c_str(),
		tKdmNO.GetNOStr(),
		tDomainNO.GetNOStr());

	return bLocalDomain;
}

//处理异域智能告警
void CCmuCoreInst::ProcDomainAlarm(IN const string& strDomain, IN const COspSipMsg *pSipMsg)
{
	CCmuDomainAlarmTask* pTask = new CCmuDomainAlarmTask(this);
	if (NULL != pTask)
	{
		pTask->InitStateMachine();

		CPuAlarmInputNtyReq cNtyReq;
		pSipMsg->GetMsgBody(cNtyReq);

		CDomainAlarmReq cReq;
		TAlarmInput tAlarm = cNtyReq.GetAlarm();
		cReq.SetAlarm(tAlarm);
		pTask->SetReq(cReq);
		TSipURI tDomainCmuUri;
		tDomainCmuUri.SetUser(UNIQUE_SIP_UA_CMU);
		tDomainCmuUri.SetDomain(strDomain);
		pTask->SetDstUri(tDomainCmuUri);
		pTask->StartReq();

		INSTLOG(CMU_MOD_INS, EVENT_LEV, "ProcDomainAlarm 转发智能告警到DomainCmu[%s], AlarmDevUri[%s], AlarmType[%s]\n",
			tDomainCmuUri.GetURIString().c_str(),
			tAlarm.GetDevUri().c_str(),
			tAlarm.GetAlarmType().c_str());

		CPuAlarmInputNtyRsp cRsp;
		cRsp.SetHeadFromReq(cReq);
		cRsp.SetErrorCode(CMS_SUCCESS);
		TOspTransID tOspTrasnID;
		tOspTrasnID.dwIID = GetOspIID();
		OspSipPostRsp(pSipMsg->GetSipRsp(), pSipMsg->GetSipTransID(), cRsp, tOspTrasnID, KDSIP_RESPONSE_200_OK);

		INSTLOG(CMU_MOD_INS, EVENT_LEV, "ProcDomainAlarm 异域智能告警，直接应答PUI成功\n");
	}
	else
	{
		//这时候应该崩溃了。
		CMS_ASSERT(false && "ProcDomainAlarm new fail");
	}
}
//}}}}}}}}}}}}}}} add by Royan 20140507 智能告警主机

bool CCmuCoreInst::GetKdsipResInfo(OUT TKDSIP_DIALOG_RESOURCE& tInfo, IN EKDSIP_DIALOG_TYPE eType)
{

	if (OspSipGetKdsipResInfo(tInfo, eType))
	{
		return true;
	}

	return false;
}

void CCmuCoreInst::AddFlowCtrlJob(u32 dwTaskNO)
{
	if (NULL == m_pcFlowCtrlMngTask)
	{
		m_pcFlowCtrlMngTask = new CFlowCtrlMngTask(this);
		m_pcFlowCtrlMngTask->InitStateMachine();
	}

	m_pcFlowCtrlMngTask->RegistTask(dwTaskNO);
}

void CCmuCoreInst::DelFlowCtrlJob(u32 dwTaskNO)
{
	if (NULL != m_pcFlowCtrlMngTask)
	{
		m_pcFlowCtrlMngTask->EndTask(dwTaskNO);
	}
}

void CCmuCoreInst::ProcDomainTableNtf(CMessage* const pcMsg)
{
	COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
	CDomainTableNtfReq cReq ;
	pcOspSipMsg->GetMsgBody(cReq);

	INSTLOG(CMU_MOD_INS, ERROR_LEV, "[ProcDomainTableNtf] Domain Num: %d\n",
		cReq.GetDomainTable().size());

	for (TDomainTable::const_iterator it=cReq.GetDomainTable().begin(); it!=cReq.GetDomainTable().end(); it++)
	{
		if (string(DOMAIN_OP_TYPE_ADD) == it->second.GetOperateType())
		{
			g_tDomainTable[it->first] = it->second;
		} 
		else
		{
			g_tDomainTable.erase(it->first);
		}
	}

	//send rsp
	CDomainTableNtfRsp cRsp;
	TOspTransID tOspTransId;
	tOspTransId.dwIID = GetOspIID();
	OspSipPostRsp(pcOspSipMsg->GetSipRsp(), pcOspSipMsg->GetSipTransID(), cRsp, tOspTransId);
}

void CCmuCoreInst::ProcNotify(const CRedisPipe *pcRedisPipe, const string &strChn, const string &strData)
{
    if (IsLocalDomainByRedisPipe(pcRedisPipe))
    {
        if (FDB_CHN_TAS_DEV_CHANGE == strChn)
        {
            CTasDevChangeNtf cNtf;
            cNtf.FromXml(strData);

            ProcDeviceNotify(cNtf);

            return;
        }
        else if (FDB_CHN_TAS_DOMAIN_CHANGE == strChn)
        {
            CTasDomainChangeNtf cNtf;
            cNtf.FromXml(strData);

            ProcDomainNotify(cNtf);

            return;
        }

    }


    // 非3as的订阅通知
    {
        string  strDomainId;
        string  strDevId;
        TSsType tSsType;
        if (RedisGetCatalogSubTypeByChn(strChn, strDomainId, tSsType))          // 目录订阅下的设备状态通知
        {
            if (PU_ON_OFF_STATUS == tSsType)
            {
                CPuStatusNtyReq cReq;
                cReq.FromXml(strData);

                TSipURI tDevUri(cReq.GetDevURI());
                strDevId = tDevUri.GetUser();

                CPuData *pcPuData = GetPuData(strDomainId, strDevId);
                if (NULL != pcPuData)
                {
                    if (IsLocalDomainByDomainId(strDomainId))                  // 本级设备
                    {
                        // 本级设备的状态是由cmu维护的，不需要订阅，也不可能收到通知
                    }
                    else                                                        // 非本级设备需要将状态更新到本级平台的redis并发布
                    {
                        pcPuData->SetPuStatus(cReq.GetOnOffStatus());

                        pcPuData->PublishOnOff(cReq.GetOnOffStatus());
                    }
                }
            }
            else if (PU_CONFIG_STATUS == tSsType)
            {
                CPuConfigNtyReq cReq;
                cReq.FromXml(strData);

                TSipURI tDevUri(cReq.GetDevURI());
                strDevId = tDevUri.GetUser();

                CPuData *pcPuData = GetPuData(strDomainId, strDevId);
                if (NULL != pcPuData)
                {
                    if (IsLocalDomainByDomainId(strDomainId))
                    {
                        pcPuData->PublishSipConfig(cReq.GetConfig());

                        // 前端视频源名称变更
                        CPuRegTask *pcTask = dynamic_cast<CPuRegTask *>(pcPuData->GetPuRegTask());
                        if (NULL != pcTask)
                        {
                            CPuiRegTask *pcPuiTask = pcTask->GetPuiRegTask();
                            if (NULL != pcPuiTask && DEV_TYPE_GBU != pcPuiTask->GetDevType() && DEV_TYPE_GBS != pcPuiTask->GetDevType())
                            {
                                pcTask->UpdateVidSrcName2Tas(cReq.GetConfig().GetVidSrcCfgList());
                            }
                        }
                    }
                    else
                    {
                        pcPuData->MergePuConfig(cReq.GetConfig());

                        pcPuData->PublishConfig(cReq.GetConfig());
                    }
                }
            }
            else if (PU_SERVICE_STATUS == tSsType)
            {
                CPuServiceStatusNtyReq cReq;
                cReq.FromXml(strData);

                TSipURI tDevUri(cReq.GetDevURI());
                strDevId = tDevUri.GetUser();

                CPuData *pcPuData = GetPuData(strDomainId, strDevId);
                if (NULL != pcPuData)
                {
                    if (IsLocalDomainByDomainId(strDomainId))
                    {
                        pcPuData->PublishSipService(cReq.GetServiceStatus());
                    }
                    else
                    {
                        pcPuData->SetPuServiceStatus(cReq.GetServiceStatus());

                        pcPuData->PublishService(cReq.GetServiceStatus());
                    }
                }
            }
            else if (PU_ALARM_STATUS == tSsType)
            {
                CPuAlarmStatusNtyReq cReq;
                cReq.FromXml(strData);

                TSipURI tDevUri(cReq.GetDevURI());
                strDevId = tDevUri.GetUser();

                CPuData *pcPuData = GetPuData(strDomainId, strDevId);
                if (NULL != pcPuData)
                {
                    if (IsLocalDomainByDomainId(strDomainId))
                    {
                        CPuRegTask *pcTask = dynamic_cast<CPuRegTask *>(pcPuData->GetPuRegTask());
                        if (NULL != pcTask && pcTask->IsService())
                        {
							INSTLOG(CMU_MOD_INS, CRITICAL_LEV, "ALARM_LOG: CPuRegTask * got!\n");

                            CAlarmInputArray &tAlarmArray = cReq.GetAlarmStatus().GetAlarmArray();
                            for (CAlarmInputArray::iterator it = tAlarmArray.begin(); tAlarmArray.end() != it; ++it)
                            {
                                // 告警数据中，部分数据需要更新
                                pcTask->UpdateAlarm(*it);
                                pcTask->WriteAlarmLog2Uas(*it);
                            }
                        }
						else//pcPuData->GetPuRegTask() 为 CPuBatchRegTask * 的情况 2019/4/28 zhusenyong
						{
							INSTLOG(CMU_MOD_INS, CRITICAL_LEV, "ALARM_LOG: CPuBatchRegTask * got!\n");
							CAlarmInputArray &tAlarmArray = cReq.GetAlarmStatus().GetAlarmArray();
							for (CAlarmInputArray::iterator it = tAlarmArray.begin(); tAlarmArray.end() != it; ++it)
							{
								const TAlarmInput& tAlarmInput = *it;								
								TAlarmLogInfo tLogInfo;								
								tLogInfo.devUri = pcPuData->GetDevUri().GetURIString();								
								tLogInfo.devName = GetUTF8String(pcPuData->GetDevName());
								tLogInfo.devAlias = GetUTF8String(pcPuData->GetDevAlias());
								tLogInfo.alias = GetUTF8String(tAlarmInput.GetAlarmAlias());
								tLogInfo.channId = tAlarmInput.GetInputId();
								tLogInfo.type = tAlarmInput.GetAlarmType();;
								tLogInfo.flag = tAlarmInput.GetAlarmFlag();
								if (tAlarmInput.GetAlarmFlag() == ALARM_FLAG_OCCUR)
								{
									tLogInfo.occurTime = tAlarmInput.GetAlarmTime();
								}
								else
								{
									tLogInfo.resumeTime = tAlarmInput.GetAlarmTime();
								}
								GetUasLogTask()->AddAlarmLog(tLogInfo);
							}
						}
                        
                        pcPuData->PublishSipAlarm(cReq.GetAlarmStatus());
                    }
                    else
                    {
                        pcPuData->UpdateAlarmStatus(cReq.GetAlarmStatus());

                        pcPuData->PublishAlarm(cReq.GetAlarmStatus());
                    }
                }
            }
            else if (PU_GPS_STATUS == tSsType)
            {
                CPuGpsStatusNtyReq cReq;
                cReq.FromXml(strData);

                TSipURI tDevUri(cReq.GetDevURI());
                strDevId = tDevUri.GetUser();

                CPuData *pcPuData = GetPuData(strDomainId, strDevId);
                if (NULL != pcPuData)
                {
                    if (IsLocalDomainByDomainId(strDomainId))
                    {
                        pcPuData->SetPuGpsData(cReq.GetGpsData());

                        pcPuData->PublishSipGpsData(cReq.GetGpsData());
                    }
                    else
                    {
                        pcPuData->SetPuGpsData(cReq.GetGpsData());

                        pcPuData->PublishGpsData(cReq.GetGpsData());
                    }                    
                }
            }
            else if (PU_TRANSDATA_STATUS == tSsType)
            {
                CPuTransDataStatusNtyReq cReq;
                cReq.FromXml(strData);

                TSipURI tDevUri(cReq.GetDevURI());
                strDevId = tDevUri.GetUser();

                CPuData *pcPuData = GetPuData(strDomainId, strDevId);
                if (NULL != pcPuData)
                {
                    if (IsLocalDomainByDomainId(strDomainId))
                    {
                        pcPuData->PublishSipTransData(cReq.GetData());
                    }
                    else
                    {
                        pcPuData->SetPuTransparentData(cReq.GetData());

                        pcPuData->PublishTransData(cReq.GetData());
                    }
                }
            }
            else if (PU_DEV_CONFIG_NTF == tSsType)
            {
                CPuConfigNtyReq cReq;
                cReq.FromXml(strData);

                TSipURI tDevUri(cReq.GetDevURI());
                strDevId = tDevUri.GetUser();

                CPuData *pcPuData = GetPuData(strDomainId, strDevId);
                if (NULL != pcPuData)
                {
                    if (IsLocalDomainByDomainId(strDomainId))
                    {
                        // PuConfig中可能有通道状态信息，对于虚拟注册的设备，当通道状态变化时，需要更新其设备状态
                        CPuRegBaseTask *pcPuTask = pcPuData->GetPuRegTask();
                        if (NULL != pcPuTask)
                        {
                            CPuiRegTask *pcPuiTask = pcPuTask->GetPuiRegTask();
                            if (NULL != pcPuiTask && DEV_TYPE_PUI != pcPuiTask->GetDevType() && DEV_TYPE_PUI_GB != pcPuiTask->GetDevType())
                            {
                                CPuChnStateList *pcChnState = cReq.GetConfig().IsCompleted() ? &(cReq.GetConfig().GetChnStateCfgList()) : NULL;
                                if (pcPuData->GetPuIsOnline() && pcPuData->IsAllChnNotOnline(pcChnState))         // 原先在线，后来所有通道变为不在线
                                {
                                    pcPuData->PublishConfig(cReq.GetConfig());

                                    pcPuData->SetPuIsOnline(false);
                                    GetUasLogTask()->AddDeviceLog(GetDevLog(pcPuData));

                                    pcPuData->PublishOnOff();
                                }
                                else if (!pcPuData->GetPuIsOnline() && !pcPuData->IsAllChnNotOnline(pcChnState))  // 原先不在线，现在部分通道在线
                                {
                                    pcPuData->SetPuIsOnline(true);
                                    GetUasLogTask()->AddDeviceLog(GetDevLog(pcPuData));

                                    pcPuData->PublishOnOff();

                                    // 先发送了上下线通知后再发送PuConfig通知
                                    pcPuData->PublishConfig(cReq.GetConfig());

                                    // 设备重新上线后，强制发送GSP使能
                                    pcPuData->EnableGps();
                                }
                                else
                                {
                                    // 设备在线的话，则转换为PuConfig进行发布
                                    if (pcPuData->GetPuIsOnline())
                                    {
                                        pcPuData->PublishConfig(cReq.GetConfig());
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        else if (RedisGetSubTypeByChn(strChn, strDevId, tSsType))               // 列表订阅下的设备状态通知
        {
            strDomainId = GetDevDomainId(strDevId);

            if (PU_ON_OFF_STATUS == tSsType)
            {
                TPuStatus tPuStatus;
                tPuStatus.FromXml(strData);

                CPuData *pcPuData = GetPuData(strDomainId, strDevId);
                if (NULL != pcPuData)
                {
                    if (IsLocalDomainByDomainId(strDomainId))                   // 本级设备只要将状态发布给sip订阅者即可
                    {
                        // 本级设备的状态是由cmu维护的，不需要订阅，也不可能收到通知
                    }
                    else                                                        // 非本级设备需要将状态更新到本级平台的redis并发布
                    {
                        pcPuData->SetPuStatus(tPuStatus);

                        pcPuData->PublishOnOff(tPuStatus);
                    }
                }
            }
            else if (PU_CONFIG_STATUS == tSsType)
            {
                TPuDevConfig tPuDevConfig;
                tPuDevConfig.FromXml(strData);

                CPuData *pcPuData = GetPuData(strDomainId, strDevId);
                if (NULL != pcPuData)
                {
                    if (IsLocalDomainByDomainId(strDomainId))
                    {
                        pcPuData->PublishSipConfig(tPuDevConfig);

                        // 前端视频源名称变更
                        CPuRegTask *pcTask = dynamic_cast<CPuRegTask *>(pcPuData->GetPuRegTask());
                        if (NULL != pcTask)
                        {
                            CPuiRegTask *pcPuiTask = pcTask->GetPuiRegTask();
                            if (NULL != pcPuiTask && DEV_TYPE_GBU != pcPuiTask->GetDevType() && DEV_TYPE_GBS != pcPuiTask->GetDevType())
                            {
                                pcTask->UpdateVidSrcName2Tas(tPuDevConfig.GetVidSrcCfgList());
                            }
                        }
                    }
                    else
                    {
                        pcPuData->MergePuConfig(tPuDevConfig);

                        pcPuData->PublishConfig(tPuDevConfig);
                    }
                }
            }
            else if (PU_SERVICE_STATUS == tSsType)
            {
                TPuServiceStatus tPuServiceStatus;
                tPuServiceStatus.FromXml(strData);

                CPuData *pcPuData = GetPuData(strDomainId, strDevId);
                if (NULL != pcPuData)
                {
                    if (IsLocalDomainByDomainId(strDomainId))
                    {
                        pcPuData->PublishSipService(tPuServiceStatus);
                    }
                    else
                    {
                        pcPuData->SetPuServiceStatus(tPuServiceStatus);

                        pcPuData->PublishService(tPuServiceStatus);
                    }
                }
            }
            else if (PU_ALARM_STATUS == tSsType)
            {
                TAlarmInput tAlarmInput;
                tAlarmInput.FromXml(strData);

                CPuData *pcPuData = GetPuData(strDomainId, strDevId);
                if (NULL != pcPuData)
                {
                    if (IsLocalDomainByDomainId(strDomainId))
                    {
                        pcPuData->PublishSipAlarm(tAlarmInput);
                    }
                    else
                    {
                        pcPuData->UpdateAlarmInput(tAlarmInput);

                        pcPuData->PublishAlarm(tAlarmInput);
                    }
                }
            }
            else if (PU_GPS_STATUS == tSsType)
            {
                TGpsData tGpsData;
                tGpsData.FromXml(strData);

                CPuData *pcPuData = GetPuData(strDomainId, strDevId);
                if (NULL != pcPuData)
                {
                    if (IsLocalDomainByDomainId(strDomainId))
                    {
                        pcPuData->SetPuGpsData(tGpsData);

                        pcPuData->PublishSipGpsData(tGpsData);
                    }
                    else
                    {
                        pcPuData->SetPuGpsData(tGpsData);

                        pcPuData->PublishGpsData(tGpsData);
                    }                    
                }
            }
            else if (PU_TRANSDATA_STATUS == tSsType)
            {
                TTransparentData tTransData;
                tTransData.FromXml(strData);

                CPuData *pcPuData = GetPuData(strDomainId, strDevId);
                if (NULL != pcPuData)
                {
                    if (IsLocalDomainByDomainId(strDomainId))
                    {
                        pcPuData->PublishSipTransData(tTransData);
                    }
                    else
                    {
                        pcPuData->SetPuTransparentData(tTransData);

                        pcPuData->PublishTransData(tTransData);
                    }
                }
            }
        }
        else
        {
            // 错误的操作类型，暂时断言
            CMS_ASSERT(false && "unreachable...");
        }
    }
}

void CCmuCoreInst::ProcRdisSdkDisc(const CRedisPipe *pcRedisPipe, EFDB_StatusCode emCode)
{
    if (IsLocalDomainByRedisPipe(pcRedisPipe))
    {
        INSTLOG(ERROR_LEV, "cmu和本域redis连接断开，cmu即将退出重启!!!\n");

        // 本域的redis都连接不上了，任何业务都无法进行了，还是直接退出吧！！！
        cmuquit();
    }
    else
    {
        // 异域的redis连接不了，有几种可能：
        // 1、下级redis-server退出了，这样的话，下级的cmu也会退出，本级cmu和下级cmu断链重连
        // 2、本级cmu只是和下级redis-server断链，下级cmu和下级redis-server仍然连接着
        CDomainData *pcDomainData = GetDomainDataByRedisPipe(pcRedisPipe);
        if (NULL != pcDomainData)
        {
            INSTLOG(ERROR_LEV, "cmu和域[%s - %s]redis连接断开，cmu将重连该域的redis!!!\n", 
                pcDomainData->GetDomainName().c_str(), pcDomainData->GetDomainId().c_str());

            CCmuConnecterTask *pcConnectTask = pcDomainData->GetConnectTask();
            if (NULL != pcConnectTask)
            {
                pcConnectTask->StartSync();
            }
        }
    }
}

void CCmuCoreInst::ProcRdisSdkNotify(const CRedisPipe *pcRedisPipe, const CRedisSsChannel &cSsChn, const string &strContent)
{
    // 来自于redissdk的通知
    if (    RedisSsChannelType_Module_Status == cSsChn.GetType()
        ||  RedisSsChannelType_Module_Type_Status == cSsChn.GetType())
    {
        TRedisModuleStatus tModuleStatus;
        tModuleStatus.FromJson(strContent);

        if (    RedisModuleState_Restart== tModuleStatus.tState 
            ||  RedisModuleState_Stop   == tModuleStatus.tState)
        {
            CRedisLoginSession *pcTask = CRedisLoginSessionMng::GetInstance()->GetLoginSession(tModuleStatus.tId.strId);
            if (NULL != pcTask)
            {
                delete pcTask;
                pcTask = NULL;
            }
        }
    }
}

void CCmuCoreInst::ProcDeviceNotify(const CTasDevChangeNtf &cNtf)
{
    for (vector<TTasDevChangeItem>::const_iterator it = cNtf.Items.begin(); cNtf.Items.end() != it; ++it)
    {
        CDomainData *pcDomainData = GetDomainDataByDomainId(it->domainId);
        if (NULL != pcDomainData)
        {
            switch (it->oprType)
            {
            case 0:                 // 设备入网通知
                {
                    pcDomainData->GetPuList().AddPuData(it->devId);

                    if (!IsLocalDomainByDomainId(it->domainId))             // 非本域设备，需要同步
                    {
                        CCmuConnecterTask *pcConnectTask = pcDomainData->GetConnectTask();
                        if (NULL != pcConnectTask)
                        {
                            pcConnectTask->AddSync(it->devId);
                        }
                    }
                }
                break;
            case 1:                 // 设备退网通知
                {
                    pcDomainData->GetPuList().DelPuData(it->devId);
                }
                break;
            case 2:                 // 设备修改通知
                {
                    const set<TChangedField> &cFields = it->GetChangedFields();
                    if (cFields.end() != cFields.find(TCHANGED_FIELD_DEVMODEL)
                        || cFields.end() != cFields.find(TCHANGED_FIELD_DEVNETTYPE)
                        || cFields.end() != cFields.find(TCHANGED_FIELD_DEVUSEFLAG))        // 这几类数据变更后是需要强制设备下线的
                    {
                        pcDomainData->GetPuList().ModPuData(it->devId);
						CPuData *pcPuData = pcDomainData->GetPuList().GetPuData(it->devId);
						if (NULL != pcPuData)
						{
							pcPuData->PublishOnOff();
						}
                    }
                }
                break;
            default:
                {
                    // 错误的操作类型，暂时断言
                    CMS_ASSERT(false && "unreachable...");
                }
                break;
            }
        }
    }
}

void CCmuCoreInst::ProcDomainNotify(const CTasDomainChangeNtf &cNtf)
{
    switch (cNtf.oprType)
    {
    case 0:                 // 下级平台入网通知
        {
            CDomainData *pcDomainData = CCmuData::GetInstance()->AddDomainData(cNtf.domainId, CDomainData::DomainType_Child);
            if (NULL != pcDomainData)
            {
                // 连接到下级平台，获取设备状态信息
                pcDomainData->Connect();
            }
        }
        break;
    case 1:                 // 下级平台退网通知（设备由3as清除，cmu只处理自己这边的数据即可）
        {
            CCmuData::GetInstance()->DelDomainData(cNtf.domainId);
        }
        break;
    case 2:                 // 域信息修改通知（下级平台修改域名、本级平台修改域名、本级添加上级平台、本级退网上级平台）
        {
            // 下级平台修改域名，域名信息保存在redis中，cmu内部不用修改
            CDomainData *pcDomainData = GetDomainDataByDomainId(cNtf.domainId);
            if (NULL != pcDomainData)
            {
                string strDomainName;
                CCmuData::GetInstance()->GetLocalRedisPipe().GetDomainName(cNtf.domainId, strDomainName);
                if (GetLocalDomainData()->GetDomainId() == cNtf.domainId)
                {
                    if (pcDomainData->GetDomainName() != strDomainName) // 本级修改域名
                    {
                        // 更新缓存数据
                        pcDomainData->SetDomainName(strDomainName);
                    }
                    else    // 上级变更（增加、删除上级平台）
                    {
                        // ParentDomainId保存在redis中，所以无需额外的处理
                    }
                }
                else        // 下级平台修改域名
                {
                    // 下级修改域名之后，下级会重启，本级清理和下级相关事务，然后重新添加下级
                    if (pcDomainData->GetDomainName() != strDomainName)
                    {
                        vector<string> cPuIdList;
                        pcDomainData->GetPuList().GetPuIdList(cPuIdList);

                        CCmuData::GetInstance()->DelDomainData(cNtf.domainId);
                        pcDomainData = NULL;

                        CDomainData *pcDomainData = CCmuData::GetInstance()->AddDomainData(cNtf.domainId, CDomainData::DomainType_Child);
                        if (NULL != pcDomainData)
                        {
                            for (vector<string>::const_iterator it = cPuIdList.begin(); cPuIdList.end() != it; ++it)
                            {
                                pcDomainData->GetPuList().AddPuData(*it);
                            }

                            pcDomainData->Connect();
                        }
                    }
                }
            }
        }
        break;
    default:
        {
            // 错误的操作类型，暂时断言
            CMS_ASSERT(false && "unreachable...");
        }
        break;
    }
}

void CCmuCoreInst::ProcSwitchConfigSetReq(CMessage* const pcMsg)
{
	COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;

	CCfgSyncSwitchSetReq cReq ;
	pcOspSipMsg->GetMsgBody(cReq);

	bool bSyncVidSrcName2Plat = false;
	if (cReq.GetSwichSets(SYNC_PUSRCNAME_TO_PLAT,bSyncVidSrcName2Plat))
	{
		bool bOldSync = g_cCmuConf.m_bSyncVidSrcName2Plat;
		g_cCmuConf.UpdateSyncVidSrcName2PlatSwitch(bSyncVidSrcName2Plat);

		//前端视频源名称同步开关从关闭修改为开启，需要将缓存的视频源别名信息同步到3as
		if (!bOldSync && bSyncVidSrcName2Plat)
		{
			SyncVidSrcAlias2Plat();
		}
	}

	bool bDisplayAreaInfo = false;
	if (cReq.GetSwichSets(DISPLAY_AREA_INFO,bDisplayAreaInfo))
	{
		g_cCmuConf.UpdateDisplayAreaInfoSwitch(bDisplayAreaInfo);
	}

	CCfgSyncSwitchSetRsp cRsp;
	cRsp.SetHeadFromReq(cReq);
	cRsp.SetErrorCode(CMS_SUCCESS);

	TOspTransID tOspTransId;
	tOspTransId.dwIID = GetOspIID();
	OspSipPostRsp(pcOspSipMsg->GetSipRsp(), pcOspSipMsg->GetSipTransID(), cRsp, tOspTransId);
}

void CCmuCoreInst::ProcSwitchConfigGetReq(CMessage* const pcMsg)
{
	COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;

	CCfgSyncSwitchGetReq cReq ;
	pcOspSipMsg->GetMsgBody(cReq);

	CCfgSyncSwitchGetRsp cRsp;
	cRsp.SetHeadFromReq(cReq);
	cRsp.SetErrorCode(CMS_SUCCESS);
	cRsp.SetSwichSets(SYNC_PUSRCNAME_TO_PLAT,g_cCmuConf.m_bSyncVidSrcName2Plat);
	cRsp.SetSwichSets(DISPLAY_AREA_INFO,g_cCmuConf.m_bDisplayAreaInfo);

	TOspTransID tOspTransId;
	tOspTransId.dwIID = GetOspIID();
	OspSipPostRsp(pcOspSipMsg->GetSipRsp(), pcOspSipMsg->GetSipTransID(), cRsp, tOspTransId);
}

void CCmuCoreInst::SyncVidSrcAlias2Plat()
{
    CPuDataMap &cPuList = GetLocalDomainData()->GetPuList().GetPuDataMap();
    for (CPuDataMap::iterator it = cPuList.begin(); cPuList.end() != it; ++it)
    {
        CPuRegBaseTask *pcTask = it->second.GetPuRegTask();
        if (NULL != pcTask)
        {
            // 只有非GBS和非GBU接入的设备需要处理前端视频源别名同步
            CPuiRegTask *pcPuiTask = pcTask->GetPuiRegTask();
            if (NULL != pcPuiTask && DEV_TYPE_GBU != pcPuiTask->GetDevType() && DEV_TYPE_GBS != pcPuiTask->GetDevType())
            {
                AddFlowCtrlJob(pcTask->GetTaskNO());
            }
        }
    }
}

CUasLogTask* CCmuCoreInst::GetUasLogTask()
{
	if (NULL == g_pcUasLogProcTask)
	{
		g_pcUasLogProcTask = new CUasLogTask(this);
		g_pcUasLogProcTask->InitStateMachine();
	}

	return g_pcUasLogProcTask;
}

TDeviceLogInfo CCmuCoreInst::GetDevLog(CPuData *pcPuData)
{
	TDeviceLogInfo tLogInfo;

	tLogInfo.happenedTime = CCMSTime().ToString();

	if (NULL != pcPuData)
	{
		tLogInfo.devUri = pcPuData->GetDevUri().GetURIString();
		CDeviceInfo cDeviceInfo = pcPuData->GetDevInfo();
		tLogInfo.devName  = cDeviceInfo.GetDeviceName();
		tLogInfo.devAlias = cDeviceInfo.GetDeviceAlias();
		tLogInfo.netAddr = pcPuData->GetPuConnectAddr();
		tLogInfo.logType = (pcPuData->GetPuIsOnline() ? enDeviceLogTypeOnline : enDeviceLogTypeOffline);
		tLogInfo.logDesc = (pcPuData->GetPuIsOnline() ? GetLanDesc(LAN_DESC_ONLINE) : GetLanDesc(LAN_DESC_OFFLINE));
	}

	return tLogInfo;
}
bool CCmuCoreInst::IsUTF8(IN unsigned char *pBuf, IN int strLen)
{
	const unsigned char byMask6 = 0xFC;//11111100//6个字节
	const unsigned char byMask5 = 0xF8;//11111000//5个字节
	const unsigned char byMask4 = 0xF0;//11110000//4个字节
	const unsigned char byMask3 = 0xE0;//11100000//3个字节
	const unsigned char byMask2 = 0xC0;//11000000//2个字节
	const unsigned char byMask1 = 0x80;//10000000//小于此值的1个字节
	for (int i = 0; i < strLen; i++)
	{
		unsigned char uch = pBuf[i];//首字节
		if (uch == 0 && i < strLen - 1)
		{
			//不是结尾发现'\0'，那肯定不是utf8了
			return false;
		}
		if (uch < byMask1)
		{
			continue;
		}
		if ((uch & 0xC0) == 0x80)//首字节在 0x80 和0xc0之间，就是非法
		{
			return false;
		}
		for (int n = 2; n <= 6; n++)
		{
			//这个掩码表示的后面还有几个字节数
			int nNeedChar = 7 - n;
			//生成掩码
			unsigned char byMask = ((0xff >> n) << n);
			if (uch >= byMask)
			{
				//检测剩余字节的数量是否符合utf8规则
				if (strLen - i - 1 < nNeedChar)
				{
					return false;
				}
				//检测剩余字节数的数值是否符合utf8规则
				for (int k = 0; k < nNeedChar; k++)
				{
					//每个字节都必须符合 10xxxxxx 格式
					if ((pBuf[i + k + 1] & 0xC0) != 0x80)
					{
						return false;
					}
				}
				//都符合了
				i += nNeedChar;
				break;
			}
		}

	}
	return true;
}

string CCmuCoreInst::GetUTF8String(const string& strOri)
{
	bool bUTF8 = IsUTF8((u8 *)strOri.c_str(), strOri.length());
	return bUTF8 ? strOri : GBKToUTF8(strOri);;
}
