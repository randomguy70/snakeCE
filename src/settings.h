#ifndef SETTINGS_H
#define SETTINGS_H

#include <stdbool.h>

#include "main.h"

struct settings {
	uint8_t show_score;    // whether to show the current score in-game
	uint8_t auto_save;     // whether to save state into appvar before exiting
	uint8_t auto_restore;  // whether to auto-start games from saved state appvar
	uint8_t size;       // should be a clean factor of 320
	uint8_t speed;      // relies on BOTH size & delay time
};

#endif