#include <CUnit/Basic.h>
#include "hash_table.h"

int init_suite(void) {
  return 0;
}

int clean_suite(void) {
  return 0;
}

void test_create_destroy(void) {
   ioopm_hash_table_t *ht = ioopm_hash_table_create();
   CU_ASSERT_PTR_NOT_NULL(ht);
   ioopm_hash_table_destroy(ht);
}

void test_insert_once()
{
  // create new hash table
  ioopm_hash_table_t *ht = ioopm_hash_table_create();

  char *key = "abc";
  int value = 123;

  // check that key is not in ht
  int result = 0;
  CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, key, &result));
  CU_ASSERT_EQUAL(result, 0);

  // insert key-value pair and check that the mapping exists
  ioopm_hash_table_insert(ht, key, value);
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key, &result));
  CU_ASSERT_EQUAL(result, value);

  // destroy hash table
  ioopm_hash_table_destroy(ht);
}

void test_key_in_use() {
  ioopm_hash_table_t *ht = ioopm_hash_table_create();

    char *key = "abc";
  int value = 123;

  // check that key IS in ht
  int result = 0;
  ioopm_hash_table_insert(ht, key, value);
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key, &result));
  CU_ASSERT_EQUAL(result, value);

  // insert key-value pair and check that the mapping exists
  ioopm_hash_table_insert(ht, key, value);
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key, &result));
  CU_ASSERT_EQUAL(result, value);

  // destroy hash table
  ioopm_hash_table_destroy(ht);

}

void test_insert_duplicate()
{
  
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  char *key = "a";
  int result = 0;

  ioopm_hash_table_insert(ht, key, 1);
  
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key, &result));
  CU_ASSERT_EQUAL(result, 1);

  ioopm_hash_table_insert(ht, key, 2);

  
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key, &result));
  CU_ASSERT_EQUAL(result, 2); 

  ioopm_hash_table_destroy(ht);
}

void test_insert_multiple_keys() 
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  char *key1 = "a";
  char *key2 = "b";
  int result = 0;

  ioopm_hash_table_insert(ht, key1, 1);

  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key1, &result));
  CU_ASSERT_EQUAL(result, 1);

  ioopm_hash_table_insert(ht, key2, 2);

  
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key2, &result));
  CU_ASSERT_EQUAL(result, 2); 

  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key1, &result)); 
  CU_ASSERT_EQUAL(result, 1);

  ioopm_hash_table_destroy(ht);
}

void test_remove_key() 
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  char *key = "a";
  int result = 0;


  ioopm_hash_table_insert(ht, key, 99);
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key, &result));
  CU_ASSERT_EQUAL(result, 99);

  ioopm_hash_table_remove(ht, key);


  CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, key, &result));

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

  if (
    (CU_add_test(my_test_suite, "Test Create & Destroy", test_create_destroy) == NULL) ||
    (CU_add_test(my_test_suite, "Test Insert Once", test_insert_once) == NULL) || 
    (CU_add_test(my_test_suite, "Test Key Already In Use", test_key_in_use) == NULL) || 
    (CU_add_test(my_test_suite, "Test Insert Multiple Keys", test_insert_multiple_keys) == NULL) ||
    (CU_add_test(my_test_suite, "Test Destroy Entry", test_remove_key) == NULL)
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