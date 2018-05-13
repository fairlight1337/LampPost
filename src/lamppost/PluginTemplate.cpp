#include <lamppost/PluginTemplate.h>


namespace lp {
	PluginTemplate::PluginTemplate(PluginTemplateConfiguration configuration) : Identifiable(configuration.mIdentifier), mConfiguration(configuration) {
	}

	PluginTemplate::~PluginTemplate() {
		mInstances.clear();

		mConfiguration.mUnloadPluginFunction();
	}

	std::shared_ptr<PluginInstance> PluginTemplate::Instantiate(PluginConfiguration configuration) {
		return mConfiguration.mInstantiateFunction(configuration);
	}
}
