#include "dboperate.h"
#include "cms/utility/db_password_codec.h"
#include "kdvsys.h"
#include <iostream>
#include <fstream>

using namespace std;

map<string, string> g_mapDevId2DomainId;

//���ݿ�������
static const s8* KEY_DB_TYPE = "DBTYPE";
static const s8* KEY_DB_IP = "IP";
static const s8* KEY_DB_PORT = "PORT";
static const s8* KEY_DB_USER = "USER";
static const s8* KEY_DB_PASSWORD = "PASSWORD";
static const s8* KEY_DB_SECURE = "SECURE";
static const s8* KEY_DB_DATABASE = "DATABASE";
static const s8* SEC_DB = "DATABASEINFO";
static const s8* SEC_FILE_ENABLE = "FILE_ENABLE_STATE";
static const s8* KEY_FILE_ENABLE = "FILE_ENABLE";

//�û���������ֻҪ���ǹ��ĵ��оͿ�����
//�û��豸������--KDMAAAA.tblDevice
static const s8* COL_NAME_DEVICE_ID = "devId";         //�豸ID

//tblDevice�ֶ�
static const s8* COL_NAME_KDID = "devId";               //KDID
static const s8* COL_NAME_DOMAINID = "domainId";        //domainid
static const s8* COL_NAME_NAME = "devName" ;             //name
static const s8* COL_NAME_ALIAS = "devAlias" ;             //alias
static const s8* COL_NAME_TYPE = "devType" ;             //type
static const s8* COL_NAME_MODE = "devModelId" ;             //modelid
static const s8* COL_NAME_FLAG = "useFlag"  ;            //Flag
static const s8* COL_NAME_NETTYPE = "netType" ;             //netType
static const s8* COL_NAME_NETADDR = "netAddr" ;             //netAddr
static const s8* COL_NAME_EXPIREDATE = "expireDate";        //expireDate
static const s8* COL_NAME_CREATER = "creater"  ;            //creater
static const s8* COL_NAME_OWNER = "owner"  ;            //owner
static const s8* COL_NAME_EXTDATA = "devData";          //devData

//tblDeviceModel�ֶ�
static const s8* COL_NAME_MANUFACTURER = "manufacturer" ;             //manufacturer
static const s8* COL_NAME_CAPID = "devCapId";              //capId
static const s8* COL_NAME_CAPNUM = "capNum";              //capNum
static const s8* COL_NAME_LICENSE = "licenseNum";        //licenseNum

//tblDeviceCapIndexData�ֶ�
static const s8* COL_NAME_DEV_CPAID = "deviceCapId" ;
static const s8* COL_NAME_CAPINDEX = "deviceCapIndex";
static const s8* COL_NAME_CPAINDEXNAME = "deviceCapIndexName";
static const s8* COL_NAME_CPAINDEXDATA = "deviceCapIndexExtData";

//tblDoaminRelation �ֶ�
static const s8* COL_NAME_DOMAIN = "domainId";
static const s8* COL_NAME_DOMAINNAME = "domainName";
static const s8* COL_NAME_PARENTDOAMINID = "parentDomainId";
static const s8* COL_NAME_DOMAINALIAS = "domainAlias";

//tblCuser �ֶ�
static const s8* COL_NAME_USERID = "cuserId";
static const s8* COL_NAME_USERGRPID = "usergroupId";

//tblCuser2Dev
static const s8* COL_NAME_DEVPRIVILEGE = "devPrivilege";

//tblCuser2DeviceCap
static const s8* COL_NAME_DEVCAPINDEX_PRIVILEGE = "devCapIndexPrivilege";

//tblCusergroup 
static const s8* COL_NAME_USER_GRPID = "cusergroupId";


void DBGlobalCBProc(u16 wErrNum, LPCSTR lpSrvMsg, void *pContext)
{
	if (NULL == lpSrvMsg)
	{
		return;
	}
	printf("\n\nDBGlobalCBProc Start*******************************************\n");
	printf("ErrNum:%d\n", wErrNum);
	printf("SrvMsg:%s\n", lpSrvMsg);
	printf("DBGlobalCBProc End*******************************************\n\n");
}

bool InitalDBEngine(ENDBType enDBType)
{
	do 
	{
		if (!DBInit(static_cast<u8>(enDBType)))
		{
			printf("���ݿ��ʼ��ʧ��\n");
			break;
		}
		if (!SetDBLocale("", DB_CHARSET_UTF8))
		{
			printf("�������ݿ��ַ���ʧ��\n");
			break;
		}
		//if (!SetupLibErrFuc(DBGlobalCBProc, NULL))
		//{
		//	OspPrintf(TRUE, false, "��װCS_Lib��Ϣ������ʧ��\n");
		//	break;
		//}
		if (!SetDBTimeOut((u8)20))
		{
			printf("�������ݿⳬʱʱ��ʧ��\n");
			break;
		}

		return true;
	} while (false);

	return false;
}

CDbOperate::CDbOperate(void)
{
	m_pc3aDBOprer = NULL;
	m_pc3aDBResultSet = NULL;
	m_pcGbsDBOprer = NULL;
	m_pcGbsDBResultSet = NULL;
	m_bConnectDB = false;
}


CDbOperate::~CDbOperate(void)
{
	DisconnectDB();
	if(m_pc3aDBOprer)
	{
		delete m_pc3aDBOprer;
		m_pc3aDBOprer = NULL;
	}
	if(m_pcGbsDBOprer)
	{
		delete m_pcGbsDBOprer;
		m_pcGbsDBOprer = NULL;
	}
}

