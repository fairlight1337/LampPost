#include <lamppost/exceptions/KeyNotFoundException.h>


namespace lp {
	namespace exceptions {
		KeyNotFoundException::KeyNotFoundException(std::string key, std::string message) : Exception(message), mKey(key) {
		}

		KeyNotFoundException::~KeyNotFoundException() {
		}

		std::string KeyNotFoundException::GetKey() {
			return mKey;
		}
	}
}
