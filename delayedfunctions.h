#ifndef DELAYED_FUNCTIONS_H
#define DELAYED_FUNCTIONS_H

#include "includes.h"

void InitDelayedFunction(void);
void DelayedFunctions_IRQTick(void);
void AddDelayedFunction(Function_t func, uint16_t delay);
void RemoveDelayFunction(Function_t func);

#endif // DELAYED_FUNCTIONS_H