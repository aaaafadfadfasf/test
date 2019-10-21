#ifndef _LOCAL_IP_TABLE_H_
#define _LOCAL_IP_TABLE_H_
#include <vector>
#include "kdvtype.h"

namespace NatAgent{namespace Types{
    
    class CLocalIpTable
    {
    protected:
        typedef std::vector<u32> LocalIpTable;
        
    public:
        typedef LocalIpTable::const_iterator const_iterator;
        CLocalIpTable(){}
        
        CLocalIpTable(const std::vector<u32>& localIps) : m_localIps(localIps)
        {
        }
        
        /**
        *  @param dwIp ±¾»úµÄip, net order
        */
        void push_back(u32 dwIp)
        {
            m_localIps.push_back(dwIp);
        }
        
        size_t size()const
        {
            return m_localIps.size();
        }
        
        void clear()
        {
            m_localIps.clear();
        }
        
        const_iterator begin()const
        {
            return m_localIps.begin();
        }
        
        const_iterator end()const
        {
            return m_localIps.end();
        }
        
        const u32 operator[](size_t nIndex)const
        {
            return m_localIps.at(nIndex);
        }
    private:
        LocalIpTable m_localIps;
    };
}}

#endif
//end of file

