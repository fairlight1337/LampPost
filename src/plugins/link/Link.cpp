#include "Link.h"


namespace lp
{
  namespace plugins
  {
    Link::Link(PluginConfiguration configuration)
      : PluginInstance(configuration)
    {
    }

    void Link::Initialize() {
      mSysInfoSubscriber = GetSubscriber("/sysinfo",
        [this](std::shared_ptr<messages::Message> message)
        {
          mLog.Info("Got a message");
        });

      mSysInfoActionConsumer = GetActionConsumer("/sysinfoaction");
    }

    void Link::Run()
    {
      while(mShouldRun)
      {
        std::shared_ptr<messages::Datagram> request = std::make_shared<messages::Datagram>();
        *request = std::string("Test request");

        std::shared_ptr<messages::Datagram> response = mSysInfoActionConsumer->Request(request);

        if(response != nullptr)
        {
          std::cout << "Received a response: " << response->Get<std::string>() << std::endl;
        }
        else
        {
          std::cout << "Failed to receive a response." << std::endl;
        }

        Sleep(std::chrono::seconds(1));
      }
    }

    void Link::Deinitialize()
    {
      DeleteSubscriber(mSysInfoSubscriber);
      mSysInfoSubscriber = nullptr;
    }
  } // namespace plugins
} // namespace lp
