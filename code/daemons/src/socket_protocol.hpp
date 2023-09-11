#ifndef __SOCKETPROTOCOL_HPP__
#define __SOCKETPROTOCOL_HPP__
#include <string>

enum DataType {
    DT_CrashMonitor = 1,
    DT_MaxEnum = 1,
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

    bool CheckHeader() {
        return memcmp(header, default_header, sizeof(header)) == 0;
    }
    bool CheckTailer() {
        return memcmp(tailer, default_tailer, sizeof(tailer)) == 0;
    }
    bool CheckDataType() {
        return data_type > 0 && data_type < DataType::DT_MaxEnum;
    }

    char header[4];
    char data_type;
    char data_len[8];
    char* data_body;
    char tailer[4];
};

#endif
