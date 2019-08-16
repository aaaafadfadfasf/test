#ifndef udp_h
#define udp_h

#include <cassert>
#include <cstdio>
#include <cstring>
#include <errno.h>
#include <iostream>
#include <cstdlib>
#include <time.h>
#include <string.h>

#include "system/config/osconfig.h"
#include "system/types/basictypes.h"

namespace SYS{ namespace Network{ 
    
    /// Open a UDP socket to receive on the given port - if port is 0, pick a a
    /// port, if interfaceIp!=0 then use ONLY the interface specified instead of
    /// all of them  
    static inline Socket openPort( unsigned short port, unsigned int interfaceIp, bool verbose )
    {
        Socket fd;
        
        fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if ( fd == INVALID_SOCKET )
        {
            int err = getErrno();
            std::cerr << "Could not create a UDP socket:" << err << std::endl;
            return INVALID_SOCKET;
        }
        
        struct sockaddr_in addr;
        memset((char*) &(addr),0, sizeof((addr)));
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port = htons(port);
        
        if ( (interfaceIp != 0) && 
            ( interfaceIp != 0x100007f ) )
        {
            addr.sin_addr.s_addr = interfaceIp;
            if (verbose )
            {
                std::clog << "Binding to interface " 
                    << std::hex << "0x" << interfaceIp << std::dec << std::endl;
            }
        }
        
        if ( bind( fd,(struct sockaddr*)&addr, sizeof(addr)) != 0 )
        {
            int e = getErrno();
            
            switch (e)
            {
            case 0:
                {
                    std::cerr << "Could not bind socket" << std::endl;
                    return INVALID_SOCKET;
                }
            case EADDRINUSE:
                {
                    std::cerr << "Port " << port << " for receiving UDP is in use" << std::endl;
                    return INVALID_SOCKET;
                }
                break;
            case EADDRNOTAVAIL:
                {
                    if ( verbose ) 
                    {
                        std::cerr << "Cannot assign requested address" << std::endl;
                    }
                    return INVALID_SOCKET;
                }
                break;
            default:
                {
                    std::cerr << "Could not bind UDP receive port"
                        << " Error=" << e << " " << strerror(e) << std::endl;
                    return INVALID_SOCKET;
                }
                break;
            }
        }
        if ( verbose )
        {
            std::clog << "Opened port " << port << " with fd " << fd << std::endl;
        }
        
        assert( fd != INVALID_SOCKET  );
        
        return fd;
    }
    
    /// recive a UDP message 
    static inline bool getMessage( Socket fd, unsigned char* buf, int* len,
        unsigned int* srcIp, unsigned short* srcPort,
        bool verbose)
    {
        assert( fd != INVALID_SOCKET );
        
        int originalSize = *len;
        assert( originalSize > 0 );
        
        struct sockaddr_in from;
        int fromLen = sizeof(from);
        
        *len = recvfrom(fd,
            reinterpret_cast<BufType*>(buf),
            originalSize,
            0,
            (struct sockaddr *)&from,
            (socklen_t*)&fromLen);
        
        if ( *len == SOCKET_ERROR )
        {
            int err = getErrno();
            
            switch (err)
            {
            case ENOTSOCK:
                std::cerr << "Error fd not a socket" <<   std::endl;
                break;
            case ECONNRESET:
                std::cerr << "Error connection reset - host not reachable" <<   std::endl;
                break;
                
            default:
                std::cerr << "Socket Error=" << err << std::endl;
            }
            
            return false;
        }
        
        if ( *len < 0 )
        {
            std::clog << "socket closed? negative len" << std::endl;
            return false;
        }
        
        if ( *len == 0 )
        {
            std::clog << "socket closed? zero len" << std::endl;
            return false;
        }
        
        *srcPort = ntohs(from.sin_port);
        *srcIp = ntohl(from.sin_addr.s_addr);
        
        if ( (*len)+1 >= originalSize )
        {
            if (verbose)
            {
                std::clog << "Received a message that was too large" << std::endl;
            }
            return false;
        }
        buf[*len]=0;
        
        return true;
    }
    
    /// send a UDP message 
    static inline bool sendMessage( Socket fd, const unsigned char* buf, int l, 
        unsigned int dstIp, unsigned short dstPort,
        bool verbose)
    {
        assert( fd != INVALID_SOCKET );
        
        int s;
        if ( dstPort == 0 )
        {
            // sending on a connected port 
            assert( dstIp == 0 );
            
            s = send(fd,reinterpret_cast<const BufType*>(buf),l,0);
        }
        else
        {
            assert( dstIp != 0 );
            assert( dstPort != 0 );
            
            struct sockaddr_in to;
            int toLen = sizeof(to);
            memset(&to,0,toLen);
            
            to.sin_family = AF_INET;
            to.sin_port = htons(dstPort);
            to.sin_addr.s_addr = dstIp;
            
            s = sendto(fd, reinterpret_cast<const BufType*>(buf), l, 0,(sockaddr*)&to, toLen);
        }
        
        if ( s == SOCKET_ERROR )
        {
            int e = getErrno();
            switch (e)
            {
            case ECONNREFUSED:
            case EHOSTDOWN:
            case EHOSTUNREACH:
                {
                    // quietly ignore this 
                }
                break;
            case EAFNOSUPPORT:
                {
                    std::cerr << "err EAFNOSUPPORT in send" << std::endl;
                }
                break;
            default:
                {
                    std::cerr << "err " << e << " "  << strerror(e) << " in send" << std::endl;
                }
            }
            return false;
        }
        
        if ( s == 0 )
        {
            std::cerr << "no data sent in send" << std::endl;
            return false;
        }
        
        if ( s != l )
        {
            if (verbose)
            {
                std::cerr << "only " << s << " out of " << l << " bytes sent" << std::endl;
            }
            return false;
        }
        
        return true;
    }
    
    /// set up network - does nothing in unix but needed for windows
    static inline void initNetwork()
    {
#ifdef WIN32 
        WORD wVersionRequested = MAKEWORD( 2, 2 );
        WSADATA wsaData;
        int err;
        
        err = WSAStartup( wVersionRequested, &wsaData );
        if ( err != 0 ) 
        {
            // could not find a usable WinSock DLL
            std::cerr << "Could not load winsock" << std::endl;
            assert(0); // is this is failing, try a different version that 2.2, 1.0 or later will likely work 
            exit(1);
        }
        
        /* Confirm that the WinSock DLL supports 2.2.*/
        /* Note that if the DLL supports versions greater    */
        /* than 2.2 in addition to 2.2, it will still return */
        /* 2.2 in wVersion since that is the version we      */
        /* requested.                                        */
        
        if ( LOBYTE( wsaData.wVersion ) != 2 ||
            HIBYTE( wsaData.wVersion ) != 2 ) 
        {
            /* Tell the user that we could not find a usable */
            /* WinSock DLL.                                  */
            WSACleanup( );
            std::cerr << "Bad winsock verion" << std::endl;
            assert(0); // is this is failing, try a different version that 2.2, 1.0 or later will likely work 
            exit(1);
        }    
#endif
    }
    
}}


#endif
