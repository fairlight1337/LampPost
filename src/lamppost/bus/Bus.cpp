#include <lamppost/bus/Bus.h>


namespace lp {
  namespace bus {
    Bus::Bus(std::string name) : mName(name) {
      if(name.empty()) {
        throw exceptions::ArgumentNullException("name", "Bus name may not be empty.");
      }

      mPublishMessageFunction = [this](std::shared_ptr<messages::Message> message) {
        std::lock_guard<std::mutex> lock(mQueueMutex);
        mQueuedMessages.push_back(message);

        mNotifier.notify_one();
      };
    }

    Bus::Bus(std::string name, std::function<void(std::shared_ptr<messages::Message>)> publishMessageFunction) : mName(name), mPublishMessageFunction(publishMessageFunction) {
      if(name.empty()) {
        throw exceptions::ArgumentNullException("name", "Bus name may not be empty.");
      }

      if(publishMessageFunction == nullptr) {
        throw exceptions::ArgumentNullException("name", "Bus publishing function may not be null.");
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
      mPublishMessageFunction(std::make_shared<messages::Message>(mName, topic, datagram));
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

    std::shared_ptr<Subscriber> Bus::CreateSubscriber(std::string topic, std::function<void(std::shared_ptr<messages::Datagram>)> callback) {
      std::shared_ptr<Subscriber> subscriber = std::make_shared<Subscriber>(topic, callback);

      std::lock_guard<std::mutex> lock(mSubscribersMutex);
      mSubscribers.push_back(subscriber);

      return subscriber;
    }

    void Bus::Start() {
      mShouldRun = true;

      Run();
    }

    void Bus::Run() {
      std::unique_lock<std::mutex> notifierLock(mNotifierMutex);

      while(mShouldRun) {
        mNotifier.wait_for(notifierLock, 100ms, [this] {
          std::lock_guard<std::mutex> lock(mQueueMutex);

          return mQueuedMessages.size() > 0;
        });

        std::lock_guard<std::mutex> lock(mQueueMutex);
        for(std::shared_ptr<messages::Message> message : mQueuedMessages) {
          Distribute(message);
        }

        mQueuedMessages.clear();
      }
    }

    void Bus::Stop() {
      mShouldRun = false;
    }

    void Bus::Distribute(std::shared_ptr<lp::messages::Message> message) {
      {
        std::lock_guard<std::mutex> lock(mSubscribersMutex);
        for(std::shared_ptr<Subscriber> subscriber : mSubscribers) {
          if(subscriber->GetTopic() == message->GetTopic()) {
            subscriber->Receive(message->GetDatagram());
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
  } // namespace bus
} // namespace lp
