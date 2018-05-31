#include <lamppost/LampPost.h>


namespace lp {
  LampPost::LampPost(LampPostConfiguration configuration)
    : mConfiguration(configuration),
      mPluginManager(std::make_unique<PluginManager>(configuration.mPluginManagerConfiguration)),
      mRootBus(std::make_shared<bus::Bus>("root")),
      mLog("LampPost") {
  }

  LampPost::~LampPost() {
    mRootBus = nullptr;
  }

  void LampPost::Start() {
    mLog.Info("Starting instance.");

    mLog.Info("Initializing plugin manager.", 1);
    mPluginManager->SetBus(mRootBus);
    PluginConfiguration pluginConfiguration;

    mLog.Info("Loading templates.", 1);
    mPluginManager->LoadTemplates();

    mLog.Info("Instantiating plugins");

    mLog.Info("SysInfo", 1);
    std::shared_ptr<PluginInstance> sysInfoInstance = mPluginManager->InstantiateTemplate("SysInfo", pluginConfiguration);
    mLog.Info("Initialize", 2);
    sysInfoInstance->Initialize();
    mLog.Info("Start", 2);
    sysInfoInstance->Start();
    mLog.Info("Clean", 2);
    sysInfoInstance = nullptr;

    mLog.Info("Link", 1);
    std::shared_ptr<PluginInstance> linkInstance = mPluginManager->InstantiateTemplate("Link", pluginConfiguration);
    mLog.Info("Initialize", 2);
    linkInstance->Initialize();
    mLog.Info("Start", 2);
    linkInstance->Start();
    mLog.Info("Clean", 2);
    linkInstance = nullptr;

    mLog.Info("Starting root bus.");
    mRootBus->Start();
    mLog.Info("Returned from running root bus.");

    mLog.Info("Shutting down");

    mLog.Info("Unloading templates", 1);
    mRootBus->Detach();
    mPluginManager->UnloadTemplates();

    mLog.Info("Disabling root bus", 1);
    mRootBus = nullptr;
    //mPluginManager->SetBus(nullptr);

    mLog.Info("Disabling plugin manager", 1);
    mPluginManager = nullptr;

    mLog.Info("Shutdown complete.");
  }

  void LampPost::Stop() {
    mLog.Info("Received stop signal.");
    mRootBus->Stop();
  }
} // namespace lp
