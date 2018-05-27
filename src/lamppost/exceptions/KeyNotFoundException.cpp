#include <lamppost/exceptions/KeyNotFoundException.h>


namespace lp {
  namespace exceptions {
    KeyNotFoundException::KeyNotFoundException(std::string key, std::string message)
      : Exception(std::move(message)), mKey(std::move(key)) {
    }

    std::string KeyNotFoundException::GetKey() {
      return mKey;
    }
  } // namespace exceptions
} // namespace lp
