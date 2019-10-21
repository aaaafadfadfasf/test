#include "cms/dbix/dbix.h"
#include "cms/dbix/dbix_define.h"
#include "cbb/dbi/dbinterface.h"
#include <set>
#include "toolsdata.h"
#include <vector>
#include "cms/commonconfig/cmcfg.h"
using namespace dbix;

#pragma once

class CResultSet
{
public:
	CResultSet();
	virtual ~CResultSet();

public:
	bool MoveNext(void);
	bool MoveNNext(u32 dwNStep, u32 &dwRealStep);
	u16  GetFieldCount();
	u32  GetCount(void);
	bool IsEof(void);
	u16  CancelCurRs(void);

public:
	CDBRs  m_cDBRs;
};

typedef std::set<int> CDeviceChnList;

class CDbOperate
{
public:
	CDbOperate(void);
	~CDbOperate(void);
public:
	bool InitDB();
	
	bool QryAllDomainRelation(vector<CDomainRelationInfo> &vecDomainRelation);
	bool QryAllDevs(vector<TExtDeviceInfo> &vecGbDev);
	void FullDeviceInfo(TExtDeviceInfo &tDev);
	bool GetDevCapInfo(TExtDeviceInfo &tDev);
	bool GetDevCapName(TExtDeviceInfo &tDev);

	bool QryAllUsers(vector<CUserInfo> &vecUsers);
	string GetDevDomainIdByDevId(const string &strDevId);
	bool GetUser2DevCapPrivilege(const string &strUserId,const string &strDevId,vector<TDeviceCapPrivilege> &vecDevCapPri);
	bool QryAllUserDevInfo(vector<CUserDeviceInfo> &vecUserDevInfo);

	bool QryAllUserGrps(vector<string> &vecGrps);
	bool GetUserGrp2DevCapPrivilege(const string &strGrpId,const string &strDevId,vector<TDeviceCapPrivilege> &vecDevCapPri);
	bool QryAllUserGrpDevInfo(vector<CUserGroupDeviceInfo> &vecUserGrpDevInfo);
	bool QryUser2DevCapPrivilege(const string &strUserId, map<string, vector<TDeviceCapPrivilege> > &mapDevCapPri);
	bool QryUserGrp2DeviceCap(const string &strGrpId,map<string, vector<TDeviceCapPrivilege> > &mapDevCapPri);

private:
	string GetAAAConfigFile();
	bool Connect3aDB(const TDBConnectCfg &cfg);
	bool ConnectGbsDB(const TDBConnectCfg &cfg);
	bool DisconnectDB();   //一次断开两个连接
	bool IsConnectDB();    //两个连接的状态
	TDBConnectCfg GetDBConfig(string iniFile);
private:
	static void DBClientCBProc(u16 wErrNum, LPCSTR lpSrvMsg, void *pContext);
	static void DBServerCBProc(u16 wErrNum, LPCSTR lpSrvMsg, void *pContext);
private:
	CDBHandler *m_pc3aDBOprer;     //3A数据库句柄
	CDBHandler *m_pcGbsDBOprer;    //GBS数据库句柄
	CResultSet *m_pc3aDBResultSet;
	CResultSet *m_pcGbsDBResultSet;
	bool m_bConnectDB;
};

