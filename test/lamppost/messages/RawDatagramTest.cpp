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

#pragma region Equality

TEST(RawDatagram, WhenTwoValueInstancesHaveTheSameContent_ThenTheyAreEqual)
{
  // Arrange.
  lp::messages::RawDatagram datagram1 = std::string("Test");
  lp::messages::RawDatagram datagram2 = std::string("Test");

  // Act, Assert.
  EXPECT_EQ(datagram1, datagram2);
}

TEST(RawDatagram, WhenTwoListInstancesHaveTheSameContent_ThenTheyAreEqual)
{
  // Arrange.
  lp::messages::RawDatagram datagram1;
  datagram1.Add(std::make_shared<lp::messages::RawDatagram>(std::string("List item 1")));
  datagram1.Add(std::make_shared<lp::messages::RawDatagram>(std::string("List item 2")));
  datagram1.Add(std::make_shared<lp::messages::RawDatagram>(std::string("List item 3")));

  lp::messages::RawDatagram datagram2;
  datagram2.Add(std::make_shared<lp::messages::RawDatagram>(std::string("List item 1")));
  datagram2.Add(std::make_shared<lp::messages::RawDatagram>(std::string("List item 2")));
  datagram2.Add(std::make_shared<lp::messages::RawDatagram>(std::string("List item 3")));

  // Act, Assert.
  EXPECT_EQ(datagram1, datagram2);
}

TEST(RawDatagram, WhenTwoDictionaryInstancesHaveTheSameContent_ThenTheyAreEqual)
{
  // Arrange.
  lp::messages::RawDatagram datagram1;
  datagram1["Key 1"] = std::make_shared<lp::messages::RawDatagram>(std::string("Value 1"));
  datagram1["Key 2"] = std::make_shared<lp::messages::RawDatagram>(std::string("Value 2"));
  datagram1["Key 3"] = std::make_shared<lp::messages::RawDatagram>(std::string("Value 3"));

  lp::messages::RawDatagram datagram2;
  datagram2["Key 1"] = std::make_shared<lp::messages::RawDatagram>(std::string("Value 1"));
  datagram2["Key 2"] = std::make_shared<lp::messages::RawDatagram>(std::string("Value 2"));
  datagram2["Key 3"] = std::make_shared<lp::messages::RawDatagram>(std::string("Value 3"));

  // Act, Assert.
  EXPECT_EQ(datagram1, datagram2);
}

#pragma endregion // Equality

#pragma region Inequality

TEST(RawDatagram, WhenTwoValueInstancesHaveDifferentContent_ThenTheyAreNotEqual)
{
  // Arrange.
  lp::messages::RawDatagram datagram1 = std::string("Test 1");
  lp::messages::RawDatagram datagram2 = std::string("Test 2");

  // Act, Assert.
  EXPECT_NE(datagram1, datagram2);
}

TEST(RawDatagram, WhenTwoListInstancesHaveDifferentContent_ThenTheyAreNotEqual)
{
  // Arrange.
  lp::messages::RawDatagram datagram1;
  datagram1.Add(std::make_shared<lp::messages::RawDatagram>(std::string("List item 1")));
  datagram1.Add(std::make_shared<lp::messages::RawDatagram>(std::string("List item 2")));
  datagram1.Add(std::make_shared<lp::messages::RawDatagram>(std::string("List item 3")));

  lp::messages::RawDatagram datagram2;
  datagram2.Add(std::make_shared<lp::messages::RawDatagram>(std::string("List item A")));
  datagram2.Add(std::make_shared<lp::messages::RawDatagram>(std::string("List item B")));
  datagram2.Add(std::make_shared<lp::messages::RawDatagram>(std::string("List item C")));

  // Act, Assert.
  EXPECT_NE(datagram1, datagram2);
}

TEST(RawDatagram, WhenTwoDictionaryInstancesHaveDifferentContent_ThenTheyAreNotEqual)
{
  // Arrange.
  lp::messages::RawDatagram datagram1;
  datagram1["Key 1"] = std::make_shared<lp::messages::RawDatagram>(std::string("Value 1"));
  datagram1["Key 2"] = std::make_shared<lp::messages::RawDatagram>(std::string("Value 2"));
  datagram1["Key 3"] = std::make_shared<lp::messages::RawDatagram>(std::string("Value 3"));

  lp::messages::RawDatagram datagram2;
  datagram2["Key A"] = std::make_shared<lp::messages::RawDatagram>(std::string("Value A"));
  datagram2["Key B"] = std::make_shared<lp::messages::RawDatagram>(std::string("Value B"));
  datagram2["Key C"] = std::make_shared<lp::messages::RawDatagram>(std::string("Value C"));

  // Act, Assert.
  EXPECT_NE(datagram1, datagram2);
}

#pragma endregion // Inequality