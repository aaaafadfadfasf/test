#ifndef _REMOTE_ADDRS_TABLE_H_
#define _REMOTE_ADDRS_TABLE_H_

#include <vector>
#include "system/types/netaddr.h"

namespace NatAgent{namespace Types{
    //
    /**
    *  �Զ˵�ַ, ��ʱ������������ַ.
    */
    class CRemoteAddrsTable
    {
    public:
        typedef std::vector<SYS::Types::CNetAddr> RemoteAddrs;
        typedef RemoteAddrs::const_iterator const_iterator;
        /**
        *  ����������ַ
        */
        void AddOutterAddr(const SYS::Types::CNetAddr& addr);
        /**
        *  ����������ַ
        */
        void AddInnerAddr(const SYS::Types::CNetAddr& addr);
        
        const_iterator begin()const;
        const_iterator end()const;
        
        size_t size()const;
        void clear();
        const SYS::Types::CNetAddr& operator[](size_t nIndex)const;
    private:    
        RemoteAddrs m_remoteAddrs;//inner ip + outter ip, inner ip��ǰ��
    };
    
}}

#endif

//end of file

