/*
���ܣ������Ǿ�������ݿ�����ࡣ

���Լ����
1. һ����ѯ�в��ܽ��������ڡ��αꡱ�Ĳ��������������һ��ȡ�꣬�м䲻������ϡ�
2. �ַ�������ʹ�õ�����������������ʹ��˫���š�
3. CDbOpr�к������������ڶ��󻯵���֯��ʽ�������ͬ�����ͼ����ط����ͽ����á�
4. �������������ھ����ҵ����Ϣ����㣬С�İ����ຯ��ֻ�����������ݣ������������������(Ҳ����������Ʒ���)��
   ��˰�������Ϊ��������������ݿ�����ҵ����ص�(��ͬ��ʹ�ó�����������Ҳ��һ��)���������ຯ����Ҫ��Ϊ�˴��븴�ö����ڵġ�
*/

#ifndef _DBOPR_H_
#define _DBOPR_H_
#include "inner_common.h"
#include "dbengine.h"
#include "dbcache.h"
#include "tas_redis.h"
#include "sdk/fastdbsdk/fastdb.h"

#define UUID_LENGTH                 32    //Bytes
#define MAX_SEARCH_NAME_LEN         512   //ƴ����������󳤶ȣ���KDMAAA.tblVideoNameSearch.searchText�ֶγ��ȣ�
struct TSqlString
{
    enum
    {
        // ����������ֶεĳ���Ϊ30Kʱ������Ͳ�ѯ�ֶλ�ʧ�ܣ�linlifen˵DBI�����SQL��䳤��������
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
    u32 m_dwSqlLen; // ���Ȳ�����������
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
    // ������ݿ����ӣ�δ����ʱ�Զ���ȡ�������ý�������
    void CheckDbConnection(bool bTryExecTestSql = false);
	void CheckFastdbConnect();
    bool IsConnectedDb(); // ���ж��Ƿ��������ݿ�
    void Dump();
    static bool Init();
    static void Quit();

public:
    /* ��¼���� */
    void Login(const CTaskMsgTransfer<CLoginTasReq> *pcMsg);
    void CuOnline(const CTaskMsgTransfer<CCuOnlineReq> *pcMsg);

    /* �豸����(������Ϣ) */
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
    void UpdateDevTreeVer(const UUIDString &strDevId,const UUIDString &devtreeId=""); // �豸��Ϊ��ʱ���������û����û����
	void UpdateTreesVerByDevId(const UUIDString &devId);//(����)�Զ����豸�����豸���������豸��ƵԴ��������ʱʹ��(�ڲ�ʵ�����ж��߼������ú�һ�����޸İ汾��)
    ENTasErrno PrepareDeviceQrySql(const CDeviceQryReq &cReq, TSqlString &tSqlSelect, TSqlString &tSqlCount);
    ENTasErrno FetchDeviceQryRecord(CDeviceInfo &device); // �豸�ͺ��޷���ȫʱ����ʧ��
    // ���ܵĴ���CMS_SUCCESS, ERR_TAS_DBERROR��ERR_TAS_RECORD_NOT_EXISTS
    ENTasErrno DeviceItemQry(const UUIDString &strId, CDeviceInfo &cInfo);
    ENTasErrno DeviceItemAdd(const CDeviceInfo &cInfo);
    ENTasErrno DeviceItemDel(const UUIDString &strDevId);
    ENTasErrno DeviceItemMod(const CDeviceInfo &cInfo);
    void DeviceIdQry(const CTaskMsgTransfer<CDeviceQryReq> *pcMsg);

    /* �豸����(������Ϣ, ����ǽ) */
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
    // ���ܵĴ���CMS_SUCCESS, ERR_TAS_DBERROR��ERR_TAS_RECORD_NOT_EXISTS
    ENTasErrno TvwallItemQry(const UUIDString &cTvwallId, CTvwallInfo &cTvwall);
    bool IsExistSameNameTvwall(const string &strName, const UUIDString *ptExcludeTvwallId = NULL);

