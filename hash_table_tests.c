#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include <CUnit/Basic.h>
#include "hash_table.h"
#include "linked_list.h"

#define No_Primes 11

static const size_t primes[] = {17, 31, 67, 127, 257, 509, 1021, 2053, 4099, 8191, 16381};

struct entry 
{
  elem_t key;       // holds the key
  elem_t value;   // holds the value
  entry_t *next; // points to the next entry (possibly NULL)
};

struct hash_table
{
  size_t size;
  double load_factor; 
  int index_primes;
  ioopm_hash_function hash_function; 
  ioopm_eq_function value_eq_function;
  ioopm_eq_function key_eq_function;
  entry_t **buckets; 
};

struct link 
{
    int value;  // holds the value
    ioopm_link_t *next; // points to the next entry (possibly NULL)
};

struct list 
{
    ioopm_link_t *first; // points to the first element in list
    ioopm_link_t *last; // points to the last element in list
    size_t size;
};

struct iterator 
{
    ioopm_link_t *current; // points to an element
    ioopm_list_t *list; // points to the list where current started
};

static entry_t **find_previous_entry_for_key_ptr(ioopm_hash_table_t *ht, entry_t **entry, elem_t search_key)
{
  assert(*entry);
  //while (*entry)
  {
    if (ht->key_eq_function((*entry)->key, search_key))
    {
      //printf("hej1");
      return entry;
    }
    else if (ht->key_eq_function((*entry)->next->key, search_key)) //Kan göras entry -> key >= searchKey, för att få det sorterat
    {
      //printf("hej2");
      return entry;
    }
    //printf("hej3");
    return find_previous_entry_for_key_ptr(ht, &(*entry)->next, search_key);
  }
  //return NULL; //Borde inte kunna hända
}

// EQ_VALUES FUNCTION 
bool string_eq(elem_t e1, elem_t e2)
{
  return strcmp(e1.string_value, e2.string_value) == 0;
}

static bool value_string_equiv(elem_t key, elem_t value, void *x)
{
  char **other_int_ptr = x;
  char *other_value = *other_int_ptr;
  return (strcmp(value.string_value, other_value) == 0);
}

static bool value_int_equiv(elem_t key, elem_t value, void *x)
{
  int *other_int_ptr = x;
  int other_value = *other_int_ptr;
  return value.int_value == other_value;
}

bool int_eq(elem_t e1, elem_t e2)
{
  return e1.int_value == e2.int_value;
}

unsigned long extract_int_hash_key(elem_t key)
{
  return key.int_value; 
}


static bool key_equiv(elem_t key, elem_t value_ignored, void *x)
{
  elem_t *other_key_ptr = x;
  elem_t other_key = *other_key_ptr;
  return extract_int_hash_key(key) == extract_int_hash_key(other_key);
}

static void update_int_value(elem_t key, elem_t *value, void *arg)
{
  int *other_int_ptr = arg;
  int other_value = *other_int_ptr;
  (*value).int_value = other_value;
}


int init_suite(void)
{
  return 0;
}

int clean_suite(void)
{
  return 0;
}


void test1_insert(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(extract_int_hash_key, int_eq, string_eq);
  CU_ASSERT(ht->size == 0);
  elem_t result = {.int_value = 0};

  for (int i = 0; i < 15; i++)
  {
    for (int j = 0; j < 5; j++)
    {
      int key_insert = random()%30000; // Finns risk att det blir samma tal, vilket gör att den entryn skrivs över?
      ioopm_hash_table_insert(ht, int_elem(key_insert), string_elem("TESTAR"));
      CU_ASSERT(ioopm_hash_table_lookup(ht, int_elem(key_insert), &result));
    }

  }
  CU_ASSERT(ht->size == (75));
  ioopm_hash_table_destroy(ht);
}


void test2_prev_entry(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(extract_int_hash_key, int_eq, string_eq);

  ioopm_hash_table_insert(ht, int_elem(1), string_elem("TESTAR"));
  ioopm_hash_table_insert(ht, int_elem(18), string_elem("WOWWWW"));
  ioopm_hash_table_insert(ht, int_elem(35), string_elem("LOVEIT"));
  ioopm_hash_table_insert(ht, int_elem(52), string_elem("HAHAH")); 

  entry_t **prev_entry = find_previous_entry_for_key_ptr(ht, &ht->buckets[1], (int_elem(18)));
  entry_t *comp_bucket = ht->buckets[1];
  int comp_elem = (comp_bucket -> next -> key).int_value;

  CU_ASSERT(comp_elem == ((*prev_entry)->key).int_value);
  
  ioopm_hash_table_destroy(ht);
}


