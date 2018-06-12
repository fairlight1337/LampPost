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
        [this](std::shared_ptr<messages::RawDatagram> datagram)
        {
          std::string message = datagram->Get<std::string>();
          mLog.Info("Got it: " + message);
        });

      mSysInfoActionProvider = GetActionProvider(
        "/sysinfoaction",
        [this](std::shared_ptr<bus::ActionProvider> provider, std::string invocationId, std::shared_ptr<messages::RawDatagram> request)
        {
          std::cout << "Received action request. Replying.";

          std::shared_ptr<messages::RawDatagram> response = std::make_shared<messages::RawDatagram>();
          (*response) = std::string("Reponse message.");

          provider->Respond(invocationId, response);
        });
    }

    void SysInfo::Run()
    {
      while(mShouldRun)
      {
        std::shared_ptr<messages::RawDatagram> datagram = std::make_shared<messages::RawDatagram>();
        *datagram = std::string("Hello Bus!");

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
