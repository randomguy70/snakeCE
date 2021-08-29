#include <tice.h>
#include <graphx.h>
#include <keypadc.h>

#include "gfx/gfx.h"

#define POINT_SIZE 5
#define SPEED POINT_SIZE
#define DELAY_TIME 20
#define STARTING_SNAKE_LEN 20
#define STARTING_DIRECTION 4
#define XMIN 0
#define XMAX LCD_WIDTH-POINT_SIZE
#define YMIN 0
#define YMAX LCD_HEIGHT-POINT_SIZE

#define SAVE_APPVAR "SNKSAVE"
#define SCORE_APPVAR "SNKSCORE"

enum color {
	BLACK = 0, GREY = 1,
	START_OF_SHADES = 2, END_OF_SHADES = 253,
};

struct point {
	int x, y;
	uint8_t color;
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
void growSnake(void);
bool snakeDied(void);

void initialiseApple(void);
bool foundApple(void);

void drawPoint(struct point* point);
void handlePresses(void);

uint8_t updateShade(void);

struct snake snake;
struct point apple;
enum color color = START_OF_SHADES;

int main(void) {
	srand(rtc_Time());
	gfx_Begin();
	gfx_SetPalette(palette, sizeof_palette, myimages_palette_offset);
	
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
			growSnake();
			initialiseApple();
		}
		if(snakeDied()) {
			break;
		}
		
		handlePresses();
		moveSnake();
		delay(DELAY_TIME);
	}
	
	gfx_End();
	return 0;
}

void initialiseSnake(void) {
	snake.length = STARTING_SNAKE_LEN;
	snake.direction = DIR_RIGHT;
	
	for(int i=0; i<STARTING_SNAKE_LEN; i++) {
		snake.points[i].x = (STARTING_SNAKE_LEN*POINT_SIZE) - (i*POINT_SIZE);
		snake.points[i].y = YMIN;
		snake.points[i].color = updateShade();
	}
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

void growSnake(void) {
	snake.length++;
	snake.points[snake.length-1] = snake.points[snake.length-2];
}

void drawPoint(struct point* point) {
	gfx_SetColor(point->color);
	gfx_FillRectangle(point->x, point->y, POINT_SIZE, POINT_SIZE);
}

void erasePoint(struct point* point) {
	gfx_SetColor(BLACK);
	gfx_FillRectangle_NoClip(point->x, point->y, POINT_SIZE, POINT_SIZE);
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

void menu(void) {
	int width = 100;
	int height = 70;
	const char* header = "You died, unfortunately.";
	const char* choiceOne = "Again! - Enter";
	const char* choiceTwo = "Quit - Clear";
	
	gfx_SetDraw(gfx_buffer);
	
	// body
	gfx_SetColor(BLACK);
	gfx_FillRectangle_NoClip(LCD_WIDTH/2-width/2, LCD_HEIGHT/2-height/2, width, height);
	// outline
	gfx_SetColor(GREY);
	gfx_Rectangle_NoClip(LCD_WIDTH/2-width/2, LCD_HEIGHT/2-height/2, width, height);
	
	// text
	gfx_PrintStringXY(header, LCD_WIDTH/2-gfx_GetStringWidth(header)/2, LCD_HEIGHT/2-height/2 + 2);
	gfx_PrintStringXY(choiceOne, LCD_WIDTH/2-gfx_GetStringWidth(choiceOne)/2, LCD_HEIGHT/2-height/2 + 2+20);
	gfx_PrintStringXY(choiceTwo, LCD_WIDTH/2-gfx_GetStringWidth(choiceTwo)/2, LCD_HEIGHT/2-height/2 + 2+40);
	gfx_SwapDraw();
	
	while(true) {
		kb_Scan();
	}
}

uint8_t updateShade(void) {
	uint8_t prevColor = color;
	if(++color > END_OF_SHADES)
		color = START_OF_SHADES;
	return prevColor;
}
