#ifndef LAMPPOST_DUPLICATEKEYEXCEPTION_H
#define LAMPPOST_DUPLICATEKEYEXCEPTION_H


#include <string>
#include <utility>

#include <lamppost/exceptions/Exception.h>


namespace lp
{
  namespace exceptions
  {
    class DuplicateKeyException : public Exception
    {
    private:
      std::string mKey;

    public:
      DuplicateKeyException(std::string key, std::string message);
      virtual ~DuplicateKeyException() = default;

      std::string GetKey();
    };
  } // namespace exceptions
} // namespace lp


#endif //LAMPPOST_DUPLICATEKEYEXCEPTION_H
