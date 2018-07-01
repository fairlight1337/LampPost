#ifndef LAMPPOST_MESSAGE_H
#define LAMPPOST_MESSAGE_H


#include <memory>
#include <string>

#include <flatbuffers/idl.h>
#include <flatbuffers/util.h>

#include <lamppost/data/RawBytes.h>
#include <lamppost/messages/Datagram.h>
#include <lamppost/schemas/FBMessage_generated.h>


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
      Message() = default;
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

      std::shared_ptr<data::RawBytes> SerializeToBytes();
      static messages::Message Deserialize(std::shared_ptr<data::RawBytes> bytesToDeserialize);

      bool operator==(const Message& rhs) const;
    };
  } // namespace messages
} // namespace lp


#endif //LAMPPOST_MESSAGE_H
