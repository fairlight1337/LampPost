#include <lamppost/LampPost.h>


namespace lp
{
  LampPost::LampPost(LampPostConfiguration configuration)
    : mRunState(RunState::Unknown),
      mConfiguration(configuration),
      mPluginManager(configuration.mPluginManagerConfiguration),
      mRootBus(std::make_shared<bus::Bus>("root")),
      mLog("LampPost")
  {
    for(const std::string& configurationFile : mConfiguration.mConfigurationFiles)
    {
      mSettingsManager.ReadConfigurationFile(configurationFile);
    }

    for(const std::string& pluginTemplateSearchPath : mSettingsManager.GetPluginTemplateSearchPaths())
    {
      mPluginManager.AddTemplateSearchPath(pluginTemplateSearchPath);
    }
  }

  LampPost::~LampPost()
  {
    mRootBus = nullptr;
  }

  void LampPost::Setup()
  {
    mLog.Info("Initializing plugin manager.", 1);
    mPluginManager.SetBus(mRootBus);

    mLog.Info("Loading templates.", 1);
    mPluginManager.LoadTemplates();
  }

  void LampPost::Teardown()
  {
    mLog.Info("Unloading templates", 1);
    mRootBus->Detach();
    mPluginManager.UnloadTemplates();

    mLog.Info("Disabling root bus", 1);
    mRootBus = nullptr;
  }

  void LampPost::InstantiatePlugin(std::string identifier, PluginConfiguration pluginConfiguration)
  {
    mLog.Info("Plugin: " + identifier, 1);
    std::shared_ptr<PluginInstance> sysInfoInstance = mPluginManager.InstantiateTemplate(identifier, pluginConfiguration);

    mLog.Info("Initialize", 2);
    sysInfoInstance->Initialize();

    mLog.Info("Start", 2);
    sysInfoInstance->Start();
  }

  void LampPost::Start()
  {
    mRunState = RunState::Running;

    mLog.Info("Starting instance.");
    Setup();

    mLog.Info("Instantiating plugins");
    PluginConfiguration pluginConfiguration;

    InstantiatePlugin("SysInfo", pluginConfiguration);
    InstantiatePlugin("Link", pluginConfiguration);

    mLog.Info("Starting root bus.");
    mRootBus->Start();
    mLog.Info("Returned from running root bus.");

    mLog.Info("Shutting down");
    Teardown();

    mLog.Info("Shutdown complete.");

    mRunState = RunState::Running;
  }

  void LampPost::Stop()
  {
    mLog.Info("Received stop signal.");
    mRootBus->Stop();
  }

  bool LampPost::IsStopped()
  {
    return mRunState == RunState::Stopped;
  }
} // namespace lp
