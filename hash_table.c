#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <CUnit/Basic.h>
#include "hash_table.h"

/*
struct entry // TODO: Var ska den ligga? .c eller .h 
{
  int key;       // holds the key
  char *value;   // holds the value
  entry_t *next; // points to the next entry (possibly NULL)
};

struct hash_table
{
  entry_t buckets[17];
};*/

void ioopm_hash_table_insert(ioopm_hash_table_t *ht, int key, char *value);
char *lookup_check(ioopm_hash_table_t *ht, int key);
ioopm_hash_table_t *ioopm_hash_table_create();
static entry_t *find_previous_entry_for_key(entry_t *bucket, int searchKey);
static entry_t *entry_create(int key, char *value, entry_t *next);
bool ioopm_hash_table_lookup(ioopm_hash_table_t *ht, int key, char **result);

ioopm_hash_table_t *ioopm_hash_table_create()
{
  /// Allocate space for a ioopm_hash_table_t = 17 pointers to
  /// entry_t's, which will be set to NULL
  ioopm_hash_table_t *result = calloc(1, sizeof(ioopm_hash_table_t));
  return result;
}

static entry_t *find_previous_entry_for_key(entry_t *entry, int searchKey) // TODO: Ta reda på vad static innebär samt hur tester skrivs med de
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

static entry_t *entry_create(int key, char *value, entry_t *next)
{
  entry_t *entry = calloc(1, sizeof(entry_t));
  entry->key = key;
  entry->value = value;
  entry->next = next;

  return entry;
}

/// TODO: Skapa test cases som testar ioopm_hash_table_insert. 
void ioopm_hash_table_insert(ioopm_hash_table_t *ht, int key, char *value)
{
  int bucket;
  /// Calculate the bucket for this entry
  if (key < 0) // checks if key is negative
  {
    bucket = 17 - abs(key % 17);
  }
  else 
  {
    bucket = key % 17; 
  }
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
    }
}

bool ioopm_hash_table_lookup(ioopm_hash_table_t *ht, int key, char **result)
{
  /// Find the previous entry for key
  entry_t *tmp = find_previous_entry_for_key(&ht->buckets[key % 17], key);
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
char *lookup_check(ioopm_hash_table_t *ht, int key) 
{

char *result = NULL; //TODO: Reservera minne för en string?
bool success = ioopm_hash_table_lookup(ht, key, &result);
/// TODO: Är det nödvändigt att printa om entry finns?
if (success == false)
  {
    // !success => result == NULL
    printf("key %d does not map to anything!\n", key);
  }
  return result;
}

void entry_destroy(entry_t *p)
{
  free(p);
}

char *ioopm_hash_table_remove(ioopm_hash_table_t *ht, int key)
{
  char *value = NULL;
  if(ioopm_hash_table_lookup(ht, key, &value)) /// TODO: Checka vad som händer när NULL är result/value-argument
  {
    entry_t *prev_entry = find_previous_entry_for_key(&ht->buckets[key % 17], key); 
    entry_t *remove_entry = prev_entry->next;
    char *value_of_key = lookup_check(ht, key);
    
    //(0,null,->) (1 "hej", ->) 
    // Case: Entry furthest to the right
    if(remove_entry->next == NULL)
    {
      prev_entry->next = NULL; // TODO: Ändrar detta remove_entry till NULL eller bara pekaren till NULL?
    }
    // Case: Entry in the middle of two other entries
    else
    {
      entry_t *next_entry = remove_entry->next;
      prev_entry->next = next_entry; //TODO: Detta var fel, stod remove_entry=... (ta bort denna rad!)
    }

    //Return the value of removed entry and free the space in the heap
    entry_destroy(remove_entry); // TODO: Funkar entry_destroy som den ska?
    return value_of_key;
  }
  else
  {
    printf("key %d does not map to anything!\n", key);
    return NULL; // TODO: Checka om det är ok att returnera NULL
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*int main(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  ioopm_hash_table_insert(ht, 2, "hej");
  lookup_check(ht, 2);
  char *str = ioopm_hash_table_remove(ht, 2);
  printf("%s",str);
  return 0;
}*/

// TODO: LEARN DEBUGGING
