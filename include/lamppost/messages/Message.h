#ifndef LAMPPOST_MESSAGE_H
#define LAMPPOST_MESSAGE_H


#include <string>

#include <lamppost/messages/Datagram.h>


namespace lp {
  namespace messages {
    class Message {
    private:
      std::string mSender;
      std::string mTopic;
      std::shared_ptr<Datagram> mDatagram;

    public:
      Message(std::string sender, std::string topic, std::shared_ptr<Datagram> datagram);
      ~Message() = default;

      std::string GetSender();
      std::string GetTopic();
      std::shared_ptr<Datagram> GetDatagram();
    };
  }
}


#endif //LAMPPOST_MESSAGE_H
