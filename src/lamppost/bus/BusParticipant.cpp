#include <lamppost/bus/BusParticipant.h>


namespace lp
{
  namespace bus
  {
    BusParticipant::BusParticipant(std::string topic)
      : mTopic(topic)
    {
    }

    std::string BusParticipant::GetTopic()
    {
      return mTopic;
    }
  }
}
