#ifndef ENTITIES_H
#define ENTITIES_H

#include "main.h"
#include "graphics.h"
#include "vectors.h"

#define STARTING_SNAKE_LEN   20
#define STARTING_DIRECTION   4

struct snake {
	struct point points[200];
	direction_t direction;
	uint8_t length;
};

void initialiseSnake(struct snake *snake, uint8_t pointSize, enum color *startingShade);
void drawSnake(struct snake *snake, uint8_t pointSize);
void moveSnake(struct snake *snake, uint8_t speed, enum color *color);
bool foundApple(struct point *apple, struct snake *snake);
void growSnake(struct snake *snake);
bool snakeDied(struct snake *snake);
void initialiseApple(struct point *apple, uint8_t pointSize);

#endif