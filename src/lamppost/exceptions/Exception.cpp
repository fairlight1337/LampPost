#include <lamppost/exceptions/Exception.h>


namespace lp {
  namespace exceptions {
    Exception::Exception(std::string message) : std::runtime_error(std::move(message)) {
    }
  } // namespace exceptions
} // namespace lp
