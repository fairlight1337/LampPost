#include "SysInfo.h"


namespace lp
{
  namespace plugins
  {
    SysInfo::SysInfo(PluginConfiguration configuration)
      : PluginInstance(std::move(configuration)),
        mSysInfoPublisher(nullptr),
        mSysInfoSubscriber(nullptr),
        mSysInfoActionProvider(nullptr),
        mSendDatagram(false)
    {
    }

    void SysInfo::Initialize()
    {
      mSysInfoPublisher = GetPublisher("/sysinfo");
      mSysInfoSubscriber = GetSubscriber(
        "/sysinfo",
        [this](messages::Datagram datagram)
        {
          std::string message = datagram.Get<std::string>();
          mLog.Info("Got it: " + message);
        });

      mSysInfoActionProvider = GetActionProvider(
        "/sysinfoaction",
        [this](std::shared_ptr<bus::ActionProvider> provider, std::string invocationId, messages::Datagram request)
        {
          std::stringstream sts;
          sts << std::string("Received action request: ");
          sts << request;
          mLog.Info(sts.str());

          messages::Datagram response(std::string("This is the response message."));

          provider->Respond(invocationId, response);
        });

      mDownloadActionConsumer = GetActionConsumer("/download");

      mSendDatagram = GetCustomConfiguration().Get<bool>("send-datagram", false);
    }

    void SysInfo::Run()
    {
      while(mShouldRun)
      {
        if(mSendDatagram)
        {
          messages::Datagram datagram(std::string("Hello Bus!"));

          mSysInfoPublisher->Publish(datagram);
          mLog.Info("Sent datagram");

          messages::Datagram download_datagram;
          download_datagram["url"] = std::string("https://www.google.de/");

          mDownloadActionConsumer->Request(download_datagram, 1000);
        }

        Sleep(std::chrono::seconds(2));
      }
    }

    void SysInfo::Deinitialize()
    {
      DeleteSubscriber(mSysInfoSubscriber);
      mSysInfoSubscriber = nullptr;

      DeletePublisher(mSysInfoPublisher);
      mSysInfoPublisher = nullptr;
    }
  } // namespace plugins
} // namespace lp
