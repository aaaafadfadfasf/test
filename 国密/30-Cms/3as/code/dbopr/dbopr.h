/*
功能：此类是具体的数据库操作类。

设计约束：
1. 一个查询中不能进行类似于“游标”的操作，结果集必须一次取完，中间不允许被打断。
2. 字符串必须使用单引号括起来，不能使用双引号。
3. CDbOpr中函数采用类似于对象化的组织方式，针对相同表或视图的相关方法就近放置。
4. 联动操作安排于具体的业务信息处理点，小的帮助类函数只操作自身数据，不联动操作相关数据(也即策略与机制分离)。
   如此安排是因为联动操作相关数据可能是业务相关的(不同的使用场景联动考虑也不一样)，而帮助类函数主要是为了代码复用而存在的。
*/

#ifndef _DBOPR_H_
#define _DBOPR_H_
#include "inner_common.h"
#include "dbengine.h"
#include "dbcache.h"
#include "tas_redis.h"
#include "sdk/fastdbsdk/fastdb.h"

#define UUID_LENGTH                 32    //Bytes
#define MAX_SEARCH_NAME_LEN         512   //拼音搜索名最大长度（即KDMAAA.tblVideoNameSearch.searchText字段长度）
struct TSqlString
{
    enum
    {
        // 试验过单个字段的长度为30K时，插入和查询字段会失败；linlifen说DBI本身对SQL语句长度无限制
        MAXLEN_SQL_STRING = 20480
    };
public:
    TSqlString()
    {
        Clear();
    }
    operator const s8*()
    {
        return m_achSqlString;
    }
    bool Empty() const
    {
        return (0 == m_dwSqlLen);
    }
    void Clear(void)
    {
        m_achSqlString[0] = '\0';
        m_dwSqlLen = 0;
    }

    void Format(const s8* pszFmt, ...)
#ifdef __GNUC__
        __attribute__((format(printf, 2, 3)))
#endif
    {
        va_list tArgptr;
        va_start(tArgptr, pszFmt);
        m_dwSqlLen = vsnprintf(m_achSqlString, MAXLEN_SQL_STRING-1, pszFmt, tArgptr);
        va_end(tArgptr);
    }
    void AppendString(const s8* pszFmt, ...)
#ifdef __GNUC__
        __attribute__((format(printf, 2, 3)))
#endif
    {
        if (m_dwSqlLen > MAXLEN_SQL_STRING)
        {
            return;
        }

        va_list tArgptr;
        va_start(tArgptr, pszFmt);
        m_dwSqlLen += vsnprintf(m_achSqlString+m_dwSqlLen, MAXLEN_SQL_STRING-1-m_dwSqlLen, pszFmt, tArgptr);
        va_end(tArgptr);
    }
    void EraseLastRedundantComma()
    {
        if (m_dwSqlLen >= 1 && ',' == m_achSqlString[m_dwSqlLen-1])
        {
            m_achSqlString[--m_dwSqlLen] = '\0';
        }
    }
    void EraseLastRedundantAnd()
    {
        if (m_dwSqlLen >= 5 && 0 == strcmp(&m_achSqlString[m_dwSqlLen-5], " and "))
        {
            m_dwSqlLen -= 5;
            m_achSqlString[m_dwSqlLen] = '\0';
        }
    }
    const s8* GetBuf(void)
    {
        return m_achSqlString;
    }
    u32 GetBufLen(void) const
    {
        return m_dwSqlLen;
    }

protected:
    s8  m_achSqlString[MAXLEN_SQL_STRING];
    u32 m_dwSqlLen; // 长度不包括结束符
};

struct CUserDeviceDelInfo
{
	CUserDeviceInfoKey key;
	UUIDString         domainId;
};

struct CUserGroupDeviceDelInfo
{
	CUserGroupDeviceInfoKey key;
	UUIDString              domainId;
};

class CDbOpr
{
    friend class CDbCache;
public:
    // 检查数据库连接，未连接时自动读取连接配置进行连接
    void CheckDbConnection(bool bTryExecTestSql = false);
	void CheckFastdbConnect();
    bool IsConnectedDb(); // 简单判断是否连接数据库
    void Dump();
    static bool Init();
    static void Quit();

public:
    /* 登录管理 */
    void Login(const CTaskMsgTransfer<CLoginTasReq> *pcMsg);
    void CuOnline(const CTaskMsgTransfer<CCuOnlineReq> *pcMsg);

    /* 设备管理(基本信息) */
    void DeviceAdd(const CTaskMsgTransfer<CDeviceAddReq> *pcMsg);
    void DeviceDel(const CTaskMsgTransfer<CDeviceDelReq> *pcMsg);
    ENTasErrno DeviceDelImp(const UUIDString &devId);
    ENTasErrno DevicePreDel(const UUIDString &devId);
    void DeviceMod(const CTaskMsgTransfer<CDeviceModReq> *pcMsg);
	void DeviceBatchChange(const CTaskMsgTransfer<CTasDeviceBatchChangeReq> *pcMsg);
	void DevBatchChg4DevAdd(const CDeviceAddReq& cDevAddReq, ECMS_ERRORCODE& errCode);
	void DevBatchChg4DevMod(const CDeviceModReq& cDevModReq, ECMS_ERRORCODE& errCode);
	void DevBatchChg4DevDel(const CDeviceDelReq& cDevDelReq, ECMS_ERRORCODE& errCode);
	void DevBatchChg4VidSrcMod(const CDeviceCapIndexNameSetReq& cDevCapIdxNameSetReq,ECMS_ERRORCODE& errCode);
	void DevBatchChg4DevGrpDevChg(const CTaskMsgTransfer<CDeviceGroupDeviceAddReq> *pcMsg, ECMS_ERRORCODE& errCode);
	void DevBatchChg4DevModelChg( const CTaskMsgTransfer<CDevCapInfoModReq> *pcMsg,ECMS_ERRORCODE& errCode );
    ENTasErrno DeviceModelPreMod( const UUIDString &tDevId, const CDeviceModelInfo &cOldModel, const CDeviceModelInfo &cNewModel );
    ENTasErrno DeviceCapIndexNameUpdate( const UUIDString &tDevId, const CDeviceModelInfo &cOldModel, const CDeviceModelInfo &cNewModel );
    ENTasErrno DevGrpDevUpdate( const UUIDString &tDevId, const CDeviceModelInfo &cOldModel, const CDeviceModelInfo &cNewModel );
	ENTasErrno PtzLockInfoUpdate( const UUIDString &tDevId, const CDeviceModelInfo &cOldModel, const CDeviceModelInfo &cNewModel );
	ENTasErrno Tv2DecoderUpdate(const CTaskMsgTransferBase *pcSrcTransfer, const UUIDString &tDevId, const CDeviceModelInfo &cOldModel, const CDeviceModelInfo &cNewModel );
    ENTasErrno CuserDevUpdate( const UUIDString &tDevId, const CDeviceModelInfo &cOldModel, const CDeviceModelInfo &cNewModel );
    ENTasErrno CuserDevGet( const UUIDString &tDevId, const CDeviceModelInfo &cNewModel, map<UUIDString, CUserDeviceInfo> &mapInfo );
    ENTasErrno CuserDevRestore( const CDeviceModelInfo &cNewModel, const map<UUIDString, CUserDeviceInfo> &mapUserDeviceInfo );
    ENTasErrno CuserGrpDevUpdate( const UUIDString &tDevId, const CDeviceModelInfo &cOldModel, const CDeviceModelInfo &cNewModel );
    ENTasErrno CuserGrpDevGet( const UUIDString &tDevId, const CDeviceModelInfo &cNewModel, map<UUIDString, CUserGroupDeviceInfo> &mapInfo );
    ENTasErrno CuserGrpDevRestore( const CDeviceModelInfo &cNewModel, const map<UUIDString, CUserGroupDeviceInfo> &mapUserGroupDeviceInfo );
    ENTasErrno CuserGrpDevItemAdd( const CUserGroupDeviceInfo &cInfo );
	ENTasErrno CuserGrpDevItemDel( const CUserGroupDeviceInfoKey &cInfo );
    ENTasErrno DeviceModelPreMod4Parent(const UUIDString &tDevId, const UUIDString &tDevDomainId, const UUIDString &tOldModelId, const UUIDString &tNewModelId);
    void DeviceQry(const CTaskMsgTransfer<CDeviceQryReq> *pcMsg);
    void UpdateDevTreeVer(const UUIDString &strDevId,const UUIDString &devtreeId=""); // 设备号为空时更新所有用户和用户组的
	void UpdateTreesVerByDevId(const UUIDString &devId);//(慎用)自定义设备数上设备被退网或设备视频源个数减少时使用(内部实现有判定逻辑，调用后不一定会修改版本号)
    ENTasErrno PrepareDeviceQrySql(const CDeviceQryReq &cReq, TSqlString &tSqlSelect, TSqlString &tSqlCount);
    ENTasErrno FetchDeviceQryRecord(CDeviceInfo &device); // 设备型号无法补全时返回失败
    // 可能的错误：CMS_SUCCESS, ERR_TAS_DBERROR，ERR_TAS_RECORD_NOT_EXISTS
    ENTasErrno DeviceItemQry(const UUIDString &strId, CDeviceInfo &cInfo);
    ENTasErrno DeviceItemAdd(const CDeviceInfo &cInfo);
    ENTasErrno DeviceItemDel(const UUIDString &strDevId);
    ENTasErrno DeviceItemMod(const CDeviceInfo &cInfo);
    void DeviceIdQry(const CTaskMsgTransfer<CDeviceQryReq> *pcMsg);

