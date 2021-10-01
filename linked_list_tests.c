#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <CUnit/Basic.h>
#include "linked_list.h"
#include "iterator.h"
#include "common.h"


struct link 
{
    elem_t value;  // holds the value
    ioopm_link_t *next; // points to the next entry (possibly NULL)
};

struct list 
{
    ioopm_link_t *first; // points to the first element
    ioopm_link_t *last; // points to the last element
    size_t size; // counts the number of elements in the list
    ioopm_eq_function comp; 
};

struct iterator 
{
    ioopm_link_t *current;  
    ioopm_list_t *list;
};

// FUNKTIONCS THAT IS NEEDED IN THE TESTS!

static bool value_int_equiv(elem_t key, elem_t value, void *x)
{
  int *other_int_ptr = x;
  int other_value = *other_int_ptr;
  return value.int_value == other_value;
}

static void update_int_value(elem_t key, elem_t *value, void *arg)
{
  int *other_int_ptr = arg;
  int other_value = *other_int_ptr;
  (*value).int_value = other_value;
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

  ioopm_eq_function func = *list->comp;
  int value = random()%100;
  ioopm_linked_list_prepend(list, int_elem(value));
  CU_ASSERT(func((list->last->value), int_elem(value))); // Test if only one element has a correct last pointer

  for (int i = 0; i < 10; i++)
  {
    int value = random()%100;
    ioopm_linked_list_prepend(list, int_elem(value));
    CU_ASSERT(ioopm_linked_list_contains(list, int_elem(value)));
    CU_ASSERT(func((list->first->value), int_elem(value)));
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
    CU_ASSERT(int_eq((list->last->value), int_elem(value)));
  }

  ioopm_linked_list_destroy(list);
}


void test3A_linked_list_insert_first(void)
{
  ioopm_list_t *list = ioopm_linked_list_create(int_eq);
  CU_ASSERT(list->size == 0);

  for (int i = 0; i < 10; i++) // Case: insert first in list
  {
    int value = random()%100;
    ioopm_linked_list_insert(list, 0, int_elem(value));
    CU_ASSERT(ioopm_linked_list_contains(list, int_elem(value)));
    CU_ASSERT(int_eq((list->first->value), int_elem(value)));
    CU_ASSERT(list->size == i+1);
  }
  ioopm_linked_list_destroy(list);
}

void test3B_linked_list_insert_last(void)
{
  ioopm_list_t *list = ioopm_linked_list_create(int_eq);

  for (int i = 0; i < 10; i++) // Case: insert last in list
  {
    int value = random()%100;
    ioopm_linked_list_insert(list, list->size, int_elem(value));
    CU_ASSERT(ioopm_linked_list_contains(list, int_elem(value)));
    CU_ASSERT(int_eq((list->last->value), int_elem(value)));
  }
  ioopm_linked_list_destroy(list);
}

void test3C_linked_list_insert_randomly(void)
{
  ioopm_list_t *list = ioopm_linked_list_create(int_eq);

  int value = random()%100;
  ioopm_linked_list_prepend(list, int_elem(value));

  for (int i = 0; i < 10; i++) // Case: insert randomly in list
  {
    int value = random()%100;
    int index = random()%list->size;
    ioopm_linked_list_insert(list, index, int_elem(value));
    CU_ASSERT(ioopm_linked_list_contains(list, int_elem(value)));
    CU_ASSERT(int_eq(ioopm_linked_list_get(list, index), int_elem(value)));
  }

  ioopm_linked_list_destroy(list);
}

void test4_linked_list_remove(void)
{
  ioopm_list_t *list = ioopm_linked_list_create(int_eq);
  
  for (int i = 0; i < 11; i++) // Prepends 11 random elements
  {
    int value = random()%100;
    ioopm_linked_list_prepend(list, int_elem(value));
  }
  CU_ASSERT(list->size == 11)

  for (int i = 0; i < 10; i++) // Removes 10 elements randomly
  {
    int index = random()%((list->size)-1); // genererar random index till element som tas bort från listan
    elem_t get = ioopm_linked_list_get(list, index);
    elem_t remove = ioopm_linked_list_remove(list, index);
    CU_ASSERT(int_eq(get, remove)); // check that the elemt removed is the element on that index
  }
  
  CU_ASSERT(list->size == 1); // check that size of linked list changes
  ioopm_linked_list_remove(list, 0);
  CU_ASSERT(ioopm_linked_list_is_empty(list));

  ioopm_linked_list_destroy(list);
}

void test5_linked_list_get(void)
{
  ioopm_list_t *list = ioopm_linked_list_create(int_eq);
  int value = random()%100;
  ioopm_linked_list_prepend(list, int_elem(value));

   for (int i = 0; i < 10; i++) // Prepends 10 random elements
  {
    int value = random()%100;
    int index = random()%list->size;
    ioopm_linked_list_insert(list, index, int_elem(value));
    CU_ASSERT(int_eq(ioopm_linked_list_get(list, index), int_elem(value)));
  }
  ioopm_linked_list_destroy(list);
}

