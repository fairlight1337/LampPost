#ifndef LAMPPOST_SUBSCRIBER_H
#define LAMPPOST_SUBSCRIBER_H


#include <functional>
#include <memory>
#include <string>
#include <utility>

#include <lamppost/messages/Datagram.h>


namespace lp {
  namespace bus {
    class Subscriber {
    private:
      std::string mTopic;
      std::function<void(std::shared_ptr<messages::Datagram>)> mCallback;

    public:
      Subscriber(std::string topic, std::function<void(std::shared_ptr<messages::Datagram>)> callback);
      ~Subscriber() = default;

      void Receive(std::shared_ptr<messages::Datagram> datagram);

      std::string GetTopic();
    };
  } // namespace bus
} // namespace lp


#endif //LAMPPOST_SUBSCRIBER_H
