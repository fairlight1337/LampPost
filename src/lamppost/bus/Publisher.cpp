#include <lamppost/bus/Publisher.h>


namespace lp {
  namespace bus {
    Publisher::Publisher(
      std::string topic,
      std::function<void(std::shared_ptr<lp::messages::Datagram>)> publishingFunction)
      : mTopic(topic), mPublishingFunction(publishingFunction) {
      if(publishingFunction == nullptr) {
        throw exceptions::ArgumentNullException("publishingFunction", "Publishing function may not be null.");
      }
    }

    void Publisher::Publish(std::shared_ptr<lp::messages::Datagram> datagram) {
      if(datagram == nullptr) {
        throw exceptions::ArgumentNullException("message", "Datagram may not be null.");
      }

      mPublishingFunction(datagram);
    }
  } // namespace bus
} // namespace lp
