#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <CUnit/Basic.h>
#include "hash_table.h"
#include "linked_list.h"

#define No_Buckets 17

//TODO: Skriva tester för hjälpfunktioner?

struct entry 
{
  elem_t key;       // holds the key
  elem_t value;   // holds the value
  entry_t *next; // points to the next entry (possibly NULL)
};

struct hash_table
{
  entry_t buckets[No_Buckets]; 
  size_t size;
  ioopm_hash_function hash_function;
  ioopm_eq_function values_eq_function;
};

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

// EQ_VALUES FUNCTION 
bool string_eq(elem_t e1, elem_t e2)
{
  return strcmp(e1.string_value, e2.string_value) == 0;
}


bool int_eq(elem_t e1, elem_t e2)
{
  return e1.int_value == e2.int_value;
}

static void update_int_value(elem_t key, elem_t *value, void *arg)
{
  int *other_int_ptr = arg;
  int other_value = *other_int_ptr;
  (*value).int_value = other_value;
}

/*
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

static bool key_equiv(int key, char *value_ignored, void *x)
{
  int *other_key_ptr = x;
  int other_key = *other_key_ptr;
  return key == other_key;
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
*/

int extract_int_hash_key(elem_t key)
{
  return key.int_value; 
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
//HJÄLP??? SKUTSVÅRT ATT GÖRA RANDOM MED INSERT ? VET EJ VAR DEN HAMNAR 
void test_insert(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(extract_int_hash_key, string_eq);
  CU_ASSERT(ht->size == 0);
  ioopm_eq_function func = *ht->values_eq_function;
   elem_t *result = NULL;

  for (int i = 0; i < No_Buckets; i++)
  {
    entry_t *testEntry = &ht->buckets[i];
    for (int j = 0; j < 5; j++)
    {
      int key_insert = random()%30000; // Finns risk att det blir samma tal, vilket gör att den entryn skrivs över?
      ioopm_hash_table_insert(ht, int_elem(key_insert), string_elem("TESTAR"));
    }
    CU_ASSERT_PTR_NOT_NULL(testEntry->next); // Kan inte ha den om vi inte vet att det alltid stoppas in i varje bucket
  }
  CU_ASSERT(ht->size == (85));
  ioopm_hash_table_destroy(ht);
}

/* //HUR SKA MAN GÖRA??
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

*/

void test4_lookup(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(extract_int_hash_key, string_eq);
  elem_t *result = NULL;

  CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, int_elem(5), result));
  for (int i = 0; i < No_Buckets; i++)
  {
    entry_t *testEntry = &ht->buckets[i];
    int key_insert = random()%30000; // Finns risk att det blir samma tal, vilket gör att den entryn skrivs över?
    ioopm_hash_table_insert(ht, int_elem(key_insert), string_elem("TESTAR"));
    ioopm_hash_table_insert(ht, int_elem(key_insert+1), string_elem("WOWWWW"));
    ioopm_hash_table_insert(ht, int_elem(key_insert+2), string_elem("LOVEIT"));
    ioopm_hash_table_insert(ht, int_elem(key_insert+3), string_elem("HAHAH"));
    //CU_ASSERT(ioopm_hash_table_lookup(ht, int_elem(key_insert), result)); BLIR SEG FAULT!!! MÅSTE GDB
    //CU_ASSERT(ioopm_hash_table_lookup(ht, int_elem(key_insert+1), result));
   // CU_ASSERT(ioopm_hash_table_lookup(ht, int_elem(key_insert+2), result));
    //CU_ASSERT(ioopm_hash_table_lookup(ht, int_elem(key_insert+3), result));
  }

  CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, int_elem(3200), result));
  ioopm_hash_table_destroy(ht);
}

void test5_remove(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(extract_int_hash_key, string_eq);
  CU_ASSERT(ht->size == 0);
  ioopm_eq_function func = *ht->values_eq_function;
  elem_t *result = NULL;

  for (int i = 0; i < No_Buckets; i++)
  {
    for (int j = 0; j < 5; j++)
    {
      int key_insert = random()%30000; // Finns risk att det blir samma tal, vilket gör att den entryn skrivs över?
      ioopm_hash_table_insert(ht, int_elem(key_insert), string_elem("TESTAR"));
      //CU_ASSERT(ioopm_hash_table_lookup(ht, string_elem("TESTAR"), result)); // FEL PÅ LOOKUP!! GDB
      ioopm_hash_table_remove(ht, int_elem(key_insert));
      CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, string_elem("TESTAR"), result));
    }
  }

  for (int i = 0; i < No_Buckets; i++)
  {
    entry_t *testEntry = &ht->buckets[i];
    CU_ASSERT_PTR_NULL(testEntry->next); // Kan inte ha den om vi inte vet att det alltid stoppas in i varje bucket
  }

  CU_ASSERT(ht->size == 0);
  ioopm_hash_table_destroy(ht);
}

