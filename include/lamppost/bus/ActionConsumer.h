#ifndef LAMPPOST_ACTIONCONSUMER_H
#define LAMPPOST_ACTIONCONSUMER_H


#include <atomic>
#include <condition_variable>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <utility>

#include <lamppost/bus/BusParticipant.h>
#include <lamppost/bus/Publisher.h>
#include <lamppost/bus/Subscriber.h>
#include <lamppost/messages/RawDatagram.h>
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

      std::map<std::string, std::function<void(std::shared_ptr<messages::RawDatagram>)>> mOpenRequests;

    public:
      ActionConsumer(std::shared_ptr<Subscriber> responseSubscriber, std::shared_ptr<Publisher> requestPublisher, std::string topic);
      ~ActionConsumer() = default;

      void Reset() override;

      void RequestAsync(std::shared_ptr<messages::RawDatagram> request, std::function<void(std::shared_ptr<messages::RawDatagram>)> callback = nullptr);
      std::shared_ptr<messages::RawDatagram> Request(std::shared_ptr<messages::RawDatagram> request, int timeoutMs = 1000);

      void ProcessResponse(std::string invocationId, std::shared_ptr<messages::RawDatagram> response);
    };
  } // namespace bus
} // namespace lp


#endif //LAMPPOST_ACTIONCONSUMER_H
