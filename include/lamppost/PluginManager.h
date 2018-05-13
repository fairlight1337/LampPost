#ifndef LAMPPOST_PLUGINMANAGER_H
#define LAMPPOST_PLUGINMANAGER_H


#include <memory>
#include <string>
#include <map>

#include <lamppost/PluginTemplate.h>
#include <lamppost/PluginInstance.h>
#include <lamppost/Filesystem.h>


namespace lp {
	class PluginManager {
	private:
		std::map<std::string, std::shared_ptr<PluginTemplate>> mTemplates;

	public:
		PluginManager();
		~PluginManager();

		bool LoadTemplate(std::string filePath);
		std::shared_ptr<PluginInstance> InstantiateTemplate(std::string templateIdentifier);
	};
}


#endif //LAMPPOST_PLUGINMANAGER_H
