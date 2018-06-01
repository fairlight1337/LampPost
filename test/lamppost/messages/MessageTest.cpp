#include <memory>
#include <string>

#include <gtest/gtest.h>

#include <lamppost/messages/Message.h>


#pragma region Constants

// The sample sender.
const std::string cSampleSender = "TheSampleSender";

// The sample topic.
const std::string cSampleTopic = "/sampletopic";

#pragma endregion

#pragma region Constructor

TEST(Datagram, WhenConstructorIsCalled_ThenThePropertiesAreSetCorrectly)
{
  // Arrange, Act.
  lp::messages::Datagram datagram = std::make_shared<lp::datagram::Datagram>();
  lp::messages::Message message(cSampleSender, cSampleTopic, datagram);

  // Assert.
  EXPECT_EQ(cSampleSender, message.GetSender());
  EXPECT_EQ(cSampleTopic, message.GetTopic());
  EXPECT_EQ(datagram, message.GetDatagram());
}

#pragma endregion // Constructor
