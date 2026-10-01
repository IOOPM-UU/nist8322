#include <CUnit/Basic.h>
#include "linked_list.h"
#include "list_iterator.h"

int init_suite(void) {
  return 0;
}

int clean_suite(void) {
  return 0;
}

void test_create_empty_list() {

    ioopm_list_t *list = ioopm_list_create();
    
    CU_ASSERT_TRUE(list != NULL);
    
    CU_ASSERT_TRUE(ioopm_list_size(list) == 0);

    ioopm_list_destroy(list);
}

void test_append_elements() {
   
    ioopm_list_t *list = ioopm_list_create();

    ioopm_list_append(list, int_elem(10));
    
    CU_ASSERT_TRUE(ioopm_list_size(list) == 1); 

    ioopm_list_append(list, int_elem(20));
    
     CU_ASSERT_TRUE(ioopm_list_size(list) == 2); 

    ioopm_list_destroy(list);
}

void test_prepend_elements() {
    
    ioopm_list_t *list = ioopm_list_create();

    ioopm_list_prepend(list, int_elem(10));
    CU_ASSERT_TRUE(ioopm_list_size(list) == 1); 

    ioopm_list_prepend(list, int_elem(20));
    CU_ASSERT_TRUE(ioopm_list_size(list) == 2); 

    ioopm_list_destroy(list);
}

void test_head_of_list() {

    ioopm_list_t *list = ioopm_list_create();

    ioopm_list_prepend(list, int_elem(10));
    ioopm_list_prepend(list, int_elem(20));

    CU_ASSERT_TRUE(ioopm_list_head(list).i == 20);

    ioopm_list_destroy(list);

}

void test_last_element() {
    ioopm_list_t *list = ioopm_list_create();

    ioopm_list_prepend(list, int_elem(10));
    ioopm_list_prepend(list, int_elem(20));
    ioopm_list_append(list, int_elem(30));

    CU_ASSERT_TRUE(ioopm_list_last(list).i == 30);

    ioopm_list_destroy(list);
}

void test_insert_element() {

    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, int_elem(10));
    ioopm_list_append(list, int_elem(20));
    ioopm_list_append(list, int_elem(30));
    ioopm_list_insert(list, 0, int_elem(40));

    CU_ASSERT_TRUE(ioopm_list_head(list).i == 40);

    ioopm_list_destroy(list);
}

void test_remove_element() {

    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, int_elem(10));
    ioopm_list_append(list, int_elem(20));
    ioopm_list_append(list, int_elem(30));

    ioopm_list_remove(list, 0);

    CU_ASSERT_FALSE(ioopm_list_head(list).i == 10);

    ioopm_list_destroy(list);
}

void test_get_element() {

    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, int_elem(10));
    ioopm_list_append(list, int_elem(20));
    ioopm_list_append(list, int_elem(30));

    CU_ASSERT_TRUE(ioopm_list_get(list, 1).i == 20);

    ioopm_list_destroy(list);
}

void test_size_of_list() {

    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, int_elem(10));
    ioopm_list_append(list, int_elem(20));

    CU_ASSERT_TRUE(ioopm_list_size(list) == 2);

    ioopm_list_destroy(list);
}

void test_list_empty() {

    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, int_elem(10));
    ioopm_list_append(list, int_elem(20));

    CU_ASSERT_FALSE(ioopm_list_is_empty(list));

    ioopm_list_destroy(list);
}

void test_create_destroy_iterator(void) {
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, int_elem(42));

    ioopm_list_iterator_t *iter = ioopm_list_iterator_create(list);
    CU_ASSERT_PTR_NOT_NULL(iter);

    ioopm_list_iterator_destroy(iter);
    ioopm_list_destroy(list);
}

void test_iterator_at_end(void) {
    
    ioopm_list_t *empty_list = ioopm_list_create();
    ioopm_list_iterator_t *iter_empty = ioopm_list_iterator_create(empty_list);
    
    CU_ASSERT_TRUE(ioopm_list_iterator_at_end(iter_empty));
    
    
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, int_elem(10));
    ioopm_list_iterator_t *iter = ioopm_list_iterator_create(list);
    
    CU_ASSERT_FALSE(ioopm_list_iterator_at_end(iter));

    ioopm_list_iterator_destroy(iter_empty);
    ioopm_list_destroy(empty_list);
    ioopm_list_iterator_destroy(iter);
    ioopm_list_destroy(list);
}

void test_iterator_advance_and_current(void) {
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, int_elem(10));
    ioopm_list_append(list, int_elem(20));
    ioopm_list_append(list, int_elem(30));

    ioopm_list_iterator_t *iter = ioopm_list_iterator_create(list);

    CU_ASSERT_EQUAL(ioopm_list_iterator_current(iter).i, 10);

    ioopm_list_iterator_advance(iter);
    CU_ASSERT_EQUAL(ioopm_list_iterator_current(iter).i, 20);

    ioopm_list_iterator_advance(iter);
    CU_ASSERT_EQUAL(ioopm_list_iterator_current(iter).i, 30);
    CU_ASSERT_FALSE(ioopm_list_iterator_at_end(iter));

    ioopm_list_iterator_advance(iter);
    CU_ASSERT_TRUE(ioopm_list_iterator_at_end(iter));

    ioopm_list_iterator_destroy(iter);
    ioopm_list_destroy(list);
}

int main() {
  if (CU_initialize_registry() != CUE_SUCCESS)
    return CU_get_error();

  CU_pSuite my_test_suite = CU_add_suite("Linked List Tests", init_suite, clean_suite);
  if (my_test_suite == NULL) {
      CU_cleanup_registry();
      return CU_get_error();
  }

  CU_pSuite iter_suite = CU_add_suite("List Iterator Tests", init_suite, clean_suite);
  if (iter_suite == NULL) {
      CU_cleanup_registry();
      return CU_get_error();
  }


  if (
    (CU_add_test(my_test_suite, "Create Empty List", test_create_empty_list) == NULL) ||
    (CU_add_test(my_test_suite, "Append Elements", test_append_elements) == NULL) ||
    (CU_add_test(my_test_suite, "Prepend Elements", test_prepend_elements) == NULL) ||
    (CU_add_test(my_test_suite, "Head of list", test_head_of_list) == NULL) ||
    (CU_add_test(my_test_suite, "Last element", test_last_element) == NULL) ||
    (CU_add_test(my_test_suite, "Insert element", test_insert_element) == NULL) ||
    (CU_add_test(my_test_suite, "Remove element", test_remove_element) == NULL) ||
    (CU_add_test(my_test_suite, "Get element", test_get_element) == NULL) ||
    (CU_add_test(my_test_suite, "Size of list", test_size_of_list) == NULL) ||
    (CU_add_test(my_test_suite, "List Empty", test_list_empty) == NULL)
  )
    {
      CU_cleanup_registry();
      return CU_get_error();
    }

    if (
    (CU_add_test(iter_suite, "Create & Destroy Iterator", test_create_destroy_iterator) == NULL) ||
    (CU_add_test(iter_suite, "Iterator at end", test_iterator_at_end) == NULL) ||
    (CU_add_test(iter_suite, "Iterator advance & current", test_iterator_advance_and_current) == NULL)
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