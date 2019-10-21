#ifndef _GET_ADDR_BY_SOCKET_H_
#define _GET_ADDR_BY_SOCKET_H_

#include "system/config/osconfig.h"
#include "system/types/netaddr.h"

namespace SYS{namespace Network{
    
    static inline Types::CNetAddr GetAddrBySocket( Socket sock )
    {
        sockaddr_in addr;
        socklen_t len = sizeof(addr);
        getsockname(sock,reinterpret_cast<sockaddr*>(&addr),&len);
        
        return Types::CNetAddr(addr.sin_addr.s_addr,ntohs(addr.sin_port));
    }

}}

#endif

//end of file