    /* �豸����(�豸�ͺ�) */
    bool GetDeviceTypeInfo(u32 dwType, TInnerDeviceTypeInfo &cInfo);
    void DeviceModelAdd(const CTaskMsgTransfer<CDeviceModelAddReq> *pcMsg);
    void DeviceModelDel(const CTaskMsgTransfer<CDeviceModelDelReq> *pcMsg);
    void DeviceModelQry(const CTaskMsgTransfer<CDeviceModelQryReq> *pcMsg);
	string DeviceModelAdd(const CTaskMsgTransfer<CDeviceModelAddReq> *pcMsg,bool bUseInner);
    ENTasErrno PrepareDeviceModelQrySql(const CDeviceModelQryReq &cReq, TSqlString &tSqlSelect, TSqlString &tSqlCount);
    void FetchDeviceModelQryRecord(CDeviceModelInfo &model);
    // ���ܵĴ���CMS_SUCCESS, ERR_TAS_DBERROR��ERR_TAS_RECORD_NOT_EXISTS
    ENTasErrno DeviceModelItemQry(const string &strModelId, CDeviceModelInfo &cInfo);
    ENTasErrno DeviceModelItemAdd(const CDeviceModelInfo &cInfo, bool bValidateCapId);
    ENTasErrno DeviceModelItemDel(const string &strModelId);
    // ���ܵĴ���CMS_SUCCESS, ERR_TAS_DBERROR��ERR_TAS_RECORD_NOT_EXISTS
    ENTasErrno DeviceModelItemQryByDevId(const string &strDevId, CDeviceModelInfo &cInfo);
	// ��ȡָ��������ͬ�����豸�ͺ���Ϣ
	ENTasErrno GetExistDeviceModelId(const string &strDomainId, const string &strModelName, TCreater tCreater, string &strModelId);
	// ��ȡ��ͬ��Ϣ���豸�ͺ�Id
	ENTasErrno GetExistDeviceModelId(const string &strDomainId, s32 nType, const string &strModelName,bool bIgnoreModelName, const string &strManufacturer, 
		TCreater tCreater, const vector<TDeviceCapInfo> &vecCapInfo, string &strModelId);
    ENTasErrno IsDeviceModelUsing(const string &modelId);// ����ERR_TAS_DEVICE_MODEL_USING,ERR_TAS_DBERROR,CMS_SUCCESS
	bool CompareCapInfo(const vector<TDeviceCapInfo> &vecCapInfo1, const vector<TDeviceCapInfo> &vecCapInfo2);
	void DevCapInfoMod(const CTaskMsgTransfer<CDevCapInfoModReq> *pcMsg);

    /* �豸����(�豸������������) */
    void DeviceCapIndexNameGet(const CTaskMsgTransfer<CDeviceCapIndexNameGetReq> *pcMsg);
    void DeviceCapIndexNameSet(const CTaskMsgTransfer<CDeviceCapIndexNameSetReq> *pcMsg);
    void DeviceCapIndexNameQry(const CTaskMsgTransfer<CDeviceCapIndexNameQryReq> *pcMsg);
    ENTasErrno PrepareDeviceCapIndexNameQrySql(const CDeviceCapIndexNameQryReq &cReq, TSqlString &tSqlSelect, TSqlString &tSqlCount);
	ENTasErrno ModDeviceVidSrcName(const UUIDString& devId, int nIndex, const string& VidSrcName);
    void FetchDeviceCapIndexNameQryRecord(TDeviceCapName &devCapName);
    // nCapIndexΪ-1ʱ��ʾnCapId�������͵������������ƣ����ܵĴ���CMS_SUCCESS, ERR_TAS_DBERROR��ERR_TAS_RECORD_NOT_EXISTS
    ENTasErrno GetDeviceCapIndexName(const string &strDevId, s32 nCapId, s32 nCapIndex, TDeviceCapName &tCapIndexName);
    // ��ͬ�����������������ϲ������ܵĴ���CMS_SUCCESS, ERR_TAS_DBERROR��ERR_TAS_RECORD_NOT_EXISTS
    ENTasErrno GetDeviceCapIndexs(const string &strDevId, vector<TDeviceCapIndexs> &tCapIndexs);
    ENTasErrno DeviceCapIndexNameDel(const UUIDString &devId);
    ENTasErrno DeviceCapIndexNameItemSet(const TDeviceCapName &cInfo, bool bAllowAppendCapNameWhenEmpty=false); // ������ʱ��ӣ�����ʱ�޸�
	
    /* ��ƵԴƴ������ */
    ENTasErrno DevicePinyinSearchVideoNameInit(const string &devId, int videoNum, int videoIndex=0, const char* szName=NULL); // ��ʼ���豸����ƵԴ��������
    ENTasErrno DevicePinyinSearchVideoNameDel(const string &devId, int videoIndex=-1); // ɾ������videoIndex����ƵԴ��������
    ENTasErrno DevicePinyinSearchVideoNameSet(const string &devId, vector<int> videoIndexs, vector<string> videoNames);

    /* �豸����(�豸����) */
    void DeviceManufacturerAdd(const CTaskMsgTransfer<CDeviceManufacturerAddReq> *pcMsg);
    void DeviceManufacturerDel(const CTaskMsgTransfer<CDeviceManufacturerDelReq> *pcMsg);
    void DeviceManufacturerQry(const CTaskMsgTransfer<CDeviceManufacturerQryReq> *pcMsg);
    ENTasErrno PrepareDeviceManufacturerQrySql(const CDeviceManufacturerQryReq &cReq, TSqlString &tSqlSelect, TSqlString &tSqlCount);
    void FetchDeviceManufacturerQryRecord(CDeviceManufacturerInfo &manufacturer);

