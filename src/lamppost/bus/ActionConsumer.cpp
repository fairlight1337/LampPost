#include <lamppost/bus/ActionConsumer.h>


namespace lp
{
  namespace bus
  {
    ActionConsumer::ActionConsumer(
      std::shared_ptr<Subscriber> responseSubscriber,
      std::shared_ptr<Publisher> requestPublisher,
      std::string topic,
      std::function<void(std::shared_ptr<messages::Datagram>)> callback)
      : BusParticipant(std::move(topic)),
        mResponseSubscriber(std::move(responseSubscriber)),
        mRequestPublisher(std::move(requestPublisher)),
        mDefaultCallback(std::move(callback)),
        mCallback(nullptr)
    {
    }

    void ActionConsumer::Reset()
    {
      mCallback = nullptr;
    }

    void ActionConsumer::RequestAsync(
      std::shared_ptr<messages::Datagram> request,
      std::function<void(std::shared_ptr<messages::Datagram>)> callback)
    {
      if(callback == nullptr)
      {
        callback = mDefaultCallback;
      }

      mCallback = callback;

      std::shared_ptr<messages::Datagram> wrappedRequest = std::make_shared<messages::Datagram>();
      (*wrappedRequest)["invocationId"] = std::make_shared<messages::Datagram>();
      (*(*wrappedRequest)["invocationId"]) = static_cast<std::string>(utilities::Uuid::CreateUuid());
      (*wrappedRequest)["request"] = std::move(request);

      mRequestPublisher->Publish(wrappedRequest);
    }

    std::shared_ptr<messages::Datagram> ActionConsumer::Request(std::shared_ptr<messages::Datagram> request, int timeoutMs)
    {
      std::atomic<bool> waiting = true;
      std::condition_variable cv;
      std::mutex mtx;

      std::unique_lock<std::mutex> ul(mtx);

      std::shared_ptr<messages::Datagram> response = nullptr;

      RequestAsync(
        std::move(request),
        [&](std::shared_ptr<messages::Datagram> internalResponse)
        {
          response = internalResponse;
          waiting = false;
          cv.notify_one();
        });

      cv.wait_for(ul, std::chrono::milliseconds(timeoutMs),
                  [&]
                  {
                    return !waiting;
                  });

      return response;
    }

    void ActionConsumer::ProcessResponse(std::shared_ptr<messages::Datagram> response)
    {
      mCallback(std::move(response));
    }
  } // namespace bus
} // namespace lp
