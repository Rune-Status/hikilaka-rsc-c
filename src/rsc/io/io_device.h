#ifndef RSC_IO_IO_DEVICE_H
#define RSC_IO_IO_DEVICE_H

#pragma once

#include <stdint.h>
#include <stddef.h>

typedef enum
{
	connect_ok,
	reconnect_ok,
	invalid_credentials,
	already_online,
	client_updated,
	login_error,
	account_banned
} player_login_res;

typedef struct
{
	player_login_res (*load_player)(uint64_t user,
		char* pass, size_t pass_len, uint8_t reconnect, uint32_t version);
} io_device_t;

/**
 * Obtains the global IO device reference.
 * An IO device is responsible for loading
 * and saving materials.
 */
io_device_t* get_io_device(void);

#endif