    /* 设备管理(基本信息, 电视墙) */
    void TvwallAdd(const CTaskMsgTransfer<CTvwallAddReq> *pcMsg);
    void TvwallDel(const CTaskMsgTransfer<CTvwallDelReq> *pcMsg);
    ENTasErrno TvwallDelImp(const UUIDString &tvwallId);
    void TvwallMod(const CTaskMsgTransfer<CTvwallModReq> *pcMsg,bool bUseInner=false);
    void TvwallQry(const CTaskMsgTransfer<CTvwallQryReq> *pcMsg);
    ENTasErrno PrepareTvwallQrySql(const CTvwallQryReq &cReq, TSqlString &tSqlSelect, TSqlString &tSqlCount);
    void FetchTvwallQryRecord(CTvwallInfo &cTvwall);
    ENTasErrno ValidateDecoderTvBinder(const vector<TvDecoderBinder> &cBinder, const UUIDString *ptExcludeTvwallId);
    ENTasErrno DecoderTvBinderDel(const UUIDString &cDecoderId);
    ENTasErrno TvwallItemAdd(const CTvwallInfo &cTvwall);
    ENTasErrno TvwallItemDel(const UUIDString &cTvwallId);
    ENTasErrno TvwallItemMod(const CTvwallInfo &cTvwall);
    // 可能的错误：CMS_SUCCESS, ERR_TAS_DBERROR，ERR_TAS_RECORD_NOT_EXISTS
    ENTasErrno TvwallItemQry(const UUIDString &cTvwallId, CTvwallInfo &cTvwall);
    bool IsExistSameNameTvwall(const string &strName, const UUIDString *ptExcludeTvwallId = NULL);

    /* 设备管理(设备型号) */
    bool GetDeviceTypeInfo(u32 dwType, TInnerDeviceTypeInfo &cInfo);
    void DeviceModelAdd(const CTaskMsgTransfer<CDeviceModelAddReq> *pcMsg);
    void DeviceModelDel(const CTaskMsgTransfer<CDeviceModelDelReq> *pcMsg);
    void DeviceModelQry(const CTaskMsgTransfer<CDeviceModelQryReq> *pcMsg);
	string DeviceModelAdd(const CTaskMsgTransfer<CDeviceModelAddReq> *pcMsg,bool bUseInner);
    ENTasErrno PrepareDeviceModelQrySql(const CDeviceModelQryReq &cReq, TSqlString &tSqlSelect, TSqlString &tSqlCount);
    void FetchDeviceModelQryRecord(CDeviceModelInfo &model);
    // 可能的错误：CMS_SUCCESS, ERR_TAS_DBERROR，ERR_TAS_RECORD_NOT_EXISTS
    ENTasErrno DeviceModelItemQry(const string &strModelId, CDeviceModelInfo &cInfo);
    ENTasErrno DeviceModelItemAdd(const CDeviceModelInfo &cInfo, bool bValidateCapId);
    ENTasErrno DeviceModelItemDel(const string &strModelId);
    // 可能的错误：CMS_SUCCESS, ERR_TAS_DBERROR，ERR_TAS_RECORD_NOT_EXISTS
    ENTasErrno DeviceModelItemQryByDevId(const string &strDevId, CDeviceModelInfo &cInfo);
	// 获取指定域下面同名的设备型号信息
	ENTasErrno GetExistDeviceModelId(const string &strDomainId, const string &strModelName, TCreater tCreater, string &strModelId);
	// 获取相同信息的设备型号Id
	ENTasErrno GetExistDeviceModelId(const string &strDomainId, s32 nType, const string &strModelName,bool bIgnoreModelName, const string &strManufacturer, 
		TCreater tCreater, const vector<TDeviceCapInfo> &vecCapInfo, string &strModelId);
    ENTasErrno IsDeviceModelUsing(const string &modelId);// 返回ERR_TAS_DEVICE_MODEL_USING,ERR_TAS_DBERROR,CMS_SUCCESS
	bool CompareCapInfo(const vector<TDeviceCapInfo> &vecCapInfo1, const vector<TDeviceCapInfo> &vecCapInfo2);
	void DevCapInfoMod(const CTaskMsgTransfer<CDevCapInfoModReq> *pcMsg);

    /* 设备管理(设备能力索引名称) */
    void DeviceCapIndexNameGet(const CTaskMsgTransfer<CDeviceCapIndexNameGetReq> *pcMsg);
    void DeviceCapIndexNameSet(const CTaskMsgTransfer<CDeviceCapIndexNameSetReq> *pcMsg);
    void DeviceCapIndexNameQry(const CTaskMsgTransfer<CDeviceCapIndexNameQryReq> *pcMsg);
    ENTasErrno PrepareDeviceCapIndexNameQrySql(const CDeviceCapIndexNameQryReq &cReq, TSqlString &tSqlSelect, TSqlString &tSqlCount);
	ENTasErrno ModDeviceVidSrcName(const UUIDString& devId, int nIndex, const string& VidSrcName);
    void FetchDeviceCapIndexNameQryRecord(TDeviceCapName &devCapName);
    // nCapIndex为-1时表示nCapId能力类型的所有索引名称，可能的错误：CMS_SUCCESS, ERR_TAS_DBERROR，ERR_TAS_RECORD_NOT_EXISTS
    ENTasErrno GetDeviceCapIndexName(const string &strDevId, s32 nCapId, s32 nCapIndex, TDeviceCapName &tCapIndexName);
    // 相同能力的能力索引不合并，可能的错误：CMS_SUCCESS, ERR_TAS_DBERROR，ERR_TAS_RECORD_NOT_EXISTS
    ENTasErrno GetDeviceCapIndexs(const string &strDevId, vector<TDeviceCapIndexs> &tCapIndexs);
    ENTasErrno DeviceCapIndexNameDel(const UUIDString &devId);
    ENTasErrno DeviceCapIndexNameItemSet(const TDeviceCapName &cInfo, bool bAllowAppendCapNameWhenEmpty=false); // 不存在时添加，存在时修改
	
