/*****************************************************************************
   模块名      : cui_fcgi 
   文件名      : cuidata.cpp
   相关文件    : 
   文件实现功能: cuidata 定义CUI的全局变量
   作者        : liangli
   版本        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人      修改内容
   2011/11/10   1.0         liangli        创建
******************************************************************************/
#include "cuidata.h"
#include "cms/commonconfig/xmlconfig.h"
//#include "cms/cms_const.h"
//#include "cms/utility/uuid.h"
#include "cuistruct.h"
#include "culogintask.h"
#include "cms/utility/strtok.h"
#include "cms/cmu/vtdulib.h"
#include "cms/utility/networkhelper.h"

TMediaTaskSets g_tMediaTaskSets;
void PushMediaTask(const string& strSession,const s32& nPlayId,CTask* pTask)
{
	string strCuid = strSession;
	string::size_type sPos = strCuid.find("@");
	if (string::npos != sPos)
	{
		strCuid = strCuid.substr(0,sPos);
	}

	TMediaTaskKey tMediaTaskKey;
	tMediaTaskKey.SetCUId(strCuid);
	tMediaTaskKey.SetPlayId(nPlayId);

	g_tMediaTaskSets.Insert(tMediaTaskKey,pTask);
}
void PopMediaTask(const string& strSession,const s32& nPlayId)
{
	string strCuid = strSession;
	string::size_type sPos = strCuid.find("@");
	if (string::npos != sPos)
	{
		strCuid = strCuid.substr(0,sPos);
	}
	TMediaTaskKey tMediaTaskKey;
	tMediaTaskKey.SetCUId(strCuid);
	tMediaTaskKey.SetPlayId(nPlayId);

	if (NULL != g_tMediaTaskSets.Find(tMediaTaskKey))
	{
		g_tMediaTaskSets.Erase(tMediaTaskKey);
	}
}
CTask* GetMediaTask(const string& strSession,const s32& nPlayId)
{
	TMediaTaskKey tMediaTaskKey;
	tMediaTaskKey.SetCUId(strSession);
	tMediaTaskKey.SetPlayId(nPlayId);

	if (NULL != g_tMediaTaskSets.Find(tMediaTaskKey))
	{
		return *(g_tMediaTaskSets.Find(tMediaTaskKey));
	}
	
	return NULL;
}

bool ChangeToNatAddr(const string& strCuSession,TNetAddr& tAddr,const u32& dwPlayId)
{
	string strCuid = strCuSession;
	string::size_type sPos = strCuid.find("@");
	if (string::npos != sPos)
	{
		strCuid = strCuid.substr(0,sPos);
	}

	const TNatData* ptNatData = FindNatData(strCuid, tAddr.GetNetPort(),dwPlayId);
	if (ptNatData != NULL)
	{
		GLOBELOG(EVENT_LEV, "NatAddr[%u:%d]\n", 
			ptNatData->GetNatIp(), ptNatData->GetNatPort());
		tAddr.SetNetIpv4(ptNatData->GetNatIp());
		tAddr.SetNetPort(ptNatData->GetNatPort());
		return true;
	}
	else
	{
		GLOBELOG(ERROR_LEV, "cu[%s]-[%s:%d]和平台在不同网络，但没有打洞，可能收不到码流\n", 
			strCuid.c_str(),tAddr.GetNetIp().c_str(), tAddr.GetNetPort());
		return false;
	}
}

inline void GetOffSetString(u32 dwOffSet, string &strOffSet)
{
    for ( u32 i=0;i<dwOffSet;i++ )
    {
        strOffSet += "\t";
    }
}

string GetPriString(const CEncoderPrivilege &cPri)
{
	string sValue;
	if (cPri.HasPrivilege(CEncoderPrivilege::en_Vod_Rec))
		sValue += "[前端录像]";

	if (cPri.HasPrivilege(CEncoderPrivilege::en_Vod_Play))
		sValue += "[前端放像]";

	if (cPri.HasPrivilege(CEncoderPrivilege::en_Vod_Del))
		sValue += "[前端录像删除]";

	if (cPri.HasPrivilege(CEncoderPrivilege::en_Vod_Download))
		sValue += "[前端录像下载]";

	if (cPri.HasPrivilege(CEncoderPrivilege::en_Param_Config))
		sValue += "[前端参数设置]";

	if (cPri.HasPrivilege(CEncoderPrivilege::en_IO_Ctrl))
		sValue += "[前端开关量输出控制]";

	if (cPri.HasPrivilege(CEncoderPrivilege::en_Defence_Ctrl))
		sValue += "[布防/撤防]";

	if (sValue.empty())
	{
		sValue = "没有任何权限";
	}

	return sValue;
}

string GetPriString(const CVideosourcePrivilege &cPri)
{
	string sValue;

	// PTZLevel 最大值 CMS_PTZ_MAX_PRI_LEV (10)
	{
		int ptzLevel = cPri.GetPTZCtrl();
		stringstream ssPri;
		ssPri << ptzLevel;
		sValue = "[云镜控制级别:" + ssPri.str() + "]";
	}

	if (cPri.HasPrivilege(CVideosourcePrivilege::en_AUDIO_PLAY))
		sValue += "[声音播放]";

	if (cPri.HasPrivilege(CVideosourcePrivilege::en_PrePos_Set))
		sValue += "[预置位设置]";

	if (cPri.HasPrivilege(CVideosourcePrivilege::en_PTZ_Lock))
		sValue += "[云台锁定]";

	return sValue;
}

string GetPriString(const CUserPrivilege &cPri)
{
	string sValue;

	if (cPri.HasPrivilege(CUserPrivilege::en_Vod_Rec))
		sValue += "[平台录像]";
	
	if (cPri.HasPrivilege(CUserPrivilege::en_Vod_Play))
		sValue += "[平台放像]";

	if (cPri.HasPrivilege(CUserPrivilege::en_Vod_Del))
		sValue += "[平台录像删除]";

	if (cPri.HasPrivilege(CUserPrivilege::en_Vod_Download))
		sValue += "[平台录像下载]";

	if (cPri.HasPrivilege(CUserPrivilege::en_Vod_Local_Rec))
		sValue += "[本地录像，本地抓拍]";

	if (cPri.HasPrivilege(CUserPrivilege::en_Rec_Query_Remote))
		sValue += "[跨域录像查询]";

	if (cPri.HasPrivilege(CUserPrivilege::en_Call_Ctrl))
		sValue += "[呼叫控制]";

	if (cPri.HasPrivilege(CUserPrivilege::en_Alarm_Manage))
		sValue += "[现场告警管理]";

	//if (cPri.HasPrivilege(CUserPrivilege::en_Explore))
	//	sValue += "[视频浏览权限]";

	//if (cPri.HasPrivilege(CUserPrivilege::en_Cycle_Explore))
	//	sValue += "[轮巡权限]";

	if (sValue.empty())
	{
		sValue = "没有任何权限";
	}

	return sValue;
}

void CPrivilege::SetPri( const int nPri )
{
    m_setPri.insert(nPri);
}

void CPrivilege::SetPri( const CBitsContainer &cPri )
{
    for ( u32 i = 0; i<32; i++ )
    {
        if ( cPri.IsBitOn(i) )
        {
            u32 dwValue = i;
            m_setPri.insert(dwValue);
        }
    }
}
bool CPrivilege::HasPri( const int nPri )
{
    return m_setPri.find(nPri-1) != m_setPri.end();
}

void CPrivilege::Clear()
{
    m_setPri.clear();
}

/*====================================================================
函数名      : GetPtzPriLevel
功能        : 获取PTZ的权限等级
算法实现    : 视频源的PTZ权限也是采用bit来存储，不过为了XML阅读方便，
             不是以数值（二进制bit位）存储，而是以低10 bit，每一个bit来标识一个数值
             例如 0x08 0000 1000 表示权限 3，第0 bit不用
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/1/31/             liangli
====================================================================*/
int CPrivilege::GetPtzPriLevel()
{
    return GetPtzPriLevelFromSet(m_setPri, CMS_PTZ_MAX_PRI_LEV);
}

string CPrivilege::ToString()
{
    string strPri("00000000000000000000000000000000");
    set<int>::iterator pPri = m_setPri.begin();
    while ( pPri != m_setPri.end() )
    {
        u32 dwTem = (u32)*pPri;
        strPri[dwTem] = '1';
        pPri ++;
    }
    return strPri;
}

bool CPrivilege::IsEmpty() const
{
    return m_setPri.empty();
}

bool CUserAccountBaseInfo::InGroup()
{
    return (! m_tUserGroupID.empty());
}

CUserAccount::CUserAccount() : CUserAccountBaseInfo()
{
    m_pcUserMgr = NULL;
    m_bDevPriState = DEVPRI_NOT_START;
    m_dwStreamConcurrency = 0;
}

CUserAccount::CUserAccount(PCUserMgr pcUserMgr) : CUserAccountBaseInfo()
{
    m_pcUserMgr = pcUserMgr;
    m_bDevPriState = DEVPRI_NOT_START;
    m_dwStreamConcurrency = 0;
}

CUserAccount::~CUserAccount()
{
    m_pcUserMgr = NULL;
}

list<PCuAgent>& CUserAccount::GetCuAgentList()
{
    return m_lstCuAgentList;
}

void CUserAccountBaseInfo::SetUserAccountBaseInfo( CUserInfo &cUserInfo, const string &strUserName )
{
    m_tUserID = cUserInfo.GetUserID();
    m_strUserName = strUserName;
    m_strPasswd = cUserInfo.GetUserPwd();
    m_tUserGroupID = cUserInfo.GetUsergrpID();
    m_tUserPri.SetPri(cUserInfo.GetUserPrivilege());
    m_nUserType = cUserInfo.GetUserType();
    m_cStrmRestrict.SetInAll( cUserInfo.GetSwitchNum() );
}

bool CUserAccount::AddCuAgent( PCuAgent pcCuAgent )
{
    CMS_ASSERT(NULL != pcCuAgent);
    CMS_ASSERT( 0 == strcmp(pcCuAgent->GetObjName(), "CCuAgent") );
    if (NULL != pcCuAgent)
    {
        m_lstCuAgentList.push_back(pcCuAgent);
        return true;
    }
    return false;
}

bool CUserAccount::DeleteCuAgent(const PCuAgent pcCuAgent )
{
    CMS_ASSERT(pcCuAgent != NULL);

    list<PCuAgent>::iterator pItem = m_lstCuAgentList.begin();
    while ( pItem != m_lstCuAgentList.end() )
    {
        if ( pcCuAgent == *(pItem) )
        {
            pItem = m_lstCuAgentList.erase(pItem);
            return true;
        }

        pItem ++;
    }
    return false;
}

bool CUserAccount::DelUserDevInfo( const string &strDevUri , bool bIsTvWall)
{
    if ( strDevUri.empty() )
    {
        return false;
    }
    
	if (bIsTvWall)
	{
		return true;
	}

    CMS_ASSERT(NULL != m_pcUserMgr);

    if ( InGroup() )
    {
        CUserGroup *pcUserGroup = m_pcUserMgr->FindUserGroupByGroupID(m_tUserGroupID);
        CMS_ASSERT( NULL != pcUserGroup );
        if ( pcUserGroup == NULL )
        {
            CLASSLOG(CUI_DATA, ERROR_LEV, "用户[%s]处于用户组[%s]中，但是用户组不存在\n",
                m_strUserName.c_str(), m_tUserGroupID.c_str());
            return false;
        }
    }

	//如果设备存在，还需要通知所有在线CuAgent
	list<PCuAgent>::iterator pItem = m_lstCuAgentList.begin();
	while ( pItem != m_lstCuAgentList.end() )
	{
		PCuAgent pcCuAgent = (PCuAgent)(*pItem);
		CMS_ASSERT(NULL != pcCuAgent);
		CMS_ASSERT(0 == strcmp(pcCuAgent->GetObjName(), "CCuAgent"));

		if ( (NULL != pcCuAgent)
			&& (0 == strcmp(pcCuAgent->GetObjName(), "CCuAgent")) )
		{
			C3acAgent* pcTasAgent =(C3acAgent*)g_cCuiDataMgr.Get3asAgentTaskPtrBy3asName(pcCuAgent->m_str3asName, pcCuAgent->GetInstance()); 
			if( pcTasAgent != NULL)
			{
				pcCuAgent->m_cNotifyPool.InsertDeviceDelNotify(strDevUri);
			}
		}
		pItem++;
	}

	return true;
}

