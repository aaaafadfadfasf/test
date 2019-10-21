#ifndef _PUNCH_ADDRS_H_
#define _PUNCH_ADDRS_H_

#include "system/types/netaddr.h"

namespace NatAgent{ namespace Types{
    //
    /**
    *  打洞的本机地址和对端地址.
    */
    class CPunchAddrs
    {
    public:
        CPunchAddrs(const SYS::Types::CNetAddr& localAddr,const SYS::Types::CNetAddr& remoteAddr);
        const SYS::Types::CNetAddr& GetLocalAddr()const;
        
        const SYS::Types::CNetAddr& GetRemoteAddr()const;
        
        void SetLocalAddr(const SYS::Types::CNetAddr& localAddr);
        
        void SetRemoteAddr(const SYS::Types::CNetAddr& remoteAddr);
        
        
        bool operator<(const CPunchAddrs& addrs)const;
        bool operator == (const CPunchAddrs& addrs)const;
        friend std::ostream& operator <<(std::ostream& output,const CPunchAddrs& addrs)
        {
            addrs.Print(output);
            return output;
        }
    protected:
        void Print(std::ostream& output)const 
        {
            output<<"local addr:"<<m_localAddr<<std::endl;
            output<<"remote addr:"<<m_remoteAddr<<std::endl;
        }
    private:
        SYS::Types::CNetAddr m_localAddr;
        SYS::Types::CNetAddr m_remoteAddr;
    };
    
}}

#endif 
// end of file

