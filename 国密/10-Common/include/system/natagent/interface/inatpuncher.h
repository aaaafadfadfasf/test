#ifndef _I_NAT_PUNCHER_H_
#define _I_NAT_PUNCHER_H_

#include "kdvtype.h"
#include "../types/localiptable.h"

namespace NatAgent{namespace Types{
    class CPunchAddrs;    
    class CRemoteAddrsTable;
    class CLocalIpTable;
}}

namespace SYS{namespace Types{
    class CNetAddr;
    class CSizableBuffer;
}}

namespace Log4KDM{namespace Interface{
    class ILogPrinter;
}}

namespace NatAgent{namespace Interface{
    
    class IPunchResultCallback;
    class INetwork;
    
    /**
    *  NatAgent 接口.
    *  支持在多个端口同时打洞,
    *  也可以创建多个 NatPuncher, 但这通常没有必要
    */
    class INatPuncher
    {
    public:
        //
        /**
        *  创建一个 NatPuncher
        *
        *  @param allLocalIps 你的本机ip们, 不包括 127.0.0.1
        *  @param network 网络, DataSwitch or BindSocket, or Customize
        *  @param resultCallback 用于接收打洞结果
        *  @logPrinter 打印输出
        *  @nTimeoutMSecond 超时的毫秒数, 超时会重发, 超时3次以上为真正的超时, 为0时不做超时检查
        *  当你不能对socket进行收包超时检查时请设置该值, 由natagent内部进行timeout处理. 
        *  如 DataswitchNetwork\n
        *  当你在network模块中能知道超时时则不设置该值
        *  如 BindedSocketNetwork 能检查到超时, 并调用本接口中的 OnTimeOut 函数.
        */
        static INatPuncher* CreateNatPuncher(const Types::CLocalIpTable& allLocalIps,
            Interface::INetwork& network,
            IPunchResultCallback& resultCallback,
            Log4KDM::Interface::ILogPrinter& logPrinter,
            s32 nTimeoutMSecond);
        
        static void DestoryNatPuncher(INatPuncher*& pPuncher);
        
        /**
        *  开始打洞, 可以多次调用来以便在不同的端口上打洞
        *
        *  @param punchKey 打洞的key, 我们会将key在网络上传输. 全局要唯一.
        *  不一定是字符串, 可以是buffer, 可以有 '\0'
        */
        virtual void StartPunch(const SYS::Types::CSizableBuffer& punchKey,
            u16 wLocalPort,
            const Types::CRemoteAddrsTable& remoteAddrsTable) = 0;
        
        virtual void StopPunch(const SYS::Types::CSizableBuffer& punchKey) = 0;
        
        /**
        *  接收udp包, 你需要将 network 层面收到的包,喂给此函数.
        */
        virtual void OnRecvUdpPacket(const Types::CPunchAddrs& addrs,const u8* pBuf,u16 wLen) = 0;
        
        /**
        *  如果你从网络上收包时超时了, 请调用这个函数.
        *  DataswitchNetwork 目前不能知道收包情况, 则不会调用此函数,
        *  则需要在 CreateNatPuncher 时指定超时时间, 由 Natagent 内部做超时
        */
        virtual void OnTimeOut(const SYS::Types::CNetAddr& localAddr) = 0;
    protected:
        INatPuncher(){}
        virtual ~INatPuncher(){}
    };
    
}}

#endif

//end of file

