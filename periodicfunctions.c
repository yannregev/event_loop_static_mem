#include "periodicfunctions.h"
#include "eventqueue.h"
#include "stdio.h"

#ifdef WIN32
#include <windows.h>

CRITICAL_SECTION cs;

#define BEGIN_CRITICAL_SECTION EnterCriticalSection(&cs);
#define END_CRITICAL_SECTION LeaveCriticalSection(&cs);
#define INITIALIZE_CRITICAL_SECTION InitializeCriticalSection(&cs);

#else
#include <pthread.h>

static pthread_mutex_t cs = PTHREAD_MUTEX_INITIALIZER;

#define BEGIN_CRITICAL_SECTION pthread_mutex_lock(&cs);
#define END_CRITICAL_SECTION   pthread_mutex_unlock(&cs);
#define INITIALIZE_CRITICAL_SECTION
#endif

typedef struct {
   Function_t func;
   uint16_t delay;
   uint16_t period; 
}PeriodicFunction_t;

#define PERIODIC_FUNC_SIZE 64
PeriodicFunction_t periodicFunctions[PERIODIC_FUNC_SIZE];


void PeriodicFunction_IRQTick(void) {
    BEGIN_CRITICAL_SECTION
    PeriodicFunction_t *entry = periodicFunctions;
    while (entry->func != NULL && entry < &periodicFunctions[PERIODIC_FUNC_SIZE]) {
        if (--entry->delay <= 0) {
            DelayedFunctionActivate(entry->func);
            entry->delay = entry->period;
        }
        entry++;
    }
    END_CRITICAL_SECTION
}

void RemovePeriodicFunction(Function_t func) {
    PeriodicFunction_t *entry = periodicFunctions;
    while (entry != NULL && entry->func != func) {
        entry++;
    }
    
    if (entry->func != func) { return; }
    BEGIN_CRITICAL_SECTION
        while (*entry->func != NULL) {    // Shrink array
        *entry = *(entry + 1);
        entry++;
    }
    END_CRITICAL_SECTION
}

void AddPeriodicFunction(Function_t func, uint16_t period) {
    if (func == NULL) return;

    if (period == 0) { //Add to queue immediatly
        return;
    }

    PeriodicFunction_t *entry = periodicFunctions;
    while (entry->func != NULL && entry->func != func) {
        entry++;
        if (entry >= &periodicFunctions[PERIODIC_FUNC_SIZE]) {
            fprintf(stderr, "Out of delayed func memory!\n");
            return;
        }
    }
    entry->delay = period;
    entry->period = period;
    entry->func = func;
}

void InitPeriodicFunctions(void) {
    INITIALIZE_CRITICAL_SECTION
    memset(periodicFunctions, 0, sizeof(periodicFunctions));
}