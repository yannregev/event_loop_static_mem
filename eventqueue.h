#ifndef EVENTQUEUE_H
#define EVENTQUEUE_H

#include "includes.h"

void EventActivate(uint16_t event, const uint16_t size, const void *data);
void EventAddCallback(uint16_t event, Function_t func);
void RemoveClosure(uint16_t event, Function_t func);
void EventQueue_Init(void);
void DelayedFunctionActivate(Function_t func);
void Run_Closures(void);

#endif