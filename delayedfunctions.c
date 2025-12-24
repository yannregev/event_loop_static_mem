#include "delayedfunctions.h"
#include "includes.h"
#include "eventqueue.h"
#include "criticalsection.h"


typedef struct {
    Function_t func;
    uint16_t delay;
}DelayedFunc_t;

#define PERIODIC_FUNC_SIZE 64
static DelayedFunc_t delayedFunctions[PERIODIC_FUNC_SIZE];

void AddDelayedFunction(Function_t func, uint16_t delay) {
    if (func == NULL) return;

    if (delay == 0) { //Add to queue immediatly
        DelayedFunctionActivate(func);
        return;
    }
    BEGIN_CRITICAL_SECTION
    DelayedFunc_t *entry = delayedFunctions;
    while (entry->func != NULL && entry->func != func) {
        entry++;
        if (entry > &delayedFunctions[PERIODIC_FUNC_SIZE]) {
            fprintf(stderr, "Out of delayed func memory!\n");
            return;
        }
    }
    entry->delay = delay;
    entry->func = func;
    END_CRITICAL_SECTION
}

void RemoveDelayFunction(Function_t func) {
    DelayedFunc_t *entry = delayedFunctions;
    BEGIN_CRITICAL_SECTION
    while (entry != NULL && entry->func != func) {
        entry++;
    }

    if (entry->func != func) { return; }
        while (*entry->func != NULL) {    // Shrink array
        *entry = *(entry + 1);
        entry++;
    }
    END_CRITICAL_SECTION
}

void DelayedFunctions_IRQTick(void) {
    DelayedFunc_t *entry = delayedFunctions;
    BEGIN_CRITICAL_SECTION
    while (entry->func != NULL && entry < &delayedFunctions[PERIODIC_FUNC_SIZE]) {
        if (--entry->delay <= 0) {
            DelayedFunctionActivate(entry->func);
            RemoveDelayFunction(entry->func);
        } else {
            entry++;
        }
    }
    END_CRITICAL_SECTION
}

void InitDelayedFunction(void) {
    INITIALIZE_CRITICAL_SECTION
    memset(delayedFunctions, 0, PERIODIC_FUNC_SIZE);
}