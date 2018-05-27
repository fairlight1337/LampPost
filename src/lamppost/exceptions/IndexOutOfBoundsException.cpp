#include <lamppost/exceptions/IndexOutOfBoundsException.h>


namespace lp {
  namespace exceptions {
    IndexOutOfBoundsException::IndexOutOfBoundsException(int index, std::string message) : Exception(message), mIndex(index) {
    }

    int IndexOutOfBoundsException::GetIndex() {
      return mIndex;
    }
  }
}