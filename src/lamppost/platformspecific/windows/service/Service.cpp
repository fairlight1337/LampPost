#include <lamppost/platformspecific/windows/service/Service.h>


namespace lp
{
  namespace platformspecific
  {
    namespace windows
    {
      namespace service
      {
        DWORD WINAPI Service::StaticCtrlHandler(DWORD dwControl, DWORD dwEventType, void* eventData, void* context)
        {
          Service* svc = static_cast<Service*>(context);
          svc->ControlHandler(dwControl);

          return NO_ERROR;
        }

        Service::Service(std::string name, std::string description)
          : mName(name),
            mDescription(description),
            mLampPost(lp::LampPostConfiguration())
        {
        }

        LPSTR Service::GetName()
        {
          return (LPSTR)mName.c_str();
        }

        void Service::Install()
        {
          SC_HANDLE schSCManager;
          SC_HANDLE schService;
          TCHAR szPath[MAX_PATH];

          if(!GetModuleFileName(nullptr, szPath, MAX_PATH))
          {
            throw lp::exceptions::InvalidOperationException("Failed to install service (" + std::to_string(GetLastError()) + ")");
          }

          // Get a handle to the SCM database.
          schSCManager = OpenSCManager(
            nullptr,                    // local computer
            nullptr,                    // ServicesActive database
            SC_MANAGER_ALL_ACCESS);     // full access rights

          if(schSCManager == nullptr)
          {
            throw lp::exceptions::InvalidOperationException("Failed to open SC manager (" + std::to_string(GetLastError()) + ")");
          }

          // Create the service
          schService = CreateService(
            schSCManager,              // SCM database
            GetName(),                 // name of service
            GetName(),                 // service name to display
            SERVICE_ALL_ACCESS,        // desired access
            SERVICE_WIN32_OWN_PROCESS, // service type
            SERVICE_AUTO_START,        // start type
            SERVICE_ERROR_NORMAL,      // error control type
            szPath,                    // path to service's binary
            nullptr,                   // no load ordering group
            nullptr,                   // no tag identifier
            nullptr,                   // no dependencies
            nullptr,                   // LocalSystem account
            nullptr);                  // no password

          if(schService == nullptr)
          {
            printf("CreateService failed (%d)\n", GetLastError());
            CloseServiceHandle(schSCManager);

            return;
          }
          else
          {
            SERVICE_DESCRIPTION serviceDescription;
            serviceDescription.lpDescription = (LPSTR)mDescription.c_str();

            ChangeServiceConfig2(
              schService,
              SERVICE_CONFIG_DESCRIPTION,
              &serviceDescription);

            printf("Service installed successfully\n");
          }

          CloseServiceHandle(schService);
          CloseServiceHandle(schSCManager);
        }

        void Service::Uninstall()
        {
          SC_HANDLE schSCManager;
          SC_HANDLE schService;

          // Get a handle to the SCM database.
          schSCManager = OpenSCManager(
            nullptr,                    // local computer
            nullptr,                    // ServicesActive database
            SC_MANAGER_ALL_ACCESS);     // full access rights

          if(schSCManager == nullptr)
          {
            throw lp::exceptions::InvalidOperationException("Failed to open SC manager (" + std::to_string(GetLastError()) + ")");
          }

          // Get a handle to the service.
          schService = OpenService(
            schSCManager,       // SCM database
            GetName(),          // name of service
            DELETE);            // need delete access

          if(schService == nullptr)
          {
            printf("OpenService failed (%d)\n", GetLastError());
            CloseServiceHandle(schSCManager);
            return;
          }

          // Delete the service.
          if(!DeleteService(schService))
          {
            printf("DeleteService failed (%d)\n", GetLastError());
          }
          else
          {
            printf("Service deleted successfully\n");
          }

          CloseServiceHandle(schService);
          CloseServiceHandle(schSCManager);
        }

