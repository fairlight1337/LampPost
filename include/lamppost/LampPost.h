#ifndef LAMPPOST_LAMPPOST_H
#define LAMPPOST_LAMPPOST_H


#include <atomic>
#include <map>
#include <memory>
#include <string>

#include <lamppost/PluginManager.h>
#include <lamppost/LampPostConfiguration.h>


namespace lp {
    class LampPost {
    private:
        std::atomic<bool> mShouldRun;
        LampPostConfiguration mConfiguration;
        PluginManager mPluginManager;

    public:
        LampPost(LampPostConfiguration configuration);
        ~LampPost();

        void Start();
        void Stop();
    };
}

#endif //LAMPPOST_LAMPPOST_H
