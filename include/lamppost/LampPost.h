#ifndef LAMPPOST_LAMPPOST_H
#define LAMPPOST_LAMPPOST_H


#include <atomic>
#include <map>
#include <memory>
#include <string>

#include <lamppost/config/ConfigurationManager.h>
#include <lamppost/LampPostConfiguration.h>
#include <lamppost/plugin/PluginManager.h>
#include <lamppost/log/Log.h>
#include <lamppost/RunState.h>


namespace lp
{
  class LampPost
  {
  private:
    std::shared_ptr<bus::Bus> mRootBus;
    LampPostConfiguration mConfiguration;
    PluginManager mPluginManager;
    log::Log mLog;
    std::atomic<RunState> mRunState;
    config::ConfigurationManager mConfigurationManager;

    void Setup();
    void Teardown();

    void InstantiatePlugin(std::string identifier, PluginConfiguration pluginConfiguration);

  public:
    LampPost(LampPostConfiguration configuration);
    virtual ~LampPost();

    void Start();
    void Stop();
    bool IsStopped();
  };
} // namespace lp


#endif //LAMPPOST_LAMPPOST_H
