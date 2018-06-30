#include <memory>
#include <string>

#include <gtest/gtest.h>

#include <lamppost/messages/Datagram.h>


#pragma region Constants

// The sample dictionary key.
const std::string cSampleDictionaryKey = "SampleKey";

// The sample dictionary string value.
const std::string cSampleDictionaryStringValue = "SampleValue";

#pragma endregion // Constants

#pragma region Constructor

TEST(Datagram, WhenDefaultConstructorIsCalled_ThenTheTypeIsEmpty)
{
  // Arrange, Act.
  lp::messages::Datagram datagram;

  // Assert.
  EXPECT_EQ(lp::messages::RawDatagramType::Empty, datagram.GetType());
}

#pragma endregion // Constructor

#pragma region Dictionary subscript operator

TEST(Datagram, WhenValueIsSetInDictionaryViaOperator_ThenTheValueCanBeRetrievedWithTheSameKey)
{
  // Arrange.
  lp::messages::Datagram datagram;

  // Act.
  datagram[cSampleDictionaryKey] = cSampleDictionaryStringValue;

  // Assert.
  EXPECT_NO_THROW(datagram[cSampleDictionaryKey].Get<std::string>());
  EXPECT_STREQ(cSampleDictionaryStringValue.c_str(), datagram[cSampleDictionaryKey].Get<std::string>().c_str());
}

TEST(Datagram, WhenValueIsSetInDictionaryViaOperatorAndADifferentTypeIsRetrieved_ThenAnExceptionIsThrown)
{
  // Arrange.
  lp::messages::Datagram datagram;

  // Act.
  datagram[cSampleDictionaryKey] = cSampleDictionaryStringValue;

  // Assert.
  EXPECT_THROW(datagram[cSampleDictionaryKey].Get<unsigned int>(), lp::exceptions::InvalidOperationException);
  EXPECT_STREQ(cSampleDictionaryStringValue.c_str(), datagram[cSampleDictionaryKey].Get<std::string>().c_str());
}

#pragma endregion // Dictionary subscript operator

#pragma region Get

TEST(Datagram, WhenValueIsRequestedThroughPathAndThePathExists_ThenTheCorrectValueIsReturned)
{
  // Arrange.
  lp::messages::Datagram datagram;
  datagram["path_element_1"] = 25;
  datagram["path_element_2"]["path_element_3"] = std::string("test");

  // Act, Assert.
  EXPECT_EQ(25, datagram.Get<int>("path_element_1"));
  EXPECT_EQ(std::string("test"), datagram.Get<std::string>("path_element_2/path_element_3"));
}

TEST(Datagram, WhenValueIsRequestedThroughPathAndThePathDoesNotExist_ThenTheDefaultValueIsReturned)
{
  // Arrange.
  lp::messages::Datagram datagram;

  // Act, Assert.
  EXPECT_EQ(std::string("default_value"), datagram.Get<std::string>("path_element_1/path_element_2", "default_value"));
}

#pragma endregion // Get