    /* 视频源拼音搜索 */
    ENTasErrno DevicePinyinSearchVideoNameInit(const string &devId, int videoNum, int videoIndex=0, const char* szName=NULL); // 初始化设备的视频源搜索名称
    ENTasErrno DevicePinyinSearchVideoNameDel(const string &devId, int videoIndex=-1); // 删除大于videoIndex的视频源搜索名称
    ENTasErrno DevicePinyinSearchVideoNameSet(const string &devId, vector<int> videoIndexs, vector<string> videoNames);

    /* 设备管理(设备厂商) */
    void DeviceManufacturerAdd(const CTaskMsgTransfer<CDeviceManufacturerAddReq> *pcMsg);
    void DeviceManufacturerDel(const CTaskMsgTransfer<CDeviceManufacturerDelReq> *pcMsg);
    void DeviceManufacturerQry(const CTaskMsgTransfer<CDeviceManufacturerQryReq> *pcMsg);
    ENTasErrno PrepareDeviceManufacturerQrySql(const CDeviceManufacturerQryReq &cReq, TSqlString &tSqlSelect, TSqlString &tSqlCount);
    void FetchDeviceManufacturerQryRecord(CDeviceManufacturerInfo &manufacturer);

    /* 设备管理(设备完整内容和设备权限) */
    void CuserDevicefullinfoAndPrivilegeQry(const CTaskMsgTransfer<CCuserDevicefullinfoAndPrivilegeQryReq> *pcMsg);
    ENTasErrno PrepareCuserDevicefullinfoAndPrivilegeQrySql(const CCuserDevicefullinfoAndPrivilegeQryReq &cReq, TSqlString &tSqlSelect, TSqlString &tSqlCount,
		bool &bNeedQryHint);
    ENTasErrno FetchCuserDevicefullinfoAndPrivilegeQryRecord(TDevicefullinfoAndPrivilege &record, bool bNewRecord, bool &bCombineFinished); // 设备型号无法补充时返回失败
    void CuserExportDevData(const CTaskMsgTransfer<CFileExportReq> *pcMsg);

	//视频源信息查询
	void VidsrcInfoQry(const CTaskMsgTransfer<CVidsrcInfoQryReq> *pcMsg);
	ENTasErrno PrepareVidsrcInfoQrySql(const UUIDString &strDevId, const int vidsrcIdx, TSqlString &tSqlSelect, TSqlString &tSqlSelect2);
    
	/* CU用户管理 */
    void CuserAdd(const CTaskMsgTransfer<CUserAddReq> *pcMsg);
    void CuserDel(const CTaskMsgTransfer<CUserDelReq> *pcMsg);
    void CuserMod(const CTaskMsgTransfer<CUserModReq> *pcMsg);
    void CuserQry(const CTaskMsgTransfer<CUserQryReq> *pcMsg);
	string GetNewUserGBID();
    void UpdateCuserDevTreeVer(const UUIDString &strUserId); // strUserId为空时表示超级管理员编号
    ENTasErrno PrepareCuserQrySql(const CUserQryReq &cReq, TSqlString &tSqlSelect, TSqlString &tSqlCount);
    void FetchCuserQryRecord(CUserInfo &cInfo);
    bool GetCuserInfo(const UUIDString &userId, CUserInfo &cInfo);
    // 可能的错误：CMS_SUCCESS, ERR_TAS_DBERROR，ERR_TAS_RECORD_NOT_EXISTS
    ENTasErrno GetCuserInfoByName(const string &strName, CUserInfo &cInfo);
    bool IsExistCuserInCertainUsergrp(const UUIDString &cusergrpid, bool &bExistFlag);
    bool GetCuserGrpId(const UUIDString &strUserId, UUIDString &strGrpId);
	ENTasErrno CuserQryByUserGrp(const UUIDString &strUserGrpId,vector<CUserInfo> &cUserInfos);
    
    /* 用户设备划归 */
    void CuserDevAdd(const CTaskMsgTransfer<CUserDeviceAddReq> *pcMsg);
    ENTasErrno CuserDevAddDefault(const UUIDString &devId, const UUIDString &devModelId); // 给超级管理员划归设备权限
    void CuserDevDel(const CTaskMsgTransfer<CUserDeviceDelReq> *pcMsg);
    ENTasErrno CuserDevDelByDevId(const UUIDString &devId);
    ENTasErrno CuserDevDelByUserId(const UUIDString &userId);
    void CuserDevMod(const CTaskMsgTransfer<CUserDeviceModReq> *pcMsg);
    void CuserDevQry(const CTaskMsgTransfer<CUserDeviceQryReq> *pcMsg);
    ENTasErrno PrepareCuserDevQrySql(const CUserDeviceQryReq &cReq, TSqlString &tSqlSelect, TSqlString &tSqlCount);
    void FetchCuserDevQryRecord(CUserDeviceInfo &cInfo);
    ENTasErrno GetCUserDeviceInfo(const UUIDString &cUserId, const UUIDString &cDevId, CUserDeviceInfo &cDevInfo);
    ENTasErrno ValidateCurrentUserDevPrivilege(const string &strSessionId, const CUserDeviceInfo &cDevInfo);
    ENTasErrno CuserDevItemAdd(const CUserDeviceInfo &cInfo);   // 支持重复添加，发现重复时自动不进行添加
    ENTasErrno CuserDevItemDel(const CUserDeviceInfoKey &cInfo);
    ENTasErrno CuserDevItemMod(const CUserDeviceInfo &cInfo);


	//设备树管理  [3/10/2016 pangubing]
	void DevTreeAdd(const CTaskMsgTransfer<CDevTreeAddReq> *pcMsg);
	void DevTreeDel(const CTaskMsgTransfer<CDevTreeDelReq> *pcMsg);
	void DevTreeMod(const CTaskMsgTransfer<CDevTreeModReq> *pcMsg);
	void DevTreeQry(const CTaskMsgTransfer<CDevTreeQryReq> *pcMsg);
	void DevTreeDevGrpAssign(const CTaskMsgTransfer<CDevTreeDevGrpAssignReq> *pcMsg);
	ENTasErrno GetFitDev4Gb(const UUIDString& dstTreeId,const UUIDString& srcTreeId, const UUIDString& devGrpId, 
		bool bOnlyOnline, bool bUnAssigned, bool bOnlyEnable, vector<TVidSrcGbInfoExt>& fitDevs);
	bool IsDevVidSrcAssigned(const UUIDString& devTreeId, const UUIDString& devId, const int vidSrcIdx);
	bool GetOriginDevGbid(const UUIDString& devId, string& strGbid);
	bool IsDevTreeCanBeUse4Gb(const UUIDString& devTreeId);
	bool GetDevTreeInfo(const UUIDString &devtreeId, CDevTreeInfo &cInfo);
	void FetchDevTreeQryRecord(CDevTreeInfo &cInfo);
	ENTasErrno PrepareDevTreeQrySql(const CDevTreeQryReq &cReq, TSqlString &tSqlSelect, TSqlString &tSqlCount);
	ENTasErrno DevTreeListQry(vector<CDevTreeInfo> &cInfoList);

