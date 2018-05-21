#ifndef LAMPPOST_EXCEPTION_H
#define LAMPPOST_EXCEPTION_H


#include <string>
#include <stdexcept>


namespace lp {
	namespace exceptions {
		class Exception : public std::runtime_error {
		public:
			Exception(std::string message);
			virtual ~Exception();
		};
	}
}


#endif //LAMPPOST_EXCEPTION_H
