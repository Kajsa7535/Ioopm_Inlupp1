#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <CUnit/Basic.h>
#include "hash_table.h"
#include "linked_list.h"
#include "common.h"
#include "iterator.h"

#define No_Buckets 17

// TODO: fixa static and public functions
struct entry 
{
  elem_t key;       // holds the key
  elem_t value;   // holds the value
  entry_t *next; // points to the next entry (possibly NULL)
};

// TODO: fixa så att det finns en value eq func och en key eq func! som användaren får skicka in
struct hash_table
{
  entry_t buckets[No_Buckets]; 
  size_t size;
  ioopm_hash_function hash_function;
  ioopm_eq_function value_eq_function;
  ioopm_eq_function key_eq_function;
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
static int extract_int_hash_key(elem_t key)
{
  return key.int_value; 
}

bool ioopm_eq_function_test(elem_t element1, elem_t element2)
{
  return (extract_int_hash_key(element1) == extract_int_hash_key(element2));
}

ioopm_hash_table_t *ioopm_hash_table_create(ioopm_hash_function hash_func, ioopm_eq_function key_eq_func, ioopm_eq_function value_eq_func)
{
  /// Allocate space for a ioopm_hash_table_t = 17 pointers to
  /// entry_t's, which will be set to NULL
  ioopm_hash_table_t *result = calloc(1, sizeof(ioopm_hash_table_t));
  result -> size = 0;
  result -> hash_function = hash_func;
  result -> key_eq_function = key_eq_func;
  result -> value_eq_function = value_eq_func;
  return result;
}


static entry_t *find_previous_entry_for_key(ioopm_hash_table_t *ht, entry_t *entry, elem_t search_key)
{
  /// Saves the first (dummy) entry as first_entry
  entry_t *first_entry = entry;
  entry_t *tmp_entry = entry;

  while (entry->next != NULL) //TODO: Möjligtvis göra om till sorterad hashtable. ta next->key >= searchKey
  {
    entry = entry->next;
    if (ht->key_eq_function(entry->key,search_key)) //Kan göras entry -> key >= searchKey, för att få det sorterat
    {
      return tmp_entry;
    }
    tmp_entry = entry;
  }
  return first_entry;
}


static entry_t *entry_create(elem_t key, elem_t value, entry_t *next)
{
  entry_t *entry = calloc(1, sizeof(entry_t));
  entry->key = key;
  entry->value = value;
  entry->next = next;

  return entry;
}


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


void ioopm_hash_table_insert(ioopm_hash_table_t *ht, elem_t key, elem_t value)
{
  int bucket = calculate_bucket(key, ht->hash_function);

  /// Search for an existing entry for a key
  entry_t *entry = find_previous_entry_for_key(ht, &ht->buckets[bucket], key);
  entry_t *next = entry->next;

  /// Check if the next entry should be updated or not
  if (next != NULL && ht->key_eq_function(next->key, key))
  {
    next->value = value;
  }
  else
  {
    entry->next = entry_create(key, value, next); // insert the new entry in the beginning of the linked list ?
    ht->size += 1;
  }
}


bool ioopm_hash_table_lookup(ioopm_hash_table_t *ht, elem_t key, elem_t *result)
{
  /// Find the previous entry for key
  int bucket = calculate_bucket(key, ht->hash_function);

  entry_t *tmp = find_previous_entry_for_key(ht, &ht->buckets[bucket], key);
  entry_t *current = tmp->next;

  if (current && ht->key_eq_function((current->key), key))
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


static void entry_destroy(entry_t *p)
{
  free(p);
}


elem_t ioopm_hash_table_remove(ioopm_hash_table_t *ht, elem_t key) //
{
  int bucket = calculate_bucket(key, ht->hash_function);

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


void ioopm_hash_table_destroy(ioopm_hash_table_t *ht) 
{
  ioopm_hash_table_clear(ht);
  free(ht);
  ht = NULL;
}


size_t ioopm_hash_table_size(ioopm_hash_table_t *ht)
{
  return ht->size;
}


bool ioopm_hash_table_is_empty(ioopm_hash_table_t *ht)
{
  return (ht->size == 0);
}


void ioopm_hash_table_clear(ioopm_hash_table_t *ht)
{
  for (int i = 0; i < No_Buckets; i++)
  {
    entry_t *bucket = &ht->buckets[i];
    bucket_destroy(bucket);
  }
  ht->size = 0;
}



ioopm_list_t *ioopm_hash_table_keys(ioopm_hash_table_t *ht) //TODO: Basfall när hashtable är tomt
{
  ioopm_list_t *result_list = ioopm_linked_list_create(ht->key_eq_function);

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


ioopm_list_t *ioopm_hash_table_values(ioopm_hash_table_t *ht)
{
  ioopm_list_t *result_list = ioopm_linked_list_create(ht->value_eq_function); //calloc(ioopm_hash_table_size(ht) + 1, sizeof(char *));

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

/*
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
}*/


// TODO: VI måste ändra så att man inte skickar in key:eqiv osv då det inte kommer fungera om vi har strings som key istället
bool ioopm_hash_table_has_key(ioopm_hash_table_t *ht, elem_t key, ioopm_predicate pred) // TODO: Ännnu ett argument?
{
  return ioopm_hash_table_any(ht, pred, &key);
}


bool ioopm_hash_table_has_value(ioopm_hash_table_t *ht, elem_t value, ioopm_predicate pred )
{

  return ioopm_hash_table_any(ht, pred, &value);
}


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
void process_word(char *word, ioopm_hash_table_t *ht)
{
  // FIXME: Rewrite to match your own interface, error-handling, etc.
    int freq;
    if (ioopm_hash_table_has_key(ht, (elem_t) {.string_value = word}))
    {
        freq = (ioopm_lookup_key(ht, (elem_t) {.void_value = word})).int_value;
    } 
    else
    {
        freq = 0;
    }
    ioopm_hash_table_insert(ht, (elem_t) {.void_value = strdup(word)}, (elem_t) {.int_value = freq + 1});
}


int main(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(extract_int_hash_key, string_eq);
  
  ioopm_hash_table_insert(ht, string_elem("tjo"), int_elem(1));
  ioopm_hash_table_insert(ht, string_elem("hej"), int_elem(2));
  ioopm_hash_table_insert(ht, string_elem("wow"), int_elem(3));
  ioopm_hash_table_insert(ht, string_elem("yey"), int_elem(4));

  elem_t first_val = ioopm_lookup_key(ht, string_elem("hej"));
  printf("first: %d\n", first_val.int_value);

  //process_word("hej", ht);
  ioopm_hash_table_insert(ht, string_elem("hej"), int_elem(3));
  elem_t new_val = ioopm_lookup_key(ht, string_elem("hej"));
  printf("second: %d\n", new_val.int_value);
  //size_t size = ioopm_hash_table_size(ht);
  
  //char **result_array = calloc(size+1, sizeof(char *));

  
  for(; ioopm_iterator_has_next(iter); acc++)
  {
    printf("%d\n", acc);
    result_array[acc] = (ioopm_iterator_current(iter)).string_value; // Seg fault
    ioopm_iterator_next(iter);
  }
  

  //printf("%s\n", ioopm_iterator_current(iter).string_value);

  //result_array[acc] = ioopm_iterator_current(iter).string_value; // Final element


}
*/
