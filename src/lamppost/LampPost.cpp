#include <lamppost/LampPost.h>


namespace lp {
    LampPost::LampPost(LampPostConfiguration configuration)
      : mConfiguration(configuration),
        mPluginManager(configuration.mPuginManagerConfiguration) {
    }

    LampPost::~LampPost() {

    }

    void LampPost::Start() {
        mShouldRun = true;

        mPluginManager.LoadTemplates();
        std::shared_ptr<PluginInstance> instance = mPluginManager.InstantiateTemplate("SysInfo", PluginConfiguration());

        while(mShouldRun) {
          // TODO: Implement main runner functionality.

          mShouldRun = false;
        }
    }

    void LampPost::Stop() {
        mShouldRun = false;
    }
}