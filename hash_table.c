#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <CUnit/Basic.h>
#include "hash_table.h"
#include "linked_list.h"
#include "common.h"
#include "iterator.h"

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
    ioopm_link_t *first;
    ioopm_link_t *last;
    size_t size;
};

struct iterator 
{
    ioopm_link_t *current; 
    ioopm_list_t *list;
};

// Creates a hash table with given load factor, index to the global primes-array for number of buckets, hash function, and compare functions for keys and values
static ioopm_hash_table_t *hash_table_create_with_load(double load_factor, int index_primes, size_t size, ioopm_hash_function hash_func, ioopm_eq_function key_eq_func, ioopm_eq_function value_eq_func)
{
  ioopm_hash_table_t *result = calloc(1, sizeof(ioopm_hash_table_t));
  entry_t **buckets = calloc(primes[index_primes], sizeof(entry_t *));
  result -> size = size;
  result -> load_factor = load_factor;
  result -> index_primes = index_primes;
  result -> hash_function = hash_func;
  result -> key_eq_function = key_eq_func;
  result -> value_eq_function = value_eq_func;
  result -> buckets = buckets; // creates an array of arrays with pointers to the first entry in each bucket (or NULL)
  return result;
}

// Creates a hash table with load factor 0.75, index 0 for the global primes-array for number of buckets, size 0, given hash function and compare functions for keys and values
ioopm_hash_table_t *ioopm_hash_table_create(ioopm_hash_function hash_func, ioopm_eq_function key_eq_func, ioopm_eq_function value_eq_func)
{
  return hash_table_create_with_load(0.75 , 0, 0, hash_func, key_eq_func, value_eq_func);
}

// Checks if the hash table should rehash according to its number of buckets, load factor and size
static bool check_bucket_load (ioopm_hash_table_t *ht)
{
  size_t size = ht->size;
  double load_factor = ht->load_factor;
  size_t No_buckets = primes[ht->index_primes];
  return (No_buckets*load_factor >= size); // if return true do not rehash
}

// Frees an entry in the hash table
static void entry_destroy(entry_t *p)
{
  free(p);
}

// Rehashes one given bucket in given hash table
static void rehash_bucket(ioopm_hash_table_t *ht, entry_t *old_bucket)
{
  entry_t *entry = old_bucket;
  while (entry)
  {
    elem_t key = entry->key;
    elem_t value = entry->value;
    ioopm_hash_table_insert(ht, key, value);
    entry_t *del_entry = entry;
    entry = entry -> next;
    entry_destroy(del_entry);
  }
}

// Rehashes all buckets in given hash table
static void hash_table_rehash (ioopm_hash_table_t *ht)
{
  if((ht->index_primes) + 1 <= No_Primes)
  {
  entry_t **del_buckets = ht-> buckets;
  size_t new_no_buckets = primes [(ht ->index_primes) + 1 ];

  ht -> buckets = calloc (new_no_buckets, sizeof(entry_t *));
  ht -> index_primes += 1;
  ht -> size = 0;

  for(int i = 0; i < primes[(ht->index_primes) - 1]; i++)
  {
    rehash_bucket(ht, del_buckets[i]);
  } 
  free(del_buckets);
  }
}

// Finds the pointer to the pointer of an entry that points to an entry with a certain key in a bucket
static entry_t **find_previous_entry_for_key_ptr(ioopm_hash_table_t *ht, entry_t **entry, elem_t search_key) 
{
  while (*entry)
  {
    if (ht->key_eq_function((*entry)->key, search_key))
    {
      return entry;
    }
    else if (ht->key_eq_function((*entry)->next->key, search_key))
    {
      return entry;
    }
    return find_previous_entry_for_key_ptr(ht, &(*entry)->next, search_key);
  }
  return NULL; //should not be able to happen
}

// Creates an entry and allocates space for it
static entry_t *entry_create(elem_t key, elem_t value, entry_t *next)
{
  entry_t *entry = calloc(1, sizeof(entry_t));
  entry->key = key;
  entry->value = value;
  entry->next = next;

  return entry;
}

// Calculates which bucket an entry should be placed in according to the hash function applied to the key modulo the number of buckets
static int calculate_bucket(ioopm_hash_table_t *ht, elem_t key, ioopm_hash_function hash_function)
{
  int key_elem = hash_function(key);
  int bucket = key_elem % primes[ht->index_primes];
  
  return bucket;
}

