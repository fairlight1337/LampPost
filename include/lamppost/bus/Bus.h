#ifndef LAMPPOST_BUS_H
#define LAMPPOST_BUS_H


// System includes
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <deque>
#include <functional>
#include <iostream>
#include <list>
#include <memory>
#include <map>
#include <mutex>
#include <string>
#include <thread>

// Private includes
#include <lamppost/bus/Publisher.h>
#include <lamppost/bus/Subscriber.h>
#include <lamppost/exceptions/DuplicateKeyException.h>
#include <lamppost/exceptions/KeyNotFoundException.h>
#include <lamppost/messages/Datagram.h>
#include <lamppost/messages/Message.h>


namespace lp {
  namespace bus {
    class Bus {
    private:
      const int BUS_NOTIFIER_CHECK_TIMEOUT_MS = 100;

      std::string mName;
      std::map<std::string, std::shared_ptr<Bus>> mChildBusses;
      std::mutex mChildBussesMutex;
      std::mutex mQueueMutex;
      std::deque<std::shared_ptr<messages::Message>> mQueuedMessages;
      std::condition_variable mNotifier;
      std::mutex mNotifierMutex;
      std::function<void(std::shared_ptr<messages::Message>)> mPublishMessageFunction;
      std::atomic<bool> mShouldRun;

      std::mutex mPublishersMutex;
      std::list<std::shared_ptr<Publisher>> mPublishers;
      std::mutex mSubscribersMutex;
      std::list<std::shared_ptr<Subscriber>> mSubscribers;

      void Publish(std::string topic, std::shared_ptr<messages::Datagram> datagram);
      void Distribute(std::shared_ptr<messages::Message> message);

    public:
      Bus(std::string name);
      Bus(std::string name, std::function<void(std::shared_ptr<messages::Message>)> publishMessageFunction);
      ~Bus();

      std::shared_ptr<Bus> CreateChildBus(std::string name);
      std::shared_ptr<Bus> GetChildBus(std::string name);

      std::string GetName();

      std::shared_ptr<Publisher> CreatePublisher(std::string topic);

      template<class ... Args>
      std::shared_ptr<Subscriber> CreateSubscriber(Args ... args) {
        std::shared_ptr<Subscriber> subscriber = std::make_shared<Subscriber>(std::forward<Args>(args)...);

        std::lock_guard<std::mutex> lock(mSubscribersMutex);
        mSubscribers.push_back(subscriber);

        return subscriber;
      }

      void DeleteSubscriber(std::shared_ptr<bus::Subscriber> subscriber);
      void DeletePublisher(std::shared_ptr<bus::Publisher> publisher);

      void Start();
      void Run();
      void Stop();

      void Detach();
    };
  } // namespace bus
} // namespace lp


#endif //LAMPPOST_BUS_H