void test3_lookup(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(extract_int_hash_key, int_eq, string_eq);
  elem_t result = {.int_value = 0};

  CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, int_elem(5), &result));
  for (int i = 0; i < 15; i++)
  {
    int key_insert = random()%300; // Finns risk att det blir samma tal, vilket gör att den entryn skrivs över?
    ioopm_hash_table_insert(ht, int_elem(key_insert), string_elem("TESTAR")); 
    ioopm_hash_table_insert(ht, int_elem(key_insert+17), string_elem("WOWWWW"));
    ioopm_hash_table_insert(ht, int_elem(key_insert+34), string_elem("LOVEIT"));
    ioopm_hash_table_insert(ht, int_elem(key_insert+51), string_elem("HAHAH"));
    CU_ASSERT(ioopm_hash_table_lookup(ht, int_elem(key_insert), &result)); 
    CU_ASSERT(ioopm_hash_table_lookup(ht, int_elem(key_insert+17), &result));
    CU_ASSERT(ioopm_hash_table_lookup(ht, int_elem(key_insert+34), &result));
    CU_ASSERT(ioopm_hash_table_lookup(ht, int_elem(key_insert+51), &result));
  }

  CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, int_elem(3200), &result));
  ioopm_hash_table_destroy(ht);
}

//testar fallet där ht är tomt vid remove
void test_nytt(void){
  ioopm_hash_table_t *ht = ioopm_hash_table_create(extract_int_hash_key, int_eq, string_eq);
  ioopm_hash_table_remove(ht, int_elem(4));

  CU_ASSERT(ht->size == 0);

  ioopm_hash_table_destroy(ht);

}

//testar remove när key inte finns
void test_nytt2(void){
  ioopm_hash_table_t *ht = ioopm_hash_table_create(extract_int_hash_key, int_eq, string_eq);
  ioopm_hash_table_insert(ht, int_elem(5), string_elem("TESTAR"));
  ioopm_hash_table_remove(ht, int_elem(4));

  elem_t test; 
  bool success = ioopm_hash_table_lookup(ht, int_elem(5), &test);
  CU_ASSERT(success);

  ioopm_hash_table_destroy(ht);
}

//testar remove för branch 2 dvs mitten-element
void test_nytt3(void){
  ioopm_hash_table_t *ht = ioopm_hash_table_create(extract_int_hash_key, int_eq, string_eq);
  ioopm_hash_table_insert(ht, int_elem(1), string_elem("TESTAR"));
  ioopm_hash_table_insert(ht, int_elem(18), string_elem("TESTAR"));
  ioopm_hash_table_insert(ht, int_elem(35), string_elem("TESTAR"));

  ioopm_hash_table_remove(ht, int_elem(18));
  

  elem_t test;
  bool success1 = ioopm_hash_table_lookup(ht, int_elem(18), &test);
  bool success2 = ioopm_hash_table_lookup(ht, int_elem(1), &test);
  bool success3 = ioopm_hash_table_lookup(ht, int_elem(35), &test);

  CU_ASSERT(success2);
  CU_ASSERT(!success1);
  CU_ASSERT(success3);
}

//testar remove för branch 3 dvs last-element
void test_nytt4(void){
  ioopm_hash_table_t *ht = ioopm_hash_table_create(extract_int_hash_key, int_eq, string_eq);
  ioopm_hash_table_insert(ht, int_elem(1), string_elem("TESTAR"));
  ioopm_hash_table_insert(ht, int_elem(18), string_elem("TESTAR"));
  
  ioopm_hash_table_remove(ht, int_elem(1));

  elem_t test;
  bool success1 = ioopm_hash_table_lookup(ht, int_elem(1), &test);

  CU_ASSERT(!success1);
}

void test_nytt5(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(extract_int_hash_key, int_eq, string_eq);

  ioopm_hash_table_insert(ht, int_elem(1), string_elem("TESTAR"));
  ioopm_hash_table_insert(ht, int_elem(18), string_elem("TESTAR"));
  ioopm_hash_table_insert(ht, int_elem(35), string_elem("TESTAR"));
  ioopm_hash_table_insert(ht, int_elem(52), string_elem("TESTAR"));

  ioopm_hash_table_remove(ht, int_elem(1));
  elem_t test;
  bool success1 = ioopm_hash_table_lookup(ht, int_elem(1), &test);
  CU_ASSERT(!success1);
  ioopm_hash_table_destroy(ht);
}




