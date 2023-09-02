#include <stdio.h>
#include <atomic>
#include "LogSDKEx.h"
#include "server.hpp"
#include <windows.h>

#pragma comment (lib, "ws2_32.lib")  //加载 ws2_32.dll

SERVICE_STATUS        g_ServiceStatus = {0};
SERVICE_STATUS_HANDLE g_StatusHandle = NULL;

VOID WINAPI ServiceMain (DWORD argc, LPTSTR *argv);
VOID WINAPI ServiceCtrlHandler (DWORD);

#define SERVICE_NAME  L"Daemons Service"

int main (int argc, char **argv)
{
    LOG_INIT;

    // test
    if (Server::Instance()->InitServer())
        Server::Instance()->Listen();
    // test end

    SERVICE_TABLE_ENTRY ServiceTable[] = 
    {
        {SERVICE_NAME, (LPSERVICE_MAIN_FUNCTION) ServiceMain},
        {NULL, NULL}
    };

    if (StartServiceCtrlDispatcher (ServiceTable) == FALSE)
    {
       return GetLastError ();
    }

    return 0;
}

VOID WINAPI ServiceMain (DWORD argc, LPTSTR *argv)
{
    DWORD Status = E_FAIL;

    g_StatusHandle = RegisterServiceCtrlHandler (SERVICE_NAME, ServiceCtrlHandler);

    if (g_StatusHandle == NULL) 
    {
        return;
    }

    // Tell the service controller we are starting
    ZeroMemory (&g_ServiceStatus, sizeof (g_ServiceStatus));
    g_ServiceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    g_ServiceStatus.dwControlsAccepted = 0;
    g_ServiceStatus.dwCurrentState = SERVICE_START_PENDING;
    g_ServiceStatus.dwWin32ExitCode = 0;
    g_ServiceStatus.dwServiceSpecificExitCode = 0;
    g_ServiceStatus.dwCheckPoint = 0;

    if (SetServiceStatus (g_StatusHandle, &g_ServiceStatus) == FALSE) 
    {
        OutputDebugString(L"My Sample Service: ServiceMain: SetServiceStatus returned error");
    }

    /*
     * Perform tasks necessary to start the service here
     */
    g_ServiceStatus.dwControlsAccepted |= (SERVICE_ACCEPT_STOP | 
                                           SERVICE_ACCEPT_SHUTDOWN);
    g_ServiceStatus.dwCurrentState = SERVICE_RUNNING;
    g_ServiceStatus.dwCheckPoint = 0;
    g_ServiceStatus.dwWaitHint = 0;

    if (SetServiceStatus (g_StatusHandle, &g_ServiceStatus) == FALSE)
    {
        OutputDebugString(L"My Sample Service: ServiceMain: SetServiceStatus returned error");
    }

    if (Server::Instance()->InitServer())
        Server::Instance()->Listen();
   
    /*
     * Perform any cleanup tasks
     */

    g_ServiceStatus.dwControlsAccepted &= ~(SERVICE_ACCEPT_STOP | 
                                           SERVICE_ACCEPT_SHUTDOWN);
    g_ServiceStatus.dwCurrentState = SERVICE_STOPPED;
    g_ServiceStatus.dwWin32ExitCode = 0;
    g_ServiceStatus.dwCheckPoint = 3;

    if (SetServiceStatus (g_StatusHandle, &g_ServiceStatus) == FALSE)
    {
        OutputDebugString(L"My Sample Service: ServiceMain: SetServiceStatus returned error");
    }

    LOG_INFO("quit");
            
    return;
}

VOID WINAPI ServiceCtrlHandler (DWORD CtrlCode)
{
    switch (CtrlCode) 
	{
     case SERVICE_CONTROL_STOP :
     {
        LOG_INFO("signal: stop");
        if (g_ServiceStatus.dwCurrentState != SERVICE_RUNNING)
           break;

        /* 
         * Perform tasks necessary to stop the service here 
         */
         
        g_ServiceStatus.dwControlsAccepted = 0;
        g_ServiceStatus.dwCurrentState = SERVICE_STOP_PENDING;
        g_ServiceStatus.dwWin32ExitCode = 0;
        g_ServiceStatus.dwCheckPoint = 4;

        if (SetServiceStatus (g_StatusHandle, &g_ServiceStatus) == FALSE)
        {
            OutputDebugString(L"My Sample Service: ServiceCtrlHandler: SetServiceStatus returned error");
        }

        // This will signal the worker thread to start shutting down
        Server::Instance()->CloseServer();
     }

        break;

     default:
         break;
    }
}