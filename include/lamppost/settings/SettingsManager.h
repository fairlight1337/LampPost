#ifndef LAMPPOST_SETTINGSMANAGER_H
#define LAMPPOST_SETTINGSMANAGER_H


#include <list>
#include <string>


namespace lp
{
  namespace settings
  {
    class SettingsManager
    {
    private:
      std::list<std::string> mPluginTemplateSearchPaths;

    public:
      SettingsManager() = default;
      ~SettingsManager() = default;

      void AddPluginTemplateSearchPath(std::string pluginTemplateSearchPath);
      const std::list<std::string> GetPluginTemplateSearchPaths();

      void ReadConfigurationFile(std::string path);
    };
  } // namespace settings
} // namespace lp


#endif //LAMPPOST_SETTINGSMANAGER_H
