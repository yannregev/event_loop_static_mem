#include "includes.h"


void Events_Init(void) {
  DelayedFunction_Init();
  EventQueue_Init();
  PeriodicFunctions_Init();
}

void Events_Tick(void) {
  PeriodicFunction_IRQTick();
  DelayedFunctions_IRQTick();
}