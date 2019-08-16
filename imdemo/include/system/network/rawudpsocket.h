// RawSocket.h: interface for the CRawUdpSocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RAWSOCKET_H__3C926DE1_A076_404D_84A4_C11E7041051F__INCLUDED_)
#define AFX_RAWSOCKET_H__3C926DE1_A076_404D_84A4_C11E7041051F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <sstream>
#include "kdvtype.h"
#include "system/config/osconfig.h"
#include "system/types/basictypes.h"
#include "system/network/rawudpsocket.h"
#include "system/network/udp.h"
#include "log4kdm/interface/ilogprinter.h"

#ifdef WIN32
//#include <Winsock2.h>
#include <ws2tcpip.h> 
#endif


namespace SYS{namespace Network{
    //
    /*------------------------------------------------------
    *Define IP header
    *-----------------------------------------------------*/
    typedef struct ip_hdr
    {
        unsigned char  ip_verlen;        // IP version & length
        unsigned char  ip_tos;           // IP type of service
        unsigned short ip_totallength;   // Total length
        unsigned short ip_id;            // Unique identifier 
        unsigned short ip_offset;        // Fragment offset field
        unsigned char  ip_ttl;           // Time to live
        unsigned char  ip_protocol;      // Protocol(TCP,UDP etc)
        unsigned short ip_checksum;      // IP checksum
        unsigned int   ip_srcaddr;       // Source address
        unsigned int   ip_destaddr;      // Destination address
    } IP_HDR, *PIP_HDR;
    
    //
    // Define UDP header 
    //
    typedef struct udp_hdr
    {
        unsigned short src_portno;       // Source port number
        unsigned short dst_portno;       // Destination port number
        unsigned short udp_length;       // UDP packet length
        unsigned short udp_checksum;     // UDP checksum (optional)
    } UDP_HDR, *PUDP_HDR;
    
    
    class CRawUdpSocket
    {
    public:
        ~CRawUdpSocket();
        CRawUdpSocket(Log4KDM::Interface::ILogPrinter& logPrinter);
        
    public:
        //创建原始SOCKET
        BOOL32 Create();
        
        //判断该对象是否有效
        BOOL32 IsValidSocket()const;;
        
        /**
        *  发送数据. buff的总大小为4096. 你的数据的大小我估计 3500 就差不多了.
        */
        u32  SendTo(
            u32 dwLocalIpAddr, u16 wLocalPort,
            u32 dwRemoteIpAddr, u16 wRemotePort, 
            const u8 *pbyBuf, u16 wBufLen);
        
    private:
        u16 checksum(u16 *pwbuffer, u32 size);
        
    protected:
        Socket    m_hSocket;       //socket
        
        //组包时使用
        IP_HDR       m_tipHdr;
        UDP_HDR      m_tudpHdr;
        u8           m_rawBuf[4096];
        sockaddr_in  m_tAddrIn;
        Log4KDM::Interface::ILogPrinter& m_logPrinter;
    };
    
}}


namespace SYS{namespace Network{
    
    inline CRawUdpSocket::CRawUdpSocket(Log4KDM::Interface::ILogPrinter& logPrinter): m_logPrinter(logPrinter)
    {
        m_hSocket = INVALID_SOCKET;
    }
    
    inline CRawUdpSocket::~CRawUdpSocket()
    {
        if( m_hSocket != INVALID_SOCKET )
        {
            closesocket( m_hSocket );
        }
    }
    
    inline BOOL32 CRawUdpSocket::Create()
    {
        m_hSocket = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
        if( m_hSocket == INVALID_SOCKET )
        {
            return FALSE;
        }
        
        // Enable the IP header include option 
        BOOL32 bOpt = TRUE;
        int nRet = setsockopt(m_hSocket, IPPROTO_IP, IP_HDRINCL, (char *)&bOpt, sizeof(bOpt));
        if (nRet == SOCKET_ERROR)
        {
            closesocket( m_hSocket );
            return FALSE;
        }
        return TRUE;
    }
    
    
    inline u16 CRawUdpSocket::checksum(u16 *pwbuffer, u32 size)
    {
        unsigned long cksum=0;
        
        while (size > 1)
        {
            cksum += *pwbuffer++;
            size  -= sizeof(u16);   
        }
        if (size)
        {
            cksum += *(u8*)pwbuffer;   
        }
        cksum = (cksum >> 16) + (cksum & 0xffff);
        cksum += (cksum >>16); 
        
        return (u16)(~cksum); 
    }
    