    /* �豸����(�豸�������ݺ��豸Ȩ��) */
    void CuserDevicefullinfoAndPrivilegeQry(const CTaskMsgTransfer<CCuserDevicefullinfoAndPrivilegeQryReq> *pcMsg);
    ENTasErrno PrepareCuserDevicefullinfoAndPrivilegeQrySql(const CCuserDevicefullinfoAndPrivilegeQryReq &cReq, TSqlString &tSqlSelect, TSqlString &tSqlCount,
		bool &bNeedQryHint);
    ENTasErrno FetchCuserDevicefullinfoAndPrivilegeQryRecord(TDevicefullinfoAndPrivilege &record, bool bNewRecord, bool &bCombineFinished); // �豸�ͺ��޷�����ʱ����ʧ��
    void CuserExportDevData(const CTaskMsgTransfer<CFileExportReq> *pcMsg);

	//��ƵԴ��Ϣ��ѯ
	void VidsrcInfoQry(const CTaskMsgTransfer<CVidsrcInfoQryReq> *pcMsg);
	ENTasErrno PrepareVidsrcInfoQrySql(const UUIDString &strDevId, const int vidsrcIdx, TSqlString &tSqlSelect, TSqlString &tSqlSelect2);
    
	/* CU�û����� */
    void CuserAdd(const CTaskMsgTransfer<CUserAddReq> *pcMsg);
    void CuserDel(const CTaskMsgTransfer<CUserDelReq> *pcMsg);
    void CuserMod(const CTaskMsgTransfer<CUserModReq> *pcMsg);
    void CuserQry(const CTaskMsgTransfer<CUserQryReq> *pcMsg);
	string GetNewUserGBID();
    void UpdateCuserDevTreeVer(const UUIDString &strUserId); // strUserIdΪ��ʱ��ʾ��������Ա���
    ENTasErrno PrepareCuserQrySql(const CUserQryReq &cReq, TSqlString &tSqlSelect, TSqlString &tSqlCount);
    void FetchCuserQryRecord(CUserInfo &cInfo);
    bool GetCuserInfo(const UUIDString &userId, CUserInfo &cInfo);
    // ���ܵĴ���CMS_SUCCESS, ERR_TAS_DBERROR��ERR_TAS_RECORD_NOT_EXISTS
    ENTasErrno GetCuserInfoByName(const string &strName, CUserInfo &cInfo);
    bool IsExistCuserInCertainUsergrp(const UUIDString &cusergrpid, bool &bExistFlag);
    bool GetCuserGrpId(const UUIDString &strUserId, UUIDString &strGrpId);
	ENTasErrno CuserQryByUserGrp(const UUIDString &strUserGrpId,vector<CUserInfo> &cUserInfos);
    
    /* �û��豸���� */
    void CuserDevAdd(const CTaskMsgTransfer<CUserDeviceAddReq> *pcMsg);
    ENTasErrno CuserDevAddDefault(const UUIDString &devId, const UUIDString &devModelId); // ����������Ա�����豸Ȩ��
    void CuserDevDel(const CTaskMsgTransfer<CUserDeviceDelReq> *pcMsg);
    ENTasErrno CuserDevDelByDevId(const UUIDString &devId);
    ENTasErrno CuserDevDelByUserId(const UUIDString &userId);
    void CuserDevMod(const CTaskMsgTransfer<CUserDeviceModReq> *pcMsg);
    void CuserDevQry(const CTaskMsgTransfer<CUserDeviceQryReq> *pcMsg);
    ENTasErrno PrepareCuserDevQrySql(const CUserDeviceQryReq &cReq, TSqlString &tSqlSelect, TSqlString &tSqlCount);
    void FetchCuserDevQryRecord(CUserDeviceInfo &cInfo);
    ENTasErrno GetCUserDeviceInfo(const UUIDString &cUserId, const UUIDString &cDevId, CUserDeviceInfo &cDevInfo);
    ENTasErrno ValidateCurrentUserDevPrivilege(const string &strSessionId, const CUserDeviceInfo &cDevInfo);
    ENTasErrno CuserDevItemAdd(const CUserDeviceInfo &cInfo);   // ֧���ظ���ӣ������ظ�ʱ�Զ����������
    ENTasErrno CuserDevItemDel(const CUserDeviceInfoKey &cInfo);
    ENTasErrno CuserDevItemMod(const CUserDeviceInfo &cInfo);


	//�豸������  [3/10/2016 pangubing]
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