s32 CUserAccount::GetUserType()
{
    CMS_ASSERT(NULL != m_pcUserMgr);

    if ( InGroup() )
    {
        CUserGroup *pcUserGroup = m_pcUserMgr->FindUserGroupByGroupID(m_tUserGroupID);
        CMS_ASSERT( NULL != pcUserGroup );
        if ( pcUserGroup == NULL )
        {
            CLASSLOG(CUI_DATA, ERROR_LEV, "用户[%s]处于用户组[%s]中，但是用户组不存在\n",
                m_strUserName.c_str(), m_tUserGroupID.c_str());
            return CUSER_TYPE_OPERATOR;
        }

        return pcUserGroup->m_nGroupType;
    }

    return m_nUserType;
}

bool CUserAccount::IsSuperAdmin()
{
	if ((u32)m_nUserType == CUSER_TYPE_SUPERADMIN)
	{
		return true;
	}

	return false;
}

bool CUserAccount::IncreaseConcurrency()
{
    CStreamConcurrencyRestrict *pcConcurRestrict = NULL;
    if( InGroup() )
    {
        CUserGroup *pcUserGroup = m_pcUserMgr->FindUserGroupByGroupID(m_tUserGroupID);
        CMS_ASSERT( NULL != pcUserGroup );
        pcConcurRestrict = &pcUserGroup->m_cStrmRestrict;
    }
    else
    {
        pcConcurRestrict = &m_cStrmRestrict;
    }
    if( pcConcurRestrict == NULL )
    {
        return false;
    }
    bool bResult = pcConcurRestrict->IsIncreAllowed( m_dwStreamConcurrency );
    if( bResult )
    {
        ++m_dwStreamConcurrency;
    }
    return bResult;
}

void CUserAccount::DecreaseConcurrency()
{
    if( m_dwStreamConcurrency > 0 )
    {
        --m_dwStreamConcurrency;
    }
}


CPrivilege &CUserAccount::GetUserPri()
{
    CMS_ASSERT(NULL != m_pcUserMgr);

    if ( InGroup() )
    {
        CUserGroup *pcUserGroup = m_pcUserMgr->FindUserGroupByGroupID(m_tUserGroupID);
        CMS_ASSERT( NULL != pcUserGroup );
        if ( pcUserGroup == NULL )
        {
            CLASSLOG(CUI_DATA, ERROR_LEV, "用户[%s]处于用户组[%s]中，但是用户组不存在\n",
                m_strUserName.c_str(), m_tUserGroupID.c_str());
            return m_tUserPri;
        }

        return pcUserGroup->m_tUserGroupPri;
    }

    return m_tUserPri;
}

void CUserAccount::PrintData( u32 dwOffSet /*= 0*/, u32 dwMaxDev /*=(~(u32)0)*/ )
{
    string strOffSet;
    GetOffSetString(dwOffSet, strOffSet);

    OspPrintf(TRUE, FALSE, "%s用户ID: %s\n", 
        strOffSet.c_str(), m_tUserID.c_str());
    OspPrintf(TRUE, FALSE, "%s用户名称: %s\n", 
        strOffSet.c_str(), m_strUserName.c_str());
    OspPrintf(TRUE, FALSE, "%s用户组ID: %s\n", 
        strOffSet.c_str(), m_tUserGroupID.c_str());
    OspPrintf(TRUE, FALSE, "%s用户权限: %s\n", 
        strOffSet.c_str(), m_tUserPri.ToString().c_str());
    OspPrintf(TRUE, FALSE, "%s用户类型: %d\n", 
        strOffSet.c_str(), m_nUserType);

	OspPrintf(TRUE, FALSE, "%s在线Cu列表: [%d]个\n", 
        strOffSet.c_str(), m_lstCuAgentList.size());
    list<PCuAgent>::iterator pCuItem = m_lstCuAgentList.begin();
    while ( pCuItem != m_lstCuAgentList.end() )
    {
        OspPrintf(TRUE, FALSE, "%s\tTaskNO[%u], 会话ID[%s]\n", 
            strOffSet.c_str(), (*pCuItem)->GetTaskNO(), (*pCuItem)->m_strSession.c_str());
        pCuItem++;
    }
}

void CUserGroupBaseInfo::SetUserGroupBaseInfo(CUserGroupInfo &cUserGrpInfo)
{
    m_tGroupID = cUserGrpInfo.GetGroupNO();
    m_strGroupName = cUserGrpInfo.GetGroupName();
    m_tUserGroupPri.SetPri(cUserGrpInfo.GetUserPrivilege());
    m_nGroupType = cUserGrpInfo.GetUserType();
    m_cStrmRestrict.SetInAll( cUserGrpInfo.GetSwitchNum() );
}

void CUserGroup::PrintData( u32 dwOffSet /*= 0*/, u32 dwMaxDev /*=(~(u32)0)*/ )
{
    string strOffSet;
    GetOffSetString(dwOffSet, strOffSet);

    OspPrintf(TRUE, FALSE, "%s用户组名称: %s\n", 
        strOffSet.c_str(), m_strGroupName.c_str());
    OspPrintf(TRUE, FALSE, "%s用户组ID: %s\n", 
        strOffSet.c_str(), m_tGroupID.c_str());
    OspPrintf(TRUE, FALSE, "%s用户组权限: %s\n", 
        strOffSet.c_str(), m_tUserGroupPri.ToString().c_str());
    OspPrintf(TRUE, FALSE, "%s用户组类型: %d\n", 
        strOffSet.c_str(), m_nGroupType);

}

bool CUserGroup::DelUserDevInfo( const string &strDevUri )
{
    CMS_ASSERT(NULL != m_pcUserMgr);
    //先对URI进行规整，提取UUID
    if ( strDevUri.empty() )
    {
        return false;
    }

    //如果设备存在，还需要通知所有在线CuAgent
    Iterator pPos;
    while ( ! pPos.End() )
    {
        string *pcUserName = NULL;
        CUserAccount **ppcUserAccount = NULL;
        CUserAccount *pcUserAccount = NULL;
        if ( m_pcUserMgr->m_mapUserName2PCuserAccount.Iterate(pPos, pcUserName, ppcUserAccount) )
        {
            CMS_ASSERT(NULL != pcUserName);
            CMS_ASSERT(NULL != ppcUserAccount);
            pcUserAccount = *ppcUserAccount;
            if ( pcUserAccount->m_tUserGroupID == m_tGroupID )
            {                    
                list<PCuAgent>::iterator pItem = pcUserAccount->GetCuAgentList().begin();
                while ( pItem != pcUserAccount->GetCuAgentList().end() )
                {
                    PCuAgent pcCuAgent = (PCuAgent)(*pItem);
                    CMS_ASSERT(NULL != pcCuAgent);
                    CMS_ASSERT(0 == strcmp(pcCuAgent->GetObjName(), "CCuAgent"));

					if ( (NULL != pcCuAgent)
						&& (0 == strcmp(pcCuAgent->GetObjName(), "CCuAgent")) )
					{
						C3acAgent* pcTasAgent =(C3acAgent*)g_cCuiDataMgr.Get3asAgentTaskPtrBy3asName(pcCuAgent->m_str3asName, pcCuAgent->GetInstance()); 
						if( pcTasAgent != NULL)
						{
							pcCuAgent->m_cNotifyPool.InsertDeviceDelNotify(strDevUri);
						}
					}
                    pItem++;
                }
            }
        }
    }
	
	return true;
}

CUserAccount * CUserMgr::FindUserAccountByUserName( const string &strUserName )
{
    CUserAccount ** pc = m_mapUserName2PCuserAccount.Find(strUserName);
    return pc!=NULL? *pc:NULL;
}

CUserGroup * CUserMgr::FindUserGroupByGroupID( const string &strGroupID )
{
    return m_mapGroupID2UserGroup.Find(strGroupID);
}

CUserAccount * CUserMgr::FindUserAccountByUserID( const string &tUserID )
{
    string strUserName;
    if ( ! m_mapUserID2UserName.Find(tUserID, strUserName) )
    {
        CLASSLOG(CUI_DATA, ERROR_LEV, "找不到UserID[%s]对应的UserName\n",
            tUserID.c_str());
        return NULL;
    }

    return FindUserAccountByUserName(strUserName);
}

CUserMgr::~CUserMgr()
{
    Iterator pUserAccountItem;
    while ( ! pUserAccountItem.End() )
    {
        string *pUserName = NULL;
        CUserAccount **ppcUserAccount = NULL;
        if ( m_mapUserName2PCuserAccount.Iterate(pUserAccountItem, pUserName, ppcUserAccount) )
        {
            delete (*ppcUserAccount);
        }
    }
}

