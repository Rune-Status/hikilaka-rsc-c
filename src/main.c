#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <lacewing.h>

#include "game.h"
#include "buffer.h"
#include "memory.h"

void on_connect(lw_server server, lw_server_client client)
{
	session_t* session = new_session(client);
	lw_stream_set_tag(client, session);

	handle_game_connection(session);
}

void on_disconnect(lw_server server, lw_server_client client)
{
	session_t* session = lw_stream_tag(client);

	handle_game_disconnection(session);

	session->client = NULL; // they're already disconnected
	free_session(session);
}

void on_data(lw_server server, lw_server_client client, const char* buf, size_t len)
{
	// fixes a compiler warning.. TODO: a better fix for this
	uint8_t bytes[len];
	memcpy(bytes, buf, len);

	session_t* session = lw_stream_tag(client);
	buffer_t* buffer = buffer_wrap(bytes, len);
	
	handle_game_data(session, buffer);
	free_buffer(buffer);
}

int main(int argc, char* argv[])
{
	game_conf_t* conf = load_game_configuration();
	printf("Initiated module %s!\n", conf->name);
	printf("Starting game on port %u\n", conf->port);

	lw_pump pump = lw_eventpump_new();
	lw_server server = lw_server_new(pump);

	lw_server_on_connect(server, on_connect);
	lw_server_on_disconnect(server, on_disconnect);
	lw_server_on_data(server, on_data);
	lw_server_host(server, conf->port);

	lw_eventpump_start_eventloop(pump);

	lw_server_delete(server);
	lw_pump_delete(pump);	
	return 0;
}