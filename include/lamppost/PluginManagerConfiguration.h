#ifndef LAMPPOST_PLUGINMANAGERCONFIGURATION_H
#define LAMPPOST_PLUGINMANAGERCONFIGURATION_H


#include <string>
#include <list>

#include <lamppost/bus/Bus.h>


namespace lp {
	struct PluginManagerConfiguration {
		std::list<std::string> mTemplateSearchPaths;
		std::shared_ptr<bus::Bus> mBus;
	};
}


#endif //LAMPPOST_PLUGINMANAGERCONFIGURATION_H
