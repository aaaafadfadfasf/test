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
    //״̬���������
    virtual u32 StartSendReq() = 0;
    TSipURI m_tPeUri;      //����ģ���uri
    string m_strCuSession;   //cmu�����cu��session 
};

bool IsNormalNotOpenPeEevent(u16 wEvent);

#endif // _COMMON_H_
