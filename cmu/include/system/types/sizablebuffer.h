#ifndef _SIZE_ABLE_BUFFER_H_
#define _SIZE_ABLE_BUFFER_H_

#include <string>
#include "kdvtype.h"

namespace SYS{namespace Types{
    
    class CSizableBuffer
    {
    public:
        CSizableBuffer()
        {
            
        }
        CSizableBuffer(const u8* pBuffer,u16 wLen): m_buffer(reinterpret_cast<const s8*>(pBuffer),wLen)
        {
            
        }
        
        explicit CSizableBuffer(const std::string& szString):m_buffer(szString)
        {            
        }
        
        CSizableBuffer(const CSizableBuffer& buffer)
        {
            this->operator =(buffer);
        }
        
        CSizableBuffer& operator = (const CSizableBuffer& buffer)
        {
            if (&buffer == this)
            {
                return *this;
            }
            m_buffer.assign(buffer.m_buffer);
            return *this;
        }
        
        void Append(const void* pBuffer,u16 wLen)
        {
            m_buffer.insert(m_buffer.length(),reinterpret_cast<const s8*>(pBuffer),wLen);
        }
        
        void Append(const CSizableBuffer& buffer)
        {
            m_buffer.insert(m_buffer.end(),
                buffer.m_buffer.begin(),
                buffer.m_buffer.end());
        }
        
        void Append(const std::string& buffer)
        {
            m_buffer.insert(m_buffer.end(),
                buffer.begin(),
                buffer.end());
        }
        
        const std::string& ToString()const
        {
            return m_buffer;
        }
        
        const u8* GetBuffer()const
        {
            return reinterpret_cast<const u8*>(m_buffer.data());
        }
        u16 GetLength()const
        {
            return m_buffer.length();
        }
        void Clear()
        {
            m_buffer = "";
        }
        bool IsEmpty()const
        {
            return m_buffer.empty();
        }
        
        CSizableBuffer Clone()const
        {
            return CSizableBuffer(*this);
        }
        
        bool operator<(const CSizableBuffer& buffer)const
        {
            return m_buffer<buffer.m_buffer;
        }
    protected:
        std::string m_buffer;
    };
}}
#endif

//end of file

