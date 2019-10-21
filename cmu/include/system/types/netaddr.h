#ifndef _NAT_ADDR_H_
#define _NAT_ADDR_H_

#include <ostream>
#include "kdvtype.h"
#include "system/config/osconfig.h"
#include "system/network/formatip.h"

//1 byte align
#ifdef WIN32
#pragma pack(push, 1)
#elif defined(__GNUC__)
#pragma pack(1)
#else
#error not supports this platform
#endif

namespace SYS{ namespace Types{
    
    class CNetAddr
    {
    public:
    protected:
        u32 m_dwIpAddr; //IP��ַ(������)
        u16 m_wPort; //�˿ں�;
    public:
        CNetAddr(u32 dwIpAddr, u16 wPort):m_dwIpAddr(dwIpAddr),m_wPort(htons(wPort))
        {
        }
        
        bool operator<(const CNetAddr& rValue)const
        {
            if (this->GetIpAddr() < rValue.GetIpAddr())
            {
                return true;
            }
            else if (this->GetIpAddr() > rValue.GetIpAddr())
            {
                return false;
            }
            
            if (this->GetPort() < rValue.GetPort())
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        
        bool operator == (const CNetAddr& addr)const
        {
            return m_wPort == addr.m_wPort && m_dwIpAddr == addr.m_dwIpAddr;
        }
        
        bool operator != (const CNetAddr& addr)const
        {
            return !(m_wPort == addr.m_wPort && m_dwIpAddr == addr.m_dwIpAddr);
        }
        //��ȡIP��ַ��������
        u32 GetIpAddr() const 
        {
            return m_dwIpAddr;
        }
        //����IP��ַ��������
        void SetIpAddr(u32 dwIpAddr) 
        {
            m_dwIpAddr = dwIpAddr;
        }
        
        //��ȡ�˿ں�
        u16 GetPort() const 
        {
            return ntohs(m_wPort);
        }
        //���ö˿ں�
        void SetPort(u16 wPort) 
        {
            m_wPort = htons(wPort);
        }
        
        
        friend std::ostream& operator<<(std::ostream& output,const CNetAddr& addr)
        {
            addr.Print(output);
            return output;
        }
    protected:
        void Print(std::ostream& outstream)const 
        {
            char ip[64] ={0};
            sprintf(ip,"%d.%d.%d.%d",FORMAT_IP(m_dwIpAddr));
            outstream<<"ip:"<<ip<<"\tport:"<<GetPort();
        }
    };
    
}}

#ifdef WIN32
#pragma pack(pop)
#elif defined (__GNUC__)
#pragma pack()
#else
#error not supports this platform
#endif

#endif

//end of file


