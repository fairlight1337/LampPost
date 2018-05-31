#include "SysInfo.h"


namespace lp {
  namespace plugins {
    SysInfo::SysInfo(PluginConfiguration configuration) : PluginInstance(configuration) {
    }

    void SysInfo::Initialize() {
      mSysInfoPublisher = GetPublisher("/sysinfo");
      mSysInfoSubscriber = GetSubscriber("/sysinfo", [](std::shared_ptr<messages::Datagram> datagram) {
        std::string message = datagram->Get<std::string>();
        std::cout << "Got it: " << message << std::endl;
      });
    }

    void SysInfo::Run() {
      while(mShouldRun) {
        std::shared_ptr<messages::Datagram> datagram = std::make_shared<messages::Datagram>();
        *datagram = std::string("Hello Bus!");

        mSysInfoPublisher->Publish(datagram);
	std::cout << "Sent" << std::endl;

        Sleep(std::chrono::seconds(2));
      }
    }

    void SysInfo::Deinitialize() {
      DeleteSubscriber(mSysInfoSubscriber);
      mSysInfoSubscriber = nullptr;

      DeletePublisher(mSysInfoPublisher);
      mSysInfoPublisher = nullptr;
    }
  } // namespace plugins
} // namespace lp
