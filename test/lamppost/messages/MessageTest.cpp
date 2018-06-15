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
