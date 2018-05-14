#ifndef LAMPPOST_BUS_H
#define LAMPPOST_BUS_H


#include <string>
#include <memory>
#include <map>
#include <functional>

#include <lamppost/messages/Message.h>
#include <lamppost/messages/Datagram.h>
#include <lamppost/bus/Publisher.h>
#include <lamppost/exceptions/DuplicateKeyException.h>
#include <lamppost/exceptions/KeyNotFoundException.h>


namespace lp {
	namespace bus {
		class Bus {
		private:
			std::string mName;
			std::map<std::string, std::shared_ptr<Bus>> mChildBusses;

			void Publish(std::string topic, std::shared_ptr<messages::Datagram> datagram);

		public:
			Bus(std::string name);
			~Bus();

			std::shared_ptr<Bus> CreateChildBus(std::string name);
			std::shared_ptr<Bus> GetChildBus(std::string name);

			std::string GetName();

			std::shared_ptr<Publisher> CreatePublisher(std::string topic);
		};
	}
}


#endif //LAMPPOST_BUS_H
