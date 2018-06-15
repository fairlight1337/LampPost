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
