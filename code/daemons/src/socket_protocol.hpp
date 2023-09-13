#ifndef __SOCKETPROTOCOL_HPP__
#define __SOCKETPROTOCOL_HPP__
#include <string>
#include "json/json.h"
#include "LogSDKEx.h"


enum DataType {
    DT_CrashMonitor = 1,         // 监控进程崩溃并重启
    DT_Feedback,                 // 服务端返回给客户端的状态信息
    DT_MaxEnum,
};

const char default_header[4] = {'J', 'A', 'Y', 'R'};
const char default_tailer[4] = {'A', 'V', 'E', 'N'};

struct SockData {
    SockData() {
        memcpy(header, default_header, sizeof(header));
        data_type = DataType::DT_MaxEnum;
        memset(data_len, 0, sizeof(data_len));
        data_body = NULL;
        memcpy(tailer, default_tailer, 4);
    }
    ~SockData() {
        if (data_body)
            delete data_body;
    }

    bool CheckHeader() {
        return memcmp(header, default_header, sizeof(header)) == 0;
    }
    bool CheckTailer() {
        return memcmp(tailer, default_tailer, sizeof(tailer)) == 0;
    }
    bool CheckDataType() {
        return data_type > 0 && data_type < DataType::DT_MaxEnum;
    }
    void SetData(const char* buf, unsigned long long buf_len) {
        memcpy(data_len, (char*)(&buf_len), sizeof(data_len));
        data_body = new char[buf_len];
        memcpy(data_body, buf, buf_len);
    }
    char* GetData() const {
        char *ret = new char[GetDataLen()];
        auto pos = sizeof(header) + sizeof(data_type) + sizeof(data_len);
        memcpy(ret, (char*)this, pos);
        memcpy(ret + pos, data_body, *(unsigned long long*)data_len);
        memcpy(ret + pos + *(unsigned long long*)data_len, tailer, sizeof(tailer));
        return ret;
    }
    unsigned int GetDataLen() const {
        return sizeof(header) + sizeof(data_type) + sizeof(data_len) + 
            *(unsigned long long*)data_len + sizeof(tailer);
    }

    char header[4];
    char data_type;
    char data_len[8];
    char* data_body;
    char tailer[4];
};

struct CrashMonitorInfo {
    CrashMonitorInfo(): process_id(0), restart(false) {}
    unsigned int process_id;
    std::string process_name;
    bool restart;
    std::string restart_exe_path;

    bool FromJsonStr(const std::string& json_str) {
        Json::Reader reader;
        Json::Value j;
        if (reader.parse(json_str, j)) {
            return false;            
        }
        process_id = j["process_id"].asUInt();
        process_name = j["process_name"].asString();
        restart = j["restart"].asBool();
        restart_exe_path = j["restart_exe_path"].asString();
        return true;
    }
};

struct FeedbackInfo {
    FeedbackInfo(): data_type(0), ret(0), msg(NULL) {}
    char data_type;
    unsigned int ret;
    std::string msg;

    std::string ToJsonStr() {
        Json::Value j;
        j["data_type"] = data_type;
        j["ret"] = ret;
        j["msg"] = msg;

        Json::FastWriter writer;
        return writer.write(j);
    }
};

#endif
