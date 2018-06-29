#include <lamppost/plugin/PluginInstance.h>


namespace lp
{
  PluginInstance::PluginInstance(PluginConfiguration configuration)
    : Identifiable(configuration.mIdentifier),
      mConfiguration(configuration),
      mShouldRun(false),
      mLog(configuration.mIdentifier)
  {
  }

  PluginInstance::~PluginInstance()
  {
    Stop();
    mConfiguration.mBus = nullptr;
  }

  messages::Datagram& PluginInstance::GetCustomConfiguration()
  {
    return mConfiguration.mCustomConfiguration;
  }

  void PluginInstance::DeleteSubscriber(std::shared_ptr<bus::Subscriber> subscriber)
  {
    mConfiguration.mBus->DeleteSubscriber(subscriber);
  }

  void PluginInstance::DeletePublisher(std::shared_ptr<bus::Publisher> publisher)
  {
    mConfiguration.mBus->DeletePublisher(publisher);
  }

  void PluginInstance::Start()
  {
    mShouldRun = true;

    std::thread workerThread(&PluginInstance::Run, this);
    std::swap(mWorkerThread, workerThread);
  }

  void PluginInstance::Stop()
  {
    if(mShouldRun)
    {
      mShouldRun = false;

      if(mWorkerThread.joinable())
      {
        mWorkerThread.join();
      }
    }
  }
} // namespace lp
