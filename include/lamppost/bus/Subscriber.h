#ifndef LAMPPOST_SUBSCRIBER_H
#define LAMPPOST_SUBSCRIBER_H


#include <string>
#include <memory>
#include <functional>

#include <lamppost/messages/Datagram.h>


namespace lp {
	namespace bus {
		class Subscriber {
		private:
			std::string mTopic;
			std::function<void(std::shared_ptr<messages::Datagram>)> mCallback;

		public:
			Subscriber(std::string topic, std::function<void(std::shared_ptr<messages::Datagram>)> callback);
			~Subscriber();

			void Receive(std::shared_ptr<messages::Datagram> datagram);

			std::string GetTopic();
		};
	}
}


#endif //LAMPPOST_SUBSCRIBER_H