    inline u32 CRawUdpSocket::SendTo(u32 dwLocalIpAddr, u16 wLocalPort, 
        u32 dwRemoteIpAddr, u16 wRemotePort, 
        const u8 *pbyBuf, u16 wBufLen)
    {
        
        int iIPVersion = 4;    
        int iIPSize = sizeof(m_tipHdr) / sizeof(unsigned long);
        int iUdpSize = sizeof(m_tudpHdr) + wBufLen;
        int iTotalSize = sizeof(m_tipHdr) + sizeof(m_tudpHdr) + wBufLen;
        
        // Initalize the IP header
        //
        // IP version goes in the high order 4 bits of ip_verlen. The
        // IP header length (in 32-bit words) goes in the lower 4 bits.
        //
        m_tipHdr.ip_verlen = (iIPVersion << 4) | iIPSize;
        m_tipHdr.ip_tos = 0;                         // IP type of service
        m_tipHdr.ip_totallength = htons(iTotalSize); // Total packet len
        m_tipHdr.ip_id = 0;                 // Unique identifier: set to 0
        m_tipHdr.ip_offset = 0;             // Fragment offset field
        m_tipHdr.ip_ttl = 128;              // Time to live
        m_tipHdr.ip_protocol = 0x11;        // Protocol(UDP) 
        m_tipHdr.ip_checksum = 0 ;          // IP checksum
        m_tipHdr.ip_srcaddr = dwLocalIpAddr;     // Source address ????
        m_tipHdr.ip_destaddr = dwRemoteIpAddr;      // Destination address
        
        // Initalize the UDP header
        //
        m_tudpHdr.src_portno = htons(wLocalPort) ;
        m_tudpHdr.dst_portno = htons(wRemotePort) ;
        m_tudpHdr.udp_length = htons(iUdpSize) ;
        m_tudpHdr.udp_checksum = 0 ;
        // 
        // Build the UDP pseudo-header for calculating the UDP checksum.
        // The pseudo-header consists of the 32-bit source IP address, 
        // the 32-bit destination IP address, a zero byte, the 8-bit
        // IP protocol field, the 16-bit UDP length, and the UDP
        // header itself along with its data (padded with a 0 if
        // the data is odd length).
        //
        
        u8* pRawBuf = m_rawBuf;
        int  iUdpChecksumSize = 0;
        
        memset(pRawBuf, 0 , sizeof(m_rawBuf));
        
        memcpy(pRawBuf, &m_tipHdr.ip_srcaddr,  sizeof(m_tipHdr.ip_srcaddr));  
        pRawBuf += sizeof(m_tipHdr.ip_srcaddr);
        iUdpChecksumSize += sizeof(m_tipHdr.ip_srcaddr);
        
        memcpy(pRawBuf, &m_tipHdr.ip_destaddr, sizeof(m_tipHdr.ip_destaddr)); 
        pRawBuf += sizeof(m_tipHdr.ip_destaddr);
        iUdpChecksumSize += sizeof(m_tipHdr.ip_destaddr);
        
        pRawBuf++;
        iUdpChecksumSize += 1;
        
        memcpy(pRawBuf, &m_tipHdr.ip_protocol, sizeof(m_tipHdr.ip_protocol)); 
        pRawBuf += sizeof(m_tipHdr.ip_protocol);
        iUdpChecksumSize += sizeof(m_tipHdr.ip_protocol);
        
        memcpy(pRawBuf, &m_tudpHdr.udp_length, sizeof(m_tudpHdr.udp_length)); 
        pRawBuf += sizeof(m_tudpHdr.udp_length);
        iUdpChecksumSize += sizeof(m_tudpHdr.udp_length);
        
        memcpy(pRawBuf, &m_tudpHdr, sizeof(m_tudpHdr)); 
        pRawBuf += sizeof(m_tudpHdr);
        iUdpChecksumSize += sizeof(m_tudpHdr);
        
        memcpy(pRawBuf, pbyBuf, wBufLen);
        iUdpChecksumSize += wBufLen;
        /*
        for(i = 0; i < strlen(strMessage); i++, ptr++)
        *ptr = strMessage[i];
        iUdpChecksumSize += strlen(strMessage);
        */
        
        u16 cksum = checksum((u16 *)&m_rawBuf, iUdpChecksumSize);
        m_tudpHdr.udp_checksum = cksum;
        
        //
        // Now assemble the IP and UDP headers along with the data
        //  so we can send it
        //    
        
        pRawBuf = m_rawBuf;
        memset(pRawBuf, 0 , sizeof(m_rawBuf));
        
        memcpy(pRawBuf, &m_tipHdr, sizeof(m_tipHdr));   
        pRawBuf += sizeof(m_tipHdr);
        memcpy(pRawBuf, &m_tudpHdr, sizeof(m_tudpHdr)); 
        pRawBuf += sizeof(m_tudpHdr);
        memcpy(pRawBuf, pbyBuf, wBufLen);
        
        // Apparently, this SOCKADDR_IN structure makes no difference.
        // Whatever we put as the destination IP addr in the IP header
        // is what goes. Specifying a different destination in remote
        // will be ignored.
        //
        
        memset(&m_tAddrIn, 0, sizeof(m_tAddrIn));
        m_tAddrIn.sin_family      = AF_INET; 
        m_tAddrIn.sin_addr.s_addr = dwRemoteIpAddr;
        m_tAddrIn.sin_port          = htons(wRemotePort);
        
        int nRet = SYS::Network::sendMessage(
            m_hSocket, 
            m_rawBuf, iTotalSize,
            dwRemoteIpAddr, wRemotePort,
            false);

        if (nRet == SOCKET_ERROR)
        {
            std::ostringstream oss;
            oss<<"[RAWSOCK]:Socket Send Error:"<<getErrno()<<std::endl;
            m_logPrinter.Log(Log4KDM::Interface::ILogPrinter::emError,oss.str());
        }
        
        return nRet;
}

inline BOOL32 CRawUdpSocket::IsValidSocket() const
{
    return m_hSocket != INVALID_SOCKET;
}
}}



#endif // !defined(AFX_RAWSOCKET_H__3C926DE1_A076_404D_84A4_C11E7041051F__INCLUDED_)



//end of file


