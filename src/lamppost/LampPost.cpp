#include <lamppost/LampPost.h>


namespace lp {
    LampPost::LampPost() {

    }

    LampPost::~LampPost() {

    }

    void LampPost::Start() {
        mShouldRun = true;

        while(mShouldRun) {
          // TODO: Implement main runner functionality.

          mShouldRun = false;
        }
    }

    void LampPost::Stop() {
        mShouldRun = false;
    }
}