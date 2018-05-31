#ifndef LAMPPOST_PLUGINTEMPLATECONFIGURATION_H
#define LAMPPOST_PLUGINTEMPLATECONFIGURATION_H


#include <functional>
#include <string>

#include <lamppost/PluginInstance.h>
#include <lamppost/Version.h>


namespace lp
{
  struct PluginTemplateConfiguration
  {
    std::string mIdentifier;
    Version mVersion;

    std::function<std::shared_ptr<PluginInstance>(PluginConfiguration)> mInstantiateFunction;

    PluginTemplateConfiguration(
      std::string identifier,
      Version version,
      std::function<std::shared_ptr<PluginInstance>(PluginConfiguration)> instantiateFunction)
    {
      mIdentifier = identifier;
      mVersion = version;
      mInstantiateFunction = instantiateFunction;
    }
  };
} // namespace lp


#endif //LAMPPOST_PLUGINTEMPLATECONFIGURATION_H