string CDbOperate::GetAAAConfigFile()
{
	////������ݿ������ļ�
	//string cfgFile = "/opt/kdm/gbs/conf/realAAA.ini";
	//FILE *fp = fopen(cfgFile.c_str(), "r+");
	//if(NULL == fp)
	//{
	//	//�ļ������ڣ�����һ��ģ��
	//	printf("�����ļ������ڣ�����һ��ģ��\n");
	//	fp = fopen(cfgFile.c_str(), "w+");
	//	char str[1024];
	//	sprintf(str, "/*��������ļ������������ݿ�λ�õģ�������ݿⲻ�ڱ����������������������ָ�����ݿ�Ĳ�����������ݿ��ڱ������뽫FILE_ENABLE������Ϊ0��*/\n"
	//		"[%s]\n%s=0\n[%s]\n%s=2\n%s=\n%s=0\n%s=\n%s=\n%s=\n", 
	//		SEC_FILE_ENABLE, KEY_FILE_ENABLE, SEC_DB, KEY_DB_TYPE, KEY_DB_IP, KEY_DB_PORT, KEY_DB_USER, KEY_DB_PASSWORD, KEY_DB_DATABASE);

	//	fprintf(fp, str);
	//	fclose(fp);

	//	cfgFile = "/opt/kdm/tas/conf/aaacfg.ini";
	//	return cfgFile;
	//}
	//fclose(fp);
	////��ȡ�����ļ�
	//s32 nFileEnable = 0;
	//if(!GetRegKeyInt(cfgFile.c_str(), SEC_FILE_ENABLE, KEY_FILE_ENABLE, 0, &nFileEnable))
	//{
	//	printf("��ȡ�ļ�����������ʧ����\n");
	//	//��ȡ�������ö�ʧ���ˣ�ֱ��ʹ��tas���ݿ�
	//	cfgFile = "/opt/kdm/tas/conf/aaacfg.ini";
	//}
	//if(nFileEnable <= 0)
	//{
	//	cfgFile = "/opt/kdm/tas/conf/aaacfg.ini";
	//}
	//return cfgFile;
	string cfgFile = "/opt/kdm/tas/conf/aaacfg.ini";
	
	return cfgFile;
}

bool CDbOperate::InitDB()
{
	string aaaCfgFile = GetAAAConfigFile();
	if (!IsFileExist(aaaCfgFile))
	{
		printf("aaaCfgFile[%s]�ļ�������! \n",aaaCfgFile.c_str());
		return false;
	}
	TDBConnectCfg cfg = GetDBConfig(aaaCfgFile);
	if(!InitalDBEngine(cfg.enDbType))
	{
		printf("���ݿ��ʼ��ʧ��\n");
		return false;
	}
	printf("���ݿ��ʼ���ɹ�\n");
	//����3A���ݿ�
	bool bRet = Connect3aDB(cfg);
	//����GBS���ݿ�
	//cfg = GetDBConfig("/opt/kdm/gbs/conf/gbscfg.ini");
	//bRet = bRet && ConnectGbsDB(cfg);
	m_bConnectDB = bRet;
	return bRet;
}

bool CDbOperate::Connect3aDB(const TDBConnectCfg &cfg)
{
	if (IsConnectDB())
	{
		printf("3A���ݿ��Ѿ�������\n");
		return true;
	}
	if (NULL == m_pc3aDBOprer)
	{
		m_pc3aDBOprer = new CDBHandler;
		if (NULL == m_pc3aDBOprer)
		{
			printf("���ݿ������������ڴ�ʧ��\n");
			return false;
		}
	}
	if (NULL == m_pc3aDBResultSet)
	{
		m_pc3aDBResultSet = new CResultSet;
		if (NULL == m_pc3aDBResultSet)
		{
			printf("���ݿⷵ�ؽ������������ڴ�ʧ��\n");
			return false;
		}
	}
	BOOL32 bRet = FALSE;
	bRet = m_pc3aDBOprer->SetupSrvErrFuc(&DBServerCBProc, this);
	if (FALSE == bRet)
	{
		printf("��װ�������˴�����Ϣ������ʧ��\n");
		return false;
	}
	bRet = m_pc3aDBOprer->SetupCtErrFuc(&DBClientCBProc, this);
	if (FALSE == bRet)
	{
		printf("��װ�ͻ��˴�����Ϣ������ʧ��\n");
		return false;
	}
	bRet = m_pc3aDBOprer->ConnecttoSrv(cfg.strDbUserName.c_str(), cfg.strDbUserPassword.c_str(),cfg.strDbName.c_str(),
		cfg.strDbServerIP.c_str(), cfg.wDbServerPort);
	if (FALSE == bRet)
	{
		printf("3A�������ݿ�ʧ��\n");
		return false;
	}
	printf("3A���ݿ����ӳɹ�\n");
	return true;
}

bool CDbOperate::ConnectGbsDB(const TDBConnectCfg &cfg)
{
	if (IsConnectDB())
	{
		printf("GBS���ݿ��Ѿ�������\n");
		return true;
	}
	if (NULL == m_pcGbsDBOprer)
	{
		m_pcGbsDBOprer = new CDBHandler;
		if (NULL == m_pcGbsDBOprer)
		{
			printf("���ݿ������������ڴ�ʧ��\n");
			return false;
		}
	}
	if (NULL == m_pcGbsDBResultSet)
	{
		m_pcGbsDBResultSet = new CResultSet;
		if (NULL == m_pcGbsDBResultSet)
		{
			printf("���ݿⷵ�ؽ������������ڴ�ʧ��\n");
			return false;
		}
	}
	BOOL32 bRet = FALSE;
	bRet = m_pcGbsDBOprer->SetupSrvErrFuc(&DBServerCBProc, this);
	if (FALSE == bRet)
	{
		printf("��װ�������˴�����Ϣ������ʧ��\n");
		return false;
	}
	bRet = m_pcGbsDBOprer->SetupCtErrFuc(&DBClientCBProc, this);
	if (FALSE == bRet)
	{
		printf("��װ�ͻ��˴�����Ϣ������ʧ��\n");
		return false;
	}
	bRet = m_pcGbsDBOprer->ConnecttoSrv(cfg.strDbUserName.c_str(), cfg.strDbUserPassword.c_str(),cfg.strDbName.c_str(),
		cfg.strDbServerIP.c_str(), cfg.wDbServerPort);
	if (FALSE == bRet)
	{
		printf("GBS�������ݿ�ʧ��\n");
		return false;
	}
	printf("GBS���ݿ����ӳɹ�\n");
	return true;
}

