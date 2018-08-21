#include "Curl.h"


namespace lp
{
  namespace plugins
  {
    unsigned int Curl::gCurlUsers = 0;
    std::mutex Curl::gCurlUsersMutex;


    size_t Curl::HandleDownloadedData(void* data, size_t size, size_t nmemb, void* userData)
    {
      unsigned int totalLength = size * nmemb;

      // ...

      return totalLength;
    }


    Curl::Curl(log::Log log)
      : mLog(log)
      , mHandle(nullptr)
    {
      {
        std::lock_guard<std::mutex> lock(gCurlUsersMutex);

        gCurlUsers++;

        if(gCurlUsers == 1)
        {
          curl_global_init(CURL_GLOBAL_ALL);

          mLog.Info("Globally initialized Curl.");
        }
      }

      mHandle = curl_easy_init();
    }

    Curl::~Curl()
    {
      if(mHandle != nullptr)
      {
        curl_easy_cleanup(mHandle);
      }

      {
        std::lock_guard<std::mutex> lock(gCurlUsersMutex);

        gCurlUsers--;

        if(gCurlUsers == 0)
        {
          curl_global_cleanup();

          mLog.Info("Globally deinitialized Curl.");
        }
      }
    }

    utilities::Uuid Curl::DownloadUrl(std::string url, std::function<void(utilities::Uuid, Curl::Result)> callback)
    {
      utilities::Uuid uuid = utilities::Uuid::CreateUuid();

      // ...

      return uuid;
    }
  } // namespace plugins
} // namespace lp