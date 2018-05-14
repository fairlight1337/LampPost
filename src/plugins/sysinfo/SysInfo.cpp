#include "SysInfo.h"


namespace lp {
	namespace plugins {
		SysInfo::SysInfo(PluginConfiguration configuration) : PluginInstance(configuration) {
			std::cout << "!" << std::endl;
		}

		SysInfo::~SysInfo() {
		}

		void SysInfo::Run() {
			std::shared_ptr<bus::Publisher> publisher = GetPublisher("/sysinfo");

			std::shared_ptr<messages::Datagram> datagram = std::make_shared<messages::Datagram>();
			*datagram = "Hello Bus!";

			publisher->Publish(datagram);
		}
	}
}