#ifndef LAMPPOST_BUS_H
#define LAMPPOST_BUS_H


#include <string>
#include <memory>
#include <map>

#include <lamppost/exceptions/DuplicateKeyException.h>
#include <lamppost/exceptions/KeyNotFoundException.h>


namespace lp {
	namespace bus {
		class Bus {
		private:
			std::string mName;
			std::map<std::string, std::shared_ptr<Bus>> mChildBusses;

		public:
			Bus(std::string name);
			~Bus();

			std::shared_ptr<Bus> CreateChildBus(std::string name);
			std::shared_ptr<Bus> GetChildBus(std::string name);

			std::string GetName();
		};
	}
}


#endif //LAMPPOST_BUS_H
