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
      std::shared_ptr<bus::Subscriber> mSysInfoSubscriber;

    public:
      SysInfo(PluginConfiguration configuration);
      ~SysInfo();

      virtual void Initialize() override;
      virtual void Run() override;
      virtual void Deinitialize() override;
    };
  }
}

extern "C" {
  GENERATE_INSTANCE_CREATOR(lp::plugins::SysInfo);
  GENERATE_INFO_CREATOR(info,
    info->mIdentifier = "SysInfo";
    info->mVersion = lp::Version(0, 1, 0, 0);
  )
}


#endif //LAMPPOST_SYSINFO_H
