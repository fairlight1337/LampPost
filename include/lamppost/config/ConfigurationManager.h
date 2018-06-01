#ifndef LAMPPOST_CONFIGURATIONMANAGER_H
#define LAMPPOST_CONFIGURATIONMANAGER_H


#include <list>
#include <string>


namespace lp
{
  namespace config
  {
    class ConfigurationManager
    {
    private:
      std::list<std::string> mPluginTemplateSearchPaths;

    public:
      ConfigurationManager() = default;
      ~ConfigurationManager() = default;

      void AddPluginTemplateSearchPath(std::string pluginTemplateSearchPath);
      const std::list<std::string> GetPluginTemplateSearchPaths();

      void ReadConfigurationFile(std::string path);
    };
  } // namespace config
} // namespace lp


#endif //LAMPPOST_CONFIGURATIONMANAGER_H
