#ifndef _SECURITY_MODULE_PROTO_DEPENDENT_H
#define _SECURITY_MODULE_PROTO_DEPENDENT_H

#ifndef _NO_OSPEXT

#include "cms/ospext/osptool.h"
#include "cms/cms_struct.h"

#else

#include "kdvtype.h"
#include <string>
using std::string;
//OSP œ˚œ¢√Ë ˆ
#define OSP_ADD_EVENT_DESC(T) OspExtAddEventDescAndID(#T, T)
u16          OspExtEventID(const char* szEventDesc);
const string OspExtEventDesc( u16 wEventID );
void         OspExtAddEventDescAndID(const char * desc, u16 event);

#ifndef CMS_CTRANSPARENTDATA
#define CMS_CTRANSPARENTDATA

class CTransparentData : public std::string
{
public:
    CTransparentData()
    {

    }
    CTransparentData(const string& cOther)
    {
        assign(cOther);
    }
    CTransparentData& operator=(const string& cOther)
    {
        if (this != &cOther)
        {
            assign(cOther);
        }

        return *this;
    }
};

#endif  //#ifndef CMS_CTRANSPARENTDATA

#include "cmsassert.h"
#include "cmstime.h"
#include "system/converthelper/toxml.h"

#endif // _NO_OSPEXT

#endif // _SECURITY_MODULE_PROTO_DEPENDENT_H
