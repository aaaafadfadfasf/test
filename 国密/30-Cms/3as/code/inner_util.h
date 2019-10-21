/*
实现内部用到的辅助方法，辅助方法与业务弱相关
*/

#ifndef _INNER_UTIL_H_
#define _INNER_UTIL_H_
#include "osp.h"
#include <string>
using std::string;
#include <vector>
using std::vector;
#include "cms/ospsip/ospsip.h"
#include "cms/cms_proto.h"
#include "cms/tas/tas_struct.h"
#include <fstream>
using namespace std;

typedef size_t ptrint; // 指针数值类型

//浮点数比较相关的宏
//默认精度
#define EPS_MIN		(1e-5)
//比较两个浮点数是否相等（带精度
#define FLOAT_EQU_P(f1, f2, p) (((f1)-(f2)<=p)&&((f2)-(f1)<=p))
//比较两个浮点数是否相等，使用默认精度
#define FLOAT_EQU(f1, f2)  FLOAT_EQU_P(f1,f2, EPS_MIN)


struct TPushXmlGrpInfo
{
	string groupId;
	string groupName;
	string groupGbid;
	string parentId;
};

struct TPushXmlGrpDevInfo
{
	string devId;
	string devName;
	string devOldGbid;
	string devNewGbid;
	int    chnid;
	int    primary;
	string civilcode;
	string parentId;
};

struct TPushXmlTreeInfo
{
	string treeid;
	string treever;
	vector<TPushXmlGrpInfo> grpinfo;
	vector<TPushXmlGrpDevInfo> grpdevinfo;
};

void TasReboot();

bool IsNtfReqType(u16 wMsgId);
bool IsNtfRspType(u16 wMsgId);
bool IsRspType(u16 wMsgId);
bool IsQryReqType(u16 wMsgId); // 分页查询请求消息类型(在query_task中被处理)

// 校验消息的合法性，合法返回true
bool ValidateMsg(CMessage * const pcMsg);

inline void CopySeqnoAndSsnid(const CEventReq &cReq, CEventRsp &cRsp)
{
    cRsp.SetSeqNum(cReq.GetSeqNum());
    cRsp.SetSession(cReq.GetSession());
}

inline string GetSipMsgSessionId(const CMessage* pcMsg)
{
    return GetSipSessionID(reinterpret_cast<COspSipMsg*>(pcMsg->content)->GetMsgBody());
}

bool RelaceSessionId(string &strMsgContent, const string &strNewSessId);

// 内部自动添加换行符
void TelePrint(const s8* cstrFormat, ...);

/* utf8字符串大小写转换 */
void MakeLower( string& str );
void MakeUper( string& str );
void _replace_str(string& str, const string& strSrc, const string& strDst);


#define SAFE_DELETE(ptr) {if(ptr){delete ptr;ptr=NULL;}}
#define SAFE_A_DELETE(ptr) {if(ptr){delete [] ptr;ptr=NULL;}}

//浮点数比较相关的宏
//默认精度
#define EPS_MIN		(1e-5)
//比较两个浮点数是否相等（带精度
#define FLOAT_EQU_P(f1, f2, p) (((f1)-(f2)<=p)&&((f2)-(f1)<=p))
//比较两个浮点数是否相等，使用默认精度
#define FLOAT_EQU(f1, f2)  FLOAT_EQU_P(f1,f2, EPS_MIN)

// 查找cInput中strParentDomainId的下级domainId列表，返回结果中的域关系纪录是排序好的。按照域层级排列，父级排列在子级前面。
void FindSubdomainList(const vector<CDomainRelationInfo> &cInput, const string &strParentDomainId, vector<CDomainRelationInfo> &cOutput);
bool IsDomainIdExists(const vector<CDomainRelationInfo> &cInput, const string &strDomainId);

bool ValidateDomainRelationInfos(const vector<CDomainRelationInfo> &cDomainRelations);
bool ValidateDomainRelationInfo4Add(const vector<CDomainRelationInfo> &cOrigin, const CDomainRelationInfo &tChangeRelation);
bool ValidateDomainRelationInfo4Mod(const vector<CDomainRelationInfo> &cOrigin, const CDomainRelationInfo &tChangeRelation);

/* 编码设备能力划归时，自动补全编码通道信息 */
void GenerateEncodeCapPrivilegeInfo(const CDeviceModelInfo &cModelInfo, const vector<TDeviceCapPrivilege> &cPrivIn, vector<TDeviceCapPrivilege> &cPrivOut);
void GenerateEncodeCapIndexInfo(const CDeviceModelInfo &cModelInfo, const vector<TDeviceCapIndexs> &cCapsIn, vector<TDeviceCapIndexs> &cCapsOut);
void AddEncodeIndexInfo(const CDeviceModelInfo &cModelInfo, vector<TDeviceCapIndexs> &cIndexs);
void AddEncodePrivilegeInfo(const CDeviceModelInfo &cModelInfo, vector<TDeviceCapPrivilege> &cPri);

bool IsAllDigit(const string& str);
string CutDevGrpGbId(const string &strOrgName);
bool  GetDirFileList(char *dir,vector<string>& vecFileNameList);
string GetMachineTypeByPUMaxNum(int dwPuMaxNum );
bool WriteTableDataToPushXmlFile(const string& strTreeId,const TPushXmlTreeInfo& tPushXmlTreeInfo);


#endif // _INNER_UTIL_H_
