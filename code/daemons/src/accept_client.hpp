#ifndef __ACCEPTCLENT_HPP__
#define __ACCEPTCLENT_HPP__

#include <Winsock2.h>
#include "threadsafe_queue.hpp"
#include "socket_protocol.hpp"
#include <atomic>

class AcceptClient
{
private:
    SOCKET socket_;
    threadsafe_queue<SockData> queue_;
    std::atomic<bool>  stop_;

public:
    AcceptClient(SOCKET socket);
    ~AcceptClient();

    void ReceiveMessageThread();

private:
    void ProcessDataQueueThread();
    void MonitorProcessCrashThread(CrashMonitorInfo info);
    void FeedbackMsg(FeedbackInfo info);
    void FeedbackMsg(char data_type, unsigned int ret, std::string msg);
};

#endif