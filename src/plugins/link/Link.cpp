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

      if(mZmqContext == nullptr)
      {
        mLog.Error("Error while creating ZMQ context: " + std::string(zmq_strerror(zmq_errno())));
      }

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

        mZmqServerPubSocket = CreateZmqSocket(ZMQ_PUB);
        BindZmqSocket(mZmqServerPubSocket, endpointAddressPub);

        mZmqServerSubSocket = CreateZmqSocket(ZMQ_SUB);
        BindZmqSocket(mZmqServerSubSocket, endpointAddressSub);
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

        mZmqClientPubSocket = CreateZmqSocket(ZMQ_PUB);
        ConnectZmqSocket(mZmqClientPubSocket, endpointAddressPub);

        mZmqClientSubSocket = CreateZmqSocket(ZMQ_SUB);
        ConnectZmqSocket(mZmqClientSubSocket, endpointAddressSub);
      }

      mWildcardSubscriber = GetSubscriber("/**/*"); // Receive every message.
      mWildcardSubscriber->SetMessageCallback(
        [this](messages::Message message)
        {
          mLog.Info("Got a message");
        });

      mSysInfoActionConsumer = GetActionConsumer("/sysinfoaction");
    bool Link::ConnectZmqSocket(void* zmqSocket, std::string endpoint)
    {
      if(zmq_connect(zmqSocket, endpoint.c_str()) == 0)
      {
        return true;
      }
      else
      {
        mLog.Error("Failed to connect socket to endpoint '" + endpoint + "': " + std::string(zmq_strerror(zmq_errno())));
      }

      return false;
    }

    bool Link::BindZmqSocket(void* zmqSocket, std::string endpoint)
    {
      if(zmq_bind(zmqSocket, endpoint.c_str()) == 0)
      {
        return true;
      }
      else
      {
        mLog.Error("Failed to bind socket to endpoint '" + endpoint + "': " + std::string(zmq_strerror(zmq_errno())));
      }

      return false;
    }

    void Link::Run()
    {
      while(mShouldRun)
      {
        /*messages::Datagram request(std::string("Test request"));

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
        }*/

        messages::Message receivedMessage;

        if(ReceiveMessage(mZmqServerSubSocket, receivedMessage))
        {
          // TODO(fairlight1337): Properly handle logic for receiving messages as a server here. Right now,
          // the messages inserted back into the bus here isn't marked as received through a specific link
          // and will most probably bounce back over the same connection it came in when transmitted to this
          // Link plugin instance again. This needs to be taken care of.
          GetBus()->Publish(receivedMessage);
        }

        if(ReceiveMessage(mZmqClientSubSocket, receivedMessage))
        {
          // TODO(fairlight1337): Properly handle logic for receiving messages as a client here. See comment above.
          GetBus()->Publish(receivedMessage);
        }

        Sleep(std::chrono::milliseconds(1));
      }
    }

    bool Link::ReceiveMessage(void* zmqSocket, messages::Message& receivedMessage)
    {
      bool successfullyReceivedMessage = false;
      std::shared_ptr<data::RawBytes> bytes = std::make_shared<data::RawBytes>();

      if(zmqSocket != nullptr)
      {
        zmq_msg_t messagePart = zmq_msg_t();
        int64_t more = 0;
        size_t sizeOfMore = sizeof(more);

        do
        {
          if(zmq_msg_init(&messagePart) == 0)
          {
            int receiveResult = zmq_recvmsg(zmqSocket, &messagePart, ZMQ_DONTWAIT);

            if(receiveResult == 0)
            {
              size_t messageSize = zmq_msg_size(&messagePart);
              void* messageData = zmq_msg_data(&messagePart);

              bytes->Append(messageData, messageSize);

              zmq_getsockopt(zmqSocket, ZMQ_RCVMORE, &more, &sizeOfMore);
            }
            else
            {
              more = 0;
            }

            zmq_msg_close(&messagePart);
          }
        } while(more > 0);

        if(bytes->GetSize() > 0)
        {
          receivedMessage = messages::Message::Deserialize(bytes);
          successfullyReceivedMessage = true;
        }
      }

      return successfullyReceivedMessage;
    }

    bool Link::SendZmqMessage(void* zmqSocket, messages::Message& messageToSend)
    {
      bool successfullySent = false;
      zmq_msg_t message;
      std::shared_ptr<data::RawBytes> rawBytes = messageToSend.SerializeToBytes();

      if(zmq_msg_init_size(&message, rawBytes->GetSize()) > -1)
      {
        std::memcpy(zmq_msg_data(&message), rawBytes->GetContent(), rawBytes->GetSize());

        zmq_send(zmqSocket, "A", 1, ZMQ_SNDMORE);
        int sendResult = zmq_msg_send(&message, zmqSocket, 0);

        if(sendResult == rawBytes->GetSize())
        {
          successfullySent = true;
        }
        else
        {
          int errorCode = zmq_errno();
          mLog.Error(std::string("ZMQ Error during send (received " + std::to_string(sendResult) + ", expected " + std::to_string(rawBytes->GetSize()) + "): ") + zmq_strerror(errorCode));
        }

        zmq_msg_close(&message);
      }
      else
      {
        int errorCode = zmq_errno();
        mLog.Error(std::string("ZMQ Error during message initialization: ") + zmq_strerror(errorCode));
      }

      return successfullySent;
    }

    void Link::Deinitialize()
    {
      DeleteSubscriber(mWildcardSubscriber);
      mWildcardSubscriber = nullptr;

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
