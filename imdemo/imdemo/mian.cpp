#include "democonf.h"

int main(int argc, char* argv[])
{
	//��ȡim�����ļ�
	CDemoConf cDemoConf;
	if (!cDemoConf.ReadDemoConf(CONF_FILE_PATH))
	{
		printf("��ȡ�����ļ�ʧ��!\n");
		return 0;
	}
	printf("��ȡ�����ļ��ɹ�!\n");

	//��ʼ����־�ļ�
	string strLogDir = GetLogDir();
	TOspExtConf tOspExtConf;
	tOspExtConf.strErrLogDir = strLogDir + "im\\errorlog\\";
	tOspExtConf.strRunLogDir = strLogDir + "im\\runlog";

	//��ʼ��OSPEXT
	tOspExtConf.ReadKeyCfg(CONF_FILE_PATH);
	if (FALSE == OspExtInit(tOspExtConf))
	{
		printf("ospext��ʼ��ʧ��\n");
		return 0;
	}
	printf("ospext��ʼ���ɹ�!\n");

	

	//��ʼ��Sip
	TOspSipConf tOspSipConf;
	tOspSipConf.tLocalAddr = cDemoConf.m_tLocalAddr;
	tOspSipConf.tLocalURI = cDemoConf.m_tLocalUri;
	tOspSipConf.tProxyAddr = cDemoConf.m_tProxyAddr;
	tOspSipConf.dwDefaultOspIID = IM_CORE_DAEMON;
	tOspSipConf.tUAType = UNIQUE_SIP_UA_IM;

	TSipStackConf tSipStackConf;
	tSipStackConf.ReadKeyCfg(CONF_FILE_PATH);
	string strSipLogPath = GetLogDir() + "im\\";
	tSipStackConf.tKdSipConfig.szLogPath = (LPSTR)strSipLogPath.c_str();

	if (!OspSipInit(tOspSipConf, tSipStackConf))
	{
		printf("InitOspSip fail!!!\n");
		return 0;
	}
	printf("Sip ��ʼ���ɹ�!\n");

}