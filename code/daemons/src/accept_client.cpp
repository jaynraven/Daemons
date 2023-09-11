#include "accept_client.hpp"
#include "LogSDKEx.h"
#include "socket.hpp"
#include <thread>
#include <functional>

AcceptClient::AcceptClient(SOCKET socket) : 
    socket_(socket)
{
}

AcceptClient::~AcceptClient()
{
    if (socket_)
        closesocket(socket_);    // V（资源占用完毕）
}

void AcceptClient::ReceiveMessageThread()
{
    LOG_INFO_FUNC_BEGIN;

    std::thread th_queue_process = std::thread(std::bind(&AcceptClient::ProcessDataQueueThread, this));

    int ret = 0;
    while (true)
    {
        SockData sock_data;

        // header
        if (!socket_read(socket_, sock_data.header, sizeof(sock_data.header)))
        {
            break;
        }
        if (sock_data.CheckHeader())
        {
            LOG_ERROR("header check failed");
            break;
        }

        // data_type
        if (!socket_read(socket_, &sock_data.data_type, sizeof(char)))
        {
            break;
        }
        if (sock_data.CheckDataType())
        {
            LOG_ERROR("data_type check failed");
            break;
        }

        // data_len
        if (!socket_read(socket_, sock_data.data_len, sizeof(sock_data.data_len)))
        {
            break;
        }

        // data_body
        sock_data.data_body = new char[*(unsigned long long*)sock_data.data_len];
        if (!socket_read(socket_, sock_data.data_body, *(unsigned long long*)sock_data.data_len))
        {
            delete sock_data.data_body;
            break;
        }

        // tailer
        if (!socket_read(socket_, sock_data.tailer, sizeof(sock_data.tailer)))
        {
            break;
        }
        if (sock_data.CheckTailer())
        {
            LOG_ERROR("tailer check failed");
            break;
        }

        // put to data queue
        queue_.push(sock_data);
    }

    if (th_queue_process.joinable())
        th_queue_process.join();
    
    if (socket_)
        closesocket(socket_);    // V（资源占用完毕）

    LOG_INFO_FUNC_END;
}

void AcceptClient::ProcessDataQueueThread() 
{
    // todo
}