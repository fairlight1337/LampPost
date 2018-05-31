#include "Link.h"


namespace lp {
  namespace plugins {
    Link::Link(PluginConfiguration configuration) : PluginInstance(configuration) {
      std::cout << "Instantiate Link" << std::endl;
    }

    void Link::Initialize() {
      mSysInfoSubscriber = GetSubscriber("/sysinfo", [](std::shared_ptr<messages::Message> message) {
        std::cout << "Got a message." << std::endl;
      });
    }

    void Link::Run() {
      while(mShouldRun) {
        // TODO: Implement this.

        Sleep(std::chrono::seconds(2));
      }
    }

    void Link::Deinitialize() {
      DeleteSubscriber(mSysInfoSubscriber);
      mSysInfoSubscriber = nullptr;
    }
  } // namespace plugins
} // namespace lp
