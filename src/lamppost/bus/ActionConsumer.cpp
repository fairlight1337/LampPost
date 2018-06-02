#include <lamppost/bus/ActionConsumer.h>


namespace lp
{
  namespace bus
  {
    ActionConsumer::ActionConsumer(std::string topic)
      : BusParticipant(topic)
    {
    }

    void ActionConsumer::Reset()
    {
    }
  } // namespace bus
} // namespace lp
