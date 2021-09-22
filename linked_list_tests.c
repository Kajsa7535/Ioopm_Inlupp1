#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <CUnit/Basic.h>
#include "linked_list.h"

static int length_of_list(ioopm_list_t *list)
{
    ioopm_link_t *element = list->first;
    int acc = 0;
  
    if(list->first == NULL)
    {
        return acc;
    }
    
    do 
    {
        acc = acc + 1;
        element = element->next;
    } while (element != NULL);
    return acc;
}

int init_suite(void)
{
  return 0;
}

int clean_suite(void)
{
  return 0;
}

void test1_linked_list_prepend(void)
{
  ioopm_list_t *list = ioopm_linked_list_create();
  ioopm_linked_list_prepend(list, 2);
  ioopm_linked_list_prepend(list, 1);
  int result = list->first->next->value;

  CU_ASSERT(result == 2);
}

void test2_linked_list_append(void)
{
  ioopm_list_t *list = ioopm_linked_list_create();
  ioopm_linked_list_append(list, 1);
  ioopm_linked_list_append(list, 2);
  int result = list->first->value;

  CU_ASSERT(result == 1);
}


void test3_length_of_list(void)
{
  ioopm_list_t *list = ioopm_linked_list_create();
  ioopm_linked_list_prepend(list, 1);
  ioopm_linked_list_append(list, 2);
  int result = length_of_list(list);

  CU_ASSERT(result == 2);
}

void test4_linked_list_insert_first(void)
{
  ioopm_list_t *list = ioopm_linked_list_create();
  ioopm_linked_list_prepend(list, 0);
  ioopm_linked_list_append(list, 1);
  ioopm_linked_list_append(list, 2);
  ioopm_linked_list_append(list, 3);
  ioopm_linked_list_insert(list, 0, 10);
  int result = list->first->value;

  CU_ASSERT(result == 10);
}

void test5_linked_list_insert_last(void)
{
  ioopm_list_t *list = ioopm_linked_list_create();
  ioopm_linked_list_prepend(list, 0);
  ioopm_linked_list_append(list, 1);
  ioopm_linked_list_append(list, 2);
  ioopm_linked_list_append(list, 3);
  ioopm_linked_list_insert(list, 4, 10);
  int result = list->last->value;

  CU_ASSERT(result == 10);
}

void test6_linked_list_insert_middle(void)
{
  ioopm_list_t *list = ioopm_linked_list_create();
  ioopm_linked_list_prepend(list, 0);
  ioopm_linked_list_append(list, 1);
  ioopm_linked_list_append(list, 2);
  ioopm_linked_list_append(list, 3);
  ioopm_linked_list_insert(list, 2, 10);
  int result1 = list->first->next->next->value;
  int result2 = list->first->next->next->next->value;

  CU_ASSERT(result1 == 10 && result2 == 2);
}

void test7_linked_list_insert_invalid(void)
{
  ioopm_list_t *list = ioopm_linked_list_create();
  ioopm_linked_list_prepend(list, 0);
  ioopm_linked_list_append(list, 1);
  ioopm_linked_list_insert(list, 5, 10);
  int length = length_of_list(list);

  CU_ASSERT(length == 2);
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
    (NULL == CU_add_test(test_suite1, "test 1", test1_linked_list_prepend)) ||
    (NULL == CU_add_test(test_suite1, "test 2", test2_linked_list_append)) ||
    (NULL == CU_add_test(test_suite1, "test 3", test3_length_of_list)) || 
    (NULL == CU_add_test(test_suite1, "test 4", test4_linked_list_insert_first))|| 
    (NULL == CU_add_test(test_suite1, "test 5", test5_linked_list_insert_last))|| 
    (NULL == CU_add_test(test_suite1, "test 6", test6_linked_list_insert_middle)) || 
    (NULL == CU_add_test(test_suite1, "test 7", test7_linked_list_insert_invalid))
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
