#ifndef __UTIL_HPP__
#define __UTIL_HPP__
#include <string>
#include <windows.h>
#include <shellapi.h>
#include <codecvt>
#include "LogSDKEx.h"

std::wstring StrToUtf16(const std::string& str)
{
	if (str.empty())
	{
		return std::wstring();
	}
    try
	{
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> conv;
		return conv.from_bytes(str);
    }
    catch (const std::exception&)
	{
		std::wstring_convert<std::codecvt_utf16<wchar_t>, wchar_t> conv;
		return conv.from_bytes(str);
    }
}

std::string WstrToUtf8(const std::wstring& wstr)
{
	if (wstr.empty())
	{
		return std::string();
	}
    try
	{
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
        return conv.to_bytes(wstr);
    }
    catch (const std::exception&)
	{
        std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
		return conv.to_bytes(wstr);
    }
}

bool ExcuteProcess(std::string exe_path)
{
    SHELLEXECUTEINFO shellInfo;
    ZeroMemory(&shellInfo, sizeof(shellInfo));

    shellInfo.cbSize = sizeof(shellInfo);
    shellInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
    shellInfo.lpVerb = L"open";  // 操作类型，默认为"open"
    shellInfo.lpFile = StrToUtf16(exe_path).c_str();  // 替换为你要启动的进程命令
    shellInfo.nShow = SW_SHOWNORMAL;  // 窗口显示方式，默认为SW_SHOWNORMAL

    if (!ShellExecuteEx(&shellInfo))
    {
        LOG_ERROR("process excute failed: %s", exe_path.c_str());
        return false;
    }

    return true;
}

#endif