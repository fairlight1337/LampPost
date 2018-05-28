#include <lamppost/LowLevel.h>


void signalHandler(int signal) {
  switch(signal) {
    case SIGINT:
    case SIGTERM:
      HandleShutdownSignal();
      break;

    default:
      break;
  }
}


void HookSystemSignals() {
  struct sigaction action = {};
  memset(&action, 0, sizeof(struct sigaction));
  action.sa_handler = signalHandler;
  action.sa_flags = SA_RESTART;
  sigfillset(&action.sa_mask);
  sigaction(SIGTERM, &action, nullptr);
  sigaction(SIGINT, &action, nullptr);
}