void test6_linked_list_contains(void)
{
  ioopm_list_t *list = ioopm_linked_list_create(int_eq);

  bool result = ioopm_linked_list_contains(list, int_elem(1)); // Case: Empty list
  CU_ASSERT_FALSE(result);

  int value = random()%100;
  ioopm_linked_list_prepend(list, int_elem(value));

  for (int i = 0; i < 10; i++) // Case: insert randomly in list
  {
    int value = random()%100;
    int index = random()%list->size;
    ioopm_linked_list_insert(list, index, int_elem(value));
    CU_ASSERT(ioopm_linked_list_contains(list, int_elem(value)));
  }

  ioopm_linked_list_destroy(list);
}

void test7_linked_list_size(void)
{
  ioopm_list_t *list = ioopm_linked_list_create(int_eq);
  
  CU_ASSERT(list->size == ioopm_linked_list_size(list)); // Case: Empty list

  int value = random()%100;
  ioopm_linked_list_prepend(list, int_elem(value));

 for (int i = 0; i < 10; i++) // Case: insert randomly in list
  {
    int value = random()%100;
    int index = random()%list->size;
    ioopm_linked_list_insert(list, index, int_elem(value));
    CU_ASSERT(list->size == ioopm_linked_list_size(list));
  }

  ioopm_linked_list_destroy(list);
}

void test8_linked_list_is_empty(void)
{
  ioopm_list_t *list = ioopm_linked_list_create(int_eq);
  CU_ASSERT(ioopm_linked_list_is_empty(list));

  int value = random()%100;
  ioopm_linked_list_prepend(list, int_elem(value));

 for (int i = 0; i < 10; i++) // Case: insert randomly in list
  {
    int value = random()%100;
    int index = random()%list->size;
    ioopm_linked_list_insert(list, index, int_elem(value));
  }

  CU_ASSERT_FALSE(ioopm_linked_list_is_empty(list));

  ioopm_linked_list_destroy(list);
}

void test9_linked_list_all(void)
{
  ioopm_list_t *list = ioopm_linked_list_create(int_eq);

  int value = random()%100;
  int value2 = random()%100;

  ioopm_linked_list_prepend(list, int_elem(value));

  for (int i = 0; i < 10; i++) // Prepend 1 10 times in list
  {
    ioopm_linked_list_prepend(list, int_elem(value));
  }

  CU_ASSERT(ioopm_linked_list_all(list, value_int_equiv, &value));

  ioopm_linked_list_prepend(list, int_elem(value2)); // Case: not true
  CU_ASSERT_FALSE(ioopm_linked_list_all(list, value_int_equiv, &value)); 

  ioopm_linked_list_destroy(list);
}


void test10_linked_list_any(void)
{
  ioopm_list_t *list = ioopm_linked_list_create(int_eq);

  int value = random()%100;
  int value2 = random()%100;

  ioopm_linked_list_prepend(list, int_elem(value2));

  for (int i = 0; i < 10; i++) // Prepend 1 10 times in list
  {
    ioopm_linked_list_prepend(list, int_elem(value2));
  }

  CU_ASSERT_FALSE(ioopm_linked_list_any(list, value_int_equiv, &value));

  ioopm_linked_list_prepend(list, int_elem(value)); // Case: not true
  CU_ASSERT(ioopm_linked_list_any(list, value_int_equiv, &value)); 

  ioopm_linked_list_destroy(list);
}

void test11_linked_apply_to_all(void)
{
  ioopm_list_t *list = ioopm_linked_list_create(int_eq);

  int value1 = random()%100;
  int value2 = random()%100;

  ioopm_linked_list_prepend(list, int_elem(value2));

  for (int i = 0; i < 10; i++) // Prepend 1 10 times in list
  {
    ioopm_linked_list_prepend(list, int_elem(value2));
  }
  
  CU_ASSERT(ioopm_linked_list_all(list, value_int_equiv, &value2));
  ioopm_linked_apply_to_all(list, update_int_value, &value1);
  CU_ASSERT(ioopm_linked_list_all(list, value_int_equiv, &value1));

  ioopm_linked_list_destroy(list);
}

void test12_iterator_next(void)
{
  ioopm_list_t *list = ioopm_linked_list_create(int_eq);
  
  for (int i = 0; i < 10; i++)
  {
    int value = random()%100;
    ioopm_linked_list_prepend(list, int_elem(value));
  }

  ioopm_list_iterator_t *iter = ioopm_list_iterator(list);
  CU_ASSERT(int_eq(iter->current->value, ioopm_linked_list_get(list, 0)));

  for (int k = 1; k < 10; k++)
  {
    CU_ASSERT(int_eq(ioopm_iterator_next(iter), ioopm_linked_list_get(list, k))); 
  }

  ioopm_iterator_destroy(iter);
  ioopm_linked_list_destroy(list);
}


