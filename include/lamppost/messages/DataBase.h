#ifndef LAMPPOST_DATABASE_H
#define LAMPPOST_DATABASE_H


namespace lp {
	namespace messages {
		class DataBase {
		public:
			DataBase();
			~DataBase();

			template<typename T>
			T Get() {
				std::shared_ptr<Data<T>> data = std::dynamic_pointer_cast<T>(this);

				if(data != nullptr) {
					return data.Get();
				} else {
					return T();
				}
			}
		};
	}
}


#endif //LAMPPOST_DATABASE_H
