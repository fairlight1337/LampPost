#include <lamppost/bus/Bus.h>


namespace lp
{
  namespace bus
  {
    Bus::Bus(std::string name) : Identifiable(name), mShouldRun(false), mIsRootBus(true)
    {
      if(name.empty())
      {
        throw exceptions::ArgumentNullException("name", "Bus name may not be empty.");
      }

      mPublishMessageFunction = [this](std::shared_ptr<messages::Message> message)
        {
        std::lock_guard<std::mutex> lock(mQueueMutex);
        mQueuedMessages.push_back(message);

        mNotifier.notify_one();
      };
    }

    Bus::Bus(std::string name, std::function<void(std::shared_ptr<messages::Message>)> publishMessageFunction)
      : Identifiable(name), mPublishMessageFunction(publishMessageFunction), mShouldRun(false), mIsRootBus(false)
    {
      if(name.empty())
      {
        throw exceptions::ArgumentNullException("name", "Bus name may not be empty.");
      }

      if(publishMessageFunction == nullptr)
      {
        throw exceptions::ArgumentNullException("name", "Bus publishing function may not be null.");
      }
    }

    Bus::~Bus()
    {
      mPublishMessageFunction = nullptr;

      {
        std::lock_guard<std::mutex> lock(mChildBussesMutex);
        for(std::pair<std::string, std::shared_ptr<Bus>> busPair : mChildBusses)
        {
          busPair.second->Stop();
          busPair.second->Detach();
        }

        mChildBusses.clear();
      }
    }

    std::shared_ptr<Bus> Bus::CreateChildBus(std::string name)
    {
      if(name.empty())
      {
        throw exceptions::ArgumentNullException("name", "Child bus name may not be empty.");
      }

      std::lock_guard<std::mutex> lock(mChildBussesMutex);
      if(mChildBusses.find(name) != mChildBusses.end())
      {
        throw exceptions::DuplicateKeyException(name, "Child bus names must be unique.");
      }

      std::shared_ptr<Bus> childBus = std::make_shared<Bus>(name, mPublishMessageFunction);
      mChildBusses[name] = childBus;
      childBus->Start();

      return childBus;
    }

    std::shared_ptr<Bus> Bus::GetChildBus(std::string name)
    {
      std::lock_guard<std::mutex> lock(mChildBussesMutex);
      if(mChildBusses.find(name) == mChildBusses.end())
      {
        throw exceptions::KeyNotFoundException(name, "Child bus by that name does not exist in this bus.");
      }

      return mChildBusses[name];
    }

    void Bus::Publish(std::string topic, std::shared_ptr<messages::Datagram> datagram)
    {
      if(mPublishMessageFunction != nullptr)
      {
        mPublishMessageFunction(std::make_shared<messages::Message>(GetIdentifier(), topic, datagram));
      }
    }

    std::shared_ptr<Publisher> Bus::CreatePublisher(std::string topic)
    {
      std::shared_ptr<Publisher> publisher = std::make_shared<Publisher>(
        topic,
        [this, topic](std::shared_ptr<messages::Datagram> datagram)
        {
          this->Publish(topic, datagram);
        });

      std::lock_guard<std::mutex> lock(mPublishersMutex);
      mPublishers.push_back(publisher);

      return publisher;
    }

    void Bus::DeleteSubscriber(std::shared_ptr<Subscriber> subscriber)
    {
      std::lock_guard<std::mutex> lock(mSubscribersMutex);
      mSubscribers.remove(subscriber);
      subscriber->Reset();
    }

    void Bus::DeletePublisher(std::shared_ptr<Publisher> publisher)
    {
      std::lock_guard<std::mutex> lock(mPublishersMutex);
      mPublishers.remove(publisher);
      publisher->Reset();
    }

    void Bus::Start()
    {
      mShouldRun = true;

      if(mIsRootBus)
      {
        Run();
      }
    }

    void Bus::Run()
    {
      std::unique_lock<std::mutex> notifierLock(mNotifierMutex);

      while(mShouldRun)
      {
        mNotifier.wait_for(notifierLock,
                           std::chrono::milliseconds(BUS_NOTIFIER_CHECK_TIMEOUT_MS),
                           [this]
                           {
                             std::lock_guard<std::mutex> lock(mQueueMutex);
                             return !mQueuedMessages.empty() || !mShouldRun;
                           });

        if(!mShouldRun)
        {
          break;
        }

        std::lock_guard<std::mutex> lock(mQueueMutex);
        for(const std::shared_ptr<messages::Message>& message : mQueuedMessages)
        {
          Distribute(message);
        }

        mQueuedMessages.clear();
      }
    }

    void Bus::Stop()
    {
      mShouldRun = false;
    }

    void Bus::Distribute(std::shared_ptr<lp::messages::Message> message)
    {
      if(mShouldRun)
      {
        {
          std::lock_guard<std::mutex> lock(mSubscribersMutex);
          for(const std::shared_ptr<Subscriber>& subscriber : mSubscribers)
          {
            // TODO(fairlight1337): Match ant-like and possibly regex expressions for topic names here.
            if(subscriber->GetTopic() == message->GetTopic())
            {
              subscriber->Receive(message);
            }
          }
        }

        {
          std::lock_guard<std::mutex> lock(mChildBussesMutex);
          for(std::pair<std::string, std::shared_ptr<Bus>> pair : mChildBusses)
          {
            pair.second->Distribute(message);
          }
        }
      }
    }

    void Bus::Detach()
    {
      mPublishMessageFunction = nullptr;
      
      std::lock_guard<std::mutex> lock(mChildBussesMutex);
      for(std::pair<std::string, std::shared_ptr<Bus>> busPair : mChildBusses)
      {
        busPair.second->Stop();
        busPair.second->Detach();
      }
    }
  } // namespace bus
} // namespace lp
