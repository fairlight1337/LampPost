#include <lamppost/messages/Message.h>


namespace lp {
  namespace messages {
    Message::Message(std::string sender, std::string topic, std::shared_ptr<lp::messages::Datagram> datagram) : mSender(sender), mTopic(topic), mDatagram(datagram) {
    }

    std::string Message::GetSender() {
      return mSender;
    }

    std::string Message::GetTopic() {
      return mTopic;
    }

    std::shared_ptr<Datagram> Message::GetDatagram() {
      return mDatagram;
    }
  } // namespace messages
} // namespace lp
