#ifndef LAMPPOST_DOWNLOADER_H
#define LAMPPOST_DOWNLOADER_H


#include <iostream>
#include <list>
#include <memory>

#include <curl/curl.h>

#include <lamppost/plugin/PluginInstance.h>
#include <lamppost/plugin/PluginTemplateInfo.h>


namespace lp
{
  namespace plugins
  {
    struct Download
    {
      std::string mInvocationId;
      std::string mUrl;
    };

    class Downloader : public PluginInstance
    {
    private:
      std::list<Download> mQueuedDownloads;
      std::shared_ptr<bus::ActionProvider> mDownloadActionProvider;

      void QueueDownload(std::string invocationId, std::string url);

    public:
      Downloader(PluginConfiguration configuration);
      ~Downloader() override = default;

      void Initialize() override;
      void Run() override;
      void Deinitialize() override;
    };
  } // namespace plugins
} // namespace lp

extern "C"
{
  GENERATE_INSTANCE_CREATOR(lp::plugins::Downloader);
  GENERATE_INFO_CREATOR(info,
    info->mIdentifier = "Downloader";
    info->mVersion = lp::Version(0, 1, 0, 0);
  )
}


#endif //LAMPPOST_DOWNLOADER_H
