#ifndef LAMPPOST_PLUGININSTANCE_H
#define LAMPPOST_PLUGININSTANCE_H


#include <string>
#include <atomic>
#include <thread>
#include <chrono>

#include <lamppost/Identifiable.h>
#include <lamppost/PluginConfiguration.h>
#include <lamppost/bus/Bus.h>


namespace lp {
	class PluginInstance : public Identifiable {
	private:
		PluginConfiguration mConfiguration;

	protected:
		std::atomic<bool> mShouldRun;

		template<class Rep, class Period>
		void Sleep(const std::chrono::duration<Rep, Period>& duration) {
			std::this_thread::sleep_for<Rep, Period>(duration);
		};

	public:
		PluginInstance(PluginConfiguration configuration);
		~PluginInstance();

		std::shared_ptr<bus::Publisher> GetPublisher(std::string topic);

		virtual void Initialize() = 0;
		virtual void Run() = 0;
		virtual void Deinitialize() = 0;

		void Start();
		void Stop();
	};
}


#endif //LAMPPOST_PLUGININSTANCE_H
