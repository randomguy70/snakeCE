#ifndef SETTINGS_H
#define SETTINGS_H

#include <stdbool.h>

#include "main.h"

#define SLOW_SPEED   40
#define MEDIUM_SPEED 20
#define FAST_SPEED   0

#define SMALL_SIZE   4
#define MEDIUM_SIZE  6
#define LARGE_SIZE   8

struct settings {
	uint8_t show_score;    // whether to show the current score in-game
	uint8_t size;          // should be a clean factor of 320
	uint8_t delay_time;
};

#endif