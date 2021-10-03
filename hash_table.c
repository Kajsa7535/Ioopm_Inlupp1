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


static ioopm_hash_table_t *hash_table_create_with_load(double load_factor, int index_primes, size_t size, ioopm_hash_function hash_func, ioopm_eq_function key_eq_func, ioopm_eq_function value_eq_func)
{
  /// Allocate space for a ioopm_hash_table_t = 17 pointers to
  /// entry_t's, which will be set to NULL
  ioopm_hash_table_t *result = calloc(1, sizeof(ioopm_hash_table_t));
  entry_t **buckets = calloc(primes[index_primes], sizeof(entry_t *));
  result -> size = size;
  result -> load_factor = load_factor;
  result -> index_primes = index_primes;
  result -> hash_function = hash_func;
  result -> key_eq_function = key_eq_func;
  result -> value_eq_function = value_eq_func;
  result -> buckets = buckets; //skapar en array (antal buckets) av arrayer till de första entryn (eller null) i varje bucket
  return result;
}


ioopm_hash_table_t *ioopm_hash_table_create(ioopm_hash_function hash_func, ioopm_eq_function key_eq_func, ioopm_eq_function value_eq_func)
{
  return hash_table_create_with_load(0.75, 0, 0, hash_func, key_eq_func, value_eq_func);
}


static bool check_bucket_load (ioopm_hash_table_t *ht)
{
  size_t size = ht->size;
  double load_factor = ht->load_factor;
  size_t No_buckets = primes[ht->index_primes];
  return (No_buckets*load_factor >= size); // if return true do not rehash
}

static void rehash_entry (ioopm_hash_table_t *ht_new, entry_t *entry)
{
  elem_t key = entry->key;
  elem_t value = entry->value;
  ioopm_hash_table_insert(ht_new, key, value);
}

//TODO: check_index_primes som kollar att index är valid

static void hash_table_rehash (ioopm_hash_table_t *ht_old)
{
  if((ht_old->index_primes) + 1 <= No_Primes)
  {
  ioopm_hash_table_t *ht_new = hash_table_create_with_load(ht_old->load_factor, (ht_old->index_primes)+1, ht_old->size, ht_old->hash_function, ht_old->key_eq_function, ht_old->value_eq_function);
  
  for(int i = 0; i < primes[ht_old->index_primes]; i++)
  {
    entry_t *entry = ht_old -> buckets[i]; 
    while (entry)
    {
      rehash_entry(ht_new, entry);
      entry = entry -> next;
    }
  }
  // pekar om old ht till nya???
  ioopm_hash_table_t *ptr_old_ht = ht_old;
  ht_old = ht_new; 
  // free gamla ht
  ioopm_hash_table_destroy(ptr_old_ht);
  }
}


static entry_t **find_previous_entry_for_key_ptr(ioopm_hash_table_t *ht, entry_t **entry, elem_t search_key)
{
  entry_t **tmp_entry = entry;

  while ((*entry)->next != NULL) //TODO: Möjligtvis göra om till sorterad hashtable. ta next->key >= searchKey
  {
    *entry = (*entry)->next;
    if (ht->key_eq_function((*entry)->key,search_key)) //Kan göras entry -> key >= searchKey, för att få det sorterat
    {
      return tmp_entry;
    }
    tmp_entry = entry;
  }
  return entry;
}


static entry_t *entry_create(elem_t key, elem_t value, entry_t *next)
{
  entry_t *entry = calloc(1, sizeof(entry_t));
  entry->key = key;
  entry->value = value;
  entry->next = next;

  return entry;
}


static int calculate_bucket(ioopm_hash_table_t *ht, elem_t key, ioopm_hash_function hash_function)
{
  int key_elem = hash_function(key);
  int bucket = key_elem % primes[ht->index_primes];
  /// Calculate the bucket for this entry
  if (key_elem < 0) // checks if key is negative
  {
    bucket = primes[ht->index_primes] - abs(bucket);
  }
  return bucket;
}


