#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <CUnit/Basic.h>
#include "linked_list.h"
#include "iterator.h"
#include "common.h"


//TODO: Ta bort alla index kollare för negativa index 

struct link 
{
    elem_t value;  // holds the value
    ioopm_link_t *next; // points to the next entry (possibly NULL)
};

struct list 
{
    ioopm_link_t *first; // points to the first element
    ioopm_link_t *last; // points to the last element
    size_t size; // counts the number of elements in the list
    ioopm_eq_function comp; 
};

struct iterator 
{
    ioopm_link_t *current;  
    ioopm_list_t *list;
};

//CHECK
ioopm_list_t *ioopm_linked_list_create(ioopm_eq_function comp_function) 
{
  ioopm_list_t *link = calloc(1, sizeof(ioopm_list_t));
  link->comp = comp_function;
  //link->last = link->first;
  return link;
}

// CHECK
static ioopm_link_t *element_create(elem_t value, ioopm_link_t *next)
{
  ioopm_link_t *new_element = calloc(1, sizeof(ioopm_link_t)); 
  new_element->value = value;
  new_element->next = next;
  return new_element;
}

//CHECK
void ioopm_linked_list_append(ioopm_list_t *list, elem_t value)
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

// CHECK
void ioopm_linked_list_prepend(ioopm_list_t *list, elem_t value)
{
    ioopm_link_t *new_element = element_create(value, list->first);
    
    if (list->first == NULL)
    {
        list->last = new_element;
    }
    
    list->first = new_element;
    list->size += 1;
}

//CHECK
void ioopm_linked_list_insert(ioopm_list_t *list, int index, elem_t value)
{
    size_t max = list->size;
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
    else // TODO: KAN ANVÄNDA OSS AV FIND_PREVIOUS
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

// CHECK
static ioopm_link_t *find_previous_element (ioopm_list_t *list, int index)
{
    ioopm_link_t *prev_element = list -> first;
    for (int i = 1; i < index; i++) 
    {
        prev_element = prev_element->next;
    }
    return prev_element;
}

//CHECK
static elem_t linked_list_remove_first(ioopm_list_t *list)
{
    ioopm_link_t *tmp = list->first;
    elem_t value = tmp->value; 
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
    list->size -= 1;
    return value;
}

// CHECK
static elem_t linked_list_remove_last(ioopm_list_t *list)
{
    elem_t value = list->last->value;
    ioopm_link_t *tmp_last = list->last;
    ioopm_link_t *prev_element = find_previous_element(list, (list->size-1));
    prev_element->next = NULL;
    list->last = prev_element;
    free(tmp_last);
    list->size -= 1;
    return value;
}

// CHECK
static elem_t linked_list_remove_middle(ioopm_list_t *list, int index)
{
    ioopm_link_t *prev_element = find_previous_element(list, index);
    ioopm_link_t *remove_element = prev_element->next;
    elem_t value = remove_element->value;

    ioopm_link_t *next_element = remove_element->next;
    prev_element->next = next_element;
    free(remove_element);
    list->size -= 1;
    return value;
}

// CHECK
elem_t ioopm_linked_list_remove(ioopm_list_t *list, int index)
{
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
         return linked_list_remove_middle(list, index);
    }
}

// CHECK
void ioopm_linked_list_destroy(ioopm_list_t *list)
{
    ioopm_linked_list_clear(list);
    free(list);
}

// CHECK
elem_t ioopm_linked_list_get(ioopm_list_t *list, int index)
{
    assert(list->first);
    assert(list);
    ioopm_link_t *element = list->first;
    for (int i = 0; i < index; i++)
    {
        element = element->next;
    }
    return element->value;
}

// CHECK
bool ioopm_linked_list_contains(ioopm_list_t *list, elem_t element)
{   
    ioopm_link_t *current_element = list->first;
    ioopm_eq_function eq_func = list->comp;
    elem_t value;
    for(int i = 0; i < list->size; i++)
    {
        value = current_element->value;
        if(eq_func(value, element))
        {
            return true;
        }
        else
        {
            current_element = current_element->next;
        }
    }
    return false;
}

// CHECK
size_t ioopm_linked_list_size(ioopm_list_t *list)
{
    return list->size;
}

// CHECK
bool ioopm_linked_list_is_empty(ioopm_list_t *list)
{
    return (list->size == 0);
}

// CHECK
void ioopm_linked_list_clear(ioopm_list_t *list)
{
    list->last = NULL;
    ioopm_link_t *current = list ->first;
    while (current)   
    { 
        ioopm_link_t *tmp = current;    
        current = current->next;      
        free(tmp);    
    }   
    // Caselist->first = NULL; 
    list->size = 0; 
}


//CHECK
static bool value_int_equiv(int key_ignore, elem_t value, void *x)
{
  int *other_int_ptr = x;
  int other_value = *other_int_ptr;
  return value.int_value == other_value;
}


