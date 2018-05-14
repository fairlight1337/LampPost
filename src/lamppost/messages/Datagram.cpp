#include <lamppost/messages/Datagram.h>


namespace lp {
	namespace messages {
		Datagram::Datagram() : mType(DatagramType::List), mValue(nullptr) {
		}

		Datagram::~Datagram() {
			mDictionary.clear();
			mList.clear();
			mValue = nullptr;
		}

		DatagramType Datagram::GetType() {
			return mType;
		}

		std::shared_ptr<Datagram>& Datagram::operator[](std::string key) {
			return Get(key);
		}

		std::shared_ptr<Datagram>& Datagram::Get(std::string key) {
			if(mType != DatagramType::Dictionary) {
				mType = DatagramType::Dictionary;
				mList.clear();
				mValue = nullptr;
			}

			if(mDictionary.find(key) != mDictionary.end()) {
				return mDictionary[key];
			} else {
				throw exceptions::KeyNotFoundException(key, "Key not found in dictionary.");
			}
		}

		void Datagram::Remove(std::string key) {
			if(mType != DatagramType::Dictionary) {
				throw exceptions::InvalidOperationException("Datagram must be a dictionary.");
			}

			std::map<std::string, std::shared_ptr<Datagram>>::iterator it = mDictionary.find(key);
			if(it != mDictionary.end()) {
				mDictionary.erase(it);
			} else {
				throw exceptions::KeyNotFoundException(key, "Key not found in dictionary.");
			}
		}

		std::shared_ptr<Datagram>& Datagram::operator[](unsigned int index) {
			return Get(index);
		}

		std::shared_ptr<Datagram>& Datagram::Get(unsigned int index) {
			if(mType != DatagramType::List) {
				mType = DatagramType::List;
				mDictionary.clear();
				mValue = nullptr;
			}

			if(index < mList.size()) {
				std::list<std::shared_ptr<Datagram>>::iterator it = mList.begin();
				std::advance(it, index);

				return *it;
			} else {
				throw exceptions::IndexOutOfBoundsException(index, "Index out of bounds.");
			}
		}

		void Datagram::Add(std::shared_ptr<Datagram> datagram) {
			if(mType != DatagramType::List) {
				throw exceptions::InvalidOperationException("Datagram must be a list.");
			}

			mList.push_back(datagram);
		}

		void Datagram::Remove(unsigned int index) {
			if(mType != DatagramType::List) {
				throw exceptions::InvalidOperationException("Datagram must be a list.");
			}

			if(index < mList.size()) {
				std::list<std::shared_ptr<Datagram>>::iterator it = mList.begin();
				std::advance(it, index);

				mList.erase(it);
			} else {
				throw exceptions::IndexOutOfBoundsException(index, "Index out of bounds.");
			}
		}

		unsigned int Datagram::GetCount() {
			if(mType == DatagramType::List) {
				return mList.size();
			} else if(mType == DatagramType::Dictionary) {
				return mDictionary.size();
			} else {
				throw exceptions::InvalidOperationException("Datagram must be a list or a dictionary.");
			}
		}

		void Datagram::Clear() {
			if(mType == DatagramType::List) {
				return mList.clear();
			} else if(mType == DatagramType::Dictionary) {
				return mDictionary.clear();
			} else {
				throw exceptions::InvalidOperationException("Datagram must be a list or a dictionary.");
			}
		}

		Datagram& Datagram::operator=(int value) {
			SetValue(value);

			return *this;
		}

		Datagram& Datagram::operator=(bool value) {
			SetValue(value);

			return *this;
		}

		Datagram& Datagram::operator=(double value) {
			SetValue(value);

			return *this;
		}

		Datagram& Datagram::operator=(std::string value) {
			SetValue(value);

			return *this;
		}
	}
}