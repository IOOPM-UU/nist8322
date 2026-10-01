#include <CUnit/Basic.h>
#include "hash_table.h"
#include "hash_table_iterator.h"
#include <stddef.h>

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

  ioopm_hash_table_remove(ht, key, &result);


  CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, key, &result));

  ioopm_hash_table_destroy(ht);
}

void test_remove_entry() 
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  char *key = "a";
  int result = 0;

  // 1. Arrange: Sätt in en nyckel och bevisa att den existerar
  ioopm_hash_table_insert(ht, key, 99);
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key, &result));
  CU_ASSERT_EQUAL(result, 99);

int removed_value = 0;
CU_ASSERT_TRUE(ioopm_hash_table_remove(ht, key, &removed_value));

  // 3. Assert: Kontrollera att nyckeln är borta.
  // (OBS: Detta kommer att MISSLYCKAS just nu, vilket är exakt vad vi vill i TDD!)
  CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, key, &result));

  // 4. Cleanup
  ioopm_hash_table_destroy(ht);
}

void test_remove_entry_not_inserted() 
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  
  // 1. Skapa variablerna kompilatorn behöver
  char *key = "spöket"; 
  int result = 0;

  // 2. Act: Försök ta bort en nyckel från en HELT tom tabell.
  // Det är HÄR ditt inbyggda skydd (current != NULL) kommer att rädda programmet 
  // från att krascha när den letar i en tom bucket.
  int removed_value = 0;
  CU_ASSERT_FALSE(ioopm_hash_table_remove(ht, key, &removed_value));

  // 3. Assert: Bevisa att datastrukturen överlevde och att nyckeln (fortfarande) inte finns
  CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, key, &result));

  // 4. Cleanup
  ioopm_hash_table_destroy(ht);
}

void test_remove_multiple_elements() 
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  int result = 0;
  
  int removed_value = 0; 

  ioopm_hash_table_insert(ht, "key1", 10);
  ioopm_hash_table_insert(ht, "key2", 20);
  ioopm_hash_table_insert(ht, "key3", 30);

  CU_ASSERT_TRUE(ioopm_hash_table_remove(ht, "key2", &removed_value));

  CU_ASSERT_EQUAL(removed_value, 20);

  CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, "key2", &result));

  // 5. Cleanup
  ioopm_hash_table_destroy(ht);
}

void test_has_key_empty()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();

  char *k = "a";

  CU_ASSERT_FALSE(ioopm_hash_table_has_key(ht, k));

  ioopm_hash_table_destroy(ht);
}

void test_has_key_one_entry()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();

  char *k = "a";
  char *k2 = "b";
  
  ioopm_hash_table_insert(ht, k, 1);

  CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, k));

  CU_ASSERT_FALSE(ioopm_hash_table_has_key(ht, k2));

  ioopm_hash_table_destroy(ht);
}

void test_has_key_three_entries()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();

  char *k1 = "a";
  char *k2 = "b";
  char *k3 = "c";
  char *k4 = "d";

  ioopm_hash_table_insert(ht, k1, 1);
  ioopm_hash_table_insert(ht, k2, 2);
  ioopm_hash_table_insert(ht, k3, 3);

  CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, k1));
  CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, k2));
  CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, k3));

  CU_ASSERT_FALSE(ioopm_hash_table_has_key(ht, k4));

  ioopm_hash_table_destroy(ht);
}

void test_has_key_insert_and_remove()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();

  int removed_value = 0;
  char *k = "a";
  

  ioopm_hash_table_insert(ht, k, 1);
  ioopm_hash_table_remove(ht, k, &removed_value);

  CU_ASSERT_FALSE(ioopm_hash_table_has_key(ht, k));

  ioopm_hash_table_destroy(ht);
}

void test_has_key_insert3_and_remove1()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();

  int removed_value = 0;
  char *k1 = "a";
  char *k2 = "b";
  char *k3 = "c";

  ioopm_hash_table_insert(ht, k1, 1);
  ioopm_hash_table_insert(ht, k2, 2);
  ioopm_hash_table_insert(ht, k3, 3);

  ioopm_hash_table_remove(ht, k2, &removed_value);

  CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, k1));
  CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, k3));

  CU_ASSERT_FALSE(ioopm_hash_table_has_key(ht, k2));

  ioopm_hash_table_destroy(ht);
}

void test_size_empty()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();

  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 0);

  ioopm_hash_table_destroy(ht);
}

