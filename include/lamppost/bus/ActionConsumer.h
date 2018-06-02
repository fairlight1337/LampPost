#ifndef LAMPPOST_ACTIONCONSUMER_H
#define LAMPPOST_ACTIONCONSUMER_H


#include <lamppost/bus/BusParticipant.h>


namespace lp
{
  namespace bus
  {
    class ActionConsumer : public BusParticipant
    {
    public:
      ActionConsumer(std::string topic);
      ~ActionConsumer() = default;

      void Reset() override;
    };
  }
}


#endif //LAMPPOST_ACTIONCONSUMER_H
