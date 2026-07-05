#include "unity.h"
#include "eventqueue.h"
#include <string.h>

#define EVENT_A 1
#define EVENT_B 2

/* --- Spy state for callbacks --- */
static int  call_count_a = 0;
static int  call_count_b = 0;
static int  last_size    = -1;
static uint8_t last_data[32];

static void callback_a(uint16_t size,const void *data) {
    call_count_a++;
    last_size = size;
    if (size > 0 && data) memcpy(last_data, data, (size_t)size);
}

static void callback_b(uint16_t size,const void *data) {
    (void)size; (void)data;
    call_count_b++;
}

void setUp(void) {
    EventQueue_Init();
    call_count_a = 0;
    call_count_b = 0;
    last_size = -1;
    memset(last_data, 0, sizeof(last_data));
}

void tearDown(void) {}

/* --- Registration --- */

void test_add_callback_then_activate_invokes_it(void) {
    EventAddCallback(EVENT_A, callback_a);

    uint8_t payload[] = {0xDE, 0xAD};
    EventActivate(EVENT_A, sizeof(payload), payload);
    Run_EventQueue();

    TEST_ASSERT_EQUAL_INT(1, call_count_a);
    TEST_ASSERT_EQUAL_INT(sizeof(payload), last_size);
    TEST_ASSERT_EQUAL_HEX8(0xDE, last_data[0]);
    TEST_ASSERT_EQUAL_HEX8(0xAD, last_data[1]);
}

void test_add_callback_twice_is_idempotent(void) {
    EventAddCallback(EVENT_A, callback_a);
    EventAddCallback(EVENT_A, callback_a); // duplicate, should be ignored

    EventActivate(EVENT_A, 0, NULL);
    Run_EventQueue();

    TEST_ASSERT_EQUAL_INT(1, call_count_a); // not 2
}

void test_multiple_listeners_on_same_event_all_fire(void) {
    EventAddCallback(EVENT_A, callback_a);
    EventAddCallback(EVENT_A, callback_b);

    EventActivate(EVENT_A, 0, NULL);
    Run_EventQueue();

    TEST_ASSERT_EQUAL_INT(1, call_count_a);
    TEST_ASSERT_EQUAL_INT(1, call_count_b);
}

void test_listeners_are_per_event(void) {
    EventAddCallback(EVENT_A, callback_a);
    EventAddCallback(EVENT_B, callback_b);

    EventActivate(EVENT_A, 0, NULL);
    Run_EventQueue();

    TEST_ASSERT_EQUAL_INT(1, call_count_a);
    TEST_ASSERT_EQUAL_INT(0, call_count_b); // B's event wasn't activated
}

/* --- Invalid input handling --- */

void test_activate_event_none_is_ignored(void) {
    EventAddCallback(EVENT_A, callback_a);
    EventActivate(EVENT_NONE, 0, NULL);
    Run_EventQueue();

    TEST_ASSERT_EQUAL_INT(0, call_count_a);
}

void test_activate_out_of_range_event_is_ignored(void) {
    EventActivate(NUMBER_OF_EVENTS, 0, NULL); // one past valid range
    Run_EventQueue(); // should not crash
    TEST_ASSERT_EQUAL_INT(0, call_count_a);
    TEST_ASSERT_EQUAL_INT(0, call_count_b);
}

void test_add_callback_with_null_func_is_ignored(void) {
    EventAddCallback(EVENT_A, NULL);
    EventActivate(EVENT_A, 0, NULL);
    Run_EventQueue();
    TEST_ASSERT_EQUAL_INT(0, call_count_a); // nothing registered, nothing to call
}

/* --- Removal --- */

void test_remove_closure_stops_future_calls(void) {
    EventAddCallback(EVENT_A, callback_a);
    EventAddCallback(EVENT_A, callback_b);

    EventRemoveCallback(EVENT_A, callback_a);

    EventActivate(EVENT_A, 0, NULL);
    Run_EventQueue();

    TEST_ASSERT_EQUAL_INT(0, call_count_a); // removed
    TEST_ASSERT_EQUAL_INT(1, call_count_b); // still registered
}

void test_remove_nonexistent_closure_is_safe(void) {
    EventAddCallback(EVENT_A, callback_a);
    EventRemoveCallback(EVENT_A, callback_b); // never registered

    EventActivate(EVENT_A, 0, NULL);
    Run_EventQueue();

    TEST_ASSERT_EQUAL_INT(1, call_count_a); // untouched
}

void test_run_closures_on_empty_queue_does_nothing(void) {
    Run_EventQueue(); // should just return, no crash
    TEST_ASSERT_EQUAL_INT(0, call_count_a);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_add_callback_then_activate_invokes_it);
    RUN_TEST(test_add_callback_twice_is_idempotent);
    RUN_TEST(test_multiple_listeners_on_same_event_all_fire);
    RUN_TEST(test_listeners_are_per_event);

    RUN_TEST(test_activate_event_none_is_ignored);
    RUN_TEST(test_activate_out_of_range_event_is_ignored);
    RUN_TEST(test_add_callback_with_null_func_is_ignored);

    RUN_TEST(test_remove_closure_stops_future_calls);
    RUN_TEST(test_remove_nonexistent_closure_is_safe);

    RUN_TEST(test_run_closures_on_empty_queue_does_nothing);

    return UNITY_END();
}