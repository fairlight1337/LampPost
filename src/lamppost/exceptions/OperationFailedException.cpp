#include <lamppost/exceptions/OperationFailedException.h>


namespace lp
{
  namespace exceptions
  {
    OperationFailedException::OperationFailedException(std::string message)
      : Exception(message)
    {
    }
  } // namespace exceptions
} // namespace lp
