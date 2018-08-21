#ifndef LAMPPOST_CURL_H
#define LAMPPOST_CURL_H


#include <mutex>

#include <lamppost/log/Log.h>
#include <lamppost/utilities/Uuid.h>

#include <curl/curl.h>


namespace lp
{
  namespace plugins
  {
    class Curl
    {
    public:
      enum class Result
      {
        Finished = 0,
        Failed = 1
      };

    private:
      static unsigned int gCurlUsers;
      static std::mutex gCurlUsersMutex;

      log::Log mLog;

      CURL* mHandle;

      static size_t HandleDownloadedData(void* data, size_t size, size_t nmemb, void* userData);

    public:
      Curl(log::Log log);
      virtual ~Curl();

      utilities::Uuid DownloadUrl(std::string url, std::function<void(utilities::Uuid, Result)> callback);
    };
  } // namespace plugins
} // namespace lp


#endif //LAMPPOST_CURL_H
