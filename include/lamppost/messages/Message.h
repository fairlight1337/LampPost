#ifndef LAMPPOST_MESSAGE_H
#define LAMPPOST_MESSAGE_H


#include <string>

#include <lamppost/messages/RawDatagram.h>


namespace lp
{
  namespace messages
  {
    class Message
    {
    private:
      std::string mSender;
      std::string mTopic;
      std::shared_ptr<RawDatagram> mDatagram;

    public:
      Message(std::string sender, std::string topic, std::shared_ptr<RawDatagram> datagram);
      ~Message() = default;

      std::string GetSender();
      std::string GetTopic();
      std::shared_ptr<RawDatagram> GetDatagram();
    };
  } // namespace messages
} // namespace lp


#endif //LAMPPOST_MESSAGE_H
