#ifndef LAMPPOST_PLUGININSTANCE_H
#define LAMPPOST_PLUGININSTANCE_H


#include <atomic>
#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <utility>

#include <lamppost/bus/Bus.h>
#include <lamppost/Identifiable.h>
#include <lamppost/log/Log.h>
#include <lamppost/plugin/PluginConfiguration.h>


namespace lp
{
#if defined(_WIN32) || defined(_WIN64)
#define LIBEXPORT __declspec(dllexport)
#else
#define LIBEXPORT
#endif

#if defined(_WIN32) || defined(_WIN64)
#define CALLINGCONVENTION __stdcall
#else
#define CALLINGCONVENTION
#endif

#define GENERATE_INSTANCE_CREATOR(cls) \
  LIBEXPORT lp::PluginInstance* CALLINGCONVENTION CreateInstance(lp::PluginConfiguration configuration) \
  { \
    return new cls(configuration); \
  } \

#define GENERATE_INFO_CREATOR(variablename, body) \
  LIBEXPORT lp::PluginTemplateInfo* CALLINGCONVENTION CreateInfo() \
  { \
    lp::PluginTemplateInfo* variablename = new lp::PluginTemplateInfo(); \
    \
    body \
    \
    return variablename; \
  } \


  class PluginInstance : public Identifiable
  {
  private:
    PluginConfiguration mConfiguration;
    std::thread mWorkerThread;

  protected:
    std::atomic<bool> mShouldRun;
    log::Log mLog;

    template<class Rep, class Period>
    void Sleep(const std::chrono::duration<Rep, Period>& duration) {
      std::this_thread::sleep_for<Rep, Period>(duration);
    };

    messages::Datagram& GetCustomConfiguration();

  public:
    PluginInstance(PluginConfiguration configuration);
    virtual ~PluginInstance();

    template<class ... Args>
    std::shared_ptr<bus::Publisher> GetPublisher(Args ... args)
    {
      return mConfiguration.mBus->CreatePublisher(std::forward<Args>(args)...);
    }

    template<class ... Args>
    std::shared_ptr<bus::Subscriber> GetSubscriber(Args ... args)
    {
      return mConfiguration.mBus->CreateSubscriber(std::forward<Args>(args)...);
    }

    template<class ... Args>
    std::shared_ptr<bus::ActionProvider> GetActionProvider(Args ... args)
    {
      return mConfiguration.mBus->CreateActionProvider(std::forward<Args>(args)...);
    }

    template<class ... Args>
    std::shared_ptr<bus::ActionConsumer> GetActionConsumer(Args ... args)
    {
      return mConfiguration.mBus->CreateActionConsumer(std::forward<Args>(args)...);
    }

    void DeleteSubscriber(std::shared_ptr<bus::Subscriber> subscriber);
    void DeletePublisher(std::shared_ptr<bus::Publisher> publisher);

    virtual void Initialize() = 0;
    virtual void Run() = 0;
    virtual void Deinitialize() = 0;

    void Start();
    void Stop();
  };
} // namespace lp


#endif //LAMPPOST_PLUGININSTANCE_H
