#ifndef _NAT_AGENT_PACKET_H_
#define _NAT_AGENT_PACKET_H_

#include <string>
#include "cms/cmsassert.h"
#include "kdvtype.h"
#include "system/types/sizablebuffer.h"

namespace NatAgent{namespace Types{
    
    class CNatAgentPacket
    {
    public:
        static BOOL32 IsValidPacket(const u8* pBuf,u16 wLen);
        
        static const SYS::Types::CSizableBuffer ParseKey(const u8* pBuf,u16 wLen);
        
        static const SYS::Types::CSizableBuffer GenPacket(const SYS::Types::CSizableBuffer& key);
        
        static const SYS::Types::CSizableBuffer GenPacket(const std::string& key);
        
        static u8 m_magicCookie[4];
    };
    
}}


#endif

//end of file

