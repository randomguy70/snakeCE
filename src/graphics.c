#include <graphx.h>
#include <string.h>

#include "graphics.h"
#include "main.h"

void drawPoint(struct point* point) {
	gfx_SetColor(point->color);
	gfx_FillRectangle(point->x, point->y, POINT_SIZE, POINT_SIZE);
}

void erasePoint(struct point* point) {
	gfx_SetColor(BLACK);
	gfx_FillRectangle_NoClip(point->x, point->y, POINT_SIZE, POINT_SIZE);
}

uint8_t updateShade(void) {
	uint8_t prevColor = color;
	if(++color > END_OF_SHADES)
		color = START_OF_SHADES;
	return prevColor;
}

void printColoredString(char* string, int x, int y) {
	gfx_SetTextXY(x, y);
	for(unsigned int i=0; i<strlen(string); i++) {
		gfx_SetTextFGColor(updateShade());
		gfx_PrintChar(string[i]);
	}
}

void thickRectangle(int x, int y, int width, int height, uint8_t thickness) {
	for(uint8_t i=0; i<thickness; i++) {
		gfx_Rectangle_NoClip(x+i, y+i, width-2*i, height-2*i);
	}
}
