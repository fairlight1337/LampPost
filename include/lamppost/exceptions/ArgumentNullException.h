#ifndef LAMPPOST_ARGUMENTNULLEXCEPTION_H
#define LAMPPOST_ARGUMENTNULLEXCEPTION_H


#include <lamppost/exceptions/Exception.h>


namespace lp {
  namespace exceptions {
    class ArgumentNullException : public Exception {
    private:
      std::string mParameterName;

    public:
      ArgumentNullException(std::string parameterName, std::string message);
      ~ArgumentNullException();

      std::string GetParameterName();
    };
  }
}


#endif //LAMPPOST_ARGUMENTNULLEXCEPTION_H
