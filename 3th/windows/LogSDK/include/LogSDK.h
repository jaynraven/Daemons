#ifndef __LOGSDK_HPP__
#define __LOGSDK_HPP__

#ifdef _WIN32
#ifdef DLL_EXPORT
#define LIBRARY_API extern "C" __declspec(dllexport)
#else
#define LIBRARY_API extern "C" __declspec(dllimport)
#endif
#else
#ifdef DLL_EXPORT
#define LIBRARY_API extern "C" __attribute__((visibility ("default")))
#else
#define LIBRARY_API extern "C" 
#endif
#endif

#include <string>

typedef enum {
    LEVEL_ERROR  = 0, 
    LEVEL_WARN,
    LEVEL_INFO,
    LEVEL_DEBUG
} LogLevel;

LIBRARY_API bool InitLog(const char* config_file, const char* log_file, const char* catalog);
LIBRARY_API bool Log(const char* catalog, LogLevel level, const char* format, ...);

#endif