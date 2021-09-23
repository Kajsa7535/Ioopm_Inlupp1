#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <CUnit/Basic.h>
#include "hash_table.h"


static void update_value(int key, char **value, void *arg)
{
  char **other_char_ptr = arg;
  char *other_value = *other_char_ptr;
  *value = other_value;
}

static bool value_equiv(int key_ignored, char *value, void *x) //TODO: Fråga om hur man slipper lägga in dessa i .h filen och kunna ha kvar den i .c filen
{
  char **other_char_ptr = x;
  char *other_value = *other_char_ptr;
  return strcmp(value,other_value) == 0;
}

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

//TODO!!! Gör testfall för när key = 0
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

void test18A_hash_table_keys(void) // TODO: Make test case for empty (no keys)
{
  int expected[4] = {1,2,19,3};
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  ioopm_hash_table_insert(ht, 1, "test1");
  ioopm_hash_table_insert(ht, 19, "ioopm");
  ioopm_hash_table_insert(ht, 2, "test2");
  ioopm_hash_table_insert(ht, 3, "test3");
  int *result = ioopm_hash_table_keys(ht);
  bool cmp1 = expected[0] == result[0];
  bool cmp2 = expected[1] == result[1];
  bool cmp3 = expected[2] == result[2];
  bool cmp4 = expected[3] == result[3];
  ioopm_hash_table_destroy(ht);
  free(result);

  CU_ASSERT(cmp1 && cmp2 && cmp3 && cmp4);
}

void test18B_hash_table_keys_empty(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  int *result = ioopm_hash_table_keys(ht);
  bool cmp = (sizeof(result) == sizeof(NULL)); //sizeof(NULL) == 8
  ioopm_hash_table_destroy(ht);
  free(result);
  CU_ASSERT(cmp);
}


void test19_hash_table_values(void)
{
  char *expected[4] = {"test1", "test2", "ioopm", "test3"};
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  ioopm_hash_table_insert(ht, 1, "test1");
  ioopm_hash_table_insert(ht, 19, "ioopm");
  ioopm_hash_table_insert(ht, 2, "test2");
  ioopm_hash_table_insert(ht, 3, "test3");
  char **result = ioopm_hash_table_values(ht);
  int cmp1 = strcmp(expected[0],result[0]); 
  int cmp2 = strcmp(expected[1],result[1]); 
  int cmp3 = strcmp(expected[2],result[2]); 
  int cmp4 = strcmp(expected[3],result[3]); 
  ioopm_hash_table_destroy(ht);
  free(result);

  CU_ASSERT(cmp1 == 0 && cmp2 == 0 && cmp3 == 0 && cmp4 == 0);
}

void test20_hash_table_values_empty(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  char **result = ioopm_hash_table_values(ht);
  char *empty = result[0];
  ioopm_hash_table_destroy(ht);
  free(result);
  CU_ASSERT_PTR_NULL(empty);
}

void test21_hash_table_has_key(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  ioopm_hash_table_insert(ht, 3, "test1");
  ioopm_hash_table_insert(ht, 21, "ioopm");
  ioopm_hash_table_insert(ht, 2, "test2");
  bool result = ioopm_hash_table_has_key(ht, 3);
  ioopm_hash_table_destroy(ht);
  CU_ASSERT(result);
}

void test22_hash_table_has_key_not(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  ioopm_hash_table_insert(ht, 1, "test1");
  ioopm_hash_table_insert(ht, 19, "ioopm");
  ioopm_hash_table_insert(ht, 2, "test2");
  bool result = ioopm_hash_table_has_key(ht, 18);
  ioopm_hash_table_destroy(ht);

  CU_ASSERT_FALSE(result);
}


void test23_hash_table_has_value_identical(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  char *str = "ioopm";
  ioopm_hash_table_insert(ht, 1, str);
  ioopm_hash_table_insert(ht, 19, "test1");
  ioopm_hash_table_insert(ht, 2, "woow");
  bool result = ioopm_hash_table_has_value(ht, str);
  ioopm_hash_table_destroy(ht);

  CU_ASSERT(result);
}


