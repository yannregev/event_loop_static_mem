#include "delayedfunctions.h"
#include "includes.h"
#include "eventqueue.h"
#include "criticalsection.h"


typedef struct {
    Function_t func;
    uint16_t delay;
}DelayedFunc_t;

#define DELAYED_FUNC_Q_SIZE 64
static DelayedFunc_t delayedFunctions[DELAYED_FUNC_Q_SIZE];

void AddDelayedFunction(Function_t func, uint16_t delay) {
    if (func == NULL) return;

    if (delay == 0) { //Add to queue immediatly
        QueueFunctionCallback(func, 0, NULL);
        return;
    }
    BEGIN_CRITICAL_SECTION
    DelayedFunc_t *entry = delayedFunctions;
    while (entry->func != NULL && entry->func != func) {
        entry++;
        if (entry > &delayedFunctions[DELAYED_FUNC_Q_SIZE]) {
            fprintf(stderr, "Out of delayed func memory!\n");
            END_CRITICAL_SECTION
            return;
        }
    }
    entry->delay = delay;
    entry->func = func;
    END_CRITICAL_SECTION
}

static void shrinkDelayedFunctionsArray(Function_t func) 
{
    DelayedFunc_t *entry = delayedFunctions;
    while (entry != NULL && entry->func != func) {
        entry++;
    }

    if (entry->func != func) { return; }
        while (*entry->func != NULL) {    // Shrink array
        *entry = *(entry + 1);
        entry++;
    }
}

void RemoveDelayedFunction(Function_t func) {
    BEGIN_CRITICAL_SECTION
    shrinkDelayedFunctionsArray(func);
    END_CRITICAL_SECTION
}

void DelayedFunctions_IRQTick(void) {
    DelayedFunc_t *entry = delayedFunctions;
    BEGIN_CRITICAL_SECTION
    while (entry->func != NULL && entry < &delayedFunctions[DELAYED_FUNC_Q_SIZE]) {
        if (--entry->delay <= 0) {
            QueueFunctionCallback(entry->func, 0, NULL);
            shrinkDelayedFunctionsArray(entry->func);
        } else {
            entry++;
        }
    }
    END_CRITICAL_SECTION
}

void DelayedFunction_Init(void) {
    INITIALIZE_CRITICAL_SECTION
    memset(delayedFunctions, 0, sizeof(delayedFunctions));
}