#ifndef GAME_H
#define GAME_H

#pragma once

#include <stdint.h>
#include "session.h"

typedef struct
{
	char* name;
	uint16_t port;
	size_t buffer_size;
	size_t max_connections;
} game_conf_t;

typedef enum
{
	game_err_nomem
} game_err;

/**
 * Loads game configurations such as the game name
 * and what port it operates on. This function is
 * invoke immediately after the game's core
 * has been initialized. The configuration struct
 * will not be deallocated automatically by the core.
 */
game_conf_t* load_game_configuration();

/**
 * Initializes the games inner module, this should load things
 * such as entity definitions and other sub-modules (if any).
 */
void intialize_game_module();

/**
 * Handles an incomming connection to the game.
 */
void handle_game_connection(session_t* session);

/**
 * Handles a connection that has been terminated.
 * Session deallocation is handled by the core.
 */
void handle_game_disconnection(session_t* session);

/**
 * Handles an incomming data to the game.
 * Buffer deallocation is handled by the core.
 */
void handle_game_data(session_t* session, buffer_t* buffer);

/**
 * Handles an error should one occur within the game's core.
 */
void handle_game_error(game_err error);

#endif