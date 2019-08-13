
#include "cms/ospext/osplog.h"
#include "cms/commonconfig/cmcfg.h"
#include "cms/utility/cmstime.h"
#include "cms/natagent/natagent.h"
#include "pusstasktest.h"
#include "cmustubtestdata.h"
#include "cmustubtestconst.h"
#include "cmustubtestconf.h"


CPuDataList g_cPuList;

CStrMap<CTask*>   g_cStubQryTaskList;

vector<string> g_pszTestPu;


string GetUriFromDevId(const string& strDevId)
{
    TSipURI tUri;
    tUri.SetUser(strDevId);
    tUri.SetDomain(g_cCmuConf.m_tProxyUri.GetDomain());
    return tUri.GetURIString();
}

bool IsLocalDomain(const string& strDevUri)
{
    TSipURI tUri;
    tUri.SetURIString(strDevUri);
    return IsLocalDomain(tUri);
}

bool IsLocalDomain(const TSipURI& tDevUri)
{
    return (g_cCmuConf.m_tLocalUri.GetDomain() == tDevUri.GetDomain());
}

const TSipURI& GetLocalUri()
{
    return g_cCmuConf.m_tLocalUri;
}

TSipURI GetCmuUriFromDevUri(const string& strDevUri)
{
    TSipURI tDevUri;
    tDevUri.SetURIString(strDevUri);
    return tDevUri.GetCmuURI();
}

TSipURI GetCmuUriFromDevUri(const TSipURI& tDevUri)
{
    return tDevUri.GetCmuURI();
}

TSipURI GetCmuUri()
{
    return g_cCmuConf.m_tCmuUri;
}

TSipURI GetAlsUri()
{
    return GetLocalUri().GetAlsURI();
}

TSipURI GetTvsUri()
{
    return GetLocalUri().GetTvsURI();
}

bool Kdm_IsValidIPStr(const char* lpszIp)
{
    if (NULL != lpszIp && 0 != lpszIp[0] && 0 != strcmp(lpszIp, "255.255.255.255"))
    {
        return true;
    }

    return false;
}

void Cms2Na_NetAddr(const TNetAddr& tNetAddr, TNatAddr& tNatAddr)
{
    strncpy(tNatAddr.m_achIP, tNetAddr.GetNetIp().c_str(), sizeof(tNatAddr.m_achIP));
    tNatAddr.m_wPort = tNetAddr.GetNetPort();
}

