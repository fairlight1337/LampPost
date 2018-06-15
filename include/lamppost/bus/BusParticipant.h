#ifndef LAMPPOST_BUSPARTICIPANT_H
#define LAMPPOST_BUSPARTICIPANT_H


#include <functional>
#include <string>

#include <lamppost/messages/Datagram.h>
#include <lamppost/messages/Message.h>


namespace lp
{
  namespace bus
  {
    typedef std::function<void(messages::Datagram)> DatagramCallbackType;
    typedef std::function<void(messages::Message)> MessageCallbackType;


    class BusParticipant
    {
    private:
      std::string mTopic;

    public:
      BusParticipant(std::string topic);
      virtual ~BusParticipant() = default;

      std::string GetTopic();

      virtual void Reset() = 0;
    };
  } // namespace bus
} // namespace lp


#endif //LAMPPOST_BUSPARTICIPANT_H
