#include <tice.h>
#include <graphx.h>
#include <keypadc.h>

#define SPEED 2
#define POINT_SIZE 4
#define STARTING_SNAKE_LEN 10

struct point {
	uint32_t x, y;
	uint8_t color;
};

typedef enum {
	DIR_RIGHT, DIR_LEFT, DIR_UP, DIR_DOWN
} direction_t;

struct snake {
	struct point points[200];
	direction_t direction;
	uint32_t length;
};

static void initialiseGame(struct snake* snake, struct point* apple);
static void drawSnake(struct snake* snake);
static void updateSnake(struct snake* snake);
static void fillPoint(struct point* point);
static void outlinePoint(struct point* point);
static void erasePoint(struct point* point);
static void drawApple(struct point* point);
bool checkForDeath(struct snake* snake);
bool checkForAppleCollision(struct snake* snake, struct point* apple);

int main(void) {
	struct point apple;
	struct snake snake;
	kb_key_t key;
	
	initialiseGame(&snake, &apple);
	gfx_SetDraw(gfx_buffer);
	gfx_ZeroScreen();
	drawSnake(&snake);
	drawApple(&apple);
	
	while(true)
	{
		gfx_SetDraw(gfx_buffer);
		gfx_ZeroScreen();
		drawApple(&apple);
		drawSnake(&snake);
		gfx_SwapDraw();
		
		updateSnake(&snake);
		
		kb_Scan();
		key = kb_Data[7];
		
		if(kb_IsDown(kb_KeyClear))
			break;
		
		/*
		if (key) {
			switch (key) {
				case kb_Down:
						snake.direction = DIR_DOWN;
 						break;
				
				case kb_Right:
					snake.direction = DIR_RIGHT;
					break;
				
				case kb_Up:
					snake.direction = DIR_UP;
					break;
				
				case kb_Left:
					snake.direction = DIR_LEFT;
					break;
				
				default:
					break;
			}
		}
			*/
		if(kb_IsDown(kb_KeyDown)) {
			snake.direction = DIR_DOWN;
		}
		if(kb_IsDown(kb_KeyUp)) {
			snake.direction = DIR_UP;
		}
		if(kb_IsDown(kb_KeyRight)) {
			snake.direction = DIR_RIGHT;
		}
		if(kb_IsDown(kb_KeyLeft)) {
			snake.direction = DIR_LEFT;
		}
	}
	
	gfx_End();
	return 0;
}

static void initialiseGame(struct snake* snake, struct point* apple) {
	gfx_Begin();
	srandom(rtc_Time());
	
	apple->x = randInt(1, LCD_WIDTH-POINT_SIZE);
	apple->y = randInt(1, LCD_HEIGHT-POINT_SIZE);
	
	snake->length = STARTING_SNAKE_LEN;
	snake->direction = DIR_DOWN;
	
	for(uint32_t i=0; i<snake->length; i++) {
		snake->points[i].x = LCD_WIDTH/2;
		snake->points[i].y = (i*POINT_SIZE)+50;
		snake->points[i].color = 255;
	}
	
	return;
}

// draws the full snake point by point
static void drawSnake(struct snake* snake) {
	for(uint32_t i=0; i<snake->length; i++) {
		fillPoint(&(snake->points[i]));
	}
	return;
}

// shifts the points
static void updateSnake(struct snake* snake) {
	uint32_t i;
	
	switch(snake->direction) {
		case DIR_RIGHT:
			for(i=0; i<snake->length-1; i++) {
				snake->points[i+1] = snake->points[i];
			}
			if(snake->points[0].x + SPEED >= LCD_WIDTH) {
				snake->points[0].x = 1;
				break;
			}
			snake->points[0].x += SPEED;
			break;
			
		case DIR_LEFT:
			for(i=0; i<snake->length-1; i++) {
				snake->points[i+1] = snake->points[i];
			}
			snake->points[0].x -= SPEED;
			break;
			
		case DIR_UP:
			for(i=0; i<snake->length-1; i++) {
				snake->points[i+1] = snake->points[i];
			}
			snake->points[0].y -= SPEED;
			break;
			
		case DIR_DOWN:
			for(i=0; i<snake->length-1; i++) {
				snake->points[i+1] = snake->points[i];
			}
			snake->points[0].y += SPEED;
			break;
	}
	
	return;
}

bool checkForDeath(struct snake* snake) {
	for(uint32_t i=1; i<snake->length; i++) {
		if(snake->points[0].x == snake->points[i].x && snake->points[0].y == snake->points[i].y)
		{
			return true;
		}
	}
	return false;
}

bool checkForAppleCollision(struct snake* snake, struct point* apple) {
	if(snake->points[0].x == apple->x && snake->points[0].y == apple->y) {
		return true;
	}
	return false;
}

static void drawApple(struct point* point) {
	fillPoint(point);
	outlinePoint(point);
}

static void fillPoint(struct point* point) {
	gfx_SetColor(point->color);
	gfx_FillRectangle_NoClip(point->x, point->y, POINT_SIZE, POINT_SIZE);
	return;
}

static void outlinePoint(struct point* point) {
	gfx_SetColor(255);
	gfx_Rectangle_NoClip(point->x, point->y, POINT_SIZE, POINT_SIZE);
	return;
}

static void erasePoint(struct point* point) {
	gfx_SetColor(0);
	gfx_FillRectangle_NoClip(point->x, point->y, POINT_SIZE, POINT_SIZE);
	return;
}