#ifndef _NAT_TYPE_H_
#define _NAT_TYPE_H_
#include "kdvtype.h"
namespace SYS{namespace Types{namespace NatType{
// Define enum with different types of NAT 
enum EmNatType
{
    NatTypeUnknown=0,//δ֪
    NatTypeFailure,//ʧ��
    NatTypeOpen,//�ڹ���
    NatTypeBlocked,//���������ͨ

    NatTypeFullConeNat,//׶��
    NatTypeIpAddrRestrictedNat,//ip��ַ������
    NatTypePortRestrictedNat,//port������
    NatTypeSymNat,//�Գ�NAT
    NatTypeFirewall,//�з���ǽ
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

