#ifndef LAMPPOST_MESSAGE_H
#define LAMPPOST_MESSAGE_H


#include <string>

#include <lamppost/messages/Datagram.h>


namespace lp
{
  namespace messages
  {
    class Message
    {
    private:
      std::string mSender;
      std::string mTopic;
      Datagram mDatagram;

    public:
      Message(std::string sender, std::string topic, Datagram datagram);
      ~Message() = default;

      std::string GetSender();
      std::string GetTopic();
      Datagram GetDatagram();
    };
  } // namespace messages
} // namespace lp


#endif //LAMPPOST_MESSAGE_H
