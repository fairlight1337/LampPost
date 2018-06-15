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
      messages::Datagram request,
      std::function<void(messages::Datagram)> callback)
    {
      if(callback == nullptr)
      {
        throw exceptions::ArgumentNullException("callback", "Callback may not be null.");
      }

      utilities::Uuid uuid = utilities::Uuid::CreateUuid();
      std::string uuidString = static_cast<std::string>(uuid);
      mOpenRequests[uuidString] = callback;

      messages::Datagram wrappedRequest;
      wrappedRequest["invocationId"] = uuidString;
      wrappedRequest["request"] = std::move(request);

      mRequestPublisher->Publish(wrappedRequest);
    }

    messages::Datagram ActionConsumer::Request(messages::Datagram request, int timeoutMs)
    {
      std::atomic<bool> waiting;
      std::condition_variable cv;
      std::mutex mtx;

      std::unique_lock<std::mutex> ul(mtx);
      messages::Datagram response;
      waiting = true;

      RequestAsync(
        std::move(request),
        [&](messages::Datagram internalResponse)
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

    void ActionConsumer::ProcessResponse(std::string invocationId, messages::Datagram response)
    {
      if(mOpenRequests.find(invocationId) != mOpenRequests.end())
      {
        mOpenRequests[invocationId](std::move(response));
        mOpenRequests.erase(invocationId);
      }

    }
  } // namespace bus
} // namespace lp
