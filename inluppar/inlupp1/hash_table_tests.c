#include <CUnit/Basic.h>
#include "hash_table.h"
#include "hash_table_iterator.h"
#include <stddef.h>

size_t string_hash(elem_t key) {
    const char *str = (char *)key.p;
    size_t result = 0;
    while (*str != '\0') {
        result = result * 31 + ((unsigned char) *str);
        str++;
    }
    return result;
}

bool string_eq(elem_t a, elem_t b) {
    return strcmp((char *)a.p, (char *)b.p) == 0;
}

size_t int_hash(elem_t key) {
    return (size_t)key.i;
}

bool int_eq(elem_t a, elem_t b) {
    return a.i == b.i;
}

int init_suite(void) {
  return 0;
}

int clean_suite(void) {
  return 0;
}

void test_create_destroy(void) {
   ioopm_hash_table_t *ht = ioopm_hash_table_create(int_hash, int_eq);
   CU_ASSERT_PTR_NOT_NULL(ht);
   ioopm_hash_table_destroy(ht);
}

void test_insert_once()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);

  elem_t key = ptr_elem("abc");
  elem_t value = int_elem(123);

  elem_t result = int_elem(0);
  CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, key, &result));
  CU_ASSERT_EQUAL(result.i, 0);

  ioopm_hash_table_insert(ht, key, value);
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key, &result));
  CU_ASSERT_EQUAL(result.i, value.i);

  ioopm_hash_table_destroy(ht);
}

void test_key_in_use() {
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);

  elem_t key = ptr_elem("abc");
  elem_t value = int_elem(123);

  elem_t result = int_elem(0);
  ioopm_hash_table_insert(ht, key, value);
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key, &result));
  CU_ASSERT_EQUAL(result.i, value.i);

  ioopm_hash_table_insert(ht, key, value);
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key, &result));
  CU_ASSERT_EQUAL(result.i, value.i);

  ioopm_hash_table_destroy(ht);
}

void test_insert_duplicate()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);
  elem_t key = ptr_elem("a");
  elem_t result = int_elem(0);

  ioopm_hash_table_insert(ht, key, int_elem(1));
  
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key, &result));
  CU_ASSERT_EQUAL(result.i, 1);

  ioopm_hash_table_insert(ht, key, int_elem(2));
  
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key, &result));
  CU_ASSERT_EQUAL(result.i, 2); 

  ioopm_hash_table_destroy(ht);
}

void test_insert_multiple_keys() 
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);
  elem_t key1 = ptr_elem("a");
  elem_t key2 = ptr_elem("b");
  elem_t result = int_elem(0);

  ioopm_hash_table_insert(ht, key1, int_elem(1));

  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key1, &result));
  CU_ASSERT_EQUAL(result.i, 1);

  ioopm_hash_table_insert(ht, key2, int_elem(2));
  
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key2, &result));
  CU_ASSERT_EQUAL(result.i, 2); 

  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key1, &result)); 
  CU_ASSERT_EQUAL(result.i, 1);

  ioopm_hash_table_destroy(ht);
}

void test_remove_key() 
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);
  elem_t key = ptr_elem("a");
  elem_t result = int_elem(0);

  ioopm_hash_table_insert(ht, key, int_elem(99));
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key, &result));
  CU_ASSERT_EQUAL(result.i, 99);

  ioopm_hash_table_remove(ht, key, &result);

  CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, key, &result));

  ioopm_hash_table_destroy(ht);
}

void test_remove_entry() 
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);
  elem_t key = ptr_elem("a");
  elem_t result = int_elem(0);

  ioopm_hash_table_insert(ht, key, int_elem(99));
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key, &result));
  CU_ASSERT_EQUAL(result.i, 99);

  elem_t removed_value = int_elem(0);
  CU_ASSERT_TRUE(ioopm_hash_table_remove(ht, key, &removed_value));

  CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, key, &result));

  ioopm_hash_table_destroy(ht);
}

void test_remove_entry_not_inserted() 
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);
  
  elem_t key = ptr_elem("spöket"); 
  elem_t result = int_elem(0);

  elem_t removed_value = int_elem(0);
  CU_ASSERT_FALSE(ioopm_hash_table_remove(ht, key, &removed_value));

  CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, key, &result));

  ioopm_hash_table_destroy(ht);
}

