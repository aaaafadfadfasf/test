#include "conf.h"
#include "kdvsys.h"
CConf g_cConf;


CConf::CConf()
{

}


CConf::~CConf()
{

}



u16 CConf::GetListenPort()
{
    int nVal;
    GetRegKeyInt(ConfPath, SEC_MAIN, "LocalPort", SECURITY_MODULE_SIP_UDP_PORT, &nVal);
    return (u16)nVal;
}


void CConf::SetCfgFilePath(LPCSTR lpszVar)
{
	CCfgFile::SetCfgFilePath(lpszVar);

}



BOOL32 CConf::Load()
{

	//��ȡ���������ļ�
	if (!ReadCommonConfig())
	{
		printf("���������ļ���ʼ��ʧ��\n");
		return FALSE;
	}

	SetCfgFilePath(ConfPath);

	BOOL32 bResult = FALSE;
	s32 nValue = 0;
	s8 achLine[256+1] = {0};


	bResult = GetRegKeyString( GetCfgFilePath(), SEC_MAIN, "SecurityID", "", achLine, sizeof(achLine));
	m_strSecurityId = achLine;

	//���SecurityIDΪ�����Զ�����һ��
	if(m_strSecurityId.empty())
	{
		CUUID cUUID;
		m_strSecurityId = cUUID;
		SetRegKeyString( GetCfgFilePath(), SEC_MAIN, "SecurityID", m_strSecurityId.c_str());
	}

	// ��ȡ Security ��Port
	bResult = GetRegKeyInt( GetCfgFilePath(), SEC_MAIN, "LocalPort", SECURITY_MODULE_SIP_UDP_PORT, &nValue );
	m_tLocalAddr.wPort = nValue;


	//DB

	s8 szVal[MAX_READ_BUF_LEN] = {0};
	int nVal = 0;
	GetRegKeyString(GetCfgFilePath(), "DB", "DbIpAddr", "127.0.0.1", szVal, MAX_READ_BUF_LEN);
	m_tDbixConf.tDBConnectCfg.strDbServerIP = szVal;

	GetRegKeyInt( GetCfgFilePath(), "DB", "DbPort", 3306, &nVal);
	m_tDbixConf.tDBConnectCfg.wDbServerPort = (u16)nVal;

	GetRegKeyInt(GetCfgFilePath(), "DB", "DbType", 0, &nVal);
	m_tDbixConf.tDBConnectCfg.enDbType = ENDBType(nVal);

	GetRegKeyString(GetCfgFilePath(), "DB", "DbName", "security", szVal, MAX_READ_BUF_LEN);
	m_tDbixConf.tDBConnectCfg.strDbName = szVal;

	GetRegKeyString(GetCfgFilePath(), "DB", "DbUser", "root", szVal, MAX_READ_BUF_LEN);
	m_tDbixConf.tDBConnectCfg.strDbUserName = szVal;

	GetRegKeyString(GetCfgFilePath(), "DB", "DbPassword", "kdc", szVal, MAX_READ_BUF_LEN);
	m_tDbixConf.tDBConnectCfg.strDbUserPassword = szVal;

	GetRegKeyInt(GetCfgFilePath(), "DB", "DbThreadNum", 0, &nVal);
	m_tDbixConf.dwThreadNum = u32(nVal);

	GetRegKeyInt(GetCfgFilePath(), "VKEK", "QueryTimeLeftShift", 5, &nVal);
	m_nVkekQueryTimeLeftShift = nVal;

	GetRegKeyInt(GetCfgFilePath(), "VKEK", "DelTimeCondition", 365, &nVal);
	m_nVkekDelTimeCondition = nVal;
}