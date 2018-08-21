#ifndef LAMPPOST_LAMPPOST_H
#define LAMPPOST_LAMPPOST_H


#include <atomic>
#include <map>
#include <memory>
#include <string>

#include <tbb/task_scheduler_init.h>

#include <lamppost/exceptions/OperationFailedException.h>
#include <lamppost/settings/SettingsManager.h>
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
    settings::SettingsManager mSettingsManager;

    tbb::task_scheduler_init mTbbTaskSchedulerInit;

    void Setup();
    void Teardown();

    void InstantiatePlugin(std::string identifier, PluginConfiguration pluginConfiguration);

    void ReadConfigurationFile(std::string file);

  public:
    LampPost(LampPostConfiguration configuration);
    virtual ~LampPost();

    void Start();
    void Stop();
    bool IsStopped();
  };
} // namespace lp


#endif //LAMPPOST_LAMPPOST_H
