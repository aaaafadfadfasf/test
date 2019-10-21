#ifndef _NAT_TYPE_H_
#define _NAT_TYPE_H_
#include "kdvtype.h"
namespace SYS{namespace Types{namespace NatType{
// Define enum with different types of NAT 
enum EmNatType
{
    NatTypeUnknown=0,//未知
    NatTypeFailure,//失败
    NatTypeOpen,//在公网
    NatTypeBlocked,//与服务器不通

    NatTypeFullConeNat,//锥形
    NatTypeIpAddrRestrictedNat,//ip地址限制形
    NatTypePortRestrictedNat,//port限制形
    NatTypeSymNat,//对称NAT
    NatTypeFirewall,//有防火墙
};

static inline const s8* GetDebugString(EmNatType emType)
{
    switch(emType)
    {
    case NatTypeUnknown:
        return "NatTypeUnknown";        
    case NatTypeFailure:
        return "NatTypeFailure";
    case NatTypeOpen:
        return "NatTypeOpen";
    case NatTypeBlocked:
        return "NatTypeBlocked";
    case NatTypeFullConeNat:
        return "NatTypeFullConeNat";
    case NatTypeIpAddrRestrictedNat:
        return "NatTypeIpAddrRestrictedNat";
    case NatTypePortRestrictedNat:
        return "NatTypePortRestrictedNat";
    case NatTypeSymNat:
        return "NatTypeSymNat";
    case NatTypeFirewall:
        return "NatTypeFirewall";
    default:
        return "unknow";
    }
    return "unknow";
}
}}}
#endif

//end of file

