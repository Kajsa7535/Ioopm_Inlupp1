#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <CUnit/Basic.h>
#include "linked_list.h"

ioopm_list_t *ioopm_linked_list_create()
{
  ioopm_list_t *result = calloc(1, sizeof(ioopm_list_t));
  return result;
}

static ioopm_list_t *element_create(int value, ioopm_list_t *next)
{
  ioopm_list_t *element = calloc(1, sizeof(ioopm_list_t)); 
  element->value = value;
  element->next = next;
  return element;
}

static int length_of_list(ioopm_list_t list)
{
    for (int i = 0; element_next != NULL; i++)
    {
        
    }
}

void ioopm_linked_list_append(ioopm_list_t *list, int value)
{
    for (int i = 0; i < length_of_list; i++)
    {
        ////////////////////////////////
        ioopm_list_t *
        
    }
    
}

void ioopm_linked_list_prepend(ioopm_list_t *list, int value)
{
    ioopm_list_t *dummy_next = list->next;
    list->next = element_create(value, dummy_next);
}

//(0, null) (3, null)

int main(void)
{
    ioopm_list_t *list = ioopm_linked_list_create();
    int value = list->value;
    ioopm_list_t *test = list->next;
    printf("value = (%d)", value);
    if (test == NULL) 
    {
        puts("yey");
    }
    return 0;
}
