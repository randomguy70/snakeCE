#include <tice.h>
#include <graphx.h>
#include <keypadc.h>
#include <string.h>
#include <fileioc.h>

#include "main.h"
#include "gfx/gfx.h"
#include "graphics.h"
#include "entities.h"
#include "settings.h"
#include "save.h"

void handlePresses(struct snake *snake);
int menu(uint16_t score);
int displaySettings(struct settings *settings);
void displayScore(uint16_t score);

int main(void) {
	handleSaveFile();
	
	struct settings settings;
	struct snake    snake;
	struct point    apple;
	enum   color    color;
	
	uint16_t score = 0;
	
	srand(rtc_Time());
	
	gfx_Begin();
	gfx_SetPalette(palette, sizeof_palette, myimages_palette_offset);
	color = START_OF_SHADES;
	
	settings.show_score = true;
	settings.size = 4;
	settings.delay_time = 5;
	
	initialiseSnake(&snake, settings.size, &color);
	initialiseApple(&apple, settings.size);
	
	while(true) {
		score = snake.length-STARTING_SNAKE_LEN;
		
		kb_Scan();
		if(kb_IsDown(kb_KeyClear))
			break;
		
		gfx_SetDraw(gfx_buffer);
		gfx_FillScreen(BLACK);
		drawSnake(&snake, settings.size);
		drawPoint(&apple, settings.size);
		if(settings.show_score) {
			displayScore(score);
		}
		gfx_SwapDraw();
		
		if(foundApple(&apple, &snake)) {
			growSnake(&snake);
			initialiseApple(&apple, settings.size);
		}
		
		if(snakeDied(&snake)) {
			if(menu(snake.length-STARTING_SNAKE_LEN) == 0) {
				break;
			}
			initialiseSnake(&snake, settings.size, &color);
			initialiseApple(&apple, settings.size);
		}
		
		handlePresses(&snake);
		moveSnake(&snake, settings.size, &color);
		delay(1); /** ERROR HERE, apparently **/
	}
	
	// if(getHighScore() < score) {
		// writeHighScore(score);
	// }
	gfx_End();
	return 0;
}

void handlePresses(struct snake *snake) {
	if(kb_IsDown(kb_KeyUp) && snake->direction!=DIR_DOWN) {
		snake->direction = DIR_UP;
		if(kb_IsDown(kb_KeyRight) || kb_IsDown(kb_KeyLeft))
			return;
	}
	if(kb_IsDown(kb_KeyDown) && snake->direction!=DIR_UP) {
		snake->direction = DIR_DOWN;
		if(kb_IsDown(kb_KeyRight) || kb_IsDown(kb_KeyLeft))
			return;
	}
	if(kb_IsDown(kb_KeyRight) && snake->direction!=DIR_LEFT) {
		snake->direction = DIR_RIGHT;
	}
	if(kb_IsDown(kb_KeyLeft) && snake->direction!=DIR_RIGHT) {
		snake->direction = DIR_LEFT;
	}
}

int menu(uint16_t score) {
	const int width = 174;
	const int height = 110;
	const int x = LCD_WIDTH/2-width/2;
	const int y = LCD_HEIGHT/2-height/2;
	char *txt[] = {"You died, unfortunately.", "Again! - Enter", "Quit - Clear", "Mode - Settings"};
	const uint8_t numTxtStrings = 4;
	const uint8_t lineSpacing = 20;
	enum color color = randInt(START_OF_SHADES, END_OF_SHADES);
	
	gfx_SetDraw(gfx_buffer);
	
	gfx_SetColor(BLACK); gfx_FillRectangle_NoClip(x, y, width, height);
	gfx_SetColor(GREY);  thickRectangle(x, y, width, height, 3);
	
	printColoredString("Score:", x+10, y+10, &color);
	gfx_PrintUInt((unsigned int) score, 2);
	printColoredString("High:", (x + width) - (30+ gfx_GetStringWidth("High:")), y+10, &color);
	gfx_PrintUInt((unsigned int) getHighScore(), 2);
	
	for(uint8_t i=0; i<numTxtStrings; i++) {
		int strX = LCD_WIDTH/2 - gfx_GetStringWidth(txt[i])/2;
		int strY = y + 30 + i*lineSpacing;
		printColoredString(txt[i], strX, strY, &color);
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
	/* XXX */
	return settings->size;	
}

void displayScore(uint16_t score) {
	gfx_SetTextFGColor(10);
	gfx_SetTextXY(2, 2);
	gfx_PrintUInt((unsigned int) score, 2);
}
