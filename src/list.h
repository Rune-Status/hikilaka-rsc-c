#ifndef list_h
#define list_h

#pragma once

#include <stddef.h>
#include <stdbool.h>

typedef void* any_t;

typedef int(*list_comparator_t)(any_t, any_t);

typedef bool(*list_filter_t)(any_t);

typedef any_t(*list_map_t)(any_t);

typedef void(*list_reduce_t)(any_t, any_t);

typedef struct list_node_s
{
    any_t val;
    struct list_node_s* next;
    struct list_node_s* prev;
} list_node_t;

typedef struct
{
    size_t size;
    list_node_t* head;
    list_node_t* tail;
} list_t;

/*
 * Iterates over the nodes of a list. To access
 * the node's data, simply use node->val
 */
#define LIST_FOR_EACH(list, node) \
    for (list_node_t* node = list->head; node; node = node->next)

/*
 * Iterates over the nodes of a list in reverse. To access
 * the node's data, simply use node->val
 */
#define LIST_REVERSE_FOR_EACH(list, node) \
    for (list_node_t* node = list->tail; node; node = node->prev)

/*
 * Iterates over the contents of a list while popping the
 * elements off at the same time
 */
#define LIST_FOR_EACH_POP(list, val) \
    for (any_t val = list_pop(list); val; val = list_pop(list))

/**
 * Allocates a new list
 * Returns NULL if malloc failed
 */
list_t* new_list();

/**
 * Deallocates a list and all of its nodes
 * Optionally will free the node's values
 */
void free_list(list_t* list);

/**
 * Clears the nodes of a list
 * Optionally will free the node's values
 */
void list_clear(list_t* list);

/**
 * Gets the n'th node's value
 * Returns NULL if pos > the list length
 */
any_t list_get(list_t* list, size_t pos);

/**
 * Removes the list's n'th value
 * Returns NULL if pos > the list length else
 * returns the value of the removed node
 */
any_t list_remove(list_t* list, size_t pos);

/**
 * Pushes an element to the front of a list
 * Returns 1 on error and 0 on success
 */
int list_add_front(list_t* list, any_t val);

/**
 * Adds an element to the back of a list
 * Returns 1 on error and 0 on success
 */
int list_add_back(list_t* list, any_t val);

/**
 * Adds an element to the back of a list
 * Returns 1 on error and 0 on success
 * This function is the same as list_add_back
 */
int list_push(list_t* list, any_t val);

/**
 * Pops an element off the top of a list
 * Returns NULL if the list is empty
 */
any_t list_pop(list_t* list);

/**
 * Filters elements into a newly allocated list, free_list
 * must be called on the returned list
 */
list_t* list_filter(list_t* list, list_filter_t filter);

/**
 * Maps a function to this list
 */
void list_map(list_t* list, list_map_t map);

/**
 * Reduces a list to a single value
 */
any_t list_reduce(list_t* list, any_t val, list_reduce_t reduce);

#endif