bool CDbOperate::DisconnectDB(void)
{
	bool bRet = false;

	if (IsConnectDB())
	{
		bRet = m_pc3aDBOprer->CloseConnection();
		bRet = bRet && m_pcGbsDBOprer->CloseConnection();
	}

	//����Ϊ�Ͽ�״̬
	m_bConnectDB = false;

	return bRet;
}

TDBConnectCfg CDbOperate::GetDBConfig(string iniFile)
{
	const s8* cfgPath = iniFile.c_str();
	//���ݿ�����
	s32 nDbType = 2;
	if(!GetRegKeyInt(cfgPath, SEC_DB, KEY_DB_TYPE, 0, &nDbType))
	{
		printf("��ȡ�����ļ������ݿ�����ʧ��\n");
	}
	//���ݿ�IP
	s8 sDbIp[1024];
	if (!GetRegKeyString(cfgPath, SEC_DB, KEY_DB_IP, "tas", sDbIp, sizeof(sDbIp)))
	{
		printf("��ȡ�����ļ������ݿ�IP��ַʧ��\n");
	}
	sDbIp[sizeof(sDbIp)-1] = '\0';
	//���ݿ�˿�
	s32 wDbPort = 0;
	if(!GetRegKeyInt(cfgPath, SEC_DB, KEY_DB_PORT, 0, &wDbPort))
	{
		printf("��ȡ�����ļ������ݿ�˿�ʧ��\n");
	}
	//���ݿ��û���
	s8 sDbUserName[1024];
	if(!GetRegKeyString(cfgPath, SEC_DB, KEY_DB_USER, "", sDbUserName, sizeof(sDbUserName)))
	{
		printf("��ȡ�����ļ��������û���ʧ��\n");
	}
	sDbUserName[sizeof(sDbUserName)-1] = '\0';
	
	
	s32 nIsSecure = 0;
	if(!GetRegKeyInt(cfgPath, SEC_DB, KEY_DB_SECURE, 0, &nIsSecure))
	{
		printf("��ȡ�����ļ����Ƿ�������ý�ʧ�ܣ���Ϊδ����\n");
		nIsSecure = 0;
	}
	
	//���ݿ�����
	s8 sDbPasswd[1024];
	if(!GetRegKeyString(cfgPath, SEC_DB, KEY_DB_PASSWORD, "", sDbPasswd, sizeof(sDbPasswd)))
	{
		printf("��ȡ�����ļ�����������ʧ��\n");
	}
	sDbPasswd[sizeof(sDbPasswd)-1] = '\0';
	//���ݿ�����
	s8 sDbName[1024];
	if(!GetRegKeyString(cfgPath, SEC_DB, KEY_DB_DATABASE, "", sDbName, sizeof(sDbName)))
	{
		printf("��ȡ�����ļ�����������ʧ��\n");
	}
	sDbName[sizeof(sDbName)-1] = '\0';

	TDBConnectCfg cfg;
	cfg.enDbType = (ENDBType)nDbType;
	cfg.strDbServerIP = sDbIp;
	cfg.wDbServerPort = wDbPort;
	cfg.strDbUserName = sDbUserName;
	if(nIsSecure == 0)
	{
		cfg.strDbUserPassword = sDbPasswd;
	}
	else
	{
		cfg.strDbUserPassword = DecryptMySqlPassword(string(sDbPasswd));
	}
	cfg.strDbName = sDbName;

	return cfg;
}

bool CDbOperate::IsConnectDB()
{
	if (NULL == m_pc3aDBOprer || NULL == m_pc3aDBResultSet)
	{
		return false;
	}

	return m_bConnectDB;
}

void CDbOperate::DBServerCBProc( u16 wErrNum, LPCSTR lpSrvMsg, void *pContext )
{
	if (NULL == lpSrvMsg)
	{
		return;
	}
	printf("\n\nDBServerCBProc Start*******************************************\n");
	printf("ErrNum:%d\n", wErrNum);
	printf("SrvMsg:%s\n", lpSrvMsg);
	printf("DBServerCBProc End*******************************************\n\n");
}

void CDbOperate::DBClientCBProc( u16 wErrNum, LPCSTR lpSrvMsg, void *pContext )
{
	if (NULL != pContext && 0 == wErrNum)
	{
		//�������
		CDbOperate* pcDbEngine = (CDbOperate*)pContext;
		pcDbEngine->m_bConnectDB = false;
		printf("DBDisconnect detected\n");
	}
}

