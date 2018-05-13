#ifndef LAMPPOST_PLUGINMANAGERCONFIGURATION_H
#define LAMPPOST_PLUGINMANAGERCONFIGURATION_H


#include <string>
#include <list>


namespace lp {
	struct PluginManagerConfiguration {
		std::list<std::string> mTemplateSearchPaths;
	};
}


#endif //LAMPPOST_PLUGINMANAGERCONFIGURATION_H
