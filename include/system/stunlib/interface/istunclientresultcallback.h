#ifndef _I_STUN_CLIENT_RESULT_CALLBACK_H_
#define _I_STUN_CLIENT_RESULT_CALLBACK_H_

#include "system/types/nattype.h"

namespace SYS{namespace Types{
    class CNetAddr;
}}

namespace StunLib{namespace Interface{
    
    class IStunClientResultCallback
    {
    public:
        enum EmNatError
        {
            NatErrorUnableOpenPort,// 打开本地端口失败
                NatErrorTimeout,
                NatErrorUnableOpenRandomPortToQueryNatType,
        };
        
        IStunClientResultCallback(){}
        virtual ~IStunClientResultCallback(){}
        
        virtual void OnGetMappedAddr(const SYS::Types::CNetAddr& innerAddr,
            const SYS::Types::CNetAddr& mappedAddr) = 0;
        
        virtual void OnError(const SYS::Types::CNetAddr& localAddr,EmNatError natError) = 0;
        
        virtual void OnGetNatType(SYS::Types::NatType::EmNatType natType) = 0;
    };
    
}}

#endif

//end of file

