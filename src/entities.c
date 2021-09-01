#include "main.h"
#include "entities.h"

void initialiseSnake(void) {
	snake.length = STARTING_SNAKE_LEN;
	snake.direction = DIR_RIGHT;
	
	for(int i=0; i<STARTING_SNAKE_LEN; i++) {
		snake.points[i].x = (STARTING_SNAKE_LEN*POINT_SIZE) - (i*POINT_SIZE);
		snake.points[i].y = YMIN;
		snake.points[i].color = updateShade();
	}
}

void drawSnake(void) {
	for(int i=0; i<snake.length; i++) {
		drawPoint(&snake.points[i]);
	}
}

void moveSnake(void) {
	// oh yea. inversed array looping
	// o-o-o-o-o-o-o-o-o
	// start here------^
	// --^------end here
	// ^--------redefine
	for(int i=snake.length-1; i>0; i--) {
		snake.points[i] = snake.points[i-1];
	}
	
	if(snake.direction == DIR_RIGHT) {
		snake.points[0].x+=SPEED;
		if(snake.points[0].x > XMAX) {
			snake.points[0].x = XMIN;
		}
	}
	if(snake.direction == DIR_LEFT) {
		snake.points[0].x-=SPEED;
		if(snake.points[0].x < XMIN) {
			snake.points[0].x = XMAX;
		}
	}
	if(snake.direction == DIR_UP) {
		snake.points[0].y-=SPEED;
		if(snake.points[0].y < YMIN) {
			snake.points[0].y = YMAX;
		}
	}
	if(snake.direction == DIR_DOWN) {
		snake.points[0].y+=SPEED;
		if(snake.points[0].y > YMAX) {
			snake.points[0].y = YMIN;
		}
	}
	snake.points[0].color = updateShade();
}

bool foundApple(void) {
	return (snake.points[0].x == apple.x && snake.points[0].y == apple.y);
}

void growSnake(void) {
	snake.length++;
	snake.points[snake.length-1] = snake.points[snake.length-2];
}

bool snakeDied(void) {
	for(int i=1; i<snake.length; i++) {
		if(snake.points[0].x == snake.points[i].x && snake.points[0].y == snake.points[i].y) {
			return true;
		}
	}
	return false;
}

void initialiseApple(void) {
	apple.x = POINT_SIZE*randInt(XMIN, LCD_WIDTH/POINT_SIZE-POINT_SIZE);
	apple.y = POINT_SIZE*randInt(YMIN, LCD_HEIGHT/POINT_SIZE-POINT_SIZE);
	apple.color = randInt(START_OF_SHADES, END_OF_SHADES);
	
	if(apple.x > LCD_WIDTH)
		apple.x = LCD_WIDTH-POINT_SIZE;
	if(apple.x < 0)
		apple.x = 0;
	if(apple.y > LCD_HEIGHT)
		apple.y = LCD_HEIGHT-POINT_SIZE;
	if(apple.y < 0)
		apple.y = 0;
}
