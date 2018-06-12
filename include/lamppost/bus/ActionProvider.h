#ifndef LAMPPOST_ACTIONPROVIDER_H
#define LAMPPOST_ACTIONPROVIDER_H


#include <functional>
#include <iostream>
#include <string>
#include <utility>

#include <lamppost/bus/BusParticipant.h>
#include <lamppost/bus/Publisher.h>
#include <lamppost/bus/Subscriber.h>
#include <lamppost/messages/Datagram.h>


namespace lp
{
  namespace bus
  {
    class ActionProvider : public BusParticipant, public std::enable_shared_from_this<ActionProvider>
    {
    private:
      std::shared_ptr<Subscriber> mRequestSubscriber;
      std::shared_ptr<Publisher> mResponsePublisher;

      std::function<void(std::shared_ptr<ActionProvider>, std::string, std::shared_ptr<messages::Datagram>)> mCallback;

    public:
      ActionProvider(std::shared_ptr<Subscriber> requestSubscriber, std::shared_ptr<Publisher> responsePublisher, std::string topic, std::function<void(std::shared_ptr<ActionProvider>, std::string, std::shared_ptr<messages::Datagram>)> callback);
      ~ActionProvider() = default;

      void Reset() override;

      void Respond(std::string invocationId, std::shared_ptr<messages::Datagram> response);

      void ProcessRequest(std::string invocationId, std::shared_ptr<messages::Datagram> request);
    };
  } // namespace bus
} // namespace lp


#endif //LAMPPOST_ACTIONPROVIDER_H
