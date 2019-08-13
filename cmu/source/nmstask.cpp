
#include "cuiregtask.h"
#include "puiregtask.h"
#include "vtduregtask.h"
#include "culogintask.h"
#include "regtastask.h"
#include "devlogintask.h"
#include "inviteswitchtask.h"
#include "cmuconst.h"
#include "cmuconf.h"
#include "nmstask.h"
#include "cmudata.h"
#include "rediscomm.h"

extern TCmuStat g_tCmuStat;
extern CRegTasTask* g_pcRegTasTask;

CNmcLib g_cNmsLib;

void CNmsTask::InitStateMachine()
{
    CStateProc cServiceProc;
    cServiceProc.ProcMsg = (CTask::PFProcMsg)&CNmsTask::OnService;
    cServiceProc.ProcErrMsg = (CTask::PFProcMsg)&CNmsTask::OnService;
    cServiceProc.TimerPoll = (CTask::PFTimerPoll)&CNmsTask::OnServiceTimer;
    AddRuleProc(Service, cServiceProc);

    NextState(Service);
}

//״̬����
u32 CNmsTask::OnService(CMessage *const pcMsg)
{
    TASKLOG(CMU_MOD_NMS, WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
    return PROCMSG_FAIL;
}

/*
#define NODE_CMU_PFMINFO2 (KDMCONF_BGN2+1)      // ��Ϣ��: TTnmCmuPfmInfo2
#define NODE_CMU_ONLINE_MODULE (KDMCONF_BGN2+2) // ��Ϣ�壺u32 ģ����� �� TTnmModuleInfo����
#define NODE_RCS_PFMINFO (KDMCONF_BGN2+3)       // ��Ϣ��: TTnmRcsPfmInfo

typedef struct tagTTnmModuleInfo
{
tagTTnmModuleInfo()
{
memset(this, 0, sizeof(*this));
}

s8 name[64];     // ģ������
s8 address[20];  // ģ���ַ
s8 uri[64];      // ģ��uri
s8 reserved[4];  // �����ֶΣ����ڶ���
}PACKED TTnmModuleInfo;

typedef struct tagTnmCmuPfmInfo2
{
tagTnmCmuPfmInfo2()
{
memset(this, 0, sizeof(*this));
}

u32 switchNum;           // ת������·��
u32 switchTotalBitRate;  // ת��������
u32 onlineVideosourceNum;// ������ƵԴ����
u32 onlineCuNum;         // ����CU����
}PACKED TTnmCmuPfmInfo2;
*/


void GetTnmModuleInfo(vector<TTnmModuleInfo>& cModuleList, CStrMap<CLoginSession*>& tLoginList)
{
    Iterator pPos;
    string strUri;
    CLoginSession* pTask = NULL;

    while (!pPos.End())
    {
        if (tLoginList.Iterate(pPos, strUri, pTask))
        {
            if (pTask == NULL)
            {
                continue;
            }

            if (!pTask->IsService())
            {
                continue;
            }

            TTnmModuleInfo tModuleInfo;

            if (pTask->GetDevUri() == g_cCmuConf.m_tLocalUri.GetURIString())
            {
                //cmu��Ϊ�ͻ���
                strcpy(tModuleInfo.name, pTask->GetServerSipUri().GetUser().c_str());
				/*
				20181024 Royan nms��uri����
				֮ǰ�޸�Ϊuser����Ϊ��������64��Խ�������
				�����޸�Ϊ�ضϷ�ʽ������Ϊhzy��Ը���޸�pms��
				�����������63�ֽڻᱻ�ضϣ�pms�ĶԱȹ������ɻ�ʧ�ܡ�
				*/
                //strcpy(tModuleInfo.uri, pTask->GetServerSipUri().GetUser().c_str());
				strncpy(tModuleInfo.uri, pTask->GetServerUri().c_str(), TNM_URI_LEN-1);
				tModuleInfo.uri[TNM_URI_LEN-1] = 0x00;

                string strIp;
                const TNetAddrList&  tAddrList = pTask->GetServerAddrList();
                if (tAddrList.size() > 0)
                {
                    strIp = tAddrList.at(0).GetNetIp();
                }
                else
                {
                    strIp = "127.0.0.1";
                }
                strcpy(tModuleInfo.address, strIp.c_str());
                cModuleList.push_back(tModuleInfo);
            }
            else
            {
                //cmu��Ϊ�����
                strcpy(tModuleInfo.name, pTask->GetDevType().c_str());
				/*
				20181024 Royan nms��uri����
				֮ǰ�޸�Ϊuser����Ϊ��������64��Խ�������
				�����޸�Ϊ�ضϷ�ʽ������Ϊhzy��Ը���޸�pms��
				�����������63�ֽڻᱻ�ضϣ�pms�ĶԱȹ������ɻ�ʧ�ܡ�
				*/
                //strcpy(tModuleInfo.uri, TSipURI(pTask->GetDevUri()).GetUser().c_str());
				strncpy(tModuleInfo.uri, pTask->GetDevUri().c_str(), TNM_URI_LEN-1);
				tModuleInfo.uri[TNM_URI_LEN-1] = 0x00;
                

                string strIp;
                const TNetAddrList&  tAddrList = pTask->GetDevAddrList();
                if (tAddrList.size() > 0)
                {
                    strIp = tAddrList.at(0).GetNetIp();
                }
                else
                {
                    strIp = "127.0.0.1";
                }
                strcpy(tModuleInfo.address, strIp.c_str());
                cModuleList.push_back(tModuleInfo);
            }      
        }
    }
}

u32 CNmsTask::OnServiceTimer()
{
    if (GetCurStateHoldTime() > CMU_UPDATE_NMS_INFO_INTERVAL)
    {
        if (g_cNmsLib.HasConnectedNms())
        {
            u32 dwOnlineCuNum = 0;
            u32 dwOnlineVideosourceNum = 0;
            u32 dwSwitchNum = 0;           
            u32 dwSwitchTotalBitRate = 0;  

            dwOnlineCuNum = g_cCuList.GetSize();
            CDomainData *pcDomainData = CCmuData::GetInstance()->GetLocalDomainData();
            if (NULL != pcDomainData)
            {
                dwOnlineVideosourceNum = CCmuData::GetInstance()->GetOnlineVidSrcNum();
            }

            GetLocalRedisPipe().GetRedisSdk().GetPlatSwitchAmount(dwSwitchNum);
            
            //dwSwitchNum = g_cSwitchList.GetDstSwitchNum();
            dwSwitchTotalBitRate = 0;  //todo, ��μ���          
            
            //ͳ����Ϣ
            TTnmCmuPfmInfo2 tTnmCmuInfo;
            tTnmCmuInfo.onlineCuNum = dwOnlineCuNum;
            tTnmCmuInfo.onlineVideosourceNum = dwOnlineVideosourceNum;
            tTnmCmuInfo.switchNum = dwSwitchNum;
            tTnmCmuInfo.switchTotalBitRate = dwSwitchTotalBitRate;

            TASKLOG(CMU_MOD_NMS, TIMER_LEV, "send cmu info to nms:OnlineCuNum[%u]-OnlineVidSrcNum[%u]-SwitchNum[%u]\n", 
                dwOnlineCuNum, dwOnlineVideosourceNum, dwSwitchNum);

            //����ͳ����Ϣ
            g_cNmsLib.SendMsg(NODE_CMU_PFMINFO2, &tTnmCmuInfo, sizeof(tTnmCmuInfo));

            //���ظ澯
            CheckOverloadAlarm(TNM_ALARM_CODE_CU_TOO_MUCH, dwOnlineCuNum);
            CheckOverloadAlarm(TNM_ALARM_CODE_PU_TOO_MUCH, dwOnlineVideosourceNum);

            if (g_cVtduList.GetSize() > 0 && g_cVtduList.GetMaxSwitchNum() > 0)
            {
                //��vtdu����ע���ſ�ʼ����ת��·���澯
                CheckOverloadAlarm(TNM_ALARM_CODE_VTDU_SWITCH_TOO_MUCH, dwSwitchNum);
            }            

            //��������ģ����Ϣ
            vector<TTnmModuleInfo> cModuleList;

            //proxy
            if (g_tCmuStat.bIsRegSipProxy)
            {
                TTnmModuleInfo tModuleInfo;
                strcpy(tModuleInfo.name, DEV_TYPE_PROXY);
                strcpy(tModuleInfo.uri, g_cCmuConf.m_tProxyUri.GetURIString().c_str());
                strcpy(tModuleInfo.address, g_cCmuConf.m_tProxyAddr.GetIpStr().c_str());
                cModuleList.push_back(tModuleInfo);
            }
            
            //tas
            if (g_pcRegTasTask != NULL && g_pcRegTasTask->IsService())
            {
                TTnmModuleInfo tModuleInfo;
                strcpy(tModuleInfo.name, DEV_TYPE_3AS);
                strcpy(tModuleInfo.uri, g_pcRegTasTask->GetServerUri().c_str());

                string strIp;
                const TNetAddrList&  tAddrList = g_pcRegTasTask->GetServerAddrList();
                if (tAddrList.size() > 0)
                {
                    strIp = tAddrList.at(0).GetNetIp();
                }
                else
                {
                    strIp = "127.0.0.1";
                }
                strcpy(tModuleInfo.address, strIp.c_str());
                cModuleList.push_back(tModuleInfo);
            }

			//rcs��tvs��als��mps��uas
            GetTnmModuleInfo(cModuleList, g_cGeneralLoginList); 

            //pui��cui��vtdu
            GetTnmModuleInfo(cModuleList, g_cPuiList);
            GetTnmModuleInfo(cModuleList, g_cCuiList);
            GetTnmModuleInfo(cModuleList, g_cVtduList);                  

            u32 dwModuleNum = cModuleList.size();
            u16 dwBufLen = u16(sizeof(u32) + dwModuleNum*sizeof(TTnmModuleInfo));
            u8* pBuf = new u8[dwBufLen];
            u16 dwBufPos = 0;
            memcpy(pBuf+dwBufPos, &dwModuleNum, sizeof(dwModuleNum));
            dwBufPos += sizeof(dwModuleNum);
            for (u32 i=0; i<dwModuleNum; i++)
            {
                const TTnmModuleInfo& tModuleInfo = cModuleList.at(i);
                TASKLOG(CMU_MOD_NMS, TIMER_LEV, "send module info to nms:Name[%s]-Uri[%s]-IpAddr[%s]\n", 
                    tModuleInfo.name, tModuleInfo.uri, tModuleInfo.address);

                memcpy(pBuf+dwBufPos, &tModuleInfo, sizeof(tModuleInfo));
                dwBufPos += sizeof(tModuleInfo);
            }

            g_cNmsLib.SendMsg(NODE_CMU_ONLINE_MODULE, pBuf, dwBufLen);

            delete[] pBuf;
            pBuf = NULL;
        }
        else
        {
            TASKLOG(CMU_MOD_NMS, ERROR_LEV, "NMSδ����!\n");
        }

        //�л�״̬���ȴ���һ�θ���
        NextState(Service);
    }

    return TIMERPOLL_DONE;
}

/*
    #define TNM_ALARM_CODE_CU_TOO_MUCH               20001  // �澯���ݣ�TTnmDevAlarmBase
    #define TNM_ALARM_CODE_PU_TOO_MUCH               20002  // �澯���ݣ�TTnmDevAlarmBase
    #define TNM_ALARM_CODE_VTDU_SWITCH_TOO_MUCH      20003  // �澯���ݣ�TTnmDevAlarmBase
    #define TNM_ALARM_CODE_MODULE_DISCONNECT         20005  // �澯����: TnmAlarmCodeModuleDisconnect
    */
void CNmsTask::SendAlarm(const TNmsAlarm& tAlarm)
{  
    TASKLOG(CMU_MOD_NMS, TIMER_LEV, "alarmCode[%u], alartState[%u], moduleName[%s], moduleId[%s], moduleIp[%s]\n", 
        tAlarm.dwAlarmCode, tAlarm.byAlarmStatus, tAlarm.strModuleName.c_str(),
        tAlarm.strModuleId.c_str(), tAlarm.strModuleIp.c_str());

    if (!g_cNmsLib.HasConnectedNms())
    {
        TASKLOG(CMU_MOD_NMS, ERROR_LEV, "NMSδ����!\n");
        return;
    }

    if (tAlarm.dwAlarmCode == TNM_ALARM_CODE_MODULE_DISCONNECT)
    {
        TNmsAlarmSrc tAlarmSrc;
        tAlarmSrc.dwAlarmCode = tAlarm.dwAlarmCode;
        tAlarmSrc.strAlarmDev = tAlarm.strModuleId;
        
        TnmAlarmCodeModuleDisconnect tModDis;
        tModDis.tDevAlarm.dwAlarmCode = tAlarm.dwAlarmCode;       
        strncpy(tModDis.moduleName, tAlarm.strModuleName.c_str(), TNM_MODULE_NAME_LEN-1);
		tModDis.moduleName[TNM_MODULE_NAME_LEN-1] = '\0';
        strncpy(tModDis.moduleId, tAlarm.strModuleId.c_str(), TNM_MODULE_ID_LEN-1);
		tModDis.moduleId[TNM_MODULE_ID_LEN-1] = '\0';
        strncpy(tModDis.moduleIp, tAlarm.strModuleIp.c_str(), TNM_MODULE_IP_LEN-1);
		tModDis.moduleIp[TNM_MODULE_IP_LEN-1] = '\0';

        if (tAlarm.byAlarmStatus == TNM_STATUS_ALARM)
        {
            if (!m_cNmsDisAlarmMap.Exist(tAlarmSrc))
            {
                //�澯����
                tModDis.alarmStatus = TNM_STATUS_ALARM;
                //������ˮ��
                tModDis.tDevAlarm.dwDevSerialNo = m_cAlarmSNPoll.PopIndex();
                //����澯���ȴ��ָ�
                m_cNmsDisAlarmMap.Insert(tAlarmSrc, tModDis);

                TASKLOG(CMU_MOD_NMS, CRITICAL_LEV, "send dev disconnect alarm[alarmCode: %u, alarmSN: %u]"
                    " OCCUR to nms:devName[%s]-devUri[%s]-devIp[%s]\n", 
                    tAlarm.dwAlarmCode, tModDis.tDevAlarm.dwDevSerialNo, 
                    tModDis.moduleName, tModDis.moduleId, tModDis.moduleIp);

                tModDis.HostToNetBase();
                g_cNmsLib.SendMsg(TRAP_TNM_DEV_ALARM, &tModDis, sizeof(tModDis));

                
            }
        }
        else
        {
            TnmAlarmCodeModuleDisconnect* ptAlarm = m_cNmsDisAlarmMap.Find(tAlarmSrc);
            if (ptAlarm != NULL)
            {
                //�澯�ָ�
                tModDis.alarmStatus = TNM_STATUS_NORMAL;
                tModDis.tDevAlarm.dwDevSerialNo = ptAlarm->tDevAlarm.dwDevSerialNo;

                //������ˮ��
                m_cAlarmSNPoll.PushIndex((u16)ptAlarm->tDevAlarm.dwDevSerialNo);
                //ɾ���澯
                m_cNmsDisAlarmMap.Erase(tAlarmSrc);

                TASKLOG(CMU_MOD_NMS, CRITICAL_LEV, "send dev disconnect alarm[alarmCode: %u, alarmSN: %u]"
                    " RESTORE to nms:devName[%s]-devUri[%s]-devIp[%s]\n", 
                    tAlarm.dwAlarmCode, tModDis.tDevAlarm.dwDevSerialNo, 
                    tModDis.moduleName, tModDis.moduleId, tModDis.moduleIp);

                tModDis.HostToNetBase();
                g_cNmsLib.SendMsg(TRAP_TNM_DEV_ALARM, &tModDis, sizeof(tModDis));               
            }
        }
    }
    else
    {
        TASKLOG(CMU_MOD_NMS, WARNING_LEV, "unknown nsm alarmCode[%u]\n", tAlarm.dwAlarmCode);
    }   
}

void CNmsTask::CheckOverloadAlarm(u32 dwAlarmCode, u32 dwCurNum)
{
    u32 dwTotalNum = 0;
    double dwThreshold = double(g_cCmuConf.m_nOverloadThreshold)/double(100);
    u32 dwAlarmOccurNum = 0;
    if (dwAlarmCode == TNM_ALARM_CODE_CU_TOO_MUCH)
    {
        dwTotalNum = g_cCmuConf.m_nMaxAcceptCu;        
    }
    else if (dwAlarmCode == TNM_ALARM_CODE_PU_TOO_MUCH)
    {
        dwTotalNum = g_cCmuConf.m_nMaxAcceptPu;
    }
    else if (dwAlarmCode == TNM_ALARM_CODE_VTDU_SWITCH_TOO_MUCH)
    {
        dwTotalNum = g_cVtduList.GetMaxSwitchNum();
    }
    else
    {
        TASKLOG(CMU_MOD_NMS, WARNING_LEV, "unknown nsm alarmCode[%u]\n", dwAlarmCode);
        return;
    }

    dwAlarmOccurNum = u32(dwTotalNum * dwThreshold);

    if (!g_cNmsLib.HasConnectedNms())
    {
        TASKLOG(CMU_MOD_NMS, ERROR_LEV, "NMSδ����!\n");
        return;
    }

    TNmsAlarmSrc tAlarmSrc;
    tAlarmSrc.dwAlarmCode = dwAlarmCode;

    if (dwCurNum >= dwAlarmOccurNum)
    {      
        if (!m_cNmsOverloadAlarmMap.Exist(tAlarmSrc))
        {
            //�澯����
            TTnmDevAlarmBase tOverLoad;
            tOverLoad.tDevAlarm.dwAlarmCode = dwAlarmCode;
            tOverLoad.alarmStatus = TNM_STATUS_ALARM;

            //������ˮ��
            tOverLoad.tDevAlarm.dwDevSerialNo = m_cAlarmSNPoll.PopIndex();
            //����澯���ȴ��ָ�
            m_cNmsOverloadAlarmMap.Insert(tAlarmSrc, tOverLoad);

            tOverLoad.HostToNetBase();
            g_cNmsLib.SendMsg(TRAP_TNM_DEV_ALARM, &tOverLoad, sizeof(tOverLoad));

            TASKLOG(CMU_MOD_NMS, CRITICAL_LEV, "send overload alarm[alarmCode: %u, alarmSN: %u] OCCUR to nms:CurNum[%u]-TotalNum[%u]-ThresholdValue[%u]\n", 
                dwAlarmCode, tOverLoad.tDevAlarm.dwDevSerialNo, dwCurNum, dwTotalNum, dwAlarmOccurNum);
        }
    }
    else
    {
        TTnmDevAlarmBase* ptAlarm = m_cNmsOverloadAlarmMap.Find(tAlarmSrc);
        if (ptAlarm != NULL)
        {
            //�澯�ָ�
            TTnmDevAlarmBase tOverLoad;
            tOverLoad.tDevAlarm.dwAlarmCode = ptAlarm->tDevAlarm.dwAlarmCode;;
            tOverLoad.alarmStatus = TNM_STATUS_NORMAL;
            tOverLoad.tDevAlarm.dwDevSerialNo = ptAlarm->tDevAlarm.dwDevSerialNo;

            //������ˮ��
            m_cAlarmSNPoll.PushIndex((u16)ptAlarm->tDevAlarm.dwDevSerialNo);
            //ɾ���澯
            m_cNmsOverloadAlarmMap.Erase(tAlarmSrc);

            tOverLoad.HostToNetBase();
            g_cNmsLib.SendMsg(TRAP_TNM_DEV_ALARM, &tOverLoad, sizeof(tOverLoad));

            TASKLOG(CMU_MOD_NMS, CRITICAL_LEV, "send overload alarm[alarmCode: %u, alarmSN: %u] RESTORE to nms:CurNum[%u]-TotalNum[%u]-ThresholdValue[%u]\n", 
                dwAlarmCode, tOverLoad.tDevAlarm.dwDevSerialNo, dwCurNum, dwTotalNum, dwAlarmOccurNum);
        }
    }
}

void CNmsTask::PrintDisAlarm() const
{
    Iterator pPos;
    
    OspPrintf(TRUE, FALSE, "��ǰ�����澯��[%u]��\n", m_cNmsDisAlarmMap.GetSize());    
    u32 i = 0;

    while (!pPos.End())
    {
        TNmsAlarmSrc tKey;
        TnmAlarmCodeModuleDisconnect tValue;
        if (m_cNmsDisAlarmMap.Iterate(pPos, tKey, tValue))
        {
            i++;

            OspPrintf(TRUE, FALSE, "  %4u. disconnect alarm[alarmCode: %u, alarmSN: %u, alarmState: %u]"
                                   "  devName[%s]-devUri[%s]-devIp[%s]\n", 
                                   i, tValue.tDevAlarm.dwAlarmCode, tValue.tDevAlarm.dwDevSerialNo, tValue.alarmStatus,
                                   tValue.moduleName, tValue.moduleId, tValue.moduleIp);
        }
    }

    OspPrintf(TRUE, FALSE, "��ǰ�����澯��[%u]��\n", m_cNmsDisAlarmMap.GetSize());
}

void CNmsTask::PrintOverloadAlarm() const
{
    Iterator pPos;

    OspPrintf(TRUE, FALSE, "��ǰ���ظ澯��[%u]��\n", m_cNmsOverloadAlarmMap.GetSize());    
    u32 i = 0;

    while (!pPos.End())
    {
        TNmsAlarmSrc tKey;
        TTnmDevAlarmBase tValue;
        if (m_cNmsOverloadAlarmMap.Iterate(pPos, tKey, tValue))
        {
            i++;

            OspPrintf(TRUE, FALSE, "  %4u. overload alarm[alarmCode: %u, alarmSN: %u, alarmState: %u]\n", 
                                   i, tValue.tDevAlarm.dwAlarmCode, tValue.tDevAlarm.dwDevSerialNo, tValue.alarmStatus);
        }
    }

    OspPrintf(TRUE, FALSE, "��ǰ���ظ澯��[%u]��\n", m_cNmsOverloadAlarmMap.GetSize());
}