#ifndef _INCLUDE_REDIS_FASTDB_EXT_
#define _INCLUDE_REDIS_FASTDB_EXT_
#include "cms/cms_struct.h"
#include <sstream>

class CFastDBInterface;

static std::string Addr2Str( const TNetAddr &tAddr )
{
    std::stringstream strm;
    strm << tAddr.GetNetIp();
    strm << ":";
    strm << tAddr.GetNetPort();
    return strm.str();
}

struct THostDocNatInfo
{
public:
    THostDocNatInfo()
    {
        bEnabled = false;
    }
public:
    string GetInUriStr() const
    {
        return Addr2Str( inNatAddr );
    }
    string GetOutUriStr() const
    {
        return Addr2Str( outNatAddr );
    }

public:
    bool bEnabled;
    TNetAddr inNatAddr;
    TNetAddr outNatAddr;
};



class CFastDbServiceExt
{
public:
    CFastDbServiceExt(void);
    ~CFastDbServiceExt(void);

public:
    bool GetRecordServerUri( THostDocNatInfo &tHostDocInfo ) const;
    bool ValidateDb() const;
    void AttachFastDb( CFastDBInterface *pcFastDb )
    {
        m_pcFastDb = pcFastDb;
    }

private:
    CFastDBInterface *m_pcFastDb;
};

#endif //_INCLUDE_REDIS_FASTDB_EXT_