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
#include <lamppost/PluginInstance.h>
#include <lamppost/PluginTemplate.h>


namespace lp {
#if defined(_WIN32) || defined(_WIN64)
  typedef PluginTemplateInfo* (CALLBACK* PluginCreateInfoFunctionType)();
  typedef void (CALLBACK* PluginDestroyInfoFunctionType)(PluginTemplateInfo*);
  typedef PluginInstance* (CALLBACK* PluginCreateInstanceFunctionType)(PluginConfiguration);
  typedef void (CALLBACK* PluginDestroyInstanceFunctionType)(PluginInstance*);
  typedef HINSTANCE PluginLibraryHandle;
#elif defined(__unix__) || defined(__linux__) || defined(__FreeBSD__) || defined(__APPLE__)
  typedef PluginTemplateInfo* (*PluginCreateInfoFunctionType)();
  typedef void (*PluginDestroyInfoFunctionType)(PluginTemplateInfo*);
  typedef PluginInstance* (*PluginCreateInstanceFunctionType)(PluginConfiguration);
  typedef void (*PluginDestroyInstanceFunctionType)(PluginInstance*);
  typedef void* PluginLibraryHandle;
#endif


  class PluginManager {
  private:
    static std::string sTemplateFileExtension;

    PluginManagerConfiguration mConfiguration;
    std::map<std::string, std::shared_ptr<PluginTemplate>> mTemplates;

    bool LoadTemplate(std::string filePath);

    static PluginLibraryHandle OpenPluginLibrary(std::string path);
    static void ClosePluginLibrary(PluginLibraryHandle handle);

    template<typename T>
    static T GetPluginLibaryFunction(PluginLibraryHandle handle, std::string functionName) {
#if defined(_WIN32) || defined(_WIN64)
      return (T)GetProcAddress(handle, functionName.c_str());
#elif defined(__unix__) || defined(__linux__) || defined(__FreeBSD__) || defined(__APPLE__)
      return (T)dlsym(handle, functionName.c_str());
#endif
    }

    template<typename TTargetType, typename TCreateFunctionType, typename TDestroyFunctionType, class ... Args>
    static std::shared_ptr<TTargetType> CreateSharedObject(TCreateFunctionType createFunction, TDestroyFunctionType destroyFunction, Args ... args) {
      TTargetType* pi = createFunction(std::forward<Args>(args)...);
      std::shared_ptr<TTargetType> object(pi, destroyFunction);

      return object;
    };

  public:
    PluginManager(PluginManagerConfiguration configuration);
    ~PluginManager();

    void SetBus(std::shared_ptr<bus::Bus> bus);
    void LoadTemplates();
    void UnloadTemplates();

    std::shared_ptr<PluginInstance> InstantiateTemplate(std::string templateIdentifier, PluginConfiguration configuration);
  };
} // namespace lp


#endif //LAMPPOST_PLUGINMANAGER_H
