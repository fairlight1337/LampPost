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
      std::function<void(std::shared_ptr<messages::Message>)> mCallback;

    public:
      Subscriber(std::string topic);
      explicit Subscriber(std::string topic, std::function<void(std::shared_ptr<messages::RawDatagram>)> callback);
      explicit Subscriber(std::string topic, std::function<void(std::shared_ptr<messages::Message>)> callback);
      virtual ~Subscriber() = default;

      void Receive(std::shared_ptr<messages::Message> message);

      void Reset() override;

      void SetCallback(std::function<void(std::shared_ptr<messages::RawDatagram>)> callback);
    };
  } // namespace bus
} // namespace lp


#endif //LAMPPOST_SUBSCRIBER_H
