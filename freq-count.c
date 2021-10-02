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

// Sums all characters in a string
int string_sum_hash(elem_t e)
{
  char *str = e.void_value;
  int result = 0;
  do
    {
      result += *str;
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
  char **result_array[size];
  int acc = 0;
  for(; ioopm_iterator_has_next(iter); acc++)
  {
    *result_array[acc] = (ioopm_iterator_current(iter)).string_value; // Seg fault
    ioopm_iterator_next(iter);
  }
  *result_array[acc+1] = ioopm_iterator_current(iter).string_value; // Final element
  ioopm_iterator_destroy(iter);
  return *result_array;
}


int main(int argc, char *argv[])
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_sum_hash, string_eq);

  if (argc > 1)
  {
    for (int i = 1; i < argc; ++i)
    {
      process_file(argv[i], ht);
    }

    // FIXME: If the keys are returned as a list, transfer them into 
    // an array to use `sort_keys` (perhaps using an iterator?)
    char **keys = linked_list_to_array(ioopm_hash_table_keys(ht)); // Seg fault

    int size = ioopm_hash_table_size(ht);
    sort_keys(keys, size); // Blev Seg fault eftersom keys returnerar en linked list, ny funktion linked_list_to_array skapad

    for (int i = 0; i < size; ++i)
    {
      // FIXME: Update to match your own interface, error handling, etc.
      int freq = (ioopm_lookup_key(ht, (elem_t) {.void_value = keys[i]})).int_value;
      printf("%s: %d\n", keys[i], freq);
    }
  }
  else
  {
    puts("Usage: freq-count file1 ... filen");
  }

  // FIXME: Leaks memory! Use valgrind to find out where that memory is 
  // being allocated, and then insert code here to free it.
  ioopm_hash_table_destroy(ht);
}
