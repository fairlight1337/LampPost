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

      friend std::ostream& operator<<(std::ostream& outputStream, const Message& message)
      {
        outputStream << "[" << message.mSender << " to " << message.mTopic << "]: " << message.mDatagram;

        return outputStream;
      }
    };
  } // namespace messages
} // namespace lp


#endif //LAMPPOST_MESSAGE_H