	//ָ����ָ�����������з�����豸���ݹ飩��Ϣ��ѯ
	void DevGrpChildrenQry(const CTaskMsgTransfer<CDeviceGroupChildrenQryReq> *pcMsg);
	ENTasErrno DevGrpChildrenQry(const UUIDString& devTreeId, const UUIDString& devGrpId, 
		vector<CDeviceGroupInfo>& devGrpInfos, vector<CDeviceGroupDeviceInfo4Gb>& devInfos);

	// PTZ������ [4/12/2016 pangubing]
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

	/* ���黮�� */
	void CuserDeviceGroupAdd(const CTaskMsgTransfer<CUserDeviceGroupAddReq> *pcMsg);
	void CuserDeviceGroupDel(const CTaskMsgTransfer<CUserDeviceGroupDelReq> *pcMsg);
	void CuserGroupDeviceGroupAdd(const CTaskMsgTransfer<CUserGroupDeviceGroupAddReq> *pcMsg);
	void CuserGroupDeviceGroupDel(const CTaskMsgTransfer<CUserGroupDeviceGroupDelReq> *pcMsg);
	void CDeviceAutoAssignToUserSet(const CTaskMsgTransfer<CDeviceAutoAssignToUserSetReq> *pcMsg);
	void CDeviceAutoAssignToUserGroupSet(const CTaskMsgTransfer<CDeviceAutoAssignToUserGroupSetReq> *pcMsg);
	void CDeviceAutoAssignToUserQry(const CTaskMsgTransfer<CDeviceAutoAssignToUserQryReq> *pcMsg);
	void CDeviceAutoAssignToUserGroupQry(const CTaskMsgTransfer<CDeviceAutoAssignToUserGroupQryReq> *pcMsg);

	/* ���黮���г�������ļ���ԭ�Ӳ��� */
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
	
	/* ���黮����ص�����ת������ */
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

	/* ��ѯģ������Ҫ�ļ������� */
	ENTasErrno PrepareDeviceAutoAssignToUserQrySql(const CDeviceAutoAssignToUserQryReq &cReq, TSqlString &tSqlSelect, TSqlString &tSqlCount);
	ENTasErrno PrepareDeviceAutoAssignToUserGroupQrySql(const CDeviceAutoAssignToUserGroupQryReq &cReq, TSqlString &tSqlSelect, TSqlString &tSqlCount);
	void FetchDeviceAutoAssignToOwnerQryRecord(CDeviceAutoAssignInfo &cInfo);


    /* �豸������� */
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
    // ���ܵĴ���CMS_SUCCESS, ERR_TAS_DBERROR��ERR_TAS_RECORD_NOT_EXISTS
    ENTasErrno DevGrpItemQry(const UUIDString &strId, CDeviceGroupInfo &cInfo,const string& devTreeId="");
	ENTasErrno DevGrpItemQryOnly(const UUIDString &strId, CDeviceGroupInfo &cInfo,const string& devTreeId="");
    ENTasErrno DevGrpItemAdd(const CDeviceGroupInfo &cInfo,const string& devTreeId="");
	bool IsVidSrcInDevGrp(const UUIDString& devTreeId, const UUIDString& devGrpId, const UUIDString& devId, const u32 dwVidSrcIndex);

	ENTasErrno DevGrpItemDel(const UUIDString &strDevGrpId,const string &devTreeId="");
    ENTasErrno DevGrpItemMod(const CDeviceGroupInfo &cInfo);
    ENTasErrno DomainAuthorizationDevGrpId(string &strId);   // ������Ȩ�豸�ķ����ţ����ܵĴ���CMS_SUCCESS, ERR_TAS_DBERROR
    ENTasErrno MoveupDevGrps(const UUIDString &grpId, const UUIDString &parentGrpId=""); // ����ָ���豸���µ��豸����ָ���豸��ĸ���

    /* �豸�����豸���� */
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

	//�豸���豸����
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
	bool GetDevGrpNextDevSeq4SameDev(const UUIDString &devTreeId, const UUIDString &devGrpId,const UUIDString& devId,int vidSrcIndex, double& devSeq); //ͬһ�����ڲ�ͬһ�豸
	bool QryNeerDevSeq(const UUIDString& devTreeId, const UUIDString& devGrpId, const double referDevSeq,
		bool bLarger, //true--ѡ�����referDevSeq��ֵ��false--ѡ��С��referDevSeq��ֵ��
		double& devSeq); //��ѯֵ��ӽ�referDevSeq��ֵ
	bool QryRangeDevSeq(const UUIDString& devTreeId, const UUIDString& devGrpId, 
		bool bMax,      //��ȡ�������������bMax=true������С��bMax=false����devSeqֵ
		double& devSeq,
		const UUIDString devId="");  //devId��Ϊ��ʱ����Χ��С���÷���&&���豸 
	ENTasErrno DisperseDevSeq(const UUIDString &devTreeId, const UUIDString &devGrpId);

