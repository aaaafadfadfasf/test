/*****************************************************************************
   �ļ�ʵ�ֹ���: �ڲ��������ֶ���
   ����        : huzhiyun
   �汾        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
   2011/11/11   1.0        huzhiyun        ����
******************************************************************************/
#ifndef _INNER_COMMON_H_
#define _INNER_COMMON_H_
#include "kdvtype.h"
#include "cmsassert.h"
#include <string>
#include <set>
#include <map>
#include <list>
#include <vector>
using namespace std;
#include "cms/cms_proto.h"
#include "cms/cms_errorcode.h"
#include "log_system.h"
#include "utility/cmstime.h"
#include "inner_util.h"
#include "inner_msg.h"
#include "cbb/nms/nmclib.h"

typedef ECMS_ERRORCODE ENTasErrno;

/****************************** �ڲ�AppID���� ******************************/
enum
{
    AID_BEGIN = 400, // 1.0�汾��AppID���滮��380
    AID_CCtrl = AID_BEGIN+1,
    AID_License = AID_BEGIN+2,
    AID_SyncAnalyze = AID_BEGIN+3,
};

/****************************** ������Ϣ�ӿ��б� ******************************/
bool PostTask2DbPool(u16 wEventId, const COspObj* cOspObj);

template<typename transfer>
bool PostTaskMsgTransfer2DbTaskPool(const transfer & trans)
{
    transfer *pNewTrans = new transfer();
    *pNewTrans = trans;

    GLOBELOG(enLogSrcDbTaskPool, ALL_LEV, "PostTaskMsgTransfer2DbTaskPool, msgobj pointer=%p\n", pNewTrans);
    if (PostTask2DbPool(msgTaskMsgTransfer4Db, pNewTrans))
    {
        return true;
    }
    else
    {
        delete pNewTrans;
        return false;
    }
}

template<typename Tbody>
void PostMsg2License(u16 wEventId, const Tbody& tbody)
{
    Tbody *ptBody = new Tbody(tbody);
    if (0 != OspPost(MAKEIID(AID_License, CInstance::DAEMON), wEventId, &ptBody, sizeof(Tbody *)))
    {
        delete ptBody;
    }
}

// Ͷ��ͬ�����ݱ��֪ͨ��CCtrl
template<typename ntftype>
void PostSyncDataChangeNtf2CCtrl(const ntftype &cChangeNtfReq)
{
    CEventReq *pcReq = new ntftype(cChangeNtfReq);
    pcReq->SetSession("111"); // ����һ�¼��ɣ������÷���ʱ�޷���ȷ�޸ĻỰ��

    if (0 != OspPost(MAKEIID(AID_CCtrl, CInstance::DAEMON), msgSyncDataChangeNtf, &pcReq, sizeof(CEventReq *)))
    {
        delete pcReq;
    }
}

template<typename dstTransferType>
void PostTaskMsgTransfer2CCtrl(const CTaskMsgTransferBase &srcTransfer, const CEventReq &cReq, CEventRsp &cRsp, dstTransferType &dstTransfer)
{
    dstTransferType *pNewTrans = new dstTransferType();

    cRsp.SetSeqNum(cReq.GetSeqNum());
    cRsp.SetSession(cReq.GetSession());
    dstTransfer.SetTransfer(srcTransfer, cRsp.GetEventId());

    *pNewTrans = dstTransfer;

    ptrint ptr=reinterpret_cast<ptrint>(pNewTrans);
    GLOBELOG(enLogSrcDbTaskPool, ALL_LEV, "PostTaskMsgTransfer2CCtrl, msgobj pointer=%p\n", ptr);
    if (0 != OspPost(MAKEIID(AID_CCtrl, CInstance::DAEMON), msgTaskMsgTransfer4Db, &ptr, sizeof(ptr)))
    {
        delete pNewTrans;
    }
}

