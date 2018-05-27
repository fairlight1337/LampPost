#ifndef LAMPPOST_PLUGINMANAGERCONFIGURATION_H
#define LAMPPOST_PLUGINMANAGERCONFIGURATION_H


#include <list>
#include <string>

#include <lamppost/bus/Bus.h>


namespace lp {
  struct PluginManagerConfiguration {
    std::list<std::string> mTemplateSearchPaths;
    std::shared_ptr<bus::Bus> mBus;

    PluginManagerConfiguration() {
      mBus = nullptr;
    }

    PluginManagerConfiguration(std::list<std::string> templateSearchPaths, std::shared_ptr<bus::Bus> bus) {
      mTemplateSearchPaths = templateSearchPaths;
      mBus = bus;
    }

    bool operator==(const lp::PluginManagerConfiguration& rhs) const {
      return
        mBus == rhs.mBus &&
        std::equal(mTemplateSearchPaths.begin(), mTemplateSearchPaths.end(), rhs.mTemplateSearchPaths.begin());
    }

    bool operator!=(const lp::PluginManagerConfiguration& rhs) const {
      return !(*this == rhs);
    }
  };
} // namespace lp


#endif //LAMPPOST_PLUGINMANAGERCONFIGURATION_H
