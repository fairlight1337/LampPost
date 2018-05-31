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

    std::string libDirectory = Filesystem::CombinePaths(currentWorkingDirectory, "lib");
    mConfiguration.mTemplateSearchPaths.push_back(libDirectory);
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

#if defined(_WIN32) || defined(_WIN64)
	std::string createInfoFunctionName = "_CreateInfo@0";
	std::string destroyInfoFunctionName = "_DestroyInfo@" + std::to_string(sizeof(PluginTemplateInfo*));
#elif defined(__unix__) || defined(__linux__) || defined(__FreeBSD__) || defined(__APPLE__)
	std::string createInfoFunctionName = "CreateInfo";
	std::string destroyInfoFunctionName = "DestroyInfo";
#endif

        PluginCreateInfoFunctionType CreateInfo = GetPluginLibaryFunction<PluginCreateInfoFunctionType>(handle, createInfoFunctionName);
        PluginDestroyInfoFunctionType DestroyInfo = GetPluginLibaryFunction<PluginDestroyInfoFunctionType>(handle, destroyInfoFunctionName);

        if(CreateInfo != nullptr && DestroyInfo != nullptr) {
          mLog.Info("Info methods present", 3);

          std::shared_ptr<PluginTemplateInfo> info = CreateSharedObject<PluginTemplateInfo>(CreateInfo, DestroyInfo);

          if(!info->mIdentifier.empty()) {
            mLog.Info("Identifier = " + info->mIdentifier, 3);

#if defined(_WIN32) || defined(_WIN64)
	std::string createInstanceFunctionName = "_CreateInstance@" + std::to_string(sizeof(PluginConfiguration));
	std::string destroyInstanceFunctionName = "_DestroyInstance@" + std::to_string(sizeof(PluginInstance*));
#elif defined(__unix__) || defined(__linux__) || defined(__FreeBSD__) || defined(__APPLE__)
	std::string createInstanceFunctionName = "CreateInstance";
	std::string destroyInstanceFunctionName = "DestroyInstance";
#endif
	
            PluginCreateInstanceFunctionType CreateInstance = GetPluginLibaryFunction<PluginCreateInstanceFunctionType>(handle, createInstanceFunctionName);
            PluginDestroyInstanceFunctionType DestroyInstance = GetPluginLibaryFunction<PluginDestroyInstanceFunctionType>(handle, destroyInstanceFunctionName);

            if(CreateInstance != nullptr && DestroyInstance != nullptr) {
              mLog.Info("Creation methods present", 3);

              PluginTemplateConfiguration configuration(
                info->mIdentifier,
                info->mVersion,
                [CreateInstance, DestroyInstance](PluginConfiguration configuration)
                {
                  return CreateSharedObject<PluginInstance>(CreateInstance, DestroyInstance, configuration);
                });

              std::shared_ptr<PluginTemplate> pluginTemplate = std::make_shared<PluginTemplate>(configuration);

              PluginTemplateDescription pluginTemplateDescription;
              pluginTemplateDescription.mLibraryHandle = handle;
              pluginTemplateDescription.mTemplate = pluginTemplate;

              mTemplates[info->mIdentifier] = pluginTemplateDescription;

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
	std::string filePath = Filesystem::CombinePaths(searchPath, file);
	
        LoadTemplate(filePath);
      }
    }
  }

  void PluginManager::UnloadTemplates() {
    mLog.Info("Unloading plugin templates:");

    for(std::pair<std::string, PluginTemplateDescription> templatePair : mTemplates) {
      mLog.Info(templatePair.first, 1);

      templatePair.second.mTemplate->Unload();
      ClosePluginLibrary(templatePair.second.mLibraryHandle);
    }

    mTemplates.clear();
  }

  void PluginManager::SetBus(std::shared_ptr<bus::Bus> bus) {
    mConfiguration.mBus = bus;
  }

  std::shared_ptr<PluginInstance> PluginManager::InstantiateTemplate(std::string templateIdentifier, PluginConfiguration configuration) {
    std::shared_ptr<PluginInstance> returnedInstance = nullptr;
    
    mLog.Info("Instantiating plugin template: " + templateIdentifier);

    if(configuration.mBus == nullptr) {
      mLog.Info("Using default bus", 1);

      configuration.mBus = mConfiguration.mBus;
    }

    if(mTemplates.find(templateIdentifier) != mTemplates.end()) {
      mLog.Info("Template found, instantiating", 1);

      returnedInstance = mTemplates[templateIdentifier].mTemplate->Instantiate(configuration, configuration.mBus);
    } else {
      mLog.Error("Template not found, failed to instantiate.", 1);
    }

    return returnedInstance;
  }
} // namespace lp
