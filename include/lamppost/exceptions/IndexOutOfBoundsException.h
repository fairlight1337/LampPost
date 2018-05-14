#ifndef LAMPPOST_INDEXOUTOFBOUNDSEXCEPTION_H
#define LAMPPOST_INDEXOUTOFBOUNDSEXCEPTION_H


#include <lamppost/exceptions/Exception.h>


namespace lp {
	namespace exceptions {
		class IndexOutOfBoundsException : public Exception {
		private:
			int mIndex;

		public:
			IndexOutOfBoundsException(int index, std::string message);
			~IndexOutOfBoundsException();

			int GetIndex();
		};
	}
}


#endif //LAMPPOST_INDEXOUTOFBOUNDSEXCEPTION_H
