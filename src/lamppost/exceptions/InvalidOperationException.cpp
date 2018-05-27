#include <lamppost/exceptions/InvalidOperationException.h>


namespace lp {
  namespace exceptions {
    InvalidOperationException::InvalidOperationException(std::string message) : Exception(message) {
    }
  }
}