	void DeviceGrpPathQry(const CTaskMsgTransfer<CDeviceGrpPathQryReq> *pcMsg);
	ENTasErrno PrepareDeviceGrpPathQrySql(const CDeviceGrpPathQryReq &cReq, TSqlString &tSqlSelect, TSqlString &tSqlCount);

	//指定树指定分组下所有分组和设备（递归）信息查询
	void DevGrpChildrenQry(const CTaskMsgTransfer<CDeviceGroupChildrenQryReq> *pcMsg);
	ENTasErrno DevGrpChildrenQry(const UUIDString& devTreeId, const UUIDString& devGrpId, 
		vector<CDeviceGroupInfo>& devGrpInfos, vector<CDeviceGroupDeviceInfo4Gb>& devInfos);

	// PTZ锁管理 [4/12/2016 pangubing]
	void PtzLockAdd(const CTaskMsgTransfer<CPtzLockAddReq> *pcMsg);
	void PtzLockDel(const CTaskMsgTransfer<CPtzLockDelReq> *pcMsg,bool bIsDelInner=false);
	void PtzLockQry(const CTaskMsgTransfer<CPtzLockQryReq> *pcMsg);
	ENTasErrno PreparePtzLockQrySql(const CPtzLockQryReq &cReq, TSqlString &tSqlSelect, TSqlString &tSqlCount);
	bool GetPtzLockInfo(const UUIDString &devId,int nVidSrcId, CPtzLockInfo &cInfo);
	void FetchPtzLockQryRecord(CPtzLockInfo &cInfo);
	bool DeletePtzLock(const UUIDString &cDeviceId,int nVidSrcId,const UUIDString &cUserId="");
	bool DeleteUserPtzLockInner(const CTaskMsgTransferBase *pcMsg,const UUIDString &cUserId,const UUIDString &cDeviceId,const vector<TDeviceCapIndexs>& tCapIndexs);
	bool DeletePtzLockInner(const CTaskMsgTransferBase *pcMsg,const UUIDString &cDeviceId,int nVidSrcId);
	bool GetUserDevCapInfo(const CUserInfo &cUserInfo,const UUIDString &cDeviceId,int nVidSrcId,CVideosourcePrivilege &cDevCapIndexPrivilege);
	bool ClearUserPtzLockInfo(const string &strUserId);
	ENTasErrno QryUserPtzLockInfo(const string &strUserId,vector<CPtzLockInfo> &cPtzLockInfos);
	ENTasErrno PtzLockDelByDevId(const UUIDString &devId);

	/* 按组划归 */
	void CuserDeviceGroupAdd(const CTaskMsgTransfer<CUserDeviceGroupAddReq> *pcMsg);
	void CuserDeviceGroupDel(const CTaskMsgTransfer<CUserDeviceGroupDelReq> *pcMsg);
	void CuserGroupDeviceGroupAdd(const CTaskMsgTransfer<CUserGroupDeviceGroupAddReq> *pcMsg);
	void CuserGroupDeviceGroupDel(const CTaskMsgTransfer<CUserGroupDeviceGroupDelReq> *pcMsg);
	void CDeviceAutoAssignToUserSet(const CTaskMsgTransfer<CDeviceAutoAssignToUserSetReq> *pcMsg);
	void CDeviceAutoAssignToUserGroupSet(const CTaskMsgTransfer<CDeviceAutoAssignToUserGroupSetReq> *pcMsg);
	void CDeviceAutoAssignToUserQry(const CTaskMsgTransfer<CDeviceAutoAssignToUserQryReq> *pcMsg);
	void CDeviceAutoAssignToUserGroupQry(const CTaskMsgTransfer<CDeviceAutoAssignToUserGroupQryReq> *pcMsg);

	/* 按组划归中抽象出来的几个原子操作 */
	ENTasErrno AssignDeviceByDevice(const CTaskMsgTransferBase *pcSrcTransfer, const CEventReq &cReq, const CUserDeviceInfo &cInfo);
	ENTasErrno AssignDeviceByDevice(const CTaskMsgTransferBase *pcSrcTransfer, const CEventReq &cReq, const CUserGroupDeviceInfo &cInfo);
	ENTasErrno AssignDeviceByDeviceGroup(const CTaskMsgTransferBase *pcSrcTransfer, const CEventReq &cReq, 
		const string &strGroupId, const string &strOperId, const string &strOwnerId, u8 byOwnerType, 
        bool bAssignPriv=false, const CEncoderPrivilege *pencPriv=NULL, const CVideosourcePrivilege *pvideoPriv=NULL);
	ENTasErrno AssignDeviceByDeviceGroup(const CTaskMsgTransferBase *pcSrcTransfer, const CEventReq &cReq, 
		const string &strGroupId, const string &strOperId, map<string, u8> cOwners);
	ENTasErrno RevokeDeviceByDevice(const CTaskMsgTransferBase *pcSrcTransfer, const CEventReq &cReq, const CUserDeviceDelInfo &cDelInfo);
	ENTasErrno RevokeDeviceByDevice(const CTaskMsgTransferBase *pcSrcTransfer, const CEventReq &cReq, const CUserGroupDeviceDelInfo &cDelInfo);
	ENTasErrno RevokeDeviceByDeviceGroup(const CTaskMsgTransferBase *pcSrcTransfer, const CEventReq &cReq, 
		const string &strGroupId, const string &strOperId, const string &strOwnerId, u8 byOwnerType = 0);
	ENTasErrno RevokeDeviceByDeviceGroup(const CTaskMsgTransferBase *pcSrcTransfer, const CEventReq &cReq, 
		const string &strGroupId, const string &strOperId, map<string, u8> cOwners);
	ENTasErrno RegistAutoAssignDeviceGroup(const string &strGroupId, const string &strOwnerId, u8 byOwnerType = 0);
	ENTasErrno RegistAutoAssignDeviceGroup(const string &strGroupId, map<string, u8> cOwners);
	ENTasErrno UnRegistAutoAssignDeviceGroup(const string &strGroupId, const string &strOwnerId, u8 byOwnerType = 0);
	ENTasErrno UnRegistAutoAssignDeviceGroup(const string &strGroupId);

	bool IsAutoAssignDeviceGroup(const string &strGroupId, const string &strOwnerId, u8 byOwnerType = 0);
	bool IsAutoAssignDeviceGroup(const string &strDevId, const vector<TDeviceCapIndexs> &vctDevCap, const string &strOwnerId, u8 byOwnerType = 0);
	bool IsTopAutoAssignDeviceGroup(const string &strGroupId, const string &strOwnerId, u8 byOwnerType = 0);
	void GetTopAutoAssignDeviceGroup(const string &strGroupId, map<string, u8> &cOwners);
	void GetAutoAssignDeviceGroup(const string &strGroupId, map<string, u8> &cOwners);
	void ClearUserAutoAssignDeviceGroup(const string &strOwnerId, u8 byOwnerType = 0);
	ENTasErrno AutoAssignDevice(const CTaskMsgTransferBase *pcSrcTransfer, const CEventReq &cReq, 
		const string &strDevId, const vector<TDeviceCapIndexs> &vctCap, const string &strGroupId);
	ENTasErrno AutoAssignDevice(const CTaskMsgTransferBase *pcSrcTransfer, const CEventReq &cReq, 
		const string &strDevId, const string &strDevDomainId, const CDeviceModelInfo &cModel, const string &strGroupId);
	ENTasErrno AutoRevokeDevice(const CTaskMsgTransferBase *pcSrcTransfer, const CEventReq &cReq, 
		const string &strDevId, const vector<TDeviceCapIndexs> &vctCap, const string &strGroupId);

