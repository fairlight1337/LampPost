#ifndef LAMPPOST_LAMPPOST_H
#define LAMPPOST_LAMPPOST_H

#include <atomic>


namespace lp {
    class LampPost {
    private:
        std::atomic<bool> mShouldRun;
    public:
        LampPost();
        ~LampPost();

        void Start();
        void Stop();
    };
}

#endif //LAMPPOST_LAMPPOST_H
