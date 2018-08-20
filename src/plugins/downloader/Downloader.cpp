#include "Downloader.h"


namespace lp
{
  namespace plugins
  {
    Downloader::Downloader(PluginConfiguration configuration)
      : PluginInstance(std::move(configuration))
      , mDownloadActionProvider(nullptr)
      , mCurl(mLog)
    {
    }

    void Downloader::Initialize()
    {
      mDownloadActionProvider = GetActionProvider(
        "/download",
        [this](std::shared_ptr<bus::ActionProvider> provider, std::string invocationId, messages::Datagram request)
        {
          this->QueueDownload(invocationId, request.Get<std::string>("url"));
        });
    }

    void Downloader::Run()
    {
      while(mShouldRun)
      {
        Sleep(std::chrono::seconds(1));
      }
    }

    void Downloader::Deinitialize()
    {
    }

    void Downloader::QueueDownload(std::string invocationId, std::string url)
    {
      Download dl;
      dl.mInvocationId = invocationId;
      dl.mUrl = url;

      mQueuedDownloads.emplace_back(dl);

      mLog.Info("Queueing download: " + url);
    }
  } // namespace plugins
} // namespace lp
