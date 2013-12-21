#include "server.h"

#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <unistd.h>
#include <fcntl.h>
#include <ev.h>
#include "buffer.h"
#include "memory.h"
#include "session.h"

void handle_read(struct ev_loop* loop, struct ev_io* watcher, int flags)
{
	char data[config->buffer_size];
	ssize_t read;

	if (EV_ERROR & flags)
	{
		perror("Invalid event in game core");
		return;
	}

	read = recv(watcher->fd, data, config->buffer_size, 0);

	if (read < 0)
	{
		perror("Read error");
		// TODO: close the connection?
		return;
	}

	session_t* session;
	hashmap_get(connections, watcher->fd, (void*) &session);

	if (read == 0)
	{
		// connection closed
		session_disconnect(session);
	}
	else
	{
		// data received
		buffer_t* buffer = buffer_wrap((uint8_t*) data, read);
		handle_game_data(session, buffer);
		free_buffer(buffer);
	}
}

void handle_accept(struct ev_loop* loop, struct ev_io* watcher, int flags)
{
	static socklen_t address_size = sizeof(struct sockaddr_in);
	struct sockaddr_in address;

	if (EV_ERROR & flags)
	{
		perror("Invalid event in game core");
		return;
	}

	int descriptor = accept(watcher->fd, (struct sockaddr*) &address, &address_size);

	if (descriptor < 0)
	{
		perror("Accept error in game core");
		return;
	}

	int dflags = fcntl(descriptor, F_GETFL, 0);
	fcntl(descriptor, F_SETFL, dflags  | O_NONBLOCK);

	struct ev_io* client = safe_alloc(sizeof(struct ev_io));
	ev_io_init(client, handle_read, descriptor, EV_READ);
	ev_io_start(loop, client);

	session_t* session = new_session(loop, client);
	int status = hashmap_put(connections, client->fd, session);

	if (status != MAP_OK)
	{
		printf("Error in game core: Couldn't write session to map\n");
		free_session(session);
		return;
	}

	handle_game_connection(session);
}

int main(int argc, char* argv[])
{
	connections = new_hashmap();
	config = load_game_configuration();
	printf("Loaded configuration for %s!\n", config->name);
	printf("Starting game server on port %u\n", config->port);
	printf("Client buffer size: %lu\n", config->buffer_size);

	intialize_game_module();

	struct ev_loop* loop = ev_default_loop(EVFLAG_AUTO);
	struct ev_io accept_watcher;
	struct sockaddr_in address;
	size_t address_size = sizeof(struct sockaddr_in);
	int descriptor;

	if ((descriptor = socket(PF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("Socket error");
		return 1;
	}

    memset(&address, 0, address_size);
	address.sin_family = AF_INET;
	address.sin_port = htons(config->port);
	address.sin_addr.s_addr = INADDR_ANY;

	setsockopt(descriptor, SOL_SOCKET, SO_REUSEADDR, &address, address_size);
	setsockopt(descriptor, IPPROTO_TCP, TCP_NODELAY, &address, address_size);

	int flags = fcntl(descriptor, F_GETFL, 0);
	fcntl(descriptor, F_SETFL, flags | O_NONBLOCK);

	if (bind(descriptor, (struct sockaddr*) &address, address_size) != 0)
	{
		perror("Bind error");
		return 1;
	}

	if (listen(descriptor, 2) < 0)
	{
		perror("Listen error");
		return 1;
	}

	ev_io_init(&accept_watcher, handle_accept, descriptor, EV_READ);
	ev_io_start(loop, &accept_watcher);

	printf("Awaiting connections..\n");

	ev_loop(loop, 0);
	return 0;
}