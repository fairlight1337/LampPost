#include <memory>
#include <string>

#include <gtest/gtest.h>

#include <lamppost/messages/Message.h>


#pragma region Constants

// The sample sender.
const std::string cSampleSender = "TheSampleSender";

// The sample topic.
const std::string cSampleTopic = "/sampletopic";

// The sample datagram content.
const std::string cSampleDatagramContent = "Some datagram content.";

#pragma endregion

#pragma region Constructor

TEST(Datagram, WhenConstructorIsCalled_ThenThePropertiesAreSetCorrectly)
{
  // Arrange, Act.
  lp::messages::Datagram datagram = cSampleDatagramContent;
  lp::messages::Message message(cSampleSender, cSampleTopic, datagram);

  // Assert.
  EXPECT_EQ(cSampleSender, message.GetSender());
  EXPECT_EQ(cSampleTopic, message.GetTopic());
  EXPECT_EQ(datagram.Get<std::string>(), message.GetDatagram().Get<std::string>());
}

#pragma endregion // Constructor

#pragma region SerializeToBytes, Deserialize

TEST(Datagram, WhenMessageIsSerializedToBytesAndIsDeserializedAgain_ThenTheResultIsEqualToTheOriginalData)
{
  // Arrange.
  lp::messages::Datagram originalDatagram;
  originalDatagram["test1"] = 5;
  originalDatagram["test 2"]["hello world"] = std::string("99!");

  lp::messages::Message originalMessage("sender", "topic", originalDatagram);

  // Act.
  std::shared_ptr<lp::data::RawBytes> bytes = originalMessage.SerializeToBytes();
  lp::messages::Message resultMessage = lp::messages::Message::Deserialize(bytes);

  // Assert.
  EXPECT_EQ(originalMessage, resultMessage);
}

#pragma endregion // SerializeToBytes, Deserialize