CDomainInfoMap::~CDomainInfoMap()
{
    Iterator pPos;
    while (!pPos.End())
    {
        string strDomainName;
        CDomainRelationInfo* pcDomainInfo;
        if (m_cDomainNameMap.Iterate(pPos, strDomainName, pcDomainInfo) && NULL != pcDomainInfo)
        {
            SAFE_DEL_PTR(pcDomainInfo);
        }
    }
};
void CDomainInfoMap::SetDomainInfo(const CDomainRelationInfo& cInfo)
{
    if (cInfo.GetDomainID().empty() || cInfo.GetDomainName().empty())
    {
        CLASSLOG(ERROR_LEV, "无效的域信息,过滤：\n%s\n", cInfo.ToXml().c_str());
        return;
    }
    CDomainRelationInfo* pcDomainInfo = new CDomainRelationInfo(cInfo);
    m_cDomainIdMap[cInfo.GetDomainID()] = pcDomainInfo;
    m_cDomainNameMap[cInfo.GetDomainName()] = pcDomainInfo;
}
void CDomainInfoMap::ModDomainInfo(const CDomainRelationInfo& cInfo)
{
    if (cInfo.GetDomainID().empty() || cInfo.GetDomainName().empty())
    {
        CLASSLOG(ERROR_LEV, "无效的域信息,过滤：\n%s\n", cInfo.ToXml().c_str());
        return;
    }
    DelDomainInfo(cInfo.GetDomainID());
    CDomainRelationInfo* pcDomainInfo = new CDomainRelationInfo(cInfo);
    m_cDomainIdMap[cInfo.GetDomainID()] = pcDomainInfo;
    m_cDomainNameMap[cInfo.GetDomainName()] = pcDomainInfo;
}
void CDomainInfoMap::DelDomainInfo(const string& strDomainId)
{
    CDomainRelationInfo* pcDomainInfo = this->FindDomainInfoById(strDomainId);
    if (NULL != pcDomainInfo)
    {
        m_cDomainIdMap.Erase(strDomainId);
        m_cDomainNameMap.Erase(pcDomainInfo->GetDomainName());
        SAFE_DEL_PTR(pcDomainInfo);
    }
}
CDomainRelationInfo* CDomainInfoMap::FindDomainInfoById(const string& strDomainId)
{
    CDomainRelationInfo** ppcDomainInfo = m_cDomainIdMap.Find(strDomainId);
    if (NULL != ppcDomainInfo)
    {
        return *ppcDomainInfo;
    }
    CLASSLOG(ERROR_LEV, "DomainId[%s]不存在\n", strDomainId.c_str());
    return NULL;
}
CDomainRelationInfo* CDomainInfoMap::FindDomainInfoByName(const string& strDomainName)
{
    CDomainRelationInfo** ppcDomainInfo = m_cDomainNameMap.Find(strDomainName);
    if (NULL != ppcDomainInfo)
    {
        return *ppcDomainInfo;
    }
    CLASSLOG(ERROR_LEV, "DomainName[%s]不存在\n", strDomainName.c_str());
    return NULL;
}
string CDomainInfoMap::GetNextDomainNameTo(const string& strDstDomainName)
{
    if (g_cCuiConfig.GetLocalURI().GetDomain() == strDstDomainName)
    {
        CLASSLOG(WARNING_LEV, "DomainName[%s]是本域域名，不处理!\n", strDstDomainName.c_str());
        return strDstDomainName;
    }
    string strParentDomainName = ::GetParentDomainName();   //::GetParentPlatformProxyUrl();
    CDomainRelationInfo* pcDomainInfo = this->FindDomainInfoByName(strDstDomainName);
    if (NULL == pcDomainInfo)
    {
        CLASSLOG(CRITICAL_LEV, "找不到DomainName[%s]默认返回上级DomainName[%s]!\n", 
            strDstDomainName.c_str(), strParentDomainName.c_str());
        return strParentDomainName;
    }
    string strParentDomainID;
    const int MAX_DOMAIN_NUM = 1000;
    for(int i = 0; i < MAX_DOMAIN_NUM; i++) 
    {
        strParentDomainID = pcDomainInfo->GetParentDomainID();
        if ( ::GetDomainUUID() == strParentDomainID)
        {
            CLASSLOG(EVENT_LEV, "找到Domain[%s] 它的 ParentDomainID[%s] 是本域 \n", 
                pcDomainInfo->GetDomainName().c_str(), strParentDomainID.c_str());
            return pcDomainInfo->GetDomainName();
        }
        pcDomainInfo = this->FindDomainInfoById(strParentDomainID);
        if (NULL == pcDomainInfo)
        {
            CLASSLOG(CRITICAL_LEV, "找不到DomainId[%s]默认返回上级DomainName[%s]!\n",
                strParentDomainID.c_str(), strParentDomainName.c_str());
            return strParentDomainName;
        }
    }
    CLASSLOG(EVENT_LEV, "遍历域个数超过最大值[%ld], 最后一个查找的域ID为【%s】\n", MAX_DOMAIN_NUM, strParentDomainID.c_str());
    return "";
}
string CDomainInfoMap::DomainIdToName(const string& strDomainId)
{
    CDomainRelationInfo* pcDomainInfo = this->FindDomainInfoById(strDomainId);
    if (NULL != pcDomainInfo)
    {
        return pcDomainInfo->GetDomainName();
    }
    return "";
}
string CDomainInfoMap::DomainNameToId(const string& strDomainName)
{
    CDomainRelationInfo* pcDomainInfo = this->FindDomainInfoByName(strDomainName);
    if (NULL != pcDomainInfo)
    {
        return pcDomainInfo->GetDomainID();
    }
    return "";
}
void CDomainInfoMap::PrintData()
{
    Iterator pPos;
    u32 i = 0;
    while (!pPos.End())
    {
        string strDomainName;
        CDomainRelationInfo* pcDomainInfo;
        if (m_cDomainNameMap.Iterate(pPos, strDomainName, pcDomainInfo))
        {
            string strType;
            if (pcDomainInfo->GetDomainID() == ::GetDomainUUID())
            {
                strType = "self";
            }
            else
            {
                strType = "other";
            }
            i++;
            OspPrintf(TRUE, FALSE, "  %4u. DomainName[%s] DomainId[%s] ParentDomainID[%s], %s\n", 
                i, 
                pcDomainInfo->GetDomainName().c_str(), 
                pcDomainInfo->GetDomainID().c_str(), 
                pcDomainInfo->GetParentDomainID().c_str(),
                strType.c_str());        
        }
    }
    OspPrintf(TRUE, FALSE, " \n");
    OspPrintf(TRUE, FALSE, "DomainInfo：[%u]个\n", m_cDomainIdMap.GetSize()); 
}

CCuiDataMgr::CCuiDataMgr( )
{
	m_dwBatOptCheckTaskNo = INVALID_TASKNO;
}

u32 CCuiDataMgr::GetTaskIDBySession( const string &strSession )
{
/*    Iterator pPos;
    while ( !pPos.End() )
    {
        u32 dwTaskID = INVALID_TASKNO;
        string strSess;
        if ( m_mapSessToTaskMap.Iterate(pPos, strSess, dwTaskID) )
        {
            if ( strSess == strSession )
            {
                return dwTaskID;
            }
        }
    }
    */
    u32 dwTaskID = INVALID_TASKNO;
    if ( m_mapSessToTaskMap.Find(strSession, dwTaskID) )
    {
        return dwTaskID;
    }

    return INVALID_TASKNO;
}
CSipTask * CCuiDataMgr::GetTaskPtrBySession( const string &strSession, const CInstExt *pcInst )
{
    u32 dwTaskID = GetTaskIDBySession(strSession);
    if ( INVALID_TASKNO == dwTaskID )
    {
        return NULL;
    }
    return static_cast<CSipTask *>(pcInst->FindTask(dwTaskID));
}

u32 CCuiDataMgr::Get3asAgentTaskIDBy3asName( const string &str3asName )
{
    u32 dwTaskID = INVALID_TASKNO;
    if ( m_map3asURITo3asAgentMap.Find(str3asName, dwTaskID) )
    {
        return dwTaskID;
    }
    return INVALID_TASKNO;
}

u32 CCuiDataMgr::GetPeAgentTaskIDByPeName(const string &strPeName)
{
    u32 dwTaskID = INVALID_TASKNO;
    if ( m_mapPeNameToPeTaskMap.Find(strPeName, dwTaskID) )
    {
        return dwTaskID;
    }
    return INVALID_TASKNO;
}

u32 CCuiDataMgr::GetSubTaskIDByDevUri(const string& strURI)
{
    u32 dwTaskID = INVALID_TASKNO;
    if( m_mapDevUriTypeToSubTaskMap.Find(strURI, dwTaskID))
    {
        return dwTaskID;
    }
    return INVALID_TASKNO;
}

CSipTask * CCuiDataMgr::Get3asAgentTaskPtrBy3asName( const string &str3asName, const CInstExt *pcInst )
{
    u32 dwTaskID = Get3asAgentTaskIDBy3asName(str3asName);
    if ( INVALID_TASKNO == dwTaskID )
    {
        return NULL;
    }
    return static_cast<CSipTask *>(pcInst->FindTask(dwTaskID));
}

CSipTask * CCuiDataMgr::GetPeAgentPtrByPeName(const string &strPeName, const CInstExt *pcInst)
{
    u32 dwTaskID = GetPeAgentTaskIDByPeName(strPeName);
    if ( INVALID_TASKNO == dwTaskID )
    {
        return NULL;
    }
    return static_cast<CSipTask *>(pcInst->FindTask(dwTaskID));
}

CSipTask * CCuiDataMgr::GetSubTaskPtrByDevUri(const string& strDevUri, const CInstExt *pcInst)
{
    u32 dwTaskID = GetSubTaskIDByDevUri(strDevUri);
    if(INVALID_TASKNO == dwTaskID)
    {
        return NULL;
    }
    return static_cast<CSipTask *>(pcInst->FindTask(dwTaskID));
}

void CCuiDataMgr::ClearAllData()
{
    m_mapSessToTaskMap.Empty();
    m_map3asURITo3asAgentMap.Empty();
    m_mapDevUriTypeToSubTaskMap.Empty();
	m_dwBatOptCheckTaskNo = INVALID_TASKNO;
	m_cPuBatOptRequestList.Clear();
	m_cPlatBatOptRequestList.Clear();
}

// const string CCuiDataMgr::GetDomainNameByDomainID( const string &strDomainID )
// {
//     string strDomainName;
//     m_mapDomainIdToDomainNameMap.Find(strDomainID, strDomainName);
//     return strDomainName;
// }

void CCuiDataMgr::PrintData(const CInstExt *pcInst, u32 dwOffSet /*= 0*/ )
{
    string strOffSet;
    GetOffSetString(dwOffSet, strOffSet);

    u32 dwIndex = 0;
    OspPrintf(TRUE, FALSE, "%s域名对应的3asAgent列表\n",
        strOffSet.c_str());
    Iterator pPos;
    if ( m_map3asURITo3asAgentMap.GetSize() == 0 )
    {
        OspPrintf(TRUE, FALSE, "%s\t当前3asAgent列表为空\n",
            strOffSet.c_str());
    }
    else
    {
        while ( ! pPos.End() )
        {
            string str3asName;
            u32 dwTaskID = INVALID_TASKNO;
            if ( m_map3asURITo3asAgentMap.Iterate(pPos, str3asName, dwTaskID) )
            {
                CMS_ASSERT(INVALID_TASKNO != dwTaskID);
                OspPrintf(TRUE, FALSE, "%s\t[%02u] 域名[%s]--3acAgent TaskID[%u]\n",
                    strOffSet.c_str(), dwIndex++, 
                    str3asName.c_str(), dwTaskID);
            }
        }
        OspPrintf(TRUE, FALSE, "%s\t总计 %d 个\n",
            strOffSet.c_str(), m_map3asURITo3asAgentMap.GetSize());
    }

    OspPrintf(TRUE, FALSE, "\n会话对应Task列表，Task可能是Cu/Tas Agent，也可能是QueryTask\n");
    pPos.SetToBegin();
    dwIndex = 0;
    if ( m_mapSessToTaskMap.GetSize() == 0 )
    {
        OspPrintf(TRUE, FALSE, "%s\t当前会话列表为空\n",
            strOffSet.c_str());
    }
    else
    {
        while ( ! pPos.End() )
        {
            string strSession;
            u32 dwTaskID = INVALID_TASKNO;
            if ( m_mapSessToTaskMap.Iterate(pPos, strSession, dwTaskID) )
            {
                CMS_ASSERT(INVALID_TASKNO != dwTaskID);
                CSipTask *pcTask = (CSipTask *)(pcInst->FindTask(dwTaskID));
                CMS_ASSERT(NULL != pcTask);
                OspPrintf(TRUE, FALSE, "%s%02u 会话[%s]--Task[%s-%u]\n",
                    strOffSet.c_str(), dwIndex++, 
                    strSession.c_str(), pcTask->GetObjName(), dwTaskID);
            }
        }
        OspPrintf(TRUE, FALSE, "%s\t总计 %d 个\n",
            strOffSet.c_str(), m_mapSessToTaskMap.GetSize());
    }

    OspPrintf(TRUE, FALSE, "\n订阅Task列表，SubKey --> TaskNo\n");
    pPos.SetToBegin();
    dwIndex = 0;
    if ( m_mapDevUriTypeToSubTaskMap.GetSize() == 0 )
    {
        OspPrintf(TRUE, FALSE, "%s\t当前订阅列表为空\n",
            strOffSet.c_str());
    }
    else
    {
        while ( ! pPos.End() )
        {
            string strSubKey;
            u32 dwTaskID = INVALID_TASKNO;
            if ( m_mapDevUriTypeToSubTaskMap.Iterate(pPos, strSubKey, dwTaskID) )
            {
                CMS_ASSERT(INVALID_TASKNO != dwTaskID);
                CSipTask *pcTask = (CSipTask *)(pcInst->FindTask(dwTaskID));
                CMS_ASSERT(NULL != pcTask);
                OspPrintf(TRUE, FALSE, "%s%02u SubKey[%s]--Task[%s-%u]\n",
                    strOffSet.c_str(), dwIndex++, 
                    strSubKey.c_str(), pcTask->GetObjName(), dwTaskID);
            }
        }
        OspPrintf(TRUE, FALSE, "%s\t总计 %d 个\n",
            strOffSet.c_str(), m_mapDevUriTypeToSubTaskMap.GetSize());
    }
}

void CCuiDataMgr::SetCuiStateHappenTime()
{
    time_t tCurrTime;
    time(&tCurrTime);

    m_cCuiState.m_tCuiStateTime.SetTime(tCurrTime);
}

void CCuiDataMgr::SetCuiBeginRunTime()
{
    time_t tCurrTime;
    time(&tCurrTime);
    m_cCuiState.m_tCuiBeginRun.SetTime(tCurrTime);
}

bool CCuiDataMgr::IsHasDevPrivilege( const string &strCuSession, const CInstExt *pcInst, const string &strDevUri, s32 nPrivilege )
{
    CCuAgent *pcCuAgent = dynamic_cast<CCuAgent*>(GetTaskPtrBySession(strCuSession, pcInst));
    if (NULL != pcCuAgent)
    {
        return pcCuAgent->IsHasDevPrivilege(strDevUri, nPrivilege);
    }
    else
    {
        return false;
    }
}

