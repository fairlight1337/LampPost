#include <lamppost/exceptions/Exception.h>


namespace lp {
  namespace exceptions {
    Exception::Exception(std::string message) : std::runtime_error(message.c_str()) {
    }

    Exception::~Exception() {
    }
  }
}
