#ifndef ENTITIES_H
#define ENTITIES_H

#include "main.h"
#include "graphics.h"
#include "vectors.h"

#define SPEED                    POINT_SIZE
#define DELAY_TIME               20
#define STARTING_SNAKE_LEN       10
#define STARTING_DIRECTION       4

struct snake {
	struct point points[200];
	direction_t direction;
	int length;
};

void initialiseSnake(void);
void drawSnake(void);
void moveSnake(void);
bool foundApple(void);
void growSnake(void);
bool snakeDied(void);
void initialiseApple(void);

#endif