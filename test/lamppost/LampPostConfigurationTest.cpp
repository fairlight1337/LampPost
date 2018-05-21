#include <memory>
#include <string>

#include <gtest/gtest.h>

#include <lamppost/LampPostConfiguration.h>
#include <lamppost/bus/Bus.h>


#pragma region Constants

// The sample bus name.
const std::string cSampleBusName("SampleBusName");

// The sample bus.
const std::shared_ptr<lp::bus::Bus> cSampleBus = std::make_shared<lp::bus::Bus>(cSampleBusName);

// The sample template search paths list.
const std::list<std::string> cSampleTemplateSearchPaths {
	"Path/1",
	"Path/2",
	"Path/3"
};

#pragma endregion


// When the constructor is called, then the properties are set correctly.
TEST(LampPostConfiguration, WhenConstructorIsCalled_ThenThePropertiesAreSetCorrectly) {
	// Arrange.
	lp::PluginManagerConfiguration pluginManagerConfiguration(cSampleTemplateSearchPaths, cSampleBus);

	// Act.
	lp::LampPostConfiguration configuration(pluginManagerConfiguration);

	// Assert.
	EXPECT_EQ(pluginManagerConfiguration, configuration.mPluginManagerConfiguration);
}