//CHECK
bool ioopm_linked_list_all(ioopm_list_t *list, ioopm_predicate prop, void *extra)
{
    ioopm_link_t *current_element = list->first;
    elem_t key_ignored = {.int_value = 0};
    for (int i = 0; i < list->size; i++)
        {
            elem_t value_compare = current_element->value;
            if (!(prop(key_ignored, value_compare, extra)))
            {
                return false;
            }
            current_element = current_element->next;
        }
    return true;
}

// CHECK
bool ioopm_linked_list_any(ioopm_list_t *list, ioopm_predicate prop, void *extra)
{  
    elem_t key_ignored = {.int_value = 0};
    ioopm_link_t *current_element = list->first;
    for (int i = 0; i < list->size; i++)
        {
            elem_t value_compare = current_element->value;
            if (prop(key_ignored, value_compare, extra))
            {
                return true;
            }
            current_element = current_element->next;
        }
    return false;
}

// HUR SKA MAN KUNNA JÄMFÖRA TVÅ ELEMENT?
static void update_int_value(int key_ignored, elem_t *value, void *arg)
{
  elem_t *other_int_ptr = arg;
  elem_t other_value = *other_int_ptr;
  *value = other_value;
}

//CHECK

void ioopm_linked_apply_to_all(ioopm_list_t *list, ioopm_apply_function fun, void *extra)
{
    elem_t key_ignored = {.int_value = 0};
    ioopm_link_t *current_element = list->first;
    for (int i = 0; i < list->size; i++)
        {
            fun(key_ignored, &current_element->value, extra);
            current_element = current_element->next;
        }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ITERATOR FUNCTIONS

ioopm_list_iterator_t *ioopm_list_iterator(ioopm_list_t *list)
{
    ioopm_list_iterator_t *iter = calloc(1, sizeof(ioopm_list_iterator_t));
    iter->current = list->first;
    iter->list = list;
    return iter;
}

bool ioopm_iterator_has_next(ioopm_list_iterator_t *iter)
{
    if (iter->list->size == 0) 
    {
        return false; 
    }
    else
    {
        return iter->current->next != NULL;
    }
}

elem_t ioopm_iterator_next(ioopm_list_iterator_t *iter)
{
    if (ioopm_iterator_has_next(iter) == true)
    {
        iter->current = iter->current->next;
        return iter->current->value;
    }
    else
    {
        iter->current = iter->list->first;
        return iter->current->value;
    }

}

static ioopm_link_t *iterator_find_previous_element(ioopm_list_iterator_t *iter)
{
    ioopm_link_t *element = iter->list->first;
    while(element->next != iter->current)
    {
        element = element->next;
    }
    return element;
}

//(0,->) (1, ->) (2,null) (4, null)
//TODO: GÖR KLART NÄR TID FINNSS!
elem_t ioopm_iterator_remove(ioopm_list_iterator_t *iter)
{
    // current dubbelpekare, pekare till listan, sista elementet specialfall med linked_list_remove
    //TODO: Skapa denna funktion om tid finns över
    //ioopm_link_t *current_element = iter->current;
    ioopm_link_t *remove_element = iter->current;
    elem_t remove_value;
    if(remove_element == iter->list->first) //Compare pointers
    {
        iter->list->first = remove_element->next;
    }
    else if(remove_element == iter->list->last) //Compare pointers
    {
        ioopm_link_t *prev_element = iterator_find_previous_element(iter);
        prev_element->next = NULL;
        iter->list->last = prev_element;
    }
    else
    {
        ioopm_link_t *prev_element = iterator_find_previous_element(iter);
        ioopm_link_t *next_element = iter->current->next;
        prev_element->next = next_element;
        
    }
    remove_value = remove_element->value;
    free (remove_element);
    return remove_value;
}

void ioopm_iterator_insert(ioopm_list_iterator_t *iter, int element)
{
    //TODO: Skapa denna funktion om tid finns över
}

void ioopm_iterator_reset(ioopm_list_iterator_t *iter)
{
    iter->current = iter->list->first;
}

elem_t ioopm_iterator_current(ioopm_list_iterator_t *iter)
{
    return iter->current->value;
}

void ioopm_iterator_destroy(ioopm_list_iterator_t *iter)
{
    free(iter);
}


bool dummy (elem_t value1, elem_t value2) 
{ 
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
int main(void)
{
    ioopm_list_t *list = ioopm_linked_list_create(int_eq);

    for (int i = 0; i < 10; i++)
    {
        int value = random()%100;
        ioopm_linked_list_prepend(list, int_elem(value));
        if (ioopm_linked_list_contains(list, int_elem(value)))
        {
            puts("yey\n");
        }
        else
        {
            puts("nej :(\n");
        }
    }
}
*/

// TODO: Update function specifications in .h-file 
// TODO: Extend the documentation with how you deal with failures and all other assumptions or caveats

