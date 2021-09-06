#ifndef SETTINGS_H
#define SETTINGS_H

#include <stdbool.h>

#include "main.h"

struct settings {
	uint8_t show_score;    // whether to show the current score in-game
	uint8_t size;          // should be a clean factor of 320
	uint8_t speed;         // relies on BOTH size & delay time
};

#endif