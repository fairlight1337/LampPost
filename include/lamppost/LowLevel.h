#ifndef LAMPPOST_LOWLEVEL_H
#define LAMPPOST_LOWLEVEL_H


#include <csignal>
#include <cstdio>
#include <string>


void HandleShutdownSignal();
void HookSystemSignals();


#endif //LAMPPOST_LOWLEVEL_H
