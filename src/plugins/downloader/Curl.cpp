#include "Curl.h"


namespace lp
{
  namespace plugins
  {
    std::atomic<unsigned int> Curl::gCurlUsers = 0;
    std::mutex Curl::gCurlUsersMutex;


    Curl::Curl()
      : mHandle(nullptr)
    {
      {
        std::lock_guard<std::mutex> lock(gCurlUsersMutex);

        gCurlUsers++;

        if(gCurlUsers == 1)
        {
          curl_global_init(CURL_GLOBAL_ALL);
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
        }
      }
    }
  }
}