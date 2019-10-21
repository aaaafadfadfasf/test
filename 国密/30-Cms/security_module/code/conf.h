#ifndef _CONF_H_
#define _CONF_H_
#include "kdvtype.h"
#include <string>
#include "cms/ospsip/ospsip.h"
#include "cms/utility/cfgfile.h"
#include "cms/commonconfig/cmcfg.h"
#include "dbixface.h"



#define ConfPath "./conf/security_module.ini"
#define SEC_MAIN "SECURITY_MODULE"


#define  MAX_BASICSTRING_LEN     (s32)(32+1)            //�ַ���������󳤶�
#define  MAX_IPADDR_LEN          (s32)(32+1)            //IP��ַ��󳤶�
#define  MAX_READ_BUF_LEN        (512)

class CConf:public CCfgFile
{
public:
	CConf();
	~CConf();
	void SetCfgFilePath(LPCSTR lpszVar);
	BOOL32 Load();

public:
	u16 GetListenPort();
	const s8 * GetSecurityID() const
	{
		return m_strSecurityId.c_str();
	}

	//[BASICINFO]



	//[DB]
	LPCSTR GetDbSrvIP()
	{
		return m_achDbSrvIP;
	}
	u16 GetDbSrvPort()
	{
		return m_wDbSrvPort;
	}
	LPCSTR GetDbUsrName()
	{ 
		return m_achDbUsrName;
	}
	LPCSTR GetDbPwd()
	{
		return m_achDbPwd;
	}
	LPCSTR GetDbName()
	{
		return m_achDbName;
	}

	int GetVkekQueryTimeLeftShift()
	{
		return m_nVkekQueryTimeLeftShift;
	}

	int GetDelVkekInterval()
	{
		return m_nVkekDelTimeCondition;
	}

private:

	string m_strSecurityId;
	TSipAddr m_tLocalAddr;
	//[DB]
	s8 m_achDbUsrName[MAX_BASICSTRING_LEN];  // ���ݿ��ʺ�
	s8 m_achDbPwd[MAX_BASICSTRING_LEN];      // ���ݿ�����
	s8 m_achDbName[MAX_BASICSTRING_LEN];     // ���ݿ�����
	s8 m_achDbSrvIP[MAX_IPADDR_LEN];         // ���ݿ��������ַ
	u16 m_wDbSrvPort;                        // ���ݿ�������˿�

	int m_nVkekQueryTimeLeftShift;           //vkek��ѯ��ʼʱ������ƫ����,��λ������
	int m_nVkekDelTimeCondition;             //ɾ��vkek��Ϣʱ��������Ĭ��365,��λ����
public:
	TDBIXConf m_tDbixConf;
};

extern CConf g_cConf;

#endif // _CONF_H_
