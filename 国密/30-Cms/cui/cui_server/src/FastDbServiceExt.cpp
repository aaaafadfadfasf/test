#include "FastDbServiceExt.h"
#include "sdk/fastdbsdk/fastdb.h"
#include "sdk/fastdbsdk/keydef.h"
#include "FastDbKeyExt.h"
#include <map>
#include <vector>

typedef std::map<std::string, std::string>  BASIC_KEY_VALUE_MAP;
typedef std::vector<std::string>            BASIC_FIELD_VECTOR;

bool Redis_To_Struct(IN const BASIC_KEY_VALUE_MAP &values, OUT THostDocNatInfo &data)
{
    BASIC_KEY_VALUE_MAP::const_iterator itrKeyVal = values.find( KEY_IS_USED );
    if( itrKeyVal == values.end() )
    {
        return false;
    }
    data.bEnabled = atoi( itrKeyVal->second.c_str() ) != 0;
    
    //in
    itrKeyVal = values.find( KEY_IN_NAT_IP );
    if( itrKeyVal == values.end() )
    {
        return false;
    }
    data.inNatAddr.SetNetIp( itrKeyVal->second );

    itrKeyVal = values.find( KEY_IN_NAT_PORT );
    if( itrKeyVal == values.end() )
    {
        return false;
    }
    data.inNatAddr.SetNetPort( atoi( itrKeyVal->second.c_str() ) );

    //out
    itrKeyVal = values.find( KEY_OUT_NAT_IP );
    if( itrKeyVal == values.end() )
    {
        return false;
    }
    data.outNatAddr.SetNetIp( itrKeyVal->second );

    itrKeyVal = values.find( KEY_OUT_NAT_PORT );
    if( itrKeyVal == values.end() )
    {
        return false;
    }
    data.outNatAddr.SetNetPort( atoi( itrKeyVal->second.c_str() ) );
    return true;
}



CFastDbServiceExt::CFastDbServiceExt(void)
{
    m_pcFastDb = NULL;
}

CFastDbServiceExt::~CFastDbServiceExt(void)
{

}


bool CFastDbServiceExt::ValidateDb() const
{
    return m_pcFastDb!= NULL && m_pcFastDb->IsConnected();
}


bool CFastDbServiceExt::GetRecordServerUri( THostDocNatInfo &tHostDocInfo ) const
{
    if( !ValidateDb() )
    {
        return false;
    }

    if( EC_OK != m_pcFastDb->IsKeyExists( HOST_DOCUMENT_SERVER_URI ) )
    {
        return false;
    }
    BASIC_FIELD_VECTOR cFields;
    cFields.push_back( KEY_IS_USED      );
    cFields.push_back( KEY_IN_NAT_IP    );
    cFields.push_back( KEY_IN_NAT_PORT  );
    cFields.push_back( KEY_OUT_NAT_IP   );
    cFields.push_back( KEY_OUT_NAT_PORT );

    BASIC_KEY_VALUE_MAP cValues;

    if( EC_OK != m_pcFastDb->HMGet( HOST_DOCUMENT_SERVER_URI, cFields, cValues ) )
    {
        return false;
    }

    return Redis_To_Struct( cValues, tHostDocInfo );
}
