#ifndef LAMPPOST_LAMPPOSTCONFIGURATION_H
#define LAMPPOST_LAMPPOSTCONFIGURATION_H


#include <lamppost/plugin/PluginManagerConfiguration.h>


namespace lp
{
  struct LampPostConfiguration
    {
    PluginManagerConfiguration mPluginManagerConfiguration;

    LampPostConfiguration() = default;

    LampPostConfiguration(PluginManagerConfiguration pluginManagerConfiguration)
    {
      mPluginManagerConfiguration = pluginManagerConfiguration;
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