void test6_remove_entry_not(void) // PRINTAR UT MASSA ONÖDIGT ???
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(extract_int_hash_key, string_eq);
  ioopm_eq_function func = *ht->values_eq_function;
  elem_t *result = NULL;

  for (int i = 0; i < No_Buckets; i++)
  {
    entry_t *testEntry = &ht->buckets[i];
    for (int j = 0; j < 5; j++)
    {
      int key_insert = random()%30000; // Finns risk att det blir samma tal, vilket gör att den entryn skrivs över?
      ioopm_hash_table_insert(ht, int_elem(key_insert), string_elem("TESTAR"));
      //CU_ASSERT(ioopm_hash_table_lookup(ht, int_elem(key_insert), result)); // FEL PÅ LOOKUP!! GDB
      size_t size_ht = ht->size;
      ioopm_hash_table_remove(ht, int_elem(5000));
      //CU_ASSERT(ioopm_hash_table_lookup(ht, int_elem(key_insert), result)); // FEL PÅ LOOKUP!! GDB
      CU_ASSERT(size_ht == ht->size);
    }
  }
  
  ioopm_hash_table_destroy(ht);
}

void test7_remove_middle(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(extract_int_hash_key, string_eq);
  CU_ASSERT(ht->size == 0);
  ioopm_eq_function func = *ht->values_eq_function;
  elem_t *result = NULL;

  for (int i = 0; i < 85; i++)
  {
    ioopm_hash_table_insert(ht, int_elem(i), string_elem("TESTAR"));
  }

  for (int i = 0; i < No_Buckets; i++)
  {
    ioopm_hash_table_remove(ht, int_elem(43+i));
  }
  
  CU_ASSERT(ht->size == 68);
  ioopm_hash_table_destroy(ht);
}

void test8_empty_hash_table(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(extract_int_hash_key, string_eq);
  CU_ASSERT(ioopm_hash_table_is_empty(ht));

  for (int i = 0; i < 50; i++)
  {
    ioopm_hash_table_insert(ht, int_elem(i), string_elem("TESTAR"));
  }
  CU_ASSERT_FALSE(ioopm_hash_table_is_empty(ht));

  ioopm_hash_table_destroy(ht);
}


void test9_multiple_entry_hash_table(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(extract_int_hash_key, string_eq);

  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 0);

  for (int i = 0; i < 50; i++)
  {
    ioopm_hash_table_insert(ht, int_elem(i), string_elem("TESTAR"));
  }
  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 50);

  ioopm_hash_table_destroy(ht);
}


void test10_hash_table_clear_size(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(extract_int_hash_key, string_eq);


  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 0);

  for (int i = 0; i < 50; i++)
  {
    ioopm_hash_table_insert(ht, int_elem(i), string_elem("TESTAR"));
  }
  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 50);
  ioopm_hash_table_clear(ht);

  for (int i = 0; i < No_Buckets; i++)
  {
    entry_t *testEntry = &ht->buckets[i];
    CU_ASSERT_PTR_NULL(testEntry->next);
    //CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, int_elem(i))); TODO: FIXA LOOKUP SNÄLLALALALALLALAL
  }
  
  ioopm_hash_table_destroy(ht);
}

void test11_hash_table_keys(void) // TODO: Make test case for empty (no keys)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(extract_int_hash_key, string_eq);
  ioopm_list_t *list = ioopm_linked_list_create(int_eq);

  for (int i = 0; i < 17; i++)
  {
    ioopm_hash_table_insert(ht, int_elem(i), string_elem("TESTAR"));
  }

  ioopm_list_t *from_ht = ioopm_hash_table_keys(ht);

  for (int i = 0; i < 17; i++)
  {
   ioopm_linked_list_append(list, int_elem(i));
  }

  for (int i = 0; i < 17; i++)
  {
   CU_ASSERT(int_eq(ioopm_linked_list_get(list, i), ioopm_linked_list_get(from_ht, i)));
  }
  ioopm_hash_table_destroy(ht);
  ioopm_linked_list_destroy(list);
  ioopm_linked_list_destroy(from_ht);
}

void test12_hash_table_keys_empty(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(extract_int_hash_key, string_eq);
  ioopm_list_t *from_ht = ioopm_hash_table_keys(ht);
  CU_ASSERT_PTR_NULL(from_ht->first);
  ioopm_hash_table_destroy(ht);
  ioopm_linked_list_destroy(from_ht);
}

