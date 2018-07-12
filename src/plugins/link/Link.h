#ifndef LAMPPOST_LINK_H
#define LAMPPOST_LINK_H


#include <iostream>
#include <memory>
#include <sstream>

#include <zmq.h>

#include <lamppost/plugin/PluginInstance.h>
#include <lamppost/plugin/PluginTemplateInfo.h>


namespace lp
{
  namespace plugins
  {
    class Link : public PluginInstance
    {
    private:
      std::shared_ptr<bus::Subscriber> mSysInfoSubscriber;
      std::shared_ptr<bus::ActionConsumer> mSysInfoActionConsumer;

      void* mZmqContext;
      void* mZmqServerPubSocket;
      void* mZmqServerSubSocket;
      void* mZmqClientPubSocket;
      void* mZmqClientSubSocket;

      bool ReceiveMessage(void* zmqSocket, messages::Message& receivedMessage);
      bool SendZmqMessage(void* zmqSocket, messages::Message& messageToSend);

      void* CreateZmqSocket(int zmqSocketType);
      bool ConnectZmqSocket(void* zmqSocket, std::string endpoint);
      bool BindZmqSocket(void* zmqSocket, std::string endpoint);

    public:
      Link(PluginConfiguration configuration);
      ~Link() override = default;

      void Initialize() override;
      void Run() override;
      void Deinitialize() override;
    };
  } // namespace plugins
} // namespace lp

extern "C"
{
  GENERATE_INSTANCE_CREATOR(lp::plugins::Link);
  GENERATE_INFO_CREATOR(info,
    info->mIdentifier = "Link";
    info->mVersion = lp::Version(0, 1, 0, 0);
  )
}


#endif //LAMPPOST_LINK_H
