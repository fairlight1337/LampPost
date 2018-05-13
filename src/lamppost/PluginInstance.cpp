#include <lamppost/PluginInstance.h>


namespace lp {
	PluginInstance::PluginInstance(PluginConfiguration configuration) : Identifiable(configuration.mIdentifier), mConfiguration(configuration) {
	}

	PluginInstance::~PluginInstance() {
	}
}
