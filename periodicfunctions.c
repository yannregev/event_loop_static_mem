
#include "periodicfunctions.h"

typedef struct {
   Function_t func;
   uint16_t delay;
   uint16_t period; 
}PeriodicFunction_t;

static List_t periodicFunctions;

void PeriodicFunction_IRQTick(void) {
}

void RemovePeriodicFunction(Function_t func) {

}

void AddPeriodicFunction(Function_t func, uint16_t period) {
}

void InitPeriodicFunctions(void) {
    periodicFunctions.first = periodicFunctions.last = NULL;
}