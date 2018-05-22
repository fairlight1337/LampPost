#include <lamppost/exceptions/ArgumentNullException.h>


namespace lp {
  namespace exceptions {
    ArgumentNullException::ArgumentNullException(std::string parameterName, std::string message) : Exception(message), mParameterName(parameterName) {

    }

    ArgumentNullException::~ArgumentNullException() {
    }

    std::string ArgumentNullException::GetParameterName() {
      return mParameterName;
    }
  }
}
