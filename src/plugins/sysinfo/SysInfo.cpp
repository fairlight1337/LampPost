#include "SysInfo.h"


namespace lp {
	namespace plugins {
		SysInfo::SysInfo(PluginConfiguration configuration) : PluginInstance(configuration) {
			std::cout << "!" << std::endl;
		}

		SysInfo::~SysInfo() {
		}

		void SysInfo::Initialize() {
			mSysInfoPublisher = GetPublisher("/sysinfo");
		}

		void SysInfo::Run() {
			while(mShouldRun) {
				std::shared_ptr<messages::Datagram> datagram = std::make_shared<messages::Datagram>();
				*datagram = "Hello Bus!";

				mSysInfoPublisher->Publish(datagram);

				Sleep(std::chrono::seconds(2));
			}
		}

		void SysInfo::Deinitialize() {
		}
	}
}
