#ifndef LAMPPOST_WINDOWS_SERVICE_H
#define LAMPPOST_WINDOWS_SERVICE_H


#include <string>

#include <strsafe.h>
#include <tchar.h>
#include <windows.h>

#include <lamppost/exceptions/InvalidOperationException.h>
#include <lamppost/LampPost.h>


#define SVC_ERROR ((DWORD)0xC0020001L)


namespace lp
{
  namespace platformspecific
  {
    namespace windows
    {
      namespace service
      {
        class Service
        {
        private:
          std::string mName;
          std::string mDescription;

          lp::LampPost mLampPost;

          SERVICE_STATUS mStatus;
          SERVICE_STATUS_HANDLE mStatusHandle;
          HANDLE mStopEventHandle = nullptr;

          DWORD mArgc;
          LPTSTR* mArgv;

          static DWORD WINAPI StaticCtrlHandler(DWORD dwControl, DWORD dwEventType, void* eventData, void* context);

          void Init();
          void WINAPI ControlHandler(DWORD dwCtrl);
          void ReportStatus(DWORD dwCurrentState, DWORD dwWin32ExitCode, DWORD dwWaitHint);

          void RunLampPost();

        public:
          Service(std::string name, std::string description);
          ~Service() = default;

          LPSTR GetName();

          void Install();
          void Uninstall();

          void WINAPI Main(DWORD dwArgc, LPTSTR* lpszArgv);
          void ReportEvent(LPTSTR szFunction);
        };
      }
    }
  }
}


#endif //LAMPPOST_WINDOWS_SERVICE_H
