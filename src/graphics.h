#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <tice.h>

#define XMIN             0
#define XMAX             LCD_WIDTH
#define YMIN             0
#define YMAX             LCD_HEIGHT

enum color {
	BLACK = 253, GREY = 254,
	START_OF_SHADES = 0, END_OF_SHADES = 252,
};

struct point {
	int x, y;
	uint8_t color;
};

void drawPoint(struct point* point, uint8_t pointSize);
uint8_t updateShade(enum color *color);
void printColoredString(char* string, int x, int y, enum color *color);
void thickRectangle(int x, int y, int width, int height, uint8_t thickness);

#endif