template<typename dstTransferType>
void PostTaskMsgTransfer2CCtrl(const CTaskMsgTransferBase &srcTransfer, const CEventReq &cReq, CEventReq &cNtf, dstTransferType &dstTransfer)
{
    dstTransferType *pNewTrans = new dstTransferType();

    cNtf.SetSeqNum(static_cast<int>(GenSequenceNum()));
    cNtf.SetSession(cReq.GetSession());
    dstTransfer.SetTransfer(srcTransfer, cNtf.GetEventId());

    *pNewTrans = dstTransfer;

    ptrint ptr=reinterpret_cast<ptrint>(pNewTrans);
    GLOBELOG(enLogSrcDbTaskPool, ALL_LEV, "PostTaskMsgTransfer2CCtrl, msgobj pointer=%p\n", ptr);
    if (0 != OspPost(MAKEIID(AID_CCtrl, CInstance::DAEMON), msgTaskMsgTransfer4Db, &ptr, sizeof(ptr)))
    {
        delete pNewTrans;
    }
}

// hzytodo2 �����ģ�����ʽ
#define PostTaskMsgTransfer2CCtrl4LargeNtf(NtfReq, Member, Container) \
    u32 dwNtfSize = cNtf.ToXml().size();\
    if (dwNtfSize < SIP_MSG_LEN_MAX)\
    {\
    PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cNtf, cTransNtf);\
    }\
    else\
    {\
    u32 dwSplitNum = dwNtfSize/SIP_MSG_LEN_MAX + 1;\
    u32 dwSplitElements = cNtf.Member().Container().size()/dwSplitNum + 1;\
    \
    NtfReq cNtfTmp = cNtf;\
    for (u32 itBegin = 0; itBegin < cNtf.Member().Container().size();)\
    {\
    u32 dwLeaf = cNtf.Member().Container().size() - itBegin;\
    u32 itSize = dwSplitElements < dwLeaf? dwSplitElements: dwLeaf;\
    \
    cNtfTmp.Member().Container().clear();\
    for (u32 i = 0; i < itSize; ++i)\
    {\
    cNtfTmp.Member().Container().push_back(cNtf.Member().Container()[itBegin++]);\
    }\
    PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cNtfTmp, cTransNtf);\
    }\
    }

template<typename dstTransferType>
void PostTaskMsgTransfer2CCtrl(const CTaskMsgTransferBase &srcTransfer, u16 eventId, dstTransferType &dstTransfer)
{
    dstTransferType *pNewTrans = new dstTransferType();

    dstTransfer.SetTransfer(srcTransfer, eventId);

    *pNewTrans = dstTransfer;

    ptrint ptr=reinterpret_cast<ptrint>(pNewTrans);
    GLOBELOG(enLogSrcDbTaskPool, ALL_LEV, "PostTaskMsgTransfer2CCtrl, msgobj pointer=%u\n", (u32)ptr);
    if (0 != OspPost(MAKEIID(AID_CCtrl, CInstance::DAEMON), msgTaskMsgTransfer4Db, &ptr, sizeof(ptr)))
    {
        delete pNewTrans;
    }
}

#define PostSipRsp2UA(cReq, cRsp, tSipTransId) \
do { \
    cRsp.SetSeqNum(cReq.GetSeqNum()); \
    if (cRsp.GetSession().empty()) cRsp.SetSession(cReq.GetSession()); \
    PostMsgRsp(tSipTransId, cRsp); \
} while(false)

template<typename transfer>
bool PostDbMsgTransfer2DbTaskPool(const transfer & trans)
{
    transfer *pNewTrans = new transfer(trans);

    GLOBELOG(enLogSrcDbTaskPool, ALL_LEV, "PostDbMsgTransfer2DbTaskPool, msgobj pointer=%p\n", pNewTrans);
    if (PostTask2DbPool(msgDbMsgTransfer, pNewTrans))
    {
        return true;
    }
    else
    {
        delete pNewTrans;
        return false;
    }
}

template<typename transfer>
void PostDbMsgTransferFromDbTaskPool(const transfer &rspTransfer)
{
    transfer *pNewTrans = new transfer();

    *pNewTrans = rspTransfer;

    ptrint ptr=reinterpret_cast<ptrint>(pNewTrans);
    GLOBELOG(enLogSrcDbTaskPool, ALL_LEV, "PostDbMsgTransferOfRsp, msgobj pointer=%u\n", (u32)ptr);
    if (0 != OspPost(pNewTrans->GetOspIID(), msgDbMsgTransfer, &ptr, sizeof(ptr)))
    {
        delete pNewTrans;
    }
}

