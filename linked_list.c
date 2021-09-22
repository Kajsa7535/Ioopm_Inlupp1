#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <CUnit/Basic.h>
#include "linked_list.h"

ioopm_list_t *ioopm_linked_list_create()
{
  ioopm_list_t *link = calloc(1, sizeof(ioopm_list_t));
  link->last = link->first;
  return link;
}


static ioopm_link_t *element_create(int value, ioopm_link_t *next)
{
  ioopm_link_t *new_element = calloc(1, sizeof(ioopm_list_t)); 
  new_element->value = value;
  new_element->next = next;
  return new_element;
}


static int length_of_list(ioopm_list_t *list)
{
    ioopm_link_t *element = list->first;
    int acc = 0;
    while (element != NULL)
    {
        acc = acc + 1;
        element = element->next;
    }
    return acc;
}

//(0,->) (1, null)

void ioopm_linked_list_append(ioopm_list_t *list, int value)
{
    ioopm_link_t *element = list->first; // går förbi dummy, pekar nu på första "riktiga elementet" eller NULL
    for (int i = 0; i < length_of_list(list); i++)
    {
        element = element->next;
    }
    ioopm_link_t *new_element = element->next;
    new_element = element_create(value, NULL);
}

void ioopm_linked_list_prepend(ioopm_list_t *list, int value)
{
    ioopm_link_t *new_element = element_create(value, list->first);
    
    if (length_of_list(list) == 1)
    {
        list->last = new_element;
    }
    
    list->first = new_element;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////


int main(void)
{
    ioopm_list_t *list = ioopm_linked_list_create();
    ioopm_link_t *first = list->first;
    int length = length_of_list(list);

    printf("(%d)", length);
    if (first == NULL)
    {
        puts("ingen dummy");
    }
    else
    {
        puts("dummy");
    }

    ioopm_linked_list_prepend(list, 3);
    //ioopm_linked_list_prepend(list, 4);

    int value = first->value;
    printf("(%d)", value);
    /*
    for (int i = 0; i < length_of_list(list); i++)
    {
        int value = first->value;
        first = first->next;
        printf("(%d)", value);
    }*/
   // ioopm_list_t *element = list->next;
    //int value;

    
    //ioopm_list_t *test = list->next;
    //printf("value = (%d)", value);
    //if (test == NULL) 
    //{
    //    puts("yey");
    //}
   // return 0;
}
