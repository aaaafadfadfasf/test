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


#define  MAX_BASICSTRING_LEN     (s32)(32+1)            //字符串基本最大长度
#define  MAX_IPADDR_LEN          (s32)(32+1)            //IP地址最大长度
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
	s8 m_achDbUsrName[MAX_BASICSTRING_LEN];  // 数据库帐号
	s8 m_achDbPwd[MAX_BASICSTRING_LEN];      // 数据库密码
	s8 m_achDbName[MAX_BASICSTRING_LEN];     // 数据库名称
	s8 m_achDbSrvIP[MAX_IPADDR_LEN];         // 数据库服务器地址
	u16 m_wDbSrvPort;                        // 数据库服务器端口

	int m_nVkekQueryTimeLeftShift;           //vkek查询起始时间向左偏移量,单位：分钟
	int m_nVkekDelTimeCondition;             //删除vkek信息时间条件，默认365,单位：天
public:
	TDBIXConf m_tDbixConf;
};

extern CConf g_cConf;

#endif // _CONF_H_
