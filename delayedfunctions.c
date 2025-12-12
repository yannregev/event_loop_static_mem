#include "delayedfunctions.h"

typedef struct {
    Function_t func;
    uint16_t delay;
}DelayedFunc_t;

static List_t delayedFunctions;

void AddDelayedFunction(Function_t func, uint16_t delay) {
    if (func == NULL) return;
}

void RemoveDelayFunction(Function_t func) {

}

void DelayedFunctions_IRQTick(void) {

}

void InitDelayedFunction(void) {
    delayedFunctions.first = delayedFunctions.last = NULL;
}