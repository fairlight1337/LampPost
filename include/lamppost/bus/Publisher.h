#ifndef LAMPPOST_PUBLISHER_H
#define LAMPPOST_PUBLISHER_H


#include <functional>
#include <iostream>
#include <string>
#include <utility>

#include <lamppost/bus/BusParticipant.h>
#include <lamppost/exceptions/ArgumentNullException.h>
#include <lamppost/messages/RawDatagram.h>


namespace lp
{
  namespace bus
  {
    class Publisher : public BusParticipant
    {
    private:
      std::function<void(std::shared_ptr<messages::RawDatagram>)> mPublishingFunction;

    public:
      Publisher(std::string topic, std::function<void(std::shared_ptr<messages::RawDatagram>)> publishingFunction);
      virtual ~Publisher() = default;

      void Publish(std::shared_ptr<messages::RawDatagram> datagram);

      void Reset() override;
    };
  } // namespace bus
} // namespace lp


#endif //LAMPPOST_PUBLISHER_H
