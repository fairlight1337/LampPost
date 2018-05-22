#ifndef LAMPPOST_KEYNOTFOUNDEXCEPTION_H
#define LAMPPOST_KEYNOTFOUNDEXCEPTION_H


#include <lamppost/exceptions/Exception.h>


namespace lp {
  namespace exceptions {
    class KeyNotFoundException : public Exception {
    private:
      std::string mKey;

    public:
      KeyNotFoundException(std::string key, std::string message);
      ~KeyNotFoundException();

      std::string GetKey();
    };
  }
}


#endif //LAMPPOST_KEYNOTFOUNDEXCEPTION_H
