#include <string.h>
#include <stdbool.h>
#include <CUnit/Basic.h>
#include "hash_table.h"



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

  CU_ASSERT(result == 1);
}

void test2_insert(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  ioopm_hash_table_insert(ht, 1, "test");
  ioopm_hash_table_insert(ht, 1, "Hej");
  entry_t *testEntry = &ht->buckets[1];
  char *result = testEntry->next->value;

  CU_ASSERT_EQUAL(strcmp(result, "Hej"), 0);
}

void test3_insert_negative(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  ioopm_hash_table_insert(ht, -20, "test");
  entry_t *testEntry = &ht->buckets[14];
  char *result = testEntry->next->value;

  CU_ASSERT_EQUAL(strcmp(result, "test"), 0);
}

void test4_lookup(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  ioopm_hash_table_insert(ht, -20, "test");
  entry_t *testEntry = &ht->buckets[14];
  char *result = testEntry->next->value;

  CU_ASSERT_EQUAL(strcmp(result, "test"), 0);
}

void test4_lookup(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  ioopm_hash_table_insert(ht, -20, "test");
  entry_t *testEntry = &ht->buckets[14];
  char *result = testEntry->next->value;

  CU_ASSERT_EQUAL(strcmp(result, "test"), 0);
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
    (NULL == CU_add_test(test_suite1, "test 3", test3_insert_negative))
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
