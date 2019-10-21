#ifndef _INCLUDE_USER_STREAM_NUM_RESTORER_H_
#define _INCLUDE_USER_STREAM_NUM_RESTORER_H_

#include <string>
#include "cbb/osp/osp.h"

class CInstExt;
class CUserAccount;

class CUserStreamNumRestorer
{
public:
    CUserStreamNumRestorer(CInstExt *pcInst );
    virtual ~CUserStreamNumRestorer();
public:
    void SetUserLoginSession( const std::string &strSession );
    bool Start();
    void Stop();
private:
    CUserAccount* FindUserAccount();

private:
    std::string m_strUserSession;
    CInstExt    *m_pcInst;
    bool        m_bStarted;
};

#endif //_INCLUDE_USER_STREAM_NUM_RESTORER_H_