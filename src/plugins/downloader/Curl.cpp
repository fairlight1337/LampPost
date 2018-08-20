#include "Curl.h"


namespace lp
{
  namespace plugins
  {
    unsigned int Curl::gCurlUsers = 0;
    std::mutex Curl::gCurlUsersMutex;


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
  } // namespace plugins
} // namespace lp