// Checks if a key exists in ht, if true it stores the value in result
bool ioopm_hash_table_lookup(ioopm_hash_table_t *ht, elem_t key, elem_t *result)
{
  int bucket = calculate_bucket(ht, key, ht->hash_function);
  entry_t *current = ht->buckets[bucket]; 

  while (current)
  {
    if (ht->key_eq_function(current->key, key))
    {
      *result = (current->value);
      return true;
    }
    current = current -> next;
  }
  return false;
}

/*
// Returns the value of an entry with a certain key if it exists in ht, otherwise prints error message
elem_t ioopm_lookup_key(ioopm_hash_table_t *ht, elem_t key) 
{
  elem_t result = {.int_value = 0};
  bool success = ioopm_hash_table_lookup(ht, key, &result);

  if (success == false)
  {
    puts("The input key does not map to anything!\n");
  }
  return result;
}*/

// Inserts an entry in ht, if rehashing is neccessary the hash table will be rehashed first and then the entry will be inserted
void ioopm_hash_table_insert(ioopm_hash_table_t *ht, elem_t key, elem_t value) 
{
  if (check_bucket_load(ht))
  {
    int bucket = calculate_bucket(ht, key, ht->hash_function);
    elem_t ignored = {.string_value = ""};

    if(ht->size == 0)
    {
      ht->buckets[bucket] = entry_create(key, value, NULL);
      ht->size += 1;
    }
    else
    {
      if (ioopm_hash_table_lookup(ht, key, &ignored)) 
      {
        if (ht->key_eq_function((ht->buckets[bucket])->key, key)) 
        {
          (ht->buckets[bucket])->value = value;
        }
        else
        {
          entry_t **prev_entry = find_previous_entry_for_key_ptr(ht, &ht->buckets[bucket], key); 
          (*prev_entry)->next->value = value;
        }
      }
      else
      {
        entry_t **prev_first = &ht->buckets[bucket];
        ht->buckets[bucket] = entry_create(key, value, *prev_first);
        ht->size += 1;
      }
    }
  }
  else
  {
    hash_table_rehash(ht);
    ioopm_hash_table_insert(ht, key, value);
  }
}

// Removes an entry from ht and changes neccessary pointers in the bucket where the entry was
elem_t ioopm_hash_table_remove(ioopm_hash_table_t *ht, elem_t key) 
{
  int bucket = calculate_bucket(ht, key, ht->hash_function);
  //elem_t value = ioopm_lookup_key(ht, key);
  elem_t result;
  bool success = ioopm_hash_table_lookup(ht, key, &result);
  elem_t value = result;

  
  if (!success || ht->size == 0){
    printf("The key does not exist in ht / ht is empty");
  }
  else{
  entry_t **prev_entry = find_previous_entry_for_key_ptr(ht, &ht->buckets[bucket], key);
  entry_t *remove_entry = (*prev_entry)->next;
  
  
  if(ht->key_eq_function((ht->buckets[bucket]->key), key)) // first element
  {
    printf("first");
    entry_t *new_first = ht->buckets[bucket]->next;
    entry_t *del = ht->buckets[bucket];
    entry_destroy(del);
    ht->buckets[bucket] = new_first;
  }
  
  else if(remove_entry ->next == NULL) // last element
  {
    printf("last");
    (*prev_entry)->next = NULL;
    entry_destroy(remove_entry);

  }
  else // element not first nor last
  {
    printf("middle");
    entry_t *next_entry = remove_entry->next;
    (*prev_entry)->next = next_entry;
    entry_destroy(remove_entry);
  }
  ht->size -= 1;
  }

  return value;
}

// Calculates the number of entries in a bucket
static size_t length_of_bucket(entry_t *entry)
{
  size_t acc = 0;
  while (entry)
  {
    acc++;
    entry = entry->next;
  }
  return acc;
}

// Destoys all entries in a bucket
/*
static void bucket_destroy(entry_t *entry)
{
  entry_t *first_entry = entry;
  entry_t *prev_entry;
  
  for (size_t acc = length_of_bucket(entry); acc > 0; acc--)
  {
    entry = first_entry;
    while (entry->next)
    {
      prev_entry = entry;
      entry = entry->next;
    }
    if (acc > 1)
    {
      prev_entry->next = NULL;
    }
    entry_destroy(entry);
  }
}*/

static void bucket_destroy(entry_t *entry)
{
  entry_t *first_entry = entry;
  entry_t *prev_entry;
  
  for (size_t i = length_of_bucket(entry); i > 0; i--)
  {
    entry = first_entry;
    while (entry->next)
    {
      prev_entry = entry;
      entry = entry->next;
    }
    if (i > 1)
    {
      prev_entry->next = NULL;
    }
    entry_destroy(entry);
  }
}

