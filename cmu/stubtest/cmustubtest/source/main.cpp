#include "cms/ospext/ospext.h"
#include "cms/ospsip/ospsip.h"
#include "cms/natagent/natagent.h"
#include "cms/commonconfig/cmcfg.h"
#include "cbb/dataswitch/datadispatch.h"
#include "cmustubtestcoreapp.h"
#include "cmustubtestconf.h"
#include "cmustubtestdbg.h"
#include "cmustubtestdata.h"
#include "cuiregtasktest.h"
#include "culogintasktest.h"

extern CCuiRegTask* g_pcCuiRegCmuTask;
CXSemLock g_cMainSemLock;
extern u32 g_dwNALogLevel;

void OnLogPrintCallBack(EmLogLev emLogLev, const s8 *pchLog)
{
    if (emLogLev <= g_dwNALogLevel)
    {
        OspPrintf(TRUE, FALSE, (s8*)pchLog);
    }   
}

void OnQueryMappedAddrCallBack(EmResult emResult, u32 dwStunResultNum, TStunResult* ptStunResultList,
    u32 dwSeqNum, void* context)
{
    TStunResultParam tParam;
    tParam.m_emResult = emResult;
    tParam.m_dwResultNum = dwStunResultNum;
    tParam.m_dwSeqNum = dwSeqNum;
    tParam.m_dwTaskNO = (u32)context;
    if (emResult == emOK && dwStunResultNum > 0 && ptStunResultList != NULL)
    {
        GLOBELOG(EVENT_LEV, "QueryMappedAddr�ɹ���errcode[%d]\n", emResult);

        for (u32 i=0; i<dwStunResultNum && i<NATAGENT_MAX_NETSEG; i++)
        {
            tParam.m_atResultList[i] = ptStunResultList[i];
        }

        if (tParam.m_dwResultNum > NATAGENT_MAX_NETSEG)
        {
            tParam.m_dwResultNum = NATAGENT_MAX_NETSEG;
        }
    }
    else
    {
        GLOBELOG(ERROR_LEV, "QueryMappedAddrʧ�ܣ�errcode[%d]\n", emResult);
    }

    OspPost(CMU_STUB_TEST_CORE_DAEMON, (u16)CMU_STUN_QRY_ADDR_NTF, &tParam, sizeof(tParam));
}

void OnPunchResultCallBack(EmResult emResult, u32 dwPunchResultNum, TPunchResult* ptPunchResultList,
    u32 dwSeqNum, void* context)
{
    TPunchResultParam tParam;
    tParam.m_emResult = emResult;
    tParam.m_dwResultNum = dwPunchResultNum;
    tParam.m_dwSeqNum = dwSeqNum;
    tParam.m_dwTaskNO = (u32)context;
    if (emResult == emOK && dwPunchResultNum > 0 && ptPunchResultList != NULL)
    {
        for (u32 i=0; i<dwPunchResultNum && i<NATAGENT_MAX_PORT_NUM; i++)
        {
            TPunchResult tResult = ptPunchResultList[i];
            GLOBELOG(EVENT_LEV, "PunchResult:LocalAddr[%s:%d]<---->RemoteAddr[%s:%d], PunchKey[%s]\n",
                tResult.m_tLocalAddr.m_achIP, tResult.m_tLocalAddr.m_wPort,
                tResult.m_tRemoteAddr.m_achIP, tResult.m_tRemoteAddr.m_wPort,
                tResult.m_tPunchKey.m_achKey);    
            tParam.m_atResultList[i] = tResult;
        }

        if (tParam.m_dwResultNum > NATAGENT_MAX_PORT_NUM)
        {
            tParam.m_dwResultNum = NATAGENT_MAX_PORT_NUM;
        }
    }
    else
    {
        GLOBELOG(ERROR_LEV, "Punchʧ�ܣ�errcode[%d]\n", emResult);
    }

    OspPost(CMU_STUB_TEST_CORE_DAEMON, (u16)CMU_PUNCH_RESULT_NTF, &tParam, sizeof(tParam));
}

