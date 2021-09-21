#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <CUnit/Basic.h>
#include "linked_list.h"

static int length_of_list(ioopm_list_t *list)
{
    ioopm_list_t *element = list->next;
    int acc = 0;
    while (element != NULL)
    {
        acc = acc + 1;
        element = element->next;
    }
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
  int result = list->next->value;

  CU_ASSERT(result == 1);
}


void test2_linked_list_append(void)
{
  ioopm_list_t *list = ioopm_linked_list_create();
  ioopm_linked_list_append(list, 1);
  ioopm_linked_list_append(list, 2);
  int result = list->next->next->value;

  CU_ASSERT(result == 2);
}

void test3_length_of_list(void)
{
  ioopm_list_t *list = ioopm_linked_list_create();
  ioopm_linked_list_prepend(list, 1);
  ioopm_linked_list_append(list, 2);
  int result = length_of_list(list);

  CU_ASSERT(result == 2);
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
    (NULL == CU_add_test(test_suite1, "test 3", test3_length_of_list))
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