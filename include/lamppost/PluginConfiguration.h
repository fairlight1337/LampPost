#ifndef LAMPPOST_PLUGINCONFIGURATION_H
#define LAMPPOST_PLUGINCONFIGURATION_H


#include <string>

#include <lamppost/bus/Bus.h>


namespace lp {
  struct PluginConfiguration {
    std::string mIdentifier;
    std::shared_ptr<bus::Bus> mBus;
  };
} // namespace lp


#endif //LAMPPOST_PLUGINCONFIGURATION_H
