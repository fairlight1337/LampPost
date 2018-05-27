#include <lamppost/exceptions/ArgumentNullException.h>


namespace lp {
  namespace exceptions {
    ArgumentNullException::ArgumentNullException(std::string parameterName, std::string message) : Exception(message), mParameterName(parameterName) {
    }

    std::string ArgumentNullException::GetParameterName() {
      return mParameterName;
    }
  } // namespace exceptions
} // namespace lp
