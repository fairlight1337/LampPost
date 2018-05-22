#include <lamppost/PluginTemplate.h>


namespace lp {
  PluginTemplate::PluginTemplate(PluginTemplateConfiguration configuration) : Identifiable(configuration.mIdentifier), mConfiguration(configuration) {
  }

  PluginTemplate::~PluginTemplate() {
    mInstances.clear();

    mConfiguration.mUnloadPluginFunction();
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
}
