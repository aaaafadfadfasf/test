#ifndef RAII_CLOSE_SOCKET_H
#define RAII_CLOSE_SOCKET_H
#include "system/types/basictypes.h"

namespace SYS{ namespace Network{ 

class CRAIICloseSocket
{
public:
    CRAIICloseSocket(Socket hSocket) : m_hSocket(hSocket)
    {
    }
    ~CRAIICloseSocket()
    {
        if(m_hSocket != INVALID_SOCKET)
        {
            closesocket(m_hSocket);
        }
    }
private:
    Socket m_hSocket;
};
}}
#endif

////end of file


