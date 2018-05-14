#include <lamppost/bus/Bus.h>


namespace lp {
	namespace bus {
		Bus::Bus(std::string name) : mName(name) {
		}

		Bus::~Bus() {
		}

		std::shared_ptr<Bus> Bus::CreateChildBus(std::string name) {
			if(mChildBusses.find(name) != mChildBusses.end()) {
				throw exceptions::DuplicateKeyException(name, "Keys must be unique.");
			}

			std::shared_ptr<Bus> childBus = std::make_shared<Bus>(name);
			mChildBusses[name] = childBus;

			return childBus;
		}

		std::shared_ptr<Bus> Bus::GetChildBus(std::string name) {
			if(mChildBusses.find(name) == mChildBusses.end()) {
				throw exceptions::KeyNotFoundException(name, "Child bus by that name does not exist in this bus.");
			}

			return mChildBusses[name];
		}

		std::string Bus::GetName() {
			return mName;
		}

		void Bus::Publish(std::string topic, std::shared_ptr<messages::Datagram> datagram) {
			// TODO: Implement publishing.
		}

		std::shared_ptr<Publisher> Bus::CreatePublisher(std::string topic) {
			std::shared_ptr<Publisher> publisher = std::make_shared<Publisher>(
				topic,
			  [this, topic](std::shared_ptr<messages::Datagram> datagram) {
					this->Publish(topic, datagram);
				});

			return publisher;
		}
	}
}
