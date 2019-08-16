#ifndef _BINDED_SOCKET_NETWORK_H_
#define _BINDED_SOCKET_NETWORK_H_

#ifdef _WIN32
#pragma warning (disable : 4786) 
#endif
#include <iostream>
#include <map>
#include <list>

#include "system/types/basictypes.h"
#include "system/types/netaddr.h"

#include "../interface/ipunchresultcallback.h"
#include "../interface/inetwork.h"
#include "../types/localiptable.h"
#include "system/libeventex/concurrency/irunnable.h"


namespace LibEvent
{
    namespace Concurrency
    {
        class CThreadRunner;
        class CQueueMsg;
    }
    
    namespace Network
    {
        class CLibEventCookie;
    }
}

namespace NatAgent{namespace Interface{
    class INetwork;
    class IPunchResultCallback;
    class INatPuncher;
}}

namespace NatAgent{namespace Types{
    class CPunchAddrs;
    class CRemoteAddrsTable;
    class CLocalIpTable;
}}

namespace SYS{namespace Types{
    class CSizableBuffer;
}}

namespace NatAgent{ namespace Network{
    
    //
    /**
    *  ��socket���� ��, �򶴳ɹ��� close socket
    *  ����ά��һ���߳�, �� libevent ʹ��
    */
    class CBindedSocketNetwork : public Interface::INetwork,
        public LibEvent::Concurrency::IRunnable,
        public Interface::IPunchResultCallback
    {
    public:
        //
        /**
        *  @param nTimeoutMSeconds ��һ��socket�Ͻ��ճ�ʱ��ʱ��, ��ʱ����ط���Ϣ, Ŀǰ��д��3�� Timeout Ϊ�����ĳ�ʱ.
        *  @param resultCallback ����̽����
        *  @param localIps �㱾��������IP, ������ 127.0.0.1
        */
        CBindedSocketNetwork(
            Interface::IPunchResultCallback& resultCallback,
            const Types::CLocalIpTable& localIps,
            s32 nTimeoutMSeconds);
        
        virtual ~CBindedSocketNetwork();
        
        /**
        *  ����INatPuncher, ��������udp��Ϣ��ʱ��Ϣʱ��ص� pPuncher
        */
        void SetNatPuncher(Interface::INatPuncher* pPuncher);
        
    protected:
        //
        /**
        *  ʵ�ֻ���ĺ���, ���ⷢ��
        */
        virtual void SendPacket( const Types::CPunchAddrs& punchAddrs, 
            const u8* pDataBuf, u16 wLength );
        //
        /**
        *  ʵ�ֻ���ĺ���, ע���Ա��ڸõ�ַ���հ�.
        *
        *  @param punchAddrs, ע��ĵ�ַ.
        */
        virtual void RegPunch( const Types::CPunchAddrs& punchAddrs );    
        
        /**
        *  ʵ�� IRunnable, ������ά��һ���߳�.
        */
        virtual void DealEvent( const LibEvent::Concurrency::CQueueMsg& msg );
        
        /**
        *  ������ʵ�� IPunchResultCallback ��Ϊ���ڳɹ���ʱ�� closesocket
        */
        virtual void OnRecvPunchResult( EmPunchResult emResult, 
            const SYS::Types::CSizableBuffer& punchKey,
            const Types::CPunchAddrs& punchAddrs,
            const Types::CRemoteAddrsTable& remoteAddrs);    
    protected:    
        typedef std::map<SYS::Types::CNetAddr,
            LibEvent::Network::CLibEventCookie*> LocalAddrSockets;
        
        // event �ṹ�岻��ɾ��
        // ����ɾ��event��, ������timeout�¼�
        typedef std::list<LibEvent::Network::CLibEventCookie*> EventPool;
        
        /**
        *  ���մ� libevent ��������UDP����
        */
        static void OnUdpSocketEvent(int fd, short flags, void *arg);
        
        static void AddToLibEvent( LibEvent::Network::CLibEventCookie &cookie,
            CBindedSocketNetwork* pThis );
        
        void DoDealRegPunchAddr(const LibEvent::Concurrency::CQueueMsg& msg);
        
        void DoDealPunchOK(const LibEvent::Concurrency::CQueueMsg& msg);
        
        LibEvent::Network::CLibEventCookie* GenerateCookieFor(Socket sock);
        
        LibEvent::Network::CLibEventCookie* AssignCookieFor( 
            const Types::CPunchAddrs* pAddrs );
        
        void DoSendPacket(const LibEvent::Concurrency::CQueueMsg& msg);    
        
    private:
        Interface::IPunchResultCallback& m_resultCallback;
        LocalAddrSockets m_localAddrSockets;
        EventPool m_eventPool;
        LibEvent::Concurrency::CThreadRunner* m_pThreadRunner;
        Interface::INatPuncher* m_pNatPuncher;
        timeval m_timeoutTime;
        Types::CLocalIpTable m_localIps;
    };
    
}}

#endif
//end of file