// Destroys an entire hash table, frees its buckets and the hash table
void ioopm_hash_table_destroy(ioopm_hash_table_t *ht) 
{
  ioopm_hash_table_clear(ht);
  free(ht->buckets);
  free(ht);
}

// Returns the size of the hash table
size_t ioopm_hash_table_size(ioopm_hash_table_t *ht)
{
  return ht->size;
}

// Checks if the hash table is empty
bool ioopm_hash_table_is_empty(ioopm_hash_table_t *ht)
{
  return (ht->size == 0);
}

// Removes all elements from a hash table, frees all entries but does not free the buckets
void ioopm_hash_table_clear(ioopm_hash_table_t *ht)
{
  for (int i = 0; i < primes[ht->index_primes]; i++)
  {
    entry_t *bucket = ht->buckets[i];
    bucket_destroy(bucket);
    ht->buckets[i] = NULL;
  }
  ht->size = 0;
}

// Returns a linked list consisting of all the keys in a hash table
ioopm_list_t *ioopm_hash_table_keys(ioopm_hash_table_t *ht) 
{
  ioopm_list_t *result_list = ioopm_linked_list_create(ht->key_eq_function);

  for (int i = 0; i < primes[ht->index_primes]; i++)
  {
    entry_t *entry = ht->buckets[i];
    while (entry)
    {
      elem_t key = (entry)->key;
      ioopm_linked_list_append(result_list, key);
      entry = (entry)->next;
    }
  }
  return result_list;
}

// Returns a linked list consisting of all values in a hash table
ioopm_list_t *ioopm_hash_table_values(ioopm_hash_table_t *ht)
{
  ioopm_list_t *result_list = ioopm_linked_list_create(ht->value_eq_function); 

  for (int i = 0; i < primes[ht->index_primes]; i++)
  {
    entry_t *entry = ht->buckets[i];
    while (entry)
    {
      elem_t value = entry->value;
      ioopm_linked_list_append(result_list, value);
      entry = entry->next;
    }
  }
  return result_list;
}

// Checks if hash table has a certain key
bool ioopm_hash_table_has_key(ioopm_hash_table_t *ht, elem_t key, ioopm_predicate pred) 
{
  return ioopm_hash_table_any(ht, pred, &key);
}

// Checks if hash table has a certain value
bool ioopm_hash_table_has_value(ioopm_hash_table_t *ht, elem_t value, ioopm_predicate pred )
{
  return ioopm_hash_table_any(ht, pred, &value);
}

// Checks if any entry in the hash table fulfills given predicate
bool ioopm_hash_table_any(ioopm_hash_table_t *ht, ioopm_predicate pred, void *arg)
{
  for (int i = 0; i < primes[ht->index_primes]; i++)
  {
    entry_t *entry = ht->buckets[i];
    entry_t *current_entry = (entry);
    for (int k = 0; k < length_of_bucket(entry); k++)
    {
      elem_t key_compare = current_entry->key;
      elem_t value_compare = current_entry->value;
      if (pred(key_compare, value_compare, arg)) 
      {
        return true;
      }
      current_entry = current_entry->next;
    }
  }
  return false;
}

// Checks if all entries in the hash table fulfills given predicate
bool ioopm_hash_table_all(ioopm_hash_table_t *ht, ioopm_predicate pred, void *arg)
{
  for (int i = 0; i < primes[ht->index_primes]; i++)
  {
    entry_t *entry = ht->buckets[i];
    entry_t *current_entry = entry;
    for (int k = 0; k < length_of_bucket(entry); k++)
    {
      elem_t key_compare = current_entry->key;
      elem_t value_compare = current_entry->value;
      if (!(pred(key_compare, value_compare, arg))) 
      {
        return false;
      }
      current_entry = current_entry->next;
    }
  }
  return true;
}

// Applies given function to all entries in hash table
void ioopm_hash_table_apply_to_all(ioopm_hash_table_t *ht, ioopm_apply_function apply_fun, void *arg)
{
  for (int i = 0; i < primes[ht->index_primes]; i++)
  {
    entry_t *entry = ht->buckets[i];
    entry_t *current_entry = entry;
    for (int k = 0; k < length_of_bucket(entry); k++)
    {
      elem_t key = current_entry->key;
      apply_fun(key, &(current_entry->value), arg);
      current_entry = current_entry->next;
    }
  }
}
