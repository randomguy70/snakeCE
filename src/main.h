#ifndef MAIN_H
#define MAIN_H

#include "graphics.h"
#include "entities.h"
#include "vectors.h"

/* NOTE! */
/**
 * The save appvar for this program is formatted thus:
 * Name: SNKSAVE
 * -------------------------------|
 * Byte offset | Function         |
 * -------------------------------|
 * 0-1         | Score            |
 * 2-3         | Score checker    |
 * 4-6         | Settings Data    |
 * -------------------------------|
**/

/* globals */
extern struct snake snake;
extern struct point apple;
extern enum color color;

#endif