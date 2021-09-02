#ifndef MAIN_H
#define MAIN_H

#include "graphics.h"
#include "entities.h"
#include "vectors.h"

/* globals */
extern struct snake snake;
extern struct point apple;
extern enum color color;

/* prototypes */
void handlePresses(void);
int menu(void);
int displaySettings();


#endif