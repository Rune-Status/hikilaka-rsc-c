#include "../game.h"

#include <stdio.h>
#include <string.h>
#include "../memory.h"
#include "../buffer.h"
#include "packet.h"
#include "p_handlers.h"
#include "io/io_device.h"

static game_conf_t* game_conf = NULL;
static io_device_t* io_device;

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

int session_req_handle(session_t* session, packet_t* packet)
{
	uint32_t uuid;
	int errors = packet_get_int(packet, &uuid);
	if (errors)
	{
		session_disconnect(session);
	}
	else
	{
		buffer_t* buffer = new_buffer(8);
		buffer_add_long(buffer, 900001);
		session_write(session, buffer);
		free_buffer(buffer);
	}
	return errors;
}

int login_req_handle(session_t* session, packet_t* packet)
{
	uint8_t reconnect, pass_len;
	uint32_t version;
	uint64_t user;

	int errors = packet_get_byte(packet, &reconnect);
	errors |= packet_get_int(packet, &version);
	errors |= packet_get_long(packet, &user);
	errors |= packet_get_byte(packet, &pass_len);

	if (errors)
	{
		session_disconnect(session);
		return errors;
	}

	char pass[pass_len];
	errors |= packet_get_str(packet, pass, pass_len);

	if (errors)
	{
		// yes, recheck for errors just incase reading
		// the password failed.
		session_disconnect(session);
	}
	else
	{
		printf("Attempted log in, hash: %llu, password: %s, reconnecting: %d, client version: %d\n", user, pass, reconnect, version);
		
		player_login_res response = io_device->load_player(user, pass, pass_len, reconnect, version);
		buffer_t* buffer = new_buffer(1);
		buffer_add_byte(buffer, response);
		session_write(session, buffer);
		free_buffer(buffer);

		if (response > reconnect_ok)
		{
			session_disconnect(session);
		}
	}
	return errors;
}

void intialize_game_module(void)
{
	// load entity descriptors,
	// start game engine, etc.
	io_device = get_io_device();
	packet_handler_register(32, &session_req_handle);
	packet_handler_register(77, &login_req_handle);
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