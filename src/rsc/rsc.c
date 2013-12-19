#include "../game.h"

#include <stdio.h>
#include <string.h>
#include "../memory.h"
#include "../buffer.h"
#include "packet.h"

static game_conf_t* game_conf = NULL;

game_conf_t* load_game_configuration()
{
	if (game_conf)
		return game_conf;

	// this allocated memory will be sustained until
	// the application is closed
	game_conf_t* conf = safe_alloc(sizeof(game_conf_t));
	conf->name = safe_calloc(6, sizeof(char));
	memcpy(conf->name, "rsc-c", 5);
	conf->port = 43595;
	return conf;
}

void intialize_game_module()
{
	// load entity descriptors,
	// start game engine, etc.
}

void handle_game_connection(session_t* session)
{
	printf("user connected\n");
}

void handle_game_disconnection(session_t* session)
{
	printf("user disconnected\n");
}

void handle_game_data(session_t* session, buffer_t* buffer)
{
	packet_t* packet = packet_decode(buffer);
	if (packet == NULL)
	{
		// disconnect the user
		printf("Error decoding packet, disconnecting user.\n");
		free_session(session);
		return;
	}
	else
	{
		printf("Received packet %u\n", packet->id);
	}
	free_packet(packet);
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