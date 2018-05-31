#include <lamppost/LowLevel.h>


void signalHandler(int signal)
{
  switch(signal)
  {
    case SIGINT:
    case SIGTERM:
    case SIGABRT:
      HandleShutdownSignal();
      break;

    default:
      break;
  }
}


void HookSystemSignals()
{
  signal(SIGINT, signalHandler);
  signal(SIGTERM, signalHandler);
  signal(SIGABRT, signalHandler);
}
