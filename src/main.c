#include <tice.h>
#include <graphx.h>
#include <keypadc.h>

#include "gfx/gfx.h"
#include <colors.h>

#define POINT_SIZE 4
#define SPEED POINT_SIZE
#define STARTING_SNAKE_LEN 20
#define STARTING_DIRECTION 4

struct point {
	int x, y;
};

typedef enum {
	DIR_RIGHT, DIR_LEFT, DIR_UP, DIR_DOWN
} direction_t;

struct snake {
	struct point points[200];
	direction_t direction;
	int length;
};

void initialiseSnake(void);
void drawSnake(void);
void moveSnake(void);
bool snakeDied(void);

void initialiseApple(void);
bool foundApple(void);

void drawPoint(struct point* point);
void handlePresses(void);

struct snake snake;
struct point apple;
	
int main(void) {
	srand(rtc_Time());
	gfx_Begin();
	gfx_SetPalette(palette, sizeof_palette, 0);
	
	initialiseSnake();
	initialiseApple();
	
	while(true) {
		kb_Scan();
		if(kb_IsDown(kb_KeyClear))
			break;
		gfx_SetDraw(gfx_buffer);
		gfx_ZeroScreen();
		drawSnake();
		drawPoint(&apple);
		gfx_SwapDraw();
		
		if(foundApple()) {
			snake.length++;
			snake.points[snake.length] = snake.points[snake.length-1];
		}
		if(snakeDied()) {
			break;
		}
		
		handlePresses();
		moveSnake();
	}
	
	gfx_End();
	return 0;
}

void initialiseSnake(void) {
	snake.length = STARTING_SNAKE_LEN;
	snake.direction = DIR_RIGHT;
	
	for(int i=0; i<STARTING_SNAKE_LEN; i++) {
		snake.points[i].x = i*POINT_SIZE;
		snake.points[i].y = 1;
	}
}

void initialiseApple(void) {
	apple.x = POINT_SIZE*randInt(1, LCD_WIDTH/POINT_SIZE-POINT_SIZE);
	apple.y = POINT_SIZE*randInt(1, LCD_HEIGHT/POINT_SIZE-POINT_SIZE);
}

void drawSnake(void) {
	gfx_SetColor(WHITE);
	for(int i=0; i<snake.length; i++) {
		drawPoint(&snake.points[i]);
	}
}

void moveSnake(void) {
	for(int i=1; i<snake.length; i++) {
		snake.points[i] = snake.points[i-1]; 
	}
	if(snake.direction == DIR_RIGHT) {
		snake.points[0].x+=SPEED;
		if(snake.points[0].x > LCD_WIDTH) {
			snake.points[0].x = 1;
		}
	}
	if(snake.direction == DIR_LEFT) {
		snake.points[0].x-=SPEED;
		if(snake.points[0].x < 1) {
			snake.points[0].x = LCD_WIDTH-POINT_SIZE;
		}
	}
	if(snake.direction == DIR_UP) {
		snake.points[0].y-=SPEED;
		if(snake.points[0].y < 1) {
			snake.points[0].y = LCD_HEIGHT-POINT_SIZE;
		}
	}
	if(snake.direction == DIR_DOWN) {
		snake.points[0].y+=SPEED;
		if(snake.points[0].y > LCD_HEIGHT) {
			snake.points[0].y = 1;
		}
	}
	
}

void drawPoint(struct point* point) {
	gfx_FillRectangle(point->x, point->y, POINT_SIZE, POINT_SIZE);
}

void handlePresses(void) {
	if(kb_IsDown(kb_KeyUp) && snake.direction!=DIR_DOWN) {
		snake.direction = DIR_UP;
	}
	if(kb_IsDown(kb_KeyDown) && snake.direction!=DIR_UP) {
		snake.direction = DIR_DOWN;
	}
	if(kb_IsDown(kb_KeyRight) && snake.direction!=DIR_LEFT) {
		snake.direction = DIR_RIGHT;
	}
	if(kb_IsDown(kb_KeyLeft) && snake.direction!=DIR_RIGHT) {
		snake.direction = DIR_LEFT;
	}
}

bool foundApple(void) {
	return (snake.points[0].x == apple.x && snake.points[0].y == apple.y);
}

bool snakeDied(void) {
	for(int i=1; i<snake.length; i++) {
		if(snake.points[0].x == snake.points[i].x && snake.points[0].y == snake.points[i].y) {
			return true;
		}
	}
	return false;
}