void CCuiDataMgr::FlushPlatformServiceStatus( const CInstExt *pcInst, CCuAgent *pcAgent, const TSipURI &tPeUri )
{
    vector<TServiceStatus> arrServiceStatus; arrServiceStatus.reserve(10);
    TServiceStatus tServiceStatus;
    Iterator pPos4Status;
    u32 dwTaskID4Status;
    string strPeName;
    string strRcsPe(string(DEV_TYPE_RCS)+"@"+GetDomainName());
    string strAlsPe(string(DEV_TYPE_ALS)+"@"+GetDomainName());
    string strTvsPe(string(DEV_TYPE_TVS)+"@"+GetDomainName());
    string strMpsPe(string(DEV_TYPE_MPS)+"@"+GetDomainName());
	string strUasPe(string(DEV_TYPE_UAS) + "@" + GetDomainName());
	//string strDpsPe(string(DEV_TYPE_DPS) + "@" + GetDomainName());

    string strCertainPe = tPeUri.GetURIString();
    bool bCertainPeValid = tPeUri.IsValidURI();
    while(!pPos4Status.End())
    {
        if (!m_mapPeNameToPeTaskMap.Iterate(pPos4Status, strPeName, dwTaskID4Status))
        {
            break;
        }

        if (bCertainPeValid && strPeName != strCertainPe)
        {
            continue;
        }

        tServiceStatus.bInService = 
            (CPeAgent::Service == dynamic_cast<CPeAgent*>(pcInst->FindTask(dwTaskID4Status))->GetState());
        if (strPeName == strRcsPe)
        {
            tServiceStatus.nServiceType = TServiceStatus::enServiceRecord;
            arrServiceStatus.push_back(tServiceStatus);
        }
        else if (strPeName == strAlsPe)
        {
            tServiceStatus.nServiceType = TServiceStatus::enServiceAlarmLink;
            arrServiceStatus.push_back(tServiceStatus);
        }
        else if (strPeName == strTvsPe)
        {
            tServiceStatus.nServiceType = TServiceStatus::enServiceTVWall;
            arrServiceStatus.push_back(tServiceStatus);
        }
        else if (strPeName == strMpsPe)
        {
            tServiceStatus.nServiceType = TServiceStatus::enServiceMap;
            arrServiceStatus.push_back(tServiceStatus);
        }
        else if (strPeName == strUasPe)
        {
            tServiceStatus.nServiceType = TServiceStatus::enServiceConfig;
            arrServiceStatus.push_back(tServiceStatus);
            tServiceStatus.nServiceType = TServiceStatus::enServiceLog;
            arrServiceStatus.push_back(tServiceStatus);
        }
    }

    if (NULL != pcAgent)
    {
        for (vector<TServiceStatus>::const_iterator it = arrServiceStatus.begin(); it != arrServiceStatus.end(); ++it)
        {
            pcAgent->m_cNotifyPool.InsertServiceStatusNotify(*it);
        }
        
        return;
    }

    Iterator pPos4Sess;
    u32 dwTaskID4Sess;
    string strSess;
    CCuAgent *pCuAgent;
    while ( !pPos4Sess.End() )
    {
        if ( !m_mapSessToTaskMap.Iterate(pPos4Sess, strSess, dwTaskID4Sess) )
        {
            break;
        }

		CTask* pTask = pcInst->FindTask(dwTaskID4Sess);
		if (pTask == NULL)
		{
			continue;
		}

		if (0 != strcmp(pTask->GetObjName(), "CCuAgent"))
		{
			continue;
		}

		pCuAgent = dynamic_cast<CCuAgent*>(pTask);
		if (pCuAgent->m_strSession != strSess || !pCuAgent->m_bNeedPlatformServiceStatue)
		{
			continue;
		}

        for (vector<TServiceStatus>::const_iterator it = arrServiceStatus.begin(); it != arrServiceStatus.end(); ++it)
        {
            pCuAgent->m_cNotifyPool.InsertServiceStatusNotify(*it);
        }
    }
}

const string& CCuiDataMgr::GetCuserIdByCuserSession( const string &strSession, const CInstExt *pcInst )
{
    CCuAgent *pcCuAgent = dynamic_cast<CCuAgent*>(GetTaskPtrBySession(strSession, pcInst));
    if (NULL != pcCuAgent && NULL != pcCuAgent->m_pcUserAccount)
    {
        return pcCuAgent->m_pcUserAccount->m_tUserID;
    }
    
    static string strNull;
    return strNull;
}

//////////////////////////////////////////////////////////////////////////
CConfig::CConfig()
{
    m_wLocalPort = (u16)CUI_SIP_UDP_PORT;
    m_wListenPort = (u16)CUI_TCP_DAT_RECV_PORT;
    m_wCuMaxNum = (u16)CUI_CU_MAX_NUM;
    m_dwSipWaitRspTimeout = DEFAULT_TIMEOUT;
    m_dwDlgWaitRspTimeout = DEFAULT_TIMEOUT * 7;
    m_dwCuHbTimeout = DEFAULT_TIMEOUT * 2;
    m_dwGetAppQueueSize = DEFAULT_APP_QUEUE_SIZE;
	m_tMapedNatIP=""; 
	m_wMapedNatPort=0;  
	m_wNatPort = DEFAULT_CUI_NAT_PORT;
	m_bEnableMapedNatAddr=FALSE;
	m_dwUpdateNetSegInterval=60*1000;
	m_bNeedCUISwitch = FALSE;
	m_nMaxCuNtfSize = DEFAULT_MAX_CU_NTF_SIZE;
	m_wBatTaskRecNumPerTime = DEFAULT_BATTASK_RECNUM_PERTIME;
	m_wBatTaskIntervalMsPerTime = DEFAULT_BATTASK_INTERVAL_PERTIME;
}

