#include "unity.h"
#include "queue.h"
#include <string.h>

#define QUEUE_CAPACITY (QUEUE_SIZE - 1)

static Queue_t q;

void setUp(void) {
    QueueInit(&q);
}

void tearDown(void) {}

/* --- Basic init --- */

void test_init_starts_empty(void) {
    TEST_ASSERT_TRUE(IsQueueEmpty(&q));
    TEST_ASSERT_EQUAL_UINT16(0, QueueSize(&q));
}

/* --- Enqueue / Dequeue round trip --- */

void test_enqueue_then_dequeue_single_byte(void) {
    uint8_t in = 0x42;
    uint8_t out = 0;

    TEST_ASSERT_EQUAL_UINT8(0, Enqueue(&q, &in, 1));
    TEST_ASSERT_FALSE(IsQueueEmpty(&q));

    TEST_ASSERT_EQUAL_UINT8(0, Dequeue(&q, &out, 1));
    TEST_ASSERT_EQUAL_UINT8(0x42, out);
    TEST_ASSERT_TRUE(IsQueueEmpty(&q));
}

void test_enqueue_dequeue_preserves_order(void) {
    uint8_t in[]  = {1, 2, 3, 4};
    uint8_t out[4] = {0};

    TEST_ASSERT_EQUAL_UINT8(0, Enqueue(&q, in, sizeof(in)));
    TEST_ASSERT_EQUAL_UINT8(0, Dequeue(&q, out, sizeof(out)));

    TEST_ASSERT_EQUAL_UINT8_ARRAY(in, out, sizeof(in));
}

void test_enqueue_zero_size_is_noop(void) {
    uint8_t in = 0xFF;
    TEST_ASSERT_EQUAL_UINT8(0, Enqueue(&q, &in, 0));
    TEST_ASSERT_TRUE(IsQueueEmpty(&q)); // nothing actually added
}

/* --- Size / free space accounting --- */

void test_size_and_free_space_after_partial_fill(void) {
    uint8_t in[3] = {1, 2, 3};
    Enqueue(&q, in, sizeof(in));

    TEST_ASSERT_EQUAL_UINT16(3, QueueSize(&q));
    TEST_ASSERT_EQUAL_UINT16(QUEUE_CAPACITY - 3, QueueFreeSpace(&q));
}

/* --- Wraparound --- */

void test_wraparound_after_several_enqueue_dequeue_cycles(void) {
    uint8_t tmp;
    // Push and pop repeatedly to walk head/tail all the way around the buffer
    for (int cycle = 0; cycle < QUEUE_SIZE * 2; cycle++) {
        uint8_t val = (uint8_t)cycle;
        TEST_ASSERT_EQUAL_UINT8(0, Enqueue(&q, &val, 1));
        TEST_ASSERT_EQUAL_UINT8(0, Dequeue(&q, &tmp, 1));
        TEST_ASSERT_EQUAL_UINT8(val, tmp);
    }
    TEST_ASSERT_TRUE(IsQueueEmpty(&q));
}

/* --- Underflow / overflow guards --- */

void test_dequeue_from_empty_queue_fails(void) {
    uint8_t out;
    TEST_ASSERT_NOT_EQUAL(0, Dequeue(&q, &out, 1));
}

void test_enqueue_up_to_capacity_succeeds(void) {
    uint8_t in[QUEUE_SIZE - 1]; // leave one slot free to avoid overflow
    memset(in, 0xAA, sizeof(in));
    TEST_ASSERT_EQUAL_UINT8(0, Enqueue(&q, in, sizeof(in)));
}

void test_enqueue_beyond_capacity_fails(void) {
    uint8_t in[QUEUE_SIZE];
    memset(in, 0xAA, sizeof(in));
    Enqueue(&q, in, sizeof(in)); // fill it completely

    uint8_t extra = 0x01;
    TEST_ASSERT_NOT_EQUAL(0, Enqueue(&q, &extra, 1)); // should reject, queue is full
}

void test_queue_size_when_full_equals_capacity(void) {
    uint8_t in[QUEUE_SIZE];
    memset(in, 0x5A, sizeof(in));
    Enqueue(&q, in, QUEUE_SIZE);

    TEST_ASSERT_EQUAL_UINT16(QUEUE_CAPACITY, QueueSize(&q));
    TEST_ASSERT_EQUAL_UINT16(0, QueueFreeSpace(&q));
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_init_starts_empty);
    RUN_TEST(test_enqueue_then_dequeue_single_byte);
    RUN_TEST(test_enqueue_dequeue_preserves_order);
    RUN_TEST(test_enqueue_zero_size_is_noop);

    RUN_TEST(test_size_and_free_space_after_partial_fill);

    RUN_TEST(test_wraparound_after_several_enqueue_dequeue_cycles);

    RUN_TEST(test_dequeue_from_empty_queue_fails);
    RUN_TEST(test_enqueue_up_to_capacity_succeeds);
    RUN_TEST(test_enqueue_beyond_capacity_fails);

    RUN_TEST(test_queue_size_when_full_equals_capacity); // expected to FAIL currently

    return UNITY_END();
}