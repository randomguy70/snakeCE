#ifndef MAIN_H
#define MAIN_H

#include "graphics.h"
#include "entities.h"
#include "vectors.h"

// globals
struct snake snake;
struct point apple;
enum color color = START_OF_SHADES;

// prototypes
void handlePresses(void);
int menu(void);
int displaySettings();


#endif