	//�����豸ͳ��
	ENTasErrno GetDevGrpDevSumNum( const string &strTreeId, const string& strGrpId,int& dwSumNum);
	void DevGrpDevSumNumQry(const CTaskMsgTransfer<CDeviceGrpDevSumNumReq> *pcMsg);
	//�豸����ѯ
	void DevGbInfoQry(const CTaskMsgTransfer<CDeviceGbinfoQryReq> *pcMsg);
	void FetchDeviceGbInfoQry(CDeviceFullGbInfo &cInfo );
	void FetchDeviceGbFullGrpInfoQry(CDeviceFullGbInfo &cInfo,const string& strTreeId);
	//�豸���鷴��ѯ
	void DevGrpGbInfoQry(const CTaskMsgTransfer<CDevgrpGbinfoQryReq> *pcMsg);
	void FetchDeviceGrpGbInfoQry(TDevGrpFullGbInfos &cInfo,const string& strTreeId);
	//����ƥ��
	void DevGrpMatch(const CTaskMsgTransfer<CDeviceMatchGbidReq> *(pcMsg));
	//�豸gbid��һ�������ѯ
	void DevGbidNextErrQry(const CTaskMsgTransfer<CDeviceGbidErrGetReq> *(pcMsg));
	void FetchDeviceErrQry(TDevGbidErrItem& tDevErr);
	//�豸gbid����
	void DevGrp2DevGbidOperate(const CDbMsgTransfer<CDevGrp2DevGbidOperateReq> *pcMsg);
	//�豸����gbid����
	void DevGrpGbidOperate(const CDbMsgTransfer<CDevGrpGbidOperateReq> *pcMsg);
	//ָ�����������������豸��ѯ
	void DevGrpAllChildDevQry(const CDbMsgTransfer<CDevGrpAllChildDevQryReq> *pcMsg);
	void FetchDevGrpAllDevQry(CDeviceGroupDeviceInfo4Gb& cInfo);
	//���ݿ�תΪ��������xml�ļ�
	void TableDataToPushXmlFile(const CDbMsgTransfer<string> *pcMsg);

    ENTasErrno DevGrpDevItemAdd(const CDeviceGroupDeviceInfo &cInfo,bool bAddDevWithGrp=false,const string &grpTreeId="",const string &devTreeId=""); // ��ӵ�����ʱ�����еĻ��鱻ɾ��
    ENTasErrno DevGrpDevItemDel(const CDeviceGroupDeviceInfo &cInfo); // ɾ�������еĻ���ʱ�����豸���豸���������Ĭ����
    // ���ݼ�¼���ϲ������ܵĴ���CMS_SUCCESS, ERR_TAS_DBERROR��ERR_TAS_RECORD_NOT_EXISTS
    ENTasErrno GetDeviceGroupDeviceInfoKeys(const string &strDevId, vector<CDeviceGroupDeviceInfoKey> &tKeys);
    // ��ָ���豸���뵽ָ�����δ����
    ENTasErrno DevGrpDevAddDefault(const UUIDString &devId, const UUIDString &devModelId, const string *pstrCertainDomainId=NULL); 
    // ��ָ���豸���뵽������Ȩ�豸����
    ENTasErrno DevGrpDevAddDomainAuthorization(const UUIDString &devId, const UUIDString &devModelId);
    ENTasErrno DevGrpDevDelByDevId(const UUIDString &devId,bool bNeedDelTreeInfo=true);
    ENTasErrno DevGrpDevExistsTest(const UUIDString &grpId, bool &bExists);
    ENTasErrno MoveupDevGrpDevs(const UUIDString &grpId, const UUIDString &parentGrpId,const string &devTreeId); // ����ָ���豸���µ��豸��ָ���豸��ĸ���

	//��������ֱɾ
	ENTasErrno DelGateWayData(const CTaskMsgTransfer<CGatewayDelReq> *pcMsg);
	ENTasErrno DelGateWayDevice(const CTaskMsgTransfer<CGatewayDelReq> *pcMsg, vector<CDeviceInfo>& vecDeviceDel);
	ENTasErrno DelGateWayDevModel(const CTaskMsgTransfer<CGatewayDelReq> *pcMsg, vector<UUIDString>& vecModelDel);
	ENTasErrno DelGateWayDevGrp(const CTaskMsgTransfer<CGatewayDelReq> *pcMsg, vector<UUIDString>& vecDevGrpDel);
	void       GateWayDataDelNtf(const CTaskMsgTransfer<CGatewayDelReq> *pcMsg, vector<CDeviceInfo>& vecDeviceDel, 
		vector<UUIDString>& vecModelDel, vector<UUIDString>& vecDevGrpDel);



