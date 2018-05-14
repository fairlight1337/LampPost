#include <lamppost/PluginInstance.h>


namespace lp {
	PluginInstance::PluginInstance(PluginConfiguration configuration) : Identifiable(configuration.mIdentifier), mConfiguration(configuration) {
	}

	PluginInstance::~PluginInstance() {
	}

	std::shared_ptr<bus::Publisher> PluginInstance::GetPublisher(std::string topic) {
		return mConfiguration.mBus->CreatePublisher(topic);
	}
}
