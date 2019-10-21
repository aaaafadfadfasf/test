#ifndef _IPUNCH_RESULT_CALLBACK_H_
#define _IPUNCH_RESULT_CALLBACK_H_

#include <string>


namespace NatAgent{namespace Types{
    class CPunchAddrs;
    class CRemoteAddrsTable;
}}

namespace SYS{namespace Types{
    class CNetAddr;
    class CSizableBuffer;
}}


namespace NatAgent{namespace Interface{
    //
    /**
    *  打洞结果通知
    */
    class IPunchResultCallback
    {
    public:
        enum EmPunchResult
        {
            emInvalid,//不会出现此情况
                emOK,//成功
                emTimeout,//超时
        };
        virtual ~IPunchResultCallback(){}
        
        /**
        *  当有结果时会调用此函数, 如果超时的话 punchAddrs 参数为无效值.
        *  如果成功打洞的话, punchAddrs 则是成功的地址对.
        */
        virtual void OnRecvPunchResult(EmPunchResult emResult,
            const SYS::Types::CSizableBuffer& punchKey,
            const Types::CPunchAddrs& punchAddrs,
            const Types::CRemoteAddrsTable& remoteAddrs) = 0;
    };
    
}}

#endif

//end of file