        void WINAPI Service::Main(DWORD dwArgc, LPTSTR* lpszArgv)
        {
          mArgc = dwArgc;
          mArgv = lpszArgv;

          // Register the handler function for the service
          mStatusHandle = RegisterServiceCtrlHandlerEx(
            GetName(),
            StaticCtrlHandler,
            this);

          if(!mStatusHandle)
          {
            ReportEvent(TEXT("RegisterServiceCtrlHandler"));
            return;
          }

          // These SERVICE_STATUS members remain as set here
          mStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
          mStatus.dwServiceSpecificExitCode = 0;

          // Report initial status to the SCM
          ReportStatus(SERVICE_START_PENDING, NO_ERROR, 3000);

          // Perform service-specific initialization and work.
          Init();
        }

        void Service::Init()
        {
          // Create an event. The control handler function, SvcCtrlHandler,
          // signals this event when it receives the stop control code.
          mStopEventHandle = CreateEvent(
            nullptr,    // default security attributes
            true,       // manual reset event
            false,      // not signaled
            nullptr);   // no name

          if(mStopEventHandle == nullptr)
          {
            ReportStatus(SERVICE_STOPPED, NO_ERROR, 0);

            return;
          }

          // Report running status when initialization is complete.
          ReportStatus(SERVICE_RUNNING, NO_ERROR, 0);

          //std::thread lampPostThread(&Service::RunLampPost, this);

          while(!mLampPost.IsStopped())
          {
            // Check whether to stop the service.
            WaitForSingleObject(mStopEventHandle, INFINITE);
            ReportStatus(SERVICE_STOPPED, NO_ERROR, 0);

            break;
          }

          mLampPost.Stop();
          //lampPostThread.join();
        }

        void Service::RunLampPost()
        {
          mLampPost.Start();
        }

        void Service::ReportStatus(DWORD dwCurrentState, DWORD dwWin32ExitCode, DWORD dwWaitHint)
        {
          static DWORD dwCheckPoint = 1;

          // Fill in the SERVICE_STATUS structure.
          mStatus.dwCurrentState = dwCurrentState;
          mStatus.dwWin32ExitCode = dwWin32ExitCode;
          mStatus.dwWaitHint = dwWaitHint;

          if(dwCurrentState == SERVICE_START_PENDING)
          {
            mStatus.dwControlsAccepted = 0;
          }
          else
          {
            mStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;
          }

          if((dwCurrentState == SERVICE_RUNNING) || (dwCurrentState == SERVICE_STOPPED))
          {
            mStatus.dwCheckPoint = 0;
          }
          else
          {
            mStatus.dwCheckPoint = dwCheckPoint++;
          }

          // Report the status of the service to the SCM.
          ::SetServiceStatus(mStatusHandle, &mStatus);
        }

        void WINAPI Service::ControlHandler(DWORD dwCtrl)
        {
          // Handle the requested control code.
          switch(dwCtrl)
          {
            case SERVICE_CONTROL_STOP:
            {
              ReportStatus(SERVICE_STOP_PENDING, NO_ERROR, 0);

              // Signal the service to stop.
              SetEvent(mStopEventHandle);
              ReportStatus(mStatus.dwCurrentState, NO_ERROR, 0);

              return;
            }

            case SERVICE_CONTROL_INTERROGATE:
            default:
            {
              break;
            }
          }
        }

        void Service::ReportEvent(LPTSTR szFunction)
        {
          HANDLE hEventSource;
          LPCTSTR lpszStrings[2];
          TCHAR Buffer[80];

          hEventSource = RegisterEventSource(nullptr, GetName());

          if(hEventSource != nullptr)
          {
            StringCchPrintf(Buffer, 80, TEXT("%s failed with %d"), szFunction, GetLastError());

            lpszStrings[0] = GetName();
            lpszStrings[1] = Buffer;

            ::ReportEvent(hEventSource,        // event log handle
                          EVENTLOG_ERROR_TYPE, // event type
                          0,                   // event category
                          SVC_ERROR,           // event identifier
                          nullptr,             // no security identifier
                          2,                   // size of lpszStrings array
                          0,                   // no binary data
                          lpszStrings,         // array of strings
                          nullptr);            // no binary data

            DeregisterEventSource(hEventSource);
          }
        }
      }
    }
  }
}
