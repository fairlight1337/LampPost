#include <memory>
#include <string>

#include <gtest/gtest.h>

#include <lamppost/PluginManagerConfiguration.h>
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
TEST(PluginManagerConfiguration, WhenConstructorIsCalled_ThenThePropertiesAreSetCorrectly) {
  // Arrange, Act.
  lp::PluginManagerConfiguration pluginManagerConfiguration(cSampleTemplateSearchPaths, cSampleBus);

  // Assert.
  EXPECT_TRUE(std::equal(cSampleTemplateSearchPaths.begin(),
                         cSampleTemplateSearchPaths.end(),
                         pluginManagerConfiguration.mTemplateSearchPaths.begin()));
  EXPECT_EQ(cSampleBus, pluginManagerConfiguration.mBus);
}
