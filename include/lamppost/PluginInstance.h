#ifndef LAMPPOST_PLUGININSTANCE_H
#define LAMPPOST_PLUGININSTANCE_H


#include <string>

#include <lamppost/Identifiable.h>
#include <lamppost/PluginConfiguration.h>
#include <lamppost/bus/Bus.h>


namespace lp {
	class PluginInstance : public Identifiable {
	private:
		PluginConfiguration mConfiguration;

	public:
		PluginInstance(PluginConfiguration configuration);
		~PluginInstance();

		std::shared_ptr<bus::Publisher> GetPublisher(std::string topic);

		virtual void Run() = 0;
	};
}


#endif //LAMPPOST_PLUGININSTANCE_H
