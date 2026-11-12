#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <CUnit/Basic.h>
#include "pqueue.h"

typedef struct item item_t;

struct item {
  int priority;
};

item_t item10 = {.priority = 10};
item_t item20a = {.priority = 20};
item_t item20b = {.priority = 20};
item_t item30 = {.priority = 30};

int get_priority(void *item) {
  return ((item_t*)item)->priority;
};

int init_suite(void)
{
  return 0;
}

int clean_suite(void)
{
  return 0;
}

void test_create_destroy(void)
{
  pqueue_t *pqueue = pqueue_create(get_priority);
  CU_ASSERT_PTR_NOT_NULL(pqueue);
  pqueue_destroy(pqueue);
}

// insert nothing and check empty
void test_empty(void)
{
  pqueue_t *pqueue = pqueue_create(get_priority);
  CU_ASSERT_PTR_NULL(pqueue_dequeue(pqueue));
  pqueue_destroy(pqueue);
}

// insert 10 and empty
void test_enqueue_one(void)
{
  pqueue_t *pqueue = pqueue_create(get_priority);
  pqueue_enqueue(pqueue, &item10);
  CU_ASSERT_PTR_EQUAL(pqueue_dequeue(pqueue), &item10);
  CU_ASSERT_PTR_NULL(pqueue_dequeue(pqueue));
  pqueue_destroy(pqueue);
}

// insert 10 and empty twice
void test_dequeue_past_empty(void)
{
  pqueue_t *pqueue = pqueue_create(get_priority);
  pqueue_enqueue(pqueue, &item10);
  CU_ASSERT_PTR_EQUAL(pqueue_dequeue(pqueue), &item10);
  CU_ASSERT_PTR_NULL(pqueue_dequeue(pqueue));
  CU_ASSERT_PTR_NULL(pqueue_dequeue(pqueue));
  pqueue_destroy(pqueue);
}

// insert 10 and destroy without emptying
void test_no_dequeue(void)
{
  pqueue_t *pqueue = pqueue_create(get_priority);
  pqueue_enqueue(pqueue, &item10);
  pqueue_destroy(pqueue);
}

// insert 10, 30, and empty
void test_enqueue_two_increasing(void)
{
  pqueue_t *pqueue = pqueue_create(get_priority);
  pqueue_enqueue(pqueue, &item10);
  pqueue_enqueue(pqueue, &item30);
  CU_ASSERT_PTR_EQUAL(pqueue_dequeue(pqueue), &item30);
  CU_ASSERT_PTR_EQUAL(pqueue_dequeue(pqueue), &item10);
  CU_ASSERT_PTR_NULL(pqueue_dequeue(pqueue));
  pqueue_destroy(pqueue);
}

// insert 30, 10, and empty
void test_enqueue_two_decreasing(void)
{
  pqueue_t *pqueue = pqueue_create(get_priority);
  pqueue_enqueue(pqueue, &item30);
  pqueue_enqueue(pqueue, &item10);
  CU_ASSERT_PTR_EQUAL(pqueue_dequeue(pqueue), &item30);
  CU_ASSERT_PTR_EQUAL(pqueue_dequeue(pqueue), &item10);
  CU_ASSERT_PTR_NULL(pqueue_dequeue(pqueue));
  pqueue_destroy(pqueue);
}

// insert 10, empty, insert 30 and empty
void test_enqueue_one_twice(void)
{
  pqueue_t *pqueue = pqueue_create(get_priority);
  pqueue_enqueue(pqueue, &item10);
  CU_ASSERT_PTR_EQUAL(pqueue_dequeue(pqueue), &item10);
  CU_ASSERT_PTR_NULL(pqueue_dequeue(pqueue));
  pqueue_enqueue(pqueue, &item30);
  CU_ASSERT_PTR_EQUAL(pqueue_dequeue(pqueue), &item30);
  CU_ASSERT_PTR_NULL(pqueue_dequeue(pqueue));
  pqueue_destroy(pqueue);
}

// insert 20a, 20b, and empty
void test_enqueue_two_equal(void)
{
  pqueue_t *pqueue = pqueue_create(get_priority);
  pqueue_enqueue(pqueue, &item20a);
  pqueue_enqueue(pqueue, &item20b);
  CU_ASSERT_PTR_EQUAL(pqueue_dequeue(pqueue), &item20a);
  CU_ASSERT_PTR_EQUAL(pqueue_dequeue(pqueue), &item20b);
  CU_ASSERT_PTR_NULL(pqueue_dequeue(pqueue));
  pqueue_destroy(pqueue);
}

