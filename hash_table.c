#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <CUnit/Basic.h>
#include "hash_table.h"
#include "linked_list.h"
#include "common.h"

#define No_Buckets 17

// TODO: Fix gcov in make file

// TODO: fixa static and public functions
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


//HASH FUNCTION!!!
// CHECK
static int extract_int_hash_key(elem_t key)
{
  return key.int_value; 
}

bool ioopm_eq_function_test(elem_t element1, elem_t element2)
{
  return (extract_int_hash_key(element1) == extract_int_hash_key(element2));
}

//CHECK
ioopm_hash_table_t *ioopm_hash_table_create(ioopm_hash_function hash_func, ioopm_eq_function value_eq)
{
  /// Allocate space for a ioopm_hash_table_t = 17 pointers to
  /// entry_t's, which will be set to NULL
  ioopm_hash_table_t *result = calloc(1, sizeof(ioopm_hash_table_t));
  result -> size = 0;
  result -> hash_function = hash_func;
  result -> values_eq_function = value_eq;
  return result;
}

// CHECK
static entry_t *find_previous_entry_for_key(ioopm_hash_table_t *ht, entry_t *entry, elem_t search_key)
{
  /// Saves the first (dummy) entry as first_entry
  entry_t *first_entry = entry;
  entry_t *tmp_entry = entry;
  ioopm_hash_function hash_func = ht->hash_function;

  while (entry->next != NULL) //TODO: Möjligtvis göra om till sorterad hashtable. ta next->key >= searchKey
  {
    entry = entry->next;
    if (hash_func(entry->key) == hash_func(search_key)) //Kan göras entry -> key >= searchKey, för att få det sorterat
    {
      return tmp_entry;
    }
    tmp_entry = entry;
  }
  return first_entry;
}


//CHECK
static entry_t *entry_create(elem_t key, elem_t value, entry_t *next)
{
  entry_t *entry = calloc(1, sizeof(entry_t));
  entry->key = key;
  entry->value = value;
  entry->next = next;

  return entry;
}

// CHECK
static int calculate_bucket(elem_t key, ioopm_hash_function hash_function)
{
  int key_elem = hash_function(key);
  int bucket = key_elem % No_Buckets;
  /// Calculate the bucket for this entry
  if (key_elem < 0) // checks if key is negative
  {
    bucket = No_Buckets - abs(bucket);
  }
  return bucket;
}

// CHECK
void ioopm_hash_table_insert(ioopm_hash_table_t *ht, elem_t key, elem_t value)
{
  ioopm_hash_function hash_func = ht->hash_function;
  int bucket = calculate_bucket(key, hash_func);

  /// Search for an existing entry for a key
  entry_t *entry = find_previous_entry_for_key(ht, &ht->buckets[bucket], key);
  entry_t *next = entry->next;

  /// Check if the next entry should be updated or not
  if (next != NULL && hash_func(next->key) == hash_func(key))
  {
    next->value = value;
  }
  else
  {
    entry->next = entry_create(key, value, next); // insert the new entry in the beginning of the linked list ?
    ht->size += 1;
  }
}

//CHECK
bool ioopm_hash_table_lookup(ioopm_hash_table_t *ht, elem_t key, elem_t *result)
{
  /// Find the previous entry for key
  ioopm_hash_function hash_func = ht->hash_function;
  int bucket = calculate_bucket(key, hash_func);

  entry_t *tmp = find_previous_entry_for_key(ht, &ht->buckets[bucket], key);
  entry_t *current = tmp->next;

  if (current && hash_func(current->key) == hash_func(key))
  {
    /// If entry was found, return true.
    *result = (current->value); // Lägger in the värde som fanns i key i 'result'
    return true;
  }
  else
  {
    /// ... else return false
    return false;
  }
}

//CHECK
//TODO: Fråga på labben om man får dela upp den i två funktioner? Och byta namn på den man fick? Den gjorde inte som den skulle?
elem_t ioopm_lookup_key(ioopm_hash_table_t *ht, elem_t key) //TODO: Gör fallet om man hittar keyn
{
  elem_t result; // TODO: Not initialized yet
  bool success = ioopm_hash_table_lookup(ht, key, &result);
  /// TODO: Är det nödvändigt att printa om entry finns?

  if (success == false)
  {
    // !success => result == NULL
    puts("The input key does not map to anything!\n");
  }
  return result;
}