void test_remove_multiple_elements() 
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);
  elem_t result = int_elem(0);
  
  elem_t removed_value = int_elem(0); 

  ioopm_hash_table_insert(ht, ptr_elem("key1"), int_elem(10));
  ioopm_hash_table_insert(ht, ptr_elem("key2"), int_elem(20));
  ioopm_hash_table_insert(ht, ptr_elem("key3"), int_elem(30));

  CU_ASSERT_TRUE(ioopm_hash_table_remove(ht, ptr_elem("key2"), &removed_value));

  CU_ASSERT_EQUAL(removed_value.i, 20);

  CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, ptr_elem("key2"), &result));

  ioopm_hash_table_destroy(ht);
}

void test_has_key_empty()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);

  elem_t k = ptr_elem("a");

  CU_ASSERT_FALSE(ioopm_hash_table_has_key(ht, k));

  ioopm_hash_table_destroy(ht);
}

void test_has_key_one_entry()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);

  elem_t k = ptr_elem("a");
  elem_t k2 = ptr_elem("b");
  
  ioopm_hash_table_insert(ht, k, int_elem(1));

  CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, k));

  CU_ASSERT_FALSE(ioopm_hash_table_has_key(ht, k2));

  ioopm_hash_table_destroy(ht);
}

void test_has_key_three_entries()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);

  elem_t k1 = ptr_elem("a");
  elem_t k2 = ptr_elem("b");
  elem_t k3 = ptr_elem("c");
  elem_t k4 = ptr_elem("d");

  ioopm_hash_table_insert(ht, k1, int_elem(1));
  ioopm_hash_table_insert(ht, k2, int_elem(2));
  ioopm_hash_table_insert(ht, k3, int_elem(3));

  CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, k1));
  CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, k2));
  CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, k3));

  CU_ASSERT_FALSE(ioopm_hash_table_has_key(ht, k4));

  ioopm_hash_table_destroy(ht);
}

void test_has_key_insert_and_remove()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);

  elem_t removed_value = int_elem(0);
  elem_t k = ptr_elem("a");
  
  ioopm_hash_table_insert(ht, k, int_elem(1));
  ioopm_hash_table_remove(ht, k, &removed_value);

  CU_ASSERT_FALSE(ioopm_hash_table_has_key(ht, k));

  ioopm_hash_table_destroy(ht);
}

void test_has_key_insert3_and_remove1()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);

  elem_t removed_value = int_elem(0);
  elem_t k1 = ptr_elem("a");
  elem_t k2 = ptr_elem("b");
  elem_t k3 = ptr_elem("c");

  ioopm_hash_table_insert(ht, k1, int_elem(1));
  ioopm_hash_table_insert(ht, k2, int_elem(2));
  ioopm_hash_table_insert(ht, k3, int_elem(3));

  ioopm_hash_table_remove(ht, k2, &removed_value);

  CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, k1));
  CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, k3));

  CU_ASSERT_FALSE(ioopm_hash_table_has_key(ht, k2));

  ioopm_hash_table_destroy(ht);
}

void test_size_empty()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);

  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 0);

  ioopm_hash_table_destroy(ht);
}

void test_size_singelton()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);

  elem_t key = ptr_elem("a");
  elem_t value = int_elem(1);

  ioopm_hash_table_insert(ht, key, value);

  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 1);

  ioopm_hash_table_destroy(ht);
}

 void test_size_larger()
 {
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);

  elem_t k1 = ptr_elem("a");
  elem_t v1 = int_elem(1);
  elem_t k2 = ptr_elem("b");
  elem_t v2 = int_elem(2);
  elem_t k3 = ptr_elem("c");
  elem_t v3 = int_elem(3); 

  ioopm_hash_table_insert(ht, k1, v1);
  ioopm_hash_table_insert(ht, k2, v2);
  ioopm_hash_table_insert(ht, k3, v3);

  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 3);

  ioopm_hash_table_destroy(ht);
 }

 void test_size_after_removing()
 {
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);

  elem_t removed_value = int_elem(0);

  elem_t k1 = ptr_elem("a");
  elem_t v1 = int_elem(1);
  elem_t k2 = ptr_elem("b");
  elem_t v2 = int_elem(2);
  elem_t k3 = ptr_elem("c");
  elem_t v3 = int_elem(3); 

  ioopm_hash_table_insert(ht, k1, v1);
  ioopm_hash_table_insert(ht, k2, v2);
  ioopm_hash_table_insert(ht, k3, v3);

  ioopm_hash_table_remove(ht, k2, &removed_value);

  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 2);

  ioopm_hash_table_destroy(ht);
 }

 void test_is_empty() 
 {
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);

  CU_ASSERT_TRUE(ioopm_hash_table_is_empty(ht));

  ioopm_hash_table_destroy(ht);
 }

  void test_is_not_empty() 
 {
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);

  elem_t key = ptr_elem("a");
  elem_t value = int_elem(1);

  ioopm_hash_table_insert(ht, key, value);

  CU_ASSERT_FALSE(ioopm_hash_table_is_empty(ht));

  ioopm_hash_table_destroy(ht);
 }

