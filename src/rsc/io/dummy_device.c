#include "io_device.h"

#include "../../memory.h"

static io_device_t* device;

static player_login_res dummy_load_player(uint64_t user,
	char* pass, size_t pass_len, uint8_t reconnect, uint32_t version)
{
	return connect_ok;
}

io_device_t* get_io_device()
{
	if (device)
		return device;

	device = safe_alloc(sizeof(io_device_t));
	device->load_player = &dummy_load_player;
	return device;
}