// CHECK
static void entry_destroy(entry_t *p)
{
  free(p);
}

// CHECK
elem_t ioopm_hash_table_remove(ioopm_hash_table_t *ht, elem_t key) //
{
  ioopm_hash_function hash_func = ht->hash_function;
  int bucket = calculate_bucket(key, hash_func);

  elem_t value; // TODO: Not initialized yet
  if (ioopm_hash_table_lookup(ht, key, &value))
  {
    entry_t *prev_entry = find_previous_entry_for_key(ht, &ht->buckets[bucket], key);
    entry_t *remove_entry = prev_entry->next;
    elem_t value_of_key = ioopm_lookup_key(ht, key);

    // Case: Entry furthest to the right
    if (remove_entry->next == NULL)
    {
      prev_entry->next = NULL;
    }
    // Case: Entry in the middle of two other entries
    else
    {
      entry_t *next_entry = remove_entry->next;
      prev_entry->next = next_entry;
    }

    //Return the value of removed entry, decrease the size by 1 and free the space in the heap
    ht->size -= 1;
    entry_destroy(remove_entry);
    return value_of_key;
  }
  else
  {
    puts("The input key does not map to anything!\n");
    return void_elem(NULL); // TODO: Checka om det är ok att returnera NULL
  }
}

//CHECK
static size_t length_of_bucket(entry_t *entry)
{
  size_t acc = 0;
  while (entry->next != NULL)
  {
    entry = entry->next;
    acc++;
  }
  return acc;
}

// CHECK
static void bucket_destroy(entry_t *entry)
{
  entry_t *dummy_entry = entry;
  entry_t *prev_entry;
  size_t acc = length_of_bucket(entry);
  // Case: Last entry
  for (; acc > 0; acc--)
  {
    entry = dummy_entry;
    while (entry->next != NULL)
    {
      prev_entry = entry;
      entry = entry->next;
    }
    prev_entry->next = NULL;
    entry_destroy(entry);
  }
}

// CHECK
void ioopm_hash_table_destroy(ioopm_hash_table_t *ht) 
{
  ioopm_hash_table_clear(ht);
  free(ht);
  ht = NULL;
}

// CHECK
size_t ioopm_hash_table_size(ioopm_hash_table_t *ht)
{
  return ht->size;
}

// CHECK
bool ioopm_hash_table_is_empty(ioopm_hash_table_t *ht)
{
  return (ht->size == 0);
}

// CHECK
void ioopm_hash_table_clear(ioopm_hash_table_t *ht)
{
  for (int i = 0; i < No_Buckets; i++)
  {
    entry_t *bucket = &ht->buckets[i];
    bucket_destroy(bucket);
  }
  ht->size = 0;
}


// CHECK
ioopm_list_t *ioopm_hash_table_keys(ioopm_hash_table_t *ht) //TODO: Basfall när hashtable är tomt
{
  ioopm_list_t *result_list = ioopm_linked_list_create(ht->values_eq_function);

  for (int i = 0; i < No_Buckets; i++)
  {
    entry_t *entry = &ht->buckets[i];
    while (entry->next)
    {
      entry = entry->next;
      elem_t key = entry->key;
      ioopm_linked_list_append(result_list, key);
    }
  }
  return result_list;
}

//CHECK
ioopm_list_t *ioopm_hash_table_values(ioopm_hash_table_t *ht)
{
  ioopm_list_t *result_list = ioopm_linked_list_create(ht->values_eq_function); //calloc(ioopm_hash_table_size(ht) + 1, sizeof(char *));

  for (int i = 0; i < No_Buckets; i++)
  {
    entry_t *entry = &ht->buckets[i];
    while (entry->next)
    {
      entry = entry->next;
      elem_t value = entry->value;
      ioopm_linked_list_append(result_list, value);
    }
  }
  //result_array[acc] = NULL; // KANSKE behövs för basfallet när det inte finns några entries
  return result_list;
}

