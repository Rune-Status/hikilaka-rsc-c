#ifndef RSC_PACKET_H
#define RSC_PACKET_H

#pragma once

#include <stdint.h>
#include "../buffer.h"

typedef struct
{
	uint8_t id;
	buffer_t* buffer;
} packet_t;

/**
 * Creates a packet from a raw buffer
 * object. Returns null if an error
 * occured while attempted to decode the packet.
 */
packet_t* packet_decode(buffer_t* buffer);

/**
 * Deallocates a packet
 */
void free_packet(packet_t* packet);

/**
 * Packet helper macros
 */
#define packet_available(packet) buffer_available(packet->buffer)

#define packet_add_byte(packet, b) buffer_add_byte(packet->buffer, b)

#define packet_add_short(packet, s) buffer_add_short(packet->buffer, s)

#define packet_add_int(packet, i) buffer_add_int(packet->buffer, i)

#define packet_add_long(packet, l) buffer_add_long(packet->buffer, l)

#define packet_add_str(packet, str) buffer_add_str(packet->buffer, str)

#define packet_add_bits(packet, value, amount) buffer_add_bits(packet->buffer, value, amount)

#define packet_get_byte(packet, b) buffer_get_byte(packet->buffer, b)

#define packet_get_short(packet, s) buffer_get_short(packet->buffer, s)

#define packet_get_int(packet, i) buffer_get_int(packet->buffer, i)

#define packet_get_long(packet, l) buffer_get_long(packet->buffer, l)

#define packet_get_str(packet, str, len) buffer_get_str(packet->buffer, str, len)

#define packet_get_bits(packet, i, amount) buffer_get_bits(packet->buffer, i, amount)

#endif