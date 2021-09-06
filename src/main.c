#include <tice.h>
#include <graphx.h>
#include <keypadc.h>
#include <string.h>

#include "main.h"
#include "gfx/gfx.h"
#include "graphics.h"
#include "entities.h"
#include "settings.h"
#include "save.h"

void handlePresses(void);
int menu(void);
int displaySettings(struct settings *settings);

int main(void) {
	
	struct settings settings;
	struct snake    snake;
	struct point    apple;
	enum   color    color;

	ti_CloseAll();
	srand(rtc_Time());
	
	gfx_Begin();
	gfx_SetPalette(palette, sizeof_palette, myimages_palette_offset);
	color = START_OF_SHADES;
	
	if(!checkSaveFileAuthenticity()) {
		resetSaveFile();
	}
	loadSettings(&settings);
	
	initialiseSnake(&snake, settings.size, color);
	initialiseApple(&apple);
	
	while(true) {
		
		kb_Scan();
		if(kb_IsDown(kb_KeyClear))
			break;
		
		gfx_SetDraw(gfx_buffer);
		gfx_FillScreen(BLACK);
		drawSnake(&snake);
		drawPoint(&apple);
		gfx_SwapDraw();
		
		if(foundApple(&apple, &snake)) {
			growSnake(&snake);
			initialiseApple(&apple);
		}
		
		if(snakeDied(&snake)) {
			if(menu() == 0) {
				saveState(&settings, &snake.length);
				gfx_End();
				return 0;
			}
			initialiseSnake(&snake, &settings.size, &color);
			initialiseApple(&apple);
		}
		
		handlePresses();
		moveSnake(&snake, settings.size, &color);
		delay(settings.speed);
	}
	
	saveState(&settings, &snake.length);
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
		printColoredString(txt[i], strX, strY, randInt(START_OF_SHADES, END_OF_SHADES));
	}
	
	gfx_SwapDraw();
	
	while(true) {
		kb_Scan();
		if(kb_IsDown(kb_KeyClear)) {
			return 0;
		}
		if(kb_IsDown(kb_KeyEnter) || kb_IsDown(kb_Key2nd)) {
			return 1;
		}
	}
}

int displaySettings(struct settings *settings) {
	
}
