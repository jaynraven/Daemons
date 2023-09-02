#include "server.hpp"
#include "LogSDKEx.h"
#include "accept_client.hpp"
#include <thread>
#include <functional>
#include <vector>
// #include <windows.h>

Server::Server(/* args */): socket_(NULL)
{
}

Server::~Server()
{
    CloseServer(); 
}

Server* Server::Instance()
{
    static Server* instance = new Server();
    return instance;
}

bool Server::InitServer()
{
    LOG_INFO_FUNC_BEGIN;
    // 加载socket动态链接库(dll)
    WORD wVersionRequested;
    WSADATA wsaData;    // 这结构是用于接收Wjndows Socket的结构信息的
    wVersionRequested = MAKEWORD( 2, 2 );   // 请求2.2版本的WinSock库
    int err = WSAStartup( wVersionRequested, &wsaData );
    if ( err != 0 ) {
        LOG_ERROR("WSAStartup failed with error %ld", WSAGetLastError());
        return false;          // 返回值为零的时候是表示成功申请WSAStartup
    }
    if ( LOBYTE( wsaData.wVersion ) != 2 || HIBYTE( wsaData.wVersion ) != 2 ) { // 检测是否2.2版本的socket库
        WSACleanup();
        LOG_WARN("Socket version is not 2.2");
        return false;
    }
    
    // 创建socket操作，建立流式套接字，返回套接字号sockSrv
    socket_ = socket(AF_INET, SOCK_STREAM, 0);

    // 套接字sockSrv与本地地址相连
    SOCKADDR_IN addrSrv;
    addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY); // 将INADDR_ANY转换为网络字节序，调用 htonl(long型)或htons(整型)
    addrSrv.sin_family = AF_INET;
    addrSrv.sin_port = htons(6000);

    if(SOCKET_ERROR == bind(socket_, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR))){ // 第二参数要强制类型转换
        LOG_ERROR("bind failed with error %ld", WSAGetLastError());
        closesocket(socket_);
        WSACleanup();
        return false;
    }

    // 将套接字设置为监听模式（连接请求）， listen()通知TCP服务器准备好接收连接
    if (listen(socket_, 20) == SOCKET_ERROR) {
        LOG_ERROR("listen failed with error: %ld", WSAGetLastError());
        closesocket(socket_);
        WSACleanup();
        return false;
    }

    LOG_INFO_FUNC_END;
    return true;
}

void Server::CloseServer()
{
    if (socket_)
        closesocket(socket_);
    WSACleanup();   // 终止对套接字库的使用  
}

void Server::Listen()
{
    LOG_INFO_FUNC_BEGIN;
    std::vector<AcceptClient*> acpt_client_group;
    std::vector<std::thread> recv_thread_group;
    while(true) {    // 不断等待客户端请求的到来
        SOCKET sockConn = accept(socket_, NULL, NULL);  
        if (SOCKET_ERROR == sockConn) { 
            LOG_ERROR("accept SOCKET_ERROR");
            continue;
        }

        LOG_INFO("accept: %d", sockConn);
        AcceptClient* acpt_client = new AcceptClient(sockConn);
        std::thread recv_thread = std::thread(std::bind(&AcceptClient::ReceiveMessageThread, acpt_client));
    }

    for (size_t i = 0; i < recv_thread_group.size(); i++)
    {
        if (recv_thread_group[i].joinable())
            recv_thread_group[i].join();
    }

    for (size_t i = 0; i < acpt_client_group.size(); i++)
    {
        if (acpt_client_group[i])
            delete acpt_client_group[i];
    }

    CloseServer();

    LOG_INFO_FUNC_END;
}