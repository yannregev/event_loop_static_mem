#include <stdio.h>
#include <inttypes.h>


#ifdef WIN32
#include <windows.h>
#else
#include <signal.h>
#include <time.h>
#include <unistd.h>
#endif
#include "eventqueue.h"
#include "delayedfunctions.h"
#include "periodicfunctions.h"
#include "events.h"

static void CallbackTest(uint16_t size, const void* data) {
    printf("Callback! data = ");
    const uint8_t* d = data;
    for (int i = 0; i < size; i++) {
        printf("%d, ", d[i]);
    }
    printf("\n");
}

static void CallbackTest2(uint16_t size, const void* data) {
    printf("Callback2! data = ");
    for (int i = 0; i < size; i++) {
        printf("%d, ", ((uint8_t*)data)[i]);
    }
    printf("\n");
}

static void DelayedFunctionTest(uint16_t, const void*) {
    printf("delayed callback!\n");
}

static void DelayedFunctionTest2(uint16_t, const void*) {
    printf("delayed callback!2\n");
}

static void DelayedFunctionTest3(uint16_t, const void*) {
    printf("delayed callback!3\n");
}

static void PeriodicFunction(uint16_t, const void*) {

    printf("Periodic func!\n");
}

#ifdef WIN32
VOID CALLBACK IRQ_Tick( 
    HWND hwnd,        // handle to window for timer messages 
    UINT message,     // WM_TIMER message 
    UINT_PTR idTimer,     // timer identifier 
    DWORD dwTime)     // current system time 
{
    PeriodicFunction_IRQTick();
    DelayedFunctions_IRQTick();
}

#else
void timer_callback(union sigval sv) {
    printf("Timer expired!\n");
}

void Setup_Timer(void) {
    struct sigevent sev;
    struct itimerspec its;
    timer_t timerid;
    int interval_ms = 1;  // Timer interval in milliseconds

    // Set up the signal event structure
    sev.sigev_notify = SIGEV_THREAD;        // Notify via a separate thread
    sev.sigev_notify_function = timer_callback;  // The callback function
    sev.sigev_notify_attributes = NULL;
    sev.sigev_value.sival_ptr = NULL;

    // Create the timer
    if (timer_create(CLOCK_REALTIME, &sev, &timerid) == -1) {
        perror("timer_create");
        exit(EXIT_FAILURE);
    }

    // Set up the timer to expire every 10ms
    its.it_value.tv_sec = 0;                          // Initial expiration in seconds
    its.it_value.tv_nsec = interval_ms * 1000000;     // Convert ms to ns
    its.it_interval.tv_sec = 0;                       // Periodic timer
    its.it_interval.tv_nsec = interval_ms * 1000000;  // Convert ms to ns

    // Start the timer
    if (timer_settime(timerid, 0, &its, NULL) == -1) {
        perror("timer_settime");
        exit(EXIT_FAILURE);
    }
}
#endif

int main(int argv, char **argc){
    
    InitEventQueue();
    InitDelayedFunction();
    InitPeriodicFunctions();
    Setup_Timer();

    EventAddCallback(EVENT_ONE, CallbackTest);
    EventAddCallback(EVENT_TWO, CallbackTest2);
    EventActivate(EVENT_ONE, 2, (uint8_t[]) {2, 4 });
    EventActivate(EVENT_TWO, 3, (uint8_t[]) {2, 4, 6 });
    //ActivateClosure(EVENT_TWO, 1, (uint8_t[]) {4});
    //ActivateClosure(EVENT_TWO, 1, (uint8_t[]) {3});

 /* 
    AddDelayedFunction(DelayedFunctionTest2, 20);
    AddDelayedFunction(DelayedFunctionTest, 10);
    AddDelayedFunction(DelayedFunctionTest3, 100);
*/

//    RemoveClosure(EVENT_ONE, CallbackTest);
    
//    AddPeriodicFunction(PeriodicFunction, 100);


    //RemovePeriodicFunction(PeriodicFunction);


#ifdef WIN32
    SetTimer(NULL, 0, 0x0000000A, (TIMERPROC)&IRQ_Tick);
#else
    Setup_Timer();
#endif
    while(1) {
        //EventActivate(EVENT_ONE, 2, (uint8_t[]) {2, 4 });
        Run_Closures();
    }
    printf("Done\n");
    return 0;
}
