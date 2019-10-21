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
    *  Stun �ͻ���
    */
    class IStunClient
    {
    public:
        //
        /**
        *  @param nTimeoutMSeconds ��ʱʱ�����, ��ʱ����ط���, ���γ�ʱ����Ϊ��ĳ�ʱ��. 
        *  @param stunServerAddr stun�������ĵ�ַ
        *  @param logPrinter ��ӡ, ���ǻὫ��ӡ����� logPrinter
        */
        static IStunClient* Create(
            IStunClientResultCallback& stunClientResultCallback,
            const SYS::Types::CNetAddr& stunServerAddr,
            Log4KDM::Interface::ILogPrinter& logPrinter,
            s32 nTimeoutMSeconds);
        
        static void Destory(IStunClient*& pStunClient);
        
        /**
        *  ��ȡNat����, �ڲ���ʹ��һ������˿��������ͨ��.
        *  
        *  @param allLocalIps ���еı���ip, ������ 0 �� 127.0.0.1
        */
        virtual void QueryNatType(const std::vector<u32>& allLocalIps) = 0;
        
        /**
        *  ��ȡĳ������ַ��Ӧ��������ַ 
        *  
        *  @param localAddr ������ַ
        *  ip������0, ����㲻֪���Լ�������ip
        *  �˿ڲ�������0, ��0����������˿�
        */
        virtual void QueryMappedAddr(const SYS::Types::CNetAddr& localAddr) = 0;
    protected:
        IStunClient(){}
        virtual ~IStunClient(){}
    };
    
    
}}
#endif

//end of file

