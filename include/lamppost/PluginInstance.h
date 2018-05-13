#ifndef LAMPPOST_PLUGININSTANCE_H
#define LAMPPOST_PLUGININSTANCE_H


#include <string>

#include <lamppost/Identifiable.h>


namespace lp {
	class PluginInstance : public Identifiable {
	public:
		PluginInstance(std::string identifier);
		~PluginInstance();
	};
}


#endif //LAMPPOST_PLUGININSTANCE_H