    /* CU�û�������� */
    void CuserGrpAdd(const CTaskMsgTransfer<CUserGroupAddReq> *pcMsg);
    void CuserGrpDel(const CTaskMsgTransfer<CUserGroupDelReq> *pcMsg);
    void CuserGrpMod(const CTaskMsgTransfer<CUserGroupModReq> *pcMsg);
    void CuserGrpQry(const CTaskMsgTransfer<CUserGroupQryReq> *pcMsg);
    void UpdateCuserGrpDevTreeVer(const UUIDString &strUserGrpId);
    ENTasErrno PrepareCuserGrpQrySql(const CUserGroupQryReq &cReq, TSqlString &tSqlSelect, TSqlString &tSqlCount);
    void FetchCuserGrpQryRecord(CUserGroupInfo &cInfo);
    bool IsExistSameNameCuserGrp(const string &strName, const string *pstrExcludeCuserGrpId = NULL);
    bool GetCuserGrpInfo(const UUIDString &strId, CUserGroupInfo &cInfo);
    /* CU�û������豸���� */
    void CuserGrpDevAdd(const CTaskMsgTransfer<CUserGroupDeviceAddReq> *pcMsg);
    void CuserGrpDevDel(const CTaskMsgTransfer<CUserGroupDeviceDelReq> *pcMsg);
    ENTasErrno CuserGrpDevDel(const UUIDString &devId);
    void CuserGrpDevMod(const CTaskMsgTransfer<CUserGroupDeviceModReq> *pcMsg);
    void CuserGrpDevQry(const CTaskMsgTransfer<CUserGroupDeviceQryReq> *pcMsg);
    ENTasErrno PrepareCuserGrpDevQrySql(const CUserGroupDeviceQryReq &cReq, TSqlString &tSqlSelect, TSqlString &tSqlCount);
    void FetchCuserGrpDevQryRecord(CUserGroupDeviceInfo &cInfo);
    ENTasErrno GetCUserGrpDeviceInfo(const UUIDString &cUserGrpId, const UUIDString &cDevId, CUserGroupDeviceInfo &cDevInfo);
	ENTasErrno GetCuserGrpIdsByCreatorId(const UUIDString& cCreatorId, vector<UUIDString>& vecCuserGrpIds);

	/* CU�û�������ƵԴ��ѯ */
	void CuserDevGrpVidSrcQry(const CTaskMsgTransfer<CUserDevGrpVidsrcQryReq> *pcMsg);
	ENTasErrno PrepareCuserDevGrpVidSrcQrySql(const CUserDevGrpVidsrcQryReq &cReq, TSqlString &tSqlSelect);
    void FetchCuserDevGrpVidSrcQryRecord(CUserDevGrpVidSrcInfo &cInfo);

	//ָ����������ƵԴ������ѯ
	void CDevGrpVidsrcNumGet(const CTaskMsgTransfer<CDevGrpVidsrcTotalnumGetReq> *pcMsg);
	ENTasErrno GetDevGbidFromTblDevice(const UUIDString& strDevId, string& strGbId);


	/* ���ϵ��������ѯ */
    void DomainRelationQry(const CTaskMsgTransfer<CDomainRelationQryReq> *pcMsg);
    ENTasErrno PrepareDomainRelationQrySql(const CDomainRelationQryReq &cReq, TSqlString &tSqlSelect, TSqlString &tSqlCount);
    void FetchDomainRelationQryRecord(CDomainRelationInfo &cInfo);
    void GetDomainName(const CTaskMsgTransfer<CDomainNameGetReq> *pcMsg);
    void DomainRelationListQry(const CDbMsgTransfer<void> *pcMsg);
    // ���ܵĴ���CMS_SUCCESS, ERR_TAS_DBERROR��ERR_TAS_RECORD_NOT_EXISTS
    ENTasErrno DomainRelationItemQry(const string &strDomainId, CDomainRelationInfo &tInfo);
    ENTasErrno DomainRelationItemAdd(const CDomainRelationInfo &tInfo);
    ENTasErrno DomainRelationItemDel(const string &strDomainId);
    ENTasErrno DomainRelationItemMod(const CDomainRelationInfo &tInfo);

    ENTasErrno DomainRelationListQry(vector<CDomainRelationInfo> &tDomains);
    // ɾ��ָ������������ݣ�ָ������Ϊ����
    ENTasErrno DeleteAllDataFromCertainDomain(const string &strDomainId);
    // ɾ��ָ�������¼�����������
    void DeleteAllDataFromSubDomain(const CDbMsgTransfer<string> *pcMsg);

