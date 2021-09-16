#include <string.h>
#include <stdbool.h>
#include <CUnit/Basic.h>
#include "hash_table.h"


static entry_t *find_previous_entry_for_key(entry_t *entry, int searchKey)
{
  /// Saves the first (dummy) entry as first_entry
  entry_t *first_entry = entry;
  entry_t *tmp_entry = entry;

  while(entry->next != NULL)
  {
    entry = entry->next;
    if (entry->key == searchKey)
    {
      return tmp_entry;
    }
    tmp_entry = entry;
  }
  
  return first_entry;
}


int init_suite(void)
{
  return 0;
}

int clean_suite(void)
{
  return 0;
}

void test_insert_not_in_use(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  ioopm_hash_table_insert(ht, 1, "test");
  entry_t *testEntry = &ht->buckets[1];
  int result = testEntry->next->key;
  ioopm_hash_table_destroy(ht);

  CU_ASSERT(result == 1);
}

void test2_insert(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  ioopm_hash_table_insert(ht, 1, "test");
  ioopm_hash_table_insert(ht, 1, "Hej");
  entry_t *testEntry = &ht->buckets[1];
  char *result = testEntry->next->value;
  ioopm_hash_table_destroy(ht);

  CU_ASSERT_EQUAL(strcmp(result, "Hej"), 0);
}


void test3_insert_negative(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  ioopm_hash_table_insert(ht, -20, "test");
  entry_t *testEntry = &ht->buckets[14];
  char *result = testEntry->next->value;
  ioopm_hash_table_destroy(ht);

  CU_ASSERT_EQUAL(strcmp(result, "test"), 0);
}

void test4_lookup(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  ioopm_hash_table_insert(ht, 1, "test1");
  ioopm_hash_table_insert(ht, 18, "test2");
  char *result = ioopm_lookup_key(ht, 1);
  ioopm_hash_table_destroy(ht);

  CU_ASSERT_EQUAL(strcmp(result, "test1"), 0);
}


void test5_lookup_existing(void) 
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  ioopm_hash_table_insert(ht, 1, "test1");
  char *result = NULL;
  bool valid = ioopm_hash_table_lookup(ht,1, &result);
  ioopm_hash_table_destroy(ht);

  CU_ASSERT_EQUAL(valid, true);
}

void test6_lookup_non_existing(void) 
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  ioopm_hash_table_insert(ht, 1, "test1");
  char *result = NULL;
  bool valid = ioopm_hash_table_lookup(ht,18, &result);
  ioopm_hash_table_destroy(ht);

  CU_ASSERT_EQUAL(valid, false);
}


void test7_remove_entry_right(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  ioopm_hash_table_insert(ht, 1, "test1");
  char *expected = ioopm_hash_table_remove(ht, 1);
  ioopm_hash_table_destroy(ht);
  
  CU_ASSERT_EQUAL(strcmp(expected, "test1"), 0);
}

void test8_remove_entry_right_null(void)
{
  char *result = NULL;
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  ioopm_hash_table_insert(ht, 1, "test1");
  ioopm_hash_table_remove(ht, 1);
  bool valid = ioopm_hash_table_lookup(ht,1, &result);
  ioopm_hash_table_destroy(ht);
  
  CU_ASSERT_EQUAL(valid, false);
}


void test9_remove_entry_ht(void)
{
  ioopm_hash_table_t *ht1 = ioopm_hash_table_create();
  ioopm_hash_table_insert(ht1, 1, "test1");
  ioopm_hash_table_remove(ht1, 1);
  entry_t *bucket = &ht1->buckets[1];
  entry_t *next = bucket->next;
  ioopm_hash_table_destroy(ht1);
  
  CU_ASSERT_PTR_NULL(next);
}


void test10_remove_entry_middle(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  ioopm_hash_table_insert(ht, 1, "test1");
  ioopm_hash_table_insert(ht, 18, "test2");
  ioopm_hash_table_insert(ht, 35, "test3");
  ioopm_hash_table_remove(ht, 18);

  entry_t *test_pointer = find_previous_entry_for_key(&ht->buckets[1], 1);
  int key = test_pointer->key;
  ioopm_hash_table_destroy(ht);

  CU_ASSERT_EQUAL(key, 35);
}

