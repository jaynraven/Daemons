#include "accept_client.hpp"
#include "LogSDKEx.h"
#include "socket.hpp"
#include <thread>
#include <functional>
#include "util.hpp"
#include "error_defines.hpp"

AcceptClient::AcceptClient(SOCKET socket) : 
    socket_(socket), stop_(false)
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
    {
        stop_ = true;
        th_queue_process.join();
    }
    
    if (socket_)
        closesocket(socket_);    // V（资源占用完毕）

    LOG_INFO_FUNC_END;
}

void AcceptClient::ProcessDataQueueThread() 
{
    LOG_INFO_FUNC_BEGIN;

    std::vector<std::thread> ths;
    while (true)
    {
        while (!queue_.empty())
        {
            std::shared_ptr<SockData> sock_data = queue_.try_pop();
            if (sock_data) 
            {
                // process socket data
                switch (sock_data->data_type)
                {
                case DT_CrashMonitor:
                {
                    CrashMonitorInfo info;
                    if (info.FromJsonStr(sock_data->data_body))
                    {
                        ths.push_back(std::thread(std::bind(&AcceptClient::MonitorProcessCrashThread, this, info)));
                        FeedbackMsg(DT_CrashMonitor, ERROR_NO, "start monitor process crash succeed!");
                    }
                }
                    break;
                default:
                    break;
                }
            }
        }
        for (size_t i = 0; i < 100; i++)
        {
            if (stop_) break;
            Sleep(10);
        }
    }

    for (size_t i = 0; i < ths.size(); i++)
    {
        if (ths[i].joinable())
            ths[i].join();
    }

    LOG_INFO_FUNC_END;
}

void AcceptClient::MonitorProcessCrashThread(CrashMonitorInfo info)
{
    LOG_INFO_FUNC_BEGIN;

    DWORD processId = 1234;  // 替换为你要监测的进程ID

    HANDLE processHandle = OpenProcess(SYNCHRONIZE, FALSE, processId);
    if (processHandle == NULL)
    {
        LOG_ERROR("OpenProcess %ld failed with error %ld", info.process_id, GetLastError());
        return;
    }

    while (true)
    {
        DWORD exitCode;
        if (GetExitCodeProcess(processHandle, &exitCode) == FALSE)
        {
            LOG_ERROR("GetExitCodeProcess %ld failed with error %ld", info.process_id, GetLastError());
            break;
        }

        if (exitCode != STILL_ACTIVE)
        {
            LOG_INFO("Process %ld crash with %ld", info.process_id, exitCode);
            if (info.restart)
            {
                ExcuteProcess(info.restart_exe_path);
            }
            break;
        }

        Sleep(1000);  // 间隔1秒钟轮询一次
    }

    CloseHandle(processHandle);

    LOG_INFO_FUNC_END;
}

void AcceptClient::FeedbackMsg(FeedbackInfo info)
{
    FeedbackMsg(info.data_type, info.ret, info.msg);
}

void AcceptClient::FeedbackMsg(char data_type, unsigned int ret, std::string msg)
{
    LOG_INFO_FUNC_BEGIN;

    FeedbackInfo info;
    info.data_type = data_type;
    info.ret = ret;
    info.msg = msg;

    SockData sock_data;
    sock_data.data_type = DT_Feedback;
    sock_data.SetData(info.ToJsonStr().c_str(), info.ToJsonStr().size());
    auto data = sock_data.GetData();
    auto ret = send(socket_, data, sock_data.GetDataLen(), NULL);
    if (ret <= 0) 
    {
        LOG_ERROR("send failed with error %ld", WSAGetLastError());
    }
    delete data;
    
    LOG_INFO_FUNC_END;
}