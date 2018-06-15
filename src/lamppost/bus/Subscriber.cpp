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

    Subscriber::Subscriber(std::string topic, DatagramCallbackType callback)
      : BusParticipant(std::move(topic)),
        mCallback([callback](lp::messages::Message message)
                  {
                    messages::Datagram datagram = message.GetDatagram();

                    // TODO: Check value here.
                    callback(datagram);
                  })
    {
    }

    void Subscriber::Receive(messages::Message message)
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

    void Subscriber::SetDatagramCallback(DatagramCallbackType callback)
    {
      mCallback = [callback](lp::messages::Message message)
        {
          messages::Datagram datagram = message.GetDatagram();

          // TODO: Check value here.
          callback(datagram);
        };
    }

    void Subscriber::SetMessageCallback(MessageCallbackType callback)
    {
      mCallback = callback;
    }


  } // namespace bus
} // namespace lp
