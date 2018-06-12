#include <lamppost/bus/ActionProvider.h>


namespace lp
{
  namespace bus
  {
    ActionProvider::ActionProvider(
      std::shared_ptr<Subscriber> requestSubscriber,
      std::shared_ptr<Publisher> responsePublisher,
      std::string topic,
      std::function<void(std::shared_ptr<ActionProvider>, std::string, std::shared_ptr<messages::RawDatagram>)> callback)
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

    void ActionProvider::Respond(std::string invocationId, std::shared_ptr<messages::RawDatagram> response)
    {
      std::shared_ptr<messages::RawDatagram> wrappedResponse = std::make_shared<messages::RawDatagram>();
      (*wrappedResponse)["invocationId"] = std::make_shared<messages::RawDatagram>();
      (*(*wrappedResponse)["invocationId"]) = std::move(invocationId);
      (*wrappedResponse)["response"] = std::move(response);

      mResponsePublisher->Publish(wrappedResponse);
    }

    void ActionProvider::ProcessRequest(std::string invocationId, std::shared_ptr<messages::RawDatagram> request)
    {
      mCallback(this->shared_from_this(), std::move(invocationId), std::move(request));
    }
  } // namespace bus
} // namespace lp
