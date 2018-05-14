#ifndef LAMPPOST_DATA_H
#define LAMPPOST_DATA_H


#include <lamppost/messages/DataBase.h>


namespace lp {
	namespace messages {
		template<typename T>
		class Data : public DataBase {
		private:
			T mValue;

		public:
			Data() : mValue(T()) {}
			Data(T value) : mValue(value) {}
			~Data() {}

			T Get() {
				return mValue;
			}
		};
	}
}


#endif //LAMPPOST_DATA_H
