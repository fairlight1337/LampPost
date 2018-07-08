#include <lamppost/bus/Bus.h>


namespace lp
{
  namespace bus
  {
    Bus::Bus(std::string name)
      : Identifiable(name),
        mShouldRun(false),
        mIsRootBus(true)
    {
      if(name.empty())
      {
        throw exceptions::ArgumentNullException("name", "Bus name may not be empty.");
      }

      mPublishMessageFunction = [this](messages::Message message)
        {
          {
            std::lock_guard<std::mutex> lock(mQueueMutex);
            mQueuedMessages.push_back(message);
          }

          mNotifier.notify_one();
        };
    }

    Bus::Bus(std::string name, std::function<void(messages::Message)> publishMessageFunction)
      : Identifiable(name),
        mPublishMessageFunction(publishMessageFunction),
        mShouldRun(false),
        mIsRootBus(false)
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

    void Bus::Publish(std::string topic, messages::Datagram datagram)
    {
      Publish(messages::Message(GetIdentifier(), std::move(topic), datagram));
    }

    void Bus::Publish(messages::Message message)
    {
      if(mPublishMessageFunction != nullptr)
      {
        mPublishMessageFunction(message);
      }
    }

    void Bus::DeleteSubscriber(std::shared_ptr<Subscriber> subscriber)
    {
      std::lock_guard<std::mutex> lock(mSubscribersMutex);
      DeleteManagedResource(mSubscribersMutex, mSubscribers, std::move(subscriber));
    }

    void Bus::DeletePublisher(std::shared_ptr<Publisher> publisher)
    {
      std::lock_guard<std::mutex> lock(mPublishersMutex);
      DeleteManagedResource(mPublishersMutex, mPublishers, std::move(publisher));
    }

    bool Bus::ContainsSubscriber(std::shared_ptr<bus::Subscriber> subscriber)
    {
      return ContainsManagedResource(mSubscribersMutex, mSubscribers, std::move(subscriber));
    }

    bool Bus::ContainsPublisher(std::shared_ptr<bus::Publisher> publisher)
    {
      return ContainsManagedResource(mPublishersMutex, mPublishers, std::move(publisher));
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

        std::deque<messages::Message> queuedMessages;
        {
          std::lock_guard<std::mutex> lock(mQueueMutex);
          queuedMessages = mQueuedMessages;

          mQueuedMessages.clear();
        }

        for(const messages::Message& message : queuedMessages)
        {
          Distribute(message);
        }
      }
    }

    void Bus::Stop()
    {
      mShouldRun = false;
    }

    bool Bus::TopicMatchesExpression(std::string expression, std::string topic)
    {
      bool matches = false;

      if(expression == topic)
      {
        matches = true;
      }
      else
      {
        // TODO(fairlight1337): Match ant-like and possibly regex expressions for topic names here.
      }

      return matches;
    }

    void Bus::Distribute(lp::messages::Message message)
    {
      if(mShouldRun)
      {
        {
          std::lock_guard<std::mutex> lock(mSubscribersMutex);
          for(const std::shared_ptr<Subscriber>& subscriber : mSubscribers)
          {
            if(TopicMatchesExpression(subscriber->GetTopic(), message.GetTopic()))
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
