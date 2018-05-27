#include <lamppost/exceptions/DuplicateKeyException.h>


namespace lp {
  namespace exceptions {
    DuplicateKeyException::DuplicateKeyException(std::string key, std::string message)
      : Exception(std::move(message)), mKey(std::move(key)) {
    }

    std::string DuplicateKeyException::GetKey() {
      return mKey;
    }
  } // namespace exceptions
} // namespace lp