bool CDbOperate::QryAllDomainRelation(vector<CDomainRelationInfo> &vecDomainRelation)
{
	vecDomainRelation.clear();
	LPCSTR lpszSql = "SELECT * FROM KDMAAA.tblDomainRelation";
	if (m_pc3aDBOprer->ExecSql(lpszSql,&m_pc3aDBResultSet->m_cDBRs))
	{
		CDomainRelationInfo cDomainRelationInfo;
		TDomainExtData tDomainExtData;
		char domainid[64]={0};
		char domainname[1024]={0};
		char parentdomain[256]={0};
		char doaminalias[1024]={0};
		while(m_pc3aDBResultSet->MoveNext() && !m_pc3aDBResultSet->IsEof())
		{
			m_pc3aDBResultSet->m_cDBRs.GetFieldValue(COL_NAME_DOMAIN,domainid,sizeof(domainid));
			m_pc3aDBResultSet->m_cDBRs.GetFieldValue(COL_NAME_DOMAINNAME,domainname,sizeof(domainname));
			m_pc3aDBResultSet->m_cDBRs.GetFieldValue(COL_NAME_PARENTDOAMINID,parentdomain,sizeof(parentdomain));
			m_pc3aDBResultSet->m_cDBRs.GetFieldValue(COL_NAME_DOMAINALIAS,doaminalias,sizeof(doaminalias));

			cDomainRelationInfo.SetDomainID(domainid);
			cDomainRelationInfo.SetDomainName(domainname);
			cDomainRelationInfo.SetParentDomainID(parentdomain);
			tDomainExtData.FromXml(doaminalias);
			cDomainRelationInfo.SetDomainAlias(tDomainExtData);

			vecDomainRelation.push_back(cDomainRelationInfo);
		}
		return true;
	}
	return false;
}

bool CDbOperate::QryAllDevs(vector<TExtDeviceInfo> &vecGbDev)
{
	vecGbDev.clear();
	LPCSTR lpszSql = "SELECT * FROM KDMAAA.tblDevice";
	if (m_pc3aDBOprer->ExecSql(lpszSql,&m_pc3aDBResultSet->m_cDBRs))
	{
		TExtDeviceInfo tDev;
		TDevExtData tExtData;
		char kdid[64]={0};
		char domainid[64]={0};
		char name[1024]={0};
		char alias[64]={0};
		int type;
		char modelId[64]={0};
		int usedFlag;
		int netType;
		char netAddress[64]={0};
		char expireDate[64]={0};
		int creater;
		char owner[64]={0};
		char devdata[1024]={0};

		while(m_pc3aDBResultSet->MoveNext() && !m_pc3aDBResultSet->IsEof())
		{
			m_pc3aDBResultSet->m_cDBRs.GetFieldValue(COL_NAME_KDID,kdid,sizeof(kdid));
			m_pc3aDBResultSet->m_cDBRs.GetFieldValue(COL_NAME_DOMAINID,domainid,sizeof(domainid));
			m_pc3aDBResultSet->m_cDBRs.GetFieldValue(COL_NAME_NAME,name,sizeof(name));
			m_pc3aDBResultSet->m_cDBRs.GetFieldValue(COL_NAME_ALIAS,alias,sizeof(alias));
			m_pc3aDBResultSet->m_cDBRs.GetFieldValue(COL_NAME_TYPE,type);
			m_pc3aDBResultSet->m_cDBRs.GetFieldValue(COL_NAME_MODE,modelId,sizeof(modelId));
			m_pc3aDBResultSet->m_cDBRs.GetFieldValue(COL_NAME_FLAG,usedFlag);
			m_pc3aDBResultSet->m_cDBRs.GetFieldValue(COL_NAME_NETTYPE,netType);
			m_pc3aDBResultSet->m_cDBRs.GetFieldValue(COL_NAME_NETADDR,netAddress,sizeof(netAddress));
			m_pc3aDBResultSet->m_cDBRs.GetFieldValue(COL_NAME_EXPIREDATE,expireDate,sizeof(expireDate));
			m_pc3aDBResultSet->m_cDBRs.GetFieldValue(COL_NAME_CREATER,creater);
			m_pc3aDBResultSet->m_cDBRs.GetFieldValue(COL_NAME_OWNER,owner,sizeof(owner));
			m_pc3aDBResultSet->m_cDBRs.GetFieldValue(COL_NAME_EXTDATA,devdata,sizeof(devdata));


			tDev.deviceInfo.SetDeviceUUID(string(kdid));
			tDev.deviceInfo.SetDomainUUID(string(domainid));
			tDev.deviceInfo.SetDeviceName(string(name));
			tDev.deviceInfo.SetDeviceAlias(string(alias));
			tDev.deviceInfo.SetDeviceModel(string(modelId));
			tDev.deviceInfo.SetDeviceType(type);
			bool tmp = usedFlag? true:false;
			tDev.deviceInfo.SetUsedFlag(tmp);
			tDev.deviceInfo.SetNetType(netType);
			tDev.deviceInfo.SetNetAddress(string(netAddress));
			tDev.deviceInfo.SetExpireDate(string(expireDate));
			tDev.deviceInfo.SetCreater(creater);
			tDev.deviceInfo.SetOwner(string(owner));
			tExtData.FromXml(devdata);

			tDev.deviceInfo.SetDevData(tExtData);
			if (!tDev.deviceInfo.GetDeviceUUID().empty())
			{
				vecGbDev.push_back(tDev);
				g_mapDevId2DomainId.insert(pair<string,string>(string(kdid), string(domainid)));
			}
		}
		return true;
	}
	return false;
}

void CDbOperate::FullDeviceInfo(TExtDeviceInfo &tDev)
{
	GetDevCapInfo(tDev);
	GetDevCapName(tDev);
}

