#include <lamppost/bus/Subscriber.h>


namespace lp {
  namespace bus {
    Subscriber::Subscriber(std::string topic, std::function<void(std::shared_ptr<lp::messages::Datagram>)> callback) : mTopic(topic), mCallback(callback) {
    }

    Subscriber::~Subscriber() {
    }

    void Subscriber::Receive(std::shared_ptr<lp::messages::Datagram> datagram) {
      mCallback(datagram);
    }

    std::string Subscriber::GetTopic() {
      return mTopic;
    }
  }
}