	// 	ENTasErrno AutoAssignDevice(const string &strDeviId, const string &strGroupId);
	// 	ENTasErrno AutoRevokeDevice();
	// 	ENTasErrno IsAutoAssignDeviceGroup();
	
//	ENTasErrno GetDevicesByUserIdAndGroupId(const UUIDString &strUserId, const UUIDString &strGroupId, vector<UUIDString> &vctGroups);
	
	/* 按组划归相关的数据转换方法 */
	ENTasErrno GetAdminUserId(string &strUserId);
	ENTasErrno GetGroupsByGroupId(const UUIDString &strGroupId, vector<UUIDString> &vctGroups,const string& devTreeId="");
	ENTasErrno GetCuserDevGroupAddInfo(const string &strGroupId, const string &strOperId, const string &strOwnerId, 
		map<UUIDString, CUserDeviceInfo> &cInfos);
	ENTasErrno GetCuserDevGroupDelInfo(const string &strGroupId, const string &strOperId, const string &strOwnerId, 
		map<UUIDString, CUserDeviceDelInfo> &cInfos);
	ENTasErrno GetCuserGroupDevGroupAddInfo(const string &strGroupId, const string &strOperId, const string &strOwnerId, 
		map<UUIDString, CUserGroupDeviceInfo> &cInfos);
	ENTasErrno GetCuserGroupDevGroupDelInfo(const string &strGroupId, const string &strOperId, const string &strOwnerId, 
		map<UUIDString, CUserGroupDeviceDelInfo> &cInfos);

	/* 查询模板中需要的几个函数 */
	ENTasErrno PrepareDeviceAutoAssignToUserQrySql(const CDeviceAutoAssignToUserQryReq &cReq, TSqlString &tSqlSelect, TSqlString &tSqlCount);
	ENTasErrno PrepareDeviceAutoAssignToUserGroupQrySql(const CDeviceAutoAssignToUserGroupQryReq &cReq, TSqlString &tSqlSelect, TSqlString &tSqlCount);
	void FetchDeviceAutoAssignToOwnerQryRecord(CDeviceAutoAssignInfo &cInfo);


    /* 设备分组管理 */
    void DevGrpAdd(const CTaskMsgTransfer<CDeviceGroupAddReq> *pcMsg);
    void DevGrpDel(const CTaskMsgTransfer<CDeviceGroupDelReq> *pcMsg);
    void DevGrpMod(const CTaskMsgTransfer<CDeviceGroupModReq> *pcMsg);
    void DevGrpQry(const CTaskMsgTransfer<CDeviceGroupQryReq> *pcMsg);
    ENTasErrno PrepareDevGrpQrySql(const CDeviceGroupQryReq &cReq, TSqlString &tSqlSelect, TSqlString &tSqlCount);
    void FetchDevGrpQryRecord(CDeviceGroupInfo &cInfo, bool &bExistDev, bool CanBeUse4Gb);
	bool IsDevGrpCivilCode(const string& strGrpGbid);
	bool IsDevGrpBussinessGrp(const string& strGrpGbid);
	void FetchDevGrpQryRecord(CDeviceGroupInfo &cInfo, bool CanBeUse4Gb = false);
    bool IsExistSameNameDevGrp(const string &strName, const string &strParentDevGrpId,const string *pstrExcludeDevGrpId = NULL,const string& devTreeId="");
    // 可能的错误：CMS_SUCCESS, ERR_TAS_DBERROR，ERR_TAS_RECORD_NOT_EXISTS
    ENTasErrno DevGrpItemQry(const UUIDString &strId, CDeviceGroupInfo &cInfo,const string& devTreeId="");
	ENTasErrno DevGrpItemQryOnly(const UUIDString &strId, CDeviceGroupInfo &cInfo,const string& devTreeId="");
    ENTasErrno DevGrpItemAdd(const CDeviceGroupInfo &cInfo,const string& devTreeId="");
	bool IsVidSrcInDevGrp(const UUIDString& devTreeId, const UUIDString& devGrpId, const UUIDString& devId, const u32 dwVidSrcIndex);

	ENTasErrno DevGrpItemDel(const UUIDString &strDevGrpId,const string &devTreeId="");
    ENTasErrno DevGrpItemMod(const CDeviceGroupInfo &cInfo);
    ENTasErrno DomainAuthorizationDevGrpId(string &strId);   // 跨域授权设备的分组编号，可能的错误：CMS_SUCCESS, ERR_TAS_DBERROR
    ENTasErrno MoveupDevGrps(const UUIDString &grpId, const UUIDString &parentGrpId=""); // 上移指定设备组下的设备组至指定设备组的父组

    /* 设备分组设备划归 */
    void DevGrpDevAdd(const CTaskMsgTransfer<CDeviceGroupDeviceAddReq> *pcMsg);
    void DevGrpDevDel(const CTaskMsgTransfer<CDeviceGroupDeviceDelReq> *pcMsg);
    void DevGrpDevQry(const CTaskMsgTransfer<CDeviceGroupDeviceQryReq> *pcMsg);
	void DevGrpDevQry4Gb(const CTaskMsgTransfer<CDeviceGroupDeviceQry4GbReq> *pcMsg);
    ENTasErrno PrepareDevGrpDevQrySql(const CDeviceGroupDeviceQryReq &cReq, TSqlString &tSqlSelect, TSqlString &tSqlCount);
	ENTasErrno PrepareDevGrpDevQry4GbSql( const CDeviceGroupDeviceQry4GbReq &cReq, TSqlString &tSqlSelect, TSqlString &tSqlCount );
    void FetchDevGrpDevQryRecord(CDeviceGroupDeviceInfo &cInfo);
	void FetchDevGrpDevQryRecord4Gb( CDeviceGroupDeviceInfo4Gb &cInfo );
    void DevGrpDevDel4Favorite(const string &userId, const string &devid, int videoid);
    void DevGrpDevDel4Favorite(const string &userId, const CUserDeviceInfoKey &key);

