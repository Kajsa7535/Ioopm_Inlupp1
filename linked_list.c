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

static int length_of_list(ioopm_list_t *list)
{
    ioopm_list_t *element = list->next;
    int acc = 0;
    while (element != NULL)
    {
        acc = acc + 1;
        element = element->next;
    }
    return acc;
}

//(0,->) (1, null)

//TODO: Fix linked_list_append, segmentation fault
void ioopm_linked_list_append(ioopm_list_t *list, int value)
{
    ioopm_list_t *element = list; // går förbi dummy, pekar nu på första "riktiga elementet" eller NULL
    for (int i = 0; i < length_of_list(list); i++)
    {
        element = element->next;
    }
    element->next = element_create(value, NULL);
}

void ioopm_linked_list_prepend(ioopm_list_t *list, int value)
{
    ioopm_list_t *dummy_next = list->next;
    list->next = element_create(value, dummy_next);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
int main(void)
{
    ioopm_list_t *list = ioopm_linked_list_create();
    //ioopm_linked_list_prepend(list, 3);
    //ioopm_linked_list_prepend(list, 4);
    ioopm_linked_list_append(list, 10);
    ioopm_list_t *element = list->next;
    int value;

    for (int i = 0; i < length_of_list(list); i++)
    {
        value = element->value;
        element = element->next;
        printf("(%d)\n", value);
    }
    
    //ioopm_list_t *test = list->next;
    //printf("value = (%d)", value);
    //if (test == NULL) 
    //{
    //    puts("yey");
    //}
   // return 0;
}
*/
