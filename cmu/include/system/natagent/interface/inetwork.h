#ifndef _I_NAT_NETWORK_H_
#define _I_NAT_NETWORK_H_

#include "kdvtype.h"

namespace NatAgent{namespace Types{
    class CPunchAddrs;
}}

namespace NatAgent{ namespace Interface{
    //
    /**
    *  网络层, 你需要实现收包发包, 以及端口和socket的管理
    */
    class INetwork
    {
    public:
        //
        /**
        *  当我有新的打洞时, 我会调用此函数
        */
        virtual void RegPunch(const Types::CPunchAddrs& punchAddrs) = 0;
        
        /**
        *  当我想发包时, 我会调用此函数.
        */
        virtual void SendPacket(const Types::CPunchAddrs& punchAddrs,
            const u8* pDataBuf,
            u16 wLength) = 0;
        
    protected:
        virtual ~INetwork(){}
    };
}}

#endif

//end of file

