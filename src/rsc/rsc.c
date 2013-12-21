#include "../game.h"

#include <stdio.h>
#include <string.h>
#include "../memory.h"
#include "../buffer.h"
#include "packet.h"
#include "p_handlers.h"

static game_conf_t* game_conf = NULL;

game_conf_t* load_game_configuration(void)
{
	if (game_conf)
		return game_conf;

	// this allocated memory will be sustained until
	// the application is closed
	// TODO: load from a config file
	game_conf_t* conf = safe_alloc(sizeof(game_conf_t));
	conf->name = safe_calloc(6, sizeof(char));
	memcpy(conf->name, "rsc-c", 5);
	conf->port = 43595;
	conf->buffer_size = 5000;
	return conf;
}

int test_handler(session_t* session, packet_t* packet)
{
	printf("Handling session request packet\n");
	int errors;
	uint32_t uuid;
	errors = packet_get_int(packet, &uuid);
	if (errors)
	{
		session_disconnect(session);
		return 1;
	}
	else
	{
		buffer_t* buffer = new_buffer(8);
		buffer_add_long(buffer, 900001);
		session_write(session, buffer);
		free_buffer(buffer);
	}
	return 0;
}

void intialize_game_module(void)
{
	// load entity descriptors,
	// start game engine, etc.
	packet_handler_register(32, &test_handler);
}

void handle_game_connection(session_t* session)
{

}

void handle_game_disconnection(session_t* session)
{

}

void handle_game_data(session_t* session, buffer_t* buffer)
{
	packet_t* packet = packet_decode(buffer);
	if (packet == NULL)
	{
		printf("Error decoding packet, disconnecting user.\n");
		session_disconnect(session);
	}
	else
	{
		printf("Received packet %u\n", packet->id);

		if (!packet_handler_handle(session, packet))
		{
			printf("No packet handler for packet id %u, disconnecting user.\n", packet->id);
			session_disconnect(session);
		}
		free_packet(packet);
	}
}

void handle_game_error(game_err error)
{
	switch (error)
	{
	case game_err_nomem:
		fprintf(stderr, "CRITICAL ERROR! NO MEMORY!\n");
		fprintf(stderr, "rsc-c is shutting down..\n");
		break;
	}
}