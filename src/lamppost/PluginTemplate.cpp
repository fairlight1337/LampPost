#include <lamppost/PluginTemplate.h>


namespace lp {
  PluginTemplate::PluginTemplate(PluginTemplateConfiguration configuration)
    : Identifiable(configuration.mIdentifier), mConfiguration(configuration) {
  }

  PluginTemplate::~PluginTemplate() {
    Unload();
  }

  bool PluginTemplate::IndexedInstanceExists(int index) {
    std::string identifier = mConfiguration.mIdentifier + std::to_string(index);

    return mInstances.find(identifier) != mInstances.end();
  }

  std::shared_ptr<PluginInstance> PluginTemplate::Instantiate(PluginConfiguration configuration, std::shared_ptr<bus::Bus> parentBus) {
    int index = 0;

    while(IndexedInstanceExists(index)) {
      index++;
    }

    configuration.mIdentifier = mConfiguration.mIdentifier + std::to_string(index);
    configuration.mBus = parentBus->CreateChildBus(configuration.mIdentifier);

    mInstances[configuration.mIdentifier] = mConfiguration.mInstantiateFunction(configuration);

    return mInstances[configuration.mIdentifier];
  }

  void PluginTemplate::Stop() {
    for(std::pair<std::string, std::shared_ptr<PluginInstance>> instancePair : mInstances) {
      instancePair.second->Stop();
    }
  }

  void PluginTemplate::Deinitialize() {
    for(std::pair<std::string, std::shared_ptr<PluginInstance>> instancePair : mInstances) {
      instancePair.second->Deinitialize();
    }

    mInstances.clear();
  }

  void PluginTemplate::Unload() {
    mConfiguration.mInstantiateFunction = nullptr;
    
    Stop();
    Deinitialize();
  }
} // namespace lp