bool SamStartPunch(TSdp& tPeerSdp, const TNetAddr& tLocalAddr, void* pvContext)
{
    if (tPeerSdp.IsAllMediaChnKeyEmpty())
    {
        //punchKey为空，不需要punch
        return true;
    }

    GLOBELOG(CRITICAL_LEV, "LocalAddr[%s:%d->%d]开始punch!\n", 
        tLocalAddr.GetNetIp().c_str(), tLocalAddr.GetNetPort(), tLocalAddr.GetNetPort()+3);

    TMediaDescList& tDstMediaTransDescList = tPeerSdp.GetMediaDescList();

    int dwPunchNum = 0;
    TPunchInfo atPunchInfo[NATAGENT_MAX_PORT_NUM]={0};

    int nPuRecPortStart = tLocalAddr.GetNetPort();

    for(TMediaDescList::iterator itrMediaDesc = tDstMediaTransDescList.begin(); 
        itrMediaDesc!=tDstMediaTransDescList.end(); itrMediaDesc++)
    {
        ///////////////////////////////////////////////////////////////
        //遍历目的TMediaTransChannelList
        TMediaTransChannelList& tMediaTransChnList = itrMediaDesc->GetMediaTransChnList();

        for(TMediaTransChannelList::iterator itrTransChn = tMediaTransChnList.begin(); 
            itrTransChn!=tMediaTransChnList.end(); itrTransChn++)
        {
            const TMediaTransChannel& tMediaTransChn = *itrTransChn;
            if (!tMediaTransChn.GetMediaChannelKey().empty())
            {
                if (dwPunchNum >= NATAGENT_MAX_PORT_NUM)
                {
                    GLOBELOG(ERROR_LEV, "TransChns to punch exceed NATAGENT_MAX_PORT_NUM, IGNORE remnant!\n");
                    break;
                }

                std::ostringstream oss;
                oss<<tMediaTransChn;
                GLOBELOG(EVENT_LEV, "MediaTransChn need to Punch:[%s]\n", oss.str().c_str());

                //PunchKey
                atPunchInfo[dwPunchNum].m_tPunchKey.m_wLen = tMediaTransChn.GetMediaChannelKey().length();
                strncpy(atPunchInfo[dwPunchNum].m_tPunchKey.m_achKey, tMediaTransChn.GetMediaChannelKey().c_str(), sizeof(atPunchInfo[dwPunchNum].m_tPunchKey.m_achKey));

                //LocalPort
                if (itrMediaDesc->GetMediaType() == MEDIA_TYPE_VIDEO)
                {
                    if (tMediaTransChn.GetTransChnType() == MEDIA_TRANS_CHN_RTP)
                    {
                        atPunchInfo[dwPunchNum].m_wLocalPort = nPuRecPortStart+0;
                    }
                    else
                    {
                        atPunchInfo[dwPunchNum].m_wLocalPort = nPuRecPortStart+1;
                    }
                }
                else
                {
                    if (tMediaTransChn.GetTransChnType() == MEDIA_TRANS_CHN_RTP)
                    {
                        atPunchInfo[dwPunchNum].m_wLocalPort = nPuRecPortStart+2;
                    }
                    else
                    {
                        atPunchInfo[dwPunchNum].m_wLocalPort = nPuRecPortStart+3;
                    }
                }                  

                //RemoteAddrList
                TNatAddr* ptRemoteAddrList = new TNatAddr[tMediaTransChn.GetAddrList().size()];
                memset(ptRemoteAddrList, 0, sizeof(tMediaTransChn.GetAddrList().size()));

                s32 nValidAddrsNum = 0;
                for (TNetAddrList::const_iterator itrNetAddr = tMediaTransChn.GetAddrList().begin(); 
                    itrNetAddr < tMediaTransChn.GetAddrList().end(); itrNetAddr++)
                {
                    if (Kdm_IsValidIPStr(itrNetAddr->GetNetIp().c_str()) && Kdm_IsValidPort(itrNetAddr->GetNetPort()))
                    {
                        Cms2Na_NetAddr(*itrNetAddr, ptRemoteAddrList[nValidAddrsNum]);

                        nValidAddrsNum++;
                    }
                    else
                    {
                        GLOBELOG(WARNING_LEV, "IGNORE invalid address [%s:%d]!\n", itrNetAddr->GetNetIp().c_str(), itrNetAddr->GetNetPort());
                    }
                }

                atPunchInfo[dwPunchNum].m_wRemoteAddrsNum = nValidAddrsNum;
                atPunchInfo[dwPunchNum].m_ptRemoteAddrList = ptRemoteAddrList;

                dwPunchNum++;
            }
        }           
    }

    //开始Punch
    u32 dwSeqNum = 0;
    ENATAGENT_OPR_RESULT eNAResult = NATAGENT_OK;

    if (dwPunchNum != 0 )
    {
        eNAResult = NAPunch(emSelfBind, dwPunchNum, atPunchInfo, NATAGENT_INVALID_IP_STR/*(s8*)tLocalAddr.GetNetIp().c_str()*/, &dwSeqNum, pvContext);
        if (NATAGENT_OK != eNAResult)
        {
            GLOBELOG(ERROR_LEV, "NAPunch is fail!\n");

            return false;
        }
        else
        {
            GLOBELOG(CRITICAL_LEV, "NAPunch is successful.\n");
        }

        //释放资源
        for (s32 dwPunIdx = 0; dwPunIdx < dwPunchNum; dwPunIdx++)
        {
            SAFE_DEL_ARRAY_PTR(atPunchInfo[dwPunIdx].m_ptRemoteAddrList);
        }
    }
    else
    {
        GLOBELOG( WARNING_LEV, "There is no TransChn need to punch!\n");
    }

    return true;
}

u64 MakeAddrID( const u32 &dwIP, const u16 &wPort )
{
    u64 ddwValue = 0;
    u64 ddwRet = 0;
    ddwValue = dwIP;
    ddwRet = (ddwValue << 32) + wPort;
    return ddwRet;
}

CPuData::CPuData()
{
    m_nHasInvitedNum = 0;
}

CPuData::~CPuData()
{
    Clear();
}

void CPuData::Clear()
{
    m_tPuConfig.Clear();
    m_tPuOnOff.Clear();
    m_tPuServiceStatus.Clear();
    m_tPuAlarmStatus.Clear();
}

