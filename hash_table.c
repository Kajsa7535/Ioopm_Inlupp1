#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <CUnit/Basic.h>
#include "hash_table.h"
#include "linked_list.h"

#define No_Buckets 17

struct entry 
{
  int key;       // holds the key
  char *value;   // holds the value
  entry_t *next; // points to the next entry (possibly NULL)
};

struct hash_table
{
  entry_t buckets[17];
  size_t size;
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


ioopm_hash_table_t *ioopm_hash_table_create()
{
  /// Allocate space for a ioopm_hash_table_t = 17 pointers to
  /// entry_t's, which will be set to NULL
  ioopm_hash_table_t *result = calloc(1, sizeof(ioopm_hash_table_t));
  result->size = 0;
  return result;
}

//(0,null, ->) (1,hej, ->) (2, hej, null)

static entry_t *find_previous_entry_for_key(entry_t *entry, int searchKey)
{
  /// Saves the first (dummy) entry as first_entry
  entry_t *first_entry = entry;
  entry_t *tmp_entry = entry;

  while (entry->next != NULL) //TODO: Möjligtvis göra om till sorterad hashtable. ta next->key >= searchKey
  {
    entry = entry->next;
    if (entry->key == searchKey) //Kan göras entry -> key >= searchKey, för att få det sorterat
    {
      return tmp_entry;
    }
    tmp_entry = entry;
  }

  return first_entry;
}

static entry_t *entry_create(int key, char *value, entry_t *next)
{
  entry_t *entry = calloc(1, sizeof(entry_t));
  entry->key = key;
  entry->value = value;
  entry->next = next;

  return entry;
}

static int calculate_bucket(int key)
{
  int bucket = key % No_Buckets;
  /// Calculate the bucket for this entry
  if (key < 0) // checks if key is negative
  {
    bucket = No_Buckets - abs(bucket);
  }
  return bucket;
}

void ioopm_hash_table_insert(ioopm_hash_table_t *ht, int key, char *value)
{
  int bucket = calculate_bucket(key);

  /// Search for an existing entry for a key
  entry_t *entry = find_previous_entry_for_key(&ht->buckets[bucket], key);
  entry_t *next = entry->next;

  /// Check if the next entry should be updated or not
  if (next != NULL && next->key == key)
  {
    next->value = value;
  }
  else
  {
    entry->next = entry_create(key, value, next); // insert the new entry in the beginning of the linked list ?
    ht->size += 1;
  }
}

bool ioopm_hash_table_lookup(ioopm_hash_table_t *ht, int key, char **result)
{
  /// Find the previous entry for key
  int bucket = calculate_bucket(key);
  entry_t *tmp = find_previous_entry_for_key(&ht->buckets[bucket], key);
  entry_t *next = tmp->next;

  if (next && next->key == key)
  {
    /// If entry was found, return true.
    *result = (next->value);
    return true;
  }
  else
  {
    /// ... else return false
    return false;
  }
}

//TODO: Fråga på labben om man får dela upp den i två funktioner? Och byta namn på den man fick? Den gjorde inte som den skulle?
char *ioopm_lookup_key(ioopm_hash_table_t *ht, int key) //TODO: Gör fallet om man hittar keyn
{
  char *result = NULL;
  bool success = ioopm_hash_table_lookup(ht, key, &result);
  /// TODO: Är det nödvändigt att printa om entry finns?

  if (success == false)
  {
    // !success => result == NULL
    printf("key %d does not map to anything!\n", key);
  }
  return result;
}

static void entry_destroy(entry_t *p)
{
  free(p);
}

char *ioopm_hash_table_remove(ioopm_hash_table_t *ht, int key) //
{
  int bucket = calculate_bucket(key);

  char *value = NULL;
  if (ioopm_hash_table_lookup(ht, key, &value))
  {
    entry_t *prev_entry = find_previous_entry_for_key(&ht->buckets[bucket], key);
    entry_t *remove_entry = prev_entry->next;
    char *value_of_key = ioopm_lookup_key(ht, key);

    //(0,null,->) (1 "hej", ->)
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
    printf("key %d does not map to anything!\n", key);
    return NULL; // TODO: Checka om det är ok att returnera NULL
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

/* // TODO: REDOVISA Svansrekursion??????? 
// acc argument always has to be zero initially
static int length_of_bucket_recursive(entry_t *entry, int acc)
{
  if (entry->next == NULL)
  {
    return acc;
  }
  return length_of_bucket_recursive(entry->next, (acc + 1));
}*/

//(0,null,null)

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

void ioopm_hash_table_destroy(ioopm_hash_table_t *ht) // TODO: Skriv om till rekursiv formel! mål!
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
  ioopm_list_t *result_list = ioopm_linked_list_create();

  for (int i = 0; i < No_Buckets; i++)
  {
    entry_t *entry = &ht->buckets[i];
    while (entry->next)
    {
      entry = entry->next;
      int key = entry->key;
      ioopm_linked_list_append(result_list, key);
    }
  }
  return result_list;
}

char **ioopm_hash_table_values(ioopm_hash_table_t *ht)
{
  char **result_array = calloc(ioopm_hash_table_size(ht) + 1, sizeof(char *));
  int acc = 0;

  for (int i = 0; i < No_Buckets; i++)
  {
    entry_t *entry = &ht->buckets[i];
    while (entry->next)
    {
      entry = entry->next;
      char *value = entry->value;
      result_array[acc] = value;
      acc++;
    }
  }
  result_array[acc] = NULL;
  return result_array;
}


static bool value_equiv(int key_ignored, char *value, void *x)
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


bool ioopm_hash_table_has_key(ioopm_hash_table_t *ht, int key)
{
  return ioopm_hash_table_any(ht, key_equiv, &key);
}

bool ioopm_hash_table_has_value(ioopm_hash_table_t *ht, char *value)
{
  return ioopm_hash_table_any(ht, value_equiv, &value);
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
      int key_compare = current_entry->key;
      char *value_compare = current_entry->value;
      if (pred(key_compare, value_compare, arg)) // GDB REDOVISNING!!!! det som blev fel!!!! ---->>>>> &&&&&&& <<<-----pred(key_compare, value_compare, &arg)
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
      int key_compare = current_entry->key;
      char *value_compare = current_entry->value;
      if (!(pred(key_compare, value_compare, arg))) 
      {
        return false;
      }
    }
  }
  return true;
}

static void update_value(int key, char **value, void *arg) // används bara vid testning av ioopm_hash_table_apply_to_all
{
  char **other_char_ptr = arg;
  char *other_value = *other_char_ptr;
  *value = other_value;
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
      int key = current_entry->key;
      apply_fun(key, &(current_entry->value), arg);
    }
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
int main(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  ioopm_hash_table_insert(ht, 3, "test1");
  ioopm_hash_table_insert(ht, 21, "ioopm");
  ioopm_hash_table_insert(ht, 2, "test2");
  bool result = ioopm_hash_table_has_key(ht, 3);
  printf("result = (%d)\n", result);
  ioopm_hash_table_destroy(ht);
}
// TODO: LEARN DEBUGGING

*/