void test24_hash_table_has_value_equivalent(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  ioopm_hash_table_insert(ht, 1, "test1");
  ioopm_hash_table_insert(ht, 19, "ioopm");
  entry_t *entry = &ht->buckets[2];
  char *copied_str = strdup(entry->next->value);
  bool result = ioopm_hash_table_has_value(ht, copied_str);
  free(copied_str);
  ioopm_hash_table_destroy(ht);

  CU_ASSERT(result);
}

void test25_hash_table_has_value_not(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  ioopm_hash_table_insert(ht, 1, "test1");
  ioopm_hash_table_insert(ht, 2, "test2");
  bool result = ioopm_hash_table_has_value(ht, "test3");
  ioopm_hash_table_destroy(ht);

  CU_ASSERT(!result);
}

void test26_hash_table_all(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  ioopm_hash_table_insert(ht, 1, "test1");
  ioopm_hash_table_insert(ht, 2, "test1");
  ioopm_hash_table_insert(ht, 18, "test1");
  ioopm_hash_table_insert(ht, 19, "test1");
  char *value = "test1";
  bool result = ioopm_hash_table_all(ht, value_equiv, &value);
  ioopm_hash_table_destroy(ht);
  CU_ASSERT(result);
}

void test27_hash_table_all_not(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  ioopm_hash_table_insert(ht, 1, "test1");
  ioopm_hash_table_insert(ht, 2, "test1");
  ioopm_hash_table_insert(ht, 18, "test1");
  ioopm_hash_table_insert(ht, 19, "test2");
  char *value = "test1";
  bool result = ioopm_hash_table_all(ht, value_equiv, &value);
  ioopm_hash_table_destroy(ht);

  CU_ASSERT_FALSE(result);
}


void test28_hash_table_apply_all(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  ioopm_hash_table_insert(ht, 1, "test1");
  ioopm_hash_table_insert(ht, 2, "Hej");
  ioopm_hash_table_insert(ht, 18, "Roligt");
  ioopm_hash_table_insert(ht, 19, "haha");
  char *value = "bad";
  ioopm_hash_table_apply_to_all(ht, update_value, &value);
  bool result = ioopm_hash_table_all(ht, value_equiv, &value);
  ioopm_hash_table_destroy(ht);

  CU_ASSERT(result);
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
    (NULL == CU_add_test(test_suite1, "test 15", test15_hash_table_is_empty_not_empty)) || 
    //(NULL == CU_add_test(test_suite1, "test 16", test16_hash_table_clear_size)) || 
    (NULL == CU_add_test(test_suite1, "test 17", test17_hash_table_clear)) || 
    (NULL == CU_add_test(test_suite1, "test 18A", test18A_hash_table_keys)) || 
    (NULL == CU_add_test(test_suite1, "test 18B", test18B_hash_table_keys_empty)) ||
    (NULL == CU_add_test(test_suite1, "test 19", test19_hash_table_values)) || 
    (NULL == CU_add_test(test_suite1, "test 20", test20_hash_table_values_empty)) || 
    (NULL == CU_add_test(test_suite1, "test 21", test21_hash_table_has_key)) || 
    (NULL == CU_add_test(test_suite1, "test 22", test22_hash_table_has_key_not)) || 
    (NULL == CU_add_test(test_suite1, "test 23", test23_hash_table_has_value_identical)) || 
    (NULL == CU_add_test(test_suite1, "test 24", test24_hash_table_has_value_equivalent)) || 
    (NULL == CU_add_test(test_suite1, "test 25", test25_hash_table_has_value_not)) || 
    (NULL == CU_add_test(test_suite1, "test 26", test26_hash_table_all)) || 
    (NULL == CU_add_test(test_suite1, "test 27", test27_hash_table_all_not)) || 
    (NULL == CU_add_test(test_suite1, "test 28", test28_hash_table_apply_all))
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

