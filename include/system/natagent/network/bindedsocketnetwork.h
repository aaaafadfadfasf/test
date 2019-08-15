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
    *  绑定socket进行 打洞, 打洞成功后 close socket
    *  本类维护一个线程, 供 libevent 使用
    */
    class CBindedSocketNetwork : public Interface::INetwork,
        public LibEvent::Concurrency::IRunnable,
        public Interface::IPunchResultCallback
    {
    public:
        //
        /**
        *  @param nTimeoutMSeconds 在一个socket上接收超时的时间, 超时后会重发消息, 目前是写死3次 Timeout 为真正的超时.
        *  @param resultCallback 接收探测结果
        *  @param localIps 你本机的所有IP, 不包括 127.0.0.1
        */
        CBindedSocketNetwork(
            Interface::IPunchResultCallback& resultCallback,
            const Types::CLocalIpTable& localIps,
            s32 nTimeoutMSeconds);
        
        virtual ~CBindedSocketNetwork();
        
        /**
        *  设置INatPuncher, 本对象有udp消息或超时消息时会回调 pPuncher
        */
        void SetNatPuncher(Interface::INatPuncher* pPuncher);
        
    protected:
        //
        /**
        *  实现基类的函数, 对外发包
        */
        virtual void SendPacket( const Types::CPunchAddrs& punchAddrs, 
            const u8* pDataBuf, u16 wLength );
        //
        /**
        *  实现基类的函数, 注册以便在该地址上收包.
        *
        *  @param punchAddrs, 注册的地址.
        */
        virtual void RegPunch( const Types::CPunchAddrs& punchAddrs );    
        
        /**
        *  实现 IRunnable, 本对象维护一个线程.
        */
        virtual void DealEvent( const LibEvent::Concurrency::CQueueMsg& msg );
        
        /**
        *  本对象实现 IPunchResultCallback 是为了在成功的时候 closesocket
        */
        virtual void OnRecvPunchResult( EmPunchResult emResult, 
            const SYS::Types::CSizableBuffer& punchKey,
            const Types::CPunchAddrs& punchAddrs,
            const Types::CRemoteAddrsTable& remoteAddrs);    
    protected:    
        typedef std::map<SYS::Types::CNetAddr,
            LibEvent::Network::CLibEventCookie*> LocalAddrSockets;
        
        // event 结构体不能删除
        // 可能删除event后, 还会有timeout事件
        typedef std::list<LibEvent::Network::CLibEventCookie*> EventPool;
        
        /**
        *  接收从 libevent 发过来的UDP数据
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