void test4_remove(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(extract_int_hash_key, int_eq, string_eq);
  CU_ASSERT(ht->size == 0);
  elem_t result = {.int_value = 0};

  for (int i = 0; i < 15; i++)
  {
    for (int j = 0; j < 5; j++)
    {
      int key_insert = random()%30000; // Finns risk att det blir samma tal, vilket gör att den entryn skrivs över?
      ioopm_hash_table_insert(ht, int_elem(key_insert), string_elem("TESTAR"));
      CU_ASSERT(ioopm_hash_table_lookup(ht, int_elem(key_insert), &result)); 
      ioopm_hash_table_remove(ht, int_elem(key_insert));
      CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, int_elem(key_insert), &result));
    }
  }
 
  CU_ASSERT(ht->size == 0);
  ioopm_hash_table_destroy(ht);
}


void test5_remove_middle(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(extract_int_hash_key, int_eq, string_eq);
  CU_ASSERT(ht->size == 0);

  for (int i = 0; i < 85; i++)
  {
    ioopm_hash_table_insert(ht, int_elem(i), string_elem("TESTAR"));
  }

  for (int i = 0; i < 15; i++)
  {
    ioopm_hash_table_remove(ht, int_elem(43+i));
  }
  
  CU_ASSERT(ht->size == 70);
  ioopm_hash_table_destroy(ht);
}


void test6_empty_hash_table(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(extract_int_hash_key, int_eq, string_eq);
  CU_ASSERT(ioopm_hash_table_is_empty(ht));

  for (int i = 0; i < 50; i++)
  {
    ioopm_hash_table_insert(ht, int_elem(i), string_elem("TESTAR"));
  }
  CU_ASSERT_FALSE(ioopm_hash_table_is_empty(ht));

  ioopm_hash_table_destroy(ht);
}


void test7_multiple_entry_hash_table(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(extract_int_hash_key, int_eq, string_eq);

  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 0);

  for (int i = 0; i < 50; i++)
  {
    ioopm_hash_table_insert(ht, int_elem(i), string_elem("TESTAR"));
  }
  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 50);

  ioopm_hash_table_destroy(ht);
}


void test8_hash_table_clear_size(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(extract_int_hash_key, int_eq, string_eq);
  elem_t result = {.int_value = 0};

  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 0);

  for (int i = 0; i < 15; i++)
  {
    ioopm_hash_table_insert(ht, int_elem(i), string_elem("TESTAR"));
  }
  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 15);
  ioopm_hash_table_clear(ht);

  
  for (int i = 0; i < primes[ht->index_primes]; i++)
  {
    entry_t *testEntry = ht->buckets[i];
    CU_ASSERT_PTR_NULL(testEntry);
    CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, int_elem(i), &result));
  }
  
  ioopm_hash_table_destroy(ht);
}

void test9_hash_table_keys(void) 
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(extract_int_hash_key, int_eq, string_eq);
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


void test10_hash_table_keys_empty(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(extract_int_hash_key, int_eq, string_eq);
  ioopm_list_t *from_ht = ioopm_hash_table_keys(ht);
  CU_ASSERT_PTR_NULL(from_ht->first);
  ioopm_hash_table_destroy(ht);
  ioopm_linked_list_destroy(from_ht);
}


void test11_hash_table_values(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(extract_int_hash_key, int_eq, string_eq);
  ioopm_list_t *list = ioopm_linked_list_create(int_eq);

  for (int i = 0; i < 3; i++)
  {
    ioopm_hash_table_insert(ht, int_elem(i), string_elem("TESTAR"));
    ioopm_hash_table_insert(ht, int_elem(i+17), string_elem("att"));
    ioopm_hash_table_insert(ht, int_elem(i+34), string_elem("det")); 
  }
  
  ioopm_list_t *from_ht = ioopm_hash_table_values(ht);

  for (int i = 0; i < 3; i++) 
  {
    ioopm_linked_list_prepend(list, string_elem("TESTAR"));
    ioopm_linked_list_prepend(list, string_elem("att"));
    ioopm_linked_list_prepend(list, string_elem("det"));
  }

  for (int i = 0; i < 3; i++)
  {
   CU_ASSERT(int_eq(ioopm_linked_list_get(list, i), ioopm_linked_list_get(from_ht, i)));
  }
  ioopm_hash_table_destroy(ht);
  ioopm_linked_list_destroy(list);
  ioopm_linked_list_destroy(from_ht);
}


void test12_hash_table_values_empty(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(extract_int_hash_key, int_eq, string_eq);
  ioopm_list_t *from_ht = ioopm_hash_table_values(ht);
  CU_ASSERT_PTR_NULL(from_ht->first);
  ioopm_hash_table_destroy(ht);
  ioopm_linked_list_destroy(from_ht);
}