void test_size_singelton()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();

  char *key = "a";
  int value = 1;

  ioopm_hash_table_insert(ht, key, value);

  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 1);

  ioopm_hash_table_destroy(ht);
}

 void test_size_larger()
 {
  ioopm_hash_table_t *ht = ioopm_hash_table_create();

  char *k1 = "a";
  int v1 = 1;
  char *k2 = "b";
  int v2 = 2;
  char *k3 = "c";
  int v3 = 3; 

  ioopm_hash_table_insert(ht, k1, v1);
  ioopm_hash_table_insert(ht, k2, v2);
  ioopm_hash_table_insert(ht, k3, v3);

  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 3);

  ioopm_hash_table_destroy(ht);
 }

 void test_size_after_removing()
 {
  ioopm_hash_table_t *ht = ioopm_hash_table_create();

  int removed_value = 0;

  char *k1 = "a";
  int v1 = 1;
  char *k2 = "b";
  int v2 = 2;
  char *k3 = "c";
  int v3 = 3; 

  ioopm_hash_table_insert(ht, k1, v1);
  ioopm_hash_table_insert(ht, k2, v2);
  ioopm_hash_table_insert(ht, k3, v3);

  ioopm_hash_table_remove(ht, k2, &removed_value);

  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 2);

  ioopm_hash_table_destroy(ht);
 }

 void test_is_empty() 
 {
  ioopm_hash_table_t *ht = ioopm_hash_table_create();

  CU_ASSERT_TRUE(ioopm_hash_table_is_empty(ht));

  ioopm_hash_table_destroy(ht);
 }

  void test_is_not_empty() 
 {
  ioopm_hash_table_t *ht = ioopm_hash_table_create();

  char *key = "a";
  int value = 1;

  ioopm_hash_table_insert(ht, key, value);

  CU_ASSERT_FALSE(ioopm_hash_table_is_empty(ht));

  ioopm_hash_table_destroy(ht);
 }

void test_iter_empty()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();

  ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(ht);

  CU_ASSERT_TRUE(ioopm_hash_table_iterator_at_end(it));

  ioopm_hash_table_iterator_destroy(it);
  ioopm_hash_table_destroy(ht);
}

void test_iter_one_entry()
{

  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  ioopm_hash_table_insert(ht, "test_key", 42);

  ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(ht);

  CU_ASSERT_FALSE(ioopm_hash_table_iterator_at_end(it)); 

  CU_ASSERT_STRING_EQUAL(ioopm_hash_table_iterator_current_key(it), "test_key");
  CU_ASSERT_EQUAL(ioopm_hash_table_iterator_current_value(it), 42);


  ioopm_hash_table_iterator_advance(it);
  CU_ASSERT_TRUE(ioopm_hash_table_iterator_at_end(it));

  ioopm_hash_table_iterator_destroy(it);
  ioopm_hash_table_destroy(ht);
}

void test_iter_several_entries()
{
  char *keys[3] = {"abc", "qwe", "asd"};
  int values[3] = {0, 1, 2};

  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  for (size_t i = 0; i != 3; i++) {
    ioopm_hash_table_insert(ht, keys[i], values[i]);
  }

  size_t iteration_count = 0;
  ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(ht);
  
  while (!ioopm_hash_table_iterator_at_end(it)) {
    iteration_count++;
    ioopm_hash_table_iterator_advance(it);
  }

  // FIX: Förstör iteratorn OCH tabellen korrekt!
  ioopm_hash_table_iterator_destroy(it);
  ioopm_hash_table_destroy(ht);
  
  CU_ASSERT_EQUAL(iteration_count, 3);
}

void test_iter_exactly_once()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  ioopm_hash_table_insert(ht, "k1", 10);
  ioopm_hash_table_insert(ht, "k2", 20);
  ioopm_hash_table_insert(ht, "k3", 30);

  ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(ht);

  // Använder ints (räknare) enligt instruktionen
  size_t count_k1 = 0, count_k2 = 0, count_k3 = 0;

  while (!ioopm_hash_table_iterator_at_end(it))
  {
    char *current_key = ioopm_hash_table_iterator_current_key(it);
    int current_val = ioopm_hash_table_iterator_current_value(it);
    
    // Check-off OCH värde-kontroll inuti loopen!
    if (strcmp(current_key, "k1") == 0) {
        count_k1++;
        CU_ASSERT_EQUAL(current_val, 10);
    }
    else if (strcmp(current_key, "k2") == 0) {
        count_k2++;
        CU_ASSERT_EQUAL(current_val, 20);
    }
    else if (strcmp(current_key, "k3") == 0) {
        count_k3++;
        CU_ASSERT_EQUAL(current_val, 30);
    }
    
    ioopm_hash_table_iterator_advance(it);
  }

  // Bevisa att varje nyckel besöktes EXAKT en gång
  CU_ASSERT_EQUAL(count_k1, 1);
  CU_ASSERT_EQUAL(count_k2, 1);
  CU_ASSERT_EQUAL(count_k3, 1);

  ioopm_hash_table_iterator_destroy(it);
  ioopm_hash_table_destroy(ht);
}

void test_iter_same_bucket()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  
  char *keys[18] = {
      "k1", "k2", "k3", "k4", "k5", "k6", "k7", "k8", "k9", 
      "k10", "k11", "k12", "k13", "k14", "k15", "k16", "k17", "k18"
  };

  for (int i = 0; i < 18; ++i) {
    ioopm_hash_table_insert(ht, keys[i], i);
  }

  size_t iteration_count = 0;
  ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(ht);
  
  while (!ioopm_hash_table_iterator_at_end(it)) {
    iteration_count++;
    ioopm_hash_table_iterator_advance(it);
  }

  // Om vi kunde stega 18 gånger utan att krascha, vet vi att iteratorn 
  // har gått genom minst en bucket som hade flera länkade noder
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