#include <tice.h>
#include <graphx.h>
#include <keypadc.h>

#define POINT_SIZE 4
#define SPEED POINT_SIZE
#define STARTING_SNAKE_LEN 20
#define STARTING_DIRECTION 4

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
static void initialiseSnake(struct snake* snake);
static void drawSnake(struct snake* snake);
static void updateSnake(struct snake* snake);
static void addSnakePoint(struct snake* snake);
static void initialiseApple(struct point* apple);
static void drawApple(struct point* apple);

static void fillPoint(struct point* point);
static void outlinePoint(struct point* point);
static void erasePoint(struct point* point);

bool checkForDeath(struct snake* snake);
bool checkForAppleCollision(struct snake* snake, struct point* apple);

int main(void) {
	struct point apple;
	struct snake snake;
		
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
		
		if(checkForAppleCollision(&snake, &apple)) {
			addSnakePoint(&snake);
		}
		
		updateSnake(&snake);
		
		kb_Scan();
		
		if(kb_IsDown(kb_KeyClear)) {
			break;
		}
		
		if(kb_IsDown(kb_KeyDown) && snake.direction != DIR_UP) {
			snake.direction = DIR_DOWN;
		}
		if(kb_IsDown(kb_KeyUp) && snake.direction != DIR_DOWN) {
			snake.direction = DIR_UP;
		}
		if(kb_IsDown(kb_KeyRight) && snake.direction != DIR_LEFT) {
			snake.direction = DIR_RIGHT;
		}
		if(kb_IsDown(kb_KeyLeft) && snake.direction != DIR_RIGHT) {
			snake.direction = DIR_LEFT;
		}
	}
	
	gfx_End();
	return 0;
}

static void initialiseGame(struct snake* snake, struct point* apple) {
	gfx_Begin();
	srandom(rtc_Time());
	
	initialiseApple(apple);
	initialiseSnake(snake);
	
	return;
}

static void initialiseSnake(struct snake* snake) {
	snake->length = STARTING_SNAKE_LEN;
	snake->direction = STARTING_DIRECTION;
	
	if(snake->direction == DIR_DOWN) {
		for(uint32_t i=0; i<snake->length; i++) {
			snake->points[i].x = 1+(20*POINT_SIZE);
			snake->points[i].y = 1+(i*POINT_SIZE);
		}
	}
	for(uint32_t i=0; i<snake->length; i++) {
		snake->points[i].x = LCD_WIDTH/2;
		snake->points[i].y = (i+1)*POINT_SIZE;
	}
}

// draws the full snake
static void drawSnake(struct snake* snake) {
	for(uint32_t i=0; i<snake->length; i++) {
		fillPoint(&(snake->points[i]));
	}
	return;
}

// shifts the snake's points
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

static void addSnakePoint(struct snake* snake) {
	snake->length++;
	snake->points[snake->length] = snake->points[snake->length-1];
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

static void drawApple(struct point* apple) {
	fillPoint(apple);
	outlinePoint(apple);
}

static void initialiseApple(struct point* apple) {
	apple->x = randInt(1, LCD_WIDTH/POINT_SIZE);
	apple->x *= POINT_SIZE;
	apple->y = randInt(1, LCD_HEIGHT/POINT_SIZE);
	apple->y *= POINT_SIZE;
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