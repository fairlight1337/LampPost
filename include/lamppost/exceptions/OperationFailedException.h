#ifndef LAMPPOST_OPERATIONFAILEDEXCEPTION_H
#define LAMPPOST_OPERATIONFAILEDEXCEPTION_H


#include <utility>

#include <lamppost/exceptions/Exception.h>


namespace lp
{
  namespace exceptions
  {
    class OperationFailedException : public Exception
    {
    public:
      OperationFailedException(std::string message);
      virtual ~OperationFailedException() = default;
    };
  } // namespace exceptions
} // namespace lp


#endif //LAMPPOST_OPERATIONFAILEDEXCEPTION_H
