#include <algorithm>
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

// The sample configuration files list.
const std::list<std::string> cSampleConfigurationFilesList {
  "ConfigFilePath/1.cfg",
  "ConfigFilePath/2.cfg",
  "ConfigFilePath/3.cfg"
};


#pragma endregion


// When the constructor is called, then the properties are set correctly.
TEST(LampPostConfiguration, WhenConstructorIsCalled_ThenThePropertiesAreSetCorrectly) {
  // Arrange.
  lp::PluginManagerConfiguration pluginManagerConfiguration(cSampleTemplateSearchPaths, cSampleBus);

  // Act.
  lp::LampPostConfiguration lampPostConfiguration(pluginManagerConfiguration, cSampleConfigurationFilesList);

  // Assert.
  EXPECT_EQ(pluginManagerConfiguration, lampPostConfiguration.mPluginManagerConfiguration);
  EXPECT_TRUE(std::equal(cSampleConfigurationFilesList.begin(), cSampleConfigurationFilesList.end(), lampPostConfiguration.mConfigurationFiles.begin()));
}
