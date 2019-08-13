
#include "cppunitlite/cppunitlite.h"

#include "cms/ospext/ospext.h"
#include "cms/ospsip/ospsip.h"
#include "cmucoreapp.h"
#include "cmuconf.h"

CXSemLock g_cMainSemLock;

int main(int argc,char** argv)
{	
    //��ʼ��OSP
    if ( !IsOspInitd() )
    {
#ifdef _LINUX_
        OspInit(TRUE, 2500, "cmu");
#elif    _DEBUG
        OspInit(TRUE);
#else
        OspInit(FALSE);
#endif
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
    tOspSipConf.dwDefaultOspIID = CMU_CORE_DAEMON;

    TSipStackConf tSipStackConf;
    if(!OspSipInit(tOspSipConf, tSipStackConf))
    {
        printf("InitOspSip fail!!!\n");
        return 0;
    }
    printf("OspSip��ʼ���ɹ�!\n");

    //��ʼ����Ϣ����
    //InitCmuMsgDesc();

    //����������OSP APP
    if (g_cCmuCoreApp.CreateOspApp("cmucore", CMU_CORE_APP_ID, 80) == FALSE)
    {
        printf("CreateOspApp fail!!!\n");
        return 0;
    }    
    ::OspPost(CMU_CORE_DAEMON, CMU_START);

    OspDelay(2000);

    char achModName[256] = {0};
    strncpy(achModName, argv[0], sizeof(achModName)-1);

    std::string szFilePath(achModName);
    szFilePath += ".xml";
    TestResultXml tr(szFilePath);

    TestRegistry::runAllTests(tr);

    //�������߳�
    g_cMainSemLock.Take();
    g_cMainSemLock.Take();

    //�˳�SIP
    OspSipQuit();

    //�˳�OSP
    OspQuit();



    return tr.wasSucessful() ? 0 : 1;
}
