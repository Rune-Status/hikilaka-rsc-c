#include "hashmap.h"

#include <stdlib.h>
#include <stdio.h>
#include "memory.h"

#define INITIAL_SIZE 1024

hashmap_t* new_hashmap()
{
	hashmap_t* m = safe_alloc(sizeof(hashmap_t));
	if(!m)
	{
		return NULL;
	}

	m->data = safe_calloc(INITIAL_SIZE, sizeof(hashmap_element));
	if(!m->data)
	{
		if (m)
			free_hashmap(m);
		return NULL;
	}

	m->table_size = INITIAL_SIZE;
	m->size = 0;
	return m;
}

void free_hashmap(hashmap_t* m)
{
	free(m->data);
	free(m);
}

unsigned int hashmap_hash_int(hashmap_t* m, unsigned int key)
{
	key += (key << 12);
	key ^= (key >> 22);
	key += (key << 4);
	key ^= (key >> 9);
	key += (key << 10);
	key ^= (key >> 2);
	key += (key << 7);
	key ^= (key >> 12);

	key = (key >> 3) * 2654435761;

	return key % m->table_size;
}

int hashmap_hash(hashmap_t* m, int key)
{
	int curr;
	int i;

	if(m->size == m->table_size) return MAP_FULL;

	curr = hashmap_hash_int(m, key);

	for(i = 0; i< m->table_size; i++)
	{
		if(m->data[curr].in_use == 0)
			return curr;

		if(m->data[curr].key == key && m->data[curr].in_use == 1)
			return curr;

		curr = (curr + 1) % m->table_size;
	}

	return MAP_FULL;
}

int hashmap_rehash(hashmap_t* m)
{
	int i;
	int old_size;
	hashmap_element* curr;

	hashmap_element* temp = (hashmap_element *)
		calloc(2 * m->table_size, sizeof(hashmap_element));
	if(!temp) return MAP_OMEM;

	curr = m->data;
	m->data = temp;

	old_size = m->table_size;
	m->table_size = 2 * m->table_size;
	m->size = 0;

	for(i = 0; i < old_size; i++)
	{
		int status = hashmap_put(m, curr[i].key, curr[i].data);
		if (status != MAP_OK)
			return status;
	}

	free(curr);

	return MAP_OK;
}

int hashmap_put(hashmap_t* m, int key, void* value)
{
	int index;

	index = hashmap_hash(m, key);
	while(index == MAP_FULL)
	{
		if (hashmap_rehash(m) == MAP_OMEM)
		{
			return MAP_OMEM;
		}
		index = hashmap_hash(m, key);
	}

	m->data[index].data = value;
	m->data[index].key = key;
	m->data[index].in_use = 1;
	m->size++;
	return MAP_OK;
}

int hashmap_get(hashmap_t* m, int key, void** arg)
{
	int curr;
	int i;

	curr = hashmap_hash_int(m, key);

	for (i = 0; i< m->table_size; i++)
	{
		if (m->data[curr].key == key && m->data[curr].in_use == 1)
		{
			*arg = (void*) (m->data[curr].data);
			return MAP_OK;
		}
		curr = (curr + 1) % m->table_size;
	}

	*arg = NULL;
	return MAP_MISSING;
}

int hashmap_get_one(hashmap_t* m, void** arg, int remove)
{
	int i;

	if (hashmap_length(m) <= 0)
		return MAP_MISSING;

	for(i = 0; i< m->table_size; i++)
	{
		if(m->data[i].in_use != 0)
		{
			*arg = (void*) (m->data[i].data);
			if (remove)
			{
				m->data[i].in_use = 0;
				m->size--;
			}
			return MAP_OK;
		}
	}
	return MAP_OK;
}

int hashmap_iterate(hashmap_t* m, PFany f, void* item)
{
	int i;

	if (hashmap_length(m) <= 0)
		return MAP_MISSING;

	for(i = 0; i< m->table_size; i++)
	{
		if(m->data[i].in_use != 0)
		{
			void* data = (void*) (m->data[i].data);
			int status = f(item, data);
			if (status != MAP_OK)
			{
				return status;
			}
		}
	}
	return MAP_OK;
}

int hashmap_remove(hashmap_t* m, int key)
{
	int i;
	int curr;

	curr = hashmap_hash_int(m, key);

	for (i = 0; i< m->table_size; i++)
	{
		if (m->data[curr].key == key && m->data[curr].in_use == 1)
		{
			m->data[curr].in_use = 0;
			m->data[curr].data = NULL;
			m->data[curr].key = 0;
			m->size--;
			return MAP_OK;
		}
		curr = (curr + 1) % m->table_size;
	}
	return MAP_MISSING;
}

int hashmap_length(hashmap_t* m)
{
	if(m != NULL)
		return m->size;
	return 0;
}