void test13_iterator_has_next(void)
{
  ioopm_list_t *list = ioopm_linked_list_create(int_eq);

  for (int i = 0; i < 10; i++)
  {
    int value = random()%100;
    ioopm_linked_list_prepend(list, int_elem(value));
  }

  ioopm_list_iterator_t *iter = ioopm_list_iterator(list);
  CU_ASSERT(ioopm_iterator_has_next(iter)); 
  for (int k = 1; k < 10; k++)
  {
    CU_ASSERT(ioopm_iterator_has_next(iter));
    ioopm_iterator_next(iter);
  }
   CU_ASSERT_FALSE(ioopm_iterator_has_next(iter)); 

  ioopm_iterator_destroy(iter);
  ioopm_linked_list_destroy(list);
}


void test14_iterator_reset(void)
{
  ioopm_list_t *list = ioopm_linked_list_create(int_eq);

  for (int i = 0; i < 10; i++)
  {
    int value = random()%100;
    ioopm_linked_list_prepend(list, int_elem(value));
  }

  ioopm_list_iterator_t *iter = ioopm_list_iterator(list);

  CU_ASSERT_PTR_EQUAL(iter->current, list->first);

  for (int k = 1; k < 10; k++)
  {
    ioopm_iterator_next(iter);
  }

  CU_ASSERT_PTR_EQUAL(iter->current, list->last);
  ioopm_iterator_reset(iter);
  CU_ASSERT_PTR_EQUAL(iter->current, list->first);

  ioopm_iterator_destroy(iter);
  ioopm_linked_list_destroy(list);
}


void test15_iterator_current(void)
{
  ioopm_list_t *list = ioopm_linked_list_create(int_eq);
  
  for (int i = 0; i < 10; i++)
  {
    int value = random()%100;
    ioopm_linked_list_prepend(list, int_elem(value));
  }

  ioopm_list_iterator_t *iter = ioopm_list_iterator(list);
  CU_ASSERT(int_eq(ioopm_iterator_current(iter), ioopm_linked_list_get(list, 0)));

  for (int k = 1; k < 10; k++)
  {
    ioopm_iterator_next(iter);
    CU_ASSERT(int_eq(ioopm_iterator_current(iter), ioopm_linked_list_get(list, k))); 
  }

  ioopm_iterator_destroy(iter);
  ioopm_linked_list_destroy(list);
}

int main()
{
  CU_pSuite test_suite1 = NULL;
  CU_pSuite test_suite2 = NULL;
  CU_pSuite test_suite3 = NULL;
  CU_pSuite test_suite4 = NULL;

  if (CUE_SUCCESS != CU_initialize_registry())
    return CU_get_error();

  test_suite1 = CU_add_suite("Test Suite 1", init_suite, clean_suite);
  test_suite2 = CU_add_suite("Test Suite 2", init_suite, clean_suite);
  test_suite3 = CU_add_suite("Test Suite 3", init_suite, clean_suite);
  test_suite4 = CU_add_suite("Test Suite 4", init_suite, clean_suite);
  
  if (NULL == test_suite1)
    {
      CU_cleanup_registry();
      return CU_get_error();
    }

  if (NULL == test_suite2)
    {
      CU_cleanup_registry();
      return CU_get_error();
    }

  if (NULL == test_suite3)
    {
      CU_cleanup_registry();
      return CU_get_error();
    }

  if (NULL == test_suite4)
    {
      CU_cleanup_registry();
      return CU_get_error();
    }

  if (
    (NULL == CU_add_test(test_suite1, "test 1", test1_linked_list_prepend)) ||
    (NULL == CU_add_test(test_suite1, "test 2", test2_linked_list_append)) ||
    (NULL == CU_add_test(test_suite1, "test 3A", test3A_linked_list_insert_first)) || 
    (NULL == CU_add_test(test_suite1, "test 3B", test3B_linked_list_insert_last)) || 
    (NULL == CU_add_test(test_suite1, "test 3C", test3C_linked_list_insert_randomly)) ||
    (NULL == CU_add_test(test_suite1, "test 4", test4_linked_list_remove)) || 
    (NULL == CU_add_test(test_suite2, "test 5", test5_linked_list_get)) ||
    (NULL == CU_add_test(test_suite2, "test 6", test6_linked_list_contains)) || 
    (NULL == CU_add_test(test_suite2, "test 7", test7_linked_list_size)) || 
    (NULL == CU_add_test(test_suite2, "test 8", test8_linked_list_is_empty)) ||
    (NULL == CU_add_test(test_suite3, "test 9", test9_linked_list_all)) ||
    (NULL == CU_add_test(test_suite3, "test 10", test10_linked_list_any)) || 
    (NULL == CU_add_test(test_suite3, "test 11", test11_linked_apply_to_all)) || 
    (NULL == CU_add_test(test_suite4, "test 12", test12_iterator_next)) || 
    (NULL == CU_add_test(test_suite4, "test 13", test13_iterator_has_next)) || 
    (NULL == CU_add_test(test_suite4, "test 14", test14_iterator_reset)) || 
    (NULL == CU_add_test(test_suite4, "test 15", test15_iterator_current))
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