void test13_hash_table_values(void) //TODO: Behöver ändra i has_key funktioner så att det som skickas in i any är funktionerna i strukten!!!!
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(extract_int_hash_key, string_eq);
  ioopm_list_t *list = ioopm_linked_list_create(int_eq);

  for (int i = 0; i < 17; i++)
  {
    ioopm_hash_table_insert(ht, int_elem(i), string_elem("TESTAR"));
    ioopm_hash_table_insert(ht, int_elem(i+17), string_elem("att"));
    ioopm_hash_table_insert(ht, int_elem(i+34), string_elem("det"));
    ioopm_hash_table_insert(ht, int_elem(i+51), string_elem("fungerar"));
  }

  ioopm_list_t *from_ht = ioopm_hash_table_values(ht);

  for (int i = 0; i < 17; i++)
  {
    ioopm_linked_list_append(list, string_elem("TESTAR"));
    ioopm_linked_list_append(list, string_elem("att"));
    ioopm_linked_list_append(list, string_elem("det"));
    ioopm_linked_list_append(list, string_elem("fungerar"));
   
  }

  for (int i = 0; i < 68; i++)
  {
   CU_ASSERT(string_eq(ioopm_linked_list_get(list, i), ioopm_linked_list_get(from_ht, i)));
  }
  ioopm_hash_table_destroy(ht);
  ioopm_linked_list_destroy(list);
  ioopm_linked_list_destroy(from_ht);
}

void test14_hash_table_values_empty(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(extract_int_hash_key, string_eq);
  ioopm_list_t *from_ht = ioopm_hash_table_values(ht);
  CU_ASSERT_PTR_NULL(from_ht->first);
  ioopm_hash_table_destroy(ht);
  ioopm_linked_list_destroy(from_ht);
}

void test15_hash_table_has_key(void) // TODO: Glömt frigöra listan i vår funktion??? som skapas där??? eller ?? alltså i .c filen
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(extract_int_hash_key, string_eq);
  CU_ASSERT_FALSE(ioopm_hash_table_has_key(ht, int_elem(100)));

  for (int i = 0; i < No_Buckets; i++)
  {
    ioopm_hash_table_insert(ht, int_elem(i), string_elem("TESTAR"));
    ioopm_hash_table_insert(ht, int_elem(i+17), string_elem("att"));
    ioopm_hash_table_insert(ht, int_elem(i+34), string_elem("det"));
    ioopm_hash_table_insert(ht, int_elem(i+51), string_elem("fungerar"));
  }

  for(int k = 0; k < No_Buckets; k++)
  {
    CU_ASSERT(ioopm_hash_table_has_key(ht, int_elem(k)));
    CU_ASSERT(ioopm_hash_table_has_key(ht, int_elem(k+17)));
    CU_ASSERT(ioopm_hash_table_has_key(ht, int_elem(k+34)));
    CU_ASSERT(ioopm_hash_table_has_key(ht, int_elem(k+51)));
  }

  CU_ASSERT_FALSE(ioopm_hash_table_has_key(ht, int_elem(100))); // kollar om den inte finns, skapa ett nytt test för bara det?
  ioopm_hash_table_destroy(ht);
}

void test16_hash_table_has_value_identical(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(extract_int_hash_key, string_eq);
  CU_ASSERT_FALSE(ioopm_hash_table_has_value(ht, string_elem("TESTAR")));

  for (int i = 0; i < No_Buckets; i++)
  {
    ioopm_hash_table_insert(ht, int_elem(i), string_elem("TESTAR"));
    ioopm_hash_table_insert(ht, int_elem(i+17), string_elem("att"));
    ioopm_hash_table_insert(ht, int_elem(i+34), string_elem("det"));
    ioopm_hash_table_insert(ht, int_elem(i+51), string_elem("fungerar"));
  }

  
  CU_ASSERT(ioopm_hash_table_has_value(ht, string_elem("TESTAR")));
  CU_ASSERT(ioopm_hash_table_has_value(ht, string_elem("att")));
  CU_ASSERT(ioopm_hash_table_has_value(ht, string_elem("det")));
  CU_ASSERT(ioopm_hash_table_has_value(ht, string_elem("fungerar")));

  CU_ASSERT_FALSE(ioopm_hash_table_has_key(ht, string_elem("Nej"))); // kollar om den inte finns, skapa ett nytt test för bara det?
  ioopm_hash_table_destroy(ht);
}

/* typ ta bort?
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
}*/

