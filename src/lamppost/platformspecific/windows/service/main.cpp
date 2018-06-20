#include <lamppost/platformspecific/windows/service/Service.h>


#include <strsafe.h>
#include <tchar.h>
#include <windows.h>


#pragma comment(lib, "advapi32.lib")


lp::platformspecific::windows::service::Service service("LampPost.Service", "The LampPost background service.");


void serviceMain(DWORD dwArgc, LPTSTR* lpszArgv)
{
  return service.Main(dwArgc, lpszArgv);
}


void __cdecl _tmain(int argc, TCHAR *argv[])
{
  // If command-line parameter is "install", install the service.
  // If command-line parameter is "uninstall", uninstall the service.
  // Otherwise, the service is probably being started by the SCM.
  if(lstrcmpi(argv[1], TEXT("install")) == 0)
  {
    service.Install();
  }
  else if(lstrcmpi(argv[1], TEXT("uninstall")) == 0)
  {
    service.Uninstall();
  }
  else
  {
    // TO_DO: Add any additional services for the process to this table.
    SERVICE_TABLE_ENTRY DispatchTable[] =
      {
        {service.GetName(), (LPSERVICE_MAIN_FUNCTION)serviceMain},
        {nullptr, nullptr}
      };

    // This call returns when the service has stopped.
    // The process should simply terminate when the call returns.
    if(!StartServiceCtrlDispatcher(DispatchTable))
    {
      service.ReportEventA(TEXT("StartServiceCtrlDispatcher"));
    }
  }
}