// insert 30, 20a, 20b, and empty
void test_enqueue_two_equal_after_higher(void)
{
  pqueue_t *pqueue = pqueue_create(get_priority);
  pqueue_enqueue(pqueue, &item30);
  pqueue_enqueue(pqueue, &item20a);
  pqueue_enqueue(pqueue, &item20b);
  CU_ASSERT_PTR_EQUAL(pqueue_dequeue(pqueue), &item30);
  CU_ASSERT_PTR_EQUAL(pqueue_dequeue(pqueue), &item20a);
  CU_ASSERT_PTR_EQUAL(pqueue_dequeue(pqueue), &item20b);
  CU_ASSERT_PTR_NULL(pqueue_dequeue(pqueue));
  pqueue_destroy(pqueue);
}

// insert 20a, 10, 20b, and empty
void test_enqueue_two_equal_with_lower(void)
{
  pqueue_t *pqueue = pqueue_create(get_priority);
  pqueue_enqueue(pqueue, &item20a);
  pqueue_enqueue(pqueue, &item10);
  pqueue_enqueue(pqueue, &item20b);
  CU_ASSERT_PTR_EQUAL(pqueue_dequeue(pqueue), &item20a);
  CU_ASSERT_PTR_EQUAL(pqueue_dequeue(pqueue), &item20b);
  CU_ASSERT_PTR_EQUAL(pqueue_dequeue(pqueue), &item10);
  CU_ASSERT_PTR_NULL(pqueue_dequeue(pqueue));
  pqueue_destroy(pqueue);
}

// insert 20a, 30, dequeue one, 10, 20b, empty
void test_several(void)
{
  pqueue_t *pqueue = pqueue_create(get_priority);
  pqueue_enqueue(pqueue, &item20a);
  pqueue_enqueue(pqueue, &item30);
  CU_ASSERT_PTR_EQUAL(pqueue_dequeue(pqueue), &item30);
  pqueue_enqueue(pqueue, &item10);
  pqueue_enqueue(pqueue, &item20b);
  CU_ASSERT_PTR_EQUAL(pqueue_dequeue(pqueue), &item20a);
  CU_ASSERT_PTR_EQUAL(pqueue_dequeue(pqueue), &item20b);
  CU_ASSERT_PTR_EQUAL(pqueue_dequeue(pqueue), &item10);
  CU_ASSERT_PTR_NULL(pqueue_dequeue(pqueue));
  pqueue_destroy(pqueue);
}

int main()
{
  CU_pSuite test_suite1 = NULL;

  if (CUE_SUCCESS != CU_initialize_registry())
    return CU_get_error();

  test_suite1 = CU_add_suite("Priority Queue Test Suite", init_suite, clean_suite);
  if (NULL == test_suite1)
    {
      CU_cleanup_registry();
      return CU_get_error();
    }

  if (
    (NULL == CU_add_test(test_suite1, "create and destroy", test_create_destroy)) ||
    (NULL == CU_add_test(test_suite1, "dequeue empty", test_empty)) ||
    (NULL == CU_add_test(test_suite1, "enqueue and dequque one", test_enqueue_one)) ||
    (NULL == CU_add_test(test_suite1, "enqueue one and dequeque twice", test_dequeue_past_empty)) ||
    (NULL == CU_add_test(test_suite1, "enqueue one but don't dequeue", test_no_dequeue)) ||
    (NULL == CU_add_test(test_suite1, "enqueue two increasing", test_enqueue_two_increasing)) ||
    (NULL == CU_add_test(test_suite1, "enqueue two decreasing", test_enqueue_two_decreasing)) ||
    (NULL == CU_add_test(test_suite1, "enqueue one, dequeue and once again", test_enqueue_one_twice)) ||
    (NULL == CU_add_test(test_suite1, "enqueue two equal priorities", test_enqueue_two_equal)) ||
    (NULL == CU_add_test(test_suite1, "enqueue one, then two equal but lower priorities", test_enqueue_two_equal_after_higher)) ||
    (NULL == CU_add_test(test_suite1, "enqueue two decreasing, then one equal to the first one", test_enqueue_two_equal_with_lower)) ||
    (NULL == CU_add_test(test_suite1, "enqueue and dequeue several itemes", test_several))
  )
    {
      CU_cleanup_registry();
      return CU_get_error();
    }

  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  CU_cleanup_registry();
  return CU_get_error();
}
