#ifndef LAMPPOST_CURL_H
#define LAMPPOST_CURL_H


#include <atomic>
#include <mutex>

#include <curl/curl.h>


namespace lp
{
  namespace plugins
  {
    class Curl {
    private:
      static std::atomic<unsigned int> gCurlUsers;
      static std::mutex gCurlUsersMutex;

      CURL* mHandle;

    public:
      Curl();
      virtual ~Curl();
    };
  } // namespace plugins
} // namespace lp


#endif //LAMPPOST_CURL_H