    /* TAS���� */
    void TasConfigGet(const CTaskMsgTransfer<CConfigTasGetReq> *pcMsg);
    void TasConfigSet(const CTaskMsgTransfer<CConfigTasSetReq> *pcMsg);
    void ChildTasAdd(const CTaskMsgTransfer<CChildtasAddReq> *pcMsg);
    void ChildTasDel(const CTaskMsgTransfer<CChildtasDelReq> *pcMsg);

    // ��ѯlicenseʹ�����
    void QueryLicenseUsage(void);
    void CuDataVersionGet(const CTaskMsgTransfer<CCuserDataVersionGetReq> *pcMsg);

    /* ˽���豸���� */
    ENTasErrno PrivateDeviceDel(const UUIDString &devId);
    void PrivateDeviceIdListQry(const CDbMsgTransfer<void> *pcMsg);

    /* ͬ��Ŀ�����Դ���� */
    void SyncDstDomainListQry(const CDbMsgTransfer<void> *pcMsg);
    void DomainAuthorizationDeviceIdListQry(const CDbMsgTransfer<string> *pcMsg);
    // ���ܵĴ���CMS_SUCCESS, ERR_TAS_DBERROR��ERR_TAS_RECORD_NOT_EXISTS
    ENTasErrno GetDomainAuthorizationInfo(const string &strDevId, const string &strUser, const string &strUserDomainId, TSyncDataUserDeviceInfo &tInfo);
    ENTasErrno DomainAuthorizationDel(const UUIDString &devId);

    ENTasErrno SyncDstDomainItemQry(vector<TSyncDstDomainInfo> &tSyncDstDomains);
    // ���Ӹ���ͬ������(����ͬ��Ŀ�������úͳ�ʼ��ͬ����¼����), ����ϼ�ʱʹ��
    ENTasErrno ParentDomainSyncDataConfigAdd(const string &strParentDomainId, const string &strParentDomainName);
    // ɾ���ϼ�ʱɾ������ͬ�����ã�����ϼ�ʱʹ��
    ENTasErrno ParentDomainSyncDataConfigDel();

	void DevGrpGbidSet(const CDbMsgTransfer<TSetDevGrpGbidReq> *pcMsg);
	bool CheckGbidFit(const UUIDString& devTreeId, const UUIDString& devGrpId, const string& devGrpGbid, string& strParentGrpId);
	void DevGbidSet(const CDbMsgTransfer<TSetDevGbidReq> *pcMsg);
	void EmptyGbidDevsQry(const CDbMsgTransfer<TQryGbidEmptyDevsReq> *pcMsg);
	void MatchChildrenGrpGbid(const CDbMsgTransfer<TMatchChildrenGrpGbidReq> *pcMsg);
	ENTasErrno MatchChildrenGrpGbid(const UUIDString& devTreeId, const UUIDString& devParentGrpId, const string& strParentGrpGbid);

    /* ����ͬ������(ͬ����¼����) */
    void SyncDataRecordAdd(const CDbMsgTransfer<TSyncDataRecordStoreReq> *pcMsg);
    void SyncDataRecordDel(const CDbMsgTransfer<TSyncDataRecordDelReq> *pcMsg);
    bool SyncDataRecordQry(const string &strDstDomainId, ENSyncType enSyncType, u32 dwExpectedNum, vector<TSyncDataRecordInfo> &tRecords);
    void SyncDomainAuthorizationDependenceQry(const CDbMsgTransfer<string> *pcMsg);
    void SyncDevInfoRelateDataQry(const CDbMsgTransfer<string> *pcMsg);
    
    /* ����ͬ������(ͬ�����ݹ���) */
    void SyncDataConstruct(const CDbMsgTransfer<TSyncDataConstructReq> *pcMsg);
    bool SyncDataConstructImp(const string &strDstDomainId, ENSyncType enSyncType, const vector<TSyncDataRecordInfo> &tRecords, vector<TSyncDataInfo> &tResults);
    bool SyncDataContent(const string &strDstDomainId, ENSyncType enSyncType, ENSyncDataType dataType, ENSyncOperateType operateType, const string &dataId, string &dataContent);
    
