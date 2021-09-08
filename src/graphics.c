#include <graphx.h>
#include <string.h>

#include "graphics.h"
#include "main.h"

void drawPoint(struct point* point, uint8_t pointSize) {
	gfx_SetColor(point->color);
	gfx_FillRectangle(point->x, point->y, pointSize, pointSize);
}

uint8_t updateShade(enum color *color) {
	uint8_t prevColor = *color;
	if(++(*color) > END_OF_SHADES) {
		*color = START_OF_SHADES;
	}
	return prevColor;
}

void printColoredString(char* string, int x, int y, enum color *color) {
	gfx_SetTextXY(x, y);
	for(unsigned int i=0; i<strlen(string); i++) {
		gfx_SetTextFGColor(updateShade(color));
		gfx_PrintChar(string[i]);
	}
}

void thickRectangle(int x, int y, int width, int height, uint8_t thickness) {
	for(uint8_t i=0; i<thickness; i++) {
		gfx_Rectangle_NoClip(x+i, y+i, width-2*i, height-2*i);
	}
}
