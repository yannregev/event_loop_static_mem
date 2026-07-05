#include "includes.h"


void Events_Init(void) {
  EventQueue_Init();
  DelayedFunction_Init();
  PeriodicFunctions_Init();
}

void Events_Tick(void) {
  PeriodicFunction_IRQTick();
  DelayedFunctions_IRQTick();
}