#include "includes.h"


void Events_Init(void) {
  InitDelayedFunction();
  InitEventQueue();
  InitPeriodicFunctions();
}

void Events_Tick(void) {
  PeriodicFunction_IRQTick();
  DelayedFunctions_IRQTick();
}