void test_iter_empty()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);

  ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(ht);

  CU_ASSERT_TRUE(ioopm_hash_table_iterator_at_end(it));

  ioopm_hash_table_iterator_destroy(it);
  ioopm_hash_table_destroy(ht);
}

void test_iter_one_entry()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);
  ioopm_hash_table_insert(ht, ptr_elem("test_key"), int_elem(42));

  ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(ht);

  CU_ASSERT_FALSE(ioopm_hash_table_iterator_at_end(it)); 

  CU_ASSERT_STRING_EQUAL((char*)ioopm_hash_table_iterator_current_key(it).p, "test_key");
  CU_ASSERT_EQUAL(ioopm_hash_table_iterator_current_value(it).i, 42);

  ioopm_hash_table_iterator_advance(it);
  CU_ASSERT_TRUE(ioopm_hash_table_iterator_at_end(it));

  ioopm_hash_table_iterator_destroy(it);
  ioopm_hash_table_destroy(ht);
}

void test_iter_several_entries()
{
  elem_t keys[3] = {ptr_elem("abc"), ptr_elem("qwe"), ptr_elem("asd")};
  elem_t values[3] = {int_elem(0), int_elem(1), int_elem(2)};

  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);
  for (size_t i = 0; i != 3; i++) {
    ioopm_hash_table_insert(ht, keys[i], values[i]);
  }

  size_t iteration_count = 0;
  ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(ht);
  
  while (!ioopm_hash_table_iterator_at_end(it)) {
    iteration_count++;
    ioopm_hash_table_iterator_advance(it);
  }

  ioopm_hash_table_iterator_destroy(it);
  ioopm_hash_table_destroy(ht);
  
  CU_ASSERT_EQUAL(iteration_count, 3);
}

void test_iter_exactly_once()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);
  ioopm_hash_table_insert(ht, ptr_elem("k1"), int_elem(10));
  ioopm_hash_table_insert(ht, ptr_elem("k2"), int_elem(20));
  ioopm_hash_table_insert(ht, ptr_elem("k3"), int_elem(30));

  ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(ht);

  size_t count_k1 = 0, count_k2 = 0, count_k3 = 0;

  while (!ioopm_hash_table_iterator_at_end(it))
  {
    elem_t current_key = ioopm_hash_table_iterator_current_key(it);
    elem_t current_val = ioopm_hash_table_iterator_current_value(it);
    
    if (strcmp((char*)current_key.p, "k1") == 0) {
        count_k1++;
        CU_ASSERT_EQUAL(current_val.i, 10);
    }
    else if (strcmp((char*)current_key.p, "k2") == 0) {
        count_k2++;
        CU_ASSERT_EQUAL(current_val.i, 20);
    }
    else if (strcmp((char*)current_key.p, "k3") == 0) {
        count_k3++;
        CU_ASSERT_EQUAL(current_val.i, 30);
    }
    
    ioopm_hash_table_iterator_advance(it);
  }

  CU_ASSERT_EQUAL(count_k1, 1);
  CU_ASSERT_EQUAL(count_k2, 1);
  CU_ASSERT_EQUAL(count_k3, 1);

  ioopm_hash_table_iterator_destroy(it);
  ioopm_hash_table_destroy(ht);
}

