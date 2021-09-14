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



// TODO: Testcase för ioopm_hash_table_insert
//Testing with a fresh key that is not already in use
//Testing with a key that is already in use
//Testing with an invalid key (if at all possible) EX negative int
void test_insert_not_in_use(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  ioopm_hash_table_insert(ht, 0, "test");
  entry_t *testEntry = &ht->buckets[0];
  int result = testEntry->next->key;

  CU_ASSERT_EQUAL(result, 1);
}

void test2_insert(void)
{
    CU_ASSERT(1 == 1);
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
    (NULL == CU_add_test(test_suite1, "test 2", test_insert_not_in_use)) ||
    (NULL == CU_add_test(test_suite1, "test 3", test2_insert))
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
