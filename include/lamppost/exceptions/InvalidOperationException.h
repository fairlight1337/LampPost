#ifndef LAMPPOST_INVALIDOPERATIONEXCEPTION_H
#define LAMPPOST_INVALIDOPERATIONEXCEPTION_H


#include <lamppost/exceptions/Exception.h>


namespace lp {
  namespace exceptions {
    class InvalidOperationException : public Exception {
    public:
      InvalidOperationException(std::string message);
      ~InvalidOperationException();
    };
  }
}


#endif //LAMPPOST_INVALIDOPERATIONEXCEPTION_H
