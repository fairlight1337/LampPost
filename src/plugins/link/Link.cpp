#include "Link.h"


namespace lp
{
  namespace plugins
  {
    Link::Link(PluginConfiguration configuration)
      : PluginInstance(configuration), mZmqContext(nullptr)
    {
    }

    void Link::Initialize() {
      mZmqContext = zmq_ctx_new();

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
        messages::Datagram request(std::string("Test request"));

        messages::Datagram response = mSysInfoActionConsumer->Request(request);

        if(!response.IsEmpty())
        {
          std::stringstream sts;
          sts << "Received a response: ";
          sts << response;

          mLog.Error(sts.str());
        }
        else
        {
          mLog.Error("Failed to receive a response.");
        }

        Sleep(std::chrono::seconds(1));
      }
    }

    void Link::Deinitialize()
    {
      DeleteSubscriber(mSysInfoSubscriber);
      mSysInfoSubscriber = nullptr;

      if(mZmqContext != nullptr)
      {
        zmq_ctx_shutdown(mZmqContext);
        zmq_ctx_term(mZmqContext);

        mZmqContext = nullptr;
      }
    }
  } // namespace plugins
} // namespace lp
