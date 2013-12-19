#include "session.h"

#include <string.h>
#include "memory.h"

session_t* new_session(lw_server_client client)
{
	session_t* session = safe_alloc(sizeof(session));
	session->client = client;
	return session;
}

void free_session(session_t* session)
{
	if (session->client)
		lw_stream_close(session->client, lw_true);
	free(session);
}

void session_set_attachment(session_t* session, void* attachment)
{
	session->attachment = attachment;
}

void* session_get_attachment(session_t* session)
{
	return session->attachment;
}

void session_write(session_t* session, buffer_t* buffer)
{
	// fixes a compiler warning.. TODO: a better fix for this
	char fix[buffer->size];
	memcpy(fix, buffer->payload, buffer->size);
	lw_stream_write(session->client, fix, buffer->size);
}