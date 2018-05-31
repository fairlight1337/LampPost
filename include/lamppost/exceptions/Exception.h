#ifndef LAMPPOST_EXCEPTION_H
#define LAMPPOST_EXCEPTION_H


#include <stdexcept>
#include <string>


namespace lp
{
  namespace exceptions
  {
    class Exception : public std::runtime_error
    {
    public:
      Exception(std::string message);
      virtual ~Exception() = default;
    };
  } // namespace exceptions
} // namespace lp


#endif //LAMPPOST_EXCEPTION_H