//TODO: fix
static bool value_equiv(elem_t key_ignored, elem_t value, void *x)
{
  elem_t *other_char_ptr = x;
  elem_t other_value = *other_char_ptr;
  return extract_int_hash_key(value) == extract_int_hash_key(other_value);
}

//TODO: fix
static bool key_equiv(elem_t key, elem_t value_ignored, void *x)
{
  elem_t *other_key_ptr = x;
  elem_t other_key = *other_key_ptr;
  return extract_int_hash_key(key) == extract_int_hash_key(other_key);
}

// CHECK
bool ioopm_hash_table_has_key(ioopm_hash_table_t *ht, elem_t key)
{
  return ioopm_hash_table_any(ht, key_equiv, &key);
}

// CHECK
bool ioopm_hash_table_has_value(ioopm_hash_table_t *ht, elem_t value)
{
  return ioopm_hash_table_any(ht, value_equiv, &value);
}

// CHECK
bool ioopm_hash_table_any(ioopm_hash_table_t *ht, ioopm_predicate pred, void *arg)
{
  for (int i = 0; i < No_Buckets; i++)
  {
    entry_t *entry = &ht->buckets[i];
    entry_t *current_entry = entry;
    for (int k = 0; k < length_of_bucket(entry); k++)
    {
      current_entry = current_entry->next;
      elem_t key_compare = current_entry->key;
      elem_t value_compare = current_entry->value;
      if (pred(key_compare, value_compare, arg)) 
      {
        return true;
      }
    }
  }
  return false;
}

// CHECK
bool ioopm_hash_table_all(ioopm_hash_table_t *ht, ioopm_predicate pred, void *arg)
{
  for (int i = 0; i < No_Buckets; i++)
  {
    entry_t *entry = &ht->buckets[i];
    entry_t *current_entry = entry;
    for (int k = 0; k < length_of_bucket(entry); k++)
    {
      current_entry = current_entry->next;
      elem_t key_compare = current_entry->key;
      elem_t value_compare = current_entry->value;
      if (!(pred(key_compare, value_compare, arg))) 
      {
        return false;
      }
    }
  }
  return true;
}

//CHECK
void ioopm_hash_table_apply_to_all(ioopm_hash_table_t *ht, ioopm_apply_function apply_fun, void *arg)
{
  for (int i = 0; i < No_Buckets; i++)
  {
    entry_t *entry = &ht->buckets[i];
    entry_t *current_entry = entry;
    for (int k = 0; k < length_of_bucket(entry); k++)
    {
      current_entry = current_entry->next;
      elem_t key = current_entry->key;
      apply_fun(key, &(current_entry->value), arg);
    }
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
bool string_eq(elem_t e1, elem_t e2)
{
  return strcmp(e1.string_value, e2.string_value) == 0;
}


int main(void)
{

  ioopm_hash_table_t *ht = ioopm_hash_table_create(extract_int_hash_key, string_eq);
  elem_t *result = NULL;
  bool b_value = ioopm_hash_table_lookup(ht, int_elem(1), result);
  printf("%d\n", b_value);
  int key_insert = 100; // Finns risk att det blir samma tal, vilket gör att den entryn skrivs över?
  int pls = 100 %17;
  printf("%d\n", pls);
  //printf("%d\n", key_insert);
  ioopm_hash_table_insert(ht, int_elem(key_insert), string_elem("TESTAR"));
  ioopm_hash_table_insert(ht, int_elem(key_insert+17), string_elem("WOWWWW"));
  ioopm_hash_table_insert(ht, int_elem(key_insert+34), string_elem("LOVEIT"));
  ioopm_hash_table_insert(ht, int_elem(key_insert+51), string_elem("HAHAH"));


  //bool b_value2 = ioopm_hash_table_lookup(ht, int_elem(key_insert), result);

  //printf("%d\n", b_value2);
  int testa = 100;
  entry_t *testEntry = &ht->buckets[15]; 
  elem_t value1 = testEntry->next->value;
  bool value = string_eq(value1, string_elem("TESTAR"));
  

  printf("%d\n", value);

 
}*/

