#include <lamppost/bus/Subscriber.h>


namespace lp
{
  namespace bus
  {
    Subscriber::Subscriber(std::string topic, std::function<void(std::shared_ptr<messages::Datagram>)> callback)
      : BusParticipant(topic),
        mCallback([callback](std::shared_ptr<lp::messages::Message> message)
                  {
                    std::shared_ptr<lp::messages::Datagram> datagram = message->GetDatagram();

                    if(datagram != nullptr) {
                      callback(datagram);
                    }
                  })
    {
    }

    Subscriber::Subscriber(std::string topic, std::function<void(std::shared_ptr<messages::Message>)> callback)
      : BusParticipant(topic),
        mCallback(callback)
    {
    }

    void Subscriber::Receive(std::shared_ptr<messages::Message> message)
    {
      if(mCallback != nullptr)
      {
        mCallback(message);
      }
    }

    void Subscriber::Reset()
    {
      mCallback = nullptr;
    }
  } // namespace bus
} // namespace lp
