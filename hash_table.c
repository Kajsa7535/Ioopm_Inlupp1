#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "hash_table.h"

struct entry // TODO: Var ska den ligga? .c eller .h 
{
  int key;       // holds the key
  char *value;   // holds the value
  entry_t *next; // points to the next entry (possibly NULL)
};

struct hash_table
{
  entry_t buckets[17];
};


static entry_t *find_previous_entry_for_key(entry_t *bucket, int searchKey);
ioopm_hash_table_t *ioopm_hash_table_create();
static entry_t *entry_create(int key, char *value, entry_t *next);
void ioopm_hash_table_insert(ioopm_hash_table_t *ht, int key, char *value);


ioopm_hash_table_t *ioopm_hash_table_create()
{
  /// Allocate space for a ioopm_hash_table_t = 17 pointers to
  /// entry_t's, which will be set to NULL
  ioopm_hash_table_t *result = calloc(1, sizeof(ioopm_hash_table_t));
  return result;
}

static entry_t *find_previous_entry_for_key(entry_t *entry, int searchKey)
{
  /// Saves the first (dummy) entry as first_entry
  entry_t *first_entry = entry;
  entry_t *tmp_entry = entry;

  //(0, null, ->) (1, "hej", null)

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

/// TODO: Skapa test cases som testar ioopm_hash_table_insert, find_previous_entry_for_key och entry_create
void ioopm_hash_table_insert(ioopm_hash_table_t *ht, int key, char *value)
{
  /// Calculate the bucket for this entry
  int bucket = key % 17;
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
      entry->next = entry_create(key, value, next);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main(void)
{
  ioopm_hash_table_t *newHT = ioopm_hash_table_create();
  ioopm_hash_table_insert(newHT, 1, "Hej");
  puts("Hej3");
  //entry_t *entry = find_previous_entry_for_key(&newHT->buckets[1], 1);
  //entry_t *value = entry->value;
  //entry_t *newEntry = entry_create(1, "Hej", NULL);
  entry_t *testHT = &newHT->buckets[1];
  puts("Hej4");
  printf("%d, %s", testHT->next->key, testHT->next->value);


  return 0;

}

// TODO: LEARN DEBUGGING