void test13_hash_table_has_key(void) 
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(extract_int_hash_key, int_eq, string_eq);
  CU_ASSERT_FALSE(ioopm_hash_table_has_key(ht, int_elem(100), key_equiv));

  for (int i = 0; i < 17; i++)
  {
    ioopm_hash_table_insert(ht, int_elem(i), string_elem("TESTAR"));
    ioopm_hash_table_insert(ht, int_elem(i+17), string_elem("att"));
    ioopm_hash_table_insert(ht, int_elem(i+34), string_elem("det"));
    ioopm_hash_table_insert(ht, int_elem(i+51), string_elem("fungerar"));
  }

  for(int k = 0; k < 17; k++)
  {
    CU_ASSERT(ioopm_hash_table_has_key(ht, int_elem(k), key_equiv));
    CU_ASSERT(ioopm_hash_table_has_key(ht, int_elem(k+17), key_equiv));
    CU_ASSERT(ioopm_hash_table_has_key(ht, int_elem(k+34), key_equiv));
    CU_ASSERT(ioopm_hash_table_has_key(ht, int_elem(k+51), key_equiv));
  }

  CU_ASSERT_FALSE(ioopm_hash_table_has_key(ht, int_elem(100), key_equiv)); // kollar om den inte finns, skapa ett nytt test för bara det?
  ioopm_hash_table_destroy(ht);
} 


void test14_hash_table_has_value_identical(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(extract_int_hash_key, int_eq, int_eq);
  CU_ASSERT_FALSE(ioopm_hash_table_has_value(ht, string_elem("TESTAR"), value_string_equiv));

  for (int i = 0; i < 15; i++)
  {
    ioopm_hash_table_insert(ht, int_elem(i), string_elem("TESTAR"));
    ioopm_hash_table_insert(ht, int_elem(i+17), string_elem("att"));
    ioopm_hash_table_insert(ht, int_elem(i+34), string_elem("det"));
    ioopm_hash_table_insert(ht, int_elem(i+51), string_elem("fungerar"));
  }

  CU_ASSERT(ioopm_hash_table_has_value(ht, string_elem("TESTAR"), value_string_equiv));
  CU_ASSERT(ioopm_hash_table_has_value(ht, string_elem("att"), value_string_equiv));
  CU_ASSERT(ioopm_hash_table_has_value(ht, string_elem("det"), value_string_equiv));
  CU_ASSERT(ioopm_hash_table_has_value(ht, string_elem("fungerar"), value_string_equiv));

  CU_ASSERT_FALSE(ioopm_hash_table_has_key(ht, string_elem("Nej"), key_equiv)); // kollar om den inte finns, skapa ett nytt test för bara det?
  ioopm_hash_table_destroy(ht);
}



void test15_hash_table_all_value_not(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(extract_int_hash_key, int_eq, int_eq);

  for (int i = 0; i < 15; i++)
  {
    ioopm_hash_table_insert(ht, int_elem(i), string_elem("TESTAR"));
    ioopm_hash_table_insert(ht, int_elem(i+17), string_elem("att"));
    ioopm_hash_table_insert(ht, int_elem(i+34), string_elem("det"));
    ioopm_hash_table_insert(ht, int_elem(i+51), string_elem("fungerar"));
  }

  char *value = "test1";
  CU_ASSERT_FALSE(ioopm_hash_table_all(ht, value_string_equiv, &value));

  ioopm_hash_table_destroy(ht);
}


void test16_hash_table_all_value(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(extract_int_hash_key, int_eq, int_eq);

  for (int i = 0; i < 15; i++)
  {
    ioopm_hash_table_insert(ht, int_elem(i), string_elem("TESTAR"));
  }

  char *value = "TESTAR";
  CU_ASSERT(ioopm_hash_table_all(ht, value_string_equiv, &value));

  ioopm_hash_table_destroy(ht);
}


void test17_hash_table_all_key_not(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(extract_int_hash_key, int_eq, int_eq);

  int value = 1;

  CU_ASSERT(ioopm_hash_table_all(ht, value_int_equiv, &value));

  for (int i = 0; i < 15; i++)
  {
    ioopm_hash_table_insert(ht, int_elem(i), string_elem("TESTAR"));
    ioopm_hash_table_insert(ht, int_elem(i+17), string_elem("att"));
    ioopm_hash_table_insert(ht, int_elem(i+34), string_elem("det"));
    ioopm_hash_table_insert(ht, int_elem(i+51), string_elem("fungerar"));
  }

  CU_ASSERT_FALSE(ioopm_hash_table_all(ht, value_int_equiv, &value));

  ioopm_hash_table_destroy(ht);
}

