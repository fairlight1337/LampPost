#include "Link.h"


namespace lp
{
  namespace plugins
  {
    Link::Link(PluginConfiguration configuration)
      : PluginInstance(std::move(configuration)),
        mZmqContext(nullptr),
        mZmqServerSocket(nullptr)
    {
    }

    void Link::Initialize() {
      mLog.Info("Initializing ZMQ context.");
      mZmqContext = zmq_ctx_new();

      bool serverRoleIsEnabled = false;
      int serverPort = 6968;
      std::string serverInterface = "*";

      if(GetCustomConfiguration().KeyExists("server-role"))
      {
        if(GetCustomConfiguration()["server-role"].KeyExists("enabled"))
        {
          serverRoleIsEnabled = GetCustomConfiguration()["server-role"]["enabled"].Get<bool>();
        }

        if(GetCustomConfiguration()["server-role"].KeyExists("port"))
        {
          serverPort = GetCustomConfiguration()["server-role"]["port"].Get<int>();
        }

        if(GetCustomConfiguration()["server-role"].KeyExists("interface"))
        {
          serverInterface = GetCustomConfiguration()["server-role"]["interface"].Get<std::string>();
        }
      }

      if(serverRoleIsEnabled)
      {
        mZmqServerSocket = zmq_socket(mZmqContext, ZMQ_SERVER);

        std::string endpointAddress = "tcp://" + serverInterface + ":" + std::to_string(serverPort);
        mLog.Info("Enabling ZMQ server role on endpoint: " + endpointAddress);

        zmq_bind(mZmqServerSocket, endpointAddress.c_str());
      }

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
        if(mZmqServerSocket != nullptr)
        {
          mLog.Info("Stopping ZMQ server.");
          zmq_close(mZmqServerSocket);
          mZmqServerSocket = nullptr;
        }

        mLog.Info("Shutting down ZMQ.");
        zmq_ctx_shutdown(mZmqContext);

        mLog.Info("Terminating ZMQ context.");
        zmq_ctx_term(mZmqContext);

        mZmqContext = nullptr;
      }
    }
  } // namespace plugins
} // namespace lp
