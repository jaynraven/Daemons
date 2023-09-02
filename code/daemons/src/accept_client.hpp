#ifndef __ACCEPTCLENT_HPP__
#define __ACCEPTCLENT_HPP__

#include <Winsock2.h>

class AcceptClient
{
private:
    SOCKET socket_;

public:
    AcceptClient(SOCKET socket);
    ~AcceptClient();

    void ReceiveMessageThread();
};

#endif