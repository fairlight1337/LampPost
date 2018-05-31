#ifndef LAMPPOST_LINK_H
#define LAMPPOST_LINK_H


#include <iostream>
#include <memory>

#include <lamppost/PluginInstance.h>
#include <lamppost/PluginTemplateInfo.h>


namespace lp {
  namespace plugins {
    class Link : public PluginInstance {
    private:
      std::shared_ptr<bus::Subscriber> mSysInfoSubscriber;

    public:
      Link(PluginConfiguration configuration);
      virtual ~Link() = default;

      virtual void Initialize() override;
      virtual void Run() override;
      virtual void Deinitialize() override;
    };
  } // namespace plugins
} // namespace lp

extern "C" {
  GENERATE_INSTANCE_CREATOR(lp::plugins::Link);
  GENERATE_INFO_CREATOR(info,
    info->mIdentifier = "Link";
    info->mVersion = lp::Version(0, 1, 0, 0);
  )
}


#endif //LAMPPOST_LINK_H
