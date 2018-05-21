#ifndef LAMPPOST_LAMPPOSTCONFIGURATION_H
#define LAMPPOST_LAMPPOSTCONFIGURATION_H


#include <lamppost/PluginManagerConfiguration.h>


namespace lp {
	struct LampPostConfiguration {
		PluginManagerConfiguration mPluginManagerConfiguration;

		LampPostConfiguration() {
		}

		LampPostConfiguration(PluginManagerConfiguration pluginManagerConfiguration) {
			mPluginManagerConfiguration = pluginManagerConfiguration;
		}

		bool operator==(const LampPostConfiguration& other) {
			return mPluginManagerConfiguration == other.mPluginManagerConfiguration;
		}

		bool operator!=(const LampPostConfiguration& other) {
			return !(*this == other);
		}
	};
}


#endif //LAMPPOST_LAMPPOSTCONFIGURATION_H
