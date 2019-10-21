#ifndef _COMMON_H_
#define _COMMON_H_
#include "cms/ospext/ospext.h"
#include "cms/ospsip/ospsip.h"

class CCallBackTask : public CSipTask 
{
public:
    CCallBackTask(CInstExt *pcInst, const string &strCuSession, const string &strPeUri);
    virtual ~CCallBackTask();
public:
    //状态机触发入口
    virtual u32 StartSendReq() = 0;
    TSipURI m_tPeUri;      //外设模块的uri
    string m_strCuSession;   //cmu分配给cu的session 
};

bool IsNormalNotOpenPeEevent(u16 wEvent);

#endif // _COMMON_H_