bool CDbOperate::GetDevCapInfo(TExtDeviceInfo &tDev)
{
	TDeviceCapInfo tDevCapInfo;
	vector<TDeviceCapInfo> vecDevCapInfo;
	char cmd[256];
	sprintf(cmd,"SELECT * FROM KDMAAA.tblDeviceModel WHERE modelId = \'%s\'",tDev.deviceInfo.GetDeviceModel().c_str());
	LPCSTR lpszSql = cmd;
	if (m_pc3aDBOprer->ExecSql(lpszSql,&m_pc3aDBResultSet->m_cDBRs))
	{
		char manufacturer[64];
		int capId;
		int capNum;
		int licenseNum;
		while(m_pc3aDBResultSet->MoveNext() && !m_pc3aDBResultSet->IsEof())
		{
			m_pc3aDBResultSet->m_cDBRs.GetFieldValue(COL_NAME_MANUFACTURER,manufacturer,sizeof(manufacturer));
			m_pc3aDBResultSet->m_cDBRs.GetFieldValue(COL_NAME_CAPID,capId);
			m_pc3aDBResultSet->m_cDBRs.GetFieldValue(COL_NAME_CAPNUM,capNum);
			m_pc3aDBResultSet->m_cDBRs.GetFieldValue(COL_NAME_LICENSE,licenseNum);

			tDevCapInfo.type = capId;
			tDevCapInfo.num = capNum;
			tDevCapInfo.license = licenseNum;
			if (capId == DEVICE_CAP_VIDEOSOURCE)
			{	
				tDev.deviceInfo.SetManufacturer(manufacturer);
			}
			vecDevCapInfo.push_back(tDevCapInfo);
		}
		tDev.deviceInfo.SetDeviceModelCapDetail(vecDevCapInfo);
		return true;
	}
	return false;
}

bool CDbOperate::GetDevCapName(TExtDeviceInfo &tDev)
{
	TDeviceCapIndexName tDevCapIndexName;
	TDeviceCapExtData tDevCapExtData;
	char cmd[256];
	sprintf(cmd,"SELECT * FROM KDMAAA.tblDeviceCapIndexData WHERE deviceId=\'%s\'",tDev.deviceInfo.GetDeviceUUID().c_str());
	LPCSTR lpszSql = cmd;
	if (m_pc3aDBOprer->ExecSql(lpszSql,&m_pc3aDBResultSet->m_cDBRs))
	{
		int capId;
		int capIndex;
		char devCapIndexName[1024]={0};
		char devCapIndexData[1024]={0};
		while(m_pc3aDBResultSet->MoveNext() && !m_pc3aDBResultSet->IsEof())
		{
			m_pc3aDBResultSet->m_cDBRs.GetFieldValue(COL_NAME_DEV_CPAID,capId);
			m_pc3aDBResultSet->m_cDBRs.GetFieldValue(COL_NAME_CAPINDEX,capIndex);
			m_pc3aDBResultSet->m_cDBRs.GetFieldValue(COL_NAME_CPAINDEXNAME,devCapIndexName,sizeof(devCapIndexName));
			m_pc3aDBResultSet->m_cDBRs.GetFieldValue(COL_NAME_CPAINDEXDATA,devCapIndexData,sizeof(devCapIndexData));

			if (capId == DEVICE_CAP_VIDEOSOURCE)
			{
				tDevCapExtData.FromXml(devCapIndexData);
				tDevCapIndexName.index = capIndex;
				tDevCapIndexName.name = devCapIndexName;
				tDevCapIndexName.extData = tDevCapExtData;

				tDev.deviceCapName.deviceId = tDev.deviceInfo.GetDeviceUUID();
				tDev.deviceCapName.devDomainId = tDev.deviceInfo.GetDomainUUID();
				tDev.deviceCapName.capId = capId;
				tDev.deviceCapName.deviceCapIndexNames.push_back(tDevCapIndexName);
			}
		}
		return true;
	}
	return false;
}

bool CDbOperate::QryAllUsers(vector<CUserInfo> &vecUsers)
{
	vecUsers.clear();
	CUserInfo cUser;
	//��ѯ�����û���������admin
	LPCSTR lpszSql = "SELECT * FROM KDMAAA.tblCuser WHERE userName != \'admin\'";
	if (m_pc3aDBOprer->ExecSql(lpszSql,&m_pc3aDBResultSet->m_cDBRs))
	{
		char userId[64]={0};
		char userGrpId[64]={0};

		while(m_pc3aDBResultSet->MoveNext() && !m_pc3aDBResultSet->IsEof())
		{
			m_pc3aDBResultSet->m_cDBRs.GetFieldValue(COL_NAME_USERID,userId,sizeof(userId));
			m_pc3aDBResultSet->m_cDBRs.GetFieldValue(COL_NAME_USERGRPID,userGrpId,sizeof(userGrpId));
			
			cUser.SetUserID(string(userId));
			cUser.SetUsergrpID(string(userGrpId));

			vecUsers.push_back(cUser);
		}
		return true;
	}
	return false;
}

string CDbOperate::GetDevDomainIdByDevId(const string &strDevId)
{
	map<string, string>::iterator mapIt = g_mapDevId2DomainId.find(strDevId);
	if (mapIt != g_mapDevId2DomainId.end())
	{
		return mapIt->second;
	}
	return "";
}

bool CDbOperate::GetUser2DevCapPrivilege(const string &strUserId,const string &strDevId,vector<TDeviceCapPrivilege> &vecDevCapPri)
{
	vecDevCapPri.clear();
	TDeviceCapPrivilege tDevCapPri;
	CBitsContainer cBit;
	char cmd[256];
	sprintf(cmd,"SELECT * FROM KDMAAA.tblCuser2DeviceCap WHERE cuserId = \'%s\' AND devId=\'%s\'",strUserId.c_str(),strDevId.c_str());
	LPCSTR lpszSql = cmd;
	if (m_pc3aDBOprer->ExecSql(lpszSql,&m_pc3aDBResultSet->m_cDBRs))
	{
		int devCapId;
		int devCapIndex;
		int devCapIndexPrivilege;
		while(m_pc3aDBResultSet->MoveNext() && !m_pc3aDBResultSet->IsEof())
		{
			m_pc3aDBResultSet->m_cDBRs.GetFieldValue(COL_NAME_CAPID,devCapId);
			m_pc3aDBResultSet->m_cDBRs.GetFieldValue("devCapIndex",devCapIndex);
			m_pc3aDBResultSet->m_cDBRs.GetFieldValue(COL_NAME_DEVCAPINDEX_PRIVILEGE,devCapIndexPrivilege);

			tDevCapPri.id = devCapId;
			cBit.ConvertFromInt(devCapIndexPrivilege);
			tDevCapPri.privileges.insert(pair< int,CBitsContainer >(devCapIndex,cBit));

			vecDevCapPri.push_back(tDevCapPri);
		}
		return true;
	}
	return false;
}


