#ifndef LAMPPOST_MESSAGE_H
#define LAMPPOST_MESSAGE_H


#include <lamppost/messages/Datagram.h>


namespace lp {
	namespace messages {
		class Message : public Datagram {
		public:
			Message();
			~Message();
		};
	}
}


#endif //LAMPPOST_MESSAGE_H
