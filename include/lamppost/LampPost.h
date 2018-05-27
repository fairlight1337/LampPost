#ifndef LAMPPOST_LAMPPOST_H
#define LAMPPOST_LAMPPOST_H


#include <atomic>
#include <map>
#include <memory>
#include <string>

#include <lamppost/PluginManager.h>
#include <lamppost/LampPostConfiguration.h>


namespace lp {
  class LampPost {
    private:
      std::shared_ptr<bus::Bus> mRootBus;
      LampPostConfiguration mConfiguration;
      PluginManager mPluginManager;

    public:
      LampPost(LampPostConfiguration configuration);
      virtual ~LampPost() = default;

      void Start();
      void Stop();
  };
}

#endif //LAMPPOST_LAMPPOST_H
