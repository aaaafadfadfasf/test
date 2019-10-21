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
    *  NatAgent �ӿ�.
    *  ֧���ڶ���˿�ͬʱ��,
    *  Ҳ���Դ������ NatPuncher, ����ͨ��û�б�Ҫ
    */
    class INatPuncher
    {
    public:
        //
        /**
        *  ����һ�� NatPuncher
        *
        *  @param allLocalIps ��ı���ip��, ������ 127.0.0.1
        *  @param network ����, DataSwitch or BindSocket, or Customize
        *  @param resultCallback ���ڽ��մ򶴽��
        *  @logPrinter ��ӡ���
        *  @nTimeoutMSecond ��ʱ�ĺ�����, ��ʱ���ط�, ��ʱ3������Ϊ�����ĳ�ʱ, Ϊ0ʱ������ʱ���
        *  ���㲻�ܶ�socket�����հ���ʱ���ʱ�����ø�ֵ, ��natagent�ڲ�����timeout����. 
        *  �� DataswitchNetwork\n
        *  ������networkģ������֪����ʱʱ�����ø�ֵ
        *  �� BindedSocketNetwork �ܼ�鵽��ʱ, �����ñ��ӿ��е� OnTimeOut ����.
        */
        static INatPuncher* CreateNatPuncher(const Types::CLocalIpTable& allLocalIps,
            Interface::INetwork& network,
            IPunchResultCallback& resultCallback,
            Log4KDM::Interface::ILogPrinter& logPrinter,
            s32 nTimeoutMSecond);
        
        static void DestoryNatPuncher(INatPuncher*& pPuncher);
        
        /**
        *  ��ʼ��, ���Զ�ε������Ա��ڲ�ͬ�Ķ˿��ϴ�
        *
        *  @param punchKey �򶴵�key, ���ǻὫkey�������ϴ���. ȫ��ҪΨһ.
        *  ��һ�����ַ���, ������buffer, ������ '\0'
        */
        virtual void StartPunch(const SYS::Types::CSizableBuffer& punchKey,
            u16 wLocalPort,
            const Types::CRemoteAddrsTable& remoteAddrsTable) = 0;
        
        virtual void StopPunch(const SYS::Types::CSizableBuffer& punchKey) = 0;
        
        /**
        *  ����udp��, ����Ҫ�� network �����յ��İ�,ι���˺���.
        */
        virtual void OnRecvUdpPacket(const Types::CPunchAddrs& addrs,const u8* pBuf,u16 wLen) = 0;
        
        /**
        *  �������������հ�ʱ��ʱ��, ������������.
        *  DataswitchNetwork Ŀǰ����֪���հ����, �򲻻���ô˺���,
        *  ����Ҫ�� CreateNatPuncher ʱָ����ʱʱ��, �� Natagent �ڲ�����ʱ
        */
        virtual void OnTimeOut(const SYS::Types::CNetAddr& localAddr) = 0;
    protected:
        INatPuncher(){}
        virtual ~INatPuncher(){}
    };
    
}}

#endif

//end of file

