#ifndef LAMPPOST_SUBSCRIBER_H
#define LAMPPOST_SUBSCRIBER_H


#include <functional>
#include <memory>
#include <string>
#include <utility>

#include <lamppost/bus/BusParticipant.h>
#include <lamppost/messages/Message.h>


namespace lp
{
  namespace bus
  {
    class Subscriber : public BusParticipant
    {
    private:
      MessageCallbackType mCallback;

    public:
      Subscriber(std::string topic);
      explicit Subscriber(std::string topic, DatagramCallbackType callback);
      virtual ~Subscriber() = default;

      void Receive(messages::Message message);

      void Reset() override;

      void SetDatagramCallback(DatagramCallbackType callback);
      void SetMessageCallback(MessageCallbackType callback);
    };
  } // namespace bus
} // namespace lp


#endif //LAMPPOST_SUBSCRIBER_H
