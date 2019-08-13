#include "ospsipgbnat.h"
#include "cbb/sip/sip_tool.h"

static CXSemLock s_cNatLock;
static map<string, vector<TOspSipNatData> > s_mapNatData;
static map<string, string> s_mapGbua2NatId;
static map<string, u32> s_mapNatId2RefCount;

void OspSipGbNatSetData(const string &strNatId, const vector<TOspSipNatData> &vecNatData)
{
    s_cNatLock.Lock();
    if (s_mapNatData.find(strNatId) == s_mapNatData.end())
    {
        s_mapNatId2RefCount[strNatId] = 0;
    }
    s_mapNatData[strNatId] = vecNatData;
    s_cNatLock.UnLock();
}

bool OspSipGbNatDelData(const string &strNatId)
{
    bool bRet = true;
    s_cNatLock.Lock();
    map<string, u32>::iterator it = s_mapNatId2RefCount.find(strNatId);
    if (it != s_mapNatId2RefCount.end())
    {
        if (it->second == 0)
        {
            s_mapNatId2RefCount.erase(it);
            s_mapNatData.erase(strNatId);
        }
        else
        {
            bRet = false;
        }
    }
    s_cNatLock.UnLock();

    return bRet;
}

bool OspSipGbNatSetGbua(const string &strGbua, const string &strNatId)
{
    bool bRet=true;
    s_cNatLock.Lock();
    map<string, string>::iterator itGbua = s_mapGbua2NatId.find(strGbua);
    if (itGbua == s_mapGbua2NatId.end())
    {
        map<string, u32>::iterator it = s_mapNatId2RefCount.find(strNatId);
        if (it != s_mapNatId2RefCount.end())
        {
            it->second += 1;
            s_mapGbua2NatId[strGbua]=strNatId;
        }
        else
        {
            bRet=false;
        }
    }
    else
    {
        if (itGbua->second != strNatId)
        {
            map<string, u32>::iterator it = s_mapNatId2RefCount.find(strNatId);
            if (it != s_mapNatId2RefCount.end())
            {
                s_mapNatId2RefCount.find(itGbua->second)->second -= 1;

                it->second += 1;
                itGbua->second=strNatId;
            }
            else
            {
                bRet=false;
            }
        }
    }
    
    s_cNatLock.UnLock();

    return bRet;
}

void OspSipGbNatDelGbua(const string &strGbua)
{
    s_cNatLock.Lock();
    map<string, string>::iterator it=s_mapGbua2NatId.find(strGbua);
    if ( it != s_mapGbua2NatId.end())
    {
        s_mapNatId2RefCount[it->second] -= 1;
        s_mapGbua2NatId.erase(it);
    }
    s_cNatLock.UnLock();
}

void OspSipGbNatClearAllData()
{
    s_cNatLock.Lock();
    s_mapNatData.clear();
    s_mapGbua2NatId.clear();
    s_mapNatId2RefCount.clear();
    s_cNatLock.UnLock();
}

void OspSipConvert2ExtraNetAddr(const TSipExtraInfo* ptSipExtraInfo, const string &strIntranetIp, u16 wIntranetPort, 
    string &strExtranetIp, u16 &wExtranetPort)
{
    strExtranetIp = strIntranetIp;
    wExtranetPort = wIntranetPort;

    if (ptSipExtraInfo == NULL || ptSipExtraInfo->strGbua.empty())
    {
        return;
    }

    s_cNatLock.Lock();
    map<string, string>::const_iterator it = s_mapGbua2NatId.find(ptSipExtraInfo->strGbua);
    if (it == s_mapGbua2NatId.end())
    {
        // nothing
    }
    else
    {
        map<string, vector<TOspSipNatData> >::const_iterator it2 = s_mapNatData.find(it->second);
        for (vector<TOspSipNatData>::const_iterator it3 = it2->second.begin(); it3 != it2->second.end();++it3)
        {
            if (it3->intraNetPortBegin <= wIntranetPort && wIntranetPort <= it3->intraNetPortEnd
                && it3->proto == ptSipExtraInfo->natProto && it3->intraNetIp == strIntranetIp)
            {
                strExtranetIp=it3->extraNetIp;
                wExtranetPort=it3->extraNetPortBegin+wIntranetPort-it3->intraNetPortBegin;
                break;
            }
        }
    }
    s_cNatLock.UnLock();
}

