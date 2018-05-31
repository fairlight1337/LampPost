#include <lamppost/bus/Bus.h>


namespace lp {
  namespace bus {
    Bus::Bus(std::string name) : mName(name), mShouldRun(false) {
      if(name.empty()) {
        throw exceptions::ArgumentNullException("name", "Bus name may not be empty.");
      }

      mPublishMessageFunction = [this](std::shared_ptr<messages::Message> message) {
        std::lock_guard<std::mutex> lock(mQueueMutex);
        mQueuedMessages.push_back(message);

        mNotifier.notify_one();
      };
    }

    Bus::Bus(std::string name, std::function<void(std::shared_ptr<messages::Message>)> publishMessageFunction)
      : mName(name), mPublishMessageFunction(publishMessageFunction), mShouldRun(false) {
      if(name.empty()) {
        throw exceptions::ArgumentNullException("name", "Bus name may not be empty.");
      }

      if(publishMessageFunction == nullptr) {
        throw exceptions::ArgumentNullException("name", "Bus publishing function may not be null.");
      }
    }

    Bus::~Bus() {
      mPublishMessageFunction = nullptr;

      {
        std::lock_guard<std::mutex> lock(mChildBussesMutex);
        for(std::pair<std::string, std::shared_ptr<Bus>> busPair : mChildBusses) {
          busPair.second->Stop();
          busPair.second->Detach();
        }

        mChildBusses.clear();
      }

      {
        std::lock_guard<std::mutex> lock(mSubscribersMutex);
        mSubscribers.clear();
      }

      {
        std::lock_guard<std::mutex> lock(mPublishersMutex);
        mPublishers.clear();
      }

      {
        std::lock_guard<std::mutex> lock(mQueueMutex);
        mQueuedMessages.clear();
      }
    }

    std::shared_ptr<Bus> Bus::CreateChildBus(std::string name) {
      if(name.empty()) {
        throw exceptions::ArgumentNullException("name", "Child bus name may not be empty.");
      }

      std::lock_guard<std::mutex> lock(mChildBussesMutex);
      if(mChildBusses.find(name) != mChildBusses.end()) {
        throw exceptions::DuplicateKeyException(name, "Keys must be unique.");
      }

      std::shared_ptr<Bus> childBus = std::make_shared<Bus>(name, mPublishMessageFunction);
      mChildBusses[name] = childBus;

      return childBus;
    }

    std::shared_ptr<Bus> Bus::GetChildBus(std::string name) {
      std::lock_guard<std::mutex> lock(mChildBussesMutex);
      if(mChildBusses.find(name) == mChildBusses.end()) {
        throw exceptions::KeyNotFoundException(name, "Child bus by that name does not exist in this bus.");
      }

      return mChildBusses[name];
    }

    std::string Bus::GetName() {
      return mName;
    }

    void Bus::Publish(std::string topic, std::shared_ptr<messages::Datagram> datagram) {
      if(mPublishMessageFunction != nullptr) {
        mPublishMessageFunction(std::make_shared<messages::Message>(mName, topic, datagram));
      }
    }

    std::shared_ptr<Publisher> Bus::CreatePublisher(std::string topic) {
      std::shared_ptr<Publisher> publisher = std::make_shared<Publisher>(
        topic,
        [this, topic](std::shared_ptr<messages::Datagram> datagram) {
          this->Publish(topic, datagram);
        });

      std::lock_guard<std::mutex> lock(mPublishersMutex);
      mPublishers.push_back(publisher);

      return publisher;
    }

    void Bus::DeleteSubscriber(std::shared_ptr<Subscriber> subscriber) {
      std::lock_guard<std::mutex> lock(mSubscribersMutex);
      mSubscribers.remove(subscriber);
      subscriber->Reset();
    }

    void Bus::DeletePublisher(std::shared_ptr<Publisher> publisher) {
      std::lock_guard<std::mutex> lock(mPublishersMutex);
      mPublishers.remove(publisher);
      publisher->Reset();
    }

    void Bus::Start() {
      mShouldRun = true;

      Run();
    }

    void Bus::Run() {
      std::unique_lock<std::mutex> notifierLock(mNotifierMutex);

      while(mShouldRun) {
        mNotifier.wait_for(notifierLock, std::chrono::milliseconds(BUS_NOTIFIER_CHECK_TIMEOUT_MS), [this] {
          std::lock_guard<std::mutex> lock(mQueueMutex);

          return !mQueuedMessages.empty();
        });

        std::lock_guard<std::mutex> lock(mQueueMutex);
        for(const std::shared_ptr<messages::Message>& message : mQueuedMessages) {
          Distribute(message);
        }

        mQueuedMessages.clear();

        if(mName == "root") {
          mShouldRun = false;
        }
      }

      std::lock_guard<std::mutex> lock(mQueueMutex);
      mQueuedMessages.clear();
    }

    void Bus::Stop() {
      mShouldRun = false;
    }

    void Bus::Distribute(std::shared_ptr<lp::messages::Message> message) {
      if(mShouldRun) {
        {
          std::lock_guard<std::mutex> lock(mSubscribersMutex);
          for(const std::shared_ptr<Subscriber>& subscriber : mSubscribers) {
            // TODO: Match ant-like and possibly regex expressions for topic names here.
            if(subscriber->GetTopic() == message->GetTopic()) {
              subscriber->Receive(message);
            }
          }
        }

        {
          std::lock_guard<std::mutex> lock(mChildBussesMutex);
          for(std::pair<std::string, std::shared_ptr<Bus>> pair : mChildBusses) {
            pair.second->Distribute(message);
          }
        }
      }
    }

    void Bus::Detach() {
      mPublishMessageFunction = nullptr;
    }
  } // namespace bus
} // namespace lp