void ioopm_hash_table_insert(ioopm_hash_table_t *ht, elem_t key, elem_t value)
{
  if (check_bucket_load(ht))
  {
    int bucket = calculate_bucket(ht, key, ht->hash_function);
    elem_t ignored = {.string_value = ""};

    /// Search for an existing entry for a key
    if(ht->size == 0)
    {
      ht->buckets[bucket] = entry_create(key, value, NULL);
      ht->size += 1;
    }
    else
    {
      if (ioopm_hash_table_lookup(ht, key, &ignored))
      {
        entry_t **prev_entry = find_previous_entry_for_key_ptr(ht, &ht->buckets[bucket], key);
        (*prev_entry)->next->value = value;
      }
      else
      {
        entry_t *prev_first = ht->buckets[bucket];
        ht->buckets[bucket] = entry_create(key, value, prev_first);
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


bool ioopm_hash_table_lookup(ioopm_hash_table_t *ht, elem_t key, elem_t *result)
{
  /// Find the previous entry for key
  int bucket = calculate_bucket(ht, key, ht->hash_function);

  entry_t **tmp = find_previous_entry_for_key_ptr(ht, &ht->buckets[bucket], key);
  entry_t *current = (*tmp)->next;

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
  int bucket = calculate_bucket(ht, key, ht->hash_function);

  elem_t value; // TODO: Not initialized yet
  if (ioopm_hash_table_lookup(ht, key, &value))
  {
    entry_t **prev_entry = find_previous_entry_for_key_ptr(ht, &ht->buckets[bucket], key);
    entry_t *remove_entry = (*prev_entry)->next;
    elem_t value_of_key = ioopm_lookup_key(ht, key);

    // Case: Entry furthest to the right
    if (remove_entry->next == NULL)
    {
      (*prev_entry)->next = NULL;
    }
    // Case: Entry in the middle of two other entries
    else
    {
      entry_t *next_entry = remove_entry->next;
      (*prev_entry)->next = next_entry;
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
  for (int i = 0; i < primes[ht->index_primes]; i++)
  {
    entry_t **bucket = &ht->buckets[i];
    bucket_destroy(*bucket); //TODO: kanske dangling pointer???
  }
  ht->size = 0;
}



ioopm_list_t *ioopm_hash_table_keys(ioopm_hash_table_t *ht) //TODO: Basfall när hashtable är tomt
{
  ioopm_list_t *result_list = ioopm_linked_list_create(ht->key_eq_function);

  for (int i = 0; i < primes[ht->index_primes]; i++)
  {
    entry_t **entry = &ht->buckets[i];
    while ((*entry)->next)
    {
      *entry = (*entry)->next;
      elem_t key = (*entry)->key;
      ioopm_linked_list_append(result_list, key);
    }
  }
  return result_list;
}


ioopm_list_t *ioopm_hash_table_values(ioopm_hash_table_t *ht)
{
  ioopm_list_t *result_list = ioopm_linked_list_create(ht->value_eq_function); //calloc(ioopm_hash_table_size(ht) + 1, sizeof(char *));

  for (int i = 0; i < primes[ht->index_primes]; i++)
  {
    entry_t **entry = &ht->buckets[i];
    while ((*entry)->next)
    {
      *entry = (*entry)->next;
      elem_t value = (*entry)->value;
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
  for (int i = 0; i < primes[ht->index_primes]; i++)
  {
    entry_t **entry = &ht->buckets[i];
    entry_t *current_entry = (*entry);
    for (int k = 0; k < length_of_bucket(*entry); k++)
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
  for (int i = 0; i < primes[ht->index_primes]; i++)
  {
    entry_t **entry = &ht->buckets[i];
    entry_t *current_entry = *entry;
    for (int k = 0; k < length_of_bucket(*entry); k++)
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
  for (int i = 0; i < primes[ht->index_primes]; i++)
  {
    entry_t **entry = &ht->buckets[i];
    entry_t *current_entry = *entry;
    for (int k = 0; k < length_of_bucket(*entry); k++)
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


bool string_eq(elem_t e1, elem_t e2)
{
  return strcmp(e1.string_value, e2.string_value) == 0;
}

bool int_eq(elem_t e1, elem_t e2)
{
  return e1.int_value == e2.int_value;
}

static bool key_equiv(elem_t key, elem_t value_ignored, void *x)
{
  elem_t *other_key_ptr = x;
  elem_t other_key = *other_key_ptr;
  return extract_int_hash_key(key) == extract_int_hash_key(other_key);
}

int main(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(extract_int_hash_key, int_eq, string_eq);

  //entry_t **test = find_previous_entry_for_key_ptr(ht, &ht->buckets[0], int_elem(17));
  //printf("%d\n", test);
  
  ioopm_hash_table_insert(ht, int_elem(1), string_elem("tjo"));
  
  if(ioopm_hash_table_has_key(ht, int_elem(1), key_equiv))
  {
    puts("yay!\n");
  }
  else
  {
    puts("dum dum dum.... \n");
  }
  //TODOioopm_hash_table_insert(ht, int_elem(2), string_elem("hej"));
  //ioopm_hash_table_insert(ht, int_elem(3), string_elem("wow"));
  //ioopm_hash_table_insert(ht, int_elem(4), string_elem("yey"));


  


  //char **result_array = calloc(size+1, sizeof(char *));

  

  //printf("%s\n", ioopm_iterator_current(iter).string_value);

  //result_array[acc] = ioopm_iterator_current(iter).string_value; // Final element


}*/

