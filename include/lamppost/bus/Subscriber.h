#ifndef LAMPPOST_SUBSCRIBER_H
#define LAMPPOST_SUBSCRIBER_H


#include <functional>
#include <memory>
#include <string>
#include <utility>

#include <lamppost/messages/Message.h>


namespace lp {
  namespace bus {
    class Subscriber {
    private:
      std::string mTopic;
      std::function<void(std::shared_ptr<messages::Message>)> mCallback;

    public:
      explicit Subscriber(std::string topic, std::function<void(std::shared_ptr<messages::Datagram>)> callback);
      explicit Subscriber(std::string topic, std::function<void(std::shared_ptr<messages::Message>)> callback);
      virtual ~Subscriber() = default;

      void Receive(std::shared_ptr<messages::Message> message);

      std::string GetTopic();

      void Reset();
    };
  } // namespace bus
} // namespace lp


#endif //LAMPPOST_SUBSCRIBER_H
