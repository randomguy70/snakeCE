#ifndef MAIN_C
#define MAIN_C

#include <tice.h>
#include <graphx.h>
#include <keypadc.h>
#include <string.h>

#include "main.h"
#include "gfx/gfx.h"
#include "graphics.h"
#include "entities.h"
#include "state.h"
#include "settings.h"

/* define globals (declared in main.h). And yes, this is horrible code, but it's snake, and it's my first time making snake, so give me a break! :P */
struct snake    snake;
struct point    apple;
enum   color    color;
struct settings settings;

/* prototypes */
void handlePresses(void);
int menu(void);
int displaySettings();

int main(void) {
	ti_CloseAll();
	srand(rtc_Time());
	gfx_Begin();
	gfx_SetPalette(palette, sizeof_palette, myimages_palette_offset);
	color = START_OF_SHADES;
	
	initialiseSnake();
	initialiseApple();
	
	while(true) {
		
		kb_Scan();
		if(kb_IsDown(kb_KeyClear))
			break;
		
		gfx_SetDraw(gfx_buffer);
		gfx_FillScreen(BLACK);
		drawSnake();
		drawPoint(&apple);
		gfx_SwapDraw();
		
		if(foundApple()) {
			growSnake();
			initialiseApple();
		}
		if(snakeDied()) {
			if(menu() == 0) {
				saveState(&settings, &snake, &apple, &color);
				gfx_End();
				return 0;
			}
			initialiseSnake();
			initialiseApple();
		}
		
		handlePresses();
		moveSnake();
		delay(DELAY_TIME);
	}
	
	saveState(&settings, &snake, &apple, &color);
	gfx_End();
	return 0;
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

int menu(void) {
	const int width = 174;
	const int height = 100;
	const int x = LCD_WIDTH/2-width/2;
	const int y = LCD_HEIGHT/2-height/2;
	char *txt[] = {"You died, unfortunately.", "Again! - Enter", "Quit - Clear", "Mode - Settings"};
	const uint8_t numTxtStrings = 4;
	const uint8_t lineSpacing = 20;
	
	gfx_SetDraw(gfx_buffer);
	
	gfx_SetColor(BLACK); gfx_FillRectangle_NoClip(x, y, width, height);
	gfx_SetColor(GREY);  thickRectangle(x, y, width, height, 3);
	
	for(uint8_t i=0; i<numTxtStrings; i++) {
		int strX = LCD_WIDTH/2 - gfx_GetStringWidth(txt[i])/2;
		int strY = y + 5 + i*lineSpacing;
		printColoredString(txt[i], strX, strY);
	}
	
	gfx_SwapDraw();
	
	while(true) {
		kb_Scan();
		if(kb_IsDown(kb_KeyClear)) {
			return 0;
		}
		if(kb_IsDown(kb_KeyEnter) || kb_IsDown(kb_Key2nd)) {
			initialiseSnake();
			initialiseApple();
			return 1;
		}
	}
}

int displaySettings(void) {
	
}

#endif