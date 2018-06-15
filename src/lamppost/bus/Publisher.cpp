#include <lamppost/bus/Publisher.h>


namespace lp
{
  namespace bus
  {
    Publisher::Publisher(
      std::string topic,
      DatagramCallbackType publishingFunction)
      : BusParticipant(topic),
        mPublishingFunction(publishingFunction)
    {
      if(publishingFunction == nullptr)
      {
        throw exceptions::ArgumentNullException("publishingFunction", "Publishing function may not be null.");
      }
    }

    void Publisher::Publish(messages::Datagram datagram)
    {
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
