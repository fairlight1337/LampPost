#ifndef LAMPPOST_DUPLICATEKEYEXCEPTION_H
#define LAMPPOST_DUPLICATEKEYEXCEPTION_H


#include <string>

#include <lamppost/exceptions/Exception.h>


namespace lp {
	namespace exceptions {
		class DuplicateKeyException : public Exception {
		private:
			std::string mKey;

		public:
			DuplicateKeyException(std::string key, std::string message);
			~DuplicateKeyException();

			std::string GetKey();
		};
	}
}


#endif //LAMPPOST_DUPLICATEKEYEXCEPTION_H
