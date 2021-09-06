#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <tice.h>

#define XMIN             0
#define XMAX             LCD_WIDTH-POINT_SIZE
#define YMIN             0
#define YMAX             LCD_HEIGHT-POINT_SIZE
#define POINT_SIZE       5

enum color {
	BLACK = 253, GREY = 254,
	START_OF_SHADES = 0, END_OF_SHADES = 252,
};

struct point {
	int x, y;
	uint8_t color;
};

void drawPoint(struct point* point);
void erasePoint(struct point* point);
uint8_t updateShade(enum color *color);
void printColoredString(char* string, int x, int y);
void thickRectangle(int x, int y, int width, int height, uint8_t thickness);

#endif