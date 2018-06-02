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
#include <lamppost/bus/ActionProvider.h>
#include <lamppost/bus/ActionConsumer.h>
#include <lamppost/bus/Publisher.h>
#include <lamppost/bus/Subscriber.h>
#include <lamppost/exceptions/DuplicateKeyException.h>
#include <lamppost/exceptions/KeyNotFoundException.h>
#include <lamppost/Identifiable.h>
#include <lamppost/messages/Datagram.h>
#include <lamppost/messages/Message.h>


namespace lp
{
  namespace bus
  {
    class Bus : public Identifiable
    {
    private:
      const int BUS_NOTIFIER_CHECK_TIMEOUT_MS = 100;

      std::map<std::string, std::shared_ptr<Bus>> mChildBusses;
      std::mutex mChildBussesMutex;
      std::mutex mQueueMutex;
      std::deque<std::shared_ptr<messages::Message>> mQueuedMessages;
      std::condition_variable mNotifier;
      std::mutex mNotifierMutex;
      std::function<void(std::shared_ptr<messages::Message>)> mPublishMessageFunction;
      std::atomic<bool> mShouldRun;
      bool mIsRootBus;

      std::mutex mPublishersMutex;
      std::list<std::shared_ptr<Publisher>> mPublishers;
      std::mutex mSubscribersMutex;
      std::list<std::shared_ptr<Subscriber>> mSubscribers;

      std::mutex mActionProvidersMutex;
      std::list<std::shared_ptr<ActionProvider>> mActionProviders;
      std::mutex mActionConsumersMutex;
      std::list<std::shared_ptr<ActionConsumer>> mActionConsumers;

      void Publish(std::string topic, std::shared_ptr<messages::Datagram> datagram);
      void Distribute(std::shared_ptr<messages::Message> message);

      template<class ClassType, class ... Args>
      std::shared_ptr<ClassType> CreateManagedResource(std::mutex& mutex, std::list<std::shared_ptr<ClassType>>& container, Args ... args)
      {
        std::shared_ptr<ClassType> instance = std::make_shared<ClassType>(std::forward<Args>(args)...);

        std::lock_guard<std::mutex> lock(mutex);
        container.push_back(instance);

        return instance;
      };

      template<class ClassType>
      void DeleteManagedResource(std::mutex& mutex, std::list<std::shared_ptr<ClassType>>& container, std::shared_ptr<ClassType> instance)
      {
        std::lock_guard<std::mutex> lock(mutex);
        container.remove(instance);
        instance->Reset();
      }

      template<class ClassType>
      bool ContainsManagedResource(std::mutex& mutex, std::list<std::shared_ptr<ClassType>>& container, std::shared_ptr<ClassType> instance)
      {
        std::lock_guard<std::mutex> lock(mutex);
        return std::find(container.begin(), container.end(), instance) != container.end();
      }

    public:
      Bus(std::string name);
      Bus(std::string name, std::function<void(std::shared_ptr<messages::Message>)> publishMessageFunction);
      virtual ~Bus();

      std::shared_ptr<Bus> CreateChildBus(std::string name);
      std::shared_ptr<Bus> GetChildBus(std::string name);

      std::string GetName();

      template<class ... Args>
      std::shared_ptr<Publisher> CreatePublisher(std::string topic, Args ... args)
      {
        return CreateManagedResource(
          mPublishersMutex,
          mPublishers,
          topic,
          [this, topic](std::shared_ptr<messages::Datagram> datagram)
          {
            this->Publish(topic, datagram);
          },
          std::forward<Args>(args)...);
      }

      template<class ... Args>
      std::shared_ptr<Subscriber> CreateSubscriber(Args ... args)
      {
        return CreateManagedResource(mSubscribersMutex, mSubscribers, std::forward<Args>(args)...);
      }

      template<class ... Args>
      std::shared_ptr<ActionProvider> CreateActionProvider(Args ... args)
      {
        return CreateManagedResource(mActionProvidersMutex, mActionProviders, std::forward<Args>(args)...);
      }

      template<class ... Args>
      std::shared_ptr<ActionConsumer> CreateActionConsumer(Args ... args)
      {
        return CreateManagedResource(mActionConsumersMutex, mActionConsumers, std::forward<Args>(args)...);
      }

      bool ContainsSubscriber(std::shared_ptr<bus::Subscriber> subscriber);
      bool ContainsPublisher(std::shared_ptr<bus::Publisher> publisher);

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
