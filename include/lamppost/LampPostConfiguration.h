#ifndef LAMPPOST_LAMPPOSTCONFIGURATION_H
#define LAMPPOST_LAMPPOSTCONFIGURATION_H


#include <list>
#include <string>

#include <lamppost/plugin/PluginManagerConfiguration.h>


namespace lp
{
  struct LampPostConfiguration
  {
    PluginManagerConfiguration mPluginManagerConfiguration;
    std::list<std::string> mConfigurationFiles;

    LampPostConfiguration() = default;

    LampPostConfiguration(PluginManagerConfiguration pluginManagerConfiguration, std::list<std::string> configurationFiles)
    {
      mPluginManagerConfiguration = pluginManagerConfiguration;
      mConfigurationFiles = configurationFiles;
    }

    bool operator==(const LampPostConfiguration& other) const
    {
      return mPluginManagerConfiguration == other.mPluginManagerConfiguration;
    }

    bool operator!=(const LampPostConfiguration& other) const
    {
      return !(*this == other);
    }
  };
} // namespace lp


#endif //LAMPPOST_LAMPPOSTCONFIGURATION_H
