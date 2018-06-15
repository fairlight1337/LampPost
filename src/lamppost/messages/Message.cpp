#include <lamppost/messages/Message.h>


namespace lp {
  namespace messages {
    Message::Message(std::string sender, std::string topic, Datagram datagram)
      : mSender(std::move(sender)),
        mTopic(std::move(topic)),
        mDatagram(datagram)
    {
    }

    std::string Message::GetSender()
    {
      return mSender;
    }

    std::string Message::GetTopic()
    {
      return mTopic;
    }

    Datagram Message::GetDatagram()
    {
      return mDatagram;
    }
  } // namespace messages
} // namespace lp
