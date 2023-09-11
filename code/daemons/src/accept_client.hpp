#ifndef __ACCEPTCLENT_HPP__
#define __ACCEPTCLENT_HPP__

#include <Winsock2.h>
#include "threadsafe_queue.hpp"
#include "socket_protocol.hpp"

class AcceptClient
{
private:
    SOCKET socket_;
    threadsafe_queue<SockData> queue_;

public:
    AcceptClient(SOCKET socket);
    ~AcceptClient();

    void ReceiveMessageThread();
    void ProcessDataQueueThread();
};

#endif