void CPuDataList::AddPuData(CPuData* pPuData)
{
    if (pPuData == NULL)
    {
        CLASSLOG(ERROR_LEV, "pPuData == NULL");
        return;
    }

    string strDevUri = pPuData->GetDevUri();
    m_cPuList.Insert(strDevUri, pPuData);
}

void CPuDataList::DelPuData(CPuData* pPuData)
{
    if (pPuData == NULL)
    {
        CLASSLOG(ERROR_LEV, "pPuData == NULL");
        return;
    }

    DelPuData(pPuData->GetDevUri());

}

void CPuDataList::DelPuData(const string& strUri)
{
    CPuData* pPuData = NULL;

    if (m_cPuList.Find(strUri, pPuData) && pPuData != NULL)
    {
        m_cPuList.Erase(strUri);
        delete pPuData;
        pPuData = NULL;
    }
    else
    {
        CLASSLOG(ERROR_LEV, "dev[%s]没有对应的pudata\n", strUri.c_str());
    }
}

CPuData* CPuDataList::FindPuData(const string& strUri)
{
    CPuData* pPuData = NULL;
    m_cPuList.Find(strUri, pPuData);
    return pPuData;
}


void CPuDataList::PrintData(u8 byFlag)
{
    if (byFlag != 0)
    {
        OspPrintf(TRUE, FALSE, "当前在线的pu：[%u]个\n", GetOnlinePuNum());
    }
    else
    {
        OspPrintf(TRUE, FALSE, "当前所有的pu：[%u]个\n", m_cPuList.GetSize()); 
    }
      
    Iterator pPos;
    u32 i = 0;

    while (!pPos.End())
    {
        string strPu;
        CPuData* pPuData = NULL;
        if (m_cPuList.Iterate(pPos, strPu, pPuData) && pPuData != NULL)
        {
            if (byFlag != 0)
            {
                //打印在线PU
                if (pPuData->GetPuStatus().GetIsOnline())
                {
                    i++;
                    OspPrintf(TRUE, FALSE, "  %4u. pu[%s] [online], ivtNum[%d]\n", 
                        i, strPu.c_str(), pPuData->GetInvitedNum());
                }             
            }
            else
            {
                //打印所有PU
                string strState = "offline";
                if (pPuData->GetPuStatus().GetIsOnline())
                {
                    strState = "online";
                }
                i++;
                OspPrintf(TRUE, FALSE, "  %4u. pu[%s] [%s]\n", i, strPu.c_str(), strState.c_str());
            }         
        }
    }

    if (byFlag != 0)
    {
        OspPrintf(TRUE, FALSE, "当前在线的pu：[%u]个\n", GetOnlinePuNum());
    }
    else
    {
        OspPrintf(TRUE, FALSE, "当前所有的pu：[%u]个\n", m_cPuList.GetSize()); 
    }
}

u32 CPuDataList::GetOnlinePuNum()
{
    Iterator pPos;   
    u32 dwOnlinePuNum = 0;

    while (!pPos.End())
    {
        string strPu;
        CPuData* pPuData = NULL;
        if (m_cPuList.Iterate(pPos, strPu, pPuData) && pPuData != NULL)
        {
            if (pPuData->GetPuStatus().GetIsOnline())
            {
                dwOnlinePuNum++;
            }
            
        }
    }

    return dwOnlinePuNum;
}

CPuDataMap CPuDataList::GetOnLinePuDataList()
{
    CPuDataMap cOnlinePulist;

    Iterator pPos;   

    while (!pPos.End())
    {
        string strPu;
        CPuData* pPuData = NULL;
        if (m_cPuList.Iterate(pPos, strPu, pPuData) && pPuData != NULL)
        {
            if (pPuData->GetPuStatus().GetIsOnline())
            {
                cOnlinePulist[strPu] = pPuData;
            }
        }
    }

    return cOnlinePulist;
}

CPuDataMap CPuDataList::GetOnLineNoInvitePuDataList()
{
    CPuDataMap cOnlineNoInvitePulist;

    Iterator pPos;   

    while (!pPos.End())
    {
        string strPu;
        CPuData* pPuData = NULL;
        if (m_cPuList.Iterate(pPos, strPu, pPuData) && pPuData != NULL)
        {
            if (pPuData->GetPuStatus().GetIsOnline() && pPuData->GetInvitedNum() == 0)
            {
                cOnlineNoInvitePulist[strPu] = pPuData;
            }
        }
    }

    return cOnlineNoInvitePulist;
}

u32 CPuDataList::GetSize()
{
    return m_cPuList.GetSize();
}