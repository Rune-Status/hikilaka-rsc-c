#ifndef RSC_P_HANDLERS_H
#define RSC_P_HANDLERS_H

#pragma once

#include "../session.h"
#include "packet.h"

typedef int(*p_handler_t)(session_t*, packet_t*);

/**
 * Registers a packet handler. Returns 1 on success or
 * 0 if a packet handler already exists for the requested id
 */
int packet_handler_register(uint8_t id, p_handler_t handler);

/**
 * Calls the associated packet handler for the passed packet.
 * Returns 1 on success and 0 if an error occurred.
 */
int packet_handler_handle(session_t* session, packet_t* packet);

#endif