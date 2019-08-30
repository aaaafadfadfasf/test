#ifndef _OSP_SIP_HB_H_
#define _OSP_SIP_HB_H_

#include "cms/ospsip/ospsip.h"

bool OspSipHeartbeatReq(const TSipURI& tDstURI, const TOspTransID& tOspTrans, LPCSTR szDirectUAAddr = NULL);
bool OspSipHeartbeatRsp(TKDSIP_TRANS_ID tSipTrans, EKDSIP_RESPONSE_CODE eResponseCode = KDSIP_RESPONSE_200_OK);

struct TOspSipHbNode
{
    TOspSipHbNode()
    {
        Clear();
    }

    void Clear()
    {
        tHbDst.Clear();
        tHbSrc = TOspTransID();
        wHbTime = 0;
        wHbCnt = 0;
        wHbWorkInst = CInstance::INVALID;
        strDirectUAAddr.clear();
    }

    TSipURI tHbDst;         //Ŀ����ԪUA��URL
    TOspTransID tHbSrc;     //�ϲ��û�APP�е�Inst�е�����Task
    u16 wHbTime;            //HB������һ��10��3��
    u16 wHbCnt;

    u16 wHbWorkInst;        //ִ��HB��InstId

    string strDirectUAAddr; //��ʹ��proxyʱ������Ŀ�ĵ�ַ
};

class COspSipHbNodeList
{
public:
    COspSipHbNodeList()
    {
        m_bIsRegSipProxy = false;
    }

    TOspSipHbNode* FindHbNode(const TSipURI& tDstURI);
    bool InsertHbNode(const TOspSipHbNode& tHbNode);
    void EraseHbNode(const TSipURI& tDstURI);
    size_t GetSize() const;

    bool IsRegSipProxy() const;
    void SetRegSipProxy(bool bIsReg);

    void PrintHbList();

private:
    bool m_bIsRegSipProxy;
    CXMap<TSipURI, TOspSipHbNode> m_cOspSipHbNodeList;
    mutable CXSemLock m_cSemLock;
};

extern COspSipHbNodeList g_cHbNodeList;


#endif  //#ifndef _OSP_SIP_HB_H_
