#include "session.h"

#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "memory.h"
#include "game.h"
#include "server.h"

session_t* new_session(struct ev_loop* loop, struct ev_io* watcher)
{
	session_t* session = safe_alloc(sizeof(session));
	session->loop = loop;
	session->watcher = watcher;
	session->attachment = NULL;
	return session;
}

void free_session(session_t* session)
{
	ev_io_stop(session->loop, session->watcher);
	close(session->watcher->fd);
	free(session->watcher);
	session->watcher = NULL;
	free(session);
}

void session_disconnect(session_t* session)
{
	handle_game_disconnection(session);
	hashmap_remove(connections, session->watcher->fd);
	free_session(session);
}

void session_write(session_t* session, buffer_t* buffer)
{
	send(session->watcher->fd, buffer->payload, buffer->size, 0);
}