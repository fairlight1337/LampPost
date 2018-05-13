#ifndef LAMPPOST_PLUGINTEMPLATE_H
#define LAMPPOST_PLUGINTEMPLATE_H


#include <memory>
#include <string>
#include <map>

#include <lamppost/PluginManagerConfiguration.h>
#include <lamppost/PluginInstance.h>
#include <lamppost/Identifiable.h>


namespace lp {
	class PluginTemplate : public Identifiable {
	private:
		std::map<std::string, std::shared_ptr<PluginInstance>> mInstances;

	public:
		PluginTemplate(std::string identifier);
		~PluginTemplate();

		PluginInstance Instantiate();
	};
}

#endif //LAMPPOST_PLUGINTEMPLATE_H