bool CDbOperate::QryUser2DevCapPrivilege(const string &strUserId, map<string, vector<TDeviceCapPrivilege> > &mapDevCapPri)
{
	vector<TDeviceCapPrivilege> vecDevCapPri;

	TDeviceCapPrivilege tDevCapPri;
	CBitsContainer cBit;
	char cmd[256];
	sprintf(cmd,"SELECT * FROM KDMAAA.tblCuser2DeviceCap WHERE cuserId = \'%s\' order by cuserId, devId, devCapId, devCapIndex",strUserId.c_str());
	LPCSTR lpszSql = cmd;
	if (m_pc3aDBOprer->ExecSql(lpszSql,&m_pc3aDBResultSet->m_cDBRs))
	{
		char devId[64] = {0};
		int devCapId;
		int devCapIndex;
		int devCapIndexPrivilege;

        string lastdevId; // �������ݿ��¼��devId
        int lastdevCapId; // �������ݿ��¼��devCapId(����lastdevId��ʼ���ˣ������Բ��ó�ʼ��)
		while(m_pc3aDBResultSet->MoveNext() && !m_pc3aDBResultSet->IsEof())
		{
			m_pc3aDBResultSet->m_cDBRs.GetFieldValue(COL_NAME_CAPID,devCapId);
			m_pc3aDBResultSet->m_cDBRs.GetFieldValue("devCapIndex",devCapIndex);
			m_pc3aDBResultSet->m_cDBRs.GetFieldValue(COL_NAME_DEVCAPINDEX_PRIVILEGE,devCapIndexPrivilege);
			m_pc3aDBResultSet->m_cDBRs.GetFieldValue("devId",devId, sizeof(devId));

            if (0 == strcmp(devId, ""))continue; // ��id�ļ�¼ֱ������

            /* devid, devcapid, devcapindex ����ṹ */ 
            if (0==strcmp(lastdevId.c_str(), devId)) // ���豸�ĺ���Ȩ��
            {
                if (lastdevCapId==devCapId)// ���豸��ͬһcapId�ĺ���capIndex
                {
                    cBit.ConvertFromInt(devCapIndexPrivilege);
                    tDevCapPri.privileges.insert(pair< int,CBitsContainer >(devCapIndex,cBit));
                }
                else
                {
                    if (!tDevCapPri.privileges.empty()) // ������豸���ϸ�capId������Ȩ��
                    {
                        vecDevCapPri.push_back(tDevCapPri);
                    }

                    tDevCapPri.privileges.clear();

                    tDevCapPri.id = devCapId;
                    cBit.ConvertFromInt(devCapIndexPrivilege);
                    tDevCapPri.privileges.insert(pair< int,CBitsContainer >(devCapIndex,cBit));

                    lastdevCapId=devCapId;
                }
            }
            else // ���豸�ĵ�һ��Ȩ��
            {
                if (!vecDevCapPri.empty() && !lastdevId.empty()) // �����ϸ��豸��Ȩ��
                {
                    mapDevCapPri.insert(pair<string, vector<TDeviceCapPrivilege> >(lastdevId, vecDevCapPri));
                }

                vecDevCapPri.clear();
                tDevCapPri.privileges.clear();

                tDevCapPri.id = devCapId;
                cBit.ConvertFromInt(devCapIndexPrivilege);
                tDevCapPri.privileges.insert(pair< int,CBitsContainer >(devCapIndex,cBit));

                lastdevId=devId;
                lastdevCapId=devCapId;
            }
		}

        /*�������һ���豸��Ȩ��*/
        if (!tDevCapPri.privileges.empty())
        {
            vecDevCapPri.push_back(tDevCapPri);
        }
        if (!vecDevCapPri.empty() && !lastdevId.empty())
        {
            mapDevCapPri.insert(pair<string, vector<TDeviceCapPrivilege> >(lastdevId, vecDevCapPri));
        }

		return true;
	}
	return false;
}


