#ifndef _I_STUN_CLIENT_H_
#define _I_STUN_CLIENT_H_

#include <vector>

namespace SYS{namespace Types{
    class CNetAddr;
}}

namespace Log4KDM{namespace Interface{
    class ILogPrinter;
}}


namespace StunLib{namespace Interface{
    
    class IStunClientResultCallback;
    
    /**
    *  Stun 客户端
    */
    class IStunClient
    {
    public:
        //
        /**
        *  @param nTimeoutMSeconds 超时时间毫秒, 超时后会重发包, 三次超时则认为真的超时了. 
        *  @param stunServerAddr stun服务器的地址
        *  @param logPrinter 打印, 我们会将打印输出到 logPrinter
        */
        static IStunClient* Create(
            IStunClientResultCallback& stunClientResultCallback,
            const SYS::Types::CNetAddr& stunServerAddr,
            Log4KDM::Interface::ILogPrinter& logPrinter,
            s32 nTimeoutMSeconds);
        
        static void Destory(IStunClient*& pStunClient);
        
        /**
        *  获取Nat类型, 内部会使用一个随机端口与服务器通信.
        *  
        *  @param allLocalIps 所有的本机ip, 不包括 0 和 127.0.0.1
        */
        virtual void QueryNatType(const std::vector<u32>& allLocalIps) = 0;
        
        /**
        *  获取某内网地址对应的外网地址 
        *  
        *  @param localAddr 内网地址
        *  ip可以填0, 如果你不知道自己的外网ip
        *  端口不建议填0, 填0将会是随机端口
        */
        virtual void QueryMappedAddr(const SYS::Types::CNetAddr& localAddr) = 0;
    protected:
        IStunClient(){}
        virtual ~IStunClient(){}
    };
    
    
}}
#endif

//end of file

