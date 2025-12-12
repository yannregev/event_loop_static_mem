#include <stdio.h>
#include <inttypes.h>
#include <windows.h>

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

VOID CALLBACK IRQ_Tick( 
    HWND hwnd,        // handle to window for timer messages 
    UINT message,     // WM_TIMER message 
    UINT_PTR idTimer,     // timer identifier 
    DWORD dwTime)     // current system time 
{
    PeriodicFunction_IRQTick();
    DelayedFunctions_IRQTick();
}

int main(int argv, char **argc){
    
    InitEventQueue();
    InitDelayedFunction();
    InitPeriodicFunctions();

    AddClosure(EVENT_ONE, CallbackTest);
    AddClosure(EVENT_TWO, CallbackTest2);
    ActivateClosure(EVENT_ONE, 2, (uint8_t[]) {2, 4 });
    ActivateClosure(EVENT_TWO, 3, (uint8_t[]) {2, 4, 6 });
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



    SetTimer(NULL, 0, 0x0000000A, (TIMERPROC)&IRQ_Tick);
    MSG msg;

    while(GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        ActivateClosure(EVENT_ONE, 2, (uint8_t[]) {2, 4 });
        Run_Closures();
    }
    printf("Done\n");
    return 0;
}
