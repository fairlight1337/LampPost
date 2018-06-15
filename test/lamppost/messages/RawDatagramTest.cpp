#include <memory>
#include <string>

#include <gtest/gtest.h>

#include <lamppost/messages/RawDatagram.h>


#pragma region Constants

// The sample datagram value.
const std::string cSampleDatagramValue = "The Sample Value";

// The other sample datagram value.
const std::string cOtherSampleDatagramValue = "The Other Sample Value";

// The sample list index.
const int cSampleListIndex = 0;

#pragma endregion

#pragma region Constructor

TEST(RawDatagram, WhenDefaultConstructorIsCalled_ThenTheTypeIsEmpty)
{
  // Arrange, Act.
  lp::messages::RawDatagram datagram = std::make_shared<lp::messages::RawDatagram>();

  // Assert.
  EXPECT_EQ(lp::messages::RawDatagramType::Empty, datagram.GetType());
}

#pragma endregion // Constructor

#pragma region GetCount

TEST(RawDatagram, WhenItemIsAddedToList_ThenTheCountIncreasesByOne)
{
  // Arrange.
  lp::messages::RawDatagram datagram;
  int oldCount = datagram.GetCount();
  std::shared_ptr<lp::messages::RawDatagram> otherDatagram = std::make_shared<lp::messages::RawDatagram>();

  // Act.
  datagram.Add(otherDatagram);

  // Assert.
  EXPECT_EQ(oldCount + 1, datagram.GetCount());
}

#pragma endregion // GetCount

#pragma region Add

TEST(RawDatagram, WhenItemIsAddedAndDatagramIsNotAList_ThenAnExceptionIsThrown)
{
  // Arrange.
  lp::messages::RawDatagram datagram;
  datagram = cSampleDatagramValue;

  std::shared_ptr<lp::messages::RawDatagram> otherDatagram = std::make_shared<lp::messages::RawDatagram>();
  datagram = cOtherSampleDatagramValue;

  // Act, Assert.
  EXPECT_THROW(datagram.Add(otherDatagram), lp::exceptions::InvalidOperationException);
}

#pragma endregion // Add

#pragma region Remove

TEST(RawDatagram, WhenItemIsRemovedFromIndexAndDatagramIsNotAList_ThenAnExceptionIsThrown)
{
  // Arrange.
  lp::messages::RawDatagram datagram;
  datagram = cSampleDatagramValue;

  // Act, Assert.
  EXPECT_THROW(datagram.Remove(cSampleListIndex), lp::exceptions::InvalidOperationException);
}

TEST(RawDatagram, WhenItemIsRemovedFromIndexAndIndexIsOutOfBoundsOfList_ThenAnExceptionIsThrown)
{
  // Arrange.
  lp::messages::RawDatagram datagram; // Datagrams are empty lists by default.

  // Act, Assert.
  EXPECT_THROW(datagram.Remove(cSampleListIndex), lp::exceptions::IndexOutOfBoundsException);
}

#pragma endregion // Remove
