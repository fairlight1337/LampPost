#include <lamppost/bus/ActionProvider.h>


namespace lp
{
  namespace bus
  {
    ActionProvider::ActionProvider(
      std::shared_ptr<Subscriber> requestSubscriber,
      std::shared_ptr<Publisher> responsePublisher,
      std::string topic,
      std::function<void(std::shared_ptr<ActionProvider>, std::string, messages::Datagram)> callback)
      : BusParticipant(std::move(topic)),
        mRequestSubscriber(std::move(requestSubscriber)),
        mResponsePublisher(std::move(responsePublisher)),
        mCallback(std::move(callback))
    {
    }

    void ActionProvider::Reset()
    {
      mCallback = nullptr;
    }

    void ActionProvider::Respond(std::string invocationId, messages::Datagram response)
    {
      messages::Datagram wrappedResponse;
      wrappedResponse["invocationId"] = std::move(invocationId);
      wrappedResponse["response"] = std::move(response);

      mResponsePublisher->Publish(wrappedResponse);
    }

    void ActionProvider::ProcessRequest(std::string invocationId, messages::Datagram request)
    {
      mCallback(this->shared_from_this(), std::move(invocationId), std::move(request));
    }
  } // namespace bus
} // namespace lp
