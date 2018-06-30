#include "Link.h"


namespace lp
{
  namespace plugins
  {
    Link::Link(PluginConfiguration configuration)
      : PluginInstance(std::move(configuration)),
        mZmqContext(nullptr),
        mZmqServerPubSocket(nullptr),
        mZmqServerSubSocket(nullptr),
        mZmqClientPubSocket(nullptr),
        mZmqClientSubSocket(nullptr)
    {
    }

    void Link::Initialize() {
      mLog.Info("Initializing ZMQ context.");
      mZmqContext = zmq_ctx_new();

      bool serverRoleIsEnabled = GetCustomConfiguration().Get<bool>("server-role/enabled", false);
      int serverPortPub = GetCustomConfiguration().Get<int>("server-role/port-pub", 6967);
      int serverPortSub = GetCustomConfiguration().Get<int>("server-role/port-sub", 6968);
      std::string serverInterface = GetCustomConfiguration().Get<std::string>("server-role/interface", "*");

      if(serverRoleIsEnabled)
      {
        std::string endpointAddressPub = "tcp://" + serverInterface + ":" + std::to_string(serverPortPub);
        std::string endpointAddressSub = "tcp://" + serverInterface + ":" + std::to_string(serverPortSub);

        mLog.Info("Enabling ZMQ server role:");
        mLog.Info("Publish: " + endpointAddressPub, 1);
        mLog.Info("Subscribe: " + endpointAddressSub, 1);

        mZmqServerPubSocket = zmq_socket(mZmqContext, ZMQ_PUB);
        zmq_bind(mZmqServerPubSocket, endpointAddressPub.c_str());

        mZmqServerSubSocket = zmq_socket(mZmqContext, ZMQ_SUB);
        zmq_bind(mZmqServerSubSocket, endpointAddressSub.c_str());
      }

      bool clientRoleIsEnabled = GetCustomConfiguration().Get<bool>("client-role/enabled", false);
      int clientPortPub = GetCustomConfiguration().Get<int>("client-role/port-pub", 6967);
      int clientPortSub = GetCustomConfiguration().Get<int>("client-role/port-sub", 6968);
      std::string clientServerHost = GetCustomConfiguration().Get<std::string>("client-role/server-host", "localhost");

      if(clientRoleIsEnabled)
      {
        std::string endpointAddressPub = "tcp://" + clientServerHost + ":" + std::to_string(clientPortPub);
        std::string endpointAddressSub = "tcp://" + clientServerHost + ":" + std::to_string(clientPortSub);

        mLog.Info("Enabling ZMQ client role:");
        mLog.Info("Publish: " + endpointAddressPub, 1);
        mLog.Info("Subscribe: " + endpointAddressSub, 1);

        mZmqClientPubSocket = zmq_socket(mZmqContext, ZMQ_PUB);
        zmq_connect(mZmqClientPubSocket, endpointAddressPub.c_str());

        mZmqClientSubSocket = zmq_socket(mZmqContext, ZMQ_SUB);
        zmq_connect(mZmqClientSubSocket, endpointAddressSub.c_str());
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
        if(mZmqServerPubSocket != nullptr)
        {
          zmq_close(mZmqServerPubSocket);
          mZmqServerPubSocket = nullptr;
        }

        if(mZmqServerSubSocket != nullptr)
        {
          zmq_close(mZmqServerSubSocket);
          mZmqServerSubSocket = nullptr;
        }

        if(mZmqClientPubSocket != nullptr)
        {
          zmq_close(mZmqClientPubSocket);
          mZmqClientPubSocket = nullptr;
        }

        if(mZmqClientSubSocket != nullptr)
        {
          zmq_close(mZmqClientSubSocket);
          mZmqClientSubSocket = nullptr;
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
