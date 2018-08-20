#ifndef LAMPPOST_CURL_H
#define LAMPPOST_CURL_H


#include <mutex>

#include <lamppost/log/Log.h>

#include <curl/curl.h>


namespace lp
{
  namespace plugins
  {
    class Curl {
    private:
      static unsigned int gCurlUsers;
      static std::mutex gCurlUsersMutex;

      log::Log mLog;

      CURL* mHandle;

    public:
      Curl(log::Log log);
      virtual ~Curl();
    };
  } // namespace plugins
} // namespace lp


#endif //LAMPPOST_CURL_H