int main(int argc, char* argv[])
{
    //��ʼ��OSPEXT
    if (FALSE == OspExtInit())
    {
        printf("ospext��ʼ��ʧ��\n");
        return 0;
    }

    //��ȡcmu�����ļ�
    if (g_cCmuConf.ReadConf(cmu_config) == FALSE)
    {
        printf("cmu ��ȡ�����ļ�ʧ��\n");
        return 0;
    }

    //��ʼ��SIP
    TOspSipConf tOspSipConf;
    tOspSipConf.tLocalAddr = g_cCmuConf.m_tLocalAddr;
    tOspSipConf.tProxyAddr = g_cCmuConf.m_tProxyAddr;
    tOspSipConf.tLocalURI = g_cCmuConf.m_tLocalUri;
    tOspSipConf.dwDefaultOspIID = CMU_STUB_TEST_CORE_DAEMON;

    TSipStackConf tSipStackConf;
    tSipStackConf.ReadKeyCfg(cmu_config);
    if(!OspSipInit(tOspSipConf, tSipStackConf))
    {
        printf("InitOspSip fail!!!\n");
        return 0;
    }
    printf("OspSip��ʼ���ɹ�!\n");

    //��ʼ����Ϣ����
    InitCmuMsgDesc();

    //���ø�ģ���ӡ����
    OSP_SET_MOD_LOG(MOD_CUI, EVENT_LEV);
    OSP_SET_MOD_LOG(MOD_CU, EVENT_LEV);
    OSP_SET_MOD_LOG(MOD_SS, EVENT_LEV);
    OSP_SET_MOD_LOG(MOD_PTZ, EVENT_LEV);
    OSP_SET_MOD_LOG(MOD_ALS, EVENT_LEV);
    OSP_SET_MOD_LOG(MOD_TVS, EVENT_LEV);
    OSP_SET_MOD_LOG(MOD_SWITCH, EVENT_LEV);

    //����������OSP APP
    if (g_cCmuStubTestCoreApp.CreateOspApp("cmucore", CMU_STUB_TEST_CORE_APP_ID, 80) == FALSE)
    {
        printf("CreateOspApp fail!!!\n");
        return 0;
    }
    ::OspPost(CMU_STUB_TEST_CORE_DAEMON, CMU_START);

    printf("cmustubtest�����ɹ�!\n");
    GLOBELOG(CRITICAL_LEV, "cmustubtest�����ɹ�!\n");

#ifdef _LINUX_
    RegCmuCmd();
#endif

    logall(ERROR_LEV);
    kdsiplog(1, 1);
    dumploglev(DumpLog_None);
    //ospsiplog(4);
    //OspSetLogTimeLev(255);

    //natagent��ʼ��
    TInitInfo natInitInfo;
    NASetInitInfoToNULL(&natInitInfo);
    for(size_t i = 0; i < NATAGENT_MAX_NETSEG; i++)
    {
        memset(natInitInfo.m_achUnusedIP[i],0,NATAGENT_MAX_IP_LEN);
        memcpy(natInitInfo.m_achUnusedIP[i], g_cCmuConf.m_tLocalAddr.GetIpStr().c_str(),NATAGENT_MAX_IP_LEN);
    }

    memset(natInitInfo.m_tSTUNAddr.m_achIP, 0, NATAGENT_MAX_IP_LEN);
    memcpy(natInitInfo.m_tSTUNAddr.m_achIP, GetStunIPAddress().c_str(), GetStunIPAddress().size());    natInitInfo.m_tSTUNAddr.m_wPort = GetStunPort();    natInitInfo.m_fLogPrinter = &OnLogPrintCallBack;
    natInitInfo.m_fQueryMappedAddr = &OnQueryMappedAddrCallBack;
    natInitInfo.m_fPunchResult = &OnPunchResultCallBack;
    if( NATAGENT_OK != NAInitLib(&natInitInfo) )
    {
        printf("NAInitLib fail!!!\n");
        return 0;
    }

    //dd��ʼ��
    TDsInitInfo tInfo;
    tInfo.eLevel = RL_FULL_BUFFERING;
    if (DSOK != dsInit(tInfo))
    {
        printf("dsInit fail!!!\n");
        return 0;
    }

    //�Զ�ģ��cui��cu��¼������������pu״̬
     if (false)
    {
        OspDelay(3000);
        CApp* ptApp = (CApp*)(&g_cCmuStubTestCoreApp);
        CCmuStubTestCoreInst* pInst = (CCmuStubTestCoreInst*)ptApp->GetInstance(CInstance::DAEMON);       

        while(1)
        {
            if (pInst->CurState() == CCmuStubTestCoreInst::Service)
            {
                OspPrintf(TRUE, FALSE, "cmu����׮�������̬����ʼģ��cui��¼......\n");

                OspDelay(1000);

                cuion();
                OspDelay(500);
                while(1)
                {
                    if (g_pcCuiRegCmuTask != NULL && g_pcCuiRegCmuTask->GetState() == CCuiRegTask::Service)
                    {
                        OspPrintf(TRUE, FALSE, "ģ��cui��¼�ɹ�\n");
                        break;
                    }
                    else
                    {
                        OspDelay(500);
                    }              
                }
                
                OspPrintf(TRUE, FALSE, "��ʼģ��cu�û���¼......\n");
                cuon(1);
                OspDelay(500);
                while(1)
                {
                    vector<CTask*> tCuList = pInst->GetTasksArray("CCuLoginTask");

                    if (tCuList.size() > 0 && tCuList[0] != NULL && tCuList[0]->GetState() == CCuLoginTask::Service)
                    {
                        OspPrintf(TRUE, FALSE, "ģ��cu�û���¼�ɹ�\n");
                        break;
                    }
                    else
                    {
                        OspDelay(500);
                    }              
                }

                /*
                OspPrintf(TRUE, FALSE, "��ʼ���������ļ��е�pu�豸.........\n");
                ssall();

                while(1)
                {
                    vector<CTask*> tSsList = pInst->GetTasksArray("CPuSubscribeTask");

                    if (tSsList.size() == g_pszTestPu.size()
                        && tSsList.size() == g_cPuList.GetSize())
                    {
                        OspPrintf(TRUE, FALSE, "[%u]��pu�豸���ĳɹ���������[%u]\n",
                            g_pszTestPu.size(), g_cPuList.GetOnlinePuNum());
                        break;
                    }
                    else
                    {
                        static int nTryNum = 0;
                        OspPrintf(TRUE, FALSE, "[%u]��pu�豸���ĳɹ���������[%u]\n",
                            g_pszTestPu.size(), g_cPuList.GetOnlinePuNum());
                        nTryNum++;
                        if (nTryNum > 8)
                        {
                            break;
                        }
                        OspDelay(1000);
                    }              
                } 
                OspPrintf(TRUE, FALSE, "��ʹ��ivtnum(n)������������\n");
                */
                OspPrintf(TRUE, FALSE, "������ʹ��cmuhelp()\n");
                break;
            }
            else
            {
                OspPrintf(TRUE, FALSE, "cmu����׮δ�������̬��2s������...��\n");
                printf("cmu����׮δ�������̬��2s������...��\n");
                OspDelay(2000);
            }
        }     
    }
    
    if(g_cCmuConf.m_nIsAutoTest != 0)
    {
        //��ʼ�Զ�����
        CApp* ptApp = (CApp*)(&g_cCmuStubTestCoreApp);
        CInstance* pInst = ptApp->GetInstance(CInstance::DAEMON);

        while(1)
        {
            if (pInst->CurState() == CCmuStubTestCoreInst::Service)
            {
                OspPrintf(TRUE, FALSE, "cmu����׮�������̬����ʼ�Զ�����...��\n");
                printf("cmu����׮�������̬����ʼ�Զ�����...��\n");

                OspDelay(1*1000);

                cuion();
                OspDelay(1000);

                cuon(1);
                OspDelay(1000);

                for (int i=0; i<1; i++)
                {
                    ss();
                    OspDelay(10);
                }
                
                ptz();

                getvp();

                setvp();

                getsp();

                setsp();

                invite(GetTestPuUri().c_str(), 0, 0);

                OspDelay(60*1000);

                CCmuStubTestCoreInst* pTestIns = (CCmuStubTestCoreInst*)pInst;

                vector<CTask*> tInviteList = pTestIns->GetTasksArray("CInviteTask");

                OspPrintf(TRUE, FALSE, "��[%d]���������\n", 
                    tInviteList.size());

                for (size_t ii = 0; ii<tInviteList.size(); ii++)
                {
                    CTask* pInviteTask = tInviteList[ii];
                    if (pInviteTask != NULL)
                    {
                        byeinvite(pInviteTask->GetTaskNO());
                    }                   
                }              

                OspDelay(5*1000);

                vector<CTask*> tCuLoginList = pTestIns->GetTasksArray("CCuLoginTask");

                OspPrintf(TRUE, FALSE, "��[%d]����¼\n", 
                    tCuLoginList.size());

                for (size_t ii = 0; ii<tCuLoginList.size(); ii++)
                {
                    CTask* pLoginTask = tCuLoginList[ii];
                    if (pLoginTask != NULL)
                    {
                        cuoff(pLoginTask->GetTaskNO());
                    }                   
                }              

                OspDelay(10*1000);

                static int s_nPuid = 0;

                if (s_nPuid%2 == 0)
                {
                    char szPuid[64] = {0};
                    sprintf(szPuid, "%d", s_nPuid);
                    setal(szPuid);
                }
                else
                {
                    setal();
                }

                s_nPuid++;

                OspDelay(10*1000);
            }
            else
            {
                OspPrintf(TRUE, FALSE, "cmu����׮δ�������̬��2s������...��\n");
                printf("cmu����׮δ�������̬��2s������...��\n");
                OspDelay(2000);
            }
        }
    }

    //�������߳�
    g_cMainSemLock.Take();
    g_cMainSemLock.Take();

    //
    dsDestroy();
    
    //�˳�SIP
    OspSipQuit();

    //�˳�OSPEXT
    OspExtQuit();

    return 0;
}







