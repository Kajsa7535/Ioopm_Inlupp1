#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <CUnit/Basic.h>
#include "linked_list.h"
#include "iterator.h"
#include "common.h"

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
    ioopm_link_t *current;  // points to an element in list
    ioopm_list_t *list; // points to the list where current started
};

// Creates a linked list, it will insert the arg comp_function in the struct of list
ioopm_list_t *ioopm_linked_list_create(ioopm_eq_function comp_function) 
{
  ioopm_list_t *list = calloc(1, sizeof(ioopm_list_t));
  list->comp = comp_function;
  return list;
}

// creates one element in a linked list, with the value that is sent as arguments
static ioopm_link_t *element_create(elem_t value, ioopm_link_t *next)
{
  ioopm_link_t *new_element = calloc(1, sizeof(ioopm_link_t)); 
  new_element->value = value;
  new_element->next = next;
  return new_element;
}

// inserts a elements, with 'value' as value, last in 'list'
void ioopm_linked_list_append(ioopm_list_t *list, elem_t value)
{
    if (list->last == NULL) // checks if the list is empty
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

// inserts a element, with 'value' as value, first in 'list' 
void ioopm_linked_list_prepend(ioopm_list_t *list, elem_t value)
{
    ioopm_link_t *new_element = element_create(value, list->first);
    
    if (list->first == NULL) // checks if list is empty
    {
        list->last = new_element;
    }
    
    list->first = new_element;
    list->size += 1;
}

// returns the pointer to the link that points to the link of the input index.
static ioopm_link_t *find_previous_element (ioopm_list_t *list, int index)
{
    ioopm_link_t *prev_element = list -> first;
    for (int i = 1; i < index; i++) 
    {
        prev_element = prev_element->next;
    }
    return prev_element;
}

// inserts an element, with input 'value' as value, in list at input index
void ioopm_linked_list_insert(ioopm_list_t *list, int index, elem_t value)
{
    size_t max = list->size;
    assert(index >= 0 && index <= max); // checks that index is valid, else crash
    if (index == 0) // if insert first in list
    {
        ioopm_linked_list_prepend(list, value);
    }
    else if (index == max) // if insert last in list
    {
        ioopm_linked_list_append(list, value);
    }
    else // if insert middle in list
    {
        ioopm_link_t *prev_element = find_previous_element(list, index);
        ioopm_link_t *next_element = prev_element -> next; // second element

        ioopm_link_t *new_element = element_create(value, next_element);
        prev_element->next = new_element;  
        list->size += 1;
    } 
}

// removes the first element in list. It will free the allocated memory space and re-point the first pointer
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

// removes the element on input index, OBS not the first. Will free the allcated memory space.
static elem_t linked_list_remove_non_first(ioopm_list_t *list, int index)
 {
     ioopm_link_t *prev_element = find_previous_element(list, index);
     ioopm_link_t *remove_element = prev_element->next;
     elem_t value = remove_element->value;

     ioopm_link_t *next_element = remove_element->next;
     prev_element->next = next_element;
     if (next_element == NULL) // checks if the removed elements is last in list, then re-point the last-pointer in list struct
     {
         list->last = prev_element;
     }
     free(remove_element);
     list->size -= 1;
     return value;
 }

// removes the element on input index
elem_t ioopm_linked_list_remove(ioopm_list_t *list, int index)
{
    assert(list); // checks that list exists, else crash
    assert(list->size > 0); // checks that there are elements in the list, else crash
    // Case: First element in non-empty list
    if (index == 0)
    {
        return linked_list_remove_first(list);
    }
    // Case: Not first element in non-empty list
    else 
    {
         return linked_list_remove_non_first(list, index);
    }
}

// "destroys"/deletes input list. It will free the allocated memory space.
void ioopm_linked_list_destroy(ioopm_list_t *list)
{
    ioopm_linked_list_clear(list);
    free(list);
}

// will return the value of the element on input index in input list
elem_t ioopm_linked_list_get(ioopm_list_t *list, int index)
{
    assert(list->first); // checks that there are elements in list, else crash
    assert(list); 
    ioopm_link_t *element = list->first;
    for (int i = 0; i < index; i++)
    {
        element = element->next;
    }
    return element->value;
}

// return a bool value if the input element exists in the input list
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

// returns the size of input list
size_t ioopm_linked_list_size(ioopm_list_t *list)
{
    return list->size;
}

// returns a bool value depending on if input list is empty or not
bool ioopm_linked_list_is_empty(ioopm_list_t *list)
{
    return (list->size == 0);
}

// will "clear" input list. It will delete all elements in input list. The input list will be empty when the function is done
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
    list->first = NULL; 
    list->size = 0; 
}

// Checks if all elements in input list fulfills given predicate
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

// checks if any element in input list fulfills give predicate
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

// applies given function on all elements in input list
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

// creates an iterator in input list. Must be elements in list
ioopm_list_iterator_t *ioopm_list_iterator(ioopm_list_t *list)
{
    ioopm_list_iterator_t *iter = calloc(1, sizeof(ioopm_list_iterator_t));
    iter->current = list->first;
    iter->list = list;
    return iter;
}

// checks if there exists another element after current iterator
bool ioopm_iterator_has_next(ioopm_list_iterator_t *iter)
{
    return iter->current->next != NULL;
}

// will move the iterator one step and return the value of the new element. If it is the last element, the iteratoir will go to the front of the list
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


// resets the iterator. The iterator will start over from the front of the list
void ioopm_iterator_reset(ioopm_list_iterator_t *iter)
{
    iter->current = iter->list->first;
}

// returns the value of the element that iterator points to
elem_t ioopm_iterator_current(ioopm_list_iterator_t *iter)
{
    return iter->current->value;
}

// It destoys the iterator and free the allocated memory space
void ioopm_iterator_destroy(ioopm_list_iterator_t *iter)
{
    free(iter);
}
