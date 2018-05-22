#include <lamppost/PluginManager.h>


namespace lp {
#if defined(_WIN32) || defined(_WIN64)
  std::string PluginManager::sTemplateFileExtension = "dll";
#elif defined(__unix__) || defined(__linux__) || defined(__FreeBSD__) || defined(__APPLE__)
  std::string PluginManager::sTemplateFileExtension = "so";
#endif

  PluginManager::PluginManager(PluginManagerConfiguration configuration) : mConfiguration(configuration) {
    std::string executableDirectory = Filesystem::GetBaseDirectory(Filesystem::GetPathOfRunningExecutable());
    std::string currentWorkingDirectory = Filesystem::GetWorkingDirectory();

    mConfiguration.mTemplateSearchPaths.push_back(executableDirectory);

    if(currentWorkingDirectory != executableDirectory) {
      mConfiguration.mTemplateSearchPaths.push_back(currentWorkingDirectory);
    }
  }

  PluginManager::~PluginManager() {
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
    if(Filesystem::PathExists(filePath) && Filesystem::GetFileExtension(filePath) == sTemplateFileExtension) {
      std::cout << "Loading: " << filePath << std::endl;

      PluginLibraryHandle handle = OpenPluginLibrary(filePath);
      if(handle != nullptr) {
        PluginCreateInfoFunctionType CreateInfo = GetPluginLibaryFunction<PluginCreateInfoFunctionType>(handle, "_CreateInfo@0");
        PluginDestroyInfoFunctionType DestroyInfo = GetPluginLibaryFunction<PluginDestroyInfoFunctionType>(handle, "_DestroyInfo@" + std::to_string(sizeof(PluginTemplateInfo*)));

        if(CreateInfo != nullptr && DestroyInfo != nullptr) {
          std::shared_ptr<PluginTemplateInfo> info = CreateSharedObject<PluginTemplateInfo>(CreateInfo, DestroyInfo);

          if(!info->mIdentifier.empty()) {
            PluginCreateInstanceFunctionType CreateInstance = GetPluginLibaryFunction<PluginCreateInstanceFunctionType>(handle, "_CreateInstance@" + std::to_string(sizeof(PluginConfiguration)));
            PluginDestroyInstanceFunctionType DestroyInstance = GetPluginLibaryFunction<PluginDestroyInstanceFunctionType>(handle, "_DestroyInstance@" + std::to_string(sizeof(PluginInstance*)));

            if(CreateInstance != nullptr && DestroyInstance != nullptr) {
              PluginTemplateConfiguration configuration(
                info->mIdentifier,
                info->mVersion,
                [CreateInstance, DestroyInstance](PluginConfiguration configuration) {
                  return CreateSharedObject<PluginInstance>(CreateInstance, DestroyInstance, configuration);
                  },
                [handle]() { ClosePluginLibrary(handle); });

              std::shared_ptr<PluginTemplate> pluginTemplate = std::make_shared<PluginTemplate>(configuration);

              mTemplates[info->mIdentifier] = pluginTemplate;
            } else {
              ClosePluginLibrary(handle);
            }
          } else {
            ClosePluginLibrary(handle);
          }
        } else {
          ClosePluginLibrary(handle);
        }
      }
    }

    return false;
  }

  void PluginManager::LoadTemplates() {
    for(std::string searchPath : mConfiguration.mTemplateSearchPaths) {
      std::list<std::string> filesInPath = Filesystem::GetDirectoryContents(searchPath, FilesystemObjectType::File);

      for(std::string file : filesInPath) {
        LoadTemplate(Filesystem::CombinePaths(searchPath, file));
      }
    }
  }

  void PluginManager::SetBus(std::shared_ptr<bus::Bus> bus) {
    mConfiguration.mBus = bus;
  }

  std::shared_ptr<PluginInstance> PluginManager::InstantiateTemplate(std::string templateIdentifier, PluginConfiguration configuration) {
    if(configuration.mBus == nullptr) {
      configuration.mBus = mConfiguration.mBus;
    }

    if(mTemplates.find(templateIdentifier) != mTemplates.end()) {
      return mTemplates[templateIdentifier]->Instantiate(configuration, mConfiguration.mBus);
    }

    return nullptr;
  }
}