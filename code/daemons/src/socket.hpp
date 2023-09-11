#ifndef __SOCKET_HPP__
#define __SOCKET_HPP__

#include <Winsock2.h>
#include "LogSDKEx.h"

bool socket_read(SOCKET socket, char* buf, unsigned long long len)
{
    unsigned long readed = 0;
    char* buf_temp = buf;
    while(readed == len) {
        int ret = recv(socket, buf_temp, len - readed, 0);
        if (ret <= 0) {
            LOG_ERROR("recv failed with error %ld", WSAGetLastError());
            delete [] buf;
            return false;
        }
        readed += ret;
        buf_temp += ret;
    }
    return true;
}

#endif