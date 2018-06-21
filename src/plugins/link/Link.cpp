#include "Link.h"


namespace lp
{
  namespace plugins
  {
    Link::Link(PluginConfiguration configuration)
      : PluginInstance(configuration)
    {
    }

    void Link::Initialize() {
      mSysInfoSubscriber = GetSubscriber("/sysinfo");
      mSysInfoSubscriber->SetMessageCallback(
        [this](messages::Message message)
        {
          mLog.Info("Got a message");
        });

      mSysInfoActionConsumer = GetActionConsumer("/sysinfoaction");
    }

    void Link::Run()
    {
      while(mShouldRun)
      {
        messages::Datagram request(std::string("Test request"));

        messages::Datagram response = mSysInfoActionConsumer->Request(request);

        if(!response.IsEmpty())
        {
          std::stringstream sts;
          sts << "Received a response: ";
          sts << response;

          mLog.Error(sts.str());
        }
        else
        {
          mLog.Error("Failed to receive a response.");
        }

        Sleep(std::chrono::seconds(1));
      }
    }

    void Link::Deinitialize()
    {
      DeleteSubscriber(mSysInfoSubscriber);
      mSysInfoSubscriber = nullptr;
    }

    std::shared_ptr<data::RawBytes> Link::Serialize(messages::Message messageToSerialize)
    {
      flatbuffers::FlatBufferBuilder flatBufferBuilder;
      flatbuffers::Offset<schemas::Message> serializedMessage = schemas::CreateMessage(
        flatBufferBuilder,
        flatBufferBuilder.CreateString(messageToSerialize.GetSender()),
        flatBufferBuilder.CreateString(messageToSerialize.GetTopic()));

      // TODO(fairlight1337): Add recursive serialization of datagram here!

      flatBufferBuilder.Finish(serializedMessage);

      return std::make_shared<data::RawBytes>(flatBufferBuilder.GetBufferPointer(), flatBufferBuilder.GetSize());
    }

    messages::Message Link::Deserialize(std::shared_ptr<data::RawBytes> bytesToDeserialize)
    {
      const schemas::Message* deserializedMessage = schemas::GetMessage(bytesToDeserialize->GetContent());

      // TODO(fairlight1337): Add recursive deserialization of datagram here!

      messages::Datagram datagram;
      messages::Message message(deserializedMessage->sender()->str(), deserializedMessage->topic()->str(), datagram);

      return message;
    }
  } // namespace plugins
} // namespace lp
