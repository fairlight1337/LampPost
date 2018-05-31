#include <lamppost/LampPost.h>


namespace lp {
  LampPost::LampPost(LampPostConfiguration configuration)
    : mConfiguration(configuration),
      mPluginManager(configuration.mPluginManagerConfiguration),
      mRootBus(std::make_shared<bus::Bus>("root")) {
  }

  LampPost::~LampPost() {
    mRootBus = nullptr;
  }

  void LampPost::Start() {
    mPluginManager.SetBus(mRootBus);
    PluginConfiguration pluginConfiguration;

    mPluginManager.LoadTemplates();

    std::shared_ptr<PluginInstance> sysInfoInstance = mPluginManager.InstantiateTemplate("SysInfo", pluginConfiguration);
    sysInfoInstance->Initialize();
    sysInfoInstance->Start();
    sysInfoInstance = nullptr;

    std::shared_ptr<PluginInstance> linkInstance = mPluginManager.InstantiateTemplate("Link", pluginConfiguration);
    linkInstance->Initialize();
    linkInstance->Start();
    linkInstance = nullptr;

    mRootBus->Start();

    mPluginManager.UnloadTemplates();
  }

  void LampPost::Stop() {
      mRootBus->Stop();
  }
} // namespace lp