	//设备组设备排序
	void DevGrpDevOrder(const CTaskMsgTransfer<CDeviceGroupDeviceOrderReq> *pcMsg);
	ENTasErrno DealInnerGrpTop(const CDeviceGroupDeviceOrderReq& cReq);
	ENTasErrno DealInnerGrpBottom(const CDeviceGroupDeviceOrderReq& cReq);
	ENTasErrno DealInnerGrpMove(const CDeviceGroupDeviceOrderReq& cReq);
	ENTasErrno DealInnerDevTop(const CDeviceGroupDeviceOrderReq& cReq);
	ENTasErrno DealInnerDevBottom(const CDeviceGroupDeviceOrderReq& cReq);
	ENTasErrno DealInnerDevMove(const CDeviceGroupDeviceOrderReq& cReq);
	ENTasErrno SetVidSrcDevSeq(const UUIDString& devTreeId, const UUIDString& devGrpId, const UUIDString& devId, const int vidSrc, const double dDevSeq);
	bool QryVidSrcDevSeq(const UUIDString& devTreeId, const UUIDString& devGrpId, const UUIDString& devId, const int vidSrc, double &dDevSeq);
	bool GetDevGrpNextDevSeq(const UUIDString &devTreeId, const UUIDString &devGrpId,const UUIDString& devId,int vidSrcIndex, double& devSeq);
	bool GetDevGrpNextDevSeq4SameDev(const UUIDString &devTreeId, const UUIDString &devGrpId,const UUIDString& devId,int vidSrcIndex, double& devSeq); //同一分组内部同一设备
	bool QryNeerDevSeq(const UUIDString& devTreeId, const UUIDString& devGrpId, const double referDevSeq,
		bool bLarger, //true--选择大于referDevSeq的值，false--选择小于referDevSeq的值。
		double& devSeq); //查询值最接近referDevSeq的值
	bool QryRangeDevSeq(const UUIDString& devTreeId, const UUIDString& devGrpId, 
		bool bMax,      //获取符合条件的最大（bMax=true）、最小（bMax=false）的devSeq值
		double& devSeq,
		const UUIDString devId="");  //devId不为空时，范围缩小到该分组&&该设备 
	ENTasErrno DisperseDevSeq(const UUIDString &devTreeId, const UUIDString &devGrpId);

	//分组设备统计
	ENTasErrno GetDevGrpDevSumNum( const string &strTreeId, const string& strGrpId,int& dwSumNum);
	void DevGrpDevSumNumQry(const CTaskMsgTransfer<CDeviceGrpDevSumNumReq> *pcMsg);
	//设备反查询
	void DevGbInfoQry(const CTaskMsgTransfer<CDeviceGbinfoQryReq> *pcMsg);
	void FetchDeviceGbInfoQry(CDeviceFullGbInfo &cInfo );
	void FetchDeviceGbFullGrpInfoQry(CDeviceFullGbInfo &cInfo,const string& strTreeId);
	//设备分组反查询
	void DevGrpGbInfoQry(const CTaskMsgTransfer<CDevgrpGbinfoQryReq> *pcMsg);
	void FetchDeviceGrpGbInfoQry(TDevGrpFullGbInfos &cInfo,const string& strTreeId);
	//分组匹配
	void DevGrpMatch(const CTaskMsgTransfer<CDeviceMatchGbidReq> *(pcMsg));
	//设备gbid下一个错误查询
	void DevGbidNextErrQry(const CTaskMsgTransfer<CDeviceGbidErrGetReq> *(pcMsg));
	void FetchDeviceErrQry(TDevGbidErrItem& tDevErr);
	//设备gbid操作
	void DevGrp2DevGbidOperate(const CDbMsgTransfer<CDevGrp2DevGbidOperateReq> *pcMsg);
	//设备分组gbid操作
	void DevGrpGbidOperate(const CDbMsgTransfer<CDevGrpGbidOperateReq> *pcMsg);
	//指定树，分组下所有设备查询
	void DevGrpAllChildDevQry(const CDbMsgTransfer<CDevGrpAllChildDevQryReq> *pcMsg);
	void FetchDevGrpAllDevQry(CDeviceGroupDeviceInfo4Gb& cInfo);
	//数据库转为国标推送xml文件
	void TableDataToPushXmlFile(const CDbMsgTransfer<string> *pcMsg);

    ENTasErrno DevGrpDevItemAdd(const CDeviceGroupDeviceInfo &cInfo,bool bAddDevWithGrp=false,const string &grpTreeId="",const string &devTreeId=""); // 添加到新组时旧组中的划归被删除
    ENTasErrno DevGrpDevItemDel(const CDeviceGroupDeviceInfo &cInfo); // 删除旧组中的划归时划归设备至设备组所属域的默认组
    // 数据记录不合并，可能的错误：CMS_SUCCESS, ERR_TAS_DBERROR，ERR_TAS_RECORD_NOT_EXISTS
    ENTasErrno GetDeviceGroupDeviceInfoKeys(const string &strDevId, vector<CDeviceGroupDeviceInfoKey> &tKeys);
    // 将指定设备加入到指定域的未分组
    ENTasErrno DevGrpDevAddDefault(const UUIDString &devId, const UUIDString &devModelId, const string *pstrCertainDomainId=NULL); 
    // 将指定设备加入到跨域授权设备分组
    ENTasErrno DevGrpDevAddDomainAuthorization(const UUIDString &devId, const UUIDString &devModelId);
    ENTasErrno DevGrpDevDelByDevId(const UUIDString &devId,bool bNeedDelTreeInfo=true);
    ENTasErrno DevGrpDevExistsTest(const UUIDString &grpId, bool &bExists);
    ENTasErrno MoveupDevGrpDevs(const UUIDString &grpId, const UUIDString &parentGrpId,const string &devTreeId); // 上移指定设备组下的设备至指定设备组的父组

	//网关数据直删
	ENTasErrno DelGateWayData(const CTaskMsgTransfer<CGatewayDelReq> *pcMsg);
	ENTasErrno DelGateWayDevice(const CTaskMsgTransfer<CGatewayDelReq> *pcMsg, vector<CDeviceInfo>& vecDeviceDel);
	ENTasErrno DelGateWayDevModel(const CTaskMsgTransfer<CGatewayDelReq> *pcMsg, vector<UUIDString>& vecModelDel);
	ENTasErrno DelGateWayDevGrp(const CTaskMsgTransfer<CGatewayDelReq> *pcMsg, vector<UUIDString>& vecDevGrpDel);
	void       GateWayDataDelNtf(const CTaskMsgTransfer<CGatewayDelReq> *pcMsg, vector<CDeviceInfo>& vecDeviceDel, 
		vector<UUIDString>& vecModelDel, vector<UUIDString>& vecDevGrpDel);



    /* CU用户分组管理 */
    void CuserGrpAdd(const CTaskMsgTransfer<CUserGroupAddReq> *pcMsg);
    void CuserGrpDel(const CTaskMsgTransfer<CUserGroupDelReq> *pcMsg);
    void CuserGrpMod(const CTaskMsgTransfer<CUserGroupModReq> *pcMsg);
    void CuserGrpQry(const CTaskMsgTransfer<CUserGroupQryReq> *pcMsg);
    void UpdateCuserGrpDevTreeVer(const UUIDString &strUserGrpId);
    ENTasErrno PrepareCuserGrpQrySql(const CUserGroupQryReq &cReq, TSqlString &tSqlSelect, TSqlString &tSqlCount);
    void FetchCuserGrpQryRecord(CUserGroupInfo &cInfo);
    bool IsExistSameNameCuserGrp(const string &strName, const string *pstrExcludeCuserGrpId = NULL);
    bool GetCuserGrpInfo(const UUIDString &strId, CUserGroupInfo &cInfo);
    /* CU用户分组设备划归 */
    void CuserGrpDevAdd(const CTaskMsgTransfer<CUserGroupDeviceAddReq> *pcMsg);
    void CuserGrpDevDel(const CTaskMsgTransfer<CUserGroupDeviceDelReq> *pcMsg);
    ENTasErrno CuserGrpDevDel(const UUIDString &devId);
    void CuserGrpDevMod(const CTaskMsgTransfer<CUserGroupDeviceModReq> *pcMsg);
    void CuserGrpDevQry(const CTaskMsgTransfer<CUserGroupDeviceQryReq> *pcMsg);
    ENTasErrno PrepareCuserGrpDevQrySql(const CUserGroupDeviceQryReq &cReq, TSqlString &tSqlSelect, TSqlString &tSqlCount);
    void FetchCuserGrpDevQryRecord(CUserGroupDeviceInfo &cInfo);
    ENTasErrno GetCUserGrpDeviceInfo(const UUIDString &cUserGrpId, const UUIDString &cDevId, CUserGroupDeviceInfo &cDevInfo);
	ENTasErrno GetCuserGrpIdsByCreatorId(const UUIDString& cCreatorId, vector<UUIDString>& vecCuserGrpIds);

