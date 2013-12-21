#include "session.h"

#include <string.h>
#include <sys/socket.h>
#include "memory.h"

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
	if (session->watcher)
	{
		ev_io_stop(session->loop, session->watcher);
		shutdown(session->watcher->fd, 2);
		free(session->watcher);
		session->watcher = NULL;
	}
	if (session->attachment)
	{
		free(session->attachment);
		session->attachment = NULL;
	}
	free(session);
}

void session_write(session_t* session, buffer_t* buffer)
{
	send(session->watcher->fd, buffer->payload, buffer->size, 0);
}