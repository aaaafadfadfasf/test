#ifndef _REMOTE_ADDRS_TABLE_H_
#define _REMOTE_ADDRS_TABLE_H_

#include <vector>
#include "system/types/netaddr.h"

namespace NatAgent{namespace Types{
    //
    /**
    *  对端地址, 打洞时会优先内网地址.
    */
    class CRemoteAddrsTable
    {
    public:
        typedef std::vector<SYS::Types::CNetAddr> RemoteAddrs;
        typedef RemoteAddrs::const_iterator const_iterator;
        /**
        *  增加外网地址
        */
        void AddOutterAddr(const SYS::Types::CNetAddr& addr);
        /**
        *  增加内网地址
        */
        void AddInnerAddr(const SYS::Types::CNetAddr& addr);
        
        const_iterator begin()const;
        const_iterator end()const;
        
        size_t size()const;
        void clear();
        const SYS::Types::CNetAddr& operator[](size_t nIndex)const;
    private:    
        RemoteAddrs m_remoteAddrs;//inner ip + outter ip, inner ip在前面
    };
    
}}

#endif

//end of file

