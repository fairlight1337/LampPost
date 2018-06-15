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
      mSysInfoSubscriber = GetSubscriber("/sysinfo");
      mSysInfoSubscriber->SetMessageCallback(
        [this](messages::Message message)
        {
          mLog.Info("Got a message");
        });

      mSysInfoActionConsumer = GetActionConsumer("/sysinfoaction");
    }

    void Link::Run()
    {
      while(mShouldRun)
      {
        messages::Datagram request;
        request = std::string("Test request");

        messages::Datagram response = mSysInfoActionConsumer->Request(request);

        if(!response.IsEmpty())
        {
          std::cout << "Received a response: " << response.Get<std::string>() << std::endl;
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
