#ifndef _DATA_SWITCH_NETWORK_FOR_NATAGENT_H_
#define _DATA_SWITCH_NETWORK_FOR_NATAGENT_H_

#ifdef _WIN32
#pragma warning (disable : 4786) 
#pragma warning (disable : 4503)
#endif
#include <map>
#include <set>

#include "natagent/interface/inetwork.h"
#include "natagent/interface/ipunchresultcallback.h"
#include "libeventex/concurrency/threaddefine.h"
#include "types/netaddr.h"
#include "cbb/dataswitch/dataswitch.h"


namespace Log4KDM{namespace Interface{
    class ILogPrinter;
}}


namespace NatAgent{namespace Interface{
    class INatPuncher;
}}

namespace SYS{namespace Network{
    class CRawUdpSocket;
}}

namespace NatAgent{namespace Network{
    
    class CDataswitchNetwork : public Interface::INetwork
    {
    public:
        static CDataswitchNetwork& GetInstance();
        
        void Init(
            DSID dwDSID,
            Interface::INatPuncher* pNatPuncher,
            Log4KDM::Interface::ILogPrinter* pLogPrinter
            );
        
        //
        /**
        *  为了支持回调多个函数.
        *  Filter 被本对象设置了, 如果外部还想设的话就调用此函数
        */
        void AddFilterToDS(u32 dwLocalIP, u16 wLocalPort, 
            FilterFunc ptFunc);
        
        void RemoveOneFilter(u32 dwLocalIP,u16 wLocalPort,
            FilterFunc ptFunc);
        
        void RemoveAllFilter(u32 dwLocalIP,u16 wLocalPort);
        
        void RemoteAll();

        /**
         *  这个函数需要使用者自己判断是否要调用.
         *  因为一个 ip+port 上可能有多个 key 在打洞.
         */
        void UnRegPunch(u32 dwLocalIP,u16 wLocalPort);
        
        ~CDataswitchNetwork();

    protected:
        void AddFilterToDS_NoLock(u32 dwLocalIP, u16 wLocalPort, 
            FilterFunc ptFunc);
        
        void RemoveOneFilter_NoLock(u32 dwLocalIP,u16 wLocalPort,
            FilterFunc ptFunc);
        
        void RemoveAllFilter_NoLock(u32 dwLocalIP,u16 wLocalPort);
        
        void RemoteAll_NoLock();
        
    protected:
        CDataswitchNetwork();        
        CDataswitchNetwork(const CDataswitchNetwork&);
        CDataswitchNetwork& operator=(const CDataswitchNetwork&);
        
    protected:    
        virtual void SendPacket( const Types::CPunchAddrs& punchAddrs,
            const u8* pDataBuf, u16 wLength );
        
        virtual void RegPunch( const Types::CPunchAddrs& punchAddrs );
        
    protected:
        static u32 DSNetworkFilterFunc(
            u32 dwRecvIP, u16 wRecvPort, 
            u32 dwSrcIP, u16 wSrcPort, 
            u8* pData, u32 uLen);
        
        static u32 DSFilterForNatAgent( u32 dwLocalIP, u16 wLocalPort, 
            u32 dwRemoteIP, u16 wRemotePort, 
            u8* pData, u32 uLen );
        
    protected:
        DSID m_dwDSID;
        Log4KDM::Interface::ILogPrinter* m_pLogPrinter;
        SYS::Network::CRawUdpSocket* m_pSocket4Send;
        
        typedef std::map<SYS::Types::CNetAddr,std::set<FilterFunc> > FilterTable;
        FilterTable m_filters;
        
        Interface::INatPuncher* m_pNatPuncher;
        
        libevent_mutex_t m_lock;
    };    
}}


#endif

// end of file

