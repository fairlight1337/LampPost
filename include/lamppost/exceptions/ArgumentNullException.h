#ifndef LAMPPOST_ARGUMENTNULLEXCEPTION_H
#define LAMPPOST_ARGUMENTNULLEXCEPTION_H


#include <utility>

#include <lamppost/exceptions/Exception.h>


namespace lp {
  namespace exceptions {
    class ArgumentNullException : public Exception {
    private:
      std::string mParameterName;

    public:
      ArgumentNullException(std::string parameterName, std::string message);
      virtual ~ArgumentNullException() = default;

      std::string GetParameterName();
    };
  } // namespace exceptions
} // namespace lp


#endif //LAMPPOST_ARGUMENTNULLEXCEPTION_H
