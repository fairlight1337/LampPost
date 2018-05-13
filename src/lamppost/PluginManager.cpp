#include <lamppost/PluginManager.h>


namespace lp {
#if defined(_WIN32) || defined(_WIN64)
	typedef PluginTemplateInfo (CALLBACK* PluginGetInfoFunctionType)();
	typedef std::shared_ptr<PluginInstance> (CALLBACK* PluginCreateInstanceFunctionType)(PluginConfiguration);

	std::string PluginManager::sTemplateFileExtension = "dll";
#elif defined(__unix__) || defined(__linux__) || defined(__FreeBSD__)
	typedef PluginTemplateInfo (*PluginGetInfoFunctionType)();
	typedef std::shared_ptr<PluginInstance> (*PluginCreateInstanceFunctionType)(PluginConfiguration);

	std::string PluginManager::sTemplateFileExtension = "so";
#elif defined(__APPLE__)
	typedef PluginTemplateInfo (CALLBACK* PluginGetInfoFunctionType)();
	typedef std::shared_ptr<PluginInstance> (CALLBACK* PluginCreateInstanceFunctionType)(PluginConfiguration);

	std::string PluginManager::sTemplateFileExtension = "dylib";
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

	bool PluginManager::LoadTemplate(std::string filePath) {
		if(Filesystem::PathExists(filePath) && Filesystem::GetFileExtension(filePath) == sTemplateFileExtension) {
			std::cout << "Loading: " << filePath << std::endl;

#if defined(_WIN32) || defined(_WIN64)
			HINSTANCE dllHandle = nullptr;
			dllHandle = LoadLibrary(filePath.c_str());

			if(dllHandle != nullptr) {
				PluginGetInfoFunctionType GetInfo;
				GetInfo = (PluginGetInfoFunctionType)GetProcAddress(dllHandle, "GetInfo");

				if(GetInfo != nullptr) {
					PluginTemplateInfo info = GetInfo();

					if(!info.mIdentifier.empty()) {
						PluginCreateInstanceFunctionType CreateInstance;
						CreateInstance = (PluginCreateInstanceFunctionType)GetProcAddress(dllHandle, "CreateInstance");

						if(CreateInstance != nullptr) {
							PluginTemplateConfiguration configuration(
								info.mIdentifier,
								info.mVersion,
								[CreateInstance](PluginConfiguration configuration) { return CreateInstance(configuration); },
							  [dllHandle]() { FreeLibrary(dllHandle); });

							std::shared_ptr<PluginTemplate> pluginTemplate = std::make_shared<PluginTemplate>(configuration);

							mTemplates[info.mIdentifier] = pluginTemplate;
						} else {
							FreeLibrary(dllHandle);
						}
					} else {
						FreeLibrary(dllHandle);
					}
				} else {
					FreeLibrary(dllHandle);
				}
			}
#elif defined(__unix__) || defined(__linux__) || defined(__FreeBSD__) || defined(__APPLE__)
			void *libHandle = nullptr;
			libHandle = dlopen(filePath.c_str());

			if(libHandle != nullptr) {
				PluginGetInfoFunctionType GetInfo;
				GetInfo = (PluginGetInfoFunctionType)dlsym(libHandle, "GetInfo");

				if(GetInfo != nullptr) {
					PluginTemplateInfo info = GetInfo();

					if(!info.mIdentifier.empty()) {
						PluginCreateInstanceFunctionType CreateInstance;
						CreateInstance = (PluginCreateInstanceFunctionType)dlsym(libHandle, "CreateInstance");

						if(CreateInstance != nullptr) {
							PluginTemplateConfiguration configuration(
								info.mIdentifier,
								info.mVersion,
								[CreateInstance](PluginConfiguration configuration) { return CreateInstance(configuration); },
							  [libHandle]() { dlclose(libHandle); });

							std::shared_ptr<PluginTemplate> pluginTemplate = std::make_shared<PluginTemplate>(configuration);

							mTemplates[info.mIdentifier] = pluginTemplate;
						} else {
							dlclose(libHandle);
						}
					} else {
						dlclose(libHandle);
					}
				} else {
					dlclose(libHandle);
				}
			}
#endif
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

	std::shared_ptr<PluginInstance> PluginManager::InstantiateTemplate(std::string templateIdentifier) {
		if(mTemplates.find(templateIdentifier) != mTemplates.end()) {
			// TODO: Implement template instantiation.
		}

		return nullptr;
	}
}