bool CConfig::ReadConfig( LPCSTR lpFileName )
{
    CXmlConfig cXmlCfg(CUI_CONFIG_ROOT);

    //首先判断配置文件是否存在
    if ( !IsFileExist(lpFileName) )
    { //生成默认配置
        u32 adwLocalIPList[20] = {0};
        // 获取IP列表
        //u16 wIpNum = OspAddrListGet( adwLocalIPList, 20);
        struct in_addr inIpV4;
        inIpV4.s_addr = adwLocalIPList[0];
        m_tLocalIP = inet_ntoa(inIpV4);

        m_wLocalPort = (u16)CUI_SIP_UDP_PORT;
        m_wListenPort = (u16)CUI_TCP_DAT_RECV_PORT;
        m_wCuMaxNum = (u16)CUI_CU_MAX_NUM;
        m_dwSipWaitRspTimeout = DEFAULT_TIMEOUT;
        m_dwDlgWaitRspTimeout = DEFAULT_TIMEOUT * 7;
        m_dwCuHbTimeout = DEFAULT_TIMEOUT * 2;
        m_nRouteMode = DEFAULT_ROUTEMODE;

		m_nMaxCuNtfSize = DEFAULT_MAX_CU_NTF_SIZE;

        CUUID cUUID;
		cUUID = string("cui-")+cUUID;
        m_tLocalURI.SetDomain(GetDomainName());
        m_tLocalURI.SetUser(cUUID);

        m_tCmuURI = m_tLocalURI.GetCmuURI();
		m_tSecureModuleURI = m_tLocalURI.GetSecureModuleURI();

        //写入配置文件
        //cXmlCfg.SetRegKeyString(SECTION_CUI_LOCAL_INFO, KEY_LOCAL_IP, m_tLocalIP);
        cXmlCfg.SetRegKeyInt(SECTION_CUI_LOCAL_INFO, KEY_LOCAL_PORT, m_wLocalPort);
        cXmlCfg.SetRegKeyString(SECTION_CUI_LOCAL_INFO, KEY_CUI_UUID, m_tLocalURI.GetUser());
        cXmlCfg.SetRegKeyInt(SECTION_CUI_LOCAL_INFO, KEY_TCP_PORT, m_wListenPort);
        cXmlCfg.SetRegKeyInt(SECTION_CUI_LOCAL_INFO, KEY_CU_MAX_NUM, m_wCuMaxNum);
        cXmlCfg.SetRegKeyInt(SECTION_CUI_LOCAL_INFO, KEY_SIP_RSP_TIMEOUT, (int)m_dwSipWaitRspTimeout);
        cXmlCfg.SetRegKeyInt(SECTION_CUI_LOCAL_INFO, KEY_DLG_RSP_TIMEOUT, (int)m_dwDlgWaitRspTimeout);     
        cXmlCfg.SetRegKeyInt(SECTION_CUI_LOCAL_INFO, KEY_CU_HB_TIMEOUT, (int)m_dwCuHbTimeout);
        cXmlCfg.SetRegKeyInt(SECTION_CUI_LOCAL_INFO, KEY_ROUTEMODE, m_nRouteMode);
        //cXmlCfg.SetRegKeyInt(SECTION_CUI_LOCAL_INFO, KEY_APP_QUEUE_SIZE, m_dwGetAppQueueSize);
        WriteFileFromString(lpFileName, cXmlCfg.GetXmlString());
        return true;
    }

    //读取配置文件
    string strXmlFile;
    ReadFileToString(lpFileName, strXmlFile);

    cXmlCfg.SetXmlString(strXmlFile);
    
    int nRet = 0;
    cXmlCfg.GetRegKeyInt(SECTION_CUI_LOCAL_INFO, KEY_LOCAL_PORT, DEFAULT_PORT, nRet);
    m_wLocalPort = (u16)nRet;
    cXmlCfg.GetRegKeyString(SECTION_CUI_LOCAL_INFO, KEY_CUI_UUID, DEFAULT_UUID, strXmlFile);
    if ( strXmlFile.empty() )
    {
        CUUID cUUID;
        m_tLocalURI.SetUser(cUUID);
        cXmlCfg.SetRegKeyString(SECTION_CUI_LOCAL_INFO, KEY_CUI_UUID, cUUID);
        WriteFileFromString(lpFileName, cXmlCfg.GetXmlString());
    }
    else
    {
        m_tLocalURI.SetUser(strXmlFile);
    }
    m_tLocalURI.SetDomain(GetDomainName());
    m_tCmuURI = m_tLocalURI.GetCmuURI();
	m_tSecureModuleURI = m_tLocalURI.GetSecureModuleURI();
    cXmlCfg.GetRegKeyInt(SECTION_CUI_LOCAL_INFO, KEY_TCP_PORT, DEFAULT_PORT, nRet);
    m_wListenPort = (u16)nRet;

    cXmlCfg.GetRegKeyInt(SECTION_CUI_LOCAL_INFO, KEY_CU_MAX_NUM, DEFAULT_CU_MAX_NUM, nRet);
    m_wCuMaxNum = (u16)nRet;
    cXmlCfg.GetRegKeyInt(SECTION_CUI_LOCAL_INFO, KEY_SUBSCRIBE_FLOW_LIMIT, DEFAULT_SUBSCRIBE_FLOW_LIMIT, nRet);
    m_wSubscribeFlowLimit = (u16)nRet;
    cXmlCfg.GetRegKeyInt(SECTION_CUI_LOCAL_INFO, KEY_UNSUBSCRIBE_FLOW_LIMIT, DEFAULT_UNSUBSCRIBE_FLOW_LIMIT, nRet);
    m_wUnsubscribeFlowLimit = (u16)nRet;

    cXmlCfg.GetRegKeyInt(SECTION_CUI_LOCAL_INFO, KEY_SIP_RSP_TIMEOUT, DEFAULT_TIMEOUT, nRet);
    m_dwSipWaitRspTimeout = (u32)nRet;
    cXmlCfg.GetRegKeyInt(SECTION_CUI_LOCAL_INFO, KEY_DLG_RSP_TIMEOUT, DEFAULT_TIMEOUT, nRet);
    m_dwDlgWaitRspTimeout = (u32)nRet;
    cXmlCfg.GetRegKeyInt(SECTION_CUI_LOCAL_INFO, KEY_CU_HB_TIMEOUT, DEFAULT_TIMEOUT*2, nRet);
    m_dwCuHbTimeout = (u32)nRet;
    cXmlCfg.GetRegKeyInt(SECTION_CUI_LOCAL_INFO, KEY_APP_QUEUE_SIZE, DEFAULT_APP_QUEUE_SIZE, nRet);
    m_dwGetAppQueueSize = (u32)nRet;

    cXmlCfg.GetRegKeyInt(SECTION_CUI_LOCAL_INFO, KEY_ROUTEMODE, DEFAULT_ROUTEMODE, nRet);
    m_nRouteMode = nRet;

    string strCuOperateLog;
    cXmlCfg.GetRegKeyString(SECTION_CUI_LOCAL_INFO, KEY_CU_OPERATELOG, string(), strCuOperateLog);
    if (strCuOperateLog.empty())
    {
        strCuOperateLog = "1,11,16,17,18,19";
    }
    vector<u32> vecCuOperateLog;
    strtok(strCuOperateLog.c_str(), ",", vecCuOperateLog);
    for (vector<u32>::const_iterator it = vecCuOperateLog.begin(); it != vecCuOperateLog.end(); ++it)
    {
        m_setCuOperateLog.insert((s32)*it);
    }

    cXmlCfg.GetRegKeyInt(SECTION_CUI_LOCAL_INFO, KEY_MOBILE_CLIENT_LIMIT, 20, nRet);
    m_nMobileClientLimit = nRet;

    cXmlCfg.GetRegKeyInt(SECTION_CUI_LOCAL_INFO, KEY_MAX_TRANS, DEFAULT_MAX_TRANS, nRet);
    m_nMaxTrans = (s32)nRet;

    cXmlCfg.GetRegKeyInt(SECTION_CUI_LOCAL_INFO, KEY_MAX_REG_DLG, DEFAULT_MAX_REG_DLG, nRet);
    m_nMaxRegDlg = (s32)nRet;

    cXmlCfg.GetRegKeyInt(SECTION_CUI_LOCAL_INFO, KEY_MAX_INVITE_DLG, DEFAULT_MAX_INVITE_DLG, nRet);
    m_nMaxInviteDlg = (s32)nRet;

    cXmlCfg.GetRegKeyInt(SECTION_CUI_LOCAL_INFO, KEY_MAX_SUBSCRIBE_DLG, DEFAULT_MAX_SUBSCRIBE_DLG, nRet);
    m_nMaxSubscribeDlg = (s32)nRet;

	cXmlCfg.GetRegKeyInt(SECTION_CUI_LOCAL_INFO,KEY_MAX_NOTIFY_PERSUB,DEFAULT_MAX_NTF_PERSUB,nRet);
	m_nMaxNtfPerSub = (s32)nRet;

    cXmlCfg.GetRegKeyInt(SECTION_CUI_LOCAL_INFO, KEY_MAX_DISABLE_CHECK_SUM, DEFAULT_DISABLE_CHECK_SUM, nRet);
    m_bDisableCheckSum = (BOOL32)nRet;

	cXmlCfg.GetRegKeyInt(SECTION_CUI_LOCAL_INFO,KEY_BIND_MAPED_NAT_ADDR,0,nRet);
	m_bEnableMapedNatAddr = (BOOL32)nRet;

	cXmlCfg.GetRegKeyInt(SECTION_CUI_LOCAL_INFO,KEY_UPDATE_NETSEG_INTERVAL,DEFAULT_UPDATE_TIMER,nRet);
	m_dwUpdateNetSegInterval = (u32)(nRet*1000);

	cXmlCfg.GetRegKeyInt(SECTION_CUI_LOCAL_INFO, KEY_NEED_CUI_SWITCH, 0, nRet);
	m_bNeedCUISwitch = (BOOL32)nRet;

	cXmlCfg.GetRegKeyInt(SECTION_CUI_LOCAL_INFO, KEY_BATTASK_RECNUM_PERTIME, DEFAULT_BATTASK_RECNUM_PERTIME, nRet);
	m_wBatTaskRecNumPerTime = (nRet>DEFAULT_BATTASK_RECNUM_PERTIME_MIN) ? (u16)nRet:DEFAULT_BATTASK_RECNUM_PERTIME_MIN;
	cXmlCfg.GetRegKeyInt(SECTION_CUI_LOCAL_INFO, KEY_BATTASK_INTERVAL_PERTIME, DEFAULT_BATTASK_INTERVAL_PERTIME, nRet);
	m_wBatTaskIntervalMsPerTime = (nRet>DEFAULT_BATTASK_INTERVAL_PERTIME_MIN) ? (u16)nRet:DEFAULT_BATTASK_INTERVAL_PERTIME_MIN;

	nRet = 0;
	cXmlCfg.GetRegKeyInt(SECTION_CUI_LOCAL_INFO, KEY_CHECK_CU_VERSION, 0, nRet);
	m_bCheckCuVersion = (0 != nRet) ? true:false;

	cXmlCfg.GetRegKeyInt(SECTION_CUI_LOCAL_INFO, KEY_MAX_CU_NTF_SIZE, DEFAULT_MAX_CU_NTF_SIZE, nRet);
	this->SetMaxCuNtfSize((s32)nRet);

	//读取端口映射表
	vector<CCommonNetNatInfo> vctMappingTableOld = GetCommonConfig().GetNetNatInfo();
	for (vector<CCommonNetNatInfo>::iterator it=vctMappingTableOld.begin(); it!=vctMappingTableOld.end(); it++)
	{
		//读取CUI码流转发端口映射
		if (it->platformPort==m_wNatPort && it->protocol==NAT_PROTOCOL_UDP)
		{
			TMappingItem tItem;
			tItem.SetLocalIP(it->platformIP);
			tItem.SetLocalPort(it->platformPort);
			tItem.SetMappingIP(it->platformNatIP);
			tItem.SetMappingPort(it->platformNatPort);
			tItem.SetProtocol(it->protocol);
			std::stringstream ssKey;
			ssKey << it->protocol << "_" << it->platformIP << "_" << it->platformPort;
			g_tMappingTable[ssKey.str()] = tItem;
		}
	}
	vector<CCommonNetNatInfo> vctMappingTable = GetCommonOtherNetNatInfo();
	for (vector<CCommonNetNatInfo>::iterator it=vctMappingTable.begin(); it!=vctMappingTable.end(); it++)
	{
		//读取CUI码流转发端口映射
		if (it->platformPort==m_wNatPort && it->protocol==NAT_PROTOCOL_UDP)
		{
			TMappingItem tItem;
			tItem.SetLocalIP(it->platformIP);
			tItem.SetLocalPort(it->platformPort);
			tItem.SetMappingIP(it->platformNatIP);
			tItem.SetMappingPort(it->platformNatPort);
			tItem.SetProtocol(it->protocol);
			std::stringstream ssKey;
			ssKey << it->protocol << "_" << it->platformIP << "_" << it->platformPort;
			g_tMappingTable[ssKey.str()] = tItem;
		}
	}

	/*cXmlCfg.GetRegKeyString(SECTION_CUI_LOCAL_INFO, KEY_MAPED_NAT_IP, string(), m_tMapedNatIP);

	cXmlCfg.GetRegKeyInt(SECTION_CUI_LOCAL_INFO, KEY_MAPED_NAT_PORT, DEFAULT_PORT, nRet);
	m_wMapedNatPort = (u16)nRet;

	// 读取网段信息 [1/23/2015 pangubing]
	u16 wSegmentNum;
	cXmlCfg.GetRegKeyInt(SECTION_CUI_SEGMENT, KEY_SEGMENT_NUM, 0, nRet);
	wSegmentNum = (u16)nRet;
	s8 szSegmentKey[20];
	string strSegmentVal;
	int nSegmentNum;
	s8 szIP[64],  szMask[64];
	memset(szIP, 0, sizeof(szIP));
	memset(szMask,0, sizeof(szMask));


	TNetSegment tNetSegmentInfo;
	m_vecSegmentInfo.clear();
	bool bRet;
	for (s32 i = 0; i < wSegmentNum; ++i)
	{
		sprintf(szSegmentKey, "item%d", i);
		bRet &= cXmlCfg.GetRegKeyString(SECTION_CUI_SEGMENT, szSegmentKey, "", strSegmentVal);
		if (!bRet)
		{
			break;
		}

		if (3 != sscanf(strSegmentVal.c_str(), "%d---%[^-]---%s",&nSegmentNum, szIP,szMask))
		{
			bRet = false;
			break;
		}
		tNetSegmentInfo.SetSegmentNum(nSegmentNum);
		tNetSegmentInfo.SetSegmentIp(szIP);
		tNetSegmentInfo.SetSegmentMask(szMask);

		m_vecSegmentInfo.push_back(tNetSegmentInfo);
	}
	*/
    return true;
}

TSipURI & CConfig::GetCmuURI()
{
    return m_tCmuURI;
}

TSipURI & CConfig::GetSecureModuleURI()
{
	return m_tSecureModuleURI;
}

TSipURI & CConfig::GetLocalURI()
{
    return m_tLocalURI;
}

// string & CConfig::GetLocalIP()
// {
//     return m_tLocalIP;
// }

u16 CConfig::GetLocalPort()
{
    return m_wLocalPort;
}

u16 CConfig::GetTcpListenPort()
{
    return m_wListenPort;
}

u16 CConfig::GetCuMaxNum()
{
    return m_wCuMaxNum;
}

u32 CConfig::GetSipWaitRspTimeout()
{
    return m_dwSipWaitRspTimeout;
}

u32 CConfig::GetDlgWaitRspTimeout()
{
    return m_dwDlgWaitRspTimeout;
}

u32 CConfig::GetCuHbTimeout()
{
    return m_dwCuHbTimeout;
}

u32 CConfig::GetAppQueueSize()
{
    return m_dwGetAppQueueSize;
}

s32 CConfig::GetRouteMode()
{
    return m_nRouteMode;
}

u32 CConfig::GetMaxTrans()
{
    return m_nMaxTrans;
}

u32 CConfig::GetMaxRegDlg()
{
    return m_nMaxRegDlg;
}

u32 CConfig::GetMaxInviteDlg()
{
    return m_nMaxInviteDlg;
}

u32 CConfig::GetMaxSubscribeDlg()
{
    return m_nMaxSubscribeDlg;
}

u32 CConfig::GetMaxNtfPerSub()
{
	return m_nMaxNtfPerSub;
}

BOOL32 CConfig::GetDisableCheckSum()
{
    return m_bDisableCheckSum;
}

void CConfig::SetSipKeyCfg(TKdsipKeyCfg& tKdsipKeyCfg)
{
    tKdsipKeyCfg.nMaxTrans = m_nMaxTrans;
    tKdsipKeyCfg.nMaxRegDlg = m_nMaxRegDlg;
    tKdsipKeyCfg.nMaxInviteDlg = m_nMaxInviteDlg;
    tKdsipKeyCfg.nMaxSubscribeDlg = m_nMaxSubscribeDlg;
    tKdsipKeyCfg.bDisableCheckSum = m_bDisableCheckSum;
    tKdsipKeyCfg.nSipRouteMode = m_nRouteMode;
	tKdsipKeyCfg.nNumOfNotifysPerSub = m_nMaxNtfPerSub;
}

