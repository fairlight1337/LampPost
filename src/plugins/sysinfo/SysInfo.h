#ifndef LAMPPOST_SYSINFO_H
#define LAMPPOST_SYSINFO_H


#include <iostream>
#include <memory>

#include <lamppost/PluginInstance.h>
#include <lamppost/PluginTemplateInfo.h>


namespace lp {
	namespace plugins {
		class SysInfo : public PluginInstance {
		private:
			std::shared_ptr<bus::Publisher> mSysInfoPublisher;

		public:
			SysInfo(PluginConfiguration configuration);
			~SysInfo();

			virtual void Initialize() override;
			virtual void Run() override;
			virtual void Deinitialize() override;
		};
	}
}

std::shared_ptr<lp::PluginInstance> CreateInstance(lp::PluginConfiguration configuration) {
	return std::make_shared<lp::plugins::SysInfo>(configuration);
}

lp::PluginTemplateInfo GetInfo() {
	lp::PluginTemplateInfo info;

	info.mIdentifier = "SysInfo";
	info.mVersion = lp::Version(0, 1, 0, 0);

	return info;
}


#endif //LAMPPOST_SYSINFO_H