void test17_hash_table_all_value_not(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(extract_int_hash_key, string_eq);

  for (int i = 0; i < No_Buckets; i++)
  {
    ioopm_hash_table_insert(ht, int_elem(i), string_elem("TESTAR"));
    ioopm_hash_table_insert(ht, int_elem(i+17), string_elem("att"));
    ioopm_hash_table_insert(ht, int_elem(i+34), string_elem("det"));
    ioopm_hash_table_insert(ht, int_elem(i+51), string_elem("fungerar"));
  }

  char *value = "test1";
  CU_ASSERT(ioopm_hash_table_all(ht, string_eq, &value));

  ioopm_hash_table_destroy(ht);
}

void test18_hash_table_all_value(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(extract_int_hash_key, string_eq);

  for (int i = 0; i < No_Buckets; i++)
  {
    ioopm_hash_table_insert(ht, int_elem(i), string_elem("TESTAR"));
  }

  char *value = "TESTAR";
  CU_ASSERT(ioopm_hash_table_all(ht, string_eq, &value));

  ioopm_hash_table_destroy(ht);
}

void test19_hash_table_all_key_not(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(extract_int_hash_key, string_eq);

  for (int i = 0; i < No_Buckets; i++)
  {
    ioopm_hash_table_insert(ht, int_elem(i), string_elem("TESTAR"));
    ioopm_hash_table_insert(ht, int_elem(i+17), string_elem("att"));
    ioopm_hash_table_insert(ht, int_elem(i+34), string_elem("det"));
    ioopm_hash_table_insert(ht, int_elem(i+51), string_elem("fungerar"));
  }

  char *value = 1;
  CU_ASSERT(ioopm_hash_table_all(ht, int_eq, &value));

  ioopm_hash_table_destroy(ht);
}

void test20_hash_table_all_key(void)
{
 ioopm_hash_table_t *ht = ioopm_hash_table_create(extract_int_hash_key, string_eq);

  for (int i = 0; i < No_Buckets; i++)
  {
    ioopm_hash_table_insert(ht, int_elem(1), string_elem("TESTAR"));
  }

  int *value = 1;
  CU_ASSERT(ioopm_hash_table_all(ht, int_eq, &value));

  ioopm_hash_table_destroy(ht);
}


void test21_hash_table_apply_all(void)
{
   ioopm_hash_table_t *ht = ioopm_hash_table_create(extract_int_hash_key, int_eq);

  for (int i = 0; i < No_Buckets; i++)
  {
    ioopm_hash_table_insert(ht, int_elem(i), int_elem(i));
  }

  int *value = 1;
  CU_ASSERT(ioopm_hash_table_all(ht, int_eq, &value));
  
  ioopm_hash_table_apply_to_all(ht, update_int_value, &value);

  CU_ASSERT(ioopm_hash_table_all(ht, int_eq, &value));

  ioopm_hash_table_destroy(ht);
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
    (NULL == CU_add_test(test_suite1, "test 1", test_insert)) ||
    /*(NULL == CU_add_test(test_suite1, "test 2", test2_insert)) || 
    (NULL == CU_add_test(test_suite1, "test 3", test3_insert_negative)) || */
    (NULL == CU_add_test(test_suite1, "test 4", test4_lookup)) || 
    (NULL == CU_add_test(test_suite1, "test 5", test5_remove)) ||
    //(NULL == CU_add_test(test_suite1, "test 6", test6_lookup_non_existing)) ||
   // (NULL == CU_add_test(test_suite1, "test 7A", test7A_remove_entry_right)) ||
    (NULL == CU_add_test(test_suite1, "test 7", test6_lookup_non_existing)) || 
    (NULL == CU_add_test(test_suite1, "test 8", test7_remove_middle)) || 
    (NULL == CU_add_test(test_suite1, "test 9", test9_remove_entry_ht)) ||
    (NULL == CU_add_test(test_suite1, "test 10", test10_remove_entry_middle)) || 
    (NULL == CU_add_test(test_suite1, "test 11", test11_empty_hash_table)) || 
    (NULL == CU_add_test(test_suite1, "test 12", test12_one_entry_hash_table)) || 
    (NULL == CU_add_test(test_suite1, "test 13", test13_multiple_entry_hash_table)) || 
    (NULL == CU_add_test(test_suite1, "test 14", test14_hash_table_is_empty)) || 
    (NULL == CU_add_test(test_suite1, "test 15", test15_hash_table_is_empty_not_empty)) || 
    (NULL == CU_add_test(test_suite1, "test 16", test16_hash_table_clear_size)) || 
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
    (NULL == CU_add_test(test_suite1, "test 26A", test26A_hash_table_all_value)) || 
    (NULL == CU_add_test(test_suite1, "test 26B", test26B_hash_table_all_key)) || 
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

