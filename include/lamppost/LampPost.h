#ifndef LAMPPOST_LAMPPOST_H
#define LAMPPOST_LAMPPOST_H


#include <atomic>
#include <map>
#include <memory>
#include <string>

#include <lamppost/LampPostConfiguration.h>
#include <lamppost/PluginManager.h>
#include <lamppost/log/Log.h>


namespace lp
{
  class LampPost
  {
    private:
      std::shared_ptr<bus::Bus> mRootBus;
      LampPostConfiguration mConfiguration;
      PluginManager mPluginManager;
      log::Log mLog;

    public:
      LampPost(LampPostConfiguration configuration);
      virtual ~LampPost();

      void Start();
      void Stop();
  };
} // namespace lp

#endif //LAMPPOST_LAMPPOST_H
