#include <memory>
#include <string>

#include <gtest/gtest.h>

#include <lamppost/messages/Datagram.h>


#pragma region Constants

// The sample datagram value.
const std::string cSampleDatagramValue = "The Sample Value";

#pragma endregion

#pragma region Constructor

TEST(Datagram, WhenDefaultConstructorIsCalled_ThenTheTypeIsList)
{
  // Arrange, Act.
  lp::messages::Datagram datagram;

  // Assert.
  EXPECT_EQ(lp::messages::DatagramType::List, datagram.GetType());
}

#pragma endregion // Constructor

#pragma region GetCount

TEST(Datagram, WhenItemIsAddedToList_ThenTheCountIncreasesByOne)
{
  // Arrange.
  lp::messages::Datagram datagram;
  int oldCount = datagram.GetCount();
  std::shared_ptr<lp::messages::Datagram> otherDatagram = std::make_shared<lp::messages::Datagram>();

  // Act.
  datagram.Add(otherDatagram);

  // Assert.
  EXPECT_EQ(oldCount + 1, datagram.GetCount());
}

#pragma endregion // GetCount
