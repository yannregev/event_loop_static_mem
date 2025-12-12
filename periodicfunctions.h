#ifndef PERIODICFUNCTIONS_H
#define PERIODICFUNCTIONS_H

#include "includes.h"

void InitPeriodicFunctions(void);
void PeriodicFunction_IRQTick(void);
void AddPeriodicFunction(Function_t func, uint16_t period);
void RemovePeriodicFunction(Function_t func);

#endif //PERIODICFUNCTIONS_H