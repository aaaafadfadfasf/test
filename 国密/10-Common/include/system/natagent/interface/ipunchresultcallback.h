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
    *  �򶴽��֪ͨ
    */
    class IPunchResultCallback
    {
    public:
        enum EmPunchResult
        {
            emInvalid,//������ִ����
                emOK,//�ɹ�
                emTimeout,//��ʱ
        };
        virtual ~IPunchResultCallback(){}
        
        /**
        *  ���н��ʱ����ô˺���, �����ʱ�Ļ� punchAddrs ����Ϊ��Чֵ.
        *  ����ɹ��򶴵Ļ�, punchAddrs ���ǳɹ��ĵ�ַ��.
        */
        virtual void OnRecvPunchResult(EmPunchResult emResult,
            const SYS::Types::CSizableBuffer& punchKey,
            const Types::CPunchAddrs& punchAddrs,
            const Types::CRemoteAddrsTable& remoteAddrs) = 0;
    };
    
}}

#endif

//end of file

