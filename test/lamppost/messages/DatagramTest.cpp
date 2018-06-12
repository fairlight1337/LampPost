#include <memory>
#include <string>

#include <gtest/gtest.h>

#include <lamppost/messages/Datagram.h>


#pragma region Constants

// The sample datagram value.
const std::string cSampleDatagramValue = "The Sample Value";

// The other sample datagram value.
const std::string cOtherSampleDatagramValue = "The Other Sample Value";

// The sample list index.
const int cSampleListIndex = 0;

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

#pragma region Add

TEST(Datagram, WhenItemIsAddedAndDatagramIsNotAList_ThenAnExceptionIsThrown)
{
  // Arrange.
  lp::messages::Datagram datagram;
  datagram = cSampleDatagramValue;

  std::shared_ptr<lp::messages::Datagram> otherDatagram = std::make_shared<lp::messages::Datagram>();
  datagram = cOtherSampleDatagramValue;

  // Act, Assert.
  EXPECT_THROW(datagram.Add(otherDatagram), lp::exceptions::InvalidOperationException);
}

#pragma endregion // Add

#pragma region Remove

TEST(Datagram, WhenItemIsRemovedFromIndexAndDatagramIsNotAList_ThenAnExceptionIsThrown)
{
  // Arrange.
  lp::messages::Datagram datagram;
  datagram = cSampleDatagramValue;

  // Act, Assert.
  EXPECT_THROW(datagram.Remove(cSampleListIndex), lp::exceptions::InvalidOperationException);
}

TEST(Datagram, WhenItemIsRemovedFromIndexAndIndexIsOutOfBoundsOfList_ThenAnExceptionIsThrown)
{
  // Arrange.
  lp::messages::Datagram datagram; // Datagrams are empty lists by default.

  // Act, Assert.
  EXPECT_THROW(datagram.Remove(cSampleListIndex), lp::exceptions::IndexOutOfBoundsException);
}

#pragma endregion // Remove