void test_iter_same_bucket()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);
  
  elem_t keys[18] = {
      ptr_elem("k1"), ptr_elem("k2"), ptr_elem("k3"), ptr_elem("k4"), ptr_elem("k5"), ptr_elem("k6"), ptr_elem("k7"), ptr_elem("k8"), ptr_elem("k9"), 
      ptr_elem("k10"), ptr_elem("k11"), ptr_elem("k12"), ptr_elem("k13"), ptr_elem("k14"), ptr_elem("k15"), ptr_elem("k16"), ptr_elem("k17"), ptr_elem("k18")
  };

  for (size_t i = 0; i < 18; ++i) {
    ioopm_hash_table_insert(ht, keys[i], int_elem(i));
  }

  size_t iteration_count = 0;
  ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(ht);
  
  while (!ioopm_hash_table_iterator_at_end(it)) {
    iteration_count++;
    ioopm_hash_table_iterator_advance(it);
  }

  CU_ASSERT_EQUAL(iteration_count, 18);

  ioopm_hash_table_iterator_destroy(it);
  ioopm_hash_table_destroy(ht);
}

int main() {
  if (CU_initialize_registry() != CUE_SUCCESS)
    return CU_get_error();

  CU_pSuite my_test_suite = CU_add_suite("Hash Table Tests", init_suite, clean_suite);
  if (my_test_suite == NULL) {
      CU_cleanup_registry();
      return CU_get_error();
  }

    CU_pSuite iter_suite = CU_add_suite("Iterator Tests", init_suite, clean_suite);
  if (iter_suite == NULL) {
      CU_cleanup_registry();
      return CU_get_error();
  }

  if (
    (CU_add_test(my_test_suite, "Test Create & Destroy", test_create_destroy) == NULL) ||
    (CU_add_test(my_test_suite, "Test Insert Once", test_insert_once) == NULL) || 
    (CU_add_test(my_test_suite, "Test Key Already In Use", test_key_in_use) == NULL) || 
    (CU_add_test(my_test_suite, "Test Insert Multiple Keys", test_insert_multiple_keys) == NULL) ||
    (CU_add_test(my_test_suite, "Test Destroy Entry", test_remove_key) == NULL) || 
    (CU_add_test(my_test_suite, "Test Remove Entry", test_remove_entry) == NULL) ||
    (CU_add_test(my_test_suite, "Test Remove Entry Not Inserted", test_remove_entry_not_inserted) == NULL) ||
    (CU_add_test(my_test_suite, "Test Remove Multiple Elements", test_remove_multiple_elements) == NULL) ||
    (CU_add_test(my_test_suite, "Test Has Key Empty", test_has_key_empty) == NULL) ||
    (CU_add_test(my_test_suite, "Test Has Key One Entry", test_has_key_one_entry) == NULL) ||
    (CU_add_test(my_test_suite, "Test Has Key Three Enties", test_has_key_three_entries) == NULL) ||
    (CU_add_test(my_test_suite, "Test Has Key Insert & Remove", test_has_key_insert_and_remove) == NULL) ||
    (CU_add_test(my_test_suite, "Test Has Key Insert3 & Remove1", test_has_key_insert3_and_remove1) == NULL) ||
    (CU_add_test(my_test_suite, "Test Size Empty", test_size_empty) == NULL) ||
    (CU_add_test(my_test_suite, "Test Size Singleton", test_size_singelton) == NULL) ||
    (CU_add_test(my_test_suite, "Test Size Larger", test_size_larger) == NULL) ||
    (CU_add_test(my_test_suite, "Test Size After Removing", test_size_after_removing) == NULL) ||
    (CU_add_test(my_test_suite, "Test Is Empty", test_is_empty) == NULL) ||
    (CU_add_test(my_test_suite, "Test Is Not Empty", test_is_not_empty) == NULL)
  )
    {
      CU_cleanup_registry();
      return CU_get_error();
    }

  if (
    (CU_add_test(iter_suite, "Test Iterate Empty", test_iter_empty) == NULL) ||
    (CU_add_test(iter_suite, "Test Iterate One Entry", test_iter_one_entry) == NULL) ||
    (CU_add_test(iter_suite, "Test Iterate Several Entries", test_iter_several_entries) == NULL) ||
    (CU_add_test(iter_suite, "Test Iterate Exactly Once", test_iter_exactly_once) == NULL) ||
    (CU_add_test(iter_suite, "Test Iterate Same Bucket", test_iter_same_bucket) == NULL)
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