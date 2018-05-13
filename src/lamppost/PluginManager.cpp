#include <lamppost/PluginManager.h>


namespace lp {
	PluginManager::PluginManager() {
	}

	PluginManager::~PluginManager() {
	}

	bool PluginManager::LoadTemplate(std::string filePath) {
		if(Filesystem::PathExists(filePath)) {
			// TODO: Implement plugin template loading.
		}

		return false;
	}

	std::shared_ptr<PluginInstance> PluginManager::InstantiateTemplate(std::string templateIdentifier) {
		if(mTemplates.find(templateIdentifier) != mTemplates.end()) {
			// TODO: Implement template instantiation.
		}

		return nullptr;
	}
}