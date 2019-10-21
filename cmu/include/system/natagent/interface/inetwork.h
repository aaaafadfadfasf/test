#ifndef _I_NAT_NETWORK_H_
#define _I_NAT_NETWORK_H_

#include "kdvtype.h"

namespace NatAgent{namespace Types{
    class CPunchAddrs;
}}

namespace NatAgent{ namespace Interface{
    //
    /**
    *  �����, ����Ҫʵ���հ�����, �Լ��˿ں�socket�Ĺ���
    */
    class INetwork
    {
    public:
        //
        /**
        *  �������µĴ�ʱ, �һ���ô˺���
        */
        virtual void RegPunch(const Types::CPunchAddrs& punchAddrs) = 0;
        
        /**
        *  �����뷢��ʱ, �һ���ô˺���.
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

