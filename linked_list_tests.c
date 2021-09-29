#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <CUnit/Basic.h>
#include "linked_list.h"
#include "iterator.h"
#include "common.h"

struct link 
{
    int value;  // holds the value
    ioopm_link_t *next; // points to the next entry (possibly NULL)
};

struct list 
{
    ioopm_link_t *first;
    ioopm_link_t *last;
    size_t size;
};

struct iterator 
{
    ioopm_link_t *current; // ska vara dubbelpekare ???
    ioopm_list_t *list;
};


static size_t length_of_list(ioopm_list_t *list)
{
    ioopm_link_t *element = list->first;
    size_t acc = 0;
  
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


static bool value_int_equiv(int value, void *x)
{
  int *other_int_ptr = x;
  int other_value = *other_int_ptr;
  return value == other_value;
}

static void update_int_value(int *value, void *arg)
{
  int *other_int_ptr = arg;
  int other_value = *other_int_ptr;
  *value = other_value;
}

bool int_eq(elem_t e1, elem_t e2)
{
  return e1.int_value == e2.int_value;
}

bool string_eq(elem_t e1, elem_t e2)
{
  return strcmp(e1.string_value, e2.string_value) == 0;
}

int init_suite(void)
{
  return 0;
}

int clean_suite(void)
{
  return 0;
}

//TODO: Skriv test för ioopm clear
void test1_linked_list_prepend(void)
{
  ioopm_list_t *list = ioopm_linked_list_create(int_eq);

  int value = random()%100;
  ioopm_linked_list_prepend(list, int_elem(value));
  CU_ASSERT(int_eq(int_elem(list->last->value), int_elem(value))); // Test if only one element has a correct last pointer

  for (int i = 0; i < 10; i++)
  {
    int value = random()%100;
    ioopm_linked_list_prepend(list, int_elem(value));
    CU_ASSERT(ioopm_linked_list_contains(list, int_elem(value)));
    CU_ASSERT(int_eq(int_elem(list->first->value), int_elem(value)));
  }

  ioopm_linked_list_destroy(list);
}

void test2_linked_list_append(void)
{
  ioopm_list_t *list = ioopm_linked_list_create(int_eq);

  for (int i = 0; i < 10; i++)
  {
    int value = random()%100;
    ioopm_linked_list_append(list, int_elem(value));
    CU_ASSERT(ioopm_linked_list_contains(list, int_elem(value)));
    CU_ASSERT(int_eq(int_elem(list->last->value), int_elem(value)));
  }

  ioopm_linked_list_destroy(list);
}


void test3_linked_list_insert(void)
{
  ioopm_list_t *list = ioopm_linked_list_create(int_eq);

  for (int i = 0; i < 10; i++)
  {
    int value = random()%100;
    ioopm_linked_list_insert(list, 0, int_elem(value));
    CU_ASSERT(ioopm_linked_list_contains(list, int_elem(value)));
    CU_ASSERT(int_eq(int_elem(list->first->value), int_elem(value)));
    ioopm_linked_list_insert(list, int_elem(list->size - 1), int_elem(value + 1));
    CU_ASSERT(ioopm_linked_list_contains(list, int_elem(value + 1)));
    CU_ASSERT(int_eq(int_elem(list->last->value), int_elem(value + 1)));
  }

  ioopm_linked_list_destroy(list);
}

/*
void test5_linked_list_insert_last(void)
{
  ioopm_list_t *list = ioopm_linked_list_create();
  ioopm_linked_list_prepend(list, 0);
  ioopm_linked_list_append(list, 1);
  ioopm_linked_list_append(list, 2);
  ioopm_linked_list_append(list, 3);
  ioopm_linked_list_insert(list, 4, 10);
  int result = list->last->value;
  ioopm_linked_list_destroy(list);

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
  ioopm_linked_list_destroy(list);

  CU_ASSERT(result1 == 10 && result2 == 2);
}


void test7_linked_list_insert_invalid(void)
{
  ioopm_list_t *list = ioopm_linked_list_create();
  ioopm_linked_list_prepend(list, 0);
  ioopm_linked_list_append(list, 1);
  ioopm_linked_list_insert(list, 5, 10);
  size_t length = length_of_list(list);
  ioopm_linked_list_destroy(list);

  CU_ASSERT(length == 2);
}

void test8_linked_list_remove_first(void)
{
  ioopm_list_t *list = ioopm_linked_list_create();
   ioopm_linked_list_prepend(list, 0);
  ioopm_linked_list_append(list, 1);
  ioopm_linked_list_append(list, 2);
  ioopm_linked_list_append(list, 3);
  int value = ioopm_linked_list_remove(list, 0);
  ioopm_linked_list_destroy(list);

  CU_ASSERT(value == 0);
}

void test9_linked_list_remove_last(void)
{
  ioopm_list_t *list = ioopm_linked_list_create();
  ioopm_linked_list_prepend(list, 0);
  ioopm_linked_list_append(list, 1);
  ioopm_linked_list_append(list, 2);
  ioopm_linked_list_append(list, 3);
  int value = ioopm_linked_list_remove(list, 3);
  ioopm_linked_list_destroy(list);

  CU_ASSERT(value == 3);
}


void test10_linked_list_remove_middle(void)
{
  ioopm_list_t *list = ioopm_linked_list_create();
  ioopm_linked_list_prepend(list, 0);
  ioopm_linked_list_append(list, 1);
  ioopm_linked_list_append(list, 2);
  ioopm_linked_list_append(list, 3);

  ioopm_linked_list_remove(list, 1);

  CU_ASSERT(list->last->value == 3);
  ioopm_linked_list_destroy(list);
}

void test11_linked_list_get_first(void)
{
  ioopm_list_t *list = ioopm_linked_list_create();
  ioopm_linked_list_prepend(list, 0);
  ioopm_linked_list_append(list, 1);
  ioopm_linked_list_append(list, 2);
  ioopm_linked_list_append(list, 3);

  int value = ioopm_linked_list_get(list, 0);

  CU_ASSERT(value == 0);
  ioopm_linked_list_destroy(list);
}

void test12_linked_list_get_last(void)
{
  ioopm_list_t *list = ioopm_linked_list_create();
  ioopm_linked_list_prepend(list, 0);
  ioopm_linked_list_append(list, 1);
  ioopm_linked_list_append(list, 2);
  ioopm_linked_list_append(list, 3);

  int value = ioopm_linked_list_get(list, 3);

  CU_ASSERT(value == 3);
  ioopm_linked_list_destroy(list);
}

void test13_linked_list_contain_first(void)
{
  ioopm_list_t *list = ioopm_linked_list_create();
  ioopm_linked_list_prepend(list, 0);
  ioopm_linked_list_append(list, 1);
  ioopm_linked_list_append(list, 2);
  ioopm_linked_list_append(list, 3);

  bool result = ioopm_linked_list_contains(list, 0);

  CU_ASSERT(result);
  ioopm_linked_list_destroy(list);
}

void test14_linked_list_contain_last(void)
{
  ioopm_list_t *list = ioopm_linked_list_create();
  ioopm_linked_list_prepend(list, 0);
  ioopm_linked_list_append(list, 1);
  ioopm_linked_list_append(list, 2);
  ioopm_linked_list_append(list, 3);

  bool result = ioopm_linked_list_get(list, 3);

  CU_ASSERT(result);
  ioopm_linked_list_destroy(list);
}

void test15_linked_list_contain_not(void)
{
  ioopm_list_t *list = ioopm_linked_list_create();
  ioopm_linked_list_prepend(list, 0);
  ioopm_linked_list_append(list, 1);
  ioopm_linked_list_append(list, 2);
  ioopm_linked_list_append(list, 3);

  bool result = ioopm_linked_list_contains(list,6);

  CU_ASSERT_FALSE(result);
  ioopm_linked_list_destroy(list);
}

void test16_linked_list_size(void)
{
  ioopm_list_t *list = ioopm_linked_list_create();
  ioopm_linked_list_prepend(list, 0);
  ioopm_linked_list_append(list, 1);
  ioopm_linked_list_append(list, 2);
  ioopm_linked_list_append(list, 3);

  size_t size = ioopm_linked_list_size(list);

  CU_ASSERT(size == 4);
  ioopm_linked_list_destroy(list);
}

void test17_linked_list_is_empty(void)
{
  ioopm_list_t *list = ioopm_linked_list_create();
  bool result = ioopm_linked_list_is_empty(list);

  CU_ASSERT(result);
  ioopm_linked_list_destroy(list);
}

void test18_linked_list_all(void)
{
  ioopm_list_t *list = ioopm_linked_list_create();
  ioopm_linked_list_prepend(list, 1);
  ioopm_linked_list_append(list, 1);
  ioopm_linked_list_append(list, 1);
  ioopm_linked_list_append(list, 1);

  int value = 1;
  bool result = ioopm_linked_list_all(list, value_int_equiv, &value);

  CU_ASSERT(result);
  ioopm_linked_list_destroy(list);
}

void test19_linked_list_all_not(void)
{
  ioopm_list_t *list = ioopm_linked_list_create();
  ioopm_linked_list_prepend(list, 1);
  ioopm_linked_list_append(list, 1);
  ioopm_linked_list_append(list, 1);
  ioopm_linked_list_append(list, 0);

  int value = 1;
  bool result = ioopm_linked_list_all(list, value_int_equiv, &value);

  CU_ASSERT_FALSE(result);
  ioopm_linked_list_destroy(list);
}

void test20_linked_list_any(void)
{
  ioopm_list_t *list = ioopm_linked_list_create();
  ioopm_linked_list_prepend(list, 1);
  ioopm_linked_list_append(list, 0);
  ioopm_linked_list_append(list, 1);
  ioopm_linked_list_append(list, 3);

  int value = 1;
  bool result = ioopm_linked_list_any(list, value_int_equiv, &value);

  CU_ASSERT(result);
  ioopm_linked_list_destroy(list);
}

void test21_linked_list_any_not(void)
{
  ioopm_list_t *list = ioopm_linked_list_create();
  ioopm_linked_list_prepend(list, 0);
  ioopm_linked_list_append(list, 2);
  ioopm_linked_list_append(list, 3);
  ioopm_linked_list_append(list, 3);

  int value = 1;
  bool result = ioopm_linked_list_any(list, value_int_equiv, &value);

  CU_ASSERT_FALSE(result);
  ioopm_linked_list_destroy(list);
}

void test22_linked_apply_to_all(void)
{
  ioopm_list_t *list = ioopm_linked_list_create();
  ioopm_linked_list_prepend(list, 0);
  ioopm_linked_list_append(list, 1);
  ioopm_linked_list_append(list, 2);
  ioopm_linked_list_append(list, 3);

  int value = 1;
  ioopm_linked_apply_to_all(list, update_int_value, &value);
  bool result = ioopm_linked_list_all(list, value_int_equiv, &value);

  CU_ASSERT(result);
  ioopm_linked_list_destroy(list);
}

void test23_iterator_next(void)
{
  ioopm_list_t *list = ioopm_linked_list_create();
  ioopm_linked_list_prepend(list, 0);
  ioopm_linked_list_append(list, 1);
  ioopm_linked_list_append(list, 2);
  ioopm_linked_list_append(list, 3);

  ioopm_list_iterator_t *iter = ioopm_list_iterator(list);
  ioopm_iterator_next(iter);
  int result = iter->current->value;

  CU_ASSERT(result == 1);
  ioopm_iterator_destroy(iter);
  ioopm_linked_list_destroy(list);
}

void test24_iterator_has_next(void)
{
  ioopm_list_t *list = ioopm_linked_list_create();
 // ioopm_linked_list_prepend(list, 0);

  ioopm_list_iterator_t *iter = ioopm_list_iterator(list);
  bool result = ioopm_iterator_has_next(iter);

  CU_ASSERT_FALSE(result);
  ioopm_iterator_destroy(iter);
  ioopm_linked_list_destroy(list);
}

void test25_iterator_reset(void)
{
  ioopm_list_t *list = ioopm_linked_list_create();
  ioopm_linked_list_prepend(list, 0);
  ioopm_linked_list_append(list, 1);

  ioopm_list_iterator_t *iter = ioopm_list_iterator(list);
  ioopm_iterator_next(iter);
  ioopm_iterator_reset(iter);
  int result = iter->current->value;

  CU_ASSERT(result == 0);
  ioopm_iterator_destroy(iter);
  ioopm_linked_list_destroy(list);
}

void test26_iterator_current(void)
{
  ioopm_list_t *list = ioopm_linked_list_create();
  ioopm_linked_list_prepend(list, 0);
  ioopm_linked_list_append(list, 1);
  ioopm_linked_list_append(list, 2);

  ioopm_list_iterator_t *iter = ioopm_list_iterator(list);
  ioopm_iterator_next(iter);
  int result = ioopm_iterator_current(iter);

  CU_ASSERT(result == 1);
  ioopm_iterator_destroy(iter);
  ioopm_linked_list_destroy(list);
}

void test27_smart(void)
{
  ioopm_list_t *list = ioopm_linked_list_create();

  for (int i = 0; i < 10; i++)
  {
    int value = random()%100;
    ioopm_linked_list_prepend(list, int_elem(value));
    CU_ASSERT(ioopm_linked_list_contains(list, int_elem(value)));
  }
  ioopm_linked_list_destroy(list);
}*/

int main()
{
  CU_pSuite test_suite1 = NULL;
  CU_pSuite test_suite2 = NULL;

  if (CUE_SUCCESS != CU_initialize_registry())
    return CU_get_error();

  test_suite1 = CU_add_suite("Test Suite 1", init_suite, clean_suite);
  test_suite2 = CU_add_suite("Test Suite 2", init_suite, clean_suite);
  if (NULL == test_suite1)
    {
      CU_cleanup_registry();
      return CU_get_error();
    }

  if (
    (NULL == CU_add_test(test_suite1, "test 1", test1_linked_list_prepend)) ||
    (NULL == CU_add_test(test_suite1, "test 2", test2_linked_list_append)) ||
    (NULL == CU_add_test(test_suite2, "test 3", test3_linked_list_insert)) /*|| 
    (NULL == CU_add_test(test_suite2, "test 5", test5_linked_list_insert_last))|| 
    (NULL == CU_add_test(test_suite2, "test 6", test6_linked_list_insert_middle)) || 
    (NULL == CU_add_test(test_suite2, "test 7", test7_linked_list_insert_invalid)) || 
    (NULL == CU_add_test(test_suite2, "test 8", test8_linked_list_remove_first)) || 
    (NULL == CU_add_test(test_suite2, "test 9", test9_linked_list_remove_last)) || 
    (NULL == CU_add_test(test_suite2, "test 10", test10_linked_list_remove_middle))|| 
    (NULL == CU_add_test(test_suite2, "test 11", test11_linked_list_get_first))|| 
    (NULL == CU_add_test(test_suite2, "test 12", test12_linked_list_get_last))|| 
    (NULL == CU_add_test(test_suite2, "test 13", test13_linked_list_contain_first))|| 
    (NULL == CU_add_test(test_suite2, "test 14", test14_linked_list_contain_last))|| 
    (NULL == CU_add_test(test_suite2, "test 15", test15_linked_list_contain_not))|| 
    (NULL == CU_add_test(test_suite2, "test 16", test16_linked_list_size))|| 
    (NULL == CU_add_test(test_suite2, "test 17", test17_linked_list_is_empty))|| 
    (NULL == CU_add_test(test_suite2, "test 18", test18_linked_list_all))|| 
    (NULL == CU_add_test(test_suite2, "test 19", test19_linked_list_all_not))|| 
    (NULL == CU_add_test(test_suite2, "test 20", test20_linked_list_any))|| 
    (NULL == CU_add_test(test_suite2, "test 21", test21_linked_list_any_not))|| 
    (NULL == CU_add_test(test_suite2, "test 22", test22_linked_apply_to_all))|| 
    (NULL == CU_add_test(test_suite2, "test 23", test23_iterator_next))|| 
    (NULL == CU_add_test(test_suite2, "test 24", test24_iterator_has_next))|| 
    (NULL == CU_add_test(test_suite2, "test 25", test25_iterator_reset))|| 
    (NULL == CU_add_test(test_suite2, "test 26", test26_iterator_current))*/
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