void CConfig::PrintData( u32 dwOffSet /*= 0*/ )
{
    string strOffSet;
    GetOffSetString(dwOffSet, strOffSet);

    OspPrintf(TRUE, FALSE, "%sCUI本机IP: %s\n", 
         strOffSet.c_str(), GetLocalIPAddress().c_str());
    OspPrintf(TRUE, FALSE, "%sCUI本机SIP端口: %hu\n", 
        strOffSet.c_str(), GetLocalPort());
    OspPrintf(TRUE, FALSE, "%sCUI的TCP监听端口: %hu\n", 
        strOffSet.c_str(), GetTcpListenPort());
    OspPrintf(TRUE, FALSE, "%sCUI本机SIP URI: %s\n", 
        strOffSet.c_str(), GetLocalURI().GetURIString().c_str());
    OspPrintf(TRUE, FALSE, "%sCMU的SIP URI: %s\n", 
        strOffSet.c_str(), GetCmuURI().GetURIString().c_str());
    OspPrintf(TRUE, FALSE, "%s等待SIP应答超时时间: %u\n", 
        strOffSet.c_str(), GetSipWaitRspTimeout());
    OspPrintf(TRUE, FALSE, "%s等待CU心跳超时时间: %u\n", 
		strOffSet.c_str(), GetCuHbTimeout());
	OspPrintf(TRUE, FALSE, "\t每个CU通知消息队列最大限制: %d\n", 
		GetMaxCuNtfSize());
    OspPrintf(TRUE, FALSE, "\t允许最大CU接入数为%hu\n",
        g_cCuiConfig.GetCuMaxNum());
    OspPrintf(TRUE, FALSE, "\tSubscribe消息的流量限制值为%hu\n",
        g_cCuiConfig.GetSubscribeFlowLimit());
    OspPrintf(TRUE, FALSE, "\tUnsubscribe消息的流量限制值为%hu\n",
        g_cCuiConfig.GetUnsubscribeFlowLimit());
    OspPrintf(TRUE, FALSE, "\tSIP消息路由模式%d\n",
        g_cCuiConfig.GetRouteMode());
    OspPrintf(TRUE, FALSE, "\t移动客户端登录限制:%d\n", m_nMobileClientLimit);
	//OspPrintf(TRUE, FALSE, "\tNAT端口:%d\n", m_wNatPort);
	OspPrintf(TRUE, FALSE, "\tNAT外网映射IP:%s\n", m_tMapedNatIP.c_str());
	OspPrintf(TRUE, FALSE, "\tNAT外网映射端口:%d\n", m_wMapedNatPort);
    OspPrintf(TRUE, FALSE, "\tCU用户操作允许记录的操作类型列表：");
    for (set<s32>::const_iterator it = m_setCuOperateLog.begin(); it != m_setCuOperateLog.end(); ++it)
    {
        OspPrintf(TRUE, FALSE, "%d ", *it);
    }
	OspPrintf(TRUE, FALSE, "\n");

	OspPrintf(TRUE, FALSE, "\tUDP端口映射配置数：%d\n", g_tMappingTable.size());
	int nCount = 0;
	for (TMappingTable::const_iterator it=g_tMappingTable.begin(); it!=g_tMappingTable.end();it++)
	{
		nCount++;
		OspPrintf(TRUE, FALSE, "   \t[%d]：[%s][Local-%s:%d -> Mapping-%s:%d]\n", 
			nCount, it->second.GetProtocol().c_str(),
			it->second.GetLocalIP().c_str(), it->second.GetLocalPort(),
			it->second.GetMappingIP().c_str(), it->second.GetMappingPort());
	}

	OspPrintf(TRUE, FALSE, "\tNAT探测包模式下，是否由CUI转码:%s\n", this->IsNeedCUISwitch()?"true":"false");

	OspPrintf(TRUE, FALSE, "\t批量任务之单次启停录像数量:%d\n", this->GetBatTaskRecNumPerTime());
	OspPrintf(TRUE, FALSE, "\t批量任务之间隔（毫秒）:%d\n", this->GetBatTaskIntervalPerTime());

    OspPrintf(TRUE, FALSE, "\n");
}

u16 CConfig::GetSubscribeFlowLimit()
{
    return m_wSubscribeFlowLimit;
}

u16 CConfig::GetUnsubscribeFlowLimit()
{
    return m_wUnsubscribeFlowLimit;
}

bool CConfig::IsCuOperateLogEnable( s32 nOperateType )
{
    return m_setCuOperateLog.find(nOperateType) != m_setCuOperateLog.end();
}

s32 CConfig::GetMobileClientLimit() const
{
    return m_nMobileClientLimit;
}

/*int CConfig::GetNetSegmentByIp(const string& strIp )
{
	int nSegment=-1;
	for (vector<TNetSegment>::const_iterator it = m_vecSegmentInfo.begin(); it != m_vecSegmentInfo.end(); ++it)
	{
		if (it->GetSegmentIp()==strIp)
		{
			nSegment = it->GetSegmentNum();
		}
	}

	return nSegment;
}*/

bool CConfig::GetNatMapAddr(const string& strSrcIp,const int& nSrcPort,string& strMapIp,int& nMapPort)
{
	std::stringstream ssKey;
	ssKey << "UDP_" << strSrcIp << "_" << nSrcPort;
	
	TMappingTable::const_iterator it = g_tMappingTable.find(ssKey.str());
	if (g_tMappingTable.end() != it)
	{
		strMapIp = it->second.GetMappingIP();
		nMapPort = it->second.GetMappingPort();

		return true;
	}

	return false;
}

bool CConfig::IsNatRevPortMaped()
{
	bool bFindDefaultIp = false;
	string strDefaultIp;

	string strDefaultNic;
	GetDefaultRouteIface(strDefaultNic);

	vector<TNicInfo> vctNic;
	if (GetOnlineNicList(vctNic))
	{
		for (vector<TNicInfo>::const_iterator it = vctNic.begin(); it!=vctNic.end(); ++it)
		{
			if (strDefaultNic == it->strNicName)
			{
				strDefaultIp = it->strNicIp;
				bFindDefaultIp = true;
				break;
			}
		}
	}

	if (bFindDefaultIp)
	{
		std::stringstream ssKey;
		ssKey << "UDP_" << strDefaultIp << "_" << m_wNatPort;

		TMappingTable::const_iterator it = g_tMappingTable.find(ssKey.str());
		if (g_tMappingTable.end() != it)
		{
			return true;
		}

		return false;
	}

	list<string> tIpList;
	if (GetOnlineIpList(tIpList))
	{
		for (list<string>::const_iterator itIp=tIpList.begin(); itIp!=tIpList.end(); itIp++)
		{
			std::stringstream ssKey;
			ssKey << "UDP_" << *itIp << "_" << m_wNatPort;

			TMappingTable::const_iterator it = g_tMappingTable.find(ssKey.str());
			if (g_tMappingTable.end() != it)
			{
				return true;
			}
		}
	}			
	return false;
}

void CConfig::UpdateMapedNatAddr()
{
	bool bFindDefaultIp = false;
	string strDefaultIp;

	string strDefaultNic;
	GetDefaultRouteIface(strDefaultNic);

	vector<TNicInfo> vctNic;
	if (GetOnlineNicList(vctNic))
	{
		for (vector<TNicInfo>::const_iterator it = vctNic.begin(); it!=vctNic.end(); ++it)
		{
			if (strDefaultNic == it->strNicName)
			{
				strDefaultIp = it->strNicIp;
				bFindDefaultIp = true;
				break;
			}
		}
	}

	if (bFindDefaultIp)
	{
		std::stringstream ssKey;
		ssKey << "UDP_" << strDefaultIp << "_" << m_wNatPort;

		TMappingTable::const_iterator it = g_tMappingTable.find(ssKey.str());
		if (g_tMappingTable.end() != it)
		{
			m_tMapedNatIP = it->second.GetMappingIP();
			m_wMapedNatPort = it->second.GetMappingPort();
			return;
		}
	}

	list<string> tIpList;
	if (GetOnlineIpList(tIpList))
	{
		for (list<string>::const_iterator itIp=tIpList.begin(); itIp!=tIpList.end(); itIp++)
		{
			std::stringstream ssKey;
			ssKey << "UDP_" << *itIp << "_" << m_wNatPort;

			TMappingTable::const_iterator it = g_tMappingTable.find(ssKey.str());
			if (g_tMappingTable.end() != it)
			{
				m_tMapedNatIP = it->second.GetMappingIP();
				m_wMapedNatPort = it->second.GetMappingPort();
				return;
			}
		}
	}			
}

bool CConfig::IsNeedCUISwitch() const
{
	return (m_bNeedCUISwitch == TRUE ? true : false);
}

u16 CConfig::GetBatTaskRecNumPerTime() const
{
	return m_wBatTaskRecNumPerTime;
}
u16 CConfig::GetBatTaskIntervalPerTime() const
{
	return m_wBatTaskIntervalMsPerTime;
}

s32 CConfig::GetMaxCuNtfSize() const
{
	return m_nMaxCuNtfSize;
}

void CConfig::SetMaxCuNtfSize(s32 cuNtfSize)
{
	if ( cuNtfSize == 0 )
	{
		cuNtfSize = DEFAULT_MAX_CU_NTF_SIZE;
	}
	else if ( cuNtfSize < DEFAULT_MIN_CU_NTF_SIZE )
	{
		return;
	}

	m_nMaxCuNtfSize = cuNtfSize;
}

void CConfig::UpdateNatMap(u16 wCuiNatPort)
{
	//读取cui NAT信息
	for (TMappingTable::const_iterator it=g_tMappingTable.begin(); it!=g_tMappingTable.end(); it++)
	{
		if (wCuiNatPort != it->second.GetLocalPort())
		{
			continue;
		}

		list<string> tIpList;
		if (GetOnlineIpList(tIpList))
		{
			for (list<string>::const_iterator itIp=tIpList.begin(); itIp!=tIpList.end(); itIp++)
			{
				if (it->second.GetLocalIP() == *itIp)
				{
					m_tMapedNatIP = it->second.GetMappingIP();
					m_wMapedNatPort = it->second.GetMappingPort();
					break;
				}
			}
		}
	}
}

CVideoSrcStatList::CVideoSrcStatList()
{
	Clear();
}

bool CVideoSrcStatList::IsFinished() const
{
	if ( m_finishTime > 0 )
	{
		return true;
	}
	return false;
}

void CVideoSrcStatList::SetFinished()
{
	m_finishTime = this->GetCurTime();
}


void CVideoSrcStatList::SetCreating()
{
	m_createTime = this->GetCurTime();
}

int CVideoSrcStatList::GetOnlineDeviceNum() const
{
	return m_onlineDeviceNum;
}

void CVideoSrcStatList::SetOnlineDeviceNum(int onlineDeviceNum)
{
	m_onlineDeviceNum = onlineDeviceNum;
}

int CVideoSrcStatList::GetIdleDeviceNum() const
{
	return m_idleDeviceNum;
}

void CVideoSrcStatList::SetIdleDeviceNum(int idleDeviceNum)
{
	m_idleDeviceNum = idleDeviceNum;
}

int CVideoSrcStatList::GetOfflineDeviceNum() const
{
	return m_offlineDeviceNum;
}

void CVideoSrcStatList::SetOfflineDeviceNum(int offlineDeviceNum)
{
	m_offlineDeviceNum = offlineDeviceNum;
}

time_t CVideoSrcStatList::GetUsingLastTime() const
{
	return m_cuUsingLastTime;
}

void CVideoSrcStatList::SetUsingLastTime()
{
	m_cuUsingLastTime = this->GetCurTime();
}

string CVideoSrcStatList::GetCuQueryTag() const
{
	return m_cuQueryTag;
}

void CVideoSrcStatList::SetCuQueryTag(string cuQueryTag)
{
	m_cuQueryTag = cuQueryTag;
}

string CVideoSrcStatList::GetTaskQueryTag() const
{
	return m_taskQueryTag;
}

void CVideoSrcStatList::SetTaskQueryTag(string taskQueryTag)
{
	m_taskQueryTag = taskQueryTag;
}

string CVideoSrcStatList::GetTaskNo() const
{
	return m_taskNo;
}

void CVideoSrcStatList::SetTaskNo(string taskNo)
{
	m_taskNo = taskNo;
}

void CVideoSrcStatList::AddVideoSrcStatList(const TVideoSrcStatList& devVideoIdList, bool isFinised /*= false*/)
{
	if (m_videoSrcStatList.size() > 0)
	{
		for ( s32 i=0; i < devVideoIdList.size(); i++)
		{
			m_videoSrcStatList.push_back(devVideoIdList[i]);
		}
	}
	else
	{
		m_videoSrcStatList = devVideoIdList;
	}

	if ( isFinised )
	{
		m_finishTime = this->GetCurTime();
	}
	else if ( m_createTime == 0 )
	{
		m_createTime = this->GetCurTime();
	}

	return ;
}