	/* CU用户分组视频源查询 */
	void CuserDevGrpVidSrcQry(const CTaskMsgTransfer<CUserDevGrpVidsrcQryReq> *pcMsg);
	ENTasErrno PrepareCuserDevGrpVidSrcQrySql(const CUserDevGrpVidsrcQryReq &cReq, TSqlString &tSqlSelect);
    void FetchCuserDevGrpVidSrcQryRecord(CUserDevGrpVidSrcInfo &cInfo);

	//指定分组下视频源个数查询
	void CDevGrpVidsrcNumGet(const CTaskMsgTransfer<CDevGrpVidsrcTotalnumGetReq> *pcMsg);
	ENTasErrno GetDevGbidFromTblDevice(const UUIDString& strDevId, string& strGbId);


	/* 域关系和域名查询 */
    void DomainRelationQry(const CTaskMsgTransfer<CDomainRelationQryReq> *pcMsg);
    ENTasErrno PrepareDomainRelationQrySql(const CDomainRelationQryReq &cReq, TSqlString &tSqlSelect, TSqlString &tSqlCount);
    void FetchDomainRelationQryRecord(CDomainRelationInfo &cInfo);
    void GetDomainName(const CTaskMsgTransfer<CDomainNameGetReq> *pcMsg);
    void DomainRelationListQry(const CDbMsgTransfer<void> *pcMsg);
    // 可能的错误：CMS_SUCCESS, ERR_TAS_DBERROR，ERR_TAS_RECORD_NOT_EXISTS
    ENTasErrno DomainRelationItemQry(const string &strDomainId, CDomainRelationInfo &tInfo);
    ENTasErrno DomainRelationItemAdd(const CDomainRelationInfo &tInfo);
    ENTasErrno DomainRelationItemDel(const string &strDomainId);
    ENTasErrno DomainRelationItemMod(const CDomainRelationInfo &tInfo);

    ENTasErrno DomainRelationListQry(vector<CDomainRelationInfo> &tDomains);
    // 删除指定域的所有数据，指定域不能为本域
    ENTasErrno DeleteAllDataFromCertainDomain(const string &strDomainId);
    // 删除指定域及其下级的所有数据
    void DeleteAllDataFromSubDomain(const CDbMsgTransfer<string> *pcMsg);

    /* TAS配置 */
    void TasConfigGet(const CTaskMsgTransfer<CConfigTasGetReq> *pcMsg);
    void TasConfigSet(const CTaskMsgTransfer<CConfigTasSetReq> *pcMsg);
    void ChildTasAdd(const CTaskMsgTransfer<CChildtasAddReq> *pcMsg);
    void ChildTasDel(const CTaskMsgTransfer<CChildtasDelReq> *pcMsg);

    // 查询license使用情况
    void QueryLicenseUsage(void);
    void CuDataVersionGet(const CTaskMsgTransfer<CCuserDataVersionGetReq> *pcMsg);

    /* 私有设备管理 */
    ENTasErrno PrivateDeviceDel(const UUIDString &devId);
    void PrivateDeviceIdListQry(const CDbMsgTransfer<void> *pcMsg);

    /* 同步目标和来源管理 */
    void SyncDstDomainListQry(const CDbMsgTransfer<void> *pcMsg);
    void DomainAuthorizationDeviceIdListQry(const CDbMsgTransfer<string> *pcMsg);
    // 可能的错误：CMS_SUCCESS, ERR_TAS_DBERROR，ERR_TAS_RECORD_NOT_EXISTS
    ENTasErrno GetDomainAuthorizationInfo(const string &strDevId, const string &strUser, const string &strUserDomainId, TSyncDataUserDeviceInfo &tInfo);
    ENTasErrno DomainAuthorizationDel(const UUIDString &devId);

    ENTasErrno SyncDstDomainItemQry(vector<TSyncDstDomainInfo> &tSyncDstDomains);
    // 增加父域同步配置(包括同步目的域配置和初始化同步记录数据), 添加上级时使用
    ENTasErrno ParentDomainSyncDataConfigAdd(const string &strParentDomainId, const string &strParentDomainName);
    // 删除上级时删除父域同步配置，添加上级时使用
    ENTasErrno ParentDomainSyncDataConfigDel();

	void DevGrpGbidSet(const CDbMsgTransfer<TSetDevGrpGbidReq> *pcMsg);
	bool CheckGbidFit(const UUIDString& devTreeId, const UUIDString& devGrpId, const string& devGrpGbid, string& strParentGrpId);
	void DevGbidSet(const CDbMsgTransfer<TSetDevGbidReq> *pcMsg);
	void EmptyGbidDevsQry(const CDbMsgTransfer<TQryGbidEmptyDevsReq> *pcMsg);
	void MatchChildrenGrpGbid(const CDbMsgTransfer<TMatchChildrenGrpGbidReq> *pcMsg);
	ENTasErrno MatchChildrenGrpGbid(const UUIDString& devTreeId, const UUIDString& devParentGrpId, const string& strParentGrpGbid);

    /* 数据同步管理(同步记录管理) */
    void SyncDataRecordAdd(const CDbMsgTransfer<TSyncDataRecordStoreReq> *pcMsg);
    void SyncDataRecordDel(const CDbMsgTransfer<TSyncDataRecordDelReq> *pcMsg);
    bool SyncDataRecordQry(const string &strDstDomainId, ENSyncType enSyncType, u32 dwExpectedNum, vector<TSyncDataRecordInfo> &tRecords);
    void SyncDomainAuthorizationDependenceQry(const CDbMsgTransfer<string> *pcMsg);
    void SyncDevInfoRelateDataQry(const CDbMsgTransfer<string> *pcMsg);
    
    /* 数据同步管理(同步数据构造) */
    void SyncDataConstruct(const CDbMsgTransfer<TSyncDataConstructReq> *pcMsg);
    bool SyncDataConstructImp(const string &strDstDomainId, ENSyncType enSyncType, const vector<TSyncDataRecordInfo> &tRecords, vector<TSyncDataInfo> &tResults);
    bool SyncDataContent(const string &strDstDomainId, ENSyncType enSyncType, ENSyncDataType dataType, ENSyncOperateType operateType, const string &dataId, string &dataContent);
    
