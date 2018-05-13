#include <lamppost/PluginManager.h>


namespace lp {
#if defined(_WIN32) || defined(_WIN64)
	std::string PluginManager::sTemplateFileExtension = "dll";
#elif defined(__unix__) || defined(__linux__) || defined(__FreeBSD__)
	std::string PluginManager::sTemplateFileExtension = "so";
#elif defined(__APPLE__)
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

			// TODO: Implement plugin template loading.
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