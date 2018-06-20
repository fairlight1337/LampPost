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
  lp::messages::RawDatagram rawDatagram;

  // Assert.
  EXPECT_EQ(lp::messages::RawDatagramType::Empty, rawDatagram.GetType());
}

#pragma endregion // Constructor

#pragma region GetCount

TEST(RawDatagram, WhenItemIsAddedToList_ThenTheCountIncreasesByOne)
{
  // Arrange.
  lp::messages::RawDatagram rawDatagram(lp::messages::RawDatagramType::List);
  std::shared_ptr<lp::messages::RawDatagram> otherRawDatagram = std::make_shared<lp::messages::RawDatagram>();
  int oldCount = rawDatagram.GetCount();

  // Act.
  rawDatagram.Add(otherRawDatagram);

  // Assert.
  EXPECT_EQ(oldCount + 1, rawDatagram.GetCount());
}

#pragma endregion // GetCount

#pragma region Remove

TEST(RawDatagram, WhenItemIsRemovedFromIndexAndDatagramIsNotAList_ThenAnExceptionIsThrown)
{
  // Arrange.
  lp::messages::RawDatagram rawDatagram(lp::messages::RawDatagramType::Value);

  // Act, Assert.
  EXPECT_THROW(rawDatagram.Remove(cSampleListIndex), lp::exceptions::InvalidOperationException);
}

TEST(RawDatagram, WhenItemIsRemovedFromIndexAndIndexIsOutOfBoundsOfList_ThenAnExceptionIsThrown)
{
  // Arrange.
  lp::messages::RawDatagram rawDatagram(lp::messages::RawDatagramType::List);

  // Act, Assert.
  EXPECT_THROW(rawDatagram.Remove(cSampleListIndex), lp::exceptions::IndexOutOfBoundsException);
}

#pragma endregion // Remove