    /* 数据同步管理(同步数据存储) */
    void SyncDataStore(const CDbMsgTransfer<TSyncDataStoreReq> *pcMsg);
    // 可能的错误：CMS_SUCCESS, ERR_TAS_DBERROR，ERR_TAS_INVALID_SYNCDATA; 此函数中加入了同步分析拦截代码
    ENTasErrno SyncDataStoreImp(ENSyncType enSyncType, const TSyncDataInfo &tSyncData);
    ENTasErrno SyncDataStoreDomainInfo(const string &strSyncDataDomainId, const CDomainRelationInfo &tSyncData, ENSyncOperateType operateType, ENSyncType enSyncType);
    ENTasErrno SyncDataStoreDeviceModel(const string &strSyncDataDomainId, const CDeviceModelInfo &tSyncData, ENSyncOperateType operateType, ENSyncType enSyncType);
    ENTasErrno SyncDataStoreDeviceInfo(const string &strSyncDataDomainId, const CDeviceInfo &tSyncData, ENSyncOperateType operateType, ENSyncType enSyncType);
    ENTasErrno SyncDataStoreDeviceCapIndexName(const string &strSyncDataDomainId, const TDeviceCapName &tSyncData, ENSyncOperateType operateType, ENSyncType enSyncType);
    ENTasErrno SyncDataStoreDeviceGroup(const string &strSyncDataDomainId, const CDeviceGroupInfo &tSyncData, ENSyncOperateType operateType, ENSyncType enSyncType);
    ENTasErrno SyncDataStoreDeviceGroup2Dev(const string &strSyncDataDomainId, const CDeviceGroupDeviceInfo &tSyncData, ENSyncOperateType operateType, ENSyncType enSyncType);
    ENTasErrno SyncDataStoreUserDevice(const string &strSyncDataDomainId, const TSyncDataUserDeviceInfo &tSyncData, ENSyncOperateType operateType, ENSyncType enSyncType);
	ENTasErrno SyncDataStoreDevGrp2DevSeq(const string &strSyncDataDomainId, const CDeviceGroupDevSeqInfo &tSyncData, ENSyncOperateType operateType, ENSyncType enSyncType);
private:
	ENTasErrno DevGrpDevAdd(const CTaskMsgTransfer<CDeviceGroupDeviceAddReq> *pcMsg,bool bAddDevWithGrp);
	ENTasErrno IsDevCapPrivilegeVailed(const vector<TDeviceCapPrivilege>& cDevCapPrivis);
	ENTasErrno DevGrpDel(const UUIDString &grpId, const UUIDString &parentGrpId,const string &devTreeId="");
	string GetTblDevGrpName( const string &devTreeId );
	string GetTblDevGrp2DevName( const string &devTreeId );
	ENTasErrno GenerateModelName(CDeviceModelInfo& cModelInfo);
	double GetNextDevGrpSeq(const string& strParentGrpId);
	ENTasErrno GetNextDevGrpGbid(const string& strTreeId, const string& strParentGrpId, TDevGroupType devGrpType, string& strAvaliableGrpGbId, 
		bool bOnlyVirOrg=false); //bOnlyVirOrg是否只要216，是则只申城216，否则根据当前分组情况决定生成215、216，仅devGrpType为2时有效
	ENTasErrno GetNextDevGrpCivilCode(const string& strTreeId, const string& strParentGrpId, string& strAvaliableGrpGbId);
	ENTasErrno GetNextDevGrpVirGrpId(const string& strTreeId, const string& strParentGrpId, string& strAvaliableGrpGbId, 
		bool bOnlyVirOrg); //bOnlyVirOrg是否只要216，是则只申城216，否则根据当前分组情况决定生成215、216
	string GetDevGrpFullPath(const string& strGrpId);
	ENTasErrno CalcDomainGrpSeq(const string& strDomainId);
private:
    const char* DbCmdSpliter();    // 数据库命令分隔符
    void AddRowNumLimitModifer(TSqlString &tSelectSql, u32 dwStartIndex, u32 dwRowNum, bool bLimitMaxRowNum=true);
    s32 GetExpectEntryNum(s32 nExpectedRecordNum, BOOL32 bIsSupoortLargeNumQuery = FALSE);   //默认限制10w，TURE时限制为200w
    void AppendOrderbyModifer(TSqlString &tSql, const CQueryReqBase &cBase, const s8* pre=NULL);
    /* 拼接SQL语句中的key-value对，形式[pre.]field=value */
    bool AppendFieldValuePairImp(TSqlString &tSql, const s8* fieldName, const string& strVal, const s8* pre=NULL);
    bool AppendFieldValuePairImp(TSqlString &tSql, const s8* fieldName, s32 nVal, const s8* pre=NULL);
    bool AppendFieldValuePairImp(TSqlString &tSql, const s8* fieldName, bool bVal, const s8* pre=NULL);
	bool AppendFieldValuePairImp(TSqlString &tSql, const s8* fieldName, double fVal, const s8* pre=NULL);
    bool AppendFieldDatetimeValuePairImp(TSqlString &tSql, const s8* fieldName, const string& cmsdateVal, const s8* pre=NULL);
    bool AppendFieldDatetimePeriodPairImp4Qry(TSqlString &tSql, const s8* fieldName, 
                                          const string& cmsdate_from, const string& cmsdate_to, const s8* pre=NULL);
    bool AppendFieldLikeStringPairImp4Qry(TSqlString &tSql, const s8* fieldName, const string& str, const s8* pre=NULL);
    /* ModReq与QryReq类消息中字段内容临时存储变量 */
    string m_strFieldValue;
    string m_strFieldValue2;
    s32 m_nFieldValue;
    bool m_bFieldValue;
	double m_fFieldValue;
private:
    void ConnectDb(const CDBEngine::TDBConnectInfo &tDbConnectInfo);
    void DisconnectDb();

    enum ENExecOpt
    {
        enExecNoRet,   // 无需返回数据行
        enExecMustRet, // 必须返回数据行，不存在数据行也认为是错误
        enExecMayRet   // 返回数据行，但是不是必须的，可能不存在数据行
    };
    bool ExecSql(const s8* sql, ENExecOpt enOpt);
    bool IsNotEof(); // 取结果前先判断是否有结果
    bool IsEof();
    bool MoveNext(); // 移动至下条记录, 出错返回false，否则返回true, 再结合IsEof来判断是否有数据
    s32 GetIntResult();

#include "dbquerytemplate.h"

private:
    /* 用于从数据库取数据 */
    const s8* GetString(const s8* field);  // 取完即刻使用
    string GetCmsDate(const s8* field);
    s32 GetInt(const s8* field);
    bool GetBool(const s8* field);
	double GetDouble(const s8* field);
    s8 __szVal[1024*30]; // DbEngine的GetFieldValue方法值临时存放处，不要直接使用

    /* 用于构造数据以备插入数据库 */
    string DbStrImp(const string &str);      // 转换至数据库字符串形式，内部对字符串中的特殊符号进行转义
    TSqlString& DbStrImp(TSqlString &tSql);  // SQL语句字符串化，用于以字符串传递SQL语句到服务端动态执行等
    string DbDateTimeImp(const string &str); // 转换CmsTime至数据库日期时间形式
    string DbNowDateTimeImp();               // 获取当前时间的数据库日期时间形式
    string CmsDateTimeImp(const s8* sz);     // 转换至CmsTime字符串形式

private: //redis相关接口
	bool InitFastDb();
	bool QuitFastDb();
	bool DumpFastDb();

	bool GetLocalDomainGbId(string& strLocalDomainGbId);


private:
    CDBEngine m_cDbEngine;
	CFastDBInterface m_cFastDb;
};

// 通过消息的会话ID获取当前CU用户的用户ID
bool GetCurrentCuserId(const string &strSessionId, string &strUserId);
string GetPinYinFromUtf8(const string& strUtf8);

#endif // _DBOPR_H_
