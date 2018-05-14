#ifndef LAMPPOST_DATAGRAM_H
#define LAMPPOST_DATAGRAM_H


#include <string>
#include <map>
#include <memory>
#include <list>
#include <algorithm>

#include <lamppost/messages/DataBase.h>
#include <lamppost/messages/Data.h>
#include <lamppost/exceptions/KeyNotFoundException.h>
#include <lamppost/exceptions/InvalidOperationException.h>
#include <lamppost/exceptions/IndexOutOfBoundsException.h>


namespace lp {
	namespace messages {
		enum class DatagramType {
			List = 0,
			Dictionary = 1,
			Value = 2
		};

		class Datagram {
		private:
			std::map<std::string, std::shared_ptr<Datagram>> mDictionary;
			std::list<std::shared_ptr<Datagram>> mList;
			std::shared_ptr<DataBase> mValue;

			DatagramType mType;

			template<typename T>
			void SetValue(T value) {
				if(mType != DatagramType::Value) {
					mDictionary.clear();
					mList.clear();

					mType = DatagramType::Value;
				}

				mValue = std::make_shared<Data<T>>(value);
			}

		public:
			Datagram();
			~Datagram();

			DatagramType GetType();

			// Dictionary
			std::shared_ptr<Datagram>& operator[](std::string key);
			std::shared_ptr<Datagram>& Get(std::string key);
			void Remove(std::string key);

			// List
			std::shared_ptr<Datagram>& operator[](unsigned int index);
			std::shared_ptr<Datagram>& Get(unsigned int index);
			void Add(std::shared_ptr<Datagram> datagram);
			void Remove(unsigned int index);

			// Dictionary and List
			unsigned int GetCount();
			void Clear();

			// Values
			Datagram& operator=(std::string value);
			Datagram& operator=(int value);
			Datagram& operator=(double value);
			Datagram& operator=(bool value);
		};
	}
}


#endif //LAMPPOST_DATAGRAM_H
