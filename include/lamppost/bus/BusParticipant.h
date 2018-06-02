#ifndef LAMPPOST_BUSPARTICIPANT_H
#define LAMPPOST_BUSPARTICIPANT_H


#include <string>


namespace lp
{
  namespace bus
  {
    class BusParticipant
    {
    private:
      std::string mTopic;

    public:
      BusParticipant(std::string topic);
      ~BusParticipant() = default;

      std::string GetTopic();

      virtual void Reset() = 0;
    };
  }
}


#endif //LAMPPOST_BUSPARTICIPANT_H
