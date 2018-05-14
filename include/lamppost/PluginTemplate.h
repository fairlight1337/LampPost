#ifndef LAMPPOST_PLUGINTEMPLATE_H
#define LAMPPOST_PLUGINTEMPLATE_H


#include <memory>
#include <string>
#include <map>

#include <lamppost/PluginManagerConfiguration.h>
#include <lamppost/PluginInstance.h>
#include <lamppost/Identifiable.h>
#include <lamppost/PluginTemplateInfo.h>
#include <lamppost/PluginTemplateConfiguration.h>


namespace lp {
	class PluginTemplate : public Identifiable {
	private:
		PluginTemplateConfiguration mConfiguration;
		std::map<std::string, std::shared_ptr<PluginInstance>> mInstances;

		bool IndexedInstanceExists(int i);

	public:
		PluginTemplate(PluginTemplateConfiguration configuration);
		~PluginTemplate();

		std::shared_ptr<PluginInstance> Instantiate(PluginConfiguration configuration);

		bool IsValid();
		std::string GetType();
	};
}

#endif //LAMPPOST_PLUGINTEMPLATE_H
