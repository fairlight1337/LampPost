#ifndef LAMPPOST_PLUGINTEMPLATE_H
#define LAMPPOST_PLUGINTEMPLATE_H


#include <iostream>
#include <map>
#include <memory>
#include <string>

#include <lamppost/Identifiable.h>
#include <lamppost/plugin/PluginInstance.h>
#include <lamppost/plugin/PluginManagerConfiguration.h>
#include <lamppost/plugin/PluginTemplateConfiguration.h>
#include <lamppost/plugin/PluginTemplateInfo.h>


namespace lp
{
  class PluginTemplate : public Identifiable
  {
  private:
    PluginTemplateConfiguration mConfiguration;
    std::map<std::string, std::shared_ptr<PluginInstance>> mInstances;

    bool IndexedInstanceExists(int index);

    void Stop();
    void Deinitialize();

  public:
    PluginTemplate(PluginTemplateConfiguration configuration);
    ~PluginTemplate();

    std::shared_ptr<PluginInstance> Instantiate(PluginConfiguration configuration, std::shared_ptr<bus::Bus> parentBus);

    bool IsValid();
    std::string GetType();

    void Unload();
  };
} // namespace lp

#endif //LAMPPOST_PLUGINTEMPLATE_H
