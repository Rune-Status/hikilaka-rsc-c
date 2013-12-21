#include "p_handlers.h"

#include <stdio.h>

static p_handler_t packet_handlers[256];

int packet_handler_register(uint8_t id, p_handler_t handler)
{
	int exists = packet_handlers[id] != NULL;
	if (!exists)
	{
		packet_handlers[id] = handler;
		printf("Registered packet handler %u\n", id);
	}
	return exists;
}

int packet_handler_handle(session_t* session, packet_t* packet)
{
	int exists = packet_handlers[packet->id] != NULL;
	if (exists)
	{
		packet_handlers[packet->id](session, packet);
	}
	return exists;
}