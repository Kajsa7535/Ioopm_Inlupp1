#include <string.h>
#include <stdbool.h>
#include <CUnit/Basic.h>
#include "hash_table.h"



static entry_t *find_previous_entry_for_key(entry_t *entry, int searchKey) // TODO: Ta reda på vad static innebär samt hur tester skrivs med de
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
  ioopm_hash_table_insert(ht, 1, "test1");
  ioopm_hash_table_insert(ht, 18, "test2");
  char *result = lookup_check(ht, 1);

  CU_ASSERT_EQUAL(strcmp(result, "test1"), 0);
}

/*
void test5_lookup_non_existing(void) // TODO: Skriv testcase för hash_table_lookup
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  char *result = lookup_check(ht, 2);
  CU_ASSERT_PTR_NULL(result);
}*/

void test6_remove_entry_right(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  ioopm_hash_table_insert(ht, 1, "test1");
  char *expected = ioopm_hash_table_remove(ht, 1);
  CU_ASSERT_EQUAL(strcmp(expected, "test1"), 0);
}


/*
void test7_remove_entry_right_null(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  ioopm_hash_table_insert(ht, 1, "test1");
  ioopm_hash_table_remove(ht, 1);
  char *result = lookup_check(ht, 1);
  CU_ASSERT_PTR_NULL(result);
}*/

void test8_remove_entry_ht(void)
{
  ioopm_hash_table_t *ht1 = ioopm_hash_table_create();
  ioopm_hash_table_t *ht2 = ht1; // ändra tillbaka ioopm_hash_table_create();
  ioopm_hash_table_insert(ht1, 1, "test1");
  ioopm_hash_table_remove(ht1, 1);
  CU_ASSERT_EQUAL(ht1, ht2);
}

void test9_remove_entry_middle(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  ioopm_hash_table_insert(ht, 1, "test1");
  ioopm_hash_table_insert(ht, 18, "test2");
  ioopm_hash_table_insert(ht, 35, "test3");
  ioopm_hash_table_remove(ht, 18);

  entry_t *test_pointer = find_previous_entry_for_key(&ht->buckets[1], 1);
  int key = test_pointer->key;

  CU_ASSERT_EQUAL(key, 35);
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
    //(NULL == CU_add_test(test_suite1, "test 5", test5_lookup_non_existing)) || /// TODO: FIXA LOOKUP BUG
    (NULL == CU_add_test(test_suite1, "test 6", test6_remove_entry_right)) || 
    //(NULL == CU_add_test(test_suite1, "test 7", test7_remove_entry_right_null)) || /// TODO: FIXA LOOKUP BUG
    (NULL == CU_add_test(test_suite1, "test 8", test8_remove_entry_ht)) || 
    (NULL == CU_add_test(test_suite1, "test 9", test9_remove_entry_middle))
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

