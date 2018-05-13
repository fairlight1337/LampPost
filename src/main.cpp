#include <iostream>
#include <cstdlib>

#include <lamppost/LowLevel.h>
#include <lamppost/LampPost.h>


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
	lamppost->Start();

	std::cout << "Exiting gracefully." << std::endl;

	return EXIT_SUCCESS;
}