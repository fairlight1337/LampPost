#ifndef LAMPPOST_PLUGINTEMPLATE_H
#define LAMPPOST_PLUGINTEMPLATE_H


#include <iostream>
#include <map>
#include <memory>
#include <string>

#include <lamppost/Identifiable.h>
#include <lamppost/PluginInstance.h>
#include <lamppost/PluginManagerConfiguration.h>
#include <lamppost/PluginTemplateConfiguration.h>
#include <lamppost/PluginTemplateInfo.h>


namespace lp
{
  class PluginTemplate : public Identifiable
  {
  private:
    PluginTemplateConfiguration mConfiguration;
    std::map<std::string, std::shared_ptr<PluginInstance>> mInstances;

    bool IndexedInstanceExists(int index);

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
