#ifndef LAMPPOST_PLUGININSTANCE_H
#define LAMPPOST_PLUGININSTANCE_H


#include <string>

#include <lamppost/Identifiable.h>
#include <lamppost/PluginConfiguration.h>


namespace lp {
	class PluginInstance : public Identifiable {
	private:
		PluginConfiguration mConfiguration;

	public:
		PluginInstance(PluginConfiguration configuration);
		~PluginInstance();
	};
}


#endif //LAMPPOST_PLUGININSTANCE_H
