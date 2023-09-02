#ifndef __SERVER_HPP__
#define __SERVER_HPP__

#include <Winsock2.h>
#include <atomic>

class Server
{
private:
    SOCKET socket_;

    Server();
    ~Server();

public:
    static Server* Instance();

    bool InitServer();
    void CloseServer();
    void Listen();
};

#endif