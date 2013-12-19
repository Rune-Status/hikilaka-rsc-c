#ifndef SESSION_H
#define SESSION_H

#pragma once

#include <lacewing.h>
#include "buffer.h"

typedef struct
{
	lw_server_client client;
	void* attachment;
} session_t;

/**
 * Allocates a new session structure. This function
 * is called automatically by the game core when a connection
 * is established.
 */
session_t* new_session(lw_server_client client);

/**
 * Deallocates a session structure. This function will also
 * disconnect the connection using the underlying networking
 * implementation.
 */
void free_session(session_t* session);

/**
 * Sets the sessions attachment
 */
void session_set_attachment(session_t* session, void* attachment);

/**
 * Gets a sessions attachment
 */
void* session_get_attachment(session_t* session);

/**
 * Writes a buffer to the underlying socket of a session.
 * This allows the game modules to not have direct access to the
 * networking implementation.
 */
void session_write(session_t* session, buffer_t* buffer);

#endif