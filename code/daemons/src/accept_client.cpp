#include "accept_client.hpp"
#include "LogSDKEx.h"

AcceptClient::AcceptClient(SOCKET socket) : 
    socket_(socket)
{
}

AcceptClient::~AcceptClient()
{
}

void AcceptClient::ReceiveMessageThread()
{
    LOG_INFO_FUNC_BEGIN;

    while(1) {     
        char recvBuf[1024];  
        recv(socket_, recvBuf, 1024, 0);
        printf("recv from client %d: %s", socket_, recvBuf);
    } 

    if (socket_)
        closesocket(socket_);    // V（资源占用完毕）

    LOG_INFO_FUNC_END;
}