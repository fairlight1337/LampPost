#include <lamppost/PluginManager.h>


namespace lp {
	PluginManager::PluginManager(PluginManagerConfiguration configuration) : mConfiguration(configuration) {
		mConfiguration.mTemplateSearchPaths.push_back(Filesystem::GetWorkingDirectory());
		mConfiguration.mTemplateSearchPaths.push_back(Filesystem::GetPathOfRunningExecutable());
	}

	PluginManager::~PluginManager() {
	}

	bool PluginManager::LoadTemplate(std::string filePath) {
		if(Filesystem::PathExists(filePath)) {
			// TODO: Implement plugin template loading.
		}

		return false;
	}

	void PluginManager::LoadTemplates() {
		for(std::string searchPath : mConfiguration.mTemplateSearchPaths) {
			std::list<std::string> filesInPath = Filesystem::GetDirectoryContents(searchPath, FilesystemObjectType::File);
		}
	}

	std::shared_ptr<PluginInstance> PluginManager::InstantiateTemplate(std::string templateIdentifier) {
		if(mTemplates.find(templateIdentifier) != mTemplates.end()) {
			// TODO: Implement template instantiation.
		}

		return nullptr;
	}
}