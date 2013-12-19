#include "memory.h"

#include <stdlib.h>
#include "game.h"

void* safe_alloc(size_t size)
{
	if (size == 0)
		return NULL;
	void* block = malloc(size);

	if (block == NULL)
		handle_game_error(game_err_nomem);

	return block;
}

void* safe_calloc(size_t amount, size_t size)
{
	if (amount == 0 || size == 0)
		return NULL;
	void* block = calloc(amount, size);

	if (block == NULL)
		handle_game_error(game_err_nomem);

	return block;
}

void* safe_realloc(void* ptr, size_t size)
{
	if (size == 0)
		return NULL;
	void* newptr = realloc(ptr, size);

	if (newptr != NULL)
		ptr = newptr;
	else
		handle_game_error(game_err_nomem);
	return newptr;
}