/****************************** �����ӿ��б� ******************************/
// У���Ƿ���ڻ������������ʱ���ڽ����޸ģ��޸�ʱ�����ڲ��޸ģ�. bAddΪtrue��ʾ��ӣ������ʾ�޸�.
bool ValidateDomainRelation(const vector<CDomainRelationInfo> &acCurrentDomainList, const CDomainRelationInfo& cDomain, bool bAdd);
// license�Ʒ�ʱ�����������ƣ�����g100_haikang��haikang��Ϊhaikang
void AdjustManufactureName(string &strManufacture, bool *pbIsOtherPuDirectAccess = NULL);

/****************************** ����ҵ�������� *******************************/
const u32 SIP_MSG_LEN_MAX = 20480;  // ����Сȡ����osp 32768�����ƣ�����ҵ����Ϣֻ��20K����
const s8* const MANUFACTURE_KEDACOM = "kedacom";
const u32 DEVID_LEN = 32;    // �豸��ų���
const s8* const FILE_KEY_NAME = "kedalicense.key";

string CurrentDomainId();    // ��ǰ����
string CurrentDomainName();  // ��ǰ������
void UpdateCurrentDomainName(const string &strNewDomainName);

const s8* const DOMAIN_AUTHORIZATION_DEVGRPNAME = "domain_authorization_device_group"; // ������Ȩ�豸������

//3as��������ض���
const s8* const LICENSESDK_PRODUCT_NAME = "���ƽ̨V2 1.0"; //license�������У�2.0��Ʒ������
const u32 LICENSESDK_MAX_RETRY_COUNT = 3;                    //SDK�����ٴκ�����Ӧ�϶�Ϊ����
const u32 LICENSESDK_CHECK_TIME = 5;                         //SDKÿ�μ����ʱ��
const u32 LICENSESDK_FREETIME = 3600;                       //SDK�����󣬾���freetime�ٵ��ûص�


//const s8* const LICENSE_GETLICENSEINFO_MD5SUM = "2118c167b9427fe1f8b57ff635c46f9c";   //getlicenseinfo���ߵ�md5ֵ
const s8* const LICENSE_GETLICENSEINFO_MD5SUM = "e39805069d562ca080120b275cbeb26e";   // �°�getlicenseinfo���ߵ�md5ֵ

const s8* const MACHINE_TYPE_2901_G2_S = "2901-G2-S";   //��������300
const s8* const MACHINE_TYPE_2901_G2_M = "2901-G2-M";   //��������10000
const s8* const MACHINE_TYPE_2901_G2_L = "2901-G2-L";   //��������100000
const s8* const MACHINE_TYPE_2901_G2_H = "2901-G2-H";   //��������300000
const s8* const MACHINE_TYPE_2901_G2_H1 = "2901-G2-H1";   //��������1000000
const s8* const MACHINE_TYPE_2901_G2_H2 = "2901-G2-H2";   //��������2000000
const u32 MACHINE_TYPE_2901_G2_S_MAX_PU_NUM = 300;
const u32 MACHINE_TYPE_2901_G2_M_MAX_PU_NUM = 10000;
const u32 MACHINE_TYPE_2901_G2_L_MAX_PU_NUM = 100000;
const u32 MACHINE_TYPE_2901_G2_H_MAX_PU_NUM = 500000;
const u32 MACHINE_TYPE_2901_G2_H1_MAX_PU_NUM = 1000000;
const u32 MACHINE_TYPE_2901_G2_H2_MAX_PU_NUM = 2000000;
const s8* const ADAPT_2901_SAN_KDSIP_VALUE_TOOL = "/opt/kdm/tas/adapt2901san.sh";
/****************************** �������ݽṹ���� *******************************/

struct TInnerDeviceTypeInfo
{
    u32 dwTypeId;
    vector<u32> adwDevCapIds;
};

#endif // _INNER_COMMON_H_
