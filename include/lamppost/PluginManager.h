#ifndef LAMPPOST_PLUGINMANAGER_H
#define LAMPPOST_PLUGINMANAGER_H


#include <memory>
#include <string>
#include <map>
#include <iostream>

#if defined(__unix__) || defined(__linux__) || defined(__FreeBSD__) || defined(__APPLE__)
#include <dlfcn.h>
#endif

#include <lamppost/PluginTemplate.h>
#include <lamppost/PluginInstance.h>
#include <lamppost/Filesystem.h>


namespace lp {
	class PluginManager {
	private:
		static std::string sTemplateFileExtension;

		PluginManagerConfiguration mConfiguration;
		std::map<std::string, std::shared_ptr<PluginTemplate>> mTemplates;

		bool LoadTemplate(std::string filePath);

	public:
		PluginManager(PluginManagerConfiguration configuration);
		~PluginManager();

		void LoadTemplates();

		std::shared_ptr<PluginInstance> InstantiateTemplate(std::string templateIdentifier);
	};
}


#endif //LAMPPOST_PLUGINMANAGER_H
