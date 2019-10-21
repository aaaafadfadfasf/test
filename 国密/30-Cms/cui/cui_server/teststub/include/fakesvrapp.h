#ifndef _STUB_APP_H_
#define _STUB_APP_H_

#include "cms/ospext/ospext.h"
#include "cms/ospsip/ospsip.h"
#include "cms/utility/uuid.h"

void * HbThread(void *pParam);
string ReplaceSessid( const s8* cstrRawdata, const string &cstrNewSessid );
string ReplaceSeqno( const s8* cstrRawdata, u32 dwNewSeqno );
string ReplaceStr(const string &strXml, const string &strReplace, const s8* lpKeyBegin, const s8* lpEnd);

class CSrvInst : public CInstExt
{
public:
    enum
    {
        Service = 0,
    };
    CSrvInst();
    LPCSTR GetStrState() const;
    void DaemonEntry(CMessage* const pcMsg);
    void NormalEntry(CMessage* const pcMsg){}
    BOOL32 OnUserInnerCommonMsg(CMessage* const pcMsg);
    void PostRsp(TKDSIP_TRANS_ID tTransId, const string &strMsg);
};

typedef COspApp<CSrvInst, 1> CFakeSvrApp;

#endif

