#ifndef HASHMAP_H
#define HASHMAP_H

#pragma once

#define MAP_MISSING -3
#define MAP_FULL -2
#define MAP_OMEM -1
#define MAP_OK 0

typedef struct {
	int key;
	int in_use;
	void* data;
} hashmap_element;

typedef struct {
	int table_size;
	int size;
	hashmap_element* data;
} hashmap_t;

typedef int (*PFany)(void*, void*);

hashmap_t* new_hashmap(void);

void free_hashmap(hashmap_t* in);

int hashmap_iterate(hashmap_t* in, PFany f, void* item);

int hashmap_put(hashmap_t* in, int key, void* value);

int hashmap_get(hashmap_t* in, int key, void** arg);

int hashmap_remove(hashmap_t* in, int key);

int hashmap_get_one(hashmap_t* in, void** arg, int remove);

int hashmap_length(hashmap_t* in);

#endif