void test11_empty_hash_table(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  int size = ht->size;
  ioopm_hash_table_destroy(ht);

  CU_ASSERT_EQUAL(size,0);
}

void test12_one_entry_hash_table(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  ioopm_hash_table_insert(ht, 1, "test1");
  int size = ht->size;
  ioopm_hash_table_destroy(ht);

  CU_ASSERT_EQUAL(size,1);
}

void test13_multiple_entry_hash_table(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  ioopm_hash_table_insert(ht, 1, "test1");
  ioopm_hash_table_insert(ht, 2, "test1");
  ioopm_hash_table_insert(ht, 18, "test1");
  ioopm_hash_table_insert(ht, 19, "test1");
  ioopm_hash_table_remove(ht, 1);
  
  int size = ht->size;
  ioopm_hash_table_destroy(ht);

  CU_ASSERT_EQUAL(size,3);
}

void test14_hash_table_is_empty(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  bool result = ioopm_hash_table_is_empty(ht);
  ioopm_hash_table_destroy(ht);
  CU_ASSERT(result);
}

void test15_hash_table_is_empty_not_empty(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  ioopm_hash_table_insert(ht, 1, "test1");
  bool result = ioopm_hash_table_is_empty(ht);
  ioopm_hash_table_destroy(ht);

  CU_ASSERT(!result);
}

void test16_hash_table_clear_size(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  ioopm_hash_table_insert(ht, 1, "test1");
  ioopm_hash_table_insert(ht, 18, "Hejsan");
  ioopm_hash_table_insert(ht, 3, "Ioopm");
  ioopm_hash_table_clear(ht);
  int size = ht->size;
  ioopm_hash_table_destroy(ht);

  CU_ASSERT_EQUAL(size, 0);
}

void test17_hash_table_clear(void)
{
  char *result = NULL;
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  ioopm_hash_table_insert(ht, 1, "test1");
  ioopm_hash_table_insert(ht, 2, "test2");
  ioopm_hash_table_insert(ht, 3, "test3");
  bool test1 = ioopm_hash_table_lookup(ht, 1, &result);
  bool test2 = ioopm_hash_table_lookup(ht, 2, &result);
  bool test3 = ioopm_hash_table_lookup(ht, 3, &result);
  ioopm_hash_table_destroy(ht);

  CU_ASSERT(test1 && test2 && test3);
}

int main()
{
  CU_pSuite test_suite1 = NULL;

  if (CUE_SUCCESS != CU_initialize_registry())
    return CU_get_error();

  test_suite1 = CU_add_suite("Test Suite 1", init_suite, clean_suite);
  if (NULL == test_suite1)
    {
      CU_cleanup_registry();
      return CU_get_error();
    }

  if (
    (NULL == CU_add_test(test_suite1, "test 1", test_insert_not_in_use)) ||
    (NULL == CU_add_test(test_suite1, "test 2", test2_insert)) || 
    (NULL == CU_add_test(test_suite1, "test 3", test3_insert_negative)) || 
    (NULL == CU_add_test(test_suite1, "test 4", test4_lookup)) || 
    (NULL == CU_add_test(test_suite1, "test 5", test5_lookup_existing)) ||
    (NULL == CU_add_test(test_suite1, "test 6", test6_lookup_non_existing)) ||
    (NULL == CU_add_test(test_suite1, "test 7", test7_remove_entry_right)) || 
    (NULL == CU_add_test(test_suite1, "test 8", test8_remove_entry_right_null)) || 
    (NULL == CU_add_test(test_suite1, "test 9", test9_remove_entry_ht)) ||
    (NULL == CU_add_test(test_suite1, "test 10", test10_remove_entry_middle)) || 
    (NULL == CU_add_test(test_suite1, "test 11", test11_empty_hash_table)) || 
    (NULL == CU_add_test(test_suite1, "test 12", test12_one_entry_hash_table)) || 
    (NULL == CU_add_test(test_suite1, "test 13", test13_multiple_entry_hash_table)) || 
    (NULL == CU_add_test(test_suite1, "test 14", test14_hash_table_is_empty)) || 
    (NULL == CU_add_test(test_suite1, "test 15", test15_hash_table_is_empty_not_empty))|| 
    (NULL == CU_add_test(test_suite1, "test 16", test16_hash_table_clear_size))|| 
    (NULL == CU_add_test(test_suite1, "test 17", test17_hash_table_clear))
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

