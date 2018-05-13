#include "SysInfo.h"


namespace lp {
	namespace plugins {
		SysInfo::SysInfo(PluginConfiguration configuration) : PluginInstance(configuration) {
			std::cout << "!" << std::endl;
		}

		SysInfo::~SysInfo() {

		}
	}
}