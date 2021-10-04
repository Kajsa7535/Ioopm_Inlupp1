#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "hash_table.h"
#include "linked_list.h"
#include "common.h"
#include "iterator.h"

#define Delimiters "+-#@()[]{}.,:;!? \t\n\r"

// Compares two string pointers, returns 0 if same?
static int cmpstringp(const void *p1, const void *p2)
{
  return strcmp(*(char *const *)p1, *(char *const *)p2);
}

// Sorts list with string-compare function
void sort_keys(char *keys[], size_t no_keys)
{
  qsort(keys, no_keys, sizeof(char *), cmpstringp); // Seg fault
}

// Inserts word into hash table with value 1 if it does not exist, otherwise adds 1 to value
void process_word(char *word, ioopm_hash_table_t *ht)
{
    elem_t ignore = {.void_value = ""};
    int freq;
    if (ioopm_hash_table_lookup(ht, void_elem(word), &ignore))
    {
      freq = (ioopm_lookup_key(ht, (elem_t) {.void_value = word})).int_value;
      ioopm_hash_table_insert(ht, void_elem(word), int_elem(freq + 1));
    } 
    else
    {
      freq = 0;
      ioopm_hash_table_insert(ht, void_elem(strdup(word)), int_elem(freq + 1)); // strdup orsakar mem leaks
    }
}

// Opens file, processes the words in the file and close the file at EOF
void process_file(char *filename, ioopm_hash_table_t *ht)
{
  FILE *f = fopen(filename, "r");

  while (true)
  {
    char *buf = NULL;
    size_t len = 0;
    getline(&buf, &len, f);

    if (feof(f))
    {
      free(buf);
      break;
    }

    for (char *word = strtok(buf, Delimiters);
         word && *word;
         word = strtok(NULL, Delimiters))
    {
      process_word(word, ht);
    }
    free(buf);
  }

  fclose(f); // Closes when on the line that ends the file (new line required at end of file)
}


unsigned long string_knr_hash(const elem_t in_str)
{
  char *str = in_str.string_value;
  unsigned long result = 0;
  do
    {
      result = result * 31 + *str;
    }
  while (*++str != '\0');
  return result;
}

// Checks if two strings are the same
bool string_eq(elem_t e1, elem_t e2)
{
  return (strcmp(e1.void_value, e2.void_value) == 0);
}


static char **linked_list_to_array(ioopm_list_t *list)
{
  ioopm_list_iterator_t *iter = ioopm_list_iterator(list);
  size_t size = ioopm_linked_list_size(list);
  char **result_array = calloc(size+1, sizeof(char*) * size);
  int acc = 0;
  
  for(; ioopm_iterator_has_next(iter); acc++)
  {
    result_array[acc] = ((ioopm_iterator_current(iter)).void_value);
    ioopm_iterator_next(iter);
  }
  result_array[acc] = (ioopm_iterator_current(iter).void_value); // Final element
  ioopm_iterator_destroy(iter);
  return result_array;
}

bool int_eq(elem_t e1, elem_t e2)
{
  return e1.int_value == e2.int_value;
}

int main(int argc, char *argv[])
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_knr_hash, string_eq, int_eq);

  if (argc > 1)
  {
    for (int i = 1; i < argc; ++i)
    {
      process_file(argv[i], ht);
    }

    ioopm_list_t *list = ioopm_hash_table_keys(ht);
    char **keys = linked_list_to_array(list); 
    ioopm_linked_list_destroy(list);

    int size = ioopm_hash_table_size(ht);
    sort_keys(keys, size); 

    for (int i = 0; i < size; i++)
    {
      int freq = (ioopm_lookup_key(ht, (elem_t) {.void_value = keys[i]})).int_value;
      printf("%s: %d\n", keys[i], freq);
    }

    for (int i = 0; i < size; i++) {
      free(keys[i]);
    }

    free(keys);
  }
  else
  {
    puts("Usage: freq-count file1 ... filen");
  }
  ioopm_hash_table_destroy(ht);
}
