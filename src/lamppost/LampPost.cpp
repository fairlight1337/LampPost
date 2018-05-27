#include <lamppost/LampPost.h>


namespace lp {
  LampPost::LampPost(LampPostConfiguration configuration)
    : mConfiguration(configuration),
      mPluginManager(configuration.mPluginManagerConfiguration),
      mRootBus(std::make_shared<bus::Bus>("root")) {
  }

  void LampPost::Start() {
    mPluginManager.SetBus(mRootBus);
    PluginConfiguration pluginConfiguration;

    mPluginManager.LoadTemplates();
    std::shared_ptr<PluginInstance> instance = mPluginManager.InstantiateTemplate("SysInfo", pluginConfiguration);

    instance->Initialize();
    instance->Start();

    mRootBus->Start();

    instance->Stop();
    instance->Deinitialize();
  }

  void LampPost::Stop() {
      mRootBus->Stop();
  }
}