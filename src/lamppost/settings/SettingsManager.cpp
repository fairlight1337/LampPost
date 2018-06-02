#include <lamppost/settings/SettingsManager.h>


namespace lp
{
  namespace settings
  {
    void SettingsManager::AddPluginTemplateSearchPath(std::string pluginTemplateSearchPath)
    {
      mPluginTemplateSearchPaths.push_back(pluginTemplateSearchPath);
    }

    const std::list<std::string> SettingsManager::GetPluginTemplateSearchPaths()
    {
      return mPluginTemplateSearchPaths;
    }

    void SettingsManager::ReadConfigurationFile(std::string /*path*/)
    {
      // TODO(fairlight1337): Trigger reading of configuration file here.
    }
  } // namespace settings
} // namespace lp
