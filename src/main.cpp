#include <iostream>
#include <cstdlib>

#include <lamppost/LowLevel.h>
#include <lamppost/LampPost.h>


// Globals
lp::LampPost lamppost;


void HandleShutdownSignal() {
	lamppost.Stop();
}


// Main
int main() {
	std::cout << "Hooking up system calls." << std::endl;
	HookSystemSignals();

	std::cout << "Starting LampPost." << std::endl;
	lamppost.Start();

	std::cout << "Exiting gracefully." << std::endl;

	return EXIT_SUCCESS;
}