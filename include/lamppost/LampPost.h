#ifndef LAMPPOST_LAMPPOST_H
#define LAMPPOST_LAMPPOST_H


#include <atomic>
#include <map>
#include <memory>
#include <string>

#include <lamppost/PluginManager.h>


namespace lp {
    class LampPost {
    private:
        std::atomic<bool> mShouldRun;
        PluginManager mPluginManager;

    public:
        LampPost();
        ~LampPost();

        void Start();
        void Stop();
    };
}

#endif //LAMPPOST_LAMPPOST_H
