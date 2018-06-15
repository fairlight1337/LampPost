#ifndef LAMPPOST_PUBLISHER_H
#define LAMPPOST_PUBLISHER_H


#include <functional>
#include <iostream>
#include <string>
#include <utility>

#include <lamppost/bus/BusParticipant.h>
#include <lamppost/exceptions/ArgumentNullException.h>
#include <lamppost/messages/Datagram.h>


namespace lp
{
  namespace bus
  {
    class Publisher : public BusParticipant
    {
    private:
      DatagramCallbackType mPublishingFunction;

    public:
      Publisher(std::string topic, DatagramCallbackType publishingFunction);
      virtual ~Publisher() = default;

      void Publish(messages::Datagram datagram);

      void Reset() override;
    };
  } // namespace bus
} // namespace lp


#endif //LAMPPOST_PUBLISHER_H
