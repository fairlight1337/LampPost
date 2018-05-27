#include <lamppost/PluginInstance.h>


namespace lp {
  PluginInstance::PluginInstance(PluginConfiguration configuration) : Identifiable(configuration.mIdentifier), mConfiguration(configuration) {
  }

  PluginInstance::~PluginInstance() {
    Stop();
  }

  std::shared_ptr<bus::Publisher> PluginInstance::GetPublisher(std::string topic) {
    return mConfiguration.mBus->CreatePublisher(topic);
  }

  std::shared_ptr<bus::Subscriber> PluginInstance::GetSubscriber(std::string topic, std::function<void(std::shared_ptr<messages::Datagram>)> callback) {
    return mConfiguration.mBus->CreateSubscriber(topic, callback);
  }

  void PluginInstance::Start() {
    mShouldRun = true;

    std::thread workerThread(&PluginInstance::Run, this);
    std::swap(mWorkerThread, workerThread);
  }

  void PluginInstance::Stop() {
    if(mShouldRun) {
      mShouldRun = false;

      if(mWorkerThread.joinable()) {
        mWorkerThread.join();
      }
    }
  }
} // namespace lp
