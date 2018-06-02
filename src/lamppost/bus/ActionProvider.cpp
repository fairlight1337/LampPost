#include <lamppost/bus/ActionProvider.h>


namespace lp
{
  namespace bus
  {
    ActionProvider::ActionProvider(std::string topic)
      : BusParticipant(topic)
    {
    }

    void ActionProvider::Reset()
    {
    }
  } // namespace bus
} // namespace lp
