#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <CUnit/Basic.h>
#include "linked_list.h"

ioopm_list_t *ioopm_linked_list_create()
{
  ioopm_list_t *link = calloc(1, sizeof(ioopm_list_t));
  //link->last = link->first;
  return link;
}

static ioopm_link_t *element_create(int value, ioopm_link_t *next)
{
  ioopm_link_t *new_element = calloc(1, sizeof(ioopm_link_t)); 
  new_element->value = value;
  new_element->next = next;
  return new_element;
}

/*
static int length_of_list(ioopm_list_t *list)
{
    ioopm_link_t *element = list->first;
    int acc = 0;
  
    if(list->first == NULL)
    {
        return acc;
    }
    do 
    {
        acc = acc + 1;
        element = element->next;
    } while (element != NULL);
    return acc;
}*/

//(0,->) (1, ->) (2,null) (4, null)

void ioopm_linked_list_append(ioopm_list_t *list, int value)
{
    if (list->last == NULL)
    {
        ioopm_linked_list_prepend(list, value);
    }
    else
    {
        ioopm_link_t *new_element = element_create(value, NULL);
        list->last->next = new_element;
        list->last = new_element;
    }
    list->size += 1;
}

void ioopm_linked_list_prepend(ioopm_list_t *list, int value)
{
    ioopm_link_t *new_element = element_create(value, list->first);
    
    if (list->first == NULL)
    {
        list->last = new_element;
    }
    
    list->first = new_element;
    list->size += 1;
}

void ioopm_linked_list_insert(ioopm_list_t *list, int index, int value)
{
    int max = list->size;
    if (index < 0 || index > max)
    {
        puts("Wrong input!");
    }
    else if (index == 0)
    {
        ioopm_linked_list_prepend(list, value);
    }
    else if (index == max)
    {
        ioopm_linked_list_append(list, value);
    }
    else
    {
        ioopm_link_t *prev_element = list -> first; //first element
        ioopm_link_t *next_element = list -> first -> next; // second element
        for (int i = 1; i < index; i++) // index 3 
        {
            prev_element = prev_element->next;
            next_element = next_element->next;
        }

        ioopm_link_t *new_element = element_create(value, next_element);
        prev_element->next = new_element;    
    } 
    list->size += 1;
}


static ioopm_link_t *find_previous_element (ioopm_list_t *list, int index)
{
    ioopm_link_t *prev_element = list -> first;
    for (int i = 1; i < index; i++) 
    {
        prev_element = prev_element->next;
    }
    return prev_element;
}

static int linked_list_remove_first(ioopm_list_t *list)
{
    ioopm_link_t *tmp = list->first;
    int value = tmp->value; 
    if (list->size == 1)
    {
        list->first = NULL;
        list->last = NULL;
    }
    else
    {
        list->first = tmp->next;
    }
    free(tmp);
    return value;
}

static int linked_list_remove_last(ioopm_list_t *list)
{
    int value = list->last->value;
    ioopm_link_t *tmp_last = list->last;
    ioopm_link_t *prev_element = find_previous_element(list, (list->size-1));
    prev_element->next = NULL;
    list->last = prev_element;
    free(tmp_last);
    return value;
}

int ioopm_linked_list_remove(ioopm_list_t *list, int index)
{
    int value;
    assert(list);
    assert(list->size > 0);
    // Case: First element in non-empty list
    if (index == 0)
    {
        return linked_list_remove_first(list);
    }
    // Case: Last element in non-empty list
    else if (index == list->size-1)
    {
        return linked_list_remove_last(list);
    }
    // Case: Not first nor last element in non-empty list
    else
    {
        ioopm_link_t *next_element = list -> first; 
        for (int i = 0; i < index; i++) 
        {
            next_element = next_element->next;
        }
        ioopm_link_t *prev_element = find_previous_element(list, index);
        value = next_element->value;
        prev_element->next = next_element->next;
        free(next_element);
        return value;
    }
    list->size -= 1;
}
//(0,->) (1, ->) (2,->) (4, null)

void ioopm_linked_list_destroy(ioopm_list_t *list)
{
    list->last = NULL;
    ioopm_link_t *current = list ->first;
    while (current)   
    { 
        ioopm_link_t *tmp = current;    
        current = current->next;      
        free(tmp);    
    }    
    free(list);

    //(0,->) (1, ->) (2,->) (4, null)
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
int main(void)
{
    ioopm_list_t *list = ioopm_linked_list_create();
    //ioopm_link_t *first = list->first;
    //int length = length_of_list(list);
    ioopm_linked_list_prepend(list, 1);
    ioopm_linked_list_append(list, 2);
    int result = length_of_list(list);
    //int lng = length_of_list(list);
    printf("(%d)\n", result);
   // ioopm_list_t *element = list->next;
    //int value;
    //int value = list->first->next->next->next->value;
    //printf("(%d)\n", value);
    
    //ioopm_list_t *test = list->next;
    //printf("value = (%d)", value);
    //if (test == NULL) 
    //{
    //    puts("yey");
    //}
   // return 0;
}*/