void CVideoSrcStatList::PrintData()
{
	OspPrintf(TRUE, FALSE, "VideoSrcStatus List size=%d\n", m_videoSrcStatList.size());

	for (size_t nFori = 0; nFori < m_videoSrcStatList.size(); nFori++)
	{
		string vsInfo = m_videoSrcStatList[nFori].ToXml();
		if (vsInfo.empty())
		{
			vsInfo = "empty";
		}

		//OspPrintf(TRUE, FALSE, "VSStatList[%d]=%s\n", nFori, vsInfo.c_str());
	}
	
	OspPrintf(TRUE, FALSE, "在线视频源数=%d\n", m_onlineDeviceNum);
	OspPrintf(TRUE, FALSE, "未启用视频源数=%d\n", m_idleDeviceNum);
	OspPrintf(TRUE, FALSE, "离线视频源数=%d\n", m_offlineDeviceNum);
	OspPrintf(TRUE, FALSE, "获取列表的错误码=%d\n", m_errorCode);

	string sTime = "0";
	CCMSTime csTime;
	if ( m_createTime > 0 )
	{
		csTime.SetTime(m_createTime);
		sTime = csTime.ToString();
	}
	OspPrintf(TRUE, FALSE, "列表创建时间=%s\n", sTime.c_str());

	sTime = "0";
	if ( m_finishTime > 0 )
	{
		csTime.SetTime(m_finishTime);
		sTime = csTime.ToString();
	}
	OspPrintf(TRUE, FALSE, "列表完成时间=%s\n", sTime.c_str());

	OspPrintf(TRUE, FALSE, "更新列表耗时=%d(s)\n", m_finishTime-m_createTime);
	
	time_t curTime = this->GetCurTime();
	sTime = "0";
	if ( curTime > 0 )
	{
		csTime.SetTime(curTime);
		sTime = csTime.ToString();
	}
	OspPrintf(TRUE, FALSE, "当前时间=%s\n", sTime.c_str());

	time_t usingLastTime = this->GetUsingLastTime();
	sTime = "0";
	if ( usingLastTime > 0 )
	{
		csTime.SetTime(usingLastTime);
		sTime = csTime.ToString();
	}
	OspPrintf(TRUE, FALSE, "最后查询列表的时间=%s\n", sTime.c_str());
}

int CVideoSrcStatList::GetErrorCode() const
{
	return m_errorCode;
}

void CVideoSrcStatList::SetErrorCode(int errorCode)
{
	m_errorCode = errorCode;
}

u32 CVideoSrcStatList::GetVideoSrcStatListSize()
{
	return (u32)m_videoSrcStatList.size();
}

bool CVideoSrcStatList::GetVideoSrcStatList(int beginIndex, int entryNum, TDeviceVideoSrcIdList &devVideoSrcIdList)
{
	if ( beginIndex < 0 )
	{
		beginIndex = 0;
	}

	m_cuUsingLastTime = this->GetCurTime();

	for ( size_t i = beginIndex; i < m_videoSrcStatList.size(); i++)
	{
		if ( i >= beginIndex+entryNum )
		{
			return true;
		}

		devVideoSrcIdList.push_back(m_videoSrcStatList[i]);
	}

	return true;
}

time_t CVideoSrcStatList::GetCurTime() const
{
	time_t curTime;
	time(&curTime);
	return curTime;
}

void CVideoSrcStatList::Clear()
{
	m_videoSrcStatList.clear();

	m_onlineDeviceNum = 0;
	m_idleDeviceNum = 0;
	m_offlineDeviceNum = 0;

	m_errorCode = 0;

	m_cuQueryTag.clear();
	m_taskQueryTag.clear();
	m_taskNo.clear();

	m_createTime = 0;
	m_finishTime = 0;
	m_cuUsingLastTime = 0;
}

CCuPtzLockInfo::CCuPtzLockInfo()
{

}

bool CCuPtzLockInfo::IsValid() const
{
	if (m_strDeviceId.empty()
		|| m_strUserId.empty()
		|| m_strEndTime.empty())
	{
		return false;
	}

	if ( IsExpired() )
	{
		return false;
	}

	return true;
}

bool CCuPtzLockInfo::IsExpired() const
{
	CCMSTime cmsEndTime(m_strEndTime);

	CCMSTime cmsCurtime;

	if (cmsEndTime < cmsCurtime
		|| cmsEndTime == cmsCurtime)
	{
		return true;
	}

	return false;
}

void CCuPtzLockInfo::Clear()
{
	m_strDeviceId.clear();
	m_nVideoSrcId = 0;
	m_strBeginTime.clear();
	m_strEndTime.clear();
	m_strUserId.clear();
	m_strUserName.clear();
	m_strPassWord.clear();
	m_nUserPtzLevel = 0;
}

void CCuPtzLockInfo::PrintData() const
{
	OspPrintf(TRUE, FALSE, "视频源PTZ锁定信息\n");

	OspPrintf(TRUE, FALSE, "设备id[%s], 视频源[%d]\n", m_strDeviceId.c_str(), m_nVideoSrcId);
	OspPrintf(TRUE, FALSE, "加锁用户ID[%s], 用户名[%s],PTZ权限等级[%d], 密码[%s]\n", m_strUserId.c_str(), m_strUserName.c_str(), m_nUserPtzLevel, m_strPassWord.c_str());
	OspPrintf(TRUE, FALSE, "加锁时间范围[%s-%s]\n", m_strBeginTime.c_str(), m_strEndTime.c_str());
}

void CCuPtzLockInfo::SetDeviceId(const string& strDeviceId)
{
	m_strDeviceId = strDeviceId;
}

string& CCuPtzLockInfo::GetDeviceId()
{
	return m_strDeviceId;
}

const string& CCuPtzLockInfo::GetDeviceId() const
{
	return m_strDeviceId;
}

void CCuPtzLockInfo::SetVideoSrcId(int nVideoSrcId)
{
	m_nVideoSrcId = nVideoSrcId;
}

int CCuPtzLockInfo::GetVideoSrcId() const
{
	return m_nVideoSrcId;
}

void CCuPtzLockInfo::SetBeginTime(const string& strBeginTime)
{
	m_strBeginTime = strBeginTime;
}

string& CCuPtzLockInfo::GetBeginTime()
{
	return m_strBeginTime;
}

const string& CCuPtzLockInfo::GetBeginTime() const
{
	return m_strBeginTime;
}

void CCuPtzLockInfo::SetEndTime(const string& strEndTime)
{
	m_strEndTime = strEndTime;
}

string& CCuPtzLockInfo::GetEndTime()
{
	return m_strEndTime;
}

const string& CCuPtzLockInfo::GetEndTime() const
{
	return m_strEndTime;
}

void CCuPtzLockInfo::SetUserId(const string& strUserId)
{
	m_strUserId = strUserId;
}

string& CCuPtzLockInfo::GetUserId()
{
	return m_strUserId;
}

const string& CCuPtzLockInfo::GetUserId() const
{
	return m_strUserId;
}

void CCuPtzLockInfo::SetUserName(const string& strUserName)
{
	m_strUserName = strUserName;
}

string& CCuPtzLockInfo::GetUserName()
{
	return m_strUserName;
}

const string& CCuPtzLockInfo::GetUserName() const
{
	return m_strUserName;
}

void CCuPtzLockInfo::SetPassword(const string& strPassword)
{
	m_strPassWord = strPassword;
}

string& CCuPtzLockInfo::GetPassword()
{
	return m_strPassWord;
}

const string& CCuPtzLockInfo::GetPassword() const
{
	return m_strPassWord;
}

void CCuPtzLockInfo::SetUserPtzLevel(int nUserPtzLevel)
{
	m_nUserPtzLevel = nUserPtzLevel;
}

int CCuPtzLockInfo::GetUserPtzLevel() const
{
	return m_nUserPtzLevel;
}

CCuPtzLockInfoList::CCuPtzLockInfoList()
{

}

CCuPtzLockInfoList::~CCuPtzLockInfoList()
{

}

void CCuPtzLockInfoList::Clear()
{
	m_mapUserIdToVsChnList.clear();
	m_mapPtzLockInfo.clear();
	m_mapDevToChnList.clear();
}

bool CCuPtzLockInfoList::IsCuPtzLockedByUser(const TChannel &devVsChn, const string &userId)
{
	CCuPtzLockInfo ptzLockInfo;
	if (!this->GetCuPtzLockInfoByVs(devVsChn, ptzLockInfo))
	{
		return false;
	}

	if (ptzLockInfo.GetUserId() != userId)
	{
		return false;
	}

	return true;
}

bool CCuPtzLockInfoList::GetCuPtzLockInfoByVs(const TChannel &devVsChn, CCuPtzLockInfo &ptzLockInfo)
{
	ptzLockInfo.Clear();
	if (devVsChn.GetDevUri().empty())
	{
		return false;
	}

	TChannel vsChn;
	vsChn.SetDevUri(GetNameWithoutDomainFromURI(devVsChn.GetDevUri()));
	vsChn.SetChnNO(devVsChn.GetChnNO());

	std::map <TChannel, CCuPtzLockInfo>::iterator iter = m_mapPtzLockInfo.find(vsChn);
	if (iter != m_mapPtzLockInfo.end())
	{
		ptzLockInfo = iter->second;
		return true;
	}
	else
	{
		return false;
	}
}

bool CCuPtzLockInfoList::AddCuPtzLockInfo(const CCuPtzLockInfo &ptzLockInfo)
{
	if ( ptzLockInfo.GetDeviceId().empty() )
	{
		OspPrintf(TRUE, FALSE, "添加PTZ锁信息失败,设备ID为空？\n");
		return false;
	}
	// 锁是3A添加的，说明数据库中有该数据，只有添加到内存中，检测过期了才会删除，3A不会主动删除过期的PTZ锁
	ptzLockInfo.IsValid();

	CCuPtzLockInfo lockInfo = ptzLockInfo;
	lockInfo.SetDeviceId(GetNameWithoutDomainFromURI(ptzLockInfo.GetDeviceId()));
	lockInfo.SetUserId(GetNameWithoutDomainFromURI(ptzLockInfo.GetUserId()));

	TChannel tVsChn;
	tVsChn.SetDevUri(lockInfo.GetDeviceId());
	tVsChn.SetChnNO(lockInfo.GetVideoSrcId());

	m_mapPtzLockInfo[tVsChn] = lockInfo;

	TChannelList userVsChnList = m_mapUserIdToVsChnList[lockInfo.GetUserId()];
	
	bool bSameVsChn = false;
	for (size_t i = 0; i < userVsChnList.size();i++)
	{
		// 看下是否有重复的数据，有重复就直接覆盖
		if ( userVsChnList[i] == tVsChn )
		{
			userVsChnList[i] = tVsChn;
			m_mapUserIdToVsChnList[lockInfo.GetUserId()] = userVsChnList;
			bSameVsChn = true;
			break;
		}
	}
	
	if ( !bSameVsChn )
	{
		userVsChnList.push_back(tVsChn);
		m_mapUserIdToVsChnList[lockInfo.GetUserId()] = userVsChnList;
	}
	
	bSameVsChn = false;
	TChannelList devVsChnList = m_mapDevToChnList[lockInfo.GetDeviceId()];

	for (size_t i = 0; i < devVsChnList.size(); i++)
	{
		// 看下是否有重复的数据，有重复就直接覆盖
		if (devVsChnList[i] == tVsChn)
		{
			devVsChnList[i] = tVsChn;
			m_mapDevToChnList[lockInfo.GetDeviceId()] = devVsChnList;
			bSameVsChn = true;
			break;
		}
	}

	if (!bSameVsChn)
	{
		devVsChnList.push_back(tVsChn);
		m_mapDevToChnList[lockInfo.GetDeviceId()] = devVsChnList;
	}


	return true;
}

bool CCuPtzLockInfoList::DelCuPtzLockInfoByUserWithVsChn(const string &strUserId, const TChannel &devVsChn)
{
	TChannel tVsChn;
	tVsChn.SetDevUri(GetNameWithoutDomainFromURI(devVsChn.GetDevUri()));
	tVsChn.SetChnNO(devVsChn.GetChnNO());

	CCuPtzLockInfo ptzLockInfo;
	if (!this->GetCuPtzLockInfoByVs(tVsChn, ptzLockInfo))
	{
		return false;
	}

	string userId = GetNameWithoutDomainFromURI(strUserId);
	if (ptzLockInfo.GetUserId() != userId)
	{
		return false;
	}

	this->DelCuPtzLockInfoByVs(tVsChn);

	return true;
}

