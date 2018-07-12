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

    std::shared_ptr<data::RawBytes> Message::SerializeToBytes()
    {
      flatbuffers::FlatBufferBuilder flatBufferBuilder;

      flatbuffers::Offset<flatbuffers::String> sender = flatBufferBuilder.CreateString(GetSender());
      flatbuffers::Offset<flatbuffers::String> topic = flatBufferBuilder.CreateString(GetTopic());
      flatbuffers::Offset<schemas::FBDatagram> serializedDatagram = mDatagram.SerializeToStructure(flatBufferBuilder);

      flatbuffers::Offset<schemas::FBMessage> serializedMessage = schemas::CreateFBMessage(
        flatBufferBuilder, sender, topic, serializedDatagram);

      flatBufferBuilder.Finish(serializedMessage);

      return std::make_shared<data::RawBytes>(flatBufferBuilder.GetBufferPointer(), flatBufferBuilder.GetSize());
    }

    messages::Message Message::Deserialize(std::shared_ptr<data::RawBytes> bytesToDeserialize)
    {
      const schemas::FBMessage* deserializedMessage = schemas::GetFBMessage(bytesToDeserialize->GetContent());

      return messages::Message(
        deserializedMessage->sender()->str(),
        deserializedMessage->topic()->str(),
        Datagram::DeserializeFromStructure(deserializedMessage->datagram()));
    }

    bool Message::operator==(const Message& rhs) const
    {
      return rhs.mSender == mSender && rhs.mTopic == mTopic && rhs.mDatagram == mDatagram;
    }

    void Message::PrependSender(std::string senderName)
    {
      mSender = senderName + "/" + mSender;
    }

    void Message::RemoveSender(std::string senderName)
    {
      if(mSender.substr(0, senderName.size() + 1) == senderName + "/")
      {
        mSender = mSender.substr(senderName.size() + 1);
      }
    }

    bool Message::WasSentBySender(std::string senderName)
    {
      return mSender.substr(0, senderName.size() + 1) == senderName + "/";
    }
  } // namespace messages
} // namespace lp
