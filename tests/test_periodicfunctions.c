#include "unity.h"
#include "periodicfunctions.h"

static int call_count_a = 0;
static int call_count_b = 0;

void TickFor(int ticks) {
    for (int i = 0; i < ticks; i++) {
        PeriodicFunction_IRQTick();
    }
}

void Callback_a(uint16_t size, const void *data) {
    call_count_a++;
}

void Callback_b(uint16_t size, const void *data) {
    call_count_b++;
}

void setUp(void) {
    PeriodicFunctions_Init();
    call_count_b = 0;
    call_count_a = 0;
}

void tearDown(void) {}


void Test_add_periodic_function_then_tick_invokes_it(void) {
    AddPeriodicFunction(Callback_a, 100);

    TickFor(100);
    TEST_ASSERT_EQUAL_INT(1, call_count_a);
}

void Test_add_periodic_function_invokes_multiple_times(void) {
    AddPeriodicFunction(Callback_a, 100);

    TickFor(100);
    TEST_ASSERT_EQUAL_INT(1, call_count_a);

    TickFor(100);
    TEST_ASSERT_EQUAL_INT(2, call_count_a);
}

void Test_remove_periodic_function_then_tick_does_not_invoke_it(void) {
    AddPeriodicFunction(Callback_a, 100);
    RemovePeriodicFunction(Callback_a);

    TickFor(100);
    TEST_ASSERT_EQUAL_INT(0, call_count_a);
}

void Test_add_function_a_then_b_invokes_them_in_order(void) {
    AddPeriodicFunction(Callback_a, 100);
    AddPeriodicFunction(Callback_b, 200);

    TickFor(100);
    TEST_ASSERT_EQUAL_INT(1, call_count_a);
    TEST_ASSERT_EQUAL_INT(0, call_count_b);

    TickFor(100);
    TEST_ASSERT_EQUAL_INT(2, call_count_a);
    TEST_ASSERT_EQUAL_INT(1, call_count_b);
}


int main(void) {
    UNITY_BEGIN();

    RUN_TEST(Test_add_periodic_function_then_tick_invokes_it);
    RUN_TEST(Test_add_periodic_function_invokes_multiple_times);
    RUN_TEST(Test_remove_periodic_function_then_tick_does_not_invoke_it);
    RUN_TEST(Test_add_function_a_then_b_invokes_them_in_order);

    return UNITY_END();
}