void CCuPtzLockInfoList::DelCuPtzLockInfoByVs(const TChannel &devVsChn)
{
	if (devVsChn.GetDevUri().empty())
	{
		return;
	}

	TChannel tVsChn;
	tVsChn.SetDevUri(GetNameWithoutDomainFromURI(devVsChn.GetDevUri()));
	tVsChn.SetChnNO(devVsChn.GetChnNO());

	string strUserId;

	std::map <TChannel, CCuPtzLockInfo>::iterator iter = m_mapPtzLockInfo.find(tVsChn);
	if (iter != m_mapPtzLockInfo.end())
	{
		strUserId = iter->second.GetUserId();
		m_mapPtzLockInfo.erase(iter);
	}

	if ( !strUserId.empty() )
	{
		std::map <string, TChannelList>::iterator userIter = m_mapUserIdToVsChnList.find(strUserId);
		if ( userIter != m_mapUserIdToVsChnList.end())
		{
			TChannelList &tUserVsChnList = userIter->second;
			for (TChannelList::iterator iter = tUserVsChnList.begin();
					iter != tUserVsChnList.end(); iter++)
			{
				if (tVsChn == *iter)
				{
					tUserVsChnList.erase(iter);
					break;
				}
			}

			if (tUserVsChnList.empty())
			{
				m_mapUserIdToVsChnList.erase(userIter);
			}
		}
	}

	std::map <string, TChannelList>::iterator devIter = m_mapDevToChnList.find(tVsChn.GetDevUri());
	if ( devIter != m_mapDevToChnList.end() )
	{
		TChannelList &tDevVsChnList = devIter->second;
		for (TChannelList::iterator vsIter = tDevVsChnList.begin();
				vsIter != tDevVsChnList.end(); vsIter++)
		{
			if ( vsIter->GetChnNO() == tVsChn.GetChnNO() )
			{
				tDevVsChnList.erase(vsIter);
				break;
			}
		}

		if ( tDevVsChnList.empty() )
		{
			m_mapDevToChnList.erase(devIter);
		}
	}
}

void CCuPtzLockInfoList::DelCuPtzLockInfoByUserId(const string &strUserId)
{
	if (strUserId.empty())
	{
		return;
	}

	string userId = GetNameWithoutDomainFromURI(strUserId);

	std::map <string, TChannelList>::iterator iter = m_mapUserIdToVsChnList.find(userId);
	if (iter != m_mapUserIdToVsChnList.end())
	{
		TChannelList &vsChnList = iter->second;
		for (int i = 0; i < vsChnList.size(); i++)
		{
			// 删除通道对应的锁信息
			std::map <TChannel, CCuPtzLockInfo>::iterator lockIter = m_mapPtzLockInfo.find(vsChnList[i]);
			if (lockIter != m_mapPtzLockInfo.end())
			{
				m_mapPtzLockInfo.erase(lockIter);
			}

			// 删除设备下某个视频源
			std::map <string, TChannelList>::iterator devIter = m_mapDevToChnList.find(vsChnList[i].GetDevUri());
			if (devIter != m_mapDevToChnList.end())
			{
				TChannelList &tChnList = devIter->second;
				for (TChannelList::iterator vsIter = tChnList.begin();
				vsIter != tChnList.end(); vsIter++)
				{
					if (vsIter->GetChnNO() == vsChnList[i].GetChnNO())
					{
						tChnList.erase(vsIter);
						break;
					}
				}
			}
		}

		m_mapUserIdToVsChnList.erase(iter);
	}

}
void CCuPtzLockInfoList::DelCuPtzLockInfoByDevId(const string &strDevId)
{
	string devId = GetNameWithoutDomainFromURI(strDevId);
	if ( devId.empty() )
	{
		return;
	}

	std::map <string, TChannelList>::iterator devIter = m_mapDevToChnList.find(devId);
	for (TChannelList::iterator chnIter = devIter->second.begin();
			chnIter != devIter->second.end(); chnIter++)
	{
		this->DelCuPtzLockInfoByVs(*chnIter);
	}
	
}

void CCuPtzLockInfoList::PrintData()
{
	if ( m_mapPtzLockInfo.empty() )
	{
		OspPrintf(TRUE, FALSE, "没有视频源锁定PTZ控制\n");
	}
	else
	{
		OspPrintf(TRUE, FALSE, "有[%d]个视频源锁定了PTZ控制\n", m_mapPtzLockInfo.size());
	}

	OspPrintf(TRUE, FALSE, "设备--视频源锁定信息\n");
	for (std::map <TChannel, CCuPtzLockInfo>::const_iterator lockIter = m_mapPtzLockInfo.begin();
	lockIter != m_mapPtzLockInfo.end(); lockIter++)
	{
		lockIter->second.PrintData();
	}

	OspPrintf(TRUE, FALSE, "用户--视频源锁定信息\n");
	for (std::map <string, TChannelList>::const_iterator userIter = m_mapUserIdToVsChnList.begin();
	userIter != m_mapUserIdToVsChnList.end(); userIter++)
	{
		OspPrintf(TRUE, FALSE, "用户锁定的视频源 用户ID[%s]\n", userIter->first.c_str());
		for (TChannelList::const_iterator vsChnIter = userIter->second.begin();
		vsChnIter != userIter->second.end(); vsChnIter++)
		{
			OspPrintf(TRUE, FALSE, "视频源[%s]\n",vsChnIter->ToString().c_str());
		}
	}

	OspPrintf(TRUE, FALSE, "设备--视频源锁定信息\n");
	for (std::map <string, TChannelList>::const_iterator devIter = m_mapDevToChnList.begin();
			devIter != m_mapDevToChnList.end(); devIter++)
	{
		OspPrintf(TRUE, FALSE, "设备下锁定的视频源 设备ID[%s]\n", devIter->first.c_str());
		for (TChannelList::const_iterator vsChnIter = devIter->second.begin();
		vsChnIter != devIter->second.end(); vsChnIter++)
		{
			OspPrintf(TRUE, FALSE, "视频源[%s]\n",vsChnIter->ToString().c_str());
		}
	}
}

void CCuPtzLockInfoList::PrintDataByDevVsChn(const TChannel &tDevVsChn)
{
	if (tDevVsChn.GetDevUri().empty())
	{
		OspPrintf(TRUE, FALSE, "设备ID不能为空\n");
		return;
	}

	TChannel vsChn;
	vsChn.SetDevUri(GetNameWithoutDomainFromURI(tDevVsChn.GetDevUri()));
	vsChn.SetChnNO(tDevVsChn.GetChnNO());

	std::map <TChannel, CCuPtzLockInfo>::iterator iter = m_mapPtzLockInfo.find(vsChn);
	if (iter != m_mapPtzLockInfo.end())
	{
		iter->second.PrintData();
	}
}

void CCuPtzLockInfoList::PrintDataByUserId(const string &strUserId)
{
	if (strUserId.empty())
	{
		return;
	}

	std::map <string, TChannelList>::iterator iter = m_mapUserIdToVsChnList.find(strUserId);
	if (iter != m_mapUserIdToVsChnList.end())
	{
		TChannelList &chnList = iter->second;
		OspPrintf(TRUE, FALSE, "用户[%s]锁定了[%d]个视频源\n", strUserId.c_str(), chnList.size());
		for (int i = 0; i < chnList.size(); i++)
		{
			PrintDataByDevVsChn(chnList[i]);
		}
	}
	else
	{
		OspPrintf(TRUE, FALSE, "用户[%s]没有锁定视频源\n", strUserId.c_str());
	}
}

void CCuPtzLockInfoList::PrintDataByDevId(const string &strdevId)
{
	string devId = GetNameWithoutDomainFromURI(strdevId);
	if ( devId.empty())
	{
		OspPrintf(TRUE, FALSE, "没有正确的设备名称,无法查询\n");
	}

	std::map <string, TChannelList>::const_iterator devIter = m_mapDevToChnList.find(devId);
	if ( devIter == m_mapDevToChnList.end())
	{
		OspPrintf(TRUE, FALSE, "没有的设备对应的PTZ锁信息\n");
		return;
	}
	
	OspPrintf(TRUE, FALSE, "设备[%s]有效的PTZ锁信息如下", devIter->first.c_str());

	TChannelList chnList = devIter->second;
	for (size_t i = 0; i < chnList.size(); i++)
	{
		CCuPtzLockInfo ptzLockInfo;
		this->GetCuPtzLockInfoByVs(chnList[i], ptzLockInfo);
		if ( ptzLockInfo.IsValid() )
		{
			ptzLockInfo.PrintData();
		}
	}
}

bool CuBatOptList::GetFrontBatOptInfo(TBatOptInfo &frontBatOptInfo)
{
	while (!m_batOptReqInfolist.empty())
	{
		if (m_batOptReqInfolist.front().m_strBatOptReq.empty())
		{
			m_batOptReqInfolist.pop_front();
		}
		else
		{
			frontBatOptInfo = m_batOptReqInfolist.front();
			return true;
		}
	}

	return false;
}

void CuBatOptList::PopFront()
{
	if (!m_batOptReqInfolist.empty())
	{
		m_batOptReqInfolist.pop_front();
	}
}

bool CuBatOptList::PopFrontByBatOptInfo(TBatOptInfo &frontBatOptInfo)
{
	if (!m_batOptReqInfolist.empty())
	{
		if ( frontBatOptInfo.m_strBatOptReq == m_batOptReqInfolist.front().m_strBatOptReq
			&& frontBatOptInfo.m_nTaskNo == m_batOptReqInfolist.front().m_nTaskNo )
		{
			m_batOptReqInfolist.pop_front();
			return true;
		}
	}
	return false;
}

bool CuBatOptList::SetFrontBatOptTaskNo(const string strReq, const int &nTaskNo)
{
	if ( m_batOptReqInfolist.empty()
		|| strReq.empty()
		|| nTaskNo == INVALID_TASKNO)
	{
		return false;
	}

	if ( m_batOptReqInfolist.front().m_strBatOptReq == strReq )
	{
		m_batOptReqInfolist.front().m_nTaskNo = nTaskNo;
		return true;
	}

	return false;
}

void CuBatOptList::PushBackBatOptStrReq(const string &strReq)
{
	TBatOptInfo batOptInfo;
	batOptInfo.m_strBatOptReq = strReq;
	m_batOptReqInfolist.push_back(batOptInfo);
}

bool CuBatOptList::SetFrontBatOptPos(const TBatOptInfo &frontBatOptInfo)
{
	if ( frontBatOptInfo.m_strBatOptReq.empty()
		|| frontBatOptInfo.m_nTaskNo == INVALID_TASKNO
		|| m_batOptReqInfolist.empty()
		|| m_batOptReqInfolist.front().m_strBatOptReq != frontBatOptInfo.m_strBatOptReq
		|| m_batOptReqInfolist.front().m_nTaskNo != frontBatOptInfo.m_nTaskNo )
	{
		return false;
	}

	m_batOptReqInfolist.front().m_nPos = frontBatOptInfo.m_nPos;
	m_batOptReqInfolist.front().m_total = frontBatOptInfo.m_total;
	return true;
}

bool CuBatOptList::Clear()
{
	m_batOptReqInfolist.clear();
}

bool CuBatOptList::IsEmpty()
{
	return m_batOptReqInfolist.empty();
}

void CuBatOptList::PrintData()
{
	OspPrintf(TRUE, FALSE, "缓存的CU批量操作 数量[%d]\n", m_batOptReqInfolist.size());

	int nIndex = 0;
	for (list<TBatOptInfo>::const_iterator iter = m_batOptReqInfolist.begin();
	iter != m_batOptReqInfolist.end(); iter++)
	{
		if ( iter->m_total == 0 && iter->m_nPos == 0)
		{
			OspPrintf(TRUE, FALSE, "index[%d] Request[%s]\n", nIndex++, iter->m_strBatOptReq.c_str());
		}
		else
		{
			OspPrintf(TRUE, FALSE, "index[%d] Request[%s] curPos[%d] total[%d]\n", nIndex++, iter->m_strBatOptReq.c_str(), iter->m_nPos, iter->m_total);
		}
	}
}
