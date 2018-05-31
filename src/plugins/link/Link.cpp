#include "Link.h"


namespace lp
{
  namespace plugins
  {
    Link::Link(PluginConfiguration configuration)
      : PluginInstance(configuration)
    {
    }

    void Link::Initialize() {
      mSysInfoSubscriber = GetSubscriber("/sysinfo",
        [this](std::shared_ptr<messages::Message> message)
        {
          mLog.Info("Got a message");
        });
    }

    void Link::Run()
    {
      while(mShouldRun)
      {
        // TODO(fairlight1337): Implement this.

        Sleep(std::chrono::seconds(2));
      }
    }

    void Link::Deinitialize()
    {
      DeleteSubscriber(mSysInfoSubscriber);
      mSysInfoSubscriber = nullptr;
    }
  } // namespace plugins
} // namespace lp
