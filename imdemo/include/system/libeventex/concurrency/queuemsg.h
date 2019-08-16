#ifndef _QUEUE_MSG_H_
#define _QUEUE_MSG_H_

#include "kdvtype.h"
#include <memory>

namespace LibEvent{namespace Concurrency{
    
    class CQueueMsg
    {
    public:
        enum
        {
            MAX_BUFFER = 2048
        };
        CQueueMsg()
        {
            m_bufLen = 0;
        }
        void SetEvent(unsigned short wEvent)
        {
            m_wEvent = wEvent;
        }
        u16 GetEvent()const 
        {
            return m_wEvent;
        }
        
        void SetBody(const void* pBuffer,u16 wLen)
        {
            memcpy(m_achBuf,pBuffer,wLen);
            m_bufLen = wLen;
        }
        const u8* GetBody(u16 wOffset = 0)const
        {
            return m_achBuf + wOffset;
        }
        u16 GetLen()const
        {
            return m_bufLen;
        }
        void CatBody(const void* pBuffer,u16 wLen)
        {
            memcpy(m_achBuf+m_bufLen,pBuffer,wLen);
            m_bufLen += wLen;
        }
    protected:
        unsigned short m_wEvent;
        unsigned short m_bufLen;
        unsigned char  m_achBuf[MAX_BUFFER];
    };
}}

#endif
//end of file