    /* ����ͬ������(ͬ�����ݴ洢) */
    void SyncDataStore(const CDbMsgTransfer<TSyncDataStoreReq> *pcMsg);
    // ���ܵĴ���CMS_SUCCESS, ERR_TAS_DBERROR��ERR_TAS_INVALID_SYNCDATA; �˺����м�����ͬ���������ش���
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
		bool bOnlyVirOrg=false); //bOnlyVirOrg�Ƿ�ֻҪ216������ֻ���216��������ݵ�ǰ���������������215��216����devGrpTypeΪ2ʱ��Ч
	ENTasErrno GetNextDevGrpCivilCode(const string& strTreeId, const string& strParentGrpId, string& strAvaliableGrpGbId);
	ENTasErrno GetNextDevGrpVirGrpId(const string& strTreeId, const string& strParentGrpId, string& strAvaliableGrpGbId, 
		bool bOnlyVirOrg); //bOnlyVirOrg�Ƿ�ֻҪ216������ֻ���216��������ݵ�ǰ���������������215��216
	string GetDevGrpFullPath(const string& strGrpId);
	ENTasErrno CalcDomainGrpSeq(const string& strDomainId);
private:
    const char* DbCmdSpliter();    // ���ݿ�����ָ���
    void AddRowNumLimitModifer(TSqlString &tSelectSql, u32 dwStartIndex, u32 dwRowNum, bool bLimitMaxRowNum=true);
    s32 GetExpectEntryNum(s32 nExpectedRecordNum, BOOL32 bIsSupoortLargeNumQuery = FALSE);   //Ĭ������10w��TUREʱ����Ϊ200w
    void AppendOrderbyModifer(TSqlString &tSql, const CQueryReqBase &cBase, const s8* pre=NULL);
    /* ƴ��SQL����е�key-value�ԣ���ʽ[pre.]field=value */
    bool AppendFieldValuePairImp(TSqlString &tSql, const s8* fieldName, const string& strVal, const s8* pre=NULL);
    bool AppendFieldValuePairImp(TSqlString &tSql, const s8* fieldName, s32 nVal, const s8* pre=NULL);
    bool AppendFieldValuePairImp(TSqlString &tSql, const s8* fieldName, bool bVal, const s8* pre=NULL);
	bool AppendFieldValuePairImp(TSqlString &tSql, const s8* fieldName, double fVal, const s8* pre=NULL);
    bool AppendFieldDatetimeValuePairImp(TSqlString &tSql, const s8* fieldName, const string& cmsdateVal, const s8* pre=NULL);
    bool AppendFieldDatetimePeriodPairImp4Qry(TSqlString &tSql, const s8* fieldName, 
                                          const string& cmsdate_from, const string& cmsdate_to, const s8* pre=NULL);
    bool AppendFieldLikeStringPairImp4Qry(TSqlString &tSql, const s8* fieldName, const string& str, const s8* pre=NULL);
    /* ModReq��QryReq����Ϣ���ֶ�������ʱ�洢���� */
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
        enExecNoRet,   // ���践��������
        enExecMustRet, // ���뷵�������У�������������Ҳ��Ϊ�Ǵ���
        enExecMayRet   // ���������У����ǲ��Ǳ���ģ����ܲ�����������
    };
    bool ExecSql(const s8* sql, ENExecOpt enOpt);
    bool IsNotEof(); // ȡ���ǰ���ж��Ƿ��н��
    bool IsEof();
    bool MoveNext(); // �ƶ���������¼, ������false�����򷵻�true, �ٽ��IsEof���ж��Ƿ�������
    s32 GetIntResult();

#include "dbquerytemplate.h"

private:
    /* ���ڴ����ݿ�ȡ���� */
    const s8* GetString(const s8* field);  // ȡ�꼴��ʹ��
    string GetCmsDate(const s8* field);
    s32 GetInt(const s8* field);
    bool GetBool(const s8* field);
	double GetDouble(const s8* field);
    s8 __szVal[1024*30]; // DbEngine��GetFieldValue����ֵ��ʱ��Ŵ�����Ҫֱ��ʹ��

    /* ���ڹ��������Ա��������ݿ� */
    string DbStrImp(const string &str);      // ת�������ݿ��ַ�����ʽ���ڲ����ַ����е�������Ž���ת��
    TSqlString& DbStrImp(TSqlString &tSql);  // SQL����ַ��������������ַ�������SQL��䵽����˶�ִ̬�е�
    string DbDateTimeImp(const string &str); // ת��CmsTime�����ݿ�����ʱ����ʽ
    string DbNowDateTimeImp();               // ��ȡ��ǰʱ������ݿ�����ʱ����ʽ
    string CmsDateTimeImp(const s8* sz);     // ת����CmsTime�ַ�����ʽ

private: //redis��ؽӿ�
	bool InitFastDb();
	bool QuitFastDb();
	bool DumpFastDb();

	bool GetLocalDomainGbId(string& strLocalDomainGbId);


private:
    CDBEngine m_cDbEngine;
	CFastDBInterface m_cFastDb;
};

// ͨ����Ϣ�ĻỰID��ȡ��ǰCU�û����û�ID
bool GetCurrentCuserId(const string &strSessionId, string &strUserId);
string GetPinYinFromUtf8(const string& strUtf8);

#endif // _DBOPR_H_
