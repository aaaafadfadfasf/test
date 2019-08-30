#include "ospsiphb.h"

COspSipHbNodeList g_cHbNodeList;

bool OspSipHeartbeatReq(const TSipURI& tDstURI, const TOspTransID& tOspTrans, LPCSTR szDirectUAAddr)
{
    string strNullBody;
    return OspSipPostReq(KDSIP_EVENT_OPTIONS_HB_REQ, strNullBody, tDstURI, tOspTrans, szDirectUAAddr);
}

bool OspSipHeartbeatRsp(TKDSIP_TRANS_ID tSipTrans, EKDSIP_RESPONSE_CODE eResponseCode)
{
    return (KDSIP_OK == KDSipOptionsRsp(tSipTrans, eResponseCode));
}

TOspSipHbNode* COspSipHbNodeList::FindHbNode(const TSipURI& tDstURI)
{
    CAutoLock cLock(m_cSemLock);
    return m_cOspSipHbNodeList.Find(tDstURI);
}

bool COspSipHbNodeList::InsertHbNode(const TOspSipHbNode& tHbNode)
{
    if(!tHbNode.tHbDst.IsValidURI())
    {
        CLASSLOG(OSPSIP, ERROR_LEV, "无效的心跳URI\n");
        return false;
    }

    CAutoLock cLock(m_cSemLock);
    TOspSipHbNode* pHbNode = m_cOspSipHbNodeList.Find(tHbNode.tHbDst);

    if (pHbNode != NULL)
    {
        pHbNode->tHbSrc = tHbNode.tHbSrc;
        pHbNode->wHbCnt = tHbNode.wHbCnt;
        pHbNode->wHbTime = tHbNode.wHbTime;
        pHbNode->strDirectUAAddr = tHbNode.strDirectUAAddr;
    }
    else
    {
        m_cOspSipHbNodeList.Insert(tHbNode.tHbDst, tHbNode);
    }

    return true;
}

void COspSipHbNodeList::EraseHbNode(const TSipURI& tDstURI)
{
    CAutoLock cLock(m_cSemLock);
    m_cOspSipHbNodeList.Erase(tDstURI);
}

size_t COspSipHbNodeList::GetSize() const
{
    CAutoLock cLock(m_cSemLock);
    return (size_t)m_cOspSipHbNodeList.GetSize();
}

bool COspSipHbNodeList::IsRegSipProxy() const
{
    CAutoLock cLock(m_cSemLock);
    return m_bIsRegSipProxy;
}

void COspSipHbNodeList::SetRegSipProxy(bool bIsReg)
{
    CAutoLock cLock(m_cSemLock);
    m_bIsRegSipProxy = bIsReg;
}

void COspSipHbNodeList::PrintHbList()
{
    CAutoLock cLock(m_cSemLock);

    OspPrintf(TRUE, FALSE, "------------------------------------------------------------------------------\n");

    OspPrintf(TRUE, FALSE, "ospsip is reg sip proxy：[%d]\n", m_bIsRegSipProxy); 

    OspPrintf(TRUE, FALSE, "ospsip心跳列表：[%u]个\n", m_cOspSipHbNodeList.GetSize());    
    u32 i = 0;
    Iterator pPos;
    while (!pPos.End())
    {
        TSipURI tDstURI;
        TOspSipHbNode tHbNode;

        if (m_cOspSipHbNodeList.Iterate(pPos, tDstURI, tHbNode))
        {                    
            i++;
            OspPrintf(TRUE, FALSE, "  %4u. HB-dst[sip(%s)-directAddr(%s)]---HB-src[osp app(%u)-ins(%u)-task(%u)-seq(%u)], HB-time[%u], HB-cnt[%u], Work-inst[%u]\n", 
                i, tDstURI.GetURIString().c_str(), tHbNode.strDirectUAAddr.c_str(), GETAPP(tHbNode.tHbSrc.dwIID), 
                GETINS(tHbNode.tHbSrc.dwIID), tHbNode.tHbSrc.dwTaskID, tHbNode.tHbSrc.dwSeqNO, tHbNode.wHbTime, 
                tHbNode.wHbCnt, tHbNode.wHbWorkInst);
        }
    }    
    OspPrintf(TRUE, FALSE, "ospsip心跳列表：[%u]个\n", m_cOspSipHbNodeList.GetSize());
    OspPrintf(TRUE, FALSE, "------------------------------------------------------------------------------\n\n");
}