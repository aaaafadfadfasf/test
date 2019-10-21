/*****************************************************************************
   文件实现功能: 内部公共部分定义
   作者        : huzhiyun
   版本        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人      修改内容
   2011/11/11   1.0        huzhiyun        创建
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

/****************************** 内部AppID定义 ******************************/
enum
{
    AID_BEGIN = 400, // 1.0版本中AppID最大规划到380
    AID_CCtrl = AID_BEGIN+1,
    AID_License = AID_BEGIN+2,
    AID_SyncAnalyze = AID_BEGIN+3,
};

/****************************** 发送消息接口列表 ******************************/
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

// 投递同步数据变更通知至CCtrl
template<typename ntftype>
void PostSyncDataChangeNtf2CCtrl(const ntftype &cChangeNtfReq)
{
    CEventReq *pcReq = new ntftype(cChangeNtfReq);
    pcReq->SetSession("111"); // 设置一下即可，不设置发送时无法正确修改会话号

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

// hzytodo2 改造成模板的形式
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

/****************************** 公共接口列表 ******************************/
// 校验是否存在环和重名（添加时存在进行修改，修改时不存在不修改）. bAdd为true表示添加，否则表示修改.
bool ValidateDomainRelation(const vector<CDomainRelationInfo> &acCurrentDomainList, const CDomainRelationInfo& cDomain, bool bAdd);
// license计费时调整厂商名称，比如g100_haikang与haikang都为haikang
void AdjustManufactureName(string &strManufacture, bool *pbIsOtherPuDirectAccess = NULL);

/****************************** 公共业务常量定义 *******************************/
const u32 SIP_MSG_LEN_MAX = 20480;  // 最大大小取决于osp 32768的限制，我们业务消息只用20K左右
const s8* const MANUFACTURE_KEDACOM = "kedacom";
const u32 DEVID_LEN = 32;    // 设备编号长度
const s8* const FILE_KEY_NAME = "kedalicense.key";

string CurrentDomainId();    // 当前域编号
string CurrentDomainName();  // 当前域名称
void UpdateCurrentDomainName(const string &strNewDomainName);

const s8* const DOMAIN_AUTHORIZATION_DEVGRPNAME = "domain_authorization_device_group"; // 跨域授权设备组组名

//3as服务器相关定义
const s8* const LICENSESDK_PRODUCT_NAME = "监控平台V2 1.0"; //license服务器中，2.0产品的名称
const u32 LICENSESDK_MAX_RETRY_COUNT = 3;                    //SDK检测多少次后无响应认定为断链
const u32 LICENSESDK_CHECK_TIME = 5;                         //SDK每次检测间隔时间
const u32 LICENSESDK_FREETIME = 3600;                       //SDK断链后，经过freetime再调用回调


//const s8* const LICENSE_GETLICENSEINFO_MD5SUM = "2118c167b9427fe1f8b57ff635c46f9c";   //getlicenseinfo工具的md5值
const s8* const LICENSE_GETLICENSEINFO_MD5SUM = "e39805069d562ca080120b275cbeb26e";   // 新版getlicenseinfo工具的md5值

const s8* const MACHINE_TYPE_2901_G2_S = "2901-G2-S";   //接入容量300
const s8* const MACHINE_TYPE_2901_G2_M = "2901-G2-M";   //接入容量10000
const s8* const MACHINE_TYPE_2901_G2_L = "2901-G2-L";   //接入容量100000
const s8* const MACHINE_TYPE_2901_G2_H = "2901-G2-H";   //接入容量300000
const s8* const MACHINE_TYPE_2901_G2_H1 = "2901-G2-H1";   //接入容量1000000
const s8* const MACHINE_TYPE_2901_G2_H2 = "2901-G2-H2";   //接入容量2000000
const u32 MACHINE_TYPE_2901_G2_S_MAX_PU_NUM = 300;
const u32 MACHINE_TYPE_2901_G2_M_MAX_PU_NUM = 10000;
const u32 MACHINE_TYPE_2901_G2_L_MAX_PU_NUM = 100000;
const u32 MACHINE_TYPE_2901_G2_H_MAX_PU_NUM = 500000;
const u32 MACHINE_TYPE_2901_G2_H1_MAX_PU_NUM = 1000000;
const u32 MACHINE_TYPE_2901_G2_H2_MAX_PU_NUM = 2000000;
const s8* const ADAPT_2901_SAN_KDSIP_VALUE_TOOL = "/opt/kdm/tas/adapt2901san.sh";
/****************************** 公共数据结构定义 *******************************/

struct TInnerDeviceTypeInfo
{
    u32 dwTypeId;
    vector<u32> adwDevCapIds;
};

#endif // _INNER_COMMON_H_
