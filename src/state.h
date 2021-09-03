#ifndef STATE_H
#define STATE_H

#include "entities.h"
#include "graphics.h"

#define SAVE_APPVAR   "SNKSAVE"
#define SCORE_APPVAR  "SNKSCORE"

int saveState(struct settings *settings, struct snake *snake, struct point *apple, enum color *color);

int loadState(struct settings *settings, struct snake *snake, struct point *apple, enum color *color);

#endif