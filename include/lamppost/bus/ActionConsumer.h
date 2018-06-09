#ifndef LAMPPOST_ACTIONCONSUMER_H
#define LAMPPOST_ACTIONCONSUMER_H


#include <atomic>
#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>
#include <string>
#include <utility>

#include <lamppost/bus/BusParticipant.h>
#include <lamppost/bus/Publisher.h>
#include <lamppost/bus/Subscriber.h>
#include <lamppost/messages/Datagram.h>
#include <lamppost/utilities/Uuid.h>


namespace lp
{
  namespace bus
  {
    class ActionConsumer : public BusParticipant
    {
    private:
      std::shared_ptr<Subscriber> mResponseSubscriber;
      std::shared_ptr<Publisher> mRequestPublisher;

      std::function<void(std::shared_ptr<messages::Datagram>)> mDefaultCallback;
      std::function<void(std::shared_ptr<messages::Datagram>)> mCallback;

    public:
      ActionConsumer(std::shared_ptr<Subscriber> responseSubscriber, std::shared_ptr<Publisher> requestPublisher, std::string topic, std::function<void(std::shared_ptr<messages::Datagram>)> callback);
      ~ActionConsumer() = default;

      void Reset() override;

      void RequestAsync(std::shared_ptr<messages::Datagram> request, std::function<void(std::shared_ptr<messages::Datagram>)> callback = nullptr);
      std::shared_ptr<messages::Datagram> Request(std::shared_ptr<messages::Datagram> request, int timeoutMs = 1000);

      void ProcessResponse(std::shared_ptr<messages::Datagram> response);
    };
  } // namespace bus
} // namespace lp


#endif //LAMPPOST_ACTIONCONSUMER_H
