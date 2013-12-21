#ifndef SESSION_H
#define SESSION_H

#pragma once

#include <ev.h>
#include "buffer.h"

typedef struct
{
	struct ev_loop* loop;
	struct ev_io* watcher;
	void* attachment;
} session_t;

/**
 * Allocates a new session structure. This function
 * is called automatically by the game core when a connection
 * is established.
 */
session_t* new_session(struct ev_loop* loop, struct ev_io* watcher);

/**
 * Deallocates a session structure along with its attachment.
 * This function will also disconnect the connection using
 * the underlying networking implementation.
 */
void free_session(session_t* session);

/**
 * Disconnects a connection while also freeing the allocated
 * memory by calling free_session.
 */
void session_disconnect(session_t* session);

/**
 * Writes a buffer to the underlying socket of a session.
 * This allows the game modules to not have direct access to the
 * networking implementation.
 */
void session_write(session_t* session, buffer_t* buffer);

#endif