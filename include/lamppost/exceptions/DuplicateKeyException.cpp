#include <lamppost/exceptions/DuplicateKeyException.h>


namespace lp {
	namespace exceptions {
		DuplicateKeyException::DuplicateKeyException(std::string key, std::string message) : Exception(message), mKey(key) {
		}

		DuplicateKeyException::~DuplicateKeyException() {
		}

		std::string DuplicateKeyException::GetKey() {
			return mKey;
		}
	}
}