bool CDbOperate::QryAllUserDevInfo(vector<CUserDeviceInfo> &vecUserDevInfo)
{
	vecUserDevInfo.clear();
	CUserDeviceInfo cUserDevInfo;
	CDevicePrivilege cDevPri;
	vector<TDeviceCapPrivilege> vecDevCapPri;

	vector<CUserInfo> vecAllUsers;
	//�Ȳ�ѯ�����û�
	QryAllUsers(vecAllUsers);

	char cmd[256];
	LPCSTR lpszSql;
	//��ѯÿ���û����豸��Ȩ��
	for (vector<CUserInfo>::iterator it_str = vecAllUsers.begin();it_str != vecAllUsers.end();it_str++)
	{
		map<string, vector<TDeviceCapPrivilege> > mapDevCapPri;
		QryUser2DevCapPrivilege(it_str->GetUserID(), mapDevCapPri);

		sprintf(cmd,"SELECT * FROM KDMAAA.tblCuser2Dev WHERE cuserId=\'%s\'",it_str->GetUserID().c_str());
		lpszSql = cmd;
		if (m_pc3aDBOprer->ExecSql(lpszSql,&m_pc3aDBResultSet->m_cDBRs))
		{
			char devId[64] ={0};
			int devPrivilege;
			//���û���ͬ���豸
			while (m_pc3aDBResultSet->MoveNext() && !m_pc3aDBResultSet->IsEof())
			{
				m_pc3aDBResultSet->m_cDBRs.GetFieldValue(COL_NAME_DEVICE_ID,devId,sizeof(devId));
				m_pc3aDBResultSet->m_cDBRs.GetFieldValue(COL_NAME_DEVPRIVILEGE,devPrivilege);

				cUserDevInfo.SetUserID(it_str->GetUserID());
				cUserDevInfo.SetDeviceID(string(devId));

				cDevPri.ConvertFromInt(devPrivilege);
				cUserDevInfo.SetDevicePrivilege(cDevPri);

				map<string, vector<TDeviceCapPrivilege> >::iterator mapIt = mapDevCapPri.find(string(devId));
				if (mapIt != mapDevCapPri.end())
				{
					cUserDevInfo.SetDeviceCapPrivilege(mapIt->second);
				}
                else
                {
                    cUserDevInfo.GetDeviceCapPrivilege().clear();
                }
				cUserDevInfo.SetDevDomainID(GetDevDomainIdByDevId(string(devId)));
				vecUserDevInfo.push_back(cUserDevInfo);
			}
		}

	}

	return true;
}

bool CDbOperate::QryAllUserGrps(vector<string> &vecGrps)
{
	vecGrps.clear();
	LPCSTR lpszSql = "SELECT * FROM KDMAAA.tblCusergroup";
	if (m_pc3aDBOprer->ExecSql(lpszSql,&m_pc3aDBResultSet->m_cDBRs))
	{
		char grpId[64]={0};

		while(m_pc3aDBResultSet->MoveNext() && !m_pc3aDBResultSet->IsEof())
		{
			m_pc3aDBResultSet->m_cDBRs.GetFieldValue(COL_NAME_USER_GRPID,grpId,sizeof(grpId));
			vecGrps.push_back(string(grpId));
		}
		return true;
	}
	return false;
}

bool CDbOperate::GetUserGrp2DevCapPrivilege(const string &strGrpId,const string &strDevId,vector<TDeviceCapPrivilege> &vecDevCapPri)
{
	vecDevCapPri.clear();
	TDeviceCapPrivilege tDevCapPri;
	CBitsContainer cBit;
	char cmd[256];
	sprintf(cmd,"SELECT * FROM KDMAAA.tblCusergrp2DeviceCap WHERE cusergroupId = \'%s\' AND devId=\'%s\'",strGrpId.c_str(),strDevId.c_str());
	LPCSTR lpszSql = cmd;
	if (m_pc3aDBOprer->ExecSql(lpszSql,&m_pc3aDBResultSet->m_cDBRs))
	{
		int devCapId;
		int devCapIndex;
		int devCapIndexPrivilege;
		while(m_pc3aDBResultSet->MoveNext() && !m_pc3aDBResultSet->IsEof())
		{
			m_pc3aDBResultSet->m_cDBRs.GetFieldValue(COL_NAME_CAPID,devCapId);
			m_pc3aDBResultSet->m_cDBRs.GetFieldValue("devCapIndex",devCapIndex);
			m_pc3aDBResultSet->m_cDBRs.GetFieldValue(COL_NAME_DEVCAPINDEX_PRIVILEGE,devCapIndexPrivilege);

			tDevCapPri.id = devCapId;
			cBit.ConvertFromInt(devCapIndexPrivilege);
			tDevCapPri.privileges.insert(pair< int,CBitsContainer >(devCapIndex,cBit));

			vecDevCapPri.push_back(tDevCapPri);
		}
		return true;
	}
	return false;
}


