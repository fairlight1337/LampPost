#ifndef LAMPPOST_LINK_H
#define LAMPPOST_LINK_H


#include <iostream>
#include <memory>
#include <sstream>

#include <lamppost/plugin/PluginInstance.h>
#include <lamppost/plugin/PluginTemplateInfo.h>
#include <lamppost/schemas/Message_generated.h>


namespace lp
{
  namespace plugins
  {
    class Link : public PluginInstance
    {
    private:
      std::shared_ptr<bus::Subscriber> mSysInfoSubscriber;
      std::shared_ptr<bus::ActionConsumer> mSysInfoActionConsumer;



    public:
      Link(PluginConfiguration configuration);
      virtual ~Link() = default;

      void Initialize() override;
      void Run() override;
      void Deinitialize() override;
    };
  } // namespace plugins
} // namespace lp

extern "C"
{
  GENERATE_INSTANCE_CREATOR(lp::plugins::Link);
  GENERATE_INFO_CREATOR(info,
    info->mIdentifier = "Link";
    info->mVersion = lp::Version(0, 1, 0, 0);
  )
}


#endif //LAMPPOST_LINK_H
