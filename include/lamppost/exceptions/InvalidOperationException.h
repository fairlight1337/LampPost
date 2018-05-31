#ifndef LAMPPOST_INVALIDOPERATIONEXCEPTION_H
#define LAMPPOST_INVALIDOPERATIONEXCEPTION_H


#include <lamppost/exceptions/Exception.h>


namespace lp
{
  namespace exceptions
  {
    class InvalidOperationException : public Exception
    {
    public:
      InvalidOperationException(std::string message);
      virtual ~InvalidOperationException() = default;
    };
  } // namespace exceptions
} // namespace lp


#endif //LAMPPOST_INVALIDOPERATIONEXCEPTION_H