void OspSipConvert2ExtraNetAddr(TOspSipSendMsg *ptSipSendMsg, bool bConvertFrom, bool bConvertContact, bool bConvertSdp)
{
    if (ptSipSendMsg == NULL || ptSipSendMsg->ptSipExtraInfo==NULL)
    {
        return;
    }
    s_cNatLock.Lock();
    if (s_mapGbua2NatId.find(ptSipSendMsg->ptSipExtraInfo->strGbua)==s_mapGbua2NatId.end())
    {
        s_cNatLock.UnLock();
        return;
    }
    s_cNatLock.UnLock();

    TSipExtraInfo *ptSipExtraInfo = ptSipSendMsg->ptSipExtraInfo;
    if (bConvertFrom && !ptSipExtraInfo->strFromUri.empty())
    {
        TSipURI tUri(ptSipExtraInfo->strFromUri);
        s8 intranetIp[100];
        intranetIp[sizeof(intranetIp)-1]='\0';
        u32 intranetPort;
        if (2 == sscanf(tUri.GetDomain().c_str(), "%99[^:]:%d", intranetIp, &intranetPort))
        {
            string extraNetIp;
            u16 extraNetPort;
            OspSipConvert2ExtraNetAddr(ptSipExtraInfo, string(intranetIp), (u16)intranetPort, extraNetIp, extraNetPort);

            char szBuf[150];
            szBuf[sizeof(szBuf)-1]='\0';
            sprintf(szBuf, "%s:%d", extraNetIp.c_str(), extraNetPort);
            tUri.SetDomain(szBuf);

            ptSipExtraInfo->strFromUri = tUri.GetURIString();
        }
    }

    if (bConvertContact && !ptSipExtraInfo->strContactUri.empty())
    {
        TSipURI tUri(ptSipExtraInfo->strContactUri);
        s8 intranetIp[100];
        intranetIp[sizeof(intranetIp)-1]='\0';
        u32 intranetPort;
        if (2 == sscanf(tUri.GetDomain().c_str(), "%99[^:]:%d", intranetIp, &intranetPort))
        {
            string extraNetIp;
            u16 extraNetPort;
            OspSipConvert2ExtraNetAddr(ptSipExtraInfo, string(intranetIp), (u16)intranetPort, extraNetIp, extraNetPort);

            char szBuf[150];
            szBuf[sizeof(szBuf)-1]='\0';
            sprintf(szBuf, "%s:%d", extraNetIp.c_str(), extraNetPort);
            tUri.SetDomain(szBuf);

            ptSipExtraInfo->strContactUri = tUri.GetURIString();
        }
    }

    if (bConvertSdp && ptSipExtraInfo->eSipContentType == KDSIP_CONTENT_SDP && !ptSipSendMsg->strMsgBuf.empty())
    {
        HTOOL_MEMLIST hMem = SIP_TOOL_CreateMemList();
        TGB_SDP tSDP;
        memset(&tSDP, 0, sizeof(tSDP));
        
        if (SIP_TOOL_CONVERT_S2TGB_SDP(hMem, ptSipSendMsg->strMsgBuf.c_str(), &tSDP) == SIP_TOOL_OK)
        {
            string extraNetIp;
            u16 extraNetPort;

            if (tSDP.szAddress_c != NULL && tSDP.nMediaListSize > 0)
            {
                OspSipConvert2ExtraNetAddr(ptSipExtraInfo, string(tSDP.szAddress_c), (u16)tSDP.ptMediaList_m[0].nPort, 
                    extraNetIp, extraNetPort);
                // 与dcg确认了tSDP中的指针，指向的是HTOOL_MEMLIST中的内存
                tSDP.ptMediaList_m[0].nPort=extraNetPort;

                if (tSDP.nMediaListSize > 1)
                {
                    string extraNetIp2;
                    u16 extraNetPort2;

                    OspSipConvert2ExtraNetAddr(ptSipExtraInfo, string(tSDP.szAddress_c), (u16)tSDP.ptMediaList_m[1].nPort, 
                        extraNetIp2, extraNetPort2);
                    tSDP.ptMediaList_m[1].nPort=extraNetPort2;
                }

                tSDP.szAddress_c = const_cast<LPSTR>(extraNetIp.c_str());

                s32 nSize = 20000;
                s8 achSdp[20000];
                memset(achSdp, 0, sizeof(achSdp));
                if (SIP_TOOL_CONVERT_TGB_SDP2S(&tSDP, achSdp, &nSize) == SIP_TOOL_OK)
                {
                    ptSipSendMsg->strMsgBuf=achSdp;
                }
            }
        }
        SIP_TOOL_FreeMemList(hMem);
    }
}

void OspSipGbNatDump()
{
    map<string, vector<TOspSipNatData> > mapNatData;
    map<string, string> mapGbua2NatId;
    map<string, u32> mapNatId2RefCount;

    s_cNatLock.Lock();
    mapNatData=s_mapNatData;
    mapGbua2NatId=s_mapGbua2NatId;
    mapNatId2RefCount=s_mapNatId2RefCount;
    s_cNatLock.UnLock();

    string strProto;
    for (map<string, vector<TOspSipNatData> >::const_iterator it=mapNatData.begin();
        it!=mapNatData.end();++it)
    {
        OspPrintf(TRUE, FALSE,"NatData table Id:%s's data list:\n\tproto intranetIp   intranetPortRange extranetIp   extranetPortRange description\n", 
            it->first.c_str());
        for (vector<TOspSipNatData>::const_iterator it2=it->second.begin();
            it2!=it->second.end();++it2)
        {
            if (it2->proto==TOspSipNatData::tcp)
            {
                strProto="tcp";
            }
            else if (it2->proto==TOspSipNatData::udp)
            {
                strProto="udp";
            }
            else
            {
                strProto="Unknown";
            }

            OspPrintf(TRUE, FALSE,"\t%s  %s %d-%d %s %d-%d %s\n", strProto.c_str(), it2->intraNetIp.c_str(),
                (s32)it2->intraNetPortBegin,(s32)it2->intraNetPortEnd, it2->extraNetIp.c_str(),
                (s32)it2->extraNetPortBegin, (s32)it2->extraNetPortEnd, it2->desc.c_str());
        }
    }

    if(!mapGbua2NatId.empty())OspPrintf(TRUE, FALSE,"\nthe map of gbua to natId:\n");
    for (map<string, string>::const_iterator it=mapGbua2NatId.begin();it!=mapGbua2NatId.end();++it)
    {
        OspPrintf(TRUE, FALSE,"\t%s--%s\n", it->first.c_str(), it->second.c_str());
    }

    if(!mapNatId2RefCount.empty())OspPrintf(TRUE, FALSE,"\nNatData table's reference count list:\n");
    for(map<string, u32>::const_iterator it=mapNatId2RefCount.begin();it!=mapNatId2RefCount.end();++it)
    {
        OspPrintf(TRUE, FALSE,"\t%s--%d\n", it->first.c_str(), (s32)it->second);
    }
}