bool CDbOperate::QryUserGrp2DeviceCap(const string &strGrpId,map<string, vector<TDeviceCapPrivilege> > &mapDevCapPri)
{
	vector<TDeviceCapPrivilege> vecDevCapPri;
	
	TDeviceCapPrivilege tDevCapPri;
	CBitsContainer cBit;
	char cmd[256];
	sprintf(cmd,"SELECT * FROM KDMAAA.tblCusergrp2DeviceCap WHERE cusergroupId = \'%s\' order by cusergroupId,devId,devCapId,devCapIndex",strGrpId.c_str());
	LPCSTR lpszSql = cmd;
	if (m_pc3aDBOprer->ExecSql(lpszSql,&m_pc3aDBResultSet->m_cDBRs))
	{
		char devId[64] = {0};
		int devCapId;
		int devCapIndex;
		int devCapIndexPrivilege;

        string lastdevId; // �������ݿ��¼��devId
        int lastdevCapId; // �������ݿ��¼��devCapId(����lastdevId��ʼ���ˣ������Բ��ó�ʼ��)
		while(m_pc3aDBResultSet->MoveNext() && !m_pc3aDBResultSet->IsEof())
		{
			m_pc3aDBResultSet->m_cDBRs.GetFieldValue(COL_NAME_CAPID,devCapId);
			m_pc3aDBResultSet->m_cDBRs.GetFieldValue("devCapIndex",devCapIndex);
			m_pc3aDBResultSet->m_cDBRs.GetFieldValue(COL_NAME_DEVCAPINDEX_PRIVILEGE,devCapIndexPrivilege);
			m_pc3aDBResultSet->m_cDBRs.GetFieldValue("devId",devId, sizeof(devId));

            if (0 == strcmp(devId, ""))continue; // ��id�ļ�¼ֱ������

            /* devid, devcapid, devcapindex ����ṹ */ 
            if (0==strcmp(lastdevId.c_str(), devId)) // ���豸�ĺ���Ȩ��
            {
                if (lastdevCapId==devCapId)// ���豸��ͬһcapId�ĺ���capIndex
                {
                    cBit.ConvertFromInt(devCapIndexPrivilege);
                    tDevCapPri.privileges.insert(pair< int,CBitsContainer >(devCapIndex,cBit));
                }
                else
                {
                    if (!tDevCapPri.privileges.empty()) // ������豸��ĳcapId������Ȩ��
                    {
                        vecDevCapPri.push_back(tDevCapPri);
                    }

                    tDevCapPri.privileges.clear();

                    tDevCapPri.id = devCapId;
                    cBit.ConvertFromInt(devCapIndexPrivilege);
                    tDevCapPri.privileges.insert(pair< int,CBitsContainer >(devCapIndex,cBit));

                    lastdevCapId=devCapId;
                }
            }
            else // ���豸�ĵ�һ��Ȩ��
            {
                if (!vecDevCapPri.empty() && !lastdevId.empty()) // �����ϸ��豸��Ȩ��
                {
                    mapDevCapPri.insert(pair<string, vector<TDeviceCapPrivilege> >(lastdevId, vecDevCapPri));
                }

                vecDevCapPri.clear();
                tDevCapPri.privileges.clear();

                tDevCapPri.id = devCapId;
                cBit.ConvertFromInt(devCapIndexPrivilege);
                tDevCapPri.privileges.insert(pair< int,CBitsContainer >(devCapIndex,cBit));

                lastdevId=devId;
                lastdevCapId=devCapId;
            }
		}

        /*�������һ���豸��Ȩ��*/
        if (!tDevCapPri.privileges.empty())
        {
            vecDevCapPri.push_back(tDevCapPri);
        }
        if (!vecDevCapPri.empty() && !lastdevId.empty())
        {
            mapDevCapPri.insert(pair<string, vector<TDeviceCapPrivilege> >(lastdevId, vecDevCapPri));
        }

		return true;
	}

	return false;
}


bool CDbOperate::QryAllUserGrpDevInfo(vector<CUserGroupDeviceInfo> &vecUserGrpDevInfo)
{
	vecUserGrpDevInfo.clear();
	CUserGroupDeviceInfo cUserGrpDevInfo;
	CDevicePrivilege cDevPri;
	vector<TDeviceCapPrivilege> vecDevCapPri;

	vector<string> vecAllUserGrp;
	//�Ȳ�ѯ���з���
	QryAllUserGrps(vecAllUserGrp);

	char cmd[256];
	LPCSTR lpszSql;
	//��ѯÿ���û�������豸��Ȩ��
	for (vector<string>::iterator it_str = vecAllUserGrp.begin();it_str != vecAllUserGrp.end();it_str++)
	{
		map<string, vector<TDeviceCapPrivilege> > mapDevCapPri;
		QryUserGrp2DeviceCap(*it_str, mapDevCapPri);

		sprintf(cmd,"SELECT * FROM KDMAAA.tblCusergrp2Dev WHERE cusergroupId=\'%s\'",it_str->c_str());
		lpszSql = cmd;
		if (m_pc3aDBOprer->ExecSql(lpszSql,&m_pc3aDBResultSet->m_cDBRs))
		{
			char devId[64] ={0};
			int devPrivilege;
			//���û���ͬ���豸
			while (m_pc3aDBResultSet->MoveNext() && !m_pc3aDBResultSet->IsEof())
			{
				m_pc3aDBResultSet->m_cDBRs.GetFieldValue(COL_NAME_DEVICE_ID,devId,sizeof(devId));
				m_pc3aDBResultSet->m_cDBRs.GetFieldValue(COL_NAME_DEVPRIVILEGE,devPrivilege);

				cUserGrpDevInfo.SetGroupID(*it_str);
				cUserGrpDevInfo.SetDeviceID(string(devId));

				cDevPri.ConvertFromInt(devPrivilege);
				cUserGrpDevInfo.SetDevicePrivilege(cDevPri);

				map<string, vector<TDeviceCapPrivilege> >::iterator mapIt = mapDevCapPri.find(string(devId));
				if (mapIt != mapDevCapPri.end())
				{
					cUserGrpDevInfo.SetDeviceCapPrivilege(mapIt->second);
				}
                else
                {
                    cUserGrpDevInfo.GetDeviceCapPrivilege().clear();
                }
				cUserGrpDevInfo.SetDevDomainID(GetDevDomainIdByDevId(string(devId)));
				vecUserGrpDevInfo.push_back(cUserGrpDevInfo);			
			}
		}
	}

	return true;
}


CResultSet::CResultSet()
{
}

CResultSet::~CResultSet()
{
}

bool CResultSet::MoveNext()
{	
	return (TRUE == m_cDBRs.MoveNext());
}

bool CResultSet::MoveNNext(u32 dwNStep, u32 &dwRealStep)
{
	dwRealStep = 0;
	if (0 != dwNStep)
	{
		while(0 != dwNStep && (FALSE == m_cDBRs.IsEof()))
		{
			m_cDBRs.MoveNext();
			dwNStep--;
			dwRealStep++;
		}
	}
	
	return (FALSE == m_cDBRs.IsEof());
}

u16 CResultSet::GetFieldCount()
{
	return m_cDBRs.GetFieldCount();
}

u32 CResultSet::GetCount()
{
	u32  dwCount = 0;
	m_cDBRs.GetFieldValue(1, dwCount);
	return dwCount;
}

bool CResultSet::IsEof(void)
{
	return (TRUE == m_cDBRs.IsEof());
}

u16 CResultSet::CancelCurRs(void)
{
	//m_cDBRs.CancelCurRs();
	return 0;//SUCCESS_INTER_DB;
}