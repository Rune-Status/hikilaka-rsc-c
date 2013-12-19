#include "packet.h"

#include <string.h>
#include "../memory.h"

packet_t* packet_decode(buffer_t* buffer)
{
	int error = buffer_ok;
	uint16_t length;
	uint8_t id;
	error |= buffer_get_short(buffer, &length);
	error |= buffer_get_byte(buffer, &id);
	error |= buffer_available(buffer) == length;

	if (error || length > 5000)
		return NULL;

	uint8_t payload[length];
	memcpy(payload, buffer->payload + buffer->position, length);

	packet_t* packet = safe_alloc(sizeof(packet_t));
	packet->id = id;
	packet->buffer = buffer_wrap(payload, length);
	return packet;
}

void free_packet(packet_t* packet)
{
	free_buffer(packet->buffer);
	free(packet);
}