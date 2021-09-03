#ifndef SETTINGS_H
#define SETTINGS_H

#include <stdbool.h>

#include "main.h"

struct settings {
	bool show_score;    // whether to show the current score in-game
	bool auto_save;     // whether to save state into appvar before exiting
	bool auto_restore;  // whether to auto-start games from saved state appvar
	uint8_t size;       // should be a clean factor of 320
	uint8_t speed;      // relies on BOTH size & delay time
};

#endif