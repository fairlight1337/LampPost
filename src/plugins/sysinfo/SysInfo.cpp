#include "SysInfo.h"


namespace lp
{
  namespace plugins
  {
    SysInfo::SysInfo(PluginConfiguration configuration)
      : PluginInstance(configuration)
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
          std::cout << "Received action request. Replying.";

          messages::Datagram response;
          response = std::string("Reponse message.");

          provider->Respond(invocationId, response);
        });
    }

    void SysInfo::Run()
    {
      while(mShouldRun)
      {
        messages::Datagram datagram;
        datagram = std::string("Hello Bus!");

        mSysInfoPublisher->Publish(datagram);
        mLog.Info("Sent datagram");

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
