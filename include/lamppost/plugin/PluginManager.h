#ifndef LAMPPOST_PLUGINMANAGER_H
#define LAMPPOST_PLUGINMANAGER_H


#include <iostream>
#include <map>
#include <memory>
#include <string>

#if defined(__unix__) || defined(__linux__) || defined(__FreeBSD__) || defined(__APPLE__)
#include <dlfcn.h>
#endif

#include <lamppost/Filesystem.h>
#include <lamppost/log/Log.h>
#include <lamppost/plugin/PluginInstance.h>
#include <lamppost/plugin/PluginTemplate.h>


namespace lp
{
#if defined(_WIN32) || defined(_WIN64)
  typedef PluginTemplateInfo* (CALLBACK* PluginCreateInfoFunctionType)();
  typedef PluginInstance* (CALLBACK* PluginCreateInstanceFunctionType)(PluginConfiguration);
  typedef HINSTANCE PluginLibraryHandle;
#elif defined(__unix__) || defined(__linux__) || defined(__FreeBSD__) || defined(__APPLE__)
  typedef PluginTemplateInfo* (*PluginCreateInfoFunctionType)();
  typedef PluginInstance* (*PluginCreateInstanceFunctionType)(PluginConfiguration);
  typedef void* PluginLibraryHandle;
#endif


  class PluginManager
  {
  private:
    #pragma region Types
    
    struct PluginTemplateDescription
    {
      PluginLibraryHandle mLibraryHandle;
      std::shared_ptr<PluginTemplate> mTemplate;
    };

    #pragma endregion

    #pragma region Statics

    static std::string sTemplateFileExtension;
    static std::string sTemplateFilePrefix;
    
    static PluginLibraryHandle OpenPluginLibrary(std::string path);
    static void ClosePluginLibrary(PluginLibraryHandle handle);

    template<typename T>
    static T GetPluginLibaryFunction(PluginLibraryHandle handle, std::string functionName)
    {
#if defined(_WIN32) || defined(_WIN64)
      return (T)GetProcAddress(handle, functionName.c_str());
#elif defined(__unix__) || defined(__linux__) || defined(__FreeBSD__) || defined(__APPLE__)
      return (T)dlsym(handle, functionName.c_str());
#endif
    }

    template<typename TTargetType, typename TCreateFunctionType, class ... Args>
    static std::shared_ptr<TTargetType> CreateSharedObject(TCreateFunctionType createFunction, Args ... args)
    {
      TTargetType* pi = createFunction(std::forward<Args>(args)...);
      std::shared_ptr<TTargetType> object(pi);

      return object;
    };

    #pragma endregion

    #pragma region Private Member Variables

    PluginManagerConfiguration mConfiguration;
    std::map<std::string, PluginTemplateDescription> mTemplates;
    log::Log mLog;

    #pragma endregion

    #pragma region Private Methods

    bool LoadTemplate(std::string filePath);

    #pragma endregion

  public:
    PluginManager(PluginManagerConfiguration configuration);
    virtual ~PluginManager();

    void AddTemplateSearchPath(std::string templateSearchPath);
    void SetBus(std::shared_ptr<bus::Bus> bus);

    void LoadTemplates();
    void UnloadTemplates();

    std::shared_ptr<PluginInstance> InstantiateTemplate(std::string templateIdentifier, PluginConfiguration configuration);
  };
} // namespace lp


#endif //LAMPPOST_PLUGINMANAGER_H
