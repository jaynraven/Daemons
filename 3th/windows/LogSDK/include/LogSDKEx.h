/**
 * @file LogSDKEx.h
 * @author your name (you@domain.com)
 * @brief Added some macro definitions to help users use logsdk more conveniently, 
 * if you feel that the extensibility is not strong, you can just use the LogSDK.h
 * @version 0.1
 * @date 2023-07-22
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef __LOGSDKEX_HPP__
#define __LOGSDKEX_HPP__

#define LOG_CATALOG     "daemons"                           //define your log catalog here
#define LOG_LOG_FILE    ".//logs//daemons.log"               //define your log file path here
#define LOG_LOG_Config  ".//config//logsdk_config.ini"   //define your log config path here

#include "LogSDK.h"

#define LOG_INIT                InitLog(LOG_LOG_Config, LOG_LOG_FILE, LOG_CATALOG)

#define LOG_INFO(format, ...)   Log(LOG_CATALOG, LEVEL_INFO, format, __VA_ARGS__)
#define LOG_ERROR(format, ...)  Log(LOG_CATALOG, LEVEL_ERROR, format, __VA_ARGS__)
#define LOG_WARN(format, ...)   Log(LOG_CATALOG, LEVEL_WARN, format, __VA_ARGS__)
#define LOG_DEBUG(format, ...)  Log(LOG_CATALOG, LEVEL_DEBUG, format, __VA_ARGS__)

#endif