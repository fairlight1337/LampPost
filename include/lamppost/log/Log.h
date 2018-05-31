#ifndef LAMPPOST_LOG_H
#define LAMPPOST_LOG_H


#include <mutex>
#include <string>
#include <iostream>


#define LOG_COLOR_INFO 0
#define LOG_COLOR_WARNING 0
#define LOG_COLOR_ERROR 0


namespace lp
{
  namespace log
  {
    class Log
    {
    private:
      static std::mutex sSynchronizationMutex;

      std::string mPrefix;

      static void WriteSynchronized(std::string line);
      static void Write(std::string prefix, std::string message, int indentationLevel, int color);

    public:
      Log(std::string prefix);
      virtual ~Log() = default;

      void Info(std::string message, int indentationLevel = 0);
      void Warning(std::string message, int indentationLevel = 0);
      void Error(std::string message, int indentationLevel = 0);
    };
  } // namespace bus
} // namespace lp


#endif //LAMPPOST_LOG_H