void test18_hash_table_all_key(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(extract_int_hash_key, int_eq, int_eq);

  ioopm_hash_table_insert(ht, int_elem(1), string_elem("TEST"));

  int value = 1;
  CU_ASSERT(ioopm_hash_table_all(ht, key_equiv, &value));

  ioopm_hash_table_destroy(ht);
}


void test19_hash_table_apply_all(void)
{
   ioopm_hash_table_t *ht = ioopm_hash_table_create(extract_int_hash_key, int_eq, int_eq);

  for (int i = 0; i < 15; i++)
  {
    ioopm_hash_table_insert(ht, int_elem(i), int_elem(i));
  }

  int value = 1;
  CU_ASSERT_FALSE(ioopm_hash_table_all(ht, key_equiv, &value));
  
  ioopm_hash_table_apply_to_all(ht, update_int_value, &value);

  CU_ASSERT(ioopm_hash_table_all(ht, value_int_equiv, &value));

  ioopm_hash_table_destroy(ht);
}

void test20_buckets_grow(void)
{
   ioopm_hash_table_t *ht = ioopm_hash_table_create(extract_int_hash_key, int_eq, string_eq);

  CU_ASSERT(primes[ht->index_primes] == 17);
  ioopm_hash_table_insert(ht, int_elem(18), string_elem("TEST"));
  CU_ASSERT((ht->buckets[1]->key).int_value == 18 );
  for (int i = 0; i < 14; i++)
  {
    ioopm_hash_table_insert(ht, int_elem(i), string_elem("TEST"));
  }
  CU_ASSERT(primes[ht->index_primes] == 31);
  CU_ASSERT((ht->buckets[18]->key).int_value == 18 );

  for (int i = 14; i < 25; i++)
  {
    ioopm_hash_table_insert(ht, int_elem(i), string_elem("TEST"));
  }
  CU_ASSERT(primes[ht->index_primes] == 67);

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
    (NULL == CU_add_test(test_suite1, "test 1", test1_insert)) ||
    (NULL == CU_add_test(test_suite1, "test 2", test2_prev_entry)) || 
    (NULL == CU_add_test(test_suite1, "test 3", test3_lookup)) || 
    (NULL == CU_add_test(test_suite1, "test 4", test4_remove)) ||
    (NULL == CU_add_test(test_suite1, "test 5", test5_remove_middle)) || 
    (NULL == CU_add_test(test_suite1, "test 6", test6_empty_hash_table)) ||
    (NULL == CU_add_test(test_suite1, "test 7", test7_multiple_entry_hash_table)) || 
    (NULL == CU_add_test(test_suite1, "test 8", test8_hash_table_clear_size)) || 
    (NULL == CU_add_test(test_suite1, "test 9", test9_hash_table_keys)) || 
    (NULL == CU_add_test(test_suite1, "test 10", test10_hash_table_keys_empty)) || 
    (NULL == CU_add_test(test_suite1, "test 11", test11_hash_table_values)) || 
    (NULL == CU_add_test(test_suite1, "test 12", test12_hash_table_values_empty)) || 
    (NULL == CU_add_test(test_suite1, "test 13", test13_hash_table_has_key)) || 
    (NULL == CU_add_test(test_suite1, "test 14", test14_hash_table_has_value_identical))|| 
    (NULL == CU_add_test(test_suite1, "test 15", test15_hash_table_all_value_not)) || 
    (NULL == CU_add_test(test_suite1, "test 16", test16_hash_table_all_value)) || 
    (NULL == CU_add_test(test_suite1, "test 17", test17_hash_table_all_key_not)) || 
    (NULL == CU_add_test(test_suite1, "test 18", test18_hash_table_all_key)) || 
    (NULL == CU_add_test(test_suite1, "test 19", test19_hash_table_apply_all))|| 
    (NULL == CU_add_test(test_suite1, "test 20", test20_buckets_grow)) ||
    (NULL == CU_add_test(test_suite1, "test nytt 1", test_nytt)) ||
    (NULL == CU_add_test(test_suite1, "test nytt 2", test_nytt2)) ||
    (NULL == CU_add_test(test_suite1, "test nytt 3", test_nytt3)) ||
    (NULL == CU_add_test(test_suite1, "test nytt 4", test_nytt4)) ||
    (NULL == CU_add_test(test_suite1, "test nytt 5", test_nytt5)) 
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

