#include <lamppost/log/Log.h>


namespace lp
{
  namespace log
  {
    std::mutex Log::sSynchronizationMutex;

    void Log::WriteSynchronized(std::string line)
    {
      std::lock_guard<std::mutex> lock(sSynchronizationMutex);

      std::cout << line << std::endl;
    }

    void Log::Write(std::string prefix, std::string message, int indentationLevel, int /*color*/)
    {
      std::string indentation;

      for(int i = 0; i < indentationLevel; ++i)
      {
        indentation += "  ";
      }

      WriteSynchronized("[" + prefix + "] " + indentation + message);
    }


    Log::Log(std::string prefix)
      : mPrefix(prefix)
    {
    }

    void Log::Info(std::string message, int indentationLevel)
    {
      Write(mPrefix, message, indentationLevel, LOG_COLOR_INFO);
    }

    void Log::Warning(std::string message, int indentationLevel)
    {
      Write(mPrefix, message, indentationLevel, LOG_COLOR_WARNING);
    }

    void Log::Error(std::string message, int indentationLevel)
    {
      Write(mPrefix, message, indentationLevel, LOG_COLOR_ERROR);
    }
  } // namespace log
} // namespace lp
