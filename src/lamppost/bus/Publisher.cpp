#include <lamppost/bus/Publisher.h>


namespace lp
{
  namespace bus
  {
    Publisher::Publisher(
      std::string topic,
      std::function<void(std::shared_ptr<lp::messages::RawDatagram>)> publishingFunction)
      : BusParticipant(topic),
        mPublishingFunction(publishingFunction)
    {
      if(publishingFunction == nullptr)
      {
        throw exceptions::ArgumentNullException("publishingFunction", "Publishing function may not be null.");
      }
    }

    void Publisher::Publish(std::shared_ptr<lp::messages::RawDatagram> datagram)
    {
      if(datagram == nullptr)
      {
        throw exceptions::ArgumentNullException("message", "RawDatagram may not be null.");
      }

      if(mPublishingFunction != nullptr)
      {
        mPublishingFunction(datagram);
      }
    }

    void Publisher::Reset()
    {
      mPublishingFunction = nullptr;
    }
  } // namespace bus
} // namespace lp
