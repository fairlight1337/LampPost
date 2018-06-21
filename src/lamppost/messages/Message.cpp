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

    std::shared_ptr<data::RawBytes> Message::Serialize()
    {
      flatbuffers::FlatBufferBuilder flatBufferBuilder;
      flatbuffers::Offset<schemas::FBMessage> serializedMessage = schemas::CreateFBMessage(
        flatBufferBuilder,
        flatBufferBuilder.CreateString(GetSender()),
        flatBufferBuilder.CreateString(GetTopic()));

      // TODO(fairlight1337): Add recursive serialization of datagram here!

      flatBufferBuilder.Finish(serializedMessage);

      return std::make_shared<data::RawBytes>(flatBufferBuilder.GetBufferPointer(), flatBufferBuilder.GetSize());
    }

    messages::Message Message::Deserialize(std::shared_ptr<data::RawBytes> bytesToDeserialize)
    {
      const schemas::FBMessage* deserializedMessage = schemas::GetFBMessage(bytesToDeserialize->GetContent());

      // TODO(fairlight1337): Add recursive deserialization of datagram here!

      messages::Datagram datagram;
      messages::Message message(deserializedMessage->sender()->str(), deserializedMessage->topic()->str(), datagram);

      return message;
    }
  } // namespace messages
} // namespace lp
