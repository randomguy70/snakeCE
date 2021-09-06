#ifndef ENTITIES_H
#define ENTITIES_H

#include "main.h"
#include "graphics.h"
#include "vectors.h"

#define STARTING_SNAKE_LEN   10
#define STARTING_DIRECTION   4

struct snake {
	struct point points[200];
	direction_t direction;
	uint8_t length;
};

void initialiseSnake(struct snake *snake);
void drawSnake(struct snake *snake);
void moveSnake(struct snake *snake);
bool foundApple(struct point *apple, struct snake *snake);
void growSnake(struct snake *snake);
bool snakeDied(struct snake *snake);
void initialiseApple(struct point *apple);

#endif