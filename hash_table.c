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

/*
int main(void);
{
  
  return 0;

}*/

ioopm_hash_table_t *ioopm_hash_table_create()
{
  /// Allocate space for a ioopm_hash_table_t = 17 pointers to
  /// entry_t's, which will be set to NULL
  ioopm_hash_table_t *result = calloc(1, sizeof(ioopm_hash_table_t));
  return result;
}

static entry_t *find_previous_entry_for_key(entry_t *bucket, int searchKey)
{
  entry_t *first_entry = bucket;
  entry_t *tmp_entry = bucket;

  do
  {
    bucket = bucket->next;
    if (bucket->key == searchKey)
    {
      return tmp_entry;
    }
    tmp_entry = bucket;
  }
  while(bucket->next != NULL);
  
  return first_entry;
}

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



/*
static entry_t *entry_create(...TODO...)
{
  //TODO...
}*/