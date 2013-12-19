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

#endif