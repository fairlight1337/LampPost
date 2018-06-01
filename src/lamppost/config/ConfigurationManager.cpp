#include <lamppost/config/ConfigurationManager.h>


namespace lp
{
  namespace config
  {
    void ConfigurationManager::AddPluginTemplateSearchPath(std::string pluginTemplateSearchPath)
    {
      mPluginTemplateSearchPaths.push_back(pluginTemplateSearchPath);
    }

    const std::list<std::string> ConfigurationManager::GetPluginTemplateSearchPaths()
    {
      return mPluginTemplateSearchPaths;
    }

    void ConfigurationManager::ReadConfigurationFile(std::string /*path*/)
    {
      // TODO(fairlight1337): Trigger reading of configuration file here.
    }
  } // namespace config
} // namespace lp
