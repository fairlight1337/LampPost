#include <memory>
#include <string>

#include <gtest/gtest.h>

#include <lamppost/bus/Bus.h>


#pragma region Constants

// The sample bus name.
const std::string cSampleBusName("TheBusName");

#pragma endregion


// When the simple constructor is called, then the properties are set correctly.
TEST(Bus, WhenSimpleConstructorIsCalled_ThenThePropertiesAreSetCorrectly) {
	// Arrange, Act.
	lp::bus::Bus bus(cSampleBusName);

	// Assert.
	EXPECT_EQ(cSampleBusName, bus.GetName());
}

// When the extended constructor is called, then the properties are set correctly.
TEST(Bus, WhenExtendedConstructorIsCalled_ThenThePropertiesAreSetCorrectly) {
	// Arrange.
	std::function<void(std::shared_ptr<lp::messages::Message>)> publishingFunction =
		[](std::shared_ptr<lp::messages::Message> message) {};

	// Act.
	lp::bus::Bus bus(cSampleBusName, publishingFunction);

	// Assert.
	EXPECT_EQ(cSampleBusName, bus.GetName());
	// TODO: This still somehow needs to make sure that the publishing function is set correctly.
}
