#include <lamppost/bus/Subscriber.h>


namespace lp
{
  namespace bus
  {
    Subscriber::Subscriber(std::string topic)
      : BusParticipant(std::move(topic)),
        mCallback(nullptr)
    {
    }

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
      : BusParticipant(std::move(topic)),
        mCallback(std::move(callback))
    {
    }

    void Subscriber::Receive(std::shared_ptr<messages::Message> message)
    {
      if(mCallback != nullptr)
      {
        mCallback(std::move(message));
      }
    }

    void Subscriber::Reset()
    {
      mCallback = nullptr;
    }

    void Subscriber::SetCallback(std::function<void(std::shared_ptr<messages::Datagram>)> callback)
    {
      mCallback = [callback](std::shared_ptr<lp::messages::Message> message)
        {
          std::shared_ptr<lp::messages::Datagram> datagram = message->GetDatagram();

          if(datagram != nullptr) {
            callback(datagram);
          }
        };
    }
  } // namespace bus
} // namespace lp
