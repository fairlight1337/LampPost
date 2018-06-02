#ifndef LAMPPOST_ACTIONPROVIDER_H
#define LAMPPOST_ACTIONPROVIDER_H


#include <string>

#include <lamppost/bus/BusParticipant.h>


namespace lp
{
  namespace bus
  {
    class ActionProvider : public BusParticipant
    {
    public:
      ActionProvider(std::string topic);
      ~ActionProvider() = default;

      void Reset() override;
    };
  }
}


#endif //LAMPPOST_ACTIONPROVIDER_H
