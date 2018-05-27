// System includes
#include <cstdlib>
#include <iostream>

// Private includes
#include <lamppost/LampPost.h>
#include <lamppost/LowLevel.h>


// Globals
std::shared_ptr<lp::LampPost> lamppost = nullptr;


void HandleShutdownSignal() {
  lamppost->Stop();
}


// Main
int main() {
  std::cout << "Hooking up system calls." << std::endl;
  HookSystemSignals();

  std::cout << "Starting LampPost." << std::endl;
  lp::LampPostConfiguration configuration;

  lamppost = std::make_shared<lp::LampPost>(configuration);

  try {
    lamppost->Start();
  } catch(std::runtime_error exception) {
    std::cout << exception.what() << std::flush << std::endl;

    throw;
  }

  std::cout << "Exiting gracefully." << std::endl;

  return EXIT_SUCCESS;
}