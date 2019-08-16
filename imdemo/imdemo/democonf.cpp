#include "democonf.h"

BOOL32 CDemoConf::ReadDemoConf(const s8 *sDemoConfPath)
{
	if (sDemoConfPath == nullptr)
	{
		printf("�����ļ�·��Ϊ��!\n");
		return false;
	}
	
	//�������ļ�
	s8 sStrBuf[MAX_READ_CONF_NUM] = { 0 };
	int nValue = 0;

	m_tLocalUri.SetDomain(GetDomainName());
	m_tLocalUri.SetUser(UNIQUE_SIP_UA_IM);

	m_tProxyUri.SetDomain(GetDomainName());
	m_tProxyUri.SetUser(UNIQUE_SIP_UA_PROXY);

	m_tLocalAddr.tIpAddr = GetLocalIPAddress();

	GetRegKeyInt(sDemoConfPath, SECTION_IM, KEY_LOCAL_PORT, DEAFULT_LOCAL_PORT, &nValue);
	m_tLocalAddr.wPort = static_cast<u16>(nValue);

	m_tProxyAddr.tIpAddr = GetProxyIPAddress();

	m_tProxyAddr.wPort = GetProxyPort();

	return TRUE;
}

void CDemoConf::PrintData()
{
	OspPrintf(TRUE,FALSE,"IM������Ϣ:\n");

	OspPrintf(TRUE, FALSE, "IM����:[%s]\n", m_tLocalUri.GetDomain().c_str());
	OspPrintf(TRUE, FALSE, "IM��ַ��Ϣ: [%s]\n", m_tLocalAddr.GetSipAddrStr().c_str());
	OspPrintf(TRUE, FALSE, "PROXY��ַ��Ϣ: [%s]\n", m_tProxyAddr.GetSipAddrStr().c_str());

}