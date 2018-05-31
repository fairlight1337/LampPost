#ifndef LAMPPOST_KEYNOTFOUNDEXCEPTION_H
#define LAMPPOST_KEYNOTFOUNDEXCEPTION_H


#include <utility>

#include <lamppost/exceptions/Exception.h>


namespace lp
{
  namespace exceptions
  {
    class KeyNotFoundException : public Exception
    {
    private:
      std::string mKey;

    public:
      KeyNotFoundException(std::string key, std::string message);
      virtual ~KeyNotFoundException() = default;

      std::string GetKey();
    };
  } // namespace exceptions
} // namespace lp


#endif //LAMPPOST_KEYNOTFOUNDEXCEPTION_H
