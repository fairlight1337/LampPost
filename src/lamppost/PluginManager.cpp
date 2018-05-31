#include <lamppost/PluginManager.h>


namespace lp {
#if defined(_WIN32) || defined(_WIN64)
  std::string PluginManager::sTemplateFileExtension = "dll";
#elif defined(__unix__) || defined(__linux__) || defined(__FreeBSD__) || defined(__APPLE__)
  std::string PluginManager::sTemplateFileExtension = "so";
#endif

  PluginManager::PluginManager(PluginManagerConfiguration configuration) : mConfiguration(configuration), mLog("PluginManager") {
    std::string executableDirectory = Filesystem::GetBaseDirectory(Filesystem::GetPathOfRunningExecutable());
    std::string currentWorkingDirectory = Filesystem::GetWorkingDirectory();

    mConfiguration.mTemplateSearchPaths.push_back(executableDirectory);

    if(currentWorkingDirectory != executableDirectory) {
      mConfiguration.mTemplateSearchPaths.push_back(currentWorkingDirectory);
    }
  }

  PluginManager::~PluginManager() {
    mTemplates.clear();
  }

  PluginLibraryHandle PluginManager::OpenPluginLibrary(std::string path) {
#if defined(_WIN32) || defined(_WIN64)
    return LoadLibrary(path.c_str());
#elif defined(__unix__) || defined(__linux__) || defined(__FreeBSD__) || defined(__APPLE__)
    return dlopen(path.c_str(), RTLD_NOW);
#endif
  }

  void PluginManager::ClosePluginLibrary(lp::PluginLibraryHandle handle) {
#if defined(_WIN32) || defined(_WIN64)
    FreeLibrary(handle);
#elif defined(__unix__) || defined(__linux__) || defined(__FreeBSD__) || defined(__APPLE__)
    dlclose(handle);
#endif
  }

  bool PluginManager::LoadTemplate(std::string filePath) {
    bool loadedSuccessfully = false;

    if(Filesystem::PathExists(filePath) && Filesystem::GetFileExtension(filePath) == sTemplateFileExtension) {
      mLog.Info("Now loading: " + filePath, 2);

      PluginLibraryHandle handle = OpenPluginLibrary(filePath);

      if(handle != nullptr) {
        mLog.Info("Acquired library handle", 3);

        PluginCreateInfoFunctionType CreateInfo = GetPluginLibaryFunction<PluginCreateInfoFunctionType>(handle, "_CreateInfo@0");
        PluginDestroyInfoFunctionType DestroyInfo = GetPluginLibaryFunction<PluginDestroyInfoFunctionType>(handle, "_DestroyInfo@" + std::to_string(sizeof(PluginTemplateInfo*)));

        if(CreateInfo != nullptr && DestroyInfo != nullptr) {
          mLog.Info("Info methods present", 3);

          std::shared_ptr<PluginTemplateInfo> info = CreateSharedObject<PluginTemplateInfo>(CreateInfo, DestroyInfo);

          if(!info->mIdentifier.empty()) {
            mLog.Info("Identifier = " + info->mIdentifier, 3);

            PluginCreateInstanceFunctionType CreateInstance = GetPluginLibaryFunction<PluginCreateInstanceFunctionType>(handle, "_CreateInstance@" + std::to_string(sizeof(PluginConfiguration)));
            PluginDestroyInstanceFunctionType DestroyInstance = GetPluginLibaryFunction<PluginDestroyInstanceFunctionType>(handle, "_DestroyInstance@" + std::to_string(sizeof(PluginInstance*)));

            if(CreateInstance != nullptr && DestroyInstance != nullptr) {
              mLog.Info("Creation methods present", 3);

              PluginTemplateConfiguration configuration(
                info->mIdentifier,
                info->mVersion,
                [CreateInstance, DestroyInstance](PluginConfiguration configuration) {
                  return CreateSharedObject<PluginInstance>(CreateInstance, DestroyInstance, configuration);
                },
                [handle]() { ClosePluginLibrary(handle); });

              std::shared_ptr<PluginTemplate> pluginTemplate = std::make_shared<PluginTemplate>(configuration);
              mTemplates[info->mIdentifier] = pluginTemplate;

              loadedSuccessfully = true;

              mLog.Info("Load successful", 3);
            } else {
              mLog.Error("Creation methods missing", 3);

              ClosePluginLibrary(handle);
            }
          } else {
            mLog.Error("Identifier empty", 3);

            ClosePluginLibrary(handle);
          }
        } else {
          mLog.Error("Info methods missing", 3);

          ClosePluginLibrary(handle);
        }
      } else {
        mLog.Error("Failed to acquire library handle.", 3);
      }
    }

    return loadedSuccessfully;
  }

  void PluginManager::LoadTemplates() {
    mLog.Info("Loading plugin templates from search paths:");

    for(const std::string& searchPath : mConfiguration.mTemplateSearchPaths) {
      mLog.Info(searchPath, 1);

      std::list<std::string> filesInPath = Filesystem::GetDirectoryContents(searchPath, FilesystemObjectType::File);

      for(const std::string& file : filesInPath) {
        LoadTemplate(Filesystem::CombinePaths(searchPath, file));
      }
    }
  }

  void PluginManager::UnloadTemplates() {
    mLog.Info("Unloading plugin templates:");

    for(std::pair<std::string, std::shared_ptr<PluginTemplate>> templatePair : mTemplates) {
      mLog.Info(templatePair.first, 1);

      templatePair.second->Unload();
    }

    mTemplates.clear();
  }

  void PluginManager::SetBus(std::shared_ptr<bus::Bus> bus) {
    mConfiguration.mBus = bus;
  }

  std::shared_ptr<PluginInstance> PluginManager::InstantiateTemplate(std::string templateIdentifier, PluginConfiguration configuration) {
    mLog.Info("Instantiating plugin template: " + templateIdentifier);

    if(configuration.mBus == nullptr) {
      mLog.Info("Using default bus", 1);

      configuration.mBus = mConfiguration.mBus;
    }

    if(mTemplates.find(templateIdentifier) != mTemplates.end()) {
      mLog.Info("Template found, instantiating", 1);

      return mTemplates[templateIdentifier]->Instantiate(configuration, configuration.mBus);
    } else {
      mLog.Error("Template not found, failed to instantiate template", 1);
    }

    return nullptr;
  }
} // namespace lp
