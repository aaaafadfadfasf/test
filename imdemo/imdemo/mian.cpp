#include "democonf.h"

int main(int argc, char* argv[])
{
	//读取im配置文件
	CDemoConf cDemoConf;
	if (!cDemoConf.ReadDemoConf(CONF_FILE_PATH))
	{
		printf("读取配置文件失败!\n");
		return 0;
	}
	printf("读取配置文件成功!\n");

	//初始化日志文件
	string strLogDir = GetLogDir();
	TOspExtConf tOspExtConf;
	tOspExtConf.strErrLogDir = strLogDir + "im\\errorlog\\";
	tOspExtConf.strRunLogDir = strLogDir + "im\\runlog";

	//初始化OSPEXT
	tOspExtConf.ReadKeyCfg(CONF_FILE_PATH);
	if (FALSE == OspExtInit(tOspExtConf))
	{
		printf("ospext初始化失败\n");
		return 0;
	}
	printf("ospext初始化成功!\n");

	

	//初始化Sip
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
	printf("Sip 初始化成功!\n");

}