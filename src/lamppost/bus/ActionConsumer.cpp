#include <lamppost/bus/ActionConsumer.h>


namespace lp
{
  namespace bus
  {
    ActionConsumer::ActionConsumer(
      std::shared_ptr<Subscriber> responseSubscriber,
      std::shared_ptr<Publisher> requestPublisher,
      std::string topic)
      : BusParticipant(std::move(topic)),
        mResponseSubscriber(std::move(responseSubscriber)),
        mRequestPublisher(std::move(requestPublisher))
    {
    }

    void ActionConsumer::Reset()
    {
    }

    void ActionConsumer::RequestAsync(
      std::shared_ptr<messages::Datagram> request,
      std::function<void(std::shared_ptr<messages::Datagram>)> callback)
    {
      if(callback == nullptr)
      {
        throw exceptions::ArgumentNullException("callback", "Callback may not be null.");
      }

      utilities::Uuid uuid = utilities::Uuid::CreateUuid();
      std::string uuidString = static_cast<std::string>(uuid);
      mOpenRequests[uuidString] = callback;

      std::shared_ptr<messages::Datagram> wrappedRequest = std::make_shared<messages::Datagram>();
      (*wrappedRequest)["invocationId"] = std::make_shared<messages::Datagram>();
      (*(*wrappedRequest)["invocationId"]) = uuidString;
      (*wrappedRequest)["request"] = std::move(request);

      mRequestPublisher->Publish(wrappedRequest);
    }

    std::shared_ptr<messages::Datagram> ActionConsumer::Request(std::shared_ptr<messages::Datagram> request, int timeoutMs)
    {
      std::atomic<bool> waiting;
      std::condition_variable cv;
      std::mutex mtx;

      std::unique_lock<std::mutex> ul(mtx);
      std::shared_ptr<messages::Datagram> response = nullptr;
      waiting = true;

      RequestAsync(
        std::move(request),
        [&](std::shared_ptr<messages::Datagram> internalResponse)
        {
          if(waiting)
          {
            response = internalResponse;
            waiting = false;
            cv.notify_one();
          }
        });

      cv.wait_for(ul, std::chrono::milliseconds(timeoutMs),
                  [&]
                  {
                    return !waiting;
                  });

      return response;
    }

    void ActionConsumer::ProcessResponse(std::string invocationId, std::shared_ptr<messages::Datagram> response)
    {
      if(mOpenRequests.find(invocationId) != mOpenRequests.end())
      {
        mOpenRequests[invocationId](std::move(response));
        mOpenRequests.erase(invocationId);
      }

    }
  } // namespace bus
} // namespace lp
