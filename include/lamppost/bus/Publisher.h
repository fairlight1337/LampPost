#ifndef LAMPPOST_PUBLISHER_H
#define LAMPPOST_PUBLISHER_H


#include <string>
#include <functional>

#include <lamppost/messages/Datagram.h>
#include <lamppost/exceptions/ArgumentNullException.h>


namespace lp {
  namespace bus {
    class Publisher {
    private:
      std::string mTopic;
      std::function<void(std::shared_ptr<messages::Datagram>)> mPublishingFunction;

    public:
      Publisher(std::string topic, std::function<void(std::shared_ptr<messages::Datagram>)> publishingFunction);
      ~Publisher() = default;

      void Publish(std::shared_ptr<messages::Datagram> message);
    };
  } // namespace bus
} // namespace lp


#endif //LAMPPOST_PUBLISHER_H
