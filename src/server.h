#ifndef SERVER_H
#define SERVER_H

#pragma once

#include <stddef.h>
#include "hashmap.h"
#include "game.h"

game_conf_t* config;

hashmap_t* connections;

#endif