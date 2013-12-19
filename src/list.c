#include "list.h"

#include <stdlib.h>

list_t* new_list()
{
    list_t* list = malloc(sizeof(list_t));
    if (list)
    {
        list->size = 0;
        list->head = NULL;
        list->tail = NULL;
    }
    return list;
}

void free_list(list_t* list)
{
    list_clear(list);
    free(list);
}

void list_clear(list_t* list)
{
    list_node_t* cur = list->head;
    while (cur != NULL)
    {
        free(cur);
        cur = cur->next;
    }
    list->size = 0;
    list->head = list->tail = NULL;
}

any_t list_get(list_t* list, size_t pos)
{
    any_t val = NULL;
    if (pos < list->size)
    {
        list_node_t* node;
        for (node = list->head; pos > 0; node = node->next, --pos);
        val = node->val;
    }
    return val;
}

any_t list_remove(list_t* list, size_t pos)
{
    any_t val = NULL;
    if (pos < list->size)
    {
        list_node_t* node;
        for (node = list->head; pos > 0; node = node->next, --pos);
        
        if (node->prev)
        {
            node->prev->next = node->next;
            node->next->prev = node->prev;
        }
        
        val = node->val;
        free(node);
        node = NULL;
    }
    return val;
}

int list_add_front(list_t* list, any_t val)
{
    list_node_t* node = malloc(sizeof(list_node_t));
    
    // malloc failed, return error code
    if (!node)
        return 1;

    node->val = val;
    node->prev = NULL;

    if (!list->head)
    {
        // the list has no elements
        list->head = list->tail = node;
    }
    else
    {
        // the list already has at least one node
        node->next = list->head;
        list->head->prev = node;
        list->head = node;
    }
    ++list->size;
    return 0;
}

int list_add_back(list_t* list, any_t val)
{
    list_node_t* node = malloc(sizeof(list_node_t));
    
    // malloc failed, return error code
    if (!node)
        return 1;
    
    node->val = val;
    
    if (!list->tail)
    {
        // the list has no elements
        node->prev = NULL;
        list->head = list->tail = node;
    }
    else
    {
        // the list already has at least one node
        node->prev = list->tail;
        list->tail->next = node;
        list->tail = node;
    }
    ++list->size;
    return 0;
}

int list_push(list_t* list, any_t val)
{
    return list_add_back(list, val);
}

any_t list_pop(list_t* list)
{
    any_t val = NULL;
    if (list->head)
    {
        list_node_t* head = list->head;
        val = head->val;
        
        if (head->next)
        {
            list->head = head->next;
        }
        else
        {
            list->head = list->tail = NULL;
        }
        free(head);
        head = NULL;
        --list->size;
    }
    return val;
}

list_t* list_filter(list_t* list, list_filter_t filter)
{
    list_t* filtered = new_list();
    if (filtered)
    {
        LIST_FOR_EACH(list, node)
        {
            if (filter(node->val))
            {
                list_push(filtered, node->val);
            }
        }
    }
    return filtered;
}

void list_map(list_t* list, list_map_t map)
{
    LIST_FOR_EACH(list, node)
    {
        node->val = map(node->val);
    }
}

any_t list_reduce(list_t* list, any_t val,  list_reduce_t reduce)
{
    LIST_FOR_EACH(list, node)
    {
        reduce(val, node->val);
    }
    return val;
}